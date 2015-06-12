////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "OutdoorPvP.h"
#include "OutdoorPvPMgr.h"

#ifndef TARREN_MILL_FUN_HPP
#define TARREN_MILL_FUN_HPP

enum eWorldStates
{
    /// Retail world states
    AllianceScore = 9384,
    HordeScore    = 9385,
    MaxScore      = 9386,

    /// Custom world states (server-side only)
    ResetScoresTimestamp = 40000
};

enum eCharacterWorldStates
{
    TarrenMillFunKill = 1,
};

enum eTarrenMillFunDatas
{
    ZoneId             = 7107,
    MapId              = 1280,
    MaxScoreValue      = 1500,
    ResetHour          = 2, ///< 2:00 AM
};

enum eCreatures
{
    AllianceSpiritGuide = 80723,
    HordeSpiritGuide    = 80724,
};

enum eGraveyards
{
    AllianceCity,
    HordeCity,
    TotalGraveyards
};

enum eWorldSafeLocs
{
    TarrenMillAllianceGraveyard = 4850,
    TarrenMillHordeGraveyard    = 4851
};

enum eGameObjects
{
    TarrenMillShipPortal = 696901
};

enum eTarrenMillEventDurations
{
    EventFFADuration = 5 * MINUTE * IN_MILLISECONDS,
    EventPortalShipDuration = 30 * MINUTE * IN_MILLISECONDS
};

enum eTarrenMillEvents
{
    EventPortalShip,
    EventFFA,
    MaxEvents
};

enum eTarrenMillEventStates
{
    NotStarted,
    Started
};

class TarrenMillEvent
{
    public:

        /// Structures
        struct EventTime
        {
            EventTime(uint8 p_Hour, uint8 p_Minute) : Hour(p_Hour), Minute(p_Minute) {}

            uint8 Hour;
            uint8 Minute;
        };

        /// Types definitions
        using TimeRange = std::pair<EventTime, EventTime>;
        using TimeRanges = std::vector<TimeRange>;

        /// Variables
        uint32 ID;
        uint8  State;
        uint32 Duration;
        TimeRanges StartRanges;
        std::vector<uint64> Data;
        uint32 NextStartTimestamp;

        /// Virtual methods
        virtual void OnStart();
        virtual void OnEnd();
        virtual void OnUpdate(const uint32 p_Diff);
        virtual void OnPlayerEnter(Player* p_Player) {};
        virtual void OnPlayerExit(Player* p_Player) {};
        virtual void OnPlayerKilled(Player* p_Player) {}
        virtual void OnFillInitialWorldStates(ByteBuffer& p_Data) {}

        /// Methods
        TarrenMillEvent(uint32 p_ID, uint8 p_State, uint32 p_Duration, TimeRanges p_StartRange);
        TarrenMillEvent() {}

        void Start();
        OutdoorPvP* GetTarrenZoneScript();
        void ComputeNextStartTime();
        void ApplyOnAllPlayers(std::function<void(Player*)> p_Lambda);
        bool IsInProgress() const;

    private:
        OutdoorPvP* m_TarrenZoneScript;
};

#define MakeEventTimeRange(hourStart, minuteStart, hourEnd, minuteEnd) std::make_pair(TarrenMillEvent::EventTime(hourStart, minuteStart), TarrenMillEvent::EventTime(hourEnd, minuteEnd))

struct TarrenMillGraveyard
{
    uint32 ID;
    TeamId StartTeam;
};

struct KillRewardInfo
{
    uint32 Kills;
    uint32 Title[MS::Battlegrounds::TeamsCount::Value];
    uint32 Item[MS::Battlegrounds::TeamsCount::Value];
    uint32 Aura[MS::Battlegrounds::TeamsCount::Value];
};

const static uint32 g_TarrenMillFunRewardSender[MS::Battlegrounds::TeamsCount::Value] =
{
    4949,
    29611,
};

const static uint32 g_TarrenMillFirstRankAura[MS::Battlegrounds::TeamsCount::Value] =
{
    168412, ///< Recruit (Alliance)
    168429, ///< Recruit (Horde)
};

TarrenMillGraveyard const g_TarenMillGraveyards[eGraveyards::TotalGraveyards] =
{
    { eWorldSafeLocs::TarrenMillAllianceGraveyard, TeamId::TEAM_ALLIANCE },  ///< 0 - Alliance base
    { eWorldSafeLocs::TarrenMillHordeGraveyard,    TeamId::TEAM_HORDE    },  ///< 1 - Horde base
};

using TitleRewards = std::list<KillRewardInfo>;
using RankInfo = std::pair<uint32, uint32>;

class OutdoorPvPTarrenMillFun : public OutdoorPvP
{
    public:

        OutdoorPvPTarrenMillFun();

        /// Overrides
        bool SetupOutdoorPvP() override;
        void FillInitialWorldStates(ByteBuffer& p_Data) override;
        void HandlePlayerKilled(Player* p_Player) override;
        void HandleKill(Player* p_Killer, Unit* p_Killed) override;
        bool Update(uint32 p_Diff) override;
        void HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID) override;
        void HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID) override;
        void OnCreatureCreate(Creature* p_Creature) override;


        /// Custom
        uint32 GetCurrentScore(TeamId p_TeamId);
        void ScheduleEventsUpdate(uint32 p_Diff);
        void StopShipEvent();
        void StartShipEvent(bool p_AllianceWon);
        void RegisterScoresResetTime();
        void InitializeGraveyards();
        void InitializeEvents();
        void ResetScores();
        void LoadKillsRewards();
        void CheckKillRewardConditions(Player* p_Player);
        RankInfo GetRankAuraAndMissingKills(Player* p_Player);
        void UpdateRankAura(Player* p_Player);
        void UpdateScoreAtKill(Player* p_Player);

    private:

        std::vector<TarrenMillEvent*> m_Events;
        time_t m_ResetScoreTimestamp;
        time_t m_ResetPortalShipEventTimestamp;
        TitleRewards m_KillRewards;
};

#endif