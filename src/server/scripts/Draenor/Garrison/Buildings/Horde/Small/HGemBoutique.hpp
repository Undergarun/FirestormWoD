////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GEM_BOUTIQUE_HORDE_HPP_GARRISON
#define GARRISON_GEM_BOUTIQUE_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79832 - Dorogarr                                                  ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_DorogarrAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_Dorogarr = ProfessionBuilding_SkillNPC<npc_DorogarrAIData::ScriptName, SKILL_JEWELCRAFTING, Quests::Horde_YourFirstJewelcraftingWorkOrder, &npc_DorogarrAIData::Recipes, &npc_DorogarrAIData::FnLevel1, &npc_DorogarrAIData::FnLevel2, &npc_DorogarrAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79830 - Elrondir Surrion                                          ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_ElrondirSurrionAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_ElrondirSurrion = ProfessionBuilding_WorkOrderNPC<npc_ElrondirSurrionAIData::ScriptName, SKILL_JEWELCRAFTING, Quests::Horde_YourFirstJewelcraftingWorkOrder, &npc_ElrondirSurrionAIData::FnLevel1, &npc_ElrondirSurrionAIData::FnLevel2, &npc_ElrondirSurrionAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GEM_BOUTIQUE_HORDE_HPP_GARRISON