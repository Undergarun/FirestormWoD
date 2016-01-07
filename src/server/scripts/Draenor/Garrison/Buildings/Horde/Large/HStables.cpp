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

    bool npc_Tormak::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        using namespace StablesData::Horde::TormakQuestGiver;
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
                using namespace StablesData::Horde;
                reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SummonRelativeCreature(g_SagePalunaQuestgiverEntry,
                    g_HordeCreaturesPos[4].X,
                    g_HordeCreaturesPos[4].Y,
                    g_HordeCreaturesPos[4].Z,
                    g_HordeCreaturesPos[4].O,
                    TEMPSUMMON_MANUAL_DESPAWN);
            }
        }

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

    bool npc_Tormak::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        using namespace StablesData::Horde::TormakQuestGiver;

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

    void npc_TormakAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        Player* l_Owner = GetOwner();

        if (l_Owner == nullptr)
            return;

        PlayerSpellMap &l_SpellMap = l_Owner->GetSpellMap();
        std::vector<uint32> l_MountEntries;
        std::list<Creature*> l_CreatureList;

        me->GetCreatureListInGrid(l_CreatureList, 12.0f);

        l_CreatureList.remove_if([](Creature* p_Creature) -> bool
        {
            if (p_Creature->GetEntry() == g_TormakQuestgiverEntry)
                return true;

            return false;
        });

        for (Creature* l_Creature : l_CreatureList)
            l_Creature->DespawnOrUnsummon();

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

        uint32 l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

        using namespace StablesData::Horde;

        SummonRelativeCreature(l_MountEntry, g_HordeCreaturesPos[0].X, g_HordeCreaturesPos[0].Y, g_HordeCreaturesPos[0].Z, g_HordeCreaturesPos[0].O, TEMPSUMMON_MANUAL_DESPAWN);

        l_MountEntries.erase(std::remove(l_MountEntries.begin(), l_MountEntries.end(), l_MountEntry), l_MountEntries.end());
        l_MountEntry = 0;

        if (!l_MountEntries.empty())
            l_MountEntry = l_MountEntries[urand(0, l_MountEntries.size() - 1)];

        if (l_MountEntry)
            SummonRelativeCreature(l_MountEntry, g_HordeCreaturesPos[1].X, g_HordeCreaturesPos[1].Y, g_HordeCreaturesPos[1].Z, g_HordeCreaturesPos[1].O, TEMPSUMMON_MANUAL_DESPAWN);

        using namespace StablesData::Horde::TormakQuestGiver;
        using namespace StablesData::Horde::SagePalunaQuestGiver;
        
        if (uint64 l_QuestID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonStablesFirstQuest))
        {
            if (!l_QuestID)
                return;

            if (Creature* l_FirstCreature = SummonRelativeCreature(305, g_HordeCreaturesPos[2].X, g_HordeCreaturesPos[2].Y, g_HordeCreaturesPos[2].Z, g_HordeCreaturesPos[2].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
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

            if (Creature* l_SecondCreature = SummonRelativeCreature(305, g_HordeCreaturesPos[3].X, g_HordeCreaturesPos[3].Y, g_HordeCreaturesPos[3].Z, g_HordeCreaturesPos[3].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
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

        if (GetClosestCreatureWithEntry(me, g_SagePalunaQuestgiverEntry, 200.0f) == nullptr && l_Owner->IsQuestRewarded(StablesData::Horde::TormakQuestGiver::ClefthoofQuests::QuestCapturingAClefthoof))
            SummonRelativeCreature(g_SagePalunaQuestgiverEntry,
            g_HordeCreaturesPos[4].X,
            g_HordeCreaturesPos[4].Y,
            g_HordeCreaturesPos[4].Z,
            g_HordeCreaturesPos[4].O,
            TEMPSUMMON_MANUAL_DESPAWN);
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

    bool npc_SagePaluna::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        using namespace StablesData::Horde::SagePalunaQuestGiver;
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

    bool npc_SagePaluna::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        using namespace StablesData::Horde::SagePalunaQuestGiver;

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

                if (l_Iterator + 1 != g_WolfQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    return true;

                if (l_QuestID == WolfQuests::QuestWanglingAWolf)
                    l_NextQuestID = g_WolfQuests[0];

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
                if (l_Iterator + 1 != g_TalbukQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    l_NextQuestID = WolfQuests::QuestWanglingAWolf;

                if (l_QuestID == TalbukQuests::QuestTamingATalbuk)
                    l_NextQuestID = g_TalbukQuests[0];

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
                if (l_Iterator + 1 != g_RiverbeastQuests.end())
                    l_NextQuestID = *(l_Iterator + 1);
                else
                    l_NextQuestID = TalbukQuests::QuestTamingATalbuk;

                if (l_QuestID == RiverbeastQuests::QuestRequisitionARiverbeast)
                    l_NextQuestID = g_RiverbeastQuests[0];

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