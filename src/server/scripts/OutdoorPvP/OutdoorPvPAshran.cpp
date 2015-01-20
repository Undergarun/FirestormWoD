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
#include "MapManager.h"

OutdoorGraveyardAshran::OutdoorGraveyardAshran(OutdoorPvPAshran* p_OutdoorPvP) : OutdoorGraveyard(p_OutdoorPvP)
{
    m_OutdoorPvP = p_OutdoorPvP;
}

OPvPCapturePoint_Middle::OPvPCapturePoint_Middle(OutdoorPvP* p_Outdoor, eBattleType p_Type, uint8 p_Faction)
    : OPvPCapturePoint(p_Outdoor), m_BattleType(p_Type), m_BattleFaction(p_Faction)
{
    SetCapturePointData(g_CapturePoint[p_Type]);
    AddCreature(eAshranDatas::AshranGenericMobTypeID + p_Type, eCreatures::SLGGenericMoPLargeAoI, TEAM_NONE, eAshranDatas::AshranMapID, g_CapturePoint[p_Type].x, g_CapturePoint[p_Type].y, g_CapturePoint[p_Type].z, M_PI);
    ((OutdoorPvPAshran*)m_PvP)->AddGenericMoPGuid(p_Type, m_Creatures[eAshranDatas::AshranGenericMobTypeID + p_Type]);

    /// Summon Tower Guardians
    if (p_Type == eBattleType::EmberfallTower)
    {
        /// A blood elf who summons brilliant phoenixes to explode upon the heads of enemies assaulting the Tower below.
        /// Two Lava Furies (Spell: http://www.wowhead.com/spell=176664 - NPC: http://www.wowhead.com/npc=88404) serve as his guardians.
        if (roll_chance_i(25))
            AddCreature(eSpecialSpawns::HordeTowerGuardian, g_FactionGuardians[TeamId::TEAM_HORDE]);
    }
    else if (p_Type == eBattleType::ArchmageOverwatch)
    {
        /// A human mage who takes pleasure in freezing Horde invaders with her frost magic.
        if (roll_chance_i(25))
            AddCreature(eSpecialSpawns::AllianceTowerGuardian, g_FactionGuardians[TeamId::TEAM_ALLIANCE]);
    }
}

void OPvPCapturePoint_Middle::ChangeState()
{
    uint32 l_UpdateVal = 0;
    switch (m_State)
    {
        case OBJECTIVESTATE_ALLIANCE:
            m_BattleFaction = eControlStatus::ControlAlliance;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = eFlagStates::FlagAlliance;
            SendUpdateWorldState(eWorldStates::WorldStateEnableTowerProgressBar, eWorldStates::WorldStateDisabled);
            break;
        case OBJECTIVESTATE_HORDE:
            m_BattleFaction = eControlStatus::ControlHorde;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = eFlagStates::FlagHorde;
            SendUpdateWorldState(eWorldStates::WorldStateEnableTowerProgressBar, eWorldStates::WorldStateDisabled);
            break;
        case OBJECTIVESTATE_NEUTRAL:
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
            m_BattleFaction = eControlStatus::ControlNeutral;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = eFlagStates::FlagNeutral;
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
    if (m_BattleFaction != eControlStatus::ControlNeutral)
        return false;

    return OPvPCapturePoint::Update(p_Diff);
}

void OPvPCapturePoint_Middle::SpawnFactionGuards(eBattleType p_BattleID, uint8 p_Faction)
{
    if (p_Faction > eControlStatus::ControlAlliance || p_BattleID >= eBattleType::MaxBattleType)
        return;

    bool l_IsInitialized = ((OutdoorPvPAshran*)m_PvP)->IsInitialized();
    if (l_IsInitialized && p_Faction != eControlStatus::ControlNeutral)
    {
        if (Creature* l_GenericMoP = sObjectAccessor->FindCreature(((OutdoorPvPAshran*)m_PvP)->GetGenericMoPGuid(p_BattleID)))
            l_GenericMoP->AI()->DoAction(p_Faction == eControlStatus::ControlHorde ? eAshranActions::AnnounceHordeVictory : eAshranActions::AnnounceAllianceVictory);
    }

    switch (p_BattleID)
    {
        case eBattleType::EmberfallTower:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt
                for (uint8 l_Count = 0; l_Count < eSpawns::EmberfallTowerCreaturesCount; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_EmberfallTowerNeutralSpawns[l_Count]);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, eFlagStates::FlagNeutral);

                DelCreature(eSpecialSpawns::EmberfallTowerSpiritHealer);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(5))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateHighWarlordVolrath : eWorldStates::WorldStateVolrathsAdvanceBattle);
                }

                // Spawn horde or alliance guards
                for (uint8 l_Count = 0; l_Count < eSpawns::EmberfallTowerCreaturesCount; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_EmberfallTowerSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count], 2 * TimeConstants::MINUTE);
                }

                // Spawn fires
                for (uint8 l_Count = eSpawns::EmberfallTowerCreaturesCount; l_Count < eSpawns::EmberfallTowerSpawnsIDs; ++l_Count)
                {
                    DelObject(l_Count);

                    if (p_Faction == eControlStatus::ControlAlliance)
                        AddObject(l_Count, g_EmberfallFiresSpawns[l_Count - eSpawns::EmberfallTowerCreaturesCount]);
                }

                DelCreature(eSpecialSpawns::HordeTowerGuardian);
                if (roll_chance_i(25) && p_Faction == eControlStatus::ControlHorde)
                    AddCreature(eSpecialSpawns::HordeTowerGuardian, g_FactionGuardians[TeamId::TEAM_HORDE], 60 * TimeConstants::MINUTE);  ///< Set respawn time at 1 hour to prevent multiple kills

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(5))
                    l_Graveyard->GiveControlTo(p_Faction == eControlStatus::ControlHorde ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE);

                DelCreature(eSpecialSpawns::EmberfallTowerSpiritHealer);
                if (p_Faction == eControlStatus::ControlHorde)
                    AddCreature(eSpecialSpawns::EmberfallTowerSpiritHealer, g_EmberfallTowerSpiritHealer[TeamId::TEAM_HORDE]);
                else
                    AddCreature(eSpecialSpawns::EmberfallTowerSpiritHealer, g_EmberfallTowerSpiritHealer[TeamId::TEAM_ALLIANCE]);
            }

            break;
        }
        case eBattleType::VolrathsAdvance:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt
                uint8 l_CreatureMaxIndex = eSpawns::EmberfallTowerSpawnsIDs + eSpawns::VolrathsAdvanceCreaturesCount;
                for (uint8 l_Count = eSpawns::EmberfallTowerSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_VolrathsAdvanceNeutralSpawns[l_Count - eSpawns::EmberfallTowerSpawnsIDs]);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, eFlagStates::FlagNeutral);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateEmberfallTowerBattle : eWorldStates::WorldStateTheCrossroadsBattle);
                }

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::EmberfallTowerSpawnsIDs + eSpawns::VolrathsAdvanceCreaturesCount;
                for (uint8 l_Count = eSpawns::EmberfallTowerSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_VolrathsAdvanceSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::EmberfallTowerSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                // Spawn fires
                uint8 l_FireIndex = eSpawns::EmberfallTowerSpawnsIDs + eSpawns::VolrathsAdvanceCreaturesCount;
                for (uint8 l_Index = l_FireIndex; l_Index < eSpawns::VolrathsAdvanceSpawnsIDs; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == eControlStatus::ControlAlliance)
                        AddObject(l_Index, g_VolrathsAdvanceFires[l_Index - l_FireIndex]);
                }
            }

            break;
        }
        case eBattleType::TheCrossroads:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt
                uint8 l_CreatureMaxIndex = eSpawns::VolrathsAdvanceSpawnsIDs + eSpawns::TheCrossroadsCreaturesCount;
                for (uint8 l_Count = eSpawns::VolrathsAdvanceSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_CrossroadsNeutralSpawns[l_Count - eSpawns::VolrathsAdvanceSpawnsIDs]);
                }

                uint8 l_FlagIndex = eSpawns::VolrathsAdvanceSpawnsIDs + eSpawns::TheCrossroadsCreaturesCount;
                for (uint8 l_Count = l_FlagIndex; l_Count < eSpawns::TheCrossroadsSpawnsIDs; ++l_Count)
                    DelObject(l_Count);

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, eFlagStates::FlagNeutral);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateVolrathsAdvanceBattle : eWorldStates::WorldStateTrembladesVanguardBattle);
                }

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::VolrathsAdvanceSpawnsIDs + eSpawns::TheCrossroadsCreaturesCount;
                for (uint8 l_Count = eSpawns::VolrathsAdvanceSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_CrossroadSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::VolrathsAdvanceSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                // Spawn flags
                uint8 l_FlagIndex = eSpawns::VolrathsAdvanceSpawnsIDs + eSpawns::TheCrossroadsCreaturesCount;
                for (uint8 l_Count = l_FlagIndex; l_Count < eSpawns::TheCrossroadsSpawnsIDs; ++l_Count)
                {
                    DelObject(l_Count);
                    AddObject(l_Count, g_CrossroadsBanners[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - l_FlagIndex]);
                }
            }

            break;
        }
        case eBattleType::TrembladesVanguard:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                // Spawn fighting guards
                // Stormshield Footman VS Warspear Grunt
                uint8 l_CreatureMaxIndex = eSpawns::TheCrossroadsSpawnsIDs + eSpawns::TrembladesVanguardCreaturesCount;
                for (uint8 l_Count = eSpawns::TheCrossroadsSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_TrembladesVanguardNeutralSpawns[l_Count - eSpawns::TheCrossroadsSpawnsIDs]);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, eFlagStates::FlagNeutral);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateTheCrossroadsBattle : eWorldStates::WorldStateArchmageOverwatchBattle);
                }

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::TheCrossroadsSpawnsIDs + eSpawns::TrembladesVanguardCreaturesCount;
                for (uint8 l_Count = eSpawns::TheCrossroadsSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_TrembladesVanguardSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::TheCrossroadsSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                // Spawn fires
                uint8 l_FireIndex = eSpawns::TheCrossroadsSpawnsIDs + eSpawns::TrembladesVanguardCreaturesCount;
                for (uint8 l_Index = l_FireIndex; l_Index < eSpawns::TrembladesVanguardSpawnsIDs; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == eControlStatus::ControlHorde)
                        AddObject(l_Index, g_TrembladesVanguardFires[l_Index - l_FireIndex]);
                }
            }

            break;
        }
        case eBattleType::ArchmageOverwatch:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                // Despawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::TrembladesVanguardSpawnsIDs + eSpawns::ArchmageOverwatchCreaturesCount;
                for (uint8 l_Count = eSpawns::TrembladesVanguardSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                    DelCreature(l_Count);

                // Spawn fighting guards
                for (uint8 l_Index = eSpawns::ArchmageOverwatchSpawnsIDs; l_Index < eSpawns::ArchmageOverwatchSpawnsIDs; ++l_Index)
                {
                    DelCreature(l_Index);
                    AddCreature(l_Index, g_ArchmageOverwatchNeutral[l_Index - eSpawns::ArchmageOverwatchSpawnsIDs]);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, eFlagStates::FlagNeutral);

                DelCreature(eSpecialSpawns::ArchmageOverwatchSpiritHealer);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(4))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_BYTES_1, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateTrembladesVanguardBattle : eWorldStates::WorldStateGrandMarshalTrembladeBattle);
                }

                // Despawn fighting guards
                for (uint8 l_Index = eSpawns::ArchmageOverwatchSpawnsIDs; l_Index < eSpawns::ArchmageOverwatchSpawnsIDs; ++l_Index)
                    DelCreature(l_Index);

                // Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::TrembladesVanguardSpawnsIDs + eSpawns::ArchmageOverwatchCreaturesCount;
                for (uint8 l_Count = eSpawns::TrembladesVanguardSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_ArchmageOverwatchSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::TrembladesVanguardSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                uint8 l_FireIndex = eSpawns::TrembladesVanguardSpawnsIDs + eSpawns::ArchmageOverwatchCreaturesCount;
                // Spawn fires
                for (uint8 l_Index = l_FireIndex; l_Index < eSpawns::ArchmageOverwatchSpawnsIDs; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == eControlStatus::ControlHorde)
                        AddObject(l_Index, g_ArchmageOverwatchFires[l_Index - l_FireIndex]);
                }

                DelCreature(eSpecialSpawns::AllianceTowerGuardian);
                if (roll_chance_i(25) && p_Faction == eControlStatus::ControlAlliance)
                    AddCreature(eSpecialSpawns::AllianceTowerGuardian, g_FactionGuardians[TeamId::TEAM_ALLIANCE], 60 * TimeConstants::MINUTE);  ///< Set respawn time at 1 hour to prevent multiple kills

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(4))
                    l_Graveyard->GiveControlTo(p_Faction == eControlStatus::ControlHorde ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE);

                DelCreature(eSpecialSpawns::ArchmageOverwatchSpiritHealer);
                if (p_Faction == eControlStatus::ControlHorde)
                    AddCreature(eSpecialSpawns::ArchmageOverwatchSpiritHealer, g_ArchmageOverwatchSpiritHealer[TeamId::TEAM_HORDE]);
                else
                    AddCreature(eSpecialSpawns::ArchmageOverwatchSpiritHealer, g_ArchmageOverwatchSpiritHealer[TeamId::TEAM_ALLIANCE]);
            }

            break;
        }
        default:
            break;
    }
}

OPvPCapturePoint_Graveyard::OPvPCapturePoint_Graveyard(OutdoorPvP* p_Outdoor) : OPvPCapturePoint(p_Outdoor)
{
    m_GraveyardState = eControlStatus::ControlNeutral;
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
            m_GraveyardState = eControlStatus::ControlAlliance;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = eFlagStates::FlagAlliance;
            m_ControlTime = 15 * MINUTE * IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(eAshranActions::AnnounceAllianceGraveyard);

            SendUpdateWorldState(eWorldStates::WorldStateEnableGraveyardProgressBar, eWorldStates::WorldStateDisabled);
            break;
        }
        case OBJECTIVESTATE_HORDE:
        {
            m_GraveyardState = eControlStatus::ControlHorde;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = eFlagStates::FlagHorde;
            m_ControlTime = 15 * MINUTE * IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(eAshranActions::AnnounceHordeGraveyard);

            SendUpdateWorldState(eWorldStates::WorldStateEnableGraveyardProgressBar, eWorldStates::WorldStateDisabled);
            break;
        }
        case OBJECTIVESTATE_NEUTRAL:
        case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
        {
            m_GraveyardState = eControlStatus::ControlNeutral;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = eFlagStates::FlagNeutral;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(eAshranActions::AnnounceMarketplaceGraveyard);

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
    SendUpdateWorldState(eWorldStates::WorldStateEnableGraveyardProgressBar, eWorldStates::WorldStateEnabled);
    uint32 l_Value = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
    SendUpdateWorldState(eWorldStates::WorldStateGraveyardProgressBar, l_Value);
    SendUpdateWorldState(eWorldStates::WorldStateGraveyardProgressBarGreyPct, m_neutralValuePct);
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
        p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnableGraveyardProgressBar, eWorldStates::WorldStateEnabled);
        uint32 l_Value = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
        p_Player->SendUpdateWorldState(eWorldStates::WorldStateGraveyardProgressBar, l_Value);
        p_Player->SendUpdateWorldState(eWorldStates::WorldStateGraveyardProgressBarGreyPct, m_neutralValuePct);
        return true;
    }

    return false;
}

void OPvPCapturePoint_Graveyard::HandlePlayerLeave(Player* p_Player)
{
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnableGraveyardProgressBar, eWorldStates::WorldStateDisabled);
    OPvPCapturePoint::HandlePlayerLeave(p_Player);
}

void OPvPCapturePoint_Graveyard::SpawnFactionFlags(uint8 p_Faction)
{
    for (uint8 l_Index = eBanners::GraveyardBanner0; l_Index < eBanners::GraveyardMaxBanner; ++l_Index)
    {
        switch (p_Faction)
        {
            case eControlStatus::ControlAlliance:
                AddObject(l_Index, g_GraveyardBanner_A[l_Index]);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(2))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_ALLIANCE);
                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(3))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);

                AddCreature(eSpecialSpawns::MarketplaceGraveyardSpiritHealer, g_MarketplaceGraveyardSpirits[TeamId::TEAM_ALLIANCE]);
                break;
            case eControlStatus::ControlHorde:
                AddObject(l_Index, g_GraveyardBanner_H[l_Index]);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(2))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);
                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(3))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_HORDE);

                AddCreature(eSpecialSpawns::MarketplaceGraveyardSpiritHealer, g_MarketplaceGraveyardSpirits[TeamId::TEAM_HORDE]);
                break;
            case eControlStatus::ControlNeutral:
                DelObject(l_Index);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(2))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);
                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(3))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);

                DelCreature(eSpecialSpawns::MarketplaceGraveyardSpiritHealer);
                break;
            default:
                break;
        }
    }
}

bool OPvPCapturePoint_Graveyard::Update(uint32 p_Diff)
{
    ScheduleNextControl(p_Diff);

    if (m_State == ObjectiveStates::OBJECTIVESTATE_ALLIANCE || m_State == ObjectiveStates::OBJECTIVESTATE_HORDE)
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
        m_State = ObjectiveStates::OBJECTIVESTATE_NEUTRAL;
        m_ControlTime = 0;
    }
    else
        m_ControlTime -= p_Diff;
}

OutdoorPvPAshran::OutdoorPvPAshran()
{
    m_TypeId                = OutdoorPvPTypes::OUTDOOR_PVP_ASHRAN;
    m_WorldPvPAreaId        = eAshranDatas::AshranPvPAreaID;
    m_HeraldGuid            = 0;
    m_HighWarlordVolrath    = 0;
    m_GrandMasrhalTremblade = 0;
    m_InitPointsTimer       = 0;
    m_IsInitialized         = false;
    m_WillBeReset           = false;

    m_Guid = MAKE_NEW_GUID(m_WorldPvPAreaId, 0, HighGuid::HIGHGUID_TYPE_BATTLEGROUND);
    m_Guid |= eAshranDatas::BattlefieldWorldPvP;

    for (uint8 l_Team = BattlegroundTeamId::BG_TEAM_ALLIANCE; l_Team < BG_TEAMS_COUNT; ++l_Team)
    {
        m_PlayersInWar[l_Team].clear();
        m_InvitedPlayers[l_Team].clear();
        m_PlayersWillBeKick[l_Team].clear();
        m_EnnemiesKilled[l_Team] = 0;
        m_EnnemiesKilledMax[l_Team] = 100;
        m_CurrentBattleState = eWorldStates::WorldStateTheCrossroadsBattle;
        m_NextBattleTimer = eAshranDatas::AshranTimeForBattle * TimeConstants::IN_MILLISECONDS;
        m_MaxBattleTime = 0;
        m_FactionGenericMoP[l_Team] = 0;
    }

    for (uint8 l_Iter = 0; l_Iter < eBattleType::MaxBattleType; ++l_Iter)
        m_GenericMoPGuids[l_Iter] = 0;

    AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[0], 5 * TimeConstants::MINUTE);
    AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[3], 5 * TimeConstants::MINUTE);
}

bool OutdoorPvPAshran::SetupOutdoorPvP()
{
    RegisterZone(eAshranDatas::AshranZoneID);

    for (uint8 l_BattleIndex = eBattleType::EmberfallTower; l_BattleIndex < eBattleType::MaxBattleType; ++l_BattleIndex)
    {
        if (g_MiddleBattlesEntries[l_BattleIndex] == m_CurrentBattleState)
            m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eBattleType(l_BattleIndex), eControlStatus::ControlNeutral);
        else
        {
            if (l_BattleIndex < 2)
                m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eBattleType(l_BattleIndex), eControlStatus::ControlHorde);
            else
                m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eBattleType(l_BattleIndex), eControlStatus::ControlAlliance);
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

    SetGraveyardNumber(eGraveyards::TotalGraveyards);
    for (uint8 l_I = 0; l_I < eGraveyards::TotalGraveyards; ++l_I)
    {
        OutdoorGraveyardAshran* l_Graveyard = new OutdoorGraveyardAshran(this);
        l_Graveyard->Initialize(g_AshranGraveyards[l_I].m_StartTeam, g_AshranGraveyards[l_I].m_ID);
        m_GraveyardList[l_I] = l_Graveyard;
    }

    /// Summon Hallowed ground and Spirit healers
    for (uint8 l_TeamID = TeamId::TEAM_ALLIANCE; l_TeamID <= TeamId::TEAM_HORDE; ++l_TeamID)
    {
        AddCreature(eSpecialSpawns::AllianceBaseSpiritHealer + l_TeamID, g_BasesSpiritHealers[l_TeamID]);
        AddAreaTrigger(g_HallowedGroundEntries[l_TeamID], 1, eAshranDatas::AshranHallowedGroundID, g_HallowedGroundPos[l_TeamID], 0, sMapMgr->CreateBaseMap(eAshranDatas::AshranMapID));
    }

    return true;
}

void OutdoorPvPAshran::HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != eAshranDatas::AshranMapID)
        return;

    if (!p_Player || p_Player->GetTeamId() >= 2 || p_Player->isInFlight())
        return;

    // If the player does not match minimal level requirements for the battlefield, kick him
    if (p_Player->getLevel() < eAshranDatas::PlayerMinLevel)
    {
        if (m_PlayersWillBeKick[p_Player->GetTeamId()].count(p_Player->GetGUID()) == 0)
            m_PlayersWillBeKick[p_Player->GetTeamId()][p_Player->GetGUID()] = time(NULL) + 10;
        return;
    }

    // Check if player is not already in war
    if (m_PlayersInWar[p_Player->GetTeamId()].count(p_Player->GetGUID()) || m_InvitedPlayers[p_Player->GetTeamId()].count(p_Player->GetGUID()))
        return;

    m_InvitedPlayers[p_Player->GetTeamId()][p_Player->GetGUID()] = time(NULL) + eAshranDatas::AshranTimeForInvite;

    WorldPacket l_Data(Opcodes::SMSG_BFMGR_ENTRY_INVITE);
    l_Data << uint64(m_Guid);                       ///< QueueID
    l_Data << uint32(eAshranDatas::AshranZoneID);   ///< Zone Id
    l_Data << uint32(time(NULL) + 20);              ///< Invite lasts until

    ///< Sending the packet to player
    p_Player->SendDirectMessage(&l_Data);

    p_Player->CastSpell(p_Player, eAshranSpells::SpellLootable, true);

    if (!m_IsInitialized)
        m_InitPointsTimer = 2000;
}

void OutdoorPvPAshran::HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != eAshranDatas::AshranMapID)
        return;

    if (p_Player->GetTeamId() < 2)
    {
        m_InvitedPlayers[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersInWar[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersWillBeKick[p_Player->GetTeamId()].erase(p_Player->GetGUID());
    }

    SendRemoveWorldStates(p_Player);
    p_Player->GetSession()->SendBfLeaveMessage(m_Guid);

    p_Player->RemoveAura(eAshranSpells::SpellLootable);
}

void OutdoorPvPAshran::HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID)
{
    if (p_Player->GetMapId() != eAshranDatas::AshranNeutralMapID && p_Player->GetMapId() != eAshranDatas::AshranMapID)
        return;

    if (p_AreaID == eAshranDatas::AshranPreAreaHorde || p_AreaID == eAshranDatas::AshranPreAreaAlliance)
    {
        uint64 l_Guid = p_Player->GetGUID();

        sMapMgr->AddCriticalOperation([l_Guid]() -> void
        {
            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
                l_Player->SwitchToPhasedMap(eAshranDatas::AshranNeutralMapID);
        });
    }

    /// +30% damage, healing and health for players in their faction bases
    if ((p_AreaID == eAshranDatas::AshranHordeBase && p_Player->GetTeamId() == TeamId::TEAM_HORDE) ||
        (p_AreaID == eAshranDatas::AshranAllianceBase && p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE))
        p_Player->CastSpell(p_Player, eAshranSpells::SpellHoldYourGround, true);
    /// +20% damage, healing and health for players in the first base
    else if ((p_AreaID == eAshranDatas::EmberfallTowerAreaID && p_Player->GetTeamId() == TeamId::TEAM_HORDE) ||
        (p_AreaID == eAshranDatas::ArchmageOverwatchAreaID && p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE))
        p_Player->CastSpell(p_Player, eAshranSpells::SpellTowerDefense, true);
    /// +10% damage, healing and health for players in the second base
    else if ((p_AreaID == eAshranDatas::VolrathsAdvanceAreaID && p_Player->GetTeamId() == TeamId::TEAM_HORDE) ||
        (p_AreaID == eAshranDatas::TrembladesVanguardAreaID && p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE))
        p_Player->CastSpell(p_Player, eAshranSpells::SpellStandFast, true);
}

void OutdoorPvPAshran::HandlePlayerLeaveArea(Player* p_Player, uint32 p_AreaID)
{
    if (p_Player->GetMapId() != eAshranDatas::AshranNeutralMapID && p_Player->GetMapId() != eAshranDatas::AshranMapID)
        return;

    if (p_AreaID == eAshranDatas::AshranPreAreaHorde || p_AreaID == eAshranDatas::AshranPreAreaAlliance)
    {
        uint64 l_Guid = p_Player->GetGUID();

        sMapMgr->AddCriticalOperation([l_Guid]() -> void
        {
            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
                l_Player->SwitchToPhasedMap(eAshranDatas::AshranMapID);
        });
    }

    if (p_AreaID == eAshranDatas::AshranHordeBase || p_AreaID == eAshranDatas::AshranAllianceBase)
        p_Player->RemoveAura(eAshranSpells::SpellHoldYourGround);
    else if (p_AreaID == eAshranDatas::EmberfallTowerAreaID || p_AreaID == eAshranDatas::ArchmageOverwatchAreaID)
        p_Player->RemoveAura(eAshranSpells::SpellTowerDefense);
    else if (p_AreaID == eAshranDatas::VolrathsAdvanceAreaID || p_AreaID == eAshranDatas::TrembladesVanguardAreaID)
        p_Player->RemoveAura(eAshranSpells::SpellStandFast);
}

void OutdoorPvPAshran::HandlePlayerKilled(Player* p_Player)
{
    // Drop half of artifact fragments at player death
    // Even if he's killed by a creature
    if (uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, false))
        p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount / 2));
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
                    if (l_Player->GetTeamId() == TeamId::TEAM_HORDE)
                        l_Player->TeleportTo(eAshranDatas::AshranNeutralMapID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
                    else
                        l_Player->TeleportTo(eAshranDatas::AshranNeutralMapID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
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
                    if (l_Player->GetTeamId() == TeamId::TEAM_HORDE)
                        l_Player->TeleportTo(eAshranDatas::AshranNeutralMapID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
                    else
                        l_Player->TeleportTo(eAshranDatas::AshranNeutralMapID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
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

        if (m_WillBeReset)
        {
            SetBattleState(m_CurrentBattleState);
            ResetControlPoints();
        }

        SendUpdateWorldState(eWorldStates::WorldStateNextBattleEnabled, eWorldStates::WorldStateDisabled);

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
            SendUpdateWorldState(eWorldStates::WorldStateControlTheFlag, eWorldStates::WorldStateEnabled);

            if (OPvPCapturePoint_Middle* l_ControlPoint = m_ControlPoints[l_Count])
            {
                l_ControlPoint->SetBattleFaction(eControlStatus::ControlNeutral);
                l_ControlPoint->SetValue(0.0f);
                l_ControlPoint->SetState(ObjectiveStates::OBJECTIVESTATE_NEUTRAL);
                l_ControlPoint->UpdateTowerState();

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(l_ControlPoint->m_capturePointGUID))
                    l_Flag->SetByteValue(EGameObjectFields::GAMEOBJECT_BYTES_1, 2, eFlagStates::FlagNeutral);
            }
        }
        // Bosses battle
        else
        {
            // Once all Towers have been claimed by a given faction, after a short interval the enemy faction base itself will become contested.
            m_MaxBattleTime = 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;
            SendUpdateWorldState(eWorldStates::WorldStateTimeRemainingForBoss, time(NULL) + (m_MaxBattleTime / TimeConstants::IN_MILLISECONDS));

            if (m_CurrentBattleState == eWorldStates::WorldStateHighWarlordVolrath)
            {
                SendUpdateWorldState(eWorldStates::WorldStateSlayVolrath, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateHighWarlordVolrath, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateWarspearOutpostStatus, eControlStatus::ControlNeutral);

                if (Creature* l_Volrath = sObjectAccessor->FindCreature(m_HighWarlordVolrath))
                    l_Volrath->AI()->DoAction(eAshranActions::WarspearOutpostInFight);
            }
            else // WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE
            {
                SendUpdateWorldState(eWorldStates::WorldStateSlayTremblade, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateStormshieldStrongholdStatus, eControlStatus::ControlNeutral);

                if (Creature* l_Tremblade = sObjectAccessor->FindCreature(m_GrandMasrhalTremblade))
                    l_Tremblade->AI()->DoAction(eAshranActions::StormshieldStrongholdInFight);
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
    if (m_MaxBattleTime <= p_Diff)
    {
        m_MaxBattleTime = 0;
        SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);

        if (m_CurrentBattleState == eWorldStates::WorldStateHighWarlordVolrath)
        {
            SendUpdateWorldState(eWorldStates::WorldStateSlayVolrath, eWorldStates::WorldStateDisabled);
            SendUpdateWorldState(eWorldStates::WorldStateHighWarlordVolrath, eWorldStates::WorldStateDisabled);
            SendUpdateWorldState(eWorldStates::WorldStateWarspearOutpostStatus, eControlStatus::ControlHorde);
            SetBattleState(eWorldStates::WorldStateEmberfallTowerBattle);

            if (Creature* l_Volrath = sObjectAccessor->FindCreature(m_HighWarlordVolrath))
                l_Volrath->AI()->DoAction(eAshranActions::WarspearVictory);
        }
        else ///< WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE
        {
            SendUpdateWorldState(eWorldStates::WorldStateSlayTremblade, eWorldStates::WorldStateDisabled);
            SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, eWorldStates::WorldStateDisabled);
            SendUpdateWorldState(eWorldStates::WorldStateStormshieldStrongholdStatus, eControlStatus::ControlAlliance);
            SetBattleState(eWorldStates::WorldStateArchmageOverwatchBattle);

            if (Creature* l_Tremblade = sObjectAccessor->FindCreature(m_GrandMasrhalTremblade))
                l_Tremblade->AI()->DoAction(eAshranActions::StormshieldVictory);
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
    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainAlliance) << uint32(m_EnnemiesKilled[TEAM_ALLIANCE]);
    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainHorde) << uint32(m_EnnemiesKilled[TEAM_HORDE]);

    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainAlianceMax) << uint32(m_EnnemiesKilledMax[TEAM_ALLIANCE]);
    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainHordeMax) << uint32(m_EnnemiesKilledMax[TEAM_HORDE]);

    p_Data << uint32(eWorldStates::WorldStateActiveStage) << uint32(-1);

    // Laps event
    p_Data << uint32(eWorldStates::WorldStateEnableLapsEvent) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateLapsAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateLapsHorde) << uint32(eWorldStates::WorldStateDisabled);

    // Ore collection event
    p_Data << uint32(eWorldStates::WorldStateOreCollectedAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateOreCollectedHorde) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateEnableOreCollection) << uint32(eWorldStates::WorldStateDisabled);

    // Fire scoring event
    p_Data << uint32(eWorldStates::WorldStateFiresScoringAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateFiresScoringHorde) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateFiresScoringEnabled) << uint32(eWorldStates::WorldStateDisabled);

    // Risen spirits event
    p_Data << uint32(eWorldStates::WorldStateRisenSpiritsCapturedAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateRisenSpiritsCapturedHorde) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateRisenSpiritsCaptureEnabled) << uint32(eWorldStates::WorldStateDisabled);

    // Faction bosses
    if (m_CurrentBattleState == eWorldStates::WorldStateGrandMarshalTrembladeBattle)
    {
        p_Data << uint32(eWorldStates::WorldStateTimeRemainingForBoss) << uint32(time(NULL) + (m_MaxBattleTime / IN_MILLISECONDS));
        p_Data << uint32(eWorldStates::WorldStateSlayVolrath) << uint32(eWorldStates::WorldStateDisabled);
        p_Data << uint32(eWorldStates::WorldStateSlayTremblade) << uint32(eWorldStates::WorldStateEnabled);
    }
    else if (m_CurrentBattleState == eWorldStates::WorldStateHighWarlordVolrath)
    {
        p_Data << uint32(eWorldStates::WorldStateTimeRemainingForBoss) << uint32(time(NULL) + (m_MaxBattleTime / IN_MILLISECONDS));
        p_Data << uint32(eWorldStates::WorldStateSlayVolrath) << uint32(eWorldStates::WorldStateEnabled);
        p_Data << uint32(eWorldStates::WorldStateSlayTremblade) << uint32(eWorldStates::WorldStateDisabled);
    }
    else
    {
        p_Data << uint32(eWorldStates::WorldStateSlayVolrath) << uint32(eWorldStates::WorldStateDisabled);
        p_Data << uint32(eWorldStates::WorldStateSlayTremblade) << uint32(eWorldStates::WorldStateDisabled);
    }

    // Horde boss
    p_Data << uint32(eWorldStates::WorldStateWarspearOutpostStatus) << uint32(eControlStatus::ControlHorde);

    // Faction mini-bosses
    for (uint32 l_BattleIndex : g_MiddleBattlesEntries)
    {
        if (m_CurrentBattleState == l_BattleIndex)
        {
            p_Data << uint32(m_CurrentBattleState) << uint32(eWorldStates::WorldStateEnabled);

            if (m_NextBattleTimer)
            {
                p_Data << uint32(eWorldStates::WorldStateNextBattleEnabled) << uint32(eWorldStates::WorldStateEnabled);
                p_Data << uint32(eWorldStates::WorldStateNextBattleTimestamp) << uint32(time(NULL) + (m_NextBattleTimer / IN_MILLISECONDS));
                p_Data << uint32(eWorldStates::WorldStateControlTheFlag) << uint32(eWorldStates::WorldStateDisabled);
            }
            else
            {
                p_Data << uint32(eWorldStates::WorldStateNextBattleEnabled) << uint32(eWorldStates::WorldStateDisabled);
                p_Data << uint32(eWorldStates::WorldStateControlTheFlag) << uint32(eWorldStates::WorldStateEnabled);
            }
        }
        else
            p_Data << uint32(l_BattleIndex) << uint32(eWorldStates::WorldStateDisabled);
    }

    // Alliance boss
    p_Data << uint32(eWorldStates::WorldStateStormshieldStrongholdStatus) << uint32(eControlStatus::ControlAlliance);

    for (OPvPCapturePointMap::iterator l_CapturePoint = m_capturePoints.begin(); l_CapturePoint != m_capturePoints.end(); ++l_CapturePoint)
        l_CapturePoint->second->FillInitialWorldStates(p_Data);
}

