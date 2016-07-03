////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_SCRIPT_DATA_HPP_GARRISON
#define GARRISON_SCRIPT_DATA_HPP_GARRISON

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"

namespace MS { namespace Garrison
{
    /// Sequence position structure
    struct SequencePosition
    {
        /// Position
        float X, Y, Z, O;
    };

    /// Garrison map ids
    enum MapIDs
    {
        MapGarrisonHordeLevel1      = 1152,
        MapGarrisonHordeLevel2      = 1330,
        MapGarrisonHordeLevel3      = 1153,
        MapGarrisonHordeShipyard    = 1474,
        MapGarrisonAllianceLevel1   = 1158,
        MapGarrisonAllianceLevel2   = 1331,
        MapGarrisonAllianceLevel3   = 1159,
        MapGarrisonAllianceShipyard = 1473
    };

    namespace GarrisonGossipMenus
    {
        enum MenuID
        {
            DefaultMenuGreetings = 1
        };

        enum GossipOption
        {
            DefaultWorkOrder              = 0,
            BarnOrderSomptuousFur         = 1,
            BarnOrderRawBeastHide         = 2,
            BarnOrderSavageFeast          = 3,
            BarnOrderFurSavageBlood       = 4,
            BarnOrderBeastHideSavageBlood = 5,
            BarnOrderSavageFeastBlood     = 6,
            DefaultTrader                 = 7,
            DefaultBanker                 = 8,
            HerbFrostweed                 = 9,
            HerbStarflower                = 10,
            HerbFireweed                  = 11,
            HerbTaladorOrchid             = 12,
            HerbGorgrondFlytrap           = 13,
            HerbNagrandArrowbloom         = 14,
            ArmoryWeeklySeal              = 15,
            MineDefaultOrder              = 16,
            DefaultSomething              = 17,
            GarrisonCreation              = 18,
            ShellyQuestGossip1            = 19,
            ShellyQuestGossip2            = 20,
            GazloweQuestGossip1           = 21,
            FollowerRecruitment           = 22
        };

    }

    enum GarrisonPhases
    {
        PhaseMagePortalFrostfireRidge = 0x00000010,
        PhaseMagePortalSpiresOfArak   = 0x00000020,
        PhaseMagePortalTalador        = 0x00000040,
        PhaseMagePortalNagrand        = 0x00000080,
        PhaseMagePortalShadowmoon     = 0x00000100,
        PhaseMagePortalGorgrond       = 0x00000200,
        PhaseLostInTransitionQuest    = 0x00000400
    };

    /// Instance data common IDs
    enum InstanceDataIDs
    {
        GARRISON_INSTANCE_DATA_PEON_ENABLED = 99,
        GARRISON_INSTANCE_DATA_THREE_FIRST  = 100,
        GARRISON_INSTANCE_DATA_THREE_END    = 110
    };

    /// Garrison common spells
    enum Spells
    {
        SPELL_GARRISON_HEARTHSTONE             = 171253, ///< Misc
        SPELL_RELIEVED                         = 162410, ///< Misc
        SPELL_LEFT_ROPE                        = 164400, ///< Misc
        SPELL_RIGHT_ROPE                       = 164422, ///< Misc
        SPELL_COMESTIC_SLEEP                   = 162907, ///< Misc
        SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER = 161329, ///< Misc
        MultiShot                              = 18651,  ///< Misc
        Disengage                              = 82707,  ///< Misc
        Shoot                                  = 6660,   ///< Misc
        SpellArming                            = 167920, ///< Barn
        SpellIronTrap                          = 180609, ///< Barn
        SpellImprovedIronTrap                  = 180610, ///< Barn
        SpellDeadlyIronTrap                    = 180606, ///< Barn
        SpellApprenticeLogging                 = 167911, ///< Lumber Mill
        SpellJourneymanLogging                 = 167946, ///< Lumber Mill
        SpellSummonStump                       = 170079, ///< Lumber Mill
        SpellGarrisonPortal                    = 182464, ///< Mage Tower/Spirit Lodge
        SpellAuraRunesOfPower                  = 178777, ///< Mage Tower/Spirit Lodge
        SpellAuraAmperialConstructVisual       = 166094  ///< Mage Tower/Spirit Lodge
    };

    enum DisplayIDs
    {
        InvisibleDisplay   = 11686,
        GobIronTrapDisplay = 14212
    };

    /// Garrison Quests
    namespace Quests
    {
        enum
        {
            /// Alliance
            QUEST_ETABLISH_YOUR_GARRISON_A              = 34586,
            QUEST_KEEPING_IT_TOGETHER                   = 35176,
            QUEST_SHIP_SALVAGE                          = 35166,
            QUEST_PALE_MOONLIGHT                        = 35174,
            Alliance_BuildYourBarracks                  = 34587,
            Alliance_LostInTransition                   = 37087,
            Alliance_BiggerIsBetter                     = 36592,
            Alliance_AshranAppearance                   = 36624,
            Alliance_QianaMoonshadow                    = 34646,
            Alliance_TheHeadHunterHarverst              = 37119,

            /// Small profession building quests
            Alliance_YourFirstBlacksmithingWorkOrder    = 35168,
            Alliance_YourFirstAlchemyWorkOrder          = 36641,
            Alliance_YourFirstLeatherworkingWorkOrder   = 36642,
            Alliance_YourFirstEnchantingWorkOrder       = 36645,
            Alliance_YourFirstJewelcraftingWorkOrder    = 36644,
            Alliance_YourFirstEngineeringWorkOrder      = 36646,
            Alliance_YourFirstInscriptionWorkOrder      = 36647,
            Alliance_YourFirstTailoringWorkOrder        = 36643,
            /// Herb garden
            Alliance_ClearingTheGarden                  = 36404,
            /// Barn
            Alliance_BreakingIntoTheTrapGame            = 36271,
            Alliance_FeedingAnArmy                      = 36272,
            Alliance_BiggerTrapBetterRewards            = 36274,
            /// Lumber Mill
            Alliance_EasingIntoLumberjacking            = 36189,
            Alliance_TurningTimberIntoProfit            = 36192,
            /// Lunar Fall Excavation
            Alliance_ThingsAreNotGorenOurWay            = 34192,
            /// Mage Tower
            Alliance_PortablePortals                    = 38354,
            /// Gladiator's Sanctum
            Alliance_WarlordOfDraenor                   = 36876,
            /// Fishing Hut
            Alliance_AnglinInOurGarrison                = 36202,
            /// Gnomish Gearworks
            Alliance_UnconventionalInventions           = 37091,
            /// Trading Post
            Alliance_TricksOfTheTrade                   = 37088,
            Alliance_AuctionningForParts                = 36948,

