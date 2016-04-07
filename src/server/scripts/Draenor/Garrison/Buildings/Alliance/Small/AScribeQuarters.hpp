////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_SCRIBE_QUARTERS_ALLIANCE_HPP_GARRISON
#define GARRISON_SCRIBE_QUARTERS_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77372 - Eric Broadoak                                             ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_EricBroadoakAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_EricBroadoak = ProfessionBuilding_SkillNPC<npc_EricBroadoakAIData::ScriptName, SKILL_INSCRIPTION, Quests::Alliance_YourFirstInscriptionWorkOrder, &npc_EricBroadoakAIData::Recipes, &npc_EricBroadoakAIData::FnLevel1, &npc_EricBroadoakAIData::FnLevel2, &npc_EricBroadoakAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77777 - KurtBroadoak                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KurtBroadoakAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_KurtBroadoak = ProfessionBuilding_WorkOrderNPC<npc_KurtBroadoakAIData::ScriptName, SKILL_INSCRIPTION, Quests::Alliance_YourFirstInscriptionWorkOrder, &npc_KurtBroadoakAIData::FnLevel1, &npc_KurtBroadoakAIData::FnLevel2, &npc_KurtBroadoakAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SCRIBE_QUARTERS_ALLIANCE_HPP_GARRISON