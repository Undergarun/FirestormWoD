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

    void npc_MadisonClarkAI::OnDataReset()
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
                                               g_QuestGiverAlliancePositions[0].X,
                                               g_QuestGiverAlliancePositions[0].Y,
                                               g_QuestGiverAlliancePositions[0].Z,
                                               g_QuestGiverAlliancePositions[0].O,
                                               TEMPSUMMON_MANUAL_DESPAWN);
                    }
                    else
                    {
                        uint32 l_FirstEntry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                        uint32 l_SecondEntry = 0;

                        SummonRelativeCreature(l_FirstEntry,
                                               g_QuestGiverAlliancePositions[1].X,
                                               g_QuestGiverAlliancePositions[1].Y,
                                               g_QuestGiverAlliancePositions[1].Z,
                                               g_QuestGiverAlliancePositions[1].O,
                                               TEMPSUMMON_MANUAL_DESPAWN);

                        do
                            l_SecondEntry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
                        while (l_SecondEntry == l_FirstEntry);

                        SummonRelativeCreature(l_SecondEntry,
                                               g_QuestGiverAlliancePositions[2].X,
                                               g_QuestGiverAlliancePositions[2].Y,
                                               g_QuestGiverAlliancePositions[2].Z,
                                               g_QuestGiverAlliancePositions[2].O,
                                               TEMPSUMMON_MANUAL_DESPAWN);
                    }
                }
            }
        }

    }

}   ///< namespace Garrison
}   ///< namespace MS