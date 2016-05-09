////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "OutdoorPvP.h"
#include "OutdoorPvPMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Map.h"
#include "MapManager.h"
#include "Group.h"
#include "WorldPacket.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

OPvPCapturePoint::OPvPCapturePoint(OutdoorPvP* pvp) :
m_capturePointGUID(0), m_capturePoint(NULL), m_maxValue(0.0f), m_minValue(0.0f), m_maxSpeed(0),
m_value(0), m_team(TEAM_NEUTRAL), m_OldState(OBJECTIVESTATE_NEUTRAL),
m_State(OBJECTIVESTATE_NEUTRAL), m_neutralValuePct(0), m_PvP(pvp)
{
}

bool OPvPCapturePoint::HandlePlayerEnter(Player* player)
{
    if (m_capturePoint)
    {
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldState1, 1);
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate2, (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f));
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate3, m_neutralValuePct);
    }

    if (player->GetTeamId() < 2)
        return m_activePlayers[player->GetTeamId()].insert(player->GetGUID()).second;

    return false;
}

void OPvPCapturePoint::HandlePlayerLeave(Player* player)
{
    if (m_capturePoint)
        player->SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldState1, 0);

    if (player->GetTeamId() < 2)
        m_activePlayers[player->GetTeamId()].erase(player->GetGUID());
}

void OPvPCapturePoint::SendChangePhase()
{
    if (!m_capturePoint)
        return;

    // send this too, sometimes the slider disappears, dunno why :(
    SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldState1, 1);
    // send these updates to only the ones in this objective
    SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate2, (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f));
    // send this too, sometimes it resets :S
    SendUpdateWorldState(m_capturePoint->GetGOInfo()->controlZone.worldstate3, m_neutralValuePct);
}

void OPvPCapturePoint::AddGO(uint32 type, uint32 guid, uint32 entry)
{
    if (!entry)
    {
        const GameObjectData* data = sObjectMgr->GetGOData(guid);
        if (!data)
            return;
        entry = data->id;
    }
    m_Objects[type] = MAKE_NEW_GUID(guid, entry, HIGHGUID_GAMEOBJECT);
    m_ObjectTypes[m_Objects[type]]=type;
}

void OPvPCapturePoint::AddCre(uint32 type, uint32 guid, uint32 entry)
{
    if (!entry)
    {
        const CreatureData* data = sObjectMgr->GetCreatureData(guid);
        if (!data)
            return;
        entry = data->id;
    }
    m_Creatures[type] = MAKE_NEW_GUID(guid, entry, HIGHGUID_UNIT);
    m_CreatureTypes[m_Creatures[type]] = type;
}

bool OPvPCapturePoint::AddObject(uint32 type, uint32 entry, uint32 map, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3)
{
    uint32 l_Guid = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);
    if (sObjectMgr->AddGOData(l_Guid, entry, map, x, y, z, o, 0, rotation0, rotation1, rotation2, rotation3))
    {
        AddGO(type, l_Guid, entry);
        return true;
    }

    return false;
}

bool OPvPCapturePoint::AddCreature(uint32 type, uint32 entry, uint32 team, uint32 map, float x, float y, float z, float o, uint32 spawntimedelay)
{
    if (uint32 guid = sObjectMgr->AddCreData(entry, team, map, x, y, z, o, spawntimedelay))
    {
        AddCre(type, guid, entry);
        return true;
    }

    return false;
}

bool OPvPCapturePoint::SetCapturePointData(uint32 entry, uint32 map, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3)
{
    sLog->outDebug(LOG_FILTER_OUTDOORPVP, "Creating capture point %u", entry);

    // check info existence
    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(entry);
    if (!goinfo || goinfo->type != GAMEOBJECT_TYPE_CONTROL_ZONE)
    {
        sLog->outError(LOG_FILTER_OUTDOORPVP, "OutdoorPvP: GO %u is not capture point!", entry);
        return false;
    }

    m_capturePointGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, HIGHGUID_GAMEOBJECT);
    if (!sObjectMgr->AddGOData(m_capturePointGUID, entry, map, x, y, z, o, 0, rotation0, rotation1, rotation2, rotation3))
        return false;

    // get the needed values from goinfo
    m_maxValue = (float)goinfo->controlZone.maxTime;
    m_maxSpeed = m_maxValue / (goinfo->controlZone.minTime ? goinfo->controlZone.minTime : 60);
    m_neutralValuePct = goinfo->controlZone.neutralPercent;
    m_minValue = CalculatePct(m_maxValue, m_neutralValuePct);

    return true;
}

bool OPvPCapturePoint::DelCreature(uint32 type)
{
    if (!m_Creatures[type])
    {
        sLog->outDebug(LOG_FILTER_OUTDOORPVP, "opvp creature type %u was already deleted", type);
        return false;
    }

    Creature* cr = HashMapHolder<Creature>::Find(m_Creatures[type]);
    if (!cr)
    {
        // can happen when closing the core
        m_Creatures[type] = 0;
        return false;
    }
    sLog->outDebug(LOG_FILTER_OUTDOORPVP, "deleting opvp creature type %u", type);
    uint32 guid = cr->GetDBTableGUIDLow();
    // Don't save respawn time
    cr->SetRespawnTime(0);
    cr->RemoveCorpse();
    // explicit removal from map
    // beats me why this is needed, but with the recent removal "cleanup" some creatures stay in the map if "properly" deleted
    // so this is a big fat workaround, if AddObjectToRemoveList and DoDelayedMovesAndRemoves worked correctly, this wouldn't be needed
    //if (Map* map = sMapMgr->FindMap(cr->GetMapId()))
    //    map->Remove(cr, false);
    // delete respawn time for this creature
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN);
    stmt->setUInt32(0, guid);
    stmt->setUInt16(1, cr->GetMapId());
    stmt->setUInt32(2, 0);  // instance id, always 0 for world maps
    CharacterDatabase.Execute(stmt);

    cr->AddObjectToRemoveList();
    sObjectMgr->DeleteCreatureData(guid);
    m_CreatureTypes[m_Creatures[type]] = 0;
    m_Creatures[type] = 0;
    return true;
}

bool OPvPCapturePoint::DelObject(uint32 type)
{
    if (!m_Objects[type])
        return false;

    GameObject* obj = HashMapHolder<GameObject>::Find(m_Objects[type]);
    if (!obj)
    {
        m_Objects[type] = 0;
        return false;
    }
    uint32 guid = obj->GetDBTableGUIDLow();
    obj->SetRespawnTime(0);                                 // not save respawn time
    obj->Delete();
    sObjectMgr->DeleteGOData(guid);
    m_ObjectTypes[m_Objects[type]] = 0;
    m_Objects[type] = 0;
    return true;
}

bool OPvPCapturePoint::DelCapturePoint()
{
    m_capturePointGUID = 0;

    if (m_capturePoint)
    {
        m_capturePoint->SetRespawnTime(0);                                 // not save respawn time
        m_capturePoint->Delete();
        m_capturePoint = nullptr;
    }

    sObjectMgr->DeleteGOData(GUID_LOPART(m_capturePointGUID));

    return true;
}

void OPvPCapturePoint::DeleteSpawns()
{
    for (std::map<uint32, uint64>::iterator i = m_Objects.begin(); i != m_Objects.end(); ++i)
        DelObject(i->first);
    for (std::map<uint32, uint64>::iterator i = m_Creatures.begin(); i != m_Creatures.end(); ++i)
        DelCreature(i->first);
    DelCapturePoint();
}

void OutdoorPvP::DeleteSpawns()
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
    {
        itr->second->DeleteSpawns();
        delete itr->second;
    }
    m_capturePoints.clear();
}

OutdoorPvP::OutdoorPvP() : m_sendUpdate(true), m_LastResurectTimer(0)
{
}

OutdoorPvP::~OutdoorPvP()
{
    DeleteSpawns();
}

void OutdoorPvP::HandlePlayerEnterZone(Player* p_Player, uint32 /*p_Zone*/)
{
    if (p_Player && p_Player->GetTeamId() < 2)
        m_Players[p_Player->GetTeamId()].insert(p_Player->GetGUID());
}

