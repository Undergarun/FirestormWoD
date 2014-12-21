/// Copyright Ashran 2014-2015

#ifndef SC_SCRIPTMGR_H
#define SC_SCRIPTMGR_H

#include "Common.h"
#include "DBCStores.h"
#include "Interfaces/Interfaces.hpp"

// Placed here due to ScriptRegistry::AddScript dependency.
#define sScriptMgr ACE_Singleton<ScriptMgr, ACE_Null_Mutex>::instance()

// Manages registration, loading, and execution of scripts.
class ScriptMgr
{
    friend class ACE_Singleton<ScriptMgr, ACE_Null_Mutex>;
    friend class ScriptObjectImpl<true>;
    friend class ScriptObjectImpl<false>;

    private:

        ScriptMgr();
        virtual ~ScriptMgr();

    public: /* Initialization */

        void Initialize();
        void LoadDatabase();
        void FillSpellSummary();

        const char* ScriptsVersion() const { return "Integrated Trinity Scripts"; }

        void IncrementScriptCount() { ++_scriptCount; }
        uint32 GetScriptCount() const { return _scriptCount; }

    public: /* Unloading */

        void Unload();

    public: /* SpellScriptLoader */

        void CreateSpellScripts(uint32 spellId, std::list<SpellScript*>& scriptVector);
        void CreateAuraScripts(uint32 spellId, std::list<AuraScript*>& scriptVector);
        void CreateSpellScriptLoaders(uint32 spellId, std::vector<std::pair<SpellScriptLoader*, std::multimap<uint32, uint32>::iterator> >& scriptVector);

    public: /* ServerScript */

        void OnNetworkStart();
        void OnNetworkStop();
        void OnSocketOpen(WorldSocket* socket);
        void OnSocketClose(WorldSocket* socket, bool wasNew);
        void OnPacketReceive(WorldSocket* socket, WorldPacket packet);
        void OnPacketSend(WorldSocket* socket, WorldPacket packet);
        void OnUnknownPacketReceive(WorldSocket* socket, WorldPacket packet);

    public: /* WorldScript */

        void OnOpenStateChange(bool open);
        void OnConfigLoad(bool reload);
        void OnMotdChange(std::string& newMotd);
        void OnShutdownInitiate(ShutdownExitCode code, ShutdownMask mask);
        void OnShutdownCancel();
        void OnWorldUpdate(uint32 diff);
        void OnStartup();
        void OnShutdown();

    public: /* FormulaScript */

        void OnHonorCalculation(float& honor, uint8 level, float multiplier);
        void OnGrayLevelCalculation(uint8& grayLevel, uint8 playerLevel);
        void OnColorCodeCalculation(XPColorChar& color, uint8 playerLevel, uint8 mobLevel);
        void OnZeroDifferenceCalculation(uint8& diff, uint8 playerLevel);
        void OnBaseGainCalculation(uint32& gain, uint8 playerLevel, uint8 mobLevel, ContentLevels content);
        void OnGainCalculation(uint32& gain, Player* player, Unit* unit);
        void OnGroupRateCalculation(float& rate, uint32 count, bool isRaid);

    public: /* MapScript */

        void OnCreateMap(Map* map);
        void OnDestroyMap(Map* map);
        void OnLoadGridMap(Map* map, GridMap* gmap, uint32 gx, uint32 gy);
        void OnUnloadGridMap(Map* map, GridMap* gmap, uint32 gx, uint32 gy);
        void OnPlayerEnterMap(Map* map, Player* player);
        void OnPlayerLeaveMap(Map* map, Player* player);
        void OnMapUpdate(Map* map, uint32 diff);

    public: /* InstanceMapScript */

        InstanceScript* CreateInstanceData(InstanceMap* map);

    public: /* ItemScript */

        bool OnDummyEffect(Unit* caster, uint32 spellId, SpellEffIndex effIndex, Item* target);
        bool OnQuestAccept(Player* player, Item* item, Quest const* quest);
        bool OnItemUse(Player* player, Item* item, SpellCastTargets const& targets);
        bool OnItemExpire(Player* player, ItemTemplate const* proto);

    public: /* CreatureScript */

        bool OnDummyEffect(Unit* caster, uint32 spellId, SpellEffIndex effIndex, Creature* target);
        bool OnGossipHello(Player* player, Creature* creature);
        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action);
        bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code);
        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest);
        bool OnQuestSelect(Player* player, Creature* creature, Quest const* quest);
        bool OnQuestComplete(Player* player, Creature* creature, Quest const* quest);
        bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 opt);
        uint32 GetDialogStatus(Player* player, Creature* creature);
        CreatureAI* GetCreatureAI(Creature* creature);
        void OnCreatureUpdate(Creature* creature, uint32 diff);

    public: /* GameObjectScript */

        bool OnDummyEffect(Unit* caster, uint32 spellId, SpellEffIndex effIndex, GameObject* target);
        bool OnGossipHello(Player* player, GameObject* go);
        bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action);
        bool OnGossipSelectCode(Player* player, GameObject* go, uint32 sender, uint32 action, const char* code);
        bool OnQuestAccept(Player* player, GameObject* go, Quest const* quest);
        bool OnQuestReward(Player* player, GameObject* go, Quest const* quest, uint32 opt);
        uint32 GetDialogStatus(Player* player, GameObject* go);
        void OnGameObjectDestroyed(GameObject* go, Player* player);
        void OnGameObjectDamaged(GameObject* go, Player* player);
        void OnGameObjectLootStateChanged(GameObject* go, uint32 state, Unit* unit);
        void OnGameObjectStateChanged(GameObject* go, uint32 state);
        void OnGameObjectUpdate(GameObject* go, uint32 diff);

        /// - Transport GO
        bool OnGameObjectElevatorCheck(GameObject const* p_GameObject) const;

        GameObjectAI* GetGameObjectAI(GameObject* go);

    public:
        /* AreaTriggerEntityScript */
        void OnCreateAreaTriggerEntity(AreaTrigger* p_AreaTrigger);
        void OnUpdateAreaTriggerEntity(AreaTrigger* p_AreaTrigger, uint32 p_Time);
        void OnRemoveAreaTriggerEntity(AreaTrigger* p_AreaTrigger, uint32 p_Time);

    public: /* AreaTriggerScript */

        bool OnAreaTrigger(Player* player, AreaTriggerEntry const* trigger);

    public: /* BattlegroundScript */

        Battleground* CreateBattleground(BattlegroundTypeId typeId);

    public: /* OutdoorPvPScript */

        OutdoorPvP* CreateOutdoorPvP(OutdoorPvPData const* data);

    public: /* CommandScript */

        std::vector<ChatCommand*> GetChatCommands();

    public: /* WeatherScript */

        void OnWeatherChange(Weather* weather, WeatherState state, float grade);
        void OnWeatherUpdate(Weather* weather, uint32 diff);

    public: /* AuctionHouseScript */

        void OnAuctionAdd(AuctionHouseObject* ah, AuctionEntry* entry);
        void OnAuctionRemove(AuctionHouseObject* ah, AuctionEntry* entry);
        void OnAuctionSuccessful(AuctionHouseObject* ah, AuctionEntry* entry);
        void OnAuctionExpire(AuctionHouseObject* ah, AuctionEntry* entry);

    public: /* ConditionScript */

        bool OnConditionCheck(Condition* condition, ConditionSourceInfo& sourceInfo);

    public: /* VehicleScript */

        void OnInstall(Vehicle* veh);
        void OnUninstall(Vehicle* veh);
        void OnReset(Vehicle* veh);
        void OnInstallAccessory(Vehicle* veh, Creature* accessory);
        void OnAddPassenger(Vehicle* veh, Unit* passenger, int8 seatId);
        void OnRemovePassenger(Vehicle* veh, Unit* passenger);

    public: /* DynamicObjectScript */

        void OnDynamicObjectUpdate(DynamicObject* dynobj, uint32 diff);

    public: /* TransportScript */

        void OnAddPassenger(Transport* transport, Player* player);
        void OnAddCreaturePassenger(Transport* transport, Creature* creature);
        void OnRemovePassenger(Transport* transport, Player* player);
        void OnTransportUpdate(Transport* transport, uint32 diff);
        void OnRelocate(Transport* transport, uint32 waypointId, uint32 mapId, float x, float y, float z);

    public: /* AchievementCriteriaScript */

        bool OnCriteriaCheck(uint32 scriptId, Player* source, Unit* target);

    public: /* PlayerScript */

        void OnPVPKill(Player* killer, Player* killed);
        void OnModifyPower(Player* player, Powers power, int32 value);
        void OnCreatureKill(Player* killer, Creature* killed);
        void OnPlayerKilledByCreature(Creature* killer, Player* killed);
        void OnPlayerLevelChanged(Player* player, uint8 oldLevel);
        void OnPlayerTalentsReset(Player* player, bool noCost);
        void OnPlayerMoneyChanged(Player* player, int64& amount);
        void OnGivePlayerXP(Player* player, uint32& amount, Unit* victim);
        void OnPlayerReputationChange(Player* player, uint32 factionID, int32& standing, bool incremental);
        void OnPlayerDuelRequest(Player* target, Player* challenger);
        void OnPlayerDuelStart(Player* player1, Player* player2);
        void OnPlayerDuelEnd(Player* winner, Player* loser, DuelCompleteType type);
        void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg);
        void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver);
        void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Group* group);
        void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild);
        void OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Channel* channel);
        void OnPlayerEmote(Player* player, uint32 emote);
        void OnPlayerTextEmote(Player* player, uint32 textEmote, uint32 emoteNum, uint64 guid);
        void OnPlayerSpellCast(Player* player, Spell* spell, bool skipCheck);
        void OnPlayerSpellLearned(Player* p_Player, uint32 p_SpellId);
        void OnPlayerLogin(Player* player);
        void OnPlayerLogout(Player* player);
        void OnPlayerCreate(Player* player);
        void OnPlayerDelete(uint64 guid);
        void OnPlayerBindToInstance(Player* player, Difficulty difficulty, uint32 mapid, bool permanent);
        void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32 p_OldZoneID, uint32 newArea);
        void OnPlayerUpdateMovement(Player* p_Player);
        void OnQuestReward(Player* player, const Quest* quest);
        void OnObjectiveValidate(Player* player, uint32 questId, uint32 ObjectiveId);
        void OnPlayerChangeShapeshift(Player* p_Player, ShapeshiftForm p_Form);

    public: /* GuildScript */

        void OnGuildAddMember(Guild* guild, Player* player, uint8& plRank);
        void OnGuildRemoveMember(Guild* guild, Player* player, bool isDisbanding, bool isKicked);
        void OnGuildMOTDChanged(Guild* guild, const std::string& newMotd);
        void OnGuildInfoChanged(Guild* guild, const std::string& newInfo);
        void OnGuildCreate(Guild* guild, Player* leader, const std::string& name);
        void OnGuildDisband(Guild* guild);
        void OnGuildMemberWitdrawMoney(Guild* guild, Player* player, uint64 &amount, bool isRepair);
        void OnGuildMemberDepositMoney(Guild* guild, Player* player, uint64 &amount);
        void OnGuildItemMove(Guild* guild, Player* player, Item* pItem, bool isSrcBank, uint8 srcContainer, uint8 srcSlotId,
            bool isDestBank, uint8 destContainer, uint8 destSlotId);
        void OnGuildEvent(Guild* guild, uint8 eventType, uint32 playerGuid1, uint32 playerGuid2, uint8 newRank);
        void OnGuildBankEvent(Guild* guild, uint8 eventType, uint8 tabId, uint32 playerGuid, uint32 itemOrMoney, uint16 itemStackCount, uint8 destTabId);

    public: /* GroupScript */

        void OnGroupAddMember(Group* group, uint64 guid);
        void OnGroupInviteMember(Group* group, uint64 guid);
        void OnGroupRemoveMember(Group* group, uint64 guid, RemoveMethod method, uint64 kicker, const char* reason);
        void OnGroupChangeLeader(Group* group, uint64 newLeaderGuid, uint64 oldLeaderGuid);
        void OnGroupDisband(Group* group);

    public: /* Scheduled scripts */

        uint32 IncreaseScheduledScriptsCount() { return ++_scheduledScripts; }
        uint32 DecreaseScheduledScriptCount() { return --_scheduledScripts; }
        uint32 DecreaseScheduledScriptCount(size_t count) { return _scheduledScripts -= count; }
        bool IsScriptScheduled() const { return _scheduledScripts > 0; }

    private:

        uint32 _scriptCount;

        //atomic op counter for active scripts amount
        ACE_Atomic_Op<ACE_Thread_Mutex, long> _scheduledScripts;
};

#endif
