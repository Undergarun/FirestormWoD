////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HTheForge.hpp"
#include "HTheForge_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79867 - Orgek Ironhand                                             ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_OrgekIronhand::npc_OrgekIronhand()
        : CreatureScript("npc_OrgekIronhand_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_OrgekIronhand::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (!p_Player->HasQuest(Quests::Horde_YourFirstBlacksmithingWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YourFirstBlacksmithingWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YourFirstBlacksmithingWorkOrder, 4);

        p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need you to do something for me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_OrgekIronhand::GetAI(Creature * p_Creature) const
    {
        return new npc_OrgekIronhandAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_OrgekIronhand::npc_OrgekIronhandAI::npc_OrgekIronhandAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
        SetupActionSequence(OrgekIronhand_Level1::MovePointLoc, OrgekIronhand_Level1::Sequence, sizeof(OrgekIronhand_Level1::Sequence), OrgekIronhand_Level1::MovePointIDs::Anvil);

        m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Anvil] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Anvil, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Anvil - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [this]() -> void
            {
                me->LoadEquipment(1, true);
                me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
            });
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Anvil, [this]() -> void
            {
                me->LoadEquipment(0, true);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Front] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Front, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Front - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Front, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Forge1] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge1, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Forge1 - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                             [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
            });
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge1, [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Forge2] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge2, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Forge2 - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                             [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge2, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Chest] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Chest, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Chest - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Chest, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79817 - Kinja                                                      ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Kinja::npc_Kinja()
        : CreatureScript("npc_Kinja_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_Kinja::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->HasQuest(Quests::Horde_YourFirstBlacksmithingWorkOrder) || p_Player->IsQuestRewarded(Quests::Horde_YourFirstBlacksmithingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        if (p_Player->HasQuest(Quests::Horde_YourFirstBlacksmithingWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YourFirstBlacksmithingWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YourFirstBlacksmithingWorkOrder, 4);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_Kinja::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_Kinja::GetAI(Creature * p_Creature) const
    {
        return new npc_KinjaAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Kinja::npc_KinjaAI::npc_KinjaAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
        SetupActionSequence(Kinja_Level1::MovePointLoc, Kinja_Level1::Sequence, sizeof(Kinja_Level1::Sequence), Kinja_Level1::MovePointIDs::Anvil);

        m_OnPointReached[Kinja_Level1::MovePointIDs::Anvil] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Anvil, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Anvil - Kinja_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                    [this]() -> void
            {
                me->LoadEquipment(1, true);
                me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
            });
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Anvil, [this]() -> void
            {
                me->LoadEquipment(0, true);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[Kinja_Level1::MovePointIDs::Front] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Front, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Front - Kinja_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                    [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Front, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[Kinja_Level1::MovePointIDs::Forge1] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge1, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Forge1 - Kinja_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
            });
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge1, [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[Kinja_Level1::MovePointIDs::Forge2] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge2, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Forge2 - Kinja_Level1::MovePointIDs::Anvil].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge2, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        DoNextSequenceAction();
    }

}   ///< namespace Garrison
}   ///< namespace MS