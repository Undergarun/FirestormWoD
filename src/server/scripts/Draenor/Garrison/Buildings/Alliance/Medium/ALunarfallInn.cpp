////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ALunarfallInn.hpp"
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
    /// 77368 - Madison Clark                                             ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_MadisonClark::npc_MadisonClark()
        : CreatureScript("npc_MadisonClark_Garr")
    {

    }

    /// Constructor
    npc_MadisonClarkAI::npc_MadisonClarkAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_MadisonClark::GetAI(Creature* p_Creature) const
    {
        return new npc_MadisonClarkAI(p_Creature);
    }

    void npc_MadisonClarkAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        if (Player* l_Player = GetOwner())
        {
            std::list<Creature*> l_CreatureList;
            MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

            if (l_GarrisonMgr == nullptr)
                return;

            me->GetCreatureListInGrid(l_CreatureList, 30.0f);

            for (Creature* l_Creature : l_CreatureList)
            {
                if (std::find(TavernDatas::g_QuestGiverEntries.begin(), TavernDatas::g_QuestGiverEntries.end(), l_Creature->GetEntry()) != TavernDatas::g_QuestGiverEntries.end())
                    l_Creature->DespawnOrUnsummon();
            }

            std::vector<uint32>& l_Entries = l_GarrisonMgr->GetGarrisonTavernDatas();

            if (l_Entries.size() == 1)
                SummonRelativeCreature(l_Entries[0],
                g_QuestGiverAlliancePositions[0].X,
                g_QuestGiverAlliancePositions[0].Y,
                g_QuestGiverAlliancePositions[0].Z,
                g_QuestGiverAlliancePositions[0].O,
                TEMPSUMMON_MANUAL_DESPAWN);
            else if (l_Entries.size() > 1)
            {
                SummonRelativeCreature(l_Entries[0],
                    g_QuestGiverAlliancePositions[1].X,
                    g_QuestGiverAlliancePositions[1].Y,
                    g_QuestGiverAlliancePositions[1].Z,
                    g_QuestGiverAlliancePositions[1].O,
                    TEMPSUMMON_MANUAL_DESPAWN);

                SummonRelativeCreature(l_Entries[1],
                    g_QuestGiverAlliancePositions[2].X,
                    g_QuestGiverAlliancePositions[2].Y,
                    g_QuestGiverAlliancePositions[2].Z,
                    g_QuestGiverAlliancePositions[2].O,
                    TEMPSUMMON_MANUAL_DESPAWN);
            }
        }
    }

    void npc_MadisonClarkAI::OnDataReset()
    {
        if (Player* l_Player = GetOwner())
        {
            MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

            if (l_GarrisonMgr == nullptr)
                return;

            l_GarrisonMgr->CleanGarrisonTavernData();

            if (roll_chance_i(50))
            {
                uint32 l_Entry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];

                l_GarrisonMgr->AddGarrisonTavernData(l_Entry);
            }
            else
            {
                uint32 l_FirstEntry  = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                uint32 l_SecondEntry = 0;

                do
                    l_SecondEntry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                while (l_SecondEntry == l_FirstEntry);

                l_GarrisonMgr->AddGarrisonTavernData(l_FirstEntry);
                l_GarrisonMgr->AddGarrisonTavernData(l_SecondEntry);
            }

            OnSetPlotInstanceID(GetPlotInstanceID());
            l_GarrisonMgr->UpdatePlot(GetPlotInstanceID());
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS