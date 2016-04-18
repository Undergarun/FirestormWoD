////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ENGINEERING_WORKS_ALLIANCE_HPP_GARRISON
#define GARRISON_ENGINEERING_WORKS_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77365 - Zaren                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_ZarenAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<RecipesConditions> Recipes;
    }

    using npc_Zaren = ProfessionBuilding_SkillNPC<npc_ZarenAIData::ScriptName, SKILL_ENGINEERING, Quests::Alliance_YourFirstEngineeringWorkOrder, 77365, &npc_ZarenAIData::FnLevel1, &npc_ZarenAIData::FnLevel2, &npc_ZarenAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77831 - Helayn Whent                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_HelaynWhentAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_HelaynWhent = ProfessionBuilding_WorkOrderNPC<npc_HelaynWhentAIData::ScriptName, SKILL_ENGINEERING, Quests::Alliance_YourFirstEngineeringWorkOrder, &npc_HelaynWhentAIData::FnLevel1, &npc_HelaynWhentAIData::FnLevel2, &npc_HelaynWhentAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_ENGINEERING_WORKS_ALLIANCE_HPP_GARRISON