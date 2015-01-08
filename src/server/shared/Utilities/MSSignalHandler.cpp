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

            // We can't rescue the thread, shutdown the server 
            exit(SIGTERM);
        }
    }
}
