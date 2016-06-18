////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

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
        case ObjectiveStates::OBJECTIVESTATE_ALLIANCE:
            m_BattleFaction = eControlStatus::ControlAlliance;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = eFlagStates::FlagAlliance;
            SendUpdateWorldState(eWorldStates::WorldStateEnableTowerProgressBar, eWorldStates::WorldStateDisabled);
            break;
        case ObjectiveStates::OBJECTIVESTATE_HORDE:
            m_BattleFaction = eControlStatus::ControlHorde;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = eFlagStates::FlagHorde;
            SendUpdateWorldState(eWorldStates::WorldStateEnableTowerProgressBar, eWorldStates::WorldStateDisabled);
            break;
        case ObjectiveStates::OBJECTIVESTATE_NEUTRAL:
        case ObjectiveStates::OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case ObjectiveStates::OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case ObjectiveStates::OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case ObjectiveStates::OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
            m_BattleFaction = eControlStatus::ControlNeutral;
            SpawnFactionGuards(m_BattleType, m_BattleFaction);
            l_UpdateVal = eFlagStates::FlagNeutral;
            break;
        default:
            break;
    }

    GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID);
    if (l_Flag)
        l_Flag->SetByteValue(EGameObjectFields::GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, l_UpdateVal);

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

    bool l_MustChangeKillCap = false;

    switch (p_BattleID)
    {
        case eBattleType::EmberfallTower:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                /// Spawn fighting guards
                /// Stormshield Footman VS Warspear Grunt
                for (uint8 l_Count = 0; l_Count < eSpawns::EmberfallTowerCreaturesCount; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_EmberfallTowerNeutralSpawns[l_Count], 5);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, eFlagStates::FlagNeutral);

                DelCreature(eSpecialSpawns::EmberfallTowerSpiritHealer);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(5))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateHighWarlordVolrath : eWorldStates::WorldStateVolrathsAdvanceBattle);
                }

                /// Spawn horde or alliance guards
                for (uint8 l_Count = 0; l_Count < eSpawns::EmberfallTowerCreaturesCount; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_EmberfallTowerSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count], 2 * TimeConstants::MINUTE);
                }

                /// Spawn fires
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

                if (p_Faction == eControlStatus::ControlAlliance)
                    l_MustChangeKillCap = true;
            }

            break;
        }
        case eBattleType::VolrathsAdvance:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                /// Spawn fighting guards
                /// Stormshield Footman VS Warspear Grunt
                uint8 l_CreatureMaxIndex = eSpawns::EmberfallTowerSpawnsIDs + eSpawns::VolrathsAdvanceCreaturesCount;
                for (uint8 l_Count = eSpawns::EmberfallTowerSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_VolrathsAdvanceNeutralSpawns[l_Count - eSpawns::EmberfallTowerSpawnsIDs], 5);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, eFlagStates::FlagNeutral);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateEmberfallTowerBattle : eWorldStates::WorldStateTheCrossroadsBattle);
                }

                /// Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::EmberfallTowerSpawnsIDs + eSpawns::VolrathsAdvanceCreaturesCount;
                for (uint8 l_Count = eSpawns::EmberfallTowerSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_VolrathsAdvanceSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::EmberfallTowerSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                /// Spawn fires
                uint8 l_FireIndex = eSpawns::EmberfallTowerSpawnsIDs + eSpawns::VolrathsAdvanceCreaturesCount;
                for (uint8 l_Index = l_FireIndex; l_Index < eSpawns::VolrathsAdvanceSpawnsIDs; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == eControlStatus::ControlAlliance)
                        AddObject(l_Index, g_VolrathsAdvanceFires[l_Index - l_FireIndex]);
                }

                if (p_Faction == eControlStatus::ControlAlliance)
                    l_MustChangeKillCap = true;
            }

            break;
        }
        case eBattleType::TheCrossroads:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                /// Spawn fighting guards
                /// Stormshield Footman VS Warspear Grunt
                uint8 l_CreatureMaxIndex = eSpawns::VolrathsAdvanceSpawnsIDs + eSpawns::TheCrossroadsCreaturesCount;
                for (uint8 l_Count = eSpawns::VolrathsAdvanceSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_CrossroadsNeutralSpawns[l_Count - eSpawns::VolrathsAdvanceSpawnsIDs], 5);
                }

                uint8 l_FlagIndex = eSpawns::VolrathsAdvanceSpawnsIDs + eSpawns::TheCrossroadsCreaturesCount;
                for (uint8 l_Count = l_FlagIndex; l_Count < eSpawns::TheCrossroadsSpawnsIDs; ++l_Count)
                    DelObject(l_Count);

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, eFlagStates::FlagNeutral);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateVolrathsAdvanceBattle : eWorldStates::WorldStateTrembladesVanguardBattle);
                }

                /// Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::VolrathsAdvanceSpawnsIDs + eSpawns::TheCrossroadsCreaturesCount;
                for (uint8 l_Count = eSpawns::VolrathsAdvanceSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_CrossroadSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::VolrathsAdvanceSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                /// Spawn flags
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
                /// Spawn fighting guards
                /// Stormshield Footman VS Warspear Grunt
                uint8 l_CreatureMaxIndex = eSpawns::TheCrossroadsSpawnsIDs + eSpawns::TrembladesVanguardCreaturesCount;
                for (uint8 l_Count = eSpawns::TheCrossroadsSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_TrembladesVanguardNeutralSpawns[l_Count - eSpawns::TheCrossroadsSpawnsIDs], 5);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, eFlagStates::FlagNeutral);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateTheCrossroadsBattle : eWorldStates::WorldStateArchmageOverwatchBattle);
                }

                /// Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::TheCrossroadsSpawnsIDs + eSpawns::TrembladesVanguardCreaturesCount;
                for (uint8 l_Count = eSpawns::TheCrossroadsSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_TrembladesVanguardSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::TheCrossroadsSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                /// Spawn fires
                uint8 l_FireIndex = eSpawns::TheCrossroadsSpawnsIDs + eSpawns::TrembladesVanguardCreaturesCount;
                for (uint8 l_Index = l_FireIndex; l_Index < eSpawns::TrembladesVanguardSpawnsIDs; ++l_Index)
                {
                    DelObject(l_Index);

                    if (p_Faction == eControlStatus::ControlHorde)
                        AddObject(l_Index, g_TrembladesVanguardFires[l_Index - l_FireIndex]);
                }

                if (p_Faction == eControlStatus::ControlHorde)
                    l_MustChangeKillCap = true;
            }

            break;
        }
        case eBattleType::ArchmageOverwatch:
        {
            if (p_Faction == eControlStatus::ControlNeutral)
            {
                /// Despawn horde or alliance guards
                /// Spawn fighting guards
                uint8 l_CreatureMaxIndex = eSpawns::TrembladesVanguardSpawnsIDs + eSpawns::ArchmageOverwatchCreaturesCount;
                for (uint8 l_Index = eSpawns::TrembladesVanguardSpawnsIDs; l_Index < l_CreatureMaxIndex; ++l_Index)
                {
                    DelCreature(l_Index);
                    AddCreature(l_Index, g_ArchmageOverwatchNeutral[l_Index - eSpawns::TrembladesVanguardSpawnsIDs], 5);
                }

                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, eFlagStates::FlagNeutral);

                DelCreature(eSpecialSpawns::ArchmageOverwatchSpiritHealer);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(4))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);
            }
            else
            {
                if (GameObject* l_Flag = sObjectAccessor->FindGameObject(m_capturePointGUID))
                    l_Flag->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, p_Faction == eControlStatus::ControlHorde ? eFlagStates::FlagHorde : eFlagStates::FlagAlliance);

                if (l_IsInitialized)
                {
                    ((OutdoorPvPAshran*)m_PvP)->SetNextBattleTimer(eAshranDatas::AshranTimeForBattle);
                    ((OutdoorPvPAshran*)m_PvP)->SetBattleState(p_Faction == eControlStatus::ControlAlliance ? eWorldStates::WorldStateTrembladesVanguardBattle : eWorldStates::WorldStateGrandMarshalTrembladeBattle);
                }

                /// Despawn fighting guards
                for (uint8 l_Index = eSpawns::ArchmageOverwatchSpawnsIDs; l_Index < eSpawns::ArchmageOverwatchSpawnsIDs; ++l_Index)
                    DelCreature(l_Index);

                /// Spawn horde or alliance guards
                uint8 l_CreatureMaxIndex = eSpawns::TrembladesVanguardSpawnsIDs + eSpawns::ArchmageOverwatchCreaturesCount;
                for (uint8 l_Count = eSpawns::TrembladesVanguardSpawnsIDs; l_Count < l_CreatureMaxIndex; ++l_Count)
                {
                    DelCreature(l_Count);
                    AddCreature(l_Count, g_ArchmageOverwatchSpawns[p_Faction == eControlStatus::ControlAlliance ? TEAM_ALLIANCE : TEAM_HORDE][l_Count - eSpawns::TrembladesVanguardSpawnsIDs], 2 * TimeConstants::MINUTE);
                }

                uint8 l_FireIndex = eSpawns::TrembladesVanguardSpawnsIDs + eSpawns::ArchmageOverwatchCreaturesCount;
                /// Spawn fires
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

                if (p_Faction == eControlStatus::ControlHorde)
                    l_MustChangeKillCap = true;
            }

            break;
        }
        default:
            break;
    }

    /// If a faction loses a Tower on their own side of the Road of Glory, their quota will be reset to 50.
    if (l_MustChangeKillCap)
        ((OutdoorPvPAshran*)m_PvP)->ResetKillCap(p_Faction == eControlStatus::ControlAlliance ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE);
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
        case ObjectiveStates::OBJECTIVESTATE_ALLIANCE:
        {
            m_GraveyardState = eControlStatus::ControlAlliance;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = eFlagStates::FlagAlliance;
            m_ControlTime = 15 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(eAshranActions::AnnounceAllianceGraveyard);

            SendUpdateWorldState(eWorldStates::WorldStateEnableGraveyardProgressBar, eWorldStates::WorldStateDisabled);
            break;
        }
        case ObjectiveStates::OBJECTIVESTATE_HORDE:
        {
            m_GraveyardState = eControlStatus::ControlHorde;
            SpawnFactionFlags(m_GraveyardState);
            l_UpdateVal = eFlagStates::FlagHorde;
            m_ControlTime = 15 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;

            if (Creature* l_Herald = ((OutdoorPvPAshran*)m_PvP)->GetHerald())
                l_Herald->AI()->DoAction(eAshranActions::AnnounceHordeGraveyard);

            SendUpdateWorldState(eWorldStates::WorldStateEnableGraveyardProgressBar, eWorldStates::WorldStateDisabled);
            break;
        }
        case ObjectiveStates::OBJECTIVESTATE_NEUTRAL:
        case ObjectiveStates::OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
        case ObjectiveStates::OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
        case ObjectiveStates::OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
        case ObjectiveStates::OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
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
        l_Flag->SetByteValue(EGameObjectFields::GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, l_UpdateVal);

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
    switch (m_GraveyardState)
    {
        case eControlStatus::ControlNeutral:
            p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForAlliance) << uint32(eWorldStates::WorldStateDisabled);
            p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForHorde) << uint32(eWorldStates::WorldStateDisabled);
            break;
        case eControlStatus::ControlAlliance:
            p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForAlliance) << uint32(eWorldStates::WorldStateEnabled);
            break;
        case eControlStatus::ControlHorde:
            p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForHorde) << uint32(eWorldStates::WorldStateEnabled);
            break;
        default:
            break;
    }
}

void OPvPCapturePoint_Graveyard::UpdateTowerState()
{
    if (m_PvP == nullptr)
        return;

    switch (m_GraveyardState)
    {
        case eControlStatus::ControlNeutral:
            m_PvP->SendUpdateWorldState(eWorldStates::WorldStateGraveyardStatusForAlliance, eWorldStates::WorldStateDisabled);
            m_PvP->SendUpdateWorldState(eWorldStates::WorldStateGraveyardStatusForHorde, eWorldStates::WorldStateDisabled);
            break;
        case eControlStatus::ControlAlliance:
            m_PvP->SendUpdateWorldState(eWorldStates::WorldStateGraveyardStatusForAlliance, eWorldStates::WorldStateEnabled);
            break;
        case eControlStatus::ControlHorde:
            m_PvP->SendUpdateWorldState(eWorldStates::WorldStateGraveyardStatusForHorde, eWorldStates::WorldStateEnabled);
            break;
        default:
            break;
    }
}

bool OPvPCapturePoint_Graveyard::HandlePlayerEnter(Player* p_Player)
{
    if (p_Player == nullptr)
        return false;

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
    if (p_Player == nullptr)
        return;

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

                DelCreature(eSpecialSpawns::MarketplaceGraveyardSpiritHealer);
                AddCreature(eSpecialSpawns::MarketplaceGraveyardSpiritHealer, g_MarketplaceGraveyardSpirits[TeamId::TEAM_ALLIANCE]);
                break;
            case eControlStatus::ControlHorde:
                AddObject(l_Index, g_GraveyardBanner_H[l_Index]);

                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(2))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_NEUTRAL);
                if (OutdoorGraveyard* l_Graveyard = m_PvP->GetGraveyardById(3))
                    l_Graveyard->GiveControlTo(TeamId::TEAM_HORDE);

                DelCreature(eSpecialSpawns::MarketplaceGraveyardSpiritHealer);
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
    m_CurrentBattleState    = eWorldStates::WorldStateTheCrossroadsBattle;
    m_NextBattleTimer       = eAshranDatas::AshranTimeForBattle * TimeConstants::IN_MILLISECONDS;
    m_MaxBattleTime         = 0;
    m_GladiatorRespawnTime  = 0;
    m_AncientArtifactTime   = 0;

    m_PlayerCurrencyLoots.clear();
    m_NeutralVignettes.clear();
    m_ActiveCaptains.clear();

    m_Guid = MAKE_NEW_GUID(m_WorldPvPAreaId, 0, HighGuid::HIGHGUID_TYPE_BATTLEGROUND);
    m_Guid |= eAshranDatas::BattlefieldWorldPvP;

    for (uint8 l_Team = 0; l_Team < TeamId::TEAM_NEUTRAL; ++l_Team)
    {
        m_PlayersInWar[l_Team].clear();
        m_InvitedPlayers[l_Team].clear();
        m_PlayersWillBeKick[l_Team].clear();
        m_FactionVignettes[l_Team].clear();

        m_EnnemiesKilled[l_Team]    = 0;
        m_EnnemiesKilledMax[l_Team] = eAshranDatas::EnnemiesSlainCap2;
        m_FactionGenericMoP[l_Team] = 0;
        m_StadiumRacingLaps[l_Team] = 0;

        for (uint8 l_I = 0; l_I < eArtifactsDatas::MaxArtifactCounts; ++l_I)
        {
            m_ArtifactsNPCGuids[l_Team][l_I] = 0;
            m_ArtifactsCollected[l_Team][l_I] = 0;
            m_ArtifactEventsLaunched[l_Team][l_I] = false;
        }
    }

    for (uint8 l_Iter = 0; l_Iter < eBattleType::MaxBattleType; ++l_Iter)
        m_GenericMoPGuids[l_Iter] = 0;

    for (uint8 l_Index = 0; l_Index < eAshranEvents::MaxEvents; ++l_Index)
    {
        m_AshranEvents[l_Index] = 0;
        m_AshranEventsWarned[l_Index] = false;
        m_AshranEventsLaunched[l_Index] = false;
    }

    AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[0], 5 * TimeConstants::MINUTE);
    AddCreature(eSpecialSpawns::AllianceMarshalKarshStormforge, g_FactionBossesGuardians[0], 5 * TimeConstants::MINUTE);
    AddCreature(eSpecialSpawns::AllianceMarshalGabriel, g_FactionBossesGuardians[1], 5 * TimeConstants::MINUTE);
    AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[3], 5 * TimeConstants::MINUTE);
    AddCreature(eSpecialSpawns::HordeGeneralAevd, g_FactionBossesGuardians[6], 5 * TimeConstants::MINUTE);
    AddCreature(eSpecialSpawns::HordeWarlordNoktyn, g_FactionBossesGuardians[7], 5 * TimeConstants::MINUTE);
}

bool OutdoorPvPAshran::SetupOutdoorPvP()
{
    RegisterZone(eAshranDatas::AshranZoneID);

    for (uint8 l_BattleIndex = eBattleType::EmberfallTower; l_BattleIndex < eBattleType::MaxBattleType; ++l_BattleIndex)
    {
        if (g_MiddleBattlesEntries[l_BattleIndex] == m_CurrentBattleState)
        {
            m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eBattleType(l_BattleIndex), eControlStatus::ControlNeutral);
            m_ControlPoints[l_BattleIndex]->SetState(ObjectiveStates::OBJECTIVESTATE_NEUTRAL);
        }
        else
        {
            if (l_BattleIndex < 2)
            {
                m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eBattleType(l_BattleIndex), eControlStatus::ControlHorde);
                m_ControlPoints[l_BattleIndex]->SetState(ObjectiveStates::OBJECTIVESTATE_HORDE);
            }
            else
            {
                m_ControlPoints[l_BattleIndex] = new OPvPCapturePoint_Middle(this, eBattleType(l_BattleIndex), eControlStatus::ControlAlliance);
                m_ControlPoints[l_BattleIndex]->SetState(ObjectiveStates::OBJECTIVESTATE_ALLIANCE);
            }
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

    /// Summon the two faction guardians
    AddCreature(eSpecialSpawns::AllianceGuardian, g_AllianceGuardian);
    AddCreature(eSpecialSpawns::HordeGuardian, g_HordeGuardian);

    /// Summon an Ancient Artifact at a random pos
    AddObject(eSpecialSpawns::AncientArtifactSpawn, g_AncientArtifactPos[urand(0, eAshranDatas::AncientArtifactCount - 1)]);

    return true;
}

void OutdoorPvPAshran::HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != eAshranDatas::AshranMapID)
        return;

    if (!p_Player || p_Player->GetTeamId() >= 2 || p_Player->isInFlight())
        return;

    /// If the player does not match minimal level requirements for the battlefield, kick him
    if (p_Player->getLevel() < eAshranDatas::PlayerMinLevel)
    {
        if (m_PlayersWillBeKick[p_Player->GetTeamId()].count(p_Player->GetGUID()) == 0)
            m_PlayersWillBeKick[p_Player->GetTeamId()][p_Player->GetGUID()] = uint32(time(nullptr)) + 10;
        return;
    }

    /// Check if player is not already in war or invited
    if (m_PlayersInWar[p_Player->GetTeamId()].count(p_Player->GetGUID()) || m_InvitedPlayers[p_Player->GetTeamId()].count(p_Player->GetGUID()))
        return;

    m_InvitedPlayers[p_Player->GetTeamId()][p_Player->GetGUID()] = uint32(time(nullptr)) + eAshranDatas::AshranTimeForInvite;

    /// Hotfix (2014-12-15): Players are now removed from a Dungeon Finder or Raid Finder group upon accepting the queue to enter Ashran.
    {
        Group* l_Group = p_Player->GetGroup();

        /// Check cheating - only leader can leave the queue
        if (!l_Group || l_Group->GetLeaderGUID() == p_Player->GetGUID())
            sLFGMgr->Leave(p_Player, l_Group);
    }

    /// Sending the packet to player
    WorldPacket l_Data(Opcodes::SMSG_BFMGR_ENTRY_INVITE);
    l_Data << uint64(m_Guid);                                           ///< QueueID
    l_Data << uint32(eAshranDatas::AshranZoneID);                       ///< Zone Id
    l_Data << uint32(time(NULL) + eAshranDatas::AshranTimeForInvite);   ///< Invite lasts until
    p_Player->SendDirectMessage(&l_Data);

    p_Player->CastSpell(p_Player, eAshranSpells::SpellLootable, true);

    /// Give first quest to player
    if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
        p_Player->CastSpell(p_Player, eAshranSpells::WelcomeToAshranAlliance, true);
    else
        p_Player->CastSpell(p_Player, eAshranSpells::WelcomeToAshranHorde, true);

    if (!m_IsInitialized && !m_InitPointsTimer)
    {
        /// Force the loading of all grids of Ashran for the first time
        p_Player->GetMap()->LoadAllGrids(3700.0f, 5100.0f, -5050.0f, -3510.0f, p_Player);
        /// Temp disable, that maybe cause lags.
        //p_Player->GetMap()->SetObjectVisibility(300.0f);
        m_InitPointsTimer = 2000;
    }

    /// Init vignettes for Player
    for (auto l_VignetteItr : m_NeutralVignettes)
    {
        if (VignetteEntry const* l_Vignette = sVignetteStore.LookupEntry(l_VignetteItr.first))
        {
            if (Creature* l_Creature = Creature::GetCreature(*p_Player, l_VignetteItr.second))
            {
                G3D::Vector3 l_Pos = G3D::Vector3(l_Creature->GetPositionX(), l_Creature->GetPositionY(), l_Creature->GetPositionZ());
                p_Player->GetVignetteMgr().CreateAndAddVignette(l_Vignette, eAshranDatas::AshranMapID, Vignette::Type::SourceScript, l_Pos, l_VignetteItr.second);
            }
        }
    }

    if (p_Player->GetTeamId() < TeamId::TEAM_NEUTRAL)
    {
        for (auto l_VignetteItr : m_FactionVignettes[p_Player->GetTeamId()])
        {
            if (VignetteEntry const* l_Vignette = sVignetteStore.LookupEntry(l_VignetteItr.first))
            {
                if (Creature* l_Creature = Creature::GetCreature(*p_Player, l_VignetteItr.second))
                {
                    G3D::Vector3 l_Pos = G3D::Vector3(l_Creature->GetPositionX(), l_Creature->GetPositionY(), l_Creature->GetPositionZ());
                    p_Player->GetVignetteMgr().CreateAndAddVignette(l_Vignette, eAshranDatas::AshranMapID, Vignette::Type::SourceScript, l_Pos, l_VignetteItr.second);
                }
            }
        }
    }
}

void OutdoorPvPAshran::HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != eAshranDatas::AshranMapID || p_Player == nullptr)
        return;

    if (p_Player->GetTeamId() < TeamId::TEAM_NEUTRAL)
    {
        m_InvitedPlayers[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersInWar[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersWillBeKick[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_Players[p_Player->GetTeamId()].erase(p_Player->GetGUID());
    }

    SendRemoveWorldStates(p_Player);
    p_Player->GetSession()->SendBfLeaveMessage(m_Guid);

    p_Player->RemoveAura(eAshranSpells::SpellLootable);

    if (m_PlayerCurrencyLoots.find(p_Player->GetGUID()) != m_PlayerCurrencyLoots.end())
        m_PlayerCurrencyLoots.erase(p_Player->GetGUID());

    /// Clean all Ashran vignettes for player
    for (auto l_VignetteItr : m_NeutralVignettes)
    {
        VignetteEntry const* l_Vignette = sVignetteStore.LookupEntry(l_VignetteItr.first);
        if (l_Vignette == nullptr)
            continue;

        p_Player->GetVignetteMgr().DestroyAndRemoveVignetteByEntry(l_Vignette);
    }

    if (p_Player->GetTeamId() < TeamId::TEAM_NEUTRAL)
    {
        for (auto l_VignetteItr : m_FactionVignettes[p_Player->GetTeamId()])
        {
            VignetteEntry const* l_Vignette = sVignetteStore.LookupEntry(l_VignetteItr.first);
            if (l_Vignette == nullptr)
                continue;

            p_Player->GetVignetteMgr().DestroyAndRemoveVignetteByEntry(l_Vignette);
        }
    }
}

void OutdoorPvPAshran::HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID)
{
    if (p_Player == nullptr || p_Player->getLevel() < MAX_LEVEL)
        return;

    if (p_Player->GetMapId() != eAshranDatas::AshranMapID)
        return;

    if (p_AreaID == eAshranDatas::AshranPreAreaHorde || p_AreaID == eAshranDatas::AshranPreAreaAlliance)
    {
        uint64 l_Guid = p_Player->GetGUID();

        sMapMgr->AddCriticalOperation([l_Guid]() -> bool
        {
            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
                l_Player->SwitchToPhasedMap(eAshranDatas::AshranNeutralMapID);

            return true;
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
    if (p_Player == nullptr || p_Player->isInFlight() || p_Player->getLevel() < MAX_LEVEL)
        return;

    if (p_Player->GetMapId() == eAshranDatas::AshranNeutralMapID)
    {
        if (p_AreaID == eAshranDatas::AshranPreAreaHorde || p_AreaID == eAshranDatas::AshranPreAreaAlliance)
        {
            uint64 l_Guid = p_Player->GetGUID();

            sMapMgr->AddCriticalOperation([l_Guid]() -> bool
            {
                if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
                    l_Player->SwitchToPhasedMap(eAshranDatas::AshranMapID);

                return true;
            });
        }
    }
    else
    {
        if (p_AreaID == eAshranDatas::AshranHordeBase || p_AreaID == eAshranDatas::AshranAllianceBase)
            p_Player->RemoveAura(eAshranSpells::SpellHoldYourGround);
        else if (p_AreaID == eAshranDatas::EmberfallTowerAreaID || p_AreaID == eAshranDatas::ArchmageOverwatchAreaID)
            p_Player->RemoveAura(eAshranSpells::SpellTowerDefense);
        else if (p_AreaID == eAshranDatas::VolrathsAdvanceAreaID || p_AreaID == eAshranDatas::TrembladesVanguardAreaID)
            p_Player->RemoveAura(eAshranSpells::SpellStandFast);
    }
}

void OutdoorPvPAshran::HandlePlayerResurrects(Player* p_Player, uint32 /*p_ZoneID*/)
{
    if (m_PlayerCurrencyLoots.find(p_Player->GetGUID()) == m_PlayerCurrencyLoots.end())
        return;

    m_PlayerCurrencyLoots.erase(p_Player->GetGUID());
}

void OutdoorPvPAshran::HandlePlayerKilled(Player* p_Player)
{
    if (p_Player == nullptr)
        return;

    /// Drop half of artifact fragments at player death
    /// Even if he's killed by a creature
    if (uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, false))
    {
        l_ArtifactCount /= 2;
        p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount));
        m_PlayerCurrencyLoots.insert(std::make_pair(p_Player->GetGUID(), l_ArtifactCount / CURRENCY_PRECISION));
    }

    /// Players can be looted by other players
    p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_SKINNABLE);
}

void OutdoorPvPAshran::HandleKill(Player* p_Killer, Unit* p_Killed)
{
    std::string l_Str = p_Killer->GetSession()->GetTrinityString(TrinityStrings::LangDisplaySlainCounter);
    uint8 l_SlayCount = 0;

    if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
        l_SlayCount = eAshranDatas::KillCountForPlayer;
    else if (IsFactionGuard(p_Killed))  ///< Only for Road of Glory
        l_SlayCount = eAshranDatas::KillCountForFactionGuard;

    if (l_SlayCount > 0)
    {
        p_Killed->SendItemBonusDebug(l_SlayCount, l_Str);

        TeamId l_Team = p_Killer->GetTeamId();
        if (l_Team < TeamId::TEAM_NEUTRAL)
        {
            /// Actual cap is not reached, just increase
            if ((m_EnnemiesKilled[l_Team] + l_SlayCount) < m_EnnemiesKilledMax[l_Team])
            {
                m_EnnemiesKilled[l_Team] += l_SlayCount;

                if (l_Team == TeamId::TEAM_ALLIANCE)
                    SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAlliance, m_EnnemiesKilled[l_Team]);
                else
                    SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHorde, m_EnnemiesKilled[l_Team]);
            }
            /// Actual cap is reached, increase cap if needed, and spawn a guardian
            else
            {
                uint8 l_Needed = m_EnnemiesKilledMax[l_Team] - m_EnnemiesKilled[l_Team];
                l_SlayCount -= l_Needed;

                m_EnnemiesKilled[l_Team] = l_SlayCount;

                switch (m_EnnemiesKilledMax[l_Team])
                {
                    case eAshranDatas::EnnemiesSlainCap1:
                        m_EnnemiesKilledMax[l_Team] = eAshranDatas::EnnemiesSlainCap2;
                        break;
                    case eAshranDatas::EnnemiesSlainCap2:
                        m_EnnemiesKilledMax[l_Team] = eAshranDatas::EnnemiesSlainCap3;
                        break;
                    case eAshranDatas::EnnemiesSlainCap3:
                        m_EnnemiesKilledMax[l_Team] = eAshranDatas::EnnemiesSlainCap4;
                        break;
                    case eAshranDatas::EnnemiesSlainCap4:
                        m_EnnemiesKilledMax[l_Team] = eAshranDatas::EnnemiesSlainCap5;
                        break;
                    case eAshranDatas::EnnemiesSlainCap5:
                        m_EnnemiesKilledMax[l_Team] = eAshranDatas::EnnemiesSlainCap6;
                        break;
                    /// This is the max cap
                    case eAshranDatas::EnnemiesSlainCap6:
                    default:
                        break;
                }

                if (l_Team == TeamId::TEAM_ALLIANCE)
                {
                    SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAlliance, m_EnnemiesKilled[l_Team]);
                    SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAllianceMax, m_EnnemiesKilledMax[l_Team]);
                }
                else
                {
                    SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHorde, m_EnnemiesKilled[l_Team]);
                    SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHordeMax, m_EnnemiesKilledMax[l_Team]);
                }

                creature_type l_Spawn;
                AshranCaptain l_Captain;
                uint8 l_MaxLoop = 255;

                do
                {
                    --l_MaxLoop;

                    if (l_Team == TeamId::TEAM_ALLIANCE)
                    {
                        l_Captain = g_AshranCaptains[urand(0, eAshranCaptains::MaxAllianceCaptains - 1)];
                        l_Spawn.teamval = Team::ALLIANCE;
                        l_Spawn.x = g_FactionBossesSpawn[2].x;
                        l_Spawn.y = g_FactionBossesSpawn[2].y;
                        l_Spawn.z = g_FactionBossesSpawn[2].z;
                        l_Spawn.o = g_FactionBossesSpawn[2].o;
                    }
                    else
                    {
                        l_Captain = g_AshranCaptains[urand(eAshranCaptains::MaxAllianceCaptains, eSpecialSpawns::MaxAshranCaptains - 1)];
                        l_Spawn.teamval = Team::HORDE;
                        l_Spawn.x = g_FactionBossesSpawn[5].x;
                        l_Spawn.y = g_FactionBossesSpawn[5].y;
                        l_Spawn.z = g_FactionBossesSpawn[5].z;
                        l_Spawn.o = g_FactionBossesSpawn[5].o;
                    }

                    l_Spawn.entry = l_Captain.Entry;
                    l_Spawn.map = eAshranDatas::AshranMapID;

                    /// Don't spawn the same Captain twice at the same time if it's possible
                    if (m_ActiveCaptains.find(l_Captain.Type) != m_ActiveCaptains.end())
                        l_Captain = AshranCaptain();
                }
                while (!l_Captain.Entry || l_MaxLoop > 0);

                m_ActiveCaptains.insert(l_Captain.Type);
                AddCreature(l_Captain.Type, l_Spawn);
            }
        }
    }
}

void OutdoorPvPAshran::ResetKillCap(uint8 p_Team)
{
    if (p_Team >= TeamId::TEAM_NEUTRAL)
        return;

    /// Actual cap is reached, spawn guardian(s) and reset it
    while (m_EnnemiesKilled[p_Team] >= eAshranDatas::EnnemiesSlainCap1)
    {
        m_EnnemiesKilled[p_Team] -= eAshranDatas::EnnemiesSlainCap1;

        creature_type l_Spawn;
        AshranCaptain l_Captain;
        if (p_Team == TeamId::TEAM_ALLIANCE)
        {
            l_Captain = g_AshranCaptains[urand(0, eAshranCaptains::MaxAllianceCaptains - 1)];
            l_Spawn.teamval = Team::ALLIANCE;
            l_Spawn.x = g_FactionBossesSpawn[2].x;
            l_Spawn.y = g_FactionBossesSpawn[2].y;
            l_Spawn.z = g_FactionBossesSpawn[2].z;
            l_Spawn.o = g_FactionBossesSpawn[2].o;
        }
        else
        {
            l_Captain = g_AshranCaptains[urand(eAshranCaptains::MaxAllianceCaptains, eSpecialSpawns::MaxAshranCaptains - 1)];
            l_Spawn.teamval = Team::HORDE;
            l_Spawn.x = g_FactionBossesSpawn[5].x;
            l_Spawn.y = g_FactionBossesSpawn[5].y;
            l_Spawn.z = g_FactionBossesSpawn[5].z;
            l_Spawn.o = g_FactionBossesSpawn[5].o;
        }

        l_Spawn.entry = l_Captain.Entry;
        l_Spawn.map = eAshranDatas::AshranMapID;

        AddCreature(l_Captain.Type, l_Spawn);
    }

    m_EnnemiesKilledMax[p_Team] = eAshranDatas::EnnemiesSlainCap1;

    if (p_Team == TeamId::TEAM_ALLIANCE)
    {
        SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAlliance, m_EnnemiesKilled[p_Team]);
        SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAllianceMax, m_EnnemiesKilledMax[p_Team]);
    }
    else
    {
        SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHorde, m_EnnemiesKilled[p_Team]);
        SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHordeMax, m_EnnemiesKilledMax[p_Team]);
    }
}

bool OutdoorPvPAshran::IsFactionGuard(Unit* p_Unit) const
{
    switch (p_Unit->GetEntry())
    {
        case eCreatures::StormshieldVanguard:
        case eCreatures::StormshieldKnight:
        case eCreatures::StormshieldSentinel:
        case eCreatures::StormshieldFootman:
        case eCreatures::StormshieldPriest:
        case eCreatures::GrandMarshalTremblade:
        case eCreatures::RylaiCrestfall:
        case eCreatures::WarspearBloodGuard:
        case eCreatures::WarspearRaptorRider:
        case eCreatures::WarspearHeadhunter:
        case eCreatures::WarspearGrunt:
        case eCreatures::WarspearPriest:
        case eCreatures::HighWarlordVolrath:
        case eCreatures::JeronEmberfall:
            return true;
        default:
            break;
    }

    return false;
}

void OutdoorPvPAshran::SpawnGladiators(uint8 p_TeamID /*= TeamId::TEAM_NEUTRAL*/, bool p_Spawn /*= true*/)
{
    /// Always despawn before spawn, for security

    if (p_TeamID == TeamId::TEAM_NEUTRAL)
    {
        /// Stormshield Gladiators
        for (uint32 l_I = eSpawns::WarspearGladiatorsSpawnsIDs; l_I < eSpawns::StormshieldGladiatorsSpawnsIDs; ++l_I)
        {
            DelCreature(l_I);

            if (p_Spawn)
                AddCreature(l_I, g_StormshieldGladiators[l_I - eSpawns::WarspearGladiatorsSpawnsIDs], 2 * TimeConstants::MINUTE);
        }

        /// Warspear Gladiators
        for (uint32 l_I = eSpawns::ArchmageOverwatchSpawnsIDs; l_I < eSpawns::WarspearGladiatorsSpawnsIDs; ++l_I)
        {
            DelCreature(l_I);

            if (p_Spawn)
                AddCreature(l_I, g_WarspearGladiators[l_I - eSpawns::ArchmageOverwatchSpawnsIDs], 2 * TimeConstants::MINUTE);
        }
    }
    else
    {
        if (p_TeamID == TeamId::TEAM_ALLIANCE)
        {
            /// Stormshield Gladiators
            for (uint32 l_I = eSpawns::WarspearGladiatorsSpawnsIDs; l_I < eSpawns::StormshieldGladiatorsSpawnsIDs; ++l_I)
            {
                DelCreature(l_I);

                if (p_Spawn)
                    AddCreature(l_I, g_StormshieldGladiators[l_I - eSpawns::WarspearGladiatorsSpawnsIDs], 2 * TimeConstants::MINUTE);
            }
        }
        else
        {
            /// Warspear Gladiators
            for (uint32 l_I = eSpawns::ArchmageOverwatchSpawnsIDs; l_I < eSpawns::WarspearGladiatorsSpawnsIDs; ++l_I)
            {
                DelCreature(l_I);

                if (p_Spawn)
                    AddCreature(l_I, g_WarspearGladiators[l_I - eSpawns::ArchmageOverwatchSpawnsIDs], 2 * TimeConstants::MINUTE);
            }
        }
    }
}

void OutdoorPvPAshran::FillCustomPvPLoots(Player* p_Looter, Loot& p_Loot, uint64 p_Container)
{
    if (m_PlayerCurrencyLoots.find(p_Container) == m_PlayerCurrencyLoots.end())
        return;

    uint32 l_ArtifactCount = m_PlayerCurrencyLoots[p_Container];
    LootStoreItem l_StoreItem = LootStoreItem(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT,  ///< ItemID (or CurrencyID)
                                              LootItemType::LOOT_ITEM_TYPE_CURRENCY,            ///< LootType
                                              100.0f,                                           ///< Chance or quest chance
                                              LootModes::LOOT_MODE_DEFAULT,                     ///< LootMode
                                              LootModes::LOOT_MODE_DEFAULT,                     ///< Group
                                              l_ArtifactCount,                                  ///< MinCount (or Ref)
                                              l_ArtifactCount,                                  ///< MaxCount
                                              std::vector<uint32>());                           ///< ItemBonuses

    p_Loot.Items.push_back(LootItem(l_StoreItem, ItemContext::None, &p_Loot));
    p_Loot.FillCurrencyLoot(p_Looter);
}

bool OutdoorPvPAshran::Update(uint32 p_Diff)
{
    PlayerTimerMap l_TempList[MS::Battlegrounds::TeamsCount::Value];

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
    ScheduleEventsUpdate(p_Diff);
    ScheduleGladiatorRespawn(p_Diff);

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
        /// Faction mini-bosses
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

        /// Road of Glory control points
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
                    l_Flag->SetByteValue(EGameObjectFields::GAMEOBJECT_FIELD_PERCENT_HEALTH, 2, eFlagStates::FlagNeutral);
            }
        }
        /// Bosses battle
        else
        {
            /// Once all Towers have been claimed by a given faction, after a short interval the enemy faction base itself will become contested.
            m_MaxBattleTime = 10 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;
            SendUpdateWorldState(eWorldStates::WorldStateTimeRemainingForBoss, uint32(time(nullptr)) + (m_MaxBattleTime / TimeConstants::IN_MILLISECONDS));

            if (m_CurrentBattleState == eWorldStates::WorldStateHighWarlordVolrath)
            {
                SendUpdateWorldState(eWorldStates::WorldStateSlayVolrath, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateHighWarlordVolrath, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateWarspearOutpostStatus, eControlStatus::ControlNeutral);

                if (Creature* l_Volrath = sObjectAccessor->FindCreature(m_HighWarlordVolrath))
                    l_Volrath->AI()->DoAction(eAshranActions::WarspearOutpostInFight);

                /// In addition, the dominant faction's Sentinels will eliminate the enemy Gladiators, rendering the enemy base vulnerable.
                /// I've checked on retail, Gladiators are just despawned, no visual effect at all
                SpawnGladiators(TeamId::TEAM_HORDE, false);
            }
            else ///< WorldStateGrandMarshalTrembladeBattle
            {
                SendUpdateWorldState(eWorldStates::WorldStateSlayTremblade, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, eWorldStates::WorldStateEnabled);
                SendUpdateWorldState(eWorldStates::WorldStateStormshieldStrongholdStatus, eControlStatus::ControlNeutral);

                if (Creature* l_Tremblade = sObjectAccessor->FindCreature(m_GrandMasrhalTremblade))
                    l_Tremblade->AI()->DoAction(eAshranActions::StormshieldStrongholdInFight);

                /// In addition, the dominant faction's Headhunters will eliminate the enemy Gladiators, rendering the enemy base vulnerable.
                /// I've checked on retail, Gladiators are just despawned, no visual effect at all
                SpawnGladiators(TeamId::TEAM_ALLIANCE, false);
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

    /// Attacking players then have a 10 minute window in which to assault the base and defeat the enemy faction leader,
    /// after which the base's NPCs will respawn, and the previous Tower will revert to neutral;
    /// attacking players will then need to capture it again in order to get another shot at assaulting the enemy base.
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

            SpawnGladiators(TeamId::TEAM_HORDE);
        }
        else ///< WorldStateGrandMarshalTrembladeBattle
        {
            SendUpdateWorldState(eWorldStates::WorldStateSlayTremblade, eWorldStates::WorldStateDisabled);
            SendUpdateWorldState(eWorldStates::WorldStateGrandMarshalTrembladeBattle, eWorldStates::WorldStateDisabled);
            SendUpdateWorldState(eWorldStates::WorldStateStormshieldStrongholdStatus, eControlStatus::ControlAlliance);
            SetBattleState(eWorldStates::WorldStateArchmageOverwatchBattle);

            if (Creature* l_Tremblade = sObjectAccessor->FindCreature(m_GrandMasrhalTremblade))
                l_Tremblade->AI()->DoAction(eAshranActions::StormshieldVictory);

            SpawnGladiators(TeamId::TEAM_ALLIANCE);
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
        InitializeEvents();
        m_IsInitialized = true;
    }
    else
        m_InitPointsTimer -= p_Diff;
}

