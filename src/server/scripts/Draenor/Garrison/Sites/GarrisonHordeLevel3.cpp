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
#include "../GarrisonScriptData.hpp"

enum
{
    GARRISON_PHASE_BASE = 0x0001,
};

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    InstanceMapScript_GarrisonHordeLevel3::InstanceMapScript_GarrisonHordeLevel3()
        : InstanceMapScript("instance_Garrison_H3", MapIDs::MapGarrisonHordeLevel3)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript* InstanceMapScript_GarrisonHordeLevel3::GetInstanceScript(InstanceMap* p_Map) const
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
    void InstanceScript_GarrisonHordeLevel3::OnOwnerEnter(Player* p_Owner)
    {
        /// Achievement "Even More Plots" horde side
        if (p_Owner->GetTeamId() == TEAM_HORDE && !p_Owner->GetAchievementMgr().HasAchieved(9546))
            p_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(9546), nullptr);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonHordeLevel3::OnQuestStarted(Player* p_Owner, const Quest* p_Quest)
    {

    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonHordeLevel3::OnQuestReward(Player* p_Owner, const Quest* p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonHordeLevel3::OnQuestAbandon(Player* p_Owner, const Quest* p_Quest)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonHordeLevel3::GetPhaseMask(Player* p_Owner)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        Manager* l_GarrisonMgr = p_Owner->GetGarrison();

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
    bool InstanceScript_GarrisonHordeLevel3::CanUseGarrisonCache(Player* p_Owner)
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    /// @p_Owner                : Garrison owner
    /// @p_CurrentGarrisonLevel : Current garrison level
    bool InstanceScript_GarrisonHordeLevel3::CanUpgrade(Player* p_Owner, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 3)
            return false;

        return false;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonHordeLevel3::OnUpgrade(Player* p_Owner)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonHordeLevel3::OnMissionStart(Player* p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonHordeLevel3::OnPrePurchaseBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BaseTime)
    {
        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel3::OnPurchaseBuilding(Player* p_Owner, uint32 p_BuildingID)
    {
        if (p_BuildingID == Buildings::TradingPost_TradingPost_Level2)
        {
            uint32 l_FactionID = p_Owner->GetTeamId() == TEAM_ALLIANCE ? 1710 : 1708;
            FactionEntry const* l_Entry = sFactionStore.LookupEntry(l_FactionID);

            if (l_Entry != nullptr)
                p_Owner->GetReputationMgr().SetReputation(l_Entry, 0);
        }
    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel3::OnBuildingActivated(Player* p_Owner, uint32 p_BuildingID)
    {
        if (MS::Garrison::Manager* l_GarrisonMgr = p_Owner->GetGarrison())
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

            if (l_BuildingEntry)
            {
                uint8 l_BuildingLevel = l_BuildingEntry->Level;

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
                    case Buildings::GemBoutique_GemBoutique_Level3:
                    {
                        for (uint32 l_I = 0; l_I < sGarrMissionStore.GetNumRows(); ++l_I)
                        {
                            GarrMissionEntry const* l_Entry = sGarrMissionStore.LookupEntry(l_I);

                            if (!l_Entry)
                                continue;

                            if (l_Entry->MissionType == MissionType::JewelCrafting)
                                l_GarrisonMgr->AddMission(l_Entry->MissionRecID);
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
    void InstanceScript_GarrisonHordeLevel3::OnUpgradeBuilding(Player* p_Owner, uint32 p_BuildingID)
    {
        if (MS::Garrison::Manager* l_GarrisonMgr = p_Owner->GetGarrison())
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

            if (l_BuildingEntry)
            {
                uint8 l_BuildingLevel = l_BuildingEntry->Level;

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
                    default:
                        break;
                }
            }
        }
    }

    void InstanceScript_GarrisonHordeLevel3::OnDeleteBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BuildingType, bool p_RemoveForUpgrade)
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

void AddSC_Garrison_H3()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonHordeLevel3();
}