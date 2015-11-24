////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "../../Alliance/Medium/ALunarfallInn.hpp"
#include "HFrostwallTavern.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79828 - Murg                                                       ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Murg::npc_Murg()
        : CreatureScript("npc_Murg_Garr")
    {

    }

    /// Constructor
    npc_MurgAI::npc_MurgAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_Murg::GetAI(Creature* p_Creature) const
    {
        return new npc_MurgAI(p_Creature);
    }

    void npc_MurgAI::OnDataReset()
    {
        if (Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript())
        {
            if (Player* l_Player = l_GarrisonSite->GetOwner())
            {
                if (MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison())
                {
                    std::vector<uint64> l_CreatureGuids = l_GarrisonMgr->GetBuildingCreaturesByBuildingType(BuildingType::Inn);

                    for (std::vector<uint64>::iterator l_Itr = l_CreatureGuids.begin(); l_Itr != l_CreatureGuids.end(); l_Itr++)
                    {
                        if (Creature* l_Creature = HashMapHolder<Creature>::Find(*l_Itr))
                        {
                            if (std::find(TavernDatas::g_QuestGiverEntries.begin(), TavernDatas::g_QuestGiverEntries.end(), l_Creature->GetEntry()) != TavernDatas::g_QuestGiverEntries.end())
                                l_Creature->DespawnOrUnsummon();
                        }
                    }

                    if (roll_chance_i(50))
                    {
                        SummonRelativeCreature(TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)],
                                               g_QuestGiverHordePositions[0].X,
                                               g_QuestGiverHordePositions[0].Y,
                                               g_QuestGiverHordePositions[0].Z,
                                               g_QuestGiverHordePositions[0].O,
                                               TEMPSUMMON_MANUAL_DESPAWN);
                    }
                    else
                    {
                        uint32 l_FirstEntry  = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                        uint32 l_SecondEntry = 0;

                        SummonRelativeCreature(l_FirstEntry,
                                               g_QuestGiverHordePositions[1].X,
                                               g_QuestGiverHordePositions[1].Y,
                                               g_QuestGiverHordePositions[1].Z,
                                               g_QuestGiverHordePositions[1].O,
                                               TEMPSUMMON_MANUAL_DESPAWN);

                        do
                            l_SecondEntry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                        while (l_SecondEntry == l_FirstEntry);

                        SummonRelativeCreature(l_SecondEntry,
                                               g_QuestGiverHordePositions[2].X,
                                               g_QuestGiverHordePositions[2].Y,
                                               g_QuestGiverHordePositions[2].Z,
                                               g_QuestGiverHordePositions[2].O,
                                               TEMPSUMMON_MANUAL_DESPAWN);
                    }
                }
            }
        }

    }

}   ///< namespace Garrison
}   ///< namespace MS