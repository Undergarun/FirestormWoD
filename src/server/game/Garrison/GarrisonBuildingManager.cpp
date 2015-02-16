////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonMgr.hpp"
#include "GarrisonBuildingManager.hpp"
#include "Player.h"
#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"

namespace MS { namespace Garrison 
{
    /// Constructor
    BuildingManager::BuildingManager()
    {

    }

    /// Destructor
    BuildingManager::~BuildingManager()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Match conditions for building
    /// @p_BuildingID : Building ID we are looking for
    /// @p_Player     : Target player
    bool BuildingManager::MatchsConditionsForBuilding(uint32 p_BuildingID, Player * p_Player)
    {
        uint32 l_ConditionCount = sizeof(gBuildingConditions) / sizeof(gBuildingConditions[0]);

        for (uint32 l_I = 0; l_I < l_ConditionCount; ++l_I)
        {
            BuildingCondition const* l_Condition = &gBuildingConditions[l_I];

            if (l_Condition->BuildingID != p_BuildingID)
                continue;

            if (p_Player->getLevel() < l_Condition->MinLevel)
                return false;

            if (p_Player->GetTeamId() == TEAM_ALLIANCE && !p_Player->IsQuestRewarded(l_Condition->QuestA))
                return false;
            else if (p_Player->GetTeamId() == TEAM_HORDE && !p_Player->IsQuestRewarded(l_Condition->QuestH))
                return false;

            return true;
        }

        return true;
    }

}   ///< namespace Garrison
}   ///< namespace MS