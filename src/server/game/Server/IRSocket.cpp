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
#include "IRSocket.h"
#include "InterRealmSession.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "InterRealmOpcodes.h"
#include "ObjectMgr.h"

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

IRSocket::IRSocket (void): IRHandler(),
m_LastPingTime(ACE_Time_Value::zero), m_OverSpeedPings(0), m_InterRealmSession(0),
m_RecvWPct(0), m_RecvPct(), m_Header(sizeof (IRInPktHeader)),
m_OutBuffer(0), m_OutBufferSize(65536), m_OutActive(false),
m_Seed(static_cast<uint32> (rand32()))
{
    reference_counting_policy().value (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

    msg_queue()->high_water_mark(8 * 1024 * 1024);
    msg_queue()->low_water_mark(8 * 1024 * 1024);
}

IRSocket::~IRSocket (void)
{
    delete m_RecvWPct;

    if (m_OutBuffer)
        m_OutBuffer->release();

    closing_ = true;

    peer().close();
}

bool IRSocket::IsClosed (void) const
{
    return closing_;
}

void IRSocket::CloseSocket (void)
{
    {
        if (closing_)
            return;

        ACE_GUARD (LockType, Guard, m_OutBufferLock);

        closing_ = true;
        peer().close_writer();
    }

    {
        ACE_GUARD (LockType, Guard, m_SessionLock);

        m_InterRealmSession = NULL;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "[INTERREALM] IRSocket::CloseSocket");
}

const std::string& IRSocket::GetRemoteAddress (void) const
{
    return m_Address;
}

int IRSocket::SendPacket(WorldPacket const* pct)
{
    ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

    if (closing_)
        return -1;

    IROutPktHeader header(pct->size() + 4, pct->GetOpcode());

    if (m_OutBuffer->space() >= pct->size() + header.getHeaderLength() && msg_queue()->is_empty())
    {
        // Put the packet on the buffer.
        if (m_OutBuffer->copy((char*) header.header, header.getHeaderLength()) == -1)
            ACE_ASSERT (false);

        if (!pct->empty())
            if (m_OutBuffer->copy((char*) pct->contents(), pct->size()) == -1)
                ACE_ASSERT (false);
    }
    else
    {
        // Enqueue the packet.
        ACE_Message_Block* mb;

        ACE_NEW_RETURN(mb, ACE_Message_Block(pct->size() + header.getHeaderLength()), -1);

        mb->copy((char*) header.header, header.getHeaderLength());

        if (!pct->empty())
            mb->copy((const char*)pct->contents(), pct->size());

        if (msg_queue()->enqueue_tail(mb, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::SendPacket enqueue_tail failed");
            mb->release();
            return -1;
        }
    }

    return 0;
}

long IRSocket::AddReference (void)
{
    return static_cast<long> (add_reference());
}

long IRSocket::RemoveReference (void)
{
    return static_cast<long> (remove_reference());
}

int IRSocket::open (void *a)
{
    m_InterRealmSession = sWorld->GetInterRealmSession();

    if (!m_InterRealmSession)
        return -1;

    ACE_UNUSED_ARG (a);

    // Prevent double call to this func.
    if (m_OutBuffer)
        return -1;

    // This will also prevent the socket from being Updated
    // while we are initializing it.
    m_OutActive = true;

    // Hook for the manager.
    if (m_InterRealmSession->OnSocketOpen(this) == -1)
        return -1;

    // Allocate the buffer.
    ACE_NEW_RETURN (m_OutBuffer, ACE_Message_Block (m_OutBufferSize), -1);

    // Store peer address.
    ACE_INET_Addr remote_addr;

    /*if (peer().get_remote_addr(remote_addr) == -1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "IRSocket::open: peer().get_remote_addr errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    m_Address = remote_addr.get_host_addr();*/

    // Register with ACE Reactor
    if (reactor()->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK) == -1)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::open: unable to register client handler errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    // reactor takes care of the socket from now on
    remove_reference();

    return 0;
}

int IRSocket::close (u_long)
{
    shutdown();

    closing_ = true;

    remove_reference();

    return 0;
}

int IRSocket::handle_input (ACE_HANDLE)
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
                return Update();  // interesting line, isn't it ?
            }

            sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::handle_input: Peer error closing connection errno = %s", ACE_OS::strerror (errno));

            errno = ECONNRESET;
            return -1;
        }
        case 0:
        {
            sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::handle_input: Peer has closed connection");

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

int IRSocket::handle_output (ACE_HANDLE)
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

int IRSocket::handle_output_queue (GuardType& g)
{
    if (msg_queue()->is_empty())
        return cancel_wakeup_output(g);

    ACE_Message_Block* mblk;

    if (msg_queue()->dequeue_head(mblk, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::handle_output_queue dequeue_head");
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
            sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::handle_output_queue enqueue_head");
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

int IRSocket::handle_close (ACE_HANDLE h, ACE_Reactor_Mask)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "IRSocket::handle_close");

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

        if (m_InterRealmSession)
        {
            m_InterRealmSession->SetConnected(false);
            m_InterRealmSession->SetProcessDisconnect(true);
            m_InterRealmSession->ClearSocket();
            m_InterRealmSession = NULL;
        }
    }

    reactor()->remove_handler(this, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::ALL_EVENTS_MASK);
    return 0;
}

int IRSocket::Update (void)
{
    if (closing_)
        return -1;

    if (m_OutActive || (m_OutBuffer->length() == 0 && msg_queue()->is_empty()))
        return 0;

    int ret;
    do
        ret = handle_output(get_handle());
    while (ret > 0);

    return ret;
}

int IRSocket::handle_input_header (void)
{
    ACE_ASSERT(m_RecvWPct == NULL);

    ACE_ASSERT(m_Header.length() == sizeof(IRInPktHeader));

    IRInPktHeader& header = *((IRInPktHeader*)m_Header.rd_ptr());

    //EndianConvertReverse(header.size);
    EndianConvert(header.size);
    EndianConvert(header.cmd);

    if (header.size < 2)  // LR (from MSG_VERIFY_CONNECTIVITY)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Header size failed, header size %u.", header.size);
        errno = EINVAL;
        return -1;
    }

    header.size -= 4;

    ACE_NEW_RETURN(m_RecvWPct, WorldPacket (Opcodes(header.cmd), header.size), -1);

    if (header.size > 0)
    {
        m_RecvWPct->resize(header.size);
        m_RecvPct.base ((char*) m_RecvWPct->contents(), m_RecvWPct->size());
    }
    else
    {
        ACE_ASSERT(m_RecvPct.space() == 0);
    }

    return 0;
}

int IRSocket::handle_input_payload (void)
{
    // set errno properly here on error !!!
    // now have a header and payload

    ACE_ASSERT (m_RecvPct.space() == 0);
    ACE_ASSERT (m_Header.space() == 0);
    ACE_ASSERT (m_RecvWPct != NULL);

    const int ret = ProcessIncoming (m_RecvWPct);

    m_RecvPct.base (NULL, 0);
    m_RecvPct.reset();
    m_RecvWPct = NULL;

    m_Header.reset();

    if (ret == -1)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "handle_input_payload failed.");
        errno = EINVAL;
    }

    return ret;
}

int IRSocket::handle_input_missing_data (void)
{
    char buf [4096];

    ACE_Data_Block db (sizeof (buf),
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

    const ssize_t n = peer().recv (message_block.wr_ptr(),
                                          recv_size);

    if (n <= 0)
        return int(n);

    message_block.wr_ptr (n);

    while (message_block.length() > 0)
    {
        if (m_Header.space() > 0)
        {
            //need to receive the header
            const size_t to_header = (message_block.length() > m_Header.space() ? m_Header.space() : message_block.length());
            m_Header.copy (message_block.rd_ptr(), to_header);
            message_block.rd_ptr (to_header);

            if (m_Header.space() > 0)
            {
                // Couldn't receive the whole header this time.
                ACE_ASSERT (message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }

            // We just received nice new header
            if (handle_input_header() == -1)
            {
                ACE_ASSERT ((errno != EWOULDBLOCK) && (errno != EAGAIN));
                return -1;
            }
        }

        // Its possible on some error situations that this happens
        // for example on closing when epoll receives more chunked data and stuff
        // hope this is not hack, as proper m_RecvWPct is asserted around
        if (!m_RecvWPct)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "Forcing close on input m_RecvWPct = NULL");
            errno = EINVAL;
            return -1;
        }

        // We have full read header, now check the data payload
        if (m_RecvPct.space() > 0)
        {
            //need more data in the payload
            const size_t to_data = (message_block.length() > m_RecvPct.space() ? m_RecvPct.space() : message_block.length());
            m_RecvPct.copy (message_block.rd_ptr(), to_data);
            message_block.rd_ptr (to_data);

            if (m_RecvPct.space() > 0)
            {
                // Couldn't receive the whole data this time.
                ACE_ASSERT (message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }
        }

        //just received fresh new payload
        if (handle_input_payload() == -1)
        {
            ACE_ASSERT ((errno != EWOULDBLOCK) && (errno != EAGAIN));
            return -1;
        }
    }

    return size_t(n) == recv_size ? 1 : 2;
}

int IRSocket::cancel_wakeup_output (GuardType& g)
{
    if (!m_OutActive)
        return 0;

    m_OutActive = false;

    g.release();

    if (reactor()->cancel_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        // would be good to store errno from reactor with errno guard
        sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::cancel_wakeup_output");
        return -1;
    }

    return 0;
}

int IRSocket::schedule_wakeup_output (GuardType& g)
{
    if (m_OutActive)
        return 0;

    m_OutActive = true;

    g.release();

    if (reactor()->schedule_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "IRSocket::schedule_wakeup_output");
        return -1;
    }

    return 0;
}

int IRSocket::ProcessIncoming(WorldPacket* new_pct)
{
    ACE_ASSERT (new_pct);

    // manage memory ;)

    uint16 opcode = new_pct->GetOpcode();

    if (closing_)
        return -1;

    //std::string opcodeName = GetIROpcodeNameForLogging(opcode);

    try
    {
        switch (opcode)
        {
            case IR_SMSG_TUNNEL_PACKET:
                return Handle_TunneledPacket(new_pct);
                break;
            default:
            {
                ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);

                // WARNING here we call it with locks held.
                // Its possible to cause deadlock if QueuePacket calls back
                m_InterRealmSession->AddPacket(new_pct);
                return 0;
            }
        }
    }
    catch (ByteBufferException &)
    {
        /*sLog->outInfo(LOG_FILTER_SERVER_LOADING, "[INTERREALM] IRSocket::ProcessIncoming ByteBufferException occured while parsing an instant handled packet %s from client %s. Disconnected client.",
                       opcodeName.c_str(), GetRemoteAddress().c_str());*/
        new_pct->hexlike();
        return -1;
    }

    ACE_NOTREACHED (return 0);
}

int IRSocket::Handle_TunneledPacket(WorldPacket* packet)
{
    uint64 playerGuid;
    uint16 opcodeId;

    *packet >> playerGuid;
    *packet >> opcodeId;

    packet->eraseFirst(10); // remove playerGuid and opcodeId data
    packet->SetOpcode((Opcodes)opcodeId);
    packet->rfinish();

    // Now we have standart packet

    if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        if (pPlayer->GetSession())
            pPlayer->GetSession()->SendPacket(packet, false, true);
    }

    delete packet;

    return 0;
}