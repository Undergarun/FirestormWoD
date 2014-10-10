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
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "Arena.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "Chat.h"
#include "Language.h"
#include "Log.h"
#include "LFG.h"
#include "Player.h"
#include "Object.h"
#include "Opcodes.h"
#include "DisableMgr.h"
#include "Group.h"

void WorldSession::HandleBattlemasterHelloOpcode(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_BATTLEMASTER_HELLO Message from (GUID: %u TypeId:%u)", GUID_LOPART(guid), GuidHigh2TypeId(GUID_HIPART(guid)));

    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
        return;

    if (!unit->isBattleMaster())                             // it's not battlemaster
        return;

    // Stop the npc if moving
    unit->StopMoving();

    BattlegroundTypeId bgTypeId = sBattlegroundMgr->GetBattleMasterBG(unit->GetEntry());

    if (!m_Player->GetBGAccessByLevel(bgTypeId))
    {
                                                            // temp, must be gossip message...
        SendNotification(LANG_YOUR_BG_LEVEL_REQ_ERROR);
        return;
    }

    SendBattleGroundList(guid, bgTypeId);
}

void WorldSession::SendBattleGroundList(uint64 guid, BattlegroundTypeId bgTypeId)
{
    WorldPacket data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&data, guid, m_Player, bgTypeId);
    SendPacket(&data);
}

void WorldSession::HandleBattlemasterJoinOpcode(WorldPacket& p_Packet)
{
    uint64 l_QueueID = 0;
    uint32 l_BlacklistMap[2];
    uint8  l_Roles = 0;
    bool   l_JoinAsGroup = 0;

    p_Packet >> l_QueueID;
    p_Packet >> l_Roles;

    for (int l_I = 0; l_I < 2; l_I++)
        p_Packet >> l_BlacklistMap[l_I];

    l_JoinAsGroup = p_Packet.ReadBit();

    uint32  l_BGTypeID_  = 0;
    uint32  l_InstanceID = 0;
    Group * l_Group      = NULL;

    bool l_IsPremade = false;

    l_BGTypeID_ = GUID_LOPART(l_QueueID);

    if (!sBattlemasterListStore.LookupEntry(l_BGTypeID_))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: invalid bgtype (%u) received. possible cheater? player guid %u", l_BGTypeID_, m_Player->GetGUIDLow());
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, l_BGTypeID_, NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_BG_DISABLED);
        return;
    }

    BattlegroundTypeId l_BGTypeID = BattlegroundTypeId(l_BGTypeID_);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_BATTLEMASTER_JOIN Message from (GUID: %u TypeId:%u)", GUID_LOPART(l_QueueID), GuidHigh2TypeId(GUID_HIPART(l_QueueID)));

    // can do this, since it's battleground, not arena
    BattlegroundQueueTypeId l_BGQueueTypeID         = BattlegroundMgr::BGQueueTypeId(l_BGTypeID, 0);
    BattlegroundQueueTypeId l_BGQueueTypeIDRandom   = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // ignore if player is already in BG
    if (m_Player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* l_BG = sBattlegroundMgr->GetBattlegroundTemplate(l_BGTypeID);

    if (!l_BG)
        return;

    // expected bracket entry
    PvPDifficultyEntry const* l_BracketEntry = GetBattlegroundBracketByLevel(l_BG->GetMapId(), m_Player->getLevel());

    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_Error = ERR_BATTLEGROUND_NONE;

    // check queue conditions
    if (!l_JoinAsGroup)
    {
        // check Deserter debuff
        if (!m_Player->CanJoinToBattleground())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, l_BG, m_Player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        if (m_Player->GetBattlegroundQueueIndex(l_BGQueueTypeIDRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            //player is already in random queue
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, l_BG, m_Player, 0, ERR_IN_RANDOM_BG);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        if (m_Player->InBattlegroundQueue() && l_BGTypeID == BATTLEGROUND_RB)
        {
            //player is already in queue, can't start random queue
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, l_BG, m_Player, 0, ERR_IN_NON_RANDOM_BG);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        // check if already in queue
        if (m_Player->GetBattlegroundQueueIndex(l_BGQueueTypeID) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            //player is already in this queue
            return;

        // check if has free queue slots
        if (!m_Player->HasFreeBattlegroundQueueId())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, l_BG, m_Player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        m_Player->SetBattleGroundRoles(l_Roles);

        BattlegroundQueue& l_BGQueue = sBattlegroundMgr->m_BattlegroundQueues[l_BGQueueTypeID];

        GroupQueueInfo * l_GroupQueueInfo = l_BGQueue.AddGroup(m_Player, NULL, l_BGTypeID, l_BracketEntry, 0, false, l_IsPremade, 0, 0);
        uint32 l_AverageTime    = l_BGQueue.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->GetBracketId());
        uint32 l_QueueSlot      = m_Player->AddBattlegroundQueueId(l_BGQueueTypeID);

        // add joined time data
        m_Player->AddBattlegroundQueueJoinTime(l_BGTypeID, l_GroupQueueInfo->JoinTime);

        WorldPacket l_Data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&l_Data, l_BG, m_Player, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->JoinTime, l_GroupQueueInfo->ArenaType);
        SendPacket(&l_Data);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", l_BGQueueTypeID, l_BGTypeID, m_Player->GetGUIDLow(), m_Player->GetName());
    }
    else
    {
        l_Group = m_Player->GetGroup();

        if (!l_Group)
            return;

        if (l_Group->GetLeaderGUID() != m_Player->GetGUID())
            return;

        l_Error = l_Group->CanJoinBattlegroundQueue(l_BG, l_BGQueueTypeID, 0, l_BG->GetMaxPlayersPerTeam(), false, 0);
        l_IsPremade = (l_Group->GetMembersCount() >= l_BG->GetMinPlayersPerTeam());

        BattlegroundQueue& bgQueue = sBattlegroundMgr->m_BattlegroundQueues[l_BGQueueTypeID];
        GroupQueueInfo* ginfo = NULL;
        uint32 avgTime = 0;

        if (!l_Error)
        {
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: the following players are joining as group:");
            ginfo = bgQueue.AddGroup(m_Player, l_Group, l_BGTypeID, l_BracketEntry, 0, false, l_IsPremade, 0, 0);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, l_BracketEntry->GetBracketId());
        }

        for (GroupReference * l_It = l_Group->GetFirstMember(); l_It != NULL; l_It = l_It->next())
        {
            Player* l_Member = l_It->getSource();

            if (!l_Member)
                continue;   // this should never happen

            if (l_Error)
            {
                WorldPacket data;
                sBattlegroundMgr->BuildStatusFailedPacket(&data, l_BG, m_Player, 0, l_Error);
                l_Member->GetSession()->SendPacket(&data);
                continue;
            }

            // add to queue
            uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

            // add joined time data
            l_Member->AddBattlegroundQueueJoinTime(l_BGTypeID, ginfo->JoinTime);

            WorldPacket l_Data; // send status packet (in queue)
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&l_Data, l_BG, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->ArenaType);
            l_Member->GetSession()->SendPacket(&l_Data);

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", l_BGQueueTypeID, l_BGTypeID, l_Member->GetGUIDLow(), l_Member->GetName());
        }

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: group end");
    }

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, l_BGQueueTypeID, l_BGTypeID, l_BracketEntry->GetBracketId());
}

void WorldSession::HandleBattlegroundPlayerPositionsOpcode(WorldPacket& /*recvData*/)
{
    Battleground* l_BG = m_Player->GetBattleground();

    if (!l_BG)                                                 // can't be received if player not in battleground
        return;

    uint32 l_Count = 0;
    std::list<Player*> l_Players;

    if (Player* plr = ObjectAccessor::FindPlayer(l_BG->GetFlagPickerGUID(TEAM_ALLIANCE)))
    {
        l_Players.push_back(plr);
        ++l_Count;
    }

    if (Player* plr = ObjectAccessor::FindPlayer(l_BG->GetFlagPickerGUID(TEAM_HORDE)))
    {
        l_Players.push_back(plr);
        ++l_Count;
    }

    WorldPacket l_Data(SMSG_BATTLEGROUND_PLAYER_POSITIONS);

    l_Data << uint32(l_Count);

    for (auto l_Itr : l_Players)
    {
        l_Data.appendPackGUID(l_Itr->GetGUID());
        l_Data << float(l_Itr->GetPositionX());
        l_Data << float(l_Itr->GetPositionY());
        l_Data << uint8(l_Itr->GetTeamId() == TEAM_ALLIANCE ? FLAG_ICON_ALLIANCE : FLAG_ICON_HORDE);
        l_Data << uint8(l_Itr->GetBGTeam());
    }

    SendPacket(&l_Data);
}

void WorldSession::HandlePVPLogDataOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd MSG_PVP_LOG_DATA Message");

    Battleground* bg = m_Player->GetBattleground();
    if (!bg)
        return;

    // Prevent players from sending BuildPvpLogDataPacket in an arena except for when sent in BattleGround::EndBattleGround.
    if (bg->isArena())
        return;

    WorldPacket data;
    sBattlegroundMgr->BuildPvpLogDataPacket(&data, bg);
    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent MSG_PVP_LOG_DATA Message");
}

void WorldSession::HandleBattlefieldListOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_BATTLEFIELD_LIST Message");

    uint32 l_ListID = 0;
    p_Packet >> l_ListID;                               ///< ID from DBC

    BattlemasterListEntry const* l_BattleMasterListEntry = sBattlemasterListStore.LookupEntry(l_ListID);

    if (!l_BattleMasterListEntry)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: invalid bgtype (%u) with player (Name: %s, GUID: %u) received.", l_ListID, m_Player->GetName(), m_Player->GetGUIDLow());
        return;
    }

    WorldPacket l_Data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&l_Data, 0, m_Player, BattlegroundTypeId(l_ListID));

    SendPacket(&l_Data);
}

