#ifndef __IRSOCKETCONNECTOR_H_
#define __IRSOCKETCONNECTOR_H_

#include "Common.h"

#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>

#include "IRSocket.h"

class IRSocketConnector : public ACE_Connector<IRSocket, ACE_SOCK_Connector>
{
public:
    IRSocketConnector(void) { }
    virtual ~IRSocketConnector(void)
    {
        if (reactor())
            reactor()->cancel_timer(this, 1);
    }

protected:

    virtual int handle_timeout(const ACE_Time_Value& /*current_time*/, const void* /*act = 0*/)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Resuming acceptor");
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

#endif /* __IRSOCKETCONNECTOR_H_ */
/// @}
