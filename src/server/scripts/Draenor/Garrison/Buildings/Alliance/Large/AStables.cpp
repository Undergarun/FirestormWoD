////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AStables.hpp"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 86974 - Fanny Firebeard                                            ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FannyFirebeard::npc_FannyFirebeard()
        : CreatureScript("npc_FannyFirebeard_Garr")
    {

    }

    /// Constructor
    npc_FannyFirebeardAI::npc_FannyFirebeardAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_FannyFirebeard::GetAI(Creature* p_Creature) const
    {
        return new npc_FannyFirebeardAI(p_Creature);
    }

    bool npc_FannyFirebeard::OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* /*p_Quest*/)
    {
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
            l_GarrisonMgr->UpdatePlot(l_AI->GetPlotInstanceID());

        return true;
    }

    bool npc_FannyFirebeard::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/)
    {
        using namespace StablesData::Alliance::FannyQuestGiver;
        uint32 l_QuestID = p_Quest->GetQuestId();
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        if (std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID) != g_BoarQuests.end() ||
            std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID) != g_ElekkQuests.end() ||
            std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID) != g_ClefthoofQuests.end() ||
            l_QuestID == BoarQuests::QuestBestingABoar || l_QuestID == ElekkQuests::QuestEntanglingAnElekk ||
            l_QuestID == ClefthoofQuests::QuestCapturingAClefthoof)
        {
            p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonStablesFirstQuest, l_QuestID |= StablesData::g_PendingQuestFlag);

            if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
                l_GarrisonMgr->UpdatePlot(l_AI->GetPlotInstanceID());
        }

        return true;
    }

    bool npc_FannyFirebeard::CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList)
    {
        for (std::vector<uint32>::const_iterator l_Itr = p_QuestList.begin(); l_Itr != p_QuestList.end(); l_Itr++)
        {
            if (p_Player->GetQuestStatus(*l_Itr) == QUEST_STATUS_COMPLETE)
            {
                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(*l_Itr);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }

        return false;
    }

    uint32 npc_FannyFirebeard::ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID)
    {
        if (p_Player == nullptr)
            return 0;

        uint64 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonStablesFirstQuest);
        std::vector<uint32>::const_iterator l_Iterator = std::find(p_QuestsList.begin(), p_QuestsList.end(), l_QuestID);
        uint32 l_NextQuestID = 0;

        if (!l_QuestID)
            return 0;

        if ((std::find(p_QuestsList.begin(), p_QuestsList.end(), l_QuestID) == p_QuestsList.end() && l_QuestID != p_FirstQuestID) ||
            l_QuestID == (p_FirstQuestID | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
            return 0;
        else if (l_Iterator != p_QuestsList.end() || l_QuestID == p_FirstQuestID)
        {
            if (l_QuestID == p_FirstQuestID)
                l_NextQuestID = p_QuestsList[0];
            else if (l_Iterator + 1 != p_QuestsList.end())
                l_NextQuestID = *(l_Iterator + 1);
            else
                l_NextQuestID = p_NextListQuestID;

            if (!l_NextQuestID)
                return 0;

            Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

            if (l_Quest != nullptr)
                p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        }

        return l_NextQuestID;
    }

    bool npc_FannyFirebeard::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (!p_Creature->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
            return true;

        using namespace StablesData::Alliance::FannyQuestGiver;
        bool l_NeedFirstQuest = true;

        std::vector<uint32> l_BasicQuests =
        {
            StablesData::Alliance::FannyQuestGiver::BoarQuests::QuestBestingABoar,
            StablesData::Alliance::FannyQuestGiver::ElekkQuests::QuestEntanglingAnElekk,
            StablesData::Alliance::FannyQuestGiver::ClefthoofQuests::QuestCapturingAClefthoof
        };

        if (CheckRewardQuest(p_Player, p_Creature, g_BoarQuests) ||
            CheckRewardQuest(p_Player, p_Creature, g_ElekkQuests) ||
            CheckRewardQuest(p_Player, p_Creature, g_ClefthoofQuests) ||
            CheckRewardQuest(p_Player, p_Creature, l_BasicQuests))
            return true;

        /// Boar questline tests
        if (p_Player->IsQuestRewarded(BoarQuests::QuestBestingABoar))
        {
            ProceedQuestSelection(p_Player, p_Creature, g_BoarQuests, 0, BoarQuests::QuestBestingABoar);
            l_NeedFirstQuest = false;
        }
        /// Elekk questline tests
        else if (p_Player->IsQuestRewarded(ElekkQuests::QuestEntanglingAnElekk))
        {
            ProceedQuestSelection(p_Player, p_Creature, g_ElekkQuests, BoarQuests::QuestBestingABoar, ElekkQuests::QuestEntanglingAnElekk);
            l_NeedFirstQuest = false;
        }
        /// Clefthoof questline test
        else if (p_Player->IsQuestRewarded(ClefthoofQuests::QuestCapturingAClefthoof))
        {
            ProceedQuestSelection(p_Player, p_Creature, g_ClefthoofQuests, ElekkQuests::QuestEntanglingAnElekk, ClefthoofQuests::QuestCapturingAClefthoof);
            l_NeedFirstQuest = false;
        }

        if (l_NeedFirstQuest)
        {
            Quest const* l_Quest = sObjectMgr->GetQuestTemplate(ClefthoofQuests::QuestCapturingAClefthoof);

            if (l_Quest != nullptr)
                p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);
        }

        p_Player->PlayerTalkClass->SendGossipMenu(1, p_Creature->GetGUID());

        return true;
    }

    void npc_FannyFirebeardAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea(m_SummonsEntries, 20.0f);
    }

    void npc_FannyFirebeardAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
    {
        Player* l_Owner = GetOwner();

        if (l_Owner == nullptr)
            return;

        MS::Garrison::Manager* l_GarrisonMgr = l_Owner->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;

        switch (l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuildingWithType(BuildingType::Stable)))
        {
            case 1:
                ProcessSummonPlotCreatures(-1);
                break;
            case 2:
                ProcessSummonPlotCreatures(4);
                break;
            case 3:
                ProcessSummonPlotCreatures(9);
                break;
            default:
                break;
        }
    }

    void npc_FannyFirebeardAI::ProcessSummonPlotCreatures(int l_Index)
    {
        Player* l_Owner = GetOwner();

        if (l_Owner == nullptr)
            return;

        me->DespawnCreaturesInArea(m_SummonsEntries, 20.0f);

        PlayerSpellMap &l_SpellMap = l_Owner->GetSpellMap();
        std::vector<uint32> l_MountEntries;

        if (!l_SpellMap.empty())
        {
            for (auto l_Spell : l_SpellMap)
            {
                if (SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_Spell.first))
                {
                    if (l_SpellInfo->HasAura(AuraType::SPELL_AURA_MOUNTED) && l_SpellInfo->Effects[0].MiscValue != 305) ///< White Stallion, Generic entry for mounts in DBC with specific displayID
                        l_MountEntries.push_back(l_SpellInfo->Effects[0].MiscValue);
                }
            }
        }

        using namespace StablesData::Alliance;

        if (!l_MountEntries.empty())
        {
            uint32 l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

            if (Creature* l_Creature = SummonRelativeCreature(l_MountEntry, g_AllianceCreaturesPos[++l_Index], TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_Creature->GetEntry());
                l_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            l_MountEntries.erase(std::remove(l_MountEntries.begin(), l_MountEntries.end(), l_MountEntry), l_MountEntries.end());
            l_MountEntry = 0;

            if (!l_MountEntries.empty())
                l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

            if (l_MountEntry)
            {
                if (Creature* l_Creature = SummonRelativeCreature(l_MountEntry, g_AllianceCreaturesPos[++l_Index], TEMPSUMMON_MANUAL_DESPAWN))
                {
                    m_SummonsEntries.push_back(l_Creature->GetEntry());
                    l_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }
        }

        using namespace StablesData::Alliance::FannyQuestGiver;
        using namespace StablesData::Alliance::KeeganQuestGiver;

        if (uint64 l_QuestID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonStablesFirstQuest))
        {
            uint32 l_TormakNextQuestID = 0;

            CreatureScript* l_CreatureScript = me->GetCreatureScript();

            if (l_CreatureScript == nullptr)
                return;

            l_TormakNextQuestID = static_cast<npc_FannyFirebeard*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_BoarQuests, 0, BoarQuests::QuestBestingABoar);

            if (!l_TormakNextQuestID)
                l_TormakNextQuestID = static_cast<npc_FannyFirebeard*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ElekkQuests, BoarQuests::QuestBestingABoar, ElekkQuests::QuestEntanglingAnElekk);

            if (!l_TormakNextQuestID)
                l_TormakNextQuestID = static_cast<npc_FannyFirebeard*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ClefthoofQuests, ElekkQuests::QuestEntanglingAnElekk, ClefthoofQuests::QuestCapturingAClefthoof);

            l_Owner->PlayerTalkClass->GetQuestMenu().ClearMenu();

            if (!l_TormakNextQuestID || l_Owner->GetQuestStatus(l_TormakNextQuestID) == QUEST_STATUS_INCOMPLETE)
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            else
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            
            if (Creature* l_FirstCreature = SummonRelativeCreature(305, g_AllianceCreaturesPos[++l_Index], TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_FirstCreature->GetEntry());

                if (std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID) != g_BoarQuests.end() || l_QuestID == BoarQuests::QuestBestingABoar)
                    l_FirstCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedRocktusk);
                else if (std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID) != g_ElekkQuests.end() || l_QuestID == ElekkQuests::QuestEntanglingAnElekk)
                    l_FirstCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedMeadowstomper);
                else if (std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID) != g_ClefthoofQuests.end() || l_QuestID == ClefthoofQuests::QuestCapturingAClefthoof)
                    l_FirstCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedIcehoof);
                else
                    l_FirstCreature->DespawnOrUnsummon();
            }
        }

        if (uint64 l_QuestID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonStablesSecondQuest))
        {
            if (Creature* l_SecondCreature = SummonRelativeCreature(305, g_AllianceCreaturesPos[++l_Index], TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_SecondCreature->GetEntry());

                if (std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID) != g_WolfQuests.end() || l_QuestID == WolfQuests::QuestWanglingAWolf)
                    l_SecondCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedSnarler);
                else if (std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID) != g_TalbukQuests.end() || l_QuestID == TalbukQuests::QuestTamingATalbuk)
                    l_SecondCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedSilverpelt);
                else if (std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID) != g_RiverbeastQuests.end() || l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
                    l_SecondCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedRiverwallow);
                else
                    l_SecondCreature->DespawnOrUnsummon();
            }
        }

        if (GetClosestCreatureWithEntry(me, g_KeeganQuestgiverEntry, 200.0f) == nullptr && l_Owner->IsQuestRewarded(StablesData::Alliance::FannyQuestGiver::ClefthoofQuests::QuestCapturingAClefthoof))
        {
            if (Creature* l_Creature = SummonRelativeCreature(uint32(g_KeeganQuestgiverEntry), g_AllianceCreaturesPos[++l_Index], TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_Creature->GetEntry());

                uint32 l_KeeganNextQuestID = 0;

                CreatureScript* l_CreatureScript = me->GetCreatureScript();

                if (l_CreatureScript == nullptr)
                    return;

                l_KeeganNextQuestID = static_cast<npc_FannyFirebeard*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_BoarQuests, 0, WolfQuests::QuestWanglingAWolf);

                if (!l_KeeganNextQuestID)
                    l_KeeganNextQuestID = static_cast<npc_FannyFirebeard*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ElekkQuests, WolfQuests::QuestWanglingAWolf, TalbukQuests::QuestTamingATalbuk);

                if (!l_KeeganNextQuestID)
                    l_KeeganNextQuestID = static_cast<npc_FannyFirebeard*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ClefthoofQuests, TalbukQuests::QuestTamingATalbuk, RiverbeastQuests::QuestRequisitionARiverbeast);

                l_Owner->PlayerTalkClass->GetQuestMenu().ClearMenu();

                if (!l_KeeganNextQuestID || l_Owner->GetQuestStatus(l_KeeganNextQuestID) == QUEST_STATUS_INCOMPLETE)
                    l_Creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                else
                    l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    /// 86973 - Keegan Firebeard                                           ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_KeeganFirebeard::npc_KeeganFirebeard()
        : CreatureScript("npc_KeeganFirebeard_Garr")
    {

    }

    bool npc_KeeganFirebeard::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/)
    {
        using namespace StablesData::Alliance::KeeganQuestGiver;
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        uint32 l_QuestID = p_Quest->GetQuestId();

        if (std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID) != g_WolfQuests.end() ||
            std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID) != g_TalbukQuests.end() ||
            std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID) != g_RiverbeastQuests.end() ||
            l_QuestID == WolfQuests::QuestWanglingAWolf || l_QuestID == TalbukQuests::QuestTamingATalbuk ||
            l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
        {
            p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonStablesSecondQuest, l_QuestID |= StablesData::g_PendingQuestFlag);
        }

        if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
            l_GarrisonMgr->UpdatePlot(l_AI->GetPlotInstanceID());

        return true;
    }

    bool npc_KeeganFirebeard::CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList)
    {
        for (std::vector<uint32>::const_iterator l_Itr = p_QuestList.begin(); l_Itr != p_QuestList.end(); l_Itr++)
        {
            if (p_Player->GetQuestStatus(*l_Itr) == QUEST_STATUS_COMPLETE)
            {
                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(*l_Itr);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }

        return false;
    }

    void npc_KeeganFirebeard::ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID)
    {
        if (p_Player == nullptr)
            return;

        uint64 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonStablesSecondQuest);
        std::vector<uint32>::const_iterator l_Iterator = std::find(p_QuestsList.begin(), p_QuestsList.end(), l_QuestID);
        uint32 l_NextQuestID = 0;

        if (!l_QuestID)
            return;

        if ((std::find(p_QuestsList.begin(), p_QuestsList.end(), l_QuestID) == p_QuestsList.end() && l_QuestID != p_FirstQuestID) ||
            l_QuestID == (p_FirstQuestID | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
            return;
        else if (l_Iterator != p_QuestsList.end() || l_QuestID == p_FirstQuestID)
        {
            if (l_QuestID == p_FirstQuestID)
                l_NextQuestID = p_QuestsList[0];
            else if (l_Iterator + 1 != p_QuestsList.end())
                l_NextQuestID = *(l_Iterator + 1);
            else
                l_NextQuestID = p_NextListQuestID;

            if (!l_NextQuestID)
                return;

            Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

            if (l_Quest != nullptr)
                p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

            p_Player->PlayerTalkClass->SendGossipMenu(1, p_Creature->GetGUID());
        }
    }

    bool npc_KeeganFirebeard::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (!p_Creature->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
            return true;

        using namespace StablesData::Alliance::KeeganQuestGiver;
        bool l_NeedFirstQuest = true;

        std::vector<uint32> l_BasicQuests =
        {
            StablesData::Alliance::KeeganQuestGiver::WolfQuests::QuestWanglingAWolf,
            StablesData::Alliance::KeeganQuestGiver::TalbukQuests::QuestTamingATalbuk,
            StablesData::Alliance::KeeganQuestGiver::RiverbeastQuests::QuestRequisitionARiverbeast
        };

        if (CheckRewardQuest(p_Player, p_Creature, g_WolfQuests) ||
            CheckRewardQuest(p_Player, p_Creature, g_TalbukQuests) ||
            CheckRewardQuest(p_Player, p_Creature, g_RiverbeastQuests) ||
            CheckRewardQuest(p_Player, p_Creature, l_BasicQuests))
            return true;

        /// Wolf questline tests
        if (p_Player->IsQuestRewarded(WolfQuests::QuestWanglingAWolf))
        {
            ProceedQuestSelection(p_Player, p_Creature, g_WolfQuests, 0, WolfQuests::QuestWanglingAWolf);
            l_NeedFirstQuest = false;
        }
        /// Talbuk questline tests
        else if (p_Player->IsQuestRewarded(TalbukQuests::QuestTamingATalbuk))
        {
            ProceedQuestSelection(p_Player, p_Creature, g_TalbukQuests, WolfQuests::QuestWanglingAWolf, TalbukQuests::QuestTamingATalbuk);
            l_NeedFirstQuest = false;
        }
        /// Riverbeast questline test
        else if (p_Player->IsQuestRewarded(RiverbeastQuests::QuestRequisitionARiverbeast))
        {
            ProceedQuestSelection(p_Player, p_Creature, g_RiverbeastQuests, TalbukQuests::QuestTamingATalbuk, RiverbeastQuests::QuestRequisitionARiverbeast);
            l_NeedFirstQuest = false;
        }

        if (l_NeedFirstQuest)
        {
            Quest const* l_Quest = sObjectMgr->GetQuestTemplate(RiverbeastQuests::QuestRequisitionARiverbeast);

            if (l_Quest != nullptr)
                p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        }

        p_Player->PlayerTalkClass->SendGossipMenu(1, p_Creature->GetGUID());

        return true;
    }
}   ///< namespace Garrison
}   ///< namespace MS
