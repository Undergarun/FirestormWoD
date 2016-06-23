////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_CONSTANTS_HPP_GARRISON
#define GARRISON_CONSTANTS_HPP_GARRISON

#include "Common.h"
#include "SharedDefines.h"
#include "GarrisonMgrStructures.hpp"

/// @Hatters gonna hate, float doesn't go into the Globals enum, Change standard ?
#define GARRISON_MISSION_DISTRIB_FOLLOWER_COEFF (0.8f)

namespace MS { namespace Garrison
{
    namespace Globals
    {
        enum
        {
            MaxLevel                        = 3,
            MaxFollowerPerMission           = 3,
            MaxFollowerLevel                = 100,
            FollowerActivationCost          = (250 * GOLD),
            FollowerActivationMaxStack      = 1,
            BaseMap                         = 1116,
            PlotInstanceCount               = 42,
            CurrencyID                      = 824,
            CacheMinToken                   = 5,
            CacheHeftyToken                 = 200,
            CacheMaxToken                   = 500,
            CacheTokenGenerateTime          = (10 * MINUTE),
            MissionDistributionInterval     = (25 * MINUTE),
            DefaultFollowerItemLevel        = 600,
            ShipyardBuildingType            = 9,
            ShipyardBuildingID              = 205,
            ShipyardPlotID                  = 98,
            MaxActiveFollowerAllowedCount   = 20
        };
    }

    namespace ShipyardMapId
    {
        enum : uint32
        {
            Alliance    = 1473,
            Horde       = 1474
        };
    }

    namespace ShipyardTerainSwaps
    {
        enum Alliance : uint32
        {
            Part1 = 992,
            Part2 = 683,
            Part3 = 910
        };
    };

    namespace FactionIndex
    {
        enum Type : uint8
        {
            Horde       = 0,
            Alliance    = 1,
            Max         = 2
        };
    }

    namespace Building
    {
        enum ID : uint8
        {
            DwarvenBunker_WarMill_Level1                = 8,
            DwarvenBunker_WarMill_Level2                = 9,
            DwarvenBunker_WarMill_Level3                = 10,
            Barracks_Barracks_Level1                    = 26,
            Barracks_Barracks_Level2                    = 27,
            Barracks_Barracks_Level3                    = 28,
            Storehouse_Storehouse_Level1                = 51,
            Storehouse_Storehouse_Level2                = 142,
            Storehouse_Storehouse_Level3                = 143,
            TheForge_TheForge_Level1                    = 60,
            TheForge_TheForge_Level2                    = 117,
            TheForge_TheForge_Level3                    = 118,
            TradingPost_TradingPost_Level1              = 111,
            TradingPost_TradingPost_Level2              = 144,
            TradingPost_TradingPost_Level3              = 145,
            TailoringEmporium_TailoringEmporium_Level1  = 94,
            TailoringEmporium_TailoringEmporium_Level2  = 127,
            TailoringEmporium_TailoringEmporium_Level3  = 128,
            AlchemyLab_AlchemyLab_Level1                = 76,
            AlchemyLab_AlchemyLab_Level2                = 119,
            AlchemyLab_AlchemyLab_Level3                = 120,
            TheTannery_TheTannery_Level1                = 90,
            TheTannery_TheTannery_Level2                = 121,
            TheTannery_TheTannery_Level3                = 122,
            EnchanterStudy_EnchanterStudy_Level1        = 93,
            EnchanterStudy_EnchanterStudy_Level2        = 125,
            EnchanterStudy_EnchanterStudy_Level3        = 126,
            GemBoutique_GemBoutique_Level1              = 96,
            GemBoutique_GemBoutique_Level2              = 131,
            GemBoutique_GemBoutique_Level3              = 132,
            EngineeringWorks_EngineeringWorks_Level1    = 91,
            EngineeringWorks_EngineeringWorks_Level2    = 123,
            EngineeringWorks_EngineeringWorks_Level3    = 124,
            ScribesQuarters_ScribesQuarters_Level1      = 95,
            ScribesQuarters_ScribesQuarters_Level2      = 129,
            ScribesQuarters_ScribesQuarters_Level3      = 130,
            HerbGarden_HerbGarden_Level1                = 29,
            HerbGarden_HerbGarden_Level2                = 136,
            HerbGarden_HerbGarden_Level3                = 137,
            Barn_Barn_Level1                            = 24,
            Barn_Barn_Level2                            = 25,
            Barn_Barn_Level3                            = 133,
            SalvageYard_SalvageYard_Level1              = 52,
            SalvageYard_SalvageYard_Level2              = 140,
            SalvageYard_SalvageYard_Level3              = 141,
            LumberMill_LumberMill_Level1                = 40,
            LumberMill_LumberMill_Level2                = 41,
            LumberMill_LumberMill_Level3                = 138,
            LunarfallExcavation_FrostwallMines_Level1   = 61,
            LunarfallExcavation_FrostwallMines_Level2   = 62,
            LunarfallExcavation_FrostwallMines_Level3   = 63,
            LunarfallInn_FrostwallTavern_Level1         = 34,
            LunarfallInn_FrostwallTavern_Level2         = 35,
            LunarfallInn_FrostwallTavern_Level3         = 36,
            MageTower_SpiritLodge_Level1                = 37,
            MageTower_SpiritLodge_Level2                = 38,
            MageTower_SpiritLodge_Level3                = 39,
            Stables_Stables_Level1                      = 65,
            Stables_Stables_Level2                      = 66,
            Stables_Stables_Level3                      = 67,
            GladiatorsSanctum_GladiatorsSanctum_Level1  = 159,
            GladiatorsSanctum_GladiatorsSanctum_Level2  = 160,
            GladiatorsSanctum_GladiatorsSanctum_Level3  = 161,
            GnomishGearworks_GoblinWorkshop_Level1      = 162,
            GnomishGearworks_GoblinWorkshop_Level2      = 163,
            GnomishGearworks_GoblinWorkshop_Level3      = 164,
            FishingShack_FishingShack_Level1            = 64,
            FishingShack_FishingShack_Level2            = 134,
            FishingShack_FishingShack_Level3            = 135
        };

        enum Type : uint8
        {
            Unk                 = 0,    ///< Unused 1
            Mine                = 1,
            Farm                = 2,
            Barn                = 3,
            LumberMill          = 4,
            Inn                 = 5,
            TradingPost         = 6,
            PetMenagerie        = 7,
            Barracks            = 8,
            Orchard             = 9,    ///< Unused 2
            Armory              = 10,   ///< War mill/Dwarven Bunker
            Stable              = 11,
            Academy             = 12,
            MageTower           = 13,
            SalvageYard         = 14,
            StoreHouse          = 15,
            Alchemy             = 16,
            Blacksmith          = 17,
            Enchanting          = 18,
            Engineering         = 19,
            Inscription         = 20,
            Jewelcrafting       = 21,
            Leatherworking      = 22,
            Tailoring           = 23,
            Fishing             = 24,
            SparringArena       = 25,
            Workshop            = 26
        };
    }

    namespace Mission
    {
        enum State : uint8
        {
            Available       = 0,
            InProgress      = 1,
            CompleteSuccess = 2,
            CompleteFailed  = 5
        };

        enum Type : uint8
        {
            Combat         = 3,
            Generic        = 4,
            Salvage        = 5,
            Logistics      = 6,
            Wildlife       = 7,
            Trading        = 8,
            Construction   = 9,
            Provision      = 10,
            Recruitement   = 11,
            Training       = 12,
            Patrol         = 13,
            Research       = 14,
            Defense        = 15,
            Exploration    = 16,
            Siege          = 17,
            Alchemy        = 18,
            BlackSmithing  = 19,
            Enchanting     = 20,
            Engineering    = 21,
            Inscription    = 22,
            JewelCrafting  = 23,
            LeatherWorking = 24,
            Tailoring      = 25,
            Treasure       = 35,
            PetBattle      = 36,
            ShipCombat     = 37,
            ShipOil        = 38,
            ShipTraining   = 39,
            ShipTreasure   = 40,
            ShipSiegeA     = 41,
            ShipSiegeH     = 42,
            ShipBonus      = 47,
            ShipLegendary  = 48,
            ShipSiegeIHA   = 49,
            ShipSiegeIHH   = 50
        };

