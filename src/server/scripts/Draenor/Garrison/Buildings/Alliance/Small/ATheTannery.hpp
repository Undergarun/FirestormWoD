////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_TANNERY_ALLIANCE_HPP_GARRISON
#define GARRISON_THE_TANNERY_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77383 - Anders Longstitch                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AndersLongstitchAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_AndersLongstitch = ProfessionBuilding_SkillNPC<npc_AndersLongstitchAIData::ScriptName, SKILL_LEATHERWORKING, Quests::Alliance_YourFirstLeatherworkingWorkOrder, &npc_AndersLongstitchAIData::Recipes, &npc_AndersLongstitchAIData::FnLevel1, &npc_AndersLongstitchAIData::FnLevel2, &npc_AndersLongstitchAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 78207 - Marianne Levine                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MarianneLevineAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_MarianneLevine = ProfessionBuilding_WorkOrderNPC<npc_MarianneLevineAIData::ScriptName, SKILL_LEATHERWORKING, Quests::Alliance_YourFirstLeatherworkingWorkOrder, &npc_MarianneLevineAIData::FnLevel1, &npc_MarianneLevineAIData::FnLevel2, &npc_MarianneLevineAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_TANNERY_ALLIANCE_HPP_GARRISON