            /// Horde
            QUEST_ETABLISH_YOUR_GARRISON_H              = 34378,
            QUEST_WHAT_WE_GOT                           = 34824,
            Horde_BuildYourBarracks                     = 34461,
            Horde_LostInTransition                      = 37060,
            Horde_BiggerIsBetter                        = 36567,
            Horde_MissionProbable                       = 34775,
            Horde_AshranAppearance                      = 36706,
            Horde_TheHeadHunterHarverst                 = 37046,
            /// Small profession building quests
            Horde_YourFirstBlacksmithingWorkOrder       = 37569,
            Horde_YourFirstTailoringWorkOrder           = 37575,
            Horde_YourFirstAlchemyWorkOrder             = 37568,
            Horde_YourFirstLeatherworkingWorkOrder      = 37574,
            Horde_YourFirstEnchantingWorkOrder          = 37570,
            Horde_YourFirstJewelcraftingWorkOrder       = 37573,
            Horde_YourFirstEngineeringWorkOrder         = 37571,
            Horde_YourFirstInscriptionWorkOrder         = 37572,
            /// Herb garden
            Horde_ClearingTheGarden                     = 34193,
            /// Barn
            Horde_BreakingIntoTheTrapGame               = 36345,
            Horde_FeedingAnArmy                         = 36344,
            Horde_BiggerTrapBetterRewards               = 36346,
            /// Lumber Mill
            Horde_EasingIntoLumberjacking               = 36137,
            Horde_TurningTimberIntoProfit               = 36138,
            /// Frostwall Mines
            Horde_ThingsAreNotGorenOurWay               = 35154,
            /// Spirit Lodge
            Horde_PortablePortals                       = 38351,
            /// Gladiator's Sanctum
            Horde_WarlordOfDraenor                      = 36874,
            /// Fishing Hut
            Horde_AnglinInOurGarrison                   = 36132,
            /// Goblin Workshop
            Horde_UnconventionalInventions              = 37044,
            /// Trading Post
            Horde_TricksOfTheTrade                      = 37062,
            Horde_AuctionningForParts                   = 37014,

            /// Both factions
            Quest_FishFight                             = 34846,

            None                                        = 0
        };
    }   ///< namespace Quests

    /// Garrison achievement related
    enum Achievements
    {
        ACHIEVEMENT_STAYING_REGULAR = 8933
    };

    /// Garrison NPCs
    enum NPCs
    {
        NPC_LADY_SENA                                   = 80223,
        NPC_GARRISON_ALLIANCE_CART                      = 81627,
        NPC_GARRISON_ALLIANCE_HORSE                     = 81633,
        NPC_SENIOR_PEON_II                              = 86775,
        NPC_FROSTWALL_PEON_DYNAMIC                      = 79605,
        NPC_FROSTWALL_GRUNT                             = 79781,
        NPC_GASLOWE                                     = 78466,
        NPC_ASSISTANT_BRIGHTSTONE                       = 84455,
        NPC_SKAGGIT                                     = 80225,
        NPC_ARCHER                                      = 87518,
        Marksman                                        = 88391,
        LunarfallRaccoon                                = 85341,
        NpcFrostwallNibbler                             = 81967,
        TrapL1QuestKillCredit                           = 84882,
        TrapL2QuestKillCredit                           = 84883,
        TrapL3QuestKillCredit                           = 84884,
        NpcFarmerLokLubSummon                           = 85093,
        NpcHomerStonefield                              = 83967,
        NpcTommyJoeStonefield                           = 83968,
        NpcHordePeon                                    = 83985,
        NpcAllianceLumberjack                           = 83950,
        NpcLunarfallGoren                               = 83628,
        NpcLunarfallGorenHatchling                      = 83629,
        NpcFrostwallGoren                               = 81362,
        NpcFrostwallGorenHatchling                      = 81398,
        NpcStonetooth                                   = 81396,
        NpcAllianceMiner                                = 77361,
        NpcHordeMiner                                   = 79837,
        NpcAllianceAncientTradingMechanismQuestGiver    = 87206,
        NpcAllianceAncientTradingMechanismAuctioneer    = 87205,
        NpcHordeAncientTradingMechanismQuestGiver       = 86806,
        NpcHordeAncientTradingMechanismAuctioneer       = 87109,
        NpcHordeJewelCraftingFollower                   = 88546,
        NpcAllianceJewelCraftingFollower                = 88545
    };

    namespace WorkshopGearworks
    {
        enum InventionsGobIDs
        {
            /// Level 1
            GobStickyGrenades                   = 234146,
            GobRoboBooster                      = 235078,
            GobPneumaticPowerGauntlet           = 234095,
            GobSkyTerrorPersonnalDeliverySystem = 233900,

            /// Level 2
            GobPrototypeMechanoHog              = 234018, ///< Guessed Horde
            GobPrototypeMekgineersChopper       = 234019, ///< Guessed Alliance
            GobNukularTargetPainter             = 235126,
            GobXD57BullseyeGuidedRocketKit      = 233899,
            GobGG117MicroJetpack                = 233901,
            GobSentryTurretDispenser            = 234017,

            /// Level 3
            GobWorkshopWorkorder                = 238756
        };

        enum InventionItemIDs
        {
            /// Level 1
            ItemStickyGrenades                   = 114983,
            ItemRoboBooster                      = 119158,
            ItemPneumaticPowerGauntlet           = 114974,
            ItemSkyTerrorPersonnalDeliverySystem = 114246,

            /// Level 2
            ItemPrototypeMechanoHog           = 114925, ///< Guessed Horde
            ItemPrototypeMekgineersChopper    = 114924, ///< Guessed Alliance
            ItemNukularTargetPainter          = 115530,
            ItemXD57BullseyeGuidedRocketKit   = 114633,
            ItemGG117MicroJetpack             = 114244,
            ItemSentryTurretDispenser         = 114744,

            /// Level 3 (shipments, not daily loot)
            ItemOverchargedSiegeEngine        = 119436, ///< Alliance
            ItemOverchargedDemolkisher        = 119437  ///< Horde
        };

        static std::vector<uint32> g_FirstLevelInventions =
        {
            GobStickyGrenades,
            GobRoboBooster,
            GobPneumaticPowerGauntlet,
            GobSkyTerrorPersonnalDeliverySystem
        };

        static std::vector<uint32> g_SecondLevelInventions =
        {
            GobPrototypeMechanoHog,
            GobPrototypeMekgineersChopper,
            GobNukularTargetPainter,
            GobXD57BullseyeGuidedRocketKit,
            GobGG117MicroJetpack,
            GobSentryTurretDispenser,
        };

        static uint32 const g_ThirdLevelInvention = GobWorkshopWorkorder;
        
        static std::map<uint32, uint32> g_GobItemRelations =
        {
            /// Level 1
            { InventionsGobIDs::GobStickyGrenades,                   InventionItemIDs::ItemStickyGrenades },
            { InventionsGobIDs::GobRoboBooster,                      InventionItemIDs::ItemRoboBooster },
            { InventionsGobIDs::GobPneumaticPowerGauntlet,           InventionItemIDs::ItemPneumaticPowerGauntlet },
            { InventionsGobIDs::GobSkyTerrorPersonnalDeliverySystem, InventionItemIDs::ItemSkyTerrorPersonnalDeliverySystem },

            /// Level 2
            { InventionsGobIDs::GobPrototypeMechanoHog,         InventionItemIDs::ItemPrototypeMechanoHog },
            { InventionsGobIDs::GobPrototypeMekgineersChopper,  InventionItemIDs::ItemPrototypeMekgineersChopper },
            { InventionsGobIDs::GobNukularTargetPainter,        InventionItemIDs::ItemNukularTargetPainter },
            { InventionsGobIDs::GobXD57BullseyeGuidedRocketKit, InventionItemIDs::ItemXD57BullseyeGuidedRocketKit },
            { InventionsGobIDs::GobGG117MicroJetpack,           InventionItemIDs::ItemGG117MicroJetpack },
            { InventionsGobIDs::GobSentryTurretDispenser,       InventionItemIDs::ItemSentryTurretDispenser },

            /// Level 3 needs custom handling, 1 gob 2 items
        };
    }

    namespace LumberMillData
    {
        static std::vector<uint32> const g_SmallTimber =
        {
            234193,
            234197,
            233604,
            234080,
            234122,
            234126,
            234109,
            234110,
            233922,
            234097
        };

        static std::vector<uint32> const g_MediumTimber =
        {
            234000,
            234022,
            234098,
            234119,
            234123,
            234127,
            234194,
            234196,
            234198,
            233634
        };

        static std::vector<uint32> const g_LargeTimber =
        {
            234000,
            234022,
            234098,
            234119,
            234123,
            234127,
            234194,
            234196
        };
    }

    namespace StablesData
    {
        std::vector<G3D::Vector3> const g_CreaturesJumps =
        {
            /// meadowstomper - 86852
            { 3419.6072f, 4388.9111f, 228.3024f },
            { 3323.6699f, 4510.6533f, 180.0031f },
            { 3322.0098f, 4475.1860f, 169.4178f },
            /// snarler - 86851
            { 2572.6372f, 6142.9878f, 80.2156f },
            { 2515.4197f, 6132.1719f, 80.0046f },
            { 2483.2539f, 6104.6841f, 83.2874f },
            { 2576.8103f, 5949.7192f, 80.4125f },
            { 2642.7153f, 5938.2900f, 84.2296f },
            { 2641.6606f, 5977.7524f, 82.7445f },
            { 2611.9395f, 6015.2808f, 78.9488f },
            /// silverpelt - 86801
            { 1511.6106f, -1065.3923f, 12.7051f },
            { 1470.9410f, -1051.7019f, -4.3017f }
        };

        static const uint64 g_PendingQuestFlag  = 0x40000000;
        static const uint32 g_LassoAllianceAura = 173686;
        static const uint32 g_LassoHordeAura    = 174070;

        enum BonusAuras
        {
            StablesAuraLevel1 = 169605,
            StablesAuraLevel2 = 169606,
            StablesAuraLevel3 = 169607
        };

        enum TrainingMountsAuras
        {
            RockstuckTrainingMountAura     = 174221, ///< Boar Quests
            MeadowstomperTrainingMountAura = 174219, ///< Elekk Quests
            IcehoofTrainingMountAura       = 174218, ///< Clefthoof Quests
            SnarlerTrainingMountAura       = 174216, ///< Wolf Quests
            RiverwallowTrainingMountAura   = 174220, ///< Riverbeast Quests
            SilverpeltTrainingMountAura    = 174222  ///< Talbuk Quests
        };

        /// Get from Mount.db2 with spellIDs used as questline rewards
        enum MountDisplayIDs
        {
            DisplayTrainedRocktusk      = 59735,
            DisplayTrainedIcehoof       = 59320,
            DisplayTrainedMeadowstomper = 59340,
            DisplayTrainedSnarler       = 59757,
            DisplayTrainedRiverwallow   = 59743,
            DisplayTrainedSilverpelt    = 59365
        };

        enum eKillCredits
        {
            ElekkKillCredit          = 87009,
            TalbukKillCredit         = 87007,
            WolfKillCredit           = 87008,
            RiverbeasttKillCredit    = 87010,
            ClefthoofKillCredit      = 87011,
            BoarKillCredit           = 87013
        };

        namespace Alliance
        {
            static const std::vector<SequencePosition> g_AllianceCreaturesPos =
            {
                /// Level 1
                { 12.5372f,  5.8878f, 0.6798f, 1.5558f }, ///< First Mount from spellbook
                {  6.3883f,  6.2564f, 0.6798f, 1.5715f }, ///< Second Mount from spellbook
                {  5.5064f, -6.1384f, 0.6720f, 4.6228f }, ///< First Mount in training
                { 16.4277f, -6.5664f, 0.6734f, 1.8818f }, ///< Second Mount in training
                { -6.6953f,  2.0611f, 0.6462f, 0.3447f }, ///< Second QuestGiver
                
                /// Level 2
                { 10.5691f, 5.2344f, 0.0905f, 1.6148f  }, ///< First Mount from spellbook
                { 0.4318f, 4.6344f, 0.0849f, 1.5637f   }, ///< Second Mount from spellbook
                { 4.6486f, -7.1033f, 0.0872f, 1.5284f  }, ///< First Mount in training
                { 10.8380f, -7.3660f, 0.0931f, 1.5284f }, ///< Second Mount in training
                { -5.4332f, 3.1585f, 0.0106f, 6.0719f  }, ///< Second QuestGiver

                /// Level 3
                { 16.2822f, 16.3610f, 0.1327f, 4.8467f  }, ///< First Mount from spellbook
                { 7.3848f, 17.7451f, 0.2598f, 4.7446f   }, ///< Second Mount from spellbook
                { 11.6410f, -18.5553f, 0.2767f, 1.6973f }, ///< First Mount in training
                { -2.0329f, -19.1035f, 0.2359f, 1.6109f }, ///< Second Mount in training
                { -12.7107f, -8.0214f, 0.4587f, 6.2565f }, ///< Second QuestGiver
            };

            namespace FannyQuestGiver
            {
                //////////////////////////
                //////////////////////////
                
                enum ElekkQuests
                {
                    /// Initial Quest
                    QuestEntanglingAnElekk = 36915,
                    /// Progression Quests
                    ElekkTrainingQuest1    = 37015,
                    ElekkTrainingQuest2    = 37016,
                    ElekkTrainingQuest3    = 37017,
                    ElekkTrainingQuest4    = 37018,
                    ElekkTrainingQuest5    = 37019,
                    ElekkTrainingQuest6    = 37020,
                    ElekkTrainingQuest7    = 37021
                };

                static const std::vector<uint32> g_ElekkQuests =
                {
                    ElekkTrainingQuest1,
                    ElekkTrainingQuest2,
                    ElekkTrainingQuest3,
                    ElekkTrainingQuest4,
                    ElekkTrainingQuest5,
                    ElekkTrainingQuest6,
                    ElekkTrainingQuest7
                };

