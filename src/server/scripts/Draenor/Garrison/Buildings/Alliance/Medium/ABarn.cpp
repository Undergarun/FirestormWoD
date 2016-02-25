////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
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

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_HomerStonefield::npc_HomerStonefield()
        : CreatureScript("npc_HomerStonefield_Garr")
    {

    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_HomerStonefieldAI::npc_HomerStonefieldAI(Creature* p_Creature)
        : SimpleSequenceCosmeticScriptAI(p_Creature)
    {
    }

    void npc_HomerStonefield::OnShipmentCreated(Player* p_Player, Creature* p_Creature, uint32 p_BuildingID)
    {
        if (p_BuildingID == 24) ///< Barn
        {
            if (p_Player->GetQuestStatus(Quests::Alliance_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(40662);
        }
    }

    bool npc_HomerStonefield::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Alliance_BreakingIntoTheTrapGame);

        if (l_Quest == nullptr)
            return true;

        if (p_Player->IsQuestRewarded(Quests::Alliance_BreakingIntoTheTrapGame) ||
           (p_Player->GetQuestStatus(Quests::Alliance_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE && !p_Player->GetQuestObjectiveCounter(276190)))
        {
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else if (p_Player->GetQuestStatus(Quests::Alliance_BreakingIntoTheTrapGame) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Alliance_BreakingIntoTheTrapGame) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

        return true;
    }

    bool npc_HomerStonefield::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr)
            return true;

        switch (p_Action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Sumptuous Fur.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Raw Beast Hide.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 2: ///< Send shipment for fur
                if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
                {
                    l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentFur);
                    reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentFur); ///< Fur
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 3: ///< Send shipment for leather
                if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
                {
                    l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather);
                    reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather); ///< Leather
                }
                break;
            default:
                break;
        }
        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_HomerStonefield::GetAI(Creature* p_Creature) const
    {
        return new npc_HomerStonefieldAI(p_Creature);
    }

    void npc_HomerStonefieldAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        GarrisonNPCAI::SetData(p_ID, p_Value);

        if (p_ID == 1)
            m_ProductionChosen = p_Value;
    }

    int npc_HomerStonefieldAI::OnShipmentIDRequest(Player* p_Player)
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