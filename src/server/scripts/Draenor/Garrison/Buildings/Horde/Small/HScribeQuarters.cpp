////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HScribeQuarters.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 79829 - Urgra                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_UrgraAIData
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

        char ScriptName[] = "npc_Urgra_Garr";

        std::vector<SkillNPC_RecipeEntry> Recipes
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
    /// 79831 - Yrogg                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YroggAIData
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

        char ScriptName[] = "npc_Yrogg_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS