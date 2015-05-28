////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "OutdoorPvP.h"
#include "OutdoorPvPMgr.h"
#include "TarrenMill.hpp"

OutdoorPvPTarrenMillFun::OutdoorPvPTarrenMillFun()
{
}

bool OutdoorPvPTarrenMillFun::SetupOutdoorPvP()
{
    RegisterZone(eTarrenMillFunDatas::ZoneId);
    RegisterScoresResetTime();

    LoadTitleRewards();

    return true;
}

void OutdoorPvPTarrenMillFun::RegisterScoresResetTime()
{
    uint32 l_NextResetTimestamp = sWorld->getWorldState(eWorldStates::ResetScoresTimestamp);
    if (l_NextResetTimestamp == 0)
    {
        /// Compute next 2:00 am timestamp
        time_t l_CurTime = time(nullptr);
        tm l_LocalTm = *localtime(&l_CurTime);
        l_LocalTm.tm_hour = eTarrenMillFunDatas::ResetHour;
        l_LocalTm.tm_min  = 0;
        l_LocalTm.tm_sec  = 0;

        l_NextResetTimestamp = mktime(&l_LocalTm);
        if (l_NextResetTimestamp < l_CurTime)
            l_NextResetTimestamp += DAY;

        sWorld->setWorldState(eWorldStates::ResetScoresTimestamp, l_NextResetTimestamp);
    }

    m_ResetScoreTimestamp = l_NextResetTimestamp;
}

void OutdoorPvPTarrenMillFun::ResetScores()
{
    /// Update next reset time
    m_ResetScoreTimestamp = sWorld->getWorldState(eWorldStates::ResetScoresTimestamp) + DAY;
    sWorld->setWorldState(eWorldStates::ResetScoresTimestamp, m_ResetScoreTimestamp);

    /// Reset scores
    sWorld->setWorldState(eWorldStates::AllianceScore, 0);
    sWorld->setWorldState(eWorldStates::HordeScore, 0);

    /// Send update to online players
    SendUpdateWorldState(eWorldStates::AllianceScore, 0);
    SendUpdateWorldState(eWorldStates::HordeScore, 0);
}

void OutdoorPvPTarrenMillFun::LoadTitleRewards()
{
    QueryResult l_Result = WorldDatabase.PQuery("SELECT `kills`, `title_h`, `title_a` FROM fun_tarrenmill_titles");
    if (!l_Result)
        return;

    do 
    {
        Field* l_Fields = l_Result->Fetch();

        TitleRewardInfo l_TitleReward;
        l_TitleReward.Kills         = l_Fields[0].GetUInt32();
        l_TitleReward.HordeTitle    = l_Fields[1].GetInt32();
        l_TitleReward.AllianceTitle = l_Fields[2].GetUInt32();

        m_TitleRewards.push_back(l_TitleReward);
    } 
    while (l_Result->NextRow());
}

bool OutdoorPvPTarrenMillFun::Update(uint32 p_Diff)
{
    if (time(nullptr) > m_ResetScoreTimestamp)
        ResetScores();

    return true;
}

void OutdoorPvPTarrenMillFun::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << (uint32)eWorldStates::AllianceScore << (uint32)sWorld->getWorldState(eWorldStates::AllianceScore);
    p_Data << (uint32)eWorldStates::HordeScore    << (uint32)sWorld->getWorldState(eWorldStates::HordeScore);
    p_Data << (uint32)eWorldStates::MaxScore      << (uint32)eTarrenMillFunDatas::MaxScoreValue;
}

void OutdoorPvPTarrenMillFun::HandlePlayerKilled(Player* p_Player)
{
    uint32 l_WorldState = 0;

    switch (p_Player->GetTeamId())
    {
        case TeamId::TEAM_ALLIANCE:
            l_WorldState = eWorldStates::AllianceScore;
            break;
        case TeamId::TEAM_HORDE:
            l_WorldState = eWorldStates::HordeScore;
            break;
    }

    /// Neutral case
    if (l_WorldState == 0)
        return;

    uint32 l_Value = sWorld->getWorldState(l_WorldState);
    l_Value++;

    /// Max score reached !
    if (l_Value == eTarrenMillFunDatas::MaxScoreValue)
        l_Value = 0;

    sWorld->setWorldState(l_WorldState, l_Value);
    SendUpdateWorldState(l_WorldState, l_Value);
}

class OutdoorPvP_TarrenMillFun : public OutdoorPvPScript
{
    public:

        OutdoorPvP_TarrenMillFun() : OutdoorPvPScript("OutdoorPvP_TarrenMillFun") { }

        OutdoorPvP* GetOutdoorPvP() const
        {
            return new OutdoorPvPTarrenMillFun();
        }
};

void AddSC_TarrenMillFun()
{
    new OutdoorPvP_TarrenMillFun();
} 