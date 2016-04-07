////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AEngineeringWorks.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77365 - Zaren                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_ZarenAIData
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

        char ScriptName[] = "npc_Zaren_Garr";

        std::vector<SkillNPC_RecipeEntry> Recipes
        {
            { 176732, 31233 },
            { 173309,     0 },
            { 173308,     0 },
            { 173289, 31232 },
            { 162217, 31233 },
            { 162214, 31233 },
            { 162210, 31233 },
            { 162209, 31233 },
            { 162208,     0 },
            { 162207,     0 },
            { 162205, 31232 },
            { 162204,     0 },
            { 162203, 31232 },
            { 162202, 31232 },
            { 162199, 31233 }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77831 - Helayn Whent                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_HelaynWhentAIData
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

        char ScriptName[] = "npc_HelaynWhent_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS
