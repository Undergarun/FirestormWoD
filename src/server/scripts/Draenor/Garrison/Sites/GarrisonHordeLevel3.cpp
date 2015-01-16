////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonHordeLevel3.hpp"
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
    InstanceMapScript_GarrisonHordeLevel3::InstanceMapScript_GarrisonHordeLevel3()
        : InstanceMapScript("instance_Garrison_H3", MapIDs::MAP_GARRISON_HORDE_LEVEL_3)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript * InstanceMapScript_GarrisonHordeLevel3::GetInstanceScript(InstanceMap * p_Map) const
    {
        return new InstanceScript_GarrisonHordeLevel3(p_Map);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonHordeLevel3::InstanceScript_GarrisonHordeLevel3(Map* p_Map)
        : GarrisonSiteBase(p_Map)
    {

    }
    /// Destructor
    InstanceScript_GarrisonHordeLevel3::~InstanceScript_GarrisonHordeLevel3()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonHordeLevel3::Update(uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonHordeLevel3::OnOwnerEnter(Player * p_Owner)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonHordeLevel3::OnQuestStarted(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonHordeLevel3::OnQuestReward(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonHordeLevel3::OnQuestAbandon(Player * p_Owner, const Quest * p_Quest)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonHordeLevel3::GetPhaseMask(Player * p_Owner)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonHordeLevel3::CanUseGarrisonCache(Player * p_Owner)
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonHordeLevel3::OnMissionStart(uint32 p_MissionID, std::vector<uint32> p_Followers)
    {

    }
    /// When a construction start
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel3::OnPurchaseBuilding(uint32 p_BuildingID)
    {

    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_H3()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonHordeLevel3();
}