////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_SCRIBE_QUARTERS_HORDE_HPP_GARRISON
#define GARRISON_SCRIBE_QUARTERS_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 79829 - Urgra                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_UrgraAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<SkillNPC_RecipeEntry> Recipes;
    }

    using npc_Urgra = ProfessionBuilding_SkillNPC<npc_UrgraAIData::ScriptName, SKILL_INSCRIPTION, Quests::Horde_YourFirstInscriptionWorkOrder, &npc_UrgraAIData::Recipes, &npc_UrgraAIData::FnLevel1, &npc_UrgraAIData::FnLevel2, &npc_UrgraAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79831 - Yrogg                                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YroggAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_Yrogg = ProfessionBuilding_WorkOrderNPC<npc_YroggAIData::ScriptName, SKILL_INSCRIPTION, Quests::Horde_YourFirstInscriptionWorkOrder, &npc_YroggAIData::FnLevel1, &npc_YroggAIData::FnLevel2, &npc_YroggAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SCRIBE_QUARTERS_HORDE_HPP_GARRISON