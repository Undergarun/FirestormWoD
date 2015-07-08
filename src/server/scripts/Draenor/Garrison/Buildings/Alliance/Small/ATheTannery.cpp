////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ATheTannery.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77383 - Anders Longstitch                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AndersLongstitchAIData
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
        
        char ScriptName[] = "npc_AndersLongstitch_Garr";

        std::vector<SkillNPC_RecipeEntry> Recipes
        { 
            { 171260,     0 },
            { 171261,     0 },
            { 171262,     0 },
            { 171263,     0 },
            { 171264,     0 },
            { 171265,     0 },
            { 171266, 28267 },
            { 171267, 28267 },
            { 171268, 28267 },
            { 171269, 28267 },
            { 171270, 28268 },
            { 171271, 28268 },
            { 171272, 28268 },
            { 171273, 28268 },
            { 171274, 28268 },
            { 171275, 28268 },
            { 171276, 28268 },
            { 171277, 28268 },
            { 171282, 28268 },
            { 171284, 28268 },
            { 171281, 28268 },
            { 171279, 28268 },
            { 171280, 28268 },
            { 171278, 28268 },
            { 171285, 28268 },
            { 171283, 28268 },
            { 178208,     0 },
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 78207 - Marianne Levine                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MarianneLevineAIData
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

        char ScriptName[] = "npc_MarianneLevine_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS