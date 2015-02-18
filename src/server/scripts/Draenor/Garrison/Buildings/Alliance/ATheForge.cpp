////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ATheForge.hpp"
#include "ATheForge_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77359 - Auria Irondreamer                                         ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_AuriaIrondreamer::npc_AuriaIrondreamer()
        : CreatureScript("npc_AuriaIrondreamer_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_AuriaIrondreamer::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need you to do something for me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_AuriaIrondreamer::GetAI(Creature * p_Creature) const
    {
        return new npc_AuriaIrondreamerAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_AuriaIrondreamer::npc_AuriaIrondreamerAI::npc_AuriaIrondreamerAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
        SetupActionSequence(AuriaIrondreamer_Level1::MovePointLoc, AuriaIrondreamer_Level1::Sequence, sizeof(AuriaIrondreamer_Level1::Sequence), AuriaIrondreamer_Level1::MovePointIDs::ForgeOre);

        m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeOre, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                  [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeOre, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ForgeFront] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeFront, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeFront - AuriaIrondreamer_Level1::MovePointIDs::ForgeFront].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                    [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeFront, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::Workorder] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::Workorder, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::Workorder - AuriaIrondreamer_Level1::MovePointIDs::Workorder].O); });
        };

        m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderDown, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown - AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                       [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderDown, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::WorkorderFront] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderFront, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre - AuriaIrondreamer_Level1::MovePointIDs::WorkorderFront].O); });
        };

        m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ChestLeft] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ChestLeft, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre - AuriaIrondreamer_Level1::MovePointIDs::ChestLeft].O); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                   [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ChestLeft, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77792 - Yulia Samras                                              ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_YuliaSamras::npc_YuliaSamras()
        : CreatureScript("npc_YuliaSamras_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_YuliaSamras::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstBlacksmithingWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstBlacksmithingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_YuliaSamras::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_YuliaSamras::GetAI(Creature * p_Creature) const
    {
        return new npc_YuliaSamrasAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_YuliaSamras::npc_YuliaSamrasAI::npc_YuliaSamrasAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
        SetupActionSequence(YuliaSamras_Level1::MovePointLoc, YuliaSamras_Level1::Sequence, sizeof(YuliaSamras_Level1::Sequence), YuliaSamras_Level1::MovePointIDs::Workorder);

        m_OnPointReached[YuliaSamras_Level1::MovePointIDs::Workorder] = [this]() -> void
        {
            AddTimedDelayedOperation(YuliaSamras_Level1::DestPointDuration::Workorder, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(YuliaSamras_Level1::MovePointLoc[YuliaSamras_Level1::MovePointIDs::Workorder - YuliaSamras_Level1::MovePointIDs::Workorder].O); });
        };

        DoNextSequenceAction();
    }

}   ///< namespace Garrison
}   ///< namespace MS