////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "Arena.h"
#include "BattlegroundMgr.hpp"
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

#include "BattlegroundPacketFactory.hpp"
#include "BattlegroundScheduler.hpp"

#include "InterRealmOpcodes.h"

void WorldSession::HandleBattlemasterHelloOpcode(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;

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
    MS::Battlegrounds::PacketFactory::List(&data, guid, m_Player, bgTypeId, 0, false, false, false, false);
    SendPacket(&data);
}

void WorldSession::HandleBattlemasterJoinOpcode(WorldPacket& p_Packet)
{
    bool l_InterRealmEnable     = sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE);
    InterRealmSession* l_Tunnel = nullptr;

    if (l_InterRealmEnable)
    {
        l_Tunnel = sWorld->GetInterRealmSession();
        if (!l_Tunnel || !l_Tunnel->IsTunnelOpened())
        {
            ChatHandler(this).PSendSysMessage(LANG_INTERREALM_DISABLED);
            return;
        }
    }

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
    uint32  l_InstanceID = 0; ///< l_InstanceID is unused
    Group*  l_Group      = nullptr;
    bool    l_IsPremade  = false;

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

    // can do this, since it's battleground, not arena
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = MS::Battlegrounds::GetTypeFromId(l_BGTypeID, 0);
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeIDRandom = MS::Battlegrounds::GetTypeFromId(BATTLEGROUND_RB, 0);

    // ignore if player is already in BG
    if (m_Player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* l_BG = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::GetSchedulerType(l_BGTypeID));

    if (!l_BG)
        return;

    // expected bracket entry
    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(m_Player->getLevel());

    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_Error = ERR_BATTLEGROUND_NONE;

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    // check queue conditions
    if (!l_JoinAsGroup)
    {
        // check Deserter debuff
        if (!m_Player->CanJoinToBattleground())
        {
            WorldPacket data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, m_Player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        if (m_Player->GetBattlegroundQueueIndex(l_BGQueueTypeIDRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            if (l_BGTypeID == BATTLEGROUND_RB)
                return;

            /// Player is already in random queue.
            WorldPacket data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, m_Player, 0, ERR_IN_RANDOM_BG);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        if (m_Player->InBattlegroundQueue() && l_BGTypeID == BATTLEGROUND_RB)
        {
            //player is already in queue, can't start random queue
            WorldPacket data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, m_Player, 0, ERR_IN_NON_RANDOM_BG);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        // check if already in queue
        if (m_Player->GetBattlegroundQueueIndex(l_BGQueueTypeID) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            //player is already in this queue
            WorldPacket data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, m_Player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        // check if has free queue slots
        if (!m_Player->HasFreeBattlegroundQueueId())
        {
            WorldPacket data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, m_Player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            m_Player->GetSession()->SendPacket(&data);
            return;
        }

        if (!l_InterRealmEnable)
        {
            m_Player->SetBattleGroundRoles(l_Roles);

            GroupQueueInfo * l_GroupQueueInfo = l_Scheduler.AddGroup(m_Player, nullptr, l_BGQueueTypeID, l_BlacklistMap, l_BracketEntry, ArenaType::None, false, 0, 0, false);
            uint32 l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);
            uint32 l_QueueSlot = m_Player->AddBattlegroundQueueId(l_BGQueueTypeID);

            // add joined time data
            m_Player->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

            WorldPacket l_Data; // send status packet (in queue)
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, m_Player, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->m_JoinTime, l_GroupQueueInfo->m_ArenaType, false);
            SendPacket(&l_Data);

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", l_BGQueueTypeID, l_BGTypeID, m_Player->GetGUIDLow(), m_Player->GetName());
        }
    }
    else
    {
        l_Group = m_Player->GetGroup();

        if (!l_Group)
            return;

        if (l_Group->GetLeaderGUID() != m_Player->GetGUID())
            return;

        l_Error = l_Group->CanJoinBattlegroundQueue(l_BG, l_BGQueueTypeID, 0);
        l_IsPremade = (l_Group->GetMembersCount() >= l_BG->GetMinPlayersPerTeam()); ///< l_ispremade is never read 01/18/16

        GroupQueueInfo* ginfo = NULL;
        uint32 avgTime = 0;

        if (!l_Error && !l_InterRealmEnable)
        {
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: the following players are joining as group:");
            ginfo = l_Scheduler.AddGroup(m_Player, l_Group, l_BGQueueTypeID, l_BlacklistMap, l_BracketEntry, ArenaType::None, false, 0, 0, false);
            avgTime = l_InvitationsMgr.GetAverageQueueWaitTime(ginfo, l_BracketEntry->m_Id);
        }

        for (GroupReference * l_It = l_Group->GetFirstMember(); l_It != NULL; l_It = l_It->next())
        {
            Player* l_Member = l_It->getSource();

            if (!l_Member)
                continue;   // this should never happen

            if (l_Error)
            {
                WorldPacket data;
                MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, m_Player, 0, l_Error);
                l_Member->GetSession()->SendPacket(&data);
                continue;
            }

            if (!l_InterRealmEnable)
            {
                // add to queue
                uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

                // add joined time data
                l_Member->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, ginfo->m_JoinTime);

                WorldPacket l_Data; // send status packet (in queue)
                MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->m_JoinTime, ginfo->m_ArenaType, false);
                l_Member->GetSession()->SendPacket(&l_Data);

                sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s", l_BGQueueTypeID, l_BGTypeID, l_Member->GetGUIDLow(), l_Member->GetName());
            }
        }

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: group end");
    }

    if (l_Error || !l_InterRealmEnable)
        return;

    if (!l_JoinAsGroup)
        l_Tunnel->SendRegisterPlayer(m_Player, l_InstanceID, l_QueueID, l_BGTypeID, l_Roles, l_BlacklistMap);
    else
        l_Tunnel->SendRegisterGroup(l_Group, l_InstanceID, l_QueueID, l_BGTypeID, l_Roles, l_BlacklistMap);
}

void WorldSession::HandlePVPLogDataOpcode(WorldPacket& /*recvData*/)
{
    Battleground* bg = m_Player->GetBattleground();
    if (!bg)
        return;

    // Prevent players from sending BuildPvpLogDataPacket in an arena except for when sent in BattleGround::EndBattleGround.
    if (bg->isArena())
        return;

    WorldPacket data;
    MS::Battlegrounds::PacketFactory::PvpLogData(&data, bg);
    SendPacket(&data);
}

void WorldSession::HandleBattlefieldListOpcode(WorldPacket& p_Packet)
{
    uint32 l_ListID = 0;
    p_Packet >> l_ListID;                               ///< ID from DBC

    BattlemasterListEntry const* l_BattleMasterListEntry = sBattlemasterListStore.LookupEntry(l_ListID);

    if (!l_BattleMasterListEntry)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundHandler: invalid bgtype (%u) with player (Name: %s, GUID: %u) received.", l_ListID, m_Player->GetName(), m_Player->GetGUIDLow());
        return;
    }

    /// InterRealm
    InterRealmSession* l_Tunnel = sWorld->GetInterRealmSession();
    if (l_Tunnel && l_Tunnel->IsTunnelOpened())
    {
        WorldPacket l_Data(IR_CMSG_BATTLEGROUND_LIST_QUERY, 8 + 4 + 1 + 1);
        l_Data << uint64(GetPlayer()->GetGUID());
        l_Data << uint32(l_ListID);
        l_Data << uint8(GetPlayer()->GetRandomWinner());
        l_Data << uint8(GetPlayer()->getLevel());

        l_Tunnel->SendPacket(&l_Data);
    }
    else
    {
        WorldPacket l_Data;
        MS::Battlegrounds::PacketFactory::List(&l_Data, 0, m_Player, BattlegroundTypeId(l_ListID), l_ListID, false, false, false, false);

        SendPacket(&l_Data);
    }
}

void WorldSession::HandleBattleFieldPortOpcode(WorldPacket& p_Packet)
{
    bool l_InterRealmEnable = sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE);
    InterRealmSession* l_Tunnel = nullptr;

    if (l_InterRealmEnable)
    {
        l_Tunnel = sWorld->GetInterRealmSession();
        if (!l_Tunnel || !l_Tunnel->IsTunnelOpened())
        {
            ChatHandler(this).PSendSysMessage(LANG_INTERREALM_DISABLED);
            return;
        }
    }

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

    if (l_QueueSlotID > PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        sLog->outAshran("HandleBattleFieldPortOpcode queueSlot %u", l_QueueSlotID);
        return;
    }

    if (l_InterRealmEnable)
    {
        if (l_AcceptedInvite == 1)
        {
            if (m_Player->GetInterRealmPlayerState() > InterRealmPlayerState::None)
            {
                sLog->outAshran("HandleBattleFieldPortOpcode: Try to transfer player to cross, but he is already in transfer");
                return;
            }

            m_Player->RemoveAurasByType(SPELL_AURA_MOUNTED);
 
            if (!m_Player->InBattleground())
                m_Player->SetBattlegroundEntryPoint();
 
            // resurrect the player
            if (!m_Player->isAlive())
            {
                m_Player->ResurrectPlayer(1.0f);
                m_Player->SpawnCorpseBones();
            }
            // stop taxi flight at port
            if (m_Player->isInFlight())
            {
                m_Player->GetMotionMaster()->MovementExpired();
                m_Player->CleanupAfterTaxiFlight();
            }
 
            SetBattlegroundPortData(GetPlayer()->GetGUID(), l_Time, l_QueueSlotID, l_AcceptedInvite);
            GetPlayer()->SetInterRealmPlayerState(InterRealmPlayerState::InTransfer);

            uint32 l_AccountID = GetAccountId();

            GetPlayer()->SaveToDB(false, std::make_shared<MS::Utilities::Callback>([l_AccountID](bool p_Success) -> void
            {
                WorldSession* l_Session = sWorld->FindSession(l_AccountID);
                if (l_Session == nullptr)
                    return;

                if (InterRealmSession* ir_session = sWorld->GetInterRealmSession())
                {
                    BattlegroundPortData l_PortData = l_Session->GetBattlegroundPortData();

                    WorldPacket pckt(IR_CMSG_BATTLEFIELD_PORT, 8 + 4 + 4 + 1);

                    pckt << uint64(l_PortData.PlayerGuid);
                    pckt << uint32(l_PortData.Time);
                    pckt << uint32(l_PortData.QueueSlot);
                    pckt << uint8(l_PortData.Action);

                    ir_session->SendPacket(&pckt);
                }
            }));
        }
        else
        {
            WorldPacket pckt(IR_CMSG_BATTLEFIELD_PORT, 8 + 4 + 4 + 1);
 
            pckt << uint64(GetPlayer()->GetGUID());
            pckt << uint32(l_Time);
            pckt << uint32(l_QueueSlotID);
            pckt << uint8(l_AcceptedInvite);
 
            l_Tunnel->SendPacket(&pckt);
        }
        return;
    }

    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = m_Player->GetBattlegroundQueueTypeId(l_QueueSlotID);

    if (l_BGQueueTypeID >= MS::Battlegrounds::BattlegroundType::Total)
    {
        sLog->outAshran("HandleBattleFieldPortOpcode bgQueueTypeId %u", l_BGQueueTypeID);
        return;
    }

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    /// We must use temporary variable, because GroupQueueInfo pointer can be deleted in BattlegroundQueue::RemovePlayer() function
    GroupQueueInfo l_GroupQueueInfo;

    if (!l_InvitationsMgr.GetPlayerGroupInfoData(m_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID) && !l_Scheduler.GetPlayerGroupInfoData(m_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: itrplayerstatus not found.");
        return;
    }

    /// if action == 1, then instanceId is required
    if (!l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID && l_AcceptedInvite == 1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: instance not found.");
        return;
    }

    /// BGTemplateId returns BATTLEGROUND_AA when it is arena queue.
    /// Do instance id search as there is no AA bg instances.
    Battleground * l_BG = sBattlegroundMgr->GetBattleground(l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID, l_BGQueueTypeID);

    /// BG template might and must be used in case of leaving queue, when instance is not created yet
    if (!l_BG && l_AcceptedInvite == 0)
        l_BG = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::IsArena(l_BGQueueTypeID) ? MS::Battlegrounds::BattlegroundType::AllArenas : l_BGQueueTypeID);

    if (!l_BG)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: bg_template not found for type id %u.", l_BGQueueTypeID);
        return;
    }

    /// Get real bg type
    BattlegroundTypeId l_BGTypeID = l_BG->GetTypeID();

    /// Expected bracket entry
    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(m_Player->getLevel());

    if (!l_BracketEntry)
        return;

    /// Some checks if player isn't cheating - it is not exactly cheating, but we cannot allow it
    if (l_AcceptedInvite == 1 && l_GroupQueueInfo.m_ArenaType == 0)
    {
        /// If player is trying to enter battleground (not arena!) and he has deserter debuff, we must just remove him from queue
        if (!m_Player->CanJoinToBattleground())
        {
            /// Send bg command result to show nice message
            WorldPacket l_SecondResponse;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_SecondResponse, l_BG, m_Player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);

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
                return; ///< cheating?

            if (!m_Player->InBattleground())
                m_Player->SetBattlegroundEntryPoint();

            /// Resurrect the player
            if (!m_Player->isAlive())
            {
                m_Player->ResurrectPlayer(1.0f);
                m_Player->SpawnCorpseBones();
            }

            /// Stop taxi flight at port
            if (m_Player->isInFlight())
            {
                m_Player->GetMotionMaster()->MovementExpired();
                m_Player->CleanupAfterTaxiFlight();
            }

            /// This is still needed here if battleground "jumping" shouldn't add deserter debuff
            /// Also this is required to prevent stuck at old battleground after SetBattlegroundId set to new
            if (Battleground* currentBg = m_Player->GetBattleground())
                currentBg->RemovePlayerAtLeave(m_Player->GetGUID(), false, true);

            /// Set the destination instance id
            m_Player->SetBattlegroundId(l_BG->GetInstanceID(), l_BGTypeID);

            /// Set the destination team
            m_Player->SetBGTeam(l_GroupQueueInfo.m_Team);

            /// Bg->HandleBeforeTeleportToBattleground(_player);
            sBattlegroundMgr->TeleportToBattleground(m_Player, l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID, l_BGQueueTypeID);

            /// Add only in HandleMoveWorldPortAck()
            /// Bg->AddPlayer(_player, team);
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", m_Player->GetName(), m_Player->GetGUIDLow(), l_BG->GetInstanceID(), l_BG->GetTypeID(), l_BGQueueTypeID);
            break;

        /// Leave queue
        case 0:
            if (l_BG->isArena() && l_BG->GetStatus() > STATUS_WAIT_JOIN)
                return;

            MS::Battlegrounds::PacketFactory::Status(&l_Response, l_BG, m_Player, l_QueueSlotID, STATUS_NONE, m_Player->GetBattlegroundQueueJoinTime(l_BGQueueTypeID), 0, l_GroupQueueInfo.m_ArenaType, l_GroupQueueInfo.m_IsSkirmish);
            SendPacket(&l_Response);

            /// Must be called this way, because if you move this call to queue->removeplayer, it causes bugs
            m_Player->RemoveBattlegroundQueueId(l_BGQueueTypeID);

            sBattlegroundMgr->RemovePlayer(m_Player->GetGUID(), true, l_BGQueueTypeID);
            m_Player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
            break;
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "Battleground port: unknown action %u", l_AcceptedInvite);
            break;
    }
}

void WorldSession::HandleLeaveBattlefieldOpcode(WorldPacket& /*p_RecvData*/)
{
    if (m_Player == nullptr)
        return;

    bool l_InterRealmEnable = sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE);
    InterRealmSession* l_Tunnel = nullptr;

    if (l_InterRealmEnable)
    {
        l_Tunnel = sWorld->GetInterRealmSession();
        if (!l_Tunnel || !l_Tunnel->IsTunnelOpened())
        {
            ChatHandler(this).PSendSysMessage(LANG_INTERREALM_DISABLED);
            return;
        }
    }

    if (l_InterRealmEnable)
    {
        WorldPacket l_Data(IR_CMSG_BATTLEFIELD_LEAVE);
        l_Data << uint64(m_Player->GetGUID());
        l_Tunnel->SendPacket(&l_Data);
        return;
    }

    if (m_Player->InArena())
        if (m_Player->GetBattleground()->GetStatus() == STATUS_WAIT_JOIN)
            return;

    m_Player->LeaveBattleground();
}

void WorldSession::HandleBattlefieldStatusOpcode(WorldPacket& /*recvData*/)
{
    if (sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE))
    {
        InterRealmSession* l_Tunnel = sWorld->GetInterRealmSession();
        if (!l_Tunnel || !l_Tunnel->IsTunnelOpened())
        {
            ChatHandler(this).PSendSysMessage(LANG_INTERREALM_DISABLED);
            return;
        }

        WorldPacket l_Data(IR_CMSG_BATTLEFIELD_STATUS_QUERY, 8);
        l_Data << uint64(GetPlayer()->GetGUID());
        l_Tunnel->SendPacket(&l_Data);

        return;
    }

    WorldPacket l_Data;

    Battleground * l_BG = nullptr;

    /// We must update all queues here
    for (uint8 l_I = 0; l_I < PLAYER_MAX_BATTLEGROUND_QUEUES; ++l_I)
    {
        MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = m_Player->GetBattlegroundQueueTypeId(l_I);
        
        /// Can also be random BG
        if (l_BGQueueTypeID == MS::Battlegrounds::BattlegroundType::End)
            continue;

        BattlegroundTypeId l_BGTypeId = MS::Battlegrounds::GetIdFromType(l_BGQueueTypeID); ///< l_bgtypeid is never read 01/18/16
        uint8 l_ArenaType = MS::Battlegrounds::BGArenaType(l_BGQueueTypeID);
        
        if (MS::Battlegrounds::GetIdFromType(l_BGQueueTypeID) == m_Player->GetBattlegroundTypeId())
        {
            l_BG = m_Player->GetBattleground();

            /// I cannot check any variable from player class because player class doesn't know if player is in 2v2 / 3v3 or 5v5 arena
            /// So i must use bg pointer to get that information
            if (l_BG && l_BG->GetArenaType() == l_ArenaType)
            {
                /// This line is checked, i only don't know if GetElapsedTime() is changing itself after bg end!
                /// Send status in Battleground
                MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, m_Player, l_I, STATUS_IN_PROGRESS, l_BG->GetExpirationDate(), l_BG->GetElapsedTime(), l_ArenaType, false);
                SendPacket(&l_Data);

                continue;
            }
        }

        /// We are sending update to player about queue - he can be invited there!
        /// get GroupQueueInfo for queue status
        MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
        MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

        GroupQueueInfo l_GroupQueueInfo;
        if (!l_InvitationsMgr.GetPlayerGroupInfoData(m_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID))
            if (!l_Scheduler.GetPlayerGroupInfoData(m_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID))
                continue;

        if (l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID)
        {
            l_BG = sBattlegroundMgr->GetBattleground(l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID, l_BGQueueTypeID);
            if (!l_BG)
                continue;

            /// Send status invited to Battleground
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, GetPlayer(), l_I, STATUS_WAIT_JOIN, l_BG->GetExpirationDate(), m_Player->GetBattlegroundQueueJoinTime(l_BGQueueTypeID), l_ArenaType, false);
            SendPacket(&l_Data);
        }
        else
        {
            l_BG = sBattlegroundMgr->GetBattlegroundTemplate(l_BGQueueTypeID);
            if (!l_BG)
                continue;

            /// Expected bracket entry
            MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(m_Player->getLevel());

            if (!l_BracketEntry)
                continue;

            uint32 l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(&l_GroupQueueInfo, l_BracketEntry->m_Id);
            
            /// Send status in Battleground Queue
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, GetPlayer(), l_I, STATUS_WAIT_QUEUE, l_AverageTime, m_Player->GetBattlegroundQueueJoinTime(l_BGQueueTypeID), l_ArenaType, l_GroupQueueInfo.m_IsSkirmish);
            SendPacket(&l_Data);
        }
    }
}

void WorldSession::HandleBattlemasterJoinArena(WorldPacket& p_Packet)
{
    bool l_InterRealmEnable = sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE);
    InterRealmSession* l_Tunnel = nullptr;

    if (l_InterRealmEnable)
    {
        l_Tunnel = sWorld->GetInterRealmSession();
        if (!l_Tunnel || !l_Tunnel->IsTunnelOpened())
        {
            ChatHandler(this).PSendSysMessage(LANG_INTERREALM_DISABLED);
            return;
        }

        if (GetInterRealmBG())
            return;
    }

    uint8 l_TeamSizeIndex;                                        ///< 2v2, 3v3 or 5v5

    p_Packet >> l_TeamSizeIndex;

    /// Ignore if we already in BG or BG queue
    if (m_Player->InBattleground())
        return;

    uint32 l_ArenaRating        = 0;
    uint32 l_MatchmakerRating   = 0;

    ArenaType l_ArenaType = Arena::GetTypeBySlot(l_TeamSizeIndex);

    /// Check existance
    Battleground* l_Battleground = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::AllArenas);

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
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = MS::Battlegrounds::GetTypeFromId(l_BGTypeId, l_ArenaType);

    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(m_Player->getLevel());
    
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

    for (const GroupReference * l_GroupReference = l_Group->GetFirstMember(); l_GroupReference != nullptr; l_GroupReference = l_GroupReference->next())
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

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    uint32 l_AverageTime = 0;
    GroupQueueInfo * l_GroupQueueInfo = nullptr;

    l_ResultError = l_Group->CanJoinBattlegroundQueue(l_Battleground, l_BGQueueTypeID, l_ArenaType);
    
    if (!l_ResultError || (l_ResultError && sBattlegroundMgr->isArenaTesting()))
    {
        if (!l_InterRealmEnable)
        {
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: leader %s queued with matchmaker rating %u for type %u", m_Player->GetName(), l_MatchmakerRating, l_ArenaType);

            l_GroupQueueInfo = l_Scheduler.AddGroup(m_Player, l_Group, l_BGQueueTypeID, nullptr, l_BracketEntry, l_ArenaType, true, l_ArenaRating, l_MatchmakerRating, false);
            l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);
        }
    }

    std::list<Player*> l_Players;

    for (GroupReference * l_It = l_Group->GetFirstMember(); l_It != NULL; l_It = l_It->next())
    {
        Player * l_Member = l_It->getSource();

        if (!l_Member)
            continue;

        if (l_ResultError && !sBattlegroundMgr->isArenaTesting())
        {
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, m_Player, 0, l_ResultError);

            l_Member->GetSession()->SendPacket(&l_Data);
            continue;
        }

        l_Players.push_back(l_Member);

        if (!l_InterRealmEnable)
        {
            /// Add to queue
            uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

            if (!l_GroupQueueInfo)
                return;

            /// Add joined time data
            l_Member->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

            WorldPacket l_Data; // send status packet (in queue)
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_Battleground, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->m_JoinTime, l_ArenaType, false);

            l_Member->GetSession()->SendPacket(&l_Data);

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", l_BGQueueTypeID, l_BGTypeId, l_Member->GetGUIDLow(), l_Member->GetName());
        }
    }

    if (!l_InterRealmEnable || l_ResultError)
        return;

    l_Tunnel->SendRegisterArena(l_Players, l_ArenaType, false);
}

void WorldSession::HandleBattlemasterJoinArenaSkirmish(WorldPacket& p_Packet)
{
    bool l_InterRealmEnable = sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE);
    InterRealmSession* l_Tunnel = nullptr;

    if (l_InterRealmEnable)
    {
        l_Tunnel = sWorld->GetInterRealmSession();
        if (!l_Tunnel || !l_Tunnel->IsTunnelOpened())
        {
            ChatHandler(this).PSendSysMessage(LANG_INTERREALM_DISABLED);
            return;
        }

        if (GetInterRealmBG())
            return;
    }

    uint8 l_Roles;
    uint8 l_Bracket;
    bool  l_JoinAsGroup;
    bool  l_Unknow;

    l_JoinAsGroup = p_Packet.ReadBit();
    l_Unknow      = p_Packet.ReadBit();     ///< Unused, always sended at 0 client-side ///< l_unknow is never read 01/18/16
    l_Roles       = p_Packet.read<uint8>();
    l_Bracket     = p_Packet.read<uint8>();

    /// Ignore if we already in BG or BG queue
    if (m_Player->InBattleground())
        return;

    /// - Check if bracket is allowed as arena skrimish
    bool l_AllowedBracket = false;
    for (SkirmishTypeId const& l_SkrimishType : g_SkirmishTypes)
    {
        if (uint8(l_SkrimishType) == l_Bracket)
        {
            l_AllowedBracket = true;
            break;
        }
    }

    if (!l_AllowedBracket)
        return;

    ArenaType l_ArenaType = ArenaType::None;
    switch (l_Bracket)
    {
        case (uint8)SkirmishTypeId::Skrimish2v2:
            l_ArenaType = ArenaType::Arena2v2;
            break;
        case (uint8)SkirmishTypeId::Skrimish3v3:
            l_ArenaType = ArenaType::Arena3v3;
            break;
    }

    /// Check existance
    Battleground* l_Battleground = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::AllArenas);
    if (!l_Battleground)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (all arenas) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, nullptr))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    BattlegroundTypeId l_BGTypeId = l_Battleground->GetTypeID();
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID  = MS::Battlegrounds::GetTypeFromId(l_BGTypeId, l_ArenaType, true);

    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(m_Player->getLevel());
    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_ResultError = ERR_BATTLEGROUND_NONE;

    Group* l_Group = m_Player->GetGroup();
    if (l_JoinAsGroup && !l_Group)
        return;

    if (l_JoinAsGroup && l_Group->GetLeaderGUID() != m_Player->GetGUID())
        return;

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    uint32 l_AverageTime = 0;
    GroupQueueInfo*  l_GroupQueueInfo = nullptr;

    std::list<Player*> l_Players;

    if (l_JoinAsGroup)
    {
        l_ResultError = l_Group->CanJoinBattlegroundQueue(l_Battleground, l_BGQueueTypeID, l_ArenaType);

        if ((!l_ResultError || (l_ResultError && sBattlegroundMgr->isArenaTesting())) && !l_InterRealmEnable)
        {
            l_GroupQueueInfo = l_Scheduler.AddGroup(m_Player, l_Group, l_BGQueueTypeID, nullptr, l_BracketEntry, l_ArenaType, false, 0, 0, true);
            l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);
        }

        for (GroupReference * l_It = l_Group->GetFirstMember(); l_It != nullptr; l_It = l_It->next())
        {
            Player*  l_Member = l_It->getSource();
            if (!l_Member)
                continue;

            if (l_ResultError && !sBattlegroundMgr->isArenaTesting())
            {
                WorldPacket l_Data;
                MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, m_Player, 0, l_ResultError);
                l_Member->GetSession()->SendPacket(&l_Data);
                continue;
            }

            if (l_InterRealmEnable)
            {
                l_Players.push_back(l_Member);
                continue;
            }

            if (!l_GroupQueueInfo)
                return;

            /// Add to queue
            uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

            /// Add joined time data
            l_Member->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

            WorldPacket l_Data; // send status packet (in queue)
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_Battleground, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->m_JoinTime, l_ArenaType, true);
            l_Member->GetSession()->SendPacket(&l_Data);
        }
    }
    else
    {
        // check Deserter debuff
        if (!m_Player->CanJoinToBattleground())
        {
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, m_Player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            m_Player->GetSession()->SendPacket(&l_Data);
            return;
        }

        if (m_Player->InArenaQueue())
        {
            /// Player is already in an arena queue.
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, m_Player, 0, ERR_BATTLEGROUND_JOIN_FAILED);
            m_Player->GetSession()->SendPacket(&l_Data);
            return;
        }

        if (m_Player->GetBattlegroundQueueIndex(l_BGQueueTypeID) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            //player is already in random queue
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, m_Player, 0, ERR_IN_RANDOM_BG);
            m_Player->GetSession()->SendPacket(&l_Data);
            return;
        }

        // check if has free queue slots
        if (!m_Player->HasFreeBattlegroundQueueId())
        {
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, m_Player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            m_Player->GetSession()->SendPacket(&l_Data);
            return;
        }

        if (!l_InterRealmEnable)
        {
            m_Player->SetBattleGroundRoles(l_Roles);

            l_GroupQueueInfo = l_Scheduler.AddGroup(m_Player, nullptr, l_BGQueueTypeID, nullptr, l_BracketEntry, l_ArenaType, false, 0, 0, true);
            uint32 l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);
            uint32 l_QueueSlot = m_Player->AddBattlegroundQueueId(l_BGQueueTypeID);

            // add joined time data
            m_Player->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

            WorldPacket l_Data; // send status packet (in queue)
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_Battleground, m_Player, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->m_JoinTime, l_GroupQueueInfo->m_ArenaType, true);
            SendPacket(&l_Data);
        }
        else
            l_Players.push_back(m_Player);
    }

    if (l_InterRealmEnable)
        l_Tunnel->SendRegisterArena(l_Players, l_ArenaType, true);
}

void WorldSession::HandleBattlemasterJoinRated(WorldPacket & /*p_Packet*/)
{
    bool l_InterRealmEnable = sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE);
    InterRealmSession* l_Tunnel = nullptr;

    if (l_InterRealmEnable)
    {
        l_Tunnel = sWorld->GetInterRealmSession();
        if (!l_Tunnel || !l_Tunnel->IsTunnelOpened())
        {
            ChatHandler(this).PSendSysMessage(LANG_INTERREALM_DISABLED);
            return;
        }

        if (GetInterRealmBG())
            return;
    }

    // ignore if we already in BG or BG queue
    if (m_Player->InBattleground())
        return;

    uint32 l_PersonalRating = 0;
    uint32 l_MatchmakerRating = 0;

    //check existance
    Battleground* l_Battleground = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::RatedBg10v10);
    if (!l_Battleground)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (10 vs 10) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_RATED_10_VS_10, NULL))
        return;

    BattlegroundTypeId l_BgTypeId = l_Battleground->GetTypeID();
    MS::Battlegrounds::BattlegroundType::Type l_BgQueueTypeId = MS::Battlegrounds::GetTypeFromId(l_BgTypeId, 0);

    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(m_Player->getLevel());
    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_Error = ERR_BATTLEGROUND_NONE;

    Group* l_Group = m_Player->GetGroup();
    if (!l_Group)
        return;

    if (l_Group->GetLeaderGUID() != m_Player->GetGUID())
        return;

    uint32 l_PlayerDivider = 0;
    for (GroupReference const* l_GroupReference = l_Group->GetFirstMember(); l_GroupReference != nullptr; l_GroupReference = l_GroupReference->next())
    {
        if (Player const* groupMember = l_GroupReference->getSource())
        {
            l_PersonalRating   += groupMember->GetArenaPersonalRating(SLOT_RBG);
            l_MatchmakerRating += groupMember->GetArenaMatchMakerRating(SLOT_RBG);

            ++l_PlayerDivider;
        }
    }

    if (!l_PlayerDivider)
        return;

    l_PersonalRating /= l_PlayerDivider;
    l_MatchmakerRating /= l_PlayerDivider;

    if (l_PersonalRating <= 0)
        l_PersonalRating = 0;
    if (l_MatchmakerRating <= 0)
        l_MatchmakerRating = 1;

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    uint32 l_AvgTime = 0;
    GroupQueueInfo* l_GroupQueue;

    l_Error = l_Group->CanJoinBattlegroundQueue(l_Battleground, l_BgQueueTypeId, 10);
    if (!l_Error)
    {
        if (!l_InterRealmEnable)
        {
            l_GroupQueue = l_Scheduler.AddGroup(m_Player, l_Group, l_BgQueueTypeId, nullptr, l_BracketEntry, ArenaType::None, true, l_PersonalRating, l_MatchmakerRating, false);
            l_AvgTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueue, l_BracketEntry->m_Id);
        }
    }

    for (GroupReference* l_Iterator = l_Group->GetFirstMember(); l_Iterator != NULL; l_Iterator = l_Iterator->next())
    {
        Player* l_Member = l_Iterator->getSource();
        if (l_Member == nullptr)
            continue;

        if (l_Error)
        {
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, m_Player, 0, l_Error);
            l_Member->GetSession()->SendPacket(&l_Data);
            continue;
        }

        if (l_InterRealmEnable)
            continue;

         // add to queue
        uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BgQueueTypeId);

        // add joined time data
        l_Member->AddBattlegroundQueueJoinTime(l_BgQueueTypeId, l_GroupQueue->m_JoinTime);

        WorldPacket l_Data; // send status packet (in queue)
        MS::Battlegrounds::PacketFactory::Status(&l_Data, l_Battleground, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AvgTime, l_GroupQueue->m_JoinTime, l_GroupQueue->m_ArenaType, false);
        l_Member->GetSession()->SendPacket(&l_Data);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for rated battleground as group bg queue type %u bg type %u: GUID %u, NAME %s", l_BgQueueTypeId, l_BgTypeId, l_Member->GetGUIDLow(), l_Member->GetName());
    }

    if (l_Error || !l_InterRealmEnable)
        return;

    l_Tunnel->SendRegisterRated(l_Group, l_PersonalRating, l_MatchmakerRating);
}

void WorldSession::HandleBattleFieldRequestScoreData(WorldPacket & /*p_Packet*/) ///< p_Packet is unused
{
    if (!m_Player || !m_Player->GetBattleground())
        return;

    WorldPacket l_ScoreData;
    MS::Battlegrounds::PacketFactory::PvpLogData(&l_ScoreData, m_Player->GetBattleground());

    m_Player->SendDirectMessage(&l_ScoreData);
}

/// @TODO: make wargame cross server (gl ...)
void WorldSession::HandleStartWarGame(WorldPacket& p_Packet)
{
    uint64 l_OpposingPartyMemberGUID;
    uint32 l_RealmID;
    uint16 l_Unk16;
    uint64 l_QueueID;
    bool   l_TournamentRules;

    p_Packet.readPackGUID(l_OpposingPartyMemberGUID);
    p_Packet >> l_RealmID;
    p_Packet >> l_Unk16;
    p_Packet >> l_QueueID;
    l_TournamentRules = p_Packet.ReadBit();

    if (m_Player->GetGroup() == nullptr)
        return;

    if (m_Player->GetGroup()->GetLeaderGUID() != m_Player->GetGUID())
        return;

    /// Try to find the opposing party leader
    Player* l_OpposingPartyLeader = sObjectAccessor->FindPlayer(l_OpposingPartyMemberGUID);
    if (l_OpposingPartyLeader == nullptr)
        return;

    if (l_OpposingPartyLeader->GetGroup() == nullptr)
        return;

    if (l_OpposingPartyLeader->GetGroup()->GetLeaderGUID() != l_OpposingPartyLeader->GetGUID())
        return;

    /// Already one invite requested
    if (m_Player->HasWargameRequest())
        return;

    /// The request is delete after 60 secs (handle in Player::Update)
    WargameRequest* l_Request = new WargameRequest();
    l_Request->OpposingPartyMemberGUID = l_OpposingPartyMemberGUID;
    l_Request->TournamentRules         = l_TournamentRules;
    l_Request->CreationDate            = time(nullptr);
    l_Request->QueueID                 = l_QueueID;

    m_Player->SetWargameRequest(l_Request);

    /// Send invitation to opposing party
    MS::Battlegrounds::PacketFactory::CheckWargameEntry(m_Player, l_OpposingPartyLeader, l_QueueID, l_TournamentRules);
}

void WorldSession::HandleAcceptWarGameInvite(WorldPacket& p_Packet)
{
    uint64 l_OpposingPartyMember;
    uint64 l_QueueID;
    bool   l_Accept;

    p_Packet.readPackGUID(l_OpposingPartyMember);
    p_Packet >> l_QueueID;
    l_Accept = p_Packet.ReadBit();

    Group* l_Group = m_Player->GetGroup();
    if (l_Group == nullptr
        || !l_Group->IsLeader(m_Player->GetGUID()))
        return;

    Player* l_OpposingPartyLeader = sObjectAccessor->FindPlayer(l_OpposingPartyMember);
    if (l_OpposingPartyLeader == nullptr)
        return;

    Group* l_OpposingGroup = l_OpposingPartyLeader->GetGroup();
    if (l_OpposingGroup == nullptr
        || !l_OpposingGroup->IsLeader(l_OpposingPartyLeader->GetGUID()))
        return;

    /// Check if the invitation realy exist, and isn't expired
    if (!l_OpposingPartyLeader->HasWargameRequest())
        return;

    WargameRequest* l_Request = l_OpposingPartyLeader->GetWargameRequest();
    if (l_Request->QueueID != l_QueueID
        || l_Request->OpposingPartyMemberGUID != m_Player->GetGUID())
        return;

    if (!l_Accept)
    {
        ///@TODO: Send notification to opposing party ?
        return;
    }

    uint32 l_BattlegroundTypeId = l_QueueID & 0xFFFF;

    BattlegroundTypeId l_BGTypeID = BattlegroundTypeId(l_BattlegroundTypeId);
    auto l_BGQueueTypeID          = MS::Battlegrounds::GetSchedulerType((BattlegroundTypeId)l_BattlegroundTypeId);
    ArenaType l_ArenaType         = ArenaType::None;

    if (MS::Battlegrounds::IsArenaType(l_BGQueueTypeID))
    {
        if (l_Group->GetMembersCount() != l_OpposingGroup->GetMembersCount())
            return;

        switch (l_Group->GetMembersCount())
        {
            case 2:
                l_ArenaType = ArenaType::Arena2v2;
                break;
            case 3:
                l_ArenaType = ArenaType::Arena3v3;
                break;
            case 5:
                l_ArenaType = ArenaType::Arena5v5;
                break;
        }
    }

    // ignore if player is already in BG
    if (m_Player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* l_BattlegroundTemplate = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::GetSchedulerType(l_BGTypeID));
    if (l_BattlegroundTemplate == nullptr)
        return;

    if (l_Group->GetMembersCount() < l_BattlegroundTemplate->GetMinPlayersPerTeam()
        || l_OpposingGroup->GetMembersCount() < l_BattlegroundTemplate->GetMinPlayersPerTeam())
        return;

    auto& l_Scheduler = sBattlegroundMgr->GetScheduler();
    auto& l_InviationMgr = sBattlegroundMgr->GetInvitationsMgr();

    auto l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(m_Player->getLevel());
    if (!l_BracketEntry)
        return;

    /// Create the new battleground.
    Battleground* l_BattlegroundInstance = sBattlegroundMgr->CreateNewBattleground(l_BGQueueTypeID, MS::Battlegrounds::Brackets::RetreiveFromId(l_BracketEntry->m_Id), l_ArenaType, false, true, l_Request->TournamentRules);
    if (l_BattlegroundInstance == nullptr)
        return;

    auto l_PrepareGroupToWargame = [this, &l_Scheduler, l_InviationMgr, l_BGQueueTypeID, l_ArenaType, l_BattlegroundTemplate, l_BattlegroundInstance, l_BracketEntry](Group* p_Group, uint32 p_Team) -> void
    {
        uint32 l_BlacklistMap[2] = { 0, 0 };

        Player* l_Leader = sObjectAccessor->FindPlayer(p_Group->GetLeaderGUID());

        auto l_GroupQueueInfo = l_Scheduler.AddGroup(l_Leader, p_Group, l_BGQueueTypeID, l_BlacklistMap, l_BracketEntry, l_ArenaType, false, 0, 0, false);
        uint32 l_AvgTime      = l_InviationMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);

        for (GroupReference * l_It = p_Group->GetFirstMember(); l_It != NULL; l_It = l_It->next())
        {
            Player* l_Member = l_It->getSource();

            if (!l_Member)
                continue;   // this should never happen

            // add to queue
            uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

            // add joined time data
            l_Member->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

            WorldPacket l_Data; // send status packet (in queue)
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BattlegroundInstance, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AvgTime, l_GroupQueueInfo->m_JoinTime, l_GroupQueueInfo->m_ArenaType, false);
            l_Member->GetSession()->SendPacket(&l_Data);
        }

        l_Scheduler.RemoveGroupFromQueues(l_GroupQueueInfo);
        l_Scheduler.AddToBG(l_GroupQueueInfo, l_BattlegroundInstance, p_Team);
    };

    /// Add groups to the battleground and remove them from waiting groups list.
    l_PrepareGroupToWargame(l_Group, HORDE);
    l_PrepareGroupToWargame(l_OpposingGroup, ALLIANCE);

    l_BattlegroundInstance->StartBattleground();
}
