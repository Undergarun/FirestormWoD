////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ATradingPost.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 87112 - Kinja                                                      ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TraderJoseph::npc_TraderJoseph()
        : CreatureScript("npc_TraderJoseph_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_TraderJoseph::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_TraderJoseph::GetAI(Creature * p_Creature) const
    {
        return new npc_TraderJosephAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TraderJoseph::npc_TraderJosephAI::npc_TraderJosephAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {

    }

}   ///< namespace Garrison
}   ///< namespace MS