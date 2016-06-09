////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HTradingPost.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 87112 - Fayla Fairfeather                                          ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FaylaFairfeather::npc_FaylaFairfeather()
        : CreatureScript("npc_FaylaFairfeather_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_FaylaFairfeather::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Horde_TricksOfTheTrade);

        if (l_Quest == nullptr)
            return false;

        if (p_Player->IsQuestRewarded(Quests::Horde_TricksOfTheTrade) || (p_Player->GetQuestStatus(Quests::Horde_TricksOfTheTrade) == QUEST_STATUS_INCOMPLETE))
        {
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else if (p_Player->GetQuestStatus(Quests::Horde_TricksOfTheTrade) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Horde_TricksOfTheTrade) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

        return true;
    }

    bool npc_FaylaFairfeather::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        p_Player->PlayerTalkClass->ClearMenus();

        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            l_AI->SendShipmentCrafterUI(p_Player, p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonTradingPostDailyRandomShipment));

        return true;
    }

    bool npc_FaylaFairfeather::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/)
    {
        if (p_Quest->GetQuestId() == Quests::Horde_TricksOfTheTrade)
        {
            if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
            {
                GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

                if (l_AI == nullptr)
                    return true;

                std::vector<uint32> l_TradingPostShipments = { 138, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 196 };
                p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomShipment, l_TradingPostShipments[urand(0, l_TradingPostShipments.size() - 1)]);

                l_GarrisonMgr->ActivateBuilding(l_AI->GetPlotInstanceID());
            }
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_FaylaFairfeather::GetAI(Creature* p_Creature) const
    {
        return new npc_FaylaFairfeatherAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FaylaFairfeather::npc_FaylaFairfeatherAI::npc_FaylaFairfeatherAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    void npc_FaylaFairfeather::npc_FaylaFairfeatherAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea(std::vector<uint32> {NPCs::NpcHordeAncientTradingMechanismQuestGiver, NPCs::NpcHordeAncientTradingMechanismAuctioneer}, 100.0f);
    }

    void npc_FaylaFairfeather::npc_FaylaFairfeatherAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        Player* l_Owner = GetOwner();

        if (l_Owner == nullptr)
            return;

        Manager* l_GarrisonMgr = l_Owner->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;

        SequencePosition l_NpcVendorPos;

        switch (l_GarrisonMgr->GetBuilding(p_PlotInstanceID).BuildingID)
        {
            case Buildings::TradingPost_TradingPost_Level1:
                l_NpcVendorPos = { 7.7799f, -4.4549f, 0.5719f, 0.1100f };
                break;
            case Buildings::TradingPost_TradingPost_Level2:
                if (l_Owner->GetQuestStatus(Quests::Horde_AuctionningForParts) != QUEST_STATUS_REWARDED)
                    SummonRelativeCreature(NPCs::NpcHordeAncientTradingMechanismQuestGiver, -3.7428f, -2.1507f, 0.3379f, 1.4059f, TEMPSUMMON_MANUAL_DESPAWN);
                else
                    SummonRelativeCreature(NPCs::NpcHordeAncientTradingMechanismAuctioneer, -3.7428f, -2.1507f, 0.3379f, 1.4059f, TEMPSUMMON_MANUAL_DESPAWN);

                l_NpcVendorPos = { 1.8293f, -1.6102f, 0.2526f, 0.5341f };
                break;
            case Buildings::TradingPost_TradingPost_Level3:
                if (l_Owner->GetQuestStatus(Quests::Horde_AuctionningForParts) != QUEST_STATUS_REWARDED)
                    SummonRelativeCreature(NPCs::NpcHordeAncientTradingMechanismQuestGiver, -3.1509f, -4.1658f, 0.6419f, 1.2944f, TEMPSUMMON_MANUAL_DESPAWN);
                else
                    SummonRelativeCreature(NPCs::NpcHordeAncientTradingMechanismAuctioneer, -3.1509f, -4.1658f, 0.6419f, 1.2944f, TEMPSUMMON_MANUAL_DESPAWN);

                l_NpcVendorPos = { -5.6686f, -2.3819f, 0.4834f, 0.3087f };
                break;
            default:
                break;
        }

        uint32 l_NpcID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonTradingPostDailyRandomTrader);

        if (!l_NpcID) ///< Quest or daily refill not done
            return;

        if (Creature* l_Creature = me->FindNearestCreature(l_NpcID, 30.0f))
            l_Creature->DespawnOrUnsummon();

        SummonRelativeCreature(l_NpcID, l_NpcVendorPos, TEMPSUMMON_MANUAL_DESPAWN);

        l_Owner->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomTrader, l_NpcID);
        l_Owner->SaveToDB();
    }

    void npc_FaylaFairfeather::npc_FaylaFairfeatherAI::OnDailyDataReset()
    {
        if (GetOwner() != nullptr)
        {
            MS::Garrison::Manager* l_GarrisonMgr = GetOwner()->GetGarrison();

            if (l_GarrisonMgr == nullptr)
                return;

            OnSetPlotInstanceID(GetPlotInstanceID());
            l_GarrisonMgr->UpdatePlot(GetPlotInstanceID());
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS
