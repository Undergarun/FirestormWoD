////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "ATradingPost.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 87112 - Kinja                                                      ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TraderJoseph::npc_TraderJoseph()
        : CreatureScript("npc_TraderJoseph_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_TraderJoseph::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Alliance_TricksOfTheTrade);

        if (l_Quest == nullptr)
            return false;

        if (p_Player->IsQuestRewarded(Quests::Alliance_TricksOfTheTrade) || (p_Player->GetQuestStatus(Quests::Alliance_TricksOfTheTrade) == QUEST_STATUS_INCOMPLETE))
        {
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else if (p_Player->GetQuestStatus(Quests::Alliance_TricksOfTheTrade) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Alliance_TricksOfTheTrade) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

        return true;
    }

    bool npc_TraderJoseph::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        p_Player->PlayerTalkClass->ClearMenus();

        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            l_AI->SendShipmentCrafterUI(p_Player, p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonTradingPostDailyRandomShipment));

        return true;
    }

    bool npc_TraderJoseph::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/)
    {
        if (p_Quest->GetQuestId() == Quests::Horde_TricksOfTheTrade)
        {
            GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

            if (l_AI == nullptr)
                return true;

            std::vector<uint32> l_TradingPostShipments = { 138, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 196 };
            p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomShipment, l_TradingPostShipments[urand(0, l_TradingPostShipments.size() - 1)]);

            if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
                l_GarrisonMgr->ActivateBuilding(l_AI->GetPlotInstanceID());
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_TraderJoseph::GetAI(Creature* p_Creature) const
    {
        return new npc_TraderJosephAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TraderJoseph::npc_TraderJosephAI::npc_TraderJosephAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    void npc_TraderJoseph::npc_TraderJosephAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea(std::vector<uint32> {NPCs::NpcAllianceAncientTradingMechanismQuestGiver, NPCs::NpcAllianceAncientTradingMechanismAuctioneer}, 100.0f);
    }

    void npc_TraderJoseph::npc_TraderJosephAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
            case Building::ID::TradingPost_TradingPost_Level1:
                l_NpcVendorPos = { 2.3595f, -5.2689f, 0.5832f, 1.2362f };
                break;
            case Building::ID::TradingPost_TradingPost_Level2:
                if (l_Owner->GetQuestStatus(Quests::Alliance_AuctionningForParts) != QUEST_STATUS_REWARDED)
                    SummonRelativeCreature(NPCs::NpcAllianceAncientTradingMechanismQuestGiver, 4.7426f, -8.5494f, 1.6656f, 1.4341f, TEMPSUMMON_MANUAL_DESPAWN);
                else
                    SummonRelativeCreature(NPCs::NpcAllianceAncientTradingMechanismAuctioneer, 4.7426f, -8.5494f, 1.6656f, 1.4341f, TEMPSUMMON_MANUAL_DESPAWN);

                l_NpcVendorPos = { 3.5440f, 2.2147f, 1.6656f, 5.9672f };
                break;
            case Building::ID::TradingPost_TradingPost_Level3:
                if (l_Owner->GetQuestStatus(Quests::Alliance_AuctionningForParts) != QUEST_STATUS_REWARDED)
                    SummonRelativeCreature(NPCs::NpcAllianceAncientTradingMechanismQuestGiver, -2.8987f, -1.3951f, 0.8036f, 0.0550f, TEMPSUMMON_MANUAL_DESPAWN);
                else
                    SummonRelativeCreature(NPCs::NpcAllianceAncientTradingMechanismAuctioneer, -2.8987f, -1.3951f, 0.8036f, 0.0550f, TEMPSUMMON_MANUAL_DESPAWN);

                l_NpcVendorPos = { 14.5129f, -4.9367f, 1.6736f, 1.6161f };
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

    void npc_TraderJoseph::npc_TraderJosephAI::OnDailyDataReset()
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

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////// PlayerScript ////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    void playerScript_Garrison_TradingPost::OnReputationChange(Player* p_Player, uint32 p_FactionID, int32& /*p_Standing*/, bool /*p_Incremential*/)
    {
        switch (p_FactionID)
        {
            case 1708: ///< Launghing Skull, Horde rep
                if (p_Player->GetTeamId() == TEAM_HORDE && CheckRequirements(p_Player))
                    return;
                break;
            case 1710: ///< Sha'tari, Alliance rep
                if (p_Player->GetTeamId() == TEAM_ALLIANCE && CheckRequirements(p_Player))
                    return;
                break;
            default:
                break;
        }
    }

    bool playerScript_Garrison_TradingPost::CheckRequirements(Player* p_Player)
    {
        if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
        {
            switch (l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuildingWithType(MS::Garrison::Building::Type::TradingPost)))
            {
                case 0:
                case 1:
                    return false;
                case 2:
                case 3:
                    return true;
                default:
                    break;
            }
        }

        return false;
    }

}   ///< namespace Garrison
}   ///< namespace MS
#endif
