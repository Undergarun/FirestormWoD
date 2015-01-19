////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_MGR_CONSTANTS_HPP_GARRISON
#define GARRISON_MGR_CONSTANTS_HPP_GARRISON

#include "GarrisonMgrStructures.hpp"

#define GARRISON_MAX_LEVEL                      3
#define GARRISON_MAX_FOLLOWER_PER_MISSION       3
#define GARRISON_BASE_MAP                       1116
#define GARRISON_PLOT_INSTANCE_COUNT            40
#define GARRISON_CURRENCY_ID                    824
#define GARRISON_MAX_FOLLOWER_LEVEL             100
#define GARRISON_DEFAULT_MAX_ACTIVE_FOLLOW      20
#define GARRISON_FOLLOWER_ACTIVATION_COST       (250 * GOLD)
#define GARRISON_FOLLOWER_ACTIVATION_MAX_STACK  1
#define GARRISON_CACHE_MAX_CURRENCY             500
#define GARRISON_CACHE_HEFTY_CURRENCY           200
#define GARRISON_CACHE_MIN_CURRENCY             5
#define GARRISON_CACHE_GENERATE_TICK            (10 * MINUTE)
#define GARRISON_MISSION_DISTRIB_INTERVAL       (25 * MINUTE)
#define GARRISON_MISSION_DISTRIB_FOLLOWER_COEFF 1.5

namespace MS { namespace Garrison 
{
    enum GarrisonFactionIndex
    {
        GARRISON_FACTION_HORDE      = 0,
        GARRISON_FACTION_ALLIANCE   = 1,
        GARRISON_FACTION_COUNT      = 2
    };

    enum GarrisonMissionState
    {
        GARRISON_MISSION_AVAILABLE              = 0,
        GARRISON_MISSION_IN_PROGRESS            = 1,
        GARRISON_MISSION_COMPLETE_SUCCESS       = 2,
        GARRISON_MISSION_COMPLETE_FAILED        = 5
    };

    enum GarrisonMissionFlag
    {
        GARRISON_MISSION_FLAG_RARE          = 0x01,
        GARRISON_MISSION_FLAG_UNK_2         = 0x02,
        GARRISON_MISSION_FLAG_EXHAUSTING    = 0x04
    };

    enum GarrisonLearnBluePrintResult
    {
        GARRISON_LEARN_BLUEPRINT_LEARNED            = 0,
        GARRISON_LEARN_BLUEPRINT_UNABLE_TO_LEARN    = 1,
        GARRISON_LEARN_BLUEPRINT_KNOWN              = 21
    };

    enum GarrisonPlotType
    {
        GARRISON_PLOT_TYPE_SMALL            = 0,
        GARRISON_PLOT_TYPE_MEDIUM           = 1,
        GARRISON_PLOT_TYPE_LARGE            = 2,
        GARRISON_PLOT_TYPE_FARM             = 3,
        GARRISON_PLOT_TYPE_MINE             = 4,
        GARRISON_PLOT_TYPE_FISHING_HUT      = 5,
        GARRISON_PLOT_TYPE_PET_MENAGERIE    = 6,
        GARRISON_PLOT_TYPE_MAX              = 7
    };

    enum GarrisonPurchaseBuildingResult
    {
        GARRISON_PURCHASE_BUILDING_OK                       = 0,
        GARRISON_PURCHASE_BUILDING_BUILDING_EXIST           = 24,
        GARRISON_PURCHASE_BUILDING_INVALID_PLOT             = 10,
        GARRISON_PURCHASE_BUILDING_INVALID_BUILDING_ID      = 11,
        GARRISON_PURCHASE_BUILDING_INVALID_PLOT_BUILDING    = 16,
        GARRISON_PURCHASE_BUILDING_REQUIRE_BLUE_PRINT       = 22,
        GARRISON_PURCHASE_BUILDING_NOT_ENOUGH_CURRENCY      = 46,
        GARRISON_PURCHASE_BUILDING_NOT_ENOUGH_GOLD          = 47
    };

    enum GarrisonMissionBonusRollResult
    {
        GARRISON_MISSION_BONUS_ROLL_OK      = 0,
        GARRISON_MISSION_BONUS_ROLL_ERROR   = 1
    };

    enum GarrisonAbilityEffectType
    {
        GARRISION_ABILITY_EFFECT_UNK_0                                  = 0,    ///< @TODO
        GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_SOLO                      = 1,    ///< Proc if MissionFollowerCount == 1
        GARRISION_ABILITY_EFFECT_MOD_WIN_RATE                           = 2,    ///< Proc every time
        GARRISION_ABILITY_EFFECT_MOD_TRAVEL_TIME                        = 3,    ///< Proc every time
        GARRISION_ABILITY_EFFECT_MOD_XP_GAIN                            = 4,    ///< Mod the XP earn (self, party)
        GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_CLASS                     = 5,    ///< Proc if Find(MissionFollowers[Class], MiscValueA) != NULL 
        GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_DURATION_MORE             = 6,    ///< Proc if Duration > (3600 * MiscValueB)
        GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_DURATION_LESS             = 7,    ///< Proc if Duration < (3600 * MiscValueB)
        GARRISION_ABILITY_EFFECT_MOD_GARR_CURRENCY_DROP                 = 8,    ///< @TODO
        GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_TRAVEL_DURATION_MORE      = 9,    ///< Proc if TravelDuration > (3600 * MiscValueB)
        GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_TRAVEL_DURATION_LESS      = 10,   ///< Proc if TravelDuration < (3600 * MiscValueB)
        GARRISION_ABILITY_EFFECT_UNK_11                                 = 11,   ///< UNUSED
        GARRISION_ABILITY_EFFECT_MOD_DUMMY_PRODUCTION                   = 12,   ///< @TODO
        GARRISION_ABILITY_EFFECT_MOD_BRONZE_TREASURE_DROP               = 13,   ///< @TODO
        GARRISION_ABILITY_EFFECT_MOD_SILVER_TREASURE_DROP               = 14,   ///< @TODO
        GARRISION_ABILITY_EFFECT_MOD_GOLD_TREASURE_DROP                 = 15,   ///< @TODO
        GARRISION_ABILITY_EFFECT_MOD_CHEST_DROP_RATE                    = 16,   ///< @TODO
        GARRISION_ABILITY_EFFECT_MOD_MISSION_DURATION                   = 17    ///< Proc every time
    };

    enum GarrisonAbilityEffectTargetMask
    {
        GARRISON_ABILITY_EFFECT_TARGET_MASK_UNK   = 0 << 0,
        GARRISON_ABILITY_EFFECT_TARGET_MASK_SELF  = 1 << 0,
        GARRISON_ABILITY_EFFECT_TARGET_MASK_PARTY = 1 << 1
    };

    enum GarrMechanicType
    {
        GARRISON_MECHANIC_TYPE_ENVIRONMENT  = 0,
        GARRISON_MECHANIC_TYPE_RACIAL       = 1,
        GARRISON_MECHANIC_TYPE_ABILITY      = 2
    };

    /// WorldState : See WorldState.dbc
    enum GarrisonWorldState
    {
        GARRISON_WORLD_STATE_CACHE_NUM_TOKEN = 9573
    };

    enum GarrisonAbilitySpells
    {
        GARRISON_SPELL_GARR_ABILITY                 = 161691,
        GARRISON_SPELL_GARR_ABILITY_HORDE_BASE      = 161693,
        GARRISON_SPELL_GARR_ABILITY_ALLIANCE_BASE   = 161694,
    };

    /// TerrainSwap : See WorldMapArea.dbc
    enum
    {
        TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_1     = 980,
        TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_2     = 990,
        TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_3     = 981,
        TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_1 = 973,
        TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_2 = 991,
        TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_3 = 974
    };

    enum 
    {
        GARRISON_CREATURE_AI_DATA_BUILDER       = 10000,
        GARRISON_CREATURE_AI_DATA_PEON_WORKING  = 10001
    };

    enum GarrisonFollowerFlags
    {
        GARRISON_FOLLOWER_FLAG_EXHAUSTED    = 0x2,
        GARRISON_FOLLOWER_FLAG_INACTIVE     = 0x4
    };

    static const uint32 gGarrisonInGarrisonAreaID[GARRISON_FACTION_COUNT] =
    {
        7004,   ///< Horde
        7078,   ///< Alliance
    };

    static const uint32 gGarrisonEmptyPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
    {
        /// Horde
        233083,     ///< GARRISON_PLOT_TYPE_SMALL
        232425,     ///< GARRISON_PLOT_TYPE_MEDIUM
        233081,     ///< GARRISON_PLOT_TYPE_LARGE
        232415,     ///< GARRISON_PLOT_TYPE_FARM
        232447,     ///< GARRISON_PLOT_TYPE_MINE
        232426,     ///< GARRISON_PLOT_TYPE_FISHING_HUT
        231706,     ///< GARRISON_PLOT_TYPE_PET_MENAGERIE
        /// Alliance
        229501,     ///< GARRISON_PLOT_TYPE_SMALL
        232283,     ///< GARRISON_PLOT_TYPE_MEDIUM
        232143,     ///< GARRISON_PLOT_TYPE_LARGE
        232286,     ///< GARRISON_PLOT_TYPE_FARM
        233485,     ///< GARRISON_PLOT_TYPE_MINE
        237223,     ///< GARRISON_PLOT_TYPE_FISHING_HUT
        0           ///< GARRISON_PLOT_TYPE_PET_MENAGERIE
    };

    static const uint32 gGarrisonBuildingPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
    {
        /// Horde
        233958,     ///< GARRISON_PLOT_TYPE_SMALL
        232373,     ///< GARRISON_PLOT_TYPE_MEDIUM
        232410,     ///< GARRISON_PLOT_TYPE_LARGE
        232373,     ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
        232373,     ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
        233958,     ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
        233958,     ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
        /// Alliance
        233957,     ///< GARRISON_PLOT_TYPE_SMALL
        232409,     ///< GARRISON_PLOT_TYPE_MEDIUM
        232411,     ///< GARRISON_PLOT_TYPE_LARGE
        232409,     ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
        232409,     ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
        233957,     ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
        233957      ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
    };

