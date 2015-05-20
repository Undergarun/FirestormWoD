////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ScriptMgr.h"

/// Constructor
/// @p_ID : Player condition ID
PlayerConditionScript::PlayerConditionScript(uint32 p_ID)
{
    sScriptMgr->RegisterPlayerConditionScript(p_ID, this);
}