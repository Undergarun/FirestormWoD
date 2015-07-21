////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonAllianceLevel1.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"

enum
{
    GARRISON_PHASE_BASE             = 0x0001,
    GARRISON_PHASE_COMPAGNION       = 0x0002,
    GARRISON_KEEPING_IT_TOGETHER    = 0x0004,
};

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    InstanceMapScript_GarrisonAllianceLevel1::InstanceMapScript_GarrisonAllianceLevel1()
        : InstanceMapScript("instance_Garrison_A1", MapIDs::MAP_GARRISON_ALLIANCE_LEVEL_1)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript * InstanceMapScript_GarrisonAllianceLevel1::GetInstanceScript(InstanceMap * p_Map) const
    {
        return new InstanceScript_GarrisonAllianceLevel1(p_Map);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonAllianceLevel1::InstanceScript_GarrisonAllianceLevel1(Map* p_Map)
        : GarrisonSiteBase(p_Map)
    {

    }
    /// Destructor
    InstanceScript_GarrisonAllianceLevel1::~InstanceScript_GarrisonAllianceLevel1()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonAllianceLevel1::Update(uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonAllianceLevel1::OnOwnerEnter(Player * p_Owner)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonAllianceLevel1::OnQuestStarted(Player * p_Owner, const Quest * p_Quest)
    {
        /// Hack fix for storehouse, need more work
        if (p_Owner && p_Quest && p_Quest->GetQuestId() == Quests::Alliance_LostInTransition)
            p_Owner->CompleteQuest(Quests::Alliance_LostInTransition);
    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonAllianceLevel1::OnQuestReward(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonAllianceLevel1::OnQuestAbandon(Player * p_Owner, const Quest * p_Quest)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonAllianceLevel1::GetPhaseMask(Player * p_Owner)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        if (p_Owner->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_A) == QUEST_STATUS_REWARDED)
            l_PhaseMask |= GARRISON_PHASE_COMPAGNION;

        if (p_Owner->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER))
            l_PhaseMask |= GARRISON_KEEPING_IT_TOGETHER;

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonAllianceLevel1::CanUseGarrisonCache(Player * p_Owner)
    {
        if (p_Owner->GetQuestStatus(Quests::QUEST_KEEPING_IT_TOGETHER) == QUEST_STATUS_REWARDED
            || p_Owner->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER))
            return true;

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    /// @p_Owner                : Garrison owner
    /// @p_CurrentGarrisonLevel : Current garrison level
    bool InstanceScript_GarrisonAllianceLevel1::CanUpgrade(Player * p_Owner, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 1)
            return false;

        if (p_Owner->getLevel() < 93)
            return false;

        if (!p_Owner->HasQuest(Quests::Alliance_BiggerIsBetter))
            return false;

        return true;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonAllianceLevel1::OnUpgrade(Player * p_Owner)
    {
        GarrSiteLevelEntry const* l_Entry = p_Owner->GetGarrison()->GetGarrisonSiteLevelEntry();

        if (!l_Entry)
            return;

        p_Owner->AddMovieDelayedTeleport(l_Entry->MovieID, l_Entry->MapID, 1759.94f, 184.86f, 71.50f, 0.57f);
        p_Owner->SendMovieStart(l_Entry->MovieID);
        p_Owner->CompleteQuest(Quests::Alliance_BiggerIsBetter);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonAllianceLevel1::OnMissionStart(Player * p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonAllianceLevel1::OnPrePurchaseBuilding(Player * p_Owner, uint32 p_BuildingID, uint32 p_BaseTime)
    {
        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel1::OnPurchaseBuilding(Player * p_Owner, uint32 p_BuildingID)
    {

    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel1::OnBuildingActivated(Player * p_Owner, uint32 p_BuildingID)
    {

    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_A1()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonAllianceLevel1();
}