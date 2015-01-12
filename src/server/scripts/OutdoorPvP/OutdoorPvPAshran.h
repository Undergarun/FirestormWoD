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

#include "OutdoorPvP.h"
#include "OutdoorPvPMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "Player.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "MapManager.h"

#ifndef OUTDOOR_PVP_ASHRAN_H
#define OUTDOOR_PVP_ASHRAN_H

typedef std::set<uint64> GuidSet;
typedef std::map<uint64, uint32> PlayerTimerMap;

enum eAshranDatas
{
    AshranZoneID                = 6941,
    AshranMapID                 = 1191,
    AshranNeutralMapID          = 1116,
    AshranPreAreaHorde          = 7333,
    AshranPreAreaAlliance       = 7332,
    AshranHordeBase             = 7099,
    AshranAllianceBase          = 7100,
    AshranPvPAreaID             = 24,
    BattlefieldWorldPvP         = 0x20000,
    PlayerMinLevel              = 100,
    AshranTimeForInvite         = 20,
    AshranTimeForBattle         = 25,

    AshranHallowedGroundH       = 7782,
    AshranHallowedGroundA       = 7781,
    AshranHallowedGroundID      = 42989,

    AshranGenericMobTypeID      = 68553,

    EmberfallTowerAreaID        = 7080,
    VolrathsAdvanceAreaID       = 7476,
    ArchmageOverwatchAreaID     = 7479,
    TrembladesVanguardAreaID    = 7478
};

enum eAshranSpells
{
    SpellLootable        = 161733,
    SpellHoldYourGround  = 173534,   ///< +30% damage, healing and health
    SpellTowerDefense    = 173541,   ///< +20% damage, healing and health
    SpellStandFast       = 173549,   ///< +10% damage, healing and health
    SpellHallowedGround  = 171496
};

enum eWorldStates
{
    WorldStateDisabled                      = 0,
    WorldStateEnabled                       = 1,
    //////////////////////////////////////////////////
    /// Molten Quarry(southeast) - Event: Collect Empowered Ore
    WorldStateOreCollectedAlliance          = 1581,
    WorldStateOreCollectedHorde             = 1582,
    WorldStateEnableOreCollection           = 9274,
    /// Molten Quarry(southeast) - Event: Collect Empowered Ore
    //////////////////////////////////////////////////
    WorldStateEnnemiesSlainAlliance         = 8933,
    WorldStateEnnemiesSlainHorde            = 8934,
    WorldStateNextBattleTimestamp           = 8945,
    WorldStateNextBattleEnabled             = 8946,
    WorldStateActiveStage                   = 8955,
    //////////////////////////////////////////////////
    /// Risen Spirits (Ashmaul Burial Grounds)
    WorldStateRisenSpiritsCaptureEnabled    = 9199,
    WorldStateRisenSpiritsCapturedAlliance  = 9200,
    WorldStateRisenSpiritsCapturedHorde     = 9201,
    /// Risen Spirits (Ashmaul Burial Grounds)
    //////////////////////////////////////////////////
    /// Amphitheater of Annihilation (northwest) - Event: The Race!
    WorldStateEnableLapsEvent               = 9290,
    WorldStateLapsAlliance                  = 9291,
    WorldStateLapsHorde                     = 9292,
    /// Amphitheater of Annihilation (northwest) - Event: The Race!
    //////////////////////////////////////////////////
    WorldStateTimeRemainingForBoss          = 9316,
    WorldStateSlayVolrath                   = 9317, ///< Enable time remaining for boss
    WorldStateSlayTremblade                 = 9326, ///< Enable time remaining for boss
    //////////////////////////////////////////////////
    /// Brute's Rise (northeast) - Event: Ogre Fires!
    WorldStateFiresScoringEnabled           = 9414,
    WorldStateFiresScoringAlliance          = 9418,
    WorldStateFiresScoringHorde             = 9419,
    WorldStateFiresControlledAlliance       = 9782,
    WorldStateFiresControlledHorde          = 9783,
    WorldStateFiresScoringMax               = 9784,
    /// Brute's Rise (northeast) - Event: Ogre Fires!
    //////////////////////////////////////////////////
    WorldStateHighWarlordVolrath            = 9713,
    WorldStateEmberfallTowerBattle          = 9714,
    WorldStateVolrathsAdvanceBattle         = 9715,
    WorldStateTheCrossroadsBattle           = 9716,
    WorldStateTrembladesVanguardBattle      = 9717,
    WorldStateArchmageOverwatchBattle       = 9718,
    WorldStateGrandMarshalTrembladeBattle   = 9719,
    WorldStateControlTheFlag                = 9785,
    WorldStateEnnemiesSlainAlianceMax       = 9801,
    WorldStateEnnemiesSlainHordeMax         = 9802,
    //////////////////////////////////////////////////
    /// Kings' Rest (west)
    WorldStateEnableGraveyardProgressBar    = 9053,
    WorldStateGraveyardProgressBar          = 9054,
    WorldStateGraveyardProgressBarGreyPct   = 9055,
    /// Kings' Rest (west)
    //////////////////////////////////////////////////
    /// Five Towers line the Road of Glory
    WorldStateEnableTowerProgressBar        = 9110,
    WorldStateTowerProgressBar              = 9111,
    WorldStateTowerProgressBarGreyPct       = 9112,
    /// Five Towers line the Road of Glory
    //////////////////////////////////////////////////
    /// Five Towers line - Control Status
    WorldStateWarspearOutpostStatus         = 9100, ///< 0 - In fight, 1 - Horde, 2 - Destroyed
    WorldStateEmberfallTowerStatus          = 9101, ///< 0 - Neutral, 1 - Horde, 2 - Alliance
    WorldStateVolrathsAdvanceStatus         = 9102, ///< 0 - Neutral, 1 - Horde, 2 - Alliance
    WorldStateTheCrossroadsStatus           = 9103, ///< 0 - Neutral, 1 - Horde, 2 - Alliance
    WorldStateTrembladesVanguardStatus      = 9104, ///< 0 - Neutral, 1 - Horde, 2 - Alliance
    WorldStateArchmageOverwatchStatus       = 9105, ///< 0 - Neutral, 1 - Horde, 2 - Alliance
    WorldStateStormshieldStrongholdStatus   = 9106, ///< 0 - In fight, 1 - Destroyed, 2 - Alliance
    /// Five Towers line - Control Status
    //////////////////////////////////////////////////
    /// Not implemented yet
    WorldStateOgreKingThroneStatus         = 9113  ///< 0 - Neutral, 1 - Horde
};

enum eControlStatus
{
    ControlNeutral,
    ControlHorde,
    ControlAlliance
};

enum eCreatures
{
    AshranHerald            = 84113,
    BladeTwisterTrigger     = 89320,
    WarspearBloodGuard      = 83699,
    WarspearRaptorRider     = 80297,
    WarspearHeadhunter      = 79993,
    WarspearGrunt           = 79269,
    WarspearPriest          = 79982,
    HighWarlordVolrath      = 82877,    ///< Horde boss
    StormshieldVanguard     = 83717,
    StormshieldKnight       = 80256,
    StormshieldSentinel     = 79990,
    StormshieldFootman      = 79268,
    StormshieldPriest       = 79947,
    GrandMarshalTremblade   = 82876,    ///< Alliance boss
    SLGGenericMoPLargeAoI   = 68553,
    JeronEmberfall          = 88178,    ///< Horde Guardian
    RylaiCrestfall          = 88224     ///< Alliance Guardian
};

enum eGameObjects
{
    GraveyardBannerHorde      = 233518,
    GraveyardBannerAlliance   = 233517,
    GraveyardControlBanner    = 231201,
    CapturePointBanner        = 230876,
    BonfireWithSmokeLarge1    = 233531,
    Smallfire1                = 233534
};

enum eAshranActions
{
    AnnounceMarketplaceGraveyard,
    AnnounceHordeGraveyard,
    AnnounceAllianceGraveyard,
    AnnounceHordeVictory,
    AnnounceAllianceKillBoss,
    AnnounceAllianceVictory,
    AnnounceHordeKillBoss,
    WarspearOutpostInFight,
    StormshieldStrongholdInFight,
    WarspearVictory,
    StormshieldVictory
};

enum eGraveyards
{
    AllianceBase    = 4742, ///< Ashran - Lane - Base GY (A)
    HordeBase       = 4743, ///< Ashran - Lane - Base GY (H)
    AllianceCenter  = 4822, ///< Ashran - Lane - Center GY (A)
    HordeCenter     = 4825, ///< Ashran - Lane - Center GY (H)
    Stage1Alliance  = 4769, ///< Ashran - Lane - Stage 1 (Horde Approach) - Alliance GY
    Stage1Horde     = 4770, ///< Ashran - Lane - Stage 1 (Horde Approach) - Horde GY
    Stage3Alliance  = 4768, ///< Ashran - Lane - Stage 3 (Alliance Approach) - Alliance GY
    Stage3Horde     = 4767, ///< Ashran - Lane - Stage 3 (Alliance Approach) - Horde GY
    TowerAlliance   = 4821, ///< Ashran - Lane - Tower GY (A)
    TowerHorde      = 4824, ///< Ashran - Lane - Tower GY (H)
    QuarryAlliance  = 4717, ///< Ashran - Quarry - Alliance TEMP GY
    QuarryHorde     = 4718, ///< Ashran - Quarry - Horde TEMP GY
    ArenaAlliance   = 4730, ///< Ashran - The Arena - Alliance GY
    ArenaHorde      = 4731  ///< Ashran - The Arena - Horde GY
};

Position const g_HordeTeleportPos = { 5216.443359f, -3963.191406f, 5.553593f, 6.242684f };
Position const g_AllianceTeleportPos = { 3849.396240f, -4013.051025f, 26.282335f, 3.141932f };

uint32 const g_HallowedGroundEntries[BG_TEAMS_COUNT] =
{
    eAshranDatas::AshranHallowedGroundH,
    eAshranDatas::AshranHallowedGroundA
};

Position const g_HallowedGroundPos[BG_TEAMS_COUNT] =
{
    { 5090.467f, -4076.731f, 49.38393f, 3.379836f },    ///< eAshranDatas::AshranHallowedGroundH
    { 3928.052f, -4032.738f, 57.41695f, 5.473989f }     ///< eAshranDatas::AshranHallowedGroundA
};

enum eBanners
{
    GraveyardBanner0,
    GraveyardBanner1,
    GraveyardBanner2,
    GraveyardBanner3,
    GraveyardMaxBanner
};

