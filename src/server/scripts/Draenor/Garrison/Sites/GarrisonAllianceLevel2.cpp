////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "GarrisonAllianceLevel2.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonNPCAI.hpp"
#include "../GarrisonScriptData.hpp"

namespace MS { namespace Garrison { namespace Sites
{
    enum GarrisonPhases
    {
        GarrisonPhaseBase             = 0x00000001,
        PhaseMagePortalFrostfireRidge = 0x00000010,
        PhaseMagePortalSpiresOfArak   = 0x00000020,
        PhaseMagePortalTalador        = 0x00000040,
        PhaseMagePortalNagrand        = 0x00000080,
        PhaseMagePortalShadowmoon     = 0x00000100,
        PhaseMagePortalGorgrond       = 0x00000200,
        PhaseLostInTransitionQuest    = 0x00000400
    };

    /// Constructor
    InstanceMapScript_GarrisonAllianceLevel2::InstanceMapScript_GarrisonAllianceLevel2()
        : InstanceMapScript("instance_Garrison_A2", MapIDs::MapGarrisonAllianceLevel2)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript* InstanceMapScript_GarrisonAllianceLevel2::GetInstanceScript(InstanceMap* p_Map) const
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
    void InstanceScript_GarrisonAllianceLevel2::Update(uint32 /*p_Diff*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonAllianceLevel2::OnOwnerEnter(Player* p_Owner)
    {
        /// Achievement "More Plots" alliance side
        if (p_Owner->GetTeamId() == TEAM_ALLIANCE && !p_Owner->GetAchievementMgr().HasAchieved(9100))
            p_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(9100), nullptr);

        /// Build your Barracks quest
        if (p_Owner->GetQuestStatus(Quests::Alliance_BuildYourBarracks) != QUEST_STATUS_REWARDED)
        {
            Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Alliance_BuildYourBarracks);
            Manager* l_GarrisonMgr = p_Owner->GetGarrison();

            if (l_GarrisonMgr == nullptr || l_Quest == nullptr)
                return;

            if (p_Owner->GetQuestStatus(Quests::Alliance_BuildYourBarracks) == QUEST_STATUS_NONE)
                p_Owner->AddQuest(l_Quest, p_Owner);

            if (p_Owner->GetGarrison()->GetBuildingWithType(Building::Type::Barracks).BuildingID)
            {
                p_Owner->QuestObjectiveSatisfy(36175, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Start Construction
                p_Owner->QuestObjectiveSatisfy(39422, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Find Blueprint
                p_Owner->QuestObjectiveSatisfy(36163, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Learn Blueprint
                p_Owner->QuestObjectiveSatisfy(36173, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Plot Finalize

                p_Owner->CompleteQuest(Quests::Alliance_BuildYourBarracks);
            }

            p_Owner->CompleteQuest(Quests::Alliance_BuildYourBarracks);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonAllianceLevel2::OnQuestStarted(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {
    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonAllianceLevel2::OnQuestReward(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonAllianceLevel2::OnQuestAbandon(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonAllianceLevel2::GetPhaseMask(Player* p_Owner)
    {
        uint32 l_PhaseMask = GarrisonPhases::GarrisonPhaseBase;
        Manager* l_GarrisonMgr = p_Owner->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return 0;

        if (p_Owner->HasQuest(Quests::Alliance_LostInTransition) || p_Owner->HasQuest(Quests::Horde_LostInTransition))
            l_PhaseMask |= GarrisonPhases::PhaseLostInTransitionQuest;

        if (l_GarrisonMgr->HasBuildingType(Building::Type::MageTower))
        {
            if (p_Owner->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestFrostfireRidge))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalFrostfireRidge;
            if (p_Owner->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestGorgrond))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalGorgrond;
            if (p_Owner->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestNagrand))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalNagrand;
            if (p_Owner->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestShadowmoon))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalShadowmoon;
            if (p_Owner->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestSpiresOfArak))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalSpiresOfArak;
            if (p_Owner->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestTalador))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalTalador;
        }

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonAllianceLevel2::CanUseGarrisonCache(Player* /*p_Owner*/)
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    /// @p_Owner                : Garrison owner
    /// @p_CurrentGarrisonLevel : Current garrison level
    bool InstanceScript_GarrisonAllianceLevel2::CanUpgrade(Player* /*p_Owner*/, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 2)
            return false;

