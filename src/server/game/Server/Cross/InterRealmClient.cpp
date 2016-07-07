#ifdef CROSS
#include "InterRealmClient.h"
#include "InterRealmOpcodes.h"
#include "World.h"
#include "BattlegroundMgr.hpp"
#include "DisableMgr.h"
#include "Chat.h"
#include "InterRealmMgr.h"
#include "Group.h"
#include "IRSocket.h"
#include "BattlegroundPacketFactory.hpp"
#include "BattlegroundScheduler.hpp"

#define INTERREALM_CHECK_PLAYERS_TIMER 2000
#define INTERREALM_PING_MYSQL_TIMER 60000
#define INTERREALM_REMOVE_PLAYERS_TIMER 1000

InterRealmClient::InterRealmClient(IRSocket* socket):
m_isNeedClose(false), m_IRSocket(socket)
{
    m_realmId = 0;

    m_rate_honor = 1.0f;
    m_rate_honor_premium = sWorld->getRate(RATE_HONOR_PREMIUM);
    m_rate_reputation = sWorld->getRate(RATE_REPUTATION_GAIN);
    m_rate_reputation_premium = sWorld->getRate(RATE_REPUTATION_GAIN_PREMIUM);

    m_check_players_timer = INTERREALM_CHECK_PLAYERS_TIMER;
    m_ping_mysql_timer  = INTERREALM_PING_MYSQL_TIMER;

    m_LocalRealmItemGuidReserveSize          = ConfigMgr::GetIntDefault("InterRealm.LocalRealmItemGuidReserveSize", 200);
    m_LocalRealmItemGuidReserveMin           = ConfigMgr::GetIntDefault("InterRealm.LocalRealmItemGuidReserveMin",   80);

    m_LocalRealmMailGuidReserveSize          = ConfigMgr::GetIntDefault("InterRealm.LocalRealmMailGuidReserveSize",  30);
    m_LocalRealmMailGuidReserveMin           = ConfigMgr::GetIntDefault("InterRealm.LocalRealmMailGuidReserveMin",   10);

    m_LocalRealmPetGuidReserveSize           = ConfigMgr::GetIntDefault("InterRealm.LocalRealmPetGuidReserveSize",   50);
    m_LocalRealmPetGuidReserveMin            = ConfigMgr::GetIntDefault("InterRealm.LocalRealmPetGuidReserveMin",    10);

    m_LocalRealmEquipmentSetGuidReserveSize  = ConfigMgr::GetIntDefault("InterRealm.LocalRealmEquipmentSetGuidReserveSize", 10);
    m_LocalRealmEquipmentSetGuidReserveMin   = ConfigMgr::GetIntDefault("InterRealm.LocalRealmEquipmentSetGuidReserveMin", 3);

    m_remove_players_timer = INTERREALM_REMOVE_PLAYERS_TIMER;

    _isDatabaseOpened = false;

    if (socket)
    {
        socket->AddReference();
    }
}

InterRealmClient::~InterRealmClient()
{
    if (_isDatabaseOpened)
        try { InterRealmDatabase.Close(); } catch (...) { }

    // Delete queued packets
    WorldPacket* packet = NULL;
    while (_queue.next(packet))
    {
        delete packet;
        packet = NULL;
    }

    if (m_IRSocket)
    {
        m_IRSocket->CloseSocket();
        m_IRSocket->RemoveReference();
        m_IRSocket = NULL;
    }
}

std::string InterRealmClient::GetRealmName() const
{
    if (const InterRealmDatabaseConfig* config = sInterRealmMgr->GetConfig(m_realmId))
        return config->name;
    return "";
}

void InterRealmClient::SendTunneledPacket(uint64 playerGuid, WorldPacket const* packet, bool forced /*= false*/)
{
    if (playerGuid == 0 || packet->GetOpcode() >= UNKNOWN_OPCODE)
        return;

    const_cast<WorldPacket*>(packet)->FlushBits();

    WorldPacket tmpPacket(IR_SMSG_TUNNEL_PACKET, 8 + 2 + packet->size());
    tmpPacket << playerGuid;
    tmpPacket << packet->GetOpcode();

    if (packet->size() > 0)
        tmpPacket.append(packet->contents(), packet->size());

    SendPacket(&tmpPacket);
}

void InterRealmClient::SendPacket(WorldPacket const* packet)
{
    if (packet == NULL)
        return;

    if (!m_IRSocket || m_IRSocket->IsClosed())
        return;

    if (m_IRSocket->SendPacket(packet) == -1)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Cannot send packet %u", packet->GetOpcode());
        m_IRSocket->CloseSocket();
    }
}

void InterRealmClient::Handle_Unhandled(WorldPacket& recvPacket)
{
    sLog->outError(LOG_FILTER_INTERREALM, "Unhandled Packet with IROpcode %u received !", recvPacket.GetOpcode());
}

void InterRealmClient::Handle_Null(WorldPacket& recvPacket)
{
    recvPacket.rfinish();
    //sLog->outError(LOG_FILTER_INTERREALM, "Packet with Invalid IROpcode %u received !", recvPacket.GetOpcode());
}

void InterRealmClient::Handle_Hello(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet CMSG_HELLO.", m_realmId);
    
    std::string hello;
    uint8 _rand;

    packet >> hello;
    packet >> _rand;

    bool non_polite = false;

    if (strcmp(hello.c_str(), "HELO") != 0)
        non_polite = true;

    if (non_polite)
    {
        m_isNeedClose = true;
        sLog->outError(LOG_FILTER_INTERREALM, "Non-polite server %s, rejecting.", hello.c_str());
    }

    WorldPacket pckt(IR_SMSG_HELLO, 10 + 1 + 1);
    pckt << std::string("HELO"); // polite
    pckt << uint8(_rand);
    pckt << uint8((non_polite ? IR_HELO_RESP_POLITE : IR_HELO_RESP_OK));

    SendPacket(&pckt);

    sLog->outDebug(LOG_FILTER_INTERREALM, "Send packet SMSG_HELLO.", m_realmId);
    
}

void InterRealmClient::Handle_TunneledPacket(WorldPacket* recvPacket)
{
    uint64 playerGuid;
    uint16 opcodeId;

    *recvPacket >> playerGuid;
    *recvPacket >> opcodeId;

    recvPacket->SetTunneled(true);
    recvPacket->SetOpcode(opcodeId);

    if (IsNeedClose())
    {
        delete recvPacket;
        return;
    }

    IRPlayers::const_iterator itr = m_Players.find(playerGuid);
    if (itr == m_Players.end() || itr->second->GetSession()->IsIRClosing() || g_OpcodeTable[WOW_CLIENT_TO_SERVER][opcodeId] == nullptr)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "no player");
        delete recvPacket;
        recvPacket = NULL;
        return;
    }

    itr->second->GetSession()->QueuePacket(recvPacket);
}

void InterRealmClient::Handle_WhoAmI(WorldPacket& packet)
{
    packet >> m_realmId;
    packet >> m_rate_honor;
    packet >> m_rate_honor_premium;

    if (packet.rpos() < packet.size())
    {
        packet >> m_rate_reputation;
        packet >> m_rate_reputation_premium;
    }
    else
    {
        m_rate_reputation = sWorld->getRate(RATE_REPUTATION_GAIN);
        m_rate_reputation_premium = sWorld->getRate(RATE_REPUTATION_GAIN_PREMIUM);
    }

    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet CMSG_WHO_AM_I.", m_realmId);

    WorldPacket pckt(IR_SMSG_WHO_AM_I, 1);

    const InterRealmDatabaseConfig* config = sInterRealmMgr->GetConfig(m_realmId);
    if (!config)
    {
        sLog->outInfo(LOG_FILTER_INTERREALM, "InterRealmClient %u has no config.", m_realmId);
        pckt << uint8(1);
        SendPacket(&pckt);
        m_isNeedClose = true;
        return;
    }

    if (!InterRealmDatabase.Open(config->connection_info, 10, 8))
    {
        sLog->outInfo(LOG_FILTER_INTERREALM, "Database connection was failed for InterRealmClient %u.", m_realmId);
        pckt << uint8(1);
        SendPacket(&pckt);
        m_isNeedClose = true;
        return;
    }

    pckt << uint8(0);
    SendPacket(&pckt);
    _isDatabaseOpened = true;
    sLog->outInfo(LOG_FILTER_INTERREALM, "Database connection was successful. %lu", time(nullptr));

    SendReserveLocalGuidRequest(HIGHGUID_ITEM,          m_LocalRealmItemGuidReserveSize);
    SendReserveLocalGuidRequest(HIGHGUID_MAIL,          m_LocalRealmMailGuidReserveSize);
    SendReserveLocalGuidRequest(HIGHGUID_PET_NUMBER,    m_LocalRealmPetGuidReserveSize);
    SendReserveLocalGuidRequest(HIGHGUID_EQUIPMENT_SET, m_LocalRealmEquipmentSetGuidReserveSize);
}

void InterRealmClient::Handle_CheckPlayers(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_CHECK_PLAYERS");
    
    uint8 _valid;
    uint32 num_players;
    std::vector<uint64> playerGuids;

    packet >> _valid;

    // Check for offline players
    // 1 means it has
    if (_valid == 0)
        return;

    packet >> num_players;

    if (num_players > 0)
    {
        for (uint16 i = 0; i < num_players; ++i)
        {
            uint64 playerGuid;
            packet >> playerGuid;
            sLog->outDebug(LOG_FILTER_INTERREALM, "Connection lost with player %u", playerGuid);

            if (Player* l_Player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
            {
                if (!l_Player->PlayOnCross() || l_Player->IsBeingTeleportedFar() || l_Player->GetSession()->PlayerLoading())
                {
                    l_Player->SetNeedRemove(true);
                }
                else
                {
                    MS::Utilities::CallBackPtr l_CallBack = std::make_shared<MS::Utilities::Callback>([playerGuid](bool p_Success) -> void
                    {
                        Player* l_Player = sObjectAccessor->FindPlayer(playerGuid);
                        if (!l_Player)
                            return;

                        l_Player->SetNeedRemove(true);
                    });

                    l_Player->SaveToDB(false, l_CallBack);
                }
            }
        }
    }
}

void InterRealmClient::Handle_PlayerLogout(WorldPacket& packet)
{
    uint64 playerGuid;

    packet >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        player->GetSession()->LogoutPlayer(true);
    }

    /*uint64 playerGuid;

    packet >> playerGuid;

    sLog->outDebug(LOG_FILTER_INTERREALM, "Logout player %u", playerGuid);

    IRPlayerSessions::iterator itr = m_sessions.find(playerGuid);
    if (itr != m_sessions.end())
    {
        itr->second->SetRemoveType(REMOVE_TYPE_ALL);
    }*/
}

void InterRealmClient::Handle_DebugBG(WorldPacket& packet)
{
    uint8 val;

    packet >> val;

    sBattlegroundMgr->ToggleTesting();

    // To do: send the packet to all clients
    WorldPacket pckt(IR_SMSG_DEBUG_BG_RESP, 1);
    pckt << uint8((sBattlegroundMgr->isTesting()) ? 1 : 0);
    SendPacket(&pckt);
}

void InterRealmClient::Handle_DebugArena(WorldPacket& packet)
{
    uint8 val;

    packet >> val;

    sBattlegroundMgr->ToggleArenaTesting();

    // To do: send the packet to all clients
    WorldPacket pckt(IR_SMSG_DEBUG_ARENA_RESP, 1);
    pckt << uint8((sBattlegroundMgr->isArenaTesting()) ? 1 : 0);
    SendPacket(&pckt);
}

void InterRealmClient::SendBattlefieldLeave(uint64 targetGuid)
{
    WorldPacket pckt(IR_SMSG_BATTLEFIELD_LEAVE, 8);
    pckt << uint64(targetGuid);
    SendPacket(&pckt);
}

void InterRealmClient::Handle_BattlegroundListQuery(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_BATTLEGROUND_LIST_QUERY");

    uint64 _playerGuid;
    uint32 _bgTypeId;
    uint8 _level, _winner;

    packet >> _playerGuid;
    packet >> _bgTypeId;
    packet >> _winner;
    packet >> _level;

    WorldPacket pckt;
    MS::Battlegrounds::PacketFactory::List(&pckt, _playerGuid, sObjectAccessor->FindPlayer(_playerGuid), BattlegroundTypeId(_bgTypeId), 0, false, false, false, false);

    SendTunneledPacket(_playerGuid, &pckt, true);
}

void InterRealmClient::RemovePlayerFromIR(Player *player)
{
    SendBattlefieldLeave(player->GetRealGUID());
    player->SetNeedRemove(true);
}

void InterRealmClient::ProcessWorldSessionPacket(WorldSession* _session, WorldPacket* packet)
{
    //_session->QueuePacket(packet);
}

void InterRealmClient::Handle_BattlefieldStatusQuery(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_BATTLEFIELD_STATUS_QUERY");
    
    uint64 l_PlayerGuid;
    packet >> l_PlayerGuid;

    if (Player* l_Player = sObjectAccessor->FindPlayerInOrOutOfWorld(l_PlayerGuid))
    {
        WorldPacket l_Data;

        Battleground * l_BG = nullptr;

        /// We must update all queues here
        for (uint8 l_I = 0; l_I < PLAYER_MAX_BATTLEGROUND_QUEUES; ++l_I)
        {
            MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = l_Player->GetBattlegroundQueueTypeId(l_I);

            /// Can also be random BG
            if (l_BGQueueTypeID == MS::Battlegrounds::BattlegroundType::End)
                continue;

            BattlegroundTypeId l_BGTypeId = MS::Battlegrounds::GetIdFromType(l_BGQueueTypeID);
            uint8 l_ArenaType = MS::Battlegrounds::BGArenaType(l_BGQueueTypeID);

            if (MS::Battlegrounds::GetIdFromType(l_BGQueueTypeID) == l_Player->GetBattlegroundTypeId())
            {
                l_BG = l_Player->GetBattleground();

                /// I cannot check any variable from player class because player class doesn't know if player is in 2v2 / 3v3 or 5v5 arena
                /// So i must use bg pointer to get that information
                if (l_BG && l_BG->GetArenaType() == l_ArenaType)
                {
                    /// This line is checked, i only don't know if GetElapsedTime() is changing itself after bg end!
                    /// Send status in Battleground
                    MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Player, l_I, STATUS_IN_PROGRESS, l_BG->GetExpirationDate(), l_BG->GetElapsedTime(), l_ArenaType, false);
                    l_Player->GetSession()->SendPacket(&l_Data);

                    continue;
                }
            }

            /// We are sending update to player about queue - he can be invited there!
            /// get GroupQueueInfo for queue status
            MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
            MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

            GroupQueueInfo l_GroupQueueInfo;
            if (!l_InvitationsMgr.GetPlayerGroupInfoData(l_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID))
            if (!l_Scheduler.GetPlayerGroupInfoData(l_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID))
                continue;

            if (l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID)
            {
                l_BG = sBattlegroundMgr->GetBattleground(l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID, l_BGQueueTypeID);
                if (!l_BG)
                    continue;

                /// Send status invited to Battleground
                MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Player, l_I, STATUS_WAIT_JOIN, l_BG->GetExpirationDate(), l_Player->GetBattlegroundQueueJoinTime(l_BGQueueTypeID), l_ArenaType, false);
                l_Player->GetSession()->SendPacket(&l_Data);
            }
            else
            {
                l_BG = sBattlegroundMgr->GetBattlegroundTemplate(l_BGQueueTypeID);
                if (!l_BG)
                    continue;

                /// Expected bracket entry
                MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(l_Player->getLevel());

                if (!l_BracketEntry)
                    continue;

                uint32 l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(&l_GroupQueueInfo, l_BracketEntry->m_Id);

                /// Send status in Battleground Queue
                MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Player, l_I, STATUS_WAIT_QUEUE, l_AverageTime, l_Player->GetBattlegroundQueueJoinTime(l_BGQueueTypeID), l_ArenaType, l_GroupQueueInfo.m_IsSkirmish);
                l_Player->GetSession()->SendPacket(&l_Data);
            }
        }
    }
}

void InterRealmClient::Handle_CurrencyCapLoad(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_CURRENCY_CAP_LOAD");

    uint64 playerGuid;

    packet >> playerGuid;

    WorldPacket pckt;

    /*if (IRCurrencyCap* const cap = sCurrencyMgr->GetCurrencyCapData(GUID_LOPART(playerGuid), m_realmId))
    {
        pckt.Initialize(IR_SMSG_CURRENCY_CAP_LOAD_RESP, 1 + 8 + 2 + 2 + 2 + 2 + 1);
        pckt << uint8(0);
        pckt << uint64(playerGuid);
        pckt << uint16(cap->highestArenaRating);
        pckt << uint16(cap->highestRBgRating);
        pckt << uint16(cap->currentArenaCap);
        pckt << uint16(cap->currentRBgCap);
        pckt << uint8(cap->requireReset);
    }
    else
    {
        pckt.Initialize(IR_SMSG_CURRENCY_CAP_LOAD_RESP, 1 + 8);

        pckt << uint8(1);
        pckt << uint64(playerGuid);
    }*/

    SendPacket(&pckt);
}

void InterRealmClient::Handle_CurrencyCapReset(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_CURRENCY_CAP_RESET");

    for (auto itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (itr->second)
            itr->second->ResetCurrencyWeekCap();
    }
}

void InterRealmClient::Handle_Whisper(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_MSG_WHISPER");

    uint64 l_SenderGUID;
    uint64 l_TargetGuid;
    std::string l_Text;
    uint32 l_Lang;
    uint8 tag;

    packet >> l_SenderGUID >> l_TargetGuid >> l_Lang >> tag >> l_Text;

    if (Player* l_Player = ObjectAccessor::FindPlayerInOrOutOfWorld(l_TargetGuid))
    {
        if (l_Player->GetSession()->GetInterRealmNumber() != GetRealmId())
            return;

        WorldPacket l_Data;
        l_Player->BuildPlayerChat(&l_Data, l_TargetGuid, CHAT_MSG_WHISPER, l_Text, l_Lang);
        l_Player->GetSession()->SendPacket(&l_Data);
    }
}

void InterRealmClient::Handle_TunneledClient(WorldPacket &recvPacket)
{
    uint64 playerGuid;
    uint16 opcodeId;

    recvPacket >> playerGuid;
    recvPacket >> opcodeId;

    WorldPacket tunPacket(opcodeId, recvPacket.size() - (8 + 2));

    for (unsigned int i = 10; i < recvPacket.size(); ++i)
    {
        uint8 rawData;
        recvPacket >> rawData;
        tunPacket << uint8(rawData);
    }

    IRPlayers::const_iterator itr = m_Players.find(playerGuid);
    if (itr == m_Players.end())
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Tunneled packet was sended for unregistered player.");
        return;
    }

    itr->second->GetSession()->SendPacket(&tunPacket);
}

void InterRealmClient::Handle_ServerAnnounce(WorldPacket& recvPacket)
{
    uint64 senderGUID;
    std::string text;

    recvPacket >> senderGUID >> text;

    WorldPacket l_Data(SMSG_CHAT_SERVER_MESSAGE, 50);              // guess size
    l_Data << uint32(SERVER_MSG_STRING);
    l_Data.WriteBits(text.size(), 11);
    l_Data.FlushBits();
    l_Data.WriteString(text);

    IRPlayers::const_iterator itr;
    for (itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player* player = itr->second;
        if (player && player->IsInWorld())
            itr->second->GetSession()->SendPacket(&l_Data);
    }
}

void InterRealmClient::Handle_TeleportPlayer(WorldPacket& recvPacket)
{
    /*
    uint64 playerGUID, muteTime;
    uint32 accountId, playerBytes1, playerBytes2, playerGuildID;
    uint8 security, isPremium, expansion, locale, recruiter;
    uint8 playerLevel, playerRace, playerClass, playerGender, playerBGStatus, isSpectator;
    std::string playerName;

    uint64 targetGUID;

    recvPacket >> playerGUID >> targetGUID >> accountId >> security >> isPremium >> expansion >> muteTime >> locale >> recruiter >> isSpectator;

    uint32 playerGUIDLow = GUID_LOPART(playerGUID);
    WorldSession* session = NULL;
    Player* player = NULL;
    Player* teleportTarget = sObjectAccessor->FindPlayer(targetGUID);

    if (!teleportTarget)
        return;

    if (isSpectator)
    {
        Map* cMap = teleportTarget->GetMap();
        if (!cMap->IsBattleArena())
            return;

        // search for two teams
        Battleground *bGround = teleportTarget->GetBattleground();

        if (!bGround)
            return;

        if (bGround->GetStartDelayTime())
            return;
    }

    uint32 generatedAccountId = sInterRealmMgr->GenerateNewId(accountId, CIM_TYPE_SESSION);

    IRPlayers::iterator itr = m_Players.find(playerGUID);
    if (itr == m_Players.end())
    {
        // Create new session
        ACE_NEW(session, WorldSession(generatedAccountId, this, AccountTypes(security), (bool)isPremium, expansion, muteTime, LocaleConstant(locale), recruiter, false, GetRealmName()));

        // Set account data
        for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        {
            std::string _data;
            time_t _time;

            recvPacket >> _data;
            recvPacket >> _time;

            session->SetAccountData(AccountDataType(i), _time, _data);
        }

        // Set tutorials
        for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
            session->SetTutorialInt(i, 0);
     
        recvPacket >> playerName >> playerLevel >> playerRace >> playerClass >> playerBytes1 >> playerBytes2 >> playerGender >> playerGuildID;

        CharacterCreateInfo createInfo(playerName, playerLevel, playerRace, playerClass, playerGender, playerBytes1, playerBytes2, 0);
        player = new Player(session);

        if (!player->Create(GUID_LOPART(playerGUID), &createInfo))
        {
            sLog->outError(LOG_FILTER_INTERREALM, "Failed to register a player.");

            WorldPacket errPacket(IR_SMSG_REGISTER_PLAYER_RESP, 1);
            errPacket << uint8(1);
            errPacket << uint64(playerGUID);
            SendPacket(&errPacket);

            delete session;
            delete player;
            return;
        }

        player->SetRealGUID(playerGUID);
        player->SetLevel(playerLevel);
        player->GetMotionMaster()->Initialize();

        // Set guild
        if (playerGuildID)
            player->SetInGuild(playerGuildID);

        // Set bg winner status
        recvPacket >> playerBGStatus;
        player->SetRandomWinner((playerBGStatus == 1) ? true : false);

        // Arena Team info
        for (uint8 i = 0; i < 21; ++i)
        {
            uint32 value = 0;
            recvPacket >> value;
            player->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + i, value);
        }

        session->SetPlayer(player);
        sObjectAccessor->AddObject(player);
        m_Players[player->GetRealGUID()] = player;
    }
    else
    {
        delete itr->second;
        m_Players.erase(playerGUID);
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to teleport a player.");
        return;
    }

    player->GetSession()->SetPlayerLoading(true);
    if (!player->LoadFromDB(playerGUIDLow))
    {
        player->GetSession()->SetPlayerLoading(false);
        sLog->outError(LOG_FILTER_INTERREALM, "Could not load a player %u", playerGUIDLow);

        WorldPacket pckt(IR_SMSG_UNREGISTER_PLAYER_RESP);
        pckt << uint8(2);
        pckt << uint64(playerGUID);
        SendPacket(&pckt);

        if (Battleground* bg = player->GetBattleground())
        {
            bg->RemovePlayerAtLeave(player->GetGUID());
            bg->RemoveFromInterRealm(player->GetGUID());
        }
        return;
    }

    player->GetSession()->SetPlayerLoading(false);
    player->SetBattlegroundEntryPoint();
    player->GetSession()->setInInterRealmBG(true);
    player->SetForceCleanupChannels(true);
    player->GetSession()->SendNameQueryOpcode(playerGUID);
    player->SetBattlegroundId(teleportTarget->GetBattlegroundId(), teleportTarget->GetBattlegroundTypeId());
    player->SetPhaseMask(teleportTarget->GetPhaseMask(), true);

    uint32 teleportOptions = 0;
    if (isSpectator)
    {
        float x, y, z;
        teleportTarget->GetContactPoint(player, x, y, z);
        player->RemoveAurasByType(SPELL_AURA_MOUNTED);
        player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
        player->UnsummonPetTemporaryIfAny();
        player->RemoveArenaAuras();
        player->SetSpectateRemoving(false);
        player->SetPhaseMask(teleportTarget->GetPhaseMask(), true);
        player->SetSpectate(true);
        teleportTarget->GetBattleground()->AddSpectator(player);
        teleportOptions = TELE_TO_GM_MODE | TELE_TO_SPECTATOR_MODE;
    }

    player->TeleportTo(teleportTarget->GetMapId(), teleportTarget->GetPositionX(), teleportTarget->GetPositionY(), teleportTarget->GetPositionZ(), teleportTarget->GetOrientation(), teleportOptions);

    if (Map *map = teleportTarget->GetMap())
        if (!player->GetMap())
            map->AddPlayerToMap(player);
            */
}

void InterRealmClient::SendSummonPlayer(uint64 const &sender, std::string const &name)
{
    WorldPacket data(IR_SUMMON_PLAYER, 8 + name.size());
    data << sender;
    data << name;
    SendPacket(&data);
}

void InterRealmClient::Handle_RatedBattlegroundStats(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_RATED_BATTLEGROUND_STATS");

    uint64 playerGuid;

    recvPacket >> playerGuid;

    uint32 lowGuid = GUID_LOPART(playerGuid);

    WorldPacket pckt;

    /*if (RatedBattlegroundStats const* stats = sBattlegroundMgr->GetRatedBattlegroundStats(lowGuid, m_realmId))
    {
        pckt.Initialize(IR_SMSG_RATED_BATTLEGROUND_STATS_RESP, 1 + 8 + 2 + 2 + 2 + 2);
        pckt << uint64(playerGuid);
        pckt << uint16(stats->weekGames);
        pckt << uint16(stats->weekWins);
        pckt << uint16(stats->personalRating);
        pckt << uint16(stats->matchMakerRating);
    }
    else
    {
        RatedBattlegroundStatsContainer& statsmap = sBattlegroundMgr->GetRatedBattlegroundStatsMap();
        RatedBattlegroundStats& newstats = statsmap[GuidRealmPair(lowGuid, m_realmId)];
        newstats.guid = lowGuid;
        newstats.realmNumber = m_realmId;
        newstats.weekGames = 0;
        newstats.weekWins = 0;
        newstats.personalRating = sWorld->getIntConfig(CONFIG_RBG_START_PERSONAL_RATING);
        newstats.matchMakerRating = sWorld->getIntConfig(CONFIG_RBG_START_MATCHMAKER_RATING);

        pckt.Initialize(IR_SMSG_RATED_BATTLEGROUND_STATS_RESP, 1 + 8);
        pckt << uint64(playerGuid);
        pckt << uint16(0);
        pckt << uint16(0);
        pckt << uint16(sWorld->getIntConfig(CONFIG_RBG_START_PERSONAL_RATING));
        pckt << uint16(sWorld->getIntConfig(CONFIG_RBG_START_MATCHMAKER_RATING));
    }

    SendPacket(&pckt);*/
}

void InterRealmClient::Handle_BattlegroundHoliday(WorldPacket& recvPacket)
{
    WorldPacket packet(IR_SMSG_BATTLEGROUND_HOLIDAY_RESP, 1 + 10);

    size_t countPos = packet.wpos();
    packet << uint8(0);

    uint8 count = 0;

    for (uint32 i = 1; i < sBattlemasterListStore.GetNumRows(); ++i)
    {
        BattlemasterListEntry const* bl = sBattlemasterListStore.LookupEntry(i);
        if (bl && bl->HolidayWorldState)
        {
            packet << uint32(bl->ID);

            if (sBattlegroundMgr->IsBGWeekend((BattlegroundTypeId)bl->ID))
                packet << uint8(1);
            else
                packet << uint8(0);

            count++;
        }
    }

    packet.put<uint8>(countPos, count);

    SendPacket(&packet);
}

void InterRealmClient::Handle_SummonPlayer(WorldPacket& recvPacket)
{
    uint64 playerGuid;

    recvPacket >> playerGuid;

    Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid);
    if (!pPlayer)
        return;

    if (pPlayer->GetSession()->GetInterRealmNumber() != this->GetRealmId())
        return;

    if (Battleground* bg = pPlayer->GetBattleground())
    {
        bg->RemovePlayerAtLeave(playerGuid, true, true);
        bg->RemoveFromInterRealm(playerGuid);
    }
}

void InterRealmClient::Handle_PlayerReconnect(WorldPacket& p_Packet)
{
    uint64 l_PlayerGuid = p_Packet.read<uint64>();
    uint32 l_InstanceId = p_Packet.read<uint32>();
    uint32 l_BgTypeId   = p_Packet.read<uint32>();

    Battleground* l_Battleground = sBattlegroundMgr->GetBattleground(l_InstanceId, MS::Battlegrounds::GetSchedulerType((BattlegroundTypeId)l_BgTypeId));

    /// Disable for now, player exploit to to reconnect full life in arena
    bool l_Result = false;// l_Battleground && l_Battleground->IsPlayerInBattleground(l_PlayerGuid) && l_Battleground->GetStatus() != BattlegroundStatus::STATUS_WAIT_LEAVE;
    SendPlayerReconnectResult(l_PlayerGuid, l_Result);
}

void InterRealmClient::Handle_ReserveLocalGuid(WorldPacket& p_Packet)
{
    HighGuid l_HighGuid  = (HighGuid)p_Packet.read<uint32>();
    uint32   l_StartGuid = p_Packet.read<uint32>();
    uint32   l_RangeSize = p_Packet.read<uint32>();

    for (uint32 l_I = l_StartGuid; l_I < l_StartGuid + l_RangeSize; l_I++)
    {
        switch (l_HighGuid)
        {
            case HIGHGUID_ITEM:
                m_ItemLocalRealmGuidReserve.add(l_I);
                break;
            case HIGHGUID_PET_NUMBER:
                m_PetLocalRealmGuidReserve.add(l_I);
                break;
            case HIGHGUID_MAIL:
                m_MailLocalRealmGuidReserve.add(l_I);
                break;
            case HIGHGUID_EQUIPMENT_SET:
                m_EquipmentSetLocalRealmGuidReserve.add(l_I);
                break;
        }
    }

    m_WaitingFoLocalRealmGuid[l_HighGuid] = false;
}

void InterRealmClient::SendKickPlayer(uint64 gmGUID, std::string const &name)
{
    WorldPacket data(IR_CMSG_KICK_CHARACTER, 8 + name.size());
    data << gmGUID;
    data << name;
    SendPacket(&data);
}

void InterRealmClient::SendWhisper(uint64 const &senderGUID, uint32 const &language, std::string const &targetName, std::string const &message)
{
    WorldPacket packet(IR_MSG_WHISPER, 8 + 8 + 4 + targetName.size() + message.size());
    packet << senderGUID;
    packet << language;
    packet << targetName;
    packet << message;
    SendPacket(&packet);
}

void InterRealmClient::SendGuildChat(uint64 const& playerGuid, uint32 const &language, std::string const &message)
{
    WorldPacket packet(IR_MSG_GUILD_CHAT, 8 + 4 + message.size());
    packet << playerGuid;
    packet << language;
    packet << message;
    SendPacket(&packet);
}

void InterRealmClient::SendArenaTeamCommandResult( uint32 team_action, const std::string& team, const std::string& player, uint32 error_id, uint64 targetGUID )
{
    WorldPacket pckt(IR_SMSG_ARENA_TEAM_SEND_ERROR, 8 + player.size() + team.size() + 1 + 4);
    pckt << uint64(targetGUID);
    pckt << std::string(team);
    pckt << std::string(player);
    pckt << uint32(team_action);
    pckt << uint32(error_id);
    SendPacket(&pckt);
}

void InterRealmClient::BroadcastEvent( uint8 event, uint64 guid, uint8 strCount, std::string const str1, std::string const str2, std::string const str3, ArenaTeam *team )
{

}

void InterRealmClient::SendUpdateArenaFields( uint32 slot, uint32 type, uint32 id, uint8 isMember, uint64 playerGuid )
{

}

void InterRealmClient::SendUpdateArenaFieldsPartTwo( uint32 slot, uint32 weekGames, uint32 seasonGames,uint32 seasonWins, uint64 playerGuid )
{

}

void InterRealmClient::SendPlayerBattlegroundZoneID(Player const *player)
{
    if (Battleground *bg = player->GetBattleground())
    {
        WorldPacket data(IR_SMSG_PLAYER_BG_ZONE_ID, 8 + 4 + 4 + 4);
        data << uint64(player->GetGUID());
        data << uint32(player->GetZoneId());
        data << uint32(player->GetAreaId());
        data << uint32(player->GetMapId());
        SendPacket(&data);
    }
}

void InterRealmClient::SendPlayerPinfoCommand(uint64 const &playerGUID, std::string const &playerName)
{
    WorldPacket data(IR_CMSG_PLAYER_PINFO, 8 + playerName.size());
    data << playerGUID;
    data << playerName;
    SendPacket(&data);
}

