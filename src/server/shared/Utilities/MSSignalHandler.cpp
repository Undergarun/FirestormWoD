#include "MSSignalHandler.h"

namespace MS
{
    namespace SignalHandler
    {
        void EnableThrowExceptionAtFailure()
        {
            gThrowExceptionAtFailure = true;
        }

        void DisableThrowExceptionAtFailure()
        {
            gThrowExceptionAtFailure = false;
        }

        void OnSignalReceive(int p_Signal)
        {
            gFailureCounter++;

            if (gThrowExceptionAtFailure && gFailureCounter < gFailureCountLimit)
            {
                // Register the signal handler for the next signal
                signal(p_Signal, &MS::SignalHandler::OnSignalReceive);

                // Throw runtime exception to catch block and go back to execution context
                throw std::runtime_error("");
            }

            ACE_Stack_Trace l_Trace;
            sLog->outError(LOG_FILTER_WORLDSERVER, "MS::SignalHandler : can't rescue the thread, shutdown the server  StackTrace : %s", l_Trace.c_str());

#ifdef _MSC_VER
            /// Pause process execution on windows only to let time to the dev
            /// to see the call stack on crash.
            system("pause");
#endif

            // We can't rescue the thread, shutdown the server 
            exit(SIGTERM);
        }
    }
}