        return false;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonAllianceLevel2::OnUpgrade(Player* p_Owner)
    {
        GarrSiteLevelEntry const* l_Entry = p_Owner->GetGarrison()->GetGarrisonSiteLevelEntry();

        if (!l_Entry)
            return;

        p_Owner->AddMovieDelayedTeleport(l_Entry->MovieID, l_Entry->MapID, 1759.94f, 184.86f, 71.50f, 0.57f);
        p_Owner->SendMovieStart(l_Entry->MovieID);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonAllianceLevel2::OnMissionStart(Player* /*p_Owner*/, uint32 /*p_MissionID*/, std::vector<uint32> /*p_Followers*/)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonAllianceLevel2::OnPrePurchaseBuilding(Player* /*p_Owner*/, uint32 /*p_BuildingID*/, uint32 p_BaseTime)
    {
        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel2::OnPurchaseBuilding(Player* /*p_Owner*/, uint32 /*p_BuildingID*/)
    {
    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel2::OnBuildingActivated(Player* p_Owner, uint32 p_BuildingID)
    {
        if (MS::Garrison::Manager* l_GarrisonMgr = p_Owner->GetGarrison())
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

            if (l_BuildingEntry)
            {
                uint8 l_BuildingLevel = l_BuildingEntry->Level;

                switch (p_BuildingID)
                {
                    case Building::ID::LumberMill_LumberMill_Level1:
                    case Building::ID::LumberMill_LumberMill_Level2:
                    case Building::ID::LumberMill_LumberMill_Level3:
                        p_Owner->SetSkill(SkillType::SKILL_LOGGING, l_BuildingLevel, l_BuildingLevel, 75);
                        break;
                    case Building::ID::Barn_Barn_Level2:
                        if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemIronTrap))
                            p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                        break;
                    case Building::ID::Barn_Barn_Level3:
                        if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemIronTrap))
                            p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                        if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemImprovedIronTrap))
                            p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                        break;
                    case Building::ID::TradingPost_TradingPost_Level2:
                    {
                        uint32 l_FactionID = p_Owner->GetTeamId() == TEAM_ALLIANCE ? 1710 : 1708;
                        FactionEntry const* l_Entry = sFactionStore.LookupEntry(l_FactionID);

                        if (l_Entry != nullptr)
                            p_Owner->GetReputationMgr().SetReputation(l_Entry, 0);

                        break;
                    }
                    case Building::ID::LunarfallExcavation_FrostwallMines_Level1:
                    case Building::ID::LunarfallExcavation_FrostwallMines_Level2:
                    case Building::ID::LunarfallExcavation_FrostwallMines_Level3:
                    {
                        std::vector<uint64> l_CreatureGuids = l_GarrisonMgr->GetBuildingCreaturesByBuildingType(Building::Type::Inn);

                        for (std::vector<uint64>::iterator l_Itr = l_CreatureGuids.begin(); l_Itr != l_CreatureGuids.end(); l_Itr++)
                        {
                            if (Creature* l_Creature = sObjectAccessor->GetCreature(*p_Owner, *l_Itr))
                            {
                                if (GarrisonNPCAI* l_AI = l_Creature->ToGarrisonNPCAI())
                                    l_AI->OnDailyDataReset();
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
    /// When a building from the same type with higher level is purchased
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel2::OnUpgradeBuilding(Player* p_Owner, uint32 p_BuildingID)
    {
        if (MS::Garrison::Manager* l_GarrisonMgr = p_Owner->GetGarrison())
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

            if (l_BuildingEntry)
            {
                uint8 l_BuildingLevel = l_BuildingEntry->Level;

                switch (p_BuildingID)
                {
                    case Building::ID::LumberMill_LumberMill_Level1:
                    case Building::ID::LumberMill_LumberMill_Level2:
                    case Building::ID::LumberMill_LumberMill_Level3:
                        p_Owner->SetSkill(SkillType::SKILL_LOGGING, l_BuildingLevel, l_BuildingLevel, 75);
                        break;
                    case Building::ID::Barn_Barn_Level2:
                        if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemIronTrap))
                            p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                        break;
                    case Building::ID::Barn_Barn_Level3:
                        if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemIronTrap))
                            p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                        if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemImprovedIronTrap))
                            p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void InstanceScript_GarrisonAllianceLevel2::OnDeleteBuilding(Player* p_Owner, uint32 /*p_BuildingID*/, uint32 p_BuildingType, bool p_RemoveForUpgrade)
    {
        if (p_RemoveForUpgrade)
            return;

        switch (p_BuildingType)
        {
            case Building::Type::LumberMill:
                p_Owner->SetSkill(SkillType::SKILL_LOGGING, 0, 0, 0);
                break;
            case Building::Type::Barn:
                if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemIronTrap))
                    p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemImprovedIronTrap))
                    p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemDeadlyIronTrap))
                    p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                break;
            default:
                break;
        }
    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

#ifndef __clang_analyzer__
void AddSC_Garrison_A2()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonAllianceLevel2();
}
#endif
#endif
