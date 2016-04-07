////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ENGINEERING_WORKS_HORDE_HPP_GARRISON
#define GARRISON_ENGINEERING_WORKS_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 79826 - Pozzlow                                                   ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_PozzlowAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_Pozzlow = ProfessionBuilding_SkillNPC<npc_PozzlowAIData::ScriptName, SKILL_ENGINEERING, Quests::Horde_YourFirstEngineeringWorkOrder, &npc_PozzlowAIData::Recipes, &npc_PozzlowAIData::FnLevel1, &npc_PozzlowAIData::FnLevel2, &npc_PozzlowAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 86696 - Garbra Fizzwonk                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GarbraFizzwonkAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_GarbraFizzwonk = ProfessionBuilding_WorkOrderNPC<npc_GarbraFizzwonkAIData::ScriptName, SKILL_ENGINEERING, Quests::Horde_YourFirstEngineeringWorkOrder, &npc_GarbraFizzwonkAIData::FnLevel1, &npc_GarbraFizzwonkAIData::FnLevel2, &npc_GarbraFizzwonkAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_ENGINEERING_WORKS_HORDE_HPP_GARRISON