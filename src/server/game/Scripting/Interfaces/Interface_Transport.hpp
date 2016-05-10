////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_TRANSPORT_HPP_INCLUDED
#define SCRIPTING_INTERFACES_TRANSPORT_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Transport Script interface
class TransportScript : public ScriptObjectImpl<true>, public UpdatableScript<Transport>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        TransportScript(const char * p_Name);

    public:
        /// Called when a player boards the transport.
        /// @p_Transport : Transport instance
        /// @p_Player    : Added player passenger instance
        virtual void OnAddPassenger(Transport * p_Transport, Player * p_Player)
        { 
            UNUSED(p_Transport);
            UNUSED(p_Player);
        }
        /// Called when a creature boards the transport.
        /// @p_Transport : Transport instance
        /// @p_Creature  : Added creature passenger instance
        virtual void OnAddCreaturePassenger(Transport * p_Transport, Creature * p_Creature)
        {
            UNUSED(p_Transport);
            UNUSED(p_Creature);
        }
        /// Called when a player exits the transport.
        /// @p_Transport : Transport instance
        /// @p_Player    : Added player passenger instance
        virtual void OnRemovePassenger(Transport * p_Transport, Player * p_Player)
        {
            UNUSED(p_Transport);
            UNUSED(p_Player);
        }

        /// Called when a transport moves.
        /// @p_Transport  : Transport instance
        /// @p_WaypointID : Reached waypoint ID
        /// @p_MapID      : Reached MapID
        /// @p_X          : New position X offset
        /// @p_Y          : New position Y offset
        /// @p_Z          : New position Z offset
        virtual void OnRelocate(Transport* p_Transport, uint32 p_WaypointID, uint32 p_MapID, float p_X, float p_Y, float p_Z)
        {
            UNUSED(p_Transport);
            UNUSED(p_WaypointID);
            UNUSED(p_MapID);
            UNUSED(p_X);
            UNUSED(p_Y);
            UNUSED(p_Z);
        }

};

#endif  ///< SCRIPTING_INTERFACES_TRANSPORT_HPP_INCLUDED
