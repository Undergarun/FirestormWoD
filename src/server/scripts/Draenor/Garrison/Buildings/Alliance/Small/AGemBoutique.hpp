////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GEM_BOUTIQUE_ALLIANCE_HPP_GARRISON
#define GARRISON_GEM_BOUTIQUE_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77356 - Costan Highwall                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_CostanHighwallAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_CostanHighwall = ProfessionBuilding_SkillNPC<npc_CostanHighwallAIData::ScriptName, SKILL_JEWELCRAFTING, Quests::Alliance_YourFirstJewelcraftingWorkOrder, &npc_CostanHighwallAIData::Recipes, &npc_CostanHighwallAIData::FnLevel1, &npc_CostanHighwallAIData::FnLevel2, &npc_CostanHighwallAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77775 - Kaya Solasen                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KayaSolasenAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_KayaSolasen = ProfessionBuilding_WorkOrderNPC<npc_KayaSolasenAIData::ScriptName, SKILL_JEWELCRAFTING, Quests::Alliance_YourFirstJewelcraftingWorkOrder, &npc_KayaSolasenAIData::FnLevel1, &npc_KayaSolasenAIData::FnLevel2, &npc_KayaSolasenAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GEM_BOUTIQUE_ALLIANCE_HPP_GARRISON