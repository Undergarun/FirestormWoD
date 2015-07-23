////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ALunarfallExcavation.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77730 - Timothy Leens                                             ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TimothyLeens::npc_TimothyLeens()
        : CreatureScript("npc_TimothyLeens_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_TimothyLeens::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
        {
            uint32 l_BuildingID = reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->GetBuildingID();

            if (l_BuildingID && sGarrisonBuildingManager->MatchsConditionsForBuilding(l_BuildingID, p_Player))
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Can you refine this draenic stone into ore for me?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }

        p_Player->SEND_GOSSIP_MENU(LunarfallExcavation::NPCTextThimothyLeens, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_TimothyLeens::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_TimothyLeens::GetAI(Creature * p_Creature) const
    {
        return new npc_TimothyLeensAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TimothyLeens::npc_TimothyLeensAI::npc_TimothyLeensAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

}   ///< namespace Garrison
}   ///< namespace MS