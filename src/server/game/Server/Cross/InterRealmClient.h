#ifdef CROSS
#ifndef __INTERREALM_CLIENT_H__
#define __INTERREALM_CLIENT_H__

#ifndef INVALID_SOCKET
#define NOT_WINDOWS 1

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#endif

#include "InterRealmDb.h"
#include "WorldPacket.h"
#include "Define.h"
#include "DatabaseEnv.h"
#include "DatabaseWorkerPool.h"
#include <LockedMap.h>

struct CharacterPortData;

struct IRInPktHeader
{
    uint32 size;
    uint32 cmd;
};

struct IROutPktHeader
{
    /**
     * size is the length of the payload _plus_ the length of the opcode
     */
    IROutPktHeader(uint32 size, uint32 cmd) : size(size)
    {
        uint8 headerIndex=0;
        
        header[headerIndex++] = 0xFF & size;
        header[headerIndex++] = 0xFF &(size >> 8);
        header[headerIndex++] = 0xFF &(size >> 16);
        header[headerIndex++] = 0xFF &(size >> 24);

        header[headerIndex++] = 0xFF & cmd;
        header[headerIndex++] = 0xFF & (cmd >> 8);
        header[headerIndex++] = 0xFF & (cmd >> 16);
        header[headerIndex++] = 0xFF & (cmd >> 24);
    }

    uint8 getHeaderLength()
    {
        // cmd = 4 bytes, size= 4 bytes
        return 4 + 4;
    }

    const uint32 size;
    uint8 header[8];
};

enum irHeloResp
{
    IR_HELO_RESP_OK                 = 0,
    IR_HELO_RESP_POLITE             = 1,
    IR_HELO_RESP_PROTOCOL_MISMATCH  = 2,
};

enum irWhoAmIResp
{
    IR_WHO_AM_I_RESP_OK             = 0,
    IR_WHO_AM_I_RESP_DISABLED       = 1,
};

enum irRegisterResp
{
    IR_REG_OK                   = 0,
    IR_REG_ALREADY_REGISTRED    = 1,
    IR_REG_GROUP_NO_GUIDS       = 2,
    IR_REG_CANNOT_CREATE        = 3,
    IR_REG_CANNOT_LOAD          = 4,
};

class IRSocket;
class ArenaTeam;
class Player;

typedef std::map<uint64 /*realGuid*/, Player*> IRPlayers;

class InterRealmClient
{
    public:
        InterRealmClient(IRSocket* socket);
        ~InterRealmClient();
        
        //void run();

        void Handle_Unhandled(WorldPacket& recvPacket);
        void Handle_Null(WorldPacket& recvPacket);
        void Handle_ServerSide(WorldPacket& recvPacket) { }
        
        void Handle_TunneledPacket(WorldPacket* recvPacket);

        void Handle_Hello(WorldPacket& recvPacket);
        void Handle_WhoAmI(WorldPacket& packet);
        void Handle_CheckPlayers(WorldPacket& recvPacket);

        void Handle_PlayerLogout(WorldPacket& recvPacket);

        void Handle_DebugBG(WorldPacket& recvPacket);
        void Handle_DebugArena(WorldPacket&/* recvPacket*/);

        void Handle_RegisterPlayer(WorldPacket& recvPacket);
        void Handle_RegisterGroup(WorldPacket& recvPacket);
        void Handle_RegisterArena(WorldPacket& recvPacket);
        void Handle_RegisterRated(WorldPacket& recvPacket);
        //void Handle_RegisterSpectator(WorldPacket& recvPacket);
        
        void Handle_BattlefieldPort(WorldPacket& recvPacket);
        void Handle_BattlefieldEnter(WorldPacket& recvPacket);
        void Handle_BattlefielLeave(WorldPacket& p_RecvPacket);
        void Handle_SpectatorPort(WorldPacket& recvPacket);

        void Handle_BattlegroundListQuery(WorldPacket& recvPacket);
        void Handle_BattlefieldStatusQuery(WorldPacket& recvPacket);

        void Handle_ArenaTeamCreate(WorldPacket& recvPacket);
        void Handle_ArenaTeamRoster(WorldPacket& recvPacket);
        void Handle_ArenaTeamQuery(WorldPacket& recvPacket);
        void Handle_ArenaTeamLoad(WorldPacket& recvPacket);
        void Handle_ArenaTeamInvite(WorldPacket& recvPacket);
        void Handle_ArenaTeamAccept(WorldPacket& recvPacket);
        void Handle_ArenaTeamLeave(WorldPacket& recvPacket);
        void Handle_ArenaTeamDisband(WorldPacket& recvPacket);
        void Handle_ArenaTeamRemove(WorldPacket& recvPacket);
        void Handle_ArenaTeamLeader(WorldPacket& recvPacket);
        void Handle_ArenaTeamInspect(WorldPacket& recvPacket);

        void Handle_CurrencyCapLoad(WorldPacket& recvPacket);
        void Handle_CurrencyCapReset(WorldPacket& recvPacket);

        void Handle_Whisper(WorldPacket& recvPacket);

        void Handle_TunneledClient(WorldPacket& recvPacket);

        void Handle_ServerAnnounce(WorldPacket& recvPacket);

        void Handle_TeleportPlayer(WorldPacket& recvPacket);

        void Handle_RatedBattlegroundStats(WorldPacket& recvPacket);

        void Handle_BattlegroundHoliday(WorldPacket& recvPacket);

        void Handle_SummonPlayer(WorldPacket& recvPacket);

        void Handle_AppearRequest(WorldPacket& recvPacket);
        void Handle_Appear(WorldPacket& recvPacket);

        void Handle_PlayerReconnect(WorldPacket& p_Packet);
        void Handle_PlayerReconnectReadyToLoad(WorldPacket& p_Packet);

        void Handle_AdditionalInfo(WorldPacket& recvPacket);
        void SendAdditionalInfoRequest(uint64 playerGuid);

        void Handle_Guild(WorldPacket& recvPacket);
        void Handle_ReserveLocalGuid(WorldPacket& p_Packet);

        void SendArenaTeamCommandResult(uint32 team_action, const std::string& team, const std::string& player, uint32 error_id, uint64 targetGUID);
        void BroadcastEvent(uint8 event, uint64 guid, uint8 strCount, std::string const str1, std::string const str2, std::string const str3, ArenaTeam *team);
        void SendUpdateArenaFields(uint32 slot, uint32 type, uint32 id, uint8 isMember, uint64 playerGuid);
        void SendUpdateArenaFieldsPartTwo(uint32 slot, uint32 weekGames, uint32 seasonGames,uint32 seasonWins, uint64 playerGuid);
        
        void SendBGQueueAnnouncer(const char* bgName, uint32 q_min_level, uint32 q_max_level,
            uint32 qAlliance, uint32 qAllianceDiff, uint32 qHorde, uint32 qHordeDiff, uint64 guid = 0);
        void SendArenaQueueAnnouncer(uint32 arenaType, uint32 teamRating);

        void SendAchievementReward(uint64 playerGuid, uint32 achievement);

        void SendAnticheatReport(uint32 guidLow, uint8 reportType);

        void SendInterRealmGuildQuery(uint64 guildGuid);

        void SendWhisper(uint64 const &senderGUID, uint32 const &language, std::string const &targetName, std::string const &message);
        void SendGuildChat(uint64 const &playerGuid, uint32 const &language, std::string const &message);
        void SendPlayerBattlegroundZoneID(Player const *player);
        void SendPlayerPinfoCommand(uint64 const &playerGUID, std::string const &playerName);
        void SendPlayerBanCharCommand(uint64 const &playerGUID, std::string const &name, std::string const &duration, std::string const &reason, std::string const &gmName);
        void SendPlayerBanCommand(uint64 const &playerGUID, uint8 const &banMode, std::string const &nameOrIp, std::string const &duration, std::string const &reason, std::string const &gmName);
        void SendPlayerUnbanCharCommand(uint64 const &playerGUID, std::string const &name, std::string const &gmName);
        void SendPlayerUnbanCommand(uint64 const &playerGUID, uint8 const &banMode, std::string const &nameOrIp, std::string const &gmName);
        void SendKickPlayer(uint64 gmGUID, std::string const &name);
        void SendSummonPlayer(uint64 const &sender, std::string const &name);
        
        void SendBattlefieldLeave(uint64 targetGuid);

        void SendBattlefieldPort(CharacterPortData const& portData);
        void SendRegisterPlayer(uint8 answer, uint64 targetGuid);
        void SendRegisterRated(uint8 answer, uint64 targetGuid);
        void SendRegisterSpectator(uint64 playerGuid, uint64 targetGuid, uint8 answer, uint32 bgZoneId);

        void SendCrossPartyInfo(Player* p_Player);

        void SendPlayerReconnectResult(uint64 p_PlayerGuid, bool p_Result);
        
        void SendPacket(WorldPacket const* packet);
        void SendTunneledPacket(uint64 playerGuid, WorldPacket const* packet, bool forced = false);

        void RemovePlayerFromIR(Player *player);

        void CheckPlayers();
        void Update(const uint32 diff);

        int32 GetRealmId() const { return m_realmId; }
        std::string GetRealmName() const;
        float GetHonorRate() const { return m_rate_honor; }
        float GetHonorPremiumRate() const { return m_rate_honor_premium; }
        float GetReputationRate() const { return m_rate_reputation; }
        float GetReputationPremiumRate() const { return m_rate_reputation_premium; }
 
        bool IsNeedClose() const { return m_isNeedClose; }
        void SetNeedClose(bool value) { m_isNeedClose = value; }
        void RemoveAllPlayers();

        void AddPacket(WorldPacket* new_packet);

        InterRealmDatabasePool* GetDatabase() { return &InterRealmDatabase; }
        bool IsDatabaseOpened() const { return _isDatabaseOpened; }

        IRPlayers const& GetIRPlayers() const { return m_Players; }
        void RemovePlayer(uint64 guid);

        uint32 GenerateLocalRealmLowGuid(HighGuid p_HighGuid);

    public:
        static InterRealmClient *GetIRClient(Player *player);

    private:

        Player* CreatePlayer(WorldPacket& packet, uint64 playerGuid);
        Player* CreatePlayerForGroup(WorldPacket& packet);

        void ProcessWorldSessionPacket(WorldSession* _session, WorldPacket* packet);

        void UpdateDatabaseConnection(const uint32 diff);

        void UpdateCheckPlayers(const uint32 diff);

        void ProcessPackets();

        void SendReserveLocalGuidRequest(HighGuid p_HighGuid, uint32 p_Range);

        ACE_Based::LockedQueue<uint64, ACE_Thread_Mutex> _removeQueue;

        int32 m_realmId;
        float m_rate_honor;
        float m_rate_honor_premium;
        float m_rate_reputation;
        float m_rate_reputation_premium;

        uint32 m_check_players_timer;
        uint32 m_ping_mysql_timer;
        uint32 m_remove_players_timer;

        IRPlayers m_Players;

        ACE_Based::LockedQueue<WorldPacket *, ACE_Thread_Mutex> _queue;

        InterRealmDatabasePool InterRealmDatabase;
        bool _isDatabaseOpened;

        bool m_isNeedClose;

        IRSocket* m_IRSocket;

        /// Local realm guids
        ACE_Based::LockedMap<uint32, bool> m_WaitingFoLocalRealmGuid;

        /// Item
        ACE_Based::LockedQueue<uint32, ACE_Thread_Mutex> m_ItemLocalRealmGuidReserve;
        uint32 m_LocalRealmItemGuidReserveSize;
        uint32 m_LocalRealmItemGuidReserveMin;

        /// Mail
        ACE_Based::LockedQueue<uint32, ACE_Thread_Mutex> m_MailLocalRealmGuidReserve;
        uint32 m_LocalRealmMailGuidReserveSize;
        uint32 m_LocalRealmMailGuidReserveMin;

        /// Pet
        ACE_Based::LockedQueue<uint32, ACE_Thread_Mutex> m_PetLocalRealmGuidReserve;
        uint32 m_LocalRealmPetGuidReserveSize;
        uint32 m_LocalRealmPetGuidReserveMin;

        /// EquipmentSet
        ACE_Based::LockedQueue<uint32, ACE_Thread_Mutex> m_EquipmentSetLocalRealmGuidReserve;
        uint32 m_LocalRealmEquipmentSetGuidReserveSize;
        uint32 m_LocalRealmEquipmentSetGuidReserveMin;
};

#endif
#endif