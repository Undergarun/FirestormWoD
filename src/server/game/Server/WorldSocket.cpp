/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <ace/Message_Block.h>
#include <ace/OS_NS_string.h>
#include <ace/OS_NS_unistd.h>
#include <ace/os_include/arpa/os_inet.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/os_include/sys/os_types.h>
#include <ace/os_include/sys/os_socket.h>
#include <ace/OS_NS_string.h>
#include <ace/Reactor.h>
#include <ace/Auto_Ptr.h>

#include "WorldSocket.h"
#include "Common.h"

#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "SharedDefines.h"
#include "ByteBuffer.h"
#include "Opcodes.h"
#include "DatabaseEnv.h"
#include "BigNumber.h"
#include "SHA1.h"
#include "WorldSession.h"
#include "WorldSocketMgr.h"
#include "Log.h"
#include "PacketLog.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "ObjectMgr.h"
#include "zlib.h"

uint32_t gReceivedBytes = 0;
uint32_t gSentBytes = 0;

std::string const WorldSocket::m_ServerConnectionInitialize("WORLD OF WARCRAFT CONNECTION - SERVER TO CLIENT");
std::string const WorldSocket::m_ClientConnectionInitialize("WORLD OF WARCRAFT CONNECTION - CLIENT TO SERVER");

uint32 const g_SizeOfClientHeader[2][2] =
{
    { 2, 0 },
    { 6, 4 }
};

uint32 const g_SizeOfServerHeader[2] = { sizeof(uint16) + sizeof(uint32), sizeof(uint32) };

WorldSocket::WorldSocket(void) : WorldHandler(),
m_LastPingTime(ACE_Time_Value::zero), m_OverSpeedPings(0), m_Session(0), m_RecvWPct(0),
m_RecvPct(), m_HeaderBuffer(g_SizeOfClientHeader[0][0]), m_PacketBuffer(4096), m_OutBuffer(0),
m_OutBufferSize(65536), m_OutActive(false), m_Seed(static_cast<uint32> (rand32())), m_Initialized(false),
m_CompressionStream(nullptr)
{
    reference_counting_policy().value(ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

    msg_queue()->high_water_mark(8 * 1024 * 1024);
    msg_queue()->low_water_mark(8 * 1024 * 1024);

}

WorldSocket::~WorldSocket (void)
{
    delete m_RecvWPct;

    if (m_OutBuffer)
        m_OutBuffer->release();

    closing_ = true;

    peer().close();

    if (m_CompressionStream)
    {
        deflateEnd(m_CompressionStream);
        delete m_CompressionStream;
    }
}

bool WorldSocket::IsClosed (void) const
{
    return closing_;
}

void WorldSocket::CloseSocket (void)
{
    {
        ACE_GUARD (LockType, Guard, m_OutBufferLock);

        if (closing_)
            return;

        closing_ = true;
        peer().close_writer();
    }

    {
        ACE_GUARD (LockType, Guard, m_SessionLock);

        m_Session = NULL;
    }
}

const std::string& WorldSocket::GetRemoteAddress (void) const
{
    return m_Address;
}

int WorldSocket::SendPacket(WorldPacket const& pct)
{
    ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

    if (closing_)
        return -1;

    // Dump outgoing packet
    if (sPacketLog->CanLogPacket())
        sPacketLog->LogPacket(pct, SERVER_TO_CLIENT);

    if (pct.GetOpcode() == 0)
        return 0;

    WorldPacket const* pkt = &pct;
    const_cast<WorldPacket*>(pkt)->FlushBits();

    gSentBytes += pkt->size() + 3;

#   ifdef WIN32
    if (sWorld->getBoolConfig(CONFIG_LOG_PACKETS))
    {
        switch (pct.GetOpcode())
        {
            case SMSG_MONSTER_MOVE:
            case SMSG_THREAT_CLEAR:
            case SMSG_THREAT_REMOVE:
            case SMSG_THREAT_UPDATE:
            case SMSG_HIGHEST_THREAT_UPDATE:
            case SMSG_ATTACKER_STATE_UPDATE:
                break;

            default:
                printf("Send packet %s\n", GetOpcodeNameForLogging(pkt->GetOpcode(), WOW_SERVER_TO_CLIENT).c_str());
        }
    }
#   endif

    sScriptMgr->OnPacketSend(this, *pkt);

    ACE_Message_Block* l_Buffer = WritePacketToBuffer(*pkt);

    if (l_Buffer != m_OutBuffer && l_Buffer != nullptr)
    {
        // Enqueue packet
        if (msg_queue()->enqueue_tail(l_Buffer, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::SendPacket enqueue_tail failed");
            l_Buffer->release();
            return -1;
        }
    }

    return 0;
}

ACE_Message_Block* WorldSocket::WritePacketToBuffer(WorldPacket const& p_Packet)
{
    uint32 l_PacketSize = p_Packet.size();
    uint32 l_SizeOfHeader = g_SizeOfServerHeader[m_Crypt.IsInitialized()];
    uint32 l_TotalSize = l_PacketSize + l_SizeOfHeader;
    uint32 l_Opcode = p_Packet.GetOpcode();

    ACE_Message_Block* l_Buffer;

    // If there is free space in the output buffer insert it instead of enqueing the packet
    if (m_OutBuffer->space() >= l_TotalSize && msg_queue()->is_empty())
        l_Buffer = m_OutBuffer;
    else
        ACE_NEW_RETURN(l_Buffer, ACE_Message_Block(l_TotalSize), nullptr);

    ServerPktHeader l_Header;
    CompressedWorldPacket l_CompressedPacket;

    char* l_HeaderPointer = l_Buffer->wr_ptr();
    l_Buffer->wr_ptr(l_SizeOfHeader);

    if (l_PacketSize > 0x400 && m_CompressionStream)
    {
        l_CompressedPacket.m_UncompressedSize = l_PacketSize + 4;
        l_CompressedPacket.m_UncompressedAdler = adler32(adler32(0x9827D8F1, (Bytef*)&l_Opcode, 4), p_Packet.contents(), l_PacketSize);

        /// Reserve space for compression info - uncompressed size and checksums
        char* l_CompressionInfo = l_Buffer->wr_ptr();
        l_Buffer->wr_ptr(sizeof(CompressedWorldPacket));

        uint32 l_CompressedSize = CompressPacket(l_Buffer->wr_ptr(), p_Packet);
        l_CompressedPacket.m_CompressedAdler = adler32(0x9827D8F1, (const Bytef*)l_Buffer->wr_ptr(), l_CompressedSize);

        l_Buffer->wr_ptr(l_CompressedSize);
        memcpy(l_CompressionInfo, &l_CompressedPacket, sizeof(CompressedWorldPacket));

        l_PacketSize = l_CompressedSize + sizeof(CompressedWorldPacket);
        l_Opcode = SMSG_COMPRESSED_PACKET;
    }
    else if (l_PacketSize)
    {
        if (l_Buffer->copy((char*)(p_Packet.contents()), l_PacketSize) == -1)
            ACE_ASSERT(false);
    }

    if (m_Crypt.IsInitialized())
    {
        l_Header.Normal.m_Size = l_PacketSize;
        l_Header.Normal.m_Command = l_Opcode;
        m_Crypt.EncryptSend((uint8*)&l_Header, l_SizeOfHeader);
    }
    else
    {
        l_Header.Setup.m_Size = l_PacketSize + 4;
        l_Header.Setup.m_Command = l_Opcode;
    }

    memcpy(l_HeaderPointer, &l_Header, l_SizeOfHeader);
    return l_Buffer;
}

uint32 WorldSocket::CompressPacket(char* p_Buffer, WorldPacket const& p_Packet)
{
    uint32 l_Opcode = p_Packet.GetOpcode();
    uint32 l_BufferSize = deflateBound(m_CompressionStream, p_Packet.size() + sizeof(l_Opcode));

    m_CompressionStream->next_out = (Bytef*)p_Buffer;
    m_CompressionStream->avail_out = l_BufferSize;
    m_CompressionStream->next_in = (Bytef*)&l_Opcode;
    m_CompressionStream->avail_in = sizeof(uint32);

    int32 z_res = deflate(m_CompressionStream, Z_BLOCK);
    if (z_res != Z_OK)
    {
        sLog->outAshran("Can't compress packet opcode (zlib: deflate) Error code: %i (%s, msg: %s)", z_res, zError(z_res), m_CompressionStream->msg);
        return 0;
    }

    m_CompressionStream->next_in = (Bytef*)p_Packet.contents();
    m_CompressionStream->avail_in = p_Packet.size();

    z_res = deflate(m_CompressionStream, Z_SYNC_FLUSH);
    if (z_res != Z_OK)
    {
        sLog->outAshran("Can't compress packet data (zlib: deflate) Error code: %i (%s, msg: %s)", z_res, zError(z_res), m_CompressionStream->msg);
        return 0;
    }

    return l_BufferSize - m_CompressionStream->avail_out;
}

long WorldSocket::AddReference (void)
{
    return static_cast<long> (add_reference());
}

long WorldSocket::RemoveReference (void)
{
    return static_cast<long> (remove_reference());
}

int WorldSocket::open (void *a)
{
    ACE_UNUSED_ARG (a);

    // Prevent double call to this func.
    if (m_OutBuffer)
        return -1;

    // This will also prevent the socket from being Updated
    // while we are initializing it.
    m_OutActive = true;

    // Hook for the manager.
    if (sWorldSocketMgr->OnSocketOpen(this) == -1)
        return -1;

    // Allocate the buffer.
    ACE_NEW_RETURN (m_OutBuffer, ACE_Message_Block (m_OutBufferSize), -1);

    // Store peer address.
    ACE_INET_Addr remote_addr;

    if (peer().get_remote_addr(remote_addr) == -1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::open: peer().get_remote_addr errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    m_Address = remote_addr.get_host_addr();

    // Send handshake string -- uint16 size at the beginning
    uint16 l_SizeOfInitString = m_ServerConnectionInitialize.size();

    if (m_OutBuffer->copy((char*)&l_SizeOfInitString, sizeof(l_SizeOfInitString)))
        ACE_ASSERT(false);

    if (m_OutBuffer->copy(m_ServerConnectionInitialize.c_str(), m_ServerConnectionInitialize.size()) == -1)
        ACE_ASSERT(false);

    // Register with ACE Reactor
    if (reactor()->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK) == -1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::open: unable to register client handler errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    // reactor takes care of the socket from now on
    remove_reference();

    return 0;
}

int WorldSocket::close (u_long)
{
    shutdown();

    closing_ = true;

    remove_reference();

    return 0;
}

int WorldSocket::handle_input (ACE_HANDLE)
{
    if (closing_)
        return -1;

    switch (HandleNewPacket())
    {
    case -1 :
        {
            if ((errno == EWOULDBLOCK) ||
                (errno == EAGAIN))
            {
                return Update();                           // interesting line, isn't it ?
            }

            sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSocket::handle_input: Peer error closing connection errno = %s", ACE_OS::strerror (errno));

            errno = ECONNRESET;
            return -1;
        }
    case 0:
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSocket::handle_input: Peer has closed connection");

            errno = ECONNRESET;
            return -1;
        }
    case 1:
        return 1;
    default:
        return Update();                               // another interesting line ;)
    }

    ACE_NOTREACHED(return -1);
}

int WorldSocket::handle_output (ACE_HANDLE)
{
    ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

    if (closing_)
        return -1;

    size_t send_len = m_OutBuffer->length();

    if (send_len == 0)
        return handle_output_queue(Guard);

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send (m_OutBuffer->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send (m_OutBuffer->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
        return -1;
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return schedule_wakeup_output (Guard);

        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        m_OutBuffer->rd_ptr (static_cast<size_t> (n));

        // move the data to the base of the buffer
        m_OutBuffer->crunch();

        return schedule_wakeup_output (Guard);
    }
    else //now n == send_len
    {
        m_OutBuffer->reset();

        return handle_output_queue (Guard);
    }

    ACE_NOTREACHED (return 0);
}

int WorldSocket::handle_output_queue (GuardType& g)
{
    if (msg_queue()->is_empty())
        return cancel_wakeup_output(g);

    ACE_Message_Block* mblk;

    if (msg_queue()->dequeue_head(mblk, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::handle_output_queue dequeue_head");
        return -1;
    }

    const size_t send_len = mblk->length();

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send (mblk->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send (mblk->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
    {
        mblk->release();

        return -1;
    }
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero);
            return schedule_wakeup_output (g);
        }

        mblk->release();
        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        mblk->rd_ptr(static_cast<size_t> (n));

        if (msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero) == -1)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::handle_output_queue enqueue_head");
            mblk->release();
            return -1;
        }

        return schedule_wakeup_output (g);
    }
    else //now n == send_len
    {
        mblk->release();

        return msg_queue()->is_empty() ? cancel_wakeup_output(g) : ACE_Event_Handler::WRITE_MASK;
    }

    ACE_NOTREACHED(return -1);
}

int WorldSocket::handle_close (ACE_HANDLE h, ACE_Reactor_Mask)
{
    // Critical section
    {
        ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

        closing_ = true;

        if (h == ACE_INVALID_HANDLE)
            peer().close_writer();
    }

    // Critical section
    {
        ACE_GUARD_RETURN (LockType, Guard, m_SessionLock, -1);

        m_Session = NULL;
    }

    reactor()->remove_handler(this, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::ALL_EVENTS_MASK);
    return 0;
}

int WorldSocket::Update (void)
{
    if (closing_)
        return -1;

    if (m_OutActive)
        return 0;

    {
        ACE_GUARD_RETURN(LockType, Guard, m_OutBufferLock, 0);
        if (m_OutBuffer->length() == 0 && msg_queue()->is_empty())
            return 0;
    }

    int ret;
    do
    ret = handle_output(get_handle());
    while (ret > 0);

    return ret;
}

int WorldSocket::ReadPacketHeader()
{
    ACE_ASSERT(m_RecvWPct == nullptr && m_HeaderBuffer.size() == g_SizeOfClientHeader[m_Initialized][m_Crypt.IsInitialized()]);
    m_Crypt.DecryptRecv(reinterpret_cast<uint8*>(m_HeaderBuffer.rd_ptr()), m_HeaderBuffer.size());

    uint32 l_Opcode;
    uint32 l_Size;

    ExtractOpcodeAndSize(reinterpret_cast<ClientPktHeader*>(m_HeaderBuffer.rd_ptr()), l_Opcode, l_Size);

#ifdef WIN32

    if (m_Initialized &&sWorld->getBoolConfig(CONFIG_LOG_PACKETS))
    {
        std::string l_OopcodeName = GetOpcodeNameForLogging((Opcodes)l_Opcode, WOW_CLIENT_TO_SERVER);
        printf("Receive opcode %s 0x%08.8X size : %u \n", l_OopcodeName.c_str(), l_Opcode, l_Size);
    }
#endif

    if (!ClientPktHeader::IsValidSize(l_Size) || (m_Initialized && !ClientPktHeader::IsValidOpcode(l_Opcode)))
    {
        Player* _player = m_Session ? m_Session->GetPlayer() : NULL;
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::ReadPacketHeader(): client (account: %u, char [GUID: %u, name: %s]) sent malformed packet (size: %d, cmd: %d)",
            m_Session ? m_Session->GetAccountId() : 0,
            _player ? _player->GetGUIDLow() : 0,
            _player ? _player->GetName() : "<none>",
            l_Size, l_Opcode);

        errno = EINVAL;
        return -1;
    }

    ACE_NEW_RETURN(m_RecvWPct, WorldPacket((Opcodes)l_Opcode, l_Size), -1);
    m_RecvWPct->resize(l_Size);
    m_RecvPct.base((char*)m_RecvWPct->contents(), l_Size);

    return 0;
}

void WorldSocket::ExtractOpcodeAndSize(ClientPktHeader const* p_Header, uint32& p_Opcode, uint32& p_Size) const
{
    if (m_Crypt.IsInitialized())
    {
        p_Opcode = p_Header->Normal.m_Command;
        p_Size = p_Header->Normal.m_Size;
    }
    else
    {
        p_Opcode = p_Header->Setup.m_Command;
        p_Size = p_Header->Setup.m_Size;

        // Include opcode
        if (m_Initialized)
            p_Size -= 4;
    }
}

int WorldSocket::HandlePacketData()
{
    // set errno properly here on error !!!
    // now have a header and payload

    ACE_ASSERT(m_RecvPct.space() == 0);
    ACE_ASSERT(m_HeaderBuffer.space() == 0);
    ACE_ASSERT(m_RecvWPct != nullptr);

    const int l_ReturnValue = ProcessIncoming(m_RecvWPct);

    m_RecvPct.base(nullptr, 0);
    m_RecvPct.reset();
    m_RecvWPct = nullptr;

    m_HeaderBuffer.reset();

    if (l_ReturnValue == -1)
        errno = EINVAL;

    return l_ReturnValue;
}

int WorldSocket::HandleNewPacket()
{
    m_PacketBuffer.reset();

    const size_t l_SizeOfBuffer = m_PacketBuffer.space();
    const ssize_t l_ReadSize = peer().recv(m_PacketBuffer.wr_ptr(), l_SizeOfBuffer);

    if (l_ReadSize <= 0)
        return int(l_ReadSize);

     m_PacketBuffer.wr_ptr(l_ReadSize);

    while (m_PacketBuffer.length() > 0)
    {
        // Receive header first
        if (m_HeaderBuffer.space() > 0)
        {
            const size_t l_HeaderReadSize = std::min(m_PacketBuffer.size(),  m_HeaderBuffer.space());
            m_HeaderBuffer.copy(m_PacketBuffer.rd_ptr(), l_HeaderReadSize);
            m_PacketBuffer.rd_ptr(l_HeaderReadSize);

            if (m_HeaderBuffer.space() > 0)
            {
                // Couldn't receive the whole header this time.
                errno = EWOULDBLOCK;
                return -1;
            }

            // We just received nice new header
            if (ReadPacketHeader() == -1)
                return -1;
        }

        // Its possible on some error situations that this happens
        // for example on closing when epoll receives more chunked data and stuff
        // hope this is not hack, as proper m_RecvWPct is asserted around
        if (!m_RecvWPct)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Forcing close on input m_RecvWPct = NULL");
            errno = EINVAL;
            return -1;
        }

        // We have full read header, now check the data payload
        if (m_RecvPct.space() > 0)
        {
            //need more data in the payload
            const size_t l_PacketRecvSize = std::min(m_PacketBuffer.size(), m_RecvPct.space());
            m_RecvPct.copy(m_PacketBuffer.rd_ptr(), l_PacketRecvSize);
            m_PacketBuffer.rd_ptr(l_PacketRecvSize);

            if (m_RecvPct.space() > 0)
            {
                // Couldn't receive the whole data this time.
                errno = EWOULDBLOCK;
                return -1;
            }
        }

        //just received fresh new payload
        if (HandlePacketData() == -1)
            return -1;
    }

    return size_t(l_ReadSize) == l_SizeOfBuffer ? 1 : 2;
}

int WorldSocket::cancel_wakeup_output (GuardType& g)
{
    if (!m_OutActive)
        return 0;

    m_OutActive = false;

    g.release();

    if (reactor()->cancel_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        // would be good to store errno from reactor with errno guard
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::cancel_wakeup_output");
        return -1;
    }

    return 0;
}

int WorldSocket::schedule_wakeup_output (GuardType& g)
{
    if (m_OutActive)
        return 0;

    m_OutActive = true;

    g.release();

    if (reactor()->schedule_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::schedule_wakeup_output");
        return -1;
    }

    return 0;
}

int WorldSocket::ProcessIncoming(WorldPacket* p_NewPacket)
{
    ACE_ASSERT(p_NewPacket);

    // manage memory ;)
    ACE_Auto_Ptr<WorldPacket> aptr(p_NewPacket);

    if (closing_)
        return -1;

    if (m_Initialized)
    {
        // Dump received packet.
        if (sPacketLog->CanLogPacket())
            sPacketLog->LogPacket(*p_NewPacket, CLIENT_TO_SERVER);

        try
        {
            switch (p_NewPacket->GetOpcode())
            {
                case CMSG_PING:
                    return HandlePing(*p_NewPacket);
                case CMSG_AUTH_SESSION:
                {
                    if (m_Session)
                    {
                        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::ProcessIncoming: received duplicate CMSG_AUTH_SESSION from %s", m_Session->GetPlayerName(false).c_str());
                        return -1;
                    }

                    sScriptMgr->OnPacketReceive(this, WorldPacket(*p_NewPacket));
                    return HandleAuthSession(*p_NewPacket);
                }
                case CMSG_KEEP_ALIVE:
                {
                    sLog->outDebug(LOG_FILTER_NETWORKIO, "%s", GetOpcodeNameForLogging(p_NewPacket->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str());
                    sScriptMgr->OnPacketReceive(this, WorldPacket(*p_NewPacket));
                    return 0;
                }
                case CMSG_LOG_DISCONNECT:
                {
                    p_NewPacket->rfinish(); // contains uint32 disconnectReason;
                    sScriptMgr->OnPacketReceive(this, WorldPacket(*p_NewPacket));
                    return 0;
                }
                // not an opcode, client sends string "WORLD OF WARCRAFT CONNECTION - CLIENT TO SERVER" without opcode
                // first 4 bytes become the opcode (2 dropped)
                case CMSG_ENABLE_NAGLE:
                {
                    sScriptMgr->OnPacketReceive(this, WorldPacket(*p_NewPacket));
                    return m_Session ? m_Session->HandleEnableNagleAlgorithm() : -1;
                }
                default:
                {
                    ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);
                    if (!m_Session)
                    {
                        sLog->outError(LOG_FILTER_OPCODES, "ProcessIncoming: Client not authed opcode = %u", uint32(p_NewPacket->GetOpcode()));
                        return -1;
                    }

                    // prevent invalid memory access/crash with custom opcodes
                    if (p_NewPacket->GetOpcode() >= NUM_OPCODE_HANDLERS)
                        return 0;

                    OpcodeHandler* handler = g_OpcodeTable[WOW_CLIENT_TO_SERVER][p_NewPacket->GetOpcode()];
                    if (!handler || handler->status == STATUS_UNHANDLED)
                    {
                        sLog->outError(LOG_FILTER_OPCODES, "No defined handler for opcode %s sent by %s", GetOpcodeNameForLogging(p_NewPacket->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), m_Session->GetPlayerName(false).c_str());
                        return 0;
                    }

                    // Our Idle timer will reset on any non PING opcodes.
                    // Catches people idling on the login screen and any lingering ingame connections.
                    m_Session->ResetTimeOutTime();

                    // OK, give the packet to WorldSession
                    aptr.release();
                    // WARNING here we call it with locks held.
                    // Its possible to cause deadlock if QueuePacket calls back
                    m_Session->QueuePacket(p_NewPacket);
                    return 0;
                }
            }
        }
        catch (ByteBufferException &)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::ProcessIncoming ByteBufferException occured while parsing an instant handled packet %s from client %s, accountid=%i. Disconnected client.",
                GetOpcodeNameForLogging(p_NewPacket->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), GetRemoteAddress().c_str(), m_Session ? int32(m_Session->GetAccountId()) : -1);
            p_NewPacket->hexlike();
            return -1;
        }
    }
    else
    {
        if (std::string(reinterpret_cast<char const*>(m_RecvPct.rd_ptr()), std::min(m_RecvPct.size(), m_ClientConnectionInitialize.length())) != m_ClientConnectionInitialize)
            return -1;

        m_CompressionStream = new z_stream();
        m_CompressionStream->zalloc = (alloc_func)NULL;
        m_CompressionStream->zfree = (free_func)NULL;
        m_CompressionStream->opaque = (voidpf)NULL;
        m_CompressionStream->avail_in = 0;
        m_CompressionStream->next_in = NULL;

        int32 z_res = deflateInit2(m_CompressionStream, sWorld->getIntConfig(CONFIG_COMPRESSION), Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
        if (z_res != Z_OK)
        {
            sLog->outAshran("Can't initialize packet compression (zlib: deflateInit) Error code: %i (%s)", z_res, zError(z_res));
            return false;
        }

        m_Initialized = true;
        m_HeaderBuffer.size(g_SizeOfClientHeader[1][0]);

        return HandleSendAuthSession();
    }

    ACE_NOTREACHED (return 0);
}

int WorldSocket::HandleSendAuthSession()
{
    WorldPacket packet(SMSG_AUTH_CHALLENGE, 37);
    packet << m_Seed;

    for (int i = 0; i < 8; i++)
        packet << uint32(0);

    packet << uint8(1);

    return SendPacket(packet);
}

void WorldSocket::SendAuthResponse(uint8 p_AuthResult, bool p_Queued, uint32 p_QueuePosition)
{
    WorldPacket l_Data(SMSG_AUTH_RESPONSE);

    uint32 l_RealmRaceCount = 15;
    uint32 l_RealmClassCount = 11;

    l_Data << uint8(p_AuthResult);
    l_Data.WriteBit(p_AuthResult == AUTH_OK);
    l_Data.WriteBit(p_Queued);
    l_Data.FlushBits();

    if (p_AuthResult == AUTH_OK)
    {
        l_Data << uint32(0);                    ///< VirtualRealmAddress
        l_Data << uint32(0);                    ///< VirtualRealms
        l_Data << uint32(0);                    ///< TimeRemain
        l_Data << uint32(0);                    ///< TimeOptions
        l_Data << uint32(0);                    ///< TimeRested
        l_Data << uint8(Expansion());           ///< ActiveExpansionLevel
        l_Data << uint8(Expansion());           ///< AccountExpansionLevel
        l_Data << uint32(0);                    ///< TimeSecondsUntilPCKick
        l_Data << uint32(l_RealmClassCount);    ///< AvailableRaces
        l_Data << uint32(l_RealmRaceCount);     ///< AvailableClasses
        l_Data << uint32(0);                    ///< Templates
        l_Data << uint32(0);                    ///< CurrencyID

        for (uint32 l_I = 0; l_I < l_RealmRaceCount; l_I++)
        {
            switch (l_I)
            {
                case 0:
                    l_Data << uint8(RACE_HUMAN);
                    l_Data << uint8(0);
                    break;
                case 1:
                    l_Data << uint8(RACE_ORC);
                    l_Data << uint8(0);
                    break;
                case 2:
                    l_Data << uint8(RACE_DWARF);
                    l_Data << uint8(0);
                    break;
                case 3:
                    l_Data << uint8(RACE_NIGHTELF);
                    l_Data << uint8(0);
                    break;
                case 4:
                    l_Data << uint8(RACE_UNDEAD_PLAYER);
                    l_Data << uint8(0);
                    break;
                case 5:
                    l_Data << uint8(RACE_TAUREN);
                    l_Data << uint8(0);
                    break;
                case 6:
                    l_Data << uint8(RACE_GNOME);
                    l_Data << uint8(0);
                    break;
                case 7:
                    l_Data << uint8(RACE_TROLL);
                    l_Data << uint8(0);
                    break;
                case 9:
                    l_Data << uint8(RACE_BLOODELF);
                    l_Data << uint8(1);
                    break;
                case 10:
                    l_Data << uint8(RACE_DRAENEI);
                    l_Data << uint8(1);
                    break;
                case 8:
                    l_Data << uint8(RACE_GOBLIN);
                    l_Data << uint8(3);
                    break;
                case 11:
                    l_Data << uint8(RACE_WORGEN);
                    l_Data << uint8(3);
                    break;
                case 12:
                    l_Data << uint8(RACE_PANDAREN_NEUTRAL);
                    l_Data << uint8(4);
                    break;
                case 13:
                    l_Data << uint8(RACE_PANDAREN_ALLI);
                    l_Data << uint8(4);
                    break;
                case 14:
                    l_Data << uint8(RACE_PANDAREN_HORDE);
                    l_Data << uint8(4);
                    break;
            }
        }

        for (uint32 l_I = 0; l_I < l_RealmClassCount; l_I++)
        {
            switch (l_I)
            {
                case 0:
                    l_Data << uint8(CLASS_WARRIOR);
                    l_Data << uint8(0); // Prebc
                    break;
                case 1:
                    l_Data << uint8(CLASS_PALADIN);
                    l_Data << uint8(0); // Prebc
                    break;
                case 2:
                    l_Data << uint8(CLASS_HUNTER);
                    l_Data << uint8(0); // Prebc
                    break;
                case 3:
                    l_Data << uint8(CLASS_ROGUE);
                    l_Data << uint8(0); // Prebc
                    break;
                case 4:
                    l_Data << uint8(CLASS_PRIEST);
                    l_Data << uint8(0); // Prebc
                    break;
                case 5:
                    l_Data << uint8(CLASS_DEATH_KNIGHT);
                    l_Data << uint8(2); // Wotlk
                    break;
                case 6:
                    l_Data << uint8(CLASS_SHAMAN);
                    l_Data << uint8(0); // Prebc
                    break;
                case 7:
                    l_Data << uint8(CLASS_MAGE);
                    l_Data << uint8(0); // Prebc
                    break;
                case 8:
                    l_Data << uint8(CLASS_WARLOCK);
                    l_Data << uint8(0); // Prebc
                    break;
                case 9:
                    l_Data << uint8(CLASS_DRUID);
                    l_Data << uint8(0); // Prebc
                    break;
                case 10:
                    l_Data << uint8(CLASS_MONK);
                    l_Data << uint8(4); // MoP
                    break;
            }
        }

        l_Data.WriteBit(0);                     ///< IsExpansionTrial
        l_Data.WriteBit(0);                     ///< ForceCharacterTemplate
        l_Data.WriteBit(0);                     ///< NumPlayersHorde
        l_Data.WriteBit(0);                     ///< NumPlayersAlliance
        l_Data.WriteBit(0);                     ///< IsVeteranTrial
        l_Data.FlushBits();
    }

    if (p_Queued)
    {
        l_Data << uint32(p_QueuePosition);
        l_Data.WriteBit(p_Queued);
        l_Data.FlushBits();
    }

    SendPacket(l_Data);
}

int WorldSocket::HandleAuthSession(WorldPacket& p_RecvPacket)
{
    uint64 l_DosResponse = 0;

    uint32 l_LoginServerID              = 0;
    uint32 l_RegionID                   = 0;
    uint32 l_SiteID                     = 0;
    uint32 l_RealmID                    = 0;
    uint32 l_ClientSeed                 = 0;
    uint32 l_AccountNameLenght          = 0;
    uint32 l_AddonsCompressedDataSize   = 0;

    uint16 l_ClientBuild = 0;

    uint8 l_ClientAuthChallenge[20];
    uint8 l_LoginServerType = 0;
    uint8 l_BuildType       = 0;

    bool l_UseIpV6 = false;

    std::string l_AccountName;

    WorldPacket l_AddonsCompressedData;

    //////////////////////////////////////////////////////////////////////////
    p_RecvPacket >> l_LoginServerID;                    ///< uint32
    p_RecvPacket >> l_ClientBuild;                      ///< uint16
    p_RecvPacket >> l_RegionID;                         ///< uint32
    p_RecvPacket >> l_SiteID;                           ///< uint32
    p_RecvPacket >> l_RealmID;                          ///< uint32
    p_RecvPacket >> l_LoginServerType;                  ///< uint8
    p_RecvPacket >> l_BuildType;                        ///< uint8
    p_RecvPacket >> l_ClientSeed;                       ///< uint32
    p_RecvPacket >> l_DosResponse;                      ///< uint64

    p_RecvPacket.read(l_ClientAuthChallenge, 20);

    l_AccountNameLenght = p_RecvPacket.ReadBits(11);
    l_AccountName = p_RecvPacket.ReadString(l_AccountNameLenght);
    l_UseIpV6 = p_RecvPacket.ReadBit();

    p_RecvPacket >> l_AddonsCompressedDataSize;
    l_AddonsCompressedData.resize(l_AddonsCompressedDataSize);
    p_RecvPacket.read((uint8*)l_AddonsCompressedData.contents(), l_AddonsCompressedDataSize);

    //////////////////////////////////////////////////////////////////////////

    uint32          l_AccountID             = 0;
    uint32          l_VoteRemainingTime     = 0;
    uint16          l_AccountGMLevel        = 0;
    uint8           l_AccountPremiumType    = 0;
    bool            l_AccountIsPremium      = false;
    LocaleConstant  l_AccountLocale;
    BigNumber       l_SessionKey;

    if (sWorld->IsClosed())
    {
        SendAuthResponse(AUTH_REJECT, false, 0);
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: World closed, denying client (%s).", GetRemoteAddress().c_str());
        return -1;
    }

    std::string l_AccountIDStr = l_AccountName;

    try
    {
        l_AccountID = std::stoi(l_AccountIDStr);
    }
    catch (std::exception e)
    {
        SendAuthResponse(AUTH_UNKNOWN_ACCOUNT, false, 0);
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: Sent Auth Response (unknown account).");
        return -1;
    }

    //                                                    0       1          2       3    4  5      6          7       8         9      10    11       12                           13             14
    QueryResult l_Result = LoginDatabase.PQuery ("SELECT id, sessionkey, last_ip, locked, v, s, expansion, mutetime, locale, recruiter, os, username, UNIX_TIMESTAMP(joindate), service_flags, custom_flags FROM account  WHERE id = %u", l_AccountID);

    /// Stop if the account is not found
    if (!l_Result)
    {
        SendAuthResponse(AUTH_UNKNOWN_ACCOUNT, false, 0);
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: Sent Auth Response (unknown account).");
        return -1;
    }

    Field * l_Fields = l_Result->Fetch();

    l_AccountName = l_Fields[11].GetString();
    std::string l_EscapedAccountName = l_AccountName;
    LoginDatabase.EscapeString(l_EscapedAccountName);

    uint32 l_AccountExpansion   = l_Fields[6].GetUInt8();
    uint32 l_ServerExpansion    = sWorld->getIntConfig(CONFIG_EXPANSION);
    uint32 l_JoinDateTimestamp  = l_Fields[12].GetUInt32();
    uint32 l_ServiceFlags       = l_Fields[13].GetUInt32();
    uint32 l_CustomFlags        = l_Fields[14].GetUInt32();

    if (l_AccountExpansion > l_ServerExpansion)
        l_AccountExpansion = l_ServerExpansion;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: (s,v) check s: %s v: %s", l_Fields[5].GetCString(), l_Fields[4].GetCString());

    /// Re-check ip locking (same check as in auth server).
    if (l_Fields[3].GetUInt16() == 1) // if ip is locked
    {
        if (strcmp(l_Fields[2].GetCString(), GetRemoteAddress().c_str()))
        {
            SendAuthResponse(AUTH_FAILED, false, 0);
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: Sent Auth Response (Account IP differs).");
            return -1;
        }
    }

    l_AccountID = l_Fields[0].GetUInt32();
    l_SessionKey.SetHexStr(l_Fields[1].GetCString());

    int64 l_MuteTime = l_Fields[7].GetInt64();

    /// Negative mute time indicates amount of seconds to be muted effective on next login - which is now.
    if (l_MuteTime < 0)
    {
        l_MuteTime = time(NULL) + llabs(l_MuteTime);

        PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);

        l_Stmt->setInt64(0, l_MuteTime);
        l_Stmt->setUInt32(1, l_AccountID);

        LoginDatabase.Execute(l_Stmt);
    }

    l_AccountLocale = LocaleConstant(l_Fields[8].GetUInt8());

    if (l_AccountLocale >= TOTAL_LOCALES)
        l_AccountLocale = LOCALE_enUS;

    uint32      l_Recruiter = l_Fields[9].GetUInt32();
    std::string l_AccountOS = l_Fields[10].GetString();

    /// Checks gmlevel per Realm                0        1
    l_Result = LoginDatabase.PQuery ("SELECT RealmID, gmlevel FROM account_access WHERE id = '%d' AND (RealmID = '%d' OR RealmID = '-1')", l_AccountID, g_RealmID);

    if (!l_Result)
        l_AccountGMLevel = 0;
    else
    {
        l_Fields            = l_Result->Fetch();
        l_AccountGMLevel    = l_Fields[1].GetUInt8();
    }

    /// Re-check account ban (same check as in auth server)
    QueryResult l_BanResult = LoginDatabase.PQuery ("SELECT 1 FROM account_banned WHERE id = %u AND active = 1  UNION  SELECT 1 FROM ip_banned WHERE ip = '%s'", l_AccountID, GetRemoteAddress().c_str());

    /// if account banned
    if (l_BanResult)
    {
        SendAuthResponse(AUTH_BANNED, false, 0);
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: Sent Auth Response (Account banned).");
        return -1;
    }

    /// - Vote buff
    {
        QueryResult l_VoteResult = LoginDatabase.PQuery("SELECT remainingTime FROM account_vote where account = %u", l_AccountID);
        if (l_VoteResult)
            l_VoteRemainingTime = l_VoteResult->Fetch()->GetUInt32();
    }

    /// - Premium
    {
        QueryResult l_PremiumResult = LoginDatabase.PQuery("SELECT premium_type FROM account_premium WHERE id = '%u' AND active = 1", l_AccountID);

        /// If account premium
        if (l_PremiumResult)
        {
            Field * l_PremiumFields = l_PremiumResult->Fetch();

            l_AccountIsPremium = true;
            l_AccountPremiumType = l_PremiumFields->GetUInt8();
        }
    }

    /// Check locked state for server
    AccountTypes l_AllowedAccountType = sWorld->GetPlayerSecurityLimit();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Allowed Level: %u Player Level %u", l_AllowedAccountType, AccountTypes(l_AccountGMLevel));

    if (l_AllowedAccountType > SEC_PLAYER && AccountTypes(l_AccountGMLevel) < l_AllowedAccountType)
    {
        SendAuthResponse(AUTH_UNAVAILABLE, false, 0);
        sLog->outInfo(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: User tries to login but his security level is not enough");

        return -1;
    }

    /// Check that Key and account name are the same on client and server
    uint32 l_ChallengeT = 0;
    uint32 l_ServerSeed = m_Seed;

    SHA1Hash l_ServerAuthChallenge;
    l_ServerAuthChallenge.UpdateData(l_AccountIDStr);
    l_ServerAuthChallenge.UpdateData((uint8*)&l_ChallengeT, 4);
    l_ServerAuthChallenge.UpdateData((uint8*)&l_ClientSeed, 4);
    l_ServerAuthChallenge.UpdateData((uint8*)&l_ServerSeed, 4);
    l_ServerAuthChallenge.UpdateBigNumbers(&l_SessionKey, NULL);
    l_ServerAuthChallenge.Finalize();

    std::string l_SessionIP = GetRemoteAddress();

    if (memcmp(l_ServerAuthChallenge.GetDigest(), l_ClientAuthChallenge, SHA_DIGEST_LENGTH))
    {
        SendAuthResponse(AUTH_FAILED, false, 0);
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: Authentication failed for account: %u ('%s') address: %s", l_AccountID, l_AccountName.c_str(), l_SessionIP.c_str());

        return -1;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: Client '%s' authenticated successfully from %s.", l_AccountName.c_str(), l_SessionIP.c_str());

    bool l_AccountIsRecruiter = false;

    /// Update the last_ip in the database
    /// No SQL injection, username escaped.
    LoginDatabase.EscapeString(l_SessionIP);
    LoginDatabase.PExecute("UPDATE account SET last_ip = '%s' WHERE username = '%s'", l_SessionIP.c_str(), l_EscapedAccountName.c_str());

    /// NOTE ATM the socket is single-threaded, have this in mind ...
    ACE_NEW_RETURN(m_Session, WorldSession(l_AccountID, this, AccountTypes(l_AccountGMLevel), l_AccountIsPremium, l_AccountPremiumType, l_AccountExpansion, l_MuteTime, l_AccountLocale, l_Recruiter, l_AccountIsRecruiter, l_VoteRemainingTime, l_ServiceFlags, l_CustomFlags), -1);

    m_Crypt.Init(&l_SessionKey);
    m_HeaderBuffer.size(g_SizeOfClientHeader[m_Initialized][m_Crypt.IsInitialized()]);

    m_Session->LoadGlobalAccountData();
    m_Session->LoadTutorialsData();
    m_Session->ReadAddonsInfo(l_AddonsCompressedData);
    m_Session->SetClientBuild(l_ClientBuild);
    m_Session->SetAccountJoinDate(l_JoinDateTimestamp);
    m_Session->LoadPremades();

    /// - First premade money
    {
        if (sWorld->getIntConfig(CONFIG_FIRST_PREMADE_MONEY))
        {
            QueryResult l_PremadeMoneyResult = CharacterDatabase.PQuery("SELECT account FROM first_premade_money WHERE account = %u", l_AccountID);
            if (!l_PremadeMoneyResult)
                m_Session->SetFirstPremadeMoney();
        }
    }

    /// Initialize Warden system only if it is enabled by config
    if (sWorld->getBoolConfig(CONFIG_WARDEN_ENABLED))
        m_Session->InitWarden(&l_SessionKey, l_AccountOS);

    /// Sleep this Network thread for
    uint32 l_SleepTime = sWorld->getIntConfig(CONFIG_SESSION_ADD_DELAY);
    ACE_OS::sleep(ACE_Time_Value(0, l_SleepTime));

    sWorld->AddSession(m_Session);

    return 0;
}

int WorldSocket::HandlePing(WorldPacket& recvPacket)
{
    uint32 ping;
    uint32 latency;

    // Get the ping packet content
    recvPacket >> ping;
    recvPacket >> latency;

    if (m_LastPingTime == ACE_Time_Value::zero)
        m_LastPingTime = ACE_OS::gettimeofday(); // for 1st ping
    else
    {
        ACE_Time_Value cur_time = ACE_OS::gettimeofday();
        ACE_Time_Value diff_time(cur_time);
        diff_time -= m_LastPingTime;
        m_LastPingTime = cur_time;

        if (diff_time < ACE_Time_Value(27))
        {
            ++m_OverSpeedPings;

            uint32 max_count = sWorld->getIntConfig(CONFIG_MAX_OVERSPEED_PINGS);

            if (max_count && m_OverSpeedPings > max_count)
            {
                ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);

                if (m_Session && AccountMgr::IsPlayerAccount(m_Session->GetSecurity()))
                {
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::HandlePing: %s kicked for over-speed pings (address: %s)",
                        m_Session->GetPlayerName(false).c_str(), GetRemoteAddress().c_str());

                    return -1;
                }
            }
        }
        else
            m_OverSpeedPings = 0;
    }

    // critical section
    {
        ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);

        if (m_Session)
        {
            m_Session->SetLatency(latency);
            m_Session->ResetClientTimeDelay();
        }
        else
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::HandlePing: peer sent CMSG_PING, "
                "but is not authenticated or got recently kicked, "
                " address = %s",
                GetRemoteAddress().c_str());
            return -1;
        }
    }

    WorldPacket packet(SMSG_PONG, 4);
    packet << ping;
    return SendPacket(packet);
}
