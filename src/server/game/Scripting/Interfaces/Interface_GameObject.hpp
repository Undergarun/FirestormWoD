////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_GAMEOBJECT_HPP_INCLUDED
#define SCRIPTING_INTERFACES_GAMEOBJECT_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// GameObject script interface
class GameObjectScript : public ScriptObjectImpl<true>, public UpdatableScript<GameObject>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        GameObjectScript(const char* p_Name);

    public:
        /// Called when a dummy spell effect is triggered on the GameObject.
        /// @p_Caster      : Spell Caster
        /// @p_SpellID     : Casted spell ID
        /// @p_EffectIndex : Dummy effect index
        /// @p_Target      : Spell target
        virtual bool OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, GameObject* p_Target)
        {
            UNUSED(p_Caster);
            UNUSED(p_SpellID);
            UNUSED(p_EffectIndex);
            UNUSED(p_Target);

            return false;
        }

        /// Called when a player opens a gossip dialog with the GameObject.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        virtual bool OnGossipHello(Player* p_Player, GameObject* p_GameObject)
        {
            UNUSED(p_Player);
            UNUSED(p_GameObject);

            return false;
        }
        /// Called when a player selects a gossip item in the GameObject's gossip menu.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Sender     : Sender menu
        /// @p_Action     : Action
        virtual bool OnGossipSelect(Player* p_Player, GameObject* p_GameObject, uint32 p_Sender, uint32 p_Action)
        {
            UNUSED(p_Player);
            UNUSED(p_GameObject);
            UNUSED(p_Sender);
            UNUSED(p_Action);

            return false;
        }
        /// Called when a player selects a gossip with a code in the GameObject's gossip menu.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Sender     : Sender menu
        /// @p_Action     : Action
        /// @p_Code       : Player input code
        virtual bool OnGossipSelectCode(Player* p_Player, GameObject* p_GameObject, uint32 p_Sender, uint32 p_Action, const char* p_Code)
        {
            UNUSED(p_Player);
            UNUSED(p_GameObject);
            UNUSED(p_Sender);
            UNUSED(p_Action);
            UNUSED(p_Code);

            return false;
        }

        /// Called when a player accepts a quest from the game object.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Quest      : Accepted quest
        virtual bool OnQuestAccept(Player* p_Player, GameObject* p_GameObject, const Quest* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_GameObject);
            UNUSED(p_Quest);

            return false;
        }
        /// Called when a player selects a quest reward.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Quest      : Reward quest
        /// @p_Option     : Reward
        virtual bool OnQuestReward(Player* p_Player, GameObject* p_GameObject, const Quest* p_Quest, uint32 p_Option)
        {
            UNUSED(p_Player);
            UNUSED(p_GameObject);
            UNUSED(p_Quest);
            UNUSED(p_Option);

            return false;
        }

        /// Called when the dialog status between a player and the GameObject is requested.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        virtual uint32 GetDialogStatus(Player* p_Player, GameObject* p_GameObject)
        {
            UNUSED(p_Player);
            UNUSED(p_GameObject);

            return 100;
        }

        // Called when the game object is destroyed (destructible buildings only).
        /// @p_GameObject : Destroyed GameObject
        /// @p_Player     : Destroyer player instance
        virtual void OnDestroyed(GameObject* p_GameObject, Player* p_Player)
        {
            UNUSED(p_GameObject);
            UNUSED(p_Player);
        }
        /// Called when the game object is damaged (destructible buildings only).
        /// @p_GameObject : Damaged GameObject
        /// @p_Player     : Damager player instance
        virtual void OnDamaged(GameObject* p_GameObject, Player* p_Player)
        {
            UNUSED(p_GameObject);
            UNUSED(p_Player);
        }
        /// Called when the game object loot state is changed.
        /// @p_GameObject : Looted GameObject
        /// @p_State      : Loot state
        /// @p_Unit       : Unit
        virtual void OnLootStateChanged(GameObject* p_GameObject, uint32 p_State, Unit* p_Unit)
        { 
            UNUSED(p_GameObject);
            UNUSED(p_State);
            UNUSED(p_Unit);
        }
        /// Called when the game object state is changed.
        /// @p_GameObject : Changed GameObject
        /// @p_State      : GameObject state
        virtual void OnGameObjectStateChanged(const GameObject* p_GameObject, uint32 p_State)
        {
            UNUSED(p_GameObject);
            UNUSED(p_State);
        }

        /// Called when server want to send elevator update, by default all GameObject type transport are elevator
        /// @p_GameObject : GameObject instance
        virtual bool OnGameObjectElevatorCheck(const GameObject* p_GameObject) const
        {
            UNUSED(p_GameObject);

            return true;
        }

        virtual bool OnGameObjectSpellCasterUse(const GameObject* p_GameObject, Player* p_User) const
        {
            UNUSED(p_GameObject);
            UNUSED(p_User);

            return true;
        }

        /// Called when a GameObjectAI object is needed for the GameObject.
        /// @p_GameObject : GameObject instance
        virtual GameObjectAI* GetAI(GameObject* p_GameObject) const 
        {
            UNUSED(p_GameObject);

            return nullptr; 
        }

};

#endif  ///< SCRIPTING_INTERFACES_GAMEOBJECT_HPP_INCLUDED