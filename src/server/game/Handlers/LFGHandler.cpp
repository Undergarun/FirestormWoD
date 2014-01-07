/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "WorldSession.h"
#include "WorldPacket.h"
#include "DBCStores.h"
#include "Player.h"
#include "Group.h"
#include "LFGMgr.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "GameEventMgr.h"
#include "InstanceScript.h"

void WorldSession::HandleLfgJoinOpcode(WorldPacket& recvData)
{
    uint32 numDungeons;
    uint32 dungeon;
    uint32 roles;
    uint8 length = 0;
    uint8 unk8 = 0;
    bool unkbit = false;

    recvData >> unk8;

    for (int i = 0; i < 3; i++)
        recvData.read_skip<uint32>();

    recvData >> roles;

    numDungeons = recvData.ReadBits(22);
    unkbit = recvData.ReadBit();
    length = recvData.ReadBits(8);

    recvData.FlushBits();

    if (!numDungeons)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_JOIN [" UI64FMTD "] no dungeons selected", GetPlayer()->GetGUID());
        recvData.rfinish();
        return;
    }

    LfgDungeonSet newDungeons;
    for (uint32 i = 0; i < numDungeons; ++i)
    {
        recvData >> dungeon;
        dungeon &= 0xFFFFFF;
        newDungeons.insert(dungeon);       // remove the type from the dungeon entry
    }

    std::string comment = recvData.ReadString(length);

    const LFGDungeonEntry* entry = sLFGDungeonStore.LookupEntry(*newDungeons.begin() & 0xFFFFFF);
    uint8 type = TYPEID_DUNGEON;
    uint8 maxGroupSize = 5;
    if (entry != NULL)
        type = entry->type;
    if (type == LFG_SUBTYPEID_RAID)
        maxGroupSize = 25;
    if (type == LFG_SUBTYPEID_SCENARIO)
        maxGroupSize = 3;

    if (!sWorld->getBoolConfig(CONFIG_DUNGEON_FINDER_ENABLE) ||
        (GetPlayer()->GetGroup() && GetPlayer()->GetGroup()->GetLeaderGUID() != GetPlayer()->GetGUID() &&
        (GetPlayer()->GetGroup()->GetMembersCount() == maxGroupSize || !GetPlayer()->GetGroup()->isLFGGroup())))
    {
        recvData.rfinish();
        return;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_JOIN [" UI64FMTD "] roles: %u, Dungeons: %u, Comment: %s", GetPlayer()->GetGUID(), roles, uint8(newDungeons.size()), comment.c_str());
    sLFGMgr->Join(GetPlayer(), uint8(roles), newDungeons, comment);
}

void WorldSession::HandleLfgLeaveOpcode(WorldPacket&  /*recvData*/)
{
    Group* grp = GetPlayer()->GetGroup();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_LEAVE [" UI64FMTD "] in group: %u", GetPlayer()->GetGUID(), grp ? 1 : 0);

    // Check cheating - only leader can leave the queue
    if (!grp || grp->GetLeaderGUID() == GetPlayer()->GetGUID())
        sLFGMgr->Leave(GetPlayer(), grp);
}

void WorldSession::HandleLfgProposalResultOpcode(WorldPacket& recvData)
{
    uint32 lfgGroupID;                                      // Internal lfgGroupID
    bool accept;                                            // Accept to join?

    recvData.read_skip<uint32>();                           // QueueId
    recvData.read_skip<uint32>();                           // Time
    recvData >> lfgGroupID;                                 // ProposalId
    recvData.read_skip<uint32>();                           // Const flag 3

    ObjectGuid guid1;
    ObjectGuid guid2;

    guid1[3] = recvData.ReadBit();
    guid1[5] = recvData.ReadBit();
    guid2[3] = recvData.ReadBit();
    guid1[1] = recvData.ReadBit();
    guid1[0] = recvData.ReadBit();
    guid1[2] = recvData.ReadBit();
    guid2[1] = recvData.ReadBit();

    accept = recvData.ReadBit();

    guid2[4] = recvData.ReadBit();
    guid1[4] = recvData.ReadBit();
    guid2[0] = recvData.ReadBit();
    guid1[7] = recvData.ReadBit();
    guid2[2] = recvData.ReadBit();
    guid2[7] = recvData.ReadBit();
    guid1[6] = recvData.ReadBit();
    guid2[6] = recvData.ReadBit();
    guid2[5] = recvData.ReadBit();

    recvData.FlushBits();

    recvData.ReadByteSeq(guid1[2]);
    recvData.ReadByteSeq(guid1[3]);
    recvData.ReadByteSeq(guid1[4]);
    recvData.ReadByteSeq(guid2[2]);
    recvData.ReadByteSeq(guid2[0]);
    recvData.ReadByteSeq(guid1[6]);
    recvData.ReadByteSeq(guid2[7]);
    recvData.ReadByteSeq(guid1[5]);

    recvData.ReadByteSeq(guid2[1]);
    recvData.ReadByteSeq(guid1[0]);
    recvData.ReadByteSeq(guid1[7]);
    recvData.ReadByteSeq(guid2[3]);
    recvData.ReadByteSeq(guid2[4]);
    recvData.ReadByteSeq(guid1[1]);
    recvData.ReadByteSeq(guid2[5]);
    recvData.ReadByteSeq(guid2[6]);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_PROPOSAL_RESULT [" UI64FMTD "] proposal: %u accept: %u", GetPlayer()->GetGUID(), lfgGroupID, accept ? 1 : 0);
    sLFGMgr->UpdateProposal(lfgGroupID, GetPlayer()->GetGUID(), accept);
}

void WorldSession::HandleLfgSetRolesOpcode(WorldPacket& recvData)
{
    uint32 roles;
    uint8 unk;

    recvData >> roles;                                    // Player Group Roles
    recvData >> unk;

    uint64 guid = GetPlayer()->GetGUID();
    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_SET_ROLES [" UI64FMTD "] Not in group", guid);
        return;
    }
    uint64 gguid = grp->GetGUID();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_SET_ROLES: Group [" UI64FMTD "], Player [" UI64FMTD "], Roles: %u", gguid, guid, roles);
    sLFGMgr->UpdateRoleCheck(gguid, guid, roles);
}

void WorldSession::HandleLfgSetCommentOpcode(WorldPacket&  recvData)
{
    std::string comment;
    recvData >> comment;
    uint64 guid = GetPlayer()->GetGUID();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SET_LFG_COMMENT [" UI64FMTD "] comment: %s", guid, comment.c_str());

    sLFGMgr->SetComment(guid, comment);
}

void WorldSession::HandleLfgSetBootVoteOpcode(WorldPacket& recvData)
{
    bool agree;                                            // Agree to kick player
    agree = recvData.ReadBit();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_SET_BOOT_VOTE [" UI64FMTD "] agree: %u", GetPlayer()->GetGUID(), agree ? 1 : 0);
    sLFGMgr->UpdateBoot(GetPlayer(), agree);
}

void WorldSession::HandleLfgTeleportOpcode(WorldPacket& recvData)
{
    bool out;
    out = recvData.ReadBit();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_TELEPORT [" UI64FMTD "] out: %u", GetPlayer()->GetGUID(), out ? 1 : 0);
    sLFGMgr->TeleportPlayer(GetPlayer(), out, true);
}

void WorldSession::HandleLfgLockInfoRequestOpcode(WorldPacket& recvData)
{
    uint8 value;
    bool groupPacket;

    recvData >> value;
    groupPacket = recvData.ReadBit();

    ObjectGuid guid = GetPlayer()->GetGUID();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFD_LOCK_INFO_REQUEST [" UI64FMTD "]", guid);

    // Get Random dungeons that can be done at a certain level and expansion
    LfgDungeonSet randomDungeons;
    uint8 level = GetPlayer()->getLevel();
    uint8 expansion = GetPlayer()->GetSession()->Expansion();
    for (uint32 i = 0; i < sLFGDungeonStore.GetNumRows(); ++i)
    {
        LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(i);
        if (dungeon && dungeon->expansion <= expansion && dungeon->minlevel <= level && level <= dungeon->maxlevel)
        {
            if (dungeon->flags & LFG_FLAG_SEASONAL)
            {
                if (HolidayIds holiday = sLFGMgr->GetDungeonSeason(dungeon->ID))
                    if (holiday == HOLIDAY_WOTLK_LAUNCH || !IsHolidayActive(holiday))
                        continue;
            }
            else if (dungeon->type != TYPEID_RANDOM_DUNGEON)
                continue;

            randomDungeons.insert(dungeon->Entry());
        }
    }

    // Get player locked Dungeons
    LfgLockMap lock = sLFGMgr->GetLockedDungeons(guid);

    uint32 rsize = uint32(randomDungeons.size());
    uint32 lsize = uint32(lock.size());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_PLAYER_INFO [" UI64FMTD "]", guid);
    WorldPacket data(SMSG_LFG_PLAYER_INFO, 1 + rsize * (4 + 1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4) + 4 + lsize * (1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4));

    bool hasGuid = true;
    data.WriteBit(hasGuid);
    if (hasGuid)
    {
        uint8 bitOrder[8] = { 7, 2, 1, 6, 3, 5, 0, 4 };
        data.WriteBitInOrder(guid, bitOrder);
    }

    data.WriteBits(randomDungeons.size(), 17);

    for (LfgDungeonSet::const_iterator it = randomDungeons.begin(); it != randomDungeons.end(); ++it)
    {
        LfgReward const* reward = sLFGMgr->GetRandomDungeonReward(*it, level);
        Quest const* qRew = NULL;

        bool done = 0;
        if (reward)
        {
            qRew = sObjectMgr->GetQuestTemplate(reward->reward[0].questId);
            if (qRew)
            {
                done = !GetPlayer()->CanRewardQuest(qRew, false);
                if (done)
                    qRew = sObjectMgr->GetQuestTemplate(reward->reward[1].questId);
            }
        }

        data.WriteBits(0, 21); // Unk count
        data.WriteBits(qRew ? qRew->GetRewCurrencyCount() : 0, 21);
        data.WriteBits(0, 19); // Unk count 2 - related to call to Arms

        data.WriteBit(!done);
        data.WriteBits(qRew ? qRew->GetRewItemsCount() : 0, 20);
        data.WriteBit(false);
    }

    data.WriteBits(lock.size(), 20);

    for (LfgDungeonSet::const_iterator it = randomDungeons.begin(); it != randomDungeons.end(); ++it)
    {
        LfgReward const* reward = sLFGMgr->GetRandomDungeonReward(*it, level);
        Quest const* qRew = NULL;
        uint8 done = 0;
        if (reward)
        {
            qRew = sObjectMgr->GetQuestTemplate(reward->reward[0].questId);
            if (qRew)
            {
                done = !GetPlayer()->CanRewardQuest(qRew, false);
                if (done)
                    qRew = sObjectMgr->GetQuestTemplate(reward->reward[1].questId);
            }
        }

        data << uint32(0);
        data << uint32(1);

        if (qRew)
        {
            if (qRew->GetRewCurrencyCount())
            {
                for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
                {
                    if (!qRew->RewardCurrencyId[i])
                        continue;

                    data << uint32(qRew->RewardCurrencyCount[i]);
                    data << uint32(qRew->RewardCurrencyId[i]);
                }
            }

            data << uint32(0);

            if (qRew->GetRewItemsCount())
            {
                ItemTemplate const* iProto = NULL;
                for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
                {
                    if (!qRew->RewardItemId[i])
                        continue;

                    iProto = sObjectMgr->GetItemTemplate(qRew->RewardItemId[i]);

                    data << uint32(iProto ? iProto->DisplayInfoID : 0);
                    data << uint32(qRew->RewardItemId[i]);
                    data << uint32(qRew->RewardItemIdCount[i]);
                }
            }

            data << uint32(1); // 1 in sniff
            data << uint32(1); // 1 in sniff

            /*for (int j = 0; j < info.Count3; ++j)*/

            data << uint32(0); // 0 in sniff
            data << uint32(0); // related to encounter - encounter progress
            data << uint32(*it); // Dungeon Entry (id + type) - progress
            data << uint32(1); // 0 in sniff
            data << uint32(1); // 0 in sniff
            data << uint32(0); // 0 in sniff
            data << uint32(0); // 0 in sniff

            /*for (int t = 0; t < 1; ++t)
            {
                data << uint32(396);
                data << uint32(3000);
            }*/

            data << uint32(qRew->XPValue(GetPlayer()));
            data << uint32(0); // 0 in sniff
            data << uint32(0); // 0 in sniff
            data << uint32(0); // 0 in sniff
            data << uint32(qRew->GetRewOrReqMoney());

            /*
            data << uint32(qRew->GetRewOrReqMoney());
            data << uint32(qRew->XPValue(GetPlayer()));
            data << uint32(reward->reward[done].variableMoney);
            data << uint32(reward->reward[done].variableXP);*/
        }
        else
        {
            data << uint32(16);

            data << uint32(1); // 1 in sniff unk4
            data << uint32(1); // 1 in sniff unk5

            data << uint32(0); // 0 in sniff unk6
            data << uint32(0); //related to encounter - encounter progress
            data << uint32(*it);
            data << uint32(1); // 0 in sniff unk9
            data << uint32(1); // 0 in sniff unk10
            data << uint32(0); // 0 in sniff unk11
            data << uint32(0); // 0 in sniff unk12

            /*for (int t = 0; t < 1; ++t)
            {
                data << uint32(396);
                data << uint32(3000);
            }*/

            data << uint32(333); // 0 in sniff unk13 // xp
            data << uint32(0); // 0 in sniff unk14
            data << uint32(0); // 0 in sniff unk15
            data << uint32(0); // 0 in sniff unk16
            data << uint32(666); // money
        }
    }

    for (LfgLockMap::const_iterator it = lock.begin(); it != lock.end(); ++it)
    {
        auto lockData = it->second;
        data << uint32(GetPlayer()->GetAverageItemLevel());
        data << uint32(it->first);                              // Dungeon entry (id + type)
        data << uint32(lockData.itemLevel);                     // Lock status
        data << uint32(lockData.lockstatus);
    }

    if (hasGuid)
    {
        uint8 byteOrder[8] = { 3, 1, 2, 6, 4, 7, 0, 5 };
        data.WriteBytesSeq(guid, byteOrder);
    }

    SendPacket(&data);
}

void WorldSession::HandleLfrSearchOpcode(WorldPacket& recvData)
{
    uint32 entry;                                          // Raid id to search
    recvData >> entry;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SEARCH_LFG_JOIN [" UI64FMTD "] dungeon entry: %u", GetPlayer()->GetGUID(), entry);
    //SendLfrUpdateListOpcode(entry);
}

void WorldSession::HandleLfrLeaveOpcode(WorldPacket& recvData)
{
    uint32 dungeonId;                                      // Raid id queue to leave
    recvData >> dungeonId;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SEARCH_LFG_LEAVE [" UI64FMTD "] dungeonId: %u", GetPlayer()->GetGUID(), dungeonId);
    //sLFGMgr->LeaveLfr(GetPlayer(), dungeonId);
}

void WorldSession::HandleLfgGetStatus(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_LFG, "CMSG_LFG_GET_STATUS %s", GetPlayer()->GetGUID());

    uint64 guid = GetPlayer()->GetGUID();
    LfgUpdateData updateData = sLFGMgr->GetLfgStatus(guid);

    if (GetPlayer()->GetGroup())
    {
        SendLfgUpdateParty(updateData);
        updateData.dungeons.clear();
        SendLfgUpdatePlayer(updateData);
    }
    else
    {
        SendLfgUpdatePlayer(updateData);
        updateData.dungeons.clear();
        SendLfgUpdateParty(updateData);
    }
}

void WorldSession::SendLfgUpdatePlayer(const LfgUpdateData& updateData)
{
    sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);

    /*bool queued = false;
    bool join = false;
    bool quit = false;

    switch (updateData.updateType)
    {
        case LFG_UPDATETYPE_JOIN_PROPOSAL:
        case LFG_UPDATETYPE_ADDED_TO_QUEUE:
            queued = true;
            break;
        //case LFG_UPDATETYPE_CLEAR_LOCK_LIST: // TODO: Sometimes has extrainfo - Check ocurrences...
        case LFG_UPDATETYPE_PROPOSAL_BEGIN:
            join = true;
            break;
        //case LFG_UPDATETYPE_UPDATES
        case LFG_UPDATETYPE_GROUP_DISBAND:
        case LFG_UPDATETYPE_GROUP_FOUND:
        case LFG_UPDATETYPE_CLEAR_LOCK_LIST:
        case LFG_UPDATETYPE_REMOVED_FROM_QUEUE:
            quit = true;
            break;
        default:
            break;
    }

    uint64 guid = GetPlayer()->GetGUID();
    uint8 size = uint8(updateData.dungeons.size());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_UPDATE_PLAYER [" UI64FMTD "] updatetype: %u", guid, updateData.updateType);
    WorldPacket data(SMSG_LFG_UPDATE_PLAYER, 1 + 1 + (extrainfo ? 1 : 0) * (1 + 1 + 1 + 1 + size * 4 + updateData.comment.length()));
    data << uint8(updateData.updateType);                 // Lfg Update type
    data << uint8(extrainfo);                             // Extra info
    if (extrainfo)
    {
        data << uint8(queued);                            // Join the queue
        data << uint8(0);                                 // unk - Always 0
        data << uint8(0);                                 // unk - Always 0

        data << uint8(size);
        if (size)
            for (LfgDungeonSet::const_iterator it = updateData.dungeons.begin(); it != updateData.dungeons.end(); ++it)
                data << uint32(*it);
        data << updateData.comment;
    }
    SendPacket(&data);*/
}

void WorldSession::SendLfgUpdateParty(const LfgUpdateData& updateData, uint32 joinTime)
{
    sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);

    /*bool join = false;
    bool extrainfo = false;
    bool queued = false;
    bool quit = false;
    bool pause = false;

    switch (updateData.updateType)
    {
        case LFG_UPDATETYPE_JOIN_PROPOSAL:
            extrainfo = true;
            break;
        case LFG_UPDATETYPE_ADDED_TO_QUEUE:
            extrainfo = true;
            join = true;
            queued = true;
            break;
        case LFG_UPDATETYPE_CLEAR_LOCK_LIST:
            // join = true;  // TODO: Sometimes queued and extrainfo - Check ocurrences...
            queued = true;
            quit = true;
            break;
        case LFG_UPDATETYPE_PROPOSAL_BEGIN:
            extrainfo = true;
            join = true;
            break;
        case LFG_UPDATETYPE_GROUP_DISBAND:
        case LFG_UPDATETYPE_GROUP_FOUND:
            quit = true;
            break;
        case LFG_UPDATETYPE_REMOVED_FROM_QUEUE:
            quit = true;
            pause = true;
            break;
        default:
            break;
    }

    uint64 guid = GetPlayer()->GetGUID();
    uint8 size = uint8(updateData.dungeons.size());
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_UPDATE_PARTY [" UI64FMTD "] updatetype: %u", guid, updateData.updateType);
    WorldPacket data(SMSG_LFG_UPDATE_PARTY, 1 + 1 + (extrainfo ? 1 : 0) * (1 + 1 + 1 + 1 + 1 + size * 4 + updateData.comment.length()));
    data << uint8(updateData.updateType);                 // Lfg Update type
    data << uint8(extrainfo);                             // Extra info
    if (extrainfo)
    {
        data << uint8(join);                              // LFG Join
        data << uint8(queued);                            // Join the queue
        data << uint8(0);                                 // unk - Always 0
        data << uint8(0);                                 // unk - Always 0
        for (uint8 i = 0; i < 3; ++i)
            data << uint8(0);                             // unk - Always 0

        data << uint8(size);
        if (size)
            for (LfgDungeonSet::const_iterator it = updateData.dungeons.begin(); it != updateData.dungeons.end(); ++it)
                data << uint32(*it);
        data << updateData.comment;
    }
    SendPacket(&data);*/
}

