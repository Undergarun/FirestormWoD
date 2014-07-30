/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "GossipDef.h"
#include "QuestDef.h"
#include "ObjectAccessor.h"
#include "Group.h"
#include "Battleground.h"
#include "BattlegroundAV.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"

void WorldSession::HandleQuestgiverStatusQueryOpcode(WorldPacket & recvData)
{
    uint64 guid;

    recvData.readPackGUID(guid);

    uint32 questStatus = DIALOG_STATUS_NONE;
    uint32 defstatus = DIALOG_STATUS_NONE;

    Object* questgiver = ObjectAccessor::GetObjectByTypeMask(*m_Player, guid, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
    if (!questgiver)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "Error in CMSG_QUESTGIVER_STATUS_QUERY, called for not found questgiver (Typeid: %u GUID: %u)", GuidHigh2TypeId(GUID_HIPART(guid)), GUID_LOPART(guid));
        return;
    }

    switch (questgiver->GetTypeId())
    {
        case TYPEID_UNIT:
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_STATUS_QUERY for npc, guid = %u", uint32(GUID_LOPART(guid)));
            Creature* cr_questgiver=questgiver->ToCreature();
            if (!cr_questgiver->IsHostileTo(m_Player))       // do not show quest status to enemies
            {
                questStatus = sScriptMgr->GetDialogStatus(m_Player, cr_questgiver);
                if (questStatus > 6)
                    questStatus = getDialogStatus(m_Player, cr_questgiver, defstatus);
            }
            break;
        }
        case TYPEID_GAMEOBJECT:
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_STATUS_QUERY for GameObject guid = %u", uint32(GUID_LOPART(guid)));
            GameObject* go_questgiver=(GameObject*)questgiver;
            questStatus = sScriptMgr->GetDialogStatus(m_Player, go_questgiver);
            if (questStatus > 6)
                questStatus = getDialogStatus(m_Player, go_questgiver, defstatus);
            break;
        }
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "QuestGiver called for unexpected type %u", questgiver->GetTypeId());
            break;
    }

    //inform client about status of quest
    m_Player->PlayerTalkClass->SendQuestGiverStatus(questStatus, guid);
}

void WorldSession::HandleQuestgiverHelloOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    uint8 bitOrder[8] = {6, 3, 4, 2, 5, 7, 0, 1};
    recvData.ReadBitInOrder(guid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = {0, 2, 6, 1, 4, 7, 5, 3};
    recvData.ReadBytesSeq(guid, byteOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_HELLO npc = %u", GUID_LOPART(guid));

    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleQuestgiverHelloOpcode - Unit (GUID: %u) not found or you can't interact with him.",
            GUID_LOPART(guid));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);
    // Stop the npc if moving
    creature->StopMoving();

    if (sScriptMgr->OnGossipHello(m_Player, creature))
        return;

    m_Player->PrepareGossipMenu(creature, creature->GetCreatureTemplate()->GossipMenuId, true);
    m_Player->SendPreparedGossip(creature);

    creature->AI()->sGossipHello(m_Player);
}

void WorldSession::HandleQuestgiverAcceptQuestOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 questId;
    uint8 unk1;

    recvData >> questId;

    guid[2] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    unk1 = recvData.ReadBit();
    guid[6] = recvData.ReadBit();

    uint8 byteOrder[8] = {6, 0, 3, 4, 7, 5, 2, 1};
    recvData.ReadBytesSeq(guid, byteOrder);

    Object* object = ObjectAccessor::GetObjectByTypeMask(*m_Player, guid, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT|TYPEMASK_ITEM|TYPEMASK_PLAYER);

    if (!object || object == m_Player)
        return;

    // no or incorrect quest giver
    if ((object->GetTypeId() != TYPEID_PLAYER && !object->hasQuest(questId)) ||
        (object->GetTypeId() == TYPEID_PLAYER && object != m_Player && !object->ToPlayer()->CanShareQuest(questId)))
    {
        m_Player->PlayerTalkClass->SendCloseGossip();
        m_Player->SaveToDB();
        m_Player->SetDivider(0);
        return;
    }

    if (object && object->GetTypeId() == TYPEID_PLAYER && object->ToPlayer()->GetQuestStatus(questId) == QUEST_STATUS_NONE)
        return;

    // some kind of WPE protection
    if (!m_Player->CanInteractWithQuestGiver(object))
        return;

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
    {
        // prevent cheating
        if (!GetPlayer()->CanTakeQuest(quest, true))
        {
            m_Player->PlayerTalkClass->SendCloseGossip();
            m_Player->SetDivider(0);
            return;
        }

        if (object && object->GetTypeId() == TYPEID_PLAYER && !quest->HasFlag(QUEST_FLAGS_SHARABLE))
            return;

        if (m_Player->GetDivider() != 0)
        {
            Player* player = ObjectAccessor::FindPlayer(m_Player->GetDivider());
            if (!player)
            {
                m_Player->SetDivider(0);
                return;
            }

            if (player)
            {
                if (!player->CanShareQuest(questId))
                {
                    player->SendPushToPartyResponse(m_Player, QUEST_PARTY_MSG_CANT_TAKE_QUEST);
                    m_Player->SetDivider(0);
                    return;
                }
                player->SendPushToPartyResponse(m_Player, QUEST_PARTY_MSG_ACCEPT_QUEST);
                m_Player->SetDivider(0);
            }
        }

        if (m_Player->CanAddQuest(quest, true))
        {
            m_Player->AddQuest(quest, object);

            if (quest->HasFlag(QUEST_FLAGS_PARTY_ACCEPT))
            {
                if (Group* group = m_Player->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* player = itr->getSource();

                        if (!player || player == m_Player)     // not self
                            continue;

                        if (player->CanTakeQuest(quest, true))
                        {
                            player->SetDivider(m_Player->GetGUID());

                            //need confirmation that any gossip window will close
                            player->PlayerTalkClass->SendCloseGossip();

                            m_Player->SendQuestConfirmAccept(quest, player);
                        }
                    }
                }
            }


            if (m_Player->CanCompleteQuest(questId))
                m_Player->CompleteQuest(questId);

            switch (object->GetTypeId())
            {
                case TYPEID_UNIT:
                    sScriptMgr->OnQuestAccept(m_Player, (object->ToCreature()), quest);
                    (object->ToCreature())->AI()->sQuestAccept(m_Player, quest);
                    break;
                case TYPEID_ITEM:
                case TYPEID_CONTAINER:
                {
                    sScriptMgr->OnQuestAccept(m_Player, ((Item*)object), quest);

                    // destroy not required for quest finish quest starting item
                    bool destroyItem = true;
                    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
                    {
                        if ((quest->RequiredItemId[i] == ((Item*)object)->GetEntry()) && (((Item*)object)->GetTemplate()->MaxCount > 0))
                        {
                            destroyItem = false;
                            break;
                        }
                    }

                    if (destroyItem)
                        m_Player->DestroyItem(((Item*)object)->GetBagSlot(), ((Item*)object)->GetSlot(), true);

                    break;
                }
                case TYPEID_GAMEOBJECT:
                    sScriptMgr->OnQuestAccept(m_Player, ((GameObject*)object), quest);
                    (object->ToGameObject())->AI()->QuestAccept(m_Player, quest);
                    break;
                default:
                    break;
            }
            m_Player->PlayerTalkClass->SendCloseGossip();

            if (quest->GetSrcSpell() > 0)
                m_Player->CastSpell(m_Player, quest->GetSrcSpell(), true);

            if (quest->IsAutoComplete())
            {
                // Add quest items for quests that require items
                for (uint8 x = 0; x < QUEST_ITEM_OBJECTIVES_COUNT; ++x)
                {
                    uint32 id = quest->RequiredItemId[x];
                    uint32 count = quest->RequiredItemCount[x];
                    if (!id || !count)
                        continue;

                    uint32 curItemCount = m_Player->GetItemCount(id, true);

                    ItemPosCountVec dest;
                    uint8 msg = m_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count-curItemCount);
                    if (msg == EQUIP_ERR_OK)
                    {
                        Item* item = m_Player->StoreNewItem(dest, id, true);
                        m_Player->SendNewItem(item, count-curItemCount, true, false);
                    }
                }

                // All creature/GO slain/casted (not required, but otherwise it will display "Creature slain 0/10")
                for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
                {
                    int32 creature = quest->RequiredNpcOrGo[i];
                    uint32 creaturecount = quest->RequiredNpcOrGoCount[i];

                    if (uint32 spell_id = quest->RequiredSpellCast[i])
                    {
                        for (uint16 z = 0; z < creaturecount; ++z)
                            if (creature > 0)
                                m_Player->CastedCreatureOrGOForQuest(creature, true, spell_id);
                            else
                                m_Player->CastedCreatureOrGOForQuest(creature, false, spell_id);
                    }
                    else if (creature > 0)
                    {
                        if (CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature))
                            for (uint16 z = 0; z < creaturecount; ++z)
                                m_Player->KilledMonster(cInfo, 0);
                    }
                    else if (creature < 0)
                    {
                        for (uint16 z = 0; z < creaturecount; ++z)
                            m_Player->CastedCreatureOrGO(creature, 0, 0);
                    }
                }

                // If the quest requires currency to complete
                for (uint8 y = 0; y < QUEST_REQUIRED_CURRENCY_COUNT; y++)
                {
                    uint32 currency = quest->RequiredCurrencyId[y];
                    uint32 currencyCount = quest->RequiredCurrencyCount[y];

                    if (!currency || !currencyCount)
                        continue;

                    m_Player->ModifyCurrency(currency, currencyCount);
                }

                // If the quest requires reputation to complete
                if (uint32 repFaction = quest->GetRepObjectiveFaction())
                {
                    uint32 repValue = quest->GetRepObjectiveValue();
                    uint32 curRep = m_Player->GetReputationMgr().GetReputation(repFaction);
                    if (curRep < repValue)
                        if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                            m_Player->GetReputationMgr().SetReputation(factionEntry, repValue);
                }

                // If the quest requires a SECOND reputation to complete
                if (uint32 repFaction = quest->GetRepObjectiveFaction2())
                {
                    uint32 repValue2 = quest->GetRepObjectiveValue2();
                    uint32 curRep = m_Player->GetReputationMgr().GetReputation(repFaction);
                    if (curRep < repValue2)
                        if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                            m_Player->GetReputationMgr().SetReputation(factionEntry, repValue2);
                }

                // If the quest requires money
                int32 ReqOrRewMoney = quest->GetRewOrReqMoney();
                if (ReqOrRewMoney < 0)
                    m_Player->ModifyMoney(-ReqOrRewMoney);

                m_Player->CompleteQuest(quest->GetQuestId());
            }

            return;
        }
    }

    m_Player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleQuestgiverQueryQuestOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Guid;
    uint32 l_QuestId;
    bool l_RespondToGiver;

    p_RecvData.readPackGUID(l_Guid);
    p_RecvData >> l_QuestId;
    l_RespondToGiver = p_RecvData.ReadBit();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_QUERY_QUEST npc = %u, quest = %u, RespondToGiver = %u", uint32(GUID_LOPART(l_Guid)), l_QuestId, l_RespondToGiver);

    // Verify that the guid is valid and is a questgiver or involved in the requested quest
    Object* object = ObjectAccessor::GetObjectByTypeMask(*m_Player, l_Guid, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT | TYPEMASK_ITEM);
    if (!object || (!object->hasQuest(l_QuestId) && !object->hasInvolvedQuest(l_QuestId)))
    {
        m_Player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(l_QuestId))
    {
        // not sure here what should happen to quests with QUEST_FLAGS_AUTOCOMPLETE
        // if this breaks them, add && object->GetTypeId() == TYPEID_ITEM to this check
        // item-started quests never have that flag
        if (m_Player->GetQuestStatus(l_QuestId) == QUEST_STATUS_NONE && !m_Player->CanTakeQuest(quest, true))
            return;

        if ( m_Player->GetQuestStatus(l_QuestId) == QUEST_STATUS_COMPLETE)
            m_Player->PlayerTalkClass->SendQuestGiverOfferReward(quest, object->GetGUID(), true);
        else if (m_Player->GetQuestStatus(l_QuestId) == QUEST_STATUS_INCOMPLETE)
            m_Player->PlayerTalkClass->SendQuestGiverRequestItems(quest, object->GetGUID(), m_Player->CanCompleteQuest(quest->GetQuestId()), true);
        else
        {
            if (quest->IsAutoAccept())
            {
                if (Creature* pQuestGiver = ObjectAccessor::GetCreature(*m_Player, l_Guid))
                    if (pQuestGiver->IsAIEnabled)
                        sScriptMgr->OnQuestAccept(m_Player, pQuestGiver, quest);

                m_Player->AddQuest(quest, object);
                if (m_Player->CanCompleteQuest(l_QuestId))
                    m_Player->CompleteQuest(l_QuestId);
            }
            m_Player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, object->GetGUID(), true);
        }
    }
}

void WorldSession::HandleQuestQueryOpcode(WorldPacket & recvData)
{
    if (!m_Player)
        return;

    uint32 questId;
    recvData >> questId;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUEST_QUERY quest = %u", questId);

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
        m_Player->PlayerTalkClass->SendQuestQueryResponse(quest);
}

void WorldSession::HandleQuestgiverChooseRewardOpcode(WorldPacket& recvData)
{
    uint32 questId, reward = 0;
    uint32 slot = 0;
    ObjectGuid guid;
    recvData >> questId >> slot;

    uint8 bitOrder[8] = {6, 7, 4, 5, 0, 3, 2, 1};
    recvData.ReadBitInOrder(guid, bitOrder);
    uint8 byteOrder[8] = {4, 1, 2, 5, 6, 0, 7, 3}; 
    recvData.ReadBytesSeq(guid, byteOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_CHOOSE_REWARD npc = %u, quest = %u, reward = %u", uint32(GUID_LOPART(guid)), questId, reward);

    Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest)
        return;

    Object* object = m_Player;

    if (!quest->HasFlag(QUEST_FLAGS_AUTO_SUBMIT))
    {
        //TODO: Doing something less dirty
        for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; i++)
            if (quest->RewardChoiceItemId[i] == slot)
                reward = i;

        if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DYNAMIC_ITEM_REWARD))
        {
            uint32 index = 0;
            for (auto itr : quest->DynamicRewards)
            {
                ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itr.itemID);
                if (!itemTemplate)
                    continue;

                // @TODO: Check if we really need to check specialisation id or just player's class
                // (if player doesn't have choosen spec, he doesn't have reward ??)
                //if (itemTemplate->HasSpec() && !itemTemplate->HasSpec(plr->GetSpecializationId(plr->GetActiveSpec())))
                //    continue;

                if (itemTemplate->HasSpec() && !itemTemplate->HasClassSpec(m_Player->getClass()))
                    continue;

                if (itr.itemID == slot)
                {
                    reward = index;
                    break;
                }

                index++;
            }
        }

        if (reward >= QUEST_REWARD_CHOICES_COUNT)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Error in CMSG_QUESTGIVER_CHOOSE_REWARD: player %s (guid %d) tried to get invalid reward (%u) (probably packet hacking)", m_Player->GetName(), m_Player->GetGUIDLow(), reward);
            return;
        }
        object = ObjectAccessor::GetObjectByTypeMask(*m_Player, guid, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
        if (!object || !object->hasInvolvedQuest(questId))
            return;

        // some kind of WPE protection
        if (!m_Player->CanInteractWithQuestGiver(object))
            return;

    }
    if ((!m_Player->CanSeeStartQuest(quest) &&  m_Player->GetQuestStatus(questId) == QUEST_STATUS_NONE) ||
        (m_Player->GetQuestStatus(questId) != QUEST_STATUS_COMPLETE && !quest->IsAutoComplete()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HACK ALERT: Player %s (guid: %u) is trying to complete quest (id: %u) but he has no right to do it!",
            m_Player->GetName(), m_Player->GetGUIDLow(), questId);
        return;
    }

    if (m_Player->CanRewardQuest(quest, reward, true))
    { 
        m_Player->RewardQuest(quest, reward, object);

        switch (object->GetTypeId())
        { 
        case TYPEID_UNIT:
        case TYPEID_PLAYER:
            {
                //For AutoSubmition was added plr case there as it almost same exclude AI script cases.
                Creature *creatureQGiver = object->ToCreature();
                if (!creatureQGiver || !(sScriptMgr->OnQuestReward(m_Player, creatureQGiver, quest, reward)))
                {
                    // Send next quest
                    if (Quest const* nextQuest = m_Player->GetNextQuest(guid, quest))
                    {
                        if (nextQuest->IsAutoAccept() && m_Player->CanAddQuest(nextQuest, true) && m_Player->CanTakeQuest(nextQuest, true))
                        {
                            m_Player->AddQuest(nextQuest, object);
                            if (m_Player->CanCompleteQuest(nextQuest->GetQuestId()))
                                m_Player->CompleteQuest(nextQuest->GetQuestId());
                        }

                        m_Player->PlayerTalkClass->SendQuestGiverQuestDetails(nextQuest, guid, true);
                    }
                    if (creatureQGiver)
                        creatureQGiver->AI()->sQuestReward(m_Player, quest, reward);
                }
                break;
            }
        case TYPEID_GAMEOBJECT:
            if (!sScriptMgr->OnQuestReward(m_Player, ((GameObject*)object), quest, reward))
            {
                // Send next quest
                if (Quest const* nextQuest = m_Player->GetNextQuest(guid, quest))
                {
                    if (nextQuest->IsAutoAccept() && m_Player->CanAddQuest(nextQuest, true) && m_Player->CanTakeQuest(nextQuest, true))
                    {
                        m_Player->AddQuest(nextQuest, object);
                        if (m_Player->CanCompleteQuest(nextQuest->GetQuestId()))
                            m_Player->CompleteQuest(nextQuest->GetQuestId());
                    }

                    m_Player->PlayerTalkClass->SendQuestGiverQuestDetails(nextQuest, guid, true);
                }

                object->ToGameObject()->AI()->QuestReward(m_Player, quest, reward);
            }
            break;
        default:
            break;
        }
    }
    else 
        m_Player->PlayerTalkClass->SendQuestGiverOfferReward(quest, guid, true);
}

void WorldSession::HandleQuestgiverRequestRewardOpcode(WorldPacket & recvData)
{
    uint32 questId;
    ObjectGuid guid;

    recvData >> questId;
    
    uint8 bitOrder[8] = {5, 0, 1, 2, 4, 3, 7, 6};
    uint8 byteOrder[8] = {5, 7, 1, 3, 4, 6, 2, 0};
    recvData.ReadBitInOrder(guid, bitOrder);
    recvData.ReadBytesSeq(guid, byteOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_REQUEST_REWARD npc = %u, quest = %u", uint32(GUID_LOPART(guid)), questId);

    Object* object = ObjectAccessor::GetObjectByTypeMask(*m_Player, guid, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
    if (!object || !object->hasInvolvedQuest(questId))
        return;

    // some kind of WPE protection
    if (!m_Player->CanInteractWithQuestGiver(object))
        return;

    if (m_Player->CanCompleteQuest(questId))
        m_Player->CompleteQuest(questId);

    if (m_Player->GetQuestStatus(questId) != QUEST_STATUS_COMPLETE)
        return;

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
        m_Player->PlayerTalkClass->SendQuestGiverOfferReward(quest, guid, true);
}

void WorldSession::HandleQuestLogRemoveQuest(WorldPacket& recvData)
{
    uint8 slot;
    recvData >> slot;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTLOG_REMOVE_QUEST slot = %u", slot);

    if (slot < MAX_QUEST_LOG_SIZE)
    {
        if (uint32 questId = m_Player->GetQuestSlotQuestId(slot))
        {
            if (!m_Player->TakeQuestSourceItem(questId, true))
                return;                                     // can't un-equip some items, reject quest cancel

            if (const Quest *quest = sObjectMgr->GetQuestTemplate(questId))
            {
                if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
                    m_Player->RemoveTimedQuest(questId);
            }

            m_Player->TakeQuestSourceItem(questId, true); // remove quest src item from player
            m_Player->RemoveActiveQuest(questId);
            m_Player->GetAchievementMgr().RemoveTimedAchievement(ACHIEVEMENT_TIMED_TYPE_QUEST, questId);

            sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %u abandoned quest %u", m_Player->GetGUIDLow(), questId);
        }

        m_Player->SetQuestSlot(slot, 0);

        m_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED, 1);
    }
}

void WorldSession::HandleQuestConfirmAccept(WorldPacket& recvData)
{
    uint32 questId;
    recvData >> questId;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUEST_CONFIRM_ACCEPT questId = %u", questId);

    if (const Quest* quest = sObjectMgr->GetQuestTemplate(questId))
    {
        if (!quest->HasFlag(QUEST_FLAGS_PARTY_ACCEPT))
            return;

        Player* pOriginalPlayer = ObjectAccessor::FindPlayer(m_Player->GetDivider());

        if (!pOriginalPlayer)
            return;

        if (quest->IsRaidQuest())
        {
            if (!m_Player->IsInSameRaidWith(pOriginalPlayer))
                return;
        }
        else
        {
            if (!m_Player->IsInSameGroupWith(pOriginalPlayer))
                return;
        }

        if (m_Player->CanAddQuest(quest, true))
            m_Player->AddQuest(quest, NULL);                // NULL, this prevent DB script from duplicate running

        m_Player->SetDivider(0);
    }
}

void WorldSession::HandleQuestgiverCompleteQuest(WorldPacket& recvData)
{
    uint32 l_QuestID;
    uint64 l_QuestGiverGUID;
    bool autoCompleteMode;      // 0 - standard complete quest mode with npc, 1 - auto-complete mode


    recvData.readPackGUID(l_QuestGiverGUID);
    recvData >> l_QuestID;

    autoCompleteMode = recvData.ReadBit();
    recvData.FlushBits();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_COMPLETE_QUEST npc = %u, questId = %u", uint32(GUID_LOPART(l_QuestGiverGUID)), l_QuestID);

    if (autoCompleteMode == 0)
    {
        Object* object = ObjectAccessor::GetObjectByTypeMask(*m_Player, l_QuestGiverGUID, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
        if (!object || !object->hasInvolvedQuest(l_QuestID))
            return;

    // some kind of WPE protection
    if (!m_Player->CanInteractWithQuestGiver(object))
        return;
    }

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(l_QuestID))
    {
        if (autoCompleteMode && !quest->HasFlag(QUEST_FLAGS_AUTO_SUBMIT))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Possible hacking attempt: Player %s [playerGuid: %u] tried to complete questId [entry: %u] by auto-submit flag for quest witch not suport it.",
                m_Player->GetName(), m_Player->GetGUIDLow(), l_QuestID);
            return;
        }
        if (!m_Player->CanSeeStartQuest(quest) && m_Player->GetQuestStatus(l_QuestID) == QUEST_STATUS_NONE)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Possible hacking attempt: Player %s [playerGuid: %u] tried to complete questId [entry: %u] without being in possession of the questId!",
                          m_Player->GetName(), m_Player->GetGUIDLow(), l_QuestID);
            return;
        }
        // TODO: need a virtual function
        if (m_Player->InBattleground())
            if (Battleground* bg = m_Player->GetBattleground())
                if (bg->GetTypeID() == BATTLEGROUND_AV)
                    ((BattlegroundAV*)bg)->HandleQuestComplete(l_QuestID, m_Player);

        if (m_Player->GetQuestStatus(l_QuestID) != QUEST_STATUS_COMPLETE)
        {
            if (quest->IsRepeatable())
                m_Player->PlayerTalkClass->SendQuestGiverRequestItems(quest, l_QuestGiverGUID, m_Player->CanCompleteRepeatableQuest(quest), false);
            else
                m_Player->PlayerTalkClass->SendQuestGiverRequestItems(quest, l_QuestGiverGUID, m_Player->CanRewardQuest(quest, false), false);
        }
        else
        {
            if (quest->GetReqItemsCount())                  // some items required
                m_Player->PlayerTalkClass->SendQuestGiverRequestItems(quest, l_QuestGiverGUID, m_Player->CanRewardQuest(quest, false), false);
            else                                            // no items required
                m_Player->PlayerTalkClass->SendQuestGiverOfferReward(quest, l_QuestGiverGUID, !autoCompleteMode);
        }
    }

    if (m_Player)
        m_Player->SaveToDB();
}

void WorldSession::HandlePushQuestToParty(WorldPacket& recvPacket)
{
    uint32 questId;
    recvPacket >> questId;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PUSHQUESTTOPARTY questId = %u", questId);

    if (m_Player->GetQuestStatus(questId) == QUEST_STATUS_NONE || m_Player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
        return;

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
    {
        if (Group* group = m_Player->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* player = itr->getSource();

                if (!player || player == m_Player)         // skip self
                    continue;

                m_Player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_SHARING_QUEST);

                if (!player->SatisfyQuestStatus(quest, false))
                {
                    m_Player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_HAVE_QUEST);
                    continue;
                }

                if (player->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE)
                {
                    m_Player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_FINISH_QUEST);
                    continue;
                }

                if (!player->CanTakeQuest(quest, false))
                {
                    m_Player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_CANT_TAKE_QUEST);
                    continue;
                }

                if (!player->SatisfyQuestLog(false))
                {
                    m_Player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_LOG_FULL);
                    continue;
                }

                if (player->GetDivider() != 0)
                {
                    m_Player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_BUSY);
                    continue;
                }

                player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, m_Player->GetGUID(), true);
                player->SetDivider(m_Player->GetGUID());
            }
        }
    }
}

void WorldSession::HandleQuestPushResult(WorldPacket& recvPacket)
{
    uint64 guid;
    uint8 msg;
    recvPacket >> guid >> msg;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received SMSG_QUEST_PUSH_RESULT");

    if (m_Player->GetDivider() != 0)
    {
        Player* player = ObjectAccessor::FindPlayer(m_Player->GetDivider());
        if (player)
        {
            WorldPacket data(SMSG_QUEST_PUSH_RESULT, (8+1));
            ObjectGuid guidObj = player->GetGUID();
            uint8 bitOrder[8] = {1, 0, 6, 5, 7, 4, 3, 2};
            uint8 byteOrder[8] = {7, 5, 1, 6, 3, 2, 4, 0};
            data.WriteBitInOrder(guidObj, bitOrder);
            data.WriteBytesSeq(guidObj, byteOrder);
            data << uint8(msg); 
            player->GetSession()->SendPacket(&data);
            m_Player->SetDivider(0);
        }
    }
}

