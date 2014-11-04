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

OPvPCapturePoint_Middle::OPvPCapturePoint_Middle(OutdoorPvP* p_Outdoor, eAshranBattleType p_Type, uint8 p_Faction)
    : OPvPCapturePoint(p_Outdoor), m_BattleType(p_Type), m_BattleFaction(p_Faction)
{
    if (SetCapturePointData(g_CapturePoint[p_Type].entry,
        g_CapturePoint[p_Type].map,
        g_CapturePoint[p_Type].x,
        g_CapturePoint[p_Type].y,
        g_CapturePoint[p_Type].z,
        g_CapturePoint[p_Type].o,
        g_CapturePoint[p_Type].rot0,
        g_CapturePoint[p_Type].rot1,
        g_CapturePoint[p_Type].rot2,
        g_CapturePoint[p_Type].rot3))
    {
        if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
        {
            switch (p_Type)
            {
                case BATTLE_TYPE_EMBERFALL_TOWER:
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, FLAG_HORDE);
                    break;
                case BATTLE_TYPE_VOLRATHS_ADVANCE:
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, FLAG_HORDE);
                    break;
                case BATTLE_TYPE_THE_CROSSROADS:
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, FLAG_NEUTRAL);
                    break;
                case BATTLE_TYPE_TREMBLADES_VANGUARD:
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, FLAG_ALLIANCE);
                    break;
                case BATTLE_TYPE_ARCHMAGE_OVERWATCH:
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, FLAG_ALLIANCE);
                    break;
                default:
                    break;
            }
        }
    }

    SpawnFactionGuards(m_BattleType, m_BattleFaction);
}

void OPvPCapturePoint_Middle::ChangeState()
{
    uint32 l_FieldID = 0;
    switch (m_OldState)
    {
        case OBJECTIVESTATE_NEUTRAL:
            break;
        case OBJECTIVESTATE_ALLIANCE:
            break;
        case OBJECTIVESTATE_HORDE:
            break;
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
            break;
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
            break;
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
            break;
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
            break;
        default:
            break;
    }

    if (l_FieldID)
    {
        m_PvP->SendUpdateWorldState(l_FieldID, 0);
        l_FieldID = 0;
    }

    uint32 l_UpdateVal = 0;
    switch (m_State)
    {
        case OBJECTIVESTATE_NEUTRAL:
            m_BattleFaction = TEAM_NEUTRAL;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = FLAG_NEUTRAL;
            break;
        case OBJECTIVESTATE_ALLIANCE:
            m_BattleFaction = TEAM_ALLIANCE;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = FLAG_ALLIANCE;
            break;
        case OBJECTIVESTATE_HORDE:
            m_BattleFaction = TEAM_HORDE;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = FLAG_HORDE;
            break;
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
            m_BattleFaction = TEAM_NEUTRAL;
            break;
        default:
            break;
    }

    GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID);
    if (l_Flag)
        l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 3, l_UpdateVal);

    if (l_FieldID)
        m_PvP->SendUpdateWorldState(l_FieldID, 1);

    UpdateTowerState();
}

void OPvPCapturePoint_Middle::SendChangePhase()
{
    SendUpdateWorldState(WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR, WORLD_STATE_ENABLED);
    uint32 l_Value = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
    SendUpdateWorldState(WORLD_STATE_TOWER_PROGRESS_BAR, l_Value);
    SendUpdateWorldState(WORLD_STATE_TOWER_PROGRESS_BAR_GREY_PCT, m_neutralValuePct);
}

void OPvPCapturePoint_Middle::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << uint32(WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_TOWER_PROGRESS_BAR) << uint32(50); // Neutral
    p_Data << uint32(WORLD_STATE_TOWER_PROGRESS_BAR_GREY_PCT) << uint32(80);
}

void OPvPCapturePoint_Middle::UpdateTowerState()
{
    m_PvP->SendUpdateWorldState(WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR, WORLD_STATE_DISABLED);
    m_PvP->SendUpdateWorldState(WORLD_STATE_TOWER_PROGRESS_BAR, 50);
    m_PvP->SendUpdateWorldState(WORLD_STATE_TOWER_PROGRESS_BAR_GREY_PCT, 80);
}

bool OPvPCapturePoint_Middle::HandlePlayerEnter(Player* p_Player)
{
    if (OPvPCapturePoint::HandlePlayerEnter(p_Player))
    {
        p_Player->SendUpdateWorldState(WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR, WORLD_STATE_ENABLED);
        uint32 l_Value = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
        p_Player->SendUpdateWorldState(WORLD_STATE_TOWER_PROGRESS_BAR, l_Value);
        p_Player->SendUpdateWorldState(WORLD_STATE_TOWER_PROGRESS_BAR_GREY_PCT, m_neutralValuePct);
        return true;
    }

    return false;
}

void OPvPCapturePoint_Middle::HandlePlayerLeave(Player* p_Player)
{
    p_Player->SendUpdateWorldState(WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR, WORLD_STATE_DISABLED);
    OPvPCapturePoint::HandlePlayerLeave(p_Player);
}

void OPvPCapturePoint_Middle::SpawnFactionGuards(eAshranBattleType p_BattleID, uint8 p_Faction)
{
    switch (p_BattleID)
    {
        case BATTLE_TYPE_EMBERFALL_TOWER:
        {
            switch (p_Faction)
            {
                case TEAM_ALLIANCE:
                    break;
                case TEAM_HORDE:
                    //SummonCreature(NPC_WARSPEAR_RAPTOR_RIDER,
                    //SummonCreature(NPC_WARSPEAR_BLOOD_GUARD, 4791.99f, -4180.81f, 33.22f, 4.95f);
                    //SummonCreature(NPC_WARSPEAR_BLOOD_GUARD, 4788.07f, -4182.17f, 33.22f, 5.80f);
                    //SummonCreature(NPC_WARSPEAR_BLOOD_GUARD, 4775.78f, -4217.50f, 32.58f, 0.00f);
                    //SummonCreature(NPC_WARSPEAR_BLOOD_GUARD, 4776.99f, -4220.96f, 32.33f, 0.91f);
                    //SummonCreature(NPC_WARSPEAR_HEADHUNTER,
                    break;
                case TEAM_NEUTRAL:
                    break;
                default:
                    break;
            }

            break;
        }
        case BATTLE_TYPE_VOLRATHS_ADVANCE:
        {
            switch (p_Faction)
            {
                case TEAM_ALLIANCE:
                    break;
                case TEAM_HORDE:
                    break;
                case TEAM_NEUTRAL:
                    break;
                default:
                    break;
            }

            break;
        }
        case BATTLE_TYPE_THE_CROSSROADS:
        {
            switch (p_Faction)
            {
                case TEAM_ALLIANCE:
                    break;
                case TEAM_HORDE:
                    break;
                case TEAM_NEUTRAL:
                    break;
                default:
                    break;
            }

            break;
        }
        case BATTLE_TYPE_TREMBLADES_VANGUARD:
        {
            switch (p_Faction)
            {
                case TEAM_ALLIANCE:
                    break;
                case TEAM_HORDE:
                    break;
                case TEAM_NEUTRAL:
                    break;
                default:
                    break;
            }

            break;
        }
        case BATTLE_TYPE_ARCHMAGE_OVERWATCH:
        {
            switch (p_Faction)
            {
                case TEAM_ALLIANCE:
                    break;
                case TEAM_HORDE:
                    break;
                case TEAM_NEUTRAL:
                    break;
                default:
                    break;
            }

            break;
        }
        default:
            break;
    }
}

OPvPCapturePoint_Graveyard::OPvPCapturePoint_Graveyard(OutdoorPvP* p_Outdoor) : OPvPCapturePoint(p_Outdoor)
{
    m_GraveyardState = TEAM_NEUTRAL;
    m_ControlTime = 0;

    SetCapturePointData(g_GraveyardBanner_N.entry,
        g_GraveyardBanner_N.map,
        g_GraveyardBanner_N.x,
        g_GraveyardBanner_N.y,
        g_GraveyardBanner_N.z,
        g_GraveyardBanner_N.o,
        g_GraveyardBanner_N.rot0,
        g_GraveyardBanner_N.rot1,
        g_GraveyardBanner_N.rot2,
        g_GraveyardBanner_N.rot3);
}

void OPvPCapturePoint_Graveyard::ChangeState()
{
    uint32 l_FieldID = 0;
    switch (m_OldState)
    {
        case OBJECTIVESTATE_NEUTRAL:
        case OBJECTIVESTATE_ALLIANCE:
        case OBJECTIVESTATE_HORDE:
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
        default:
            break;
    }

    if (l_FieldID)
    {
        m_PvP->SendUpdateWorldState(l_FieldID, 0);
        l_FieldID = 0;
    }

    uint32 l_UpdateVal = 0;
    switch (m_State)
    {
        case OBJECTIVESTATE_ALLIANCE:
        {
            m_GraveyardState = TEAM_ALLIANCE;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = FLAG_ALLIANCE;
            m_ControlTime = 15 * MINUTE * IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(ACTION_ANNOUNCE_ALLIANCE_GRAVEYARD_OWNER);

            DelCapturePoint();
            SendUpdateWorldState(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR, WORLD_STATE_DISABLED);
            break;
        }
        case OBJECTIVESTATE_HORDE:
        {
            m_GraveyardState = TEAM_HORDE;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = FLAG_HORDE;
            m_ControlTime = 15 * MINUTE * IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(ACTION_ANNOUNCE_HORDE_GRAVEYARD_OWNER);

            DelCapturePoint();
            SendUpdateWorldState(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR, WORLD_STATE_DISABLED);
            break;
        }
        case OBJECTIVESTATE_NEUTRAL:
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
        {
            m_GraveyardState = TEAM_NEUTRAL;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = FLAG_NEUTRAL;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(ACTION_ANNOUNCE_MARKETPLACE_GRAVEYARD);

            SetCapturePointData(g_GraveyardBanner_N.entry,
                g_GraveyardBanner_N.map,
                g_GraveyardBanner_N.x,
                g_GraveyardBanner_N.y,
                g_GraveyardBanner_N.z,
                g_GraveyardBanner_N.o,
                g_GraveyardBanner_N.rot0,
                g_GraveyardBanner_N.rot1,
                g_GraveyardBanner_N.rot2,
                g_GraveyardBanner_N.rot3);
            break;
        }
        default:
            break;
    }

    GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID);
    if (l_Flag)
        l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, l_UpdateVal);

    if (l_FieldID)
        m_PvP->SendUpdateWorldState(l_FieldID, 1);
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
    p_Data << uint32(WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_GRAVEYARD_PROGRESS_BAR) << uint32(50); // Neutral
    p_Data << uint32(WORLD_STATE_GRAVEYARD_PROGRESS_BAR_GREY_PCT) << uint32(70);
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
            case TEAM_ALLIANCE:
                AddObject(l_Index, g_GraveyardBanner_A[l_Index]);
                break;
            case TEAM_HORDE:
                AddObject(l_Index, g_GraveyardBanner_H[l_Index]);
                break;
            case TEAM_NEUTRAL:
                DelObject(l_Index);
                break;
            default:
                break;
        }
    }
}

bool OPvPCapturePoint_Graveyard::Update(uint32 p_Diff)
{
    if (m_ControlTime)
    {
        if (m_ControlTime <= p_Diff)
        {
            m_value = 0;
            m_State = OBJECTIVESTATE_NEUTRAL;
            m_ControlTime = 0;
        }
        else
            m_ControlTime -= p_Diff;
    }

    return OPvPCapturePoint::Update(p_Diff);
}

OutdoorPvPAshran::OutdoorPvPAshran()
{
    m_TypeId            = OUTDOOR_PVP_ASHRAN;
    m_WorldPvPAreaId    = ASHRAN_WORLD_PVP_AREA_ID;
    m_HeraldGuid        = 0;

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
    }
}

