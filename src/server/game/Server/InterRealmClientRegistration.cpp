#include "InterRealmClient.h"
#include "InterRealmOpcodes.h"
#include "World.h"
#include "BattlegroundMgr.h"
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
        time_t _time;
        *packet >> temp;
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
        std::string _data;
        time_t _time;

        packet >> _data;
        packet >> _time;

        session->SetAccountData(AccountDataType(i), _time, _data);
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

    CharacterCreateInfo createInfo(playerName, playerLevel, playerRace, 
        playerClass, playerGender, playerBytes1, playerBytes2, 0);

    Player* player = new Player(session);

    if (!player->Create(GUID_LOPART(playerGuid), &createInfo))
    {
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
    player->SetRandomWinner((bg_status == 1) ? true : false);

    session->SetPlayer(player);

    return player;
}

Player* InterRealmClient::CreatePlayerForGroup(WorldPacket& packet)
{
    return NULL;
}

void ReadOrSkipArenaInfoBlock(WorldPacket& packet, Player* player = NULL)
{
    if (player == NULL)
    {
        packet.read_skip(4 * 9);
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

void InterRealmClient::Handle_RegisterPlayer(WorldPacket& packet)
{
    uint64 playerGuid;
    
    packet >> playerGuid;

    Player* player = NULL;
    bool hasExtistedPlayer = false;

    if (sWorld->HasPlayer(playerGuid))
    {
        Player* existedPlayer = sWorld->GetPlayer(playerGuid);
        if (existedPlayer->GetSession()->GetInterRealmNumber() != GetRealmId() ||
            existedPlayer->GetGroup() ||
            existedPlayer->InArenaQueue())
        {
            packet.rfinish();
            SendRegisterPlayer(IR_REG_ALREADY_REGISTRED, playerGuid);
            return;
        }

        player = existedPlayer;
        hasExtistedPlayer = true;
    }

    if (!hasExtistedPlayer)
    {
        player = CreatePlayer(packet, playerGuid);
        if (!player)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "Failed to create a player.");
            packet.rfinish();

            SendRegisterPlayer(IR_REG_ALREADY_REGISTRED, playerGuid);
            return;
        }

        sObjectAccessor->AddObject(player);
        m_Players[player->GetRealGUID()] = player;
        sWorld->AddPlayer(player);
    }
    else
    {
        SkipPlayerBlock(&packet);
    }

    // Try to register the player into battleground system

    uint32 _instanceId;
    uint64 _bgGuid;
    uint32 bgTypeId_;

    packet >> _instanceId;
    packet >> _bgGuid;
    packet >> bgTypeId_;

    ReadOrSkipArenaInfoBlock(packet, !hasExtistedPlayer ? player : NULL);

    //extract from guid
    // uint32 bgTypeId_ = GUID_LOPART(_bgGuid);

    if (!sBattlemasterListStore.LookupEntry(bgTypeId_))
        return;

    BattlegroundTypeId bgTypeId = BattlegroundTypeId(bgTypeId_);

    // can do this, since it's battleground, not arena
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);
    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // ignore if player is already in BG
    if (player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* bg = NULL;
    if (_instanceId)
        bg = sBattlegroundMgr->GetBattlegroundThroughClientInstance(_instanceId, bgTypeId);

    if (!bg)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return;

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

    if (player->GetBattlegroundQueueIndex(bgQueueTypeIdRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
    {
        // player is already in random queue
        WorldPacket data;
        sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, player, 0, ERR_IN_RANDOM_BG);
        player->GetSession()->SendPacket(&data);
        return;
    }

    if (player->InBattlegroundQueue() && bgTypeId == BATTLEGROUND_RB)
    {
        // player is already in queue, can't start random queue
        WorldPacket data;
        sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, player, 0, ERR_IN_NON_RANDOM_BG);
        player->GetSession()->SendPacket(&data);
        return;
    }

    // check if already in queue
    if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        // player is already in this queue
        return;

    // check if has free queue slots
    if (!player->HasFreeBattlegroundQueueId())
    {
        WorldPacket data;
        sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, player, 0, ERR_BATTLEGROUND_TOO_MANY_QUEUES);
        player->GetSession()->SendPacket(&data);
        return;
    }

    BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
    GroupQueueInfo* ginfo = bgQueue.AddGroup(player, NULL, bgTypeId, bracketEntry, 0, false, false, 0, 0);

    uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
    uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

    // add joined time data
    player->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

    WorldPacket data; // send status packet (in queue)
    sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, player, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->ArenaType);
    player->GetSession()->SendPacket(&data);

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

    sLog->outDebug(LOG_FILTER_INTERREALM, "Succeed to register a player.");

    SendRegisterPlayer(IR_REG_OK, playerGuid);

    SendAdditionalInfoRequest(playerGuid);

    if (player->GetGuildId())
    {
        uint64 guildGuid = MAKE_NEW_GUID(player->GetGuildId(), 0, HIGHGUID_GUILD);

        if (!sGuildMgr->HasInterRealmGuild(GetRealmId(), guildGuid))
        {
            SendInterRealmGuildQuery(guildGuid);
        }
    }
}

void InterRealmClient::Handle_RegisterGroup(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_REGISTER_GROUP");

    uint8 num;
    uint32 _instanceId, _bgTypeId;
    uint64 _bgGuid;
    std::vector<uint64> playerGuids;

    packet >> num;
    packet >> _instanceId >> _bgGuid >> _bgTypeId;

    if (num > 0)
    {
        for (uint8 i = 0; i < num; ++i)
        {
            uint64 guid;
            packet >> guid;
            playerGuids.push_back(guid);
        }
    }

    if (playerGuids.empty())
        return;

    for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
    {
        if (sWorld->HasPlayer((*itr)) || sWorld->FindPlayerOnMaps((*itr)))
        {
            WorldPacket errPacket(IR_SMSG_REGISTER_PLAYER_RESP, 1);
            errPacket << uint8(IR_REG_ALREADY_REGISTRED);
            errPacket << uint64((*itr));
            SendPacket(&errPacket);

            packet.rfinish();
            return;
        }
    }

    //extract from guid
    uint32 bgTypeId_ = GUID_LOPART(_bgGuid);

    if (!sBattlemasterListStore.LookupEntry(bgTypeId_))
        return;

    BattlegroundTypeId bgTypeId = BattlegroundTypeId(bgTypeId_);

    // can do this, since it's battleground, not arena
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);
    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BATTLEGROUND_RB, 0);

    // get bg instance or bg template if instance not found
    Battleground* bg = NULL;
    if (_instanceId)
        bg = sBattlegroundMgr->GetBattlegroundThroughClientInstance(_instanceId, bgTypeId);

    if (!bg)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return;

    std::list<Player*> players;
    for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
    {
        uint64 _playerGuid, _mute_time;
        uint32 _accountId, _bytes1, _bytes2;
        uint8 _security, _isPremium, _expansion, _locale, _recruiter;
        uint8 _level, _race, _class, _gender, _bg_status;
        std::string _name;

        _playerGuid = (*itr);

        packet >> _accountId;
        packet >> _security;
        packet >> _isPremium;
        packet >> _expansion;
        packet >> _mute_time;
        packet >> _locale;
        packet >> _recruiter;

        WorldSession* _session = NULL;
        Player* _player = NULL;

        _session = new WorldSession(_accountId, this, AccountTypes(_security), (bool)_isPremium, _expansion, _mute_time, LocaleConstant(_locale), _recruiter, false, GetRealmName());

        for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
            _session->SetTutorialInt(i, 0);
      
        packet >> _name;
        packet >> _level;
        packet >> _race;
        packet >> _class;
        packet >> _bytes1;
        packet >> _bytes2;
        packet >> _gender;

        uint32 guildId, guildLevel;
        uint8 guildRank;

        packet >> guildId;
        packet >> guildRank;
        packet >> guildLevel;

        packet >> _bg_status;

        

        CharacterCreateInfo createInfo(_name, _level, _race, _class, _gender, _bytes1, _bytes2, 0);
        _player = new Player(_session);

        if (!_player->Create(GUID_LOPART(_playerGuid), &createInfo))
        {
            for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* member = (*itr);
                WorldSession* pSession = member->GetSession();

                sObjectAccessor->RemoveObject(member);
                IRPlayers::iterator itr2 = m_Players.find(member->GetRealGUID());
                if (itr2 != m_Players.end())
                    m_Players.erase(itr2);
                delete pSession;
                delete member;
            }
            sObjectAccessor->RemoveObject(_player);
            delete _session;
            delete _player;

            packet.rfinish();
            return;
        }

        _player->SetRealGUID(_playerGuid);
        _player->SetLevel(_level);
        _player->GetMotionMaster()->Initialize();

        if (guildId)
            _player->SetInGuild(guildId);

        if (guildRank)
            _player->SetRank(guildRank);

        if (guildLevel)
            _player->SetGuildLevel(guildLevel);

        // Set bg winner status
        _player->SetRandomWinner((_bg_status == 1) ? true : false);

        ReadOrSkipArenaInfoBlock(packet, _player);

        _session->SetPlayer(_player); 

        players.push_back(_player); 
    }

    Group* group = NULL;
    Player* pLeader = NULL;
    for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* pPlayer = (*itr);

        sObjectAccessor->AddObject(pPlayer);
        m_Players[pPlayer->GetRealGUID()] = pPlayer;
        sWorld->AddPlayer(pPlayer);
        
        if (group == NULL)
        {
            group = new Group();
            group->Create(pPlayer);
            sGroupMgr->AddGroup(group);
            pLeader = pPlayer;
        }
        else
        {
            group->AddMember(pPlayer);
        }
    }

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), pLeader->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

    err = group->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, bg->GetMaxPlayersPerTeam(), false, 0);
    bool isPremade = (group->GetMembersCount() >= bg->GetMinPlayersPerTeam());

    BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
    GroupQueueInfo* ginfo = NULL;
    uint32 avgTime = 0;

    if (err)
    {
        for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* member = (*itr);

            //member->GetSession()->SetInterRealmClient(NULL);
            member->SetNeedRemove(true);
        }

        delete group;
        return;
    }
    
    ginfo = bgQueue.AddGroup(pLeader, group, bgTypeId, bracketEntry, 0, false, isPremade, 0, 0);
    avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());

    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();

        // add to queue
        uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

        // add joined time data
        member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->ArenaType);
        member->GetSession()->SendPacket(&data);

        SendAdditionalInfoRequest(member->GetGUID());
    }

    // delete group
    //group->Disband(true);

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

    sLog->outDebug(LOG_FILTER_INTERREALM, "Succeed to register a group.");
}

void InterRealmClient::Handle_RegisterArena(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_REGISTER_ARENA");

    uint8 num, _arenaSlot;
    std::vector<uint64> playerGuids;

    packet >> num;
    packet >> _arenaSlot;

    if (num > 0)
    {
        for (uint8 i = 0; i < num; ++i)
        {
            uint64 guid;
            packet >> guid;
            playerGuids.push_back(guid);
        }
    }

    if (playerGuids.empty())
        return;

    for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
    {
        if (sWorld->HasPlayer((*itr)) || sWorld->FindPlayerOnMaps((*itr)))
        {
            WorldPacket errPacket(IR_SMSG_REGISTER_PLAYER_RESP, 1);
            errPacket << uint8(IR_REG_ALREADY_REGISTRED);
            errPacket << uint64((*itr));
            SendPacket(&errPacket);

            packet.rfinish();
            return;
        }
    }

    uint8 arenatype = Arena::GetTypeBySlot(_arenaSlot);

    //check existance
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
    if (!bg)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Battleground: template bg (all arenas) not found");
        return;
    }

    std::list<Player*> players;
    for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
    {
        uint64 _playerGuid, _mute_time;
        uint32 _accountId, _bytes1, _bytes2;
        uint8 _security, _isPremium, _expansion, _locale, _recruiter;
        uint8 _level, _race, _class, _gender, _bg_status;
        std::string _name;

        _playerGuid = (*itr);

        packet >> _accountId;
        packet >> _security;
        packet >> _isPremium;
        packet >> _expansion;
        packet >> _mute_time;
        packet >> _locale;
        packet >> _recruiter;

        WorldSession* _session = NULL;
        Player* _player = NULL;

        _session = new WorldSession(_accountId, this, AccountTypes(_security), (bool)_isPremium, _expansion, _mute_time, LocaleConstant(_locale), _recruiter, false, GetRealmName());

        for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
            _session->SetTutorialInt(i, 0);
      
        packet >> _name;
        packet >> _level;
        packet >> _race;
        packet >> _class;
        packet >> _bytes1;
        packet >> _bytes2;
        packet >> _gender;

        uint32 guildId, guildLevel;
        uint8 guildRank;

        packet >> guildId;
        packet >> guildRank;
        packet >> guildLevel;

        packet >> _bg_status;

        CharacterCreateInfo createInfo(_name, _level, _race, _class, _gender, _bytes1, _bytes2, 0);
        _player = new Player(_session);

        if (!_player->Create(GUID_LOPART(_playerGuid), &createInfo))
        {
            for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* member = (*itr);
                WorldSession* pSession = member->GetSession();

                sObjectAccessor->RemoveObject(member);
                IRPlayers::iterator itr2 = m_Players.find(member->GetRealGUID());
                if (itr2 != m_Players.end())
                    m_Players.erase(itr2);

                delete pSession;
                delete member;
            }
            sObjectAccessor->RemoveObject(_player);
            delete _session;
            delete _player;

            packet.rfinish();
            return;
        }

        _player->SetRealGUID(_playerGuid);
        _player->SetLevel(_level);
        _player->GetMotionMaster()->Initialize();

        if (guildId)
            _player->SetInGuild(guildId);

        if (guildRank)
            _player->SetRank(guildRank);

        if (guildLevel)
            _player->SetGuildLevel(guildLevel);

        // Set bg winner status
        _player->SetRandomWinner((_bg_status == 1) ? true : false);

        ReadOrSkipArenaInfoBlock(packet, _player);

        _session->SetPlayer(_player); 

        players.push_back(_player); 
    }

    Group* group = NULL;
    Player* pLeader = NULL;
    for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* pPlayer = (*itr);

        sObjectAccessor->AddObject(pPlayer);
        m_Players[pPlayer->GetRealGUID()] = pPlayer;
        sWorld->AddPlayer(pPlayer);
        
        if (group == NULL)
        {
            group = new Group();
            group->Create(pPlayer);
            sGroupMgr->AddGroup(group);
            pLeader = pPlayer;
        }
        else
        {
            group->AddMember(pPlayer);
        }
    }
            
    BattlegroundTypeId bgTypeId = bg->GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
    
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), pLeader->getLevel());
    if (!bracketEntry)
        return;
    
    uint32 arenaRating = 0;
    uint32 matchmakerRating = 0;
    uint32 playerDivider = 0;
    for (GroupReference const* ref = group->GetFirstMember(); ref != NULL; ref = ref->next())
    {
        if (Player const* groupMember = ref->getSource())
        {
            arenaRating += groupMember->GetArenaPersonalRating(_arenaSlot);
            matchmakerRating += groupMember->GetArenaMatchMakerRating(_arenaSlot);
            ++playerDivider;
        }
    }

    if (!playerDivider)
        return;

    arenaRating /= playerDivider;
    matchmakerRating /= playerDivider;

    if (arenaRating <= 0)
        arenaRating = 1;
    if (matchmakerRating <= 0)
        matchmakerRating = 1;

    BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    uint32 avgTime = 0;
    GroupQueueInfo* ginfo;
    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE; 
    err = group->CanJoinBattlegroundQueue(bg, bgQueueTypeId, arenatype, arenatype, true, _arenaSlot);
    if (err)
    {
        for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* member = (*itr);
            
            //member->GetSession()->SetInterRealmClient(NULL);
            member->SetNeedRemove(true);
        }

        delete group;

        packet.rfinish();
        return;
    }
        
    bg->SetRated(true);
    ginfo = bgQueue.AddGroup(pLeader, group, bgTypeId, bracketEntry, arenatype, true, false, arenaRating, matchmakerRating);
    avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());

    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();

        uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);
        
        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, arenatype);
        member->GetSession()->SendPacket(&data);

        SendAdditionalInfoRequest(member->GetGUID());
    }

    // delete group
    //group->Disband(true);

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

}

void InterRealmClient::Handle_RegisterRated(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_REGISTER_RATED");

    uint8 num;
    std::vector<uint64> playerGuids;
    uint32 personalRating, matchMakerRating;

    packet >> num;
    packet >> personalRating;
    packet >> matchMakerRating;

    if (num > 0)
    {
        for (uint8 i = 0; i < num; ++i)
        {
            uint64 guid;
            packet >> guid;
            playerGuids.push_back(guid);
        }
    }

    if (playerGuids.empty())
        return;

    for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
    {
        if (sWorld->HasPlayer((*itr)) || sWorld->FindPlayerOnMaps((*itr)))
        {
            SendRegisterRated(IR_REG_ALREADY_REGISTRED, *itr);
            return;
        }
    }

    //check existance
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_RATED_10_VS_10);
    if (!bg)
    {
        SendRegisterRated(IR_REG_CANNOT_CREATE, 0);
        sLog->outError(LOG_FILTER_INTERREALM, "Battleground: template bg (all arenas) not found");
        return;
    }

    std::list<Player*> players;

    for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
    {
        uint64 _playerGuid, _mute_time;
        uint32 _accountId, _bytes1, _bytes2;
        uint8 _security, _isPremium, _expansion, _locale, _recruiter;
        uint8 _level, _race, _class, _gender, _bg_status;
        std::string _name;

        _playerGuid = (*itr);

        packet >> _accountId;
        packet >> _security;
        packet >> _isPremium;
        packet >> _expansion;
        packet >> _mute_time;
        packet >> _locale;
        packet >> _recruiter;

        WorldSession* _session = NULL;
        Player* _player = NULL;

        _session = new WorldSession(_accountId, this, AccountTypes(_security), (bool)_isPremium, _expansion, _mute_time, LocaleConstant(_locale), _recruiter, false, GetRealmName());

        // Set tutorials
        for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
            _session->SetTutorialInt(i, 0);


        // Read base info        
        packet >> _name;
        packet >> _level;
        packet >> _race;
        packet >> _class;
        packet >> _bytes1;
        packet >> _bytes2;
        packet >> _gender;

        uint32 guildId, guildLevel;
        uint8 guildRank;

        packet >> guildId;
        packet >> guildRank;
        packet >> guildLevel;

        packet >> _bg_status;

        CharacterCreateInfo createInfo(_name, _level, _race, _class, _gender, _bytes1, _bytes2, 0);
        _player = new Player(_session);

        if (!_player->Create(GUID_LOPART(_playerGuid), &createInfo))
        {
            for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* member = (*itr);
                WorldSession* pSession = member->GetSession();

                sObjectAccessor->RemoveObject(member);
                IRPlayers::iterator itr2 = m_Players.find(member->GetRealGUID());
                if (itr2 != m_Players.end())
                    m_Players.erase(itr2);

                delete pSession;
                delete member;
            }
            sObjectAccessor->RemoveObject(_player);
            delete _session;
            delete _player;

            packet.rfinish();
            return;
        }

        _player->SetRealGUID(_playerGuid);
        _player->SetLevel(_level);
        _player->GetMotionMaster()->Initialize();

        if (guildId)
            _player->SetInGuild(guildId);

        if (guildRank)
            _player->SetRank(guildRank);

        if (guildLevel)
            _player->SetGuildLevel(guildLevel);

        // Set bg winner status
                
        _player->SetRandomWinner((_bg_status == 1) ? true : false);

        ReadOrSkipArenaInfoBlock(packet, _player);

        _session->SetPlayer(_player);

        players.push_back(_player); 
    }

    Group* group = NULL;
    Player* pLeader = NULL;
    for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* pPlayer = (*itr);

        sObjectAccessor->AddObject(pPlayer);
        m_Players[pPlayer->GetRealGUID()] = pPlayer;
        sWorld->AddPlayer(pPlayer);
        
        if (group == NULL)
        {
            group = new Group();
            group->Create(pPlayer);
            sGroupMgr->AddGroup(group);
            pLeader = pPlayer;
        }
        else
        {
            group->AddMember(pPlayer);
        }
    }
            
    uint32 arenaRating = 0;

    BattlegroundTypeId bgTypeId = bg->GetTypeID();

    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), pLeader->getLevel());
    if (!bracketEntry)
    {
        SendRegisterRated(IR_REG_CANNOT_CREATE, 0);
        return;
    }

    BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    uint32 avgTime = 0;
    GroupQueueInfo* ginfo;
    GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE; 
    err = group->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 10, 10, false, 0);
    if (err)
    {
        for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* member = (*itr);
            
            //member->GetSession()->SetInterRealmClient(NULL);
            member->SetNeedRemove(true);
        }

        delete group;

        packet.rfinish();
        return;
    }
        
    ginfo = bgQueue.AddGroup(pLeader, group, bgTypeId, bracketEntry, 0, true, false, personalRating, matchMakerRating);
    avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());

    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();

        // add to queue
        uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);
        member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, 0);
        member->GetSession()->SendPacket(&data);
    }

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void InterRealmClient::Handle_RegisterSpectator(WorldPacket& packet)
{
    uint64 playerGuid;
    uint64 targetGuid;
    
    packet >> playerGuid;
    packet >> targetGuid;

    if (playerGuid == 0)
    {
        sLog->outInfo(LOG_FILTER_WORLDSERVER, "Wrong player guid (%u), realm %u %s",
            playerGuid, GetRealmId(), GetRealmName().c_str());
        ASSERT(false);
    }

    if (sWorld->HasPlayer(playerGuid))
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to register a spectator becouse he is already at the cross-server.");
        packet.rfinish();

        SendRegisterPlayer(IR_REG_ALREADY_REGISTRED, playerGuid);
        return;
    }

    Player* targetPlayer = sObjectAccessor->FindPlayer(targetGuid);
    if (!targetPlayer || targetPlayer->IsNeedRemove())
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to register a spectator becouse targeted player is not at the cross-server.");
        packet.rfinish();
        return;
    }

    if (!targetPlayer->GetBattlegroundId() || !targetPlayer->GetBattleground())
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to register a spectator becouse targeted player is not at the arena.");
        packet.rfinish();
        return;
    }

    Player* player = CreatePlayer(packet, playerGuid);
    if (!player)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Failed to create a spectator.");
        packet.rfinish();

        SendRegisterPlayer(IR_REG_CANNOT_CREATE, playerGuid);
        return;
    }

    // learn default spells to enable chat commands
    player->learnDefaultSpells();

    uint32 bgZoneId = targetPlayer->GetBattleground()->GetZoneId();

    sObjectAccessor->AddObject(player);
    m_Players[player->GetRealGUID()] = player;
    sWorld->AddPlayer(player);

    sLog->outDebug(LOG_FILTER_INTERREALM, "Succeed to register a spectator.");

    SendRegisterSpectator(playerGuid, targetGuid, IR_REG_OK, bgZoneId);
}

void InterRealmClient::Handle_AppearRequest(WorldPacket& packet)
{
    uint64 playerGuid;
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

    SendPacket(&data);
}

void InterRealmClient::Handle_Appear(WorldPacket& packet)
{
    uint64 playerGuid, targetGuid;

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
    player->SetForceCleanupChannels(true);
}

void InterRealmClient::Handle_BattlefieldPort(WorldPacket& packet)
{
    uint64 _playerGuid;
    uint32 _time, _queueSlot;
    uint8 _action;

    packet >> _playerGuid;
    packet >> _time;
    packet >> _queueSlot;
    packet >> _action;

    Player* _player = sObjectAccessor->FindPlayerInOrOutOfWorld(_playerGuid);
    if (!_player)
        return;

    if (!_player->InBattlegroundQueue())
    {
        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, NULL, _player, _queueSlot, STATUS_NONE, 0, 0, 0);
        SendPacket(&data);
        return;
    }

    BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(_queueSlot);
    if (bgQueueTypeId == BATTLEGROUND_QUEUE_NONE)
        return;

    BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);
    BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    //we must use temporary variable, because GroupQueueInfo pointer can be deleted in BattlegroundQueue::RemovePlayer() function
    GroupQueueInfo ginfo;
    if (!bgQueue.GetPlayerGroupInfoData(_player->GetGUID(), &ginfo))
        return;

    // if action == 1, then instanceId is required
    if (!ginfo.IsInvitedToBGInstanceGUID && _action == 1)
        return;

    // BGTemplateId returns BATTLEGROUND_AA when it is arena queue.
    // Do instance id search as there is no AA bg instances.
    Battleground* bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, bgTypeId == BATTLEGROUND_AA ? BATTLEGROUND_TYPE_NONE : bgTypeId);

    // hack
    // try to get random bg with same instance id
    if (!bg && _action == 1)
        bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, BATTLEGROUND_RB);

    // bg template might and must be used in case of leaving queue, when instance is not created yet
    if (!bg && _action == 0)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    
    if (!bg)
        return;

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    //some checks if player isn't cheating - it is not exactly cheating, but we cannot allow it
    if (_action == 1 && ginfo.ArenaType == 0)
    {
        //if player don't match battleground max level, then do not allow him to enter! (this might happen when player leveled up during his waiting in queue
        if (_player->getLevel() > bg->GetMaxLevel())
            _action = 0;
    }

    WorldPacket data;
    switch (_action)
    {
        case 1: 
        {
            // port to battleground
            if (!_player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
                return;                                 // cheating?

            if (!_player->InBattleground())
                _player->SetBattlegroundEntryPoint();

            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, _queueSlot, STATUS_IN_PROGRESS, _player->GetBattlegroundQueueJoinTime(bgTypeId), bg->GetElapsedTime(), bg->GetArenaType());
            _player->GetSession()->SendPacket(&data);
            // remove battleground queue status from BGmgr
            bgQueue.RemovePlayer(_player->GetGUID(), false);

            uint32 otherIndex = _player->GetOtherBattlegroundQueueIndex(bgQueueTypeId);
            if (otherIndex != PLAYER_MAX_BATTLEGROUND_QUEUES)
            {
                BattlegroundQueueTypeId otherQueueId = _player->GetBattlegroundQueueTypeId(otherIndex);
                BattlegroundQueue& otherQueue = sBattlegroundMgr->GetBattlegroundQueue(otherQueueId);
                otherQueue.RemovePlayer(_player->GetGUID(), false);
                _player->RemoveBattlegroundQueueId(otherQueueId);
            }

            // set the destination instance id
            _player->SetBattlegroundId(bg->GetInstanceID(), bg->GetTypeID()/*bgTypeId*/);
             // set the destination team
            _player->SetBGTeam(ginfo.Team);

            // Do this after IR_CMSG_BATTLEFIELD_ENTER opcode
            //sBattlegroundMgr->SendToBattleground(_player, ginfo.IsInvitedToBGInstanceGUID, bgTypeId);

            CharacterPortData portData;
            portData.playerGuid = _playerGuid;
            portData.IsInvitedToBGInstanceGUID = ginfo.IsInvitedToBGInstanceGUID;
            portData.bgTypeId = bgTypeId;
            portData.bgZoneId = bg->GetZoneId() ? bg->GetZoneId() : 1;

            _player->GetSession()->LoadCharacter(portData);
            //SendBattlefieldPort(portData);
            break;
        }
        case 0:  
        {
            if (_player->InArena() &&
                (_player->GetBattleground()->GetStatus() == STATUS_WAIT_JOIN || 
                _player->GetBattleground()->GetStatus() == STATUS_IN_PROGRESS))
                    return;

            _player->RemoveBattlegroundQueueId(bgQueueTypeId);  // must be called this way, because if you move this call to queue->removeplayer, it causes bugs
            
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, _queueSlot, STATUS_NONE, _player->GetBattlegroundQueueJoinTime(bgTypeId), 0, 0);
            _player->GetSession()->SendPacket(&data);

            bgQueue.RemovePlayer(_player->GetGUID(), true);
            // player left queue, we should update it - do not update Arena Queue
            if (!ginfo.ArenaType)
                sBattlegroundMgr->ScheduleQueueUpdate(ginfo.ArenaMatchmakerRating, ginfo.ArenaType, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
            
            //_player->GetSession()->RemovePlayer();
            _player->SetNeedRemove(true);
            
            break;
        }
        default:
            break;
    }
}

void InterRealmClient::Handle_BattlefieldEnter(WorldPacket& packet)
{
    sLog->outDebug(LOG_FILTER_INTERREALM, "Received packet IR_CMSG_BATTLEFIELD_ENTER");
    
    uint64 _playerGuid;
    uint32 _playerGuidLow, _instanceId;
    uint16 _bgTypeId;

    packet >> _playerGuid;
    packet >> _playerGuidLow;
    packet >> _instanceId;
    packet >> _bgTypeId;

    if (Player* _player = sObjectAccessor->FindPlayerInOrOutOfWorld(_playerGuid))
    {
        // hack
        if (_player->GetRBGFromNormalMark())
            _bgTypeId = BATTLEGROUND_RB;

        sBattlegroundMgr->SendToBattleground(_player, _instanceId, BattlegroundTypeId(_bgTypeId));
        _player->GetSession()->setInInterRealmBG(true);
        _player->SetForceCleanupChannels(true);

        //_player->GetSession()->SendNameQueryOpcode(_playerGuid);
    }
}

void InterRealmClient::Handle_SpectatorPort(WorldPacket& packet)
{
    uint64 playerGuid, targetGuid;

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
    player->SetForceCleanupChannels(true);
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
    uint32 membersCount, guildLevel;
    uint64 experience = 0;
    uint32 emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor;
    uint32 ranksCount;

    packet >> guildGuid >> guildName;
    packet >> membersCount >> guildLevel >> experience;
    packet >> emblemStyle >> emblemColor >> borderStyle >> borderColor >> backgroundColor;
    packet >> ranksCount;

    InterRealmGuildEmblem* emblem = new InterRealmGuildEmblem(emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor);

    InterRealmGuild* guild = new InterRealmGuild(guildGuid, guildName, membersCount, guildLevel, experience, emblem);

    for (uint32 j = 0; j < ranksCount; ++j)
    {
        uint32 rankId;
        std::string rankName;

        packet >> rankId >> rankName;

        guild->AddRank(new InterRealmGuildRank(rankId, rankName));
    }

    sGuildMgr->AddInterRealmGuild(GetRealmId(), guild);
}

void InterRealmClient::Handle_AccountSpells(WorldPacket& recvPacket)
{
    uint64 playerGuid;

    recvPacket >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        player->GetSession()->GetCharacterCachedSpells().clear();

        uint32 count;

        recvPacket >> count;

        if (count > 0)
        {
            for (uint32 i = 0; i < count; ++i)
            {
                uint32 spellId;
                uint8 active;
                uint8 dependent;
                uint8 disabled;

                recvPacket >> spellId >> active >> dependent >> disabled;

                player->GetSession()->GetCharacterCachedSpells().push_back(CharacterCachedSpell(spellId, active, dependent, disabled));
            }
        }
    }
}

void InterRealmClient::Handle_CharacterSkills(WorldPacket& recvPacket)
{
    uint64 playerGuid;

    recvPacket >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        player->GetSession()->GetCharacterCachedSkills().clear();

        uint32 count;

        recvPacket >> count;

        if (count > 0)
        {
            for (uint32 i = 0; i < count; ++i)
            {
                uint32 skill;
                uint32 value;
                uint32 maxValue;

                recvPacket >> skill >> value >> maxValue;

                player->GetSession()->GetCharacterCachedSkills().push_back(CharacterCachedSkill(skill, value, maxValue));
            }
        }
    }
}

void InterRealmClient::SendBattlefieldPort(CharacterPortData const data)
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