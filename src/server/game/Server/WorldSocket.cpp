////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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

uint32_t gReceivedBytes = 0;
uint32_t gSentBytes = 0;

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct ServerPktHeader
{
    ServerPktHeader(uint32 size, uint32 cmd, AuthCrypt* _authCrypt) : size(size)
    {
        if (_authCrypt->IsInitialized())
        {
            uint32 data = (size << 13) | cmd & 0x1FFF;
            memcpy(&header[0], &data, 4);
            _authCrypt->EncryptSend((uint8*)&header[0], getHeaderLength());
        }
        else
        {
            // Dynamic header size is not needed anymore, we are using not encrypted part for only the first few packets
            memcpy(&header[0], &size, 2);
            memcpy(&header[2], &cmd, 2);
        }
    }

    uint8 getHeaderLength()
    {
        return 4;
    }

    const uint32 size;
    uint8 header[4];
};

struct AuthClientPktHeader
{
    uint16 size;
    uint32 cmd;
};

struct WorldClientPktHeader
{
    uint16 size;
    uint16 cmd;
};

#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

WorldSocket::WorldSocket(void) : WorldHandler(),
m_LastPingTime(ACE_Time_Value::zero), m_OverSpeedPings(0), m_Session(0),
m_RecvWPct(0), m_RecvPct(), m_Header(sizeof(AuthClientPktHeader)),
m_WorldHeader(sizeof(WorldClientPktHeader)), m_OutBuffer(0),
m_OutBufferSize(65536), m_OutActive(false),