bool OutdoorPvPAshran::SetupOutdoorPvP()
{
    RegisterZone(ASHRAN_ZONE_ID);

    for (uint8 l_TeamID = TEAM_ALLIANCE; l_TeamID <= TEAM_HORDE; ++l_TeamID)
        AddAreaTrigger(g_HallowedGroundEntries[l_TeamID], 1, ASHRAN_HALLOWED_GROUND_ID, g_HallowedGroundPos[l_TeamID], 0, sMapMgr->FindMap(ASHRAN_MAP_ID, 0));

    for (uint8 l_BattleIndex = BATTLE_TYPE_EMBERFALL_TOWER; l_BattleIndex < BATTLE_TYPE_MAX; ++l_BattleIndex)
    {
        if (g_MiddleBattlesEntries[l_BattleIndex] == m_CurrentBattleState)
            AddCapturePoint(new OPvPCapturePoint_Middle(this, eAshranBattleType(l_BattleIndex), TEAM_NEUTRAL));
        else
        {
            if (l_BattleIndex < 2)
                AddCapturePoint(new OPvPCapturePoint_Middle(this, eAshranBattleType(l_BattleIndex), TEAM_HORDE));
            else
                AddCapturePoint(new OPvPCapturePoint_Middle(this, eAshranBattleType(l_BattleIndex), TEAM_ALLIANCE));
        }
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

    if (m_NextBattleTimer)
    {
        if (m_NextBattleTimer <= p_Diff)
        {
            m_NextBattleTimer = 0;
            SendUpdateWorldState(WORLD_STATE_CONTROL_THE_FLAG, WORLD_STATE_ENABLED);
            SendUpdateWorldState(WORLD_STATE_NEXT_BATTLE_ENABLED, WORLD_STATE_DISABLED);
        }
        else
            m_NextBattleTimer -= p_Diff;
    }

    return OutdoorPvP::Update(p_Diff);
}

void OutdoorPvPAshran::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE) << uint32(m_EnnemiesKilled[TEAM_ALLIANCE]);
    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_HORDE) << uint32(m_EnnemiesKilled[TEAM_HORDE]);

    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE_MAX) << uint32(m_EnnemiesKilledMax[TEAM_ALLIANCE]);
    p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_HORDE_MAX) << uint32(m_EnnemiesKilledMax[TEAM_HORDE]);

    p_Data << uint32(WORLD_STATE_ACTIVE_STAGE) << uint32(-1);
    p_Data << uint32(WORLD_STATE_CONTROL_THE_FLAG) << uint32(WORLD_STATE_DISABLED);

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

    // Battle timers
    p_Data << uint32(WORLD_STATE_NEXT_BATTLE_TIMESTAMP) << uint32(time(NULL) + (m_NextBattleTimer / IN_MILLISECONDS));
    p_Data << uint32(WORLD_STATE_NEXT_BATTLE_ENABLED) << uint32(WORLD_STATE_ENABLED);

    // Faction bosses
    p_Data << uint32(WORLD_STATE_TIME_REMAINING_FOR_BOSS) << uint32(time(NULL));
    p_Data << uint32(WORLD_STATE_SLAY_VOLRATH) << uint32(WORLD_STATE_DISABLED);
    p_Data << uint32(WORLD_STATE_SLAY_TREMBLADE) << uint32(WORLD_STATE_DISABLED);

    // Faction mini-bosses
    for (uint32 l_BattleIndex : g_MiddleBattlesEntries)
    {
        if (m_CurrentBattleState == l_BattleIndex)
            p_Data << uint32(m_CurrentBattleState) << uint32(WORLD_STATE_ENABLED);
        else
            p_Data << uint32(l_BattleIndex) << uint32(WORLD_STATE_DISABLED);
    }

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
    if (p_Creature->GetEntry() == ASHRAN_HERALD)
        m_HeraldGuid = p_Creature->GetGUID();
}

Creature* OutdoorPvPAshran::GetHerald() const
{
    return sObjectAccessor->FindCreature(m_HeraldGuid);
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

// Ashran Herald - 84113
class npc_ashran_herald : public CreatureScript
{
    public:
        npc_ashran_herald() : CreatureScript("npc_ashran_herald") { }

        struct npc_ashran_heraldAI : public ScriptedAI
        {
            npc_ashran_heraldAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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

void AddSC_OutdoorPvPAshran()
{
    new OutdoorPvP_Ashran();
    new npc_ashran_herald();
}
