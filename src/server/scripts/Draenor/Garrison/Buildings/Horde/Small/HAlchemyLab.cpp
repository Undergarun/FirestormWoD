////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HAlchemyLab.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79813 - Albert De Hyde                                            ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AlbertDeHydeAIData
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

        char ScriptName[] = "npc_AlbertDeHyde_Garr";

        std::vector<SkillNPC_RecipeEntry> Recipes
        {
            { 156582,     0 },
            { 156585,     0 },
            { 156577, 27405 },
            { 156578, 27405 },
            { 156579, 27405 },
            { 156580, 27405 },
            { 156581, 27405 },
            { 156561, 27406 },
            { 156563, 27406 },
            { 156564, 27406 },
            { 175869,     0 },
            { 175868,     0 },
            { 175867,     0 },
            { 175866,     0 },
            { 175865,     0 },
            { 175853,     0 },
            { 156568, 27406 }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79814 - Keyana Tone                                               ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KeyanaToneAIData
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

        char ScriptName[] = "npc_KeyanaTone_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS