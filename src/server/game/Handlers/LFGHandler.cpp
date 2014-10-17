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

void WorldSession::HandleLfgJoinOpcode(WorldPacket & p_Packet)
{
    LfgDungeonSet l_Slots;

    std::string l_Comment = "";

    uint32 l_SlotCount      = 0;
    uint32 l_Roles          = 0;
    uint32 l_CommentLenght  = 0;

    uint8 l_PartyIndex = 0;

    bool l_QueueAsGroup = false;

    l_QueueAsGroup  = p_Packet.ReadBit();
    l_CommentLenght = p_Packet.ReadBits(8);

    p_Packet >> l_PartyIndex;
    p_Packet >> l_Roles;
    p_Packet >> l_SlotCount;

    for (int l_I = 0; l_I < 3; ++l_I)
        p_Packet.read_skip<uint32>();

    l_Comment = p_Packet.ReadString(l_CommentLenght);

    for (uint32 l_I = 0; l_I < l_SlotCount; ++l_I)
    {
        /// Remove the type from the dungeon entry
        l_Slots.insert(p_Packet.read<uint32>() & 0xFFFFFF);
    }

    if (!l_SlotCount)
        return;

    const LFGDungeonEntry * l_Entry = sLFGDungeonStore.LookupEntry(*l_Slots.begin() & 0xFFFFFF);

    uint8 l_Type = TYPEID_DUNGEON;
    uint8 l_MaxGroupSize = 5;

    if (l_Entry != NULL)
        l_Type = l_Entry->type;

    if (l_Type == LFG_SUBTYPEID_RAID)
        l_MaxGroupSize = 25;
    if (l_Type == LFG_SUBTYPEID_SCENARIO)
        l_MaxGroupSize = 3;

    if (!sWorld->getBoolConfig(CONFIG_DUNGEON_FINDER_ENABLE) ||
        (GetPlayer()->GetGroup() && GetPlayer()->GetGroup()->GetLeaderGUID() != GetPlayer()->GetGUID() &&
        (GetPlayer()->GetGroup()->GetMembersCount() == l_MaxGroupSize || !GetPlayer()->GetGroup()->isLFGGroup())))
    {
        return;
    }

    sLFGMgr->Join(GetPlayer(), uint8(l_Roles), l_Slots, l_Comment);
}

void WorldSession::HandleLfgLeaveOpcode(WorldPacket&  /*recvData*/)
{
    Group * l_Group = GetPlayer()->GetGroup();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_LEAVE [" UI64FMTD "] in group: %u", GetPlayer()->GetGUID(), l_Group ? 1 : 0);

    /// Check cheating - only leader can leave the queue
    if (!l_Group || l_Group->GetLeaderGUID() == GetPlayer()->GetGUID())
        sLFGMgr->Leave(GetPlayer(), l_Group);
}

void WorldSession::HandleLfgProposalResultOpcode(WorldPacket& recvData)
{
    uint32 lfgGroupID;                                     // Internal lfgGroupID
    bool accept;                                           // Accept to join?

    recvData.read_skip<uint32>();                          // QueueId
    recvData.read_skip<uint32>();                          // Time
    recvData >> lfgGroupID;                                // ProposalId
    recvData.read_skip<uint32>();                          // Const flag 3

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

void WorldSession::HandleLfgLockInfoRequestOpcode(WorldPacket & p_Packet)
{
    uint8 l_PartyIndex;
    bool l_GroupPacket;

    l_GroupPacket = p_Packet.ReadBit();
    p_Packet >> l_PartyIndex;

    if (l_GroupPacket)
    {
        Group * l_Group = GetPlayer()->GetGroup();

        if (!l_Group)
            return;

        std::map<uint64, LfgLockMap> l_LockMap;

        for (GroupReference* l_It = l_Group->GetFirstMember(); l_It != NULL; l_It = l_It->next())
        {
            Player* l_CurrentGroupPlayer = l_It->getSource();

            if (!l_CurrentGroupPlayer || l_CurrentGroupPlayer == m_Player)
                continue;

            sLFGMgr->InitializeLockedDungeons(l_CurrentGroupPlayer);
            l_LockMap[l_CurrentGroupPlayer->GetGUID()] = sLFGMgr->GetLockedDungeons(l_CurrentGroupPlayer->GetGUID());
        }

        WorldPacket l_Data(SMSG_LFG_PARTY_INFO, 1000);

        l_Data << uint32(l_LockMap.size());

        for (std::map<uint64, LfgLockMap>::const_iterator l_It = l_LockMap.begin(); l_It != l_LockMap.end(); ++l_It)
        {
            l_Data.WriteBit(l_It->first != 0);
            l_Data.FlushBits();

            l_Data << uint32(l_It->second.size());

            if (l_It->first)
                l_Data.appendPackGUID(l_It->first);

            for (LfgLockMap::const_iterator l_It2 = l_It->second.begin(); l_It2 != l_It->second.end(); ++l_It2)
            {
                LfgLockStatus l_LockData = l_It2->second;
                l_Data << uint32(l_It2->first);                         ///< Slot
                l_Data << uint32(l_LockData.lockstatus);                ///< Reason
                l_Data << uint32(l_LockData.itemLevel);                 ///< Sub Reason 1
                l_Data << uint32(GetPlayer()->GetAverageItemLevel());   ///< Sub Reason 2
            }
        }

        SendPacket(&l_Data);
    }
    else
    {
        /// Get Random dungeons that can be done at a certain level and expansion
        LfgDungeonSet l_RandomDungeons;

        uint8 l_Level = GetPlayer()->getLevel();
        uint8 l_Expansion = GetPlayer()->GetSession()->Expansion();

        for (uint32 l_I = 0; l_I < sLFGDungeonStore.GetNumRows(); ++l_I)
        {
            const LFGDungeonEntry * l_Dungeon = sLFGDungeonStore.LookupEntry(l_I);

            if (l_Dungeon && l_Dungeon->expansion <= l_Expansion && l_Dungeon->minlevel <= l_Level && l_Level <= l_Dungeon->maxlevel)
            {
                if (l_Dungeon->flags & LFG_FLAG_SEASONAL)
                {
                    if (HolidayIds l_Holiday = sLFGMgr->GetDungeonSeason(l_Dungeon->ID))
                    {
                        if (l_Holiday == HOLIDAY_WOTLK_LAUNCH || !IsHolidayActive(l_Holiday))
                            continue;
                    }
                }
                else if (l_Dungeon->type != TYPEID_RANDOM_DUNGEON)
                    continue;

                l_RandomDungeons.insert(l_Dungeon->Entry());
            }
        }

        // Get player locked Dungeons
        LfgLockMap l_Lock = sLFGMgr->GetLockedDungeons(GetPlayer()->GetGUID());

        uint32 l_DungeonCount = uint32(l_RandomDungeons.size());
        uint32 l_LockCount = uint32(l_Lock.size());

        bool l_HasGUID = true;

        WorldPacket l_Data(SMSG_LFG_PLAYER_INFO, 3000);

        l_Data << uint32(l_DungeonCount);                           ///< Dungeon Count

        l_Data.WriteBit(true);                                      ///< Has GUID
        l_Data.FlushBits();
        l_Data << uint32(l_LockCount);                              ///< Lock Count

        if (l_HasGUID)
            l_Data.appendPackGUID(GetPlayer()->GetGUID());          ///< Player Guid

        for (LfgLockMap::const_iterator l_It = l_Lock.begin(); l_It != l_Lock.end(); ++l_It)
        {
            LfgLockStatus l_LockData = l_It->second;
            l_Data << uint32(l_It->first);                          ///< Slot
            l_Data << uint32(l_LockData.lockstatus);                ///< Reason
            l_Data << uint32(l_LockData.itemLevel);                 ///< Sub Reason 1
            l_Data << uint32(GetPlayer()->GetAverageItemLevel());   ///< Sub Reason 2
        }

        for (LfgDungeonSet::const_iterator l_It = l_RandomDungeons.begin(); l_It != l_RandomDungeons.end(); ++l_It)
        {
            const LfgReward * l_Reward = sLFGMgr->GetRandomDungeonReward(*l_It, l_Level);
            const Quest     * l_Quest = NULL;

            bool l_IsDone = 0;
            if (l_Reward)
            {
                l_Quest = sObjectMgr->GetQuestTemplate(l_Reward->reward[0].questId);

                if (l_Quest)
                {
                    l_IsDone = !GetPlayer()->CanRewardQuest(l_Quest, false);

                    if (l_IsDone)
                        l_Quest = sObjectMgr->GetQuestTemplate(l_Reward->reward[1].questId);
                }
            }

            uint32 l_ShortageRewardCount = 0;

            l_Data << uint32(*l_It);                    ///< Slots
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(l_ShortageRewardCount);

            /// Rewards
            {
                if (l_Quest)
                {
                    l_Data << uint32(0); ///< Mask
                    l_Data << uint32(l_Quest->GetRewOrReqMoney());
                    l_Data << uint32(l_Quest->XPValue(GetPlayer()));
                    l_Data << uint32(l_Quest->GetRewItemsCount());
                    l_Data << uint32(l_Quest->GetRewCurrencyCount());
                    l_Data << uint32(0);
                }
                else
                {
                    l_Data << uint32(0);
                    l_Data << uint32(0);
                    l_Data << uint32(0);
                    l_Data << uint32(0);
                    l_Data << uint32(0);
                    l_Data << uint32(0);
                }

                if (l_Quest)
                {
                    if (l_Quest->GetRewItemsCount())
                    {
                        const ItemTemplate * l_ItemTemplate = NULL;

                        for (uint8 l_I = 0; l_I < QUEST_REWARDS_COUNT; ++l_I)
                        {
                            if (!l_Quest->RewardItemId[l_I])
                                continue;

                            l_ItemTemplate = sObjectMgr->GetItemTemplate(l_Quest->RewardItemId[l_I]);

                            l_Data << uint32(l_Quest->RewardItemId[l_I]);
                            l_Data << uint32(l_Quest->RewardItemIdCount[l_I]);
                        }
                    }

                    if (l_Quest->GetRewCurrencyCount())
                    {
                        for (uint8 l_I = 0; l_I < QUEST_REWARDS_COUNT; ++l_I)
                        {
                            if (!l_Quest->RewardCurrencyId[l_I])
                                continue;

                            uint32 l_Precision = 1;

                            if (const CurrencyTypesEntry * l_Entry = sCurrencyTypesStore.LookupEntry(l_Quest->RewardCurrencyId[l_I]))
                                l_Precision = l_Entry->Flags & CURRENCY_FLAG_HIGH_PRECISION ? CURRENCY_PRECISION : 1;

                            l_Data << uint32(l_Quest->RewardCurrencyId[l_I]);
                            l_Data << uint32(l_Quest->RewardCurrencyCount[l_I] * l_Precision);
                        }
                    }
                }

                l_Data.WriteBit(false);
                l_Data.FlushBits();
            }

            l_Data.WriteBit(0);
            l_Data.WriteBit(!l_IsDone);
            l_Data.FlushBits();
        }

        SendPacket(&l_Data);
    }
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
    sLog->outDebug(LOG_FILTER_LFG, "CMSG_LFG_GET_STATUS [" UI64FMTD "]", GetPlayer()->GetGUID());

    /*uint64 guid = GetPlayer()->GetGUID();
    LfgUpdateData updateData = sLFGMgr->GetLfgStatus(guid);

    if (GetPlayer()->GetGroup())
    {
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
        updateData.dungeons.clear();
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
    }
    else
    {
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
        updateData.dungeons.clear();
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
    }*/
}

void WorldSession::SendLfgRoleChosen(ObjectGuid guid, uint8 roles)
{
    uint8 byteOrder[8] = {4, 2, 5, 1, 6, 3, 0, 7};

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
    data.WriteBytesSeq(guid, byteOrder);
    SendPacket(&data);
}

void WorldSession::SendLfgRoleCheckUpdate(const LfgRoleCheck* pRoleCheck)
{
    ASSERT(pRoleCheck);
    LfgDungeonSet dungeons;
    if (pRoleCheck->rDungeonId)
        dungeons.insert(pRoleCheck->rDungeonId);
    else
        dungeons = pRoleCheck->dungeons;

    ObjectGuid unkGuid = 0;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_ROLE_CHECK_UPDATE [" UI64FMTD "]", GetPlayer()->GetGUID());
    WorldPacket data(SMSG_LFG_ROLE_CHECK_UPDATE, 4 + 1 + 1 + dungeons.size() * 4 + 1 + pRoleCheck->roles.size() * (8 + 1 + 4 + 1));
    ByteBuffer dataBuffer;

    data.WriteBit(pRoleCheck->state == LFG_ROLECHECK_INITIALITING);
    data.WriteBit(unkGuid[5]);
    data.WriteBit(unkGuid[3]);
    data.WriteBit(unkGuid[6]);
    data.WriteBit(unkGuid[4]);
    data.WriteBits(dungeons.size(), 22);
    data.WriteBit(unkGuid[0]);
    data.WriteBit(unkGuid[1]);
    data.WriteBits(pRoleCheck->roles.size(), 21);

    if (!pRoleCheck->roles.empty())
    {
        ObjectGuid guid = pRoleCheck->leader;
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
            if (it->first == pRoleCheck->leader)
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

    data.WriteBit(unkGuid[7]);
    data.WriteBit(unkGuid[2]);
    data.FlushBits();
    data.WriteByteSeq(unkGuid[1]);

    data.append(dataBuffer);
    data << uint8(1);

    data.WriteByteSeq(unkGuid[3]);
    data.WriteByteSeq(unkGuid[4]);
    data.WriteByteSeq(unkGuid[0]);
    data.WriteByteSeq(unkGuid[5]);
    data.WriteByteSeq(unkGuid[6]);

    data << uint8(pRoleCheck->state);

    data.WriteByteSeq(unkGuid[7]);

    if (!dungeons.empty())
    {
        for (LfgDungeonSet::iterator it = dungeons.begin(); it != dungeons.end(); ++it)
        {
            LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(*it);
            data << uint32(dungeon ? dungeon->Entry() : 0); // Dungeon
        }
    }

    data.WriteByteSeq(unkGuid[2]);

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

void WorldSession::SendLfgQueueStatus(uint32 p_Dungeon, int32 p_WaitTime, int32 p_AvgWaitTime, int32 p_WaitTimeTanks, int32 p_WaitTimeHealer, int32 p_WaitTimeDps, uint32 p_QueuedTime, uint8 p_TankCount, uint8 p_HealerCount, uint8 p_DPSCount)
{
    LfgQueueInfo * l_Info = sLFGMgr->GetLfgQueueInfo(GetPlayer()->GetGroup() ? GetPlayer()->GetGroup()->GetGUID() : GetPlayer()->GetGUID());
    
    if (!l_Info)
        return;

    WorldPacket l_Data(SMSG_LFG_QUEUE_STATUS, 4 + 4 + 4 + 4 + 4 +4 + 1 + 1 + 1 + 4);
    l_Data.appendPackGUID(GetPlayer()->GetGUID());          ///< Requester Guid
    l_Data << uint32(0);                                    ///< Id
    l_Data << uint32(2);                                    ///< Type
    l_Data << int32(l_Info->joinTime);                      ///< Time

    l_Data << uint32(p_Dungeon);                            ///< Slot
    l_Data << uint32(p_WaitTime);                           ///< Avg Wait Time Me
    l_Data << uint32(p_AvgWaitTime);                        ///< Avg Wait Time

    l_Data << int32(p_WaitTimeTanks);                       ///< Avg Wait Time By Role 0
    l_Data << uint8(p_TankCount);                           ///< Last Needed 0

    l_Data << int32(p_WaitTimeHealer);                      ///< Avg Wait Time By Role 1
    l_Data << uint8(p_HealerCount);                         ///< Last Needed 1

    l_Data << int32(p_WaitTimeDps);                         ///< Avg Wait Time By Role 2
    l_Data << uint8(p_DPSCount);                            ///< Last Needed 2

    l_Data << uint32(p_QueuedTime);                         ///< Queued Time

    SendPacket(&l_Data);
}

void WorldSession::SendLfgPlayerReward(uint32 rdungeonEntry, uint32 sdungeonEntry, uint8 done, const LfgReward* reward, const Quest* qRew)
{
    if (!rdungeonEntry || !sdungeonEntry || !qRew)
        return;

    uint8 itemNum = uint8(qRew ? qRew->GetRewItemsCount() + qRew->GetRewCurrencyCount()  : 0);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_PLAYER_REWARD [" UI64FMTD "] rdungeonEntry: %u - sdungeonEntry: %u - done: %u", GetPlayer()->GetGUID(), rdungeonEntry, sdungeonEntry, done);

    ByteBuffer bytereward;
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

            data.WriteBit(0);

            iProto = sObjectMgr->GetItemTemplate(qRew->RewardItemId[i]);

            bytereward << uint32(iProto ? iProto->DisplayInfoID : 0);
            bytereward << uint32(qRew->RewardItemId[i]);
            bytereward << uint32(0);
            bytereward << uint8(qRew->RewardItemIdCount[i]);
        }
    }
    if (qRew && qRew->GetRewCurrencyCount())
    {
        for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        {
            if (!qRew->RewardCurrencyId[i])
                continue;

            data.WriteBit(1);

            bytereward << uint32(0);
            bytereward << uint32(qRew->RewardCurrencyId[i]);
            bytereward << uint32(0);
            bytereward << uint32(qRew->RewardCurrencyCount[i]);
        }
    }
    data.append(bytereward);
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
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_BOOT_PLAYER [" UI64FMTD "] inProgress: %u - didVote: %u - agree: %u - victim: [" UI64FMTD "] votes: %u - agrees: %u - left: %u - needed: %u - reason %s",
        guid, uint8(pBoot->inProgress), uint8(playerVote != LFG_ANSWER_PENDING), uint8(playerVote == LFG_ANSWER_AGREE), pBoot->victim, votesNum, agreeNum, secsleft, pBoot->votedNeeded, pBoot->reason.c_str());
    WorldPacket data(SMSG_LFG_BOOT_PLAYER, 1 + 1 + 1 + 8 + 4 + 4 + 4 + 4 + pBoot->reason.length());
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
