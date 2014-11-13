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
    ASHRAN_ZONE_ID              = 6941,
    ASHRAN_MAP_ID               = 1191,
    ASHRAN_NEUTRAL_MAP_ID       = 1116,
    ASHRAN_PRE_AREA_HORDE       = 7333,
    ASHRAN_PRE_AREA_ALLIANCE    = 7332,
    ASHRAN_HORDE_BASE           = 7099,
    ASHRAN_ALLIANCE_BASE        = 7100,
    ASHRAN_WORLD_PVP_AREA_ID    = 24,
    BATTLEFIELD_TYPE_WORLD_PVP  = 0x20000,
    PLAYER_MIN_LEVEL            = 100,
    ASHRAN_TIME_FOR_INVITE      = 20,
    ASHRAN_TIME_FOR_BATTLE      = 25,
    ASHRAN_HALLOWED_GROUND_H    = 7782,
    ASHRAN_HALLOWED_GROUND_A    = 7781,
    ASHRAN_HALLOWED_GROUND_ID   = 42989,
    ASHRAN_GENERIC_MOB_TYPEID   = 68553
};

enum eAshranSpells
{
    SPELL_LOOTABLE          = 161733,
    SPELL_HOLD_YOUR_GROUND  = 173534,   // +30% damage, healing and health
    SPELL_TOWER_DEFENSE     = 173541,   // +20% damage, healing and health
    SPELL_STAND_FAST        = 173549,   // +10% damage, healing and health
    SPELL_HALLOWED_GROUND   = 171496
};

enum eAshranWorldStates
{
    WORLD_STATE_DISABLED                        = 0,
    WORLD_STATE_ENABLED                         = 1,
    //////////////////////////////////////////////////
    // Molten Quarry(southeast) - Event: Collect Empowered Ore
    WORLD_STATE_ORE_COLLECTED_ALLIANCE          = 1581,
    WORLD_STATE_ORE_COLLECTED_HORDE             = 1582,
    WORLD_STATE_ENABLE_ORE_COLLECTION           = 9274,
    // Molten Quarry(southeast) - Event: Collect Empowered Ore
    //////////////////////////////////////////////////
    WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE         = 8933,
    WORLD_STATE_ENNEMIES_SLAIN_HORDE            = 8934,
    WORLD_STATE_NEXT_BATTLE_TIMESTAMP           = 8945,
    WORLD_STATE_NEXT_BATTLE_ENABLED             = 8946,
    WORLD_STATE_ACTIVE_STAGE                    = 8955,
    //////////////////////////////////////////////////
    // Risen Spirits (Ashmaul Burial Grounds)
    WORLD_STATE_RISEN_SPIRITS_CAPTURE_ENABLED   = 9199,
    WORLD_STATE_RISEN_SPIRITS_CAPTURED_ALLIANCE = 9200,
    WORLD_STATE_RISEN_SPIRITS_CAPTURED_HORDE    = 9201,
    // Risen Spirits (Ashmaul Burial Grounds)
    //////////////////////////////////////////////////
    // Amphitheater of Annihilation (northwest) - Event: The Race!
    WORLD_STATE_ENABLE_LAPS_EVENT               = 9290,
    WORLD_STATE_LAPS_ALLIANCE                   = 9291,
    WORLD_STATE_LAPS_HORDE                      = 9292,
    // Amphitheater of Annihilation (northwest) - Event: The Race!
    //////////////////////////////////////////////////
    WORLD_STATE_TIME_REMAINING_FOR_BOSS         = 9316,
    WORLD_STATE_SLAY_VOLRATH                    = 9317, // Enable time remaining for boss
    WORLD_STATE_SLAY_TREMBLADE                  = 9326, // Enable time remaining for boss
    //////////////////////////////////////////////////
    // Brute's Rise (northeast) - Event: Ogre Fires!
    WORLD_STATE_FIRES_SCORING_ENABLED           = 9414,
    WORLD_STATE_FIRES_SCORING_ALLIANCE          = 9418,
    WORLD_STATE_FIRES_SCORING_HORDE             = 9419,
    WORLD_STATE_FIRES_CONTROLLED_ALLIANCE       = 9782,
    WORLD_STATE_FIRES_CONTROLLED_HORDE          = 9783,
    WORLD_STATE_FIRES_SCORING_MAX               = 9784,
    // Brute's Rise (northeast) - Event: Ogre Fires!
    //////////////////////////////////////////////////
    WORLD_STATE_HIGH_WARLORD_VOLRATH            = 9713,
    WORLD_STATE_EMBERFALL_TOWER_BATTLE          = 9714,
    WORLD_STATE_VOLRATHS_ADVANCE_BATTLE         = 9715,
    WORLD_STATE_THE_CROSSROADS_BATTLE           = 9716,
    WORLD_STATE_TREMBLADES_VANGUARD_BATTLE      = 9717,
    WORLD_STATE_ARCHMAGE_OVERWATCH_BATTLE       = 9718,
    WORLD_STATE_GRAND_MARSHAL_TREMBLADE_BATTLE  = 9719,
    WORLD_STATE_CONTROL_THE_FLAG                = 9785,
    WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE_MAX     = 9801,
    WORLD_STATE_ENNEMIES_SLAIN_HORDE_MAX        = 9802,
    //////////////////////////////////////////////////
    // Kings' Rest (west)
    WORLD_STATE_ENABLE_GRAVEYARD_PROGRESS_BAR   = 9053,
    WORLD_STATE_GRAVEYARD_PROGRESS_BAR          = 9054,
    WORLD_STATE_GRAVEYARD_PROGRESS_BAR_GREY_PCT = 9055,
    // Kings' Rest (west)
    //////////////////////////////////////////////////
    // Five Towers line the Road of Glory
    WORLD_STATE_ENABLE_TOWER_PROGRESS_BAR       = 9110,
    WORLD_STATE_TOWER_PROGRESS_BAR              = 9111,
    WORLD_STATE_TOWER_PROGRESS_BAR_GREY_PCT     = 9112,
    // Five Towers line the Road of Glory
    //////////////////////////////////////////////////
    // Five Towers line - Control Status
    WORLD_STATE_WARSPEAR_OUTPOST_STATUS         = 9100, // 0 - In fight, 1 - Horde, 2 - Destroyed
    WORLD_STATE_EMBERFALL_TOWER_STATUS          = 9101, // 0 - Neutral, 1 - Horde, 2 - Alliance
    WORLD_STATE_VOLRATHS_ADVANCE_STATUS         = 9102, // 0 - Neutral, 1 - Horde, 2 - Alliance
    WORLD_STATE_THE_CROSSROADS_STATUS           = 9103, // 0 - Neutral, 1 - Horde, 2 - Alliance
    WORLD_STATE_TREMBLADES_VANGUARD_STATUS      = 9104, // 0 - Neutral, 1 - Horde, 2 - Alliance
    WORLD_STATE_ARCHMAGE_OVERWATCH_STATUS       = 9105, // 0 - Neutral, 1 - Horde, 2 - Alliance
    WORLD_STATE_STORMSHIELD_STRONGHOLD_STATUS   = 9106, // 0 - In fight, 1 - Destroyed, 2 - Alliance
    // Five Towers line - Control Status
    //////////////////////////////////////////////////
    // Not implemented yet
    WORLD_STATE_OGRE_KING_THRONE_STATUS         = 9113  // 0 - Neutral, 1 - Horde
};

