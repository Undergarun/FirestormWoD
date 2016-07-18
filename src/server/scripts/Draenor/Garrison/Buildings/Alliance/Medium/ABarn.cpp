////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "ABarn.hpp"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 85048 - Farmer Lok'Lub                                             ///
    //////////////////////////////////////////////////////////////////////////

    namespace npc_HomerStonefieldAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(HomerStonefield_Level1::MovePointLoc, HomerStonefield_Level1::Sequence, sizeof(HomerStonefield_Level1::Sequence), HomerStonefield_Level1::MovePointIDs::Start);

            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Start] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(8 * IN_MILLISECONDS, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void { p_This->SetFacingBuildingRelative(HomerStonefield_Level1::MovePointLoc[HomerStonefield_Level1::MovePointIDs::Start - HomerStonefield_Level1::MovePointIDs::Start].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(8 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Way1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(0, [p_This]() -> void { p_This->DoNextSequenceAction(); });
            };
            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Way2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(0, [p_This]() -> void { p_This->DoNextSequenceAction(); });
            };
            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Way3] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(0, [p_This]() -> void { p_This->DoNextSequenceAction(); });
            };

            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Way4] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(8 * IN_MILLISECONDS, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void { p_This->SetFacingBuildingRelative(HomerStonefield_Level1::MovePointLoc[HomerStonefield_Level1::MovePointIDs::Way4 - HomerStonefield_Level1::MovePointIDs::Way4].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(8 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Way3] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(0, [p_This]() -> void { p_This->DoNextSequenceAction(); });
            };
            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Way2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(0, [p_This]() -> void { p_This->DoNextSequenceAction(); });
            };
            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::Way1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(0, [p_This]() -> void { p_This->DoNextSequenceAction(); });
            };

            p_This->m_OnPointReached[HomerStonefield_Level1::MovePointIDs::SecondPause] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(8 * IN_MILLISECONDS, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void { p_This->SetFacingBuildingRelative(HomerStonefield_Level1::MovePointLoc[HomerStonefield_Level1::MovePointIDs::SecondPause - HomerStonefield_Level1::MovePointIDs::SecondPause].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(8 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_HomerStonefield_Garr::npc_HomerStonefield_Garr()
        : CreatureScript("npc_HomerStonefield_Garr")
    {

    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_HomerStonefieldAI::npc_HomerStonefieldAI(Creature* p_Creature)
        : SimpleSequenceCosmeticScriptAI(p_Creature)
    {
    }

    void npc_HomerStonefield_Garr::OnShipmentCreated(Player* p_Player, Creature* /*p_Creature*/, uint32 p_BuildingID)
    {
        switch (p_BuildingID)
        {
        case Building::ID::Barn_Barn_Level1:
            if (p_Player->GetQuestStatus(Quests::Alliance_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(40662);
            break;
        case Building::ID::Barn_Barn_Level2:
            if (p_Player->GetQuestStatus(Quests::Alliance_FeedingAnArmy) == QUEST_STATUS_INCOMPLETE)
                p_Player->QuestObjectiveSatisfy(40672, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
            break;
        case Building::ID::Barn_Barn_Level3:
            if (p_Player->GetQuestStatus(Quests::Alliance_BiggerTrapBetterRewards) == QUEST_STATUS_INCOMPLETE)
                p_Player->QuestObjectiveSatisfy(40695, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
            break;
        default:
            break;
        }
    }

    bool npc_HomerStonefield_Garr::HandleGossipActions(Player* p_Player, Creature* p_Creature, uint32 p_QuestID, uint32 p_Action)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(p_QuestID);

        if (l_Quest == nullptr)
            return false;

        if (p_Player->IsQuestRewarded(p_QuestID) || (p_Player->GetQuestStatus(p_QuestID) == QUEST_STATUS_INCOMPLETE && !p_Player->GetQuestObjectiveCounter(276190)))
        {
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, p_Action);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    bool npc_HomerStonefield_Garr::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_GarrisonMgr == nullptr || l_AI == nullptr)
            return false;

        uint32 l_PlotInstanceID = l_AI->GetPlotInstanceID();

        if (!l_PlotInstanceID)
            return false;

        GarrisonBuilding l_Building = l_GarrisonMgr->GetBuilding(l_PlotInstanceID);

        switch (l_Building.BuildingID)
        {
            case Building::ID::Barn_Barn_Level1:
                return HandleGossipActions(p_Player, p_Creature, Quests::Alliance_BreakingIntoTheTrapGame, GOSSIP_ACTION_INFO_DEF + 1);
            case Building::ID::Barn_Barn_Level2:
                return HandleGossipActions(p_Player, p_Creature, Quests::Alliance_FeedingAnArmy, GOSSIP_ACTION_INFO_DEF + 2);
            case Building::ID::Barn_Barn_Level3:
                return HandleGossipActions(p_Player, p_Creature, Quests::Alliance_BiggerTrapBetterRewards, GOSSIP_ACTION_INFO_DEF + 3);
            default:
                break;
        }

        return true;
    }

    bool npc_HomerStonefield_Garr::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        using namespace GarrisonGossipMenus;

        switch (p_Action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderSomptuousFur, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderRawBeastHide, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderSomptuousFur, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderRawBeastHide, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderSavageFeast, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderSomptuousFur, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderRawBeastHide, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderSavageFeast, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderFurSavageBlood, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderBeastHideSavageBlood, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                p_Player->ADD_GOSSIP_ITEM_DB(MenuID::DefaultMenuGreetings, GossipOption::BarnOrderSavageFeastBlood, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 4: ///< Send shipment for fur
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentFurredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentFurredBeast); ///< Fur
                break;
            case GOSSIP_ACTION_INFO_DEF + 5: ///< Send shipment for leather
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentLeatheredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentLeatheredBeast); ///< Leather
                break;
            case GOSSIP_ACTION_INFO_DEF + 6: ///< Send shipment for Meat
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentMeatyBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentMeatyBeast); ///< Meat
                break;
            case GOSSIP_ACTION_INFO_DEF + 7: ///< Send shipment for more fur + savage blood
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulFurredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulFurredBeast); ///< ShipmentPowerfulFurredBeast
                break;
            case GOSSIP_ACTION_INFO_DEF + 8: ///< Send shipment for more leather + savage blood
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulLeatheredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulLeatheredBeast); ///< ShipmentPowerfulLeatheredBeast
                break;
            case GOSSIP_ACTION_INFO_DEF + 9: ///< Send shipment for more meat + savage blood
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulMeatyBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulMeatyBeast); ///< ShipmentPowerfulMeatyBeast
                break;
            default:
                break;
        }
        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_HomerStonefield_Garr::GetAI(Creature* p_Creature) const
    {
        return new npc_HomerStonefieldAI(p_Creature);
    }

    void npc_HomerStonefieldAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        GarrisonNPCAI::SetData(p_ID, p_Value);

        if (p_ID == 1)
            m_ProductionChosen = p_Value;
    }

    int npc_HomerStonefieldAI::OnShipmentIDRequest(Player* /*p_Player*/)
    {
        return m_ProductionChosen ? m_ProductionChosen : -1;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// 83967 - Homer Stonefield (escort)                                  ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_HomerStonefield_Garr_Trap::npc_HomerStonefield_Garr_Trap()
        : CreatureScript("npc_HomerStonefield_Garr_Trap")
    {
    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_HomerStonefield_Garr_TrapAI::npc_HomerStonefield_Garr_TrapAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        Talk(urand(0, 16));
        me->DespawnOrUnsummon(8 * IN_MILLISECONDS);
    }

    CreatureAI* npc_HomerStonefield_Garr_Trap::GetAI(Creature* p_Creature) const
    {
        return new npc_HomerStonefield_Garr_TrapAI(p_Creature);
    }
}   ///< namespace Garrison
}   ///< namespace MS
#endif
