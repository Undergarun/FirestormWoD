////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonHordeLevel2.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "../GarrisonScriptData.hpp"

enum
{
    GARRISON_PHASE_BASE = 0x0001,
};

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    InstanceMapScript_GarrisonHordeLevel2::InstanceMapScript_GarrisonHordeLevel2()
        : InstanceMapScript("instance_Garrison_H2", MapIDs::MapGarrisonHordeLevel2)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript* InstanceMapScript_GarrisonHordeLevel2::GetInstanceScript(InstanceMap* p_Map) const
    {
        return new InstanceScript_GarrisonHordeLevel2(p_Map);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonHordeLevel2::InstanceScript_GarrisonHordeLevel2(Map* p_Map)
        : GarrisonSiteBase(p_Map)
    {

    }
    /// Destructor
    InstanceScript_GarrisonHordeLevel2::~InstanceScript_GarrisonHordeLevel2()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonHordeLevel2::Update(uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonHordeLevel2::OnOwnerEnter(Player* p_Owner)
    {
        /// Achievement "More Plots" horde side
        if (p_Owner->GetTeamId() == TEAM_HORDE && !p_Owner->GetAchievementMgr().HasAchieved(9545))
            p_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(9545), nullptr);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonHordeLevel2::OnQuestStarted(Player* p_Owner, const Quest* p_Quest)
    {

    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonHordeLevel2::OnQuestReward(Player* p_Owner, const Quest* p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonHordeLevel2::OnQuestAbandon(Player* p_Owner, const Quest* p_Quest)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonHordeLevel2::GetPhaseMask(Player* p_Owner)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonHordeLevel2::CanUseGarrisonCache(Player* p_Owner)
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    /// @p_Owner                : Garrison owner
    /// @p_CurrentGarrisonLevel : Current garrison level
    bool InstanceScript_GarrisonHordeLevel2::CanUpgrade(Player* p_Owner, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 2)
            return false;

        return false;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonHordeLevel2::OnUpgrade(Player* p_Owner)
    {
        GarrSiteLevelEntry const* l_Entry = p_Owner->GetGarrison()->GetGarrisonSiteLevelEntry();

        if (!l_Entry)
            return;

        p_Owner->AddMovieDelayedTeleport(l_Entry->MovieID, l_Entry->MapID, 5622.5063f, 4465.5161f, 130.1637f, 0.0f);
        p_Owner->SendMovieStart(l_Entry->MovieID);
    }
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonHordeLevel2::OnMissionStart(Player* p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers)
    {
        if (p_Owner->HasQuest(Quests::Horde_MissionProbable))
        {
            if (std::find(p_Followers.begin(), p_Followers.end(), 34) != p_Followers.end())
                p_Owner->QuestObjectiveSatisfy(35706, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
        }
    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonHordeLevel2::OnPrePurchaseBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BaseTime)
    {
        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel2::OnPurchaseBuilding(Player* p_Owner, uint32 p_BuildingID)
    {

    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel2::OnBuildingActivated(Player* p_Owner, uint32 p_BuildingID)
    {
        if (MS::Garrison::Manager* l_GarrisonMgr = p_Owner->GetGarrison())
        {
            uint8 l_BuildingLevel = l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuilding(p_BuildingID));

            switch (p_BuildingID)
            {
                case Buildings::LumberMill_LumberMill_Level1:
                case Buildings::LumberMill_LumberMill_Level2:
                case Buildings::LumberMill_LumberMill_Level3:
                    p_Owner->SetSkill(SkillType::SKILL_LOGGING, l_BuildingLevel, l_BuildingLevel, 75);
                    break;
                default:
                    break;
            }
        }
    }
    /// When a building from the same type with higher level is purchased
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel2::OnUpgradeBuilding(Player* p_Owner, uint32 p_BuildingID)
    {
    }

    void InstanceScript_GarrisonHordeLevel2::OnDeleteBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BuildingType, bool p_RemoveForUpgrade)
    {
        if (p_RemoveForUpgrade)
            return;

        switch (p_BuildingType)
        {
            case BuildingType::Type::LumberMill:
                p_Owner->SetSkill(SkillType::SKILL_LOGGING, 0, 0, 0);
                break;
            default:
                break;
        }
    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_H2()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonHordeLevel2();
}