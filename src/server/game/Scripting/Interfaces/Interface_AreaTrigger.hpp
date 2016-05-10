////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
# ifndef SCRIPTING_INTERFACES_AREATRIGGER_HPP_INCLUDED
#define SCRIPTING_INTERFACES_AREATRIGGER_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Area Trigger script interface
class AreaTriggerScript : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        AreaTriggerScript(char const* name);

    public:
        /// Called when the area trigger is activated by a player.
        /// @p_Player  : Player who trigger this area trigger
        /// @p_Trigger : Area Trigger 
        virtual bool OnTrigger(Player* p_Player, AreaTriggerEntry const* p_Trigger)
        {
            UNUSED(p_Player);
            UNUSED(p_Trigger);

            return false;
        }

        /// Called when a player enters the AreaTrigger
        virtual void OnEnter(Player* /*p_Player*/, AreaTriggerEntry const* /*p_AreaTrigger*/) { }

        /// Called when a player exits the AreaTrigger
        virtual void OnExit(Player* /*p_Player*/, AreaTriggerEntry const* /*p_AreaTrigger*/) { }

        /// Called on each update of AreaTriggers.
        /// @p_AreaTrigger : Area Trigger instance
        virtual void OnUpdate(AreaTrigger* p_AreaTrigger)
        {
            UNUSED(p_AreaTrigger);
        }

};

/// Area Trigger Entity script interface
class AreaTriggerEntityScript : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script name
        AreaTriggerEntityScript(char const* p_Name);

    public:
        //// Destructor
        virtual ~AreaTriggerEntityScript()
        {

        }

        /// Procs before creation to specify position and linear destination of the areatrigger
        /// @p_AreaTrigger: Areatrigger Instance
        /// @p_Caster: Caster because he the Areatrigger is not spawned so caster is not defined
        /// @p_SourcePosition: Spawn location of the Areatrigger
        /// @p_DestinationPostion: Linear destination of the Areatrigger
        /// @p_PathToLinearDestination: Path to endpoint without the starting or end point
        virtual void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination)
        {
            UNUSED(p_AreaTrigger);
            UNUSED(p_Caster);
            UNUSED(p_SourcePosition);
            UNUSED(p_DestinationPosition);
            UNUSED(p_PathToLinearDestination);
        }

        /// Proc when AreaTrigger is created.
        /// @p_AreaTrigger : AreaTrigger instance
        virtual void OnCreate(AreaTrigger* p_AreaTrigger) 
        {
            UNUSED(p_AreaTrigger);
        }

        /// Proc when AreaTrigger is updated.
        /// @p_AreaTrigger : AreaTrigger instance
        /// @p_Time        : Diff since last update
        virtual void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) 
        {
            UNUSED(p_AreaTrigger);
            UNUSED(p_Time);
        }

        /// Proc when AreaTrigger is removed.
        /// @p_AreaTrigger : AreaTrigger instance
        /// @p_Time        : Diff since last update
        virtual void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) 
        {
            UNUSED(p_AreaTrigger);
            UNUSED(p_Time);
        }

        /// Called when AreaTrigger reached its Destination
        virtual void OnDestinationReached(AreaTrigger* p_AreaTrigger)
        {
            UNUSED(p_AreaTrigger);
        }

        /// Allow to make local AreaTriggerEntityScript for each AreaTrigger
        virtual AreaTriggerEntityScript* GetAI() const = 0;

};

#endif  ///< SCRIPTING_INTERFACES_AREATRIGGER_HPP_INCLUDED
