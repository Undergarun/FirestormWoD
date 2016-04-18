////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "AScribeQuarters.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77372 - Eric Broadoak                                             ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_EricBroadoakAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        char ScriptName[] = "npc_EricBroadoak_Garr";

        std::vector<RecipesConditions> Recipes
        {
            { 175392,     0 },
            { 175390,     0 },
            { 175389,     0 },
            { 166669,     0 },
            { 166363, 31230 },
            { 166359, 31231 },
            { 166356, 31231 },
            { 165804, 31231 },
            { 163294, 31230 },
            { 178550,     0 }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77777 - KurtBroadoak                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KurtBroadoakAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        char ScriptName[] = "npc_KurtBroadoak_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS