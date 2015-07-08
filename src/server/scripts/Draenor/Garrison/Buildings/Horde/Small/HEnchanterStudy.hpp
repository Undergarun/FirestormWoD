////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ENCHANTER_STUDY_HORDE_HPP_GARRISON
#define GARRISON_ENCHANTER_STUDY_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79821 - Yukla Greenshadow                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YuklaGreenshadowAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_YuklaGreenshadow = ProfessionBuilding_SkillNPC<npc_YuklaGreenshadowAIData::ScriptName, SKILL_ENCHANTING, Quests::Horde_YourFirstEnchantingWorkOrder, &npc_YuklaGreenshadowAIData::Recipes, &npc_YuklaGreenshadowAIData::FnLevel1, &npc_YuklaGreenshadowAIData::FnLevel2, &npc_YuklaGreenshadowAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79820 - Garra                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GarraAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_Garra = ProfessionBuilding_WorkOrderNPC<npc_GarraAIData::ScriptName, SKILL_ENCHANTING, Quests::Horde_YourFirstEnchantingWorkOrder, &npc_GarraAIData::FnLevel1, &npc_GarraAIData::FnLevel2, &npc_GarraAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_ENCHANTER_STUDY_HORDE_HPP_GARRISON