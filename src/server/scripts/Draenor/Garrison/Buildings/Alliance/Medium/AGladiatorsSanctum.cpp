////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AGladiatorsSanctum.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 86639 - Altar of Bones                                            ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AltarOfBonesAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };
    }

    /// Constructor
    npc_AltarOfBones::npc_AltarOfBones()
        : CreatureScript("npc_AltarOfBones")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_AltarOfBones::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Creature->ToGarrisonNPCAI() && p_Creature->GetScriptName() == CreatureScript::GetName())
            p_Creature->ToGarrisonNPCAI()->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_AltarOfBones::GetAI(Creature* p_Creature) const
    {
        return new npc_AltarOfBonesAI(p_Creature);
    }

    /// Constructor
    npc_AltarOfBones::npc_AltarOfBonesAI::npc_AltarOfBonesAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    //////////////////////////////////////////////////////////////////////////
    /// 86677 - Kuros                                                      ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Kuros_Garr::npc_Kuros_Garr()
        : CreatureScript("npc_Kuros_Garr")
    {
    }

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_Kuros_Garr::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_WarlordOfDraenor, 4);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_Kuros_GarrAI::npc_Kuros_GarrAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, 114823);
        me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 2, 114824);

        me->SetAIAnimKitId(7224);
    }

    CreatureAI* npc_Kuros_Garr::GetAI(Creature* p_Creature) const
    {
        return new npc_Kuros_GarrAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS