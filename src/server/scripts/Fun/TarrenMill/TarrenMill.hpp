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

enum eTarrenMillFunDatas
{
    ZoneId        = 7107,
    MaxScoreValue = 5000,
    ResetHour     = 2       ///< 2:00 AM
};

struct TitleRewardInfo
{
    uint32 Kills;
    uint32 AllianceTitle;
    uint32 HordeTitle;
};

using TitleRewards = std::list<TitleRewardInfo>;

class OutdoorPvPTarrenMillFun : public OutdoorPvP
{
    public:

        OutdoorPvPTarrenMillFun();

        /// Overrides
        bool SetupOutdoorPvP() override;
        void FillInitialWorldStates(ByteBuffer& p_Data) override;
        void HandlePlayerKilled(Player* p_Player) override;
        bool Update(uint32 p_Diff) override;

        /// Custom
        void RegisterScoresResetTime();
        void ResetScores();
        void LoadTitleRewards();

    private:

        time_t m_ResetScoreTimestamp;
        TitleRewards m_TitleRewards;
};

#endif