    static const float gGarrisonBuildingPlotAABBDiminishReturnFactor[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
    {
        /// Horde
        0,          ///< GARRISON_PLOT_TYPE_SMALL
        0,          ///< GARRISON_PLOT_TYPE_MEDIUM
        24,         ///< GARRISON_PLOT_TYPE_LARGE
        0,          ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
        0,          ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
        0,          ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
        0,          ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
        /// Alliance
        10,         ///< GARRISON_PLOT_TYPE_SMALL
        16,         ///< GARRISON_PLOT_TYPE_MEDIUM
        24,         ///< GARRISON_PLOT_TYPE_LARGE
        16,         ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
        16,         ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
        10,         ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
        10          ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
    };

    static const uint32 gGarrisonBuildingActivationGameObject[GARRISON_FACTION_COUNT] =
    {
        233248,     ///< Horde
        233250      ///< Alliance
    };

    static const GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[GARRISON_PLOT_INSTANCE_COUNT] =
    {
        /// SiteLevelID PlotInstanceID      X            Y            Z           O
        /// Alliance Level 1
        {       5,          19,         1829.896f,    197.5504f,    72.00920f,   1.8849560f  },
        {       5,          23,         1911.550f,    232.9792f,    76.65489f,   2.7952700f  },
        /// Alliance Level 2
        {     444,          18,         1819.583f,    231.2813f,    72.17403f,  -1.2915440f  },
        {     444,          19,         1829.896f,    197.5504f,    71.98585f,   1.8849560f  },
        {     444,          22,         1864.955f,    320.2083f,    81.66045f,  -1.4835300f  },
        {     444,          23,         1918.637f,    228.7674f,    76.63956f,   2.7750740f  },
        {     444,          59,         1845.083f,    146.2743f,    53.43811f,   0.3490658f  },
        {     444,          63,         1847.615f,    134.7257f,    78.10705f,   2.7052600f  },
        {     444,          67,         2031.594f,    174.4410f,    84.59409f,   2.8361600f  },
        /// Alliance Level 3
        {       6,          18,         1819.583f,    231.2813f,    72.17403f,  -1.2915440f  },
        {       6,          19,         1830.528f,    196.7465f,    71.98586f,   1.9024090f  },
        {       6,          20,         1804.333f,    189.1458f,    70.07482f,   1.9634950f  },
        {       6,          22,         1864.955f,    320.2083f,    81.66045f,  -1.4835300f  },
        {       6,          23,         1918.637f,    228.7674f,    76.63956f,   2.7750740f  },
        {       6,          24,         1814.323f,    286.3941f,    76.62489f,  -0.6544983f  },
        {       6,          25,         1893.337f,    185.1684f,    78.05621f,   2.5307280f  },
        {       6,          59,         1845.083f,    146.2743f,    53.41686f,   0.3490658f  },
        {       6,          63,         1847.615f,    134.7257f,    78.10705f,   2.7052600f  },
        {       6,          67,         2031.594f,    174.4410f,    84.36597f,   2.8361600f  },
        {       6,          81,         1914.083f,    354.1875f,    88.96585f,  -1.8763060f  },
        /// Horde Level 1
        {     258,          18,         5645.124f,   4508.9600f,   119.27010f,   2.0423500f  },
        {     258,          23,         5575.461f,   4459.3380f,   130.36810f,   0.9599311f  },
        /// Horde level 2
        {     445,          18,         5645.124f,   4508.9600f,   119.27010f,   2.0420350f  },
        {     445,          19,         5665.686f,   4549.2660f,   119.27020f,  -2.7488930f  },
        {     445,          22,         5525.882f,   4523.5690f,   131.71830f,  -0.0436332f  },
        {     445,          23,         5574.562f,   4460.0310f,   130.36800f,   0.9424779f  },
        {     445,          59,         5399.831f,   4465.9760f,   114.46130f,   0.8464847f  },
        {     445,          63,         5415.374f,   4586.4390f,   136.62080f,   2.3561950f  },
        {     445,          67,         5476.589f,   4622.7070f,   134.44980f,  -1.3962630f  },
        /// Horde level 3
        {     259,          18,         5643.449f,   4503.7780f,   119.27010f,   1.6318830f  },
        {     259,          19,         5665.686f,   4549.2660f,   119.26810f,  -2.7488930f  },
        {     259,          20,         5617.798f,   4510.2760f,   119.27020f,   0.8028514f  },
        {     259,          22,         5527.659f,   4523.2130f,   131.72760f,  -0.0436332f  },
        {     259,          23,         5575.183f,   4459.6670f,   130.36800f,   0.9424779f  },
        {     259,          24,         5651.279f,   4441.2680f,   130.52540f,   1.8936820f  },
        {     259,          25,         5693.363f,   4471.8970f,   130.52540f,   2.3300140f  },
        {     259,          59,         5399.831f,   4465.9760f,   114.53970f,   0.8464847f  },
        {     259,          63,         5415.374f,   4586.4390f,   136.58310f,   2.3561950f  },
        {     259,          67,         5476.589f,   4622.7070f,   134.44980f,  -1.3962630f  },
        {     259,          81,         5626.042f,   4660.6960f,   142.45800f,  -1.9024090f  }
    };

    /// Cache game object position for each faction / level
    static const GarrisonCacheInfoLocation gGarrisonCacheInfoLocation[GARRISON_FACTION_COUNT * GARRISON_MAX_LEVEL] =
    {
        /// SiteLevelID         X           Y          Z         O
        /// Horde Level 1
        {     258,          5565.679f,  4499.0090f, 132.02610f, 0.081281f  },
        /// Horde level 2
        {     445,             0.f,        0.f,       0.f,      0.f        },
        /// Horde level 3
        {     259,          5592.272f,  4589.9390f, 136.66830f, 5.858787f  },
        /// Alliance Level 1
        {       5,          1893.729f,   208.8733f,  77.06371f, 1.685312f  },
        /// Alliance Level 2
        {     444,             0.f,        0.f,       0.f,      0.f        },
        /// Alliance Level 3
        {       6,             0.f,        0.f,       0.f,      0.f        }
    };

    /// Cache game object entry for each faction / state
    static const uint32 gGarrisonCacheGameObjectID[GARRISON_FACTION_COUNT * 3] =
    {
        /// Horde
        237191,         ///< Normal
        237720,         ///< Hefty
        237722,         ///< Full
        /// Alliance
        236916,         ///< Normal
        237723,         ///< Hefty
        237724          ///< Full
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MGR_CONSTANTS_HPP_GARRISON