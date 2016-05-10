////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_CREATURE_HPP_INCLUDED
#define SCRIPTING_INTERFACES_CREATURE_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Creature script interface
class CreatureScript : public ScriptObjectImpl<true>, public UpdatableScript<Creature>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        CreatureScript(const char* p_Name);

    public:
        /// Called when a dummy spell effect is triggered on the creature.
        /// @p_Caster      : Spell Caster
        /// @p_SpellID     : Casted spell ID
        /// @p_EffectIndex : Dummy effect index
        /// @p_Target      : Spell target
        virtual bool OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, Creature* p_Target)
        {
            UNUSED(p_Caster);
            UNUSED(p_SpellID);
            UNUSED(p_EffectIndex);
            UNUSED(p_Target);

            return false;
        }

        /// Called when a player opens a gossip dialog with the creature.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);

            return false;
        }
        /// Called when a player selects a gossip item in the creature's gossip menu.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Sender   : Sender menu
        /// @p_Action   : Action
        virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);
            UNUSED(p_Sender);
            UNUSED(p_Action);

            return false;
        }
        /// Called when a player selects a gossip with a code in the creature's gossip menu.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Sender   : Sender menu
        /// @p_Action   : Action
        /// @p_Code     : Player input code
        virtual bool OnGossipSelectCode(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action, const char * p_Code)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);
            UNUSED(p_Sender);
            UNUSED(p_Action);
            UNUSED(p_Code);

            return false;
        }

        /// Called when a player accepts a quest from the creature.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Accepted quest
        virtual bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);
            UNUSED(p_Quest);

            return false;
        }
        /// Called when a player selects a quest in the creature's quest menu.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Selected quest
        virtual bool OnQuestSelect(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);
            UNUSED(p_Quest);

            return false;
        }
        /// Called when a player completes a quest with the creature.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Completed quest
        virtual bool OnQuestComplete(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);
            UNUSED(p_Quest);

            return false;
        }
        /// Called when a player selects a quest reward.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Reward quest
        /// @p_Option   : Reward
        virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);
            UNUSED(p_Quest);
            UNUSED(p_Option);

            return false;
        }

        /// Called when the dialog status between a player and the creature is requested.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        virtual uint32 GetDialogStatus(Player* p_Player, Creature* p_Creature)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);

            return 100;
        }

        /// Called when player asks from a creature gossip to create a shipment.
        /// @p_Player       : Player that created the shipment
        /// @p_Creature     : NPC that got the gossip
        virtual void OnShipmentCreated(Player* p_Player, Creature* p_Creature, uint32 p_BuildingID)
        {
            UNUSED(p_Player);
            UNUSED(p_Creature);
            UNUSED(p_BuildingID);

            return;
        }

        /// Called when a CreatureAI object is needed for the creature.
        /// @p_Creature : Target creature instance
        virtual CreatureAI* GetAI(Creature* p_Creature) const
        {
            UNUSED(p_Creature);

            return nullptr;
        }

};

#endif  ///< SCRIPTING_INTERFACES_CREATURE_HPP_INCLUDED
