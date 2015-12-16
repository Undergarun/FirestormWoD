////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HStables.hpp"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 91582 - Apprentice Var'Nath                                        ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Tormak::npc_Tormak()
        : CreatureScript("npc_Tormak_Garr")
    {

    }

    /// Constructor
    npc_TormakAI::npc_TormakAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_Tormak::GetAI(Creature* p_Creature) const
    {
        return new npc_TormakAI(p_Creature);
    }

    bool npc_Tormak::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        /// /!\ ALLIANCE DATAS, TEST ALGORITHM ! /!\

        using namespace StablesData::Alliance::FannyQuestGiver;

        if (p_Player->IsQuestRewarded(RiverbeastQuests::QuestRequisitionARiverbeast))
        {
            for (std::vector<uint32>::const_reverse_iterator l_Itr = g_RiverbeastQuests.rbegin(); l_Itr != g_RiverbeastQuests.rend(); l_Itr++)
            {
                if (p_Player->IsQuestRewarded(*l_Itr))
                {
                    if (++l_Itr != g_RiverbeastQuests.rend())
                    {
                        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(*l_Itr);

                        if (l_Quest != nullptr)
                            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
                    }

                    break;
                }
            }
        }
        else if (p_Player->IsQuestRewarded(WolfQuests::QuestWanglingAWolf))
        {
            for (std::vector<uint32>::const_reverse_iterator l_Itr = g_WolfQuests.rbegin(); l_Itr != g_WolfQuests.rend(); l_Itr++)
            {
                if (p_Player->IsQuestRewarded(*l_Itr))
                {
                    if (++l_Itr != g_WolfQuests.rend())
                    {
                        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(*l_Itr);

                        if (l_Quest != nullptr)
                            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
                    }

                    break;
                }
            }
        }
        else if (p_Player->IsQuestRewarded(TalbukQuests::QuestTamingATalbuk))
        {
            for (std::vector<uint32>::const_reverse_iterator l_Itr = g_TalbukQuests.rbegin(); l_Itr != g_TalbukQuests.rend(); l_Itr++)
            {
                if (p_Player->IsQuestRewarded(*l_Itr))
                {
                    if (++l_Itr != g_TalbukQuests.rend())
                    {
                        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(*l_Itr);

                        if (l_Quest != nullptr)
                            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
                    }

                    break;
                }
            }
        }
        return true;
    }

    void npc_TormakAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        std::vector<SequencePosition> l_MountPos =
        {
            { 10.8978f, 6.2676f, 0.4298f, 2.1677f },
            {  6.9427f, 4.4816f, 0.4298f, 1.8300f },
            {  2.3547f, 1.0306f, 0.4298f, 6.1104f }
        };

        Player* l_Owner = GetOwner();

        if (l_Owner == nullptr)
            return;

        PlayerSpellMap &l_SpellMap = l_Owner->GetSpellMap();
        std::vector<uint32> l_MountEntries;

        if (!l_SpellMap.empty())
        {
            for (auto l_Spell : l_SpellMap)
            {
                if (SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_Spell.first))
                {
                    if (l_SpellInfo->HasAura(AuraType::SPELL_AURA_MOUNTED))
                        l_MountEntries.push_back(l_SpellInfo->Effects[0].MiscValue);
                }
            }
        }

        uint32 l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

        SummonRelativeCreature(l_MountEntry, l_MountPos[0].X, l_MountPos[0].Y, l_MountPos[0].Z, l_MountPos[0].O, TEMPSUMMON_MANUAL_DESPAWN);

        l_MountEntries.erase(std::remove(l_MountEntries.begin(), l_MountEntries.end(), l_MountEntry), l_MountEntries.end());
        l_MountEntry = 0;

        if (!l_MountEntries.empty())
            l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

        if (l_MountEntry)
            SummonRelativeCreature(l_MountEntry, l_MountPos[1].X, l_MountPos[1].Y, l_MountPos[1].Z, l_MountPos[1].O, TEMPSUMMON_MANUAL_DESPAWN);

        ///////////////////////////// PENDING MOUNT :( //////////////////////////////////
    }
}   ///< namespace Garrison
}   ///< namespace MS