                enum ClefthoofQuests
                {
                    /// Initial Quest
                    QuestCapturingAClefthoof  = 36916,
                    /// Progression Quests
                    ClefthoofTrainingQuest1   = 36983,
                    ClefthoofTrainingQuest2   = 36984,
                    ClefthoofTrainingQuest3   = 36985,
                    ClefthoofTrainingQuest4   = 36986,
                    ClefthoofTrainingQuest5   = 36987,
                    ClefthoofTrainingQuest6   = 36988,
                    ClefthoofTrainingQuest7   = 36989,
                    ClefthoofTrainingQuest8   = 36990,
                    ClefthoofTrainingQuest9   = 36991,
                    ClefthoofTrainingQuest10  = 36992,
                    ClefthoofTrainingQuest11  = 36993,
                    ClefthoofTrainingQuest12  = 36994
                };

                static const std::vector<uint32> g_ClefthoofQuests =
                {
                    ClefthoofTrainingQuest1,
                    ClefthoofTrainingQuest2,
                    ClefthoofTrainingQuest3,
                    ClefthoofTrainingQuest4,
                    ClefthoofTrainingQuest5,
                    ClefthoofTrainingQuest6,
                    ClefthoofTrainingQuest7,
                    ClefthoofTrainingQuest8,
                    ClefthoofTrainingQuest9,
                    ClefthoofTrainingQuest10,
                    ClefthoofTrainingQuest11,
                    ClefthoofTrainingQuest12
                };

                enum BoarQuests
                {
                    /// Initial Quest
                    QuestBestingABoar   = 36913,
                    /// Progression Quests
                    BoarTrainingQuest1  = 36995,
                    BoarTrainingQuest2  = 36996,
                    BoarTrainingQuest3  = 36997,
                    BoarTrainingQuest4  = 36998,
                    BoarTrainingQuest5  = 36999,
                    BoarTrainingQuest6  = 37000,
                    BoarTrainingQuest7  = 37001,
                    BoarTrainingQuest8  = 37002,
                    BoarTrainingQuest9  = 37003,
                    BoarTrainingQuest10 = 37004
                };

                static const std::vector<uint32> g_BoarQuests =
                {
                    BoarTrainingQuest1,
                    BoarTrainingQuest2,
                    BoarTrainingQuest3,
                    BoarTrainingQuest4,
                    BoarTrainingQuest5,
                    BoarTrainingQuest6,
                    BoarTrainingQuest7,
                    BoarTrainingQuest8,
                    BoarTrainingQuest9,
                    BoarTrainingQuest10
                };
            }

            namespace KeeganQuestGiver
            {
                enum WolfQuests
                {
                    /// Initial Quest
                    QuestWanglingAWolf = 36914,
                    /// Progression Quests
                    WolfTrainingQuest1 = 37022,
                    WolfTrainingQuest2 = 37023,
                    WolfTrainingQuest3 = 37024,
                    WolfTrainingQuest4 = 37025,
                    WolfTrainingQuest5 = 37026,
                    WolfTrainingQuest6 = 37027,
                    WolfTrainingQuest7 = 37028
                };

                static const std::vector<uint32> g_WolfQuests =
                {
                    WolfTrainingQuest1,
                    WolfTrainingQuest2,
                    WolfTrainingQuest3,
                    WolfTrainingQuest4,
                    WolfTrainingQuest5,
                    WolfTrainingQuest6,
                    WolfTrainingQuest7
                };

                enum RiverbeastQuests
                {
                    /// Initial Quest
                    QuestRequisitionARiverbeast = 36918,
                    /// Progression Quests
                    RiverbeastTrainingQuest1    = 37005,
                    RiverbeastTrainingQuest2    = 37006,
                    RiverbeastTrainingQuest3    = 37007,
                    RiverbeastTrainingQuest4    = 37008,
                    RiverbeastTrainingQuest5    = 37009,
                    RiverbeastTrainingQuest6    = 37010,
                    RiverbeastTrainingQuest7    = 37011,
                    RiverbeastTrainingQuest8    = 37012,
                    RiverbeastTrainingQuest9    = 37013
                };

                static const std::vector<uint32> g_RiverbeastQuests =
                {
                    RiverbeastTrainingQuest1,
                    RiverbeastTrainingQuest2,
                    RiverbeastTrainingQuest3,
                    RiverbeastTrainingQuest4,
                    RiverbeastTrainingQuest5,
                    RiverbeastTrainingQuest6,
                    RiverbeastTrainingQuest7,
                    RiverbeastTrainingQuest8,
                    RiverbeastTrainingQuest9
                };
                enum TalbukQuests
                {
                    /// Initial Quest
                    QuestTamingATalbuk    = 36911,
                    /// Progression Quests
                    TalbukTrainingQuest1  = 36971,
                    TalbukTrainingQuest2  = 36972,
                    TalbukTrainingQuest3  = 36973,
                    TalbukTrainingQuest4  = 36974,
                    TalbukTrainingQuest5  = 36975,
                    TalbukTrainingQuest6  = 36976,
                    TalbukTrainingQuest7  = 36977,
                    TalbukTrainingQuest8  = 36978,
                    TalbukTrainingQuest9  = 36979,
                    TalbukTrainingQuest10 = 36980,
                    TalbukTrainingQuest11 = 36981,
                    TalbukTrainingQuest12 = 36982
                };

                static const std::vector<uint32> g_TalbukQuests =
                {
                    TalbukTrainingQuest1,
                    TalbukTrainingQuest2,
                    TalbukTrainingQuest3,
                    TalbukTrainingQuest4,
                    TalbukTrainingQuest5,
                    TalbukTrainingQuest6,
                    TalbukTrainingQuest7,
                    TalbukTrainingQuest8,
                    TalbukTrainingQuest9,
                    TalbukTrainingQuest10,
                    TalbukTrainingQuest11,
                    TalbukTrainingQuest12
                };
            }
        }

        namespace Horde
        {
            static const std::vector<SequencePosition> g_HordeCreaturesPos =
            {
                /// Level 1
                { 10.8978f,  6.2676f, 0.4298f, 2.1677f }, ///< First Mount from spellbook
                {  6.9427f,  4.4816f, 0.4298f, 1.8300f }, ///< Second Mount from spellbook
                {  7.3520f, -6.3004f, 0.4294f, 4.4899f }, ///< First Mount in training
                { 15.7939f, -5.6128f, 0.4465f, 1.7449f }, ///< Second Mount in training
                { -3.5501f, -0.0913f, 0.4295f, 0.0799f }, ///< Second QuestGiver
                
                /// Level 2
                { 8.4927f, 9.8405f, 0.2139f, 1.4275f   }, ///< First Mount from spellbook
                { 1.2528f, 10.8851f, 0.2077f, 1.4275f  }, ///< Second Mount from spellbook
                { 1.1784f, -12.7750f, 0.2393f, 1.5155f }, ///< First Mount in training
                { 7.7486f, -11.7724f, 0.2714f, 1.5194f }, ///< Second Mount in training
                { -3.0227f, 2.5846f, 0.1670f, 0.1944f  }, ///< Second QuestGiver

                /// Level 3
                { 7.8584f, 14.2116f, 0.6680f, 1.6189f   }, ///< First Mount from spellbook
                { 14.2914f, 15.2063f, 0.6681f, 1.6032f  }, ///< Second Mount from spellbook
                { 17.2089f, -15.8898f, 0.6771f, 1.7407f }, ///< First Mount in training
                { 9.4654f, -19.1903f, 0.6674f, 1.7407f  }, ///< Second Mount in training
                { -1.9750f, 2.5123f, 0.6682f, 5.6323f   }  ///< Second QuestGiver
            };

            namespace SagePalunaQuestGiver
            {
                enum WolfQuests
                {
                    /// Initial Quest
                    QuestWanglingAWolf = 36950,
                    /// Progression Quests
                    WolfTrainingQuest1 = 37105,
                    WolfTrainingQuest2 = 37106,
                    WolfTrainingQuest3 = 37107,
                    WolfTrainingQuest4 = 37108,
                    WolfTrainingQuest5 = 37109,
                    WolfTrainingQuest6 = 37110,
                    WolfTrainingQuest7 = 37111
                };

                static const std::vector<uint32> g_WolfQuests =
                {
                    WolfTrainingQuest1,
                    WolfTrainingQuest2,
                    WolfTrainingQuest3,
                    WolfTrainingQuest4,
                    WolfTrainingQuest5,
                    WolfTrainingQuest6,
                    WolfTrainingQuest7
                };

                enum RiverbeastQuests
                {
                    /// Initial Quest
                    QuestRequisitionARiverbeast = 36945,
                    /// Progression Quests
                    RiverbeastTrainingQuest1    = 37071,
                    RiverbeastTrainingQuest2    = 37072,
                    RiverbeastTrainingQuest3    = 37073,
                    RiverbeastTrainingQuest4    = 37074,
                    RiverbeastTrainingQuest5    = 37075,
                    RiverbeastTrainingQuest6    = 37076,
                    RiverbeastTrainingQuest7    = 37077,
                    RiverbeastTrainingQuest8    = 37078,
                    RiverbeastTrainingQuest9    = 37079
                };

                static const std::vector<uint32> g_RiverbeastQuests =
                {
                    RiverbeastTrainingQuest1,
                    RiverbeastTrainingQuest2,
                    RiverbeastTrainingQuest3,
                    RiverbeastTrainingQuest4,
                    RiverbeastTrainingQuest5,
                    RiverbeastTrainingQuest6,
                    RiverbeastTrainingQuest7,
                    RiverbeastTrainingQuest8,
                    RiverbeastTrainingQuest9
                };
                enum TalbukQuests
                {
                    /// Initial Quest
                    QuestTamingATalbuk    = 36917,
                    /// Progression Quests
                    TalbukTrainingQuest1  = 37093,
                    TalbukTrainingQuest2  = 37094,
                    TalbukTrainingQuest3  = 37095,
                    TalbukTrainingQuest4  = 37096,
                    TalbukTrainingQuest5  = 37097,
                    TalbukTrainingQuest6  = 37098,
                    TalbukTrainingQuest7  = 37099,
                    TalbukTrainingQuest8  = 37100,
                    TalbukTrainingQuest9  = 37101,
                    TalbukTrainingQuest10 = 37102,
                    TalbukTrainingQuest11 = 37103,
                    TalbukTrainingQuest12 = 37104
                };

                static const std::vector<uint32> g_TalbukQuests =
                {
                    TalbukTrainingQuest1,
                    TalbukTrainingQuest2,
                    TalbukTrainingQuest3,
                    TalbukTrainingQuest4,
                    TalbukTrainingQuest5,
                    TalbukTrainingQuest6,
                    TalbukTrainingQuest7,
                    TalbukTrainingQuest8,
                    TalbukTrainingQuest9,
                    TalbukTrainingQuest10,
                    TalbukTrainingQuest11,
                    TalbukTrainingQuest12
                };
            }

            namespace TormakQuestGiver
            {
                enum ElekkQuests
                {
                    /// Initial Quest
                    QuestEntanglingAnElekk = 36946,
                    /// Progression Quests
                    ElekkTrainingQuest1    = 37063,
                    ElekkTrainingQuest2    = 37064,
                    ElekkTrainingQuest3    = 37065,
                    ElekkTrainingQuest4    = 37066,
                    ElekkTrainingQuest5    = 37067,
                    ElekkTrainingQuest6    = 37068,
                    ElekkTrainingQuest7    = 37069
                };

                static const std::vector<uint32> g_ElekkQuests =
                {
                    ElekkTrainingQuest1,
                    ElekkTrainingQuest2,
                    ElekkTrainingQuest3,
                    ElekkTrainingQuest4,
                    ElekkTrainingQuest5,
                    ElekkTrainingQuest6,
                    ElekkTrainingQuest7
                };

                enum ClefthoofQuests
                {
                    /// Initial Quest
                    QuestCapturingAClefthoof  = 36912,
                    /// Progression Quests
                    ClefthoofTrainingQuest1   = 37048,
                    ClefthoofTrainingQuest2   = 37049,
                    ClefthoofTrainingQuest3   = 37050,
                    ClefthoofTrainingQuest4   = 37051,
                    ClefthoofTrainingQuest5   = 37052,
                    ClefthoofTrainingQuest6   = 37053,
                    ClefthoofTrainingQuest7   = 37054,
                    ClefthoofTrainingQuest8   = 37055,
                    ClefthoofTrainingQuest9   = 37056,
                    ClefthoofTrainingQuest10  = 37057,
                    ClefthoofTrainingQuest11  = 37058,
                    ClefthoofTrainingQuest12  = 37059
                };

                static const std::vector<uint32> g_ClefthoofQuests =
                {
                    ClefthoofTrainingQuest1,
                    ClefthoofTrainingQuest2,
                    ClefthoofTrainingQuest3,
                    ClefthoofTrainingQuest4,
                    ClefthoofTrainingQuest5,
                    ClefthoofTrainingQuest6,
                    ClefthoofTrainingQuest7,
                    ClefthoofTrainingQuest8,
                    ClefthoofTrainingQuest9,
                    ClefthoofTrainingQuest10,
                    ClefthoofTrainingQuest11,
                    ClefthoofTrainingQuest12
                };

