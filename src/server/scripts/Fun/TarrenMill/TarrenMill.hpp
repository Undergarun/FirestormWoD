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
    ZoneId        = 7107,
    MaxScoreValue = 5000,
    ResetHour     = 2       ///< 2:00 AM
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

        /// Custom
        void RegisterScoresResetTime();
        void ResetScores();
        void LoadKillsRewards();
        void CheckKillRewardConditions(Player* p_Player);
        RankInfo GetRankAuraAndMissingKills(Player* p_Player);
        void UpdateRankAura(Player* p_Player);

    private:

        time_t m_ResetScoreTimestamp;
        TitleRewards m_KillRewards;
};

#endif