m_Seed(static_cast<uint32> (rand32()))
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

    sScriptMgr->OnPacketSend(this, *pkt);

    ServerPktHeader header(!m_Crypt.IsInitialized() ? pkt->size() + 2 : pct.size(), pkt->GetOpcode(), &m_Crypt);

    if (m_OutBuffer->space() >= pkt->size() + header.getHeaderLength() && msg_queue()->is_empty())
    {
        // Put the packet on the buffer.
        if (m_OutBuffer->copy((char*)header.header, header.getHeaderLength()) == -1)
            ACE_ASSERT(false);

        if (!pkt->empty())
        if (m_OutBuffer->copy((char*)pkt->contents(), pkt->size()) == -1)
            ACE_ASSERT(false);
    }
    else
    {
        // Enqueue the packet.
        ACE_Message_Block* mb;

        ACE_NEW_RETURN(mb, ACE_Message_Block(pkt->size() + header.getHeaderLength()), -1);

        mb->copy((char*)header.header, header.getHeaderLength());

        if (!pkt->empty())
            mb->copy((const char*)pkt->contents(), pkt->size());

        if (msg_queue()->enqueue_tail(mb, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::SendPacket enqueue_tail failed");
            mb->release();
            return -1;
        }
    }

    return 0;
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

    // not an opcode. this packet sends raw string WORLD OF WARCRAFT CONNECTION - SERVER TO CLIENT"
    // because of our implementation, bytes "WO" become the opcode
    WorldPacket packet(SMSG_HANDSHAKE);
    packet << "RLD OF WARCRAFT CONNECTION - SERVER TO CLIENT";

    if (SendPacket(packet) == -1)
        return -1;

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

    switch (handle_input_missing_data())
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

int WorldSocket::handle_input_header (void)
{
    ACE_ASSERT(m_RecvWPct == NULL);

    if (m_Crypt.IsInitialized())
    {
        ACE_ASSERT(m_WorldHeader.length() == sizeof(WorldClientPktHeader));
        uint8* uintHeader = (uint8*)m_WorldHeader.rd_ptr();
        m_Crypt.DecryptRecv(uintHeader, sizeof(WorldClientPktHeader));
        WorldClientPktHeader& header = *(WorldClientPktHeader*)uintHeader;

        uint32 value = *(uint32*)uintHeader;
        header.cmd = value & 0x1FFF;
        header.size = ((value & ~(uint32)0x1FFF) >> 13);

        if (sWorld->getBoolConfig(CONFIG_LOG_PACKETS))
        {
            std::string opcodeName = GetOpcodeNameForLogging((Opcodes)header.cmd, WOW_CLIENT_TO_SERVER);
            printf("Receive opcode %s 0x%08.8X size : %u \n", opcodeName.c_str(), header.cmd, header.size);
        }

        if (header.size > 10236)
        {
            Player* _player = m_Session ? m_Session->GetPlayer() : NULL;
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::handle_input_header(): client (account: %u, char [GUID: %u, name: %s]) sent malformed packet (size: %d, cmd: %d)",
                m_Session ? m_Session->GetAccountId() : 0,
                _player ? _player->GetGUIDLow() : 0,
                _player ? _player->GetName() : "<none>",
                header.size, header.cmd);

            errno = EINVAL;
            return -1;
        }

        ACE_NEW_RETURN(m_RecvWPct, WorldPacket(PacketFilter::DropHighBytes(header.cmd), header.size), -1);

        if (header.size > 0)
        {
            m_RecvWPct->resize(header.size);
            m_RecvPct.base((char*)m_RecvWPct->contents(), m_RecvWPct->size());
        }
        else
            ACE_ASSERT(m_RecvPct.space() == 0);
    }
    else
    {
        ACE_ASSERT(m_Header.length() == sizeof(AuthClientPktHeader));
        uint8* uintHeader = (uint8*)m_Header.rd_ptr();
        AuthClientPktHeader& header = *((AuthClientPktHeader*)uintHeader);

        if ((header.size < 4) || (header.size > 10240))
        {
            Player* _player = m_Session ? m_Session->GetPlayer() : NULL;
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::handle_input_header(): client (account: %u, char [GUID: %u, name: %s]) sent malformed packet (size: %d, cmd: %d)",
                m_Session ? m_Session->GetAccountId() : 0,
                _player ? _player->GetGUIDLow() : 0,
                _player ? _player->GetName() : "<none>",
                header.size, header.cmd);

            errno = EINVAL;
            return -1;
        }

        header.size -= 4;

        ACE_NEW_RETURN(m_RecvWPct, WorldPacket(PacketFilter::DropHighBytes(header.cmd), header.size), -1);

        if (header.size > 0)
        {
            m_RecvWPct->resize(header.size);
            m_RecvPct.base((char*)m_RecvWPct->contents(), m_RecvWPct->size());
        }
        else
            ACE_ASSERT(m_RecvPct.space() == 0);
    }

    return 0;
}

int WorldSocket::handle_input_payload (void)
{
    // set errno properly here on error !!!
    // now have a header and payload

    if (m_Crypt.IsInitialized())
    {
        ACE_ASSERT(m_RecvPct.space() == 0);
        ACE_ASSERT(m_WorldHeader.space() == 0);
        ACE_ASSERT(m_RecvWPct != NULL);

        const int ret = ProcessIncoming(m_RecvWPct);

        m_RecvPct.base(NULL, 0);
        m_RecvPct.reset();
        m_RecvWPct = NULL;

        m_WorldHeader.reset();

        if (ret == -1)
            errno = EINVAL;

        return ret;
    }
    else
    {
        ACE_ASSERT(m_RecvPct.space() == 0);
        ACE_ASSERT(m_Header.space() == 0);
        ACE_ASSERT(m_RecvWPct != NULL);

        const int ret = ProcessIncoming(m_RecvWPct);

        m_RecvPct.base(NULL, 0);
        m_RecvPct.reset();
        m_RecvWPct = NULL;

        m_Header.reset();

        if (ret == -1)
            errno = EINVAL;

        return ret;
    }
}

int WorldSocket::handle_input_missing_data (void)
{
    char buf[4096];

    ACE_Data_Block db(sizeof (buf),
        ACE_Message_Block::MB_DATA,
        buf,
        0,
        0,
        ACE_Message_Block::DONT_DELETE,
        0);

    ACE_Message_Block message_block(&db,
        ACE_Message_Block::DONT_DELETE,
        0);

    const size_t recv_size = message_block.space();

    const ssize_t n = peer().recv(message_block.wr_ptr(),
        recv_size);

    if (n <= 0)
        return int(n);

    message_block.wr_ptr(n);

    while (message_block.length() > 0)
    {
        if (m_Crypt.IsInitialized())
        {
            if (m_WorldHeader.space() > 0)
            {
                //need to receive the header
                const size_t to_header = (message_block.length() > m_WorldHeader.space() ? m_WorldHeader.space() : message_block.length());
                m_WorldHeader.copy(message_block.rd_ptr(), to_header);
                message_block.rd_ptr(to_header);

                if (m_WorldHeader.space() > 0)
                {
                    // Couldn't receive the whole header this time.
                    ACE_ASSERT(message_block.length() == 0);
                    errno = EWOULDBLOCK;
                    return -1;
                }

                // We just received nice new header
                if (handle_input_header() == -1)
                {
                    ACE_ASSERT((errno != EWOULDBLOCK) && (errno != EAGAIN));
                    return -1;
                }
            }
        }
        else
        {
            if (m_Header.space() > 0)
            {
                //need to receive the header
                const size_t to_header = (message_block.length() > m_Header.space() ? m_Header.space() : message_block.length());
                m_Header.copy(message_block.rd_ptr(), to_header);
                message_block.rd_ptr(to_header);

                if (m_Header.space() > 0)
                {
                    // Couldn't receive the whole header this time.
                    ACE_ASSERT(message_block.length() == 0);
                    errno = EWOULDBLOCK;
                    return -1;
                }

                // We just received nice new header
                if (handle_input_header() == -1)
                {
                    ACE_ASSERT((errno != EWOULDBLOCK) && (errno != EAGAIN));
                    return -1;
                }
            }
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
            const size_t to_data = (message_block.length() > m_RecvPct.space() ? m_RecvPct.space() : message_block.length());
            m_RecvPct.copy(message_block.rd_ptr(), to_data);
            message_block.rd_ptr(to_data);

            if (m_RecvPct.space() > 0)
            {
                // Couldn't receive the whole data this time.
                ACE_ASSERT(message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }
        }

        //just received fresh new payload
        if (handle_input_payload() == -1)
        {
            ACE_ASSERT((errno != EWOULDBLOCK) && (errno != EAGAIN));
            return -1;
        }
    }

    return size_t(n) == recv_size ? 1 : 2;
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

int WorldSocket::ProcessIncoming(WorldPacket* new_pct)
{
    ACE_ASSERT (new_pct);

    // manage memory ;)
    ACE_Auto_Ptr<WorldPacket> aptr(new_pct);

    uint16 opcode = PacketFilter::DropHighBytes(new_pct->GetOpcode());

    if (closing_)
        return -1;

    // Dump received packet.
    if (sPacketLog->CanLogPacket())
        sPacketLog->LogPacket(*new_pct, CLIENT_TO_SERVER);

    /// Remove log for latency
    ///std::string opcodeName = GetOpcodeNameForLogging(opcode, WOW_CLIENT_TO_SERVER);
    ///if (opcode != CMSG_MOVE_START_FORWARD)
    ///    sLog->outInfo(LOG_FILTER_OPCODES, "C->S: %s", opcodeName.c_str());

    try
    {
        switch (opcode)
        {
            case CMSG_PING:
                return HandlePing(*new_pct);
            case CMSG_AUTH_SESSION:
            {
                if (m_Session)
                {
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::ProcessIncoming: received duplicate CMSG_AUTH_SESSION from %s", m_Session->GetPlayerName(false).c_str());
                    return -1;
                }

                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return HandleAuthSession(*new_pct);
            }
            case CMSG_KEEP_ALIVE:
            {
                sLog->outDebug(LOG_FILTER_NETWORKIO, "%s", GetOpcodeNameForLogging(opcode, WOW_CLIENT_TO_SERVER).c_str());
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return 0;
            }
            case CMSG_LOG_DISCONNECT:
            {
                new_pct->rfinish(); // contains uint32 disconnectReason;
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return 0;
            }
            // not an opcode, client sends string "WORLD OF WARCRAFT CONNECTION - CLIENT TO SERVER" without opcode
            // first 4 bytes become the opcode (2 dropped)
            case CMSG_HANDSHAKE:
            {
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                std::string str;
                *new_pct >> str;
                if (str != "D OF WARCRAFT CONNECTION - CLIENT TO SERVER")
                    return -1;
                return HandleSendAuthSession();
            }
            case CMSG_ENABLE_NAGLE:
            {
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return m_Session ? m_Session->HandleEnableNagleAlgorithm() : -1;
            }
            default:
            {
                ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);
                if (!m_Session)
                {
                    sLog->outError(LOG_FILTER_OPCODES, "ProcessIncoming: Client not authed opcode = %u", uint32(opcode));
                    return -1;
                }

                // prevent invalid memory access/crash with custom opcodes
                if (opcode >= NUM_OPCODE_HANDLERS)
                    return 0;

                OpcodeHandler* handler = g_OpcodeTable[WOW_CLIENT_TO_SERVER][opcode];
                if (!handler || handler->status == STATUS_UNHANDLED)
                {
                    return 0;
                }
                // Our Idle timer will reset on any non PING opcodes.
                // Catches people idling on the login screen and any lingering ingame connections.
                m_Session->ResetTimeOutTime();

                // OK, give the packet to WorldSession
                aptr.release();
                // WARNING here we call it with locks held.
                // Its possible to cause deadlock if QueuePacket calls back
                if (handler->forwardToIR == PROCESS_DISTANT_IF_NEED && m_Session->GetInterRealmBG())
                {
                    Player* player = m_Session->GetPlayer();
                    if (!player)
                        return 0;

                    sWorld->GetInterRealmSession()->SendTunneledPacket(player->GetGUID(), new_pct);
                }
                else
                {
                    // WARNING here we call it with locks held.
                    // Its possible to cause deadlock if QueuePacket calls back
                    m_Session->QueuePacket(new_pct);
                }
                return 0;
            }
        }
    }
    catch (ByteBufferException &)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSocket::ProcessIncoming ByteBufferException occured while parsing an instant handled packet %s from client %s, accountid=%i. Disconnected client.",
            GetOpcodeNameForLogging(opcode, WOW_CLIENT_TO_SERVER).c_str(), GetRemoteAddress().c_str(), m_Session ? int32(m_Session->GetAccountId()) : -1);
        new_pct->hexlike();
        return -1;
    }

    ACE_NOTREACHED (return 0);
}

int WorldSocket::HandleSendAuthSession()
{
    WorldPacket packet(SMSG_AUTH_CHALLENGE, 37);
    packet << uint16(0);

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
    uint64 l_JoinDateTimestamp  = l_Fields[12].GetUInt64();
    uint32 l_ServiceFlags       = l_Fields[13].GetUInt32();
    uint32 l_CustomFlags        = l_Fields[14].GetUInt32();

    if (l_AccountExpansion > l_ServerExpansion)
        l_AccountExpansion = l_ServerExpansion;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSocket::HandleAuthSession: (s,v) check s: %s v: %s", l_Fields[5].GetCString(), l_Fields[4].GetCString());

    /// Re-check ip locking (same check as in auth server).
    if (l_Fields[3].GetUInt8() == 1) // if ip is locked
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

    m_Session->LoadGlobalAccountData();
    m_Session->LoadTutorialsData();
    m_Session->ReadAddonsInfo(l_AddonsCompressedData);
    m_Session->SetClientBuild(l_ClientBuild);
    m_Session->SetAccountJoinDate(l_JoinDateTimestamp);
    m_Session->LoadPremades();

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
