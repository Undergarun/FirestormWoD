////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HTailoringEmporium.hpp"
#include "HTailoringEmporium_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79864 - Warra The Weaver                                          ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_WarraTheWeaverData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            //p_This->SetupActionSequence(ChristopherMacdonald_Level1::MovePointLoc, ChristopherMacdonald_Level1::Sequence, sizeof(ChristopherMacdonald_Level1::Sequence), ChristopherMacdonald_Level1::MovePointIDs::Home);

            //p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_WarraTheWeaver::npc_WarraTheWeaver()
        : CreatureScript("npc_WarraTheWeaver_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_WarraTheWeaver::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (!p_Player->HasQuest(Quests::Horde_YourFirstTailoringWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YourFirstTailoringWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YourFirstTailoringWorkOrder, 4);

        p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need you to do something for me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_WarraTheWeaver::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->CLOSE_GOSSIP_MENU();
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_WarraTheWeaver::GetAI(Creature * p_Creature) const
    {
        return new npc_WarraTheWeaverAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79863 - Turga                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_TurgaAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(Turga_Level1::MovePointLoc, Turga_Level1::Sequence, sizeof(Turga_Level1::Sequence), Turga_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[Turga_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Turga_Level1::DestPointDuration::Home, [p_This]() -> void { p_This->DoNextSequenceAction(); });

                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(Turga_Level1::MovePointLoc[Turga_Level1::MovePointIDs::Home - Turga_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(Turga_Level1::DestPointDuration::Home, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[Turga_Level1::MovePointIDs::Body] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Turga_Level1::DestPointDuration::Body, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(Turga_Level1::MovePointLoc[Turga_Level1::MovePointIDs::Body - Turga_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                });
                p_This->AddTimedDelayedOperation(Turga_Level1::DestPointDuration::Body, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[Turga_Level1::MovePointIDs::Table] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Turga_Level1::DestPointDuration::Table, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void { p_This->SetFacingBuildingRelative(Turga_Level1::MovePointLoc[Turga_Level1::MovePointIDs::Table - Turga_Level1::MovePointIDs::Home].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);                });
                p_This->AddTimedDelayedOperation(10,                  [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_READ_AND_TALK); });
                p_This->AddTimedDelayedOperation(Turga_Level1::DestPointDuration::Table, [p_Me]() -> void
                {
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

    /// Constructor
    npc_Turga::npc_Turga()
        : CreatureScript("npc_Turga_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_Turga::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->HasQuest(Quests::Horde_YourFirstTailoringWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YourFirstTailoringWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YourFirstTailoringWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Horde_YourFirstTailoringWorkOrder) || p_Player->IsQuestRewarded(Quests::Horde_YourFirstTailoringWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_Turga::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_Turga::GetAI(Creature * p_Creature) const
    {
        return new npc_TurgaAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS