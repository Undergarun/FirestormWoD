////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_FORGE_ALLIANCE_HPP_GARRISON
#define GARRISON_THE_FORGE_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77359 - Auria Irondreamer
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AuriaIrondreamerAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_AuriaIrondreamer = ProfessionBuilding_SkillNPC<npc_AuriaIrondreamerAIData::ScriptName, SKILL_BLACKSMITHING, Quests::Alliance_YourFirstBlacksmithingWorkOrder, &npc_AuriaIrondreamerAIData::Recipes, &npc_AuriaIrondreamerAIData::FnLevel1, &npc_AuriaIrondreamerAIData::FnLevel2, &npc_AuriaIrondreamerAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77792 - Yulia Samras
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YuliaSamrasAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }
    
    using npc_YuliaSamras = ProfessionBuilding_WorkOrderNPC<npc_YuliaSamrasAIData::ScriptName, SKILL_BLACKSMITHING, Quests::Alliance_YourFirstBlacksmithingWorkOrder, &npc_YuliaSamrasAIData::FnLevel1, &npc_YuliaSamrasAIData::FnLevel2, &npc_YuliaSamrasAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_FORGE_ALLIANCE_HPP_GARRISON