void WorldSession::HandleBattleFieldPortOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_BATTLEFIELD_PORT Message");

    uint64 l_RequesterGuid = 0;
    uint32 l_QueueSlotID = 0;                           ///< guessed
    uint32 l_Time = 0;
    uint32 l_Type = 0;                                  ///< type id from dbc
    uint8 l_AcceptedInvite = 0;                         ///< enter battle 0x1, leave queue 0x0

    p_Packet.readPackGUID(l_RequesterGuid);
    p_Packet >> l_QueueSlotID;
    p_Packet >> l_Type;
    p_Packet >> l_Time;

    l_AcceptedInvite = p_Packet.ReadBit();

    if (l_QueueSlotID == 3)
        l_QueueSlotID = 0;

    if (!m_Player->InBattlegroundQueue())
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: Invalid CMSG_BATTLEFIELD_PORT received from player (Name: %s, GUID: %u), he is not in bg_queue.", m_Player->GetName(), m_Player->GetGUIDLow());
        return;
    }

    if (l_QueueSlotID > PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        sLog->OutPandashan("HandleBattleFieldPortOpcode queueSlot %u", l_QueueSlotID);
        return;
    }

    BattlegroundQueueTypeId l_BGQueueTypeID = m_Player->GetBattlegroundQueueTypeId(l_QueueSlotID);

    if (l_BGQueueTypeID == BATTLEGROUND_QUEUE_NONE)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: invalid queueSlot (%u) received.", l_QueueSlotID);
        return;
    }

    if (l_BGQueueTypeID >= MAX_BATTLEGROUND_QUEUE_TYPES)
    {
        sLog->OutPandashan("HandleBattleFieldPortOpcode bgQueueTypeId %u", l_BGQueueTypeID);
        return;
    }

    BattlegroundQueue& l_BGQueue = sBattlegroundMgr->m_BattlegroundQueues[l_BGQueueTypeID];

    /// We must use temporary variable, because GroupQueueInfo pointer can be deleted in BattlegroundQueue::RemovePlayer() function
    GroupQueueInfo l_GroupQueueInfo;

    if (!l_BGQueue.GetPlayerGroupInfoData(m_Player->GetGUID(), &l_GroupQueueInfo))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: itrplayerstatus not found.");
        return;
    }

    /// if action == 1, then instanceId is required
    if (!l_GroupQueueInfo.IsInvitedToBGInstanceGUID && l_AcceptedInvite == 1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: instance not found.");
        return;
    }

    BattlegroundTypeId l_BGTypeID = BattlegroundMgr::BGTemplateId(l_BGQueueTypeID);
    /// BGTemplateId returns BATTLEGROUND_AA when it is arena queue.
    /// Do instance id search as there is no AA bg instances.
    Battleground * l_BG = sBattlegroundMgr->GetBattleground(l_GroupQueueInfo.IsInvitedToBGInstanceGUID, l_BGTypeID == BATTLEGROUND_AA ? BATTLEGROUND_TYPE_NONE : l_BGTypeID);

    /// BG template might and must be used in case of leaving queue, when instance is not created yet
    if (!l_BG && l_AcceptedInvite == 0)
        l_BG = sBattlegroundMgr->GetBattlegroundTemplate(l_BGTypeID);

    if (!l_BG)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: bg_template not found for type id %u.", l_BGTypeID);
        return;
    }

    /// Get real bg type
    l_BGTypeID = l_BG->GetTypeID();

    /// Expected bracket entry
    const PvPDifficultyEntry * l_BracketEntry = GetBattlegroundBracketByLevel(l_BG->GetMapId(), m_Player->getLevel());

    if (!l_BracketEntry)
        return;

    /// Some checks if player isn't cheating - it is not exactly cheating, but we cannot allow it
    if (l_AcceptedInvite == 1 && l_GroupQueueInfo.ArenaType == 0)
    {
        /// If player is trying to enter battleground (not arena!) and he has deserter debuff, we must just remove him from queue
        if (!m_Player->CanJoinToBattleground())
        {
            /// Send bg command result to show nice message
            WorldPacket l_SecondResponse;
            sBattlegroundMgr->BuildStatusFailedPacket(&l_SecondResponse, l_BG, m_Player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);

            m_Player->GetSession()->SendPacket(&l_SecondResponse);

            l_AcceptedInvite = 0;

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) has a deserter debuff, do not port him to battleground!", m_Player->GetName(), m_Player->GetGUIDLow());
        }
        /// If player don't match battleground max level, then do not allow him to enter! (this might happen when player leveled up during his waiting in queue
        if (m_Player->getLevel() > l_BG->GetMaxLevel())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: Player %s (%u) has level (%u) higher than maxlevel (%u) of battleground (%u)! Do not port him to battleground!",
                m_Player->GetName(), m_Player->GetGUIDLow(), m_Player->getLevel(), l_BG->GetMaxLevel(), l_BG->GetTypeID());

            l_AcceptedInvite = 0;
        }
    }

    WorldPacket l_Response;

    switch (l_AcceptedInvite)
    {
        /// Port to battleground
        case 1:                                         
            if (!m_Player->IsInvitedForBattlegroundQueueType(l_BGQueueTypeID))
                return;                                 ///< cheating?

            if (!m_Player->InBattleground())
                m_Player->SetBattlegroundEntryPoint();

            /// Resurrect the player
            if (!m_Player->isAlive())
            {
                m_Player->ResurrectPlayer(1.0f);
                m_Player->SpawnCorpseBones();
            }

            m_Player->GetMotionMaster()->MovementExpired();
            /// Stop taxi flight at port
            if (m_Player->isInFlight())
                m_Player->CleanupAfterTaxiFlight();

            /// Remove battleground queue status from BGmgr
            l_BGQueue.RemovePlayer(m_Player->GetGUID(), false);
            /// This is still needed here if battleground "jumping" shouldn't add deserter debuff
            /// Also this is required to prevent stuck at old battleground after SetBattlegroundId set to new
            if (Battleground* currentBg = m_Player->GetBattleground())
                currentBg->RemovePlayerAtLeave(m_Player->GetGUID(), false, true);

            /// Set the destination instance id
            m_Player->SetBattlegroundId(l_BG->GetInstanceID(), l_BGTypeID);

            /// Set the destination team
            m_Player->SetBGTeam(l_GroupQueueInfo.Team);

            /// Bg->HandleBeforeTeleportToBattleground(_player);
            sBattlegroundMgr->SendToBattleground(m_Player, l_GroupQueueInfo.IsInvitedToBGInstanceGUID, l_BGTypeID);

            /// Add only in HandleMoveWorldPortAck()
            /// Bg->AddPlayer(_player, team);
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", m_Player->GetName(), m_Player->GetGUIDLow(), l_BG->GetInstanceID(), l_BG->GetTypeID(), l_BGQueueTypeID);
            break;

        /// Leave queue
        case 0:
            if (l_BG->isArena() && l_BG->GetStatus() > STATUS_WAIT_QUEUE)
                return;

            // if player leaves rated arena match before match start, it is counted as he played but he lost
            /*if (ginfo.IsRated && ginfo.IsInvitedToBGInstanceGUID)
            {
                if (ginfo.group)
                {
                    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "UPDATING memberLost's personal arena rating for %u by opponents rating: %u, because he has left queue!", GUID_LOPART(_player->GetGUID()), ginfo.OpponentsTeamRating);
                    ginfo.group->MemberLost(_player, ginfo.OpponentsMatchmakerRating);
                }
            }*/

            sBattlegroundMgr->BuildBattlegroundStatusPacket(&l_Response, l_BG, m_Player, l_QueueSlotID, STATUS_NONE, m_Player->GetBattlegroundQueueJoinTime(l_BGTypeID), 0, 0);
            SendPacket(&l_Response);

            /// Must be called this way, because if you move this call to queue->removeplayer, it causes bugs
            m_Player->RemoveBattlegroundQueueId(l_BGQueueTypeID);  

            l_BGQueue.RemovePlayer(m_Player->GetGUID(), true);
            
            /// Player left queue, we should update it - do not update Arena Queue
            if (!l_GroupQueueInfo.ArenaType)
                sBattlegroundMgr->ScheduleQueueUpdate(l_GroupQueueInfo.ArenaMatchmakerRating, l_GroupQueueInfo.ArenaType, l_BGQueueTypeID, l_BGTypeID, l_BracketEntry->GetBracketId());

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) left queue for bgtype %u, queue type %u.", m_Player->GetName(), m_Player->GetGUIDLow(), l_BG->GetTypeID(), l_BGQueueTypeID);
            break;

        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "Battleground port: unknown action %u", l_AcceptedInvite);
            break;

    }
}

void WorldSession::HandleLeaveBattlefieldOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_LEAVE_BATTLEFIELD Message");

    if (m_Player->InArena())
        if (m_Player->GetBattleground()->GetStatus() == STATUS_WAIT_JOIN)
            return;

    m_Player->LeaveBattleground();
}

void WorldSession::HandleBattlefieldStatusOpcode(WorldPacket& /*recvData*/)
{
    /// Empty opcode
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_REQUEST_BATTLEFIELD_STATUS Message");

    WorldPacket l_Data;

    Battleground * l_BG = NULL;

    /// We must update all queues here
    for (uint8 l_I = 0; l_I < PLAYER_MAX_BATTLEGROUND_QUEUES; ++l_I)
    {
        BattlegroundQueueTypeId l_BGQueueTypeID = m_Player->GetBattlegroundQueueTypeId(l_I);
        
        if (!l_BGQueueTypeID)
            continue;

        BattlegroundTypeId l_BGTypeID = BattlegroundMgr::BGTemplateId(l_BGQueueTypeID);
        uint8 l_ArenaType = BattlegroundMgr::BGArenaType(l_BGQueueTypeID);
        
        if (l_BGTypeID == m_Player->GetBattlegroundTypeId())
        {
            l_BG = m_Player->GetBattleground();

            /// I cannot check any variable from player class because player class doesn't know if player is in 2v2 / 3v3 or 5v5 arena
            /// So i must use bg pointer to get that information
            if (l_BG && l_BG->GetArenaType() == l_ArenaType)
            {
                /// This line is checked, i only don't know if GetElapsedTime() is changing itself after bg end!
                /// Send status in Battleground
                sBattlegroundMgr->BuildBattlegroundStatusPacket(&l_Data, l_BG, m_Player, l_I, STATUS_IN_PROGRESS, m_Player->GetBattlegroundQueueJoinTime(l_BGTypeID), l_BG->GetElapsedTime(), l_ArenaType);
                SendPacket(&l_Data);

                continue;
            }
        }

        /// We are sending update to player about queue - he can be invited there!
        /// get GroupQueueInfo for queue status
        BattlegroundQueue& l_BGQueue = sBattlegroundMgr->m_BattlegroundQueues[l_BGQueueTypeID];
        GroupQueueInfo l_GroupQueueInfo;

        if (!l_BGQueue.GetPlayerGroupInfoData(m_Player->GetGUID(), &l_GroupQueueInfo))
            continue;

        if (l_GroupQueueInfo.IsInvitedToBGInstanceGUID)
        {
            l_BG = sBattlegroundMgr->GetBattleground(l_GroupQueueInfo.IsInvitedToBGInstanceGUID, l_BGTypeID);
            if (!l_BG)
                continue;

            /// Send status invited to Battleground
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&l_Data, l_BG, GetPlayer(), l_I, STATUS_WAIT_JOIN, getMSTimeDiff(getMSTime(), l_GroupQueueInfo.RemoveInviteTime), m_Player->GetBattlegroundQueueJoinTime(l_BGTypeID), l_ArenaType);
            SendPacket(&l_Data);
        }
        else
        {
            l_BG = sBattlegroundMgr->GetBattlegroundTemplate(l_BGTypeID);
            if (!l_BG)
                continue;

            /// Expected bracket entry
            PvPDifficultyEntry const* l_BracketEntry = GetBattlegroundBracketByLevel(l_BG->GetMapId(), m_Player->getLevel());

            if (!l_BracketEntry)
                continue;

            uint32 l_AverageTime = l_BGQueue.GetAverageQueueWaitTime(&l_GroupQueueInfo, l_BracketEntry->GetBracketId());
            
            /// Send status in Battleground Queue
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&l_Data, l_BG, GetPlayer(), l_I, STATUS_WAIT_QUEUE, l_AverageTime, m_Player->GetBattlegroundQueueJoinTime(l_BGTypeID), l_ArenaType);
            SendPacket(&l_Data);
        }
    }
}

void WorldSession::HandleBattlemasterJoinArena(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_BATTLEMASTER_JOIN_ARENA");

    uint8 l_TeamSizeIndex;                                        ///< 2v2, 3v3 or 5v5

    p_Packet >> l_TeamSizeIndex;

    /// Ignore if we already in BG or BG queue
    if (m_Player->InBattleground())
        return;

    uint32 l_ArenaRating        = 0;
    uint32 l_MatchmakerRating   = 0;

    uint8 l_ArenaType = Arena::GetTypeBySlot(l_TeamSizeIndex);

    /// Check existance
    Battleground* l_Battleground = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);

    if (!l_Battleground)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (all arenas) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    BattlegroundTypeId      l_BGTypeId      = l_Battleground->GetTypeID();
    BattlegroundQueueTypeId l_BGQueueTypeID = BattlegroundMgr::BGQueueTypeId(l_BGTypeId, l_ArenaType);

    PvPDifficultyEntry const* l_BracketEntry = GetBattlegroundBracketByLevel(l_Battleground->GetMapId(), m_Player->getLevel());
    
    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_ResultError = ERR_BATTLEGROUND_NONE;

    Group* l_Group = m_Player->GetGroup();

    /// No group found, error
    if (!l_Group)
        return;

    if (l_Group->GetLeaderGUID() != m_Player->GetGUID())
        return;

    uint32 l_PlayerDivider = 0;

    for (const GroupReference * l_GroupReference = l_Group->GetFirstMember(); l_GroupReference != NULL; l_GroupReference = l_GroupReference->next())
    {
        if (const Player * l_GroupMember = l_GroupReference->getSource())
        {
            l_ArenaRating       += l_GroupMember->GetArenaPersonalRating(l_TeamSizeIndex);
            l_MatchmakerRating  += l_GroupMember->GetArenaMatchMakerRating(l_TeamSizeIndex);

            ++l_PlayerDivider;
        }
    }

    if (!l_PlayerDivider)
        return;

    l_ArenaRating       /= l_PlayerDivider;
    l_MatchmakerRating  /= l_PlayerDivider;

    if (l_ArenaRating <= 0)
        l_ArenaRating = 1;

    if (l_MatchmakerRating <= 0)
        l_MatchmakerRating = 1;

    BattlegroundQueue & l_BGQueue = sBattlegroundMgr->GetBattlegroundQueue(l_BGQueueTypeID);

    uint32 l_AverageTime = 0;
    GroupQueueInfo * l_GroupQueueInfo = NULL;

    l_ResultError = l_Group->CanJoinBattlegroundQueue(l_Battleground, l_BGQueueTypeID, l_ArenaType, l_ArenaType, true, l_TeamSizeIndex);
    
    if (!l_ResultError || (l_ResultError && sBattlegroundMgr->isArenaTesting()))
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: leader %s queued with matchmaker rating %u for type %u", m_Player->GetName(), l_MatchmakerRating, l_ArenaType);

        l_GroupQueueInfo = l_BGQueue.AddGroup(m_Player, l_Group, l_BGTypeId, l_BracketEntry, l_ArenaType, true, false, l_ArenaRating, l_MatchmakerRating);
        l_AverageTime = l_BGQueue.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->GetBracketId());
    }

    for (GroupReference * l_It = l_Group->GetFirstMember(); l_It != NULL; l_It = l_It->next())
    {
        Player * l_Member = l_It->getSource();

        if (!l_Member)
            continue;

        if (l_ResultError && !sBattlegroundMgr->isArenaTesting())
        {
            WorldPacket l_Data;
            sBattlegroundMgr->BuildStatusFailedPacket(&l_Data, l_Battleground, m_Player, 0, l_ResultError);

            l_Member->GetSession()->SendPacket(&l_Data);
            continue;
        }

        /// Add to queue
        uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

        if (!l_GroupQueueInfo)
        {
            sLog->OutPandashan("NULL ginfo !!!!");
            return;
        }

        /// Add joined time data
        l_Member->AddBattlegroundQueueJoinTime(l_BGTypeId, l_GroupQueueInfo->JoinTime);

        WorldPacket l_Data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&l_Data, l_Battleground, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->JoinTime, l_ArenaType);
        
        l_Member->GetSession()->SendPacket(&l_Data);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", l_BGQueueTypeID, l_BGTypeId, l_Member->GetGUIDLow(), l_Member->GetName());
    }

    sBattlegroundMgr->ScheduleQueueUpdate(l_MatchmakerRating, l_ArenaType, l_BGQueueTypeID, l_BGTypeId, l_BracketEntry->GetBracketId());
}

void WorldSession::HandleBattlemasterJoinRated(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_BATTLEMASTER_JOIN_RATED");

    // ignore if we already in BG or BG queue
    if (m_Player->InBattleground())
        return;

    uint32 personalRating = 0;
    uint32 matchmakerRating = 0;

    //check existance
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_RATED_10_VS_10);
    if (!bg)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (10 vs 10) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_RATED_10_VS_10, NULL))
        return;

    BattlegroundTypeId bgTypeId = bg->GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), m_Player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

    Group* grp = m_Player->GetGroup();

    // no group found, error
    if (!grp)
        return;

    if (grp->GetLeaderGUID() != m_Player->GetGUID())
        return;

    uint32 playerDivider = 0;
    for (GroupReference const* ref = grp->GetFirstMember(); ref != NULL; ref = ref->next())
    {
        if (Player const* groupMember = ref->getSource())
        {
            personalRating += groupMember->GetArenaPersonalRating(SLOT_RBG);
            matchmakerRating += groupMember->GetArenaMatchMakerRating(SLOT_RBG);
        }
    }

    if (!playerDivider)
        return;

    personalRating /= playerDivider;
    matchmakerRating /= playerDivider;

    if (personalRating <= 0)
        personalRating = 0;
    if (matchmakerRating <= 0)
        matchmakerRating = 1;

    BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    uint32 avgTime = 0;
    GroupQueueInfo* ginfo;

    err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 10, 10, true, 0);
    if (!err)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: leader %s queued");

        ginfo = bgQueue.AddGroup(m_Player, grp, bgTypeId, bracketEntry, 0, false, true, personalRating, matchmakerRating);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
    }

    for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member)
            continue;

        if (err)
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, m_Player, 0, err);
            member->GetSession()->SendPacket(&data);
            continue;
        }

         // add to queue
        uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

        // add joined time data
        member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->ArenaType);
        member->GetSession()->SendPacket(&data);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for rated battleground as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName());
    }

    sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void WorldSession::HandleBattleFieldRequestScoreData(WorldPacket & p_Packet)
{
    if (!m_Player || !m_Player->GetBattleground())
        return;

    WorldPacket l_ScoreData;
    sBattlegroundMgr->BuildPvpLogDataPacket(&l_ScoreData, m_Player->GetBattleground());

    m_Player->SendDirectMessage(&l_ScoreData);
}

