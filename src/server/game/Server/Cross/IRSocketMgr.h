#ifdef CROSS
#ifndef __IRSOCKETMGR_H
#define __IRSOCKETMGR_H

#include <ace/Basic_Types.h>
#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>
#include "IRSocketAcceptor.h"

class IRSocket;
class ReactorRunnable;
class ACE_Event_Handler;

/// Manages all sockets connected to peers and network threads
class IRSocketMgr
{
public:
    friend class IRSocket;
    friend class ACE_Singleton<IRSocketMgr, ACE_Thread_Mutex>;

    /// Start network, listen at address:port .
    int StartNetwork(ACE_UINT16 port);

    /// Stops all network threads, It will wait for all running threads .
    void StopNetwork();

    /// Wait untill all network threads have "joined" .
    void Wait();

private:
    int OnSocketOpen(IRSocket* sock);

    int StartReactiveIO(ACE_UINT16 port);

private:
    IRSocketMgr();
    virtual ~IRSocketMgr();

    ReactorRunnable* m_NetThreads;
    size_t m_NetThreadsCount;

    int m_SockOutKBuff;
    int m_SockOutUBuff;
    bool m_UseNoDelay;

    IRSocketAcceptor* m_Acceptor;
};

#define sIRSocketMgr ACE_Singleton<IRSocketMgr, ACE_Thread_Mutex>::instance()

#endif
/// @}
#endif