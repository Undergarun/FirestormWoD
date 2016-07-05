////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_PLAYER_CONDITION_HPP_INCLUDED
#define SCRIPTING_INTERFACES_PLAYER_CONDITION_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Player condition Script Interface
class PlayerConditionScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_ID : Player condition ID
        PlayerConditionScript(uint32 p_ID);

    public:
        /// Called when a single condition is checked for a player.
        /// @p_ConditionID : Player condition ID
        /// @p_Condition   : Player condition instance
        /// @p_SourceInfo  : Player
        virtual bool OnConditionCheck(uint32 p_ConditionID, PlayerConditionEntry const* p_Condition, Player const* p_Player)
        {
            UNUSED(p_ConditionID);
            UNUSED(p_Condition);
            UNUSED(p_Player);

            return true;
        }

};

#endif  ///< SCRIPTING_INTERFACES_PLAYER_CONDITION_HPP_INCLUDED
