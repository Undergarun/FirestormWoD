////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_TANNERY_HORDE_HPP_GARRISON
#define GARRISON_THE_TANNERY_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77383 - Anders Longstitch                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MurneGreenhoofAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_MurneGreenhoof = ProfessionBuilding_SkillNPC<npc_MurneGreenhoofAIData::ScriptName, SKILL_LEATHERWORKING, Quests::Horde_YourFirstLeatherworkingWorkOrder, &npc_MurneGreenhoofAIData::Recipes, &npc_MurneGreenhoofAIData::FnLevel1, &npc_MurneGreenhoofAIData::FnLevel2, &npc_MurneGreenhoofAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79833 - Yanney                                                    ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YanneyAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_Yanney = ProfessionBuilding_WorkOrderNPC<npc_YanneyAIData::ScriptName, SKILL_LEATHERWORKING, Quests::Horde_YourFirstLeatherworkingWorkOrder, &npc_YanneyAIData::FnLevel1, &npc_YanneyAIData::FnLevel2, &npc_YanneyAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_TANNERY_HORDE_HPP_GARRISON