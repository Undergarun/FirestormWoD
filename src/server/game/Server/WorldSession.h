////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __WORLDSESSION_H
#define __WORLDSESSION_H

#include "Common.h"
#include "SharedDefines.h"
#include "AddonMgr.h"
#include "DatabaseEnv.h"
#include "World.h"
#include "WorldPacket.h"
#include "Cryptography/BigNumber.h"
#include "Opcodes.h"
#include "LFGListMgr.h"
#include "MSCallback.hpp"
#ifdef CROSS
#include "Cross/InterRealmClient.h"
#endif /* CROSS */

class Creature;
class GameObject;
class InstanceSave;
class Item;
class Object;
class Player;
class Quest;
class SpellCastTargets;
class Unit;
class Warden;
class WorldPacket;
class WorldSocket;
struct AreaTableEntry;
struct AuctionEntry;
struct DeclinedName;
struct ItemTemplate;
struct LfgJoinResultData;
struct LfgLockStatus;
struct LfgPlayerBoot;
struct LfgProposal;
struct LfgReward;
struct LfgRoleCheck;
struct LfgUpdateData;
struct MovementInfo;
struct PetBattleRequest;
class PetBattle;

enum AccountDataType
{
    GLOBAL_CONFIG_CACHE             = 0,                    // 0x01 g
    PER_CHARACTER_CONFIG_CACHE      = 1,                    // 0x02 p
    GLOBAL_BINDINGS_CACHE           = 2,                    // 0x04 g
    PER_CHARACTER_BINDINGS_CACHE    = 3,                    // 0x08 p
    GLOBAL_MACROS_CACHE             = 4,                    // 0x10 g
    PER_CHARACTER_MACROS_CACHE      = 5,                    // 0x20 p
    PER_CHARACTER_LAYOUT_CACHE      = 6,                    // 0x40 p
    PER_CHARACTER_CHAT_CACHE        = 7,                    // 0x80 p
};

#define NUM_ACCOUNT_DATA_TYPES        8

#define GLOBAL_CACHE_MASK           0x15
#define PER_CHARACTER_CACHE_MASK    0xAA

#define REGISTERED_ADDON_PREFIX_SOFTCAP 64

struct AccountData
{
    AccountData() : Time(0), Data("") {}

    time_t Time;
    std::string Data;
};

struct BattlegroundPortData
{
    uint64 PlayerGuid;
    uint32 Time;
    uint32 QueueSlot;
    uint8 Action;
};

struct CrossPartyInfo
{
    CrossPartyInfo()
    {
        memset(this, 0, sizeof(CrossPartyInfo));
    }

    uint64 PlayerGuid;
    uint64 GroupGUID;
    uint64 LeaderGUID;
    uint8  PartyFlags;
    uint8  PartyIndex;
    uint8  PartyType;
};

struct CharacterPortData
{
    uint64 playerGuid;
    uint32 IsInvitedToBGInstanceGUID;
    uint16 bgTypeId;
    uint32 bgZoneId;
};

class LoginQueryHolder : public SQLQueryHolder
{
    private:
        uint32 m_accountId;
        uint64 m_guid;
        uint32 m_realmId;
        CharacterPortData m_CharacterPortData;
    public:
        LoginQueryHolder(uint32 accountId, uint64 guid, uint32 realmId, CharacterPortData const& characterPortData)
            : m_accountId(accountId), m_guid(guid), m_realmId(realmId), m_CharacterPortData(characterPortData) { }
        uint64 GetGuid() const { return m_guid; }
        uint32 GetAccountId() const { return m_accountId; }
        uint32 GetRealmId() const { return m_realmId; }
        CharacterPortData const& GetCharacterPortData() const { return m_CharacterPortData; }
        bool Initialize();
};

class LoginDBQueryHolder : public SQLQueryHolder
{
    private:
        uint32 m_AccountId;
    public:
        LoginDBQueryHolder(uint32 p_AccountId)
            : m_AccountId(p_AccountId) { }
        uint32 GetAccountId() const { return m_AccountId; }
        bool Initialize();
};


enum PartyCommand
{
    PARTY_CMD_INVITE   = 0,
    PARTY_CMD_UNINVITE = 1,
    PARTY_CMD_LEAVE    = 2,
    PARTY_CMD_SWAP     = 4
};

enum PartyResult
{
    ERR_PARTY_RESULT_OK                      = 0,
    ERR_BAD_PLAYER_NAME_S                    = 1,
    ERR_TARGET_NOT_IN_GROUP_S                = 2,
    ERR_TARGET_NOT_IN_INSTANCE_S             = 3,
    ERR_GROUP_FULL                           = 4,
    ERR_ALREADY_IN_GROUP_S                   = 5,
    ERR_NOT_IN_GROUP                         = 6,
    ERR_NOT_LEADER                           = 7,
    ERR_PLAYER_WRONG_FACTION                 = 8,
    ERR_IGNORING_YOU_S                       = 9,
    ERR_LFG_PENDING                          = 12,
    ERR_INVITE_RESTRICTED                    = 13,
    ERR_GROUP_SWAP_FAILED                    = 14,               // if (PartyOperation == PARTY_OP_SWAP) ERR_GROUP_SWAP_FAILED else ERR_INVITE_IN_COMBAT
    ERR_INVITE_UNKNOWN_REALM                 = 15,
    ERR_INVITE_NO_PARTY_SERVER               = 16,
    ERR_INVITE_PARTY_BUSY                    = 17,
    ERR_PARTY_TARGET_AMBIGUOUS               = 18,
    ERR_PARTY_LFG_INVITE_RAID_LOCKED         = 19,
    ERR_PARTY_LFG_BOOT_LIMIT                 = 20,
    ERR_PARTY_LFG_BOOT_COOLDOWN_S            = 21,
    ERR_PARTY_LFG_BOOT_IN_PROGRESS           = 22,
    ERR_PARTY_LFG_BOOT_TOO_FEW_PLAYERS       = 23,
    ERR_PARTY_LFG_BOOT_NOT_ELIGIBLE_S        = 24,
    ERR_RAID_DISALLOWED_BY_LEVEL             = 25,
    ERR_PARTY_LFG_BOOT_IN_COMBAT             = 26,
    ERR_VOTE_KICK_REASON_NEEDED              = 27,
    ERR_PARTY_LFG_BOOT_DUNGEON_COMPLETE      = 28,
    ERR_PARTY_LFG_BOOT_LOOT_ROLLS            = 29,
    ERR_PARTY_LFG_TELEPORT_IN_COMBAT         = 30,
    ERR_PARTY_ALREADY_IN_BATTLEGROUND_QUEUE  = 31,
    ERR_PARTY_CONFIRMING_BATTLEGROUND_QUEUE  = 32,
    ERR_PARTY_CROSS_REAM_RAID_INVITE         = 33,
    ERR_RAID_DISALLOWED_BY_CROSS_REALM       = 34,
    ERR_PARTY_ROLE_NOT_AVAILABLE             = 35,
    ERR_PARTY_LFG_BOOT_VOTE_REGISTRED        = 36,
    ERR_PARTY_PRIVATE_GROUP_ONLY             = 37
};

enum BFLeaveReason
{
    BF_LEAVE_REASON_CLOSE     = 0x00000001,
    //BF_LEAVE_REASON_UNK1      = 0x00000002, (not used)
    //BF_LEAVE_REASON_UNK2      = 0x00000004, (not used)
    BF_LEAVE_REASON_EXITED    = 0x00000008,
    BF_LEAVE_REASON_LOW_LEVEL = 0x00000010,
};

enum ChatRestrictionType
{
    ERR_CHAT_RESTRICTED = 0,
    ERR_CHAT_THROTTLED  = 1,
    ERR_USER_SQUELCHED  = 2,
    ERR_YELL_RESTRICTED = 3
};

enum CharterTypes
{
    GUILD_CHARTER_TYPE                            = 4,
    ARENA_TEAM_CHARTER_2v2_TYPE                   = 2,
    ARENA_TEAM_CHARTER_3v3_TYPE                   = 3,
    ARENA_TEAM_CHARTER_5v5_TYPE                   = 5,
};

enum DB2Types : uint32
{
    DB2_REPLY_SPARSE                        = 0x919BE54E,   // Hash of item-sparse.db2
    DB2_REPLY_ITEM                          = 0x50238EC2,   // Hash of item.db2
    DB2_REPLY_SCENE_SCRIPT                  = 0xD4B163CC,   // Hash of ScreneScript.db2
    DB2_REPLY_BATTLE_PET_EFFECT_PROPERTIES  = 0x63B4C4BA,   // Hash of BattlePetEffectProperties.db2
    DB2_REPLY_BROADCAST_TEXT                = 0x021826BB,   // Hash of BroadcastText.db2
    DB2_REPLY_ITEM_EXTENDED_COST            = 0xBB858355,   // Hash of ItemExtendedCost.db2
    DB2_REPLY_MAP_CHALLENGE_MODE            = 0x383B4C27,   // Hash of MapChallengeMode.db2
    DB2_REPLY_QUEST_PACKAGE_ITEM            = 0xCC2F84F0,   // Hash of QuestPackageItem.db2
};

#define VOTE_BUFF           176151
#define VOTE_SYNC_TIMER     600000      ///< 10 mins

namespace ServiceFlags
{
    enum
    {
        Premade     = 0x01,
        Season2Gold = 0x02,
        Season2Item = 0x04
    };
}

namespace AccountCustomFlags
{
    enum
    {
        NoChatLocaleFiltering = 0x1
    };
}

namespace Taxi
{
    class ShowTaxiNodes;
    class TaxiNodeStatusQuery;
    class EnableTaxiNode;
    class TaxiQueryAvailableNodes;
    class ActivateTaxi;
    class TaxiRequestEarlyLanding;
}

enum class LoyaltyEvent : uint8
{
    Battleground,
    Arena,
    DungeonFinder,
    BattlePet,
    KillRareOrBossCreature,
    Achievement,
    Craft,
    EpicCraft,
    GrabRessource,
    Duel,
    Max
};

static const uint32 g_LoyaltyEventObjectives[(uint32)LoyaltyEvent::Max] =
{
    1,	///< Complete battleground
    5,	///< Complete arena
    1,  ///< Complete dungeon finder
    5,  ///< Do pet battles
    1,  ///< Kill rare creature
    3,  ///< Complete new achievement
    5,  ///< Craft items
    1,	///< Craft epic item
    5,  ///< Grab ressources
    5   ///< Do duel
};

static const uint32 g_LoyaltyEventReward[(uint32)LoyaltyEvent::Max] =
{
    2,	///< Complete battleground
    2,	///< Complete arena
    2,  ///< Complete dungeon finder
    1,  ///< Do pet battles
    1,  ///< Kill rare creature
    1,  ///< Complete new achievement
    1,  ///< Craft items
    1,	///< Craft epic item
    1,  ///< Grab ressources
    1   ///< Do duel
};

//class to deal with packet processing
//allows to determine if next packet is safe to be processed
class PacketFilter
{
public:
    explicit PacketFilter(WorldSession* pSession) : m_pSession(pSession) {}
    virtual ~PacketFilter() {}

    virtual bool Process(WorldPacket* /*packet*/) { return true; }
    virtual bool ProcessLogout() const { return true; }

    static uint16 DropHighBytes(uint16 opcode) { return uint16(opcode & 0xFFFF); }

protected:
    WorldSession* const m_pSession;
};
//process only thread-safe packets in Map::Update()
class MapSessionFilter : public PacketFilter
{
public:
    explicit MapSessionFilter(WorldSession* pSession) : PacketFilter(pSession) {}
    ~MapSessionFilter() {}

    virtual bool Process(WorldPacket* packet);
    //in Map::Update() we do not process player logout!
    virtual bool ProcessLogout() const { return false; }
};

//class used to filer only thread-unsafe packets from queue
//in order to update only be used in World::UpdateSessions()
class WorldSessionFilter : public PacketFilter
{
public:
    explicit WorldSessionFilter(WorldSession* pSession) : PacketFilter(pSession) {}
    ~WorldSessionFilter() {}

    virtual bool Process(WorldPacket* packet);
};

// Proxy structure to contain data passed to callback function,
// only to prevent bloating the parameter list
class CharacterCreateInfo
{
#ifndef CROSS
    friend class WorldSession;
    friend class Player;

    protected:
        CharacterCreateInfo(std::string name, uint8 race, uint8 cclass, uint8 gender, uint8 skin, uint8 face, uint8 hairStyle, uint8 hairColor, uint8 facialHair, uint8 outfitId, uint32 templateId,
        WorldPacket& data) : Name(name), Race(race), Class(cclass), Gender(gender), Skin(skin), Face(face), HairStyle(hairStyle), HairColor(hairColor), FacialHair(facialHair), OutfitId(outfitId),
        TemplateId(templateId), Data(data), CharCount(0)
#else /* CROSS */
    public:
        CharacterCreateInfo(std::string name, uint8 race, uint8 cclass, uint8 gender, uint8 skin, uint8 face, uint8 hairStyle, uint8 hairColor, uint8 facialHair, uint8 outfitId, uint32 templateId) : 
            Name(name), Race(race), Class(cclass), Gender(gender), Skin(skin), Face(face), HairStyle(hairStyle), HairColor(hairColor), FacialHair(facialHair), OutfitId(outfitId),
        TemplateId(templateId), CharCount(0)
#endif /* CROSS */
        {}

        /// User specified variables
        std::string Name;
        uint8 Race;
        uint8 Class;
        uint8 Gender;
        uint8 Skin;
        uint8 Face;
        uint8 HairStyle;
        uint8 HairColor;
        uint8 FacialHair;
        uint8 OutfitId;
        uint32 TemplateId;
#ifndef CROSS
        WorldPacket Data;
#endif /* not CROSS */

        /// Server side data
        uint8 CharCount;

#ifndef CROSS
    private:
#endif /* not CROSS */
        virtual ~CharacterCreateInfo(){};
};

#ifdef CROSS
#define SessionRealmDatabase (*GetInterRealmClient()->GetDatabase())
#else
#define SessionRealmDatabase CharacterDatabase
#endif
/// Player session in the World
class WorldSession
{
    public:
#ifndef CROSS
        WorldSession(uint32 id, WorldSocket* sock, AccountTypes sec, bool ispremium, uint8 premiumType, uint8 expansion, time_t mute_time, LocaleConstant locale,
                     uint32 recruiter, bool isARecruiter, uint32 p_VoteRemainingTime, uint32 p_ServiceFlags, uint32 p_CustomFlags);
#else /* CROSS */
        WorldSession(uint32 id, InterRealmClient* irc, AccountTypes sec, bool ispremium, uint8 expansion, time_t mute_time, LocaleConstant locale,
            uint32 recruiter, bool isARecruiter, std::string server_name = "");

        void SetPlayerLoading(bool val) { m_playerLoading = val; }
        void SetRemoteAddress(std::string& address) { m_Address = address; }

        void SetBattlegroundLeaveData(uint64 guid, uint8 desertir)
        {
            _battlegroundLeaveData.PlayerGuid = guid;
            _battlegroundLeaveData.desertir = desertir;

        }

        void SetIRClosing(bool val) { m_ir_closing = val; }
        bool IsIRClosing() { return m_ir_closing; }
        bool isInInterRealmBG() { return m_isinIRBG; }
        void setInInterRealmBG(bool inIRBG) { m_isinIRBG = inIRBG; }
        InterRealmClient* GetInterRealmClient() const { return m_ir_socket; }
        void SetInterRealmClient(InterRealmClient* irc) { m_ir_socket = irc; }
        uint32 GetInterRealmNumber() const { return m_ir_number; }
        void SetInterRealmNumber(uint32 realm) { m_ir_number = realm; }
        std::string GetServerName() { return m_ir_server_name; }
#endif

        ~WorldSession();

        uint64 GetWoWAccountGUID()
        {
            return MAKE_NEW_GUID(GetAccountId(), 0, HIGHGUID_WOW_ACCOUNT);
        }
        uint64 GetBNetAccountGUID()
        {
            return MAKE_NEW_GUID(GetAccountId(), 0, HIGHGUID_BNET_ACCOUNT);
        }

        bool PlayerLoading() const { return m_playerLoading; }
        bool PlayerLogout() const { return m_playerLogout; }
        bool PlayerLogoutWithSave() const { return m_playerLogout && m_playerSave; }
        bool PlayerRecentlyLoggedOut() const { return m_playerRecentlyLogout; }

        void ReadAddonsInfo(WorldPacket& data);
        void SendAddonsInfo();
        void SendFeatureSystemStatus();
        void SendTimeZoneInformations();
        bool IsAddonRegistered(const std::string& prefix) const;

        void ReadMovementInfo(WorldPacket& data, MovementInfo* mi);
        static void WriteMovementInfo(WorldPacket& data, MovementInfo* mi);

        void SendPacket(WorldPacket const* packet, bool forced = false, bool ir_packet = false);
        void SendNotification(const char *format, ...) ATTR_PRINTF(2, 3);
        void SendNotification(uint32 string_id, ...);
        void SendPetNameInvalid(uint32 error, const std::string& name, DeclinedName *declinedName);
        void SendPartyResult(PartyCommand p_Command, const std::string& p_Name, PartyResult p_Result, uint32 p_ResultData = 0, uint64 p_ResultGuid = 0);
        void SendSetPhaseShift(const std::set<uint32> & p_PhaseIds, const std::set<uint32> & p_TerrainSwaps, const std::set<uint32> & p_InactiveTerrainSwap);
        void SendQueryTimeResponse();
        void HandleLearnPetSpecialization(WorldPacket& data);

        void SendAuthResponse(uint8 code, bool queued, uint32 queuePos = 0);
        void SendClientCacheVersion(uint32 version);

        AccountTypes GetSecurity() const { return _security; }
        bool IsPremium() const { return _ispremium; }
        uint8 getPremiumType() const { return m_PremiumType; }
        uint32 GetAccountId() const { return _accountId; }
        Player* GetPlayer() const { return m_Player; }
        std::string GetPlayerName(bool simple = true) const;
        uint32 GetGuidLow() const;
        void SetSecurity(AccountTypes security) { _security = security; }
        std::string const& GetRemoteAddress() { return m_Address; }
        void SetPlayer(Player* player);
        uint8 Expansion() const { return m_expansion; }

        void InitWarden(BigNumber* k, std::string os);

        /// Session in auth.queue currently
        void SetInQueue(bool state) { m_inQueue = state; }

        /// Is the user engaged in a log out process?
        bool isLogingOut() const { return _logoutTime || m_playerLogout; }

        /// Engage the logout process for the user
        void LogoutRequest(time_t requestTime)
        {
            _logoutTime = requestTime;
        }

        /// Is logout cooldown expired?
        bool ShouldLogOut(time_t currTime) const
        {
            return (_logoutTime > 0 && currTime >= _logoutTime + 20);
        }

        void LoginPlayer(uint64 p_Guid);
        void LogoutPlayer(bool p_Save, bool p_AfterInterRealm = false);

        void KickPlayer();

        void QueuePacket(WorldPacket* new_packet);
        bool Update(uint32 diff, PacketFilter& updater);

        /// Handle the authentication waiting queue (to be completed)
        void SendAuthWaitQue(uint32 position);

        //void SendTestCreatureQueryOpcode(uint32 entry, uint64 guid, uint32 testvalue);

        void SendNameQueryOpcode(uint64 guid, bool atLeave = false);

        void SendTrainerList(uint64 guid);
        void SendTrainerList(uint64 guid, const std::string& strTitle);
        void SendListInventory(uint64 guid, bool p_FunCustomPlayerConditionFilter = false, bool p_NoFilters = false);
        void SendShowBank(uint64 guid);
        void SendTabardVendorActivate(uint64 guid);
        void SendSpiritResurrect();
        void SendBindPoint(Creature* npc);

        void SendAttackStop(Unit const* enemy);

        void SendBattleGroundList(uint64 guid, BattlegroundTypeId bgTypeId);

        void SendTradeStatus(TradeStatus status);
        void SendUpdateTrade(bool trader_data = true);
        void SendCancelTrade();

        void SendPetitionQueryOpcode(uint64 petitionguid);

        // Pet
        void SendPetNameQuery(uint64 p_PetGUID);
        void SendStablePet(uint64 guid);
        void SendStablePetCallback(PreparedQueryResult result, uint64 guid);
        void SendStableResult(uint8 guid);
        bool CheckStableMaster(uint64 guid);

        // Battle pet
        void HandleSummonBattlePet(WorldPacket& recvData);

        // Account Data
        AccountData* GetAccountData(AccountDataType type) { return &m_accountData[type]; }
        void SetAccountData(AccountDataType type, time_t tm, std::string data);
        void SendAccountDataTimes(uint64 p_Guid);
        void LoadGlobalAccountData();
        void LoadAccountData(PreparedQueryResult result, uint32 mask);

        void LoadTutorialsData();
        void SendTutorialsData();
        void SaveTutorialsData(SQLTransaction& trans);
        uint32 GetTutorialInt(uint8 index) const { return m_Tutorials[index]; }
        void SetTutorialInt(uint8 index, uint32 value)
        {
            if (m_Tutorials[index] != value)
            {
                m_Tutorials[index] = value;
                m_TutorialsChanged = true;
            }
        }
        //used with item_page table
        bool SendItemInfo(uint32 itemid, WorldPacket data);

#ifndef CROSS
        //auction
        void SendAuctionHello(uint64 p_Guid, Creature* p_Unit);
        void SendAuctionCommandResult(AuctionEntry* p_Auction, uint32 p_Action, uint32 p_Error, uint32 p_BidError = 0);
        void SendAuctionBidderNotification(AuctionEntry* p_Auction, uint64 p_Bidder, uint64 p_BidSum);
        void SendAuctionOwnerNotification(AuctionEntry* p_Auction);

        void SendPetitionShowList(uint64 guid);
#endif

        // Item Enchantment
        void SendEnchantmentLog(uint64 Target, uint64 Caster, uint32 ItemID, uint32 enchantID, uint8 slotID);
        void SendItemEnchantTimeUpdate(uint64 Playerguid, uint64 Itemguid, uint32 slot, uint32 Duration);

        //Taxi
        void SendTaxiStatus(uint64 guid);
        void SendTaxiMenu(Creature* unit);
        void SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode = 0);
        bool SendLearnNewTaxiNode(Creature* unit);
        void SendDiscoverNewTaxiNode(uint32 nodeid);

        // Guild/Arena Team
        void SendNotInArenaTeamPacket(uint8 type);
        void SendPetitionSignResult(uint64 p_PlayerGUID, uint64 p_ItemGUID, uint8 p_Result);
        void SendAlreadySigned(uint64 p_PlayerGUID);
        void SendPetitionDeclined(uint64 p_PlayerGUID);

        void BuildPartyMemberStatsChangedPacket(Player* p_Player, WorldPacket* p_Data, uint32 p_Mask, bool p_Ennemy = false);

        void DoLootRelease(uint64 lguid);

        // Account mute time
        time_t m_muteTime;

        // Locales
        LocaleConstant GetSessionDbcLocale() const { return m_sessionDbcLocale; }
        LocaleConstant GetSessionDbLocaleIndex() const { return m_sessionDbLocaleIndex; }
        const char *GetTrinityString(int32 entry) const;

        uint32 GetLatency() const { return m_latency; }
        void SetLatency(uint32 latency) { m_latency = latency; }
        void ResetClientTimeDelay() { m_clientTimeDelay = 0; }
        uint32 getDialogStatus(Player* player, Object* questgiver, uint32 defstatus);

        time_t m_timeOutTime;
        void UpdateTimeOutTime(uint32 diff)
        {
            if (time_t(diff) > m_timeOutTime)
                m_timeOutTime = 0;
            else
                m_timeOutTime -= diff;
        }
        void ResetTimeOutTime() { m_timeOutTime = sWorld->getIntConfig(CONFIG_SOCKET_TIMEOUTTIME); }
        bool IsConnectionIdle() const { return (m_timeOutTime <= 0 && !m_inQueue); }

        // Recruit-A-Friend Handling
        uint32 GetRecruiterId() const { return recruiterId; }
        bool IsARecruiter() const { return isRecruiter; }

        // Antispam Functions
        void UpdateAntispamTimer(uint32 diff)
        {
            if (m_uiAntispamMailSentTimer <= diff)
            {
                m_uiAntispamMailSentTimer = sWorld->getIntConfig(CONFIG_ANTISPAM_MAIL_TIMER);
                m_uiAntispamMailSentCount = 0;
            }
            else
                m_uiAntispamMailSentTimer -= diff;
        }

        bool UpdateAntispamCount()
        {
            if (!sWorld->getBoolConfig(CONFIG_ANTISPAM_ENABLED))
                return true;

            m_uiAntispamMailSentCount++;
            if (m_uiAntispamMailSentCount > sWorld->getIntConfig(CONFIG_ANTISPAM_MAIL_COUNT))
                return false;
            return true;
        }

        z_stream_s* GetCompressionStream() { return _compressionStream; }

        void SetClientBuild(uint16 p_ClientBuild) { m_ClientBuild = p_ClientBuild; }
        uint16 GetClientBuild() const { return m_ClientBuild; }

        /// Return join date as unix timestamp
        uint64 GetAccountJoinDate() const { return m_AccountJoinDate; }

        /// Set join date as unix timestamp
        /// @p_JoinDate : unix timestamp of the account creation
        void SetAccountJoinDate(uint64 p_JoinDate) { m_AccountJoinDate = p_JoinDate; }

        time_t GetLoginTime() const { return m_LoginTime; }

        //////////////////////////////////////////////////////////////////////////
        /// Vote
        //////////////////////////////////////////////////////////////////////////
        bool HaveVoteRemainingTime() const { return m_VoteRemainingTime != 0; }
        uint32 GetVoteRemainingTime() const { return m_VoteRemainingTime; }

        void SetServiceFlags(uint32 p_Flags);
        void UnsetServiceFlags(uint32 p_Flags);
        bool HasServiceFlags(uint32 p_Flags) const { return m_ServiceFlags & p_Flags; }

        void SetCustomFlags(uint32 p_Flags);
        void UnsetCustomFlags(uint32 p_Flags);
        bool HasCustomFlags(uint32 p_Flags) const { return m_CustomFlags & p_Flags; }

        void LoadPremades();
        void LoadLoyaltyData();

        /// Send a game error
        /// @p_Error : Game error
        /// @p_Data1 : Additional data 1
        /// @p_Data2 : Additional data 2
        void SendGameError(GameError::Type p_Error, uint32 p_Data1 = 0xF0F0F0F0, uint32 p_Data2 = 0xF0F0F0F0);

#ifndef CROSS
        /// ============== Cross realm ========================= ///
        uint32 GetInterRealmBG() { return m_InterRealmZoneId; }
        void SetInterRealmBG(uint32 value) { m_InterRealmZoneId = value; }

        void SetBattlegroundPortData(uint64 guid, uint32 time, uint32 queueslot, uint8 action)
        {
            _battlegroundPortData.PlayerGuid = guid;
            _battlegroundPortData.Time = time;
            _battlegroundPortData.QueueSlot = queueslot;
            _battlegroundPortData.Action = action;
        }

        BattlegroundPortData const& GetBattlegroundPortData() const { return _battlegroundPortData; }

        void SetCrossPartyInfo(CrossPartyInfo p_CrossPartyInfo) { m_CrossPartyInfo = p_CrossPartyInfo; }
        CrossPartyInfo const& GetCrossPartyInfo() const { return m_CrossPartyInfo; }

        void SetBackFromCross(bool p_Back) { m_BackFromCross = p_Back; }
        bool IsBackFromCross() const { return m_BackFromCross; }

        void SaveSpecialChannels();
        void RestoreSpecialChannels();

        /// ==================================================== ///

#endif /* not CROSS */
    public:                                                 // opcodes handlers

        void Handle_NULL(WorldPacket& recvPacket);          // not used
        void Handle_EarlyProccess(WorldPacket& recvPacket); // just mark packets processed in WorldSocket::OnRead
        void Handle_ServerSide(WorldPacket& recvPacket);    // sever side only, can't be accepted from client
        void Handle_Deprecated(WorldPacket& recvPacket);    // never used anymore by client

        void BuildCharacterRename(WorldPacket* pkt, ObjectGuid guid, uint8 result, std::string name);

        void HandleLoadScreenOpcode(WorldPacket& recvPacket);

        void HandleSuspendToken(WorldPacket& recvData);

        // played time
        void HandlePlayedTime(WorldPacket& recvPacket);

        // new
        void HandleMoveUnRootAck(WorldPacket& recvPacket);
        void HandleMoveRootAck(WorldPacket& recvPacket);
        void HandleLookingForGroup(WorldPacket& recvPacket);
        void HandleReturnToGraveyard(WorldPacket& recvPacket);

        // new inspect
        void HandleInspectOpcode(WorldPacket& recvPacket);

        // new party stats
        void HandleInspectHonorStatsOpcode(WorldPacket& recvPacket);
        void HandleInspectRatedBGStatsOpcode(WorldPacket& recvPacket);

        void HandleMoveWaterWalkAck(WorldPacket& recvPacket);
        void HandleFeatherFallAck(WorldPacket& recvData);

        void HandleMoveHoverAck(WorldPacket& recvData);

        void HandleMountSpecialAnimOpcode(WorldPacket& recvdata);

        // character view
        void HandleShowAccountAchievement(WorldPacket& recvData);
        void HandleShowingHelmOpcode(WorldPacket& recvData);
        void HandleShowingCloakOpcode(WorldPacket& recvData);

        // repair
        void HandleRepairItemOpcode(WorldPacket& recvPacket);

        // Knockback
        void HandleMoveKnockBackAck(WorldPacket& recvPacket);

        void HandleMoveTeleportAck(WorldPacket& recvPacket);
        void HandleForceSpeedChangeAck(WorldPacket& recvData);

        /// Live realm only handlers
#ifndef CROSS
        void HandleCharEnumOpcode(WorldPacket& recvPacket);
        void HandleCharDeleteOpcode(WorldPacket& recvPacket);
        void HandleCharCreateOpcode(WorldPacket& recvPacket);
        void HandleCharCreateCallback(PreparedQueryResult result, CharacterCreateInfo* createInfo);
        void HandlePlayerLoginOpcode(WorldPacket& recvPacket);
        void HandleCharEnum(PreparedQueryResult result);
        void HandlePlayerLogin(LoginQueryHolder* l_CharacterHolder, LoginDBQueryHolder* l_LoginHolder);
        void HandleCharRaceOrFactionChange(WorldPacket& recvData);
        void HandleRandomizeCharNameOpcode(WorldPacket& recvData);
        void HandleReorderCharacters(WorldPacket& recvData);

        void HandleAuthSessionOpcode(WorldPacket& recvPacket);

        // GM Ticket opcodes
        void OnGMTicketGetTicketEvent();
        void HandleGMTicketGetWebTicketOpcode(WorldPacket& recvPacket);
        void HandleGMTicketSystemStatusOpcode(WorldPacket& recvPacket);
        void HandleGMSurveySubmit(WorldPacket& recvPacket);
        void HandleReportLag(WorldPacket& recvPacket);
        void SendTicketStatusUpdate(uint8 p_Response);

        void HandleAuctionHelloOpcode(WorldPacket& recvPacket);
        void HandleAuctionListItems(WorldPacket& recvData);
        void HandleAuctionListBidderItems(WorldPacket& recvData);
        void HandleAuctionSellItem(WorldPacket& recvData);
        void HandleAuctionRemoveItem(WorldPacket& recvData);
        void HandleAuctionListOwnerItems(WorldPacket& recvData);
        void HandleAuctionPlaceBid(WorldPacket& recvData);
        void HandleAuctionListPendingSales(WorldPacket& recvData);

        void HandleGetMailList(WorldPacket& recvData);
        void HandleSendMail(WorldPacket& recvData);
        void HandleMailTakeMoney(WorldPacket& recvData);
        void HandleMailTakeItem(WorldPacket& recvData);
        void HandleMailMarkAsRead(WorldPacket& recvData);
        void HandleMailReturnToSender(WorldPacket& recvData);
        void HandleMailDelete(WorldPacket& recvData);

        // Calendar
        void HandleCalendarGetCalendar(WorldPacket& p_RecvData);
        void HandleCalendarGetEvent(WorldPacket& p_RecvData);
        void HandleCalendarGuildFilter(WorldPacket& p_RecvData);
        void HandleCalendarAddEvent(WorldPacket& p_RecvData);
        void HandleCalendarUpdateEvent(WorldPacket& p_RecvData);
        void HandleCalendarRemoveEvent(WorldPacket& p_RecvData);
        void HandleCalendarCopyEvent(WorldPacket& p_RecvData);
        void HandleCalendarEventInvite(WorldPacket& p_RecvData);
        void HandleCalendarEventRsvp(WorldPacket& p_RecvData);
        void HandleCalendarEventRemoveInvite(WorldPacket& p_RecvData);
        void HandleCalendarEventStatus(WorldPacket& p_RecvData);
        void HandleCalendarEventModeratorStatus(WorldPacket& p_RecvData);
        void HandleCalendarComplain(WorldPacket& p_RecvData);
        void HandleCalendarGetNumPending(WorldPacket& p_RecvData);
        void HandleCalendarEventSignup(WorldPacket& p_RecvData);
        void HandleSetSavedInstanceExtend(WorldPacket& p_RecvData);

        void SendCalendarRaidLockout(InstanceSave const* save, bool add);
        void SendCalendarRaidLockoutUpdated(InstanceSave const* save);

        // Black Market
        void HandleBlackMarketHello(WorldPacket& recvData);
        void SendBlackMarketHello(uint64 npcGuid);
        void HandleBlackMarketRequestItems(WorldPacket& recvData);
        void SendBlackMarketRequestItemsResult();
        void HandleBlackMarketBid(WorldPacket& recvData);
        void SendBlackMarketBidResult(uint32 itemEntry, uint32 auctionId);

        //////////////////////////////////////////////////////////////////////////
        /// Battlepay
        //////////////////////////////////////////////////////////////////////////
        void HandleBattlepayGetProductListQuery(WorldPacket& p_RecvData);
        void HandleBattlepayGetPurchaseList(WorldPacket& p_RecvData);
        void HandleBattlePayStartPurchase(WorldPacket& p_RecvData);
        void HandleBattlePayConfirmPurchase(WorldPacket& p_RecvData);
#endif

        void HandlePingOpcode(WorldPacket& recvPacket);
        void HandleRepopRequestOpcode(WorldPacket& recvPacket);
        void HandleLootItemOpcode(WorldPacket& recvPacket);
        void HandleLootMoneyOpcode(WorldPacket& recvPacket);
        void HandleLootOpcode(WorldPacket& recvPacket);
        void HandleLootReleaseOpcode(WorldPacket& recvPacket);
        void HandleMasterLootItemOpcode(WorldPacket& recvPacket);
        void HandleWhoOpcode(WorldPacket& recvPacket);
        void HandleLogoutRequestOpcode(WorldPacket& recvPacket);
        void HandlePlayerLogoutOpcode(WorldPacket& recvPacket);
        void HandleLogoutCancelOpcode(WorldPacket& recvPacket);

        void HandleTogglePvP(WorldPacket& recvPacket);

        void HandleZoneUpdateOpcode(WorldPacket& recvPacket);
        void HandleSetSelectionOpcode(WorldPacket& recvPacket);
        void HandleStandStateChangeOpcode(WorldPacket& recvPacket);
        void HandleEmoteOpcode(WorldPacket& recvPacket);
        void HandleContactListOpcode(WorldPacket& recvPacket);
        void HandleAddFriendOpcode(WorldPacket& recvPacket);
        void HandleAddFriendOpcodeCallBack(PreparedQueryResult result, std::string friendNote);
        void HandleDelFriendOpcode(WorldPacket& recvPacket);
        void HandleAddIgnoreOpcode(WorldPacket& recvPacket);
        void HandleAddIgnoreOpcodeCallBack(PreparedQueryResult result);
        void HandleDelIgnoreOpcode(WorldPacket& recvPacket);
        void HandleSetContactNotesOpcode(WorldPacket& recvPacket);
        void HandleReportBugOpcode(WorldPacket& recvPacket);
        void HandleReportSuggestionOpcode(WorldPacket& recvPacket);

        void HandleSendDuelRequest(WorldPacket& recvPacket);

        void HandleAreaTriggerOpcode(WorldPacket& recvPacket);

        void HandleSetFactionAtWar(WorldPacket& recvData);
        void HandleUnSetFactionAtWar(WorldPacket& recvData);
        void HandleSetFactionCheat(WorldPacket& recvData);
        void HandleSetWatchedFactionOpcode(WorldPacket& recvData);
        void HandleSetFactionInactiveOpcode(WorldPacket& recvData);

        void HandleUpdateAccountData(WorldPacket& recvPacket);
        void HandleRequestAccountData(WorldPacket& recvPacket);
        void HandleSetActionButtonOpcode(WorldPacket& recvPacket);

        void HandleGameObjectUseOpcode(WorldPacket& recPacket);
        void HandleMeetingStoneInfo(WorldPacket& recPacket);
        void HandleGameobjectReportUse(WorldPacket& recvPacket);

        void HandleNameQueryOpcode(WorldPacket& recvPacket);

        void HandleQueryTimeOpcode(WorldPacket& recvPacket);

        void HandleCreatureQueryOpcode(WorldPacket& recvPacket);

        void HandleGameObjectQueryOpcode(WorldPacket& recvPacket);

        void HandleMoveWorldportAckOpcode(WorldPacket& recvPacket);
        void HandleMoveWorldportAckOpcode();                // for server-side calls

        void HandleMovementOpcodes(WorldPacket& recvPacket);
        void HandleSetActiveMoverOpcode(WorldPacket& recvData);
        void HandleMoveNotActiveMover(WorldPacket& recvData);
        void HandleDismissControlledVehicle(WorldPacket& recvData);
        void HandleRequestVehicleExit(WorldPacket& recvData);
        void HandleChangeSeatsOnControlledVehicle(WorldPacket& recvData);
        void HandleMoveTimeSkippedOpcode(WorldPacket& recvData);
        void SendCancelVehicleRideAura();

        void HandleRequestRaidInfoOpcode(WorldPacket& recvData);

        void HandleBattlefieldStatusOpcode(WorldPacket& recvData);
        void HandleBattleMasterHelloOpcode(WorldPacket& recvData);

        void HandleGroupInviteOpcode(WorldPacket& recvPacket);
        //void HandleGroupCancelOpcode(WorldPacket& recvPacket);
        void HandleGroupInviteResponseOpcode(WorldPacket& recvPacket);
        void HandlePartyUninviteOpcode(WorldPacket& recvPacket);
        void HandleSetPartyLeaderOpcode(WorldPacket& recvPacket);
        void HandleSetRoleOpcode(WorldPacket& recvData);
        void HandleSetLootMethodOpcode(WorldPacket& recvData);
        void HandleLeaveGroupOpcode(WorldPacket& recvPacket);
        void HandleOptOutOfLootOpcode(WorldPacket& recvData);
        void HandleLootMethodOpcode(WorldPacket& recvPacket);
        void HandleLootRoll(WorldPacket& recvData);
        void HandleDoMasterLootRollOpcode(WorldPacket& recvData);
        void HandleRequestPartyMemberStatsOpcode(WorldPacket& recvData);
        void HandleRaidConfirmReadyCheck(WorldPacket& recvData);
        void HandleRaidLeaderReadyCheck(WorldPacket& recvData);
        void HandleUpdateRaidTargetOpcode(WorldPacket& recvData);
        void HandleGroupRaidConvertOpcode(WorldPacket& recvData);
        void HandleGroupChangeSubGroupOpcode(WorldPacket& recvData);
        void HandleGroupSwapSubGroupOpcode(WorldPacket& recvData);
        void HandleGroupAssistantLeaderOpcode(WorldPacket& recvData);
        void HandleGroupEveryoneIsAssistantOpcode(WorldPacket& recvData);
        void HandlePartyAssignmentOpcode(WorldPacket& recvData);
        void HandleRolePollBegin(WorldPacket& recvData);
        void HandleRequestJoinUpdates(WorldPacket& recvData);
        void HandleClearRaidMarkerOpcode(WorldPacket& recvData);

        void HandleQueryGuildInfoOpcode(WorldPacket& recvPacket);

#ifndef CROSS
        void HandlePetitionBuyOpcode(WorldPacket& recvData);
        void HandlePetitionShowSignOpcode(WorldPacket& recvData);
        void HandlePetitionQueryOpcode(WorldPacket& recvData);
        void HandlePetitionRenameOpcode(WorldPacket& recvData);
        void HandlePetitionSignOpcode(WorldPacket& recvData);
        void HandlePetitionDeclineOpcode(WorldPacket& recvData);
        void HandleOfferPetitionOpcode(WorldPacket& recvData);
        void HandleTurnInPetitionOpcode(WorldPacket& recvData);
        void HandlePetitionShowListOpcode(WorldPacket& recvPacket);


        void HandleGuildInviteByNameOpcode(WorldPacket& recvPacket);
        void HandleGuildOfficierRemoveMemberOpcode(WorldPacket& recvPacket);
        void HandleGuildMasterReplaceOpcode(WorldPacket& recvPacket);
        void HandleAcceptGuildInviteOpcode(WorldPacket& recvPacket);
        void HandleGuildDeclineInvitationsOpcode(WorldPacket& recvPacket);
        void HandleGuildEventLogQueryOpcode(WorldPacket& recvPacket);
        void HandleGuildRosterOpcode(WorldPacket& recvPacket);
        void HandleRequestGuildRewardsListOpcode(WorldPacket& recvPacket);
        void HandleGuildAssignRankOpcode(WorldPacket& recvPacket);
        void HandleGuildLeaveOpcode(WorldPacket& recvPacket);
        void HandleGuildDeleteOpcode(WorldPacket& recvPacket);
        void HandleGuildSetGuildMasterOpcode(WorldPacket& recvPacket);
        void HandleGuildUpdateMOTDTextOpcode(WorldPacket& recvPacket);
        void HandleGuildNewsUpdateStickyOpcode(WorldPacket& recvPacket);
        void HandleGuildSetMemberNoteOpcode(WorldPacket& recvPacket);
        void HandleGuildGetRanksOpcode(WorldPacket& recvPacket);
        void HandleGuildQueryNewsOpcode(WorldPacket& recvPacket);
        void HandleShiftRanks(WorldPacket& recvPacket);
        void HandleGuildSetRankPermissionsOpcode(WorldPacket& recvPacket);
        void HandleGuildAddRankOpcode(WorldPacket& recvPacket);
        void HandleGuildDeleteRankOpcode(WorldPacket& recvPacket);
        void HandleGuildUpdateInfoTextOpcode(WorldPacket& recvPacket);
        void HandlePlayerSaveGuildEmblemOpcode(WorldPacket& recvPacket);
        void HandleRequestGuildPartyState(WorldPacket& recvPacket);
        void HandleAutoDeclineGuildInvites(WorldPacket& recvPacket);
        void HandleGuildChallengeUpdateRequest(WorldPacket& recvPacket);
        void HandleGuildRequestGuildRecipes(WorldPacket& recvPacket);

        void HandleGuildFinderAddRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderBrowse(WorldPacket& recvPacket);
        void HandleGuildFinderDeclineRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderGetApplications(WorldPacket& recvPacket);
        void HandleGuildFinderGetRecruits(WorldPacket& recvPacket);
        void HandleGuildFinderPostRequest(WorldPacket& recvPacket);
        void HandleGuildFinderRemoveRecruit(WorldPacket& recvPacket);
        void HandleGuildFinderSetGuildPost(WorldPacket& recvPacket);
#endif

        void HandleEnableTaxiNodeOpcode(WorldPacket& recvPacket);
        void HandleTaxiNodeStatusQueryOpcode(WorldPacket& recvPacket);
        void HandleTaxiQueryAvailableNodes(WorldPacket& recvPacket);
        void HandleActivateTaxiOpcode(WorldPacket& recvPacket);
        void HandleMoveSplineDoneOpcode(WorldPacket& recvPacket);
        void HandleTaxiRequestEarlyLandingOpcode(WorldPacket & p_Packet);
        void SendActivateTaxiReply(ActivateTaxiReply reply);

        void HandleTabardVendorActivateOpcode(WorldPacket& recvPacket);

        void HandleBankerActivateOpcode(WorldPacket& recvPacket);
        void HandleBuyBankSlotOpcode(WorldPacket& recvPacket);
        void HandleBuyReagentBankOpcode(WorldPacket& recvPacket);
        void HandleSortReagentBankBagsOpcode(WorldPacket& recvPacket);
        void HandleDepositAllReagentsOpcode(WorldPacket& p_RecvData);
        void HandleUseCritterItemOpcode(WorldPacket& p_RecvData);

        void HandleTrainerListOpcode(WorldPacket& recvPacket);
        void HandleTrainerBuySpellOpcode(WorldPacket& recvPacket);
        void HandleGossipHelloOpcode(WorldPacket& recvPacket);
        void HandleGossipSelectOptionOpcode(WorldPacket& recvPacket);
        void HandleSpiritHealerActivateOpcode(WorldPacket& recvPacket);
        void HandleNpcTextQueryOpcode(WorldPacket& recvPacket);
        void HandleBinderActivateOpcode(WorldPacket& recvPacket);
        void HandleListStabledPetsOpcode(WorldPacket& recvPacket);
        void HandleStableSetPetSlot(WorldPacket& recvPacket);
        void HandleStableSetPetSlotCallback(PreparedQueryResult result, uint32 petId);
        void SendTrainerService(uint64 guid, uint32 spellId, uint32 trainState);

        void HandleDuelResponseOpcode(WorldPacket& recvPacket);

        void HandleAcceptTradeOpcode(WorldPacket& recvPacket);
        void HandleBusyTradeOpcode(WorldPacket& p_RecvData);
        void HandleBeginTradeOpcode(WorldPacket& recvPacket);
        void HandleCancelTradeOpcode(WorldPacket& recvPacket);
        void HandleClearTradeItemOpcode(WorldPacket& recvPacket);
        void HandleIgnoreTradeOpcode(WorldPacket& recvPacket);
        void HandleInitiateTradeOpcode(WorldPacket& recvPacket);
        void HandleSetTradeGoldOpcode(WorldPacket& recvPacket);
        void HandleSetTradeItemOpcode(WorldPacket& recvPacket);
        void HandleUnacceptTradeOpcode(WorldPacket& recvPacket);

        void HandleItemTextQuery(WorldPacket& recvData);
#ifndef CROSS
        void HandleMailCreateTextItem(WorldPacket& recvData);
        void HandleQueryNextMailTime(WorldPacket& recvData);
#endif
        void HandleCancelChanneling(WorldPacket& recvData);

        void SendItemPageInfo(ItemTemplate* itemProto);
        void HandleSplitItemOpcode(WorldPacket& recvPacket);
        void HandleSwapInvItemOpcode(WorldPacket& recvPacket);
        void HandleDestroyItemOpcode(WorldPacket& recvPacket);
        void HandleAutoEquipItemOpcode(WorldPacket& recvPacket);
        void SendBroadcastTextDb2Reply(uint32 entry);
        void HandleSellItemOpcode(WorldPacket& recvPacket);
        void HandleBuyItemInSlotOpcode(WorldPacket& recvPacket);
        void HandleBuyItemOpcode(WorldPacket& recvPacket);
        void HandleListInventoryOpcode(WorldPacket& recvPacket);
        void HandleAutoStoreBagItemOpcode(WorldPacket& recvPacket);
        void HandleReadItem(WorldPacket& recvPacket);
        void HandleAutoEquipItemSlotOpcode(WorldPacket& recvPacket);
        void HandleSwapItem(WorldPacket& recvPacket);
        void HandleBuybackItem(WorldPacket& recvPacket);
        void HandleAutoBankItemOpcode(WorldPacket& recvPacket);
        void HandleAutoStoreBankItemOpcode(WorldPacket& recvPacket);
        void HandleWrapItemOpcode(WorldPacket& recvPacket);

        void HandleAttackSwingOpcode(WorldPacket& recvPacket);
        void HandleAttackStopOpcode(WorldPacket& recvPacket);
        void HandleSetSheathedOpcode(WorldPacket& recvPacket);

        void HandleUseItemOpcode(WorldPacket& recvPacket);
        void HandleOpenItemOpcode(WorldPacket& recvPacket);
        void HandleCastSpellOpcode(WorldPacket& recvPacket);
        void HandleCancelCastOpcode(WorldPacket& recvPacket);
        void HandleCancelAuraOpcode(WorldPacket& recvPacket);
        void HandleCancelGrowthAuraOpcode(WorldPacket& recvPacket);
        void HandleCancelAutoRepeatSpellOpcode(WorldPacket& recvPacket);

        void HandleTalentWipeConfirmOpcode(WorldPacket& recvPacket);
        void HandleUnlearnSkillOpcode(WorldPacket& recvPacket);
        void HandleSetSpecialization(WorldPacket& recvPacket);
        void HandleLearnTalents(WorldPacket& recvPacket);

        void HandleArcheologyRequestHistory(WorldPacket& recvPacket);

        void HandleQuestgiverStatusQueryOpcode(WorldPacket& recvPacket);
        void HandleQuestgiverStatusMultipleQuery(WorldPacket& recvPacket);
        void HandleQuestgiverHelloOpcode(WorldPacket& recvPacket);
        void HandleQuestgiverAcceptQuestOpcode(WorldPacket& recvPacket);
        void HandleQuestgiverQueryQuestOpcode(WorldPacket& recvPacket);
        void HandleQuestgiverChooseRewardOpcode(WorldPacket& recvPacket);
        void HandleQuestgiverRequestRewardOpcode(WorldPacket& recvPacket);
        void HandleQuestQueryOpcode(WorldPacket& recvPacket);
        void HandleQuestLogRemoveQuest(WorldPacket& recvData);
        void HandleQuestConfirmAccept(WorldPacket& recvData);
        void HandleQuestgiverCompleteQuest(WorldPacket& recvData);
        void HandleQuestgiverQuestAutoLaunch(WorldPacket& recvPacket);
        void HandlePushQuestToParty(WorldPacket& recvPacket);
        void HandleQuestPushResult(WorldPacket& recvPacket);

        bool processChatmessageFurtherAfterSecurityChecks(std::string&, uint32);
        void HandleMessagechatOpcode(WorldPacket& recvPacket);
        void HandleAddonMessagechatOpcode(WorldPacket& recvPacket);
        void SendPlayerNotFoundNotice(std::string name);
        void SendPlayerAmbiguousNotice(std::string name);
        void SendChatRestrictedNotice(ChatRestrictionType restriction);
        void HandleTextEmoteOpcode(WorldPacket& recvPacket);

        void HandleUnregisterAddonPrefixesOpcode(WorldPacket& recvPacket);
        void HandleAddonRegisteredPrefixesOpcode(WorldPacket& recvPacket);

        void HandleRequestGmTicket(WorldPacket& recvPakcet);
        void HandleReclaimCorpseOpcode(WorldPacket& recvPacket);
        void HandleCorpseLocationFromClientQueryOpcode(WorldPacket& recvPacket);
        void HandleCemeteryListOpcode(WorldPacket& recvData);
        void HandleForcedReactionsOpcode(WorldPacket& recvData);
        void HandleCorpseTransportQueryOpcode(WorldPacket& recvPacket);
        void HandleResurrectResponseOpcode(WorldPacket& recvPacket);
        void HandleSummonResponseOpcode(WorldPacket& recvData);

        void HandleJoinChannel(WorldPacket& recvPacket);
        void HandleLeaveChannel(WorldPacket& recvPacket);
        void HandleChannelList(WorldPacket& recvPacket);
        void HandleChannelPassword(WorldPacket& recvPacket);
        void HandleChannelSetOwner(WorldPacket& recvPacket);
        void HandleChannelOwner(WorldPacket& recvPacket);
        void HandleChannelModerator(WorldPacket& recvPacket);
        void HandleChannelUnmoderator(WorldPacket& recvPacket);
        void HandleChannelMute(WorldPacket& recvPacket);
        void HandleChannelUnmute(WorldPacket& recvPacket);
        void HandleChannelInvite(WorldPacket& recvPacket);
        void HandleChannelKick(WorldPacket& recvPacket);
        void HandleChannelBan(WorldPacket& recvPacket);
        void HandleChannelUnban(WorldPacket& recvPacket);
        void HandleChannelAnnouncements(WorldPacket& recvPacket);
        void HandleChannelModerate(WorldPacket& recvPacket);
        void HandleChannelDeclineInvite(WorldPacket& recvPacket);
        void HandleChannelDisplayListQuery(WorldPacket& recvPacket);

        void HandleCompleteCinematic(WorldPacket& recvPacket);
        void HandleNextCinematicCamera(WorldPacket& recvPacket);
        void HandleCompleteMovieOpcode(WorldPacket & p_Packet);
        void HandleSceneTriggerEventOpcode(WorldPacket & p_Packet);
        void HandleSceneCancelOpcode(WorldPacket & p_Packet);

        void HandlePageTextQueryOpcode(WorldPacket& recvPacket);

        void HandleTutorial (WorldPacket& recvData);

        //Pet
        void HandlePetAction(WorldPacket& recvData);
        void HandlePetStopAttack(WorldPacket& recvData);
        void HandlePetActionHelper(Unit* pet, uint64 guid1, uint32 spellid, uint16 flag, uint64 guid2, float x, float y, float z);
        void HandlePetNameQuery(WorldPacket& recvData);
        void HandlePetSetAction(WorldPacket& recvData);
        void HandlePetAbandon(WorldPacket& recvData);
        void HandlePetRename(WorldPacket& recvData);
        void HandlePetCancelAuraOpcode(WorldPacket& recvPacket);
        void HandlePetCastSpellOpcode(WorldPacket& recvPacket);

        void HandleSetActionBarToggles(WorldPacket& recvData);

#ifndef CROSS
        void HandleCharRenameOpcode(WorldPacket& recvData);
        void HandleCharCustomize(WorldPacket& recvData);
#endif

        void HandleChangePlayerNameOpcodeCallBack(PreparedQueryResult result, std::string newName);
        void HandleSetPlayerDeclinedNames(WorldPacket& recvData);
        void SendPlayerDeclinedNamesResult(uint64 guid, uint32 result);
        void HandleMountSetFavoriteOpcode(WorldPacket & p_Packet);

        void HandleTotemDestroyed(WorldPacket& recvData);
        void HandleDismissCritter(WorldPacket& recvData);

        //Battleground
        void HandleBattlemasterHelloOpcode(WorldPacket& recvData);
        void HandleBattlemasterJoinOpcode(WorldPacket& recvData);
        void HandlePVPLogDataOpcode(WorldPacket& recvData);
        void HandleBattleFieldPortOpcode(WorldPacket& recvData);
        void HandleBattlefieldListOpcode(WorldPacket& recvData);
        void HandleLeaveBattlefieldOpcode(WorldPacket& recvData);
        void HandleBattlemasterJoinArena(WorldPacket& recvData);
        void HandleBattlemasterJoinArenaSkirmish(WorldPacket& p_RecvData);
        void HandleBattlemasterJoinRated(WorldPacket& recvData);
        void HandleBattleFieldRequestScoreData(WorldPacket & p_Packet);
        void HandleStartWarGame(WorldPacket& p_RecvData);
        void HandleAcceptWarGameInvite(WorldPacket& p_Packet);

        void HandleReportPvPAFK(WorldPacket& recvData);
        void HandleRequestPvpOptions(WorldPacket& recvData);
        void HandleRequestPvpReward(WorldPacket& recvData);
        void HandleRequestRatedBgStats(WorldPacket& recvData);
        void HandleRequestConquestFormulaConstants(WorldPacket& p_RecvData);

        void HandleWardenDataOpcode(WorldPacket& recvData);
        void HandleWorldTeleportOpcode(WorldPacket& recvData);
        void HandleMinimapPingOpcode(WorldPacket& recvData);
        void HandleRandomRollOpcode(WorldPacket& recvData);
        void HandleFarSightOpcode(WorldPacket& recvData);
        void HandleCloseInteraction(WorldPacket& p_RecvData);
        void HandleSetDungeonDifficultyOpcode(WorldPacket& recvData);
        void HandleSetRaidDifficultyOpcode(WorldPacket& recvData);
        void HandleMoveSetCanFlyAckOpcode(WorldPacket& recvData);
        void HandleSetTitleOpcode(WorldPacket& recvData);
        void HandleRealmSplitOpcode(WorldPacket& recvData);
        void HandleRealmQueryNameOpcode(WorldPacket& recvData);
        void HandleTimeSyncResp(WorldPacket& recvData);
        void HandleWhoisOpcode(WorldPacket& recvData);
        void HandleResetInstancesOpcode(WorldPacket& recvData);
        void HandleResetChallengeModeOpcode(WorldPacket& recvData);
        void HandleHearthAndResurrect(WorldPacket& recvData);
        void HandleInstanceLockResponse(WorldPacket& recvPacket);

        // Battlefield
        void SendBfInvitePlayerToWar(uint64 guid,uint32 ZoneId,uint32 time);
        void SendBfInvitePlayerToQueue(uint64 guid);
        void SendBfQueueInviteResponse(uint64 guid,uint32 ZoneId, bool CanQueue = true, bool Full = false);
        void SendBfEntered(uint64 guid);
        void SendBfLeaveMessage(uint64 guid, BFLeaveReason reason = BF_LEAVE_REASON_EXITED);
        void HandleBfQueueInviteResponse(WorldPacket &recv_data);
        void HandleBfEntryInviteResponse(WorldPacket &recv_data);
        void HandleBfExitQueueRequest(WorldPacket &recv_data);
        void HandleBfExitRequest(WorldPacket &recv_data);
        void HandleBfQueueRequest(WorldPacket &recv_data);

        // Looking for Dungeon/Raid
        void HandleLfgSetCommentOpcode(WorldPacket& recvData);
        void HandleLfgLockInfoRequestOpcode(WorldPacket& recvData);
        void HandleLfgJoinOpcode(WorldPacket& recvData);
        void HandleLfgLeaveOpcode(WorldPacket& recvData);
        void HandleDfSetRolesOpcode(WorldPacket& recvData);
        void HandleLfgProposalResultOpcode(WorldPacket& recvData);
        void HandleLfgSetBootVoteOpcode(WorldPacket& recvData);
        void HandleLfgTeleportOpcode(WorldPacket& recvData);
        void HandleLfrSearchOpcode(WorldPacket& recvData);
        void HandleLfrLeaveOpcode(WorldPacket& recvData);
        void HandleLfgGetStatus(WorldPacket& recvData);

        void SendLfgRoleChosen(uint64 p_Guid, uint8 p_Roles);
        void SendLfgRoleCheckUpdate(const LfgRoleCheck* pRoleCheck);
        void SendLfgUpdateSearch(bool update);
        void SendLfgJoinResult(uint64 guid_, const LfgJoinResultData& joinData);
        void SendLfgQueueStatus(uint32 dungeon, int32 waitTime, int32 avgWaitTime, int32 waitTimeTanks, int32 waitTimeHealer, int32 waitTimeDps, uint32 queuedTime, uint8 tanks, uint8 healers, uint8 dps);
        void SendLfgPlayerReward(uint32 rdungeonEntry, uint32 sdungeonEntry, uint8 done, const LfgReward* reward, const Quest *qRew);
        void SendLfgBootPlayer(const LfgPlayerBoot* pBoot);
        void SendLfgUpdateProposal(uint32 proposalId, const LfgProposal *pProp);
        void SendLfgDisabled();
        void SendLfgOfferContinue(uint32 dungeonEntry);
        void SendLfgTeleportError(uint8 err);

        void HandleAreaSpiritHealerQueryOpcode(WorldPacket& recvData);
        void HandleAreaSpiritHealerQueueOpcode(WorldPacket& recvData);
        void HandleCancelMountAuraOpcode(WorldPacket& recvData);
        void HandleSelfResOpcode(WorldPacket& recvData);
        void HandleComplainOpcode(WorldPacket& recvData);
        void HandleRequestPetInfoOpcode(WorldPacket& recvData);

        // Lfg List
        void HandleRequestLfgListBlacklist(WorldPacket& p_RecvData);
        void HandleLfgListJoin(WorldPacket& p_RecvData);
        void HandleLfgListUpdateRequest(WorldPacket& p_RecvData);
        void HandleLfgListLeave(WorldPacket& p_RecvData);
        void HandleLfgListSearch(WorldPacket& p_RecvData);
        void HandleLfgListApplyForGroup(WorldPacket& p_RecvData);
        void HandleLfgListInviteApplicant(WorldPacket& p_RecvData);
        void HandleLfgListRemoveApplicant(WorldPacket& p_RecvData);
        void HandleLfgListCancelApplication(WorldPacket& p_RecvData);
        void HandleLfgListInvitationAnswer(WorldPacket& p_RecvData);

        void SendLfgListJoinResult(LFGListEntry const* p_Entry, uint8 p_Error);
        void SendLfgSearchResponse(uint32 p_ActivityCategory, uint32 p_ActivitySubCategory, std::string p_FilterString);
        void SendLfgListApplyForGroupResult(LFGListEntry const* p_LFGEntry, LFGListEntry::LFGListApplicationEntry const* p_Application);
        void SendLfgListApplicantGroupInviteResponse(LFGListEntry::LFGListApplicationEntry const* p_Applicant);

        static void BuildLfgListRideTicket(WorldPacket* p_Data, LFGListEntry const* p_Entry);
        static void BuildLfgListApplicationRideTicket(WorldPacket* p_Data, LFGListEntry::LFGListApplicationEntry const* p_Entry);
        static void BuildLfgListEntryInfo(WorldPacket* p_Data, LFGListEntry const* p_Entry);
        static void BuildLfgListJoinRequest(WorldPacket* p_Data, LFGListEntry const* p_Entry);
        static void BuildLfgListQueueUpdate(WorldPacket* p_Data, LFGListEntry const* p_Entry, bool p_Listed);
        static LFGListEntry* ReadLfgListRideTicketInfo(WorldPacket* p_Data, uint32* l_ID);
        static LFGListEntry::LFGListApplicationEntry* ReadLfgListApplicanmtRideTicketInfo(WorldPacket* p_Data, LFGListEntry* p_ReferenceEntry, uint32* l_ID);
        static LFGListEntry::LFGListApplicationEntry* ReadLfgListApplicanmtRideTicketInfo(WorldPacket* p_Data, uint32* l_ID);
        static void BuildLfgListApplicantListUpdate(WorldPacket* p_Data, LFGListEntry const* p_Entry, LFGListEntry::LFGListApplicationEntry const* p_Applicant);
        static void BuildLfgListApplicantListUpdate(WorldPacket* p_Data, LFGListEntry const* p_Entry, std::list<LFGListEntry::LFGListApplicationEntry const*> p_ApplicantList);
        static void BuilfLfgListApplicantGroupInvite(WorldPacket* p_Data, LFGListEntry::LFGListApplicationEntry const* p_Applicant);

        // Socket gem
        void HandleSocketOpcode(WorldPacket& recvData);

        void HandleCancelTempEnchantmentOpcode(WorldPacket& recvData);

        void HandleItemRefundInfoRequest(WorldPacket& recvData);
        void HandleItemRefund(WorldPacket& recvData);

        void HandleChannelVoiceOnOpcode(WorldPacket& recvData);
        void HandleVoiceSessionEnableOpcode(WorldPacket& recvData);
        void HandleSetActiveVoiceChannel(WorldPacket& recvData);
        void HandleSetTaxiBenchmarkOpcode(WorldPacket& recvData);

        // Guild Bank
#ifndef CROSS
        void HandleGuildPermissionsQueryOpcode(WorldPacket& recvData);
        void HandleGuildBankRemainingWithdrawMoneyQueryOpcode(WorldPacket& recvData);
        void HandleGuildBankActivate(WorldPacket& recvData);
        void HandleGuildBankQueryTab(WorldPacket& recvData);
        void HandleGuildBankLogQuery(WorldPacket& recvData);
        void HandleGuildBankDepositMoney(WorldPacket& recvData);
        void HandleGuildBankWithdrawMoney(WorldPacket& recvData);
        void HandleGuildBankSwapItems(WorldPacket& recvData);

        void HandleGuildBankUpdateTab(WorldPacket& recvData);
        void HandleGuildBankBuyTab(WorldPacket& recvData);
        void HandleQueryGuildBankTextQuery(WorldPacket& recvData);
        void HandleSetGuildBankTabText(WorldPacket& recvData);
#endif

        // Refer-a-Friend
        void HandleGrantLevel(WorldPacket& recvData);
        void HandleAcceptGrantLevel(WorldPacket& recvData);

        // Void Storage
        void HandleVoidStorageUnlock(WorldPacket& recvData);
        void HandleVoidStorageQuery(WorldPacket& recvData);
        void HandleVoidStorageTransfer(WorldPacket& recvData);
        void HandleVoidSwapItem(WorldPacket& recvData);
        void SendVoidStorageTransferResult(VoidTransferError result);

        // Transmogrification
        void HandleTransmogrifyItems(WorldPacket& recvData);

        // Item Upgrade
        void HandleUpgradeItemOpcode(WorldPacket& recvData);
        void SendItemUpgradeResult(bool success);

        // Loot specialization
        void HandleSetLootSpecialization(WorldPacket& recvData);

        // Miscellaneous
        void HandleSaveCUFProfiles(WorldPacket& recvData);
        void HandleSpellClick(WorldPacket& recvData);
        void HandleMirrorImageDataRequest(WorldPacket& recvData);
        void HandleAlterAppearance(WorldPacket& recvData);
        void HandleRemoveGlyph(WorldPacket& recvData);
        void HandleQueryInspectAchievements(WorldPacket& recvData);
        void HandleGuildAchievementProgressQuery(WorldPacket& recvData);
        void HandleEquipmentSetSave(WorldPacket& recvData);
        void HandleEquipmentSetDelete(WorldPacket& recvData);
        void HandleEquipmentSetUse(WorldPacket& recvData);
        void HandleWorldStateUITimerUpdate(WorldPacket& recvData);
        void HandleUndeleteCharacter(WorldPacket& recvData);
        void HandleQueryQuestCompletionNpcs(WorldPacket& RecvData);
        void HandleQuestPOIQuery(WorldPacket& recvData);
        void HandleEjectPassenger(WorldPacket& data);
        void HandleEnterPlayerVehicle(WorldPacket& data);
        void HandleDBQueryBulk(WorldPacket& recvPacket);
        void HandleUpdateMissileTrajectory(WorldPacket& recvPacket);
        void HandleViolenceLevel(WorldPacket& recvPacket);
        void HandleObjectUpdateFailedOpcode(WorldPacket& recvPacket);
        void HandleSetFactionOpcode(WorldPacket& recvPacket);
        void HandleCategoryCooldownOpcode(WorldPacket& recvPacket);
        void HandleChangeCurrencyFlags(WorldPacket& recvPacket);
        void HandleShowTradeSkillOpcode(WorldPacket& p_RecvPacket);
        int32 HandleEnableNagleAlgorithm();

        // Twitter
        void HandleRequestTwitterStatus(WorldPacket& p_RecvData);
        void SendTwitterStatus(bool l_Enabled);

        //////////////////////////////////////////////////////////////////////////
        /// Garrison
        //////////////////////////////////////////////////////////////////////////
#ifndef CROSS
        void HandleGetGarrisonInfoOpcode(WorldPacket& p_RecvData);
        void HandleRequestGarrisonUpgradeableOpcode(WorldPacket& p_RecvData);
        void HandleUpgradeGarrisonOpcode(WorldPacket& p_RecvData);
        void HandleRequestLandingPageShipmentInfoOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonMissionNPCHelloOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonRequestSetMissionNPC(WorldPacket& p_RecvData);
        void HandleGarrisonRequestBuildingsOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonPurchaseBuildingOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonCancelConstructionOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonStartMissionOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonCompleteMissionOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonMissionBonusRollOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonGenerateRecruitsOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonSetRecruitmentPreferencesOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonRecruitFollower(WorldPacket& p_RecvData);
        void HandleGarrisonChangeFollowerActivationStateOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonAssignFollowerToBuilding(WorldPacket& p_RecvData);
        void HandleGarrisonRemoveFollowerFromBuilding(WorldPacket& p_RecvData);
        void HandleGarrisonGetShipmentInfoOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonCreateShipmentOpcode(WorldPacket& p_RecvData);
        void HandleGarrisonFollowerRename(WorldPacket& p_RecvData);
        void HandleGarrisonDecommisionShip(WorldPacket& p_RecvData);

        void SendGarrisonOpenArchitect(uint64 p_CreatureGUID);
        void SendGarrisonOpenMissionNpc(uint64 p_CreatureGUID);
        void SendGarrisonSetMissionNpc(uint64 p_CreatureGUID);
#endif

        /// Battle pet
        void SendBattlePetUpdates(bool p_AddedPet);
        void SendBattlePetTrapLevel();
        void SendBattlePetJournalLockAcquired();
        void SendBattlePetJournalLockDenied();
        void SendBattlePetJournal();
        void SendBattlePetDeleted(uint64 p_BattlePetGUID);
        void SendBattlePetRevoked(uint64 p_BattlePetGUID);
        void SendBattlePetRestored(uint64 p_BattlePetGUID);
        void SendBattlePetsHealed();
        void SendBattlePetLicenseChanged();
        void SendBattlePetError(uint32 p_Result, uint32 p_CreatureID);
        void SendBattlePetCageDateError(uint32 p_SecondsUntilCanCage);
        void HandleBattlePetQueryName(WorldPacket& p_RecvData);
        void HandleBattlePetsReconvert(WorldPacket& p_RecvData);
        void HandleBattlePetUpdateNotify(WorldPacket& p_RecvData);
        void HandleBattlePetRequestJournalLock(WorldPacket& p_RecvData);
        void HandleBattlePetRequestJournal(WorldPacket& p_RecvData);
        void HandleBattlePetDeletePet(WorldPacket& p_RecvData);
        void HandleBattlePetDeletePetCheat(WorldPacket& p_RecvData);
        void HandleBattlePetDeleteJournal(WorldPacket& p_RecvData);
        void HandleBattlePetModifyName(WorldPacket& p_RecvData);
        void HandleBattlePetSummon(WorldPacket& p_RecvData);
        void HandleBattlePetSetLevel(WorldPacket& p_RecvData);
        void HandleBattlePetSetBattleSlot(WorldPacket& p_RecvData);
        void HandleBattlePetSetCollar(WorldPacket& p_RecvData);
        void HandleBattlePetSetFlags(WorldPacket& p_RecvData);
        void HandleBattlePetsRestoreHealth(WorldPacket& p_RecvData);
        void HandleBattlePetAdd(WorldPacket& p_RecvData);
        void HandleBattlePetSetQualityCheat(WorldPacket& p_RecvData);
        void HandleBattlePetCage(WorldPacket& p_RecvData);

        /// Pet battle
        void SendPetBattleSlotUpdates(bool p_NewSlotUnlocked);
        void SendPetBattleRequestFailed(uint8 p_Reason);
        void SendPetBattlePvPChallenge(PetBattleRequest* p_Request);
        void SendPetBattleFinalizeLocation(PetBattleRequest* p_Request);
        void SendPetBattleInitialUpdate(PetBattle* p_Battle);
        void SendPetBattleFirstRound(PetBattle* p_Battle);
        void SendPetBattleRoundResult(PetBattle* p_Battle);
        void SendPetBattleReplacementMade(PetBattle* p_Battle);
        void SendPetBattleFinalRound(PetBattle* p_Battle);
        void SendPetBattleFinished(PetBattle* p_Battle);
        void SendPetBattleChatRestricted();
        void SendPetBattleMaxGameLenghtWarning();
        void SendPetBattleQueueProposeMatch();
        void SendPetBattleQueueStatus(uint32 p_TicketTime, uint32 p_TicketID, uint32 p_Status, uint32 p_AvgWaitTime);
        void SendPetBattleDebugQueueDumpResponse();
        void HandlePetBattleJoinQueue(WorldPacket& p_RecvData);
        void HandlePetBattleLeaveQueue(WorldPacket& p_RecvData);
        void HandlePetBattleRequestWild(WorldPacket& p_RecvData);
        void HandlePetBattleWildLocationFail(WorldPacket& p_RecvData);
        void HandlePetBattleRequestPvP(WorldPacket& p_RecvData);
        void HandlePetBattleRequestUpdate(WorldPacket& p_RecvData);
        void HandlePetBattleQuitNotify(WorldPacket& p_RecvData);
        void HandlePetBattleFinalNotify(WorldPacket& p_RecvData);
        void HandlePetBattleScriptErrorNotify(WorldPacket& p_RecvData);
        void HandlePetBattleQueueProposeMatchResult(WorldPacket& p_RecvData);
        void HandlePetBattleFirstPet(WorldPacket& p_RecvData);
        void HandlePetBattleInput(WorldPacket& p_RecvData);
        void HandlePetBattleReplaceFrontPet(WorldPacket& p_RecvData);
        void HandlePetBattleDebugQueueDump(WorldPacket& p_RecvData);

        //////////////////////////////////////////////////////////////////////////
        /// ToyBox
        //////////////////////////////////////////////////////////////////////////
        void HandleAddNewToyToBoxOpcode(WorldPacket& p_RecvData);
        void HandleSetFavoriteToyOpcode(WorldPacket& p_RecvData);
        void HandleUseToyOpcode(WorldPacket& p_RecvData);

        //////////////////////////////////////////////////////////////////////////
        /// Challenges
        //////////////////////////////////////////////////////////////////////////
        void HandleGetChallengeModeRewards(WorldPacket& p_RecvData);
        void HandleChallengeModeRequestLeaders(WorldPacket& p_RecvData);
        void HandleChallengeModeRequestMapStats(WorldPacket& p_RecvData);
        void SendChallengeModeMapStatsUpdate(uint32 p_MapID);

        /// Auto sort bags.
        void HandleSortBags(WorldPacket& p_RecvData);

        /// Gms command cooldowns (performances)
        time_t m_TimeLastTicketOnlineList;

#ifdef CROSS
        /// Cross related
        void LoadCharacter(CharacterPortData const& p_CharacterPortData);
        void LoadCharacterDone(LoginQueryHolder* p_CharHolder, LoginDBQueryHolder* p_AuthHolder);
#endif
       
        void SetStressTest(bool p_Value) { m_IsStressTestSession = p_Value; }
        bool IsStressTest() const { return m_IsStressTestSession; }

        uint32 GetActivityDays() const { return m_ActivityDays; }
        time_t GetLastBan() const { return m_LastBan; };
        time_t GetLastClaim() const { return m_LastClaim; }
        time_t GetLastEventReset() const { return m_LastEventReset; }
        bool   IsEmailValidated() const { return m_EmailValidated; }
        bool   HaveAlreadyPurchasePoints() const { return m_AlreadyPurchasePoints; }
        uint32 GetLoyaltyEventCount(LoyaltyEvent p_Event) { return m_LoyaltyEvents[(uint32)p_Event]; }
        void   AddLoyaltyPoints(uint32 p_Count, std::string p_Reason);
        void   SetLastClaim(time_t p_ClaimTime) { m_LastClaim = p_ClaimTime; }
        void   CompleteLoyaltyEvent(LoyaltyEvent p_Event);

    private:
        void InitializeQueryCallbackParameters();
        void ProcessQueryCallbacks();

        QueryCallback<QueryResult, bool, true> m_VoteTimeCallback;

        PreparedQueryResultFuture m_CharEnumCallback;
        PreparedQueryResultFuture m_AddIgnoreCallback;

#ifdef CROSS
        struct BattlegroundLeaveData
        {
            uint64 PlayerGuid;
            uint8 desertir;
        };

        BattlegroundLeaveData _battlegroundLeaveData;
#else
        /// Cross realm sync data
        BattlegroundPortData _battlegroundPortData;
        CrossPartyInfo m_CrossPartyInfo;
        uint32 m_InterRealmZoneId;
        bool m_BackFromCross;
        std::list<std::string> m_SpecialChannelsSave;
#endif
        QueryCallback<PreparedQueryResult, std::string> _charRenameCallback;
        QueryCallback<PreparedQueryResult, std::string> _addFriendCallback;
        QueryCallback<PreparedQueryResult, uint32> _setPetSlotCallback;
        QueryCallback<PreparedQueryResult, uint64> _sendStabledPetCallback;
        QueryCallback<PreparedQueryResult, CharacterCreateInfo*, true> _charCreateCallback;
        QueryResultHolderFuture m_CharacterLoginCallback;
        QueryResultHolderFuture m_CharacterLoginDBCallback;

        //////////////////////////////////////////////////////////////////////////
        /// New transaction query callback system
        //////////////////////////////////////////////////////////////////////////
        using TransactionCallbacks = std::forward_list < std::shared_ptr<MS::Utilities::Callback> > ;
        std::unique_ptr<TransactionCallbacks> m_TransactionCallbacks;
        std::mutex m_TransactionCallbackLock;

        //////////////////////////////////////////////////////////////////////////
        /// New prepare statement query callback system
        //////////////////////////////////////////////////////////////////////////
        using PrepareStatementCallback = std::pair < std::function<void(PreparedQueryResult)>, PreparedQueryResultFuture > ;
        using PreparedStatementCallbacks = std::forward_list < PrepareStatementCallback > ;
        std::unique_ptr<PreparedStatementCallbacks> m_PreparedStatementCallbacks;
        std::unique_ptr<PreparedStatementCallbacks> m_PreparedStatementCallbacksBuffer;
        std::mutex m_PreparedStatementCallbackLock;

    private:
        // private trade methods
        void moveItems(Item* myItems[], Item* hisItems[]);

        // logging helper
        void LogUnexpectedOpcode(WorldPacket* packet, const char* status, const char *reason);
        void LogUnprocessedTail(WorldPacket* packet);

        // EnumData helpers
        bool CharCanLogin(uint32 lowGUID)
        {
            return _allowedCharsToLogin.find(lowGUID) != _allowedCharsToLogin.end();
        }

        // this stores the GUIDs of the characters who can login
        // characters who failed on Player::BuildEnumData shouldn't login
        std::set<uint32> _allowedCharsToLogin;

        uint32 m_GUIDLow;                                   // set loggined or recently logout player (while m_playerRecentlyLogout set)

        Player* m_Player;
        std::string m_Address;

        AccountTypes _security;
        uint32 _accountId;
        uint8 m_expansion;

        uint16 m_ClientBuild;

        uint64 m_AccountJoinDate;

#ifdef CROSS
        InterRealmClient* m_ir_socket;
        bool m_isinIRBG;
        std::string m_ir_server_name;
        bool m_ir_closing;
        uint32 m_ir_number;
        uint8 m_RemoveType;
        uint8 m_RemoveProgress;

        uint64 m_GUID;
        uint64 m_RealGUID; 
#else
        WorldSocket* m_Socket;

#endif

        //////////////////////////////////////////////////////////////////////////
        /// Premium
        //////////////////////////////////////////////////////////////////////////
        bool _ispremium;
        uint8 m_PremiumType;

        //////////////////////////////////////////////////////////////////////////
        /// Vote
        //////////////////////////////////////////////////////////////////////////
        uint32 m_VoteRemainingTime;
        uint32 m_VoteTimePassed;
        uint32 m_VoteSyncTimer;

        //////////////////////////////////////////////////////////////////////////
        /// Loyalty points system
        //////////////////////////////////////////////////////////////////////////

        uint32 m_ActivityDays;
        time_t m_LastBan;
        time_t m_LastClaim;
        time_t m_LastEventReset;
        bool   m_EmailValidated;
        bool   m_AlreadyPurchasePoints;
        std::vector<uint32> m_LoyaltyEvents;

        typedef std::list<AddonInfo> AddonsList;

        // Warden
        Warden* _warden;                                    // Remains NULL if Warden system is not enabled by config

        time_t _logoutTime;
        bool m_inQueue;                                     // session wait in auth.queue
        bool m_playerLoading;                               // code processed in LoginPlayer
        bool m_playerLogout;                                // code processed in LogoutPlayer
        bool m_playerRecentlyLogout;
        bool m_playerSave;
        bool m_IsPetBattleJournalLocked;
        LocaleConstant m_sessionDbcLocale;
        LocaleConstant m_sessionDbLocaleIndex;
        uint32 m_latency;
        std::atomic<uint32> m_clientTimeDelay;
        AccountData m_accountData[NUM_ACCOUNT_DATA_TYPES];
        uint32 m_Tutorials[MAX_ACCOUNT_TUTORIAL_VALUES];
        bool   m_TutorialsChanged;
        AddonsList m_addonsList;
        std::vector<std::string> _registeredAddonPrefixes;
        bool _filterAddonMessages;
        uint32 recruiterId;
        bool isRecruiter;
        ACE_Based::LockedQueue<WorldPacket*, ACE_Thread_Mutex> _recvQueue;
        time_t timeLastWhoCommand;
        time_t timeCharEnumOpcode;
        time_t m_TimeLastChannelInviteCommand;
        time_t m_TimeLastChannelPassCommand;
        time_t m_TimeLastChannelMuteCommand;
        time_t m_TimeLastChannelBanCommand;
        time_t m_TimeLastChannelUnbanCommand;
        time_t m_TimeLastChannelAnnounceCommand;
        time_t m_TimeLastGroupInviteCommand;
        time_t m_TimeLastGuildInviteCommand;
        time_t m_TimeLastChannelModerCommand;
        time_t m_TimeLastChannelOwnerCommand;
        time_t m_TimeLastChannelSetownerCommand;
        time_t m_TimeLastChannelUnmoderCommand;
        time_t m_TimeLastChannelUnmuteCommand;
        time_t m_TimeLastChannelKickCommand;
        time_t timeLastServerCommand;
        time_t timeLastArenaTeamCommand;
        time_t timeLastChangeSubGroupCommand;
        time_t m_TimeLastSellItemOpcode;
        time_t m_TimeLastUseItem;

        uint32 m_uiAntispamMailSentCount;
        uint32 m_uiAntispamMailSentTimer;

        uint8 m_PlayerLoginCounter;
        z_stream_s* _compressionStream;

        uint32 m_ServiceFlags;
        uint32 m_CustomFlags;
        time_t m_LoginTime;

        bool m_IsStressTestSession;
};
#endif
/// @}