void OutdoorPvPAshran::ScheduleEventsUpdate(uint32 p_Diff)
{
    if (!m_IsInitialized)
        return;

    uint32 l_TimeForWarn = eAshranDatas::AshranEventWarning * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;
    for (uint8 l_Index = 0; l_Index < eAshranEvents::MaxEvents; ++l_Index)
    {
        if (!m_AshranEvents[l_Index])   ///< Events are rescheduled only at their end
            continue;

        if (m_AshranEvents[l_Index] <= l_TimeForWarn && !m_AshranEventsWarned[l_Index])
        {
            m_AshranEventsWarned[l_Index] = true;
            SendEventWarningToPlayers(g_EventWarnTexts[l_Index]);
        }

        if (m_AshranEvents[l_Index] <= p_Diff)
        {
            m_AshranEvents[l_Index] = 0;
            m_AshranEventsWarned[l_Index] = false;
            StartEvent(l_Index);
        }
        else
            m_AshranEvents[l_Index] -= p_Diff;
    }
}

void OutdoorPvPAshran::ScheduleGladiatorRespawn(uint32 p_Diff)
{
    if (!m_GladiatorRespawnTime)
        return;

    if (m_GladiatorRespawnTime <= p_Diff)
    {
        m_GladiatorRespawnTime = 0;
        SpawnGladiators();
    }
    else
        m_GladiatorRespawnTime -= p_Diff;
}

void OutdoorPvPAshran::StartEvent(uint8 p_EventID)
{
    if (p_EventID >= eAshranEvents::MaxEvents)  ///< Shouldn't happens
        return;

    m_AshranEventsLaunched[p_EventID] = true;

    switch (p_EventID)
    {
        case eAshranEvents::EventKorlokTheOgreKing:
        {
            SendEventWarningToPlayers(TrinityStrings::LangKorlokIsAwakening);
            AddCreature(eSpecialSpawns::NeutralKorlokTheOgreKing, g_Korlok, 5 * TimeConstants::MINUTE);
            AddCreature(eSpecialSpawns::OgreAllianceChampion, g_AllianceChapion, 5 * TimeConstants::MINUTE);
            AddCreature(eSpecialSpawns::OgreHordeChapion, g_HordeChampion, 5 * TimeConstants::MINUTE);
            break;
        }
        case eAshranEvents::EventStadiumRacing:
        {
            SendEventWarningToPlayers(TrinityStrings::LangStadiumRacingHasBegun);

            for (uint8 l_I = 0; l_I < eSpecialSpawns::MaxRacingFlags; ++l_I)
                AddObject(eSpecialSpawns::AllianceRacingFlagSpawn1 + l_I, g_RacingFlagsPos[l_I]);

            for (uint8 l_I = 0; l_I < eSpecialSpawns::MaxRacingCreatures; ++l_I)
                AddCreature(eSpecialSpawns::SpeedyHordeRacerSpawn + l_I, g_RacingCreaturesPos[l_I]);

            SendUpdateWorldState(eWorldStates::WorldStateEnableLapsEvent, eWorldStates::WorldStateEnabled);
            SendUpdateWorldState(eWorldStates::WorldStateLapsAlliance, m_StadiumRacingLaps[TeamId::TEAM_ALLIANCE]);
            SendUpdateWorldState(eWorldStates::WorldStateLapsHorde, m_StadiumRacingLaps[TeamId::TEAM_HORDE]);
            break;
        }
        case eAshranEvents::MaxEvents:
        default:
            break;
    }
}

void OutdoorPvPAshran::EndEvent(uint8 p_EventID, bool p_ScheduleNext /*= true*/)
{
    if (p_EventID >= eAshranEvents::MaxEvents)  ///< Shouldn't happens
        return;

    if (p_ScheduleNext)
        m_AshranEvents[p_EventID] = eAshranDatas::AshranEventTimer * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;

    switch (p_EventID)
    {
        case eAshranEvents::EventKorlokTheOgreKing:
        {
            if (p_ScheduleNext)
            {
                DelCreature(eSpecialSpawns::NeutralKorlokTheOgreKing);
                break;
            }

            DelCreature(eSpecialSpawns::OgreAllianceChampion);
            DelCreature(eSpecialSpawns::OgreHordeChapion);
            break;
        }
        case eAshranEvents::EventStadiumRacing:
        {
            for (uint8 l_I = 0; l_I < eSpecialSpawns::MaxRacingFlags; ++l_I)
                DelObject(eSpecialSpawns::AllianceRacingFlagSpawn1 + l_I);

            for (uint8 l_I = 0; l_I < eSpecialSpawns::MaxRacingCreatures; ++l_I)
                DelCreature(eSpecialSpawns::SpeedyHordeRacerSpawn + l_I);

            SendUpdateWorldState(eWorldStates::WorldStateLapsAlliance, 0);
            SendUpdateWorldState(eWorldStates::WorldStateLapsHorde, 0);
            SendUpdateWorldState(eWorldStates::WorldStateEnableLapsEvent, eWorldStates::WorldStateDisabled);
            break;
        }
        case eAshranEvents::MaxEvents:
        default:
            break;
    }

    m_AshranEventsLaunched[p_EventID] = false;
}

void OutdoorPvPAshran::SendEventWarningToPlayers(uint32 p_LangID)
{
    for (uint8 l_I = 0; l_I < MS::Battlegrounds::TeamsCount::Value; ++l_I)
    {
        for (uint64 l_Guid : m_PlayersInWar[l_I])
        {
            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
            {
                std::string l_Text = l_Player->GetSession()->GetTrinityString(p_LangID);
                WorldPacket l_Data;
#ifndef CROSS
                l_Player->BuildPlayerChat(&l_Data, nullptr, CHAT_MSG_TEXT_EMOTE, l_Text.c_str(), LANG_UNIVERSAL);
#else /* CROSS */
                l_Player->BuildPlayerChat(&l_Data, 0, CHAT_MSG_TEXT_EMOTE, l_Text.c_str(), LANG_UNIVERSAL);
#endif /* CROSS */
                l_Player->GetSession()->SendPacket(&l_Data);
            }
        }

        /// Send announces to invitees too
        for (auto l_Invitee : m_InvitedPlayers[l_I])
        {
            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Invitee.first))
            {
                std::string l_Text = l_Player->GetSession()->GetTrinityString(p_LangID);
                WorldPacket l_Data;
#ifndef CROSS
                l_Player->BuildPlayerChat(&l_Data, nullptr, CHAT_MSG_TEXT_EMOTE, l_Text.c_str(), LANG_UNIVERSAL);
#else /* CROSS */
                l_Player->BuildPlayerChat(&l_Data, 0, CHAT_MSG_TEXT_EMOTE, l_Text.c_str(), LANG_UNIVERSAL);
#endif /* CROSS */
                l_Player->GetSession()->SendPacket(&l_Data);
            }
        }
    }
}

void OutdoorPvPAshran::SetEventData(uint8 p_EventID, uint8 p_TeamID, uint32 p_Data)
{
    if (p_EventID >= eAshranEvents::MaxEvents)
        return;

    if (!m_AshranEventsLaunched[p_EventID])
        return;

    switch (p_EventID)
    {
        case eAshranEvents::EventStadiumRacing:
        {
            if (p_TeamID >= TeamId::TEAM_NEUTRAL)
                break;

            /// End event
            if ((m_StadiumRacingLaps[p_TeamID] + p_Data) >= eAshranDatas::MaxStadiumRacingLaps)
            {
                EndEvent(eAshranEvents::EventStadiumRacing);
                /// Reward players

                if (Creature* l_Herald = GetHerald())
                {
                    if (l_Herald->IsAIEnabled)
                    {
                        if (p_TeamID == TeamId::TEAM_ALLIANCE)
                            l_Herald->AI()->Talk(eAshranTalks::AllianceVictorious, 0, TEXT_RANGE_MAP);
                        else
                            l_Herald->AI()->Talk(eAshranTalks::HordeVictorious, 0, TEXT_RANGE_MAP);
                    }
                }
            }
            else
            {
                m_StadiumRacingLaps[p_TeamID] += p_Data;

                if (p_TeamID == TeamId::TEAM_ALLIANCE)
                    SendUpdateWorldState(eWorldStates::WorldStateLapsAlliance, m_StadiumRacingLaps[p_TeamID]);
                else
                    SendUpdateWorldState(eWorldStates::WorldStateLapsHorde, m_StadiumRacingLaps[p_TeamID]);
            }

            break;
        }
        case eAshranEvents::EventKorlokTheOgreKing:
        default:
            break;
    }
}

void OutdoorPvPAshran::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainAlliance) << uint32(m_EnnemiesKilled[TEAM_ALLIANCE]);
    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainHorde) << uint32(m_EnnemiesKilled[TEAM_HORDE]);

    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainAllianceMax) << uint32(m_EnnemiesKilledMax[TEAM_ALLIANCE]);
    p_Data << uint32(eWorldStates::WorldStateEnnemiesSlainHordeMax) << uint32(m_EnnemiesKilledMax[TEAM_HORDE]);

    p_Data << uint32(eWorldStates::WorldStateActiveStage) << uint32(-1);

    /// Laps event
    p_Data << uint32(eWorldStates::WorldStateEnableLapsEvent) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateLapsAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateLapsHorde) << uint32(eWorldStates::WorldStateDisabled);

    /// Ore collection event
    p_Data << uint32(eWorldStates::WorldStateOreCollectedAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateOreCollectedHorde) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateEnableOreCollection) << uint32(eWorldStates::WorldStateDisabled);

    /// Fire scoring event
    p_Data << uint32(eWorldStates::WorldStateFiresScoringAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateFiresScoringHorde) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateFiresScoringEnabled) << uint32(eWorldStates::WorldStateDisabled);

    /// Risen spirits event
    p_Data << uint32(eWorldStates::WorldStateRisenSpiritsCapturedAlliance) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateRisenSpiritsCapturedHorde) << uint32(eWorldStates::WorldStateDisabled);
    p_Data << uint32(eWorldStates::WorldStateRisenSpiritsCaptureEnabled) << uint32(eWorldStates::WorldStateDisabled);

    /// King's Rest
    if (m_GraveYard != nullptr)
    {
        switch (m_GraveYard->GetGraveyardState())
        {
            case eControlStatus::ControlNeutral:
                p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForAlliance) << uint32(eWorldStates::WorldStateDisabled);
                p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForHorde) << uint32(eWorldStates::WorldStateDisabled);
                break;
            case eControlStatus::ControlAlliance:
                p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForHorde) << uint32(eWorldStates::WorldStateDisabled);
                p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForAlliance) << uint32(eWorldStates::WorldStateEnabled);
                break;
            case eControlStatus::ControlHorde:
                p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForAlliance) << uint32(eWorldStates::WorldStateDisabled);
                p_Data << uint32(eWorldStates::WorldStateGraveyardStatusForHorde) << uint32(eWorldStates::WorldStateEnabled);
                break;
            default:
                break;
        }
    }

    /// Faction bosses
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

    /// Horde boss
    p_Data << uint32(eWorldStates::WorldStateWarspearOutpostStatus) << uint32(eControlStatus::ControlHorde);

    /// Faction mini-bosses
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

    /// Alliance boss
    p_Data << uint32(eWorldStates::WorldStateStormshieldStrongholdStatus) << uint32(eControlStatus::ControlAlliance);

    /// Artifact Fragments
    /// Alliance - Current
    p_Data << uint32(eWorldStates::WorldStateAllianceMageArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForMage]);
    p_Data << uint32(eWorldStates::WorldStateAllianceWarlockArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForWarlock]);
    p_Data << uint32(eWorldStates::WorldStateAllianceWarriorArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForWarriorPaladin]);
    p_Data << uint32(eWorldStates::WorldStateAllianceShamanArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForDruidShaman]);
    /// Horde - Current
    p_Data << uint32(eWorldStates::WorldStateHordeMageArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_HORDE][eArtifactsDatas::CountForMage]);
    p_Data << uint32(eWorldStates::WorldStateHordeWarlockArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_HORDE][eArtifactsDatas::CountForWarlock]);
    p_Data << uint32(eWorldStates::WorldStateHordeWarriorArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_HORDE][eArtifactsDatas::CountForWarriorPaladin]);
    p_Data << uint32(eWorldStates::WorldStateHordeShamanArtifactCount) << uint32(m_ArtifactsCollected[TeamId::TEAM_HORDE][eArtifactsDatas::CountForDruidShaman]);
    /// General - Max
    p_Data << uint32(eWorldStates::WorldStateMageArtifactMaxCount) << uint32(eArtifactsDatas::MaxCountForMage);
    p_Data << uint32(eWorldStates::WorldStateWarlockArtifactMaxCount) << uint32(eArtifactsDatas::MaxCountForWarlock);
    p_Data << uint32(eWorldStates::WorldStateWarriorArtifactMaxCount) << uint32(eArtifactsDatas::MaxCountForWarriorPaladin);
    p_Data << uint32(eWorldStates::WorldStateShamanArtifactMaxCount) << uint32(eArtifactsDatas::MaxCountForDruidShaman);

    for (OPvPCapturePointMap::iterator l_CapturePoint = m_capturePoints.begin(); l_CapturePoint != m_capturePoints.end(); ++l_CapturePoint)
        l_CapturePoint->second->FillInitialWorldStates(p_Data);
}

void OutdoorPvPAshran::SendRemoveWorldStates(Player* p_Player)
{
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAlliance, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainHorde, 0);
    p_Player->SendUpdateWorldState(eWorldStates::WorldStateEnnemiesSlainAllianceMax, 0);
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
        m_Players[p_Player->GetTeamId()].insert(p_Player->GetGUID());

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

bool OutdoorPvPAshran::HandleOpenGo(Player* p_Player, uint64 p_Guid)
{
    /// Handle Ancient Artifact opening
    if (m_Objects[eSpecialSpawns::AncientArtifactSpawn] == p_Guid)
    {
        if (Aura* l_Aura = p_Player->AddAura(eAshranSpells::SpellAncientArtifact, p_Player))
        {
            if (m_AncientArtifactTime > 0)
            {
                l_Aura->SetDuration(m_AncientArtifactTime);
                l_Aura->SetMaxDuration(m_AncientArtifactTime);
            }
            else
                m_AncientArtifactTime = eAshranDatas::AncientArtifactMaxTime;
        }

        if (Creature* l_Herald = GetHerald())
        {
            if (l_Herald->IsAIEnabled)
            {
                if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
                    l_Herald->AI()->Talk(eAshranTalks::ArtifactLootedByAlliance, p_Player->GetGUID(), TextRange::TEXT_RANGE_MAP);
                else
                    l_Herald->AI()->Talk(eAshranTalks::ArtifactLootedByHorde, p_Player->GetGUID(), TextRange::TEXT_RANGE_MAP);
            }
        }

        DelObject(eSpecialSpawns::AncientArtifactSpawn);
        return true;
    }

    return false;
}

void OutdoorPvPAshran::HandleArtifactDrop(Unit* p_Unit, uint32 p_Time)
{
    /// Aura was removed by expire, Ancient Artifact must be respawned somewhere...
    if (!p_Time)
    {
        m_AncientArtifactTime = 0;
        AddObject(eSpecialSpawns::AncientArtifactSpawn, g_AncientArtifactPos[urand(0, eAshranDatas::AncientArtifactCount - 1)]);
    }
    /// Aura was canceled manually or removed by death, Ancient Artifact must be respawned at the target location
    else
    {
        if (p_Unit != nullptr)
        {
            go_type const l_GoType = { eGameObjects::AncientArtifact, eAshranDatas::AshranMapID, p_Unit->m_positionX, p_Unit->m_positionY, p_Unit->m_positionZ, p_Unit->m_orientation, 0.0f, 0.0f, 0.0f, 0.0f };
            AddObject(eSpecialSpawns::AncientArtifactSpawn, l_GoType);
            m_AncientArtifactTime = p_Time;
        }
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
        case eCreatures::KorlokTheOgreKing:
            AddVignetteOnPlayers(p_Creature, eAshranVignettes::VignetteKorlok);
            break;
        case eCreatures::VignetteDummyA:
            AddVignetteOnPlayers(p_Creature, eAshranVignettes::VignetteStormshieldPortal, TeamId::TEAM_ALLIANCE);
            break;
        case eCreatures::VignetteDummyH:
            AddVignetteOnPlayers(p_Creature, eAshranVignettes::VignetteWarspearPortal, TeamId::TEAM_HORDE);
            break;
        case eCreatures::Nisstyr:
            m_ArtifactsNPCGuids[TeamId::TEAM_HORDE][eArtifactsDatas::CountForWarlock] = p_Creature->GetGUID();
            break;
        case eCreatures::Fura:
            m_ArtifactsNPCGuids[TeamId::TEAM_HORDE][eArtifactsDatas::CountForMage] = p_Creature->GetGUID();
            break;
        case eCreatures::Kalgan:
            m_ArtifactsNPCGuids[TeamId::TEAM_HORDE][eArtifactsDatas::CountForWarriorPaladin] = p_Creature->GetGUID();
            break;
        case eCreatures::Atomik:
            m_ArtifactsNPCGuids[TeamId::TEAM_HORDE][eArtifactsDatas::CountForDruidShaman] = p_Creature->GetGUID();
            break;
        case eCreatures::Marketa:
            m_ArtifactsNPCGuids[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForWarlock] = p_Creature->GetGUID();
            break;
        case eCreatures::Ecilam:
            m_ArtifactsNPCGuids[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForMage] = p_Creature->GetGUID();
            break;
        case eCreatures::ValantBrightsworn:
            m_ArtifactsNPCGuids[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForWarriorPaladin] = p_Creature->GetGUID();
            break;
        case eCreatures::Anenga:
            m_ArtifactsNPCGuids[TeamId::TEAM_ALLIANCE][eArtifactsDatas::CountForDruidShaman] = p_Creature->GetGUID();
            break;
        case eCreatures::Frangraal:
            AddVignetteOnPlayers(p_Creature, eAshranVignettes::VignetteFangraal, TeamId::TEAM_ALLIANCE);
            break;
        case eCreatures::Kronus:
            AddVignetteOnPlayers(p_Creature, eAshranVignettes::VignetteKronus, TeamId::TEAM_HORDE);
            break;
        default:
            break;
    }
}

void OutdoorPvPAshran::OnCreatureRemove(Creature* p_Creature)
{
    switch (p_Creature->GetEntry())
    {
        case eCreatures::KorlokTheOgreKing:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteKorlok);
            break;
        case eCreatures::VignetteDummyA:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteStormshieldPortal, TeamId::TEAM_ALLIANCE);
            break;
        case eCreatures::VignetteDummyH:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteWarspearPortal, TeamId::TEAM_HORDE);
            break;
        case eCreatures::Frangraal:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteFangraal, TeamId::TEAM_ALLIANCE);
            break;
        case eCreatures::Kronus:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteKronus, TeamId::TEAM_HORDE);
            break;
        case eCreatures::HighWarlordVolrath:
            DelCreature(eCreatures::SLGGenericMoPLargeAoI + TeamId::TEAM_HORDE);
            m_FactionGenericMoP[TeamId::TEAM_HORDE] = 0;
            break;
        case eCreatures::GrandMarshalTremblade:
            DelCreature(eCreatures::SLGGenericMoPLargeAoI + TeamId::TEAM_ALLIANCE);
            m_FactionGenericMoP[TeamId::TEAM_ALLIANCE] = 0;
            break;
        default:
            break;
    }
}

void OutdoorPvPAshran::OnGameObjectCreate(GameObject* p_GameObject)
{
    switch (p_GameObject->GetEntry())
    {
        case eGameObjects::HordeGateway1:
            AddVignetteOnPlayers(p_GameObject, eAshranVignettes::VignetteWarlockGateway1, TeamId::TEAM_HORDE);
            break;
        case eGameObjects::HordeGateway2:
            AddVignetteOnPlayers(p_GameObject, eAshranVignettes::VignetteWarlockGateway2, TeamId::TEAM_HORDE);
            break;
        case eGameObjects::AllianceGateway1:
            AddVignetteOnPlayers(p_GameObject, eAshranVignettes::VignetteWarlockGateway1, TeamId::TEAM_ALLIANCE);
            break;
        case eGameObjects::AllianceGateway2:
            AddVignetteOnPlayers(p_GameObject, eAshranVignettes::VignetteWarlockGateway2, TeamId::TEAM_ALLIANCE);
            break;
        default:
            break;
    }

    OutdoorPvP::OnGameObjectCreate(p_GameObject);
}

void OutdoorPvPAshran::OnGameObjectRemove(GameObject* p_GameObject)
{
    switch (p_GameObject->GetEntry())
    {
        case eGameObjects::HordeGateway1:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteWarlockGateway1, TeamId::TEAM_HORDE);
            break;
        case eGameObjects::HordeGateway2:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteWarlockGateway2, TeamId::TEAM_HORDE);
            break;
        case eGameObjects::AllianceGateway1:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteWarlockGateway1, TeamId::TEAM_ALLIANCE);
            break;
        case eGameObjects::AllianceGateway2:
            RemoveVignetteOnPlayers(eAshranVignettes::VignetteWarlockGateway2, TeamId::TEAM_ALLIANCE);
            break;
        default:
            break;
    }

    OutdoorPvP::OnGameObjectRemove(p_GameObject);
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

    SpawnGladiators();
}

