////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
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

    void npc_MurgAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
    {
        if (Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript())
        {
            if (Player* l_Player = l_GarrisonSite->GetOwner())
            {
                std::list<Creature*> l_CreatureList;
                me->GetCreatureListInGrid(l_CreatureList, 30.0f);

                for (Creature* l_Creature : l_CreatureList)
                {
                    if (std::find(TavernDatas::g_QuestGiverEntries.begin(), TavernDatas::g_QuestGiverEntries.end(), l_Creature->GetEntry()) != TavernDatas::g_QuestGiverEntries.end())
                        l_Creature->DespawnOrUnsummon();
                }

                MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

                if (l_GarrisonMgr == nullptr)
                    return;

                std::vector<uint32>& l_Entries = l_GarrisonMgr->GetGarrisonTavernDatas();

                if (l_Entries.size() == 1)
                    SummonRelativeCreature(l_Entries[0],
                    g_QuestGiverHordePositions[0].X,
                    g_QuestGiverHordePositions[0].Y,
                    g_QuestGiverHordePositions[0].Z,
                    g_QuestGiverHordePositions[0].O,
                    TEMPSUMMON_MANUAL_DESPAWN);
                else if (l_Entries.size() > 1)
                {
                    SummonRelativeCreature(l_Entries[0],
                        g_QuestGiverHordePositions[1].X,
                        g_QuestGiverHordePositions[1].Y,
                        g_QuestGiverHordePositions[1].Z,
                        g_QuestGiverHordePositions[1].O,
                        TEMPSUMMON_MANUAL_DESPAWN);

                    SummonRelativeCreature(l_Entries[1],
                        g_QuestGiverHordePositions[2].X,
                        g_QuestGiverHordePositions[2].Y,
                        g_QuestGiverHordePositions[2].Z,
                        g_QuestGiverHordePositions[2].O,
                        TEMPSUMMON_MANUAL_DESPAWN);
                }
            }
        }
    }

    void npc_MurgAI::OnDataReset()
    {
        if (Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript())
        {
            if (Player* l_Player = l_GarrisonSite->GetOwner())
            {
                MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

                if (l_GarrisonMgr == nullptr)
                    return;

                if (roll_chance_i(50))
                {
                    uint32 l_Entry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];

                    l_GarrisonMgr->CleanGarrisonTavernData();
                    l_GarrisonMgr->AddGarrisonTavernData(l_Entry);
                }
                else
                {
                    uint32 l_FirstEntry  = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                    uint32 l_SecondEntry = 0;

                    do
                        l_SecondEntry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                    while (l_SecondEntry == l_FirstEntry);

                    l_GarrisonMgr->CleanGarrisonTavernData();
                    l_GarrisonMgr->AddGarrisonTavernData(l_FirstEntry);
                    l_GarrisonMgr->AddGarrisonTavernData(l_SecondEntry);
                }

                OnSetPlotInstanceID(GetPlotInstanceID());
                l_GarrisonMgr->UpdatePlot(GetPlotInstanceID());
            }
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS
