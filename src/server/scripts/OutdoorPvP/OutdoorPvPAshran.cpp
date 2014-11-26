/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "OutdoorPvPAshran.h"
#include "ScriptPCH.h"

OPvPCapturePoint_Middle::OPvPCapturePoint_Middle(OutdoorPvP* p_Outdoor, eAshranBattleType p_Type, uint8 p_Faction)
    : OPvPCapturePoint(p_Outdoor), m_BattleType(p_Type), m_BattleFaction(p_Faction)
{
    SetCapturePointData(g_CapturePoint[p_Type]);
    AddCreature(ASHRAN_GENERIC_MOB_TYPEID + p_Type, SLG_GENERIC_MOP_LARGE_AOI, TEAM_NONE, ASHRAN_MAP_ID, g_CapturePoint[p_Type].x, g_CapturePoint[p_Type].y, g_CapturePoint[p_Type].z, M_PI);
    ((OutdoorPvPAshran*)m_PvP)->AddGenericMoPGuid(p_Type, m_Creatures[ASHRAN_GENERIC_MOB_TYPEID + p_Type]);
}

void OPvPCapturePoint_Middle::ChangeState()
{
    uint32 l_UpdateVal = 0;
    switch (m_State)
    {
        case OBJECTIVESTATE_ALLIANCE:
            m_BattleFaction = CONTROL_ALLIANCE;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = FLAG_ALLIANCE;
            SendUpdateWorldState(WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR, WORLD_STATE_DISABLED);
            break;
        case OBJECTIVESTATE_HORDE:
            m_BattleFaction = CONTROL_HORDE;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = FLAG_HORDE;
            SendUpdateWorldState(WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR, WORLD_STATE_DISABLED);
            break;
        case OBJECTIVESTATE_NEUTRAL:
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
            m_BattleFaction = CONTROL_NEUTRAL;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = FLAG_NEUTRAL;
            break;
        default:
            break;
    }

    GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID);
    if (l_Flag)
        l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, l_UpdateVal);

    UpdateTowerState();
}

void OPvPCapturePoint_Middle::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << g_TowerControlStatus[GetBattleType()] << GetBattleFaction();
}

void OPvPCapturePoint_Middle::UpdateTowerState()
{
    m_PvP->SendUpdateWorldState(g_TowerControlStatus[GetBattleType()], GetBattleFaction());
}

bool OPvPCapturePoint_Middle::Update(uint32 p_Diff)
{
    if (m_BattleFaction != CONTROL_NEUTRAL)
        return false;

    return OPvPCapturePoint::Update(p_Diff);
}

void OPvPCapturePoint_Middle::SpawnFactionGuards(eAshranBattleType p_BattleID, uint8 p_Faction)
{
    if (p_Faction > CONTROL_ALLIANCE || p_BattleID >= BATTLE_TYPE_MAX)
        return;

    bool l_IsInitialized = ((OutdoorPvPAshran*)m_PvP)->IsInitialized();
    if (l_IsInitialized && p_Faction != CONTROL_NEUTRAL)
    {
        if (Creature* l_GenericMoP = sObjectAccessor->FindCreature(((OutdoorPvPAshran*)m_PvP)->GetGenericMoPGuid(p_BattleID)))
            l_GenericMoP->AI()->DoAction(p_Faction == CONTROL_HORDE ? ACTION_ANNOUNCE_HORDE_VICTORY : ACTION_ANNOUNCE_ALLIANCE_VICTORY);
    }

    switch (p_BattleID)
    {
        case BATTLE_TYPE_EMBERFALL_TOWER:
        {
            if (p_Faction == CONTROL_NEUTRAL)
            {
                for (uint8 l_Count = 0; l_Count < EMBERFALL_TOWER_CREATURES_COUNT; ++l_Count)
                    DelCreature(l_Count);

                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, FLAG_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == CONTROL_HORDE ? FLAG_HORDE : FLAG_ALLIANCE);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(ASHRAN_TIME_FOR_BATTLE);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == CONTROL_ALLIANCE ? WORLD_STATE_HIGH_WARLORD_VOLRATH : WORLD_STATE_VOLRATHS_ADVANCE_BATTLE);
                }

                // Spawn horde or alliance guards
                for (uint8 l_Count = 0; l_Count < EMBERFALL_TOWER_CREATURES_COUNT; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_EmberfallTowerSpawns[p_Faction == CONTROL_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE][l_Count]);
                }

                // Spawn fires
                for (uint8 l_Count = EMBERFALL_TOWER_CREATURES_COUNT; l_Count < EMBERFALL_TOWER_SPAWNS_IDS; ++l_Count)
                {
                    DelObject(l_Count);

                    if (p_Faction == CONTROL_ALLIANCE)
                        AddObject(l_Count, g_EmberfallFiresSpawns[l_Count - EMBERFALL_TOWER_CREATURES_COUNT]);
                }
            }

            break;
        }
        case BATTLE_TYPE_VOLRATHS_ADVANCE:
        {
            if (p_Faction == CONTROL_NEUTRAL)
            {
                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, FLAG_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == CONTROL_HORDE ? FLAG_HORDE : FLAG_ALLIANCE);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(ASHRAN_TIME_FOR_BATTLE);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == CONTROL_ALLIANCE ? WORLD_STATE_EMBERFALL_TOWER_BATTLE : WORLD_STATE_THE_CROSSROADS_BATTLE);
                }

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = EMBERFALL_TOWER_SPAWNS_IDS + VOLRATHS_ADVANCE_CREATURES_COUNT;
                for (uint8 l_Count = EMBERFALL_TOWER_SPAWNS_IDS; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_TrembladesVanguardSpawns[p_Faction == CONTROL_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - EMBERFALL_TOWER_SPAWNS_IDS]);
                }

                // Spawn fires
                uint8 l_FireIndex = EMBERFALL_TOWER_SPAWNS_IDS + VOLRATHS_ADVANCE_CREATURES_COUNT;
                for (uint8 l_Index = l_FireIndex; l_Index < VOLRATHS_ADVANCE_SPAWNS_IDS; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == CONTROL_ALLIANCE)
                        AddObject(l_Index, g_TrembladesVanguardFires[l_Index - l_FireIndex]);
                }
            }

            break;
        }
        case BATTLE_TYPE_THE_CROSSROADS:
        {
            if (p_Faction == CONTROL_NEUTRAL)
            {
                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, FLAG_NEUTRAL);

                for (uint8 l_Count = VOLRATHS_ADVANCE_SPAWNS_IDS; l_Count < THE_CROSSROADS_SPAWNS_IDS; ++l_Count)
                    DelObject(l_Count);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == CONTROL_HORDE ? FLAG_HORDE : FLAG_ALLIANCE);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(ASHRAN_TIME_FOR_BATTLE);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == CONTROL_ALLIANCE ? WORLD_STATE_VOLRATHS_ADVANCE_BATTLE : WORLD_STATE_TREMBLADES_VANGUARD_BATTLE);
                }

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = VOLRATHS_ADVANCE_SPAWNS_IDS + THE_CROSSROADS_CREATURES_COUNT;
                for (uint8 l_Count = VOLRATHS_ADVANCE_SPAWNS_IDS; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_TrembladesVanguardSpawns[p_Faction == CONTROL_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - VOLRATHS_ADVANCE_SPAWNS_IDS]);
                }

                // Spawn flags
                uint8 l_FlagIndex = VOLRATHS_ADVANCE_SPAWNS_IDS + THE_CROSSROADS_CREATURES_COUNT;
                for (uint8 l_Count = l_FlagIndex; l_Count < THE_CROSSROADS_SPAWNS_IDS; ++l_Count)
                {
                    DelObject(l_Count);
                    AddObject(l_Count, g_CrossroadsBanners[p_Faction == CONTROL_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - l_FlagIndex]);
                }
            }

            break;
        }
        case BATTLE_TYPE_TREMBLADES_VANGUARD:
        {
            if (p_Faction == CONTROL_NEUTRAL)
            {
                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, FLAG_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == CONTROL_HORDE ? FLAG_HORDE : FLAG_ALLIANCE);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(ASHRAN_TIME_FOR_BATTLE);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == CONTROL_ALLIANCE ? WORLD_STATE_THE_CROSSROADS_BATTLE : WORLD_STATE_ARCHMAGE_OVERWATCH_BATTLE);
                }

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = THE_CROSSROADS_SPAWNS_IDS + TREMBLADES_VANGUARD_CREATURES_COUNT;
                for (uint8 l_Count = THE_CROSSROADS_SPAWNS_IDS; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_TrembladesVanguardSpawns[p_Faction == CONTROL_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - THE_CROSSROADS_SPAWNS_IDS]);
                }

                uint8 l_FireIndex = THE_CROSSROADS_SPAWNS_IDS + TREMBLADES_VANGUARD_CREATURES_COUNT;
                // Spawn fires
                for (uint8 l_Index = l_FireIndex; l_Index < ARCHMAGE_OVERWATCH_SPAWNS_IDS; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == CONTROL_HORDE)
                        AddObject(l_Index, g_TrembladesVanguardFires[l_Index - l_FireIndex]);
                }
            }

            break;
        }
        case BATTLE_TYPE_ARCHMAGE_OVERWATCH:
        {
            if (p_Faction == CONTROL_NEUTRAL)
            {
                // Despawn horde or alliance guards
                uint8 l_CreatureMaxIndex = TREMBLADES_VANGUARD_SPAWNS_IDS + ARCHMAGE_OVERWATCH_CREATURES_COUNT;
                for (uint8 l_Count = TREMBLADES_VANGUARD_SPAWNS_IDS; l_Count < l_CreatureMaxIndex; ++l_Count)
                    DelCreature(l_Count);

                // Spawn fighting guards
                for (uint8 l_Index = ARCHMAGE_OVERWATCH_SPAWNS_IDS; l_Index < TOTAL_NEUTRAL_SPAWNS_IDS; ++l_Index)
                {
                    DelCreature(l_Index);
                    AddCreature(l_Index, g_ArchmageOverwatchNeutral[l_Index - ARCHMAGE_OVERWATCH_SPAWNS_IDS]);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, FLAG_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == CONTROL_HORDE ? FLAG_HORDE : FLAG_ALLIANCE);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(ASHRAN_TIME_FOR_BATTLE);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == CONTROL_ALLIANCE ? BATTLE_TYPE_TREMBLADES_VANGUARD : WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE);
                }

                // Despawn fighting guards
                for (uint8 l_Index = ARCHMAGE_OVERWATCH_SPAWNS_IDS; l_Index < TOTAL_NEUTRAL_SPAWNS_IDS; ++l_Index)
                    DelCreature(l_Index);

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = TREMBLADES_VANGUARD_SPAWNS_IDS + ARCHMAGE_OVERWATCH_CREATURES_COUNT;
                for (uint8 l_Count = TREMBLADES_VANGUARD_SPAWNS_IDS; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_ArchmageOverwatchSpawns[p_Faction == CONTROL_ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - TREMBLADES_VANGUARD_SPAWNS_IDS]);
                }

                uint8 l_FireIndex = TREMBLADES_VANGUARD_SPAWNS_IDS + ARCHMAGE_OVERWATCH_CREATURES_COUNT;
                // Spawn fires
                for (uint8 l_Index = l_FireIndex; l_Index < ARCHMAGE_OVERWATCH_SPAWNS_IDS; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == CONTROL_HORDE)
                        AddObject(l_Index, g_ArchmageOverwatchFires[l_Index - l_FireIndex]);
                }
            }

            break;
        }
        default:
            break;
    }
}

OPvPCapturePoint_Graveyard::OPvPCapturePoint_Graveyard(OutdoorPvP* p_Outdoor) : OPvPCapturePoint(p_Outdoor)
{
    m_GraveyardState = CONTROL_NEUTRAL;
    m_ControlTime = 0;

    SetCapturePointData(g_GraveyardBanner_N);
}

void OPvPCapturePoint_Graveyard::ChangeState()
{
    uint32 l_UpdateVal = 0;
    switch (m_State)
    {
        case OBJECTIVESTATE_ALLIANCE:
        {
            m_GraveyardState = CONTROL_ALLIANCE;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = FLAG_ALLIANCE;
            m_ControlTime = 15 * MINUTE * IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(ACTION_ANNOUNCE_ALLIANCE_GRAVEYARD_OWNER);

            SendUpdateWorldState(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR, WORLD_STATE_DISABLED);
            break;
        }
        case OBJECTIVESTATE_HORDE:
        {
            m_GraveyardState = CONTROL_HORDE;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = FLAG_HORDE;
            m_ControlTime = 15 * MINUTE * IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(ACTION_ANNOUNCE_HORDE_GRAVEYARD_OWNER);

            SendUpdateWorldState(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR, WORLD_STATE_DISABLED);
            break;
        }
        case OBJECTIVESTATE_NEUTRAL:
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
        {
            m_GraveyardState = CONTROL_NEUTRAL;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = FLAG_NEUTRAL;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(ACTION_ANNOUNCE_MARKETPLACE_GRAVEYARD);

            break;
        }
        default:
            break;
    }

    GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID);
    if (l_Flag)
        l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, l_UpdateVal);

    UpdateTowerState();
}

void OPvPCapturePoint_Graveyard::SendChangePhase()
{
    SendUpdateWorldState(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR, WORLD_STATE_ENABLED);
    uint32 l_Value = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
    SendUpdateWorldState(WORLD_STATE_GRAVEYARD_PROGRESS_BAR, l_Value);
    SendUpdateWorldState(WORLD_STATE_GRAVEYARD_PROGRESS_BAR_GREY_PCT, m_neutralValuePct);
}

void OPvPCapturePoint_Graveyard::FillInitialWorldStates(ByteBuffer& p_Data)
{
    // Must send control status ?
    /*p_Data << uint32(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_GRAVEYARD_PROGRESS_BAR) << uint32(50); // Neutral
    p_Data << uint32(WORLD_STATE_GRAVEYARD_PROGRESS_BAR_GREY_PCT) << uint32(70);*/
}

void OPvPCapturePoint_Graveyard::UpdateTowerState()
{
    // Must update control status here
}

bool OPvPCapturePoint_Graveyard::HandlePlayerEnter(Player* p_Player)
{
    if (OPvPCapturePoint::HandlePlayerEnter(p_Player))
    {
        p_Player->SendUpdateWorldState(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR, WORLD_STATE_ENABLED);
        uint32 l_Value = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
        p_Player->SendUpdateWorldState(WORLD_STATE_GRAVEYARD_PROGRESS_BAR, l_Value);
        p_Player->SendUpdateWorldState(WORLD_STATE_GRAVEYARD_PROGRESS_BAR_GREY_PCT, m_neutralValuePct);
        return true;
    }

    return false;
}

void OPvPCapturePoint_Graveyard::HandlePlayerLeave(Player* p_Player)
{
    p_Player->SendUpdateWorldState(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR, WORLD_STATE_DISABLED);
    OPvPCapturePoint::HandlePlayerLeave(p_Player);
}

void OPvPCapturePoint_Graveyard::SpawnFactionFlags(uint8 p_Faction)
{
    for (uint8 l_Index = GRAVEYARD_BANNER_0; l_Index < GRAVEYARD_MAX_BANNER; ++l_Index)
    {
        switch (p_Faction)
        {
            case CONTROL_ALLIANCE:
                AddObject(l_Index, g_GraveyardBanner_A[l_Index]);
                break;
            case CONTROL_HORDE:
                AddObject(l_Index, g_GraveyardBanner_H[l_Index]);
                break;
            case CONTROL_NEUTRAL:
                DelObject(l_Index);
                break;
            default:
                break;
        }
    }
}

bool OPvPCapturePoint_Graveyard::Update(uint32 p_Diff)
{
    ScheduleNextControl(p_Diff);

    if (m_State == OBJECTIVESTATE_ALLIANCE || m_State == OBJECTIVESTATE_HORDE)
        return false;

    return OPvPCapturePoint::Update(p_Diff);
}

void OPvPCapturePoint_Graveyard::ScheduleNextControl(uint32 p_Diff)
{
    if (!m_ControlTime)
        return;

    if (m_ControlTime <= p_Diff)
    {
        m_value = 0;
        m_State = OBJECTIVESTATE_NEUTRAL;
        m_ControlTime = 0;
    }
    else
        m_ControlTime -= p_Diff;
}

OutdoorPvPAshran::OutdoorPvPAshran()
{
    m_TypeId                = OUTDOOR_PVP_ASHRAN;
    m_WorldPvPAreaId        = ASHRAN_WORLD_PVP_AREA_ID;
    m_HeraldGuid            = 0;
    m_HighWarlordVolrath    = 0;
    m_GrandMasrhalTremblade = 0;
    m_InitPointsTimer       = 0;
    m_IsInitialized         = false;

    m_Guid = MAKE_NEW_GUID(m_WorldPvPAreaId, 0, HIGHGUID_TYPE_BATTLEGROUND);
    m_Guid |= BATTLEFIELD_TYPE_WORLD_PVP;

    for (uint8 l_Team = BG_TEAM_ALLIANCE; l_Team < BG_TEAMS_COUNT; ++l_Team)
    {
        m_PlayersInWar[l_Team].clear();
        m_InvitedPlayers[l_Team].clear();
        m_PlayersWillBeKick[l_Team].clear();
        m_EnnemiesKilled[l_Team] = 0;
        m_EnnemiesKilledMax[l_Team] = 100;
        m_CurrentBattleState = WORLD_STATE_THE_CROSSROADS_BATTLE;
        m_NextBattleTimer = ASHRAN_TIME_FOR_BATTLE * IN_MILLISECONDS;
        m_MaxBattleTime = 0;
        m_FactionGenericMoP[l_Team] = 0;
    }

    for (uint8 l_Iter = 0; l_Iter < BATTLE_TYPE_MAX; ++l_Iter)
        m_GenericMoPGuids[l_Iter] = 0;
}

bool OutdoorPvPAshran::SetupOutdoorPvP()
{
    RegisterZone(ASHRAN_ZONE_ID);

    for (uint8 l_TeamID = TEAM_ALLIANCE; l_TeamID <= TEAM_HORDE; ++l_TeamID)
        AddAreaTrigger(g_HallowedGroundEntries[l_TeamID], 1, ASHRAN_HALLOWED_GROUND_ID, g_HallowedGroundPos[l_TeamID], 0, sMapMgr->CreateBaseMap(ASHRAN_MAP_ID));

    for (uint8 l_BattleIndex = BATTLE_TYPE_EMBERFALL_TOWER; l_BattleIndex < BATTLE_TYPE_MAX; ++l_BattleIndex)
    {
        if (g_MiddleBattlesEntries[l_BattleIndex] == m_CurrentBattleState)
            m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eAshranBattleType(l_BattleIndex), CONTROL_NEUTRAL);
        else
        {
            if (l_BattleIndex < 2)
                m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eAshranBattleType(l_BattleIndex), CONTROL_HORDE);
            else
                m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eAshranBattleType(l_BattleIndex), CONTROL_ALLIANCE);
        }

        AddCapturePoint(m_ControlPoints[l_BattleIndex]);
    }

    /*
    * - Kings' Rest (west):
    *     Just west of the center of the zone lies Kings' Rest, a small area with links to nearby secondary areas.
    *     The area is notable for holding the Marketplace Graveyard, a capturable graveyard and the only resurrection point
    *     outside of the faction bases. The Graveyard can be captured similarly to bases in Eye of the Storm,
    *     allowing a faction to seize control of it for 15 minutes, after which it will revert to a neutral state.
    *     While captured, players of the controlling faction will be able to respawn there. To capture the Graveyard,
    *     the capture bar must be moved all the way to the far end.
    */
    m_GraveYard = new OPvPCapturePoint_Graveyard(this);
    AddCapturePoint(m_GraveYard);

    return true;
}

void OutdoorPvPAshran::HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != ASHRAN_MAP_ID)
        return;

    if (!p_Player || p_Player->GetTeamId() >= 2 || p_Player->isInFlight())
        return;

    // If the player does not match minimal level requirements for the battlefield, kick him
    if (p_Player->getLevel() < PLAYER_MIN_LEVEL)
    {
        if (m_PlayersWillBeKick[p_Player->GetTeamId()].count(p_Player->GetGUID()) == 0)
            m_PlayersWillBeKick[p_Player->GetTeamId()][p_Player->GetGUID()] = time(NULL) + 10;
        return;
    }

    // Check if player is not already in war
    if (m_PlayersInWar[p_Player->GetTeamId()].count(p_Player->GetGUID()) || m_InvitedPlayers[p_Player->GetTeamId()].count(p_Player->GetGUID()))
        return;

    m_InvitedPlayers[p_Player->GetTeamId()][p_Player->GetGUID()] = time(NULL) + ASHRAN_TIME_FOR_INVITE;

    WorldPacket l_Data(SMSG_BFMGR_ENTRY_INVITE);
    l_Data << uint64(m_Guid);           ///< QueueID
    l_Data << uint32(ASHRAN_ZONE_ID);   ///< Zone Id
    l_Data << uint32(time(NULL) + 20);  ///< Invite lasts until

    ///< Sending the packet to player
    p_Player->SendDirectMessage(&l_Data);

    p_Player->CastSpell(p_Player, SPELL_LOOTABLE, true);

    if (!m_IsInitialized)
        m_InitPointsTimer = 2000;
}

void OutdoorPvPAshran::HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != ASHRAN_MAP_ID)
        return;

    if (p_Player->GetTeamId() < 2)
    {
        m_InvitedPlayers[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersInWar[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersWillBeKick[p_Player->GetTeamId()].erase(p_Player->GetGUID());
    }

    SendRemoveWorldStates(p_Player);
    p_Player->GetSession()->SendBfLeaveMessage(m_Guid);

    p_Player->RemoveAura(SPELL_LOOTABLE);
}

void OutdoorPvPAshran::HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID)
{
    if (p_Player->GetMapId() != ASHRAN_NEUTRAL_MAP_ID && p_Player->GetMapId() != ASHRAN_MAP_ID)
        return;

    if (p_AreaID == ASHRAN_PRE_AREA_HORDE || p_AreaID == ASHRAN_PRE_AREA_ALLIANCE)
        p_Player->SwitchToPhasedMap(ASHRAN_NEUTRAL_MAP_ID);

    if ((p_AreaID == ASHRAN_HORDE_BASE && p_Player->GetTeamId() == TEAM_HORDE) ||
        (p_AreaID == ASHRAN_ALLIANCE_BASE && p_Player->GetTeamId() == TEAM_ALLIANCE))
        p_Player->CastSpell(p_Player, SPELL_HOLD_YOUR_GROUND, true);
}

void OutdoorPvPAshran::HandlePlayerLeaveArea(Player* p_Player, uint32 p_AreaID)
{
    if (p_Player->GetMapId() != ASHRAN_NEUTRAL_MAP_ID && p_Player->GetMapId() != ASHRAN_MAP_ID)
        return;

    if (p_AreaID == ASHRAN_PRE_AREA_HORDE || p_AreaID == ASHRAN_PRE_AREA_ALLIANCE)
        p_Player->SwitchToPhasedMap(ASHRAN_MAP_ID);

    if (p_AreaID == ASHRAN_HORDE_BASE || p_AreaID == ASHRAN_ALLIANCE_BASE)
        p_Player->RemoveAura(SPELL_HOLD_YOUR_GROUND);
}

void OutdoorPvPAshran::HandlePlayerKilled(Player* p_Player)
{
    // Drop half of artifact fragments at player death
    // Even if he's killed by a creature
    if (uint32 l_ArtifactCount = p_Player->GetCurrency(CURRENCY_TYPE_ARTIFACT_FRAGEMENT, false))
        p_Player->ModifyCurrency(CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount / 2));
}

bool OutdoorPvPAshran::Update(uint32 p_Diff)
{
    PlayerTimerMap l_TempList[BG_TEAMS_COUNT];

    for (uint8 l_Team = 0; l_Team < 2; ++l_Team)
    {
        l_TempList[l_Team] = m_InvitedPlayers[l_Team];

        for (PlayerTimerMap::iterator l_Iter = l_TempList[l_Team].begin(); l_Iter != l_TempList[l_Team].end(); ++l_Iter)
        {
            // Remove player after 20s if not accepted
            if ((*l_Iter).second <= time(NULL))
            {
                if (Player* l_Player = sObjectAccessor->FindPlayer((*l_Iter).first))
                {
                    if (l_Player->GetTeamId() == TEAM_HORDE)
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
                    else
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
                }
            }
        }

        l_TempList[l_Team] = m_PlayersWillBeKick[l_Team];

        for (PlayerTimerMap::iterator l_Iter = l_TempList[l_Team].begin(); l_Iter != l_TempList[l_Team].end(); ++l_Iter)
        {
            if ((*l_Iter).second <= time(NULL))
            {
                if (Player* l_Player = sObjectAccessor->FindPlayer((*l_Iter).first))
                {
                    if (l_Player->GetTeamId() == TEAM_HORDE)
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
                    else
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
                }
            }
        }
    }

    ScheduleNextBattle(p_Diff);
    ScheduleEndOfBattle(p_Diff);
    ScheduleInitPoints(p_Diff);

    return OutdoorPvP::Update(p_Diff);
}

void OutdoorPvPAshran::ScheduleNextBattle(uint32 p_Diff)
{
    if (!m_NextBattleTimer)
        return;

    if (m_NextBattleTimer <= p_Diff)
    {
        m_NextBattleTimer = 0;
        SendUpdateWorldState(WORLD_STATE_NEXT_BATTLE_ENABLED, WORLD_STATE_DISABLED);

        bool l_Found = false;
        uint8 l_Count = 0;
        // Faction mini-bosses
        for (uint32 l_BattleIndex : g_MiddleBattlesEntries)
        {
            if (m_CurrentBattleState == l_BattleIndex)
                l_Found = true;
            else
            {
                if (!l_Found)
                    ++l_Count;
            }
        }

        // Road of Glory control points
        if (l_Found)
        {
            SendUpdateWorldState(WORLD_STATE_CONTROL_THE_FLAG, WORLD_STATE_ENABLED);

            if (OPvPCapturePoint_Middle* l_ControlPoint = m_ControlPoints[l_Count])
            {
                l_ControlPoint->SetBattleFaction(CONTROL_NEUTRAL);
                l_ControlPoint->SetValue(0.0f);
                l_ControlPoint->SetState(OBJECTIVESTATE_NEUTRAL);
                l_ControlPoint->UpdateTowerState();

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(l_ControlPoint->m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, FLAG_NEUTRAL);
            }
        }
        // Bosses battle
        else
        {
            // Once all Towers have been claimed by a given faction, after a short interval the enemy faction base itself will become contested.
            m_MaxBattleTime = 10 * MINUTE * IN_MILLISECONDS;
            SendUpdateWorldState(WORLD_STATE_TIME_REMAINING_FOR_BOSS, time(NULL) + (m_MaxBattleTime / IN_MILLISECONDS));

            if (m_CurrentBattleState == WORLD_STATE_HIGH_WARLORD_VOLRATH)
            {
                SendUpdateWorldState(WORLD_STATE_SLAY_VOLRATH, WORLD_STATE_ENABLED);
                SendUpdateWorldState(WORLD_STATE_HIGH_WARLORD_VOLRATH, WORLD_STATE_ENABLED);
                SendUpdateWorldState(WORLD_STATE_WARSPEAR_OUTPOST_STATUS, CONTROL_NEUTRAL);

                if (Creature* l_Volrath = sObjectAccessor->FindCreature(m_HighWarlordVolrath))
                    l_Volrath->AI()->DoAction(ACTION_WARSPEAR_OUTPOST_IN_FIGHT);
            }
            else // WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE
            {
                SendUpdateWorldState(WORLD_STATE_SLAY_TREMBLADE, WORLD_STATE_ENABLED);
                SendUpdateWorldState(WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE, WORLD_STATE_ENABLED);
                SendUpdateWorldState(WORLD_STATE_STORMSHIELD_STRONGHOLD_STATUS, CONTROL_NEUTRAL);

                if (Creature* l_Tremblade = sObjectAccessor->FindCreature(m_GrandMasrhalTremblade))
                    l_Tremblade->AI()->DoAction(ACTION_STORMSHIELD_STRONGHOLD_IN_FIGHT);
            }
        }
    }
    else
        m_NextBattleTimer -= p_Diff;
}

void OutdoorPvPAshran::ScheduleEndOfBattle(uint32 p_Diff)
{
    if (!m_MaxBattleTime)
        return;

    // Attacking players then have a 10 minute window in which to assault the base and defeat the enemy faction leader,
    // after which the base's NPCs will respawn, and the previous Tower will revert to neutral;
    // attacking players will then need to capture it again in order to get another shot at assaulting the enemy base.
    // Should the attacking faction succeed in slaying the enemy faction leader, the battle will be won, and after a moderate interval for looting,
    // the zone will be reset, with control of the Towers reverting to the initial configuration.
    // A special NPC also spawns to allow attacking players to fly back to their own base before the guards respawn.
    if (m_MaxBattleTime <= p_Diff)
    {
        m_MaxBattleTime = 0;
        SetNextBattleTimer(ASHRAN_TIME_FOR_BATTLE);

        if (m_CurrentBattleState == WORLD_STATE_HIGH_WARLORD_VOLRATH)
        {
            SendUpdateWorldState(WORLD_STATE_SLAY_VOLRATH, WORLD_STATE_DISABLED);
            SendUpdateWorldState(WORLD_STATE_HIGH_WARLORD_VOLRATH, WORLD_STATE_DISABLED);
            SendUpdateWorldState(WORLD_STATE_WARSPEAR_OUTPOST_STATUS, CONTROL_HORDE);
            SetBattleState(WORLD_STATE_EMBERFALL_TOWER_BATTLE);

            if (Creature* l_Volrath = sObjectAccessor->FindCreature(m_HighWarlordVolrath))
                l_Volrath->AI()->DoAction(ACTION_WARSPEAR_VICTORY);
        }
        else // WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE
        {
            SendUpdateWorldState(WORLD_STATE_SLAY_TREMBLADE, WORLD_STATE_DISABLED);
            SendUpdateWorldState(WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE, WORLD_STATE_DISABLED);
            SendUpdateWorldState(WORLD_STATE_STORMSHIELD_STRONGHOLD_STATUS, CONTROL_ALLIANCE);
            SetBattleState(WORLD_STATE_ARCHMAGE_OVERWATCH_BATTLE);

            if (Creature* l_Tremblade = sObjectAccessor->FindCreature(m_GrandMasrhalTremblade))
                l_Tremblade->AI()->DoAction(ACTION_STORMSHIELD_VICTORY);
        }
    }
    else
        m_MaxBattleTime -= p_Diff;
}

void OutdoorPvPAshran::ScheduleInitPoints(uint32 p_Diff)
{
    if (!m_InitPointsTimer || m_IsInitialized)
        return;

    if (m_InitPointsTimer <= p_Diff)
    {
        m_InitPointsTimer = 0;
        InitializeControlPoints();
        m_IsInitialized = true;
    }
    else
        m_InitPointsTimer -= p_Diff;
}

void OutdoorPvPAshran::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE) << uint32(m_EnnemiesKilled[TEAM_ALLIANCE]);
    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_HORDE) << uint32(m_EnnemiesKilled[TEAM_HORDE]);

    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE_MAX) << uint32(m_EnnemiesKilledMax[TEAM_ALLIANCE]);
    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_HORDE_MAX) << uint32(m_EnnemiesKilledMax[TEAM_HORDE]);

    p_Data << uint32(WORLD_STATE_ACTIVE_STAGE) << uint32(-1);

    // Laps event
    p_Data << uint32(WORLD_STATE_ENABLE_LAPS_EVENT) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_LAPS_ALLIANCE) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_LAPS_HORDE) << uint32(WORLD_STATE_DISABLED);

    // Ore collection event
    p_Data << uint32(WORLD_STATE_ORE_COLLECTED_ALLIANCE) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_ORE_COLLECTED_HORDE) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_ENABLE_ORE_COLLECTION) << uint32(WORLD_STATE_DISABLED);

    // Fire scoring event
    p_Data << uint32(WORLD_STATE_FIRES_SCORING_ALLIANCE) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_FIRES_SCORING_HORDE) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_FIRES_SCORING_ENABLED) << uint32(WORLD_STATE_DISABLED);

    // Risen spirits event
    p_Data << uint32(WORLD_STATE_RISEN_SPIRITS_CAPTURED_ALLIANCE) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_RISEN_SPIRITS_CAPTURED_HORDE) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_RISEN_SPIRITS_CAPTURE_ENABLED) << uint32(WORLD_STATE_DISABLED);

    // Faction bosses
    if (m_CurrentBattleState == WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE)
    {
        p_Data << uint32(WORLD_STATE_TIME_REMAINING_FOR_BOSS) << uint32(time(NULL) + (m_MaxBattleTime / IN_MILLISECONDS));
        p_Data << uint32(WORLD_STATE_SLAY_VOLRATH) << uint32(WORLD_STATE_DISABLED);
        p_Data << uint32(WORLD_STATE_SLAY_TREMBLADE) << uint32(WORLD_STATE_ENABLED);
    }
    else if (m_CurrentBattleState == WORLD_STATE_HIGH_WARLORD_VOLRATH)
    {
        p_Data << uint32(WORLD_STATE_TIME_REMAINING_FOR_BOSS) << uint32(time(NULL) + (m_MaxBattleTime / IN_MILLISECONDS));
        p_Data << uint32(WORLD_STATE_SLAY_VOLRATH) << uint32(WORLD_STATE_ENABLED);
        p_Data << uint32(WORLD_STATE_SLAY_TREMBLADE) << uint32(WORLD_STATE_DISABLED);
    }
    else
    {
        p_Data << uint32(WORLD_STATE_SLAY_VOLRATH) << uint32(WORLD_STATE_DISABLED);
        p_Data << uint32(WORLD_STATE_SLAY_TREMBLADE) << uint32(WORLD_STATE_DISABLED);
    }

    // Horde boss
    p_Data << uint32(WORLD_STATE_WARSPEAR_OUTPOST_STATUS) << uint32(CONTROL_HORDE);

    // Faction mini-bosses
    for (uint32 l_BattleIndex : g_MiddleBattlesEntries)
    {
        if (m_CurrentBattleState == l_BattleIndex)
        {
            p_Data << uint32(m_CurrentBattleState) << uint32(WORLD_STATE_ENABLED);

            if (m_NextBattleTimer)
            {
                p_Data << uint32(WORLD_STATE_NEXT_BATTLE_ENABLED) << uint32(WORLD_STATE_ENABLED);
                p_Data << uint32(WORLD_STATE_NEXT_BATTLE_TIMESTAMP) << uint32(time(NULL) + (m_NextBattleTimer / IN_MILLISECONDS));
                p_Data << uint32(WORLD_STATE_CONTROL_THE_FLAG) << uint32(WORLD_STATE_DISABLED);
            }
            else
            {
                p_Data << uint32(WORLD_STATE_NEXT_BATTLE_ENABLED) << uint32(WORLD_STATE_DISABLED);
                p_Data << uint32(WORLD_STATE_CONTROL_THE_FLAG) << uint32(WORLD_STATE_ENABLED);
            }
        }
        else
            p_Data << uint32(l_BattleIndex) << uint32(WORLD_STATE_DISABLED);
    }

    // Alliance boss
    p_Data << uint32(WORLD_STATE_STORMSHIELD_STRONGHOLD_STATUS) << uint32(CONTROL_ALLIANCE);

    for (OPvPCapturePointMap::iterator l_CapturePoint = m_capturePoints.begin(); l_CapturePoint != m_capturePoints.end(); ++l_CapturePoint)
        l_CapturePoint->second->FillInitialWorldStates(p_Data);
}

void OutdoorPvPAshran::SendRemoveWorldStates(Player* p_Player)
{
    p_Player->SendUpdateWorldState(WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ENNEMIES_SLAIN_HORDE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE_MAX, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ENNEMIES_SLAIN_HORDE_MAX, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ACTIVE_STAGE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_CONTROL_THE_FLAG, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ENABLE_LAPS_EVENT, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_LAPS_ALLIANCE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_LAPS_HORDE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ORE_COLLECTED_ALLIANCE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ORE_COLLECTED_HORDE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ENABLE_ORE_COLLECTION, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_FIRES_SCORING_ALLIANCE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_FIRES_SCORING_HORDE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_FIRES_SCORING_ENABLED, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_RISEN_SPIRITS_CAPTURED_ALLIANCE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_RISEN_SPIRITS_CAPTURED_HORDE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_RISEN_SPIRITS_CAPTURE_ENABLED, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_NEXT_BATTLE_TIMESTAMP, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_NEXT_BATTLE_ENABLED, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_TIME_REMAINING_FOR_BOSS, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_SLAY_VOLRATH, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_SLAY_TREMBLADE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_EMBERFALL_TOWER_BATTLE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_VOLRATHS_ADVANCE_BATTLE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_THE_CROSSROADS_BATTLE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_TREMBLADES_VANGUARD_BATTLE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_ARCHMAGE_OVERWATCH_BATTLE, 0);
    p_Player->SendUpdateWorldState(WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE, 0);
}

void OutdoorPvPAshran::HandleBFMGREntryInviteResponse(bool p_Accepted, Player* p_Player)
{
    if (p_Accepted)
    {
        m_PlayersInWar[p_Player->GetTeamId()].insert(p_Player->GetGUID());
        m_InvitedPlayers[p_Player->GetTeamId()].erase(p_Player->GetGUID());
    }
    else
    {
        if (p_Player->GetTeamId() == TEAM_HORDE)
            p_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
        else
            p_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
    }
}

void OutdoorPvPAshran::OnCreatureCreate(Creature* p_Creature)
{
    switch (p_Creature->GetEntry())
    {
        case ASHRAN_HERALD:
            m_HeraldGuid = p_Creature->GetGUID();
            break;
        case ASHRAN_HIGH_WARLORD_VOLRATH:
        {
            m_HighWarlordVolrath = p_Creature->GetGUID();
            AddCreature(SLG_GENERIC_MOP_LARGE_AOI + TEAM_HORDE, SLG_GENERIC_MOP_LARGE_AOI, TEAM_NONE, ASHRAN_MAP_ID, p_Creature->m_positionX, p_Creature->m_positionY, p_Creature->m_positionZ, M_PI);
            m_FactionGenericMoP[TEAM_HORDE] = m_Creatures[SLG_GENERIC_MOP_LARGE_AOI + TEAM_HORDE];
            break;
        }
        case ASHRAN_GRAND_MARSHAL_TREMBLADE:
        {
            m_GrandMasrhalTremblade = p_Creature->GetGUID();
            AddCreature(SLG_GENERIC_MOP_LARGE_AOI + TEAM_ALLIANCE, SLG_GENERIC_MOP_LARGE_AOI, TEAM_NONE, ASHRAN_MAP_ID, p_Creature->m_positionX, p_Creature->m_positionY, p_Creature->m_positionZ, M_PI);
            m_FactionGenericMoP[TEAM_ALLIANCE] = m_Creatures[SLG_GENERIC_MOP_LARGE_AOI + TEAM_ALLIANCE];
            break;
        }
        default:
            break;
    }
}

Creature* OutdoorPvPAshran::GetHerald() const
{
    return sObjectAccessor->FindCreature(m_HeraldGuid);
}

void OutdoorPvPAshran::InitializeControlPoints()
{
    for (uint8 l_BattleId = BATTLE_TYPE_EMBERFALL_TOWER; l_BattleId < BATTLE_TYPE_MAX; ++l_BattleId)
    {
        if (OPvPCapturePoint_Middle* l_CapturePoint = m_ControlPoints[l_BattleId])
            l_CapturePoint->SpawnFactionGuards(l_CapturePoint->GetBattleType(), l_CapturePoint->GetBattleFaction());
    }
}

void OutdoorPvPAshran::SetBattleState(uint32 p_NewState)
{
    if (!m_IsInitialized)
        return;

    m_CurrentBattleState = p_NewState;

    // Faction mini-bosses
    for (uint32 l_BattleIndex : g_MiddleBattlesEntries)
    {
        if (m_CurrentBattleState == l_BattleIndex)
            SendUpdateWorldState(m_CurrentBattleState, WORLD_STATE_ENABLED);
        else
            SendUpdateWorldState(l_BattleIndex, WORLD_STATE_DISABLED);
    }

    // Faction bosses
    if (m_CurrentBattleState == WORLD_STATE_HIGH_WARLORD_VOLRATH)
        SendUpdateWorldState(WORLD_STATE_HIGH_WARLORD_VOLRATH, WORLD_STATE_ENABLED);
    else if (m_CurrentBattleState == WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE)
        SendUpdateWorldState(WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE, WORLD_STATE_ENABLED);

    SendUpdateWorldState(WORLD_STATE_NEXT_BATTLE_TIMESTAMP, time(NULL) + (m_NextBattleTimer / IN_MILLISECONDS));
    SendUpdateWorldState(WORLD_STATE_NEXT_BATTLE_ENABLED, WORLD_STATE_ENABLED);
    SendUpdateWorldState(WORLD_STATE_CONTROL_THE_FLAG, WORLD_STATE_DISABLED);
}

class OutdoorPvP_Ashran : public OutdoorPvPScript
{
    public:

        OutdoorPvP_Ashran() : OutdoorPvPScript("outdoorpvp_ashran") { }

        OutdoorPvP* GetOutdoorPvP() const
        {
            return new OutdoorPvPAshran();
        }
};

// A'shran Herald - 84113
class npc_ashran_herald : public CreatureScript
{
    public:
        npc_ashran_herald() : CreatureScript("npc_ashran_herald") { }

        struct npc_ashran_heraldAI : public ScriptedAI
        {
            npc_ashran_heraldAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eTalk
            {
                TALK_ANNOUNCE_NEUTRAL_GRAVEYARD,
                TALK_ANNOUNCE_HORDE_GRAVEYARD,
                TALK_ANNOUNCE_ALLIANCE_GRAVEYARD
            };

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ANNOUNCE_MARKETPLACE_GRAVEYARD:
                        Talk(TALK_ANNOUNCE_NEUTRAL_GRAVEYARD);
                        break;
                    case ACTION_ANNOUNCE_HORDE_GRAVEYARD_OWNER:
                        Talk(TALK_ANNOUNCE_HORDE_GRAVEYARD);
                        break;
                    case ACTION_ANNOUNCE_ALLIANCE_GRAVEYARD_OWNER:
                        Talk(TALK_ANNOUNCE_ALLIANCE_GRAVEYARD);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_heraldAI(p_Creature);
        }
};

// SLG Generic MoP (Large AOI) - 68553
class npc_slg_generic_mop : public CreatureScript
{
    public:
        npc_slg_generic_mop() : CreatureScript("npc_slg_generic_mop") { }

        struct npc_slg_generic_mopAI : public ScriptedAI
        {
            npc_slg_generic_mopAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eTalk
            {
                TALK_HORDE_VICTORY,
                TALK_ALLIANCE_KILL_BOSS,
                TALK_ALLIANCE_VICTORY,
                TALK_HORDE_KILL_BOSS
            };

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ANNOUNCE_HORDE_VICTORY:
                        Talk(TALK_HORDE_VICTORY);
                        break;
                    case ACTION_ANNOUNCE_ALLIANCE_KILL_BOSS:
                        Talk(TALK_ALLIANCE_KILL_BOSS);
                        break;
                    case ACTION_ANNOUNCE_ALLIANCE_VICTORY:
                        Talk(TALK_ALLIANCE_VICTORY);
                        break;
                    case ACTION_ANNOUNCE_HORDE_KILL_BOSS:
                        Talk(TALK_HORDE_KILL_BOSS);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_slg_generic_mopAI(p_Creature);
        }
};

// High Warlord Volrath - 82877
// Grand Marshal Tremblade - 82876
class npc_faction_boss : public CreatureScript
{
    public:
        npc_faction_boss() : CreatureScript("npc_faction_boss") { }

        struct npc_faction_bossAI : public ScriptedAI
        {
            npc_faction_bossAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_ZoneScript = sOutdoorPvPMgr->GetZoneScript(p_Creature->GetZoneId());
            }

            enum eSpells
            {
                SPELL_BLADE_TWISTER_SEARCHER    = 178798,   // Uses 178797 on the target (Only 1)
                SPELL_BLADE_TWISTER_MISSILE     = 178797,   // Launch 178795, Summons 89320
                SPELL_BLADE_TWISTER_VISUAL      = 178796,
                SPELL_MORTAL_CLEAVE             = 177147,
                SPELL_ENABLE_UNIT_FRAME         = 177684
            };

            enum eTalk
            {
                TALK_INTRO,
                TALK_AGGRO,
                TALK_SLAY,
                TALK_DEATH,
                TALK_VICTORY
            };

            enum eEvents
            {
                EVENT_MORTAL_CLEAVE = 1,
                EVENT_BLADE_TWISTER
            };

            EventMap m_Events;
            ZoneScript* m_ZoneScript;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveAura(SPELL_ENABLE_UNIT_FRAME);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(TALK_AGGRO);

                m_Events.ScheduleEvent(EVENT_MORTAL_CLEAVE, 5000);
                m_Events.ScheduleEvent(EVENT_BLADE_TWISTER, 15000);

                me->CastSpell(me, SPELL_ENABLE_UNIT_FRAME, true);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_SLAY);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(TALK_DEATH);

                uint64 l_GenericGuid = ((OutdoorPvPAshran*)m_ZoneScript)->GetFactionGenericMoP(me->GetEntry() == ASHRAN_GRAND_MARSHAL_TREMBLADE ? TEAM_ALLIANCE : TEAM_HORDE);
                if (Creature* l_GenericMoP = sObjectAccessor->FindCreature(l_GenericGuid))
                    l_GenericMoP->AI()->DoAction(me->GetEntry() == ASHRAN_GRAND_MARSHAL_TREMBLADE ? ACTION_ANNOUNCE_HORDE_KILL_BOSS : ACTION_ANNOUNCE_ALLIANCE_KILL_BOSS);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_WARSPEAR_OUTPOST_IN_FIGHT:
                    case ACTION_STORMSHIELD_STRONGHOLD_IN_FIGHT:
                        Talk(TALK_INTRO);
                        break;
                    case ACTION_WARSPEAR_VICTORY:
                    case ACTION_STORMSHIELD_VICTORY:
                        Talk(TALK_VICTORY);
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Target, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == SPELL_BLADE_TWISTER_SEARCHER)
                    me->CastSpell(p_Target, SPELL_BLADE_TWISTER_MISSILE, false);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_MORTAL_CLEAVE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_MORTAL_CLEAVE, false);
                        m_Events.ScheduleEvent(EVENT_MORTAL_CLEAVE, 15000);
                        break;
                    case EVENT_BLADE_TWISTER:
                        me->CastSpell(me, SPELL_BLADE_TWISTER_SEARCHER, true);
                        m_Events.ScheduleEvent(EVENT_BLADE_TWISTER, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_faction_bossAI(p_Creature);
        }
};

// Blade Twister - 178795
class spell_blade_twister : public SpellScriptLoader
{
    public:
        spell_blade_twister() : SpellScriptLoader("spell_blade_twister") { }

        class spell_blade_twister_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_blade_twister_AuraScript);

            enum eSpells
            {
                SPELL_BLADE_TWISTER_DAMAGE  = 177167
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    std::list<Creature*> l_Blades;
                    l_Target->GetCreatureListWithEntryInGrid(l_Blades, ASHRAN_BLADE_TWISTER_TRIGGER, 50.0f);

                    if (l_Blades.empty())
                    {
                        p_AurEff->GetBase()->Remove();
                        return;
                    }

                    l_Blades.remove_if([this, l_Target](Creature* p_Creature) -> bool
                    {
                        if (!p_Creature->GetOwner())
                            return true;

                        if (p_Creature->GetOwner() != l_Target)
                            return true;

                        return false;
                    });

                    for (Creature* l_Creature : l_Blades)
                        l_Target->CastSpell(l_Creature, SPELL_BLADE_TWISTER_DAMAGE, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_blade_twister_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_blade_twister_AuraScript();
        }
};

void AddSC_OutdoorPvPAshran()
{
    new OutdoorPvP_Ashran();
    new npc_ashran_herald();
    new npc_slg_generic_mop();
    new npc_faction_boss();
    new spell_blade_twister();
}
