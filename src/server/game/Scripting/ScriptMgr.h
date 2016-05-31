////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SC_SCRIPTMGR_H
# define SC_SCRIPTMGR_H

#include "Common.h"
#include "DBCStores.h"
#include "Interfaces/Interfaces.hpp"
#include "MutexedMap.hpp"

/// Placed here due to ScriptRegistry::AddScript dependency.
#define sScriptMgr ACE_Singleton<ScriptMgr, ACE_Null_Mutex>::instance()

/// Manages registration, loading, and execution of scripts.
class ScriptMgr
{
    friend class ACE_Singleton<ScriptMgr, ACE_Null_Mutex>;
    friend class ScriptObjectImpl<true>;
    friend class ScriptObjectImpl<false>;

    private:
        /// Constructor
        ScriptMgr();
        /// Destructor
        virtual ~ScriptMgr();

    public:
        /// Initialize Script Mgr and bind all script
        void Initialize();
        /// Unload all script
        void Unload();

        /// Load all script data (text, waypoint, ...)
        void LoadDatabase();

        /// Increase script count
        void IncrementScriptCount();
        /// Get script count
        uint32 GetScriptCount() const;

        /// Initialize some spell date for scripted creature
        void FillSpellSummary();

    /// Scheduled scripts
    public:
        /// Increase scheduled script count
        uint32 IncreaseScheduledScriptsCount();
        /// Decrease scheduled script count
        uint32 DecreaseScheduledScriptCount();
        /// Decrease scheduled script count
        uint32 DecreaseScheduledScriptCount(size_t count);
        /// Is script scheduled
        bool IsScriptScheduled() const;

    /// AreaTriggerScript
    public:
        /// Called when the area trigger is activated by a player.
        /// @p_Player  : Player who trigger this area trigger
        /// @p_Trigger : Area Trigger
        bool OnAreaTrigger(Player* p_Player, AreaTriggerEntry const* p_Trigger);

        /// Called when a player enters the AreaTrigger
        void OnEnterAreaTrigger(Player* p_Player, AreaTriggerEntry const* p_AreatriggerEntry);

        /// Called when a player exits the AreaTrigger
        void OnExitAreaTrigger(Player* p_Player, AreaTriggerEntry const* p_AreatriggerEntry);

    /// AreaTriggerScript
    public:
        /// Assign script to Areatrigger
        void InitScriptEntity(AreaTrigger* p_AreaTrigger);

        /// Proc when AreaTrigger is created.
        /// @p_AreaTrigger : AreaTrigger instance
        void OnCreateAreaTriggerEntity(AreaTrigger* p_AreaTrigger);

        /// Procs before creation to specify position and linear destination of the areatrigger
        /// @p_AreaTrigger: Areatrigger Instance
        /// @p_Caster: Caster because he the Areatrigger is not spawned so caster is not defined
        /// @p_SourcePosition: Spawn location of the Areatrigger
        /// @p_DestinationPostion: Linear destination of the Areatrigger
        /// @p_PathToLinearDestination: Linear path without the endpoint
        void OnSetCreatePositionEntity(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination);

        /// Proc when AreaTrigger is updated.
        /// @p_AreaTrigger : AreaTrigger instance
        /// @p_Time        : Diff since last update
        void OnUpdateAreaTriggerEntity(AreaTrigger* p_AreaTrigger, uint32 p_Time);

        /// Proc when AreaTrigger is removed.
        /// @p_AreaTrigger : AreaTrigger instance
        /// @p_Time        : Diff since last update
        void OnRemoveAreaTriggerEntity(AreaTrigger* p_AreaTrigger, uint32 p_Time);

        /// Called when AreaTrigger is arrived to DestPos
        void OnDestinationReached(AreaTrigger* p_AreaTrigger);

    /// CreatureScript
    public:
        /// Get Creature script by ScriptID
        /// @p_ScriptID : ScriptID from the creature template
        CreatureScript* GetCreatureScriptByID(uint32 p_ScriptID);

        /// Called when a dummy spell effect is triggered on the creature.
        /// @p_Caster      : Spell Caster
        /// @p_SpellID     : Casted spell ID
        /// @p_EffectIndex : Dummy effect index
        /// @p_Target      : Spell target
        bool OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, Creature* p_Target);

        /// Called when a player opens a gossip dialog with the creature.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        bool OnGossipHello(Player* p_Player, Creature* p_Creature);
        /// Called when a player selects a gossip item in the creature's gossip menu.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Sender   : Sender menu
        /// @p_Action   : Action
        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action);
        /// Called when a player selects a gossip with a code in the creature's gossip menu.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Sender   : Sender menu
        /// @p_Action   : Action
        /// @p_Code     : Player input code
        bool OnGossipSelectCode(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action, char const* p_Code);

        /// Called when a player accepts a quest from the creature.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Accepted quest
        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, Quest const* p_Quest);
        /// Called when a player selects a quest in the creature's quest menu.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Selected quest
        bool OnQuestSelect(Player* p_Player, Creature* p_Creature, Quest const* p_Quest);
        /// Called when a player completes a quest with the creature.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Completed quest
        bool OnQuestComplete(Player* p_Player, Creature* p_Creature, Quest const* p_Quest);
        /// Called when a player selects a quest reward.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Quest    : Reward quest
        /// @p_Option   : Reward
        bool OnQuestReward(Player* p_Player, Creature* p_Creature, Quest const* p_Quest, uint32 p_Option);

        /// Called when the dialog status between a player and the creature is requested.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        uint32 GetDialogStatus(Player* p_Player, Creature* p_Creature);

        /// Called when player asks from a creature gossip to create a shipment.
        /// @p_Player       : Player that created the shipment
        /// @p_Creature     : NPC that got the gossip
        void OnShipmentCreated(Player* p_Player, Creature* p_Creature, uint32 p_BuildingID);

        /// Called when a CreatureAI object is needed for the creature.
        /// @p_Creature : Target creature instance
        CreatureAI* GetCreatureAI(Creature* p_Creature);

        /// On update
        /// @p_Object : Updated object instance
        /// @p_Diff   : Time since last update
        void OnCreatureUpdate(Creature* p_Object, uint32 p_Diff);

    /// GameObjectScript
    public:
        /// Called when a dummy spell effect is triggered on the GameObject.
        /// @p_Caster      : Spell Caster
        /// @p_SpellID     : Casted spell ID
        /// @p_EffectIndex : Dummy effect index
        /// @p_Target      : Spell target
        bool OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, GameObject* p_Target);

        /// Called when a player opens a gossip dialog with the GameObject.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target GameObject instance
        bool OnGossipHello(Player* p_Player, GameObject* p_GameObject);
        /// Called when a player selects a gossip item in the GameObject's gossip menu.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Sender     : Sender menu
        /// @p_Action     : Action
        bool OnGossipSelect(Player* p_Player, GameObject* p_GameObject, uint32 p_Sender, uint32 p_Action);
        /// Called when a player selects a gossip with a code in the GameObject's gossip menu.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Sender     : Sender menu
        /// @p_Action     : Action
        /// @p_Code       : Player input code
        bool OnGossipSelectCode(Player* p_Player, GameObject* p_GameObject, uint32 p_Sender, uint32 p_Action, char const* p_Code);

        /// Called when a player accepts a quest from the game object.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Quest      : Accepted quest
        bool OnQuestAccept(Player* p_Player, GameObject* p_GameObject, Quest const* p_Quest);
        /// Called when a player selects a quest reward.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        /// @p_Quest      : Reward quest
        /// @p_Option     : Reward
        bool OnQuestReward(Player* p_Player, GameObject* p_GameObject, Quest const* p_Quest, uint32 p_Option);

        /// Called when the dialog status between a player and the GameObject is requested.
        /// @p_Player     : Source player instance
        /// @p_GameObject : Target GameObject instance
        uint32 GetDialogStatus(Player* p_Player, GameObject* p_GameObject);

        // Called when the game object is destroyed (destructible buildings only).
        /// @p_GameObject : Destroyed GameObject
        /// @p_Player     : Destroyer player instance
        void OnGameObjectDestroyed(GameObject* p_GameObject, Player* p_Player);
        /// Called when the game object is damaged (destructible buildings only).
        /// @p_GameObject : Damaged GameObject
        /// @p_Player     : Damager player instance
        void OnGameObjectDamaged(GameObject* p_GameObject, Player* p_Player);
        /// Called when the game object loot state is changed.
        /// @p_GameObject : Looted GameObject
        /// @p_State      : Loot state
        /// @p_Unit       : Unit
        void OnGameObjectLootStateChanged(GameObject* p_GameObject, uint32 p_State, Unit* p_Unit);
        /// Called when the game object state is changed.
        /// @p_GameObject : Changed GameObject
        /// @p_State      : GameObject state
        void OnGameObjectStateChanged(GameObject* p_GameObject, uint32 p_State);

        /// Called when server want to send elevator update, by default all GameObject type transport are elevator
        /// @p_GameObject : GameObject instance
        bool OnGameObjectElevatorCheck(GameObject const* p_GameObject) const;

        bool OnGameObjectSpellCasterUse(GameObject const* p_GameObject, Player* p_User) const;

        /// Called when a GameObjectAI object is needed for the GameObject.
        /// @p_GameObject : GameObject instance
        GameObjectAI* GetGameObjectAI(GameObject* p_GameObject);

        /// On update
        /// @p_Object : Updated object instance
        /// @p_Diff   : Time since last update
        void OnGameObjectUpdate(GameObject* p_Object, uint32 p_Diff);

    /// GroupScript
    public:
        /// Called when a member is added to a group.
        /// @p_Group : Group Instance
        /// @p_GUID  : Added member GUID
        void OnGroupAddMember(Group* p_Group, uint64 p_Guid);
        /// Called when a member is invited to join a group.
        /// @p_Group : Group Instance
        /// @p_GUID  : Invited member GUID
        void OnGroupInviteMember(Group* p_Group, uint64 p_Guid);
        /// Called when a member is removed from a group.
        /// @p_Group      : Group Instance
        /// @p_GUID       : Removed member GUID
        /// @p_Method     : Remove method
        /// @p_KickerGUID : Kicker GUID
        /// @p_Reason     : Kick reason
        void OnGroupRemoveMember(Group* p_Group, uint64 p_GUID, RemoveMethod p_Method, uint64 p_KickerGUID, char const* p_Reason);

        /// Called when the leader of a group is changed.
        /// @p_Group         : Group Instance
        /// @p_NewLeaderGUID : New group leader GUID
        /// @p_OldLeaderGUID : Old group leader GUID
        void OnGroupChangeLeader(Group* p_Group, uint64 p_NewLeaderGUID, uint64 p_OldLeaderGUID);

        /// Called when a group is disbanded.
        /// @p_Group : Group Instance
        void OnGroupDisband(Group* p_Group);

    /// GuildScript
    public:
        /// Called when a member is added to the guild.
        /// @p_Guild  : Guild instance
        /// @p_Player : Added player
        /// @p_Rank   : Added player destination rank
        void OnGuildAddMember(Guild* p_Guild, Player* p_Player, uint8& p_Rank);
        /// Called when a member is removed from the guild.
        /// @p_Guild        : Guild instance
        /// @p_Player       : Removed player
        /// @p_IdDisbanding : Player is removed from a guild disbanding
        /// @p_IsKicked     : Is that removed player kicked
        void OnGuildRemoveMember(Guild* p_Guild, Player* p_Player, bool p_IdDisbanding, bool p_IsKicked);

        /// Called when the guild MOTD (message of the day) changes.
        /// @p_Guild   : Guild instance
        /// @p_NewMotd : New message of the day
        void OnGuildMOTDChanged(Guild* p_Guild, std::string const& p_NewMotd);
        /// Called when the guild info is altered.
        /// @p_Guild   : Guild instance
        /// @p_NewInfo : New guild info
        void OnGuildInfoChanged(Guild* p_Guild, std::string const& p_NewInfo);

        /// Called when a guild is created.
        /// @p_Guild  : Guild instance
        /// @p_Leader : Guild leader
        /// @p_Name   : Guild Name
        void OnGuildCreate(Guild* p_Guild, Player* p_Leader, std::string const& p_Name);
        /// Called when a guild is disbanded.
        /// @p_Guild : Guild instance
        void OnGuildDisband(Guild* p_Guild);

        /// Called when a guild member withdraws money from a guild bank.
        /// @p_Guild    : Guild instance
        /// @p_Player   : Withdrawer player
        /// @p_Amount   : Dest gold amount
        /// @p_IsRepair : Is repair
        void OnGuildMemberWitdrawMoney(Guild* p_Guild, Player* p_Player, uint64& p_Amount, bool p_IsRepair);
        /// Called when a guild member deposits money in a guild bank.
        /// @p_Guild  : Guild instance
        /// @p_Player : Depositor player
        /// @p_Amount : Dest gold amount
        void OnGuildMemberDepositMoney(Guild* p_Guild, Player* p_Player, uint64& p_Amount);

        /// Called when a guild member moves an item in a guild bank.
        /// @p_Guild         : Guild instance
        /// @p_Player        : Player item mover
        /// @p_Item          : Item instance
        /// @p_IsSrcBank     : Is from guild bank
        /// @p_SrcContainer  : Source Bag
        /// @p_SrcSlotID     : Source Bag slot ID
        /// @p_IsDestBank    : Is to guild bank
        /// @p_DestContainer : Destination Bag
        /// @p_DestSlotID    : Destination Bag slot ID
        void OnGuildItemMove(Guild* p_Guild, Player* p_Player, Item* p_Item, bool p_IsSrcBank, uint8 p_SrcContainer, uint8 p_SrcSlotID, bool p_IsDestBank, uint8 p_DestContainer, uint8 p_DestSlotID);

        /// On Guild event
        /// @p_Guild       : Guild instance
        /// @p_EventType   : Event type
        /// @p_PlayerGUID1 : Player GUID 1
        /// @p_PlayerGUID2 : Player GUID 2
        /// @p_NewRank     : New Rank (contextual)
        void OnGuildEvent(Guild* p_Guild, uint8 p_EventType, uint32 p_PlayerGUID1, uint32 p_PlayerGUID2, uint8 p_NewRank);
        /// @p_Guild          : Guild instance
        /// @p_EventType      : Event type
        /// @p_TabID          : Source tab ID
        /// @p_PlayerGUID     : Player GUID
        /// @p_ItemOrMoney    : Item entry or gold amount
        /// @p_ItemStackCount : Item stack count
        /// @p_DestTabID      : Destination tab ID
        void OnGuildBankEvent(Guild* p_Guild, uint8 p_EventType, uint8 p_TabID, uint32 p_PlayerGUID, uint64 p_ItemOrMoney, uint16 p_ItemStackCount, uint8 p_DestTabID);

    /// ItemScript
    public:
        /// Called when a dummy spell effect is triggered on the item.
        /// @p_Caster      : Caster unit instance
        /// @p_SpellID     : Dummy effect origin spell ID
        /// @p_EffectIndex : Dummy effect index
        /// @p_Target      : Spell target
        bool OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, Item* p_Target);

        /// Called when a player accepts a quest from the item.
        /// @p_Player : Player who accepted quest from this item
        /// @p_Item   : Item quest owner instance
        /// @p_Quest  : Accepted quest instance
        bool OnQuestAccept(Player* p_Player, Item* p_Item, Quest const* p_Quest);

        /// Called when a player uses the item.
        /// @p_Player           : Player who use this item
        /// @p_Item             : Used Item instance
        /// @p_SpellCastTargets : Item spell action targets
        bool OnItemUse(Player* p_Player, Item* p_Item, SpellCastTargets const& p_SpellCastTargets);

        /// Called when a player open the item
        /// @p_Player : The Player who has used this item
        /// @p_Item   : Used Item instance
        bool OnItemOpen(Player* p_Player, Item* p_Item);

        /// Called when the item expires (is destroyed).
        /// @p_Player       : Item destroyer player instance
        /// @p_ItemTemplate : Destroyed item template
        bool OnItemExpire(Player* p_Player, ItemTemplate const* p_ItemTemplate);

    /// MapScript
    public:
        /// Called when the map is created.
        /// @p_Map : Created map instance
        void OnCreateMap(Map* p_Map);
        /// Called just before the map is destroyed.
        /// @p_Map : Destroyed map instance
        void OnDestroyMap(Map* p_Map);

        /// Called when a grid map is loaded.
        /// @p_Map     : Context map
        /// @p_GridMap : Grid map container
        /// @p_GridX   : Loaded grid X offset
        /// @p_GridY   : Loaded grid Y offset
        void OnLoadGridMap(Map* p_Map, GridMap* p_GripMap, uint32 p_GridX, uint32 p_GridY);
        /// Called when a grid map is unloaded.
        /// @p_Map     : Context map
        /// @p_GridMap : Grid map container
        /// @p_GridX   : Unloaded grid X offset
        /// @p_GridY   : Unloaded grid Y offset
        void OnUnloadGridMap(Map* p_Map, GridMap* p_GripMap, uint32 p_GridX, uint32 p_GridY);

        /// Called when a player enters the map.
        /// @p_Map    : Context map
        /// @p_Player : Entered player instance
        void OnPlayerEnterMap(Map* p_Map, Player* p_Player);
        /// Called when a player leaves the map.
        /// @p_Map    : Context map
        /// @p_Player : Leaved player instance
        void OnPlayerLeaveMap(Map* p_Map, Player* p_Player);

        /// Called on every map update tick.
        /// @p_Map  : Context map
        /// @p_Diff : Time since last update
        void OnMapUpdate(Map* p_Map, uint32 p_Diff);

    /// InstanceMapScript
    public:
        /// Gets an InstanceScript object for this instance.
        /// @p_Map : Context map
        InstanceScript* CreateInstanceData(InstanceMap* p_Map);

    /// ServerScript
    public:
        /// Called when reactive socket I/O is started (WorldSocketMgr).
        void OnNetworkStart();
        /// Called when reactive I/O is stopped.
        void OnNetworkStop();

        /// Called when a remote socket establishes a connection to the server. Do not store the socket object.
        /// @p_Socket : Opened socket
        void OnSocketOpen(WorldSocket* p_Socket);
        /// Called when a socket is closed. Do not store the socket object, and do not rely on the connection being open; it is not.
        /// @p_Socket : Closed socket
        /// @p_WasNew : Was new ?
        void OnSocketClose(WorldSocket* p_Socket, bool p_WasNew);

        /// Called when a packet is sent to a client. The packet object is a copy of the original packet, so reading and modifying it is safe.
        /// @p_Socket  : Socket who send the packet
        /// @p_Packet  : Sent packet
        /// @p_Session : Session who receive the packet /!\ CAN BE NULLPTR
        void OnPacketReceive(WorldSocket* p_Socket, WorldPacket p_Packet, WorldSession* p_Session = nullptr);

        /// Called when a (valid) packet is received by a client. The packet object is a copy of the original packet, so reading and modifying it is safe.
        /// @p_Socket : Socket who received the packet
        /// @p_Packet : Received packet
        void OnPacketSend(WorldSocket* p_Socket, WorldPacket p_Packet);
        /// Called when an invalid (unknown opcode) packet is received by a client. The packet is a reference to the original packet; not a copy.
        /// This allows you to actually handle unknown packets (for whatever purpose).
        /// @p_Socket : Socket who received the packet
        /// @p_Packet : Received packet
        void OnUnknownPacketReceive(WorldSocket* p_Socket, WorldPacket p_Packet);

    /// WorldScript
    public:
        /// Called when the open/closed state of the world changes.
        /// @p_Open : Open ?
        void OnOpenStateChange(bool p_Open);

        /// Called after the world configuration is (re)loaded.
        /// @p_Reload : Is the config reload
        void OnConfigLoad(bool p_Reload);

        /// Called before the message of the day is changed.
        /// @p_NewMotd : New server message of the day
        void OnMotdChange(std::string& p_NewMotd);

        /// Called when a world shutdown is initiated.
        /// @p_Code : Server exit code
        /// @p_Mask : Shutdown mask
        void OnShutdownInitiate(ShutdownExitCode p_Code, ShutdownMask p_Mask);

        /// Called when a world shutdown is cancelled.
        void OnShutdownCancel();

        /// Called on every world tick (don't execute too heavy code here).
        /// @p_Diff : Time since last update
        void OnWorldUpdate(uint32 p_Diff);

        /// Called when the world is started.
        void OnStartup();
        /// Called when the world is actually shut down.
        void OnShutdown();

    /// CommandScript
    public:
        /// Should return a pointer to a valid command table (ChatCommand array) to be used by ChatHandler.
        std::vector<ChatCommand*> GetChatCommands();

    /// WeatherScript
    public:
        /// Called when the weather changes in the zone this script is associated with.
        /// @p_Weather : Weather instance
        /// @p_State   : New weather state
        /// @p_Grade   : New weather grade
        void OnWeatherChange(Weather* p_Weather, WeatherState p_State, float p_Grade);

        /// On update
        /// @p_Object : Updated object instance
        /// @p_Diff   : Time since last update
        void OnWeatherUpdate(Weather* p_Object, uint32 p_Diff);

    /// DynamicObjectScript
    public:
        /// On update
        /// @p_Object : Updated object instance
        /// @p_Diff   : Time since last update
        void OnDynamicObjectUpdate(DynamicObject* p_Object, uint32 p_Diff);

    /// AuctionHouseScript
    public:
        /// Called when an auction is added to an auction house.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction to add
        void OnAuctionAdd(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry);

        /// Called when an auction is removed from an auction house.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction to remove
        void OnAuctionRemove(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry);
        /// Called when an auction was successfully completed.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction instance
        void OnAuctionSuccessful(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry);
        /// Called when an auction expires.
        /// @p_AuctionHouseObject : Auction House Object Instance
        /// @p_Entry              : Auction who expired
        void OnAuctionExpire(AuctionHouseObject* p_AuctionHouseObject, AuctionEntry* p_Entry);

    /// FormulaScript
    public:
        /// Called after calculating honor.
        /// @p_Honor      : Dest honor
        /// @p_Level      : Player level
        /// @p_Multiplier : Honor multiplier
        void OnHonorCalculation(float& p_Honor, uint8 p_Level, float p_Multiplier);

        /// Called after gray level calculation.
        /// @p_GrayLevel   : Dest gray level
        /// @p_PlayerLevel : Player level
        void OnGrayLevelCalculation(uint8& p_GrayLevel, uint8 p_PlayerLevel);

        /// Called after calculating experience color.
        /// @p_Color       : Dest XP Color
        /// @p_PlayerLevel : Player level
        /// @p_MobLevel    : Killed mob level
        void OnColorCodeCalculation(XPColorChar& p_Color, uint8 p_PlayerLevel, uint8 p_MobLevel);

        /// Called after calculating zero difference.
        /// @p_Diff        : Level difference
        /// @p_PlayerLevel : Player level
        void OnZeroDifferenceCalculation(uint8& p_Diff, uint8 p_PlayerLevel);

        /// Called after calculating base experience gain.
        /// @p_Gain        : Dest XP Gain
        /// @p_PlayerLevel : Rewarded player instance
        /// @p_MobLevel    : Killed mob level
        /// @p_Content     : Content expansion mob
        void OnBaseGainCalculation(uint32& p_Gain, uint8 p_PlayerLevel, uint8 p_MobLevel, ContentLevels p_Content);

        /// Called after calculating experience gain.
        /// @p_Gain   : Dest XP Gain
        /// @p_Player : Player instance for XP computation
        /// @p_Unit   : Killed unit
        void OnGainCalculation(uint32& p_Gain, Player* p_Player, Unit* p_Unit);

        /// Called when calculating the experience rate for group experience.
        /// @p_Rate   : Dest XP rate
        /// @p_Count  : Group member count
        /// @p_IsRaid : Is a raid group
        void OnGroupRateCalculation(float& p_Rate, uint32 p_Count, bool p_IsRaid);

    /// AchievementCriteriaScript
    public:
        /// Called when an additional criteria is checked.
        /// @p_ScriptID : Script ID
        /// @p_Source   : Criteria owner player
        /// @p_Target   : Target instance
        bool OnCriteriaCheck(uint32 p_ScriptID, Player* p_Source, Unit* p_Target);

    /// ConditionScript
    public:
        /// Called when a single condition is checked for a player.
        /// @p_Condition  : Condition instance
        /// @p_SourceInfo : Condition  source
        bool OnConditionCheck(Condition const* p_Condition, ConditionSourceInfo& p_SourceInfo);

    /// PlayerScript
    public:

        /// Called just before item is destroyed
        /// @p_Item        : Item to be destroyed
        /// @p_Player      : Player level
        void OnItemDestroyed(Player* p_Player, Item* p_Item);
        /// Called when a player kills another player
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        void OnPVPKill(Player* p_Killer, Player* p_Killed);

        /// Called when a player kills a Unit
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        void OnKill(Player* p_Killer, Unit* p_Killed);
        
        /// Called when a player kills a creature
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        void OnCreatureKill(Player* p_Killer, Creature* p_Killed);
        
        /// Called when a player is killed by a creature
        /// @p_Killer : Killer instance
        /// @p_Killed : Killed instance
        void OnPlayerKilledByCreature(Creature* p_Killer, Player* p_Killed);
        
        /// Called when power change is modify (SetPower)
        /// @p_Player : Player instance
        /// @p_Power  : Power type
        /// @p_OldValue  : Old value
        /// @p_NewValue  : New value
        /// @p_Regen  : If it's a regen modification
        /// @p_After : If it's after modification
        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After);

        /// Called when the player switch from indoors to outdoors or from outdoors to indoors
        /// @p_Player : Player instance
        /// @p_IsOutdoors : Bool setting whether player is indoors or outdoors
        void OnSwitchOutdoorsState(Player* p_Player, bool p_IsOutdoors);

        /// Called when specialisation is modify (SetSpecializationId)
        /// @p_Player : Player instance
        /// @p_NewSpec  : New Specialisation
        void OnModifySpec(Player* p_Player, int32 p_NewSpec);

        /// Called when a player kills another player
        /// @p_Player : Player instance
        /// @p_Value  : New value
        void OnModifyHealth(Player* p_Player, int32 p_Value);

        /// Called when a player's level changes (right before the level is applied)
        /// @p_Player   : Player instance
        /// @p_OldLevel : Old player Level
        void OnPlayerLevelChanged(Player* p_Player, uint8 p_OldLevel);

        /// Called when a player's talent points are reset (right before the reset is done)
        /// @p_Player : Player instance
        /// @p_NoCost : Talent was reset without cost
        void OnPlayerTalentsReset(Player* p_Player, bool p_NoCost);

        /// Called when a player's money is modified (before the modification is done)
        /// @p_Player : Player instance
        /// @p_Amount : Modified money amount
        void OnPlayerMoneyChanged(Player* p_Player, int64& p_Amount);

        /// Called when a player gains XP (before anything is given)
        /// @p_Player : Player instance
        /// @p_Amount : Modified XP amount
        /// @p_Victim : XP Source
        void OnGivePlayerXP(Player* p_Player, uint32& p_Amount, Unit* p_Victim);

        /// Called when a player's reputation changes (before it is actually changed)
        /// @p_Player       : Player instance
        /// @p_FactionID    : Reward faction ID
        /// @p_Standing     : Standing
        /// @p_Incremential : Is incremental
        void OnPlayerReputationChange(Player* p_Player, uint32 p_FactionID, int32& p_Standing, bool p_Incremential);

        /// Called when a duel is requested
        /// @p_Target     : Duel target
        /// @p_Challenger : Duel challenger
        void OnPlayerDuelRequest(Player* p_Target, Player* p_Challenger);
        /// Called when a duel starts (after 3s countdown)
        /// @p_Player1 : First player
        /// @p_Player1 : Second player
        void OnPlayerDuelStart(Player* p_Player1, Player* p_Player2);
        /// Called when a duel ends
        /// @p_Winner         : Duel winner
        /// @p_Looser         : Duel looser
        /// @p_CompletionType : Duel Completion Type
        void OnPlayerDuelEnd(Player* p_Winner, Player* p_Looser, DuelCompleteType p_CompletionType);

        /// Called when the player get Teleport
        /// @p_Player : Player
        /// @p_SpellID : SpellID
        void OnTeleport(Player* p_Player, SpellInfo const* p_SpellInfo);

        /// The following methods are called when a player sends a chat message. (World)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        void OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message);
        /// The following methods are called when a player sends a chat message. (Whisper)
        /// @p_Player   : Player instance
        /// @p_Type     : Message type
        /// @p_Lang     : Message language (WoW)
        /// @p_Message  : Message content
        /// @p_Receiver : Message receiver
        void OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Player* p_Receiver);
        /// The following methods are called when a player sends a chat message. (Party)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        /// @p_Group   : Message group target
        void OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Group* p_Group);
        /// The following methods are called when a player sends a chat message. (Guild)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        /// @p_Guild   : Message guild target
        void OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Guild* p_Guild);
        /// The following methods are called when a player sends a chat message. (Channel)
        /// @p_Player  : Player instance
        /// @p_Type    : Message type
        /// @p_Lang    : Message language (WoW)
        /// @p_Message : Message content
        /// @p_Channel : Message channel target
        void OnPlayerChat(Player* p_Player, uint32 p_Type, uint32 p_Lang, std::string& p_Message, Channel* p_Channel);

        /// Both of the below are called on emote opcodes.
        /// @p_Player : Player instance
        /// @p_Emote  : Emote ID
        void OnPlayerEmote(Player* p_Player, uint32 p_Emote);
        /// When player start a text emote
        /// @p_Player     : Player instance
        /// @p_TextEmote  : Text emote ID
        /// @p_SoundIndex : Emote sound ID
        /// @p_TargetGUID : Text emote target GUID
        void OnPlayerTextEmote(Player* p_Player, uint32 p_TextEmote, uint32 p_SoundIndex, uint64 p_TargetGUID);

        /// Called in Spell::Cast.
        /// @p_Player    : Player instance
        /// @p_Spell     : Casted spell
        /// @p_SkipCheck : Skipped checks
        void OnPlayerSpellCast(Player* p_Player, Spell* p_Spell, bool p_SkipCheck);
        /// When the player learn a spell
        /// @p_Player  : Player instance
        /// @p_SpellID : Learned spell ID
        void OnPlayerSpellLearned(Player* p_Player, uint32 p_SpellID);

        /// Called when a player logs in.
        /// @p_Player : Player instance
        void OnPlayerLogin(Player* p_Player);

        /// Called when a player logs out.
        /// @p_Player : Player instance
        void OnPlayerLogout(Player* p_Player);

        /// Called when a player is created.
        /// @p_Player : Player instance
        void OnPlayerCreate(Player* p_Player);

        /// Called when a player is deleted.
        /// @p_GUID : Player instance
        void OnPlayerDelete(uint64 p_GUID);

        /// Called when a update() of a player is done
        /// @p_Player : Player instance
        /// @p_Diff : diff time
        void OnPlayerUpdate(Player* p_Player, uint32 p_Diff);

        /// Called when a player is bound to an instance
        /// @p_Player     : Player instance
        /// @p_Difficulty : Instance Difficulty ID
        /// @p_MapID      : Instance Map ID
        /// @p_Permanent  : Is a permanent bind
        void OnPlayerBindToInstance(Player* p_Player, Difficulty p_Difficulty, uint32 p_MapID, bool p_Permanent);

        /// Called when a player switches to a new zone
        /// @p_Player    : Player instance
        /// @p_NewZoneID : New player zone ID
        /// @p_OldZoneID : Old player zone ID
        /// @p_NewAreaID : New player area ID
        void OnPlayerUpdateZone(Player* p_Player, uint32 p_NewZoneID, uint32 p_OldZoneID, uint32 p_NewAreaID);

        /// Called when a player updates his movement
        /// @p_Player : Player instance
        void OnPlayerUpdateMovement(Player* p_Player);
        /// Called when a spline step is done
        /// @p_Player   : Player instance
        /// @p_MoveType : Movement type
        /// @p_ID       : Movement ID
        void OnPlayerMovementInform(Player* p_Player, uint32 p_MoveType, uint32 p_ID);

        /// Called when player accepts some quest
        /// @p_Player : Player instance
        /// @p_Quest  : Accpeted quest
        void OnQuestAccept(Player * p_Player, const Quest * p_Quest);
        /// Called when player rewards some quest
        /// @p_Player : Player instance
        /// @p_Quest  : Rewarded quest
        void OnQuestReward(Player* p_Player, Quest const* p_Quest);
        /// Called when player rewards some quest
        /// @p_Player : Player instance
        /// @p_Quest  : Rewarded quest
        void OnQuestComplete(Player* p_Player, Quest const* p_Quest);
        /// Called when player abandons some quest
        /// @p_Player : Player instance
        /// @p_Quest  : Removed quest
        void OnQuestAbandon(Player* p_Player, Quest const* p_Quest);
        /// Called when player has quest removed from questlog (active or rewarded)
        /// @p_Player : Player instance
        /// @p_Quest  : Removed quest
        void OnQuestCleared(Player* p_Player, Quest const* p_Quest);
        /// Called when a player validates some quest objective
        /// @p_Player      : Player instance
        /// @p_QuestID     : Quest ID
        /// @p_ObjectiveID : Validated quest objective ID
        void OnObjectiveValidate(Player* p_Player, uint32 p_QuestID, uint32 p_ObjectiveID);

        /// Called when a player shapeshift
        /// @p_Player : Player instance
        /// @p_Form   : New shapeshift from
        void OnPlayerChangeShapeshift(Player* p_Player, ShapeshiftForm p_Form);

        /// Called when a player changes his faction
        /// @p_Player : Player instance
        void OnPlayerFactionChanged(Player* p_Player);

        /// Called when a player loot an item
        /// @p_Player : Player instance
        /// @p_Item   : New looted item instance
        void OnPlayerItemLooted(Player* p_Player, Item* p_Item);

        /// Called when a player enter in combat
        /// @p_Player : Player instance
        void OnPlayerEnterInCombat(Player* p_Player);

        /// Called when a player enters a mount
        /// @p_Player     : Player instance
        /// @p_CreatureID : Mount entry
        void OnPlayerMount(Player* p_Player, uint32 p_CreatureID);

        /// Called when a player leave combat status
        /// @p_Player : Player instance
        void OnPlayerLeaveCombat(Player* p_Player);

        /// Called when a player receive a scene triggered event
        /// @p_Player          : Player instance
        /// @p_SceneInstanceID : Standalone scene instance ID
        /// @p_Event           : Event string received from client
        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event);

        /// Called when a player cancels some scene
        /// @p_Player          : Player instance
        /// @p_SceneInstanceID : Standalone scene instance ID
        void OnSceneCancel(Player* p_Player, uint32 p_SceneInstanceId);

        /// Called when a player enter in bg
        /// @p_Player   : Player instance
        /// @p_MapID    : Map ID
        void OnEnterBG(Player* p_Player, uint32 p_MapID);

        /// Called when a leave a bg
        /// @p_Player   : Player instance
        /// @p_MapID    : Map ID
        void OnLeaveBG(Player* p_Player, uint32 p_MapID);

        /// Called when a player finish a movement like a jump
        /// @p_Player   : Player instance
        /// @p_SpellID  : Spell ID
        /// @p_TargetGUID : Target GUID
        void OnFinishMovement(Player* p_Player, uint32 p_SpellID, uint64 const p_TargetGUID);

        /// Called when a player regen a power
        /// @p_Player         : Player instance
        /// @p_Power          : Power to be regenerate
        /// @p_AddValue       : amount of power to regenerate
        /// @p_PreventDefault : avoid default regeneration
        void OnPlayerRegenPower(Player* p_Player, Powers const p_Power, float& p_AddValue, bool& p_PreventDefault);

        /// Called when a player take damage
        /// @p_Player          : Player instance
        /// @p_DamageEffectTyp : Damage type
        /// @p_Damage          : Amount of damage taken
        void OnPlayerTakeDamage(Player* p_Player, DamageEffectType p_DamageEffectType, uint32 p_Damage, SpellSchoolMask p_SchoolMask, CleanDamage const* p_CleanDamage);

        /// Called when player block attack
        /// @p_Player : Player instance
        /// @p_Attacker  : Damage Infos
        void OnPlayerBlock(Player* p_Player, Unit* p_Attacker);

    /// BattlegroundScript
    public:
        /// Should return a fully valid Battleground object for the type ID.
        /// @p_TypeID : Battleground Type ID
        Battleground* CreateBattleground(BattlegroundTypeId p_TypeID);

    /// OutdoorPvPScript
    public:
        /// Should return a fully valid OutdoorPvP object for the type ID.
        /// @p_Data : Outdoor PvP Data
        OutdoorPvP* CreateOutdoorPvP(OutdoorPvPData const* p_Data);

    /// SpellScriptLoader
    public:
        /// Should return a fully valid list of SpellScript pointer.
        /// @p_SpellID      : Spell ID
        /// @p_ScriptVector : Scripts for spell ID
        void CreateSpellScripts(uint32 p_SpellID, std::list<SpellScript*>& p_ScriptVector);
        /// Should return a fully valid list of AuraScript pointer.
        /// @p_SpellID      : Spell ID
        /// @p_ScriptVector : Scripts for spell ID
        void CreateAuraScripts(uint32 p_SpellID, std::list<AuraScript*>& p_ScriptVector);
        /// Create new spell script loaders
        /// @p_SpellID      : Spell ID
        /// @p_ScriptVector :
        void CreateSpellScriptLoaders(uint32 p_SpellID, std::vector<std::pair<SpellScriptLoader*, std::multimap<uint32, uint32>::iterator>>& p_ScriptVector);

    /// TransportScript
    public:
        /// Called when a player boards the transport.
        /// @p_Transport : Transport instance
        /// @p_Player    : Added player passenger instance
        void OnAddPassenger(Transport* p_Transport, Player* p_Player);
        /// Called when a creature boards the transport.
        /// @p_Transport : Transport instance
        /// @p_Creature  : Added creature passenger instance
        void OnAddCreaturePassenger(Transport* p_Transport, Creature* p_Creature);
        /// Called when a player exits the transport.
        /// @p_Transport : Transport instance
        /// @p_Player    : Added player passenger instance
        void OnRemovePassenger(Transport* p_Transport, Player* p_Player);

        /// Called when a transport moves.
        /// @p_Transport  : Transport instance
        /// @p_WaypointID : Reached waypoint ID
        /// @p_MapID      : Reached MapID
        /// @p_X          : New position X offset
        /// @p_Y          : New position Y offset
        /// @p_Z          : New position Z offset
        void OnRelocate(Transport* transport, uint32 waypointId, uint32 mapId, float x, float y, float z);

        /// Called on every Transport update tick.
        /// @p_Transport : Context Transport
        /// @p_Diff      : Time since last update
        void OnTransportUpdate(Transport* p_Transport, uint32 p_Diff);

    /// VehicleScript
    public:
        /// Called after a vehicle is installed.
        /// @p_Vehicle : Vehicle instance
        void OnInstall(Vehicle* p_Vehicle);
        /// Called after a vehicle is uninstalled.
        /// @p_Vehicle : Vehicle instance
        void OnUninstall(Vehicle* p_Vehicle);

        /// Called when a vehicle resets.
        /// @p_Vehicle : Vehicle instance
        void OnReset(Vehicle* p_Vehicle);

        /// Called after an accessory is installed in a vehicle.
        /// @p_Vehicle   : Vehicle instance
        /// @p_Accessory : Accessory to install
        void OnInstallAccessory(Vehicle* p_Vehicle, Creature* p_Accessory);

        /// Called after a passenger is added to a vehicle.
        /// @p_Vehicle   : Vehicle instance
        /// @p_Passanger : Passenger to add
        /// @p_SeatID    : Passenger destination seat ID
        void OnAddPassenger(Vehicle* p_Vehicle, Unit* p_Passenger, int8 p_SeatID);
        /// Called after a passenger is removed from a vehicle.
        /// @p_Vehicle   : Vehicle instance
        /// @p_Passanger : Passenger to remove
        void OnRemovePassenger(Vehicle* p_Vehicle, Unit* p_Passenger);

    /// Player conditions
    public:
        /// Register a player condition script
        /// @p_ID     : Condition ID
        /// @p_Script : Script instance
        void RegisterPlayerConditionScript(uint32 p_ID, PlayerConditionScript* p_Script);
        /// Has player condition script
        /// @p_ID: Player condition ID
        bool HasPlayerConditionScript(uint32 p_ID);
        /// Eval a player condition script
        /// @p_ConditionID : Condition ID
        /// @p_Condition   : Condition
        /// @p_Player      : Player instance
        bool EvalPlayerConditionScript(uint32 p_ConditionID, PlayerConditionEntry const* p_Condition, Player* p_Player);

    /// Battle Pay product scripts
    public:
        void RegisterBattlePayProductScript(std::string p_ScriptName, BattlePayProductScript* p_Script);
        void OnBattlePayProductDelivery(WorldSession* p_Session, Battlepay::Product const& p_Product);
        bool BattlePayCanBuy(WorldSession* p_Session, Battlepay::Product const& p_Product, std::string& p_Reason);

    /// Encounter scripts
    public:
        void OnEncounterEnd(EncounterDatas const* p_EncounterDatas);

    private:
        /// Registered script count
        uint32 m_ScriptCount;
        /// Atomic op counter for active scripts amount
        std::atomic<long> m_ScheduledScripts;
        /// Player condition scripts
        MS::Utilities::MutextedMap<uint32, PlayerConditionScript*> m_PlayerConditionScripts;
        /// Battle Pay Product Script
        std::map<std::string, BattlePayProductScript*> m_BattlePayProductScripts;

};

#endif
