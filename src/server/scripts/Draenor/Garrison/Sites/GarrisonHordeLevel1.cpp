////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "GarrisonHordeLevel1.hpp"
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
        PhaseBuildYourBarracks        = 0x00000002,
        PhaseMagePortalFrostfireRidge = 0x00000010,
        PhaseMagePortalSpiresOfArak   = 0x00000020,
        PhaseMagePortalTalador        = 0x00000040,
        PhaseMagePortalNagrand        = 0x00000080,
        PhaseMagePortalShadowmoon     = 0x00000100,
        PhaseMagePortalGorgrond       = 0x00000200,
        PhaseLostInTransitionQuest    = 0x00000400
    };

    /// Constructor
    InstanceMapScript_GarrisonHordeLevel1::InstanceMapScript_GarrisonHordeLevel1()
        : InstanceMapScript("instance_Garrison_H1", MapIDs::MapGarrisonHordeLevel1)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript* InstanceMapScript_GarrisonHordeLevel1::GetInstanceScript(InstanceMap* p_Map) const
    {
        return new InstanceScript_GarrisonHordeLevel1(p_Map);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonHordeLevel1::InstanceScript_GarrisonHordeLevel1(Map* p_Map)
        : GarrisonSiteBase(p_Map)
    {
        SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED, 0);
    }
    /// Destructor
    InstanceScript_GarrisonHordeLevel1::~InstanceScript_GarrisonHordeLevel1()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonHordeLevel1::Update(uint32 /*p_Diff*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonHordeLevel1::OnOwnerEnter(Player* p_Owner)
    {
        if (p_Owner->GetQuestStatus(Quests::QUEST_WHAT_WE_GOT) == QUEST_STATUS_REWARDED || p_Owner->GetQuestObjectiveCounter(273085) == 1)
        {
            SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED, 1);

            if (!m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].empty())
            {
                for (uint64 l_DynamicPeon : m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC])
                {
                    Creature* l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].begin()));

                    if (l_Creature && l_Creature->AI())
                        l_Creature->AI()->SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING);
                }
            }
        }

        if (p_Owner->GetQuestObjectiveCounter(275373) == 1 && !m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].empty())
        {
            Creature* l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].begin()));

            if (l_Creature && l_Creature->GetDistance2d(5593.0f, 4476.0f) > 2)
            {
                l_Creature->SetWalk(false);
                l_Creature->LoadPath(Waypoints::MOVE_PATH_SENIOR_PEON_II);
                l_Creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                l_Creature->GetMotionMaster()->Initialize();
            }
        }

        /// Build your Barracks quest
        if (p_Owner->HasQuest(Quests::Horde_BuildYourBarracks))
        {
            Manager* l_GarrisonMgr = p_Owner->GetGarrison();

            if (l_GarrisonMgr == nullptr)
                return;

            if (p_Owner->GetGarrison()->GetBuildingWithType(Building::Type::Barracks).BuildingID)
            {
                p_Owner->QuestObjectiveSatisfy(36167, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Start Construction
                p_Owner->QuestObjectiveSatisfy(39015, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Find Blueprint
                p_Owner->QuestObjectiveSatisfy(39012, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Learn Blueprint
                p_Owner->QuestObjectiveSatisfy(35753, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID()); ///< Plot Finalize
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonHordeLevel1::OnQuestStarted(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {
    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonHordeLevel1::OnQuestReward(Player* /*p_Owner*/, const Quest* /*p_Quest*/)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonHordeLevel1::OnQuestAbandon(Player* /*p_Owner*/, const Quest* p_Quest)
    {
        if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_WHAT_WE_GOT)
        {
            if (!m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].empty())
            {
                Creature * l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].begin()));

                if (l_Creature && l_Creature->GetDistance2d(5593.0f, 4476.0f) < 2)
                {
                    l_Creature->SetWalk(false);
                    l_Creature->LoadPath(Waypoints::MOVE_PATH_SENIOR_PEON_II_BACK);
                    l_Creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                    l_Creature->GetMotionMaster()->Initialize();
                }
            }

            SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED, 0);

            if (!m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].empty())
            {
                for (uint64 l_DynamicPeon : m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC])
                {
                    Creature * l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].begin()));

                    if (l_Creature && l_Creature->AI())
                        l_Creature->AI()->SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_BACK_TO_HOME);
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonHordeLevel1::GetPhaseMask(Player* p_Owner)
    {
        uint32 l_PhaseMask = GarrisonPhases::GarrisonPhaseBase;
        Manager* l_GarrisonMgr = p_Owner->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return 0;

        if (p_Owner->HasQuest(Quests::Horde_BuildYourBarracks))
            l_PhaseMask |= GarrisonPhases::PhaseBuildYourBarracks;

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
    bool InstanceScript_GarrisonHordeLevel1::CanUseGarrisonCache(Player* p_Owner)
    {
        /// Horde
        if (p_Owner->GetQuestStatus(Quests::QUEST_WHAT_WE_GOT) == QUEST_STATUS_REWARDED
            || p_Owner->HasQuest(Quests::QUEST_WHAT_WE_GOT))
            return true;

        /// [FACTION CHANGE CASE] Alliance
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
    bool InstanceScript_GarrisonHordeLevel1::CanUpgrade(Player* p_Owner, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 1)
            return false;

        if (p_Owner->getLevel() < 93)
            return false;

        if (!p_Owner->HasQuest(Quests::Horde_BiggerIsBetter))
            return false;

        return true;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonHordeLevel1::OnUpgrade(Player* p_Owner)
    {
        GarrSiteLevelEntry const* l_Entry = p_Owner->GetGarrison()->GetGarrisonSiteLevelEntry();

        if (!l_Entry)
            return;

        p_Owner->AddMovieDelayedTeleport(l_Entry->MovieID, l_Entry->MapID, 5754.82f, 4495.425f, 132.50f, 2.90f);
        p_Owner->SendMovieStart(l_Entry->MovieID);
        p_Owner->CompleteQuest(Quests::Horde_BiggerIsBetter);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonHordeLevel1::OnMissionStart(Player* /*p_Owner*/, uint32 /*p_MissionID*/, std::vector<uint32> /*p_Followers*/)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonHordeLevel1::OnPrePurchaseBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BaseTime)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Building::ID::Barracks_Barracks_Level1 && p_Owner->HasQuest(Quests::Horde_BuildYourBarracks))
            return 2;   ///< 2 second, unk retail value

        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel1::OnPurchaseBuilding(Player* p_Owner, uint32 p_BuildingID)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Building::ID::Barracks_Barracks_Level1 && p_Owner->HasQuest(Quests::Horde_BuildYourBarracks))
        {
            p_Owner->QuestObjectiveSatisfy(36167, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID());

            if (!m_CreaturesPerEntry[NPCs::NPC_GASLOWE].empty())
            {
                Creature* l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_GASLOWE].begin()));

                if (l_Creature && l_Creature->AI())
                {
                    l_Creature->AI()->Talk(0);

                    l_Creature->SetWalk(false);
                    l_Creature->GetMotionMaster()->MovePoint(MovePointIDs::MOVE_POINT_GAZLOWE_BARRACK_A, 5590.83f, 4481.74f, 129.92f);

                    p_Owner->QuestObjectiveSatisfy(NPCs::NPC_GASLOWE, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Owner->GetGUID());
                }
            }
        }
        else if (p_BuildingID == Building::ID::TradingPost_TradingPost_Level2)
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
    void InstanceScript_GarrisonHordeLevel1::OnBuildingActivated(Player* p_Owner, uint32 p_BuildingID)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Building::ID::Barracks_Barracks_Level1 && p_Owner->HasQuest(Quests::Horde_BuildYourBarracks))
        {
            p_Owner->QuestObjectiveSatisfy(35753, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_Owner->GetGUID());
        }

        if (MS::Garrison::Manager* l_GarrisonMgr = p_Owner->GetGarrison())
        {
            uint8 l_BuildingLevel = l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuilding(p_BuildingID));

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
    /// When a building from the same type with higher level is purchased
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel1::OnUpgradeBuilding(Player* /*p_Owner*/, uint32 /*p_BuildingID*/)
    {
    }

    void InstanceScript_GarrisonHordeLevel1::OnDeleteBuilding(Player* p_Owner, uint32 /*p_BuildingID*/, uint32 p_BuildingType, bool p_RemoveForUpgrade)
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
void AddSC_Garrison_H1()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonHordeLevel1();
}
#endif
#endif