                enum BoarQuests
                {
                    /// Initial Quest
                    QuestBestingABoar   = 36944,
                    /// Progression Quests
                    BoarTrainingQuest1  = 37032,
                    BoarTrainingQuest2  = 37033,
                    BoarTrainingQuest3  = 37034,
                    BoarTrainingQuest4  = 37035,
                    BoarTrainingQuest5  = 37036,
                    BoarTrainingQuest6  = 37037,
                    BoarTrainingQuest7  = 37038,
                    BoarTrainingQuest8  = 37039,
                    BoarTrainingQuest9  = 37040,
                    BoarTrainingQuest10 = 37041
                };

                static const std::vector<uint32> g_BoarQuests =
                {
                    BoarTrainingQuest1,
                    BoarTrainingQuest2,
                    BoarTrainingQuest3,
                    BoarTrainingQuest4,
                    BoarTrainingQuest5,
                    BoarTrainingQuest6,
                    BoarTrainingQuest7,
                    BoarTrainingQuest8,
                    BoarTrainingQuest9,
                    BoarTrainingQuest10
                };
            }
        }
    }

    namespace GarrisonPortals
    {
        static std::vector<std::vector<std::vector<Position>>> const g_GobPos =
        {
            /// Alliance
            {
                /// Level 1
                {
                    { 13.5690f, -5.1295f, 1.3013f, 2.1965f },
                    { 9.88640f, -7.8239f, 1.2993f, 1.8823f },
                    { 1.76120f, -6.5470f, 1.3018f, 0.7749f },
                    { -0.8525f, -1.5060f, 1.3028f, 0.1977f },
                    { 0.54270f, 6.00540f, 1.3089f, 5.6326f },
                    { 9.68800f, 8.16940f, 1.3003f, 4.5645f },
                },

                /// Level 2
                {
                    { 7.15510f, -4.6149f, 1.0919f, 2.1715f },
                    { 2.78940f, -6.1055f, 1.0927f, 1.6021f },
                    { -1.2754f, -3.6913f, 1.0927f, 0.7185f },
                    { -2.9137f, -0.0758f, 1.0927f, 0.0313f },
                    { -1.6291f, 4.83730f, 1.0927f, 5.5095f },
                    { 3.19270f, 6.77720f, 1.0927f, 4.5395f },
                },

                /// Level 3
                {
                    { 2.4348f, -5.3152f, 22.0779f, 0.6157f },
                    { 0.8293f, -1.9244f, 22.0816f, 0.2662f },
                    { 1.3369f, 1.94620f, 22.0800f, 5.7050f },
                    { 5.0144f, 5.88490f, 22.0800f, 4.8521f },
                    { 7.4165f, 5.74850f, 22.0800f, 4.4123f },
                    { 10.5512f, 4.2319f, 22.0800f, 3.5797f },
                },
            },

            /// Horde
            {
                /// Level 1
                {
                    { 7.73170f, -3.8722f, 1.8845f, 2.3378f },
                    { 4.06240f, -4.6278f, 1.8839f, 1.5289f },
                    { 1.09380f, -3.0149f, 1.8846f, 0.7906f },
                    { -0.0707f, -0.1262f, 1.8841f, 6.2648f },
                    { 0.89660f, 3.25760f, 1.8838f, 5.5030f },
                    { 4.36360f, 4.51080f, 1.8847f, 4.7451f },
                },

                /// Level 2
                {
                    { 10.8699f, -6.2968f, 1.4459f, 2.3428f },
                    { 3.83070f, -7.8079f, 1.4459f, 1.4671f },
                    { -1.0515f, -5.2857f, 1.4459f, 0.7210f },
                    { -3.5338f, -0.2027f, 1.4470f, 6.2266f },
                    { -1.6294f, 5.27480f, 1.4461f, 5.5567f },
                    { 4.08910f, 7.86270f, 1.4502f, 4.7752f },
                },

                /// Level 3
                {
                    { 8.2846f, -5.28290f, 28.5693f, 2.3263f },
                    { 5.7219f, -6.13510f, 28.5601f, 1.8825f },
                    { 2.9724f, -6.37400f, 28.5709f, 1.3995f },
                    { -0.4703f, -4.2473f, 28.5654f, 0.7280f },
                    { -1.9028f, -0.4023f, 28.5751f, 0.1154f },
                    { -1.3891f, 2.42860f, 28.5747f, 5.7624f },
                },
            },
        };

        enum DraenorZones : uint16
        {
            /// TODO : Handle Tanaan Jungle in build > 6.2
            ZoneFrostfireRidge    = 6720,
            ZoneSpiresOfArak      = 6722,
            ZoneTalador           = 6662,
            ZoneNagrand           = 6755,
            ZoneShadowmoon        = 6719,
            ZoneGorgrond          = 6721
        };

        /// Those quests are used to handle portals phasing in Garrison and in Draenor
        enum PortalsQuests
        {
            QuestFrostfireRidge = 36904,
            QuestSpiresOfArak   = 36885,
            QuestTalador        = 36905,
            QuestNagrand        = 36906,
            QuestShadowmoon     = 36903,
            QuestGorgrond       = 36886,
            QuestTanaanJungle   = 39497
        };

        /// EffectForcePlayerInteraction, triggers a spell which completes associated quest
        enum ActivationSpells
        {
            FrostfireRidgeActivationSpell = 173817,
            SpiresOfArakActivationSpell   = 173554,
            TaladorActivationSpell        = 173806,
            NagrandActivationSpell        = 173809,
            ShadowmoonActivationSpell     = 173821,
            GorgrondActivationSpell       = 173504
        };

        /// EffectForcePlayerInteraction, triggers a spell which removes the associated quest from questlog,
        /// being incomplete or even rewarded
        enum DeactivationSpells
        {
            FrostfireRidgeDeactivationSpell = 173815,
            SpiresOfArakDeactivationSpell   = 173550,
            TaladorDeactivationSpell        = 173646,
            NagrandDeactivationSpell        = 173811,
            ShadowmoonDeactivationSpell     = 173819,
            GorgrondDeactivationSpell       = 173493
        };

        enum WorldGobs
        {
            FrostfireRidgeWorldGob   = 236911,
            SpiresOfArakWorldGob     = 236774,
            TaladorWorldGob          = 236906,
            NagrandWorldGob          = 236910,
            ShadowmoonValleyWorldGob = 236912,
            GorgrondWorldGob         = 236765
        };

        enum GarrisonGobs
        {
            /// Most of IDs are custom here, have them all would mean sniff them all and loose much time to farm stones on retail...
            FrostfireRidgeBuildingGob  = 236024,
            GorgrondBuildingGob        = 500003,
            NagrandBuildingGob         = 500004,
            ShadowmoonBuildingGob      = 500005,
            SpiresOfArakBuildingGob    = 500006,
            TaladorBuildingGob         = 500007
        };

        enum DraenorPortalPhases
        {
            WorldPhaseDeactivatedPortal = 0x80000000,
            WorldPhaseActivatedPortal   = 0x40000000
        };
    }

    namespace GarrisonAbilities
    {
        /// All this data lists have been parsed from wowhead
        std::vector<uint32> const g_FollowerTraits =
        {
            253,
            54,
            67,
            63,
            326,
            65,
            69,
            29,
            76,
            244,
            58,
            256,
            303,
            49,
            61,
            68,
            232,
            62,
            75,
            74,
            56,
            66,
            80,
            325,
            201,
            71,
            79,
            55,
            231,
            46,
            9,
            53,
            64,
            57,
            59,
            78,
            47,
            48,
            314,
            248,
            52,
            7,
            221,
            44,
            254,
            227,
            252,
            324,
            236,
            70,
            8,
            255,
            77,
            257,
            72,
            60,
            45,
            43,
            38,
            4,
            37,
            36,
            41,
            40,
            39,
            42,
            228,
            250,
            73
        };

        std::vector<uint32> const g_FollowerAbilities =
        {
            139,
            158,
            104,
            138,
            5,
            168,
            102,
            153,
            115,
            124,
            176,
            119,
            182,
            154,
            155,
            171,
            105,
            131,
            135,
            165,
            162,
            145,
            148,
            164,
            173,
            175,
            132,
            151,
            120,
            125,
            159,
            161,
            249,
            163,
            101,
            141,
            117,
            108,
            150,
            152,
            183,
            133,
            181,
            169,
            156,
            157,
            177,
            172,
            116,
            149,
            142,
            137,
            144,
            6,
            140,
            180,
            167,
            147,
            178,
            11 ,
            100,
            107,
            128,
            118,
            127,
            126,
            174,
            123,
            143,
            106,
            134,
            10,
            136,
            114,
            160,
            129,
            122,
            130,
            146,
            166,
            103,
            170,
            179,
            121
        };

        std::vector<uint32> const g_ShipAbilities =
        {
            260,
            261,
            262,
            263,
            264,
            265,
            266,
            267,
            268,
            269,
            270,
            271,
            272,
            273,
            274,
            275,
            276,
            277,
            278,
            279,
            280,
            281,
            282,
            283,
            284,
            285,
            286,
            287,
            288,
            289,
            290,
            291,
            292,
            293,
            294,
            299,
            300,
            304,
            305,
            306,
            315,
            317,
            318,
            319,
            320,
            321,
            322,
            323,
            327
        };
    }

    enum GameObjects
    {
        GobStump                = 234568,
        GobBlackrockDeposit     = 232542,
        GobRichBlackrockDeposit = 232543,
        GobTrueIronDeposit      = 232544,
        GobRichTrueIronDeposit  = 232545
    };

    /// NPC texts id
    namespace NPCTexts
    {
        enum
        {
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_1        = 85492,
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_2        = 85490,
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_3        = 85493,
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST    = 83682,
            NPC_TEXT_SHELLY_HAMBY_TEXT_QUEST             = 83505,
            NPC_TEXT_LADY_SENA                           = 82232,
            NPC_TEXT_SERGENT_GRIMJAW                     = 84128,
            NPC_TEXT_SENIOR_PEON_II                      = 88704,
            OllyNimkipDefault                            = 86941
        };
    }   ///< namespace NPCTexts

    /// Creature text index
    enum CreatureTexts
    {
        CREATURE_TEXT_BAROS_ETABLISH_YOUR_GARRISON_CHAT           = 0,
        CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_START_CHAT        = 1,
        CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_END_CHAT          = 2,
        CREATURE_TEXT_BAROS_SHIP_SALVAGE_START_CHAT               = 3,
        CREATURE_TEXT_BAROS_SHIP_SALVAGE_END_CHAT                 = 4,
        CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT = 0,
        CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT   = 1,
        CREATURE_TEXT_SENIOR_PEON_II_WHAT_WE_GOT                  = 0
    };

    /// Items
    enum Items
    {
        ITEM_SHELLY_HAMBY_REPORT                = 112730,
        ITEM_GARRISON_BLUEPRINT_BARRACKS_LEVEL1 = 111956,
        ItemFurryCagedBeast                     = 119813,
        ItemLeatheryCagedBeast                  = 119814,
        ItemMeatyCagedBeast                     = 119810,
        ItemCagedMightyClefthoof                = 119819,
        ItemCagedMightRiverbeast                = 119817,
        ItemCagedMightyWolf                     = 119815,
        ItemFireWeed                            = 109125,
        ItemTaladorOrchid                       = 109129,
        ItemNagrandArrowbloom                   = 109128,
        ItemStarflower                          = 109127,
        ItemGorgrondFlytrap                     = 109126,
        ItemFrostWeed                           = 109124,
        ItemBagOfSalvagedGoods                  = 114116,
        ItemCrateOfSalvage                      = 114119,
        ItemBigCrateOfSalvage                   = 114120,
        ItemTimber                              = 114781,
        ItemTimberSample                        = 114827,
        ItemGarrisonResources                   = 114677,
        ItemSecondGarrisonResources             = 118111,
        ItemMine                                = 116055,
        ItemBlackrockOre                        = 109118,
        ItemTrueIronOre                         = 109119,
        ItemIronTrap                            = 113991,
        ItemImprovedIronTrap                    = 115009,
        ItemDeadlyIronTrap                      = 115010
    };

    /// Waypoints
    enum Waypoints
    {
        MOVE_PATH_SENIOR_PEON_II        = 600010,
        MOVE_PATH_SENIOR_PEON_II_BACK   = 600011
    };

    /// Waypoints points
    enum WaypointsPoints
    {
        MOVE_PATH_SENIOR_PEON_II_END = 31
    };

    /// Move point IDs
    enum MovePointIDs
    {
        MOVE_POINT_GAZLOWE_BARRACK_A = 0
    };

    enum ObjectIDs
    {
        TimberWorkOrderAlly  = 37548,
        TimberWorkOrderHorde = 37487,
        FirstMageTowerSpiritLodgeObjectID = 91574,
        MageTowerQuestActivateOgreWaygate = 39133
    };

    /// Garrison creation coords
    static const float gGarrisonCreationCoords[][4] =
    {
        { 1766.761475f,  191.2846830f,  72.115326f, 0.4649370f },   ///< TEAM_ALLIANCE
        { 5698.020020f, 4512.1635574f, 127.401695f, 2.8622720f }    ///< TEAM_HORDE
    };

    /// Garrison hearthstone teleport coords
    static const float gGarrisonHearthStoneTeleportCoord[][4] =
    {
        /// Horde
        { 5561.15f,  4508.785f, 132.70000f, 3.406f   }, ///< Horde Level 1
        { 5565.86f,  4593.500f, 140.89300f, 0.000f   }, ///< Horde Level 2
        { 5563.70f,  4599.920f, 141.71100f, 0.000f   }, ///< Horde Level 3
        /// Alliance
        { 1850.71f,   254.430f,  78.08300f, 1.76f    }, ///< Alliance Level 1
        { 1947.08f,   324.883f,  90.28076f, 0.00f    }, ///< Alliance Level 2
        {    0.0f,       0.0f,      0.0f,      0.00f    }  ///< Alliance Level 3
    };

    /// Creature that follow garrison owner level
    static const uint32 gGarrisonLevelUpdateCreatures[] =
    {
        /// Alliance
        79953,  ///< Lientenant Thorn
        77370,  ///< Lunarfall Footman
        85312,  ///< Lunarfall Rifleman
        88223,  ///< SergeantCrowler
        /// Horde
        79781,  ///< Frostwall grunt
        80299,  ///< Frostwakk Axe thrower
        88228,  ///< Sergent Grinjaw
        80572   ///< Frostwall wolf
    };

    namespace ShipmentContainer
    {
        enum
        {
            ShipmentTest                    = 2, ///< Probably debug for Blizz
            ShipmentUnk1                    = 3, ///< Maybe fishing ? dunno yet
            ShipmentFishingHut              = 30,
            ShipmentAlchemyLab              = 31,
            ShipmentBarn                    = 32,
            ShipmentLumberMill              = 36,
            ShipmentTailoring               = 37,
            ShipmentBlacksmitthing          = 49,
            ShipmentLeathorworking          = 50,
            ShipmentJewelCrafting           = 51,
            ShipmentEnchanting              = 52,
            ShipmentEngineering             = 53,
            ShipmentInscription             = 54,
            ShipmentHerbGarden              = 55,
            ShipmentMine                    = 56,
            ShipmentAlchemyUnk              = 57,
            ShipmentBlacksmitthingUnk       = 58,
            ShipmentEnchantingUnk           = 59,
            ShipmentEngineeringUnk          = 60,
            ShipmentInscriptionUnk          = 61,
            ShipmentJewelCraftingUnk        = 62,
            ShipmentLeathorworkingUnk       = 63,
            ShipmentTailoringUnk            = 64,
            ShipmentConquerorsTribute       = 65,
            ShipmentTradingPost             = 66,
            ShipmentTradingPostUnk          = 70,
            ShipmentOverchargedDemolisher   = 74,
            ShipmentOverchargedSiegeEngine  = 75,
            ShipmentArmory                  = 77,
            ShipmentArmoryUnk               = 78,
            ShipmentMineUnk                 = 79,
            ShipmentHerbGardenUnk           = 80,
            ShipmentMageTower               = 100,
            ShipmentMageTowerUnk            = 101,
            ShipmentShipDestroyer           = 103, ///< Shipyard Related
            ShipmentShipSubmarine           = 104, ///< Shipyard Related
            ShipmentShipBattleship          = 105, ///< Shipyard Related
            ShipmentShipTransport           = 106, ///< Shipyard Related
            ShipmentShipCarrier             = 107, ///< Shipyard Related
            ShipmentShipTransportUnk        = 109, ///< Shipyard Related
            ShipmentShipSubmarineUnk        = 111, ///< Shipyard Related
            ShipmentShipBattleshipUnk       = 112, ///< Shipyard Related
            ShipmentShipDestroyerUnk        = 113, ///< Shipyard Related
            ShipmentShipDestroyerUnk2       = 114, ///< Shipyard Related
            ShipmentShipDestroyerUnk3       = 115, ///< Shipyard Related
            ShipmentShipDestroyerUnk4       = 119, ///< Shipyard Related
            ShipmentShipDestroyerUnk5       = 121, ///< Shipyard Related
            ShipmentGladiatorsSanctum       = 137  ///< Data from Sovak, cannot find it
        };

    }

    namespace Barn
    {
        /// Creatures that can be tamed with the Iron Trap for the Barn building
        namespace TameableEntries
        {
            /// First building level

            /* Result from :
            SELECT DISTINCT
            ct.entry
            FROM
            creature_template ct
            JOIN creature c
            ON (c.id = ct.`entry`)
            WHERE c.`map` = 1116       ///< Draenor
            AND ct.`family` IN (1, 43) ///< Wolf, Clefthoof
            AND ct.`rank` = 0          ///< Standard
            AND ct.`type` = 1          ///< Beast
            ORDER BY entry ASC ;

            Examples :

            72162,
            72991,
            73132,
            73205,
            73234*/
        }

        enum ShipmentIDS
        {
            /// Some shipments are unk atm
            ShipmentFurredBeast            = 82,
            ShipmentPowerfulFurredBeast    = 83,
            ShipmentLeatheredBeast         = 85,
            ShipmentPowerfulLeatheredBeast = 86,
            ShipmentMeatyBeast             = 88,
            ShipmentPowerfulMeatyBeast     = 89,
        };
    }

    namespace HordePeonData
    {
        enum
        {
            MOVE_POINT_ID_HOME_POSITION         = 100,
            MOVE_POINT_ID_THREE_FIRST           = 101,
            MOVE_POINT_ID_THREE_END             = 111,
            MOVE_POINT_ID_WOODCUTTING_DEPOSIT_A = 112,

            MOVE_PATH_FROSTWALL_A                   = 600000,
            MOVE_PATH_FROSTWALL_A_POINT_NEAR_HOME   = 1,
            MOVE_PATH_FROSTWALL_B                   = 600001,
            MOVE_PATH_FROSTWALL_B_POINT_NEAR_HOME   = 1,
            MOVE_PATH_WOOD_CUTTING_A                = 600002,
            MOVE_PATH_WOOD_CUTTING_A_MID            = 10,
            MOVE_PATH_WOOD_CUTTING_A_END            = 20,

            PHASE_BACK_TO_HOME          = 0,
            PHASE_WOODCUTTING           = 1,
            PHASE_WOODCUTTING_DISENGAGE = 2,
            PHASE_WOODCUTTING_REENGAGE  = 3,
            PHASE_WOODCUTTING_DEPOSIT   = 4,

            WOODCUTTING_TIME            = 30 * IN_MILLISECONDS,
            WOODCUTTING_DISENGAGE_TIME  =  2 * IN_MILLISECONDS,

            WOODCUTTING_MAX_CYLE_COUNT  = 1    /// Unk retail value range 1 := 4
        };

        static const float FrostWallTreePos[][4] =
        {
            { 5543.7100f, 4600.020f, 142.000f, 2.8211f },
            { 5558.2724f, 4608.380f, 138.500f, 1.8433f },
            { 5550.3200f, 4594.158f, 136.795f, 3.7440f },
            { 5548.6600f, 4583.240f, 136.640f, 5.4320f },
            { 5544.5209f, 4600.591f, 141.500f, 2.8604f },
            { 5541.5766f, 4584.437f, 137.900f, 1.7134f },
            { 5535.4940f, 4567.104f, 137.480f, 4.0602f },
            { 5561.4848f, 4616.658f, 139.720f, 2.8347f }
        };

        static const float FrostWallWoodCuttingA[3] =
        {
            5572.927f, 4602.003f, 136.58f
        };

    }   ///< namespace HordePeonData

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SCRIPT_DATA_HPP_GARRISON
