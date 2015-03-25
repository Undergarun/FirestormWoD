////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HAlchemyLab.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79813 - Albert De Hyde                                            ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AlbertDeHydeAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

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

    /// Constructor
    npc_AlbertDeHyde::npc_AlbertDeHyde()
        : CreatureScript("npc_AlbertDeHyde_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_AlbertDeHyde::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (!p_Player->HasQuest(Quests::Horde_YoutFirstAlchemyWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YoutFirstAlchemyWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YoutFirstAlchemyWorkOrder, 4);

        p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need you to do something for me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_AlbertDeHyde::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->CLOSE_GOSSIP_MENU();
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_AlbertDeHyde::GetAI(Creature * p_Creature) const
    {
        return new npc_KeyanaToneAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79814 - Keyana Tone                                               ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KeyanaToneAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

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

    /// Constructor
    npc_KeyanaTone::npc_KeyanaTone()
        : CreatureScript("npc_KeyanaTone_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_KeyanaTone::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->HasQuest(Quests::Horde_YoutFirstAlchemyWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YoutFirstAlchemyWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YoutFirstAlchemyWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Horde_YoutFirstAlchemyWorkOrder) || p_Player->IsQuestRewarded(Quests::Horde_YoutFirstAlchemyWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_KeyanaTone::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_KeyanaTone::GetAI(Creature * p_Creature) const
    {
        return new npc_KeyanaToneAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS