////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2014-2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef SCRIPTING_INTERFACES_ENCOUNTER_HPP_INCLUDED
 #define SCRIPTING_INTERFACES_ENCOUNTER_HPP_INCLUDED

 #include "InterfaceBase.hpp"

/// Encounter Script Interface
class EncounterScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        EncounterScript();

    public:
        /// Called when an Encounter is started
        virtual void OnEncounterStart(InstanceScript* p_Instance, uint32 p_EncounterID)
        {
            UNUSED(p_Instance);
            UNUSED(p_EncounterID);
        }

        /// Called when an Encounter is ended
        virtual void OnEncounterEnd(InstanceScript* p_Instance, uint32 p_EncounterID, bool p_Success)
        {
            UNUSED(p_Instance);
            UNUSED(p_EncounterID);
            UNUSED(p_Success);
        }
};

#endif  ///< SCRIPTING_INTERFACES_ENCOUNTER_HPP_INCLUDED