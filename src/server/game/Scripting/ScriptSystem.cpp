////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptSystem.h"
#include "ObjectMgr.h"
#include "DatabaseEnv.h"
#include "ScriptMgr.h"

ScriptPointVector const SystemMgr::_empty;

void SystemMgr::LoadScriptTexts()
{
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Script Texts...");
    LoadTrinityStrings("script_texts", TEXT_SOURCE_RANGE, 1+(TEXT_SOURCE_RANGE*2));

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Script Texts additional data...");
    uint32 oldMSTime = getMSTime();

    //                                                 0      1      2      3
    QueryResult result = WorldDatabase.Query("SELECT entry, sound, type, language, emote FROM script_texts");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 additional Script Texts data. DB table `script_texts` is empty.");
        return;
    }

    uint32 uiCount = 0;

    do
    {
        Field* pFields = result->Fetch();
        StringTextData temp;

        int32 iId           = pFields[0].GetInt32();
        temp.uiSoundId     = pFields[1].GetUInt32();
        temp.uiType        = pFields[2].GetUInt8();
        temp.uiLanguage    = pFields[3].GetUInt8();
        temp.uiEmote       = pFields[4].GetUInt16();

        if (iId >= 0)
        {
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `script_texts` is not a negative value.", iId);
            continue;
        }

        if (iId > TEXT_SOURCE_RANGE || iId <= TEXT_SOURCE_RANGE*2)
        {
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `script_texts` is out of accepted entry range for table.", iId);
            continue;
        }

        if (temp.uiSoundId)
        {
            if (!sSoundEntriesStore.LookupEntry(temp.uiSoundId))
                sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `script_texts` has soundId %u but sound does not exist.", iId, temp.uiSoundId);
        }

        if (!GetLanguageDescByID(temp.uiLanguage))
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `script_texts` using Language %u but Language does not exist.", iId, temp.uiLanguage);

        if (temp.uiType > CHAT_TYPE_ZONE_YELL)
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `script_texts` has Type %u but this Chat Type does not exist.", iId, temp.uiType);

        m_mTextDataMap[iId] = temp;
        ++uiCount;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u additional Script Texts data in %u ms", uiCount, GetMSTimeDiffToNow(oldMSTime));
}

void SystemMgr::LoadScriptTextsCustom()
{
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Custom Texts...");
    LoadTrinityStrings("custom_texts", TEXT_SOURCE_RANGE*2, 1+(TEXT_SOURCE_RANGE*3));

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Custom Texts additional data...");

    QueryResult result = WorldDatabase.Query("SELECT entry, sound, type, language, emote FROM custom_texts");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 additional Custom Texts data. DB table `custom_texts` is empty.");
        return;
    }

    uint32 uiCount = 0;

    do
    {
        Field* pFields = result->Fetch();
        StringTextData temp;

        int32 iId             = pFields[0].GetInt32();
        temp.uiSoundId        = pFields[1].GetUInt32();
        temp.uiType           = pFields[2].GetUInt8();
        temp.uiLanguage       = pFields[3].GetUInt8();
        temp.uiEmote          = pFields[4].GetUInt16();

        if (iId >= 0)
        {
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `custom_texts` is not a negative value.", iId);
            continue;
        }

        if (iId > TEXT_SOURCE_RANGE*2 || iId <= TEXT_SOURCE_RANGE*3)
        {
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `custom_texts` is out of accepted entry range for table.", iId);
            continue;
        }

        if (temp.uiSoundId)
        {
            if (!sSoundEntriesStore.LookupEntry(temp.uiSoundId))
                sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `custom_texts` has soundId %u but sound does not exist.", iId, temp.uiSoundId);
        }

        if (!GetLanguageDescByID(temp.uiLanguage))
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `custom_texts` using Language %u but Language does not exist.", iId, temp.uiLanguage);

        if (temp.uiType > CHAT_TYPE_ZONE_YELL)
            sLog->outError(LOG_FILTER_SQL, "TSCR: Entry %i in table `custom_texts` has Type %u but this Chat Type does not exist.", iId, temp.uiType);

        m_mTextDataMap[iId] = temp;
        ++uiCount;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u additional Custom Texts data.", uiCount);
}

void SystemMgr::LoadScriptWaypoints()
{
    uint32 oldMSTime = getMSTime();

    // Drop Existing Waypoint list
    m_mPointMoveMap.clear();

    uint64 uiCreatureCount = 0;

    // Load Waypoints
    QueryResult result = WorldDatabase.Query("SELECT COUNT(entry) FROM script_waypoint GROUP BY entry");
    if (result)
        uiCreatureCount = result->GetRowCount();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Script Waypoints for " UI64FMTD " creature(s)...", uiCreatureCount);

    //                                     0       1         2           3           4           5
    result = WorldDatabase.Query("SELECT entry, pointid, location_x, location_y, location_z, waittime FROM script_waypoint ORDER BY pointid");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Script Waypoints. DB table `script_waypoint` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* pFields = result->Fetch();
        ScriptPointMove temp;

        temp.uiCreatureEntry   = pFields[0].GetUInt32();
        uint32 uiEntry          = temp.uiCreatureEntry;
        temp.uiPointId         = pFields[1].GetUInt32();
        temp.fX                = pFields[2].GetFloat();
        temp.fY                = pFields[3].GetFloat();
        temp.fZ                = pFields[4].GetFloat();
        temp.uiWaitTime        = pFields[5].GetUInt32();

        CreatureTemplate const* pCInfo = sObjectMgr->GetCreatureTemplate(temp.uiCreatureEntry);

        if (!pCInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "TSCR: DB table script_waypoint has waypoint for non-existant creature entry %u", temp.uiCreatureEntry);
            continue;
        }

        if (!pCInfo->ScriptID)
            sLog->outError(LOG_FILTER_SQL, "TSCR: DB table script_waypoint has waypoint for creature entry %u, but creature does not have ScriptName defined and then useless.", temp.uiCreatureEntry);

        m_mPointMoveMap[uiEntry].push_back(temp);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Script Waypoint nodes in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}
