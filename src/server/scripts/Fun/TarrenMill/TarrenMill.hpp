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
    MaxScoreValue      = 5000,
    ResetHour          = 2, ///< 2:00 AM
    PortalShipDuration = 60 ///< minutes
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

enum eTarrenMillEvents
{
    EventPortalShip,
    MaxEvents
};


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
        void ScheduleEventsUpdate(uint32 p_Diff);
        void ResetFinalEvent();
        void LaunchFinalEvent(bool p_AllianceWon, uint32 p_Diff);
        void RegisterScoresResetTime();
        void InitializeGraveyards();
        void InitializeEvents();
        void ResetScores();
        void LoadKillsRewards();
        void CheckKillRewardConditions(Player* p_Player);
        RankInfo GetRankAuraAndMissingKills(Player* p_Player);
        void UpdateRankAura(Player* p_Player);

    private:

        uint32 m_TarrenMillEvents[eTarrenMillEvents::MaxEvents];
        bool m_TarrenMillEventsActivated[eTarrenMillEvents::MaxEvents];
        time_t m_ResetScoreTimestamp;
        time_t m_ResetPortalShipEventTimestamp;
        TitleRewards m_KillRewards;
};

#endif