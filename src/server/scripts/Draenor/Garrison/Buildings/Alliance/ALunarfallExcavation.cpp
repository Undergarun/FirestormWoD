////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "../Horde/HFrostwallMines.hpp"
#include "ALunarfallExcavation.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "../../GarrisonScriptData.hpp"
#include "../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    std::vector<uint32> g_AllyDepositsEntry
    {
        232542, ///< GameObjects::GobBlackrockDeposit
        232543, ///< GameObjects::GobRichBlackrockDeposit
        232544, ///< GameObjects::GobTrueIronDeposit
        232545  ///< GameObjects::GobRichTrueIronDeposit
    };

    std::vector<GatheringPlotInfos> g_AllyMineDeposits
    {
        /// Level 1
        { 1, -21.2715f, -13.8611f, -59.2858f, 2.9218f }, //
        { 1, 38.9916f, 18.4105f, -43.6418f, 4.1906f },//
        { 1, -30.3754f, -2.0047f, -55.6653f, 4.4898f },//
        { 1, 54.8163f, -86.7227f, -22.4267f, 4.8856f },//
        { 1, -15.7798f, -82.9398f, 16.0685f, 1.2571f },//
        { 1, 5.5319f, -103.7457f, 1.8457f, 1.5045f },//
        { 1, 43.0758f, -75.4841f, -24.3843f, 2.3300f },//
        { 1, -5.3802f, 5.2938f, -32.1254f, 0.8919f }//
        /// Level 2

        /// Level 3
    };

    //////////////////////////////////////////////////////////////////////////
    /// 77730 - Timothy Leens                                             ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TimothyLeens::npc_TimothyLeens()
        : CreatureScript("npc_TimothyLeens_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    std::vector<SequencePosition> npc_TimothyLeens::CalculateDepositsPositions()
    {
        std::vector<SequencePosition> l_DepositsPositions;

        for (SequencePosition l_Position : g_MineLeftSideDeposits)
        {
            if (l_DepositsPositions.size() < 10 && roll_chance_i(50))
                l_DepositsPositions.push_back(l_Position);
            else if (l_DepositsPositions.size() >= 10)
                break;
        }

        for (SequencePosition l_Position : g_MineRightSideDeposits)
        {
            if (l_DepositsPositions.size() >= 10 && l_DepositsPositions.size() < 20 && roll_chance_i(50))
                l_DepositsPositions.push_back(l_Position);
            else if (l_DepositsPositions.size() >= 20)
                break;
        }

        return l_DepositsPositions;
    }

    void npc_TimothyLeens::SummonDeposits(std::vector<SequencePosition> p_SpawnPos, GarrisonNPCAI* l_AI)
    {
        for (SequencePosition l_Position : p_SpawnPos)
        {
            if (roll_chance_i(50)) ///< Blackrock Deposit
                l_AI->SummonRelativeGameObject(roll_chance_i(20) ? GameObjects::GobRichBlackrockDeposit : GameObjects::GobBlackrockDeposit, l_Position.X, l_Position.Y, l_Position.Z, l_Position.O);
            else ///< True Iron Deposit
                l_AI->SummonRelativeGameObject(roll_chance_i(20) ? GameObjects::GobRichTrueIronDeposit : GameObjects::GobTrueIronDeposit, l_Position.X, l_Position.Y, l_Position.Z, l_Position.O);
        }
    }

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_TimothyLeens::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Alliance_ThingsAreNotGorenOurWay);

        if (l_Quest == nullptr)
            return true;

        if (p_Player->IsQuestRewarded(Quests::Alliance_ThingsAreNotGorenOurWay) ||
            (p_Player->GetQuestStatus(Quests::Alliance_ThingsAreNotGorenOurWay) == QUEST_STATUS_INCOMPLETE))
        {
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::MineDefaultOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else if (p_Player->GetQuestStatus(Quests::Alliance_ThingsAreNotGorenOurWay) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Alliance_ThingsAreNotGorenOurWay) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_TimothyLeens::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        p_Player->PlayerTalkClass->ClearMenus();

        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_TimothyLeens::GetAI(Creature* p_Creature) const
    {
        return new npc_TimothyLeensAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TimothyLeensAI::npc_TimothyLeensAI(Creature* p_Creature)
        : GatheringBuildingMaster(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_TimothyLeensAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea({ NPCs::NpcLunarfallGorenHatchling, NPCs::NpcLunarfallGoren, NPCs::NpcStonetooth }, 200.0f);
    }

    void npc_TimothyLeensAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

        if (!l_GarrisonSite)
            return;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (!l_Owner)
            return;

        if (!l_Owner->IsQuestRewarded(Quests::Alliance_ThingsAreNotGorenOurWay))
        {
            std::vector<uint32> l_CreatureEntries = { NPCs::NpcLunarfallGoren, NPCs::NpcLunarfallGorenHatchling, NPCs::NpcStonetooth };
            me->DespawnCreaturesInArea(l_CreatureEntries, 100.0f);

            for (SequencePosition l_Pos : g_GorenHatchlingPositions)
            {
                if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcLunarfallGoren, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                    l_Creature->GetMotionMaster()->MoveRandom(7.0f);
            }

            for (SequencePosition l_Pos : g_GorenPositions)
            {
                if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcLunarfallGorenHatchling, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                    l_Creature->GetMotionMaster()->MoveRandom(7.0f);
            }

            SummonRelativeCreature(NPCs::NpcStonetooth, 1.26916f, 3.68763f, -33.3223f, 4.37961f, TEMPSUMMON_MANUAL_DESPAWN);
        }
        else
        {
//             std::vector<SequencePosition> l_DepositsPositions = CalculateDepositsPositions();
//             SummonDeposits(l_DepositsPositions, this);

            std::vector<uint32> l_CreatureEntries = { NPCs::NpcLunarfallGorenHatchling, NPCs::NpcLunarfallGoren, NPCs::NpcStonetooth };
            me->DespawnCreaturesInArea(l_CreatureEntries, 100.0f);

            std::list<Creature*> l_MinersList;
            me->GetCreatureListWithEntryInGrid(l_MinersList, NPCs::NpcAllianceMiner, 150.0f);

            if (l_MinersList.empty())
            {
                for (SequencePosition l_Pos : g_MinersPositions)
                {
                    if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcAllianceMiner, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                        l_Creature->GetMotionMaster()->MoveRandom(3.0f);
                }
            }
        }
    }

    void npc_TimothyLeensAI::sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 /*p_Option*/)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

        if (!l_GarrisonSite)
            return;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (l_Owner != p_Player)
            return;

        if (p_Quest->GetQuestId() == Quests::Alliance_ThingsAreNotGorenOurWay)
        {
            if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
            {
                std::vector<uint32> l_CreatureEntries = { NPCs::NpcLunarfallGoren, NPCs::NpcLunarfallGorenHatchling, NPCs::NpcStonetooth };
                me->DespawnCreaturesInArea(l_CreatureEntries, 100.0f);

                std::list<Creature*> l_MinersList;
                me->GetCreatureListWithEntryInGrid(l_MinersList, NPCs::NpcAllianceMiner, 150.0f);

                if (l_MinersList.empty())
                {
                    for (SequencePosition l_Pos : g_MinersPositions)
                    {
                        if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcAllianceMiner, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                            l_Creature->GetMotionMaster()->MoveRandom(7.0f);
                    }

                    GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_GarrisonMgr->GetBuildingWithType(Building::Type::Mine).BuildingID);
                    Position l_Pos = *l_Owner;

                    if (l_BuildingEntry)
                        l_Owner->PlayStandaloneScene(l_Owner->GetGarrison()->GetGarrisonFactionIndex() ? l_BuildingEntry->HordeActivationScenePackageID : l_BuildingEntry->AllianceActivationScenePackageID, 0, l_Pos);

                    l_GarrisonMgr->ActivateBuilding(GetPlotInstanceID());
                }
            }
            InitGatheringPlots(0);
        }
    }

    /// Select game object entry for a fresh gathering spawn
    /// @p_MiscData : Misc data
    uint32 npc_TimothyLeensAI::SelectGameObjectEntryForGatheringSpawn(uint32 /*p_MiscData*/)
    {
        uint32 l_Entry = 0;

        if (roll_chance_i(50))
            l_Entry = roll_chance_i(20) ? GameObjects::GobRichBlackrockDeposit : GameObjects::GobBlackrockDeposit;
        else ///< True Iron Deposit
            l_Entry = roll_chance_i(20) ? GameObjects::GobRichTrueIronDeposit : GameObjects::GobTrueIronDeposit;

        return l_Entry;
    }

}   ///< namespace Garrison
}   ///< namespace MS
