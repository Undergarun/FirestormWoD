#ifndef _IRSOCKET_H
#define _IRSOCKET_H

#include <ace/Basic_Types.h>
#include <ace/Synch_Traits.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Unbounded_Queue.h>
#include <ace/Message_Block.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Common.h"

class ACE_Message_Block;
class WorldPacket;
class InterRealmSession;

/// Handler that can communicate over stream sockets.
typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> IRHandler;

class IRSocket : public IRHandler
{
    public:
        IRSocket (void);
        virtual ~IRSocket (void);

        /// Mutex type used for various synchronizations.
        typedef ACE_Thread_Mutex LockType;
        typedef ACE_Guard<LockType> GuardType;

        /// Check if socket is closed.
        bool IsClosed (void) const;

        /// Close the socket.
        void CloseSocket (void);

        /// Get address of connected peer.
        const std::string& GetRemoteAddress (void) const;

        /// Send A packet on the socket, this function is reentrant.
        /// @param pct packet to send
        /// @return -1 of failure
        int SendPacket(const WorldPacket* pct);

        /// Add reference to this object.
        long AddReference (void);

        /// Remove reference to this object.
        long RemoveReference (void);

        /// things called by ACE framework.

        /// Called on open, the void* is the acceptor.
        virtual int open (void *);

        /// Called on failures inside of the acceptor, don't call from your code.
        virtual int close (u_long);

        /// Called when we can read from the socket.
        virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);

        /// Called when the socket can write.
        virtual int handle_output (ACE_HANDLE = ACE_INVALID_HANDLE);

        /// Called when connection is closed or error happens.
        virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

        /// Called by WorldSocketMgr/ReactorRunnable.
        int Update (void);

        /// Size of the m_OutBuffer.
        size_t m_OutBufferSize;

    private:
        /// Helper functions for processing incoming data.
        int handle_input_header (void);
        int handle_input_payload (void);
        int handle_input_missing_data (void);

        /// Help functions to mark/unmark the socket for output.
        /// @param g the guard is for m_OutBufferLock, the function will release it
        int cancel_wakeup_output (GuardType& g);
        int schedule_wakeup_output (GuardType& g);

        /// Drain the queue if its not empty.
        int handle_output_queue (GuardType& g);

        /// process one incoming packet.
        /// @param new_pct received packet, note that you need to delete it.
        int ProcessIncoming (WorldPacket* new_pct);

        int Handle_TunneledPacket(WorldPacket* new_pct);

    private:
        /// Time in which the last ping was received
        ACE_Time_Value m_LastPingTime;

        /// Keep track of over-speed pings, to prevent ping flood.
        uint32 m_OverSpeedPings;

        /// Address of the remote peer
        std::string m_Address;

        /// Mutex lock to protect m_Session
        LockType m_SessionLock;

        /// here are stored the fragments of the received data
        WorldPacket* m_RecvWPct;

        /// This block actually refers to m_RecvWPct contents,
        /// which allows easy and safe writing to it.
        /// It wont free memory when its deleted. m_RecvWPct takes care of freeing.
        ACE_Message_Block m_RecvPct;

        /// Fragment of the received header.
        ACE_Message_Block m_Header;

        /// Mutex for protecting output related data.
        LockType m_OutBufferLock;

        /// Buffer used for writing output.
        ACE_Message_Block* m_OutBuffer;

        /// True if the socket is registered with the reactor for output
        bool m_OutActive;

        uint32 m_Seed;

        InterRealmSession* m_InterRealmSession;
};

#endif  /* _IRSOCKET_H */

/// @}