void InterRealmClient::SendPlayerBanCharCommand(uint64 const &playerGUID, std::string const &name, std::string const &duration, std::string const &reason, std::string const &gmName)
{
    WorldPacket data(IR_CMSG_BAN_CHARACTER, 8 + name.size() + duration.size() + reason.size() + gmName.size());
    data << playerGUID;
    data << name;
    data << duration;
    data << reason;
    data << gmName;
    SendPacket(&data);
}

void InterRealmClient::SendPlayerBanCommand(uint64 const &playerGUID, uint8 const &banMode, std::string const &nameOrIp, std::string const &duration, std::string const &reason, std::string const &gmName)
{
    WorldPacket data(IR_CMSG_BAN, 8 + 1 + nameOrIp.size() + duration.size() + reason.size() + gmName.size());
    data << playerGUID;
    data << banMode;
    data << nameOrIp;
    data << duration;
    data << reason;
    data << gmName;
    SendPacket(&data);
}

void InterRealmClient::SendPlayerUnbanCharCommand(uint64 const &playerGUID, std::string const &name, std::string const &gmName)
{
    WorldPacket data(IR_CMSG_UNBAN_CHARACTER, 8 + name.size() + gmName.size());
    data << playerGUID;
    data << name;
    data << gmName;
    SendPacket(&data);
}

void InterRealmClient::SendPlayerUnbanCommand(uint64 const &playerGUID, uint8 const &banMode, std::string const &nameOrIp, std::string const &gmName)
{
    WorldPacket data(IR_CMSG_UNBAN, 8 + 1 + nameOrIp.size() + gmName.size());
    data << playerGUID;
    data << banMode;
    data << nameOrIp;
    data << gmName;
    SendPacket(&data);
}

void InterRealmClient::SendBGQueueAnnouncer(const char* bgName, uint32 q_min_level, uint32 q_max_level,
    uint32 qAlliance, uint32 qAllianceDiff, uint32 qHorde, uint32 qHordeDiff, uint64 guid)
{
    WorldPacket pckt(IR_SMSG_BG_QUEUE_ANNOUNCER, 1 + (guid ? 8 : 0) + strlen(bgName) + 4 + 4 + 4 + 4 + 4 + 4);
    
    std::string strBgName(bgName, 0, strlen(bgName));

    pckt << strBgName;
    pckt << uint32(q_min_level) << uint32 (q_max_level);
    pckt << uint32(qAlliance) << uint32(qAllianceDiff);
    pckt << uint32(qHorde) << uint32(qHordeDiff);

    if (guid)
    {
        pckt << uint8(1);
        pckt << uint64(guid);
    }
    else
        pckt << uint8(0);

    SendPacket(&pckt);

}

void InterRealmClient::SendArenaQueueAnnouncer(uint32 arenaType, uint32 teamRating)
{
    WorldPacket pckt(IR_SMSG_ARENA_QUEUE_ANNOUNCER, 4 + 4);

    pckt << uint32(arenaType);
    pckt << uint32(teamRating);

    SendPacket(&pckt);
}

void InterRealmClient::SendAchievementReward(uint64 playerGuid, uint32 achievement)
{
    WorldPacket packet(IR_SMSG_ACHIEVEMENT_REWARD);
    packet << playerGuid;
    packet << achievement;
    SendPacket(&packet);
}

void InterRealmClient::SendAnticheatReport(uint32 guidLow, uint8 reportType)
{
    WorldPacket packet(IR_SMSG_ANTICHEAT_REPORT);
    packet << guidLow;
    packet << reportType;
    SendPacket(&packet);
}

void InterRealmClient::SendPlayerReconnectResult(uint64 p_PlayerGuid, bool p_Result)
{
    WorldPacket l_Data(IR_SMSG_PLAYER_RECONNECT_RESULT);
    l_Data << uint64(p_PlayerGuid);
    l_Data << uint8(p_Result);
    SendPacket(&l_Data);
}

void InterRealmClient::CheckPlayers()
{
    if (!m_Players.empty())
    {
        WorldPacket pckt(IR_SMSG_CHECK_PLAYERS, 4 + ((8 + 1) * m_Players.size()));
        
        uint32 count = 0;
        size_t num_pos = pckt.wpos();
        pckt << uint32(0);

        for (IRPlayers::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        {
            if (itr->second->IsNeedRemove())
                continue;

            pckt << uint64(itr->first);
            pckt << uint8(itr->second->GetSession()->isInInterRealmBG());

            count++;
        }

        pckt.put<uint32>(num_pos, count);

        SendPacket(&pckt);
    }
}

void InterRealmClient::Update(const uint32 diff)
{
    ProcessPackets();

    UpdateDatabaseConnection(diff);

    UpdateCheckPlayers(diff);
}

void InterRealmClient::AddPacket(WorldPacket* new_packet)
{
    _queue.add(new_packet);
}

void InterRealmClient::RemovePlayer(uint64 guid)
{
    IRPlayers::iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
        m_Players.erase(itr);
}

void InterRealmClient::RemoveAllPlayers()
{
    for (IRPlayers::iterator itr = m_Players.begin(); itr != m_Players.end();)
    {
        Player* l_Player = itr->second;
        uint64  l_Guid   = l_Player->GetGUID();

        l_Player->RemovePlayer();
        delete l_Player;
        sWorld->RemovePlayer(l_Guid);

        itr = m_Players.begin();
    }

    m_Players.clear();
}

InterRealmClient *InterRealmClient::GetIRClient(Player *player)
{
    if (!player || !player->GetSession())
        return nullptr;

    if (InterRealmClient *irClient = player->GetSession()->GetInterRealmClient())
        return irClient;

    return nullptr;
}

void InterRealmClient::UpdateDatabaseConnection(const uint32 diff)
{
    if (m_ping_mysql_timer <= diff)
    {
        GetDatabase()->KeepAlive();
        m_ping_mysql_timer = INTERREALM_PING_MYSQL_TIMER;
    }
    else
        m_ping_mysql_timer -= diff;
}

void InterRealmClient::UpdateCheckPlayers(const uint32 diff)
{
    if (m_check_players_timer <= diff)
    {
        CheckPlayers();
        m_check_players_timer = INTERREALM_CHECK_PLAYERS_TIMER;
    }
    else
    {
        m_check_players_timer -= diff;
    }
}

void InterRealmClient::SendCrossPartyInfo(Player* p_Player)
{
    Group* l_Group = p_Player->GetGroup();
    if (l_Group == nullptr)
        return;

    WorldPacket l_Packet(IR_SMSG_CROSS_PARTY_INFO);
    l_Packet << uint64(p_Player->GetRealGUID());
    l_Packet << uint8(l_Group->GetPartyFlags());
    l_Packet << uint8(l_Group->GetPartyIndex());
    l_Packet << uint8(l_Group->GetPartyType());
    l_Packet << uint64(l_Group->GetGUID());
    l_Packet << uint64(l_Group->GetLeaderGUID());
    SendPacket(&l_Packet);
}

void InterRealmClient::SendReserveLocalGuidRequest(HighGuid p_HighGuid, uint32 p_Range)
{
    if (m_WaitingFoLocalRealmGuid[p_HighGuid])
        return;

    m_WaitingFoLocalRealmGuid[p_HighGuid] = true;

    WorldPacket packet(IR_SMSG_RESERVE_LOCAL_GUID);
    packet << uint32(p_HighGuid);
    packet << uint32(p_Range);
    SendPacket(&packet);
}

uint32 InterRealmClient::GenerateLocalRealmLowGuid(HighGuid p_HighGuid)
{
    switch (p_HighGuid)
    {
        case HIGHGUID_ITEM:
        {
            uint32 l_NextGuid = 0;
            if (!m_ItemLocalRealmGuidReserve.empty())
                m_ItemLocalRealmGuidReserve.next(l_NextGuid);

            if (m_ItemLocalRealmGuidReserve.size() < m_LocalRealmItemGuidReserveMin)
                SendReserveLocalGuidRequest(p_HighGuid, m_LocalRealmItemGuidReserveSize);

            return l_NextGuid;
        }
        case HIGHGUID_MAIL:
        {
            uint32 l_NextGuid = 0;
            if (!m_MailLocalRealmGuidReserve.empty())
                m_MailLocalRealmGuidReserve.next(l_NextGuid);

            if (m_MailLocalRealmGuidReserve.size() < m_LocalRealmMailGuidReserveMin)
                SendReserveLocalGuidRequest(p_HighGuid, m_LocalRealmMailGuidReserveSize);

            return l_NextGuid;
        }
        case HIGHGUID_PET_NUMBER:
        {
            uint32 l_NextGuid = 0;
            if (!m_PetLocalRealmGuidReserve.empty())
                m_PetLocalRealmGuidReserve.next(l_NextGuid);

            if (m_PetLocalRealmGuidReserve.size() < m_LocalRealmPetGuidReserveMin)
                SendReserveLocalGuidRequest(p_HighGuid, m_LocalRealmPetGuidReserveSize);

            return l_NextGuid;
        }
        case HIGHGUID_EQUIPMENT_SET:
        {
            uint32 l_NextGuid = 0;
            if (!m_EquipmentSetLocalRealmGuidReserve.empty())
                m_EquipmentSetLocalRealmGuidReserve.next(l_NextGuid);

            if (m_EquipmentSetLocalRealmGuidReserve.size() < m_LocalRealmEquipmentSetGuidReserveMin)
                SendReserveLocalGuidRequest(p_HighGuid, m_LocalRealmEquipmentSetGuidReserveSize);

            return l_NextGuid;
        }
        default:
            break;
    }

    ASSERT(false);
    return 0;
}

void InterRealmClient::ProcessPackets()
{
    uint32 processedPackets = 0;
    WorldPacket* packet = NULL;
    while (m_IRSocket && !m_IRSocket->IsClosed() && !_queue.empty() && _queue.next(packet))
    {
        if (packet->GetOpcode() < IR_NUM_MSG_TYPES)
        {
            try
            {
                IROpcodeHandler* opHandle = IRopcodeTable[packet->GetOpcode()];
                if (!opHandle)
                {
                    sLog->outInfo(LOG_FILTER_INTERREALM, "Cannot find handle for the opcode (%u). Skipped packet.",
                    packet->GetOpcode());
                    continue;
                }
                (this->*(opHandle->IRhandler))(*packet);
            }
            catch(ByteBufferException &)
            {
                sLog->outError(LOG_FILTER_INTERREALM, "InterRealmClient ByteBufferException occured while parsing a packet (opcode: %u) from InterRealm Server. Skipped packet.",
                    packet->GetOpcode());
            }    
            if (packet->rpos() < packet->wpos())
                sLog->outError(LOG_FILTER_INTERREALM, "Unprocessed tail data (read stop at %u from %u) in opcode %s", packet->rpos(), packet->wpos(), IRopcodeTable[packet->GetOpcode()]->name);
        }
        else
            Handle_Unhandled(*packet);

        if (packet != NULL)
        {
            delete packet;
            packet = NULL;
        }    

        #define MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE 200 
        processedPackets++;

        if (processedPackets > MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE)
            break;
    }
}
#endif