const go_type g_GraveyardBanner_H[eBanners::GraveyardMaxBanner] =
{
    { eGameObjects::GraveyardBannerHorde, eAshranDatas::AshranMapID, 4527.93f, -3999.18f, 5.95707f, 0.588123f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::GraveyardBannerHorde, eAshranDatas::AshranMapID, 4528.02f, -4006.75f, 6.05358f, 2.277280f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::GraveyardBannerHorde, eAshranDatas::AshranMapID, 4537.33f, -3999.50f, 6.13882f, 5.566630f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::GraveyardBannerHorde, eAshranDatas::AshranMapID, 4536.55f, -4006.41f, 6.38824f, 4.122270f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const go_type g_GraveyardBanner_A[eBanners::GraveyardMaxBanner] =
{
    { eGameObjects::GraveyardBannerAlliance, eAshranDatas::AshranMapID, 4527.93f, -3999.18f, 5.95707f, 0.588123f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::GraveyardBannerAlliance, eAshranDatas::AshranMapID, 4528.02f, -4006.75f, 6.05358f, 2.277280f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::GraveyardBannerAlliance, eAshranDatas::AshranMapID, 4537.33f, -3999.50f, 6.13882f, 5.566630f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::GraveyardBannerAlliance, eAshranDatas::AshranMapID, 4536.55f, -4006.41f, 6.38824f, 4.122270f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const go_type g_GraveyardBanner_N = { eGameObjects::GraveyardControlBanner, eAshranDatas::AshranMapID, 4532.632f, -4003.269f, 6.317888f, 0.0f, 0.0f, 0.0f, 0.008727f, -0.999962f };

enum eFlagStates
{
    FlagNeutral    = 21,
    FlagHorde      = 1,
    FlagAlliance   = 2
};

enum eBattleType
{
    EmberfallTower,
    VolrathsAdvance,
    TheCrossroads,
    TrembladesVanguard,
    ArchmageOverwatch,
    MaxBattleType
};

uint32 const g_TowerControlStatus[eBattleType::MaxBattleType] =
{
    eWorldStates::WorldStateEmberfallTowerStatus,
    eWorldStates::WorldStateVolrathsAdvanceStatus,
    eWorldStates::WorldStateTheCrossroadsStatus,
    eWorldStates::WorldStateTrembladesVanguardStatus,
    eWorldStates::WorldStateArchmageOverwatchStatus
};

enum eSpawns
{
    EmberfallTowerCreaturesCount     = 15,
    EmberfallTowerObjectsCount       = 12,
    EmberfallTowerSpawnsIDs          = EmberfallTowerCreaturesCount + EmberfallTowerObjectsCount,
    VolrathsAdvanceCreaturesCount    = 18,
    VolrathsAdvanceObjectsCount      = 5,
    VolrathsAdvanceSpawnsIDs         = EmberfallTowerSpawnsIDs + VolrathsAdvanceCreaturesCount + VolrathsAdvanceObjectsCount,
    TheCrossroadsCreaturesCount      = 10,
    TheCrossroadsObjectsCount        = 2,
    TheCrossroadsSpawnsIDs           = VolrathsAdvanceSpawnsIDs + TheCrossroadsCreaturesCount + TheCrossroadsObjectsCount,
    TrembladesVanguardCreaturesCount = 10,
    TrembladesVanguardObjectsCount   = 5,
    TrembladesVanguardSpawnsIDs      = TheCrossroadsSpawnsIDs + TrembladesVanguardCreaturesCount + TrembladesVanguardObjectsCount,
    ArchmageOverwatchCreaturesCount  = 10,
    ArchmageOverwatchObjectsCount    = 11,
    ArchmageOverwatchSpawnsIDs       = TrembladesVanguardSpawnsIDs + ArchmageOverwatchCreaturesCount + ArchmageOverwatchObjectsCount
};

enum eSpecialSpawns
{
    /// Tower guardians (Emberfall Tower & Archmage Overwatch)
    HordeTowerGuardian = eSpawns::ArchmageOverwatchSpawnsIDs,
    AllianceTowerGuardian,
    MaxTowerGuardians = 2,
    /// Faction bosses (High Warlord Volrath and Grand Marshal Tremblade)
    HordeFactionBoss = AllianceTowerGuardian + 1,
    AllianceFactionBoss,
    MaxFactionBosses = 2
};

const creature_type g_FactionGuardians[eSpecialSpawns::MaxTowerGuardians] =
{
    { eCreatures::RylaiCrestfall, Team::ALLIANCE, eAshranDatas::AshranMapID, 4271.71f, -4202.35f, 55.0845f, 6.0219f },
    { eCreatures::JeronEmberfall, Team::HORDE,    eAshranDatas::AshranMapID, 4763.33f, -4231.81f, 56.6295f, 3.0479f }
};

/// Three spawn positions depending on towers status
const creature_type g_FactionBossesSpawn[eSpecialSpawns::MaxFactionBosses * 3] =
{
    { eCreatures::GrandMarshalTremblade, Team::ALLIANCE, eAshranDatas::AshranMapID, 3921.64f, -4086.35f, 66.53f, 0.1372f }, ///< The Crossroads
    { eCreatures::GrandMarshalTremblade, Team::ALLIANCE, eAshranDatas::AshranMapID, 3955.19f, -4081.59f, 63.72f, 0.1411f }, ///< Tremblade's Vanguard
    { eCreatures::GrandMarshalTremblade, Team::ALLIANCE, eAshranDatas::AshranMapID, 3991.94f, -4094.43f, 57.47f, 6.0214f }, ///< Archmage Overwatch
    { eCreatures::HighWarlordVolrath,    Team::HORDE,    eAshranDatas::AshranMapID, 5125.03f, -4115.48f, 59.13f, 3.8966f }, ///< The Crossroads
    { eCreatures::HighWarlordVolrath,    Team::HORDE,    eAshranDatas::AshranMapID, 5073.46f, -4160.39f, 47.21f, 3.8534f }, ///< Volrath's Advance
    { eCreatures::HighWarlordVolrath,    Team::HORDE,    eAshranDatas::AshranMapID, 5046.81f, -4185.73f, 45.47f, 2.7925f }  ///< Emberfall Tower
};

const creature_type g_EmberfallTowerSpawns[BG_TEAMS_COUNT][eSpawns::EmberfallTowerCreaturesCount] =
{
    // ALLIANCE
    {
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4791.99f, -4180.81f, 33.22f, 4.95f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4788.07f, -4182.17f, 33.22f, 5.80f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4775.78f, -4217.50f, 32.58f, 0.00f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4776.99f, -4220.96f, 32.33f, 0.91f },
        { eCreatures::StormshieldSentinel,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4788.38f, -4226.64f, 56.56f, 0.80f },
        { eCreatures::StormshieldSentinel,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4780.22f, -4219.33f, 56.11f, 1.43f },
        { eCreatures::StormshieldSentinel,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4795.50f, -4237.04f, 56.35f, 6.15f },
        { eCreatures::StormshieldSentinel,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4785.99f, -4247.97f, 56.57f, 5.44f },
        { eCreatures::StormshieldSentinel,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4767.79f, -4228.28f, 56.62f, 2.25f },
        { eCreatures::StormshieldKnight,    Team::ALLIANCE, eAshranDatas::AshranMapID, 4781.27f, -4265.98f, 27.76f, 3.72f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4783.97f, -4261.41f, 28.89f, 3.72f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4787.13f, -4266.29f, 28.49f, 3.72f },
        { eCreatures::StormshieldKnight,    Team::ALLIANCE, eAshranDatas::AshranMapID, 4816.92f, -4207.30f, 32.73f, 1.97f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4821.17f, -4209.80f, 32.63f, 1.97f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4813.77f, -4212.76f, 33.13f, 1.97f }
    },
    // HORDE
    {
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4791.99f, -4180.81f, 33.22f, 4.95f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4788.07f, -4182.17f, 33.22f, 5.80f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4775.78f, -4217.50f, 32.58f, 0.00f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4776.99f, -4220.96f, 32.33f, 0.91f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4788.38f, -4226.64f, 56.56f, 0.80f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4780.22f, -4219.33f, 56.11f, 1.43f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4795.50f, -4237.04f, 56.35f, 6.15f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4785.99f, -4247.97f, 56.57f, 5.44f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4767.79f, -4228.28f, 56.62f, 2.25f },
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4781.27f, -4265.98f, 27.76f, 3.72f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4783.97f, -4261.41f, 28.89f, 3.72f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4787.13f, -4266.29f, 28.49f, 3.72f },
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4816.92f, -4207.30f, 32.73f, 1.97f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4821.17f, -4209.80f, 32.63f, 1.97f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4813.77f, -4212.76f, 33.13f, 1.97f }
    }
};