        enum Flags : uint8
        {
            Rare        = 0x01,
            Unk2        = 0x02,
            Exhausting  = 0x04
        };

        enum BonusRollResults : uint8
        {
            Ok    = 0,
            Error = 1
        };

        static const std::map<Mission::Type, Building::Type> g_MissionBuildingTypesMap
        {
            { Mission::Type::Alchemy,        Building::Type::Alchemy        },
            { Mission::Type::LeatherWorking, Building::Type::Leatherworking },
            { Mission::Type::BlackSmithing,  Building::Type::Blacksmith     },
            { Mission::Type::Enchanting,     Building::Type::Enchanting     },
            { Mission::Type::JewelCrafting,  Building::Type::Jewelcrafting  },
            { Mission::Type::Engineering,    Building::Type::Engineering    },
            { Mission::Type::Tailoring,      Building::Type::Tailoring      },
            { Mission::Type::Inscription,    Building::Type::Inscription    }
        };
    }

    namespace FollowerType
    {
        enum : uint32
        {
            NPC  = 1,
            Ship = 2
        };
    }

    namespace LearnBluePrintResults
    {
        enum Type : uint8
        {
            Learned             = 0,
            UnableToLearn       = 1,
            Known               = 21
        };
    }

    namespace PlotTypes
    {
        enum
        {
            Small           = 0,
            Medium          = 1,
            Large           = 2,
            Farm            = 3,
            Mine            = 4,
            FishingHut      = 5,
            PetMenagerie    = 6,
            Shipyard        = 7,
            Max             = 8
        };
    }

    namespace PurchaseBuildingResults
    {
        enum Type
        {
            Ok                  = 0,
            BuildingExists      = 24,
            InvalidBuildingID   = 11,
            InvalidPlot         = 10,
            InvalidPlotBuilding = 16,
            RequireBluePrint    = 22,
            NotEnoughCurrency   = 46,
            NotEnoughGold       = 47
        };
    }

    namespace AbilityEffectTypes
    {
        enum Type
        {
            ModUnk0                        = 0,    ///< @TODO
            ModWinRateSolo                 = 1,    ///< Proc if MissionFollowerCount == 1
            ModWinRate                     = 2,    ///< Proc every time
            ModTravelTime                  = 3,    ///< Proc every time
            ModXpGain                      = 4,    ///< Mod the XP earn (self, party)
            ModWinRateClass                = 5,    ///< Proc if Find(MissionFollowers[Class], MiscValueA) != NULL
            ModWinRateDurationMore         = 6,    ///< Proc if Duration > (3600 * MiscValueB)
            ModWinRateDurationLess         = 7,    ///< Proc if Duration < (3600 * MiscValueB)
            ModGarrCurrencyDrop            = 8,    ///< Implemented.
            ModWinRateTravelDurationMore   = 9,    ///< Proc if TravelDuration > (3600 * MiscValueB)
            ModWinRateTravelDurationLess   = 10,   ///< Proc if TravelDuration < (3600 * MiscValueB)
            ModUnk11                       = 11,   ///< UNUSED
            ModDummyProduction             = 12,   ///< @TODO
            ModBronzeTreasureDrop          = 13,   ///< @TODO
            ModSilverTreasureDrop          = 14,   ///< @TODO
            ModGoldTreasureDrop            = 15,   ///< @TODO
            ModChestDropRate               = 16,   ///< @TODO
            ModMissionDuration             = 17    ///< Proc every time
        };
    }

    namespace AbilityEffectTargetMasks
    {
        enum Type
        {
            Unk     = 0 << 0,
            Self    = 1 << 0,
            Party   = 1 << 1
        };
    }

    namespace MechanicTypes
    {
        enum Type
        {
            Environment = 0,
            Racial      = 1,
            Ability     = 2
        };
    }

