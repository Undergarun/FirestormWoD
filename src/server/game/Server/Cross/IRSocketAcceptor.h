#ifdef CROSS
#ifndef __IRSOCKETACCEPTOR_H_
#define __IRSOCKETACCEPTOR_H_

#include "Common.h"

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include "IRSocket.h"

class IRSocketAcceptor : public ACE_Acceptor<IRSocket, ACE_SOCK_Acceptor>
{
public:
    IRSocketAcceptor(void) { }
    virtual ~IRSocketAcceptor(void)
    {
        if (reactor())
            reactor()->cancel_timer(this, 1);
    }

protected:

    virtual int handle_timeout(const ACE_Time_Value& /*current_time*/, const void* /*act = 0*/)
    {
        sLog->outDebug(LOG_FILTER_INTERREALM, "Resuming acceptor");
        reactor()->cancel_timer(this, 1);
        return reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
    }

    virtual int handle_accept_error(void)
    {
#if defined(ENFILE) && defined(EMFILE)
        if (errno == ENFILE || errno == EMFILE)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "Out of file descriptors, suspending incoming connections for 10 seconds");
            reactor()->remove_handler(this, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL);
            reactor()->schedule_timer(this, NULL, ACE_Time_Value(10));
        }
#endif
        return 0;
    }
};

#endif /* __IRSOCKETACCEPTOR_H_ */
/// @}
#endif
