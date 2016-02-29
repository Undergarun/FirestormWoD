////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
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
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else if (p_Player->GetQuestStatus(Quests::Alliance_TricksOfTheTrade) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Alliance_TricksOfTheTrade) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

        return true;
    }

    bool npc_TraderJoseph::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr || p_Creature == nullptr || p_Creature->GetScriptName() != CreatureScript::GetName())
            return true;

        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (p_Player && p_Creature && p_Creature->GetScriptName() == GetName())
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player);
        }

        return true;
    }

    bool npc_TraderJoseph::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        if (p_Quest->GetQuestId() == Quests::Horde_TricksOfTheTrade)
        {
            if (p_Player && p_Creature)
            {
                if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
                {
                    CreatureAI* l_AI = p_Creature->AI();

                    if (l_AI == nullptr)
                        return true;

                    if (GarrisonNPCAI* l_GarrisonAI = dynamic_cast<GarrisonNPCAI*>(l_AI))
                        l_GarrisonMgr->ActivateBuilding(l_GarrisonAI->GetPlotInstanceID());
                }
            }
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

}   ///< namespace Garrison
}   ///< namespace MS