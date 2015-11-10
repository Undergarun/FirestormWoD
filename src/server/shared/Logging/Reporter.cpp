////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2014-2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "Reporter.hpp"

#define CURL_STATICLIB
#include "curl/curl.h"

void Reporter::ScheduleNextReport()
{
    ByteBuffer l_ReportDatas;

    m_ReportQueue.next(l_ReportDatas);

    size_t l_Size = l_ReportDatas.size();

    CURL* l_Curl = nullptr;
    CURLcode l_Res = CURLcode::CURLE_OK;

    l_Curl = curl_easy_init();
    if (l_Curl)
    {
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_URL, m_Address.c_str());

        /// If there is a redirection, the request will follow it.
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_POSTFIELDS, "Data=%s", l_ReportDatas.ReadString(l_Size).c_str());

        /// Perform the request, res will get the return code.
        l_Res = curl_easy_perform(l_Curl);

        /// Check for errors.
        if (l_Res != CURLcode::CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(l_Res));

        /// Always cleanup.
        curl_easy_cleanup(l_Curl);
    }
}

void Reporter::EnqueueReport(ByteBuffer p_Datas)
{
    m_ReportQueue.add(p_Datas);
}