void OutdoorPvPAshran::SendRemoveWorldStates(Player* p_Player)
{
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAlliance, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHorde, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAlianceMax, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHordeMax, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateActiveStage, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateControlTheFlag, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnableLapsEvent, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateLapsAlliance, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateLapsHorde, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateOreCollectedAlliance, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateOreCollectedHorde, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnableOreCollection, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateFiresScoringAlliance, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateFiresScoringHorde, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateFiresScoringEnabled, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateRisenSpiritsCapturedAlliance, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateRisenSpiritsCapturedHorde, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateRisenSpiritsCaptureEnabled, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateNextBattleTimestamp, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateNextBattleEnabled, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateTimeRemainingForBoss, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateSlayVolrath, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateSlayTremblade, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEmberfallTowerBattle, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateVolrathsAdvanceBattle, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateTheCrossroadsBattle, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateTrembladesVanguardBattle, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateArchmageOverwatchBattle, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, 0);
}

void OutdoorPvPAshran::HandleBFMGREntryInviteResponse(bool p_Accepted, Player* p_Player)
{
    if (p_Accepted)
    {
        m_PlayersInWar[p_Player->GetTeamId()].insert(p_Player->GetGUID());
        m_InvitedPlayers[p_Player->GetTeamId()].erase(p_Player->GetGUID());

        p_Player->GetSession()->SendBfEntered(m_Guid);
    }
    else
    {
        if (p_Player->GetTeamId() == TeamId::TEAM_HORDE)
            p_Player->TeleportTo(eAshranDatas::AshranNeutralMapID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
        else
            p_Player->TeleportTo(eAshranDatas::AshranNeutralMapID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
    }
}

void OutdoorPvPAshran::OnCreatureCreate(Creature* p_Creature)
{
    switch (p_Creature->GetEntry())
    {
        case eCreatures::AshranHerald:
            m_HeraldGuid = p_Creature->GetGUID();
            break;
        case eCreatures::HighWarlordVolrath:
            m_HighWarlordVolrath = p_Creature->GetGUID();
            AddCreature(eCreatures::SLGGenericMoPLargeAoI + TeamId::TEAM_HORDE, eCreatures::SLGGenericMoPLargeAoI, Team::TEAM_NONE, eAshranDatas::AshranMapID, p_Creature->m_positionX, p_Creature->m_positionY, p_Creature->m_positionZ, M_PI);
            m_FactionGenericMoP[TeamId::TEAM_HORDE] = m_Creatures[eCreatures::SLGGenericMoPLargeAoI + TeamId::TEAM_HORDE];
            break;
        case eCreatures::GrandMarshalTremblade:
            m_GrandMasrhalTremblade = p_Creature->GetGUID();
            AddCreature(eCreatures::SLGGenericMoPLargeAoI + TeamId::TEAM_ALLIANCE, eCreatures::SLGGenericMoPLargeAoI, Team::TEAM_NONE, eAshranDatas::AshranMapID, p_Creature->m_positionX, p_Creature->m_positionY, p_Creature->m_positionZ, M_PI);
            m_FactionGenericMoP[TeamId::TEAM_ALLIANCE] = m_Creatures[eCreatures::SLGGenericMoPLargeAoI + TeamId::TEAM_ALLIANCE];
            break;
        case eCreatures::AllianceSpiritGuide:
        case eCreatures::HordeSpiritGuide:
        {
            TeamId l_TeamID = p_Creature->GetEntry() == eCreatures::HordeSpiritGuide ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE;
            uint8 l_GraveyardID = GetSpiritGraveyardID(p_Creature->GetAreaId(), l_TeamID);
            if (m_GraveyardList[l_GraveyardID] != nullptr)
                m_GraveyardList[l_GraveyardID]->SetSpirit(p_Creature, l_TeamID);
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

void OutdoorPvPAshran::ResetControlPoints()
{
    if (!m_WillBeReset)
        return;

    m_IsInitialized = false;

    /// Despawn flight masters and gryphons/wyverns
    for (uint8 l_I = 0; l_I < eSpecialSpawns::MaxTaxiToBases; ++l_I)
    {
        DelCreature(eSpecialSpawns::AllianceTaxiToBase1 + l_I);
        DelCreature(eSpecialSpawns::HordeTaxiToBase1 + l_I);
    }

    for (uint8 l_BattleIndex = eBattleType::EmberfallTower; l_BattleIndex < eBattleType::MaxBattleType; ++l_BattleIndex)
    {
        if (OPvPCapturePoint_Middle* l_CapturePoint = m_ControlPoints[l_BattleIndex])
        {
            if (g_MiddleBattlesEntries[l_BattleIndex] == m_CurrentBattleState)
            {
                l_CapturePoint->SetBattleFaction(eControlStatus::ControlNeutral);
                l_CapturePoint->SetValue(0.0f);
                l_CapturePoint->SetState(ObjectiveStates::OBJECTIVESTATE_NEUTRAL);
                l_CapturePoint->UpdateTowerState();
            }
            else
            {
                l_CapturePoint->SetBattleFaction(l_BattleIndex < 2 ? eControlStatus::ControlHorde : eControlStatus::ControlAlliance);
                l_CapturePoint->SetState(l_BattleIndex < 2 ? ObjectiveStates::OBJECTIVESTATE_HORDE : ObjectiveStates::OBJECTIVESTATE_ALLIANCE);
                l_CapturePoint->UpdateTowerState();
            }

            l_CapturePoint->SpawnFactionGuards(l_CapturePoint->GetBattleType(), l_CapturePoint->GetBattleFaction());
        }
    }

    SendUpdateWorldState(eWorldStates::WorldStateWarspearOutpostStatus, eControlStatus::ControlHorde);
    SendUpdateWorldState(eWorldStates::WorldStateStormshieldStrongholdStatus, eControlStatus::ControlAlliance);

    m_WillBeReset = false;
    m_IsInitialized = true;
}

void OutdoorPvPAshran::InitializeControlPoints()
{
    for (uint8 l_BattleId = eBattleType::EmberfallTower; l_BattleId < eBattleType::MaxBattleType; ++l_BattleId)
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

    /// Changing boss positions depending on towers status
    if (!m_WillBeReset)
    {
        switch (m_CurrentBattleState)
        {
            case eWorldStates::WorldStateEmberfallTowerBattle:
                DelCreature(eSpecialSpawns::HordeFactionBoss);
                AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[5], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateVolrathsAdvanceBattle:
                DelCreature(eSpecialSpawns::HordeFactionBoss);
                AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[4], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateTheCrossroadsBattle:
                DelCreature(eSpecialSpawns::AllianceFactionBoss);
                DelCreature(eSpecialSpawns::HordeFactionBoss);
                AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[0], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[3], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateTrembladesVanguardBattle:
                DelCreature(eSpecialSpawns::AllianceFactionBoss);
                AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[1], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateArchmageOverwatchBattle:
                DelCreature(eSpecialSpawns::AllianceFactionBoss);
                AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[2], 5 * TimeConstants::MINUTE);
                break;
            default:
                break;
        }
    }

    // Faction mini-bosses
    for (uint32 l_BattleIndex : g_MiddleBattlesEntries)
    {
        if (m_CurrentBattleState == l_BattleIndex)
            SendUpdateWorldState(m_CurrentBattleState, eWorldStates::WorldStateEnabled);
        else
            SendUpdateWorldState(l_BattleIndex, eWorldStates::WorldStateDisabled);
    }

    // Faction bosses
    if (m_CurrentBattleState == eWorldStates::WorldStateHighWarlordVolrath)
        SendUpdateWorldState(eWorldStates::WorldStateHighWarlordVolrath, eWorldStates::WorldStateEnabled);
    else if (m_CurrentBattleState == eWorldStates::WorldStateGrandMarshalTrembladeBattle)
        SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, eWorldStates::WorldStateEnabled);
    else
    {
        SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, eWorldStates::WorldStateDisabled);
        SendUpdateWorldState(eWorldStates::WorldStateHighWarlordVolrath, eWorldStates::WorldStateDisabled);
    }

    SendUpdateWorldState(eWorldStates::WorldStateNextBattleTimestamp, time(NULL) + (m_NextBattleTimer / TimeConstants::IN_MILLISECONDS));
    SendUpdateWorldState(eWorldStates::WorldStateNextBattleEnabled, eWorldStates::WorldStateEnabled);
    SendUpdateWorldState(eWorldStates::WorldStateControlTheFlag, eWorldStates::WorldStateDisabled);
}

void OutdoorPvPAshran::HandleFactionBossDeath(uint8 p_Faction)
{
    /*******************************************************************************************************************
        Should the attacking faction succeed in slaying the enemy faction leader, the battle will be won
        and after a moderate interval for looting, the zone will be reset,
        with control of the Towers reverting to the initial configuration.
        A special NPC also spawns to allow attacking players to fly back to their own base before the guards respawn.
    *******************************************************************************************************************/

    if (m_CurrentBattleState == eWorldStates::WorldStateHighWarlordVolrath)
    {
        SendUpdateWorldState(eWorldStates::WorldStateSlayVolrath, eWorldStates::WorldStateDisabled);
        SendUpdateWorldState(eWorldStates::WorldStateHighWarlordVolrath, eWorldStates::WorldStateDisabled);

        if (Creature* l_Tremblade = sObjectAccessor->FindCreature(m_GrandMasrhalTremblade))
            l_Tremblade->AI()->DoAction(eAshranActions::StormshieldVictory);
    }
    else if (m_CurrentBattleState == eWorldStates::WorldStateGrandMarshalTrembladeBattle)
    {
        SendUpdateWorldState(eWorldStates::WorldStateSlayTremblade, eWorldStates::WorldStateDisabled);
        SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, eWorldStates::WorldStateDisabled);

        if (Creature* l_Volrath = sObjectAccessor->FindCreature(m_HighWarlordVolrath))
            l_Volrath->AI()->DoAction(eAshranActions::WarspearVictory);
    }
    else
        return; ///< Nothing to do here, boss shouldn't be killed yet

    /// When the Battleground is won by either side, there is a 5 minute pause; after which,
    /// the Battleground simply restarts fresh at the Crossroads.
    SetNextBattleTimer(5 * TimeConstants::MINUTE);

    m_WillBeReset = true;   ///< Must do that before changing state
    SetBattleState(eWorldStates::WorldStateTheCrossroadsBattle);

    for (uint8 l_I = 0; l_I < eSpecialSpawns::MaxTaxiToBases; ++l_I)
    {
        DelCreature(eSpecialSpawns::AllianceTaxiToBase1 + l_I);
        DelCreature(eSpecialSpawns::HordeTaxiToBase1 + l_I);

        if (p_Faction == TeamId::TEAM_ALLIANCE)
            AddCreature(eSpecialSpawns::AllianceTaxiToBase1 + l_I, g_FactionTaxisToBase[p_Faction][l_I], 5 * TimeConstants::MINUTE);
        else if (p_Faction == TeamId::TEAM_HORDE)
            AddCreature(eSpecialSpawns::HordeTaxiToBase1 + l_I, g_FactionTaxisToBase[p_Faction][l_I], 5 * TimeConstants::MINUTE);
        else
            return; ///< Nothing to do here
    }
}

WorldSafeLocsEntry const* OutdoorPvPAshran::GetClosestGraveyard(Player* p_Player)
{
    WorldSafeLocsEntry const* l_Graveyard = nullptr;

    float l_PosX = p_Player->GetPositionX();
    float l_PosY = p_Player->GetPositionY();
    float l_MinDist = 1000000.0f;

    uint8 l_TeamID = p_Player->GetTeamId();
    for (uint8 l_I = 0; l_I < eGraveyards::MaxGraveyards; ++l_I)
    {
        /// Check owner of Marketplace Graveyard
        if (g_GraveyardIDs[l_TeamID][l_I] == eGraveyards::AllianceCenter || g_GraveyardIDs[l_TeamID][l_I] == eGraveyards::HordeCenter)
        {
            if (m_GraveYard != nullptr)
            {
                uint8 l_State = m_GraveYard->GetGraveyardState();
                if (l_State == eControlStatus::ControlNeutral || (l_State == eControlStatus::ControlAlliance && l_TeamID != TeamId::TEAM_ALLIANCE) ||
                    (l_State == eControlStatus::ControlHorde && l_TeamID != TeamId::TEAM_HORDE))
                    continue;
            }
        }
        /// Check status of Archmage Overwatch and Emberfall Tower
        else if (g_GraveyardIDs[l_TeamID][l_I] == eGraveyards::TowerAlliance || g_GraveyardIDs[l_TeamID][l_I] == eGraveyards::TowerHorde)
        {
            if (OPvPCapturePoint_Middle* l_CapturePoint = m_ControlPoints[eBattleType::ArchmageOverwatch])
            {
                uint32 l_State = l_CapturePoint->GetBattleFaction();
                if (l_State == eControlStatus::ControlNeutral || (l_State == eControlStatus::ControlAlliance && l_TeamID != TeamId::TEAM_ALLIANCE) ||
                    (l_State == eControlStatus::ControlHorde && l_TeamID != TeamId::TEAM_HORDE))
                    continue;
            }
        }

        /// No other checks needed, other graveyards are main bases
        if (WorldSafeLocsEntry const* l_SafeLoc = sWorldSafeLocsStore.LookupEntry(g_GraveyardIDs[l_TeamID][l_I]))
        {
            float l_Dist = ((l_SafeLoc->x - l_PosX) * (l_SafeLoc->x - l_PosX)) + ((l_SafeLoc->y - l_PosY) * (l_SafeLoc->y - l_PosY));
            if (l_MinDist > l_Dist)
            {
                l_MinDist = l_Dist;
                l_Graveyard = l_SafeLoc;
            }
        }
    }

    return l_Graveyard;
}

uint8 OutdoorPvPAshran::GetSpiritGraveyardID(uint32 p_AreaID, TeamId p_Team) const
{
    switch (p_AreaID)
    {
        case eAshranDatas::AshranAllianceBase:
            return 0;
        case eAshranDatas::AshranHordeBase:
            return 1;
        case eAshranDatas::KingsRestAreaID:
        {
            if (p_Team == TeamId::TEAM_ALLIANCE)
                return 2;
            else
                return 3;
        }
        case eAshranDatas::ArchmageOverwatchAreaID:
            return 4;
        case eAshranDatas::EmberfallTowerAreaID:
            return 5;
        default:
            break;
    }

    return 0;
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

/// A'shran Herald - 84113
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
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eAshranActions::AnnounceMarketplaceGraveyard:
                        Talk(eTalk::TALK_ANNOUNCE_NEUTRAL_GRAVEYARD);
                        break;
                    case eAshranActions::AnnounceHordeGraveyard:
                        Talk(eTalk::TALK_ANNOUNCE_HORDE_GRAVEYARD);
                        break;
                    case eAshranActions::AnnounceAllianceGraveyard:
                        Talk(eTalk::TALK_ANNOUNCE_ALLIANCE_GRAVEYARD);
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

/// SLG Generic MoP (Large AOI) - 68553
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
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eAshranActions::AnnounceHordeVictory:
                        Talk(eTalk::TALK_HORDE_VICTORY);
                        break;
                    case eAshranActions::AnnounceAllianceKillBoss:
                        Talk(eTalk::TALK_ALLIANCE_KILL_BOSS);
                        break;
                    case eAshranActions::AnnounceAllianceVictory:
                        Talk(eTalk::TALK_ALLIANCE_VICTORY);
                        break;
                    case eAshranActions::AnnounceHordeKillBoss:
                        Talk(eTalk::TALK_HORDE_KILL_BOSS);
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

/// High Warlord Volrath - 82877
/// Grand Marshal Tremblade - 82876
class npc_faction_boss : public CreatureScript
{
    public:
        npc_faction_boss() : CreatureScript("npc_faction_boss") { }

        struct npc_faction_bossAI : public ScriptedAI
        {
            npc_faction_bossAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_ZoneScript = sOutdoorPvPMgr->GetZoneScript(p_Creature->GetZoneId());
                m_BaseHP = me->GetMaxHealth();
            }

            enum eSpells
            {
                SpellBladeTwisterSearcher    = 178798,   ///< Uses 178797 on the target (Only 1)
                SpellBladeTwisterMissile     = 178797,   ///< Launch 178795, Summons 89320
                SpellMortalCleave            = 177147,
                SpellEnableUnitFrame         = 177684
            };

            enum eTalk
            {
                TalkIntro,
                TalkAggro,
                TalkSlay,
                TalkDeath,
                TalkVictory
            };

            enum eEvents
            {
                EventMortalCleave = 1,
                EventBladeTwister
            };

            EventMap m_Events;
            ZoneScript* m_ZoneScript;

            bool m_FirstVictim;
            uint32 m_BaseHP;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveAura(eSpells::SpellEnableUnitFrame);

                m_FirstVictim = true;

                me->SetHealth(m_BaseHP);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventMortalCleave, 5000);
                m_Events.ScheduleEvent(eEvents::EventBladeTwister, 15000);

                me->CastSpell(me, eSpells::SpellEnableUnitFrame, true);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(eTalk::TalkDeath);

                uint64 l_GenericGuid = ((OutdoorPvPAshran*)m_ZoneScript)->GetFactionGenericMoP(me->GetEntry() == eCreatures::GrandMarshalTremblade ? TeamId::TEAM_ALLIANCE : TeamId::TEAM_HORDE);
                if (Creature* l_GenericMoP = sObjectAccessor->FindCreature(l_GenericGuid))
                    l_GenericMoP->AI()->DoAction(me->GetEntry() == eCreatures::GrandMarshalTremblade ? eAshranActions::AnnounceHordeKillBoss : eAshranActions::AnnounceAllianceKillBoss);

                ((OutdoorPvPAshran*)m_ZoneScript)->HandleFactionBossDeath(me->GetEntry() == eCreatures::GrandMarshalTremblade ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE);

                /// Upon successfully defeating the enemy leader, those present receive 50 Honor and 250 Conquest
                std::list<Player*> l_PlayerList;
                me->GetPlayerListInGrid(l_PlayerList, 100.0f);

                l_PlayerList.remove_if([this](Player* p_Player) -> bool
                {
                    if (p_Player == nullptr)
                        return true;

                    if (!me->IsValidAttackTarget(p_Player))
                        return true;

                    return false;
                });

                for (Player* l_Player : l_PlayerList)
                {
                    /// Must do a * 100 because of currency precision
                    l_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_CONQUEST_POINTS, 250 * 100);
                    l_Player->RewardHonor(l_Player, 1, 50 * 100);
                }

                /// Trigger strongboxes loot for near players
                if (me->GetEntry() == eCreatures::GrandMarshalTremblade)
                    p_Killer->CastSpell(p_Killer, eAshranSpells::SpellAllianceReward, true);
                else
                    p_Killer->CastSpell(p_Killer, eAshranSpells::SpellHordeReward, true);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eAshranActions::WarspearOutpostInFight:
                    case eAshranActions::StormshieldStrongholdInFight:
                        Talk(eTalk::TalkIntro);
                        break;
                    case eAshranActions::WarspearVictory:
                    case eAshranActions::StormshieldVictory:
                        Talk(eTalk::TalkVictory);
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Target, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::SpellBladeTwisterSearcher)
                    me->CastSpell(p_Target, eSpells::SpellBladeTwisterMissile, false);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventMortalCleave:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellMortalCleave, false);
                        m_Events.ScheduleEvent(eEvents::EventMortalCleave, 15000);
                        break;
                    case eEvents::EventBladeTwister:
                        me->CastSpell(me, eSpells::SpellBladeTwisterSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventBladeTwister, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void OnHostileReferenceAdded(Unit* p_Ennemy)
            {
                if (p_Ennemy->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (m_FirstVictim)
                {
                    m_FirstVictim = false;
                    return;
                }

                float l_HealthPct = me->GetHealthPct();
                uint32 l_AddedValue = m_BaseHP / 2;

                me->SetMaxHealth(me->GetMaxHealth() + l_AddedValue);
                me->SetHealth(CalculatePct(me->GetMaxHealth(), l_HealthPct));
            }

            void OnHostileReferenceRemoved(Unit* p_Ennemy)
            {
                if (p_Ennemy->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                float l_HealthPct = me->GetHealthPct();
                uint32 l_AddedValue = m_BaseHP / 2;

                if ((me->GetMaxHealth() - l_AddedValue) < m_BaseHP)
                {
                    me->SetMaxHealth(m_BaseHP);
                    me->SetHealth(CalculatePct(m_BaseHP, l_HealthPct));
                    return;
                }

                me->SetMaxHealth(me->GetMaxHealth() - l_AddedValue);
                me->SetHealth(CalculatePct(me->GetMaxHealth(), l_HealthPct));
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_faction_bossAI(p_Creature);
        }
};

/// Jeron Emberfall <Warspear Tower Guardian> - 88178
class npc_jeron_emberfall : public CreatureScript
{
    public:
        npc_jeron_emberfall() : CreatureScript("npc_jeron_emberfall") { }

        struct npc_jeron_emberfallAI : public ScriptedAI
        {
            npc_jeron_emberfallAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CheckAroundingPlayersTimer = 0;
                m_PhoenixStrikeTimer = 0;
            }

            enum eSpells
            {
                Fireball                = 176652,
                Ignite                  = 176600,
                CombustionNova          = 176605,
                CombustionNovaStun      = 176608,
                LivingBomb              = 176670,
                SummonLavaFury          = 176664,

                TargetedByTheTowerMage  = 176076,
                PhoenixStrikeSearcher   = 176086,
                PhoenixStrikeMissile    = 176066
            };

            enum eTalk
            {
                TalkAggro,
                TalkSlay,
                TalkDeath,
                TalkSpell,
                TalkLivingBomb
            };

            enum eEvents
            {
                EventFireball = 1,
                EventIgnite,
                EventLivingBomb,
                EventCombustionNova
            };

            EventMap m_Events;

            uint32 m_CheckAroundingPlayersTimer;
            uint32 m_PhoenixStrikeTimer;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventFireball, 4000);
                m_Events.ScheduleEvent(eEvents::EventIgnite, 8000);
                m_Events.ScheduleEvent(eEvents::EventLivingBomb, 12000);
                m_Events.ScheduleEvent(eEvents::EventCombustionNova, 15000);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(eTalk::TalkDeath);
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo)
            {
                if (p_Victim == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::PhoenixStrikeSearcher:
                        me->CastSpell(p_Victim, eSpells::PhoenixStrikeMissile, false);
                        break;
                    case eSpells::CombustionNova:
                        if (p_Victim->HasAura(eSpells::Ignite))
                            me->CastSpell(p_Victim, eSpells::CombustionNovaStun, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                {
                    ScheduleTargetingPlayers(p_Diff);
                    SchedulePhoenixStrike(p_Diff);
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFireball:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::Fireball, false);
                        m_Events.ScheduleEvent(eEvents::EventFireball, 10000);
                        break;
                    case eEvents::EventIgnite:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::Ignite, true);
                        m_Events.ScheduleEvent(eEvents::EventIgnite, 9000);
                        break;
                    case eEvents::EventLivingBomb:
                        Talk(eTalk::TalkLivingBomb);
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::LivingBomb, false);
                        m_Events.ScheduleEvent(eEvents::EventLivingBomb, 15000);
                        break;
                    case eEvents::EventCombustionNova:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::CombustionNova, false);
                        m_Events.ScheduleEvent(eEvents::EventCombustionNova, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ScheduleTargetingPlayers(uint32 const p_Diff)
            {
                if (!m_CheckAroundingPlayersTimer)
                    return;

                if (m_CheckAroundingPlayersTimer <= p_Diff)
                {
                    m_CheckAroundingPlayersTimer = 2500;

                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 200.0f);

                    l_PlayerList.remove_if([this](Player* p_Player) -> bool
                    {
                        if (p_Player == nullptr)
                            return true;

                        if (!me->IsValidAttackTarget(p_Player))
                            return true;

                        if (p_Player->HasAura(eSpells::TargetedByTheTowerMage))
                            return true;

                        return false;
                    });

                    for (Player* l_Player : l_PlayerList)
                        l_Player->CastSpell(l_Player, eSpells::TargetedByTheTowerMage, true, nullptr, nullptr, me->GetGUID());
                }
                else
                    m_CheckAroundingPlayersTimer -= p_Diff;
            }

            void SchedulePhoenixStrike(uint32 const p_Diff)
            {
                if (!m_PhoenixStrikeTimer)
                    return;

                if (m_PhoenixStrikeTimer <= p_Diff)
                {
                    if (!me->isInCombat())
                        me->CastSpell(me, eSpells::PhoenixStrikeSearcher, true);
                    m_PhoenixStrikeTimer = 10000;
                }
                else
                    m_PhoenixStrikeTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_jeron_emberfallAI(p_Creature);
        }
};

