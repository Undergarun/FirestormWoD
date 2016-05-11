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
#include "Chat.h"

namespace MS { namespace Garrison 
{
    /// Constructor
    BuildingManager::BuildingManager()
    {
        m_AllowedBluePrints.push_back(Buildings::AlchemyLab__AlchemyLab_Level1);
        m_AllowedBluePrints.push_back(Buildings::Barn__Barn_Level1);
        m_AllowedBluePrints.push_back(Buildings::Barracks__Barracks_Level1);
        m_AllowedBluePrints.push_back(Buildings::DwarvenBunker__WarMill_Level1);
        m_AllowedBluePrints.push_back(Buildings::EnchanterStudy_EnchanterStudy_Level1);
        m_AllowedBluePrints.push_back(Buildings::EngineeringWorks__EngineeringWorks_Level1);
        m_AllowedBluePrints.push_back(Buildings::GemBoutique_GemBoutique_Level1);
        m_AllowedBluePrints.push_back(Buildings::LumberMill_LumberMill_Level1);
        m_AllowedBluePrints.push_back(Buildings::LunarfallInn_FrostwallTavern_Level1);
        m_AllowedBluePrints.push_back(Buildings::MageTower_SpiritLodge_Level1);
        m_AllowedBluePrints.push_back(Buildings::SalvageYard_SalvageYard_Level1);
        m_AllowedBluePrints.push_back(Buildings::ScribesQuarters__ScribesQuarters_Level1);
///        m_AllowedBluePrints.push_back(Buildings::Stables_Stables_Level1);
        m_AllowedBluePrints.push_back(Buildings::Storehouse__Storehouse_Level1);
        m_AllowedBluePrints.push_back(Buildings::TailoringEmporium_TailoringEmporium_Level1);
        m_AllowedBluePrints.push_back(Buildings::TheForge__TheForge_Level1);
        m_AllowedBluePrints.push_back(Buildings::TheTannery_TheTannery_Level1);
        m_AllowedBluePrints.push_back(Buildings::TradingPost__TradingPost_Level1);
        m_AllowedBluePrints.push_back(Buildings::GladiatorsSanctum_GladiatorsSanctum_Level1);
    }

    /// Destructor
    BuildingManager::~BuildingManager()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Learn allowed building blue prints
    /// @p_Player     : Target player
    /// @p_Garrison   : Target garrison
    void BuildingManager::LearnAllowedBuildings(Player * p_Player, Manager * p_Garrison)
    {
        if (!p_Player || !p_Garrison)
            return;

        auto l_KnownBluePrints = p_Garrison->GetKnownBlueprints();

        for (int32 l_BluePrint : m_AllowedBluePrints)
        {
            /// Don't relearn the BluePrint if the player already known it
            if (std::find(l_KnownBluePrints.begin(), l_KnownBluePrints.end(), l_BluePrint) != l_KnownBluePrints.end())
                continue;

            p_Garrison->LearnBlueprint(l_BluePrint);
        }
    }

    /// Is that blue print allowed for Purchase a building
    /// @p_BuildingID : Building ID we are looking for
    /// @p_Player     : Target player
    bool BuildingManager::IsBluePrintAllowedForPurchasingBuilding(uint32 p_BuildingID, Player * p_Player)
    {
        if (!p_Player || !p_Player->GetGarrison())
            return false;

        if (p_Player->isGameMaster())
        {
            ChatHandler(p_Player).PSendSysMessage("IsBluePrintAllowedForPurchasingBuilding Overrided by GM on.");
            return true;
        }

        return std::find(m_AllowedBluePrints.begin(), m_AllowedBluePrints.end(), p_BuildingID) != m_AllowedBluePrints.end();
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