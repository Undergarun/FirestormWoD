////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "AEnchanterStudy.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77354 - Ayada The White                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AyadaTheWhiteAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        char ScriptName[] = "npc_AyadaTheWhite_Garr";

        std::vector<SkillNPC_RecipeEntry> Recipes
        {
            { 158877, 31228 },
            { 158907,     0 },
            { 158892, 31228 },
            { 158893, 31228 },
            { 158878, 31228 },
            { 158908,     0 },
            { 158879, 31228 },
            { 158894, 31228 },
            { 158909,     0 },
            { 158880, 31228 },
            { 158910,     0 },
            { 158895, 31228 },
            { 158896, 31228 },
            { 158911,     0 },
            { 158881, 31228 },
            { 162948,     0 },
            { 169091,     0 },
            { 159236,     0 },
            { 158914, 31228 },
            { 158899, 31229 },
            { 158884, 31229 },
            { 158915, 31228 },
            { 158900, 31229 },
            { 158885, 31229 },
            { 158916, 31228 },
            { 158901, 31229 },
            { 158886, 31229 },
            { 158917, 31228 },
            { 158902, 31229 },
            { 158887, 31229 },
            { 158918, 31228 },
            { 158903, 31229 },
            { 158889, 31229 },
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77781 - Garn                                                      ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GarmAIData
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

        char ScriptName[] = "npc_Garm_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS