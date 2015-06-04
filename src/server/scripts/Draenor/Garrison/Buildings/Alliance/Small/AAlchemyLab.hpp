////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ALCHEMY_LAB_ALLIANCE_HPP_GARRISON
#define GARRISON_ALCHEMY_LAB_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77363 - Mary Kearie                                               ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MaryKearieAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_MaryKearie = ProfessionBuilding_SkillNPC<npc_MaryKearieAIData::ScriptName, SKILL_ALCHEMY, Quests::Alliance_YourFirstAlchemyWorkOrder, &npc_MaryKearieAIData::Recipes, &npc_MaryKearieAIData::FnLevel1, &npc_MaryKearieAIData::FnLevel2, &npc_MaryKearieAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77791 - Peter Kearie                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_PeterKearieAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_PeterKearie = ProfessionBuilding_WorkOrderNPC<npc_PeterKearieAIData::ScriptName, SKILL_ALCHEMY, Quests::Alliance_YourFirstAlchemyWorkOrder, &npc_PeterKearieAIData::FnLevel1, &npc_PeterKearieAIData::FnLevel2, &npc_PeterKearieAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_ALCHEMY_LAB_ALLIANCE_HPP_GARRISON