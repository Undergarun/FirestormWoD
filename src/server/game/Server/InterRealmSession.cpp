#ifndef CROSS
#include "InterRealmOpcodes.h"
#include "InterRealmSession.h"
#include "World.h"
#include "Config.h"
#include "AccountMgr.h"
#include "Chat.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "BattlegroundMgr.hpp"
#include "AnticheatMgr.h"
#include "GuildMgr.h"
#include "Channel.h"

#ifdef NOT_WINDOWS
#include <netinet/tcp.h>
#endif

#define SLEEP_TIME 30*IN_MILLISECONDS

class IRReactorRunnable : protected ACE_Task_Base
{
    public:

        IRReactorRunnable() :
            m_Reactor(0),
            m_ThreadId(-1)
        {
            ACE_Reactor_Impl* imp = 0;

            #if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)

            imp = new ACE_Dev_Poll_Reactor();

            imp->max_notify_iterations (128);
            imp->restart (1);

            #else

            imp = new ACE_TP_Reactor();
            imp->max_notify_iterations (128);

            #endif

            m_Reactor = new ACE_Reactor (imp, 1);

            m_IRSocket = NULL;
        }

        virtual ~IRReactorRunnable()
        {
            Stop();
            Wait();

            delete m_Reactor;
        }

        void Stop()
        {
            m_Reactor->end_reactor_event_loop();
        }

        int Start()
        {
            if (m_ThreadId != -1)
                return -1;

            return (m_ThreadId = activate());
        }

        void Wait() { ACE_Task_Base::wait(); }

        int SetSocket (IRSocket* sock)
        {
            sock->AddReference();
            sock->reactor(m_Reactor);
            m_IRSocket = sock;

            return 0;
        }

        ACE_Reactor* GetReactor()
        {
            return m_Reactor;
        }

    protected:

        virtual int svc()
        {
            ACE_ASSERT (m_Reactor);

            while (!m_Reactor->reactor_event_loop_done())
            {
                // dont be too smart to move this outside the loop
                // the run_reactor_event_loop will modify interval
                ACE_Time_Value interval (0, 10000);

                if (m_Reactor->run_reactor_event_loop (interval) == -1)
                    break;

                if (m_IRSocket)
                {
                    if (m_IRSocket->Update() == -1)
                    {
                        m_IRSocket->CloseSocket();
                        m_IRSocket->RemoveReference();
                        m_IRSocket = NULL;
                    }
                }
            }

            return 0;
        }

    private:
        typedef ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> AtomicInt;

        ACE_Reactor* m_Reactor;
        int m_ThreadId;

        IRSocket* m_IRSocket;
};

InterRealmSession::InterRealmSession() : m_rand(0), m_tunnel_open(false),
m_RecvWPct(0), m_RecvPct()
{
    m_IsConnected = false;
    m_force_stop = false;
    m_port = 12345;
    m_ir_id = 0;
    m_IRSocket = NULL;
    m_SockOutKBuff = -1;
    m_SockOutUBuff = 65536;
    m_UseNoDelay = true;
    m_needProcessDisconnect = false;
}

InterRealmSession::~InterRealmSession()
{
}

int InterRealmSession::OnSocketOpen(IRSocket* socket)
{
    m_SockOutKBuff = ConfigMgr::GetIntDefault ("Network.OutKBuff", -1);
    m_SockOutUBuff = ConfigMgr::GetIntDefault ("Network.OutUBuff", 65536);

    // set some options here
    if (m_SockOutKBuff >= 0)
    {
        if (socket->peer().set_option (SOL_SOCKET,
            SO_SNDBUF,
            (void*) & m_SockOutKBuff,
            sizeof (int)) == -1 && errno != ENOTSUP)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "InterRealmSession::OnSocketOpen set_option SO_SNDBUF");
            return -1;
        }
    }

    static const int ndoption = 1;

    // Set TCP_NODELAY.
    if (m_UseNoDelay)
    {
        if (socket->peer().set_option (ACE_IPPROTO_TCP,
            TCP_NODELAY,
            (void*)&ndoption,
            sizeof (int)) == -1)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "InterRealmSession::OnSocketOpen peer().set_option TCP_NODELAY errno = %s", ACE_OS::strerror (errno));
            return -1;
        }
    }

    socket->m_OutBufferSize = static_cast<size_t> (m_SockOutUBuff);


    return m_Reactor->SetSocket(socket);
}

void InterRealmSession::ProcessPlayersAfterDisconnect()
{
    m_needProcessDisconnect = false;

    const SessionMap& _sessionMap = sWorld->GetAllSessions();
    if (!_sessionMap.empty())
    {
        for (SessionMap::const_iterator itr = _sessionMap.begin(); itr != _sessionMap.end(); ++itr)
        {
            if (itr->second->GetInterRealmBG())
            {
                itr->second->SetInterRealmBG(0);

                if (Player* l_Player = itr->second->GetPlayer())
                {
                    uint64 l_Guid = l_Player->GetGUID();

                    CrossPartyInfo const l_PartyInfo = itr->second->GetCrossPartyInfo();
                    if (l_PartyInfo.GroupGUID)
                    {
                        /// Clear cross group UI client-side
                        WorldPacket l_Data(SMSG_PARTY_UPDATE, 200);
                        l_Data << uint8(l_PartyInfo.PartyFlags);
                        l_Data << uint8(l_PartyInfo.PartyIndex);
                        l_Data << uint8(l_PartyInfo.PartyType);
                        l_Data << int32(-1);                    ///< MyIndex
                        l_Data.appendPackGUID(l_PartyInfo.GroupGUID);
                        l_Data << uint32(0);
                        l_Data.appendPackGUID(l_PartyInfo.LeaderGUID);
                        l_Data << uint32(0);
                        l_Data.WriteBit(false); ///< LfgInfos
                        l_Data.WriteBit(false); ///< LootSettings
                        l_Data.WriteBit(false);
                        l_Data.FlushBits();
                        l_Player->GetSession()->SendPacket(&l_Data);

                        /// Clear cross party info server-side
                        CrossPartyInfo l_CleanPartyInfo;
                        l_Player->GetSession()->SetCrossPartyInfo(l_CleanPartyInfo);
                    }

                    /// Logout player ...
                    WorldSession* l_Session = l_Player->GetSession();
                    l_Session->LogoutPlayer(false, true);
                    l_Player = nullptr;

                    /// Then login again
                    l_Session->LoginPlayer(l_Guid);
                }
            }
        }
    }
}

void InterRealmSession::ClearSocket()
{
    m_tunnel_open = false;

    if (m_IRSocket)
    {
        m_IRSocket = NULL;
    }

    if (m_Reactor)
    {
        m_Reactor->Stop();
        delete m_Reactor;
    }

    if (m_Connector)
    {
        m_Connector->close();
        delete m_Connector;
    }
}

void InterRealmSession::run()
{
    sLog->outError(LOG_FILTER_INTERREALM, "Connecting to InterRealm...");

    if (!sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE))
    {
        sLog->outError(LOG_FILTER_INTERREALM, "InterRealm is disabled.");
        return;
    }

    m_IP = ConfigMgr::GetStringDefault("InterRealm.IP", "0.0.0.0");
    m_port = uint16(ConfigMgr::GetIntDefault("InterRealm.Port", 12345));
    m_ir_id = ConfigMgr::GetIntDefault("InterRealm.Id", 1);

    sLog->outError(LOG_FILTER_INTERREALM, "Loaded InterRealm configuration, %s:%u, id %u", m_IP.c_str(), m_port, m_ir_id);    

    m_Connector = NULL;

    ACE_INET_Addr connect_addr (m_port, m_IP.c_str());

    while (!World::IsStopped())
    {
        if (!m_IRSocket || m_IRSocket->IsClosed())
        {
            //int i_ret = m_Connector->open(m_Reactor->GetReactor(), ACE_NONBLOCK);
            sLog->outError(LOG_FILTER_INTERREALM, "Trying to connect to interrealm.");

            m_Reactor = new IRReactorRunnable();
            m_Connector = new IRSocketConnector();

            int ret = m_Connector->connect(m_IRSocket, connect_addr);
            if (ret != 0)
            {
                ClearSocket();
                sLog->outError(LOG_FILTER_INTERREALM, "Cannot connect interrealm");    
                ACE_Based::Thread::Sleep(30000);
                continue;
            }

            m_Reactor->Start();

            m_force_stop = false;

            sLog->outError(LOG_FILTER_INTERREALM, "Sending 'hello' message to InterRealm.");
            
            m_rand = urand(0, 255);
            WorldPacket hello_packet(IR_CMSG_HELLO, 10 + 1 + 1 + 1 + 1 + 1);
            hello_packet << std::string("HELO");
            hello_packet << uint8(m_rand);
            SendPacket(&hello_packet);
        }

        ACE_Based::Thread::Sleep(30000);
    }
}

void InterRealmSession::SendPing(uint64 guid, uint32 ping, uint32 latency)
{
    WorldPacket packet(IR_CMSG_PING);
    packet << guid;
    packet << latency;
    packet << ping;
    SendPacket(&packet);
}

void InterRealmSession::SendTunneledPacket(uint64 playerGuid, WorldPacket const* packet)
{
    if (playerGuid == 0)
    {
        delete packet;
        return;
    }

    if (!m_tunnel_open)
    {
        delete packet;
        return;
    }
    
    WorldPacket tmpPacket(IR_CMSG_TUNNEL_PACKET, 8 + 2 + packet->size());
    tmpPacket << (uint64)playerGuid;
    tmpPacket << (uint16)packet->GetOpcode();

    if (packet->size() > 0)
        tmpPacket.append(packet->contents(), packet->size());

    delete packet;
    
    SendPacket(&tmpPacket);
}

void InterRealmSession::SendTunneledPacketToClient(uint64 guid, WorldPacket const *packet)
{
    if (guid == 0)
        return;

    if (packet == NULL)
        return;

    if (!m_IRSocket)
        return;

    WorldPacket tmpPacket(IR_TUNNELED_PACKET_TO_CLIENT, 8 + 2 + packet->size());
    tmpPacket << (uint64)guid;
    tmpPacket << (uint16)packet->GetOpcode();

    for (unsigned int i = 0; i < packet->size(); ++i)
        tmpPacket << (uint8)packet->contents()[i];

    SendPacket(&tmpPacket);
}

void InterRealmSession::SendPacket(WorldPacket const* packet)
{
    if (packet == NULL)
        return;

    if (packet->GetOpcode() != IR_CMSG_WHO_AM_I && 
        packet->GetOpcode() != IR_CMSG_HELLO && !IsConnected())
        return;

    if (!m_IRSocket || m_IRSocket->IsClosed())
        return;

    if (m_IRSocket->SendPacket(packet) == -1)
    {
        //sLog->outInterRealm("[INTERREALM] Cannot send packet %u, closing socket.", packet->GetOpcode());
        //m_IRSocket->CloseSocket();
    }
}

void InterRealmSession::SendPSysMessage(Player *player, char const *format, ...)
{
    va_list ap;
    char str[2048];
    va_start(ap, format);
    vsnprintf(str, 2048, format, ap);
    va_end(ap);

    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = strdup(str);
    char* pos = buf;

    while (char* line = ChatHandler::LineFromMessage(pos))
    {
        ChatHandler(player).FillSystemMessageData(&data, line);
        SendTunneledPacketToClient(player->GetGUID(), &data);
    }

    free(buf);
}

void InterRealmSession::SendServerAnnounce(uint64 guid, std::string const &text)
{
    WorldPacket data(IR_SMSG_SERVER_ANNOUNCE, 8 + text.size());
    data << guid;
    data << text;
    SendPacket(&data);
}

void InterRealmSession::Handle_Unhandled(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Unhandled Packet with IROpcode %u received !",recvPacket.GetOpcode());
}

void InterRealmSession::Handle_Null(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Packet with Invalid IROpcode %u received !",recvPacket.GetOpcode());
}

void InterRealmSession::Handle_Hello(WorldPacket& packet)
{
    //sLog->outInterRealm("[INTERREALM] Received packet IR_SMSG_HELLO");

    std::string _hello;
    uint8 _rand, _resp;

    packet >> _hello;
    packet >> _rand;
    packet >> _resp;

    if (strcmp(_hello.c_str(), "HELO") != 0)
    {
       sLog->outError(LOG_FILTER_INTERREALM, "closing socket !");
        m_force_stop = true;
    }

    if (_rand != m_rand)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Random hello check is incorrect, closing socket");
        m_force_stop = true;
    }

    if (_resp == IR_HELO_RESP_PROTOCOL_MISMATCH)
        sLog->outError(LOG_FILTER_INTERREALM, "InterRealm Protocol Mismatch, closing doors to me !");

    if(_resp == IR_HELO_RESP_POLITE)
        sLog->outError(LOG_FILTER_INTERREALM, "Server like to be polite, closing doors to me !");

    if  (!m_force_stop && _resp == IR_HELO_RESP_OK)
    {
        //sLog->outInterRealm("[INTERREALM] Hello was succeed. Sending id...");

        WorldPacket packet(IR_CMSG_WHO_AM_I, 4);
        packet << uint32(m_ir_id); // Realm Id
        packet << float(sWorld->getRate(RATE_HONOR));
        packet << float(sWorld->getRate(RATE_HONOR_PREMIUM));
        //packet << uint32(sWorld->getIntConfig(CONFIG_MAX_HONOR_POINTS));
        //packet << uint32(sWorld->getIntConfig(CONFIG_MAX_ARENA_POINTS));
        packet << float(sWorld->getRate(RATE_REPUTATION_GAIN));
        packet << float(sWorld->getRate(RATE_REPUTATION_GAIN_PREMIUM));
        SendPacket(&packet);
    }
}

void InterRealmSession::Handle_TunneledPacket(WorldPacket& recvPacket)
{
    uint64 playerGuid;
    uint16 opcodeId;
    recvPacket >> playerGuid;
    recvPacket >> opcodeId;

    recvPacket.eraseFirst(10); // remove playerGuid and opcodeId data
    recvPacket.SetOpcode(opcodeId);
    recvPacket.rfinish();

    // Now we have standart packet

    if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        pPlayer->GetSession()->SendPacket(&recvPacket);
    }
}

void InterRealmSession::Handle_WhoAmI(WorldPacket& packet)
{
    uint8 _valid;
    packet >> _valid;

    if (_valid == 0)
    {
        m_tunnel_open = true;

        SetConnected(true);

        sLog->outError(LOG_FILTER_INTERREALM, "Tunnel is opened.");

        SendBattlegroundHolidaysQuery();
    }
    else
    {
        m_force_stop = true;
        sLog->outError(LOG_FILTER_INTERREALM, "Registration was failed.");
    }
}

void InterRealmSession::Handle_CheckPlayers(WorldPacket& packet)
{
    //sLog->outInterRealm(LOG_FILTER_GENERAL, "[INTERREALM] Received a packet IR_SMSG_CHECK_PLAYERS");
    
    uint32 num_players;
    std::vector<uint64> playerGuids;

    packet >> num_players;

    for (uint32 i = 0; i < num_players; ++i)
    {
        uint64 playerGuid;
        uint8 inIRBG;
        packet >> playerGuid;
        packet >> inIRBG;
        if (inIRBG == 0)
        {
            if (!sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
                playerGuids.push_back(playerGuid);
        }
        else
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
            {
                if (!pPlayer->GetSession()->GetInterRealmBG())
                    playerGuids.push_back(playerGuid);
            }
            else
                playerGuids.push_back(playerGuid);
        }
    }

    if (!playerGuids.empty())
    {
        WorldPacket pckt(IR_CMSG_CHECK_PLAYERS, 1 + 4 + (8 * playerGuids.size()));
        pckt << uint8(1); // has offline players
        pckt << uint32(playerGuids.size());

        for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
            pckt << uint64((*itr));

        SendPacket(&pckt);
    }
    else
    {
        WorldPacket pckt(IR_CMSG_CHECK_PLAYERS, 1);
        pckt << uint8(0);
        SendPacket(&pckt);
    }
}

void InterRealmSession::Handle_DebugBGResp(WorldPacket& packet)
{
    uint8 val;

    packet >> val;

    sWorld->SendWorldText((val != 0) ? LANG_DEBUG_BG_ON : LANG_DEBUG_BG_OFF);
}

void InterRealmSession::Handle_DebugArenaResp(WorldPacket& packet)
{
    uint8 val;

    packet >> val;

    sWorld->SendWorldText((val != 0) ? LANG_DEBUG_ARENA_ON : LANG_DEBUG_ARENA_OFF);
}

void InterRealmSession::Handle_RegisterPlayerResp(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Received a packet IR_SMSG_REGISTER_PLAYER_RESP");
    uint64 playerGuid;
    uint8 _valid;

    recvPacket >> _valid;
    recvPacket >> playerGuid;

    if (_valid != 0)
    {
        //sLog->outInterRealm("[INTERREALM] Player registration was failed (%u)(loguid %u).", _valid, GUID_LOPART(playerGuid));
        if (Player* pPlayer = ObjectAccessor::FindPlayer(playerGuid))
        {
            switch (_valid)
            {
                case 1:
                    pPlayer->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_FAILED);
                    break;
                case 2:
                    pPlayer->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_LOAD_FAILED);
                    break;
                case 3:
                    if (Group* grp = pPlayer->GetGroup())
                    {
                        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
                            if (Player* pMember = itr->getSource())
                                pMember->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_GROUP_FAILED, pPlayer->GetName());
                    }
                    else
                        pPlayer->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_GROUP_FAILED, pPlayer->GetName());
                    break;
                default:
                    break;
            }
        }
    }
}

void InterRealmSession::Handle_RegisterRatedResp(WorldPacket& recvPacket)
{

}

void InterRealmSession::Handle_RegisterSpectatorResp(WorldPacket& packet)
{
    uint64 playerGuid, targetGuid;
    uint32 bgZoneId;
    uint8 answer;

    packet >> playerGuid;
    packet >> targetGuid;
    packet >> answer;
    packet >> bgZoneId;

    ASSERT(bgZoneId);

    Player* pPlayer = sObjectAccessor->FindPlayer(playerGuid);
    if (!pPlayer)
        return;

    if (answer == IR_REG_SPEC_CANNOT_FIND)
    {
        ChatHandler(pPlayer).PSendSysMessage("Target was not found in arena.");
        return;
    }
    else if (answer != IR_REG_OK)
    {
        ChatHandler(pPlayer).PSendSysMessage("Cannot register a spectator.");
        return;
    }

    pPlayer->SetBattlegroundEntryPoint();

    pPlayer->TradeCancel(false);
    pPlayer->DuelComplete(DUEL_INTERRUPTED);

    pPlayer->SaveToDB();

    if (pPlayer->GetMap())
        pPlayer->GetMap()->RemovePlayerFromMap(pPlayer, false);     

    pPlayer->GetSession()->SetInterRealmBG(bgZoneId);

    WorldPacket pckt(IR_CMSG_SPECTATOR_PORT, 8 + 8);
    pckt << uint64(playerGuid);
    pckt << uint64(targetGuid);
    SendPacket(&pckt);
}

void InterRealmSession::Handle_UnRegisterPlayerResp(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Received a packet IR_SMSG_UNREGISTER_PLAYER_RESP");
    
    uint8 reason;
    uint64 playerGuid;

    recvPacket >> reason; // = 2 if cannot load player
    recvPacket >> playerGuid;

    if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
        pPlayer->GetSession()->SetInterRealmBG(0);
}

void InterRealmSession::Handle_BattlefieldPortResp(WorldPacket& packet)
{
    //sLog->outInterRealm("[INTERREALM] Received a packet IR_SMSG_BATTLEFIELD_PORT_RESP");

    uint8 _valid;
    uint64 _playerGuid;
    uint32 _instanceId, bgZoneId;
    uint16 _bgTypeId;

    packet >> _valid;
    packet >> _playerGuid;
    packet >> _instanceId;
    packet >> _bgTypeId;
    packet >> bgZoneId;

    ASSERT(bgZoneId);

    if (Player* pPlayer = sObjectAccessor->FindPlayer(_playerGuid))
    {
        pPlayer->TradeCancel(false);
        pPlayer->DuelComplete(DUEL_INTERRUPTED);

        if (Transport* l_Transport = pPlayer->GetTransport())
            l_Transport->RemovePassenger(pPlayer);

        if (pPlayer->GetMap())
            pPlayer->GetMap()->RemovePlayerFromMap(pPlayer, false);

        pPlayer->GetSession()->SetInterRealmBG(bgZoneId);
        pPlayer->SetInterRealmPlayerState(InterRealmPlayerState::PlayOnCross);

        WorldPacket pckt(IR_CMSG_BATTLEFIELD_ENTER, 8 + 4 + 4 + 2);
        pckt << uint64(_playerGuid);
        pckt << uint32(pPlayer->GetGUIDLow());
        pckt << uint32(_instanceId);
        pckt << uint16(_bgTypeId);
        SendPacket(&pckt);

        /// Loyalty System
        if (sWorld->getBoolConfig(CONFIG_WEB_DATABASE_ENABLE))
        {
            switch (_bgTypeId)
            {
                case MS::Battlegrounds::BattlegroundType::Arena2v2:
                case MS::Battlegrounds::BattlegroundType::Arena3v3:
                case MS::Battlegrounds::BattlegroundType::Arena5v5:
                case MS::Battlegrounds::BattlegroundType::ArenaSkirmish2v2:
                case MS::Battlegrounds::BattlegroundType::ArenaSkirmish3v3:
                case MS::Battlegrounds::BattlegroundType::TigersPeaks:
                case MS::Battlegrounds::BattlegroundType::TolvironArena:
                case MS::Battlegrounds::BattlegroundType::BladeEdgeArena:
                case MS::Battlegrounds::BattlegroundType::DalaranArena:
                case MS::Battlegrounds::BattlegroundType::RuinsOfLordaeron:
                case MS::Battlegrounds::BattlegroundType::NagrandArena:
                    pPlayer->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::Arena);
                    break;
                default:
                    pPlayer->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::Battleground);
                    break;
            }
        }
    }
}

void InterRealmSession::SendPlayerTeleport(Player *player, uint32 zoneId, Player *target, bool isSpectator /* = false */)
{
    player->SetBattlegroundEntryPoint();

    WorldPacket data(IR_CMSG_TELEPORT_PLAYER, 8 + 8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 + NUM_ACCOUNT_DATA_TYPES * 1
                     + 10 + strlen(player->GetName()) + 1 + 1 + 1 + 4 + 4 + 1 + 4 + 1 + 8);

    data << uint64(player->GetGUID());
    data << uint64(target->GetGUID());
    data << uint32(player->GetSession()->GetAccountId());
    data << uint8(player->GetSession()->GetSecurity());
    data << uint8(player->GetSession()->IsPremium());
    data << uint8(player->GetSession()->Expansion());
    data << uint64(player->GetSession()->m_muteTime);
    data << uint8(player->GetSession()->GetSessionDbLocaleIndex());
    data << uint8(player->GetSession()->GetRecruiterId());
    data << uint8(isSpectator ? 1 : 0);

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = player->GetSession()->GetAccountData(AccountDataType(i));

        data << uint32(pData->Data.size());
        data.WriteString(pData->Data);
        data << pData->Time;
    }

    data << std::string(player->GetName());

    data << uint8(player->getLevel()); // Level
    data << uint8(player->getRace()); // Race
    data << uint8(player->getClass()); // Class
    data << uint32(player->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID)); // Skin, Face, Hairstyle, Haircolor
    data << uint32(player->GetUInt32Value(PLAYER_FIELD_REST_STATE)); // FacialHair
    data << uint8(player->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); // Gender
    data << uint32(player->GetGuildId());
    data << uint8(player->GetRandomWinner() ? 1 : 0); // Has random winner

    player->TradeCancel(false);
    player->DuelComplete(DUEL_INTERRUPTED);

    if (player->GetMap())
        player->GetMap()->RemovePlayerFromMap(player, false);

    player->GetSession()->SetInterRealmBG(zoneId);
    SendPacket(&data);
}