    /// WorldState : See WorldState.dbc
    namespace WorldStates
    {
        enum Type
        {
            CacheNumToken = 9573
        };
    }

    namespace BaseQuests
    {
        enum
        {
            FoundedGarrisonH = 36793,
            FoundedGarrisonA = 36797
        };
    }

    enum GarrisonAbilitySpells
    {
        GARRISON_SPELL_GARR_ABILITY                 = 161691,
        GARRISON_SPELL_GARR_ABILITY_HORDE_BASE      = 161693,
        GARRISON_SPELL_GARR_ABILITY_ALLIANCE_BASE   = 161694
    };

    enum GarrisonAbilitiesEnum
    {
        AbilityHerbalism = 53
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

    namespace CreatureAIDataIDs
    {
        enum
        {
            Builder         = 10000,
            PeonWorking     = 10001,
            BuildingID      = 10002,
            PlotInstanceID  = 10003,
            GatheredPos     = 10004,
            DailyReset      = 10005,
            OwnerGuid       = 10006,
            DespawnData     = 10007,
            HasRecipe       = 0x40000000
        };
    }

    namespace TaxiNodes
    {
        enum
        {
            Alliance = 1476,
            Horde    = 1432
        };
    }

    enum GarrisonFollowerFlags
    {
        GARRISON_FOLLOWER_FLAG_EXHAUSTED    = 0x2,
        GARRISON_FOLLOWER_FLAG_INACTIVE     = 0x4
    };

    /// 6.1.2 19865 - SceneScript.db2 "Garrison - Building Activation Init"

    namespace BuildingCategory
    {
        enum Type
        {
            Prebuilt    = 0,
            UserBuilt   = 1
        };
    }

    namespace WorkOrderGODisplayID
    {
        enum
        {
            BaseA       = 15585,
            BaseH       = 20508,
            BaseBarn    = 13845,
            AllComplete = 16091
        };
    }


    namespace FollowerTypeFlags
    {
        enum : uint32
        {
            CanRenameFollower           = 1,    ///< The specified type can be renamed, eg ships - LUA RenameFollower
            CanStartMissionPastSoftCap  = 2,    ///< You can start a mission above follower soft cap - LUA AllowMissionStartAboveSoftCap
            Unk1                        = 4,    ///< Unk, only regular follower has it
            Unk2                        = 8     ///< Unk, only ships have it
        };
    }

    static const uint32 gGarrisonBuildingMaxWorkOrderPerBuildingLevel[Globals::MaxLevel] =
    {
        7,
        14,
        21
    };

    static const uint32 gGarrisonInGarrisonAreaID[FactionIndex::Max] =
    {
        7004,   ///< Horde
        7078    ///< Alliance
    };

    static const uint32 gGarrisonShipyardAreaID[FactionIndex::Max] =
    {
        7765,   ///< Horde
        7760    ///< Alliance
    };

    static const uint32 gGarrisonEmptyPlotGameObject[PlotTypes::Max * FactionIndex::Max] =
    {
        /// Horde
        233083,     ///< PlotTypes::Small
        232425,     ///< PlotTypes::Medium
        233081,     ///< PlotTypes::Large
        232425,     ///< PlotTypes::Farm
        232425,     ///< PlotTypes::Mine
        233083,     ///< PlotTypes::FishingHut
        233083,     ///< PlotTypes::PetMenagerie
        0,          ///< PlotTypes::Shipyard
        /// Alliance
        229501,     ///< PlotTypes::Small
        232283,     ///< PlotTypes::Medium
        232143,     ///< PlotTypes::Large
        232283,     ///< PlotTypes::Farm
        232283,     ///< PlotTypes::Mine
        229501,     ///< PlotTypes::FishingHut
        229501,     ///< PlotTypes::PetMenagerie
        0           ///< PlotTypes::Shipyard
    };