const creature_type g_EmberfallTowerNeutralSpawns[eSpawns::EmberfallTowerCreaturesCount] =
{
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4791.99f, -4180.81f, 33.22f, 4.95f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4788.07f, -4182.17f, 33.22f, 5.80f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4775.78f, -4217.50f, 32.58f, 0.00f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4776.99f, -4220.96f, 32.33f, 0.91f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4824.54f, -4183.08f, 31.66f, 3.81f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4827.80f, -4189.65f, 31.68f, 3.41f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4802.03f, -4234.42f, 33.04f, 5.03f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4810.90f, -4247.38f, 33.12f, 2.83f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4789.45f, -4185.29f, 32.91f, 1.28f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4794.69f, -4180.93f, 33.22f, 2.73f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4817.72f, -4186.66f, 31.83f, 0.07f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4822.87f, -4192.89f, 31.68f, 1.17f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4775.27f, -4224.87f, 32.67f, 0.55f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4772.74f, -4218.13f, 32.32f, 0.36f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4768.51f, -4225.33f, 31.27f, 0.63f }
};

const go_type g_EmberfallFiresSpawns[eSpawns::EmberfallTowerObjectsCount] =
{
    { eGameObjects::BonfireWithSmokeLarge1, eAshranDatas::AshranMapID, 4778.00f, -4224.00f, 56.68f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::BonfireWithSmokeLarge1, eAshranDatas::AshranMapID, 4790.39f, -4241.27f, 56.15f, 0.09f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4812.25f, -4252.67f, 37.19f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4796.12f, -4234.99f, 37.11f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4827.44f, -4195.64f, 37.34f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4816.45f, -4164.16f, 35.71f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4799.46f, -4164.38f, 35.47f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4776.97f, -4185.94f, 41.45f, 2.35f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4769.31f, -4182.60f, 46.10f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4756.42f, -4175.85f, 38.16f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4749.02f, -4183.04f, 38.13f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4768.50f, -4245.03f, 47.74f, 3.60f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const creature_type g_VolrathsAdvanceSpawns[BG_TEAMS_COUNT][eSpawns::VolrathsAdvanceCreaturesCount] =
{
    // ALLIANCE
    {
        { eCreatures::StormshieldSentinel, Team::ALLIANCE, eAshranDatas::AshranMapID, 4671.80f, -4242.89f, 29.6425f, 6.10519f },
        { eCreatures::StormshieldSentinel, Team::ALLIANCE, eAshranDatas::AshranMapID, 4653.12f, -4206.77f, 28.3985f, 4.78364f },
        { eCreatures::StormshieldSentinel, Team::ALLIANCE, eAshranDatas::AshranMapID, 4679.35f, -4242.53f, 28.9622f, 2.04980f },
        { eCreatures::StormshieldSentinel, Team::ALLIANCE, eAshranDatas::AshranMapID, 4654.56f, -4199.31f, 27.2997f, 5.45237f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4652.03f, -4218.00f, 9.92943f, 4.59145f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4652.04f, -4223.96f, 9.92325f, 1.58571f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4654.54f, -4224.20f, 11.4234f, 0.00943f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4654.81f, -4218.55f, 11.4234f, 0.00935f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4683.18f, -4211.89f, 10.3594f, 3.00827f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4683.80f, -4205.95f, 10.1736f, 3.14209f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4688.85f, -4230.05f, 11.1861f, 0.24750f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4693.90f, -4232.60f, 11.1861f, 0.43090f },
        { eCreatures::StormshieldKnight,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4650.39f, -4220.74f, 9.55521f, 6.17425f },
        { eCreatures::StormshieldKnight,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4681.88f, -4208.51f, 10.2640f, 3.28998f },
        { eCreatures::StormshieldPriest,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4725.44f, -4203.77f, 22.3601f, 3.56446f },
        { eCreatures::StormshieldPriest,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4651.05f, -4211.01f, 9.87623f, 5.53973f },
        { eCreatures::StormshieldPriest,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4668.79f, -4257.62f, 11.5811f, 1.59311f },
        { eCreatures::StormshieldPriest,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4682.44f, -4184.54f, 21.8149f, 4.45475f }
    },
    // HORDE
    {
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4671.80f, -4242.89f, 29.6425f, 6.10519f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4653.12f, -4206.77f, 28.3985f, 4.78364f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4679.35f, -4242.53f, 28.9622f, 2.04980f },
        { eCreatures::WarspearHeadhunter,  Team::HORDE, eAshranDatas::AshranMapID, 4654.56f, -4199.31f, 27.2997f, 5.45237f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4652.03f, -4218.00f, 9.92943f, 4.59145f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4652.04f, -4223.96f, 9.92325f, 1.58571f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4654.54f, -4224.20f, 11.4234f, 0.00943f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4654.81f, -4218.55f, 11.4234f, 0.00935f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4683.18f, -4211.89f, 10.3594f, 3.00827f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4683.80f, -4205.95f, 10.1736f, 3.14209f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4688.85f, -4230.05f, 11.1861f, 0.24750f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4693.90f, -4232.60f, 11.1861f, 0.43090f },
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4650.39f, -4220.74f, 9.55521f, 6.17425f },
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4681.88f, -4208.51f, 10.2640f, 3.28998f },
        { eCreatures::WarspearPriest,      Team::HORDE, eAshranDatas::AshranMapID, 4725.44f, -4203.77f, 22.3601f, 3.56446f },
        { eCreatures::WarspearPriest,      Team::HORDE, eAshranDatas::AshranMapID, 4651.05f, -4211.01f, 9.87623f, 5.53973f },
        { eCreatures::WarspearPriest,      Team::HORDE, eAshranDatas::AshranMapID, 4668.79f, -4257.62f, 11.5811f, 1.59311f },
        { eCreatures::WarspearPriest,      Team::HORDE, eAshranDatas::AshranMapID, 4682.44f, -4184.54f, 21.8149f, 4.45475f }
    }
};

const creature_type g_VolrathsAdvanceNeutralSpawns[eSpawns::VolrathsAdvanceCreaturesCount] =
{
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4709.55f, -4221.30f, 10.9748f, 5.71988f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4713.37f, -4218.79f, 11.1445f, 5.13476f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4682.40f, -4265.17f, 12.1166f, 0.39958f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4681.34f, -4259.01f, 11.6778f, 6.22331f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4682.55f, -4185.68f, 21.4261f, 6.22722f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4683.04f, -4180.22f, 23.2955f, 5.99160f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4648.47f, -4232.44f, 10.1874f, 4.64069f },
    { eCreatures::WarspearPriest,     Team::HORDE,    eAshranDatas::AshranMapID, 4732.59f, -4210.11f, 24.4721f, 3.63575f },
    { eCreatures::WarspearPriest,     Team::HORDE,    eAshranDatas::AshranMapID, 4663.53f, -4201.14f, 9.47571f, 5.10444f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4718.06f, -4222.33f, 11.1647f, 2.62541f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4713.48f, -4226.52f, 11.1247f, 1.92248f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4694.93f, -4262.08f, 11.8211f, 3.12492f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4692.43f, -4256.23f, 11.8035f, 3.69040f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4697.10f, -4181.13f, 22.7873f, 3.26234f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4695.53f, -4187.58f, 20.5008f, 2.86964f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4646.38f, -4243.53f, 10.9999f, 1.22023f },
    { eCreatures::StormshieldPriest,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4657.05f, -4256.82f, 13.1483f, 1.58978f },
    { eCreatures::StormshieldPriest,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4652.04f, -4210.78f, 9.98361f, 5.58745f }
};