enum eAshranControlStatus
{
    CONTROL_NEUTRAL     = 0,
    CONTROL_HORDE       = 1,
    CONTROL_ALLIANCE    = 2
};

enum eAshranCreatures
{
    ASHRAN_HERALD                   = 84113,
    ASHRAN_BLADE_TWISTER_TRIGGER    = 89320,
    ASHRAN_WARSPEAR_BLOOD_GUARD     = 83699,
    ASHRAN_WARSPEAR_RAPTOR_RIDER    = 80297,
    ASHRAN_WARSPEAR_HEADHUNTER      = 79993,
    ASHRAN_WARSPEAR_GRUNT           = 79269,
    ASHRAN_WARSPEAR_PRIEST          = 79982,
    ASHRAN_HIGH_WARLORD_VOLRATH     = 82877,
    ASHRAN_STORMSHIELD_VANGUARD     = 83717,
    ASHRAN_STORMSHIELD_KNIGHT       = 80256,
    ASHRAN_STORMSHIELD_SENTINEL     = 79990,
    ASHRAN_STORMSHIELD_FOOTMAN      = 79268,
    ASHRAN_STORMSHIELD_PRIEST       = 79947,
    ASHRAN_GRAND_MARSHAL_TREMBLADE  = 82876,
    SLG_GENERIC_MOP_LARGE_AOI       = 68553
};

enum eAshranGameObjects
{
    GRAVEYARD_BANNER_HORDE      = 233518,
    GRAVEYARD_BANNER_ALLIANCE   = 233517,
    GRAVEYARD_CONTROL_BANNER    = 231201,
    CAPTURE_POINT_BANNER        = 230876,
    BONFIRE_WITH_SMOKE_LARGE_1  = 233531,
    SMALLFIRE_1                 = 233534
};

enum eAshranActions
{
    ACTION_ANNOUNCE_MARKETPLACE_GRAVEYARD,
    ACTION_ANNOUNCE_HORDE_GRAVEYARD_OWNER,
    ACTION_ANNOUNCE_ALLIANCE_GRAVEYARD_OWNER,
    ACTION_ANNOUNCE_HORDE_VICTORY,
    ACTION_ANNOUNCE_ALLIANCE_KILL_BOSS,
    ACTION_ANNOUNCE_ALLIANCE_VICTORY,
    ACTION_ANNOUNCE_HORDE_KILL_BOSS,
    ACTION_WARSPEAR_OUTPOST_IN_FIGHT,
    ACTION_STORMSHIELD_STRONGHOLD_IN_FIGHT,
    ACTION_WARSPEAR_VICTORY,
    ACTION_STORMSHIELD_VICTORY
};

enum eGraveyardsIDs
{
    GRAVEYARD_ALLIANCE_BASE     = 4742, // Ashran - Lane - Base GY (A)
    GRAVEYARD_HORDE_BASE        = 4743, // Ashran - Lane - Base GY (H)
    GRAVEYARD_ALLIANCE_CENTER   = 4822, // Ashran - Lane - Center GY (A)
    GRAVEYARD_HORDE_CENTER      = 4825, // Ashran - Lane - Center GY (H)
    GRAVEYARD_STAGE_1_ALLIANCE  = 4769, // Ashran - Lane - Stage 1 (Horde Approach) - Alliance GY
    GRAVEYARD_STAGE_1_HORDE     = 4770, // Ashran - Lane - Stage 1 (Horde Approach) - Horde GY
    GRAVEYARD_STAGE_3_ALLIANCE  = 4768, // Ashran - Lane - Stage 3 (Alliance Approach) - Alliance GY
    GRAVEYARD_STAGE_3_HORDE     = 4767, // Ashran - Lane - Stage 3 (Alliance Approach) - Horde GY
    GRAVEYARD_TOWER_ALLIANCE    = 4821, // Ashran - Lane - Tower GY (A)
    GRAVEYARD_TOWER_HORDE       = 4824, // Ashran - Lane - Tower GY (H)
    GRAVEYARD_QUARRY_ALLIANCE   = 4717, // Ashran - Quarry - Alliance TEMP GY
    GRAVEYARD_QUARRY_HORDE      = 4718, // Ashran - Quarry - Horde TEMP GY
    GRAVEYARD_ARENA_ALLIANCE    = 4730, // Ashran - The Arena - Alliance GY
    GRAVEYARD_ARENA_HORDE       = 4731  // Ashran - The Arena - Horde GY
};

Position const g_HordeTeleportPos = { 5216.443359f, -3963.191406f, 5.553593f, 6.242684f };
Position const g_AllianceTeleportPos = { 3849.396240f, -4013.051025f, 26.282335f, 3.141932f };

uint32 const g_HallowedGroundEntries[BG_TEAMS_COUNT] =
{
    ASHRAN_HALLOWED_GROUND_H,
    ASHRAN_HALLOWED_GROUND_A
};

Position const g_HallowedGroundPos[BG_TEAMS_COUNT] =
{
    { 5090.467f, -4076.731f, 49.38393f, 3.379836f },    // ASHRAN_HALLOWED_GROUND_H
    { 3928.052f, -4032.738f, 57.41695f, 5.473989f }     // ASHRAN_HALLOWED_GROUND_A
};

enum eAshranBanners
{
    GRAVEYARD_BANNER_0,
    GRAVEYARD_BANNER_1,
    GRAVEYARD_BANNER_2,
    GRAVEYARD_BANNER_3,
    GRAVEYARD_MAX_BANNER
};

