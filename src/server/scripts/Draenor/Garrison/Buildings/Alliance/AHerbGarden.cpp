////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "AHerbGarden.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 85514 - Olly Nimkip                                                ///
    //////////////////////////////////////////////////////////////////////////
    #pragma region 
    namespace npc_OllyNimkipData
    {
        /// Cosmetic init for level 1
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        /// Cosmetic init for level 2
        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        /// Cosmetic init for level 3
        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };
    }   ///< namespace npc_TharisStrongcastData

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_OllyNimkip::npc_OllyNimkip()
        : CreatureScript("npc_OllyNimkip_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_OllyNimkip::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->IsQuestRewarded(Quests::Alliance_ClearingTheGarden))
        {
            /// @TODO
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
            return true;
        }
        else
        {
            p_Player->SEND_GOSSIP_MENU(NPCTexts::OllyNimkipDefault, p_Creature->GetGUID());
            return true;
        }
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_OllyNimkip::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (!p_Player->IsQuestRewarded(Quests::Alliance_ClearingTheGarden))
            return true;

        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_OllyNimkip::GetAI(Creature * p_Creature) const
    {
        return new SimpleSequenceCosmeticScriptAI<&npc_OllyNimkipData::FnLevel1, &npc_OllyNimkipData::FnLevel2, &npc_OllyNimkipData::FnLevel3>(p_Creature);
    }
    #pragma endregion 

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////



}   ///< namespace Garrison
}   ///< namespace MS