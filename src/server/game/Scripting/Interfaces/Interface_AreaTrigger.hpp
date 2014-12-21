/// Copyright Ashran 2014-2015

#ifndef SCRIPTING_INTERFACES_AREATRIGGER
#define SCRIPTING_INTERFACES_AREATRIGGER

#include "InterfaceBase.hpp"

/// Area Trigger script interface
class AreaTriggerScript : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        AreaTriggerScript(const char* name);

    public:
        /// Called when the area trigger is activated by a player.
        /// @p_Player  : Player who trigger this area trigger
        /// @p_Trigger : Area Trigger 
        virtual bool OnTrigger(Player * p_Player, const AreaTriggerEntry * p_Trigger)
        {
            UNUSED(p_Player);
            UNUSED(p_Trigger);

            return false;
        }

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
        AreaTriggerEntityScript(const char* p_Name);

    public:
        //// Destructor
        virtual ~AreaTriggerEntityScript()
        {

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

        /// Allow to make local AreaTriggerEntityScript for each AreaTrigger
        virtual AreaTriggerEntityScript* GetAI() const = 0;

};

#endif  ///< SCRIPTING_INTERFACES_AREATRIGGER