void WorldSession::SendLfgRoleCheckUpdate(const LfgRoleCheck* pRoleCheck, bool updateAll)
{
    ASSERT(pRoleCheck);
    LfgDungeonSet dungeons;
    if (pRoleCheck->rDungeonId)
        dungeons.insert(pRoleCheck->rDungeonId);
    else
        dungeons = pRoleCheck->dungeons;

    ObjectGuid guid = 0;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_ROLE_CHECK_UPDATE [" UI64FMTD "]", GetPlayer()->GetGUID());
    WorldPacket data(SMSG_LFG_ROLE_CHECK_UPDATE, 4 + 1 + 1 + dungeons.size() * 4 + 1 + pRoleCheck->roles.size() * (8 + 1 + 4 + 1));
    ByteBuffer dataBuffer;

    data.WriteBit(pRoleCheck->state == LFG_ROLECHECK_INITIALITING);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[4]);
    data.WriteBits(dungeons.size(), 22);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[1]);
    data.WriteBits(updateAll ? pRoleCheck->roles.size() : 1, 21);

    sLog->outError(LOG_FILTER_NETWORKIO, "RoleCheckUpdate: %u, state: %u", (int)pRoleCheck->roles.size(), pRoleCheck->state);

    if (!pRoleCheck->roles.empty())
    {
        // Player info MUST be sent 1st :S
        uint8 roles = pRoleCheck->roles.find(guid)->second;
        Player* player = ObjectAccessor::FindPlayer(guid);

        data.WriteBit(guid[7]);
        data.WriteBit(guid[6]);
        data.WriteBit(roles > 0);
        data.WriteBit(guid[4]);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[2]);
        data.WriteBit(guid[5]);

        dataBuffer << uint32(roles);                                   // Roles
        dataBuffer.WriteByteSeq(guid[1]);
        dataBuffer << uint8(player ? player->getLevel() : 0);          // Level
        dataBuffer.WriteByteSeq(guid[3]);
        dataBuffer.WriteByteSeq(guid[7]);
        dataBuffer.WriteByteSeq(guid[4]);
        dataBuffer.WriteByteSeq(guid[0]);
        dataBuffer.WriteByteSeq(guid[5]);
        dataBuffer.WriteByteSeq(guid[2]);
        dataBuffer.WriteByteSeq(guid[6]);

        for (LfgRolesMap::const_reverse_iterator it = pRoleCheck->roles.rbegin(); it != pRoleCheck->roles.rend(); ++it)
        {
            if (it->first == GetPlayer()->GetGUID() || !updateAll)
                continue;

            guid = it->first;
            roles = it->second;
            player = ObjectAccessor::FindPlayer(guid);

            data.WriteBit(guid[7]);
            data.WriteBit(guid[6]);
            data.WriteBit(roles > 0);
            data.WriteBit(guid[4]);
            data.WriteBit(guid[3]);
            data.WriteBit(guid[1]);
            data.WriteBit(guid[0]);
            data.WriteBit(guid[2]);
            data.WriteBit(guid[5]);

            dataBuffer << uint32(roles);                                   // Roles
            dataBuffer.WriteByteSeq(guid[1]);
            dataBuffer << uint8(player ? player->getLevel() : 0);          // Level
            dataBuffer.WriteByteSeq(guid[3]);
            dataBuffer.WriteByteSeq(guid[7]);
            dataBuffer.WriteByteSeq(guid[4]);
            dataBuffer.WriteByteSeq(guid[0]);
            dataBuffer.WriteByteSeq(guid[5]);
            dataBuffer.WriteByteSeq(guid[2]);
            dataBuffer.WriteByteSeq(guid[6]);
        }
    }

    data.WriteBit(guid[7]);
    data.WriteBit(guid[2]);
    data.WriteByteSeq(guid[1]);

    data.append(dataBuffer);
    data << uint8(1);

    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[6]);

    data << uint8(pRoleCheck->state);

    data.WriteByteSeq(guid[7]);

    if (!dungeons.empty())
    {
        for (LfgDungeonSet::iterator it = dungeons.begin(); it != dungeons.end(); ++it)
        {
            LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(*it);
            data << uint32(dungeon ? dungeon->Entry() : 0); // Dungeon
        }
    }

    data.WriteByteSeq(guid[2]);

    SendPacket(&data);
}

void WorldSession::SendLfgRoleChosen(ObjectGuid guid, uint8 roles)
{
    uint8 bytesOrder[8] = { 4, 2, 5, 1, 6, 3, 0, 7 };

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_ROLE_CHOSEN [" UI64FMTD "] guid: [" UI64FMTD "] roles: %u", GetPlayer()->GetGUID(), guid, roles);

    WorldPacket data(SMSG_LFG_ROLE_CHOSEN, 12);

    data.WriteBit(guid[6]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[7]);
    data.WriteBit(roles > 0);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[0]);
    data << uint32(roles);
    data.WriteBytesSeq(guid, bytesOrder);

    SendPacket(&data);
}

void WorldSession::SendLfgJoinResult(uint64 guid_, const LfgJoinResultData& joinData)
{
    ObjectGuid guid = guid_;

    uint32 size = 0;
    for (LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
        size += 8 + 4 + uint32(it->second.size()) * (4 + 4 + 4 + 4);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_JOIN_RESULT [" UI64FMTD "] checkResult: %u checkValue: %u", GetPlayer()->GetGUID(), joinData.result, joinData.state);

    WorldPacket data(SMSG_LFG_JOIN_RESULT, 4 + 4 + size);

    data.WriteBit(guid[4]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[2]);

    data.WriteBits(joinData.lockmap.size(), 22);

    for (LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
    {
        ObjectGuid guid1 = it->first;

        data.WriteBit(guid1[4]);
        data.WriteBit(guid1[5]);
        data.WriteBit(guid1[6]);
        data.WriteBit(guid1[0]);

        data.WriteBits(it->second.size(), 20);

        data.WriteBit(guid1[7]);
        data.WriteBit(guid1[3]);
        data.WriteBit(guid1[2]);
        data.WriteBit(guid1[1]);
    }

    data.WriteBit(guid[1]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);

    for (LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
    {
        LfgLockMap second = it->second;
        for (LfgLockMap::const_iterator itr = second.begin(); itr != second.end(); ++itr)
        {
            auto lockData = itr->second;
            data << uint32(GetPlayer()->GetAverageItemLevel());
            data << uint32(itr->first);                         // Dungeon entry (id + type)
            data << uint32(lockData.itemLevel);                 // Lock status
            data << uint32(lockData.lockstatus);
        }

        ObjectGuid guid1 = it->first;

        uint8 byteOrder[8] = {0, 1, 2, 5, 3, 6, 4, 7};
        data.WriteBytesSeq(guid1, byteOrder);
    }

    data << uint8(joinData.result);                       // Check Result
    data << uint8(joinData.state);                        // Check Value

    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[5]);

    data << uint32(3);                                    // Unk
    data << uint32(getMSTime());                          // Time

    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[3]);

    data << uint32(0);                                    // Queue Id

    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[6]);

    SendPacket(&data);
}

void WorldSession::SendLfgQueueStatus(uint32 dungeon, int32 waitTime, int32 avgWaitTime, int32 waitTimeTanks, int32 waitTimeHealer, int32 waitTimeDps, uint32 queuedTime, uint8 tanks, uint8 healers, uint8 dps)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_QUEUE_STATUS [" UI64FMTD "] dungeon: %u - waitTime: %d - avgWaitTime: %d - waitTimeTanks: %d - waitTimeHealer: %d - waitTimeDps: %d - queuedTime: %u - tanks: %u - healers: %u - dps: %u", GetPlayer()->GetGUID(), dungeon, waitTime, avgWaitTime, waitTimeTanks, waitTimeHealer, waitTimeDps, queuedTime, tanks, healers, dps);
    ObjectGuid guid = GetPlayer()->GetGUID();

    LfgQueueInfo* info = sLFGMgr->GetLfgQueueInfo(GetPlayer()->GetGroup() ? GetPlayer()->GetGroup()->GetGUID() : GetPlayer()->GetGUID());
    if (!info)
        return;

    WorldPacket data(SMSG_LFG_QUEUE_STATUS, 4 + 4 + 4 + 4 + 4 +4 + 1 + 1 + 1 + 4);
    data << uint32(dungeon);                               // Dungeon
    data << uint32(info->joinTime);                        // Time
    data << uint32(waitTime);
    data << uint32(0);                                     // QueueId
    data << uint32(3);                                     // Some Flags
    data << uint32(avgWaitTime);

    data << int32(waitTimeTanks);                          // Wait Tanks
    data << uint8(tanks);                                  // Tanks needed
    
    data << int32(waitTimeHealer);                         // Wait Healers
    data << uint8(healers);                                // Healers needed
    
    data << int32(waitTimeDps);                            // Wait Dps
    data << uint8(dps);                                    // Dps needed
    
    data << int32(queuedTime);

    data.WriteBit(guid[7]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[1]);

    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[0]);

    SendPacket(&data);
}

void WorldSession::SendLfgPlayerReward(uint32 rdungeonEntry, uint32 sdungeonEntry, uint8 done, const LfgReward* reward, const Quest* qRew)
{
    if (!rdungeonEntry || !sdungeonEntry || !qRew)
        return;

    uint8 itemNum = uint8(qRew ? qRew->GetRewItemsCount() + qRew->GetRewCurrencyCount()  : 0);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_PLAYER_REWARD [" UI64FMTD "] rdungeonEntry: %u - sdungeonEntry: %u - done: %u", GetPlayer()->GetGUID(), rdungeonEntry, sdungeonEntry, done);

    ByteBuffer rewards;
    WorldPacket data(SMSG_LFG_PLAYER_REWARD, 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 1 + itemNum * (4 + 4 + 4));
    data << uint32(rdungeonEntry);                         // Random Dungeon Finished
    data << uint32(sdungeonEntry);                         // Dungeon Finished
    data << uint32(qRew->GetRewOrReqMoney());
    data << uint32(qRew->XPValue(GetPlayer()));

    data.WriteBits(itemNum, 20);

    if (qRew && qRew->GetRewItemsCount())
    {
        ItemTemplate const* iProto = NULL;
        for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        {
            if (!qRew->RewardItemId[i])
                continue;

            data.WriteBit(false);

            iProto = sObjectMgr->GetItemTemplate(qRew->RewardItemId[i]);

            rewards << uint32(iProto ? iProto->DisplayInfoID : 0);
            rewards << uint32(qRew->RewardItemId[i]);
            rewards << uint32(0);
            rewards << uint8(qRew->RewardItemIdCount[i]);
        }
    }

    if (qRew && qRew->GetRewCurrencyCount())
    {
        for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        {
            if (!qRew->RewardCurrencyId[i])
                continue;

            data.WriteBit(true);

            rewards << uint32(0);
            rewards << uint32(qRew->RewardCurrencyId[i]);
            rewards << uint32(0);
            rewards << uint32(qRew->RewardCurrencyCount[i]);
        }
    }

    if (rewards.size())
        data.append(rewards);

    SendPacket(&data);
}

void WorldSession::SendLfgBootPlayer(const LfgPlayerBoot* pBoot)
{
    uint64 guid = GetPlayer()->GetGUID();
    LfgAnswer playerVote = pBoot->votes.find(guid)->second;
    uint8 votesNum = 0;
    uint8 agreeNum = 0;
    uint32 secsleft = uint8((pBoot->cancelTime - time(NULL)) / 1000);
    for (LfgAnswerMap::const_iterator it = pBoot->votes.begin(); it != pBoot->votes.end(); ++it)
    {
        if (it->second != LFG_ANSWER_PENDING)
        {
            ++votesNum;
            if (it->second == LFG_ANSWER_AGREE)
                ++agreeNum;
        }
    }
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_BOOT_PROPOSAL_UPDATE [" UI64FMTD "] inProgress: %u - didVote: %u - agree: %u - victim: [" UI64FMTD "] votes: %u - agrees: %u - left: %u - needed: %u - reason %s",
        guid, uint8(pBoot->inProgress), uint8(playerVote != LFG_ANSWER_PENDING), uint8(playerVote == LFG_ANSWER_AGREE), pBoot->victim, votesNum, agreeNum, secsleft, pBoot->votedNeeded, pBoot->reason.c_str());
    WorldPacket data(SMSG_LFG_BOOT_PROPOSAL_UPDATE, 1 + 1 + 1 + 8 + 4 + 4 + 4 + 4 + pBoot->reason.length());
    data << uint8(pBoot->inProgress);                      // Vote in progress
    data << uint8(playerVote != LFG_ANSWER_PENDING);       // Did Vote
    data << uint8(playerVote == LFG_ANSWER_AGREE);         // Agree
    data << uint8(0);                                      // Unknown 4.2.2
    data << uint64(pBoot->victim);                         // Victim GUID
    data << uint32(votesNum);                              // Total Votes
    data << uint32(agreeNum);                              // Agree Count
    data << uint32(secsleft);                              // Time Left
    data << uint32(pBoot->votedNeeded);                    // Needed Votes
    data << pBoot->reason.c_str();                         // Kick reason
    SendPacket(&data);
}

void WorldSession::SendLfgUpdateProposal(uint32 proposalId, const LfgProposal* pProp)
{
    if (!pProp)
        return;

    uint64 guid = GetPlayer()->GetGUID();
    LfgProposalPlayerMap::const_iterator itPlayer = pProp->players.find(guid);
    if (itPlayer == pProp->players.end())                  // Player MUST be in the proposal
        return;

    LfgProposalPlayer* ppPlayer = itPlayer->second;
    uint32 pLowGroupGuid = ppPlayer->groupLowGuid;
    uint32 dLowGuid = pProp->groupLowGuid;
    uint32 dungeonId = pProp->dungeonId;
    bool isSameDungeon = false;
    bool isContinue = false;
    Group* grp = dLowGuid ? sGroupMgr->GetGroupByGUID(dLowGuid) : NULL;
    uint32 completedEncounters = 0;
    if (grp)
    {
        uint64 gguid = grp->GetGUID();
        isContinue = grp->isLFGGroup() && sLFGMgr->GetState(gguid) != LFG_STATE_FINISHED_DUNGEON;
        isSameDungeon = GetPlayer()->GetGroup() == grp && isContinue;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_PROPOSAL_UPDATE [" UI64FMTD "] state: %u", GetPlayer()->GetGUID(), pProp->state);
    WorldPacket data(SMSG_LFG_PROPOSAL_UPDATE, 4 + 1 + 4 + 4 + 1 + 1 + pProp->players.size() * (4 + 1 + 1 + 1 + 1 +1));

    if (!isContinue)                                       // Only show proposal dungeon if it's continue
    {
        LfgDungeonSet playerDungeons = sLFGMgr->GetSelectedDungeons(guid);
        if (playerDungeons.size() == 1)
            dungeonId = (*playerDungeons.begin());
    }

    if (LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(dungeonId))
    {
        dungeonId = dungeon->Entry();

        // Select a player inside to be get completed encounters from
        if (grp)
        {
            for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupMember = itr->getSource();
                if (groupMember && groupMember->GetMapId() == uint32(dungeon->map))
                {
                    if (InstanceScript* instance = groupMember->GetInstanceScript())
                        completedEncounters = instance->GetCompletedEncounterMask();
                    break;
                }
            }
        }
    }

    ObjectGuid playerGUID = guid;
    ObjectGuid InstanceSaveGUID = MAKE_NEW_GUID(dungeonId, 0, HIGHGUID_INSTANCE_SAVE);

    data.WriteBit(playerGUID[1]);
    data.WriteBit(isContinue);
    data.WriteBit(InstanceSaveGUID[0]);
    data.WriteBit(InstanceSaveGUID[1]);
    data.WriteBit(playerGUID[4]);
    data.WriteBit(InstanceSaveGUID[7]);
    data.WriteBit(playerGUID[2]);
    data.WriteBit(InstanceSaveGUID[3]);
    data.WriteBit(InstanceSaveGUID[5]);

    data.WriteBits(pProp->players.size(), 21);

    data.WriteBit(InstanceSaveGUID[4]);
    data.WriteBit(isSameDungeon);
    data.WriteBit(playerGUID[6]);

    for (itPlayer = pProp->players.begin(); itPlayer != pProp->players.end(); ++itPlayer)
    {
        bool inDungeon = false;
        bool inSameGroup = false;

        if (itPlayer->second->groupLowGuid)
        {
            inDungeon = itPlayer->second->groupLowGuid == dLowGuid;
            inSameGroup = itPlayer->second->groupLowGuid == pLowGroupGuid;
        }

        data.WriteBit(inDungeon);                                       // In dungeon (silent)
        data.WriteBit(itPlayer->second->accept == LFG_ANSWER_AGREE);    // Accepted
        data.WriteBit(itPlayer->first == guid);                         // Self player
        data.WriteBit(itPlayer->second->accept!= LFG_ANSWER_PENDING);   // Answered
        data.WriteBit(inSameGroup);                                     // Same Group than player
    }

    data.WriteBit(playerGUID[5]);
    data.WriteBit(playerGUID[7]);
    data.WriteBit(playerGUID[3]);

    data.WriteBit(InstanceSaveGUID[2]);
    data.WriteBit(InstanceSaveGUID[6]);

    data.WriteBit(playerGUID[0]);

    data.WriteByteSeq(InstanceSaveGUID[5]);
    data.WriteByteSeq(InstanceSaveGUID[1]);
    data.WriteByteSeq(playerGUID[5]);

    data << uint32(proposalId);                            // Proposal Id

    data.WriteByteSeq(InstanceSaveGUID[2]);
    data.WriteByteSeq(InstanceSaveGUID[3]);
    data.WriteByteSeq(InstanceSaveGUID[7]);

    data << uint8(pProp->state);                           // Result state
    data << uint32(dungeonId);                             // Dungeon

    data.WriteByteSeq(InstanceSaveGUID[4]);
    data.WriteByteSeq(InstanceSaveGUID[6]);

    data << uint32(completedEncounters);                   // Bosses killed

    data.WriteByteSeq(playerGUID[4]);
    data.WriteByteSeq(playerGUID[3]);
    data.WriteByteSeq(playerGUID[0]);
    data.WriteByteSeq(playerGUID[2]);
    data.WriteByteSeq(playerGUID[6]);

    data << uint32(getMSTime());                           // Date

    data.WriteByteSeq(playerGUID[7]);

    for (itPlayer = pProp->players.begin(); itPlayer != pProp->players.end(); ++itPlayer)
        data << uint32(itPlayer->second->role);                    // Role

    data.WriteByteSeq(playerGUID[1]);

    data << uint32(0x03);                                  // unk id or flags ? always 3

    data.WriteByteSeq(InstanceSaveGUID[0]);

    data << uint32(0);                                     // QueueId

    SendPacket(&data);
}

void WorldSession::SendLfgUpdateSearch(bool update)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_UPDATE_SEARCH [" UI64FMTD "] update: %u", GetPlayer()->GetGUID(), update ? 1 : 0);
    WorldPacket data(SMSG_LFG_UPDATE_SEARCH, 1);
    data << uint8(update);                                 // In Lfg Queue?
    SendPacket(&data);
}

void WorldSession::SendLfgDisabled()
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_DISABLED [" UI64FMTD "]", GetPlayer()->GetGUID());
    WorldPacket data(SMSG_LFG_DISABLED, 0);
    SendPacket(&data);
}

void WorldSession::SendLfgOfferContinue(uint32 dungeonEntry)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_OFFER_CONTINUE [" UI64FMTD "] dungeon entry: %u", GetPlayer()->GetGUID(), dungeonEntry);
    WorldPacket data(SMSG_LFG_OFFER_CONTINUE, 4);
    data << uint32(dungeonEntry);
    SendPacket(&data);
}

void WorldSession::SendLfgTeleportError(uint8 err)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_TELEPORT_DENIED [" UI64FMTD "] reason: %u", GetPlayer()->GetGUID(), err);
    WorldPacket data(SMSG_LFG_TELEPORT_DENIED, 4);
    //Not sure it is no 4bits.
    data.WriteBits(err, 4);                                   // Error
    data.FlushBits();
    SendPacket(&data);
}

/*
void WorldSession::SendLfrUpdateListOpcode(uint32 dungeonEntry)
{
    sLog->outDebug(LOG_FILTER_PACKETIO, "SMSG_LFG_UPDATE_LIST [" UI64FMTD "] dungeon entry: %u", GetPlayer()->GetGUID(), dungeonEntry);
    WorldPacket data(SMSG_LFG_UPDATE_LIST);
    SendPacket(&data);
}
*/