void OutdoorPvP::HandlePlayerLeaveZone(Player* p_Player, uint32 /*zone*/)
{
    /// Inform the objectives of the leaving
    for (OPvPCapturePointMap::iterator l_Iter = m_capturePoints.begin(); l_Iter != m_capturePoints.end(); ++l_Iter)
        l_Iter->second->HandlePlayerLeave(p_Player);

    /// Remove the world state information from the player (we can't keep everyone up to date, so leave out those who are not in the concerning zones)
    if (!p_Player->GetSession()->PlayerLogout())
        SendRemoveWorldStates(p_Player);

    if (p_Player->GetTeamId() < 2)
        m_Players[p_Player->GetTeamId()].erase(p_Player->GetGUID());
}

bool OutdoorPvP::Update(uint32 p_Diff)
{
    bool l_ObjectiveChanged = false;

    for (OPvPCapturePointMap::iterator l_Iter = m_capturePoints.begin(); l_Iter != m_capturePoints.end(); ++l_Iter)
    {
        if (l_Iter->second->Update(p_Diff))
            l_ObjectiveChanged = true;
    }

    if (m_LastResurectTimer <= p_Diff)
    {
        for (uint8 l_I = 0; l_I < m_GraveyardList.size(); ++l_I)
        {
            if (GetGraveyardById(l_I))
                m_GraveyardList[l_I]->Resurrect();
        }

        m_LastResurectTimer = BattlegroundTimeIntervals::RESURRECTION_INTERVAL;
    }
    else
        m_LastResurectTimer -= p_Diff;

    return l_ObjectiveChanged;
}

bool OPvPCapturePoint::Update(uint32 diff)
{
    if (!m_capturePoint)
        return false;

    float radius = (float)m_capturePoint->GetGOInfo()->controlZone.radius;

    for (uint32 team = 0; team < 2; ++team)
    {
        for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end();)
        {
            Player* player = sObjectAccessor->FindPlayer(*itr);
            ++itr;

            if (player)
            {
                if (!m_capturePoint->IsWithinDistInMap(player, radius) || !player->IsOutdoorPvPActive())
                    HandlePlayerLeave(player);
            }
        }
    }

    std::list<Player*> players;
    JadeCore::AnyPlayerInObjectRangeCheck checker(m_capturePoint, radius);
    JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(m_capturePoint, players, checker);
    m_capturePoint->VisitNearbyWorldObject(radius, searcher);

    for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        if ((*itr)->IsOutdoorPvPActive() && (*itr)->GetTeamId() < 2)
        {
            if (m_activePlayers[(*itr)->GetTeamId()].insert((*itr)->GetGUID()).second)
                HandlePlayerEnter(*itr);
        }
    }

    // get the difference of numbers
    float fact_diff = ((float)m_activePlayers[0].size() - (float)m_activePlayers[1].size()) * diff / OUTDOORPVP_OBJECTIVE_UPDATE_INTERVAL;
    if (!fact_diff)
        return false;

    uint32 Challenger = 0;
    float maxDiff = m_maxSpeed * diff;

    if (fact_diff < 0)
    {
        // horde is in majority, but it's already horde-controlled -> no change
        if (m_State == OBJECTIVESTATE_HORDE && m_value <= -m_maxValue)
            return false;

        if (fact_diff < -maxDiff)
            fact_diff = -maxDiff;

        Challenger = HORDE;
    }
    else
    {
        // ally is in majority, but it's already ally-controlled -> no change
        if (m_State == OBJECTIVESTATE_ALLIANCE && m_value >= m_maxValue)
            return false;

        if (fact_diff > maxDiff)
            fact_diff = maxDiff;

        Challenger = ALLIANCE;
    }

    float oldValue = m_value;
    TeamId oldTeam = m_team;

    m_OldState = m_State;

    m_value += fact_diff;

    if (m_value < -m_minValue) // red
    {
        if (m_value < -m_maxValue)
            m_value = -m_maxValue;
        m_State = OBJECTIVESTATE_HORDE;
        m_team = TEAM_HORDE;
    }
    else if (m_value > m_minValue) // blue
    {
        if (m_value > m_maxValue)
            m_value = m_maxValue;
        m_State = OBJECTIVESTATE_ALLIANCE;
        m_team = TEAM_ALLIANCE;
    }
    else if (oldValue * m_value <= 0) // grey, go through mid point
    {
        // if challenger is ally, then n->a challenge
        if (Challenger == ALLIANCE)
            m_State = OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE;
        // if challenger is horde, then n->h challenge
        else if (Challenger == HORDE)
            m_State = OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }
    else // grey, did not go through mid point
    {
        // old phase and current are on the same side, so one team challenges the other
        if (Challenger == ALLIANCE && (m_OldState == OBJECTIVESTATE_HORDE || m_OldState == OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE))
            m_State = OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE;
        else if (Challenger == HORDE && (m_OldState == OBJECTIVESTATE_ALLIANCE || m_OldState == OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE))
            m_State = OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }

    if (m_value != oldValue)
        SendChangePhase();

    if (m_OldState != m_State)
    {
        //sLog->outError(LOG_FILTER_OUTDOORPVP, "%u->%u", m_OldState, m_State);
        if (oldTeam != m_team)
            ChangeTeam(oldTeam);
        ChangeState();
        return true;
    }

    return false;
}

void OutdoorPvP::SendUpdateWorldState(uint32 field, uint32 value)
{
    if (m_sendUpdate)
    {
        for (uint8 l_I = 0; l_I < 2; ++l_I)
        {
            for (uint64 l_Guid : m_Players[l_I])
            {
                if (Player* l_Player = HashMapHolder<Player>::Find(l_Guid))
                    l_Player->SendUpdateWorldState(field, value);
            }
        }
    }
}

void OPvPCapturePoint::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (uint32 team = 0; team < 2; ++team)
    {
        // send to all players present in the area
        for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr)
        {
            if (Player* player = sObjectAccessor->FindPlayer((*itr)))
                player->SendUpdateWorldState(field, value);
        }
    }
}

void OPvPCapturePoint::SendObjectiveComplete(uint32 id, uint64 guid)
{
    uint32 team;
    switch (m_State)
    {
    case OBJECTIVESTATE_ALLIANCE:
        team = 0;
        break;
    case OBJECTIVESTATE_HORDE:
        team = 1;
        break;
    default:
        return;
    }

    // send to all players present in the area
    for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr)
    {
        if (Player* player = sObjectAccessor->FindPlayer((*itr)))
            player->KilledMonsterCredit(id, guid);
    }
}

void OutdoorPvP::HandleKill(Player* killer, Unit* killed)
{
    if (Group* group = killer->GetGroup())
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* groupGuy = itr->getSource();

            if (!groupGuy)
                continue;

            // skip if too far away
            if (!groupGuy->IsAtGroupRewardDistance(killed))
                continue;

            // creature kills must be notified, even if not inside objective / not outdoor pvp active
            // player kills only count if active and inside objective
            if ((groupGuy->IsOutdoorPvPActive() && IsInsideObjective(groupGuy)) || killed->GetTypeId() == TYPEID_UNIT)
            {
                HandleKillImpl(groupGuy, killed);
            }
        }
    }
    else
    {
        // creature kills must be notified, even if not inside objective / not outdoor pvp active
        if (killer && ((killer->IsOutdoorPvPActive() && IsInsideObjective(killer)) || killed->GetTypeId() == TYPEID_UNIT))
        {
            HandleKillImpl(killer, killed);
        }
    }
}

bool OutdoorPvP::IsInsideObjective(Player* player) const
{
    for (OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->IsInsideObjective(player))
            return true;

    return false;
}

bool OPvPCapturePoint::IsInsideObjective(Player* player) const
{
    if (player && player->GetTeamId() < 2)
        return m_activePlayers[player->GetTeamId()].find(player->GetGUID()) != m_activePlayers[player->GetTeamId()].end();

    return false;
}

bool OutdoorPvP::HandleCustomSpell(Player* player, uint32 spellId, GameObject* go)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleCustomSpell(player, spellId, go))
            return true;

    return false;
}

bool OPvPCapturePoint::HandleCustomSpell(Player* player, uint32 /*spellId*/, GameObject* /*go*/)
{
    if (!player->IsOutdoorPvPActive())
        return false;
    return false;
}

bool OutdoorPvP::HandleOpenGo(Player* player, uint64 guid)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleOpenGo(player, guid) >= 0)
            return true;

    return false;
}

bool OutdoorPvP::HandleGossipOption(Player* player, uint64 guid, uint32 id)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleGossipOption(player, guid, id))
            return true;

    return false;
}

bool OutdoorPvP::CanTalkTo(Player* player, Creature* c, GossipMenuItems const& gso)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->CanTalkTo(player, c, gso))
            return true;

    return false;
}

bool OutdoorPvP::HandleDropFlag(Player* player, uint32 id)
{
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (itr->second->HandleDropFlag(player, id))
            return true;

    return false;
}

bool OPvPCapturePoint::HandleGossipOption(Player* /*player*/, uint64 /*guid*/, uint32 /*id*/)
{
    return false;
}

bool OPvPCapturePoint::CanTalkTo(Player* /*player*/, Creature* /*c*/, GossipMenuItems const& /*gso*/)
{
    return false;
}

bool OPvPCapturePoint::HandleDropFlag(Player* /*player*/, uint32 /*id*/)
{
    return false;
}

int32 OPvPCapturePoint::HandleOpenGo(Player* /*player*/, uint64 guid)
{
    std::map<uint64, uint32>::iterator itr = m_ObjectTypes.find(guid);
    if (itr != m_ObjectTypes.end())
    {
        return itr->second;
    }
    return -1;
}

bool OutdoorPvP::HandleAreaTrigger(Player* /*player*/, uint32 /*trigger*/)
{
    return false;
}

void OutdoorPvP::BroadcastPacket(WorldPacket& p_Data) const
{
    /// This is faster than sWorld->SendZoneMessage
    for (uint32 l_TeamID = 0; l_TeamID < 2; ++l_TeamID)
    {
        for (uint64 l_Guid : m_Players[l_TeamID])
        {
            if (Player* l_Player = HashMapHolder<Player>::Find(l_Guid))
                l_Player->GetSession()->SendPacket(&p_Data);
        }
    }
}

void OutdoorPvP::RegisterZone(uint32 zoneId)
{
    sOutdoorPvPMgr->AddZone(zoneId, this);
}

bool OutdoorPvP::HasPlayer(Player* p_Player) const
{
    if (p_Player && p_Player->GetTeamId() < 2)
        return m_Players[p_Player->GetTeamId()].find(p_Player->GetGUID()) != m_Players[p_Player->GetTeamId()].end();

    return false;
}

void OutdoorPvP::TeamCastSpell(TeamId p_TeamID, int32 p_SpellID)
{
    if (p_SpellID > 0)
    {
        for (uint64 l_Guid : m_Players[p_TeamID])
        {
            if (Player* l_Player = HashMapHolder<Player>::Find(l_Guid))
                l_Player->CastSpell(l_Player, (uint32)p_SpellID, true);
        }
    }
    else
    {
        for (uint64 l_Guid : m_Players[p_TeamID])
        {
            if (Player* l_Player = HashMapHolder<Player>::Find(l_Guid))
                l_Player->RemoveAura((uint32)-p_SpellID); ///< By stack?
        }
    }
}

void OutdoorPvP::AddAreaTrigger(uint32 p_Entry, uint32 p_PhaseMask, uint32 p_SpellVisualID, Position const& p_Pos, uint32 p_Duration, Map* p_Map)
{
    AreaTrigger* l_AreaTrigger = new AreaTrigger;
    if (!l_AreaTrigger->CreateAreaTrigger(p_Entry, sObjectMgr->GenerateLowGuid(HIGHGUID_AREATRIGGER), p_PhaseMask, p_SpellVisualID, p_Pos, p_Duration, p_Map))
        delete l_AreaTrigger;
}

bool OutdoorPvP::AddCreature(uint32 p_Type, uint32 p_Entry, uint32 p_Team, uint32 p_MapID, float p_X, float p_Y, float p_Z, float p_O, uint32 p_SpawnTime /*= 0*/)
{
    if (uint32 l_Guid = sObjectMgr->AddCreData(p_Entry, p_Team, p_MapID, p_X, p_Y, p_Z, p_O, p_SpawnTime))
    {
        if (!p_Entry)
        {
            CreatureData const* l_Data = sObjectMgr->GetCreatureData(l_Guid);
            if (!l_Data)
                return false;

            p_Entry = l_Data->id;
        }

        CreatureTemplate const* l_Template = sObjectMgr->GetCreatureTemplate(p_Entry);
        if (l_Template == nullptr)
            return false;

        m_Creatures[p_Type] = MAKE_NEW_GUID(l_Guid, p_Entry, l_Template->VehicleId ? HighGuid::HIGHGUID_VEHICLE : HighGuid::HIGHGUID_UNIT);
        m_CreatureTypes[m_Creatures[p_Type]] = p_Type;
        return true;
    }

    return false;
}

bool OutdoorPvP::DelCreature(uint32 p_Type)
{
    if (!m_Creatures[p_Type])
    {
        sLog->outDebug(LOG_FILTER_OUTDOORPVP, "OutdoorPvP::DelCreature, creature type %u was already deleted", p_Type);
        return false;
    }

    Creature* l_Creature = HashMapHolder<Creature>::Find(m_Creatures[p_Type]);
    if (!l_Creature)
    {
        // Can happen when closing the core
        m_Creatures[p_Type] = 0;
        return false;
    }

    sLog->outDebug(LOG_FILTER_OUTDOORPVP, "OutdoorPvP::DelCreature, deleting creature type %u", p_Type);

    uint32 l_Guid = l_Creature->GetDBTableGUIDLow();

    // Don't save respawn time
    l_Creature->SetRespawnTime(0);
    l_Creature->RemoveCorpse();

    // Delete respawn time for this creature
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN);
    l_Statement->setUInt32(0, l_Guid);
    l_Statement->setUInt16(1, l_Creature->GetMapId());
    l_Statement->setUInt32(2, 0);  // InstanceID, always 0 for world maps
    CharacterDatabase.Execute(l_Statement);

    l_Creature->AddObjectToRemoveList();
    sObjectMgr->DeleteCreatureData(l_Guid);
    m_CreatureTypes[m_Creatures[p_Type]] = 0;
    m_Creatures[p_Type] = 0;
    return true;
}

bool OutdoorPvP::AddObject(uint32 p_Type, uint32 p_Entry, uint32 p_Map, float p_X, float p_Y, float p_Z, float p_O, float p_Rot0, float p_Rot1, float p_Rot2, float p_Rot3)
{
    uint32 l_Guid = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);
    if (sObjectMgr->AddGOData(l_Guid, p_Entry, p_Map, p_X, p_Y, p_Z, p_O, 0, p_Rot0, p_Rot1, p_Rot2, p_Rot3))
    {
        m_Objects[p_Type] = MAKE_NEW_GUID(l_Guid, p_Entry, HIGHGUID_GAMEOBJECT);
        m_ObjectTypes[m_Objects[p_Type]] = p_Type;
        return true;
    }

    return false;
}

bool OutdoorPvP::DelObject(uint32 p_Type)
{
    if (!m_Objects[p_Type])
        return false;

    GameObject* l_GameObject = HashMapHolder<GameObject>::Find(m_Objects[p_Type]);
    if (!l_GameObject)
    {
        /// Can happen when closing the core
        m_Objects[p_Type] = 0;
        return false;
    }

    uint32 l_Guid = l_GameObject->GetDBTableGUIDLow();

    /// Don't save respawn time
    l_GameObject->SetRespawnTime(0);

    /// Delete respawn time for this creature
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GO_RESPAWN);
    l_Statement->setUInt32(0, l_Guid);
    l_Statement->setUInt16(1, l_GameObject->GetMapId());
    l_Statement->setUInt32(2, 0);  ///< InstanceID, always 0 for world maps
    CharacterDatabase.Execute(l_Statement);

    l_GameObject->AddObjectToRemoveList();
    sObjectMgr->DeleteGOData(l_Guid);
    m_ObjectTypes[m_Objects[p_Type]] = 0;
    m_Objects[p_Type] = 0;
    return true;
}

void OutdoorPvP::TeamApplyBuff(TeamId team, uint32 spellId, uint32 spellId2)
{
    TeamCastSpell(team, spellId);
    TeamCastSpell(OTHER_TEAM(team), spellId2 ? -(int32)spellId2 : -(int32)spellId);
}

void OutdoorPvP::OnGameObjectCreate(GameObject* go)
{
    if (go->GetGoType() != GAMEOBJECT_TYPE_CONTROL_ZONE)
        return;

    if (OPvPCapturePoint *cp = GetCapturePoint(go->GetGUID()))
        cp->m_capturePoint = go;
}

void OutdoorPvP::OnGameObjectRemove(GameObject* p_Go)
{
    if (p_Go->GetGoType() != GAMEOBJECT_TYPE_CONTROL_ZONE)
        return;

    if (OPvPCapturePoint *cp = GetCapturePoint(p_Go->GetGUID()))
        cp->m_capturePoint = NULL;

    ZoneScript::OnGameObjectRemove(p_Go);
}

WorldSafeLocsEntry const* OutdoorPvP::GetClosestGraveyard(Player* p_Player)
{
    OutdoorGraveyard* l_ClosestGraveyard = nullptr;
    float l_MaxDist = 1000000.0f;

    for (uint8 l_I = 0; l_I < m_GraveyardList.size(); l_I++)
    {
        if (m_GraveyardList[l_I])
        {
            if (m_GraveyardList[l_I]->GetControlTeamId() != p_Player->GetTeamId())
                continue;

            float l_Dist = m_GraveyardList[l_I]->GetDistance(p_Player);
            if (l_Dist < l_MaxDist || l_MaxDist < 0)
            {
                l_ClosestGraveyard = m_GraveyardList[l_I];
                l_MaxDist = l_Dist;
            }
        }
    }

    if (l_ClosestGraveyard)
        return sWorldSafeLocsStore.LookupEntry(l_ClosestGraveyard->GetGraveyardId());

    return nullptr;
}

void OutdoorPvP::SendAreaSpiritHealerQueryOpcode(Player* p_Player, uint64 const& p_Guid)
{
    ASSERT(p_Player && p_Player->GetSession());

    WorldPacket l_Data(Opcodes::SMSG_AREA_SPIRIT_HEALER_TIME, 16 + 2 + 4);
    l_Data.appendPackGUID(p_Guid);
    l_Data << uint32(m_LastResurectTimer);
    p_Player->GetSession()->SendPacket(&l_Data);
}

void OutdoorPvP::AddPlayerToResurrectQueue(uint64 p_SpiritGuid, uint64 p_PlayerGuid)
{
    for (uint8 l_I = 0; l_I < m_GraveyardList.size(); l_I++)
    {
        if (m_GraveyardList[l_I] == nullptr)
            continue;

        if (m_GraveyardList[l_I]->HasNpc(p_SpiritGuid))
        {
            m_GraveyardList[l_I]->AddPlayer(p_PlayerGuid);
            break;
        }
    }
}

OutdoorGraveyard* OutdoorPvP::GetGraveyardById(uint32 p_ID)
{
    if (p_ID < m_GraveyardList.size())
    {
        if (m_GraveyardList[p_ID])
            return m_GraveyardList[p_ID];
        else
            sLog->outError(LOG_FILTER_BATTLEFIELD, "OutdoorPvP::GetGraveyardById Id:%u not existed", p_ID);
    }
    else
        sLog->outError(LOG_FILTER_BATTLEFIELD, "OutdoorPvP::GetGraveyardById Id:%u cant be found", p_ID);

    return nullptr;
}

uint64 OutdoorPvP::GetCreature(uint32 p_Type)
{
    if (m_Creatures.find(p_Type) == m_Creatures.end())
        return 0;

    return m_Creatures[p_Type];
}

OutdoorGraveyard::OutdoorGraveyard(OutdoorPvP* p_OutdoorPvP)
{
    m_OutdoorPvP = p_OutdoorPvP;
    m_GraveyardId = 0;
    m_ControlTeam = TeamId::TEAM_NEUTRAL;
    m_SpiritGuide[0] = 0;
    m_SpiritGuide[1] = 0;
    m_ResurrectQueue.clear();
}

void OutdoorGraveyard::Initialize(TeamId p_Team, uint32 p_Graveyard)
{
    m_ControlTeam = p_Team;
    m_GraveyardId = p_Graveyard;
}

void OutdoorGraveyard::SetSpirit(Creature* p_Spirit, TeamId p_Team)
{
    if (p_Spirit == nullptr)
    {
        sLog->outError(LOG_FILTER_BATTLEFIELD, "OutdoorGraveyard::SetSpirit -> Invalid Spirit.");
        return;
    }

    m_SpiritGuide[p_Team] = p_Spirit->GetGUID();
    p_Spirit->SetReactState(ReactStates::REACT_PASSIVE);
}

float OutdoorGraveyard::GetDistance(Player* p_Player) const
{
    WorldSafeLocsEntry const* l_SafeLoc = sWorldSafeLocsStore.LookupEntry(m_GraveyardId);
    return p_Player->GetDistance2d(l_SafeLoc->x, l_SafeLoc->y);
}

void OutdoorGraveyard::AddPlayer(uint64 p_Guid)
{
    if (!m_ResurrectQueue.count(p_Guid))
    {
        m_ResurrectQueue.insert(p_Guid);

        if (Player* l_Player = sObjectAccessor->FindPlayer(p_Guid))
            l_Player->CastSpell(l_Player, BattlegroundSpells::SPELL_WAITING_FOR_RESURRECT, true);
    }
}

void OutdoorGraveyard::RemovePlayer(uint64 p_Guid)
{
    m_ResurrectQueue.erase(m_ResurrectQueue.find(p_Guid));

    if (Player* l_Player = sObjectAccessor->FindPlayer(p_Guid))
        l_Player->RemoveAurasDueToSpell(BattlegroundSpells::SPELL_WAITING_FOR_RESURRECT);
}

void OutdoorGraveyard::Resurrect()
{
    if (m_ResurrectQueue.empty())
        return;

    for (GuidSet::const_iterator l_Iter = m_ResurrectQueue.begin(); l_Iter != m_ResurrectQueue.end(); ++l_Iter)
    {
        /// Get player object from his guid
        Player* l_Player = sObjectAccessor->FindPlayer(*l_Iter);
        if (!l_Player)
            continue;

        /// Check  if the player is in world and on the good graveyard
        if (l_Player->IsInWorld())
        {
            if (Unit* l_Spirit = sObjectAccessor->FindUnit(m_SpiritGuide[m_ControlTeam]))
                l_Spirit->CastSpell(l_Spirit, BattlegroundSpells::SPELL_SPIRIT_HEAL, true);
        }

        /// Resurrect player
        l_Player->CastSpell(l_Player, BattlegroundSpells::SPELL_RESURRECTION_VISUAL, true);
        l_Player->ResurrectPlayer(1.0f);
        l_Player->CastSpell(l_Player, BattlegroundSpells::SPELL_PET_SUMMONED, true);
        l_Player->CastSpell(l_Player, BattlegroundSpells::SPELL_SPIRIT_HEAL_MANA, true);

        sObjectAccessor->ConvertCorpseForPlayer(l_Player->GetGUID());
    }

    m_ResurrectQueue.clear();
}

void OutdoorGraveyard::GiveControlTo(TeamId p_Team)
{
    m_ControlTeam = p_Team;
    RelocateDeadPlayers();
}

void OutdoorGraveyard::RelocateDeadPlayers()
{
    WorldSafeLocsEntry const* l_ClosestGrave = nullptr;
    for (GuidSet::const_iterator l_Iter = m_ResurrectQueue.begin(); l_Iter != m_ResurrectQueue.end(); ++l_Iter)
    {
        Player* l_Player = sObjectAccessor->FindPlayer(*l_Iter);
        if (!l_Player)
            continue;

        if (l_ClosestGrave)
            l_Player->TeleportTo(l_Player->GetMapId(), l_ClosestGrave->x, l_ClosestGrave->y, l_ClosestGrave->z, l_Player->GetOrientation());
        else
        {
            l_ClosestGrave = m_OutdoorPvP->GetClosestGraveyard(l_Player);
            if (l_ClosestGrave)
                l_Player->TeleportTo(l_Player->GetMapId(), l_ClosestGrave->x, l_ClosestGrave->y, l_ClosestGrave->z, l_Player->GetOrientation());
        }
    }
}
