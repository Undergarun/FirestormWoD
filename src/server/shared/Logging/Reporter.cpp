////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Reporter.hpp"

#define CURL_STATICLIB
#include "curl/curl.h"

void Reporter::ScheduleNextReport()
{
    std::string l_ReportDatas;

    m_ReportQueue.next(l_ReportDatas);

    CURL* l_Curl = curl_easy_init();
    CURLcode l_Res = CURLcode::CURLE_OK;

    if (l_Curl)
    {
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_URL, (m_Address + m_Index).c_str());

        /// If there is a redirection, the request will follow it.
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_POSTFIELDS, l_ReportDatas.c_str());
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_POSTFIELDSIZE, l_ReportDatas.size());
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_POST, 1);

        struct curl_slist* l_Headers = nullptr;
        l_Headers = curl_slist_append(l_Headers, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_HTTPHEADER, l_Headers);

        /// Perform the request, res will get the return code.
        l_Res = curl_easy_perform(l_Curl);

        /// Check for errors.
        if (l_Res != CURLcode::CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(l_Res));
            EnqueueReport(l_ReportDatas);
        }

        /// Always cleanup.
        curl_easy_cleanup(l_Curl);
        curl_slist_free_all(l_Headers);
    }
}

void Reporter::EnqueueReport(std::string p_Datas)
{
    if (!m_Enabled)
        return;

    m_ReportQueue.add(p_Datas);
}
