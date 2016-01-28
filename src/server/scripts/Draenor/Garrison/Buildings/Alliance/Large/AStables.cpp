////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
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

    bool npc_FannyFirebeard::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        using namespace StablesData::Alliance::FannyQuestGiver;
        uint32 l_QuestID = p_Quest->GetQuestId();

        if (std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID) != g_BoarQuests.end() ||
            std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID) != g_ElekkQuests.end() ||
            std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID) != g_ClefthoofQuests.end() ||
            l_QuestID == BoarQuests::QuestBestingABoar || l_QuestID == ElekkQuests::QuestEntanglingAnElekk ||
            l_QuestID == ClefthoofQuests::QuestCapturingAClefthoof)
        {
            p_Player->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest, l_QuestID |= StablesData::g_PendingQuestFlag);

            if (l_QuestID == g_BoarQuests.back() && p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName())
            {
                using namespace StablesData::Alliance;
                reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SummonRelativeCreature(g_KeeganQuestgiverEntry,
                    g_AllianceCreaturesPos[4].X,
                    g_AllianceCreaturesPos[4].Y,
                    g_AllianceCreaturesPos[4].Z,
                    g_AllianceCreaturesPos[4].O,
                    TEMPSUMMON_MANUAL_DESPAWN);
            }
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

    bool npc_FannyFirebeard::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        using namespace StablesData::Alliance::FannyQuestGiver;

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

        if (p_Player->IsQuestRewarded(BoarQuests::QuestBestingABoar))
        {
            uint32 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest);
            std::vector<uint32>::const_iterator l_Iterator = std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID);
            uint32 l_NextQuestID = 0;

            if (!l_QuestID)
                return true;

            /// Boar questline tests
            if ((std::find(g_BoarQuests.begin(), g_BoarQuests.end(), l_QuestID) == g_BoarQuests.end() && l_QuestID != BoarQuests::QuestBestingABoar) ||
                l_QuestID == (BoarQuests::QuestBestingABoar | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
                return true;
            else if (l_Iterator != g_BoarQuests.end() || l_QuestID == BoarQuests::QuestBestingABoar)
            {

                if (l_Iterator + 1 != g_BoarQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    return true;

                if (l_QuestID == BoarQuests::QuestBestingABoar)
                    l_NextQuestID = g_BoarQuests[0];

                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }
        /// Elekk questline tests
        else if (p_Player->IsQuestRewarded(ElekkQuests::QuestEntanglingAnElekk))
        {
            uint64 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest);
            std::vector<uint32>::const_iterator l_Iterator = std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID);
            uint32 l_NextQuestID = 0;

            if (!l_QuestID)
                return true;

            if ((std::find(g_ElekkQuests.begin(), g_ElekkQuests.end(), l_QuestID) == g_ElekkQuests.end() && l_QuestID != ElekkQuests::QuestEntanglingAnElekk) ||
                l_QuestID == (ElekkQuests::QuestEntanglingAnElekk | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
                return true;
            else if (l_Iterator != g_ElekkQuests.end() || l_QuestID == ElekkQuests::QuestEntanglingAnElekk)
            {
                if (l_Iterator + 1 != g_ElekkQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    l_NextQuestID = BoarQuests::QuestBestingABoar;

                if (l_QuestID == ElekkQuests::QuestEntanglingAnElekk)
                    l_NextQuestID = g_ElekkQuests[0];

                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }
        /// Clefthoof questline test
        else if (p_Player->IsQuestRewarded(ClefthoofQuests::QuestCapturingAClefthoof))
        {
            uint64 l_QuestID     = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest);
            std::vector<uint32>::const_iterator l_Iterator = std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID);
            uint32 l_NextQuestID = 0;

            if (!l_QuestID)
                return true;

            if ((std::find(g_ClefthoofQuests.begin(), g_ClefthoofQuests.end(), l_QuestID) == g_ClefthoofQuests.end() && l_QuestID != ClefthoofQuests::QuestCapturingAClefthoof) ||
                l_QuestID == (ClefthoofQuests::QuestCapturingAClefthoof | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
                return true;
            else if (l_Iterator != g_ClefthoofQuests.end() || l_QuestID == ClefthoofQuests::QuestCapturingAClefthoof)
            {
                if (l_Iterator + 1 != g_ClefthoofQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    l_NextQuestID = ElekkQuests::QuestEntanglingAnElekk;

                if (l_QuestID == ClefthoofQuests::QuestCapturingAClefthoof)
                    l_NextQuestID = g_ClefthoofQuests[0];

                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }

        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(ClefthoofQuests::QuestCapturingAClefthoof);

        if (l_Quest != nullptr)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

        return true;
    }

    void npc_FannyFirebeardAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea(m_SummonsEntries, 20.0f);
    }

    void npc_FannyFirebeardAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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

        using namespace StablesData::Alliance;

        if (Creature* l_Creature = SummonRelativeCreature(l_MountEntry, g_AllianceCreaturesPos[0].X, g_AllianceCreaturesPos[0].Y, g_AllianceCreaturesPos[0].Z, g_AllianceCreaturesPos[0].O, TEMPSUMMON_MANUAL_DESPAWN))
            m_SummonsEntries.push_back(l_Creature->GetEntry());

        l_MountEntries.erase(std::remove(l_MountEntries.begin(), l_MountEntries.end(), l_MountEntry), l_MountEntries.end());
        l_MountEntry = 0;

        if (!l_MountEntries.empty())
            l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

        if (l_MountEntry)
        {
            if (Creature* l_Creature = SummonRelativeCreature(l_MountEntry, g_AllianceCreaturesPos[1].X, g_AllianceCreaturesPos[1].Y, g_AllianceCreaturesPos[1].Z, g_AllianceCreaturesPos[1].O, TEMPSUMMON_MANUAL_DESPAWN))
                m_SummonsEntries.push_back(l_Creature->GetEntry());
        }

        using namespace StablesData::Alliance::FannyQuestGiver;
        using namespace StablesData::Alliance::KeeganQuestGiver;
        
        if (uint64 l_QuestID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest))
        {
            if (!l_QuestID)
                return;

            if (Creature* l_FirstCreature = SummonRelativeCreature(305, g_AllianceCreaturesPos[2].X, g_AllianceCreaturesPos[2].Y, g_AllianceCreaturesPos[2].Z, g_AllianceCreaturesPos[2].O, TEMPSUMMON_MANUAL_DESPAWN))
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

        if (uint64 l_QuestID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesSecondQuest))
        {
            if (!l_QuestID)
                return;

            if (Creature* l_SecondCreature = SummonRelativeCreature(305, g_AllianceCreaturesPos[3].X, g_AllianceCreaturesPos[3].Y, g_AllianceCreaturesPos[3].Z, g_AllianceCreaturesPos[3].O, TEMPSUMMON_MANUAL_DESPAWN))
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
            if (Creature* l_Creature = SummonRelativeCreature(g_KeeganQuestgiverEntry,
                g_AllianceCreaturesPos[4].X,
                g_AllianceCreaturesPos[4].Y,
                g_AllianceCreaturesPos[4].Z,
                g_AllianceCreaturesPos[4].O,
                TEMPSUMMON_MANUAL_DESPAWN))
                m_SummonsEntries.push_back(l_Creature->GetEntry());
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

    bool npc_KeeganFirebeard::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        using namespace StablesData::Alliance::KeeganQuestGiver;
        uint32 l_QuestID = p_Quest->GetQuestId();

        if (std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID) != g_WolfQuests.end() ||
            std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID) != g_TalbukQuests.end() ||
            std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID) != g_RiverbeastQuests.end() ||
            l_QuestID == WolfQuests::QuestWanglingAWolf || l_QuestID == TalbukQuests::QuestTamingATalbuk ||
            l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
        {
            p_Player->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest, l_QuestID |= StablesData::g_PendingQuestFlag);
        }

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

    bool npc_KeeganFirebeard::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        using namespace StablesData::Alliance::KeeganQuestGiver;

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

        if (p_Player->IsQuestRewarded(WolfQuests::QuestWanglingAWolf))
        {
            uint32 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest);
            std::vector<uint32>::const_iterator l_Iterator = std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID);
            uint32 l_NextQuestID = 0;

            if (!l_QuestID)
                return true;

            /// Boar questline tests
            if ((std::find(g_WolfQuests.begin(), g_WolfQuests.end(), l_QuestID) == g_WolfQuests.end() && l_QuestID != WolfQuests::QuestWanglingAWolf) ||
                l_QuestID == (WolfQuests::QuestWanglingAWolf | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
                return true;
            else if (l_Iterator != g_WolfQuests.end() || l_QuestID == WolfQuests::QuestWanglingAWolf)
            {
                if (l_QuestID == WolfQuests::QuestWanglingAWolf)
                    l_NextQuestID = g_WolfQuests[0];
                else if (l_Iterator + 1 != g_WolfQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    return true;

                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }
        /// Elekk questline tests
        else if (p_Player->IsQuestRewarded(TalbukQuests::QuestTamingATalbuk))
        {
            uint64 l_QuestID = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest);
            std::vector<uint32>::const_iterator l_Iterator = std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID);
            uint32 l_NextQuestID = 0;

            if (!l_QuestID)
                return true;

            if ((std::find(g_TalbukQuests.begin(), g_TalbukQuests.end(), l_QuestID) == g_TalbukQuests.end() && l_QuestID != TalbukQuests::QuestTamingATalbuk) ||
                l_QuestID == (TalbukQuests::QuestTamingATalbuk | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
                return true;
            else if (l_Iterator != g_TalbukQuests.end() || l_QuestID == TalbukQuests::QuestTamingATalbuk)
            {
                if (l_QuestID == TalbukQuests::QuestTamingATalbuk)
                    l_NextQuestID = g_TalbukQuests[0];
                else if (l_Iterator + 1 != g_TalbukQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    l_NextQuestID = WolfQuests::QuestWanglingAWolf;

                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }
        /// Clefthoof questline test
        else if (p_Player->IsQuestRewarded(RiverbeastQuests::QuestRequisitionARiverbeast))
        {
            uint64 l_QuestID     = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest);
            std::vector<uint32>::const_iterator l_Iterator = std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID);
            uint32 l_NextQuestID = 0;

            if (!l_QuestID)
                return true;

            if ((std::find(g_RiverbeastQuests.begin(), g_RiverbeastQuests.end(), l_QuestID) == g_RiverbeastQuests.end() && l_QuestID != RiverbeastQuests::QuestRequisitionARiverbeast) ||
                l_QuestID == (RiverbeastQuests::QuestRequisitionARiverbeast | StablesData::g_PendingQuestFlag)) ///< QUEST DIDN'T GOT DAILY RESET YET, SO NEXT QUEST ISN'T OFFERED
                return true;
            else if (l_Iterator != g_RiverbeastQuests.end() || l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
            {
                if (l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
                    l_NextQuestID = g_RiverbeastQuests[0];
                else if (l_Iterator + 1 != g_RiverbeastQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    l_NextQuestID = TalbukQuests::QuestTamingATalbuk;

                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_NextQuestID);

                if (l_Quest != nullptr)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

                return true;
            }
        }

        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(RiverbeastQuests::QuestRequisitionARiverbeast);

        if (l_Quest != nullptr)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());

        return true;
    }
}   ///< namespace Garrison
}   ///< namespace MS