const go_type g_GraveyardBanner_H[GRAVEYARD_MAX_BANNER] =
{
    { GRAVEYARD_BANNER_HORDE, ASHRAN_MAP_ID, 4527.93f, -3999.18f, 5.95707f, 0.588123f, 0.00f, 0.00f, 0.00f, 0.00f },
    { GRAVEYARD_BANNER_HORDE, ASHRAN_MAP_ID, 4528.02f, -4006.75f, 6.05358f, 2.277280f, 0.00f, 0.00f, 0.00f, 0.00f },
    { GRAVEYARD_BANNER_HORDE, ASHRAN_MAP_ID, 4537.33f, -3999.50f, 6.13882f, 5.566630f, 0.00f, 0.00f, 0.00f, 0.00f },
    { GRAVEYARD_BANNER_HORDE, ASHRAN_MAP_ID, 4536.55f, -4006.41f, 6.38824f, 4.122270f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const go_type g_GraveyardBanner_A[GRAVEYARD_MAX_BANNER] =
{
    { GRAVEYARD_BANNER_ALLIANCE, ASHRAN_MAP_ID, 4527.93f, -3999.18f, 5.95707f, 0.588123f, 0.00f, 0.00f, 0.00f, 0.00f },
    { GRAVEYARD_BANNER_ALLIANCE, ASHRAN_MAP_ID, 4528.02f, -4006.75f, 6.05358f, 2.277280f, 0.00f, 0.00f, 0.00f, 0.00f },
    { GRAVEYARD_BANNER_ALLIANCE, ASHRAN_MAP_ID, 4537.33f, -3999.50f, 6.13882f, 5.566630f, 0.00f, 0.00f, 0.00f, 0.00f },
    { GRAVEYARD_BANNER_ALLIANCE, ASHRAN_MAP_ID, 4536.55f, -4006.41f, 6.38824f, 4.122270f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const go_type g_GraveyardBanner_N = { GRAVEYARD_CONTROL_BANNER, ASHRAN_MAP_ID, 4532.632f, -4003.269f, 6.317888f, 0.0f, 0.0f, 0.0f, 0.008727f, -0.999962f };

enum eAshranFlagStates
{
    FLAG_NEUTRAL    = 21,
    FLAG_HORDE      = 1,
    FLAG_ALLIANCE   = 2
};

enum eAshranBattleType
{
    BATTLE_TYPE_EMBERFALL_TOWER,
    BATTLE_TYPE_VOLRATHS_ADVANCE,
    BATTLE_TYPE_THE_CROSSROADS,
    BATTLE_TYPE_TREMBLADES_VANGUARD,
    BATTLE_TYPE_ARCHMAGE_OVERWATCH,
    BATTLE_TYPE_MAX
};

uint32 const g_TowerControlStatus[BATTLE_TYPE_MAX] =
{
    WORLD_STATE_EMBERFALL_TOWER_STATUS,
    WORLD_STATE_VOLRATHS_ADVANCE_STATUS,
    WORLD_STATE_THE_CROSSROADS_STATUS,
    WORLD_STATE_TREMBLADES_VANGUARD_STATUS,
    WORLD_STATE_ARCHMAGE_OVERWATCH_STATUS
};

enum eAshranSpawnsCount
{
    EMBERFALL_TOWER_CREATURES_COUNT     = 15,
    EMBERFALL_TOWER_OBJECTS_COUNT       = 12,
    EMBERFALL_TOWER_SPAWNS_IDS          = EMBERFALL_TOWER_CREATURES_COUNT + EMBERFALL_TOWER_OBJECTS_COUNT,
    VOLRATHS_ADVANCE_CREATURES_COUNT    = 18,
    VOLRATHS_ADVANCE_OBJECT_COUNT       = 5,
    VOLRATHS_ADVANCE_SPAWNS_IDS         = EMBERFALL_TOWER_SPAWNS_IDS + VOLRATHS_ADVANCE_OBJECT_COUNT + VOLRATHS_ADVANCE_CREATURES_COUNT,
    THE_CROSSROADS_CREATURES_COUNT      = 10,
    THE_CROSSROADS_OBJECT_COUNT         = 2,
    THE_CROSSROADS_SPAWNS_IDS           = VOLRATHS_ADVANCE_SPAWNS_IDS + THE_CROSSROADS_OBJECT_COUNT + THE_CROSSROADS_CREATURES_COUNT,
    TREMBLADES_VANGUARD_CREATURES_COUNT = 10,
    TREMBLADES_VANGUARD_OBJECT_COUNT    = 5,
    TREMBLADES_VANGUARD_SPAWNS_IDS      = THE_CROSSROADS_SPAWNS_IDS + TREMBLADES_VANGUARD_OBJECT_COUNT + TREMBLADES_VANGUARD_CREATURES_COUNT,
    ARCHMAGE_OVERWATCH_CREATURES_COUNT  = 10,
    ARCHMAGE_OVERWATCH_OBJECT_COUNT     = 11,
    ARCHMAGE_OVERWATCH_SPAWNS_IDS       = TREMBLADES_VANGUARD_SPAWNS_IDS + ARCHMAGE_OVERWATCH_CREATURES_COUNT + ARCHMAGE_OVERWATCH_OBJECT_COUNT,
    ARCHMAGE_OVERWATCH_NEUTRAL_COUNT    = 10,
    TOTAL_NEUTRAL_SPAWNS_IDS            = ARCHMAGE_OVERWATCH_SPAWNS_IDS + ARCHMAGE_OVERWATCH_NEUTRAL_COUNT
};

const creature_type g_EmberfallTowerSpawns[BG_TEAMS_COUNT][EMBERFALL_TOWER_CREATURES_COUNT] =
{
    // ALLIANCE
    {
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4791.99f, -4180.81f, 33.22f, 4.95f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4788.07f, -4182.17f, 33.22f, 5.80f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4775.78f, -4217.50f, 32.58f, 0.00f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4776.99f, -4220.96f, 32.33f, 0.91f },
        { ASHRAN_STORMSHIELD_SENTINEL,  ALLIANCE, ASHRAN_MAP_ID, 4788.38f, -4226.64f, 56.56f, 0.80f },
        { ASHRAN_STORMSHIELD_SENTINEL,  ALLIANCE, ASHRAN_MAP_ID, 4780.22f, -4219.33f, 56.11f, 1.43f },
        { ASHRAN_STORMSHIELD_SENTINEL,  ALLIANCE, ASHRAN_MAP_ID, 4795.50f, -4237.04f, 56.35f, 6.15f },
        { ASHRAN_STORMSHIELD_SENTINEL,  ALLIANCE, ASHRAN_MAP_ID, 4785.99f, -4247.97f, 56.57f, 5.44f },
        { ASHRAN_STORMSHIELD_SENTINEL,  ALLIANCE, ASHRAN_MAP_ID, 4767.79f, -4228.28f, 56.62f, 2.25f },
        { ASHRAN_STORMSHIELD_KNIGHT,    ALLIANCE, ASHRAN_MAP_ID, 4781.27f, -4265.98f, 27.76f, 3.72f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4783.97f, -4261.41f, 28.89f, 3.72f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4787.13f, -4266.29f, 28.49f, 3.72f },
        { ASHRAN_STORMSHIELD_KNIGHT,    ALLIANCE, ASHRAN_MAP_ID, 4816.92f, -4207.30f, 32.73f, 1.97f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4821.17f, -4209.80f, 32.63f, 1.97f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4813.77f, -4212.76f, 33.13f, 1.97f }
    },
    // HORDE
    {
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4791.99f, -4180.81f, 33.22f, 4.95f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4788.07f, -4182.17f, 33.22f, 5.80f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4775.78f, -4217.50f, 32.58f, 0.00f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4776.99f, -4220.96f, 32.33f, 0.91f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4788.38f, -4226.64f, 56.56f, 0.80f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4780.22f, -4219.33f, 56.11f, 1.43f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4795.50f, -4237.04f, 56.35f, 6.15f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4785.99f, -4247.97f, 56.57f, 5.44f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4767.79f, -4228.28f, 56.62f, 2.25f },
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4781.27f, -4265.98f, 27.76f, 3.72f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4783.97f, -4261.41f, 28.89f, 3.72f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4787.13f, -4266.29f, 28.49f, 3.72f },
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4816.92f, -4207.30f, 32.73f, 1.97f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4821.17f, -4209.80f, 32.63f, 1.97f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4813.77f, -4212.76f, 33.13f, 1.97f }
    }
};

