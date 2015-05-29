////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_FORGE_HORDE_HPP_GARRISON
#define GARRISON_THE_FORGE_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79867 - Orgek Ironhand                                             ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_OrgekIronhandAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_OrgekIronhand = ProfessionBuilding_SkillNPC<npc_OrgekIronhandAIData::ScriptName, SKILL_BLACKSMITHING, Quests::Horde_YourFirstBlacksmithingWorkOrder, &npc_OrgekIronhandAIData::Recipes, &npc_OrgekIronhandAIData::FnLevel1, &npc_OrgekIronhandAIData::FnLevel2, &npc_OrgekIronhandAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79817 - Kinja                                                      ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KinjaAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_Kinja = ProfessionBuilding_WorkOrderNPC<npc_KinjaAIData::ScriptName, SKILL_BLACKSMITHING, Quests::Horde_YourFirstBlacksmithingWorkOrder, &npc_KinjaAIData::FnLevel1, &npc_KinjaAIData::FnLevel2, &npc_KinjaAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_FORGE_HORDE_HPP_GARRISON