void OutdoorPvPAshran::InitializeEvents()
{
    /// Initialize timers for events
    /// Must calculate an equal interval between each events - Should be 6min between each
    uint32 l_Timer = 0;
    uint32 l_TimerInterval = eAshranDatas::AshranEventTimer * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS / eAshranEvents::MaxEvents;
    for (uint8 l_Index = 0; l_Index < eAshranEvents::MaxEvents; ++l_Index)
    {
        if (l_Index != eAshranEvents::EventKorlokTheOgreKing &&
            l_Index != eAshranEvents::EventStadiumRacing)   ///< Just Kor'lok and Stadium Racing yet
            break;

        l_Timer += l_TimerInterval;
        m_AshranEvents[l_Index] = l_Timer;
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
                DelCreature(eSpecialSpawns::HordeGeneralAevd);
                DelCreature(eSpecialSpawns::HordeWarlordNoktyn);
                AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[5], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeGeneralAevd, g_FactionBossesGuardians[10], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeWarlordNoktyn, g_FactionBossesGuardians[11], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateVolrathsAdvanceBattle:
                DelCreature(eSpecialSpawns::HordeFactionBoss);
                DelCreature(eSpecialSpawns::HordeGeneralAevd);
                DelCreature(eSpecialSpawns::HordeWarlordNoktyn);
                AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[4], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeGeneralAevd, g_FactionBossesGuardians[8], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeWarlordNoktyn, g_FactionBossesGuardians[9], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateTheCrossroadsBattle:
                DelCreature(eSpecialSpawns::AllianceFactionBoss);
                DelCreature(eSpecialSpawns::HordeFactionBoss);
                DelCreature(eSpecialSpawns::AllianceMarshalKarshStormforge);
                DelCreature(eSpecialSpawns::AllianceMarshalGabriel);
                DelCreature(eSpecialSpawns::HordeGeneralAevd);
                DelCreature(eSpecialSpawns::HordeWarlordNoktyn);
                AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[0], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::AllianceMarshalKarshStormforge, g_FactionBossesGuardians[0], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::AllianceMarshalGabriel, g_FactionBossesGuardians[1], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeFactionBoss, g_FactionBossesSpawn[3], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeGeneralAevd, g_FactionBossesGuardians[6], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::HordeWarlordNoktyn, g_FactionBossesGuardians[7], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateTrembladesVanguardBattle:
                DelCreature(eSpecialSpawns::AllianceFactionBoss);
                DelCreature(eSpecialSpawns::AllianceMarshalKarshStormforge);
                DelCreature(eSpecialSpawns::AllianceMarshalGabriel);
                AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[1], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::AllianceMarshalKarshStormforge, g_FactionBossesGuardians[2], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::AllianceMarshalGabriel, g_FactionBossesGuardians[3], 5 * TimeConstants::MINUTE);
                break;
            case eWorldStates::WorldStateArchmageOverwatchBattle:
                DelCreature(eSpecialSpawns::AllianceFactionBoss);
                DelCreature(eSpecialSpawns::AllianceMarshalKarshStormforge);
                DelCreature(eSpecialSpawns::AllianceMarshalGabriel);
                AddCreature(eSpecialSpawns::AllianceFactionBoss, g_FactionBossesSpawn[2], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::AllianceMarshalKarshStormforge, g_FactionBossesGuardians[4], 5 * TimeConstants::MINUTE);
                AddCreature(eSpecialSpawns::AllianceMarshalGabriel, g_FactionBossesGuardians[5], 5 * TimeConstants::MINUTE);
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

    SendUpdateWorldState(eWorldStates::WorldStateNextBattleTimestamp, uint32(time(nullptr)) + (m_NextBattleTimer / TimeConstants::IN_MILLISECONDS));
    SendUpdateWorldState(eWorldStates::WorldStateNextBattleEnabled, eWorldStates::WorldStateEnabled);
    SendUpdateWorldState(eWorldStates::WorldStateControlTheFlag, eWorldStates::WorldStateDisabled);
}

uint32 OutdoorPvPAshran::GetCurrentBattleType() const
{
    switch (m_CurrentBattleState)
    {
        case eWorldStates::WorldStateEmberfallTowerBattle:
            return eBattleType::EmberfallTower;
        case eWorldStates::WorldStateVolrathsAdvanceBattle:
            return eBattleType::VolrathsAdvance;
        case eWorldStates::WorldStateTheCrossroadsBattle:
            return eBattleType::TheCrossroads;
        case eWorldStates::WorldStateTrembladesVanguardBattle:
            return eBattleType::TrembladesVanguard;
        case eWorldStates::WorldStateArchmageOverwatchBattle:
            return eBattleType::ArchmageOverwatch;
        default:                                ///< Shouldn't happens
            return eBattleType::TheCrossroads;  ///< Default BattleType
    }
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

    /// When the Battleground is won by either side, there is a 5 minute pause; after which, the Battleground simply restarts fresh at the Crossroads.
    /// Patch 6.1.0 (2015-02-24): There's now a 10 minute delay between battles after Grand Marshal Tremblade or High Warlord Volrath has been defeated (down from 30 minutes).
    SetNextBattleTimer(10 * TimeConstants::MINUTE);

    /// Hotfix (2015-01-27): Stormshield Gladiators and Warspear Gladiators now respawn 3 minutes after Grand Marshal Tremblade or High Warlord Volrath has been defeated.
    m_GladiatorRespawnTime = 3 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;

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

void OutdoorPvPAshran::HandleCaptainDeath(uint32 p_Type)
{
    OutdoorPvP::DelCreature(p_Type);

    if (m_ActiveCaptains.find(p_Type) != m_ActiveCaptains.end())
        m_ActiveCaptains.erase(p_Type);
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
        /// Check status of Archmage Overwatch
        else if (g_GraveyardIDs[l_TeamID][l_I] == eGraveyards::TowerAlliance)
        {
            if (OPvPCapturePoint_Middle* l_CapturePoint = m_ControlPoints[eBattleType::ArchmageOverwatch])
            {
                uint32 l_State = l_CapturePoint->GetBattleFaction();
                if (l_State == eControlStatus::ControlNeutral || (l_State == eControlStatus::ControlAlliance && l_TeamID != TeamId::TEAM_ALLIANCE) ||
                    (l_State == eControlStatus::ControlHorde && l_TeamID != TeamId::TEAM_HORDE))
                    continue;
            }
        }
        /// Check status of Emberfall Tower
        else if (g_GraveyardIDs[l_TeamID][l_I] == eGraveyards::TowerHorde)
        {
            if (OPvPCapturePoint_Middle* l_CapturePoint = m_ControlPoints[eBattleType::EmberfallTower])
            {
                uint32 l_State = l_CapturePoint->GetBattleFaction();
                if (l_State == eControlStatus::ControlNeutral || (l_State == eControlStatus::ControlHorde && l_TeamID != TeamId::TEAM_HORDE) ||
                    (l_State == eControlStatus::ControlAlliance && l_TeamID != TeamId::TEAM_ALLIANCE))
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

void OutdoorPvPAshran::AddCollectedArtifacts(uint8 p_TeamID, uint8 p_Type, uint32 p_Count)
{
    if (p_TeamID > TeamId::TEAM_HORDE || p_Type >= eArtifactsDatas::MaxArtifactCounts || p_Count == 0)
        return;

    /// Loop on this to prevent something like giving 900 artifact fragments to a NPC
    /// which require only 400 fragments to launch event, he'll have 500/400 fragments...
    /// Will be useless for many events, except Knight and Wolf Riders
    while ((m_ArtifactsCollected[p_TeamID][p_Type] + p_Count) >= g_MaxArtifactsToCollect[p_Type])
    {
        p_Count -= g_MaxArtifactsToCollect[p_Type] - m_ArtifactsCollected[p_TeamID][p_Type];
        m_ArtifactsCollected[p_TeamID][p_Type] = 0;
        StartArtifactEvent(p_TeamID, p_Type);
    };

    m_ArtifactsCollected[p_TeamID][p_Type] += p_Count;
    SendUpdateWorldState(g_ArtifactsWorldStates[p_TeamID][p_Type], m_ArtifactsCollected[p_TeamID][p_Type]);
}

void OutdoorPvPAshran::StartArtifactEvent(uint8 p_TeamID, uint8 p_Type)
{
    if (p_Type >= eArtifactsDatas::MaxArtifactCounts || p_TeamID > TeamId::TEAM_HORDE)
        return;

    if (m_ArtifactEventsLaunched[p_TeamID][p_Type])
        return;

    m_ArtifactEventsLaunched[p_TeamID][p_Type] = true;
    AnnounceArtifactEvent(p_TeamID, p_Type, true);

    if (p_TeamID == TeamId::TEAM_ALLIANCE)
    {
        switch (p_Type)
        {
            case eArtifactsDatas::CountForMage:
                AddCreature(eSpecialSpawns::AllianceMagePortal1, g_MagePortalsSpawns[p_TeamID][0]);
                AddCreature(eSpecialSpawns::AllianceMagePortal2, g_MagePortalsSpawns[p_TeamID][1]);
                AddCreature(eSpecialSpawns::AllianceVignetteDummy, g_MagePortalsSpawns[p_TeamID][2]);
                AddCreature(eSpecialSpawns::AllianceKauper, g_MagePortalsSpawns[p_TeamID][3]);
                AddObject(eSpecialSpawns::AlliancePortalToStormshield, g_MagePortalsGob[p_TeamID]);
                break;
            case eArtifactsDatas::CountForWarlock:
                AddCreature(eSpecialSpawns::AllianceFalconAtherton, g_WarlockGatewaysSpawns[p_TeamID][0]);
                AddCreature(eSpecialSpawns::AllianceDeckerWatts, g_WarlockGatewaysSpawns[p_TeamID][1]);
                AddObject(eSpecialSpawns::AllianceWarlockGateway1, g_WarlockGatewaysGob[p_TeamID][0]);
                AddObject(eSpecialSpawns::AllianceWarlockGateway2, g_WarlockGatewaysGob[p_TeamID][1]);
                break;
            case eArtifactsDatas::CountForWarriorPaladin:
                /// This event can happens many times without reset
                m_ArtifactEventsLaunched[p_TeamID][p_Type] = false;
                break;
            case eArtifactsDatas::CountForDruidShaman:
                DelCreature(eSpecialSpawns::AllianceGuardian);
                AddCreature(eSpecialSpawns::AllianceFangraal, g_AllianceFangraal);
                break;
        }
    }
    else if (p_TeamID == TeamId::TEAM_HORDE)
    {
        switch (p_Type)
        {
            case eArtifactsDatas::CountForMage:
                AddCreature(eSpecialSpawns::HordeMagePortal1, g_MagePortalsSpawns[p_TeamID][0]);
                AddCreature(eSpecialSpawns::HordeMagePortal2, g_MagePortalsSpawns[p_TeamID][1]);
                AddCreature(eSpecialSpawns::HordeVignetteDummy, g_MagePortalsSpawns[p_TeamID][2]);
                AddCreature(eSpecialSpawns::HordeZaramSunraiser, g_MagePortalsSpawns[p_TeamID][3]);
                AddObject(eSpecialSpawns::HordePortalToWarspear, g_MagePortalsGob[p_TeamID]);
                break;
            case eArtifactsDatas::CountForWarlock:
                AddCreature(eSpecialSpawns::HordeGaylePlagueheart, g_WarlockGatewaysSpawns[p_TeamID][0]);
                AddCreature(eSpecialSpawns::HordeIlyaPlagueheart, g_WarlockGatewaysSpawns[p_TeamID][1]);
                AddObject(eSpecialSpawns::HordeWarlockGateway1, g_WarlockGatewaysGob[p_TeamID][0]);
                AddObject(eSpecialSpawns::HordeWarlockGateway2, g_WarlockGatewaysGob[p_TeamID][1]);
                break;
            case eArtifactsDatas::CountForWarriorPaladin:
                /// This event can happens many times without reset
                m_ArtifactEventsLaunched[p_TeamID][p_Type] = false;
                break;
            case eArtifactsDatas::CountForDruidShaman:
                DelCreature(eSpecialSpawns::HordeGuardian);
                AddCreature(eSpecialSpawns::HordeKronus, g_HordeKronus);
                break;
        }
    }
}

void OutdoorPvPAshran::EndArtifactEvent(uint8 p_TeamID, uint8 p_Type)
{
    if (p_Type >= eArtifactsDatas::MaxArtifactCounts || p_TeamID > TeamId::TEAM_HORDE)
        return;

    if (!m_ArtifactEventsLaunched[p_TeamID][p_Type])
        return;

    m_ArtifactEventsLaunched[p_TeamID][p_Type] = false;
    AnnounceArtifactEvent(p_TeamID, p_Type, false);

    if (p_TeamID == TeamId::TEAM_ALLIANCE)
    {
        switch (p_Type)
        {
            case eArtifactsDatas::CountForMage:
                DelCreature(eSpecialSpawns::AllianceMagePortal1);
                DelCreature(eSpecialSpawns::AllianceMagePortal2);
                DelCreature(eSpecialSpawns::AllianceVignetteDummy);
                DelCreature(eSpecialSpawns::AllianceKauper);
                DelObject(eSpecialSpawns::AlliancePortalToStormshield);
                break;
            case eArtifactsDatas::CountForWarlock:
                DelCreature(eSpecialSpawns::AllianceFalconAtherton);
                DelCreature(eSpecialSpawns::AllianceDeckerWatts);
                DelObject(eSpecialSpawns::AllianceWarlockGateway1);
                DelObject(eSpecialSpawns::AllianceWarlockGateway2);
                break;
            case eArtifactsDatas::CountForWarriorPaladin:
                break;
            case eArtifactsDatas::CountForDruidShaman:
                AddCreature(eSpecialSpawns::AllianceGuardian, g_AllianceGuardian);
                DelCreature(eSpecialSpawns::AllianceFangraal);
                break;
            default:    ///< Can't happen
                break;
        }
    }
    else if (p_TeamID == TeamId::TEAM_HORDE)
    {
        switch (p_Type)
        {
            case eArtifactsDatas::CountForMage:
                DelCreature(eSpecialSpawns::HordeMagePortal1);
                DelCreature(eSpecialSpawns::HordeMagePortal2);
                DelCreature(eSpecialSpawns::HordeVignetteDummy);
                DelCreature(eSpecialSpawns::HordeZaramSunraiser);
                DelObject(eSpecialSpawns::HordePortalToWarspear);
                break;
            case eArtifactsDatas::CountForWarlock:
                DelCreature(eSpecialSpawns::HordeGaylePlagueheart);
                DelCreature(eSpecialSpawns::HordeIlyaPlagueheart);
                DelObject(eSpecialSpawns::HordeWarlockGateway1);
                DelObject(eSpecialSpawns::HordeWarlockGateway2);
                break;
            case eArtifactsDatas::CountForWarriorPaladin:
                break;
            case eArtifactsDatas::CountForDruidShaman:
                AddCreature(eSpecialSpawns::HordeGuardian, g_HordeGuardian);
                DelCreature(eSpecialSpawns::HordeKronus);
                break;
            default:    ///< Can't happen
                break;
        }
    }
}

bool OutdoorPvPAshran::IsArtifactEventLaunched(uint8 p_TeamID, uint8 p_Type) const
{
    if (p_Type >= eArtifactsDatas::MaxArtifactCounts || p_TeamID > TeamId::TEAM_HORDE)
        return false;

    return m_ArtifactEventsLaunched[p_TeamID][p_Type];
}

void OutdoorPvPAshran::AnnounceArtifactEvent(uint8 p_TeamID, uint8 p_Type, bool p_Apply)
{
    if (p_Type >= eArtifactsDatas::MaxArtifactCounts || p_TeamID > TeamId::TEAM_HORDE)
        return;

    if (!m_ArtifactsNPCGuids[p_TeamID][p_Type])
        return;

    Creature* l_Creature = sObjectAccessor->FindCreature(m_ArtifactsNPCGuids[p_TeamID][p_Type]);
    if (l_Creature == nullptr || l_Creature->GetAI() == nullptr)
        return;

    /// 0: Event activated
    /// 1: Event deactivated
    l_Creature->AI()->Talk(p_Apply ? 0 : 1);
}

void OutdoorPvPAshran::RewardHonorAndReputation(uint32 p_ArtifactCount, Player* p_Player)
{
    if (p_Player == nullptr)
        return;

    p_Player->PlayerTalkClass->SendCloseGossip();

    /// Turning in Artifact Fragments for these purposes is the aim of a number of repeatable quests
    /// Rewarding 3 Honor Points and 5 reputation with the respective faction for each 1 Artifact Fragment turned in.
    /// However, the main goal is to further the faction's progress within the current battle.
    int32 l_HonorPoints = p_ArtifactCount * eArtifactsDatas::HonorConversionRate * CURRENCY_PRECISION;
    int32 l_Reputation = p_ArtifactCount * eArtifactsDatas::ReputationConversionRate;

    p_Player->RewardHonor(nullptr, 1, l_HonorPoints);

    FactionEntry const* l_Faction = sFactionStore.LookupEntry(p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE ? eFactions::WrynnsVanguard : eFactions::VoljinsSpear);
    if (l_Faction == nullptr)
        return;

    p_Player->GetReputationMgr().ModifyReputation(l_Faction, l_Reputation);
}

template <class T>
inline void OutdoorPvPAshran::AddVignetteOnPlayers(T const* p_Object, uint32 p_VignetteID, uint8 p_TeamID /*= TeamId::TEAM_NEUTRAL*/)
{
    VignetteEntry const* l_Vignette = sVignetteStore.LookupEntry(p_VignetteID);
    if (l_Vignette == nullptr)
        return;

    G3D::Vector3 l_Position = G3D::Vector3(p_Object->GetPositionX(), p_Object->GetPositionY(), p_Object->GetPositionZ());
    uint64 l_Guid = p_Object->GetGUID();

    if (p_TeamID == TeamId::TEAM_NEUTRAL)
        m_NeutralVignettes.insert(std::make_pair(p_VignetteID, l_Guid));
    else
        m_FactionVignettes[p_TeamID].insert(std::make_pair(p_VignetteID, l_Guid));

    for (uint8 l_Team = 0; l_Team < MS::Battlegrounds::TeamsCount::Value; ++l_Team)
    {
        if (p_TeamID != TeamId::TEAM_NEUTRAL && l_Team != p_TeamID)
            continue;

        for (uint64 l_Guid : m_PlayersInWar[l_Team])
        {
            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
            {
                Vignette::Manager& l_VignetteMgr = l_Player->GetVignetteMgr();
                l_VignetteMgr.CreateAndAddVignette(l_Vignette, eAshranDatas::AshranMapID, Vignette::Type::SourceScript, l_Position, l_Guid);
            }
        }
    }
}

template void OutdoorPvPAshran::AddVignetteOnPlayers(Creature const*, uint32 p_VignetteID, uint8 p_TeamID /*= TeamId::TEAM_NEUTRAL*/);
template void OutdoorPvPAshran::AddVignetteOnPlayers(GameObject const*, uint32 p_VignetteID, uint8 p_TeamID /*= TeamId::TEAM_NEUTRAL*/);

void OutdoorPvPAshran::RemoveVignetteOnPlayers(uint32 p_VignetteID, uint8 p_TeamID /*= TeamId::TEAM_NEUTRAL*/)
{
    VignetteEntry const* l_Vignette = sVignetteStore.LookupEntry(p_VignetteID);
    if (l_Vignette == nullptr)
        return;

    if (p_TeamID == TeamId::TEAM_NEUTRAL)
        m_NeutralVignettes.erase(p_VignetteID);
    else
        m_FactionVignettes[p_TeamID].erase(p_VignetteID);

    for (uint8 l_Team = 0; l_Team < MS::Battlegrounds::TeamsCount::Value; ++l_Team)
    {
        if (p_TeamID != TeamId::TEAM_NEUTRAL && l_Team != p_TeamID)
            continue;

        for (uint64 l_Guid : m_PlayersInWar[l_Team])
        {
            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
            {
                Vignette::Manager& l_VignetteMgr = l_Player->GetVignetteMgr();
                l_VignetteMgr.DestroyAndRemoveVignetteByEntry(l_Vignette);
            }
        }
    }
}

void OutdoorPvPAshran::CastSpellOnTeam(Unit* p_Caster, uint8 p_Team, uint32 p_SpellID)
{
    if (p_Team > TeamId::TEAM_HORDE)
        return;

    for (uint64 l_Guid : m_PlayersInWar[p_Team])
    {
        if (!p_Caster->getThreatManager().HaveInThreatList(l_Guid))
            continue;

        if (Player* l_Player = Player::GetPlayer(*p_Caster, l_Guid))
            p_Caster->CastSpell(l_Player, p_SpellID, true);
    }
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
#ifndef __clang_analyzer__
void AddSC_AshranMgr()
{
    new OutdoorPvP_Ashran();
}
#endif