void InterRealmSession::Handle_BattlefieldLeave(WorldPacket& p_Packet)
{
    //sLog->outInterRealm(LOG_FILTER_GENERAL, "[INTERREALM] Received a packet IR_SMSG_BATTLEFIELD_LEAVE");

    uint64 l_PlayerGuid;
    p_Packet >> l_PlayerGuid;

    if (Player* l_Player = sObjectAccessor->FindPlayerInOrOutOfWorld(l_PlayerGuid))
    {
        /// Logout player ...
        WorldSession* l_Session = l_Player->GetSession();
        l_Session->SetBackFromCross(true);
        l_Session->SaveSpecialChannels();

        l_Session->LogoutPlayer(false, true);
        l_Player = nullptr;

        /// Then login again
        l_Session->LoginPlayer(l_PlayerGuid);
    }
}

void InterRealmSession::Handle_BGQueueAnnouncer(WorldPacket& packet)
{
    uint8 toPlayer;
    std::string bgName;
    uint32 q_min_level, q_max_level;
    uint32 qAlliance, qAllianceDiff, qHorde, qHordeDiff;

    packet >> bgName;
    packet >> q_min_level >> q_max_level;
    packet >> qAlliance >> qAllianceDiff;
    packet >> qHorde >> qHordeDiff;
    packet >> toPlayer;
    
    
    if (toPlayer == 1)
    {
        uint64 playerGuid;
        packet >> playerGuid;

        if (Player* pPlayer = sObjectAccessor->FindPlayer(playerGuid))
            ChatHandler(pPlayer).PSendSysMessage(LANG_BG_QUEUE_ANNOUNCE_SELF, bgName.c_str(), q_min_level, q_max_level,
                qAlliance, qAllianceDiff, qHorde, qHordeDiff);      
    }
    else
        sWorld->SendWorldText(LANG_BG_QUEUE_ANNOUNCE_WORLD, bgName.c_str(), q_min_level, q_max_level,
            qAlliance, qAllianceDiff, qHorde, qHordeDiff);
                
}

void InterRealmSession::Handle_ArenaQueueAnnouncer(WorldPacket& packet)
{
    uint32 arenaType, teamRating;

    packet >> arenaType >> teamRating;

    sWorld->SendWorldText(LANG_ARENA_QUEUE_ANNOUNCE_WORLD_JOIN, arenaType, arenaType, teamRating);
}

void InterRealmSession::Handle_BattlegroundStartAnnoucer(WorldPacket& packet)
{
    std::string name;
    uint32 minLevel, maxLevel;

    packet >> name >> minLevel >> maxLevel;

    sWorld->SendWorldText(LANG_BG_STARTED_ANNOUNCE_WORLD, name.c_str(), minLevel, maxLevel);
}

void InterRealmSession::Handle_Whisper(WorldPacket& packet)
{
    uint64 senderGUID;
    std::string message, targetName;
    uint32 language;

    packet >> senderGUID >> language >> targetName >> message;

    bool sent = false;
    if (Player *receiver = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(targetName.c_str()))
    {
        if (Player *sender = ObjectAccessor::FindPlayerInOrOutOfWorld(senderGUID))
        {
            if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
            {
                if (receiver->GetTeam() != sender->GetTeam())
                    return;
            }

            sent = true;
            sender->Whisper(message, language, receiver->GetGUID());
        }
    }

    if (!sent)
    {
        WorldPacket l_Data(SMSG_CHAT_PLAYER_NOTFOUND, targetName.size() + 3);
        l_Data.WriteBits(targetName.size(), 9);
        l_Data.FlushBits();
        l_Data.WriteString(targetName);
        SendTunneledPacketToClient(senderGUID, &l_Data);
    }
}

void InterRealmSession::Handle_GuildChat(WorldPacket& packet)
{
    uint64 playerGuid;
    uint32 lang;
    std::string msg;

    packet >> playerGuid;
    packet >> lang;
    packet >> msg;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        if (player->GetGuildId())
        {
            if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
            {
                guild->BroadcastToGuild(player->GetSession(), false, msg, lang == LANG_ADDON ? LANG_ADDON : LANG_UNIVERSAL);
            }
        }
    }
            
}

void InterRealmSession::Handle_PlayerZoneID(WorldPacket& recvPacket)
{
    uint64 playerGUID;
    uint32 zoneId, areaId, mapId;

    recvPacket >> playerGUID;
    recvPacket >> zoneId;
    recvPacket >> areaId;
    recvPacket >> mapId;

    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(playerGUID))
    {
        player->SetInterRealmZoneId(zoneId);
        player->SetInterRealmAreaId(areaId);
        player->SetInterRealmMapId(mapId);
    }
}

void InterRealmSession::Handle_PlayerPInfo(WorldPacket& recvPacket)
{
    std::string playerName;
    uint64 guid;
    recvPacket >> guid >> playerName;

    Player *receiver = ObjectAccessor::FindPlayerInOrOutOfWorld(guid);
    if (!receiver)
        return;

    Player *pinfoPlayer = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(playerName.c_str());
    if (!pinfoPlayer)
    {
        SendPSysMessage(receiver, "Player not found");
        return;
    }

    /*if (!Player::HandlePinfoCommand(receiver, pinfoPlayer, pinfoPlayer->GetGUID(), playerName))
    {
        SendPSysMessage(receiver, "Player not found");
        return;
    }*/
}

void InterRealmSession::Handle_Ban(WorldPacket &recvPacket)
{
    // @todo need better way to implement this shit
    uint64 guid;
    uint8 banMode;
    std::string nameOrIP, duration, reason, gmName;

    recvPacket >> guid >> banMode >> nameOrIP >> duration >> reason >> gmName;

    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid))
    {
        ChatHandler handler(player);
        //AccountMgr::BanHelper(banMode, nameOrIP, duration, reason, gmName, &handler, player->GetSession());
    }
}

void InterRealmSession::Handle_BanCharacter(WorldPacket& recvPacket)
{
    uint64 guid;
    std::string name, duration, reason, gmName;

    recvPacket >> guid >> name >> duration >> reason >> gmName;

    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid))
    {
        ChatHandler handler(player);
        //AccountMgr::BanCharacter(name, duration, reason, gmName, &handler, player->GetSession());
    }
}

void InterRealmSession::Handle_Unban(WorldPacket& recvPacket)
{
    uint64 guid;
    uint8 banMode;
    std::string nameOrIP, gmName;

    recvPacket >> guid >> banMode >> nameOrIP >> gmName;

    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid))
    {
        ChatHandler handler(player);
        //AccountMgr::UnbanHelper(banMode, nameOrIP, &handler, player->GetSession());
    }
}

void InterRealmSession::Handle_UnbanCharacter(WorldPacket& recvPacket)
{
    uint64 guid;
    std::string name, gmName;

    recvPacket >> guid >> name >> gmName;

    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid))
    {
        ChatHandler handler(player);
        //AccountMgr::UnbanCharacter(name, &handler, player->GetSession());
    }   
}

void InterRealmSession::Handle_KickPlayer(WorldPacket& recvPacket)
{
    uint64 gmGUID;
    std::string name;
    recvPacket >> gmGUID >> name;

    // @todo need better way to implement this shit
    Player *target = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(name.c_str());
    Player *gm = ObjectAccessor::FindPlayerInOrOutOfWorld(gmGUID);

    if (!target || !gm)
    {
        SendPSysMessage(gm, "Player or gm not found. If you are gm u're probably very weird person >_>");
        return;
    }

    ChatHandler handler = ChatHandler(gm);

    if (target == gm)
    {
        SendPSysMessage(gm, handler.GetTrinityString(LANG_COMMAND_KICKSELF));
        return;
    }

    // check online security
    if (handler.HasLowerSecurity(target, 0))
        return;

    if (sWorld->getBoolConfig(CONFIG_SHOW_KICK_IN_WORLD))
    {
        SendServerAnnounce(gm->GetGUID(), handler.GetTrinityString(LANG_COMMAND_KICKMESSAGE));
        sWorld->SendWorldText(LANG_COMMAND_KICKMESSAGE, target->GetName());
    }
    else
    {
        SendPSysMessage(gm, handler.GetTrinityString(LANG_COMMAND_KICKMESSAGE), target->GetName());
    }

    target->GetSession()->KickPlayer();
}

void InterRealmSession::Handle_BattlegroundHolidayResp(WorldPacket& recvPacket)
{
    /*uint8 num, state;
    uint32 bgTypeId;

    recvPacket >> num;

    if (num > 0)
    {
        BattlegroundHolidayContainer& holidays = sBattlegroundMgr->GetBattlegroundHolidays();

        holidays.clear();

        for (uint8 i = 0; i < num; ++i)
        {
            recvPacket >> bgTypeId;
            recvPacket >> state;

            holidays[(BattlegroundTypeId)bgTypeId] = (bool)state;
        }
    }*/
}

void InterRealmSession::Handle_SummonPlayer(WorldPacket& recvPacket)
{
    uint64 senderGUID;
    std::string playerName;

    recvPacket >> senderGUID >> playerName;

    if (auto teleportTarget = ObjectAccessor::FindPlayerInOrOutOfWorld(senderGUID))
    {
        if (auto player = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(playerName.c_str()))
            SendPlayerTeleport(player, teleportTarget->GetInterRealmZoneId(), teleportTarget);
        else
            SendPSysMessage(teleportTarget, "Player not found");
    }
}

void InterRealmSession::Handle_SpectatorData(WorldPacket& recvPacket)
{
    /*uint16 teamsCount;
    uint64 portplayerGUID;
    std::string specData;

    recvPacket >> teamsCount;

    sBattlegroundMgr->ClearSpectatorData();

    if (teamsCount > 0)
    {
        for (uint16 i = 0; i < teamsCount; ++i)
        {
            recvPacket >> portplayerGUID >> specData;
            sBattlegroundMgr->AddSpectatorData(portplayerGUID, specData);
        }
    }*/
}

void InterRealmSession::Handle_DistributeArenaPointsResp(WorldPacket& recvPacket)
{
    /*sLog->outError(LOG_FILTER_INTERREALM, "Received IR_SMSG_DISTRIBUTE_ARENA_POINTS_RESP.");

    uint32 count;
    
    recvPacket >> count;

    if (count > 0)
    {
        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_START);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_ONLINE_START);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        uint32 realCount = 0;

        for (uint32 i = 0; i < count; ++i)
        {
            uint64 guid;
            uint32 arenaPoints;

            recvPacket >> guid >> arenaPoints;

            if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(guid))
            {
                // skip
                if (player->GetSession()->GetInterRealmBG())
                {
                    continue;
                }

                player->ModifyArenaPoints(arenaPoints, &trans);

                realCount++;
            }
            else
            {
                trans->PAppend("UPDATE characters SET arenaPoints=arenaPoints+%u WHERE guid=%u", arenaPoints, GUID_LOPART(guid));
            
                realCount++;
            }
        }

        CharacterDatabase.CommitTransaction(trans);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_ONLINE_END);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_TEAM_START);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_TEAM_END);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_END);
    }*/
}

void InterRealmSession::Handle_AchievementReward(WorldPacket& recvPacket)
{
    uint64 playerGuid;
    uint32 achievement;

    recvPacket >> playerGuid;
    recvPacket >> achievement;

    Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid);
    if (!player)
        return;

    AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(achievement);
    if (!achievementEntry)
        return;

    AchievementReward const* reward = sAchievementMgr->GetAchievementReward(achievementEntry);
    if (!reward)
        return;

    if (reward->sender)
    {
        Item* item = reward->itemId ? Item::CreateItem(reward->itemId, 1, player) : NULL;

        int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

        // subject and text
        std::string subject = reward->subject;
        std::string text = reward->text;
        if (loc_idx >= 0)
        {
            if (AchievementRewardLocale const* loc = sAchievementMgr->GetAchievementRewardLocale(achievementEntry))
            {
                ObjectMgr::GetLocaleString(loc->subject, loc_idx, subject);
                ObjectMgr::GetLocaleString(loc->text, loc_idx, text);
            }
        }

        MailDraft draft(subject, text);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        if (item)
        {
            // save new item before send
            item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted

            // item
            draft.AddItem(item);
        }

        draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, reward->sender));
        CharacterDatabase.CommitTransaction(trans);
    }
}

void InterRealmSession::Handle_AnticheatReport(WorldPacket& recvPacket)
{
    uint32 guidLow;
    uint8 reportType;

    recvPacket >> guidLow >> reportType;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(MAKE_NEW_GUID(guidLow, 0, HIGHGUID_PLAYER)))
    {
        sAnticheatMgr->BuildReport(player, reportType);
    }
}

void InterRealmSession::Handle_AppearRequestResp(WorldPacket& recvPacket)
{
    uint64 playerGuid, targetGuid;
    uint32 bgZoneId;
    uint8 answer;

    recvPacket >> playerGuid;
    recvPacket >> targetGuid;
    recvPacket >> answer;
    recvPacket >> bgZoneId;

    ASSERT(bgZoneId);

    Player* pPlayer = sObjectAccessor->FindPlayer(playerGuid);
    if (!pPlayer)
        return;

    pPlayer->SetBattlegroundEntryPoint();

    pPlayer->TradeCancel(false);
    pPlayer->DuelComplete(DUEL_INTERRUPTED);

    pPlayer->SaveToDB();

    if (pPlayer->GetMap())
        pPlayer->GetMap()->RemovePlayerFromMap(pPlayer, false);     

    pPlayer->GetSession()->SetInterRealmBG(bgZoneId);

    WorldPacket pckt(IR_CMSG_APPEAR, 8 + 8);
    pckt << uint64(playerGuid);
    pckt << uint64(targetGuid);
    SendPacket(&pckt);
}

void InterRealmSession::Handle_AdditionalInfo(WorldPacket& recvPacket)
{
    uint64 playerGuid;
    recvPacket >> playerGuid;

    Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid);
    if (!player)
        return;
    

    WorldPacket packet(IR_CMSG_ADDITIONAL_INFO);
    packet << playerGuid;
    packet << uint8(1); // count ?
    packet << std::string(player->GetSession()->GetRemoteAddress());
    SendPacket(&packet);
}

void InterRealmSession::Handle_GuildQuery(WorldPacket& recvPacket)
{
    sLog->outInfo(LOG_FILTER_INTERREALM, "Received guild query for new guilds.");

    uint64 guildGuid = 0;

    recvPacket >> guildGuid;

    SendGuild(guildGuid);
}

void InterRealmSession::Handle_ReserveLocalGuid(WorldPacket& p_Packet)
{
    HighGuid l_HighGuid = (HighGuid)p_Packet.read<uint32>();
    uint32   l_Size     = p_Packet.read<uint32>();

    WorldPacket l_Data(IR_CMSG_RESERVE_LOCAL_GUID);
    l_Data << uint32(l_HighGuid);
    l_Data << uint32(sObjectMgr->GenerateLowGuid(l_HighGuid, l_Size));
    l_Data << uint32(l_Size);
    SendPacket(&l_Data);
}

void InterRealmSession::Handle_CrossPartyInfo(WorldPacket& p_Packet)
{
    CrossPartyInfo l_PartyInfo;
    l_PartyInfo.PlayerGuid = p_Packet.read<uint64>();
    l_PartyInfo.PartyFlags = p_Packet.read<uint8>();
    l_PartyInfo.PartyIndex = p_Packet.read<uint8>();
    l_PartyInfo.PartyType  = p_Packet.read<uint8>();
    l_PartyInfo.GroupGUID  = p_Packet.read<uint64>();
    l_PartyInfo.LeaderGUID = p_Packet.read<uint64>();

    Player* l_Player = ObjectAccessor::FindPlayerInOrOutOfWorld(l_PartyInfo.PlayerGuid);
    if (l_Player == nullptr)
        return;

    l_Player->GetSession()->SetCrossPartyInfo(l_PartyInfo);
}

void InterRealmSession::Handle_PlayerReconnectResult(WorldPacket& p_Packet)
{
    uint64 l_PlayerGuid = p_Packet.read<uint64>();
    bool   l_Result     = p_Packet.read<uint8>();

    Player* l_Player = ObjectAccessor::FindPlayerInOrOutOfWorld(l_PlayerGuid);
    if (l_Player == nullptr)
        return;

    /// Player can't go back into the battleground / arena
    if (!l_Result)
    {
        l_Player->SetBattlegroundId(0, BattlegroundTypeId::BATTLEGROUND_TYPE_NONE);
        return;
    }

    l_Player->SetInterRealmPlayerState(InterRealmPlayerState::InTransfer);

    uint32 l_AccountID = l_Player->GetSession()->GetAccountId();

    l_Player->SaveToDB(false, std::make_shared<MS::Utilities::Callback>([l_AccountID, this](bool p_Success) -> void
    {
        WorldSession* l_Session = sWorld->FindSession(l_AccountID);
        if (l_Session == nullptr)
            return;

        Player* l_Player = l_Session->GetPlayer();

        if (InterRealmSession* l_Tunnel = sWorld->GetInterRealmSession())
        {
            WorldPacket l_Data(IR_CMSG_PLAYER_RECONNECT_READY_TO_LOAD);

            /// Build player block
            {
                l_Data << uint64(l_Player->GetGUID());
                l_Data << uint32(l_Player->GetSession()->GetAccountId());
                l_Data << uint8(l_Player->GetSession()->GetSecurity());
                l_Data << uint8(l_Player->GetSession()->IsPremium());
                l_Data << uint8(l_Player->GetSession()->Expansion());
                l_Data << uint64(l_Player->GetSession()->m_muteTime);
                l_Data << uint8(l_Player->GetSession()->GetSessionDbLocaleIndex());
                l_Data << uint8(l_Player->GetSession()->GetRecruiterId());

                for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
                {
                    AccountData* pData = l_Player->GetSession()->GetAccountData(AccountDataType(i));

                    l_Data << uint32(pData->Data.size());
                    l_Data.WriteString(pData->Data);
                    l_Data << pData->Time;
                }

                l_Data << std::string(l_Player->GetName());

                l_Data << uint8(l_Player->getLevel());
                l_Data << uint8(l_Player->getRace());
                l_Data << uint8(l_Player->getClass());
                l_Data << uint32(l_Player->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID));                            ///< Skin, Face, Hairstyle, Haircolor
                l_Data << uint32(l_Player->GetUInt32Value(PLAYER_FIELD_REST_STATE));                               ///< FacialHair
                l_Data << uint8(l_Player->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); ///< Gender
                l_Data << uint32(l_Player->GetGuildId());
                l_Data << uint8(l_Player->GetRank());
                l_Data << uint32(l_Player->GetGuildLevel());
                l_Data << uint8(l_Player->GetRandomWinner() ? 1 : 0);

                BuildPlayerArenaInfoBlock(l_Player, l_Data);
            }

            l_Data << uint32(l_Player->GetBattlegroundId());
            l_Data << uint32(l_Player->GetBattlegroundTypeId());
            l_Tunnel->SendPacket(&l_Data);
        }
    }));
}

void InterRealmSession::SendWhisper(uint64 sender, uint64 receiver, const std::string& text, uint32 language)
{
    /*Player* pSender = ObjectAccessor::FindPlayer(sender);
    Player* pReceiver = ObjectAccessor::FindPlayerInOrOutOfWorld(receiver);

    bool isAddonMessage = language == LANG_ADDON;

    if (!isAddonMessage) // if not addon data
        language = LANG_UNIVERSAL; // whispers should always be readable

    std::string _text(text);

    if (!pReceiver->isAcceptWhispers() && !pSender->isGameMaster())
    {
        ChatHandler(pSender->GetSession()).SendSysMessage(LANG_COMMAND_WHISPEROFF);
        return;
    }

    WorldPacket pckt(IR_MSG_WHISPER);
    pckt << uint64(sender);
    pckt << uint64(receiver);
    pckt << uint32(language);
    pckt << uint8(pSender->GetChatTag());
    pckt << text;
    SendPacket(&pckt); 

    // rest stuff shouldn't happen in case of addon message
    if (isAddonMessage)
        return;

    pckt.Initialize(SMSG_MESSAGECHAT, 200);
    //pReceiver->BuildPlayerChat(&pckt, CHAT_MSG_WHISPER_INFORM, _text, language);

	pckt << uint8(CHAT_MSG_WHISPER_INFORM);
    pckt << uint32(language);
    pckt << uint64(pReceiver->GetGUID());
    pckt << uint32(0);                                      // constant unknown time
    pckt << uint64(pReceiver->GetGUID());
    pckt << uint32(_text.length() + 1);
    pckt << _text;
    pckt << uint8(pReceiver->GetChatTag());

    pSender->GetSession()->SendPacket(&pckt);

    if (!pSender->isAcceptWhispers() && !pSender->isGameMaster() && !pReceiver->isGameMaster())
    {
        pSender->SetAcceptWhispers(true);
        ChatHandler(pSender->GetSession()).SendSysMessage(LANG_COMMAND_WHISPERON);
    }*/
}

void InterRealmSession::SendBattlegroundHolidaysQuery()
{
    WorldPacket packet(IR_CMSG_BATTLEGROUND_HOLIDAY);

    SendPacket(&packet);
}

void InterRealmSession::BuildPlayerArenaInfoBlock(Player* player, ByteBuffer& packet)
{
    for (uint8 i = 0; i < MAX_PVP_SLOT; ++i)
    {
        packet << player->GetArenaPersonalRating(i);
        packet << player->GetBestRatingOfWeek(i);
        packet << player->GetBestRatingOfSeason(i);
        packet << player->GetArenaMatchMakerRating(i);
        packet << player->GetWeekGames(i);
        packet << player->GetWeekWins(i);
        packet << player->GetPrevWeekWins(i);
        packet << player->GetSeasonGames(i);
        packet << player->GetSeasonWins(i);
    }
}

void InterRealmSession::SendRegisterPlayer(Player* player, uint32 bgInstanceId, uint64 bgGuid, uint32 bgTypeId, uint8 role, uint32* blacklist)
{
    WorldPacket pckt(IR_CMSG_REGISTER_PLAYER, 8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 + NUM_ACCOUNT_DATA_TYPES * 1
                         + 10 + strlen(player->GetName()) + 1 + 1 + 1 + 4 + 4 + 1 + 1 + 4 + 8);

    pckt << uint64(player->GetGUID());
    pckt << uint32(player->GetSession()->GetAccountId());
    pckt << uint8(player->GetSession()->GetSecurity());
    pckt << uint8(player->GetSession()->IsPremium());
    pckt << uint8(player->GetSession()->Expansion());
    pckt << uint64(player->GetSession()->m_muteTime);
    pckt << uint8(player->GetSession()->GetSessionDbLocaleIndex());
    pckt << uint8(player->GetSession()->GetRecruiterId());

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = player->GetSession()->GetAccountData(AccountDataType(i));

        pckt << uint32(pData->Data.size());
        pckt.WriteString(pData->Data);
        pckt << pData->Time;
    }

    pckt << std::string(player->GetName());

    pckt << uint8(player->getLevel()); // Level
    pckt << uint8(player->getRace()); // Race
    pckt << uint8(player->getClass()); // Class
    pckt << uint32(player->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID)); // Skin, Face, Hairstyle, Haircolor
    pckt << uint32(player->GetUInt32Value(PLAYER_FIELD_REST_STATE)); // FacialHair
    pckt << uint8(player->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); // Gender
    pckt << uint32(player->GetGuildId());
    pckt << uint8(player->GetRank());
    pckt << uint32(player->GetGuildLevel());
    
    pckt << uint8(player->GetRandomWinner() ? 1 : 0); // Has random winner

    BuildPlayerArenaInfoBlock(player, pckt);

    pckt << uint32(bgInstanceId); // bg instance id
    pckt << uint64(bgGuid); // bg guid
    pckt << uint32(bgTypeId);
    pckt << uint8(role);

    for (int i = 0; i < 2; i++)
        pckt << blacklist[i];

    SendPacket(&pckt);
}

void InterRealmSession::SendRegisterGroup(Group* group, uint32 bgInstanceId, uint64 bgGuid, uint32 bgTypeId, uint8 role, uint32* blacklist)
{
    WorldPacket pckt(IR_CMSG_REGISTER_GROUP, (8 + 4 + 8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 +
            10 + 1 + 1 + 1 + 4 + 4 + 1 + 1));

    ByteBuffer data;

    size_t num_pos = pckt.wpos();
    pckt << uint8(0);
    pckt << uint32(bgInstanceId); // bg instance id
    pckt << uint64(bgGuid); // bg guid
    pckt << uint32(bgTypeId);
    pckt << uint8(role);

    for (int i = 0; i < 2; i++)
        pckt << uint32(blacklist[i]);

    uint8 count = 0;
    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member || !member->IsInWorld())
            continue;

        pckt << uint64(member->GetGUID());

        count++;

        uint64 _mute;
        uint32 _accountId;
        uint8 _security, _premium, _exp, _locale, _recruiter;

        _accountId = member->GetSession()->GetAccountId();
        _security = member->GetSession()->GetSecurity();
        _premium = member->GetSession()->IsPremium();
        _exp = member->GetSession()->Expansion();
        _mute = member->GetSession()->m_muteTime;
        _locale = member->GetSession()->GetSessionDbLocaleIndex();
        _recruiter = member->GetSession()->GetRecruiterId();

        data << uint32(_accountId);
        data << uint8(_security);
        data << uint8(_premium);
        data << uint8(_exp);
        data << uint64(_mute);
        data << uint8(_locale);
        data << uint8(_recruiter);

        for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        {
            AccountData* pData = member->GetSession()->GetAccountData(AccountDataType(i));

            data << uint32(pData->Data.size());
            data.WriteString(pData->Data);
            data << pData->Time;
        }

        data << std::string(member->GetName());

        data << uint8(member->getLevel()); // Level
        data << uint8(member->getRace()); // Race
        data << uint8(member->getClass()); // Class
        data << uint32(member->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID)); // Skin, Face, Hairstyle, Haircolor
        data << uint32(member->GetUInt32Value(PLAYER_FIELD_REST_STATE)); // FacialHair
        data << uint8(member->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); // Gender
        data << uint32(member->GetGuildId());
        data << uint8(member->GetRank());
        data << uint32(member->GetGuildLevel());

        data << uint8(member->GetRandomWinner() ? 1 : 0); // Has random winner

        BuildPlayerArenaInfoBlock(member, data);
    }

    pckt.put<uint8>(num_pos, count);
    pckt.append(data);

    SendPacket(&pckt);
}

void InterRealmSession::SendRegisterArena(std::list<Player*> p_Players, uint32 p_ArenaSlot, bool p_IsSkirmish)
{
    WorldPacket l_Pckt(IR_CMSG_REGISTER_ARENA, 1 + 4 + (8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 +
        10 + 1 + 1 + 1 + 4 + 4 + 1 + 1));
    
    ByteBuffer l_Data;

    size_t l_NumPos = l_Data.wpos();
    l_Pckt << uint8(0);
    l_Pckt << uint8(p_ArenaSlot);
    l_Pckt << uint8(p_IsSkirmish);

    uint8 l_Count = 0;
    for (Player* l_Player : p_Players)
    {
        if (!l_Player || !l_Player->IsInWorld())
            continue;

        l_Count++;

        l_Pckt << uint64(l_Player->GetGUID());

        uint32 l_AccountId  = l_Player->GetSession()->GetAccountId();
        uint8  l_Security   = l_Player->GetSession()->GetSecurity();
        uint8  l_Premium    = l_Player->GetSession()->IsPremium();
        uint8  l_Exp        = l_Player->GetSession()->Expansion();
        uint64 l_Mute       = l_Player->GetSession()->m_muteTime;
        uint8  l_Locale     = l_Player->GetSession()->GetSessionDbLocaleIndex();
        uint8  l_Recruiter  = l_Player->GetSession()->GetRecruiterId();

        l_Data << uint32(l_AccountId);
        l_Data << uint8(l_Security);
        l_Data << uint8(l_Premium);
        l_Data << uint8(l_Exp);
        l_Data << uint64(l_Mute);
        l_Data << uint8(l_Locale);
        l_Data << uint8(l_Recruiter);

        for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        {
            AccountData* pData = l_Player->GetSession()->GetAccountData(AccountDataType(i));

            l_Data << uint32(pData->Data.size());
            l_Data.WriteString(pData->Data);
            l_Data << pData->Time;
        }

        l_Data << std::string(l_Player->GetName());

        l_Data << uint8(l_Player->getLevel()); // Level
        l_Data << uint8(l_Player->getRace()); // Race
        l_Data << uint8(l_Player->getClass()); // Class
        l_Data << uint32(l_Player->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID)); // Skin, Face, Hairstyle, Haircolor
        l_Data << uint32(l_Player->GetUInt32Value(PLAYER_FIELD_REST_STATE)); // FacialHair
        l_Data << uint8(l_Player->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); // Gender
        l_Data << uint32(l_Player->GetGuildId());
        l_Data << uint8(l_Player->GetRank());
        l_Data << uint32(l_Player->GetGuildLevel());

        l_Data << uint8(l_Player->GetRandomWinner() ? 1 : 0); // Has random winner

        BuildPlayerArenaInfoBlock(l_Player, l_Data);
    }

    l_Pckt.put<uint8>(l_NumPos, l_Count);
    l_Pckt.append(l_Data);

    SendPacket(&l_Pckt);
}

void InterRealmSession::SendRegisterRated(Group* group, uint32 personalRating, uint32 matchmakerRating)
{
    WorldPacket pckt(IR_CMSG_REGISTER_RATED, 1 + 4 * (8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 +
        10 + 1 + 1 + 1 + 4 + 4 + 1 + 1));

    ByteBuffer data;

    size_t num_pos = pckt.wpos();
    pckt << uint8(0);
    pckt << uint32(personalRating);
    pckt << uint32(matchmakerRating);

    uint8 count = 0;
    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member || !member->IsInWorld())
            continue;

        pckt << uint64(member->GetGUID());

        count++;

        uint64 _mute;
        uint32 _accountId;
        uint8 _security, _premium, _exp, _locale, _recruiter;

        _accountId = member->GetSession()->GetAccountId();
        _security = member->GetSession()->GetSecurity();
        _premium = member->GetSession()->IsPremium();
        _exp = member->GetSession()->Expansion();
        _mute = member->GetSession()->m_muteTime;
        _locale = member->GetSession()->GetSessionDbLocaleIndex();
        _recruiter = member->GetSession()->GetRecruiterId();

        data << uint32(_accountId);
        data << uint8(_security);
        data << uint8(_premium);
        data << uint8(_exp);
        data << uint64(_mute);
        data << uint8(_locale);
        data << uint8(_recruiter);

        for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        {
            AccountData* pData = member->GetSession()->GetAccountData(AccountDataType(i));

            data << uint32(pData->Data.size());
            data.WriteString(pData->Data);
            data << pData->Time;
        }

        data << std::string(member->GetName());

        data << uint8(member->getLevel()); // Level
        data << uint8(member->getRace()); // Race
        data << uint8(member->getClass()); // Class
        data << uint32(member->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID)); // Skin, Face, Hairstyle, Haircolor
        data << uint32(member->GetUInt32Value(PLAYER_FIELD_REST_STATE)); // FacialHair
        data << uint8(member->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); // Gender
        data << uint32(member->GetGuildId());
        data << uint8(member->GetRank());
        data << uint32(member->GetGuildLevel());

        data << uint8(member->GetRandomWinner() ? 1 : 0); // Has random winner

        BuildPlayerArenaInfoBlock(member, data);
    }

    pckt.put<uint8>(num_pos, count);
    pckt.append(data);

    SendPacket(&pckt);
}

void InterRealmSession::SendRegisterSpectator(Player* player, uint64 targetGuid)
{
    WorldPacket packet(IR_CMSG_REGISTER_SPECTATOR);

    packet << uint64(player->GetGUID());
    packet << uint64(targetGuid);
    packet << uint32(player->GetSession()->GetAccountId());
    packet << uint8(player->GetSession()->GetSecurity());
    packet << uint8(player->GetSession()->IsPremium());
    packet << uint8(player->GetSession()->Expansion());
    packet << uint64(player->GetSession()->m_muteTime);
    packet << uint8(player->GetSession()->GetSessionDbLocaleIndex());
    packet << uint8(player->GetSession()->GetRecruiterId());

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = player->GetSession()->GetAccountData(AccountDataType(i));

        packet << uint32(pData->Data.size());
        packet.WriteString(pData->Data);
        packet << pData->Time;
    }

    packet << std::string(player->GetName());

    packet << uint8(player->getLevel()); // Level
    packet << uint8(player->getRace()); // Race
    packet << uint8(player->getClass()); // Class
    packet << uint32(player->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID)); // Skin, Face, Hairstyle, Haircolor
    packet << uint32(player->GetUInt32Value(PLAYER_FIELD_REST_STATE)); // FacialHair
    packet << uint8(player->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); // Gender
    packet << uint32(player->GetGuildId());
    packet << uint8(player->GetRank());
    packet << uint32(player->GetGuildLevel());
    packet << uint8(player->GetRandomWinner() ? 1 : 0); // Has random winner

    SendPacket(&packet);
}

void InterRealmSession::SendCurrencyReset()
{
    WorldPacket packet(IR_CMSG_CURRENCY_CAP_RESET);
    SendPacket(&packet);
}

void InterRealmSession::SendSummonPlayer(uint64 playerGuid)
{
    WorldPacket data(IR_CMSG_SUMMON_PLAYER);
    data << playerGuid;
    SendPacket(&data);
}

void InterRealmSession::SendAppearRequest(Player* sender, uint64 targetGuid)
{
    WorldPacket data(IR_CMSG_APPEAR_REQUEST);
    data << sender->GetGUID();
    data << targetGuid;

    data << uint32(sender->GetSession()->GetAccountId());
    data << uint8(sender->GetSession()->GetSecurity());
    data << uint8(sender->GetSession()->IsPremium());
    data << uint8(sender->GetSession()->Expansion());
    data << uint64(sender->GetSession()->m_muteTime);
    data << uint8(sender->GetSession()->GetSessionDbLocaleIndex());
    data << uint8(sender->GetSession()->GetRecruiterId());

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = sender->GetSession()->GetAccountData(AccountDataType(i));

        data << uint32(pData->Data.size());
        data.WriteString(pData->Data);
        data << pData->Time;
    }

    data << std::string(sender->GetName());

    data << uint8(sender->getLevel()); // Level
    data << uint8(sender->getRace()); // Race
    data << uint8(sender->getClass()); // Class
    data << uint32(sender->GetUInt32Value(PLAYER_FIELD_HAIR_COLOR_ID)); // Skin, Face, Hairstyle, Haircolor
    data << uint32(sender->GetUInt32Value(PLAYER_FIELD_REST_STATE)); // FacialHair
    data << uint8(sender->GetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_GENDER)); // Gender
    data << uint32(sender->GetGuildId());
    data << uint8(sender->GetRank());
    data << uint8(sender->GetRandomWinner() ? 1 : 0); // Has random winner

    SendPacket(&data);
}

void InterRealmSession::SendPlayerReconnect(uint64 p_PlayerGuid, uint32 p_BattlegroundInstanceId, BattlegroundTypeId p_BattlegroundTypeId)
{
    WorldPacket l_Data(IR_CMSG_PLAYER_RECONNECT);
    l_Data << uint64(p_PlayerGuid);
    l_Data << uint32(p_BattlegroundInstanceId);
    l_Data << uint32(p_BattlegroundTypeId);
    SendPacket(&l_Data);
}

void InterRealmSession::SendGuild(uint64 guildGuid)
{
    WorldPacket packet(IR_CMSG_GUILD);

    Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid);
    if (!guild)
        return;

    packet << guild->GetGUID();
    packet << guild->GetName();
    packet << guild->GetMembersCount();
    packet << guild->GetEmblemInfo().GetStyle();
    packet << guild->GetEmblemInfo().GetColor();
    packet << guild->GetEmblemInfo().GetBorderStyle();
    packet << guild->GetEmblemInfo().GetBorderColor();
    packet << guild->GetEmblemInfo().GetBackgroundColor();
    packet << guild->GetAchievementMgr().GetAchievementPoints();

    Guild::Ranks const* ranks = guild->GetRanks();

    packet << uint32(ranks->size());

    for (Guild::Ranks::const_iterator itr = ranks->begin(); itr != ranks->end(); ++itr)
    {
        packet << itr->GetId();
        packet << itr->GetName();
    }

    SendPacket(&packet);
}

void InterRealmSession::AddPacket(WorldPacket* new_packet)
{
    _queue.add(new_packet);
}

void InterRealmSession::Update(const uint32 diff)
{
    if (m_needProcessDisconnect)
        ProcessPlayersAfterDisconnect();

    // Process packets
    WorldPacket* packet = NULL;

    /*if (sBattlegroundMgr->HaveSpectatorData() && (!m_IRSocket || m_IRSocket && m_IRSocket->IsClosed()))
        sBattlegroundMgr->ClearSpectatorData();*/

    while (m_IRSocket && !m_IRSocket->IsClosed() && !_queue.empty() && _queue.next(packet))
    {
        // Handle Packet
        if (packet->GetOpcode() < IR_NUM_MSG_TYPES)
        {
            try
            {
                IROpcodeHandler* IRopHandle = IRopcodeTable[packet->GetOpcode()];
                if (!IRopHandle)
                {
                    sLog->outError(LOG_FILTER_INTERREALM, "Cannot find handle for the opcode (%u). Skipped packet.",
                    packet->GetOpcode());
                    continue;
                }
                (this->*(IRopHandle->IRhandler))(*packet);
            }
            catch(ByteBufferException &)
            {
                sLog->outError(LOG_FILTER_INTERREALM, "InterRealmSession ByteBufferException occured while parsing a packet (opcode: %u). Skipped packet.",
                    packet->GetOpcode());
            }
            if (packet->rpos() < packet->wpos())
            {
                sLog->outError(LOG_FILTER_INTERREALM, "Unprocessed tail data (read stop at %u from %u) in opcode %s", packet->rpos(), packet->wpos(), IRopcodeTable[packet->GetOpcode()]->name);
            }
        }
        else
            this->Handle_Unhandled(*packet);

        // Delete Packet from memory
        if (packet != NULL)
        {
            //sLog->outInterRealm(LOG_FILTER_GENERAL, "[INTERREALM] Deleting packet");
            delete packet;
        }
    }
}
#endif