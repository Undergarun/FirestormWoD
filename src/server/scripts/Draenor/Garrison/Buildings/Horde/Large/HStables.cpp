////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HStables.hpp"
#include "ScriptMgr.h"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 86979 - Tormak                                                     ///
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

    bool npc_Tormak::OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
    {
        GarrisonNPCAI* l_AI = p_Creature->GetAI() ? dynamic_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
            l_GarrisonMgr->UpdatePlot(l_AI->GetPlotInstanceID());

        return true;
    }

    bool npc_Tormak::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        using namespace StablesData::Horde::TormakQuestGiver;
        uint32 l_QuestID = p_Quest->GetQuestId();
        GarrisonNPCAI* l_AI = p_Creature->GetAI() ? dynamic_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        if (std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID) != g_BoarQuests.end() ||
            std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID) != g_ElekkQuests.end() ||
            std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID) != g_ClefthoofQuests.end() ||
            l_QuestID == BoarQuests::QuestBestingABoar || l_QuestID == ElekkQuests::QuestEntanglingAnElekk ||
            l_QuestID == ClefthoofQuests::QuestCapturingAClefthoof)
        {
            p_Player->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest, l_QuestID |= StablesData::g_PendingQuestFlag);

            if (l_QuestID == g_BoarQuests.back() && p_Creature->AI())
            {
                using namespace StablesData::Horde;

                l_AI->SummonRelativeCreature(g_SagePalunaQuestgiverEntry,
                    g_HordeCreaturesPos[4].X,
                    g_HordeCreaturesPos[4].Y,
                    g_HordeCreaturesPos[4].Z,
                    g_HordeCreaturesPos[4].O,
                    TEMPSUMMON_MANUAL_DESPAWN);
            }
        }

        if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
            l_GarrisonMgr->UpdatePlot(l_AI->GetPlotInstanceID());

        return true;
    }

    bool npc_Tormak::CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList)
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

    uint32 npc_Tormak::ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID)
    {
        if (p_Player == nullptr)
            return 0;

        uint64 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest);
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

            if (l_Quest != nullptr && p_Player->CanTakeQuest(l_Quest, false))
                p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);
        }

        return l_NextQuestID;
    }

    bool npc_Tormak::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (!p_Creature->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
            return true;

        using namespace StablesData::Horde::TormakQuestGiver;
        bool l_NeedFirstQuest = true;

        std::vector<uint32> l_BasicQuests =
        {
            StablesData::Horde::TormakQuestGiver::BoarQuests::QuestBestingABoar,
            StablesData::Horde::TormakQuestGiver::ElekkQuests::QuestEntanglingAnElekk,
            StablesData::Horde::TormakQuestGiver::ClefthoofQuests::QuestCapturingAClefthoof
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

            if (l_Quest != nullptr && p_Player->CanTakeQuest(l_Quest, false))
                p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);
        }

        p_Player->PlayerTalkClass->SendGossipMenu(1, p_Creature->GetGUID());

        return true;
    }

    void npc_TormakAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea(m_SummonsEntries, 20.0f);
    }

    void npc_TormakAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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

        if (l_MountEntries.empty())
            return;

        uint32 l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

        using namespace StablesData::Horde;

        if (Creature* l_Creature = SummonRelativeCreature(l_MountEntry, g_HordeCreaturesPos[0].X, g_HordeCreaturesPos[0].Y, g_HordeCreaturesPos[0].Z, g_HordeCreaturesPos[0].O, TEMPSUMMON_MANUAL_DESPAWN))
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
            if (Creature* l_Creature = SummonRelativeCreature(l_MountEntry, g_HordeCreaturesPos[1].X, g_HordeCreaturesPos[1].Y, g_HordeCreaturesPos[1].Z, g_HordeCreaturesPos[1].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_Creature->GetEntry());
                l_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }

        using namespace StablesData::Horde::TormakQuestGiver;
        using namespace StablesData::Horde::SagePalunaQuestGiver;

        if (uint64 l_QuestID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest))
        {
            uint32 l_TormakNextQuestID = 0;

            CreatureScript* l_CreatureScript = me->GetCreatureScript();

            if (l_CreatureScript == nullptr)
                return;

            l_TormakNextQuestID = static_cast<npc_Tormak*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_BoarQuests, 0, BoarQuests::QuestBestingABoar);

            if (!l_TormakNextQuestID)
                l_TormakNextQuestID = static_cast<npc_Tormak*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ElekkQuests, BoarQuests::QuestBestingABoar, ElekkQuests::QuestEntanglingAnElekk);

            if (!l_TormakNextQuestID)
                l_TormakNextQuestID = static_cast<npc_Tormak*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ClefthoofQuests, ElekkQuests::QuestEntanglingAnElekk, ClefthoofQuests::QuestCapturingAClefthoof);

            l_Owner->PlayerTalkClass->GetQuestMenu().ClearMenu();

            if (!l_TormakNextQuestID || l_Owner->GetQuestStatus(l_TormakNextQuestID) == QUEST_STATUS_INCOMPLETE)
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            else
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

            if (Creature* l_FirstCreature = SummonRelativeCreature(305, g_HordeCreaturesPos[2].X, g_HordeCreaturesPos[2].Y, g_HordeCreaturesPos[2].Z, g_HordeCreaturesPos[2].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_FirstCreature->GetEntry());

                if (std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID) != g_BoarQuests.end() || std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID | StablesData::g_PendingQuestFlag) != g_BoarQuests.end() || l_QuestID == BoarQuests::QuestBestingABoar)
                    l_FirstCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedRocktusk);
                else if (std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID) != g_ElekkQuests.end() || std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID | StablesData::g_PendingQuestFlag) != g_ElekkQuests.end() || l_QuestID == ElekkQuests::QuestEntanglingAnElekk)
                    l_FirstCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedMeadowstomper);
                else if (std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID) != g_ClefthoofQuests.end()|| std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID | StablesData::g_PendingQuestFlag) != g_ClefthoofQuests.end() || l_QuestID == ClefthoofQuests::QuestCapturingAClefthoof)
                    l_FirstCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedIcehoof);
                else
                    l_FirstCreature->DespawnOrUnsummon();
            }
        }

        if (uint64 l_QuestID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesSecondQuest))
        {
            if (Creature* l_SecondCreature = SummonRelativeCreature(305, g_HordeCreaturesPos[3].X, g_HordeCreaturesPos[3].Y, g_HordeCreaturesPos[3].Z, g_HordeCreaturesPos[3].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_SecondCreature->GetEntry());

                if (std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID) != g_WolfQuests.end() || std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID | StablesData::g_PendingQuestFlag) != g_WolfQuests.end() || l_QuestID == WolfQuests::QuestWanglingAWolf)
                    l_SecondCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedSnarler);
                else if (std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID) != g_TalbukQuests.end() || std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID | StablesData::g_PendingQuestFlag) != g_TalbukQuests.end() || l_QuestID == TalbukQuests::QuestTamingATalbuk)
                    l_SecondCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedSilverpelt);
                else if (std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID) != g_RiverbeastQuests.end() || std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID | StablesData::g_PendingQuestFlag) != g_RiverbeastQuests.end() || l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
                    l_SecondCreature->SetDisplayId(StablesData::MountDisplayIDs::DisplayTrainedRiverwallow);
                else
                    l_SecondCreature->DespawnOrUnsummon();
            }
        }

        if (GetClosestCreatureWithEntry(me, g_SagePalunaQuestgiverEntry, 200.0f) == nullptr && l_Owner->IsQuestRewarded(StablesData::Horde::TormakQuestGiver::ClefthoofQuests::QuestCapturingAClefthoof))
        {
            if (Creature* l_Creature = SummonRelativeCreature(g_SagePalunaQuestgiverEntry,
                g_HordeCreaturesPos[4].X,
                g_HordeCreaturesPos[4].Y,
                g_HordeCreaturesPos[4].Z,
                g_HordeCreaturesPos[4].O,
                TEMPSUMMON_MANUAL_DESPAWN))
            {
                m_SummonsEntries.push_back(l_Creature->GetEntry());

                uint32 l_PalunaNextQuestID = 0;

                CreatureScript* l_CreatureScript = me->GetCreatureScript();

                if (l_CreatureScript == nullptr)
                    return;

                l_PalunaNextQuestID = static_cast<npc_Tormak*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_BoarQuests, 0, BoarQuests::QuestBestingABoar);

                if (!l_PalunaNextQuestID)
                    l_PalunaNextQuestID = static_cast<npc_Tormak*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ElekkQuests, BoarQuests::QuestBestingABoar, ElekkQuests::QuestEntanglingAnElekk);

                if (!l_PalunaNextQuestID)
                    l_PalunaNextQuestID = static_cast<npc_Tormak*>(l_CreatureScript)->ProceedQuestSelection(l_Owner, me, g_ClefthoofQuests, ElekkQuests::QuestEntanglingAnElekk, ClefthoofQuests::QuestCapturingAClefthoof);

                l_Owner->PlayerTalkClass->GetQuestMenu().ClearMenu();


                if (!l_PalunaNextQuestID || l_Owner->GetQuestStatus(l_PalunaNextQuestID) == QUEST_STATUS_INCOMPLETE &&
                    (l_Owner->IsQuestRewarded(ClefthoofQuests::QuestCapturingAClefthoof) && l_Owner->IsQuestRewarded(RiverbeastQuests::QuestRequisitionARiverbeast)))
                    l_Creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                else
                    l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    /// 87242 - Sage Paluna                                                ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_SagePaluna::npc_SagePaluna()
        : CreatureScript("npc_SagePaluna_Garr")
    {

    }

    /// Constructor
    npc_SagePalunaAI::npc_SagePalunaAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_SagePaluna::GetAI(Creature* p_Creature) const
    {
        return new npc_SagePalunaAI(p_Creature);
    }

    bool npc_SagePaluna::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        using namespace StablesData::Horde::SagePalunaQuestGiver;
        GarrisonNPCAI* l_AI = p_Creature->GetAI() ? dynamic_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true; 

        uint32 l_QuestID = p_Quest->GetQuestId();

        if (std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID) != g_WolfQuests.end() ||
            std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID) != g_TalbukQuests.end() ||
            std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID) != g_RiverbeastQuests.end() ||
            l_QuestID == WolfQuests::QuestWanglingAWolf || l_QuestID == TalbukQuests::QuestTamingATalbuk ||
            l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
        {
            p_Player->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonStablesSecondQuest, l_QuestID |= StablesData::g_PendingQuestFlag);
        }

        if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
            l_GarrisonMgr->UpdatePlot(l_AI->GetPlotInstanceID());

        return true;
    }

    bool npc_SagePaluna::CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList)
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

    void npc_SagePaluna::ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID)
    {
        if (p_Player == nullptr)
            return;

        uint64 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesSecondQuest);
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

            if (l_Quest != nullptr && p_Player->CanTakeQuest(l_Quest, false))
                p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);

            p_Player->PlayerTalkClass->SendGossipMenu(1, p_Creature->GetGUID());
        }
    }

    bool npc_SagePaluna::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (!p_Creature->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
            return true;

        using namespace StablesData::Horde::SagePalunaQuestGiver;
        bool l_NeedFirstQuest = true;

        std::vector<uint32> l_BasicQuests =
        {
            StablesData::Horde::SagePalunaQuestGiver::WolfQuests::QuestWanglingAWolf,
            StablesData::Horde::SagePalunaQuestGiver::TalbukQuests::QuestTamingATalbuk,
            StablesData::Horde::SagePalunaQuestGiver::RiverbeastQuests::QuestRequisitionARiverbeast
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

            if (l_Quest != nullptr && p_Player->CanTakeQuest(l_Quest, false))
                p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);
        }

        p_Player->PlayerTalkClass->SendGossipMenu(1, p_Creature->GetGUID());

        return true;
    }
}   ///< namespace Garrison
}   ///< namespace MS