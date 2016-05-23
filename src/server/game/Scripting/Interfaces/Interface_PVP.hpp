////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_PVP_HPP_INCLUDED
#define SCRIPTING_INTERFACES_PVP_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// BattleGround Script Interface
class BattlegroundScript : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        BattlegroundScript(const char * p_Name);

    public:
        /// Should return a fully valid Battleground object for the type ID.
        virtual Battleground* GetBattleground() const = 0;

};

/// OutdoorPvP Script Interface
class OutdoorPvPScript : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script Name
        OutdoorPvPScript(const char* p_Name);

    public:
        /// Should return a fully valid OutdoorPvP object for the type ID.
        virtual OutdoorPvP* GetOutdoorPvP() const = 0;

};

#endif  ///< SCRIPTING_INTERFACES_PVP_HPP_INCLUDED
