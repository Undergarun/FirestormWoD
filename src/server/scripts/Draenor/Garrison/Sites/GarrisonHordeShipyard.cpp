////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GarrisonHordeShipyard.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"

enum
{
    GARRISON_PHASE_BASE = 0x0001
};

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    InstanceMapScript_GarrisonHordeShipyard::InstanceMapScript_GarrisonHordeShipyard()
        : InstanceMapScript("instance_Garrison_HS", MapIDs::MapGarrisonHordeShipyard)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript* InstanceMapScript_GarrisonHordeShipyard::GetInstanceScript(InstanceMap* p_Map) const
    {
        return new InstanceScript_GarrisonHordeShipyard(p_Map);
    }


    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonHordeShipyard::InstanceScript_GarrisonHordeShipyard(Map* p_Map)
        : GarrisonSiteBase(p_Map)
    {

    }
    /// Destructor
    InstanceScript_GarrisonHordeShipyard::~InstanceScript_GarrisonHordeShipyard()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonHordeShipyard::Update(uint32 /*p_Diff*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonHordeShipyard::OnOwnerEnter(Player* p_Owner)
    {
        /// Achievement "Even More Plots" alliance side
        if (p_Owner->GetTeamId() == TEAM_ALLIANCE && !p_Owner->GetAchievementMgr().HasAchieved(9101))
            p_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(9101), nullptr);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonHordeShipyard::OnQuestStarted(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {

    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonHordeShipyard::OnQuestReward(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonHordeShipyard::OnQuestAbandon(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonHordeShipyard::GetPhaseMask(Player* /*p_Owner*/)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonHordeShipyard::CanUseGarrisonCache(Player* /*p_Owner*/)
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    /// @p_Owner                : Garrison owner
    /// @p_CurrentGarrisonLevel : Current garrison level
    bool InstanceScript_GarrisonHordeShipyard::CanUpgrade(Player* /*p_Owner*/, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 3)
            return false;

        return false;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonHordeShipyard::OnUpgrade(Player* /*p_Owner*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonHordeShipyard::OnMissionStart(Player* /*p_Owner*/, uint32 /*p_MissionID*/, std::vector<uint32> /*p_Followers*/)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonHordeShipyard::OnPrePurchaseBuilding(Player* /*p_Owner*/, uint32 /*p_BuildingID*/, uint32 p_BaseTime)
    {
        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeShipyard::OnPurchaseBuilding(Player* /*p_Owner*/, uint32 /*p_BuildingID*/)
    {

    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeShipyard::OnBuildingActivated(Player* /*p_Owner*/, uint32 /*p_BuildingID*/)
    {

    }
    /// When a building from the same type with higher level is purchased
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeShipyard::OnUpgradeBuilding(Player* /*p_Owner*/, uint32 /*p_BuildingID*/)
    {

    }

    void InstanceScript_GarrisonHordeShipyard::OnDeleteBuilding(Player* /*p_Owner*/, uint32 /*p_BuildingID*/, uint32 /*p_BuildingType*/, bool /*p_RemoveForUpgrade*/)
    {

    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

#ifndef __clang_analyzer__
void AddSC_Garrison_HS()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonHordeShipyard();
}
#endif