void WorldSession::HandleWargameQueryOpcode(WorldPacket& p_RecvData)
{
    ObjectGuid l_FirstLeader = 0;
    ObjectGuid l_SecondLeader = 0;

    l_FirstLeader[7] = p_RecvData.ReadBit();
    l_FirstLeader[0] = p_RecvData.ReadBit();
    l_FirstLeader[6] = p_RecvData.ReadBit();
    l_FirstLeader[3] = p_RecvData.ReadBit();
    l_FirstLeader[1] = p_RecvData.ReadBit();
    l_FirstLeader[2] = p_RecvData.ReadBit();
    l_SecondLeader[1] = p_RecvData.ReadBit();
    l_FirstLeader[5] = p_RecvData.ReadBit();
    l_SecondLeader[6] = p_RecvData.ReadBit();
    l_SecondLeader[3] = p_RecvData.ReadBit();
    l_SecondLeader[0] = p_RecvData.ReadBit();
    l_SecondLeader[5] = p_RecvData.ReadBit();
    l_SecondLeader[2] = p_RecvData.ReadBit();
    l_SecondLeader[4] = p_RecvData.ReadBit();
    l_SecondLeader[7] = p_RecvData.ReadBit();
    l_FirstLeader[4] = p_RecvData.ReadBit();
}
