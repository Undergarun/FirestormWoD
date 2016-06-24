////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Chat.h"
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

void WorldSession::HandleQuestgiverStatusQueryOpcode(WorldPacket & p_RecvData)
{
    uint64 l_QuestGiverGUID;
    p_RecvData.readPackGUID(l_QuestGiverGUID);

    uint32 l_QuestStatus = DIALOG_STATUS_NONE;
    uint32 l_DefStatus   = DIALOG_STATUS_NONE;

    Object* l_QuestGiver = ObjectAccessor::GetObjectByTypeMask(*m_Player, l_QuestGiverGUID, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
    if (!l_QuestGiver)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "Error in CMSG_QUESTGIVER_STATUS_QUERY, called for not found questgiver (Typeid: %u GUID: %u)", GuidHigh2TypeId(GUID_HIPART(l_QuestGiverGUID)), GUID_LOPART(l_QuestGiverGUID));
        return;
    }

    switch (l_QuestGiver->GetTypeId())
    {
        case TYPEID_UNIT:
        {
            Creature* l_CreatureQuestGiver = l_QuestGiver->ToCreature();
            if (!l_CreatureQuestGiver->IsHostileTo(m_Player))       // do not show quest status to enemies
            {
                l_QuestStatus = sScriptMgr->GetDialogStatus(m_Player, l_CreatureQuestGiver);
                if (l_QuestStatus > 6)
                    l_QuestStatus = getDialogStatus(m_Player, l_CreatureQuestGiver, l_DefStatus);
            }
            break;
        }
        case TYPEID_GAMEOBJECT:
        {
            GameObject* GameObjectQuestGiver=(GameObject*)l_QuestGiver;
            l_QuestStatus = sScriptMgr->GetDialogStatus(m_Player, GameObjectQuestGiver);
            if (l_QuestStatus > 6)
                l_QuestStatus = getDialogStatus(m_Player, GameObjectQuestGiver, l_DefStatus);
            break;
        }
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "QuestGiver called for unexpected type %u", l_QuestGiver->GetTypeId());
            break;
    }

    //inform client about status of quest
    m_Player->PlayerTalkClass->SendQuestGiverStatus(l_QuestStatus, l_QuestGiverGUID);
}

void WorldSession::HandleQuestgiverHelloOpcode(WorldPacket& recvData)
{
    uint64 guid;

    recvData.readPackGUID(guid);

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
    uint64 guid;
    uint32 questId;
    uint8 unk1;

    recvData.readPackGUID(guid);
    recvData >> questId;
    unk1 = recvData.ReadBit(); ///< unk1 is never read 01/18/16

    Object* object = ObjectAccessor::GetObjectByTypeMask(*m_Player, guid, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT|TYPEMASK_ITEM|TYPEMASK_PLAYER);
    Quest const* l_Quest = sObjectMgr->GetQuestTemplate(questId);

    if (!l_Quest || !object || (object == m_Player && !l_Quest->IsAutoTake()))
        return;

    // no or incorrect quest giver (probably missing quest relation)
    if ((object->GetTypeId() != TYPEID_PLAYER && !object->hasQuest(questId)) ||
        (object->IsPlayer() && object != m_Player && !object->ToPlayer()->CanShareQuest(questId)))
    {
        m_Player->PlayerTalkClass->SendCloseGossip();
        m_Player->SaveToDB();
        m_Player->SetDivider(0);

        if (m_Player->m_IsDebugQuestLogs)
            ChatHandler(m_Player).PSendSysMessage(LANG_DEBUG_QUEST_LOGS_NO_QUESTGIVER);

        return;
    }

    if (object->IsPlayer() && object != m_Player && object->ToPlayer()->GetQuestStatus(questId) == QUEST_STATUS_NONE)
        return;

    // some kind of WPE protection
    if (!m_Player->CanInteractWithQuestGiver(object))
        return;

    if (l_Quest)
    {
        // prevent cheating
        if (!GetPlayer()->CanTakeQuest(l_Quest, true))
        {
            m_Player->PlayerTalkClass->SendCloseGossip();
            m_Player->SetDivider(0);
            return;
        }

        if (object && object->IsPlayer() && !(l_Quest->HasFlag(QUEST_FLAGS_SHARABLE) || l_Quest->IsAutoTake()))
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

        if (m_Player->CanAddQuest(l_Quest, true))
        {
            m_Player->AddQuest(l_Quest, object);

            if (l_Quest->HasFlag(QUEST_FLAGS_PARTY_ACCEPT))
            {
                if (Group* group = m_Player->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* player = itr->getSource();

                        if (!player || player == m_Player)     // not self
                            continue;

                        if (player->CanTakeQuest(l_Quest, true))
                        {
                            player->SetDivider(m_Player->GetGUID());

                            //need confirmation that any gossip window will close
                            player->PlayerTalkClass->SendCloseGossip();

                            m_Player->SendQuestConfirmAccept(l_Quest, player);
                        }
                    }
                }
            }

            if (m_Player->CanCompleteQuest(questId))
                m_Player->CompleteQuest(questId);

            switch (object->GetTypeId())
            {
                case TYPEID_UNIT:
                    sScriptMgr->OnQuestAccept(m_Player, (object->ToCreature()), l_Quest);
                    (object->ToCreature())->AI()->sQuestAccept(m_Player, l_Quest);
                    break;
                case TYPEID_ITEM:
                case TYPEID_CONTAINER:
                {
                    sScriptMgr->OnQuestAccept(m_Player, ((Item*)object), l_Quest);

                    if (!l_Quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))
                        break;

                    // destroy not required for quest finish quest starting item
                    bool destroyItem = true;
                    for (QuestObjective l_Objective : l_Quest->QuestObjectives)
                    {
                        if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_ITEM &&
                            (l_Objective.ObjectID == ((Item*)object)->GetEntry()) && (((Item*)object)->GetTemplate()->MaxCount > 0)) ///< Comparison of integers of different signs: 'int32' (aka 'int') and 'uint32' (aka 'unsigned int')
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
                    sScriptMgr->OnQuestAccept(m_Player, ((GameObject*)object), l_Quest);
                    (object->ToGameObject())->AI()->QuestAccept(m_Player, l_Quest);
                    break;
                default:
                    break;
            }
            m_Player->PlayerTalkClass->SendCloseGossip();

            if (l_Quest->GetSrcSpell() > 0)
                m_Player->CastSpell(m_Player, l_Quest->GetSrcSpell(), true);

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
    l_RespondToGiver = p_RecvData.ReadBit(); ///< l_RespondToGiver is never read 01/18/16

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
            m_Player->PlayerTalkClass->SendQuestGiverOfferReward(quest, object->GetGUID());
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
            else
                m_Player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, object->GetGUID());
        }
    }
}

void WorldSession::HandleQuestQueryOpcode(WorldPacket & recvData)
{
    if (!m_Player)
        return;

    uint32 questId;
    recvData >> questId;

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
        m_Player->PlayerTalkClass->SendQuestQueryResponse(quest);
}

void WorldSession::HandleQuestgiverChooseRewardOpcode(WorldPacket& p_RecvData)
{
    uint32 l_QuestId;
    uint32 l_RewardEntry;
    uint32 l_Slot = 0;
    uint64 l_Guid;

    p_RecvData.readPackGUID(l_Guid);
    p_RecvData >> l_QuestId >> l_RewardEntry;

    Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_QuestId);
    if (!l_Quest)
        return;

    Object*     l_Object            = m_Player;
    bool        l_LegacyRewardFound = false;
    bool        l_CloseGossip       = false;

    /// - No package id, we use legacy item choice
    if (l_Quest->GetQuestPackageID() == 0)
    {
        for (int l_I = 0; l_I < QUEST_REWARD_CHOICES_COUNT; l_I++)
        {
            if (l_Quest->RewardChoiceItemId[l_I] == l_RewardEntry)
            {
                l_Slot = l_I;
                l_LegacyRewardFound = true;
                break;
            }
        }

        if (!l_LegacyRewardFound)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Error in CMSG_QUESTGIVER_CHOOSE_REWARD: player %s (guid %d) tried to get invalid reward (%u) (probably packet hacking)", m_Player->GetName(), m_Player->GetGUIDLow(), l_RewardEntry);
            return;
        }
    }

    if (!l_Quest->HasFlag(QUEST_FLAGS_AUTO_SUBMIT))
    {
        l_Object = ObjectAccessor::GetObjectByTypeMask(*m_Player, l_Guid, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
        if (!l_Object || !l_Object->hasInvolvedQuest(l_QuestId))
            return;

        /// some kind of WPE protection
        if (!m_Player->CanInteractWithQuestGiver(l_Object))
            return;
    }

    if ((!m_Player->CanSeeStartQuest(l_Quest) &&  m_Player->GetQuestStatus(l_QuestId) == QUEST_STATUS_NONE)
        || (m_Player->GetQuestStatus(l_QuestId) != QUEST_STATUS_COMPLETE && !l_Quest->IsAutoComplete()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HACK ALERT: Player %s (guid: %u) is trying to complete quest (id: %u) but he has no right to do it!",
            m_Player->GetName(), m_Player->GetGUIDLow(), l_QuestId);
        return;
    }

    if (m_Player->CanRewardQuest(l_Quest, l_LegacyRewardFound ? l_Slot : l_RewardEntry, true))
    {
        m_Player->RewardQuest(l_Quest, l_LegacyRewardFound ? l_Slot : l_RewardEntry, l_Object);

        switch (l_Object->GetTypeId())
        {
            case TYPEID_UNIT:
            case TYPEID_PLAYER:
            {
                //For AutoSubmition was added plr case there as it almost same exclude AI script cases.
                Creature* l_CreatureQGiver = l_Object->ToCreature();
                if (!l_CreatureQGiver || !(sScriptMgr->OnQuestReward(m_Player, l_CreatureQGiver, l_Quest, l_LegacyRewardFound ? l_Slot : l_RewardEntry)))
                {
                    // Send next quest
                    if (Quest const* l_NextQuest = m_Player->GetNextQuest(l_Guid, l_Quest))
                    {
                        if (m_Player->CanAddQuest(l_NextQuest, true) && m_Player->CanTakeQuest(l_NextQuest, true) && l_NextQuest->IsAutoTake())
                        {
                            if (l_NextQuest->IsAutoAccept())
                            {
                                m_Player->AddQuest(l_NextQuest, l_Object);

                                if (m_Player->CanCompleteQuest(l_NextQuest->GetQuestId()))
                                    m_Player->CompleteQuest(l_NextQuest->GetQuestId());
                            }

                            m_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_NextQuest, l_Guid);
                        }
                    }
                    else
                        l_CloseGossip = true;

                    if (l_CreatureQGiver)
                    {
                        l_CreatureQGiver->AI()->sQuestReward(m_Player, l_Quest, l_LegacyRewardFound ? l_Slot : l_RewardEntry);

                        sScriptMgr->OnQuestComplete(m_Player, (l_Object->ToCreature()), l_Quest);
                        (l_Object->ToCreature())->AI()->sQuestComplete(m_Player, l_Quest);
                    }
                }
                break;
            }
            case TYPEID_GAMEOBJECT:
            {
                if (!sScriptMgr->OnQuestReward(m_Player, ((GameObject*)l_Object), l_Quest, l_LegacyRewardFound ? l_Slot : l_RewardEntry))
                {
                    // Send next quest
                    if (Quest const* l_NextQuest = m_Player->GetNextQuest(l_Guid, l_Quest))
                    {
                        if (m_Player->CanAddQuest(l_NextQuest, true) && m_Player->CanTakeQuest(l_NextQuest, true) && l_NextQuest->IsAutoTake())
                        {
                            if (l_NextQuest->IsAutoAccept())
                            {
                                m_Player->AddQuest(l_NextQuest, l_Object);

                                if (m_Player->CanCompleteQuest(l_NextQuest->GetQuestId()))
                                    m_Player->CompleteQuest(l_NextQuest->GetQuestId());
                            }

                            m_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_NextQuest, l_Guid);
                        }
                    }
                    else
                        l_CloseGossip = true;

                    l_Object->ToGameObject()->AI()->QuestReward(m_Player, l_Quest, l_LegacyRewardFound ? l_Slot : l_RewardEntry);
                }
                break;
            }
            default:
                break;
        }
    }
    else
        m_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, l_Guid);

    if (l_CloseGossip)
        m_Player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleQuestgiverRequestRewardOpcode(WorldPacket & recvData)
{
    uint32 questId;
    uint64 guid;

    recvData.readPackGUID(guid);
    recvData >> questId;

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
        m_Player->PlayerTalkClass->SendQuestGiverOfferReward(quest, guid);
}

void WorldSession::HandleQuestLogRemoveQuest(WorldPacket& recvData)
{
    uint8 slot;
    recvData >> slot;

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
        }

        m_Player->SetQuestSlot(slot, 0);

        m_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED, 1);
    }
}

void WorldSession::HandleQuestConfirmAccept(WorldPacket& recvData)
{
    uint32 questId;
    recvData >> questId;

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
        if (autoCompleteMode && !quest->HasFlag(QUEST_FLAGS_AUTO_SUBMIT | QUEST_FLAGS_AUTOCOMPLETE))
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
            if (quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))                  // some items required
                m_Player->PlayerTalkClass->SendQuestGiverRequestItems(quest, l_QuestGiverGUID, m_Player->CanRewardQuest(quest, false), false);
            else                                            // no items required
            {
                m_Player->PlayerTalkClass->SendQuestGiverOfferReward(quest, l_QuestGiverGUID);
                m_Player->SaveToDB();
            }
        }
    }

}

void WorldSession::HandlePushQuestToParty(WorldPacket& recvPacket)
{
    uint32 questId;
    recvPacket >> questId;

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

                player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, m_Player->GetGUID());
                player->SetDivider(m_Player->GetGUID());
            }
        }
    }
}

void WorldSession::HandleQuestPushResult(WorldPacket& p_RecvPacket)
{
    uint64 l_TargetGUID;
    int32  l_QuestID;
    uint8  l_Result;

    p_RecvPacket.readPackGUID(l_TargetGUID);
    p_RecvPacket >> l_QuestID;
    p_RecvPacket >> l_Result;

    if (m_Player->GetDivider() != 0)
    {
        Player* l_Player = ObjectAccessor::FindPlayer(m_Player->GetDivider());
        if (l_Player)
        {
            WorldPacket l_Data(SMSG_QUEST_PUSH_RESULT, (8 + 1));

            l_Data.appendPackGUID(l_Player->GetGUID());
            l_Data << uint8(l_Result);

            l_Player->GetSession()->SendPacket(&l_Data);
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

        if (!sConditionMgr->IsObjectMeetingNotGroupedConditions(CONDITION_SOURCE_TYPE_QUEST_SHOW_MARK, quest->GetQuestId(), player))
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

        if (!sConditionMgr->IsObjectMeetingNotGroupedConditions(CONDITION_SOURCE_TYPE_QUEST_SHOW_MARK, quest->GetQuestId(), player))
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
    uint32 count = 0;
    for (auto itr = m_Player->m_clientGUIDs.begin(); itr != m_Player->m_clientGUIDs.end(); ++itr)
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

    ByteBuffer buff(1024);
    WorldPacket data(SMSG_QUEST_GIVER_STATUS_MULTIPLE, 1024);
    data << uint32(count);

    for (auto itr = m_Player->m_clientGUIDs.begin(); itr != m_Player->m_clientGUIDs.end(); ++itr)
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

void WorldSession::HandleQueryQuestCompletionNpcs(WorldPacket& p_RecvData)
{
    uint32 l_QuestIDCount;
    p_RecvData >> l_QuestIDCount;

    if (l_QuestIDCount >= MAX_QUEST_LOG_SIZE || l_QuestIDCount == 0)
        return;

    std::vector<const Quest*> l_Quests;
    for (uint32 l_Index = 0; l_Index < l_QuestIDCount; l_Index++)
    {
        int32 l_QuestID = p_RecvData.read<int32>();

        const Quest* l_Quest = sObjectMgr->GetQuestTemplate(l_QuestID);
        if (l_Quest == nullptr)
            continue;

        l_Quests.push_back(l_Quest);
    }

    WorldPacket l_Data(SMSG_QUEST_COMPLETION_NPCRESPONSE, 1024);
    l_Data << uint32(l_Quests.size());                          ///< Quest size

    for (const Quest* l_QuestItr : l_Quests)
    {
        l_Data << uint32(l_QuestItr->GetQuestId());             ///< QuestID
        l_Data << uint32(l_QuestItr->completionsNpcs.size());   ///< Completions npcs size

        for (const uint32 l_NpcItr : l_QuestItr->completionsNpcs)
            l_Data << uint32(l_NpcItr);
    }

    SendPacket(&l_Data);
}
