////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_PLAYER_CONDITION_HPP_GARRISON
#define GARRISON_PLAYER_CONDITION_HPP_GARRISON

#include "GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    /// Generic has building player condition script
    template<uint32 t_ConditionID, uint32 t_BuildingID> class GarrisonHasBuildingPlayerCondition : PlayerConditionScript
    {
        public:
            /// Constructor
            GarrisonHasBuildingPlayerCondition()
                : PlayerConditionScript(t_ConditionID)
            {

            }

            /// Called when a single condition is checked for a player.
            /// @p_ConditionID : Player condition ID
            /// @p_Condition   : Player condition instance
            /// @p_SourceInfo  : Player
            virtual bool OnConditionCheck(uint32 p_ConditionID, PlayerConditionEntry const* p_Condition, Player* p_Player) override
            {
                if (!p_Player || !p_Player->GetGarrison() || !p_Player->GetGarrison()->HasActiveBuilding(t_BuildingID))
                    return false;

                return true;
            }

    };

    /// Generic has building type player condition script
    template<uint32 t_ConditionID, uint32 t_BuildingTypeID> class GarrisonHasBuildingTypePlayerCondition : PlayerConditionScript
    {
        public:
        /// Constructor
        GarrisonHasBuildingTypePlayerCondition()
            : PlayerConditionScript(t_ConditionID)
        {

        }

        /// Called when a single condition is checked for a player.
        /// @p_ConditionID : Player condition ID
        /// @p_Condition   : Player condition instance
        /// @p_SourceInfo  : Player
        virtual bool OnConditionCheck(uint32 p_ConditionID, PlayerConditionEntry const* p_Condition, Player* p_Player) override
        {
            if (!p_Player || !p_Player->GetGarrison() || !p_Player->GetGarrison()->HasBuildingType((BuildingType::Type)t_BuildingTypeID))
                return false;

            return true;
        }

    };

    template<uint32 t_ConditionID, uint32 t_BuildingID, uint32 t_QuestA, uint32 t_QuestH> class GarrisonHasUnlockedBuildingPlayerCondition : PlayerConditionScript
    {
        public:
            /// Constructor
            GarrisonHasUnlockedBuildingPlayerCondition()
                : PlayerConditionScript(t_ConditionID)
            {

            }

            /// Called when a single condition is checked for a player.
            /// @p_ConditionID : Player condition ID
            /// @p_Condition   : Player condition instance
            /// @p_SourceInfo  : Player
            virtual bool OnConditionCheck(uint32 p_ConditionID, PlayerConditionEntry const* p_Condition, Player* p_Player) override
            {
                if (!p_Player || !p_Player->GetGarrison() || !p_Player->GetGarrison()->HasBuildingType((BuildingType::Type)t_BuildingID))
                    return false;

                if (!p_Player->IsQuestRewarded(t_QuestA) && !p_Player->IsQuestRewarded(t_QuestH))
                    return false;

                return true;
            }

    };

    using PlayerCondition_HasHerbGarden                     = GarrisonHasUnlockedBuildingPlayerCondition<26967, BuildingType::Farm, Quests::Alliance_ClearingTheGarden,       Quests::Horde_ClearingTheGarden>;
    using PlayerCondition_HasMine                           = GarrisonHasUnlockedBuildingPlayerCondition<43000, BuildingType::Mine, Quests::Alliance_ThingsAreNotGorenOurWay, Quests::Horde_ThingsAreNotGorenOurWay>; ///< Custom condition, to avoid conflict with herbs system

    using PlayerCondition_AlchemyLab_Level2                 = GarrisonHasBuildingPlayerCondition<27405, Buildings::AlchemyLab_AlchemyLab_Level2>;
    using PlayerCondition_AlchemyLab_Level3                 = GarrisonHasBuildingPlayerCondition<27406, Buildings::AlchemyLab_AlchemyLab_Level3>;

    using PlayerCondition_TailoringEmporium_Level2          = GarrisonHasBuildingPlayerCondition<27678, Buildings::TailoringEmporium_TailoringEmporium_Level2>;
    using PlayerCondition_TailoringEmporium_Level3          = GarrisonHasBuildingPlayerCondition<27679, Buildings::TailoringEmporium_TailoringEmporium_Level3>;

    using PlayerCondition_GemBoutique_Level2                = GarrisonHasBuildingPlayerCondition<28179, Buildings::GemBoutique_GemBoutique_Level2>;
    using PlayerCondition_GemBoutique_Level3                = GarrisonHasBuildingPlayerCondition<28180, Buildings::GemBoutique_GemBoutique_Level3>;

    using PlayerCondition_TheTannery_Level2                 = GarrisonHasBuildingPlayerCondition<28267, Buildings::TheTannery_TheTannery_Level2>;
    using PlayerCondition_TheTannery_Level3                 = GarrisonHasBuildingPlayerCondition<28268, Buildings::TheTannery_TheTannery_Level3>;

    using PlayerCondition_TheForge_Level2                   = GarrisonHasBuildingPlayerCondition<28366, Buildings::TheForge_TheForge_Level2>;
    using PlayerCondition_TheForge_Level3                   = GarrisonHasBuildingPlayerCondition<28367, Buildings::TheForge_TheForge_Level3>;

    using PlayerCondition_EnchanterStudy_Level2             = GarrisonHasBuildingPlayerCondition<29533, Buildings::EnchanterStudy_EnchanterStudy_Level2>;
    using PlayerCondition_EnchanterStudy_Level3             = GarrisonHasBuildingPlayerCondition<29534, Buildings::EnchanterStudy_EnchanterStudy_Level3>;

    using PlayerCondition_EngineeringWorks_Level2           = GarrisonHasBuildingPlayerCondition<29535, Buildings::EngineeringWorks_EngineeringWorks_Level2>;
    using PlayerCondition_EngineeringWorks_Level3           = GarrisonHasBuildingPlayerCondition<29536, Buildings::EngineeringWorks_EngineeringWorks_Level3>;

    using PlayerCondition_ScribesQuarters_Level2            = GarrisonHasBuildingPlayerCondition<29537, Buildings::ScribesQuarters_ScribesQuarters_Level2>;
    using PlayerCondition_ScribesQuarters_Level3            = GarrisonHasBuildingPlayerCondition<29538, Buildings::ScribesQuarters_ScribesQuarters_Level3>;

    using PlayerCondition_EnchanterStudy_Level2_Second      = GarrisonHasBuildingPlayerCondition<31228, Buildings::EnchanterStudy_EnchanterStudy_Level2>;
    using PlayerCondition_EnchanterStudy_Level3_Second      = GarrisonHasBuildingPlayerCondition<31229, Buildings::EnchanterStudy_EnchanterStudy_Level3>;

    using PlayerCondition_ScribesQuarters_Level2_Second     = GarrisonHasBuildingPlayerCondition<31230, Buildings::ScribesQuarters_ScribesQuarters_Level2>;
    using PlayerCondition_ScribesQuarters_Level3_Second     = GarrisonHasBuildingPlayerCondition<31231, Buildings::ScribesQuarters_ScribesQuarters_Level3>;

    using PlayerCondition_EngineeringWorks_Level2_Second    = GarrisonHasBuildingPlayerCondition<31232, Buildings::EngineeringWorks_EngineeringWorks_Level2>;
    using PlayerCondition_EngineeringWorks_Level3_Second    = GarrisonHasBuildingPlayerCondition<31233, Buildings::EngineeringWorks_EngineeringWorks_Level3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GO_HPP_GARRISON