const creature_type g_VolrathsAdvanceSpawns[BG_TEAMS_COUNT][VOLRATHS_ADVANCE_CREATURES_COUNT] =
{
    // ALLIANCE
    {
        { ASHRAN_STORMSHIELD_SENTINEL, ALLIANCE, ASHRAN_MAP_ID, 4671.80f, -4242.89f, 29.6425f, 6.10519f },
        { ASHRAN_STORMSHIELD_SENTINEL, ALLIANCE, ASHRAN_MAP_ID, 4653.12f, -4206.77f, 28.3985f, 4.78364f },
        { ASHRAN_STORMSHIELD_SENTINEL, ALLIANCE, ASHRAN_MAP_ID, 4679.35f, -4242.53f, 28.9622f, 2.04980f },
        { ASHRAN_STORMSHIELD_SENTINEL, ALLIANCE, ASHRAN_MAP_ID, 4654.56f, -4199.31f, 27.2997f, 5.45237f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4652.03f, -4218.00f, 9.92943f, 4.59145f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4652.04f, -4223.96f, 9.92325f, 1.58571f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4654.54f, -4224.20f, 11.4234f, 0.00943f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4654.81f, -4218.55f, 11.4234f, 0.00935f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4683.18f, -4211.89f, 10.3594f, 3.00827f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4683.80f, -4205.95f, 10.1736f, 3.14209f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4688.85f, -4230.05f, 11.1861f, 0.24750f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4693.90f, -4232.60f, 11.1861f, 0.43090f },
        { ASHRAN_STORMSHIELD_KNIGHT,   ALLIANCE, ASHRAN_MAP_ID, 4650.39f, -4220.74f, 9.55521f, 6.17425f },
        { ASHRAN_STORMSHIELD_KNIGHT,   ALLIANCE, ASHRAN_MAP_ID, 4681.88f, -4208.51f, 10.2640f, 3.28998f },
        { ASHRAN_STORMSHIELD_PRIEST,   ALLIANCE, ASHRAN_MAP_ID, 4725.44f, -4203.77f, 22.3601f, 3.56446f },
        { ASHRAN_STORMSHIELD_PRIEST,   ALLIANCE, ASHRAN_MAP_ID, 4651.05f, -4211.01f, 9.87623f, 5.53973f },
        { ASHRAN_STORMSHIELD_PRIEST,   ALLIANCE, ASHRAN_MAP_ID, 4668.79f, -4257.62f, 11.5811f, 1.59311f },
        { ASHRAN_STORMSHIELD_PRIEST,   ALLIANCE, ASHRAN_MAP_ID, 4682.44f, -4184.54f, 21.8149f, 4.45475f }
    },
    // HORDE
    {
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4671.80f, -4242.89f, 29.6425f, 6.10519f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4653.12f, -4206.77f, 28.3985f, 4.78364f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4679.35f, -4242.53f, 28.9622f, 2.04980f },
        { ASHRAN_WARSPEAR_HEADHUNTER,   HORDE, ASHRAN_MAP_ID, 4654.56f, -4199.31f, 27.2997f, 5.45237f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4652.03f, -4218.00f, 9.92943f, 4.59145f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4652.04f, -4223.96f, 9.92325f, 1.58571f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4654.54f, -4224.20f, 11.4234f, 0.00943f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4654.81f, -4218.55f, 11.4234f, 0.00935f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4683.18f, -4211.89f, 10.3594f, 3.00827f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4683.80f, -4205.95f, 10.1736f, 3.14209f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4688.85f, -4230.05f, 11.1861f, 0.24750f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4693.90f, -4232.60f, 11.1861f, 0.43090f },
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4650.39f, -4220.74f, 9.55521f, 6.17425f },
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4681.88f, -4208.51f, 10.2640f, 3.28998f },
        { ASHRAN_WARSPEAR_PRIEST,       HORDE, ASHRAN_MAP_ID, 4725.44f, -4203.77f, 22.3601f, 3.56446f },
        { ASHRAN_WARSPEAR_PRIEST,       HORDE, ASHRAN_MAP_ID, 4651.05f, -4211.01f, 9.87623f, 5.53973f },
        { ASHRAN_WARSPEAR_PRIEST,       HORDE, ASHRAN_MAP_ID, 4668.79f, -4257.62f, 11.5811f, 1.59311f },
        { ASHRAN_WARSPEAR_PRIEST,       HORDE, ASHRAN_MAP_ID, 4682.44f, -4184.54f, 21.8149f, 4.45475f }
    }
};

const go_type g_EmberfallFiresSpawns[EMBERFALL_TOWER_OBJECTS_COUNT] =
{
    { BONFIRE_WITH_SMOKE_LARGE_1, ASHRAN_MAP_ID, 4778.00f, -4224.00f, 56.68f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { BONFIRE_WITH_SMOKE_LARGE_1, ASHRAN_MAP_ID, 4790.39f, -4241.27f, 56.15f, 0.09f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4812.25f, -4252.67f, 37.19f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4796.12f, -4234.99f, 37.11f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4827.44f, -4195.64f, 37.34f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4816.45f, -4164.16f, 35.71f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4799.46f, -4164.38f, 35.47f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4776.97f, -4185.94f, 41.45f, 2.35f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4769.31f, -4182.60f, 46.10f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4756.42f, -4175.85f, 38.16f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4749.02f, -4183.04f, 38.13f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4768.50f, -4245.03f, 47.74f, 3.60f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const go_type g_VolrathsAdvanceFires[VOLRATHS_ADVANCE_OBJECT_COUNT] =
{
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4720.46f, -4225.85f, 16.50f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4715.67f, -4239.35f, 13.88f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4703.38f, -4247.46f, 13.71f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4642.08f, -4236.12f, 13.30f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4646.33f, -4207.88f, 12.42f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const creature_type g_CrossroadSpawns[BG_TEAMS_COUNT][THE_CROSSROADS_CREATURES_COUNT] =
{
    // Alliance
    {
        { ASHRAN_STORMSHIELD_KNIGHT,    ALLIANCE, ASHRAN_MAP_ID, 4478.58f, -4198.21f, 6.18646f, 2.85147f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4481.10f, -4196.31f, 7.18646f, 2.85147f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4480.48f, -4201.73f, 2.92569f, 2.85147f },
        { ASHRAN_STORMSHIELD_KNIGHT,    ALLIANCE, ASHRAN_MAP_ID, 4592.31f, -4218.70f, 7.07966f, 3.05845f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4593.84f, -4221.67f, 7.27999f, 3.05845f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4594.30f, -4216.03f, 7.27999f, 3.05845f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4526.08f, -4183.99f, 7.08054f, 0.02788f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4537.70f, -4183.91f, 7.07746f, 3.18518f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4526.93f, -4238.27f, 7.11127f, 6.25608f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4540.17f, -4238.74f, 7.16359f, 3.11056f }
    },
    // Horde
    {
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4478.58f, -4198.21f, 6.18646f, 2.85147f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4481.10f, -4196.31f, 7.18646f, 2.85147f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4480.48f, -4201.73f, 2.92569f, 2.85147f },
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4592.31f, -4218.70f, 7.07966f, 3.05845f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4593.84f, -4221.67f, 7.27999f, 3.05845f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4594.30f, -4216.03f, 7.27999f, 3.05845f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4526.08f, -4183.99f, 7.08054f, 0.02788f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4537.70f, -4183.91f, 7.07746f, 3.18518f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4526.93f, -4238.27f, 7.11127f, 6.25608f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4540.17f, -4238.74f, 7.16359f, 3.11056f }
    }
};

const go_type g_CrossroadsBanners[BG_TEAMS_COUNT][THE_CROSSROADS_OBJECT_COUNT] =
{
    // ALLIANCE
    {
        { GRAVEYARD_BANNER_ALLIANCE, ASHRAN_MAP_ID, 4546.98f, -4195.84f, 7.10f, 3.79f, 0.00f, 0.00f, 0.00f, 0.00f },
        { GRAVEYARD_BANNER_ALLIANCE, ASHRAN_MAP_ID, 4513.73f, -4220.42f, 7.14f, 0.53f, 0.00f, 0.00f, 0.00f, 0.00f }
    },
    // HORDE
    {
        { GRAVEYARD_BANNER_HORDE, ASHRAN_MAP_ID, 4546.98f, -4195.84f, 7.10f, 3.79f, 0.00f, 0.00f, 0.00f, 0.00f },
        { GRAVEYARD_BANNER_HORDE, ASHRAN_MAP_ID, 4513.73f, -4220.42f, 7.14f, 0.53f, 0.00f, 0.00f, 0.00f, 0.00f }
    }
};

const creature_type g_TrembladesVanguardSpawns[BG_TEAMS_COUNT][TREMBLADES_VANGUARD_CREATURES_COUNT] =
{
    // Alliance
    {
        { ASHRAN_STORMSHIELD_KNIGHT,    ALLIANCE, ASHRAN_MAP_ID, 4353.65f, -4206.14f, 10.2067f, 6.11085f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4347.50f, -4202.97f, 10.2067f, 6.11085f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4346.38f, -4208.36f, 10.2067f, 6.11085f },
        { ASHRAN_STORMSHIELD_KNIGHT,    ALLIANCE, ASHRAN_MAP_ID, 4406.39f, -4188.80f, 7.57000f, 3.17523f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4410.28f, -4191.92f, 7.46672f, 3.08886f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4410.58f, -4186.27f, 7.57197f, 3.08906f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4368.73f, -4186.40f, 10.3865f, 1.85123f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4384.90f, -4186.78f, 9.54397f, 2.45305f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4380.17f, -4191.64f, 9.98860f, 2.90894f },
        { ASHRAN_STORMSHIELD_VANGUARD,  ALLIANCE, ASHRAN_MAP_ID, 4358.06f, -4179.99f, 10.2831f, 2.98455f }
    },
    // Horde
    {
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4353.65f, -4206.14f, 10.2067f, 6.11085f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4347.50f, -4202.97f, 10.2067f, 6.11085f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4346.38f, -4208.36f, 10.2067f, 6.11085f },
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4406.39f, -4188.80f, 7.57000f, 3.17523f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4410.28f, -4191.92f, 7.46672f, 3.08886f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4410.58f, -4186.27f, 7.57197f, 3.08906f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4368.73f, -4186.40f, 10.3865f, 1.85123f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4384.90f, -4186.78f, 9.54397f, 2.45305f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4380.17f, -4191.64f, 9.98860f, 2.90894f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4358.06f, -4179.99f, 10.2831f, 2.98455f }
    }
};

const go_type g_TrembladesVanguardFires[TREMBLADES_VANGUARD_OBJECT_COUNT] =
{
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4394.97f, -4176.13f, 13.97f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4395.08f, -4201.61f, 14.55f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4332.16f, -4221.05f, 16.76f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4316.36f, -4206.56f, 16.92f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1, ASHRAN_MAP_ID, 4307.32f, -4195.97f, 10.80f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const creature_type g_ArchmageOverwatchSpawns[BG_TEAMS_COUNT][ARCHMAGE_OVERWATCH_CREATURES_COUNT] =
{
    // Alliance
    {
        { ASHRAN_STORMSHIELD_KNIGHT,   ALLIANCE, ASHRAN_MAP_ID, 4235.87f, -4219.78f, 31.6387f, 2.41959f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4244.08f, -4223.27f, 30.4281f, 2.41959f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4242.08f, -4229.02f, 29.7179f, 2.45058f },
        { ASHRAN_STORMSHIELD_KNIGHT,   ALLIANCE, ASHRAN_MAP_ID, 4232.65f, -4183.01f, 31.0318f, 0.78809f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4228.64f, -4182.23f, 31.0318f, 0.78809f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4228.85f, -4181.65f, 31.0318f, 0.78809f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4261.52f, -4171.32f, 31.1670f, 2.14280f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4262.90f, -4167.40f, 31.1766f, 3.10849f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4240.65f, -4140.60f, 32.0990f, 4.85066f },
        { ASHRAN_STORMSHIELD_VANGUARD, ALLIANCE, ASHRAN_MAP_ID, 4244.48f, -4144.43f, 32.4948f, 3.10849f }
    },
    // Horde
    {
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4235.87f, -4219.78f, 31.6387f, 2.41959f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4244.08f, -4223.27f, 30.4281f, 2.41959f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4242.08f, -4229.02f, 29.7179f, 2.45058f },
        { ASHRAN_WARSPEAR_RAPTOR_RIDER, HORDE, ASHRAN_MAP_ID, 4232.65f, -4183.01f, 31.0318f, 0.78809f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4228.64f, -4182.23f, 31.0318f, 0.78809f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4228.85f, -4181.65f, 31.0318f, 0.78809f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4261.52f, -4171.32f, 31.1670f, 2.14280f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4262.90f, -4167.40f, 31.1766f, 3.10849f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4240.65f, -4140.60f, 32.0990f, 4.85066f },
        { ASHRAN_WARSPEAR_BLOOD_GUARD,  HORDE, ASHRAN_MAP_ID, 4244.48f, -4144.43f, 32.4948f, 3.10849f }
    }
};

const go_type g_ArchmageOverwatchFires[ARCHMAGE_OVERWATCH_OBJECT_COUNT] =
{
    { BONFIRE_WITH_SMOKE_LARGE_1, ASHRAN_MAP_ID, 4267.42f, -4199.62f, 61.32f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { BONFIRE_WITH_SMOKE_LARGE_1, ASHRAN_MAP_ID, 4266.43f, -4184.83f, 54.81f, 0.09f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4269.72f, -4191.55f, 47.15f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4257.58f, -4211.56f, 45.87f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4246.40f, -4191.69f, 46.00f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4234.25f, -4122.79f, 36.43f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4212.66f, -4126.20f, 37.60f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4195.31f, -4146.53f, 31.68f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4195.42f, -4159.43f, 31.68f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4239.21f, -4199.39f, 35.55f, 0.41f, 0.00f, 0.00f, 0.00f, 0.00f },
    { SMALLFIRE_1,                ASHRAN_MAP_ID, 4220.10f, -4223.96f, 37.71f, 0.37f, 0.00f, 0.00f, 0.00f, 0.00f }
};

const creature_type g_ArchmageOverwatchNeutral[ARCHMAGE_OVERWATCH_NEUTRAL_COUNT] =
{
    { ASHRAN_WARSPEAR_GRUNT,       HORDE,    ASHRAN_MAP_ID, 4271.14f, -4166.63f, 31.3710f, 3.36416f },
    { ASHRAN_WARSPEAR_GRUNT,       HORDE,    ASHRAN_MAP_ID, 4269.84f, -4161.86f, 31.1973f, 3.50306f },
    { ASHRAN_WARSPEAR_GRUNT,       HORDE,    ASHRAN_MAP_ID, 4246.79f, -4137.83f, 32.9107f, 3.57439f },
    { ASHRAN_WARSPEAR_GRUNT,       HORDE,    ASHRAN_MAP_ID, 4247.10f, -4139.70f, 32.9390f, 2.83231f },
    { ASHRAN_WARSPEAR_GRUNT,       HORDE,    ASHRAN_MAP_ID, 4232.03f, -4191.03f, 31.0079f, 1.10651f },
    { ASHRAN_STORMSHIELD_FOOTMAN,  ALLIANCE, ASHRAN_MAP_ID, 4260.53f, -4170.63f, 31.1360f, 0.23275f },
    { ASHRAN_STORMSHIELD_FOOTMAN,  ALLIANCE, ASHRAN_MAP_ID, 4259.43f, -4164.99f, 31.0425f, 0.13952f },
    { ASHRAN_STORMSHIELD_FOOTMAN,  ALLIANCE, ASHRAN_MAP_ID, 4245.96f, -4140.50f, 32.8186f, 1.27162f },
    { ASHRAN_STORMSHIELD_FOOTMAN,  ALLIANCE, ASHRAN_MAP_ID, 4244.52f, -4138.88f, 32.6640f, 0.43295f },
    { ASHRAN_STORMSHIELD_FOOTMAN,  ALLIANCE, ASHRAN_MAP_ID, 4238.14f, -4180.38f, 30.9901f, 4.00969f }
};

// See order below
const go_type g_CapturePoint[BATTLE_TYPE_MAX] =
{
    { CAPTURE_POINT_BANNER, ASHRAN_MAP_ID, 4801.65f, -4211.40f, 32.9733f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f },
    { CAPTURE_POINT_BANNER, ASHRAN_MAP_ID, 4677.77f, -4222.42f, 10.1084f, 0.01f, 0.00f, 0.00f, 0.00f, 0.00f },
    { CAPTURE_POINT_BANNER, ASHRAN_MAP_ID, 4533.45f, -4211.45f, 7.11222f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f },
    { CAPTURE_POINT_BANNER, ASHRAN_MAP_ID, 4353.77f, -4190.81f, 10.0985f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f },
    { CAPTURE_POINT_BANNER, ASHRAN_MAP_ID, 4226.71f, -4171.36f, 31.2031f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f }
};

uint32 const g_MiddleBattlesEntries[BATTLE_TYPE_MAX] =
{
    WORLD_STATE_EMBERFALL_TOWER_BATTLE,
    WORLD_STATE_VOLRATHS_ADVANCE_BATTLE,
    WORLD_STATE_THE_CROSSROADS_BATTLE,
    WORLD_STATE_TREMBLADES_VANGUARD_BATTLE,
    WORLD_STATE_ARCHMAGE_OVERWATCH_BATTLE
};

class OutdoorPvPAshran;

class OPvPCapturePoint_Middle : public OPvPCapturePoint
{
    public:

        OPvPCapturePoint_Middle(OutdoorPvP* p_Outdoor, eAshranBattleType p_Type, uint8 p_Faction);

        void ChangeState();

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void UpdateTowerState();

        bool Update(uint32 p_Diff);

        void SpawnFactionGuards(eAshranBattleType p_BattleID, uint8 p_Faction);

        void SetBattleFaction(uint32 p_Faction) { m_BattleFaction = p_Faction; }
        uint32 GetBattleFaction() const { return m_BattleFaction; }
        eAshranBattleType GetBattleType() const { return m_BattleType; }

    protected:

        eAshranBattleType m_BattleType;
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

        void InitializeControlPoints();
        bool IsInitialized() const { return m_IsInitialized; }

        void SetBattleState(uint32 p_NewState);
        void SetNextBattleTimer(uint32 p_Time) { m_NextBattleTimer = p_Time * IN_MILLISECONDS; }

        void AddGenericMoPGuid(uint8 p_Type, uint64 p_Guid) { m_GenericMoPGuids[p_Type] = p_Guid; }
        uint64 GetGenericMoPGuid(uint8 p_Type) const { return m_GenericMoPGuids[p_Type]; }
        uint64 GetFactionGenericMoP(uint8 p_Faction) const { return m_FactionGenericMoP[p_Faction]; }

        OPvPCapturePoint_Middle* GetCapturePoint(uint8 p_Index) const { return m_ControlPoints[p_Index]; }

    private:

        OPvPCapturePoint_Graveyard* m_GraveYard;
        OPvPCapturePoint_Middle* m_ControlPoints[BATTLE_TYPE_MAX];
        uint64 m_GenericMoPGuids[BATTLE_TYPE_MAX];
        uint64 m_FactionGenericMoP[BG_TEAMS_COUNT];
        uint32 m_InitPointsTimer;
        bool m_IsInitialized;

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
