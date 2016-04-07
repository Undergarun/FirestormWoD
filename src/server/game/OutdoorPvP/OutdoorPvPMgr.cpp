////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "OutdoorPvPMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"

OutdoorPvPMgr::OutdoorPvPMgr()
{
    m_UpdateTimer = 0;
    //sLog->outDebug(LOG_FILTER_OUTDOORPVP, "Instantiating OutdoorPvPMgr");
}

void OutdoorPvPMgr::Die()
{
    //sLog->outDebug(LOG_FILTER_OUTDOORPVP, "Deleting OutdoorPvPMgr");
    for (OutdoorPvPSet::iterator itr = m_OutdoorPvPSet.begin(); itr != m_OutdoorPvPSet.end(); ++itr)
        delete *itr;

    for (OutdoorPvPDataMap::iterator itr = m_OutdoorPvPDatas.begin(); itr != m_OutdoorPvPDatas.end(); ++itr)
        delete itr->second;
}

void OutdoorPvPMgr::InitOutdoorPvP()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0       1
    QueryResult result = WorldDatabase.Query("SELECT TypeId, ScriptName FROM outdoorpvp_template");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 outdoor PvP definitions. DB table `outdoorpvp_template` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 typeId = 0;

    do
    {
        Field* fields = result->Fetch();

        typeId = fields[0].GetUInt8();

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_OUTDOORPVP, typeId, NULL))
            continue;

        if (typeId >= MAX_OUTDOORPVP_TYPES)
        {
            sLog->outError(LOG_FILTER_SQL, "Invalid OutdoorPvPTypes value %u in outdoorpvp_template; skipped.", typeId);
            continue;
        }

        OutdoorPvPData* data = new OutdoorPvPData();
        OutdoorPvPTypes realTypeId = OutdoorPvPTypes(typeId);
        data->TypeId = realTypeId;
        data->ScriptId = sObjectMgr->GetScriptId(fields[1].GetCString());
        m_OutdoorPvPDatas[realTypeId] = data;

        ++count;
    }
    while (result->NextRow());

    OutdoorPvP* pvp;
    for (uint8 i = 1; i < MAX_OUTDOORPVP_TYPES; ++i)
    {
        OutdoorPvPDataMap::iterator iter = m_OutdoorPvPDatas.find(OutdoorPvPTypes(i));
        if (iter == m_OutdoorPvPDatas.end())
        {
            sLog->outError(LOG_FILTER_SQL, "Could not initialize OutdoorPvP object for type ID %u; no entry in database.", uint32(i));
            continue;
        }

        pvp = sScriptMgr->CreateOutdoorPvP(iter->second);
        if (!pvp)
        {
            sLog->outError(LOG_FILTER_OUTDOORPVP, "Could not initialize OutdoorPvP object for type ID %u; got NULL pointer from script.", uint32(i));
            continue;
        }

        if (!pvp->SetupOutdoorPvP())
        {
            sLog->outError(LOG_FILTER_OUTDOORPVP, "Could not initialize OutdoorPvP object for type ID %u; SetupOutdoorPvP failed.", uint32(i));
            delete pvp;
            continue;
        }

        m_OutdoorPvPSet.push_back(pvp);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u outdoor PvP definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void OutdoorPvPMgr::AddZone(uint32 zoneid, OutdoorPvP* handle)
{
    m_OutdoorPvPMap[zoneid] = handle;
}

void OutdoorPvPMgr::HandlePlayerEnterZone(Player* player, uint32 zoneid)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(zoneid);
    if (itr == m_OutdoorPvPMap.end())
        return;

    if (itr->second->HasPlayer(player))
        return;

    itr->second->HandlePlayerEnterZone(player, zoneid);
}

void OutdoorPvPMgr::HandlePlayerLeaveZone(Player* player, uint32 zoneid)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(zoneid);
    if (itr == m_OutdoorPvPMap.end())
        return;

    // teleport: remove once in removefromworld, once in updatezone
    if (!itr->second->HasPlayer(player))
        return;

    itr->second->HandlePlayerLeaveZone(player, zoneid);
}

void OutdoorPvPMgr::HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(p_Player->GetZoneId());
    if (itr == m_OutdoorPvPMap.end())
        return;

    itr->second->HandlePlayerEnterMap(p_Player, p_MapID);
}

void OutdoorPvPMgr::HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(p_Player->GetZoneId());
    if (itr == m_OutdoorPvPMap.end())
        return;

    itr->second->HandlePlayerLeaveMap(p_Player, p_MapID);
}

void OutdoorPvPMgr::HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(p_Player->GetZoneId());
    if (itr == m_OutdoorPvPMap.end())
        return;

    itr->second->HandlePlayerEnterArea(p_Player, p_AreaID);
}

void OutdoorPvPMgr::HandlePlayerLeaveArea(Player* p_Player, uint32 p_AreaID)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(p_Player->GetZoneId());
    if (itr == m_OutdoorPvPMap.end())
        return;

    itr->second->HandlePlayerLeaveArea(p_Player, p_AreaID);
}

OutdoorPvP* OutdoorPvPMgr::GetOutdoorPvPToZoneId(uint32 zoneid)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(zoneid);
    if (itr == m_OutdoorPvPMap.end())
    {
        // no handle for this zone, return
        return NULL;
    }
    return itr->second;
}

void OutdoorPvPMgr::Update(uint32 diff)
{
    m_UpdateTimer += diff;
    if (m_UpdateTimer > OUTDOORPVP_OBJECTIVE_UPDATE_INTERVAL)
    {
        for (OutdoorPvPSet::iterator itr = m_OutdoorPvPSet.begin(); itr != m_OutdoorPvPSet.end(); ++itr)
            (*itr)->Update(m_UpdateTimer);
        m_UpdateTimer = 0;
    }
}

bool OutdoorPvPMgr::HandleCustomSpell(Player* player, uint32 spellId, GameObject* go)
{
    for (OutdoorPvPSet::iterator itr = m_OutdoorPvPSet.begin(); itr != m_OutdoorPvPSet.end(); ++itr)
    {
        if ((*itr)->HandleCustomSpell(player, spellId, go))
            return true;
    }
    return false;
}

ZoneScript* OutdoorPvPMgr::GetZoneScript(uint32 zoneId)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(zoneId);
    if (itr != m_OutdoorPvPMap.end())
        return itr->second;
    else
        return NULL;
}

bool OutdoorPvPMgr::HandleOpenGo(Player* player, uint64 guid)
{
    for (OutdoorPvPSet::iterator itr = m_OutdoorPvPSet.begin(); itr != m_OutdoorPvPSet.end(); ++itr)
    {
        if ((*itr)->HandleOpenGo(player, guid))
            return true;
    }
    return false;
}

void OutdoorPvPMgr::HandleGossipOption(Player* player, uint64 guid, uint32 gossipid)
{
    for (OutdoorPvPSet::iterator itr = m_OutdoorPvPSet.begin(); itr != m_OutdoorPvPSet.end(); ++itr)
    {
        if ((*itr)->HandleGossipOption(player, guid, gossipid))
            return;
    }
}

bool OutdoorPvPMgr::CanTalkTo(Player* player, Creature* creature, GossipMenuItems const& gso)
{
    for (OutdoorPvPSet::iterator itr = m_OutdoorPvPSet.begin(); itr != m_OutdoorPvPSet.end(); ++itr)
    {
        if ((*itr)->CanTalkTo(player, creature, gso))
            return true;
    }
    return false;
}

void OutdoorPvPMgr::HandleDropFlag(Player* player, uint32 spellId)
{
    for (OutdoorPvPSet::iterator itr = m_OutdoorPvPSet.begin(); itr != m_OutdoorPvPSet.end(); ++itr)
    {
        if ((*itr)->HandleDropFlag(player, spellId))
            return;
    }
}

void OutdoorPvPMgr::HandlePlayerResurrects(Player* player, uint32 zoneid)
{
    OutdoorPvPMap::iterator itr = m_OutdoorPvPMap.find(zoneid);
    if (itr == m_OutdoorPvPMap.end())
        return;

    if (itr->second->HasPlayer(player))
        itr->second->HandlePlayerResurrects(player, zoneid);
}
