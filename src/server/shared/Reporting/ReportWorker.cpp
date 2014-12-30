#include "ReportWorker.hpp"
#include "Reporter.hpp"

#include <iostream>

namespace MS
{
    namespace Reporting
    {
        ReportWorker::ReportWorker(std::string const&& p_Address)
            : m_Address(p_Address)
        {
        }

        ReportWorker::~ReportWorker()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        bool ReportWorker::ProcessReporting()
        {
            if (!sReporter->HasReports())
                return true;

            auto const&& l_ReportData = sReporter->Next();

            auto l_Size = l_ReportData->size();

            CURL* l_Curl = nullptr;
            CURLcode l_Res = CURLE_OK;

            l_Curl = curl_easy_init();
            if (l_Curl)
            {
                curl_easy_setopt(l_Curl, CURLOPT_URL, m_Address.c_str());
                curl_easy_setopt(l_Curl, CURLOPT_FOLLOWLOCATION, 1L); // If there is a redirection, the request will follow it.
                curl_easy_setopt(l_Curl, CURLOPT_POSTFIELDS, "Data=%s", l_ReportData->ReadString(l_Size).c_str());

                // Perform the request, res will get the return code.
                l_Res = curl_easy_perform(l_Curl);
                // Check for errors.
                if (l_Res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(l_Res));

                // Always cleanup.
                curl_easy_cleanup(l_Curl);
                return true;
            }

            return false;
        }
    }
}