/// Rylai Crestfall <Stormshield Tower Guardian> - 88224
class npc_rylai_crestfall : public CreatureScript
{
    public:
        npc_rylai_crestfall() : CreatureScript("npc_rylai_crestfall") { }

        struct npc_rylai_crestfallAI : public ScriptedAI
        {
            npc_rylai_crestfallAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CheckAroundingPlayersTimer = 0;
                m_FreezingFieldTimer = 0;
            }

            enum eSpells
            {
                Frostbolt       = 176268,
                FrostboltVolley = 176273,
                IceBlock        = 176269,
                Hypotermia      = 41425,
                MassPolymorph   = 176204,
                FrostNovaCasted = 176327,
                NorthrendWinds  = 176267,
                FrostNova       = 176276,
                DeepFreeze      = 176278,
                SummonIceShard  = 177599,   ///< @TODO

                TowerMageTargetingAura  = 176162,   ///< Put on ennemy players around 200 yards
                FreezingFieldSearcher   = 176163,   ///< Launch frost missile on one player targeted
                FreezingFieldMissile    = 176165
            };

            enum eTalk
            {
                TalkAggro,
                TalkSlay,
                TalkDeath,
                TalkSpell
            };

            enum eEvents
            {
                EventFrostbolt = 1,
                EventFrostboltVolley,
                EventMassPolymorph,
                EventFrostNova,
                EventNorthrendWinds
            };

            EventMap m_Events;

            uint32 m_CheckAroundingPlayersTimer;
            uint32 m_FreezingFieldTimer;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);

                m_CheckAroundingPlayersTimer = 2000;
                m_FreezingFieldTimer = 10000;
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventFrostbolt, 4000);
                m_Events.ScheduleEvent(eEvents::EventMassPolymorph, 6000);
                m_Events.ScheduleEvent(eEvents::EventFrostboltVolley, 9000);
                m_Events.ScheduleEvent(eEvents::EventFrostNova, 12500);
                m_Events.ScheduleEvent(eEvents::EventNorthrendWinds, 15000);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(eTalk::TalkDeath);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage)
            {
                if (me->HealthBelowPctDamaged(20, p_Damage) && !me->HasAura(eSpells::Hypotermia))
                {
                    me->CastSpell(me, eSpells::IceBlock, true);
                    me->CastSpell(me, eSpells::Hypotermia, true);
                }
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo)
            {
                if (p_Victim == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::FreezingFieldSearcher:
                        me->CastSpell(p_Victim, eSpells::FreezingFieldMissile, false);
                        break;
                    case eSpells::NorthrendWinds:
                        if (p_Victim->HasAura(eSpells::FrostNova))
                            me->CastSpell(p_Victim, eSpells::DeepFreeze, true);
                        else
                            me->CastSpell(p_Victim, eSpells::FrostNova, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                {
                    ScheduleTargetingPlayers(p_Diff);
                    ScheduleFreezingField(p_Diff);
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFrostbolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::Frostbolt, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostbolt, 10000);
                        break;
                    case eEvents::EventFrostboltVolley:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::FrostboltVolley, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostboltVolley, 20000);
                        break;
                    case eEvents::EventMassPolymorph:
                        me->CastSpell(me, eSpells::MassPolymorph, false);
                        m_Events.ScheduleEvent(eEvents::EventMassPolymorph, 25000);
                        break;
                    case eEvents::EventFrostNova:
                        me->CastSpell(me, eSpells::FrostNovaCasted, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostNova, 27500);
                        break;
                    case eEvents::EventNorthrendWinds:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::NorthrendWinds, false);
                        m_Events.ScheduleEvent(eEvents::EventNorthrendWinds, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ScheduleTargetingPlayers(uint32 const p_Diff)
            {
                if (!m_CheckAroundingPlayersTimer)
                    return;

                if (m_CheckAroundingPlayersTimer <= p_Diff)
                {
                    m_CheckAroundingPlayersTimer = 2500;

                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 200.0f);

                    l_PlayerList.remove_if([this](Player* p_Player) -> bool
                    {
                        if (p_Player == nullptr)
                            return true;

                        if (!me->IsValidAttackTarget(p_Player))
                            return true;

                        if (p_Player->HasAura(eSpells::TowerMageTargetingAura))
                            return true;

                        return false;
                    });

                    for (Player* l_Player : l_PlayerList)
                        l_Player->CastSpell(l_Player, eSpells::TowerMageTargetingAura, true, nullptr, nullptr, me->GetGUID());
                }
                else
                    m_CheckAroundingPlayersTimer -= p_Diff;
            }

            void ScheduleFreezingField(uint32 const p_Diff)
            {
                if (!m_FreezingFieldTimer)
                    return;

                if (m_FreezingFieldTimer <= p_Diff)
                {
                    if (!me->isInCombat())
                        me->CastSpell(me, eSpells::FreezingFieldSearcher, true);
                    m_FreezingFieldTimer = 10000;
                }
                else
                    m_FreezingFieldTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_rylai_crestfallAI(p_Creature);
        }
};

/// Shevan Manille <Flight Master> - 87672
/// Tina Kelatara <Flight Master> - 87617
class npc_ashran_flight_masters : public CreatureScript
{
    public:
        npc_ashran_flight_masters() : CreatureScript("npc_ashran_flight_masters") { }

        bool OnGossipSelect(Player* p_Player, Creature*, uint32, uint32)
        {
            if (p_Player == nullptr || !p_Player->IsInWorld())
                return true;

            if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
                p_Player->ActivateTaxiPathTo(eAshranDatas::TaxiPathBaseHordeToAlliance, 0, true);
            else
                p_Player->ActivateTaxiPathTo(eAshranDatas::TaxiPathBaseAllianceToHorde, 0, true);

            return false;
        }

        struct npc_ashran_flight_mastersAI : public ScriptedAI
        {
            npc_ashran_flight_mastersAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_ATTACKABLE_1 | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_flight_mastersAI(p_Creature);
        }
};

/// Alliance Spirit Guide - 80723
/// Horde Spirit Guide - 80724
class npc_ashran_spirit_healer : public CreatureScript
{
    public:
        npc_ashran_spirit_healer() : CreatureScript("npc_ashran_spirit_healer") { }

        struct npc_ashran_spirit_healerAI : public ScriptedAI
        {
            npc_ashran_spirit_healerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->setDeathState(DeathState::DEAD);
                me->SetGuidValue(EUnitFields::UNIT_FIELD_CHANNEL_OBJECT, me->GetGUID());
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_CHANNEL_SPELL, eAshranSpells::SpellSpiritHeal);
                me->SetFloatValue(EUnitFields::UNIT_FIELD_MOD_CASTING_SPEED, 1.0f);
                me->SetFloatValue(EUnitFields::UNIT_FIELD_MOD_SPELL_HASTE, 1.0f);
                DoCast(me, eAshranSpells::SpellSpiritHeal);
            }

            void JustRespawned() { }

            void UpdateAI(uint32 const)
            {
                if (!me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    DoCast(me, eAshranSpells::SpellSpiritHeal);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_spirit_healerAI(p_Creature);
        }
};

/// Blade Twister - 178795
class spell_blade_twister: public SpellScriptLoader
{
    public:
        spell_blade_twister() : SpellScriptLoader("spell_blade_twister") { }

        class spell_blade_twister_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_blade_twister_AuraScript);

            enum eSpells
            {
                BladeTwisterDamage = 177167
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    std::list<Creature*> l_Blades;
                    l_Target->GetCreatureListWithEntryInGrid(l_Blades, eCreatures::BladeTwisterTrigger, 50.0f);

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
                        l_Target->CastSpell(l_Creature, eSpells::BladeTwisterDamage, true);
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

/// Living Bomb - 176670
class spell_emberfall_living_bomb: public SpellScriptLoader
{
    public:
        spell_emberfall_living_bomb() : SpellScriptLoader("spell_emberfall_living_bomb") { }

        class spell_emberfall_living_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_emberfall_living_bomb_AuraScript);

            enum eSpells
            {
                LivingBombTriggered = 176673
            };

            void AfterRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode != AuraRemoveMode::AURA_REMOVE_BY_DEATH && removeMode != AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, eSpells::LivingBombTriggered, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_emberfall_living_bomb_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_emberfall_living_bomb_AuraScript();
        }
};

/// Alliance Reward - 178531
/// Horde Reward - 178533
class spell_ashran_faction_rewards : public SpellScriptLoader
{
    public:
        spell_ashran_faction_rewards() : SpellScriptLoader("spell_ashran_faction_rewards") { }

        class spell_ashran_faction_rewards_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_faction_rewards_SpellScript);

            enum Items
            {
                StrongboxHorde      = 120151,
                StrongboxAlliance   = 118065
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return true;

                    Player* l_Player = p_Object->ToPlayer();
                    if (l_Player == nullptr)
                        return true;

                    /// Only one strongbox per day
                    if (!l_Player->CanHaveDailyLootForItem(Items::StrongboxAlliance) ||
                        !l_Player->CanHaveDailyLootForItem(Items::StrongboxHorde))
                        return true;

                    return false;
                });
            }

            void HandleOnHit()
            {
                if (GetHitUnit() == nullptr)
                    return;

                if (Player* l_Player = GetHitUnit()->ToPlayer())
                {
                    if (GetSpellInfo()->Id == eAshranSpells::SpellAllianceReward)
                        l_Player->AddDailyLootCooldown(Items::StrongboxHorde);
                    else
                        l_Player->AddDailyLootCooldown(Items::StrongboxAlliance);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_faction_rewards_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnHit += SpellHitFn(spell_ashran_faction_rewards_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ashran_faction_rewards_SpellScript();
        }
};

void AddSC_OutdoorPvPAshran()
{
    new OutdoorPvP_Ashran();

    new npc_ashran_herald();
    new npc_slg_generic_mop();
    new npc_faction_boss();
    new npc_jeron_emberfall();
    new npc_rylai_crestfall();
    new npc_ashran_flight_masters();
    new npc_ashran_spirit_healer();

    new spell_blade_twister();
    new spell_emberfall_living_bomb();
    new spell_ashran_faction_rewards();
}
