////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonAllianceLevel2.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"

enum
{
    GARRISON_PHASE_BASE = 0x0001,
};

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    InstanceMapScript_GarrisonAllianceLevel2::InstanceMapScript_GarrisonAllianceLevel2()
        : InstanceMapScript("instance_Garrison_A2", MapIDs::MAP_GARRISON_ALLIANCE_LEVEL_2)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript * InstanceMapScript_GarrisonAllianceLevel2::GetInstanceScript(InstanceMap * p_Map) const
    {
        return new InstanceScript_GarrisonAllianceLevel2(p_Map);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonAllianceLevel2::InstanceScript_GarrisonAllianceLevel2(Map* p_Map)
        : GarrisonSiteBase(p_Map)
    {

    }
    /// Destructor
    InstanceScript_GarrisonAllianceLevel2::~InstanceScript_GarrisonAllianceLevel2()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonAllianceLevel2::Update(uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonAllianceLevel2::OnOwnerEnter(Player * p_Owner)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonAllianceLevel2::OnQuestStarted(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonAllianceLevel2::OnQuestReward(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonAllianceLevel2::OnQuestAbandon(Player * p_Owner, const Quest * p_Quest)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonAllianceLevel2::GetPhaseMask(Player * p_Owner)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonAllianceLevel2::CanUseGarrisonCache(Player * p_Owner)
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonAllianceLevel2::OnMissionStart(uint32 p_MissionID, std::vector<uint32> p_Followers)
    {

    }
    /// When a construction start
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel2::OnPurchaseBuilding(uint32 p_BuildingID)
    {

    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_A2()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonAllianceLevel2();
}