    static const uint32 gGarrisonBuildingPlotGameObject[PlotTypes::Max * FactionIndex::Max] =
    {
        /// Horde
        233958,     ///< PlotTypes::Small
        232373,     ///< PlotTypes::Medium
        232410,     ///< PlotTypes::Large
        232373,     ///< PlotTypes::Farm          same as PlotTypes::Medium
        0,          ///< PlotTypes::Mine          same as PlotTypes::Medium
        0,          ///< PlotTypes::FishingHut    same as PlotTypes::Small
        0,          ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
        0,          ///< PlotTypes::Shipyard
        /// Alliance
        233957,     ///< PlotTypes::Small
        232409,     ///< PlotTypes::Medium
        232411,     ///< PlotTypes::Large
        0,          ///< PlotTypes::Farm          same as PlotTypes::Medium
        0,          ///< PlotTypes::Mine          same as PlotTypes::Medium
        0,          ///< PlotTypes::FishingHut    same as PlotTypes::Small
        0,          ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
        0           ///< PlotTypes::Shipyard
    };

    static const float gGarrisonBuildingPlotAABBDiminishReturnFactor[PlotTypes::Max * FactionIndex::Max] =
    {
        /// Horde
        10,         ///< PlotTypes::Small
        16,         ///< PlotTypes::Medium
        24,         ///< PlotTypes::Large
        0,          ///< PlotTypes::Farm          same as PlotTypes::Medium
        0,          ///< PlotTypes::Mine          same as PlotTypes::Medium
        0,          ///< PlotTypes::FishingHut    same as PlotTypes::Small
        0,          ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
        0,          ///< PlotTypes::Shipyard
        /// Alliance
        10,         ///< PlotTypes::Small
        16,         ///< PlotTypes::Medium
        24,         ///< PlotTypes::Large
        0,          ///< PlotTypes::Farm          same as PlotTypes::Medium
        0,          ///< PlotTypes::Mine          same as PlotTypes::Medium
        0,          ///< PlotTypes::FishingHut    same as PlotTypes::Small
        0,          ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
        0           ///< PlotTypes::Shipyard
    };

    static const uint32 gGarrisonBuildingActivationGameObject[FactionIndex::Max] =
    {
        233248,     ///< Horde
        233250      ///< Alliance
    };

    enum GarrisonBuildingWorkOrderGameObject
    {
        GobBarnWOrkOrder = 238761
    };

    static const GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[Globals::PlotInstanceCount] =
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
        {       6,          98,         0.0f,          0.0f,          0.0f,         0.0f,    },
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
        {     259,          81,         5626.042f,   4660.6960f,   142.45800f,  -1.9024090f  },
        {     259,          98,         0.0f,         0.0f,          0.0f,         0.0f,     }
    };

    /// Cache game object position for each faction / level
    static const GarrisonCacheInfoLocation gGarrisonCacheInfoLocation[FactionIndex::Max * Globals::MaxLevel] =
    {
        /// SiteLevelID         X           Y          Z         O
        /// Horde Level 1
        {     258,          5565.679f,  4499.0090f, 132.02610f, 0.081281f  },
        /// Horde level 2
        {     445,          5589.409f,  4596.8510f, 136.58750f, 5.953404f  },
        /// Horde level 3
        {     259,          5592.272f,  4589.9390f, 136.66830f, 5.858787f  },
        /// Alliance Level 1
        {       5,          1893.729f,   208.8733f,  77.06371f, 1.685312f  },
        /// Alliance Level 2
        {     444,          1949.946f,   287.2795f,  88.96585f, 3.255662f  },
        /// Alliance Level 3
        {       6,             0.0f,        0.0f,       0.0f,      0.0f    }
    };

    /// Cache game object entry for each faction / state
    static const uint32 gGarrisonCacheGameObjectID[FactionIndex::Max * 3] =
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

    namespace TavernDatas
    {
        static const std::vector<uint32> g_QuestGiverEntries
        {
            87991,
            87992,
            87994,
            87995,
            87996,
            87997,
            87998,
            87999,
            88000,
            88001,
            88002,
            88003,
            88004,
            88005,
            88006,
            88007,
            88024,
            88025,
            88026,
            88027,
            88013,
            88023,
            88017,
            88022
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_CONSTANTS_HPP_GARRISON
