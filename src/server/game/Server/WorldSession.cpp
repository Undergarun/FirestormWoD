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

/** \file
    \ingroup u2w
*/

#include "WorldSocket.h"                                    // must be first to make ACE happy with ACE includes in it
#include <zlib.h>
#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "Group.h"
#include "Guild.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "BattlegroundMgr.hpp"
#include "OutdoorPvPMgr.h"
#include "MapManager.h"
#include "SocialMgr.h"
#include "zlib.h"
#include "ScriptMgr.h"
#include "Transport.h"
#include "WardenWin.h"
#include "WardenMac.h"
#include "GarrisonMgr.hpp"
#include "AccountMgr.h"
#include "PetBattle.h"

bool MapSessionFilter::Process(WorldPacket* packet)
{
    Opcodes opcode = DropHighBytes(packet->GetOpcode());
    OpcodeHandler const* opHandle = g_OpcodeTable[WOW_CLIENT_TO_SERVER][opcode];

    //let's check if our opcode can be really processed in Map::Update()
    if (opHandle->packetProcessing == PROCESS_INPLACE)
        return true;

    //we do not process thread-unsafe packets
    if (opHandle->packetProcessing == PROCESS_THREADUNSAFE)
        return false;

    Player* player = m_pSession->GetPlayer();
    if (!player)
        return false;

    //in Map::Update() we do not process packets where player is not in world!
    return player->IsInWorld();
}

//we should process ALL packets when player is not in world/logged in
//OR packet handler is not thread-safe!
bool WorldSessionFilter::Process(WorldPacket* packet)
{
    Opcodes opcode = DropHighBytes(packet->GetOpcode());
    OpcodeHandler const* opHandle = g_OpcodeTable[WOW_CLIENT_TO_SERVER][opcode];
    //check if packet handler is supposed to be safe
    if (opHandle->packetProcessing == PROCESS_INPLACE)
        return true;

    //thread-unsafe packets should be processed in World::UpdateSessions()
    if (opHandle->packetProcessing == PROCESS_THREADUNSAFE)
        return true;

    //no player attached? -> our client! ^^
    Player* player = m_pSession->GetPlayer();
    if (!player)
        return true;

    //lets process all packets for non-in-the-world player
    return (player->IsInWorld() == false);
}

/// WorldSession constructor
WorldSession::WorldSession(uint32 id, WorldSocket* sock, AccountTypes sec, bool ispremium, uint8 premiumType, uint8 expansion, time_t mute_time, LocaleConstant locale, uint32 recruiter, bool isARecruiter, uint32 p_VoteRemainingTime, uint32 p_ServiceFlags, uint32 p_CustomFlags)
{
    ///////////////////////////////////////////////////////////////////////////////
    /// Members initialization
    ///////////////////////////////////////////////////////////////////////////////
    m_Player                = nullptr;
    m_VoteSyncTimer         = VOTE_SYNC_TIMER;
    m_muteTime              = mute_time;
    m_Socket                = sock;
    _security               = sec;
    _accountId              = id;
    m_expansion             = expansion;
    _ispremium              = ispremium;
    m_PremiumType           = premiumType;
    m_VoteRemainingTime     = p_VoteRemainingTime;
    m_sessionDbLocaleIndex  = locale;
    recruiterId             = recruiter;
    isRecruiter             = isARecruiter;
    m_ServiceFlags          = p_ServiceFlags;
    m_CustomFlags           = p_CustomFlags;
    m_sessionDbcLocale      = sWorld->GetAvailableDbcLocale(locale);

    m_timeOutTime                       = 0;
    timeCharEnumOpcode                  = 0;
    m_TimeLastChannelInviteCommand      = 0;
    m_TimeLastChannelPassCommand        = 0;
    m_TimeLastChannelMuteCommand        = 0;
    m_TimeLastChannelBanCommand         = 0;
    m_TimeLastChannelUnbanCommand       = 0;
    m_TimeLastChannelAnnounceCommand    = 0;
    m_TimeLastGroupInviteCommand        = 0;
    m_TimeLastChannelModerCommand       = 0;
    m_TimeLastChannelOwnerCommand       = 0;
    m_TimeLastChannelUnmoderCommand     = 0;
    timeLastArenaTeamCommand            = 0;
    m_TimeLastChannelSetownerCommand    = 0;
    m_TimeLastChannelUnmuteCommand      = 0;
    m_TimeLastChannelKickCommand        = 0;
    timeLastServerCommand               = 0;
    timeLastChangeSubGroupCommand       = 0;
    m_TimeLastSellItemOpcode            = 0;
    m_uiAntispamMailSentCount           = 0;
    m_PlayerLoginCounter                = 0;
    m_clientTimeDelay                   = 0;
    m_TimeLastUseItem                   = 0;
    m_TimeLastTicketOnlineList          = 0;
    m_AccountJoinDate                   = 0;
    timeLastWhoCommand                  = 0;
    _logoutTime                         = 0;
    m_latency                           = 0;
    m_VoteTimePassed                    = 0;

    m_IsStressTestSession   = false;
    m_playerRecentlyLogout  = false;
    m_playerSave            = false;
    m_TutorialsChanged      = false;
    m_playerLoading         = false;
    m_playerLogout          = false;
    m_inQueue               = false;
    m_IsPetBattleJournalLocked = false;
    ///////////////////////////////////////////////////////////////////////////////

    _warden = NULL;
    _filterAddonMessages = false;
    m_LoginTime = time(nullptr);

    if (sock)
    {
        m_Address = sock->GetRemoteAddress();
        sock->AddReference();
        ResetTimeOutTime();
        LoginDatabase.PExecute("UPDATE account SET online = 1 WHERE id = %u;", GetAccountId());     // One-time query
    }

    InitializeQueryCallbackParameters();

    m_TransactionCallbacks             = std::unique_ptr<TransactionCallbacks>(new TransactionCallbacks());
    m_PreparedStatementCallbacks       = std::unique_ptr<PreparedStatementCallbacks>(new PreparedStatementCallbacks());
    m_PreparedStatementCallbacksBuffer = std::unique_ptr<PreparedStatementCallbacks>(new PreparedStatementCallbacks());

    _compressionStream = new z_stream();
    _compressionStream->zalloc = (alloc_func)NULL;
    _compressionStream->zfree = (free_func)NULL;
    _compressionStream->opaque = (voidpf)NULL;
    _compressionStream->avail_in = 0;
    _compressionStream->next_in = NULL;
    int32 z_res = deflateInit(_compressionStream, sWorld->getIntConfig(CONFIG_COMPRESSION));
    if (z_res != Z_OK)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Can't initialize packet compression (zlib: deflateInit) Error code: %i (%s)", z_res, zError(z_res));
        return;
    }
}

/// WorldSession destructor
WorldSession::~WorldSession()
{
    ///- unload player if not unloaded
    if (m_Player)
        LogoutPlayer (true);

    /// - If have unclosed socket, close it
    if (m_Socket)
    {
        m_Socket->CloseSocket();
        m_Socket->RemoveReference();
        m_Socket = NULL;
    }

    if (_warden)
        delete _warden;

    ///- empty incoming packet queue
    WorldPacket* packet = NULL;
    while (_recvQueue.next(packet))
        delete packet;

    if (m_VoteTimePassed)
        LoginDatabase.PExecute("UPDATE account_vote SET remainingTime = remainingTime - %u WHERE account = %u", m_VoteTimePassed, GetAccountId());

    LoginDatabase.PExecute("UPDATE account SET online = 0 WHERE id = %u;", GetAccountId());     // One-time query

    int32 z_res = deflateEnd(_compressionStream);
    if (z_res != Z_OK && z_res != Z_DATA_ERROR) // Z_DATA_ERROR signals that internal state was BUSY
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Can't close packet compression stream (zlib: deflateEnd) Error code: %i (%s)", z_res, zError(z_res));
        return;
    }

    delete _compressionStream;
}

/// Get the player name
std::string WorldSession::GetPlayerName(bool simple /* = true */) const
 {
    std::string name = "[Player: ";
    uint32 guidLow = 0;

    if (Player* player = GetPlayer())
    {
        name.append(player->GetName());
        guidLow = player->GetGUIDLow();
    }
    else
        name.append("<none>");

    if (!simple)
    {
        std::ostringstream ss;
        ss << " (Guid: " << guidLow << ", Account: " << GetAccountId() << ")";
        name.append(ss.str());
    }

    name.append("]");
    return name;
}

/// Get player guid if available. Use for logging purposes only
uint32 WorldSession::GetGuidLow() const
{
    return GetPlayer() ? GetPlayer()->GetGUIDLow() : 0;
}

/// Send a packet to the client
void WorldSession::SendPacket(WorldPacket const* packet, bool forced /*= false*/)
{
    if (!m_Socket)
        return;

    const_cast<WorldPacket*>(packet)->OnSend();

    if (packet->GetOpcode() == NULL_OPCODE && !forced)
    {
        sLog->outError(LOG_FILTER_OPCODES, "Prevented sending of NULL_OPCODE to %s", GetPlayerName(false).c_str());
        return;
    }
    else if (packet->GetOpcode() == UNKNOWN_OPCODE && !forced)
    {
        sLog->outError(LOG_FILTER_OPCODES, "Prevented sending of UNKNOWN_OPCODE to %s", GetPlayerName(false).c_str());
        return;
    }

    if (!forced)
    {
        OpcodeHandler* handler = g_OpcodeTable[WOW_SERVER_TO_CLIENT][packet->GetOpcode()];
        if (!handler || handler->status == STATUS_UNHANDLED)
        {
            sLog->outError(LOG_FILTER_OPCODES, "Prevented sending disabled opcode %s to %s", GetOpcodeNameForLogging(packet->GetOpcode(), WOW_SERVER_TO_CLIENT).c_str(), GetPlayerName(false).c_str());
            return;
        }
    }

#ifdef TRINITY_DEBUG
    // Code for network use statistic
    static uint64 sendPacketCount = 0;
    static uint64 sendPacketBytes = 0;

    static time_t firstTime = time(NULL);
    static time_t lastTime = firstTime;                     // next 60 secs start time

    static uint64 sendLastPacketCount = 0;
    static uint64 sendLastPacketBytes = 0;

    time_t cur_time = time(NULL);

    if ((cur_time - lastTime) < 60)
    {
        sendPacketCount+=1;
        sendPacketBytes+=packet->size();

        sendLastPacketCount+=1;
        sendLastPacketBytes+=packet->size();
    }
    else
    {
        uint64 minTime = uint64(cur_time - lastTime);
        uint64 fullTime = uint64(lastTime - firstTime);
        sLog->outInfo(LOG_FILTER_GENERAL, "Send all time packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f time: %u", sendPacketCount, sendPacketBytes, float(sendPacketCount)/fullTime, float(sendPacketBytes)/fullTime, uint32(fullTime));
        sLog->outInfo(LOG_FILTER_GENERAL, "Send last min packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f", sendLastPacketCount, sendLastPacketBytes, float(sendLastPacketCount)/minTime, float(sendLastPacketBytes)/minTime);

        lastTime = cur_time;
        sendLastPacketCount = 1;
        sendLastPacketBytes = packet->wpos();               // wpos is real written size
    }
#endif                                                      // !TRINITY_DEBUG

    if (m_Socket->SendPacket(*packet) == -1)
        m_Socket->CloseSocket();
}

/// Add an incoming packet to the queue
void WorldSession::QueuePacket(WorldPacket* new_packet)
{
    _recvQueue.add(new_packet);
}

/// Logging helper for unexpected opcodes
void WorldSession::LogUnexpectedOpcode(WorldPacket* packet, const char* status, const char *reason)
{
    sLog->outError(LOG_FILTER_OPCODES, "Received unexpected opcode %s Status: %s Reason: %s from %s",
        GetOpcodeNameForLogging(packet->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), status, reason, GetPlayerName(false).c_str());
}

/// Logging helper for unexpected opcodes
void WorldSession::LogUnprocessedTail(WorldPacket* packet)
{
    sLog->outError(LOG_FILTER_OPCODES, "Unprocessed tail data (read stop at %u from %u) Opcode %s from %s",
        uint32(packet->rpos()), uint32(packet->wpos()), GetOpcodeNameForLogging(packet->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), GetPlayerName(false).c_str());
    packet->print_storage();
}

struct OpcodeInfo
{
    OpcodeInfo(uint32 nb, uint32 time) : nbPkt(nb), totalTime(time) {}
    uint32 nbPkt;
    uint32 totalTime;
};

/// Update the WorldSession (triggered by World update)
bool WorldSession::Update(uint32 diff, PacketFilter& updater)
{
    uint32 sessionDiff = getMSTime();
    uint32 nbPacket = 0;
    std::map<uint32, OpcodeInfo> pktHandle; // opcodeId / OpcodeInfo

    /// Antispam Timer update
    if (sWorld->getBoolConfig(CONFIG_ANTISPAM_ENABLED))
        UpdateAntispamTimer(diff);

    /// - Vote remaining time
    if (m_VoteRemainingTime != 0                //< Session have remaining time
        && m_Player != nullptr                  //< Player is online
        && m_VoteTimeCallback.GetStage() == 0)  //< We doesn't waiting for vote callback
    {
        m_VoteTimePassed += diff;
        if (m_VoteTimePassed >= m_VoteRemainingTime)
        {
            m_VoteTimePassed    = m_VoteRemainingTime;
            m_VoteRemainingTime = 0;

            std::ostringstream l_Query;
            l_Query << "UPDATE account_vote SET remainingTime = remainingTime - " << m_VoteTimePassed << " WHERE account = " << GetAccountId();

            // Callback to sync core / database remaining time
            m_VoteTimeCallback.NextStage();
            m_VoteTimeCallback.SetParam(false);     //< not sync callback
            m_VoteTimeCallback.SetFutureResult(LoginDatabase.AsyncQuery(l_Query.str().c_str()));
        }
    }

    /// - Vote sync
    if (m_VoteSyncTimer <= diff)
    {
        if (m_VoteTimeCallback.GetStage() == 0)
        {
            std::ostringstream l_Query;

            if (!m_VoteTimePassed)
            {
                l_Query << "SELECT remainingTime FROM account_vote WHERE account = " << GetAccountId();
                m_VoteTimeCallback.NextStage();     //< Switch to stage 2 directly
            }
            else
                l_Query << "UPDATE account_vote SET remainingTime = remainingTime - " << m_VoteTimePassed << " WHERE account = " << GetAccountId();

            // Callback to sync core with database
            m_VoteTimeCallback.NextStage();
            m_VoteTimeCallback.SetParam(true);  //< sync callback
            m_VoteTimeCallback.SetFutureResult(LoginDatabase.AsyncQuery(l_Query.str().c_str()));
        }
        else
            m_VoteSyncTimer = VOTE_SYNC_TIMER;
    }
    else
        m_VoteSyncTimer -= diff;

    /// Update Timeout timer.
    UpdateTimeOutTime(diff);

    ///- Before we process anything:
    /// If necessary, kick the player from the character select screen
    if (IsConnectionIdle() && m_Socket)
        m_Socket->CloseSocket();

    ///- Retrieve packets from the receive queue and call the appropriate handlers
    /// not process packets if socket already closed
    WorldPacket* packet = NULL;
    //! Delete packet after processing by default
    bool deletePacket = true;
    //! To prevent infinite loop
    WorldPacket* firstDelayedPacket = NULL;
    //! If _recvQueue.peek() == firstDelayedPacket it means that in this Update call, we've processed all
    //! *properly timed* packets, and we're now at the part of the queue where we find
    //! delayed packets that were re-enqueued due to improper timing. To prevent an infinite
    //! loop caused by re-enqueueing the same packets over and over again, we stop updating this session
    //! and continue updating others. The re-enqueued packets will be handled in the next Update call for this session.
    uint32 processedPackets = 0;
    while (m_Socket && !m_Socket->IsClosed() &&
            !_recvQueue.empty() && _recvQueue.peek(true) != firstDelayedPacket &&
            _recvQueue.next(packet, updater))
    {
        const OpcodeHandler* opHandle = g_OpcodeTable[WOW_CLIENT_TO_SERVER][packet->GetOpcode()];
        uint32 pktTime = getMSTime();

        try
        {
            switch (opHandle->status)
            {
                case STATUS_LOGGEDIN:
                    if (!m_Player)
                    {
                        // skip STATUS_LOGGEDIN opcode unexpected errors if player logout sometime ago - this can be network lag delayed packets
                        //! If player didn't log out a while ago, it means packets are being sent while the server does not recognize
                        //! the client to be in world yet. We will re-add the packets to the bottom of the queue and process them later.
                        if (!m_playerRecentlyLogout)
                        {
                            //! Prevent infinite loop
                            if (!firstDelayedPacket)
                                firstDelayedPacket = packet;
                            //! Because checking a bool is faster than reallocating memory
                            deletePacket = false;
                            QueuePacket(packet);
                            //! Log
                                sLog->outDebug(LOG_FILTER_NETWORKIO, "Re-enqueueing packet with opcode %s with with status STATUS_LOGGEDIN. "
                                    "Player is currently not in world yet.", GetOpcodeNameForLogging(packet->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str());
                        }
                    }
                    else if (m_Player->IsInWorld())
                    {
                        sScriptMgr->OnPacketReceive(m_Socket, WorldPacket(*packet), this);
                        (this->*opHandle->handler)(*packet);
                        if (sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                            LogUnprocessedTail(packet);
                    }
                    // lag can cause STATUS_LOGGEDIN opcodes to arrive after the player started a transfer
                    break;
                case STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT:
                    if (!m_Player && !m_playerRecentlyLogout && !m_playerLogout) // There's a short delay between _player = null and m_playerRecentlyLogout = true during logout
                        LogUnexpectedOpcode(packet, "STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT",
                            "the player has not logged in yet and not recently logout");
                    else
                    {
                        // not expected _player or must checked in packet hanlder
                        sScriptMgr->OnPacketReceive(m_Socket, WorldPacket(*packet), this);
                        (this->*opHandle->handler)(*packet);
                        if (sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                            LogUnprocessedTail(packet);
                    }
                    break;
                case STATUS_TRANSFER:
                    if (!m_Player)
                        LogUnexpectedOpcode(packet, "STATUS_TRANSFER", "the player has not logged in yet");
                    else if (m_Player->IsInWorld())
                        LogUnexpectedOpcode(packet, "STATUS_TRANSFER", "the player is still in world");
                    else
                    {
                        sScriptMgr->OnPacketReceive(m_Socket, WorldPacket(*packet), this);
                        (this->*opHandle->handler)(*packet);
                        if (sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                            LogUnprocessedTail(packet);
                    }
                    break;
                case STATUS_AUTHED:
                    // prevent cheating with skip queue wait
                    if (m_inQueue)
                    {
                        LogUnexpectedOpcode(packet, "STATUS_AUTHED", "the player not pass queue yet");
                        break;
                    }

                    // some auth opcodes can be recieved before STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT opcodes
                    // however when we recieve CMSG_CHAR_ENUM we are surely no longer during the logout process.
                    if (packet->GetOpcode() == CMSG_ENUM_CHARACTERS)
                        m_playerRecentlyLogout = false;

                    sScriptMgr->OnPacketReceive(m_Socket, WorldPacket(*packet), this);
                    (this->*opHandle->handler)(*packet);
                    if (sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                        LogUnprocessedTail(packet);
                    break;
                case STATUS_NEVER:
                        sLog->outError(LOG_FILTER_OPCODES, "Received not allowed opcode %s from %s", GetOpcodeNameForLogging(packet->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str()
                            , GetPlayerName(false).c_str());
                    break;
                case STATUS_UNHANDLED:
                        sLog->outError(LOG_FILTER_OPCODES, "Received not handled opcode %s from %s", GetOpcodeNameForLogging(packet->GetOpcode(), WOW_CLIENT_TO_SERVER).c_str()
                            , GetPlayerName(false).c_str());
                    break;
            }
        }
        catch(ByteBufferException &)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::Update ByteBufferException occured while parsing a packet (opcode: %u) from client %s, accountid=%i. Skipped packet.",
                    packet->GetOpcode(), GetRemoteAddress().c_str(), GetAccountId());
            packet->hexlike();
        }

        nbPacket++;

        std::map<uint32, OpcodeInfo>::iterator itr = pktHandle.find(packet->GetOpcode());
        if (itr == pktHandle.end())
            pktHandle.insert(std::make_pair(packet->GetOpcode(), OpcodeInfo(1, getMSTime() - pktTime)));
        else
        {
            OpcodeInfo& data = (*itr).second;
            data.nbPkt += 1;
            data.totalTime += getMSTime() - pktTime;
        }


        if (deletePacket)
            delete packet;

#define MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE 50
        processedPackets++;

        //process only a max amout of packets in 1 Update() call.
        //Any leftover will be processed in next update
        if (processedPackets > MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE)
            break;
    }

    if (m_Socket && !m_Socket->IsClosed() && _warden)
        _warden->Update();

    ProcessQueryCallbacks();

    //check if we are safe to proceed with logout
    //logout procedure should happen only in World::UpdateSessions() method!!!
    if (updater.ProcessLogout() && !m_IsStressTestSession)
    {
        time_t currTime = time(NULL);
        ///- If necessary, log the player out
        if (ShouldLogOut(currTime) && !m_playerLoading)
            LogoutPlayer(true);

        if (m_Socket && GetPlayer() && _warden)
            _warden->Update();

        ///- Cleanup socket pointer if need
        if (m_Socket && m_Socket->IsClosed())
        {
            m_Socket->RemoveReference();
            m_Socket = NULL;
        }

        if (!m_Socket)
            return false;                                       //Will remove this session from the world session map
    }

    sessionDiff = getMSTime() - sessionDiff;
    if (sessionDiff > 100)
    {
        std::map<uint32, OpcodeInfo>::iterator itr = pktHandle.find(CMSG_ADD_FRIEND);
        if (itr != pktHandle.end())
        {
            if ((*itr).second.nbPkt > 5)
            {
                sLog->outAshran("Account [%u] has been kicked for flood of CMSG_ADD_FRIEND (count : %u)", GetAccountId(), (*itr).second.nbPkt);
                KickPlayer();
                return false;
            }
        }

        sLog->outAshran("Session of account [%u] take more than 100 ms to execute (%u ms)", GetAccountId(), sessionDiff);
        for (auto itr : pktHandle)
            sLog->outAshran("-----> %u %s (%u ms)", itr.second.nbPkt, GetOpcodeNameForLogging((Opcodes)itr.first, WOW_CLIENT_TO_SERVER).c_str(), itr.second.totalTime);
    }

    return true;
}

/// %Log the player out
void WorldSession::LogoutPlayer(bool Save)
{
    sPetBattleSystem->LeaveQueue(m_Player);

    // fix exploit with Aura Bind Sight
    m_Player->StopCastingBindSight();
    m_Player->StopCastingCharm();
    m_Player->RemoveAurasByType(SPELL_AURA_BIND_SIGHT);

    // finish pending transfers before starting the logout
    while (m_Player && m_Player->IsBeingTeleportedFar())
        HandleMoveWorldportAckOpcode();

    m_playerLogout = true;
    m_playerSave = Save;

    if (m_Player)
    {
        if (m_Player->IsInGarrison())
            m_Player->GetGarrison()->OnPlayerLeave();

        if (uint64 lguid = m_Player->GetLootGUID())
            DoLootRelease(lguid);

        ///- If the player just died before logging out, make him appear as a ghost
        //FIXME: logout must be delayed in case lost connection with client in time of combat
        if (m_Player->GetDeathTimer())
        {
            m_Player->getHostileRefManager().deleteReferences();
            m_Player->BuildPlayerRepop();
            m_Player->RepopAtGraveyard();
        }
        else if (!m_Player->getAttackers().empty())
        {
            // build set of player who attack _player or who have pet attacking of _player
            std::set<Player*> aset;
            for (Unit::AttackerSet::const_iterator itr = m_Player->getAttackers().begin(); itr != m_Player->getAttackers().end(); ++itr)
            {
                Unit* owner = (*itr)->GetOwner();           // including player controlled case
                if (owner && owner->IsPlayer())
                    aset.insert(owner->ToPlayer());
                else if ((*itr)->IsPlayer())
                    aset.insert((Player*)(*itr));
            }

            // CombatStop() method is removing all attackers from the AttackerSet
            // That is why it must be AFTER building current set of attackers
            m_Player->CombatStop();
            m_Player->getHostileRefManager().setOnlineOfflineState(false);
            m_Player->RemoveAllAurasOnDeath();
            m_Player->SetPvPDeath(!aset.empty());
            m_Player->KillPlayer();
            m_Player->BuildPlayerRepop();
            m_Player->RepopAtGraveyard();

            // give honor to all attackers from set like group case
            for (std::set<Player*>::const_iterator itr = aset.begin(); itr != aset.end(); ++itr)
                (*itr)->RewardHonor(m_Player, aset.size());

            // give bg rewards and update counters like kill by first from attackers
            // this can't be called for all attackers.
            if (!aset.empty())
                if (Battleground* bg = m_Player->GetBattleground())
                    bg->HandleKillPlayer(m_Player, *aset.begin());
        }
        else if (m_Player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
        {
            // this will kill character by SPELL_AURA_SPIRIT_OF_REDEMPTION
            m_Player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
            m_Player->KillPlayer();
            m_Player->BuildPlayerRepop();
            m_Player->RepopAtGraveyard();
        }
        else if (m_Player->HasPendingBind())
        {
            m_Player->RepopAtGraveyard();
            m_Player->SetPendingBind(0, 0);
        }
        else if (m_Player->GetVehicleBase() && m_Player->isInCombat())
        {
            m_Player->KillPlayer();
            m_Player->BuildPlayerRepop();
            m_Player->RepopAtGraveyard();
        }

        //drop a flag if player is carrying it
        if (Battleground* bg = m_Player->GetBattleground())
            bg->EventPlayerLoggedOut(m_Player);

        ///- Teleport to home if the player is in an invalid instance
        if (!m_Player->m_InstanceValid && !m_Player->isGameMaster())
            m_Player->TeleportTo(m_Player->m_homebindMapId, m_Player->m_homebindX, m_Player->m_homebindY, m_Player->m_homebindZ, m_Player->GetOrientation());

        sOutdoorPvPMgr->HandlePlayerLeaveZone(m_Player, m_Player->GetZoneId());

        for (int i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
        {
            if (MS::Battlegrounds::BattlegroundType::Type bgQueueTypeId = m_Player->GetBattlegroundQueueTypeId(i))
            {
                m_Player->RemoveBattlegroundQueueId(bgQueueTypeId);
                sBattlegroundMgr->RemovePlayer(m_Player->GetGUID(), true, bgQueueTypeId);
            }
        }

        /// If, when the player logout, the battleground pointer of the player is still good, we apply deserter buff.
        if (Save && m_Player->GetBattleground() != nullptr && m_Player->GetBattleground()->GetStatus() != STATUS_WAIT_LEAVE)
        {
            /// We add the Deserter buff, otherwise it can be used bug.
            m_Player->AddAura(MS::Battlegrounds::Spells::DeserterBuff, m_Player);
        }

        // Repop at GraveYard or other player far teleport will prevent saving player because of not present map
        // Teleport player immediately for correct player save
        while (m_Player->IsBeingTeleportedFar())
            HandleMoveWorldportAckOpcode();

        ///- If the player is in a guild, update the guild roster and broadcast a logout message to other guild members
        if (Guild* guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
            guild->HandleMemberLogout(this);

        m_Player->UnsummonCurrentBattlePetIfAny(true);

        ///- Remove pet
        if (m_Player->getClass() != CLASS_WARLOCK)
        {
            if (Pet* _pet = m_Player->GetPet())
                m_Player->RemovePet(_pet, PET_SLOT_ACTUAL_PET_SLOT, true, _pet->m_Stampeded);
            else
                m_Player->RemovePet(NULL, PET_SLOT_ACTUAL_PET_SLOT, true, true);
        }
        else
        {
            if (Pet* _pet = m_Player->GetPet())
                _pet->SavePetToDB(PET_SLOT_ACTUAL_PET_SLOT, _pet->m_Stampeded);
        }

        //! Call script hook before deletion
        sScriptMgr->OnPlayerLogout(m_Player);

        ///- empty buyback items and save the player in the database
        // some save parts only correctly work in case player present in map/player_lists (pets, etc)
        if (Save)
        {
            uint32 eslot;
            for (int j = BUYBACK_SLOT_START; j < BUYBACK_SLOT_END; ++j)
            {
                eslot = j - BUYBACK_SLOT_START;
                m_Player->SetGuidValue(PLAYER_FIELD_INV_SLOTS + (BUYBACK_SLOT_START * 4) + (eslot * 4), 0);
                m_Player->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE + eslot, 0);
                m_Player->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP + eslot, 0);
            }
            m_Player->SaveToDB();
        }

        ///- Leave all channels before player delete...
        m_Player->CleanupChannels();

        ///- If the player is in a group (or invited), remove him. If the group if then only 1 person, disband the group.
        m_Player->UninviteFromGroup();

        // remove player from the group if he is:
        // a) in group; b) not in raid group; c) logging out normally (not being kicked or disconnected)
        if (m_Player->GetGroup() && !m_Player->GetGroup()->isRaidGroup() && m_Socket)
            m_Player->RemoveFromGroup();

        //! Send update to group and reset stored max enchanting level
        if (m_Player->GetGroup())
        {
            m_Player->GetGroup()->SendUpdate();
            m_Player->GetGroup()->ResetMaxEnchantingLevel();
        }

        //! Broadcast a logout message to the player's friends
        sSocialMgr->SendFriendStatus(m_Player, FRIEND_OFFLINE, m_Player->GetGUIDLow(), true);
        sSocialMgr->RemovePlayerSocial(m_Player->GetGUIDLow());

        //! Remove the player from the world
        // the player may not be in the world when logging out
        // e.g if he got disconnected during a transfer to another map
        // calls to GetMap in this case may cause crashes
        m_Player->CleanupsBeforeDelete();
        sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Logout Character:[%s] (GUID: %u) Level: %d", GetAccountId(), GetRemoteAddress().c_str(), m_Player->GetName(), m_Player->GetGUIDLow(), m_Player->getLevel());
        if (Map* _map = m_Player->FindMap())
            _map->RemovePlayerFromMap(m_Player, true);

        SetPlayer(NULL); //! Pointer already deleted during RemovePlayerFromMap

        //! Send the 'logout complete' packet to the client
        //! Client will respond by sending 3x CMSG_CANCEL_TRADE, which we currently dont handle
        WorldPacket data(SMSG_LOGOUT_COMPLETE, 16);
        data.appendPackGUID(0);
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "SESSION: Sent SMSG_LOGOUT_COMPLETE Message");

        //! Since each account can only have one online character at any given time, ensure all characters for active account are marked as offline
        CharacterDatabase.PExecute("UPDATE characters SET online = 0 WHERE account = '%u'", GetAccountId());
    }

    m_playerLogout = false;
    m_playerSave = false;
    m_playerRecentlyLogout = true;
    LogoutRequest(0);
}

/// Kick a player out of the World
void WorldSession::KickPlayer()
{
    if (m_Socket)
        m_Socket->CloseSocket();
}

void WorldSession::SendNotification(const char *format, ...)
{
    if (format)
    {
        va_list ap;
        char szStr[1024];
        szStr[0] = '\0';
        va_start(ap, format);
        vsnprintf(szStr, 1024, format, ap);
        va_end(ap);

        size_t len = strlen(szStr);
        WorldPacket l_Data(SMSG_PRINT_NOTIFICATION, 2 + len);
        l_Data.WriteBits(len, 12);
        l_Data.FlushBits();
        l_Data.append(szStr, len);
        SendPacket(&l_Data);
    }
}

void WorldSession::SendNotification(uint32 string_id, ...)
{
    char const* format = GetTrinityString(string_id);
    if (format)
    {
        va_list ap;
        char szStr[1024];
        szStr[0] = '\0';
        va_start(ap, string_id);
        vsnprintf(szStr, 1024, format, ap);
        va_end(ap);

        size_t len = strlen(szStr);
        WorldPacket l_Data(SMSG_PRINT_NOTIFICATION, 2 + len);
        l_Data.WriteBits(len, 12);
        l_Data.FlushBits();
        l_Data.append(szStr, len);
        SendPacket(&l_Data);
    }
}

const char *WorldSession::GetTrinityString(int32 entry) const
{
    return sObjectMgr->GetTrinityString(entry, GetSessionDbLocaleIndex());
}

void WorldSession::Handle_NULL(WorldPacket& recvPacket)
{
    ///sLog->outError(LOG_FILTER_OPCODES, "Received unhandled opcode %s from %s", GetOpcodeNameForLogging(recvPacket.GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), GetPlayerName(false).c_str());
}

void WorldSession::Handle_EarlyProccess(WorldPacket& recvPacket)
{
    sLog->outError(LOG_FILTER_OPCODES, "Received opcode %s that must be processed in WorldSocket::OnRead from %s", GetOpcodeNameForLogging(recvPacket.GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), GetPlayerName(false).c_str());
}

void WorldSession::Handle_ServerSide(WorldPacket& recvPacket)
{
    sLog->outError(LOG_FILTER_OPCODES, "Received server-side opcode %s from %s", GetOpcodeNameForLogging(recvPacket.GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), GetPlayerName(false).c_str());
}

void WorldSession::Handle_Deprecated(WorldPacket& recvPacket)
{
    ///sLog->outError(LOG_FILTER_OPCODES, "Received deprecated opcode %s from %s", GetOpcodeNameForLogging(recvPacket.GetOpcode(), WOW_CLIENT_TO_SERVER).c_str(), GetPlayerName(false).c_str());
}

void WorldSession::SendAuthWaitQue(uint32 position)
{
    if (position == 0)
        SendAuthResponse(AUTH_OK, false);
    else
        SendAuthResponse(AUTH_OK, true, position);
}

void WorldSession::LoadGlobalAccountData()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_DATA);
    stmt->setUInt32(0, GetAccountId());
    LoadAccountData(CharacterDatabase.Query(stmt), GLOBAL_CACHE_MASK);
}

void WorldSession::LoadAccountData(PreparedQueryResult result, uint32 mask)
{
    for (uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        if (mask & (1 << i))
            m_accountData[i] = AccountData();

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 type = fields[0].GetUInt8();
        if (type >= NUM_ACCOUNT_DATA_TYPES)
        {
            sLog->outError(LOG_FILTER_GENERAL, "Table `%s` have invalid account data type (%u), ignore.",
                mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", type);
            continue;
        }

        if ((mask & (1 << type)) == 0)
        {
            sLog->outError(LOG_FILTER_GENERAL, "Table `%s` have non appropriate for table  account data type (%u), ignore.",
                mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", type);
            continue;
        }

        m_accountData[type].Time = time_t(fields[1].GetUInt32());
        m_accountData[type].Data = fields[2].GetString();
    }
    while (result->NextRow());
}

void WorldSession::SetAccountData(AccountDataType type, time_t tm, std::string data)
{
    uint32 id = 0;
    CharacterDatabaseStatements index;
    if ((1 << type) & GLOBAL_CACHE_MASK)
    {
        id = GetAccountId();
        index = CHAR_REP_ACCOUNT_DATA;
    }
    else
    {
        // _player can be NULL and packet received after logout but m_GUID still store correct guid
        if (!m_GUIDLow)
            return;

        id = m_GUIDLow;
        index = CHAR_REP_PLAYER_ACCOUNT_DATA;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(index);
    stmt->setUInt32(0, id);
    stmt->setUInt8 (1, type);
    stmt->setUInt32(2, uint32(tm));
    stmt->setString(3, data);
    CharacterDatabase.Execute(stmt);

    m_accountData[type].Time = tm;
    m_accountData[type].Data = data;
}

void WorldSession::SendAccountDataTimes(uint64 p_Guid)
{
    WorldPacket l_Data(SMSG_ACCOUNT_DATA_TIMES, 16 + 2 + 4 + (NUM_ACCOUNT_DATA_TYPES * 4));
    l_Data.appendPackGUID(p_Guid);
    l_Data << uint32(time(NULL));                                           ///< Server time

    for (uint32 l_I = 0; l_I < NUM_ACCOUNT_DATA_TYPES; ++l_I)
        l_Data << uint32(GetAccountData(AccountDataType(l_I))->Time);       ///< also unix time

    SendPacket(&l_Data);
}

void WorldSession::LoadTutorialsData()
{
    memset(m_Tutorials, 0, sizeof(uint32) * MAX_ACCOUNT_TUTORIAL_VALUES);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TUTORIALS);
    stmt->setUInt32(0, GetAccountId());
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
        for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
            m_Tutorials[i] = (*result)[i].GetUInt32();

    m_TutorialsChanged = false;
}

void WorldSession::SendTutorialsData()
{
    WorldPacket data(SMSG_TUTORIAL_FLAGS, 4 * MAX_ACCOUNT_TUTORIAL_VALUES);
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        data << m_Tutorials[i];
    SendPacket(&data);
}

void WorldSession::SaveTutorialsData(SQLTransaction &trans)
{
    if (!m_TutorialsChanged)
        return;

    // Modify data in DB
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_TUTORIALS);
    for (uint8 l_I = 0; l_I < MAX_ACCOUNT_TUTORIAL_VALUES; ++l_I)
        l_Statement->setUInt32(l_I, m_Tutorials[l_I]);
    l_Statement->setUInt32(MAX_ACCOUNT_TUTORIAL_VALUES, GetAccountId());
    trans->Append(l_Statement);

    m_TutorialsChanged = false;
}

void WorldSession::ReadAddonsInfo(WorldPacket &data)
{
    if (data.rpos() + 4 > data.size())
        return;

    uint32 size;
    data >> size;

    if (!size)
        return;

    if (size > 0xFFFFF)
    {
        sLog->outError(LOG_FILTER_GENERAL, "WorldSession::ReadAddonsInfo addon info too big, size %u", size);
        return;
    }

    uLongf uSize = size;

    uint32 pos = data.rpos();

    ByteBuffer addonInfo;
    addonInfo.resize(size);

    if (uncompress(const_cast<uint8*>(addonInfo.contents()), &uSize, const_cast<uint8*>(data.contents() + pos), data.size() - pos) == Z_OK)
    {
        uint32 addonsCount;
        addonInfo >> addonsCount;                         // addons count

        for (uint32 i = 0; i < addonsCount; ++i)
        {
            std::string addonName;
            uint8 enabled;
            uint32 crc, unk1;

            // check next addon data format correctness
            if (addonInfo.rpos() + 1 > addonInfo.size())
                return;

            addonInfo >> addonName;

            addonInfo >> enabled >> crc >> unk1;

            sLog->outInfo(LOG_FILTER_GENERAL, "ADDON: Name: %s, Enabled: 0x%x, CRC: 0x%x, Unknown2: 0x%x", addonName.c_str(), enabled, crc, unk1);

            AddonInfo addon(addonName, enabled, crc, 2, true);

            SavedAddon const* savedAddon = AddonMgr::GetAddonInfo(addonName);
            if (savedAddon)
            {
                bool match = true;

                if (addon.CRC != savedAddon->CRC)
                    match = false;

                if (!match)
                    sLog->outInfo(LOG_FILTER_GENERAL, "ADDON: %s was known, but didn't match known CRC (0x%x)!", addon.Name.c_str(), savedAddon->CRC);
                else
                    sLog->outInfo(LOG_FILTER_GENERAL, "ADDON: %s was known, CRC is correct (0x%x)", addon.Name.c_str(), savedAddon->CRC);
            }
            else
            {
                AddonMgr::SaveAddon(addon);

                sLog->outInfo(LOG_FILTER_GENERAL, "ADDON: %s (0x%x) was not known, saving...", addon.Name.c_str(), addon.CRC);
            }

            // TODO: Find out when to not use CRC/pubkey, and other possible states.
            m_addonsList.push_back(addon);
        }

        uint32 currentTime;
        addonInfo >> currentTime;
        sLog->outDebug(LOG_FILTER_NETWORKIO, "ADDON: CurrentTime: %u", currentTime);

        if (addonInfo.rpos() != addonInfo.size())
            sLog->outDebug(LOG_FILTER_NETWORKIO, "packet under-read!");
    }
    else
        sLog->outError(LOG_FILTER_GENERAL, "Addon packet uncompress error!");
}

void WorldSession::SendAddonsInfo()
{
    uint8 l_AddonPublicKey[256] =
    {
        0xC3, 0x5B, 0x50, 0x84, 0xB9, 0x3E, 0x32, 0x42, 0x8C, 0xD0, 0xC7, 0x48, 0xFA, 0x0E, 0x5D, 0x54,
        0x5A, 0xA3, 0x0E, 0x14, 0xBA, 0x9E, 0x0D, 0xB9, 0x5D, 0x8B, 0xEE, 0xB6, 0x84, 0x93, 0x45, 0x75,
        0xFF, 0x31, 0xFE, 0x2F, 0x64, 0x3F, 0x3D, 0x6D, 0x07, 0xD9, 0x44, 0x9B, 0x40, 0x85, 0x59, 0x34,
        0x4E, 0x10, 0xE1, 0xE7, 0x43, 0x69, 0xEF, 0x7C, 0x16, 0xFC, 0xB4, 0xED, 0x1B, 0x95, 0x28, 0xA8,
        0x23, 0x76, 0x51, 0x31, 0x57, 0x30, 0x2B, 0x79, 0x08, 0x50, 0x10, 0x1C, 0x4A, 0x1A, 0x2C, 0xC8,
        0x8B, 0x8F, 0x05, 0x2D, 0x22, 0x3D, 0xDB, 0x5A, 0x24, 0x7A, 0x0F, 0x13, 0x50, 0x37, 0x8F, 0x5A,
        0xCC, 0x9E, 0x04, 0x44, 0x0E, 0x87, 0x01, 0xD4, 0xA3, 0x15, 0x94, 0x16, 0x34, 0xC6, 0xC2, 0xC3,
        0xFB, 0x49, 0xFE, 0xE1, 0xF9, 0xDA, 0x8C, 0x50, 0x3C, 0xBE, 0x2C, 0xBB, 0x57, 0xED, 0x46, 0xB9,
        0xAD, 0x8B, 0xC6, 0xDF, 0x0E, 0xD6, 0x0F, 0xBE, 0x80, 0xB3, 0x8B, 0x1E, 0x77, 0xCF, 0xAD, 0x22,
        0xCF, 0xB7, 0x4B, 0xCF, 0xFB, 0xF0, 0x6B, 0x11, 0x45, 0x2D, 0x7A, 0x81, 0x18, 0xF2, 0x92, 0x7E,
        0x98, 0x56, 0x5D, 0x5E, 0x69, 0x72, 0x0A, 0x0D, 0x03, 0x0A, 0x85, 0xA2, 0x85, 0x9C, 0xCB, 0xFB,
        0x56, 0x6E, 0x8F, 0x44, 0xBB, 0x8F, 0x02, 0x22, 0x68, 0x63, 0x97, 0xBC, 0x85, 0xBA, 0xA8, 0xF7,
        0xB5, 0x40, 0x68, 0x3C, 0x77, 0x86, 0x6F, 0x4B, 0xD7, 0x88, 0xCA, 0x8A, 0xD7, 0xCE, 0x36, 0xF0,
        0x45, 0x6E, 0xD5, 0x64, 0x79, 0x0F, 0x17, 0xFC, 0x64, 0xDD, 0x10, 0x6F, 0xF3, 0xF5, 0xE0, 0xA6,
        0xC3, 0xFB, 0x1B, 0x8C, 0x29, 0xEF, 0x8E, 0xE5, 0x34, 0xCB, 0xD1, 0x2A, 0xCE, 0x79, 0xC3, 0x9A,
        0x0D, 0x36, 0xEA, 0x01, 0xE0, 0xAA, 0x91, 0x20, 0x54, 0xF0, 0x72, 0xD8, 0x1E, 0xC7, 0x89, 0xD2
    };

    uint32 l_AllowedAddonCount  = m_addonsList.size();
    uint32 l_BannedAddonCount   = 0;

    WorldPacket l_Data(SMSG_ADDON_INFO, 15 * 1024);

    l_Data << uint32(l_AllowedAddonCount);                                  ///< Allowed addon count
    l_Data << uint32(l_BannedAddonCount);                                   ///< Banned addon count

    for (AddonsList::iterator l_It = m_addonsList.begin(); l_It != m_addonsList.end(); ++l_It)
    {
        std::string l_Url = "";

        bool l_HasUrl       = !l_Url.empty();
        bool l_UsePublicKey = l_It->CRC != STANDARD_ADDON_CRC;
        bool l_UsePublicCRC = l_It->UsePublicKeyOrCRC;

        l_Data << uint8(l_It->State);                                       ///< Addon state (2 for login)

        l_Data.WriteBit(l_UsePublicCRC);                                    ///< Has crc informations
        l_Data.WriteBit(l_UsePublicKey);                                    ///< Has new addon public key
        l_Data.WriteBit(l_HasUrl);                                          ///< Has addon url
        l_Data.FlushBits();

        if (l_HasUrl)
        {
            l_Data.WriteBits(l_Url.length(), 8);                            ///< Url length
            l_Data.FlushBits();
            l_Data.WriteString(l_Url);                                      ///< Url str
        }

        if (l_UsePublicCRC)
        {
            uint32 l_PublicCRC = 0;

            uint8 l_IsEnabled = 1;

            l_Data << l_IsEnabled;                                          ///< uint8  => Is enabled
            l_Data << l_PublicCRC;                                          ///< uint32 => new CRC
        }

        if (l_UsePublicKey)
        {
            sLog->outInfo(LOG_FILTER_GENERAL, "ADDON: CRC (0x%x) for addon %s is wrong (does not match expected 0x%x), sending pubkey", l_It->CRC, l_It->Name.c_str(), STANDARD_ADDON_CRC);

            l_Data.append(l_AddonPublicKey, sizeof(l_AddonPublicKey));      ///< Addon public key
        }
    }

    m_addonsList.clear();

    SendPacket(&l_Data);
}

void WorldSession::SendFeatureSystemStatus()
{
    bool l_EuropaTicketSystemEnabled            = true;
    bool l_PlayTimeAlert                        = false;
    bool l_ScrollOfResurrectionEnabled          = false;
    bool l_VoiceChatSystemEnabled               = false;
    bool l_ItemRestorationButtonEnbaled         = false;
    bool l_RecruitAFriendSystem                 = false;
    bool l_HasTravelPass                        = false;
    bool l_InGameBrowser                        = sBattlepayMgr->IsAvailable(this);;
    bool l_StoreEnabled                         = true;
    bool l_StoreIsDisabledByParentalControls    = false;
    bool l_StoreIsAvailable                     = sBattlepayMgr->IsAvailable(this);
    bool l_IsRestrictedAccount                  = false;
    bool l_IsTutorialEnabled                    = false;
    bool l_ShowNPETutorial                      = true;
    bool l_TwitterEnabled                       = true;
    bool l_CommerceSystemEnabled                = true;

    uint32 l_PlayTimeAlertDisplayAlertTime      = 0;
    uint32 l_PlayTimeAlertDisplayAlertDelay     = 0;
    uint32 l_PlayTimeAlertDisplayAlertPeriod    = 0;

    uint32 l_SORRemaining = 1;
    uint32 l_SORMaxPerDay = 1;

    uint32 l_ConfigRealmRecordID    = 640;
    uint32 l_ConfigRealmID          = g_RealmID;

    uint32 l_ComplainSystemStatus = 2;                              ///< 0 - Disabled | 1 - Calendar & Mail | 2 - Calendar & Mail & Ignoring system

    uint32 l_TwitterPostThrottleLimit       = 60;
    uint32 l_TwitterPostThrottleCooldown    = 20;
    uint32 l_TokenPollTimeSeconds           = 300;
    uint32 l_TokenRedeemIndex               = 0;

    WorldPacket l_Data(SMSG_FEATURE_SYSTEM_STATUS, 100);

    l_Data << uint8(l_ComplainSystemStatus);                        ///< Complaints System Status
    l_Data << uint32(l_SORMaxPerDay);                               ///< Max SOR Per day
    l_Data << uint32(l_SORRemaining);                               ///< SOR remaining
    l_Data << uint32(l_ConfigRealmID);                              ///< Config Realm ID
    l_Data << uint32(l_ConfigRealmRecordID);                        ///< Config Realm Record ID (used for url dbc reading)
    l_Data << uint32(l_TwitterPostThrottleLimit);                   ///< Number of twitter posts the client can send before they start being throttled
    l_Data << uint32(l_TwitterPostThrottleCooldown);                ///< Time in seconds the client has to wait before posting again after hitting post limit
    l_Data << uint32(l_TokenPollTimeSeconds);                       ///< TokenPollTimeSeconds
    l_Data << uint32(l_TokenRedeemIndex);                           ///< TokenRedeemIndex

    l_Data.WriteBit(l_VoiceChatSystemEnabled);                      ///< voice Chat System Status
    l_Data.WriteBit(l_EuropaTicketSystemEnabled);                   ///< Europa Ticket System Enabled
    l_Data.WriteBit(l_ScrollOfResurrectionEnabled);                 ///< Scroll Of Resurrection Enabled
    l_Data.WriteBit(l_StoreEnabled);                                ///< Store system status
    l_Data.WriteBit(l_StoreIsAvailable);                            ///< Can purchase in store
    l_Data.WriteBit(l_StoreIsDisabledByParentalControls);           ///< Is store disabled by parental controls
    l_Data.WriteBit(l_ItemRestorationButtonEnbaled);                ///< Item Restoration Button Enabled
    l_Data.WriteBit(l_InGameBrowser);                               ///< Web ticket system enabled
    l_Data.WriteBit(l_PlayTimeAlert);                               ///< Session Alert Enabled
    l_Data.WriteBit(l_RecruitAFriendSystem);                        ///< Recruit A Friend System Status
    l_Data.WriteBit(l_HasTravelPass);                               ///< Has travel pass (can group with cross-realm Battle.net friends.)
    l_Data.WriteBit(l_IsRestrictedAccount);                         ///< Is restricted account
    l_Data.WriteBit(l_IsTutorialEnabled);                           ///< Is tutorial system enabled
    l_Data.WriteBit(l_ShowNPETutorial);                             ///< Show NPE tutorial
    l_Data.WriteBit(l_TwitterEnabled);                              ///< Enable ingame twitter interface 
    l_Data.WriteBit(l_CommerceSystemEnabled);                       ///< Commerce System Enabled (WoWToken)
    l_Data.WriteBit(1);                                             ///< Unk 6.1.2 19796
    l_Data.WriteBit(1);                                             ///< WillKickFromWorld
    l_Data.WriteBit(0);                                             ///< Unk 6.1.2 19796 -- unk block
    l_Data.FlushBits();

    if (l_EuropaTicketSystemEnabled)
    {
        l_Data.WriteBit(true);                                      ///< TicketsEnabled
        l_Data.WriteBit(true);                                      ///< BugsEnabled
        l_Data.WriteBit(true);                                      ///< ComplaintsEnabled
        l_Data.WriteBit(true);                                      ///< SuggestionsEnabled
        l_Data.FlushBits();

        l_Data << uint32(10);                                       ///< Max Tries
        l_Data << uint32(60000);                                    ///< Per Milliseconds
        l_Data << uint32(1);                                        ///< Try Count
        l_Data << uint32(0);                                        ///< Last Reset Time Before Now
    }

    if (l_PlayTimeAlert)
    {
        l_Data << uint32(l_PlayTimeAlertDisplayAlertDelay);         ///< Alert delay
        l_Data << uint32(l_PlayTimeAlertDisplayAlertPeriod);        ///< Alert period
        l_Data << uint32(l_PlayTimeAlertDisplayAlertTime);          ///< Alert display time
    }

    SendPacket(&l_Data);
}

void WorldSession::SendTimeZoneInformations()
{
    const static std::string l_TimeZoneName = "Europe/Paris";

    WorldPacket l_Data(SMSG_SET_TIME_ZONE_INFORMATION, 26);
    l_Data.WriteBits(l_TimeZoneName.size(), 7);
    l_Data.WriteBits(l_TimeZoneName.size(), 7);
    l_Data.FlushBits();

    l_Data.WriteString(l_TimeZoneName);
    l_Data.WriteString(l_TimeZoneName);

    SendPacket(&l_Data);
}

bool WorldSession::IsAddonRegistered(const std::string& prefix) const
{
    if (!_filterAddonMessages) // if we have hit the softcap (64) nothing should be filtered
        return true;

    if (_registeredAddonPrefixes.empty())
        return false;

    std::vector<std::string>::const_iterator itr = std::find(_registeredAddonPrefixes.begin(), _registeredAddonPrefixes.end(), prefix);
    return itr != _registeredAddonPrefixes.end();
}

void WorldSession::HandleUnregisterAddonPrefixesOpcode(WorldPacket& /*recvPacket*/) // empty packet
{
    _registeredAddonPrefixes.clear();
}

void WorldSession::HandleAddonRegisteredPrefixesOpcode(WorldPacket& p_Packet)
{
    /// This is always sent after CMSG_UNREGISTER_ALL_ADDON_PREFIXES
    uint32 l_Count = 0;
    
    p_Packet >> l_Count;
    
    if (l_Count > REGISTERED_ADDON_PREFIX_SOFTCAP)
    {
        /// if we have hit the softcap (64) nothing should be filtered
        _filterAddonMessages = false;
        p_Packet.rfinish();
        return;
    }

    for (uint32 l_I = 0; l_I < l_Count; ++l_I)
    {
        uint8 l_Size = p_Packet.ReadBits(5);

        p_Packet.ResetBitReading();

        _registeredAddonPrefixes.push_back(p_Packet.ReadString(l_Size));
    }

    if (_registeredAddonPrefixes.size() > REGISTERED_ADDON_PREFIX_SOFTCAP) // shouldn't happen
    {
        _filterAddonMessages = false;
        return;
    }

    _filterAddonMessages = true;
}

void WorldSession::SetPlayer(Player* player)
{
    m_Player = player;

    // set m_GUID that can be used while player loggined and later until m_playerRecentlyLogout not reset
    if (m_Player)
        m_GUIDLow = m_Player->GetGUIDLow();
}

void WorldSession::InitializeQueryCallbackParameters()
{
    // Callback parameters that have pointers in them should be properly
    // initialized to NULL here.
    _charCreateCallback.SetParam(NULL);
}

void WorldSession::ProcessQueryCallbacks()
{
    uint32 l_StartTime = getMSTime();
    std::vector<uint32> l_Times;

    PreparedQueryResult result;

    //! Vote
    if (m_VoteTimeCallback.IsReady())
    {
        switch (m_VoteTimeCallback.GetStage())
        {
            /// - DB is sync (time elapsed update is done)
            case 1:
            {
                std::ostringstream l_Query;
                l_Query << "SELECT remainingTime FROM account_vote WHERE account = " << GetAccountId();

                m_VoteTimeCallback.FreeResult();
                m_VoteTimeCallback.SetFutureResult(LoginDatabase.AsyncQuery(l_Query.str().c_str()));
                m_VoteTimeCallback.NextStage();
                break;
            }
            /// - Now, we can sync the core !
            case 2:
            {
                QueryResult l_Result;
                m_VoteTimeCallback.GetResult(l_Result);

                m_VoteTimePassed    = 0;
                m_VoteRemainingTime = l_Result ? l_Result->Fetch()->GetUInt32() : 0;

                if (m_VoteRemainingTime == 0 && m_Player && m_Player->HasAura(VOTE_BUFF))
                    m_Player->RemoveAurasDueToSpell(VOTE_BUFF);
                else if (m_Player && m_VoteRemainingTime != 0)
                {
                    Aura* l_Aura = m_Player->HasAura(VOTE_BUFF) ? m_Player->GetAura(VOTE_BUFF) : m_Player->AddAura(VOTE_BUFF, m_Player);
                    if (l_Aura != nullptr)
                        l_Aura->SetDuration(m_VoteRemainingTime + 60 * IN_MILLISECONDS);    //< Add remaining time + 1 mins (callback lag)
                }

                m_VoteSyncTimer = VOTE_SYNC_TIMER;

                m_VoteTimeCallback.FreeResult();
                m_VoteTimeCallback.Reset();
                break;
            }
            /// - Can't happen
            default:
                break;
        }
    }

    l_Times.push_back(getMSTime() - l_StartTime);

    //! HandleCharEnumOpcode
    if (m_CharEnumCallback.ready())
    {
        m_CharEnumCallback.get(result);
        HandleCharEnum(result);
        m_CharEnumCallback.cancel();
    }

    l_Times.push_back(getMSTime() - l_StartTime);

    if (_charCreateCallback.IsReady())
    {
        _charCreateCallback.GetResult(result);
        HandleCharCreateCallback(result, _charCreateCallback.GetParam());
        // Don't call FreeResult() here, the callback handler will do that depending on the events in the callback chain
    }

    l_Times.push_back(getMSTime() - l_StartTime);

    //! HandlePlayerLoginOpcode
    if (m_CharacterLoginCallback.ready() && m_CharacterLoginDBCallback.ready())
    {
        SQLQueryHolder* l_Param;
        SQLQueryHolder* l_Param2;
        m_CharacterLoginCallback.get(l_Param);
        m_CharacterLoginDBCallback.get(l_Param2);
        HandlePlayerLogin((LoginQueryHolder*)l_Param, (LoginDBQueryHolder*)l_Param2);
        m_CharacterLoginCallback.cancel();
        m_CharacterLoginDBCallback.cancel();
    }

    l_Times.push_back(getMSTime() - l_StartTime);


    //! HandleAddFriendOpcode
    if (_addFriendCallback.IsReady())
    {
        std::string param = _addFriendCallback.GetParam();
        _addFriendCallback.GetResult(result);
        HandleAddFriendOpcodeCallBack(result, param);
        _addFriendCallback.FreeResult();
    }

    l_Times.push_back(getMSTime() - l_StartTime);

    //- HandleCharRenameOpcode
    if (_charRenameCallback.IsReady())
    {
        std::string param = _charRenameCallback.GetParam();
        _charRenameCallback.GetResult(result);
        HandleChangePlayerNameOpcodeCallBack(result, param);
        _charRenameCallback.FreeResult();
    }

    l_Times.push_back(getMSTime() - l_StartTime);

    //- HandleCharAddIgnoreOpcode
    if (m_AddIgnoreCallback.ready())
    {
        m_AddIgnoreCallback.get(result);
        HandleAddIgnoreOpcodeCallBack(result);
        m_AddIgnoreCallback.cancel();
    }

    l_Times.push_back(getMSTime() - l_StartTime);

    //- SendStabledPet
    if (_sendStabledPetCallback.IsReady())
    {
        uint64 param = _sendStabledPetCallback.GetParam();
        _sendStabledPetCallback.GetResult(result);
        SendStablePetCallback(result, param);
        _sendStabledPetCallback.FreeResult();
    }

    l_Times.push_back(getMSTime() - l_StartTime);

    //- HandleStableSwapPet
    if (_setPetSlotCallback.IsReady())
    {
        uint32 param = _setPetSlotCallback.GetParam();
        _setPetSlotCallback.GetResult(result);
        HandleStableSetPetSlotCallback(result, param);
        _setPetSlotCallback.FreeResult();
    }

    uint32 l_EndTime = getMSTime() - l_StartTime;

    if (l_EndTime > 80)
    {
        sLog->outAshran("ProcessQueryCallbacks take more than 80 ms to execute for account [%u]", GetAccountId());

        uint32 l_Idx = 0;
        for (auto l_DiffTime : l_Times)
        {
            sLog->outAshran("[%u] -----> (%u ms)", l_Idx, l_DiffTime);
            l_Idx++;
        }
    }
}

void WorldSession::InitWarden(BigNumber* k, std::string os)
{
    if (os == "Win")
    {
        _warden = new WardenWin();
        _warden->Init(this, k);
    }
    else if (os == "OSX")
    {
        // Disabled as it is causing the client to crash
        // _warden = new WardenMac();
        // _warden->Init(this, k);
    }
}

void WorldSession::SetServiceFlags(uint32 p_Flags)
{
    m_ServiceFlags |= p_Flags;

    PreparedStatement* l_Statement = LoginDatabase.GetPreparedStatement(LoginDatabaseStatements::LOGIN_SET_ACCOUNT_SERVICE);
    l_Statement->setUInt32(0, p_Flags);
    l_Statement->setUInt32(1, GetAccountId());
    LoginDatabase.AsyncQuery(l_Statement);
}

void WorldSession::UnsetServiceFlags(uint32 p_Flags)
{
    m_ServiceFlags &= ~p_Flags;

    PreparedStatement* l_Statement = LoginDatabase.GetPreparedStatement(LoginDatabaseStatements::LOGIN_REMOVE_ACCOUNT_SERVICE);
    l_Statement->setUInt32(0, p_Flags);
    l_Statement->setUInt32(1, GetAccountId());
    LoginDatabase.AsyncQuery(l_Statement);
}

void WorldSession::SetCustomFlags(uint32 p_Flags)
{
    m_CustomFlags |= p_Flags;

    LoginDatabase.AsyncPQuery("UPDATE account SET custom_flags = custom_flags | %u WHERE id = %u", p_Flags, GetAccountId());
}

void WorldSession::UnsetCustomFlags(uint32 p_Flags)
{
    m_CustomFlags &= ~p_Flags;

    LoginDatabase.AsyncPQuery("UPDATE account SET custom_flags = custom_flags &~ %u WHERE id = %u", p_Flags, GetAccountId());
}

void WorldSession::LoadPremades()
{
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PREMADES);
    l_Statement->setUInt32(0, GetAccountId());

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Statement);
    if (!l_Result)
        return;

    uint32 l_CharactersCount = AccountMgr::GetCharactersCount(GetAccountId());
    if (l_CharactersCount >= sWorld->getIntConfig(CONFIG_CHARACTERS_PER_REALM))
        return;

    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_Transaction = l_Fields[0].GetUInt32();
        uint32 l_TemplateID = l_Fields[1].GetUInt32();
        uint32 l_Faction = l_Fields[2].GetUInt8();
        uint8  l_Type = l_Fields[3].GetUInt8();

        CharacterTemplate const* l_Template = sObjectMgr->GetCharacterTemplate(l_TemplateID);
        if (!l_Template)
            continue;

        WorldPacket l_Data; ///< Dummy
        CharacterCreateInfo* l_CreateInfo = new CharacterCreateInfo("#Premade", l_Faction == TEAM_HORDE ? l_Template->m_HordeDefaultRace : l_Template->m_AllianceDefaultRace, l_Template->m_PlayerClass, 0, 0, 0, 0, 0, 0, 0, 0, l_Data);

        Player l_NewCharacter(this);
        l_NewCharacter.GetMotionMaster()->Initialize();

        if (!l_NewCharacter.Create(sObjectMgr->GenerateLowGuid(HIGHGUID_PLAYER), l_CreateInfo))
        {
            l_NewCharacter.CleanupsBeforeDelete();
            delete l_CreateInfo;
            continue;
        }

        l_NewCharacter.SetAtLoginFlag(AT_LOGIN_FIRST);
        l_NewCharacter.SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);

        l_NewCharacter.SetLevel(l_Template->m_Level);
        l_NewCharacter.SetMoney(l_Template->m_Money);

        for (auto& l_ReputationInfo : l_Template->m_TemplateFactions)
        {
            if (FactionEntry const* l_Faction = sFactionStore.LookupEntry(l_ReputationInfo.m_FactionID))
                l_NewCharacter.GetReputationMgr().SetReputation(l_Faction, l_ReputationInfo.m_Reputaion);
        }

        for (auto l_Spell : l_Template->m_SpellIDs)
            l_NewCharacter.learnSpell(l_Spell, false);

        std::list<CharacterTemplate::TemplateItem const*> l_RemainingTemplates;
        for (auto& l_Item : l_Template->m_TemplateItems)
        {
            if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item.m_ItemID))
            {
                // Give bags first to the players, then the equipment
                if (l_Proto->Class == ITEM_CLASS_CONTAINER)
                {
                    if (!l_Item.m_Faction || (l_Item.m_Faction == 1 && l_NewCharacter.GetTeam() == ALLIANCE) || (l_Item.m_Faction == 2 && l_NewCharacter.GetTeam() == HORDE))
                        l_NewCharacter.StoreNewItemInBestSlots(l_Item.m_ItemID, l_Item.m_Count);

                    continue;
                }
            }
            l_RemainingTemplates.push_back(&l_Item);
        }

        for (auto l_Item : l_RemainingTemplates)
        {
            if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item->m_ItemID))
            {
                if ((l_Proto->AllowableRace & l_NewCharacter.getRaceMask()) == 0)
                    continue;

                if (!l_Item->m_Faction || (l_Item->m_Faction == 1 && l_NewCharacter.GetTeam() == ALLIANCE) || (l_Item->m_Faction == 2 && l_NewCharacter.GetTeam() == HORDE))
                {
                    ItemContext l_Context = ItemContext::None;

                    /// Pve premade have LFR items
                    if (l_Type == 1)
                        l_Context = ItemContext::RaidLfr;

                    if (l_Item->m_Type == 0 || l_Type == l_Item->m_Type)
                        l_NewCharacter.StoreNewItemInBestSlots(l_Item->m_ItemID, l_Item->m_Count, l_Context);
                }
            }
        }

        l_NewCharacter.SaveToDB(true);

        if (l_CreateInfo->Class == CLASS_HUNTER)
        {
            uint32 pet_id = sObjectMgr->GeneratePetNumber();

            switch (l_CreateInfo->Race)
            {
                case RACE_HUMAN:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 299, %u, 903, 13481, 1, 1, 0, 0, 'Wolf', 0, 0, 192, 0, 1295727347, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_DWARF:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42713, %u, 822, 13481, 1, 1, 0, 0, 'Bear', 0, 0, 212, 0, 1295727650, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_ORC:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42719, %u, 744, 13481, 1, 1, 0, 0, 'Boar', 0, 0, 212, 0, 1295727175, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_NIGHTELF:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42718, %u,  17090, 13481, 1, 1, 0, 0, 'Cat', 0, 0, 192, 0, 1295727501, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_UNDEAD_PLAYER:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 51107, %u,  368, 13481, 1, 1, 0, 0, 'Spider', 0, 0, 202, 0, 1295727821, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_TAUREN:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42720, %u,  29057, 13481, 1, 1, 0, 0, 'Tallstrider', 0, 0, 192, 0, 1295727912, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_TROLL:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42721, %u,  23518, 13481, 1, 1, 0, 0, 'Raptor', 0, 0, 192, 0, 1295727987, '7 2 7 1 7 0 129 2649 129 50498 129 16827 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_GOBLIN:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42715, %u, 27692, 13481, 1, 1, 0, 0, 'Crab', 0, 0, 212, 0, 1295720595, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_BLOODELF:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42710, %u, 23515, 13481, 1, 1, 0, 0, 'Dragonhawk', 0, 0, 202, 0, 1295728068, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_DRAENEI:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42712, %u, 29056, 13481, 1, 1, 0, 0, 'Moth', 0, 0, 192, 0, 1295728128, '7 2 7 1 7 0 129 2649 129 49966 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_WORGEN:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42722, %u, 30221, 13481, 1, 1, 0, 0, 'Dog', 0, 0, 192, 0, 1295728219, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                case RACE_PANDAREN_NEUTRAL:
                    CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 57239, %u, 42656, 13481, 1, 1, 0, 0, 'Turtle', 0, 0, 192, 0, 1295728219, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, l_NewCharacter.GetGUIDLow());
                    break;
                default:
                    break;
            }

            CharacterDatabase.PExecute("UPDATE characters SET currentPetSlot = '0', petSlotUsed = '1' WHERE guid = %u", l_NewCharacter.GetGUIDLow());
            l_NewCharacter.SetTemporaryUnsummonedPetNumber(pet_id);
        }

        l_NewCharacter.CleanupsBeforeDelete();
        delete l_CreateInfo;

        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PREMADE_SUCESS);
        l_Statement->setUInt32(0, l_Transaction);
        CharacterDatabase.Execute(l_Statement);
    } while (l_Result->NextRow());
}

/// Send a game error
/// @p_Error : Game error
/// @p_Data1 : Additional data 1
/// @p_Data2 : Additional data 2
void WorldSession::SendGameError(GameError::Type p_Error, uint32 p_Data1, uint32 p_Data2)
{
    WorldPacket l_Packet(SMSG_DISPLAY_GAME_ERROR, 13);
    l_Packet << uint32(p_Error);
    l_Packet.WriteBit(p_Data1 != 0xF0F0F0F0);
    l_Packet.WriteBit(p_Data2 != 0xF0F0F0F0);
    l_Packet.FlushBits();

    if (p_Data1 != 0xF0F0F0F0)
        l_Packet << uint32(p_Data1);

    if (p_Data2 != 0xF0F0F0F0)
        l_Packet << uint32(p_Data2);

    SendPacket(&l_Packet);
}
