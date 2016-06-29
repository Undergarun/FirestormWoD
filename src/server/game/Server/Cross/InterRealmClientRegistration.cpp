#ifdef CROSS
#include "InterRealmClient.h"
#include "InterRealmOpcodes.h"
#include "World.h"
#include "BattlegroundMgr.hpp"
#include "BattlegroundPacketFactory.hpp"
#include "DisableMgr.h"
#include "Chat.h"
#include "InterRealmMgr.h"
#include "Group.h"
#include "GroupMgr.h"
#include "IRSocket.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"

void SkipPlayerBlock(WorldPacket* packet)
{
    std::string temp;

    packet->read_skip(4); // accountId
    packet->read_skip(1); // security
    packet->read_skip(1); // isPremium
    packet->read_skip(1); // expansion
    packet->read_skip(8); // mute_time
    packet->read_skip(1); // locale
    packet->read_skip(1); // recruiter

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        uint32 size;
        time_t _time;

        *packet >> size;
        packet->ReadString(size);
        *packet >> _time;
    }

    *packet >> temp;      // playerName
    packet->read_skip(1); // playerLevel
    packet->read_skip(1); // playerRace
    packet->read_skip(1); // playerClass
    packet->read_skip(4); // playerBytes1
    packet->read_skip(4); // playerBytes2
    packet->read_skip(1); // playerGender
    packet->read_skip(4); // guildId
    packet->read_skip(1); // guildRank
    packet->read_skip(4); // guildLevel
    packet->read_skip(1); // bg_status

    packet->read_skip(MAX_PVP_SLOT * 4 * 9);    ///< PlayerArenaInfo block
}

void ReadOrSkipArenaInfoBlock(WorldPacket& packet, Player* player = NULL)
{
    if (player == NULL)
    {
        packet.read_skip(MAX_PVP_SLOT * 4 * 9);
        return;
    }

    uint32 personalRating, bestOfWeek, bestOfSeason;
    uint32 matchMakerRating, weekGames, weekWins;
    uint32 prevWeekWins, seasonGames, seasonWins;

    for (uint8 i = 0; i < MAX_PVP_SLOT; ++i)
    {
        packet >> personalRating >> bestOfWeek >> bestOfSeason;
        packet >> matchMakerRating >> weekGames >> weekWins;
        packet >> prevWeekWins >> seasonGames >> seasonWins;

        player->InitArenaPersonalRating(i, personalRating);
        player->InitBestRatingOfWeek(i, bestOfWeek);
        player->InitBestRatingOfSeason(i, bestOfSeason);
        player->InitArenaMatchMakerRating(i, matchMakerRating);
        player->InitWeekGames(i, weekGames);
        player->InitWeekWins(i, weekWins);
        player->InitPrevWeekWins(i, prevWeekWins);
        player->InitSeasonGames(i, seasonGames);
        player->InitSeasonWins(i, seasonWins);
    }
}

Player* InterRealmClient::CreatePlayer(WorldPacket& packet, uint64 playerGuid)
{
    uint64 mute_time;
    uint32 accountId, playerBytes1, playerBytes2, guildId, guildLevel;
    uint8 security, isPremium, expansion, locale, recruiter;
    uint8 playerLevel, playerRace, playerClass, playerGender, guildRank, bg_status;
    std::string playerName;

    // Read worldsession data
    packet >> accountId;
    packet >> security;
    packet >> isPremium;
    packet >> expansion;
    packet >> mute_time;
    packet >> locale;
    packet >> recruiter;

    // Create worldsession
    WorldSession* session = new WorldSession(accountId, this, AccountTypes(security), (bool)isPremium, expansion, mute_time, LocaleConstant(locale), recruiter, false, GetRealmName());

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        time_t l_Time;
        uint32 l_Size;

        packet >> l_Size;
        std::string l_Data = packet.ReadString(l_Size);
        packet >> l_Time;

        session->SetAccountData(AccountDataType(i), l_Time, l_Data);
    }

    // Set tutorials
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        session->SetTutorialInt(i, 0);

    packet >> playerName;
    packet >> playerLevel;
    packet >> playerRace;
    packet >> playerClass;
    packet >> playerBytes1;
    packet >> playerBytes2;
    packet >> playerGender;
    packet >> guildId;
    packet >> guildRank;
    packet >> guildLevel;
    packet >> bg_status;

    CharacterCreateInfo createInfo(playerName, playerRace, playerClass, playerGender, playerBytes1 & 0xFF, (playerBytes1 >> 8) & 0xFF, (playerBytes1 >> 16) & 0xFF, (playerBytes1 >> 24) & 0xFF, playerBytes2 & 0xFF, 0, 0);

    Player* player = new Player(session);

    if (!player->Create(GUID_LOPART(playerGuid), &createInfo))
    {
        printf("Can't create player %llu\r\n", playerGuid);
        packet.read_skip(MAX_PVP_SLOT * 4 * 9);
        delete session;
        delete player;
        return NULL;
    }

    player->SetRealGUID(playerGuid);
    player->SetLevel(playerLevel);
    player->GetMotionMaster()->Initialize();

    // Set guild data
    if (guildId)
        player->SetInGuild(guildId);

    if (guildRank)
        player->SetRank(guildRank);

    if (guildLevel)
        player->SetGuildLevel(guildLevel);

    // Set bg winner status
    player->SetRandomWinner((bg_status == 1) ? true : false, false);

    session->SetPlayer(player);

    ReadOrSkipArenaInfoBlock(packet, player);

    return player;
}

Player* InterRealmClient::CreatePlayerForGroup(WorldPacket& packet)
{
    return NULL;
}

void InterRealmClient::Handle_RegisterPlayer(WorldPacket& p_Packet)
{
    uint64 l_PlayerGuid;
    
    p_Packet >> l_PlayerGuid;

    Player* l_Player = NULL;
    bool l_HasExtistedPlayer = false;

    if (sWorld->HasPlayer(l_PlayerGuid))
    {
        Player* existedPlayer = sWorld->GetPlayer(l_PlayerGuid);
        if (existedPlayer->GetSession()->GetInterRealmNumber() != GetRealmId() ||
            existedPlayer->GetGroup() ||
            existedPlayer->InArenaQueue())
        {
            p_Packet.rfinish();
            SendRegisterPlayer(IR_REG_ALREADY_REGISTRED, l_PlayerGuid);
            return;
        }

        l_Player = existedPlayer;
        l_HasExtistedPlayer = true;
    }

    if (!l_HasExtistedPlayer)
    {
        l_Player = CreatePlayer(p_Packet, l_PlayerGuid);
        if (!l_Player)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "Failed to create a player.");
            p_Packet.rfinish();

            SendRegisterPlayer(IR_REG_ALREADY_REGISTRED, l_PlayerGuid);
            return;
        }

        sObjectAccessor->AddObject(l_Player);
        m_Players[l_Player->GetRealGUID()] = l_Player;
        sWorld->AddPlayer(l_Player);
    }
    else
    {
        SkipPlayerBlock(&p_Packet);
    }

    // Try to register the player into battleground system

    uint32 l_InstanceID;
    uint64 l_BattlegroundGUID;
    uint32 l_BGType;
    uint8  l_Role;
    uint32 l_BlacklistMap[2];

    p_Packet >> l_InstanceID;
    p_Packet >> l_BattlegroundGUID;
    p_Packet >> l_BGType;
    p_Packet >> l_Role;

    for (int l_I = 0; l_I < 2; l_I++)
        p_Packet >> l_BlacklistMap[l_I];

    if (!sBattlemasterListStore.LookupEntry(l_BGType))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: invalid bgtype (%u) received. possible cheater? player guid %u", l_BGType, l_Player->GetGUIDLow());
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, l_BGType, NULL))
    {
        ChatHandler(l_Player).PSendSysMessage(LANG_BG_DISABLED);
        return;
    }

    BattlegroundTypeId l_BGTypeID = BattlegroundTypeId(l_BGType);

    // can do this, since it's battleground, not arena
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = MS::Battlegrounds::GetTypeFromId(l_BGTypeID, 0);
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeIDRandom = MS::Battlegrounds::GetTypeFromId(BATTLEGROUND_RB, 0);

    // ignore if player is already in BG
    if (l_Player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* l_BG = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::GetSchedulerType(l_BGTypeID));

    if (!l_BG)
        return;

    // expected bracket entry
    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(l_Player->getLevel());

    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_Error = ERR_BATTLEGROUND_NONE;

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    // check Deserter debuff
    if (!l_Player->CanJoinToBattleground())
    {
        WorldPacket data;
        MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, l_Player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
        l_Player->GetSession()->SendPacket(&data);
        return;
    }

    if (l_Player->GetBattlegroundQueueIndex(l_BGQueueTypeIDRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        return;

    if (l_Player->InBattlegroundQueue() && l_BGTypeID == BATTLEGROUND_RB)
    {
        //player is already in queue, can't start random queue
        WorldPacket data;
        MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, l_Player, 0, ERR_IN_NON_RANDOM_BG);
        l_Player->GetSession()->SendPacket(&data);
        return;
    }

    // check if already in queue
    if (l_Player->GetBattlegroundQueueIndex(l_BGQueueTypeID) < PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        //player is already in this queue
        WorldPacket data;
        MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, l_Player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
        l_Player->GetSession()->SendPacket(&data);
        return;
    }

    // check if has free queue slots
    if (!l_Player->HasFreeBattlegroundQueueId())
    {
        WorldPacket data;
        MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, l_Player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
        l_Player->GetSession()->SendPacket(&data);
        return;
    }

    l_Player->SetBattleGroundRoles(l_Role);

    GroupQueueInfo * l_GroupQueueInfo = l_Scheduler.AddGroup(l_Player, nullptr, l_BGQueueTypeID, l_BlacklistMap, l_BracketEntry, ArenaType::None, false, 0, 0, false);
    uint32 l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);
    uint32 l_QueueSlot = l_Player->AddBattlegroundQueueId(l_BGQueueTypeID);

    // add joined time data
    l_Player->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

    WorldPacket l_Data; // send status packet (in queue)
    MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Player, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->m_JoinTime, l_GroupQueueInfo->m_ArenaType, false);
    l_Player->GetSession()->SendPacket(&l_Data);

    SendRegisterPlayer(IR_REG_OK, l_PlayerGuid);

    SendAdditionalInfoRequest(l_PlayerGuid);

    if (l_Player->GetGuildId())
    {
        uint64 l_GuildGuid = MAKE_NEW_GUID(l_Player->GetGuildId(), 0, HIGHGUID_GUILD);

        if (!sGuildMgr->HasInterRealmGuild(GetRealmId(), l_GuildGuid))
            SendInterRealmGuildQuery(l_GuildGuid);
    }
}

void InterRealmClient::Handle_PlayerReconnectReadyToLoad(WorldPacket& p_Packet)
{
    uint64 l_PlayerGuid = p_Packet.read<uint64>();
    Player* l_Player    = sWorld->GetPlayer(l_PlayerGuid);

    if (l_Player == nullptr)
    {
        l_Player = CreatePlayer(p_Packet, l_PlayerGuid);
        if (!l_Player)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "Failed to create a player.");
            p_Packet.rfinish();

            SendRegisterPlayer(IR_REG_ALREADY_REGISTRED, l_PlayerGuid);
            return;
        }

        sObjectAccessor->AddObject(l_Player);
        m_Players[l_Player->GetRealGUID()] = l_Player;
        sWorld->AddPlayer(l_Player);
    }
    else
    {
        SkipPlayerBlock(&p_Packet);
    }

    uint32 l_InstanceId = p_Packet.read<uint32>();
    uint32 l_BgTypeId   = p_Packet.read<uint32>();

    Battleground* l_Battleground = sBattlegroundMgr->GetBattleground(l_InstanceId, MS::Battlegrounds::GetSchedulerType((BattlegroundTypeId)l_BgTypeId));

    CharacterPortData l_PortData;
    l_PortData.playerGuid                = l_PlayerGuid;
    l_PortData.IsInvitedToBGInstanceGUID = l_InstanceId;
    l_PortData.bgTypeId                  = MS::Battlegrounds::GetSchedulerType((BattlegroundTypeId)l_BgTypeId);
    l_PortData.bgZoneId                  = l_Battleground ? l_Battleground->GetZoneId() : 1;

    l_Player->GetSession()->LoadCharacter(l_PortData);
}

void InterRealmClient::Handle_RegisterGroup(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_REGISTER_GROUP");

    printf("IR_CMSG_REGISTER_GROUP\r\n");

    uint8  l_PlayerCount;
    uint8  l_Role;
    uint32 l_InstanceId;
    uint32 l_BgTypeId;
    uint64 l_BattlegroundGUID;
    uint32 l_BlacklistMap[2];
    std::vector<uint64> l_PlayerGuids;

    p_Packet >> l_PlayerCount;
    p_Packet >> l_InstanceId >> l_BattlegroundGUID >> l_BgTypeId >> l_Role;

    for (int l_I = 0; l_I < 2; l_I++)
        p_Packet >> l_BlacklistMap[l_I];

    if (l_PlayerCount > 0)
    {
        for (uint8 i = 0; i < l_PlayerCount; ++i)
        {
            uint64 l_Guid;
            p_Packet >> l_Guid;
            l_PlayerGuids.push_back(l_Guid);
        }
    }

    printf("IR_CMSG_REGISTER_GROUP guid readed\r\n");


    if (l_PlayerGuids.empty())
        return;

    for (std::vector<uint64>::const_iterator l_Itr = l_PlayerGuids.begin(); l_Itr != l_PlayerGuids.end(); ++l_Itr)
    {
        if (sWorld->HasPlayer(*l_Itr))
        {
            WorldPacket l_ErrorPacket(IR_SMSG_REGISTER_PLAYER_RESP, 1);
            l_ErrorPacket << uint8(IR_REG_ALREADY_REGISTRED);
            l_ErrorPacket << uint64((*l_Itr));
            SendPacket(&l_ErrorPacket);

            p_Packet.rfinish();
            return;
        }
    }

    if (!sBattlemasterListStore.LookupEntry(l_BgTypeId))
        return;

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, l_BgTypeId, NULL))
        return;

    BattlegroundTypeId l_BGTypeID = BattlegroundTypeId(l_BgTypeId);

    // can do this, since it's battleground, not arena
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = MS::Battlegrounds::GetTypeFromId(l_BGTypeID, 0);
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeIDRandom = MS::Battlegrounds::GetTypeFromId(BATTLEGROUND_RB, 0);

    // get bg instance or bg template if instance not found
    Battleground* l_BG = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::GetSchedulerType(l_BGTypeID));
    if (!l_BG)
        return;

    std::list<Player*> l_Players;
    uint8 count = 1;
    for (std::vector<uint64>::const_iterator l_Itr = l_PlayerGuids.begin(); l_Itr != l_PlayerGuids.end(); ++l_Itr)
    {
        Player* l_Player = CreatePlayer(p_Packet, (*l_Itr));
        printf("IR_CMSG_REGISTER_GROUP player %u read!\r\n", count);
        count++;

        if (l_Player == nullptr)
            continue;

        l_Players.push_back(l_Player);
    }

    printf("IR_CMSG_REGISTER_GROUP all player readed\r\n");

    if (l_Players.empty())
        return;

    Group* l_Group   = nullptr;
    Player* l_Leader = nullptr;

    for (std::list<Player*>::const_iterator l_Itr = l_Players.begin(); l_Itr != l_Players.end(); ++l_Itr)
    {
        Player* l_Player = (*l_Itr);

        sObjectAccessor->AddObject(l_Player);
        m_Players[l_Player->GetRealGUID()] = l_Player;
        sWorld->AddPlayer(l_Player);
        
        if (l_Group == nullptr)
        {
            l_Group = new Group();
            l_Group->Create(l_Player);
            sGroupMgr->AddGroup(l_Group);

            l_Leader = l_Player;
        }
        else
        {
            l_Group->AddMember(l_Player);
        }
    }

    // expected bracket entry
    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(l_Leader->getLevel());
    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_Error = ERR_BATTLEGROUND_NONE;

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    l_Error = l_Group->CanJoinBattlegroundQueue(l_BG, l_BGQueueTypeID, 0);
    bool l_IsPremade = (l_Group->GetMembersCount() >= l_BG->GetMinPlayersPerTeam());

    GroupQueueInfo* l_GroupQueueInfo = NULL;
    uint32 avgTime = 0;

    if (!l_Error)
    {
        l_GroupQueueInfo = l_Scheduler.AddGroup(l_Leader, l_Group, l_BGQueueTypeID, l_BlacklistMap, l_BracketEntry, ArenaType::None, false, 0, 0, false);
        avgTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);
    }

    for (GroupReference * l_It = l_Group->GetFirstMember(); l_It != NULL; l_It = l_It->next())
    {
        Player* l_Member = l_It->getSource();

        if (!l_Member)
            continue;   // this should never happen

        if (l_Error)
        {
            /// Send fail packet
            WorldPacket data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&data, l_BG, l_Leader, 0, l_Error);
            l_Member->GetSession()->SendPacket(&data);
            
            /// Enable remove from cross state
            l_Member->SetNeedRemove(true);

            continue;
        }

        // add to queue
        uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

        // add joined time data
        l_Member->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);
        l_Member->SetBattleGroundRoles(l_Role);

        WorldPacket l_Data; // send status packet (in queue)
        MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, avgTime, l_GroupQueueInfo->m_JoinTime, l_GroupQueueInfo->m_ArenaType, false);
        l_Member->GetSession()->SendPacket(&l_Data);

        SendAdditionalInfoRequest(l_Member->GetGUID());
    }

    if (l_Error)
    {
        sGroupMgr->RemoveGroup(l_Group);
        delete l_Group;
        return;
    }

    sLog->outDebug(LOG_FILTER_INTERREALM, "Succeed to register a group.");
}

void InterRealmClient::Handle_RegisterArena(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_REGISTER_ARENA");

    uint8 l_PlayerCount;
    ArenaType l_ArenaType;
    bool  l_Skirmish;
    std::vector<uint64> l_PlayersGuid;

    l_PlayerCount = p_Packet.read<uint8>();
    l_ArenaType   = (ArenaType)p_Packet.read<uint8>();
    l_Skirmish    = p_Packet.read<uint8>();

    if (l_PlayerCount > 0)
    {
        for (uint8 l_I = 0; l_I < l_PlayerCount; ++l_I)
        {
            uint64 l_Guid;
            p_Packet >> l_Guid;
            l_PlayersGuid.push_back(l_Guid);
        }
    }

    if (l_PlayersGuid.empty())
        return;

    for (std::vector<uint64>::const_iterator l_Itr = l_PlayersGuid.begin(); l_Itr != l_PlayersGuid.end(); ++l_Itr)
    {
        if (sWorld->HasPlayer(*l_Itr))
        {
            WorldPacket l_ErrorPacket(IR_SMSG_REGISTER_PLAYER_RESP, 1);
            l_ErrorPacket << uint8(IR_REG_ALREADY_REGISTRED);
            l_ErrorPacket << uint64((*l_Itr));
            SendPacket(&l_ErrorPacket);

            p_Packet.rfinish();
            return;
        }
    }

    std::list<Player*> l_Players;
    for (std::vector<uint64>::const_iterator l_Itr = l_PlayersGuid.begin(); l_Itr != l_PlayersGuid.end(); ++l_Itr)
    {
        Player* l_Player = CreatePlayer(p_Packet, (*l_Itr));
        if (l_Player == nullptr)
            continue;

        l_Players.push_back(l_Player);
    }

    uint32 l_ArenaRating = 0;
    uint32 l_MatchmakerRating = 0;

    /// Check existance
    Battleground* l_Battleground = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::AllArenas);

    if (!l_Battleground)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (all arenas) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
        return;

    Group*  l_Group  = nullptr;
    Player* l_Leader = nullptr;

    for (std::list<Player*>::const_iterator l_Itr = l_Players.begin(); l_Itr != l_Players.end(); ++l_Itr)
    {
        Player* l_Player = (*l_Itr);

        sObjectAccessor->AddObject(l_Player);
        m_Players[l_Player->GetRealGUID()] = l_Player;
        sWorld->AddPlayer(l_Player);
        
        if (l_Group == nullptr)
        {
            l_Group = new Group();
            l_Group->Create(l_Player);
            sGroupMgr->AddGroup(l_Group);

            l_Leader = l_Player;
        }
        else
        {
            l_Group->AddMember(l_Player);
        }
    }

    if (!l_Leader)
        return;

    BattlegroundTypeId      l_BGTypeId = l_Battleground->GetTypeID();
    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = MS::Battlegrounds::GetTypeFromId(l_BGTypeId, l_ArenaType, l_Skirmish);

    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(l_Leader->getLevel());

    if (!l_BracketEntry)
        return;

    if (!l_Skirmish)
    {
        uint32 l_PlayerDivider = 0;

        for (const GroupReference * l_GroupReference = l_Group->GetFirstMember(); l_GroupReference != nullptr; l_GroupReference = l_GroupReference->next())
        {
            if (const Player * l_GroupMember = l_GroupReference->getSource())
            {
                l_ArenaRating += l_GroupMember->GetArenaPersonalRating(Arena::GetSlotByType(l_ArenaType));
                l_MatchmakerRating += l_GroupMember->GetArenaMatchMakerRating(Arena::GetSlotByType(l_ArenaType));

                ++l_PlayerDivider;
            }
        }

        if (!l_PlayerDivider)
            return;

        l_ArenaRating /= l_PlayerDivider;
        l_MatchmakerRating /= l_PlayerDivider;

        if (l_ArenaRating <= 0)
            l_ArenaRating = 1;

        if (l_MatchmakerRating <= 0)
            l_MatchmakerRating = 1;
    }

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    uint32 l_AverageTime = 0;
    GroupQueueInfo * l_GroupQueueInfo = nullptr;
    GroupJoinBattlegroundResult l_ResultError = GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE;

    bool l_JoinAsGroup = l_Players.size() > 1;

    if (l_JoinAsGroup)
    {
        GroupJoinBattlegroundResult l_ResultError = l_Group->CanJoinBattlegroundQueue(l_Battleground, l_BGQueueTypeID, l_ArenaType);

        if (!l_ResultError || (l_ResultError && sBattlegroundMgr->isArenaTesting()))
        {
            l_GroupQueueInfo = l_Scheduler.AddGroup(l_Leader, l_Group, l_BGQueueTypeID, nullptr, l_BracketEntry, l_ArenaType, l_Skirmish, l_ArenaRating, l_MatchmakerRating, l_Skirmish);
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
                MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, l_Leader, 0, l_ResultError);
                l_Member->GetSession()->SendPacket(&l_Data);

                /// Enable remove from cross state
                l_Member->SetNeedRemove(true);

                continue;
            }

            if (!l_GroupQueueInfo)
                return;

            /// Add to queue
            uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

            /// Add joined time data
            l_Member->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

            WorldPacket l_Data; // send status packet (in queue)
            MS::Battlegrounds::PacketFactory::Status(&l_Data, l_Battleground, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->m_JoinTime, l_ArenaType, l_Skirmish);
            l_Member->GetSession()->SendPacket(&l_Data);

            SendAdditionalInfoRequest(l_Member->GetGUID());
        }
    }
    else
    {
        // check Deserter debuff
        if (!l_Leader->CanJoinToBattleground())
        {
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, l_Leader, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            l_Leader->GetSession()->SendPacket(&l_Data);
            l_Leader->SetNeedRemove(true);
            return;
        }

        if (l_Leader->InArenaQueue())
        {
            /// Player is already in an arena queue.
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, l_Leader, 0, ERR_BATTLEGROUND_JOIN_FAILED);
            l_Leader->GetSession()->SendPacket(&l_Data);
            l_Leader->SetNeedRemove(true);
            return;
        }

        if (l_Leader->GetBattlegroundQueueIndex(l_BGQueueTypeID) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            //player is already in random queue
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, l_Leader, 0, ERR_IN_RANDOM_BG);
            l_Leader->GetSession()->SendPacket(&l_Data);
            l_Leader->SetNeedRemove(true);
            return;
        }

        // check if has free queue slots
        if (!l_Leader->HasFreeBattlegroundQueueId())
        {
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, l_Leader, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            l_Leader->GetSession()->SendPacket(&l_Data);
            l_Leader->SetNeedRemove(true);
            return;
        }

        l_GroupQueueInfo = l_Scheduler.AddGroup(l_Leader, nullptr, l_BGQueueTypeID, nullptr, l_BracketEntry, l_ArenaType, l_Skirmish, l_ArenaRating, l_MatchmakerRating, l_Skirmish);
        uint32 l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueueInfo, l_BracketEntry->m_Id);
        uint32 l_QueueSlot = l_Leader->AddBattlegroundQueueId(l_BGQueueTypeID);

        // add joined time data
        l_Leader->AddBattlegroundQueueJoinTime(l_BGQueueTypeID, l_GroupQueueInfo->m_JoinTime);

        WorldPacket l_Data; // send status packet (in queue)
        MS::Battlegrounds::PacketFactory::Status(&l_Data, l_Battleground, l_Leader, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, l_GroupQueueInfo->m_JoinTime, l_GroupQueueInfo->m_ArenaType, l_Skirmish);
        l_Leader->GetSession()->SendPacket(&l_Data);

        SendAdditionalInfoRequest(l_Leader->GetGUID());
    }

    if (l_ResultError)
    {
        sGroupMgr->RemoveGroup(l_Group);
        delete l_Group;
        return;
    }
}

void InterRealmClient::Handle_RegisterRated(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_REGISTER_RATED");

    uint8 l_PlayerCount;
    std::vector<uint64> l_PlayersGuids;
    uint32 l_PersonalRating;
    uint32 l_MatchmakerRating ;

    p_Packet >> l_PlayerCount;
    p_Packet >> l_PersonalRating;
    p_Packet >> l_MatchmakerRating ;

    if (l_PlayerCount > 0)
    {
        for (uint8 l_I = 0; l_I < l_PlayerCount; ++l_I)
        {
            uint64 guid;
            p_Packet >> guid;
            l_PlayersGuids.push_back(guid);
        }
    }

    if (l_PlayersGuids.empty())
        return;

    for (std::vector<uint64>::const_iterator l_Itr = l_PlayersGuids.begin(); l_Itr != l_PlayersGuids.end(); ++l_Itr)
    {
        if (sWorld->HasPlayer(*l_Itr))
        {
            SendRegisterRated(IR_REG_ALREADY_REGISTRED, *l_Itr);
            return;
        }
    }

    //check existance
    Battleground* l_Battleground = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::RatedBg10v10);
    if (!l_Battleground || DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_RATED_10_VS_10, NULL))
    {
        SendRegisterRated(IR_REG_CANNOT_CREATE, 0);
        sLog->outError(LOG_FILTER_INTERREALM, "Battleground: template bg (all arenas) not found or disable");
        return;
    }

    std::list<Player*> l_Players;
    for (std::vector<uint64>::const_iterator l_Itr = l_PlayersGuids.begin(); l_Itr != l_PlayersGuids.end(); ++l_Itr)
    {
        Player* l_Player = CreatePlayer(p_Packet, (*l_Itr));
        if (l_Player == nullptr)
            continue;

        l_Players.push_back(l_Player);
    }

    Group*  l_Group = nullptr;
    Player* l_Leader = nullptr;

    for (std::list<Player*>::const_iterator l_Itr = l_Players.begin(); l_Itr != l_Players.end(); ++l_Itr)
    {
        Player* l_Player = (*l_Itr);

        sObjectAccessor->AddObject(l_Player);
        m_Players[l_Player->GetRealGUID()] = l_Player;
        sWorld->AddPlayer(l_Player);

        if (l_Group == nullptr)
        {
            l_Group = new Group();
            l_Group->Create(l_Player);
            sGroupMgr->AddGroup(l_Group);

            l_Leader = l_Player;
        }
        else
        {
            l_Group->AddMember(l_Player);
        }
    }
    BattlegroundTypeId l_BgTypeId = l_Battleground->GetTypeID();
    MS::Battlegrounds::BattlegroundType::Type l_BgQueueTypeId = MS::Battlegrounds::GetTypeFromId(l_BgTypeId, 0);

    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(l_Leader->getLevel());
    if (!l_BracketEntry)
        return;

    GroupJoinBattlegroundResult l_Error = ERR_BATTLEGROUND_NONE;

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    uint32 l_AvgTime = 0;
    GroupQueueInfo* l_GroupQueue;

    l_Error = l_Group->CanJoinBattlegroundQueue(l_Battleground, l_BgQueueTypeId, 10);
    if (!l_Error)
    {
        l_GroupQueue = l_Scheduler.AddGroup(l_Leader, l_Group, l_BgQueueTypeId, nullptr, l_BracketEntry, ArenaType::None, true, l_PersonalRating, l_MatchmakerRating, false);
        l_AvgTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GroupQueue, l_BracketEntry->m_Id);
    }

    for (GroupReference* l_Iterator = l_Group->GetFirstMember(); l_Iterator != NULL; l_Iterator = l_Iterator->next())
    {
        Player* l_Member = l_Iterator->getSource();
        if (l_Member == nullptr)
            continue;

        if (l_Error)
        {
            WorldPacket l_Data;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_Data, l_Battleground, l_Leader, 0, l_Error);
            l_Member->GetSession()->SendPacket(&l_Data);
            l_Member->SetNeedRemove(true);
            continue;
        }

        // add to queue
        uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BgQueueTypeId);

        // add joined time data
        l_Member->AddBattlegroundQueueJoinTime(l_BgQueueTypeId, l_GroupQueue->m_JoinTime);

        WorldPacket l_Data; // send status packet (in queue)
        MS::Battlegrounds::PacketFactory::Status(&l_Data, l_Battleground, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AvgTime, l_GroupQueue->m_JoinTime, l_GroupQueue->m_ArenaType, false);
        l_Member->GetSession()->SendPacket(&l_Data);

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for rated battleground as group bg queue type %u bg type %u: GUID %u, NAME %s", l_BgQueueTypeId, l_BgTypeId, l_Member->GetGUIDLow(), l_Member->GetName());
    }

    if (l_Error)
    {
        sGroupMgr->RemoveGroup(l_Group);
        delete l_Group;
        return;
    }
}

/// @TODO
void InterRealmClient::Handle_AppearRequest(WorldPacket& packet)
{
    /*uint64 playerGuid;
    uint64 targetGuid;
    
    packet >> playerGuid;
    packet >> targetGuid;

    if (playerGuid == 0)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Wrong player guid (%u), realm %u %s",
            playerGuid, GetRealmId(), GetRealmName().c_str());
        packet.rfinish();
        return;
    }

    if (sWorld->HasPlayer(playerGuid))
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to appear becouse player is already at the cross-server.");
        packet.rfinish();
        return;
    }

    Player* targetPlayer = sObjectAccessor->FindPlayer(targetGuid);
    if (!targetPlayer || targetPlayer->IsNeedRemove())
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to appear becouse targeted player is not at the cross-server.");
        packet.rfinish();
        return;
    }

    if (!targetPlayer->GetBattlegroundId() || !targetPlayer->GetBattleground())
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to appear becouse targeted player is not at the arena.");
        packet.rfinish();
        return;
    }

    Player* player = CreatePlayer(packet, playerGuid);
    if (!player)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to appear.");
        packet.rfinish();
        return;
    }

    uint32 bgZoneId = targetPlayer->GetBattleground()->GetZoneId();

    sObjectAccessor->AddObject(player);
    m_Players[player->GetRealGUID()] = player;
    sWorld->AddPlayer(player);

    WorldPacket data(IR_SMSG_APPEAR_REQUEST_RESP);
    data << playerGuid;
    data << targetGuid;
    data << uint8(IR_REG_OK);
    data << bgZoneId;

    SendPacket(&data);*/
}

/// @TODO
void InterRealmClient::Handle_Appear(WorldPacket& packet)
{
/*    uint64 playerGuid, targetGuid;

    packet >> playerGuid;
    packet >> targetGuid;

    Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid);
    if (!player)
        return;

    Player* targetPlayer = sObjectAccessor->FindPlayer(targetGuid);
    if (!targetPlayer || targetPlayer->IsNeedRemove())
        return;

    if (!targetPlayer->GetBattlegroundId())
        return;

    Battleground* bg = targetPlayer->GetBattleground();
    if (!bg)
        return;

    player->SetBattlegroundId(targetPlayer->GetBattlegroundId(), targetPlayer->GetBattlegroundTypeId());

    float x, y, z;
    targetPlayer->GetContactPoint(player, x, y, z);
    player->RemoveAurasByType(SPELL_AURA_MOUNTED);
    player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
    player->UnsummonPetTemporaryIfAny();
    player->RemoveArenaAuras();
    player->SetPhaseMask(targetPlayer->GetPhaseMask(), true);

    player->TeleportTo(targetPlayer->GetMapId(), x, y, z + 3.0f, player->GetAngle(targetPlayer), TELE_TO_GM_MODE);
    
    player->GetSession()->setInInterRealmBG(true);
    player->SetForceCleanupChannels(true);*/
}

void InterRealmClient::Handle_BattlefieldPort(WorldPacket& packet)
{
    uint64 l_PlayerGuid;
    uint32 l_Time;
    uint32 l_QueueSlot;
    uint8 l_Action;

    packet >> l_PlayerGuid;
    packet >> l_Time;
    packet >> l_QueueSlot;
    packet >> l_Action;

    Player* l_Player = sObjectAccessor->FindPlayerInOrOutOfWorld(l_PlayerGuid);
    if (!l_Player)
        return;

    MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = l_Player->GetBattlegroundQueueTypeId(l_QueueSlot);

    if (l_BGQueueTypeID >= MS::Battlegrounds::BattlegroundType::Total)
    {
        sLog->outAshran("HandleBattleFieldPortOpcode bgQueueTypeId %u", l_BGQueueTypeID);
        return;
    }

    MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
    MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

    /// We must use temporary variable, because GroupQueueInfo pointer can be deleted in BattlegroundQueue::RemovePlayer() function
    GroupQueueInfo l_GroupQueueInfo;

    if (!l_InvitationsMgr.GetPlayerGroupInfoData(l_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID) && !l_Scheduler.GetPlayerGroupInfoData(l_Player->GetGUID(), l_GroupQueueInfo, l_BGQueueTypeID))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: itrplayerstatus not found.");
        return;
    }

    /// if action == 1, then instanceId is required
    if (!l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID && l_Action == 1)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: instance not found.");
        return;
    }

    /// BGTemplateId returns BATTLEGROUND_AA when it is arena queue.
    /// Do instance id search as there is no AA bg instances.
    Battleground * l_BG = sBattlegroundMgr->GetBattleground(l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID, l_BGQueueTypeID);

    /// BG template might and must be used in case of leaving queue, when instance is not created yet
    if (!l_BG && l_Action == 0)
        l_BG = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::IsArena(l_BGQueueTypeID) ? MS::Battlegrounds::BattlegroundType::AllArenas : l_BGQueueTypeID);

    if (!l_BG)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "BattlegroundHandler: bg_template not found for type id %u.", l_BGQueueTypeID);
        return;
    }

    /// Get real bg type
    BattlegroundTypeId l_BGTypeID = l_BG->GetTypeID();

    /// Expected bracket entry
    MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(l_Player->getLevel());

    if (!l_BracketEntry)
        return;

    /// Some checks if player isn't cheating - it is not exactly cheating, but we cannot allow it
    if (l_Action == 1 && l_GroupQueueInfo.m_ArenaType == 0)
    {
        /// If player is trying to enter battleground (not arena!) and he has deserter debuff, we must just remove him from queue
        if (!l_Player->CanJoinToBattleground())
        {
            /// Send bg command result to show nice message
            WorldPacket l_SecondResponse;
            MS::Battlegrounds::PacketFactory::StatusFailed(&l_SecondResponse, l_BG, l_Player, 0, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            l_Player->GetSession()->SendPacket(&l_SecondResponse);

            l_Action = 0;

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) has a deserter debuff, do not port him to battleground!", l_Player->GetName(), l_Player->GetGUIDLow());
        }
        /// If player don't match battleground max level, then do not allow him to enter! (this might happen when player leveled up during his waiting in queue
        if (l_Player->getLevel() > l_BG->GetMaxLevel())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: Player %s (%u) has level (%u) higher than maxlevel (%u) of battleground (%u)! Do not port him to battleground!",
                l_Player->GetName(), l_Player->GetGUIDLow(), l_Player->getLevel(), l_BG->GetMaxLevel(), l_BG->GetTypeID());

            l_Action = 0;
        }
    }

    WorldPacket l_Response;

    switch (l_Action)
    {
        /// Port to battleground
        case 1:                                         
            if (!l_Player->IsInvitedForBattlegroundQueueType(l_BGQueueTypeID))
                return;                                 ///< cheating?

            /// Resurrect the player
            if (!l_Player->isAlive())
            {
                l_Player->ResurrectPlayer(1.0f);
                l_Player->SpawnCorpseBones();
            }

            /// Stop taxi flight at port
            if (l_Player->isInFlight())
            {
                l_Player->GetMotionMaster()->MovementExpired();
                l_Player->CleanupAfterTaxiFlight();
            }

            /// This is still needed here if battleground "jumping" shouldn't add deserter debuff
            /// Also this is required to prevent stuck at old battleground after SetBattlegroundId set to new
            if (Battleground* currentBg = l_Player->GetBattleground())
                currentBg->RemovePlayerAtLeave(l_Player->GetGUID());

            /// Set the destination instance id
            l_Player->SetBattlegroundId(l_BG->GetInstanceID(), l_BGTypeID);

            /// Set the destination team
            l_Player->SetBGTeam(l_GroupQueueInfo.m_Team);

            CharacterPortData l_PortData;
            l_PortData.playerGuid = l_PlayerGuid;
            l_PortData.IsInvitedToBGInstanceGUID = l_GroupQueueInfo.m_IsInvitedToBGInstanceGUID;
            l_PortData.bgTypeId = l_BGQueueTypeID;
            l_PortData.bgZoneId = l_BG->GetZoneId() ? l_BG->GetZoneId() : 1;

            l_Player->GetSession()->LoadCharacter(l_PortData);
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", l_Player->GetName(), l_Player->GetGUIDLow(), l_BG->GetInstanceID(), l_BG->GetTypeID(), l_BGQueueTypeID);
            sLog->outAshran("Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", l_Player->GetName(), l_Player->GetGUIDLow(), l_BG->GetInstanceID(), l_BG->GetTypeID(), l_BGQueueTypeID);
            break;

        /// Leave queue
        case 0:
            if (l_BG->isArena() && l_BG->GetStatus() > STATUS_WAIT_JOIN)
                return;

            MS::Battlegrounds::PacketFactory::Status(&l_Response, l_BG, l_Player, l_QueueSlot, STATUS_NONE, l_Player->GetBattlegroundQueueJoinTime(l_BGQueueTypeID), 0, l_GroupQueueInfo.m_ArenaType, l_GroupQueueInfo.m_IsSkirmish);
            l_Player->GetSession()->SendPacket(&l_Response);

            /// Must be called this way, because if you move this call to queue->removeplayer, it causes bugs
            l_Player->RemoveBattlegroundQueueId(l_BGQueueTypeID);

            sBattlegroundMgr->RemovePlayer(l_Player->GetGUID(), true, l_BGQueueTypeID);
            l_Player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);

            for (auto l_PlayerInfo : l_GroupQueueInfo.m_Players)
            {
                Player* l_Member = ObjectAccessor::FindPlayerInOrOutOfWorld(l_PlayerInfo.first);
                if (l_Member == nullptr || l_Player == l_Member)
                    continue;

                MS::Battlegrounds::PacketFactory::Status(&l_Response, l_BG, l_Member, l_QueueSlot, STATUS_NONE, l_Player->GetBattlegroundQueueJoinTime(l_BGQueueTypeID), 0, l_GroupQueueInfo.m_ArenaType, l_GroupQueueInfo.m_IsSkirmish);
                l_Member->GetSession()->SendPacket(&l_Response);

                sBattlegroundMgr->RemovePlayer(l_Member->GetGUID(), true, l_BGQueueTypeID);
                l_Member->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);

                /// Remove from cross
                l_Member->SetNeedRemove(true);
            }
            
            /// Remove from cross
            l_Player->SetNeedRemove(true);
            break;
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "Battleground port: unknown action %u", l_Action);
            break;
    }
}

void InterRealmClient::Handle_BattlefieldEnter(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_BATTLEFIELD_ENTER");
    
    uint64 l_PlayerGuid;
    uint32 l_PlayerGuidLow;
    uint32 l_InstanceId;
    uint16 l_BgTypeId;

    packet >> l_PlayerGuid;
    packet >> l_PlayerGuidLow;
    packet >> l_InstanceId;
    packet >> l_BgTypeId;

    if (Player* l_Player = sObjectAccessor->FindPlayerInOrOutOfWorld(l_PlayerGuid))
    {
        Battleground* l_Battleground = sBattlegroundMgr->GetBattleground(l_InstanceId, (MS::Battlegrounds::BattlegroundType::Type)l_BgTypeId);

        /// Reconnect case
        if (l_Battleground && l_Battleground->IsPlayerInBattleground(l_PlayerGuid))
        {
            MS::Battlegrounds::BattlegroundType::Type l_BgQueueTypeId = MS::Battlegrounds::GetTypeFromId(l_Battleground->GetTypeID(), l_Battleground->GetArenaType(), l_Battleground->IsSkirmish());

            l_Player->SetBGTeam(l_Player->GetReconnectBGTeam());
            l_Player->AddBattlegroundQueueId(l_BgQueueTypeId);
            l_Player->SetBattlegroundId(l_InstanceId, l_Battleground->GetTypeID());

            l_Battleground->EventPlayerLoggedIn(l_Player);

            l_Player->SetInviteForBattlegroundQueueType(l_BgQueueTypeId, l_Battleground->GetInstanceID());

            /// We give the deserter aura by default when we leave a battleground
            /// so if we succeed at re-entering the battleground, we remove the aura.
            if (l_Player->HasAura(MS::Battlegrounds::Spells::DeserterBuff))
                l_Player-> RemoveAura(MS::Battlegrounds::Spells::DeserterBuff);
        }

        sBattlegroundMgr->TeleportToBattleground(l_Player, l_InstanceId, (MS::Battlegrounds::BattlegroundType::Type)l_BgTypeId);
        l_Player->GetSession()->setInInterRealmBG(true);
        l_Player->SetForceCleanupChannels(true);
    }
}

void InterRealmClient::Handle_BattlefielLeave(WorldPacket& p_Packet)
{
    uint64 l_PlayerGuid = p_Packet.read<uint64>();

    if (Player* l_Player = sObjectAccessor->FindPlayerInOrOutOfWorld(l_PlayerGuid))
        l_Player->LeaveBattleground();
}

void InterRealmClient::Handle_SpectatorPort(WorldPacket& packet)
{
    /*uint64 playerGuid, targetGuid;

    packet >> playerGuid;
    packet >> targetGuid;

    Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid);
    if (!player)
        return;

    Player* targetPlayer = sObjectAccessor->FindPlayer(targetGuid);
    if (!targetPlayer || targetPlayer->IsNeedRemove())
        return;

    if (!targetPlayer->GetBattlegroundId())
        return;

    Battleground* bg = targetPlayer->GetBattleground();
    if (!bg)
        return;

    player->SetBattlegroundId(targetPlayer->GetBattlegroundId(), targetPlayer->GetBattlegroundTypeId());

    float x, y, z;
    targetPlayer->GetContactPoint(player, x, y, z);
    player->RemoveAurasByType(SPELL_AURA_MOUNTED);
    player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
    player->UnsummonPetTemporaryIfAny();
    player->RemoveArenaAuras();
    player->SetSpectateRemoving(false);
    player->SetPhaseMask(targetPlayer->GetPhaseMask(), true);
    player->SetSpectate(true);
    targetPlayer->GetBattleground()->AddSpectator(player);
    
    player->TeleportTo(targetPlayer->GetMapId(), x, y, z + 3.0f, player->GetAngle(targetPlayer), TELE_TO_GM_MODE | TELE_TO_SPECTATOR_MODE);
    
    player->GetSession()->setInInterRealmBG(true);
    player->SetForceCleanupChannels(true);*/
}

void InterRealmClient::Handle_AdditionalInfo(WorldPacket& packet)
{
    uint64 playerGuid;
    uint8 count;

    packet >> playerGuid;
    packet >> count;

    if (count > 0)
    {
        std::string address;
        packet >> address;

        if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
        {
            player->GetSession()->SetRemoteAddress(address);
        }
    }
}

void InterRealmClient::Handle_Guild(WorldPacket& packet)
{
    uint64 guildGuid = 0;
    std::string guildName;
    uint32 membersCount;
    uint32 achievementPoints = 0;
    uint32 emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor;
    uint32 ranksCount;

    packet >> guildGuid >> guildName;
    packet >> membersCount;
    packet >> emblemStyle >> emblemColor >> borderStyle >> borderColor >> backgroundColor;
    packet >> achievementPoints;
    packet >> ranksCount;

    InterRealmGuildEmblem* emblem = new InterRealmGuildEmblem(emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor);

    InterRealmGuild* guild = new InterRealmGuild(guildGuid, guildName, membersCount, 0, 0, achievementPoints, emblem);

    for (uint32 j = 0; j < ranksCount; ++j)
    {
        uint32 rankId;
        std::string rankName;

        packet >> rankId >> rankName;

        guild->AddRank(new InterRealmGuildRank(rankId, rankName));
    }

    sGuildMgr->AddInterRealmGuild(GetRealmId(), guild);
}

void InterRealmClient::SendBattlefieldPort(CharacterPortData const& data)
{
    WorldPacket pckt(IR_SMSG_BATTLEFIELD_PORT_RESP, 1 + 8 + 4 + 2);
    pckt << uint8(0); // 0 - ok, 1... - fail
    pckt << uint64(data.playerGuid);
    pckt << uint32(data.IsInvitedToBGInstanceGUID);
    pckt << uint16(data.bgTypeId);
    pckt << uint32(data.bgZoneId);
    SendPacket(&pckt);
}

void InterRealmClient::SendRegisterPlayer(uint8 answer, uint64 targetGuid)
{
    WorldPacket errPacket(IR_SMSG_REGISTER_PLAYER_RESP, 1);
    errPacket << answer;
    errPacket << targetGuid;
    SendPacket(&errPacket);
}

void InterRealmClient::SendRegisterRated(uint8 answer, uint64 targetGuid)
{
    WorldPacket errPacket(IR_SMSG_REGISTER_RATED_RESP, 1);
    errPacket << answer;
    errPacket << targetGuid;
    SendPacket(&errPacket);
}

void InterRealmClient::SendRegisterSpectator(uint64 playerGuid, uint64 targetGuid, uint8 answer, uint32 bgZoneId)
{
    WorldPacket packet(IR_SMSG_REGISTER_SPECTATOR_RESP, 1);
    packet << playerGuid;
    packet << targetGuid;
    packet << answer;
    packet << bgZoneId;
    SendPacket(&packet);
}

void InterRealmClient::SendAdditionalInfoRequest(uint64 playerGuid)
{
    WorldPacket packet(IR_SMSG_ADDITIONAL_INFO);
    packet << playerGuid;
    SendPacket(&packet);
 }

void InterRealmClient::SendInterRealmGuildQuery(uint64 guildGuid)
{
    WorldPacket packet(IR_SMSG_GUILD_QUERY);
    packet << uint64(guildGuid);
    SendPacket(&packet);
}
#endif