const go_type g_VolrathsAdvanceFires[eSpawns::VolrathsAdvanceObjectsCount] =
{
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4720.46f, -4225.85f, 16.50f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4715.67f, -4239.35f, 13.88f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4703.38f, -4247.46f, 13.71f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4642.08f, -4236.12f, 13.30f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4646.33f, -4207.88f, 12.42f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const creature_type g_CrossroadSpawns[BG_TEAMS_COUNT][eSpawns::TheCrossroadsCreaturesCount] =
{
    // Alliance
    {
        { eCreatures::StormshieldKnight,    Team::ALLIANCE, eAshranDatas::AshranMapID, 4478.58f, -4198.21f, 6.18646f, 2.85147f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4481.10f, -4196.31f, 7.18646f, 2.85147f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4480.48f, -4201.73f, 2.92569f, 2.85147f },
        { eCreatures::StormshieldKnight,    Team::ALLIANCE, eAshranDatas::AshranMapID, 4592.31f, -4218.70f, 7.07966f, 3.05845f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4593.84f, -4221.67f, 7.27999f, 3.05845f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4594.30f, -4216.03f, 7.27999f, 3.05845f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4526.08f, -4183.99f, 7.08054f, 0.02788f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4537.70f, -4183.91f, 7.07746f, 3.18518f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4526.93f, -4238.27f, 7.11127f, 6.25608f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4540.17f, -4238.74f, 7.16359f, 3.11056f }
    },
    // Horde
    {
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4478.58f, -4198.21f, 6.18646f, 2.85147f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4481.10f, -4196.31f, 7.18646f, 2.85147f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4480.48f, -4201.73f, 2.92569f, 2.85147f },
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4592.31f, -4218.70f, 7.07966f, 3.05845f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4593.84f, -4221.67f, 7.27999f, 3.05845f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4594.30f, -4216.03f, 7.27999f, 3.05845f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4526.08f, -4183.99f, 7.08054f, 0.02788f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4537.70f, -4183.91f, 7.07746f, 3.18518f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4526.93f, -4238.27f, 7.11127f, 6.25608f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4540.17f, -4238.74f, 7.16359f, 3.11056f }
    }
};

const creature_type g_CrossroadsNeutralSpawns[eSpawns::TheCrossroadsCreaturesCount] =
{
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4540.60f, -4201.94f, 6.95165f, 2.68286f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4543.13f, -4196.73f, 6.96004f, 2.79282f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4535.51f, -4243.45f, 7.26939f, 1.68147f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4539.97f, -4234.36f, 7.27037f, 4.51675f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4537.95f, -4169.41f, 7.09234f, 3.12659f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4535.55f, -4193.37f, 7.02832f, 5.67916f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4531.69f, -4200.82f, 7.05973f, 5.90299f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4528.11f, -4243.37f, 7.11128f, 1.50082f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4528.08f, -4234.97f, 7.14110f, 4.84268f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4527.00f, -4169.13f, 7.09213f, 6.21319f }
};

const go_type g_CrossroadsBanners[BG_TEAMS_COUNT][eSpawns::TheCrossroadsObjectsCount] =
{
    // ALLIANCE
    {
        { eGameObjects::GraveyardBannerAlliance, eAshranDatas::AshranMapID, 4546.98f, -4195.84f, 7.10f, 3.79f, 0.00f, 0.00f, 0.00f, 0.00f },
        { eGameObjects::GraveyardBannerAlliance, eAshranDatas::AshranMapID, 4513.73f, -4220.42f, 7.14f, 0.53f, 0.00f, 0.00f, 0.00f, 0.00f }
    },
    // HORDE
    {
        { eGameObjects::GraveyardBannerHorde, eAshranDatas::AshranMapID, 4546.98f, -4195.84f, 7.10f, 3.79f, 0.00f, 0.00f, 0.00f, 0.00f },
        { eGameObjects::GraveyardBannerHorde, eAshranDatas::AshranMapID, 4513.73f, -4220.42f, 7.14f, 0.53f, 0.00f, 0.00f, 0.00f, 0.00f }
    }
};

const creature_type g_TrembladesVanguardSpawns[BG_TEAMS_COUNT][eSpawns::TrembladesVanguardCreaturesCount] =
{
    // Alliance
    {
        { eCreatures::StormshieldKnight,    Team::ALLIANCE, eAshranDatas::AshranMapID, 4353.65f, -4206.14f, 10.2067f, 6.11085f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4347.50f, -4202.97f, 10.2067f, 6.11085f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4346.38f, -4208.36f, 10.2067f, 6.11085f },
        { eCreatures::StormshieldKnight,    Team::ALLIANCE, eAshranDatas::AshranMapID, 4406.39f, -4188.80f, 7.57000f, 3.17523f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4410.28f, -4191.92f, 7.46672f, 3.08886f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4410.58f, -4186.27f, 7.57197f, 3.08906f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4368.73f, -4186.40f, 10.3865f, 1.85123f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4384.90f, -4186.78f, 9.54397f, 2.45305f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4380.17f, -4191.64f, 9.98860f, 2.90894f },
        { eCreatures::StormshieldVanguard,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4358.06f, -4179.99f, 10.2831f, 2.98455f }
    },
    // Horde
    {
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4353.65f, -4206.14f, 10.2067f, 6.11085f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4347.50f, -4202.97f, 10.2067f, 6.11085f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4346.38f, -4208.36f, 10.2067f, 6.11085f },
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4406.39f, -4188.80f, 7.57000f, 3.17523f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4410.28f, -4191.92f, 7.46672f, 3.08886f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4410.58f, -4186.27f, 7.57197f, 3.08906f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4368.73f, -4186.40f, 10.3865f, 1.85123f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4384.90f, -4186.78f, 9.54397f, 2.45305f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4380.17f, -4191.64f, 9.98860f, 2.90894f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4358.06f, -4179.99f, 10.2831f, 2.98455f }
    }
};

const creature_type g_TrembladesVanguardNeutralSpawns[eSpawns::TrembladesVanguardCreaturesCount] =
{
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4375.42f, -4175.97f, 11.1146f, 2.25815f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4320.26f, -4202.23f, 10.8944f, 2.15211f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4342.93f, -4221.81f, 11.8033f, 3.66008f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4389.26f, -4201.89f, 10.8939f, 3.46087f },
    { eCreatures::WarspearGrunt,      Team::HORDE,    eAshranDatas::AshranMapID, 4384.46f, -4195.61f, 10.1722f, 3.99101f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4369.57f, -4169.96f, 11.2516f, 5.39975f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4315.65f, -4194.23f, 10.7408f, 5.36439f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4333.98f, -4230.01f, 12.4511f, 0.67163f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4379.19f, -4202.59f, 11.2631f, 0.63343f },
    { eCreatures::StormshieldFootman, Team::ALLIANCE, eAshranDatas::AshranMapID, 4382.84f, -4207.47f, 11.7597f, 0.92795f }
};

const go_type g_TrembladesVanguardFires[eSpawns::TrembladesVanguardObjectsCount] =
{
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4394.97f, -4176.13f, 13.97f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4395.08f, -4201.61f, 14.55f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4332.16f, -4221.05f, 16.76f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4316.36f, -4206.56f, 16.92f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1, eAshranDatas::AshranMapID, 4307.32f, -4195.97f, 10.80f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const creature_type g_ArchmageOverwatchSpawns[BG_TEAMS_COUNT][eSpawns::ArchmageOverwatchCreaturesCount] =
{
    // Alliance
    {
        { eCreatures::StormshieldKnight,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4235.87f, -4219.78f, 31.6387f, 2.41959f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4244.08f, -4223.27f, 30.4281f, 2.41959f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4242.08f, -4229.02f, 29.7179f, 2.45058f },
        { eCreatures::StormshieldKnight,   Team::ALLIANCE, eAshranDatas::AshranMapID, 4232.65f, -4183.01f, 31.0318f, 0.78809f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4234.03f, -4187.15f, 30.9069f, 0.78809f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4228.85f, -4181.65f, 31.0318f, 0.78809f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4261.52f, -4171.32f, 31.1670f, 2.14280f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4262.90f, -4167.40f, 31.1766f, 3.10849f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4240.65f, -4140.60f, 32.0990f, 4.85066f },
        { eCreatures::StormshieldVanguard, Team::ALLIANCE, eAshranDatas::AshranMapID, 4244.48f, -4144.43f, 32.4948f, 3.10849f }
    },
    // Horde
    {
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4235.87f, -4219.78f, 31.6387f, 2.41959f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4244.08f, -4223.27f, 30.4281f, 2.41959f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4242.08f, -4229.02f, 29.7179f, 2.45058f },
        { eCreatures::WarspearRaptorRider, Team::HORDE, eAshranDatas::AshranMapID, 4232.65f, -4183.01f, 31.0318f, 0.78809f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4234.03f, -4187.15f, 30.9069f, 0.78809f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4228.85f, -4181.65f, 31.0318f, 0.78809f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4261.52f, -4171.32f, 31.1670f, 2.14280f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4262.90f, -4167.40f, 31.1766f, 3.10849f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4240.65f, -4140.60f, 32.0990f, 4.85066f },
        { eCreatures::WarspearBloodGuard,  Team::HORDE, eAshranDatas::AshranMapID, 4244.48f, -4144.43f, 32.4948f, 3.10849f }
    }
};

const go_type g_ArchmageOverwatchFires[eSpawns::ArchmageOverwatchObjectsCount] =
{
    { eGameObjects::BonfireWithSmokeLarge1, eAshranDatas::AshranMapID, 4267.42f, -4199.62f, 61.32f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::BonfireWithSmokeLarge1, eAshranDatas::AshranMapID, 4266.43f, -4184.83f, 54.81f, 0.09f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4269.72f, -4191.55f, 47.15f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4257.58f, -4211.56f, 45.87f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4246.40f, -4191.69f, 46.00f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4234.25f, -4122.79f, 36.43f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4212.66f, -4126.20f, 37.60f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4195.31f, -4146.53f, 31.68f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4195.42f, -4159.43f, 31.68f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4239.21f, -4199.39f, 35.55f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::Smallfire1,             eAshranDatas::AshranMapID, 4220.10f, -4223.96f, 37.71f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const creature_type g_ArchmageOverwatchNeutral[eSpawns::ArchmageOverwatchCreaturesCount] =
{
    { eCreatures::WarspearGrunt,       Team::HORDE,    eAshranDatas::AshranMapID, 4271.14f, -4166.63f, 31.3710f, 3.36416f },
    { eCreatures::WarspearGrunt,       Team::HORDE,    eAshranDatas::AshranMapID, 4269.84f, -4161.86f, 31.1973f, 3.50306f },
    { eCreatures::WarspearGrunt,       Team::HORDE,    eAshranDatas::AshranMapID, 4246.79f, -4137.83f, 32.9107f, 3.57439f },
    { eCreatures::WarspearGrunt,       Team::HORDE,    eAshranDatas::AshranMapID, 4247.10f, -4139.70f, 32.9390f, 2.83231f },
    { eCreatures::WarspearGrunt,       Team::HORDE,    eAshranDatas::AshranMapID, 4232.03f, -4191.03f, 31.0079f, 1.10651f },
    { eCreatures::StormshieldFootman,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4260.53f, -4170.63f, 31.1360f, 0.23275f },
    { eCreatures::StormshieldFootman,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4259.43f, -4164.99f, 31.0425f, 0.13952f },
    { eCreatures::StormshieldFootman,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4245.96f, -4140.50f, 32.8186f, 1.27162f },
    { eCreatures::StormshieldFootman,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4244.52f, -4138.88f, 32.6640f, 0.43295f },
    { eCreatures::StormshieldFootman,  Team::ALLIANCE, eAshranDatas::AshranMapID, 4238.14f, -4180.38f, 30.9901f, 4.00969f }
};

// See order below
const go_type g_CapturePoint[eBattleType::MaxBattleType] =
{
    { eGameObjects::CapturePointBanner, eAshranDatas::AshranMapID, 4801.65f, -4211.40f, 32.9733f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::CapturePointBanner, eAshranDatas::AshranMapID, 4677.77f, -4222.42f, 10.1084f, 0.01f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::CapturePointBanner, eAshranDatas::AshranMapID, 4533.45f, -4211.45f, 7.11222f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::CapturePointBanner, eAshranDatas::AshranMapID, 4353.77f, -4190.81f, 10.0985f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f },
    { eGameObjects::CapturePointBanner, eAshranDatas::AshranMapID, 4226.71f, -4171.36f, 31.2031f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f }
};

uint32 const g_MiddleBattlesEntries[eBattleType::MaxBattleType] =
{
    eWorldStates::WorldStateEmberfallTowerBattle,
    eWorldStates::WorldStateVolrathsAdvanceBattle,
    eWorldStates::WorldStateTheCrossroadsBattle,
    eWorldStates::WorldStateTrembladesVanguardBattle,
    eWorldStates::WorldStateArchmageOverwatchBattle
};

class OutdoorPvPAshran;

class OPvPCapturePoint_Middle : public OPvPCapturePoint
{
    public:

        OPvPCapturePoint_Middle(OutdoorPvP* p_Outdoor, eBattleType p_Type, uint8 p_Faction);

        void ChangeState();

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void UpdateTowerState();

        bool Update(uint32 p_Diff);

        void SpawnFactionGuards(eBattleType p_BattleID, uint8 p_Faction);

        void SetBattleFaction(uint32 p_Faction) { m_BattleFaction = p_Faction; }
        uint32 GetBattleFaction() const { return m_BattleFaction; }
        eBattleType GetBattleType() const { return m_BattleType; }

    protected:

        eBattleType m_BattleType;
        uint32 m_BattleFaction;
};

class OPvPCapturePoint_Graveyard : public OPvPCapturePoint
{
    public:

        OPvPCapturePoint_Graveyard(OutdoorPvP* p_Outdoor);

        void ChangeState();

        void SendChangePhase();

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void UpdateTowerState();

        // Used when player is activated/inactivated in the area
        bool HandlePlayerEnter(Player* p_Player);
        void HandlePlayerLeave(Player* p_Player);

        void SpawnFactionFlags(uint8 p_Faction);

        bool Update(uint32 p_Diff);
        void ScheduleNextControl(uint32 p_Diff);

    protected:

        uint8 m_GraveyardState;
        uint32 m_ControlTime;
};

class OutdoorPvPAshran : public OutdoorPvP
{
    public:
        OutdoorPvPAshran();

        bool SetupOutdoorPvP();

        void HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID);
        void HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID);
        void HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID);
        void HandlePlayerLeaveArea(Player* p_Player, uint32 p_AreaID);

        void HandlePlayerKilled(Player* p_Player);

        bool Update(uint32 p_Diff);
        void ScheduleNextBattle(uint32 p_Diff);
        void ScheduleEndOfBattle(uint32 p_Diff);
        void ScheduleInitPoints(uint32 p_Diff);

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void SendRemoveWorldStates(Player* p_Player);

        void HandleBFMGREntryInviteResponse(bool p_Accepted, Player* p_Player);

        void OnCreatureCreate(Creature* p_Creature);
        Creature* GetHerald() const;

        void ResetControlPoints();
        void InitializeControlPoints();
        bool IsInitialized() const { return m_IsInitialized; }

        void SetBattleState(uint32 p_NewState);
        void SetNextBattleTimer(uint32 p_Time) { m_NextBattleTimer = p_Time * TimeConstants::IN_MILLISECONDS; }

        void AddGenericMoPGuid(uint8 p_Type, uint64 p_Guid) { m_GenericMoPGuids[p_Type] = p_Guid; }
        uint64 GetGenericMoPGuid(uint8 p_Type) const { return m_GenericMoPGuids[p_Type]; }
        uint64 GetFactionGenericMoP(uint8 p_Faction) const { return m_FactionGenericMoP[p_Faction]; }

        void HandleFactionBossDeath(uint8 p_Faction);

        OPvPCapturePoint_Middle* GetCapturePoint(uint8 p_Index) const { return m_ControlPoints[p_Index]; }

    private:

        OPvPCapturePoint_Graveyard* m_GraveYard;
        OPvPCapturePoint_Middle* m_ControlPoints[eBattleType::MaxBattleType];
        uint64 m_GenericMoPGuids[eBattleType::MaxBattleType];
        uint64 m_FactionGenericMoP[BG_TEAMS_COUNT];
        uint32 m_InitPointsTimer;
        bool m_IsInitialized;
        bool m_WillBeReset;

        uint64 m_Guid;
        uint64 m_HeraldGuid;
        uint64 m_HighWarlordVolrath;
        uint64 m_GrandMasrhalTremblade;
        uint32 m_WorldPvPAreaId;

        GuidSet m_PlayersInWar[BG_TEAMS_COUNT];
        PlayerTimerMap m_InvitedPlayers[BG_TEAMS_COUNT];
        PlayerTimerMap m_PlayersWillBeKick[BG_TEAMS_COUNT];

        uint32 m_EnnemiesKilled[BG_TEAMS_COUNT];
        uint32 m_EnnemiesKilledMax[BG_TEAMS_COUNT];

        uint32 m_CurrentBattleState;
        uint32 m_NextBattleTimer;
        uint32 m_MaxBattleTime;
};

#endif
