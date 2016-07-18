////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef GARRISON_BUILDING_MANAGER_HPP_GARRISON
# define GARRISON_BUILDING_MANAGER_HPP_GARRISON

#include "Common.h"

namespace MS { namespace Garrison
{
    /// Building condition structure
    struct BuildingCondition
    {
        uint32 BuildingID;  ///< Building ID @GarrBuilding.db2
        uint32 MinLevel;    ///< Building access min player level
        uint32 QuestA;      ///< Alliance required rewarded quest ID
        uint32 QuestH;      ///< Horde required rewarded quest ID
    };

    /// Building condition's quest IDs
    namespace BuildingConditionQuest
    {
        enum
        {
            ClearingTheGardenA       = 36404,
            ClearingTheGardenH       = 34193,
            BattlePetRoundup         = 36483,
            Scrappin                 = 36662,
            ThingsAreNotGorenOurWayA = 34192,
            ThingsAreNotGorenOurWayH = 35154,
            FishFight                = 34846
        };
    }

    /// Building conditions
    static const BuildingCondition gBuildingConditions[] =
    {
        /// BuildingID, MinLevel QuestA                                            QuestH
        {   29,         96,      BuildingConditionQuest::ClearingTheGardenA,       BuildingConditionQuest::ClearingTheGardenH       },    ///< Herb Garden
        {   42,         98,      BuildingConditionQuest::BattlePetRoundup,         BuildingConditionQuest::Scrappin                 },    ///< Menagerie
        {   61,         92,      BuildingConditionQuest::ThingsAreNotGorenOurWayA, BuildingConditionQuest::ThingsAreNotGorenOurWayH },    ///< Lunarfall Excavation / Frostwall Mines
        {   64,         94,      BuildingConditionQuest::FishFight,                BuildingConditionQuest::FishFight                }     ///< Fishing Shack
    };

    class Manager;

    /// Building manager class
    class BuildingManager
    {
        /// Grant access to ACE_Singleton class
        friend class ACE_Singleton<BuildingManager, ACE_Null_Mutex>;

        private:
            /// Constructor
            BuildingManager();
            /// Destructor
            ~BuildingManager();

        public:
            /// Learn allowed building blue prints
            /// @p_Player     : Target player
            /// @p_Garrison   : Target garrison
            void LearnAllowedBuildings(Player* p_Player, Manager* p_Garrison);
            /// Is that blue print allowed for Purchase a building
            /// @p_BuildingID : Building ID we are looking for
            /// @p_Player     : Target player
            bool IsBluePrintAllowedForPurchasingBuilding(uint32 p_BuildingID, Player* p_Player);

            /// Match conditions for building
            /// @p_BuildingID : Building ID we are looking for
            /// @p_Player     : Target player
            bool MatchsConditionsForBuilding(uint32 p_BuildingID, Player* p_Player);

        private:
            std::vector<int32> m_AllowedBluePrints; ///< Allowed blue print list

    };

}   ///< namespace Garrison
}   ///< namespace MS

#define sGarrisonBuildingManager ACE_Singleton<MS::Garrison::BuildingManager, ACE_Null_Mutex>::instance()

#endif  ///< GARRISON_BUILDING_MANAGER_HPP_GARRISON
#endif
