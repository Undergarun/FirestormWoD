////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_PLAYER_CONDITION_HPP_GARRISON
#ifndef CROSS
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
            virtual bool OnConditionCheck(uint32 /*p_ConditionID*/, PlayerConditionEntry const* /*p_Condition*/, Player const* p_Player) override
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
        virtual bool OnConditionCheck(uint32 /*p_ConditionID*/, PlayerConditionEntry const* /*p_Condition*/, Player const* p_Player) override
        {
            if (!p_Player || !p_Player->GetGarrison() || !p_Player->GetGarrison()->HasBuildingType((Building::Type)t_BuildingTypeID))
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
            virtual bool OnConditionCheck(uint32 /*p_ConditionID*/, PlayerConditionEntry const* /*p_Condition*/, Player const* p_Player) override
            {
                if (!p_Player || !p_Player->GetGarrison() || !p_Player->GetGarrison()->HasBuildingType((Building::Type)t_BuildingID))
                    return false;

                if (!p_Player->IsQuestRewarded(t_QuestA) && !p_Player->IsQuestRewarded(t_QuestH))
                    return false;

                return true;
            }

    };

    using PlayerCondition_HasHerbGarden                     = GarrisonHasUnlockedBuildingPlayerCondition<26967, Building::Type::Farm, Quests::Alliance_ClearingTheGarden,       Quests::Horde_ClearingTheGarden>;
    using PlayerCondition_HasMine                           = GarrisonHasUnlockedBuildingPlayerCondition<43000, Building::Type::Mine, Quests::Alliance_ThingsAreNotGorenOurWay, Quests::Horde_ThingsAreNotGorenOurWay>; ///< Custom condition, to avoid conflict with herbs system

    using PlayerCondition_AlchemyLab_Level2                 = GarrisonHasBuildingPlayerCondition<27405, Building::ID::AlchemyLab_AlchemyLab_Level2>;
    using PlayerCondition_AlchemyLab_Level3                 = GarrisonHasBuildingPlayerCondition<27406, Building::ID::AlchemyLab_AlchemyLab_Level3>;

    using PlayerCondition_TailoringEmporium_Level2          = GarrisonHasBuildingPlayerCondition<27678, Building::ID::TailoringEmporium_TailoringEmporium_Level2>;
    using PlayerCondition_TailoringEmporium_Level3          = GarrisonHasBuildingPlayerCondition<27679, Building::ID::TailoringEmporium_TailoringEmporium_Level3>;

    using PlayerCondition_GemBoutique_Level2                = GarrisonHasBuildingPlayerCondition<28179, Building::ID::GemBoutique_GemBoutique_Level2>;
    using PlayerCondition_GemBoutique_Level3                = GarrisonHasBuildingPlayerCondition<28180, Building::ID::GemBoutique_GemBoutique_Level3>;

    using PlayerCondition_TheTannery_Level2                 = GarrisonHasBuildingPlayerCondition<28267, Building::ID::TheTannery_TheTannery_Level2>;
    using PlayerCondition_TheTannery_Level3                 = GarrisonHasBuildingPlayerCondition<28268, Building::ID::TheTannery_TheTannery_Level3>;

    using PlayerCondition_TheForge_Level2                   = GarrisonHasBuildingPlayerCondition<28366, Building::ID::TheForge_TheForge_Level2>;
    using PlayerCondition_TheForge_Level3                   = GarrisonHasBuildingPlayerCondition<28367, Building::ID::TheForge_TheForge_Level3>;

    using PlayerCondition_EnchanterStudy_Level2             = GarrisonHasBuildingPlayerCondition<29533, Building::ID::EnchanterStudy_EnchanterStudy_Level2>;
    using PlayerCondition_EnchanterStudy_Level3             = GarrisonHasBuildingPlayerCondition<29534, Building::ID::EnchanterStudy_EnchanterStudy_Level3>;

    using PlayerCondition_EngineeringWorks_Level2           = GarrisonHasBuildingPlayerCondition<29535, Building::ID::EngineeringWorks_EngineeringWorks_Level2>;
    using PlayerCondition_EngineeringWorks_Level3           = GarrisonHasBuildingPlayerCondition<29536, Building::ID::EngineeringWorks_EngineeringWorks_Level3>;

    using PlayerCondition_ScribesQuarters_Level2            = GarrisonHasBuildingPlayerCondition<29537, Building::ID::ScribesQuarters_ScribesQuarters_Level2>;
    using PlayerCondition_ScribesQuarters_Level3            = GarrisonHasBuildingPlayerCondition<29538, Building::ID::ScribesQuarters_ScribesQuarters_Level3>;

    using PlayerCondition_EnchanterStudy_Level2_Second      = GarrisonHasBuildingPlayerCondition<31228, Building::ID::EnchanterStudy_EnchanterStudy_Level2>;
    using PlayerCondition_EnchanterStudy_Level3_Second      = GarrisonHasBuildingPlayerCondition<31229, Building::ID::EnchanterStudy_EnchanterStudy_Level3>;

    using PlayerCondition_ScribesQuarters_Level2_Second     = GarrisonHasBuildingPlayerCondition<31230, Building::ID::ScribesQuarters_ScribesQuarters_Level2>;
    using PlayerCondition_ScribesQuarters_Level3_Second     = GarrisonHasBuildingPlayerCondition<31231, Building::ID::ScribesQuarters_ScribesQuarters_Level3>;

    using PlayerCondition_EngineeringWorks_Level2_Second    = GarrisonHasBuildingPlayerCondition<31232, Building::ID::EngineeringWorks_EngineeringWorks_Level2>;
    using PlayerCondition_EngineeringWorks_Level3_Second    = GarrisonHasBuildingPlayerCondition<31233, Building::ID::EngineeringWorks_EngineeringWorks_Level3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GO_HPP_GARRISON
#endif
