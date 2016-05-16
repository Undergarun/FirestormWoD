////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GLADIATORS_SANCTUM_ALLIANCE_GARRISON
#define GARRISON_GLADIATORS_SANCTUM_ALLIANCE_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "../../MultiFactionaProfessionlBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 86639 - Altar of Bones                                            ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AltarOfBonesAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    ///< Quests are not required in order to open the NPC

    using npc_AltarOfBones = MultiFactionalBuilding_WorkOrderNPC<npc_AltarOfBonesAIData::ScriptName, SkillType::SKILL_NONE, /*Quests::Alliance_WarlordOfDraenor*/ Quests::None, /*Quests::Horde_WarlordOfDraenor*/ Quests::None, &npc_AltarOfBonesAIData::FnLevel1, &npc_AltarOfBonesAIData::FnLevel2, &npc_AltarOfBonesAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 86677 - Kuros                                                      ///
    //////////////////////////////////////////////////////////////////////////

    class npc_Kuros_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_Kuros_Garr();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_Kuros_GarrAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_Kuros_GarrAI(Creature* p_Creature);
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GLADIATORS_SANCTUM_ALLIANCE_GARRISON