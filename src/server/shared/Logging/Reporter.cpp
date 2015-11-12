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
    std::string l_ReportDatas;

    m_ReportQueue.next(l_ReportDatas);

    size_t l_Size = l_ReportDatas.size();

    CURL* l_Curl = curl_easy_init();
    CURLcode l_Res = CURLcode::CURLE_OK;

    if (l_Curl)
    {
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_URL, (m_Address + m_Index).c_str());

        /// If there is a redirection, the request will follow it.
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_POSTFIELDS, l_ReportDatas.c_str());
        curl_easy_setopt(l_Curl, CURLoption::CURLOPT_POSTFIELDSIZE, l_ReportDatas.size());

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
    }
}

void Reporter::EnqueueReport(std::string p_Datas)
{
    m_ReportQueue.add(p_Datas);
}

std::string Reporter::BuildPvEReport(ByteBuffer p_Datas)
{
    EasyJSon::Node<std::string> l_Node;

    l_Node["Expansion"]         = p_Datas.read<uint32>();
    l_Node["RealmID"]           = p_Datas.read<uint32>();
    l_Node["GuildID"]           = p_Datas.read<uint32>();
    l_Node["GuildFaction"]      = p_Datas.read<uint32>();

    std::string l_Str;
    p_Datas >> l_Str;

    l_Node["GuildName"]         = l_Str;
    l_Node["MapID"]             = p_Datas.read<uint32>();
    l_Node["EncounterID"]       = p_Datas.read<uint32>();
    l_Node["DifficultyID"]      = p_Datas.read<uint32>();
    l_Node["StartTime"]         = p_Datas.read<uint32>();
    l_Node["CombatDuration"]    = p_Datas.read<uint32>();
    l_Node["Success"]           = p_Datas.read<uint8>();

    l_Str = "";
    p_Datas >> l_Str;

    l_Node["RosterDatas"]       = l_Str;

    l_Str = "";
    p_Datas >> l_Str;

    l_Node["EncounterHealth"]   = l_Str;
    l_Node["DeadCount"]         = p_Datas.read<uint32>();

    return l_Node.Serialize<std::ostringstream>(true);
}