uint32 WorldSession::getDialogStatus(Player* player, Object* questgiver, uint32 defstatus)
{
    uint32 result = defstatus;

    QuestRelationBounds qr;
    QuestRelationBounds qir;

    switch (questgiver->GetTypeId())
    {
        case TYPEID_GAMEOBJECT:
        {
            qr  = sObjectMgr->GetGOQuestRelationBounds(questgiver->GetEntry());
            qir = sObjectMgr->GetGOQuestInvolvedRelationBounds(questgiver->GetEntry());
            break;
        }
        case TYPEID_UNIT:
        {
            qr  = sObjectMgr->GetCreatureQuestRelationBounds(questgiver->GetEntry());
            qir = sObjectMgr->GetCreatureQuestInvolvedRelationBounds(questgiver->GetEntry());
            break;
        }
        default:
            //its imposible, but check ^)
            sLog->outError(LOG_FILTER_NETWORKIO, "Warning: GetDialogStatus called for unexpected type %u", questgiver->GetTypeId());
            return DIALOG_STATUS_NONE;
    }

    for (QuestRelations::const_iterator i = qir.first; i != qir.second; ++i)
    {
        uint32 result2 = 0;
        uint32 quest_id = i->second;
        Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id);
        if (!quest)
            continue;

        ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_QUEST_SHOW_MARK, quest->GetQuestId());
        if (!sConditionMgr->IsObjectMeetToConditions(player, conditions))
            continue;

        QuestStatus status = player->GetQuestStatus(quest_id);
        if ((status == QUEST_STATUS_COMPLETE && !player->GetQuestRewardStatus(quest_id)) ||
            (quest->IsAutoComplete() && player->CanTakeQuest(quest, false)))
        {
            if (quest->IsAutoComplete() && quest->IsRepeatable())
                result2 = DIALOG_STATUS_REWARD_REP;
            else
                result2 = DIALOG_STATUS_REWARD;
        }
        else if (status == QUEST_STATUS_INCOMPLETE)
            result2 = DIALOG_STATUS_INCOMPLETE;

        if (result2 > result)
            result = result2;
    }

    for (QuestRelations::const_iterator i = qr.first; i != qr.second; ++i)
    {
        uint32 result2 = 0;
        uint32 quest_id = i->second;
        Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id);
        if (!quest)
            continue;

        ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_QUEST_SHOW_MARK, quest->GetQuestId());
        if (!sConditionMgr->IsObjectMeetToConditions(player, conditions))
            continue;

        QuestStatus status = player->GetQuestStatus(quest_id);
        if (status == QUEST_STATUS_NONE)
        {
            if (player->CanSeeStartQuest(quest))
            {
                if (player->SatisfyQuestLevel(quest, false))
                {
                    if (quest->IsAutoComplete() || (quest->IsRepeatable() && player->IsQuestRewarded(quest_id)))
                        result2 = DIALOG_STATUS_REWARD_REP;
                    else if (player->getLevel() <= ((player->GetQuestLevel(quest) == -1) ? player->getLevel() : player->GetQuestLevel(quest) + sWorld->getIntConfig(CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF)))
                    {
                        if (quest->HasFlag(QUEST_FLAGS_DAILY) || quest->HasFlag(QUEST_FLAGS_WEEKLY))
                            result2 = DIALOG_STATUS_AVAILABLE_REP;
                        else
                            result2 = DIALOG_STATUS_AVAILABLE;
                    }
                    else
                        result2 = DIALOG_STATUS_LOW_LEVEL_AVAILABLE;
                }
                else
                    result2 = DIALOG_STATUS_UNAVAILABLE;
            }
        }

        if (result2 > result)
            result = result2;
    }

    return result;
}

void WorldSession::HandleQuestgiverStatusMultipleQuery(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY");

    uint32 count = 0;
    for (Player::ClientGUIDs::const_iterator itr = m_Player->m_clientGUIDs.begin(); itr != m_Player->m_clientGUIDs.end(); ++itr)
    {
        if (IS_CRE_OR_VEH_OR_PET_GUID(*itr))
        {
            // need also pet quests case support
            Creature* questgiver = ObjectAccessor::GetCreatureOrPetOrVehicle(*GetPlayer(), *itr);
            if (!questgiver || questgiver->IsHostileTo(m_Player))
                continue;
            if (!questgiver->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
                continue;

            count++;

        }
        else if (IS_GAMEOBJECT_GUID(*itr))
        {
            GameObject* questgiver = GetPlayer()->GetMap()->GetGameObject(*itr);
            if (!questgiver)
                continue;
            if (questgiver->GetGoType() != GAMEOBJECT_TYPE_QUESTGIVER)
                continue;

            count++;
        }
    }

    ByteBuffer buff;
    WorldPacket data(SMSG_QUEST_GIVER_STATUS_MULTIPLE);
    data << uint32(count);

    for (Player::ClientGUIDs::const_iterator itr = m_Player->m_clientGUIDs.begin(); itr != m_Player->m_clientGUIDs.end(); ++itr)
    {
        uint32 questStatus = DIALOG_STATUS_NONE;
        uint32 defstatus = DIALOG_STATUS_NONE;

        if (IS_CRE_OR_VEH_OR_PET_GUID(*itr))
        {
            // need also pet quests case support
            Creature* questgiver = ObjectAccessor::GetCreatureOrPetOrVehicle(*GetPlayer(), *itr);
            if (!questgiver || questgiver->IsHostileTo(m_Player))
                continue;
            if (!questgiver->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
                continue;
            questStatus = sScriptMgr->GetDialogStatus(m_Player, questgiver);
            if (questStatus > 6)
                questStatus = getDialogStatus(m_Player, questgiver, defstatus);

            data.appendPackGUID(questgiver->GetGUID());
            data << uint32(questStatus);
        }
        else if (IS_GAMEOBJECT_GUID(*itr))
        {
            GameObject* questgiver = GetPlayer()->GetMap()->GetGameObject(*itr);
            if (!questgiver)
                continue;
            if (questgiver->GetGoType() != GAMEOBJECT_TYPE_QUESTGIVER)
                continue;
            questStatus = sScriptMgr->GetDialogStatus(m_Player, questgiver);
            if (questStatus > 6)
                questStatus = getDialogStatus(m_Player, questgiver, defstatus);

            data.appendPackGUID(questgiver->GetGUID());
            data << uint32(questStatus);
        }
    }

    SendPacket(&data);
}

void WorldSession::HandleQueryQuestsCompleted(WorldPacket& /*recvData*/)
{
    size_t rew_count = m_Player->GetRewardedQuestCount();


    const RewardedQuestSet &rewQuests = m_Player->getRewardedQuests();
    for (RewardedQuestSet::const_iterator itr = rewQuests.begin(); itr != rewQuests.end(); ++itr)
    {
        WorldPacket data(SMSG_IS_QUEST_COMPLETE_RESPONSE, 4 + 4 * rew_count);
        data << uint32(*itr);
        data.WriteBit(1);
        SendPacket(&data);
    }

}
