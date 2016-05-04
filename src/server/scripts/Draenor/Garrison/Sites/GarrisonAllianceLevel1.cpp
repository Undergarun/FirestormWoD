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
#include "../GarrisonScriptData.hpp"

namespace MS { namespace Garrison { namespace Sites
{
    enum GarrisonPhases
    {
        GarrisonPhaseBase             = 0x00000001,
        GarrisonPhaseCompagnon        = 0x00000002,
        GarrisonKeepingItTogether     = 0x00000004,
        GarrisonQiannaMoonshadow      = 0x00000008,
        PhaseMagePortalFrostfireRidge = 0x00000010,
        PhaseMagePortalSpiresOfArak   = 0x00000020,
        PhaseMagePortalTalador        = 0x00000040,
        PhaseMagePortalNagrand        = 0x00000080,
        PhaseMagePortalShadowmoon     = 0x00000100,
        PhaseMagePortalGorgrond       = 0x00000200,
        PhaseLostInTransitionQuest    = 0x00000400
    };

    /// Constructor
    InstanceMapScript_GarrisonAllianceLevel1::InstanceMapScript_GarrisonAllianceLevel1()
        : InstanceMapScript("instance_Garrison_A1", MapIDs::MapGarrisonAllianceLevel1)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript* InstanceMapScript_GarrisonAllianceLevel1::GetInstanceScript(InstanceMap* p_Map) const
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
    void InstanceScript_GarrisonAllianceLevel1::OnOwnerEnter(Player* p_Owner)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonAllianceLevel1::OnQuestStarted(Player* p_Owner, const Quest* p_Quest)
    {
    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonAllianceLevel1::OnQuestReward(Player* p_Owner, const Quest* p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonAllianceLevel1::OnQuestAbandon(Player* p_Owner, const Quest* p_Quest)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonAllianceLevel1::GetPhaseMask(Player* p_Owner)
    {
        uint32 l_PhaseMask = GarrisonPhases::GarrisonPhaseBase;
        Manager* l_GarrisonMgr = p_Owner->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return 0;

        if (p_Owner->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_A) == QUEST_STATUS_REWARDED)
            l_PhaseMask |= GarrisonPhases::GarrisonPhaseCompagnon;

        if (p_Owner->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER))
            l_PhaseMask |= GarrisonPhases::GarrisonKeepingItTogether;

        if (p_Owner->HasQuest(Quests::Alliance_LostInTransition) || p_Owner->HasQuest(Quests::Horde_LostInTransition))
            l_PhaseMask |= GarrisonPhases::PhaseLostInTransitionQuest;

        if (p_Owner->GetQuestStatus(Quests::Alliance_QianaMoonshadow) == QUEST_STATUS_COMPLETE || p_Owner->IsQuestRewarded(Quests::Alliance_QianaMoonshadow))
            l_PhaseMask |= GarrisonQiannaMoonshadow;

        if (l_GarrisonMgr->HasBuildingType(BuildingType::MageTower))
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
    bool InstanceScript_GarrisonAllianceLevel1::CanUseGarrisonCache(Player* p_Owner)
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
    bool InstanceScript_GarrisonAllianceLevel1::CanUpgrade(Player* p_Owner, uint32 p_CurrentGarrisonLevel)
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
    void InstanceScript_GarrisonAllianceLevel1::OnUpgrade(Player* p_Owner)
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
    void InstanceScript_GarrisonAllianceLevel1::OnMissionStart(Player* p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonAllianceLevel1::OnPrePurchaseBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BaseTime)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Buildings::Barracks_Barracks_Level1 && p_Owner->HasQuest(Quests::Alliance_BuildYourBarracks))
            return 2;   ///< 2 second, unk retail value

        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel1::OnPurchaseBuilding(Player* p_Owner, uint32 p_BuildingID)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Buildings::Barracks_Barracks_Level1 && p_Owner->HasQuest(Quests::Alliance_BuildYourBarracks))
            p_Owner->CompleteQuest(Quests::Alliance_BuildYourBarracks);
    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel1::OnBuildingActivated(Player* p_Owner, uint32 p_BuildingID)
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
                case Buildings::Barn_Barn_Level2:
                    if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemIronTrap))
                        p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                    break;
                case Buildings::Barn_Barn_Level3:
                    if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemIronTrap))
                        p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                    if (Item* l_Item = p_Owner->GetItemByEntry(Items::ItemImprovedIronTrap))
                        p_Owner->RemoveItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
                    break;
                case Buildings::TradingPost_TradingPost_Level2:
                {
                    uint32 l_FactionID = p_Owner->GetTeamId() == TEAM_ALLIANCE ? 1710 : 1708;
                    FactionEntry const* l_Entry = sFactionStore.LookupEntry(l_FactionID);

                    if (l_Entry != nullptr)
                        p_Owner->GetReputationMgr().SetReputation(l_Entry, 0);

                    break;
                }
                default:
                    break;
            }
        }
    }
    /// When a building from the same type with higher level is purchased
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonAllianceLevel1::OnUpgradeBuilding(Player* p_Owner, uint32 p_BuildingID)
    {

    }

    void InstanceScript_GarrisonAllianceLevel1::OnDeleteBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BuildingType, bool p_RemoveForUpgrade)
    {
        if (p_RemoveForUpgrade)
            return;

        switch (p_BuildingType)
        {
            case BuildingType::Type::LumberMill:
                p_Owner->SetSkill(SkillType::SKILL_LOGGING, 0, 0, 0);
                break;
            case BuildingType::Type::Barn:
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

void AddSC_Garrison_A1()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonAllianceLevel1();
}