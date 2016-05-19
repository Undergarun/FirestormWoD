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
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * /*p_Me*/)
        {
            p_This->SetAIObstacleManagerEnabled(true);
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        char ScriptName[] = "npc_AltarOfBones";

    }

    //////////////////////////////////////////////////////////////////////////
    /// 86677 - Kuros                                                      ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Kuros_Garr::npc_Kuros_Garr()
        : CreatureScript("npc_Kuros_Garr")
    {
    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_Kuros_GarrAI::npc_Kuros_GarrAI(Creature * p_Creature)
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