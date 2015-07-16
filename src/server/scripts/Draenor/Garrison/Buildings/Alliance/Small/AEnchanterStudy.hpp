////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ENCHANTER_STUDY_ALLIANCE_HPP_GARRISON
#define GARRISON_ENCHANTER_STUDY_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77354 - Ayada The White                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AyadaTheWhiteAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_AyadaTheWhite = ProfessionBuilding_SkillNPC<npc_AyadaTheWhiteAIData::ScriptName, SKILL_ENCHANTING, Quests::Alliance_YourFirstEnchantingWorkOrder, &npc_AyadaTheWhiteAIData::Recipes, &npc_AyadaTheWhiteAIData::FnLevel1, &npc_AyadaTheWhiteAIData::FnLevel2, &npc_AyadaTheWhiteAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77781 - Garn                                                      ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GarmAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_Garm = ProfessionBuilding_WorkOrderNPC<npc_GarmAIData::ScriptName, SKILL_ENCHANTING, Quests::Alliance_YourFirstEnchantingWorkOrder, &npc_GarmAIData::FnLevel1, &npc_GarmAIData::FnLevel2, &npc_GarmAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_ENCHANTER_STUDY_ALLIANCE_HPP_GARRISON