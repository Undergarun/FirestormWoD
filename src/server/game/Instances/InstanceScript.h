////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_INSTANCE_DATA_H
#define TRINITY_INSTANCE_DATA_H

#include "ZoneScript.h"
#include "World.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "AchievementMgr.h"
#include "Reporter.hpp"
#include <EasyJSon.hpp>
//#include "GameObject.h"
//#include "Map.h"

#define OUT_SAVE_INST_DATA             sLog->outDebug(LOG_FILTER_TSCR, "Saving Instance Data for Instance %s (Map %d, Instance Id %d)", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_SAVE_INST_DATA_COMPLETE    sLog->outDebug(LOG_FILTER_TSCR, "Saving Instance Data for Instance %s (Map %d, Instance Id %d) completed.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA(a)          sLog->outDebug(LOG_FILTER_TSCR, "Loading Instance Data for Instance %s (Map %d, Instance Id %d). Input is '%s'", instance->GetMapName(), instance->GetId(), instance->GetInstanceId(), a)
#define OUT_LOAD_INST_DATA_COMPLETE    sLog->outDebug(LOG_FILTER_TSCR, "Instance Data Load for Instance %s (Map %d, Instance Id: %d) is complete.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA_FAIL        sLog->outError(LOG_FILTER_TSCR, "Unable to load Instance Data for Instance %s (Map %d, Instance Id: %d).", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())

class Map;
class Unit;
class Player;
class GameObject;
class Creature;

typedef std::set<GameObject*> DoorSet;
typedef std::set<Creature*> MinionSet;

enum EncounterFrameType
{
    ENCOUNTER_FRAME_ENGAGE,
    ENCOUNTER_FRAME_DISENGAGE,
    ENCOUNTER_FRAME_UPDATE_PRIORITY,
    ENCOUNTER_FRAME_START_TIMER,
    ENCOUNTER_FRAME_START_OBJECTIVE,
    ENCOUNTER_FRAME_COMPLETE_OBJECTIVE,
    ENCOUNTER_FRAME_START,
    ENCOUNTER_FRAME_UPDATE_OBJECTIVE,
    ENCOUNTER_FRAME_END,
    ENCOUNTER_FRAME_IN_COMBAT_RESURRECTION,
    ENCOUNTER_FRAME_PHASE_SHIFT_CHANGED,
    ENCOUNTER_FRAME_GAIN_COMBAT_RESURRECTION_CHARGE
};

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAIL          = 2,
    DONE          = 3,
    SPECIAL       = 4,
    TO_BE_DECIDED = 5
};

enum DoorType
{
    DOOR_TYPE_ROOM          = 0,    // Door can open if encounter is not in progress
    DOOR_TYPE_PASSAGE       = 1,    // Door can open if encounter is done
    DOOR_TYPE_SPAWN_HOLE    = 2,    // Door can open if encounter is in progress, typically used for spawning places
    MAX_DOOR_TYPES
};

enum BoundaryType
{
    BOUNDARY_NONE = 0,
    BOUNDARY_N,
    BOUNDARY_S,
    BOUNDARY_E,
    BOUNDARY_W,
    BOUNDARY_NE,
    BOUNDARY_NW,
    BOUNDARY_SE,
    BOUNDARY_SW,
    BOUNDARY_MAX_X = BOUNDARY_N,
    BOUNDARY_MIN_X = BOUNDARY_S,
    BOUNDARY_MAX_Y = BOUNDARY_W,
    BOUNDARY_MIN_Y = BOUNDARY_E
};

typedef std::map<BoundaryType, float> BossBoundaryMap;

struct DoorData
{
    uint32 entry, bossId;
    DoorType type;
    uint32 boundary;
};

struct MinionData
{
    uint32 entry, bossId;
};

struct BossInfo
{
    BossInfo() : state(TO_BE_DECIDED) {}
    EncounterState state;
    DoorSet door[MAX_DOOR_TYPES];
    MinionSet minion;
    BossBoundaryMap boundary;
};

struct DoorInfo
{
    explicit DoorInfo(BossInfo* _bossInfo, DoorType _type, BoundaryType _boundary)
        : bossInfo(_bossInfo), type(_type), boundary(_boundary) {}
    BossInfo* bossInfo;
    DoorType type;
    BoundaryType boundary;
};

struct MinionInfo
{
    explicit MinionInfo(BossInfo* _bossInfo) : bossInfo(_bossInfo) {}
    BossInfo* bossInfo;
};

typedef std::multimap<uint32 /*entry*/, DoorInfo> DoorInfoMap;
typedef std::map<uint32 /*entry*/, MinionInfo> MinionInfoMap;

struct BossScenarios
{
    BossScenarios()
    {
        m_BossID = 0;
        m_ScenarioID = 0;
    }

    BossScenarios(uint32 p_ID, uint32 p_Scenario)
    {
        m_BossID = p_ID;
        m_ScenarioID = p_Scenario;
    }

    uint32 m_BossID;
    uint32 m_ScenarioID;
};

struct RosterData
{
    RosterData()
    {
        GuidLow     = 0;
        Level       = 0;
        Class       = 0;
        SpecID      = 0;
        Role        = 0;
        ItemLevel   = 0;
    }

    uint32      GuidLow;
    std::string Name;
    uint8       Level;
    uint8       Class;
    uint32      SpecID;
    uint32      Role;
    uint32      ItemLevel;
};

typedef std::vector<RosterData> RosterDatas;

struct EncounterDatas
{
    EncounterDatas()
    {
        Expansion       = 0;
        RealmID         = 0;
        GuildID         = 0;
        GuildFaction    = 0;
        MapID           = 0;
        EncounterID     = 0;
        DifficultyID    = 0;
        StartTime       = 0;
        CombatDuration  = 0;
        EndTime         = 0;
        Success         = 0;
        DeadCount       = 0;
    }

    uint32      Expansion;
    uint32      RealmID;
    uint32      GuildID;
    uint32      GuildFaction;
    std::string GuildName;
    uint32      MapID;
    uint32      EncounterID;
    uint32      DifficultyID;
    uint32      StartTime;
    uint32      CombatDuration;
    uint32      EndTime;
    bool        Success;
    uint32      DeadCount;
    RosterDatas RosterDatas;
    std::string EncounterHealth;
};

enum eChallengeMedals
{
    MedalTypeNone,
    MedalTypeBronze,
    MedalTypeSilver,
    MedalTypeGold,
    MaxMedalType
};

#define CHALLENGE_MOD_ORB 211674

enum eInstanceSpells
{
    SpellDetermination          = 139068,
    /// Heroism, Bloodlust...
    ShamanSated                 = 57724,
    HunterInsanity              = 95809,
    MageTemporalDisplacement    = 80354,
    ShamanExhaustion            = 57723,
    Bloodlust                   = 2825,
    Heroism                     = 32182,
    TempralDisplacement         = 80354,
    AncientHysteria             = 90355,
    MaxBloodlustSpells          = 4,
    /// Battle resurrection spells
    Rebirth                     = 20484,
    Soulstone                   = 20707,
    RaiseAlly                   = 61999,
    EternalGuardian             = 126393,
    GiftOfChiJi                 = 159931,
    DustOfLife                  = 159956,
    MaxBattleResSpells          = 6
};

uint32 const g_BattleResSpells[eInstanceSpells::MaxBattleResSpells] =
{
    eInstanceSpells::Rebirth,
    eInstanceSpells::Soulstone,
    eInstanceSpells::RaiseAlly,
    eInstanceSpells::EternalGuardian,
    eInstanceSpells::GiftOfChiJi,
    eInstanceSpells::DustOfLife
};

uint32 const g_BloodlustSpells[eInstanceSpells::MaxBloodlustSpells] =
{
    eInstanceSpells::Bloodlust,
    eInstanceSpells::Heroism,
    eInstanceSpells::TempralDisplacement,
    eInstanceSpells::AncientHysteria
};

class InstanceScript : public ZoneScript
{
    public:
        explicit InstanceScript(Map* p_Map);

        virtual ~InstanceScript() {}

        Map* instance;

        // On creation, NOT load.
        virtual void Initialize() {}

        // On load
        virtual void Load(char const* data) { LoadBossState(data); }

        // When save is needed, this function generates the data
        virtual std::string GetSaveData() { return GetBossSaveData(); }

        void SaveToDB();

        virtual void Update(uint32 p_Diff) { UpdateOperations(p_Diff); }
        void UpdateOperations(uint32 const p_Diff);

        // Used by the map's CanEnter function.
        // This is to prevent players from entering during boss encounters.
        virtual bool IsEncounterInProgress() const;

        // Called when a player begins to enter the instance.
        virtual void BeforePlayerEnter(Player* /*player*/) {}

        // Called when a player successfully enters the instance.
        virtual void OnPlayerEnter(Player* p_Player);

        /// Called when a player successfully exits the instance
        virtual void OnPlayerExit(Player* p_Player);

        // Handle open / close objects
        // use HandleGameObject(0, boolen, GO); in OnObjectCreate in instance scripts
        // use HandleGameObject(GUID, boolen, NULL); in any other script
        void HandleGameObject(uint64 guid, bool open, GameObject* go = NULL);

        // change active state of doors or buttons
        void DoUseDoorOrButton(uint64 guid, uint32 withRestoreTime = 0, bool useAlternativeState = false);

        // Respawns a GO having negative spawntimesecs in gameobject-table
        void DoRespawnGameObject(uint64 guid, uint32 timeToDespawn = MINUTE);

        // sends world state update to all players in instance
        void DoUpdateWorldState(uint32 worldstateId, uint32 worldstateValue);

        // Send Notify to all players in instance
        void DoSendNotifyToInstance(char const* format, ...);

        // Reset Achievement Criteria
        void DoResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 = 0, uint64 miscValue2 = 0, bool evenIfCriteriaComplete = false);

        // Complete Achievement for all players in instance
        void DoCompleteAchievement(uint32 achievement);

        // Update Achievement Criteria for all players in instance
        void DoUpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 = 0, uint32 miscValue2 = 0, uint32 miscValue3 = 0, Unit* unit = NULL);

        // Start/Stop Timed Achievement Criteria for all players in instance
        void DoStartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);
        void DoStopTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);

        /// Remove movement forces on all players for the specified source
        void DoRemoveForcedMovementsOnPlayers(uint64 p_Source);

        // Remove Auras due to Spell on all players in instance
        void DoRemoveAurasDueToSpellOnPlayers(uint32 spell);

        // Cast spell on all players in instance
        void DoCastSpellOnPlayers(uint32 spell);

        void DoSetAlternatePowerOnPlayers(int32 value);

        void DoModifyPlayerCurrencies(uint32 id, int32 value);

        void DoNearTeleportPlayers(const Position pos, bool casting = false);

        void DoKilledMonsterKredit(uint32 questId, uint32 entry, uint64 guid = 0);

        // Add aura on all players in instance
        void DoAddAuraOnPlayers(uint32 spell);

        /// Remove cooldown for spell on all players in instance
        void DoRemoveSpellCooldownOnPlayers(uint32 p_SpellID);

        /// Remove cooldowns equal or less than specified time to all players in instance
        void DoRemoveSpellCooldownWithTimeOnPlayers(uint32 p_MinRecoveryTime);

        /// Used to apply or unapply item set bonuses on all players in instance
        void HandleItemSetBonusesOnPlayers(bool p_Apply);

        /// Used to apply or unapply gem bonuses on all player in instance
        void HandleGemBonusesOnPlayers(bool p_Apply);

        /// Do combat stop on all players in instance
        void DoCombatStopOnPlayers();

        /// Set progress on a criteria for all players
        void SetCriteriaProgressOnPlayers(CriteriaEntry const* p_Criteria, uint64 p_ChangeValue, ProgressType p_Type);

        /// Teleport players to instance entrance
        void RepopPlayersAtGraveyard(bool p_ForceGraveyard = false);

        /// Respawn specified creature, or all creatures
        void RespawnCreature(uint64 p_Guid = 0);

        // Return wether server allow two side groups or not
        bool ServerAllowsTwoSideGroups() { return sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP); }

        virtual bool SetBossState(uint32 id, EncounterState state);
        EncounterState GetBossState(uint32 id) const { return id < m_Bosses.size() ? m_Bosses[id].state : TO_BE_DECIDED; }
        BossBoundaryMap const* GetBossBoundary(uint32 id) const { return id < m_Bosses.size() ? &m_Bosses[id].boundary : NULL; }

        // Achievement criteria additional requirements check
        // NOTE: not use this if same can be checked existed requirement types from AchievementCriteriaRequirementType
        virtual bool CheckAchievementCriteriaMeet(uint32 /*criteria_id*/, Player const* /*source*/, Unit const* /*target*/ = NULL, uint64 /*miscvalue1*/ = 0);

        // Checks boss requirements (one boss required to kill other)
        virtual bool CheckRequiredBosses(uint32 /*bossId*/, Player const* /*player*/ = NULL) const;

        // Checks encounter state at kill/spellcast
        void UpdateEncounterState(EncounterCreditType type, uint32 creditEntry, Unit* source);

        void SendEncounterStart(uint32 p_EncounterID);
        void SendEncounterEnd(uint32 p_EncounterID, bool p_Success);
        uint32 GetEncounterIDForBoss(Creature* p_Boss) const;

        void SaveEncounterLogs(Creature* p_Creature, uint32 p_EncounterID);

        // Used only during loading
        void SetCompletedEncountersMask(uint32 newMask) { m_CompletedEncounters = newMask; }

        // Returns completed encounters mask for packets
        uint32 GetCompletedEncounterMask() const { return m_CompletedEncounters; }

        virtual void OnGameObjectRemove(GameObject* p_Go);

        /// Called when falling damage are calculated for player
        virtual bool IsPlayerImmuneToFallDamage(Player* /*p_Player*/) const { return false; }

        /// Add timed delayed operation
        /// @p_Timeout  : Delay time
        /// @p_Function : Callback function
        void AddTimedDelayedOperation(uint32 p_Timeout, std::function<void()> && p_Function)
        {
            m_EmptyWarned = false;
            m_TimedDelayedOperations.push_back(std::pair<uint32, std::function<void()>>(p_Timeout, p_Function));
        }

        /// Called after last delayed operation was deleted
        /// Do whatever you want
        virtual void LastOperationCalled() { }

        std::vector<std::pair<int32, std::function<void()>>>    m_TimedDelayedOperations;   ///< Delayed operations
        bool                                                    m_EmptyWarned;              ///< Warning when there are no more delayed operations

        struct CriteriaProgressData
        {
            CriteriaProgressData(uint32 p_ID, uint64 p_Quantity, uint64 p_Guid, uint32 p_Date, uint32 p_StartTime, uint8 p_Flags)
            {
                m_ID                = p_ID;
                m_Quantity          = p_Quantity;
                m_Guid              = p_Guid;
                m_Date              = p_Date;
                m_TimeFromStart     = p_StartTime;
                m_TimeFromCreate    = p_StartTime;
                m_Flags             = p_Flags;
            }

            CriteriaProgressData()
            {
                m_ID                = 0;
                m_Quantity          = 0;
                m_Guid              = 0;
                m_Date              = 0;
                m_TimeFromStart     = 0;
                m_TimeFromCreate    = 0;
                m_Flags             = 0;
            }

            uint32 m_ID;
            uint64 m_Quantity;
            uint64 m_Guid;
            uint32 m_Date;
            uint32 m_TimeFromStart;
            uint32 m_TimeFromCreate;
            uint8  m_Flags;
        };

        struct BonusObjectiveData
        {
            BonusObjectiveData(uint32 p_ID, bool p_Complete)
            {
                m_ObjectiveID       = p_ID;
                m_ObjectiveComplete = p_Complete;
            }

            BonusObjectiveData()
            {
                m_ObjectiveID       = 0;
                m_ObjectiveComplete = false;
            }

            uint32 m_ObjectiveID;
            bool m_ObjectiveComplete;
        };

        struct ScenarioData
        {
            ScenarioData(uint32 p_ScenarioID, uint32 p_StepID, uint32 p_CurrWave, uint32 p_MaxWave, uint32 p_Timer, uint32 p_CriteriaCount,
                uint32 p_BonusCount, bool p_Complete)
            {
                m_ScenarioID        = p_ScenarioID;
                m_StepID            = p_StepID;
                m_WaveCurrent       = p_CurrWave;
                m_WaveMax           = p_MaxWave;
                m_TimerDuration     = p_Timer;
                m_CriteriaCount     = p_CriteriaCount;
                m_BonusCount        = p_BonusCount;
                m_ScenarioComplete  = p_Complete;

                m_CriteriaProgress.resize(m_CriteriaCount);
                m_BonusObjectives.resize(m_BonusCount);
            }

            ScenarioData(uint32 p_ScenarioID, uint32 p_StepID)
            {
                m_ScenarioID        = p_ScenarioID;
                m_StepID            = p_StepID;
                m_WaveCurrent       = 0;
                m_WaveMax           = 0;
                m_TimerDuration     = 0;
                m_CriteriaCount     = 0;
                m_BonusCount        = 0;
                m_ScenarioComplete  = false;

                m_CriteriaProgress.clear();
                m_BonusObjectives.clear();
            }

            ScenarioData()
            {
                m_ScenarioID        = 0;
                m_StepID            = 0;
                m_WaveCurrent       = 0;
                m_WaveMax           = 0;
                m_TimerDuration     = 0;
                m_CriteriaCount     = 0;
                m_BonusCount        = 0;
                m_ScenarioComplete  = false;

                m_CriteriaProgress.clear();
                m_BonusObjectives.clear();
            }

            void AddBonusObjective(BonusObjectiveData p_Data)
            {
                m_BonusObjectives.push_back(p_Data);
            }

            void AddCriteriaProgress(CriteriaProgressData p_Data)
            {
                m_CriteriaProgress.push_back(p_Data);
            }

            uint32 m_ScenarioID;
            uint32 m_StepID;
            uint32 m_WaveCurrent;
            uint32 m_WaveMax;
            uint32 m_TimerDuration;
            uint32 m_CriteriaCount;
            uint32 m_BonusCount;
            bool m_ScenarioComplete;

            std::vector<CriteriaProgressData> m_CriteriaProgress;
            std::vector<BonusObjectiveData> m_BonusObjectives;
        };

        void SendEncounterUnit(uint32 type, Unit* unit = NULL, uint8 param1 = 0, uint8 param2 = 0);
        void SendScenarioState(ScenarioData p_Data, Player* p_Player = nullptr);
        void SendScenarioProgressUpdate(CriteriaProgressData p_Data, Player* p_Player = nullptr);
        void BuildCriteriaProgressPacket(WorldPacket* p_Data, CriteriaProgressData p_CriteriaProgress);
        void UpdateCriteriasAfterLoading();

        //////////////////////////////////////////////////////////////////////////
        /// ChallengesMode
        void SendChallengeModeStart();
        void SendChallengeStartTimer(uint32 p_Time);
        void SendChallengeStartElapsedTimer(uint32 p_TimerID, uint32 p_Time);
        void SendChallengeStopElapsedTimer(uint32 p_TimerID, bool p_KeepTimer = false);
        void SetChallengeModeStarted();
        bool IsChallengeModeStarted() const { return m_ChallengeStarted; }
        bool IsConditionCompleted() const { return m_ConditionCompleted; }
        void ScheduleChallengeStartup(uint32 p_Diff);
        void ScheduleChallengeTimeUpdate(uint32 p_Diff);
        void ScheduleBeginningTimeUpdate(uint32 p_Diff);
        void SendChallengeNewPlayerRecord();
        void SendChallengeModeComplete(uint32 p_Money);
        void SaveChallengeDatasIfNeeded();
        void SaveNewGroupChallenge(uint32 p_GuildID = 0);
        uint32 RewardChallengers();
        void RewardNewRealmRecord(RealmCompletedChallenge* p_OldChallenge = nullptr);
        void ResetChallengeMode();

        bool   m_ChallengeStarted;
        bool   m_ConditionCompleted;
        uint32 m_CreatureKilled;
        uint32 m_StartChallengeTime;
        uint64 m_ChallengeDoorGuid;
        uint64 m_ChallengeOrbGuid;
        uint32 m_ChallengeTime;
        uint8  m_MedalType;
        uint64 m_InstanceGuid;
        uint32 m_BeginningTime;
        uint32 m_ScenarioID;
        uint8  m_ScenarioStep;
        //////////////////////////////////////////////////////////////////////////

        // Called when a creature is killed by a player
        virtual void OnCreatureKilled(Creature* /*p_Creature*/, Player* /*p_Player*/) { }

        // Check if all players are dead (except gamemasters)
        virtual bool IsWipe();

        virtual void FillInitialWorldStates(ByteBuffer& /*data*/) {}

        void UpdatePhasing();
        void UpdateCreatureGroupSizeStats();

        void SetDisabledBosses(uint32 p_DisableMask) { m_DisabledMask = p_DisableMask; }

        BossInfo* GetBossInfo(uint32 p_ID)
        {
            if (p_ID < m_Bosses.size())
                return &m_Bosses[p_ID];

            return nullptr;
        }

        //////////////////////////////////////////////////////////////////////////
        /// Combat Resurrection - http://wow.gamepedia.com/Resurrect#Combat_resurrection
        void ResetCombatResurrection();
        void StartCombatResurrection();
        void UpdateCombatResurrection(uint32 const p_Diff);
        bool CanUseCombatResurrection() const;
        void ConsumeCombatResurrectionCharge();

        uint32 m_InCombatResCount;
        uint32 m_MaxInCombatResCount;
        uint32 m_CombatResChargeTime;
        uint32 m_NextCombatResChargeTime;
        //////////////////////////////////////////////////////////////////////////

    protected:
        void SetBossNumber(uint32 p_Number);
        void LoadDoorData(DoorData const* data);
        void LoadScenariosInfos(BossScenarios const* p_Scenarios, uint32 p_ScenarioID);
        void LoadMinionData(MinionData const* data);

        void AddDoor(GameObject* door, bool add);
        void AddMinion(Creature* minion, bool add);

        void UpdateDoorState(GameObject* door, bool p_WithDelay = false);
        void UpdateMinionState(Creature* minion, EncounterState state);

        std::string LoadBossState(char const* data);
        std::string GetBossSaveData();
    private:
        std::vector<BossInfo> m_Bosses;
        std::vector<BossScenarios> m_BossesScenarios;
        DoorInfoMap doors;
        MinionInfoMap minions;
        uint32 m_CompletedEncounters; // completed encounter mask, bit indexes are DungeonEncounter.dbc boss numbers, used for packets
        uint32 m_EncounterTime;
        uint32 m_DisabledMask;

        EncounterDatas m_EncounterDatas;
};
#endif
