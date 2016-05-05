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

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "OutdoorPvPMgr.h"
#include "Opcodes.h"

//This send to player windows for invite player to join the war
//Param1:(m_Guid) the BattleId of Bf
//Param2:(ZoneId) the zone where the battle is (4197 for wg)
//Param3:(time) Time in second that the player have for accept
void WorldSession::SendBfInvitePlayerToWar(uint64 guid, uint32 zoneId, uint32 pTime)
{
    ///< Send packet
    WorldPacket data(SMSG_BFMGR_ENTRY_INVITE);
    data << uint64(guid);               ///< QueueID
    data << uint32(zoneId);             ///< Zone Id
    data << uint32(pTime);              ///< Invite lasts until

    ///< Sending the packet to player
    SendPacket(&data);
}

//This send invitation to player to join the queue
//Param1:(guid) the guid of Bf
void WorldSession::SendBfInvitePlayerToQueue(uint64 p_Guid)
{
    ///< @todo check me, nothing in dump/Tc have this struc, check SMSG_BFMGR_QUEUE_INVITE
    //uint64 QueueID;            ///< Offset 0x0000 Type _MJVTBL_UINT64
    //uint32 InstanceID;         ///< Offset 0x0008 Type _MJVTBL_UINT32
    //uint32 Timeout;            ///< Offset 0x000C Type _MJVTBL_UINT32
    //int32  MapID;              ///< Offset 0x0010 Type _MJVTBL_INT32
    //int32  MaxLevel;           ///< Offset 0x0014 Type _MJVTBL_INT32
    //int32  MinLevel;           ///< Offset 0x0018 Type _MJVTBL_INT32
    //int8   BattleState;        ///< Offset 0x001C Type _MJVTBL_INT8
    //int8   Index;              ///< Offset 0x001D Type _MJVTBL_INT8
    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_INVITE);
    data << uint64(p_Guid);         ///< QueueID
    data << uint8(1);               ///< BattleState
    data << int32(0);
    data << int32(0);
    data << int32(0);
    data << uint32(0);
    data << uint32(0);

    data.WriteBit(0);
    data.FlushBits();

    ///< Sending packet to player
    SendPacket(&data);
}

//This send packet for inform player that he join queue
//Param1:(guid) the guid of Bf
//Param2:(ZoneId) the zone where the battle is (4197 for wg)
//Param3:(CanQueue) if able to queue
//Param4:(Full) on log in is full
void WorldSession::SendBfQueueInviteResponse(uint64 p_Guid, uint32 p_ZoneID, bool p_CanQueue, bool p_Full)
{
    WorldPacket l_Data(SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE);
    l_Data << uint64(p_Guid);               ///< QueueID
    l_Data << uint32(p_ZoneID);             ///< AreaID
    l_Data << uint8((p_CanQueue ? 1 : 0));  ///< Result (0 : you cannot queue wg, 1 : you are queued)
    l_Data << uint8(1);                     ///< BattleState
    l_Data.appendPackGUID(0);               ///< FailedPlayerGUID

    l_Data.WriteBit((p_Full ? 0 : 1));      ///< LoggingIn(0 : wg full, 1 : queue for upcoming (we may need to swap it))
    l_Data.FlushBits();

    SendPacket(&l_Data);
}

//This is call when player accept to join war
//Param1:(guid) the guid of Bf
void WorldSession::SendBfEntered(uint64 p_Guid)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_ENTERING);
    data.WriteBit(m_Player->isAFK() ? 1 : 0);   ///< Cleared AFK
    data.WriteBit(true);                        ///< On Offense
    data.WriteBit(true);                        ///< Relocated
    data.FlushBits();
    data << uint64(p_Guid);

    SendPacket(&data);
}

void WorldSession::SendBfLeaveMessage(uint64 p_Guid, BFLeaveReason p_Reason)
{
    WorldPacket l_Data(SMSG_BATTLEFIELD_MGR_EJECTED);
    l_Data << uint64(p_Guid);   ///< Queue ID
    l_Data << uint8(p_Reason);  ///< Reason
    l_Data << uint8(2);         ///< BattleStatus

    l_Data.WriteBit(false);     ///< Relocated
    l_Data.FlushBits();

    SendPacket(&l_Data);
}

/// Send by client when he click on accept for queue
void WorldSession::HandleBfQueueInviteResponse(WorldPacket & p_Packet)
{
    uint64 l_QueueID        = 0;
    bool   l_AcceptedInvite = false;

    p_Packet >> l_QueueID;
    l_AcceptedInvite = p_Packet.ReadBit();

    sLog->outTrace(LOG_FILTER_GENERAL, "HandleQueueInviteResponse: GUID:" UI64FMTD " Accepted:%u", (uint64)l_QueueID, l_AcceptedInvite);

    if (!l_AcceptedInvite)
        return;

    Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID);

    if (!l_Battlefield)
        return;

    l_Battlefield->PlayerAcceptInviteToQueue(m_Player);
}

/// Send by client on clicking in accept or refuse of invitation windows for join game
void WorldSession::HandleBfEntryInviteResponse(WorldPacket & p_Packet)
{
    uint64 l_QueueID = 0;
    bool l_AcceptedInvite = false;

    p_Packet >> l_QueueID;
    l_AcceptedInvite = p_Packet.ReadBit();

    sLog->outTrace(LOG_FILTER_GENERAL, "HandleBattlefieldInviteResponse: GUID:" UI64FMTD " Accepted:%u", uint64(l_QueueID), l_AcceptedInvite);

    OutdoorPvP* l_OutdoorPVP = nullptr;
    uint32 l_GuidLow = GUID_LOPART(l_QueueID);
    if (l_GuidLow & 0x20000)
    {
        // BATTLEFIELD_TYPE_WORLD_PVP
        l_GuidLow &= ~0x20000;

        if (World_PVP_AreaEntry const* l_PvPArea = sWorld_PVP_AreaStore.LookupEntry(l_GuidLow))
            l_OutdoorPVP = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(l_PvPArea->AreaID);
    }

    Battlefield* l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID);
    if (!l_Battlefield && !l_OutdoorPVP)
        return;

    if (l_Battlefield)
    {
        if (l_AcceptedInvite)
            l_Battlefield->PlayerAcceptInviteToWar(m_Player);
        else
        {
            if (m_Player->GetZoneId() == l_Battlefield->GetZoneId())
                l_Battlefield->KickPlayerFromBattlefield(m_Player->GetGUID());
        }
    }
    else if (l_OutdoorPVP)
        l_OutdoorPVP->HandleBFMGREntryInviteResponse(l_AcceptedInvite, m_Player);
}

void WorldSession::HandleBfQueueRequest(WorldPacket& p_Packet)
{
    uint64 l_QueueID = 0;

    p_Packet >> l_QueueID;

    sLog->outError(LOG_FILTER_GENERAL, "HandleBfQueueRequest: GUID:" UI64FMTD " ", (uint64)l_QueueID);

    if (Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID))
    {
        if (l_Battlefield->IsWarTime())
            l_Battlefield->InvitePlayerToWar(m_Player);
        else
        {
            uint32 l_Timer = l_Battlefield->GetTimer() / 1000;

            if (l_Timer < 15 * MINUTE)
                l_Battlefield->InvitePlayerToQueue(m_Player);
        }
    }
}

void WorldSession::HandleBfExitQueueRequest(WorldPacket & p_Packet)
{
    uint64 l_QueueID = 0;

    p_Packet >> l_QueueID;

    sLog->outTrace(LOG_FILTER_GENERAL, "HandleBfExitQueueRequest: GUID:" UI64FMTD " ", (uint64)l_QueueID);

    SendBfLeaveMessage(l_QueueID);

    if (Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID))
        l_Battlefield->AskToLeaveQueue(m_Player);
}

void WorldSession::HandleBfExitRequest(WorldPacket& /*recv_data*/)
{
    sLog->outError(LOG_FILTER_GENERAL, "HandleBfExitRequest");
    Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(m_Player->GetZoneId());
    if (bf)
         bf->KickPlayerFromBattlefield(m_Player->GetGUID());
}

void WorldSession::HandleReportPvPAFK(WorldPacket& recvData)
{
    uint64 l_PlayerGUID = 0;
    recvData.readPackGUID(l_PlayerGUID);

    Player* l_ReportedPlayer = ObjectAccessor::FindPlayer(l_PlayerGUID);
    if (!l_ReportedPlayer)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "WorldSession::HandleReportPvPAFK: player not found");
        return;
    }

    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "WorldSession::HandleReportPvPAFK: %s reported %s", m_Player->GetName(), l_ReportedPlayer->GetName());

    l_ReportedPlayer->ReportedAfkBy(m_Player);
}

void WorldSession::HandleRequestPvpOptions(WorldPacket& /*recvData*/)
{
    /// @Todo: perfome research in this case
    WorldPacket data(SMSG_PVP_OPTIONS_ENABLED, 1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandleRequestPvpReward(WorldPacket& /*recvData*/)
{
    m_Player->SendPvpRewards();
}

void WorldSession::HandleRequestRatedBgStats(WorldPacket& /*recvData*/)
{
    WorldPacket l_Data(SMSG_RATED_BATTLEFIELD_INFO, MAX_PVP_SLOT * (4 + 4 + 4 + 4 + 4 + 4 + 4 + 4));

    for (int i = 0; i < MAX_PVP_SLOT; i++)
    {
        l_Data << uint32(m_Player->GetArenaPersonalRating(i));    ///< current rating
        l_Data << uint32(0);                                      ///< Ranking
        l_Data << uint32(m_Player->GetSeasonGames(i));            ///< games of season
        l_Data << uint32(m_Player->GetSeasonWins(i));             ///< games of season
        l_Data << uint32(m_Player->GetWeekGames(i));              ///< games of week
        l_Data << uint32(m_Player->GetWeekWins(i));               ///< won games of week
        l_Data << uint32(m_Player->GetBestRatingOfWeek(i));       ///< best rating of week
        l_Data << uint32(m_Player->GetBestRatingOfSeason(i));     ///< best rating of season
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleRequestConquestFormulaConstants(WorldPacket& /*p_RecvData*/)
{
    WorldPacket l_Data(SMSG_CONQUEST_FORMULA_CONSTANTS, 5 * 4);
    l_Data << uint32(Arena::g_PvpMinCPPerWeek);
    l_Data << uint32(Arena::g_PvpMaxCPPerWeek);
    l_Data << float(Arena::g_PvpCPNumerator);
    l_Data << float(Arena::g_PvpCPBaseCoefficient);
    l_Data << float(Arena::g_PvpCPExpCoefficient);
    SendPacket(&l_Data);
}
