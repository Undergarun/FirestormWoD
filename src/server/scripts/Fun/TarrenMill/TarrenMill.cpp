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

    LoadKillsRewards();

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

void OutdoorPvPTarrenMillFun::LoadKillsRewards()
{
    QueryResult l_Result = WorldDatabase.PQuery("SELECT `kills`, `title_a`, `title_h`, `item_a`, `item_h`, `aura_a`, `aura_h` FROM fun_tarrenmill_reward");
    if (!l_Result)
        return;

    do 
    {
        Field* l_Fields = l_Result->Fetch();

        KillRewardInfo l_KillReward;
        l_KillReward.Kills    = l_Fields[0].GetUInt32();
        l_KillReward.Title[0] = l_Fields[1].GetUInt32();
        l_KillReward.Title[1] = l_Fields[2].GetUInt32();
        l_KillReward.Item[0]  = l_Fields[3].GetUInt32();
        l_KillReward.Item[1]  = l_Fields[4].GetUInt32();
        l_KillReward.Aura[0]  = l_Fields[5].GetUInt32();
        l_KillReward.Aura[1]  = l_Fields[6].GetUInt32();

        m_KillRewards.push_back(l_KillReward);
    } 
    while (l_Result->NextRow());

    /// Sort rewards per kills
    m_KillRewards.sort([](KillRewardInfo const& p_Reward1, KillRewardInfo const& p_Reward2) -> bool
    {
        if (p_Reward1.Kills < p_Reward2.Kills)
            return true;

        return false;
    });
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

void OutdoorPvPTarrenMillFun::HandleKill(Player* p_Killer, Unit* p_Killed)
{
    if (p_Killed->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    if (p_Killed->ToPlayer()->GetTeamId() == p_Killer->GetTeamId())
        return;

    uint32 l_Kills = p_Killer->GetCharacterWorldStateValue(eCharacterWorldStates::TarrenMillFunKill);
    l_Kills++;

    p_Killer->SetCharacterWorldState(eCharacterWorldStates::TarrenMillFunKill, l_Kills);
    UpdateRankAura(p_Killer);
}

void OutdoorPvPTarrenMillFun::CheckKillRewardConditions(Player* p_Player)
{
    uint32 l_Kills = p_Player->GetCharacterWorldStateValue(eCharacterWorldStates::TarrenMillFunKill);

    for (auto l_Iterator : m_KillRewards)
    {
        if (l_Iterator.Kills == l_Kills)
        {
            TeamId l_Team = p_Player->GetTeamId();
            if (l_Team == TeamId::TEAM_NEUTRAL)
                break;

            CharTitlesEntry const* l_Title = sCharTitlesStore.LookupEntry(l_Iterator.Title[l_Team]);
            if (l_Title)
                p_Player->SetTitle(l_Title);

            ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(l_Iterator.Item[l_Team]);
            if (l_ItemTemplate)
            {
                uint32 l_NoSpace = 0;
                p_Player->AddItem(l_ItemTemplate->ItemId, 1, &l_NoSpace);

                if (l_NoSpace)
                {
                    Item* l_Item = Item::CreateItem(l_ItemTemplate->ItemId, 1, p_Player);
                    MailDraft l_Draft("Tarren Mill Reward", "");

                    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
                    if (l_Item)
                    {
                        l_Item->SaveToDB(l_Transaction);
                        l_Draft.AddItem(l_Item);
                    }

                    l_Draft.SendMailTo(l_Transaction, p_Player, MailSender(MAIL_CREATURE, g_TarrenMillFunRewardSender[l_Team]));
                    CharacterDatabase.CommitTransaction(l_Transaction);
                }
            }

            break;
        }
    }
}

RankInfo OutdoorPvPTarrenMillFun::GetRankAuraAndMissingKills(Player* p_Player)
{
    RankInfo l_RankInfo;
    l_RankInfo.first  = 0;  ///< Rank aura
    l_RankInfo.second = 0;  ///< Missing Kill

    TeamId l_TeamId = p_Player->GetTeamId();

    if (l_TeamId == TeamId::TEAM_NEUTRAL)
        return l_RankInfo;

    uint32 l_Kills   = p_Player->GetCharacterWorldStateValue(eCharacterWorldStates::TarrenMillFunKill);
    l_RankInfo.first = g_TarrenMillFirstRankAura[l_TeamId];

    /// Assume m_KillRewards is ordered by lower to higher (see OutdoorPvPTarrenMillFun::LoadTitleRewards)
    for (auto l_Iterator : m_KillRewards)
    {
        if (l_Iterator.Kills > l_Kills)
        {
            l_RankInfo.second = l_Iterator.Kills - l_Kills;
            break;
        }

        l_RankInfo.first  = l_Iterator.Aura[l_TeamId];
        l_Kills -= l_Iterator.Kills;
    }

    return l_RankInfo;
}

void OutdoorPvPTarrenMillFun::UpdateRankAura(Player* p_Player)
{
    RankInfo l_RankInfo = GetRankAuraAndMissingKills(p_Player);

    if (!p_Player->HasAura(l_RankInfo.first))
        p_Player->CastSpell(p_Player, l_RankInfo.first, true);

    if (AuraEffectPtr l_RankAura = p_Player->GetAuraEffect(l_RankInfo.first, 0))
        l_RankAura->ChangeAmount(l_RankInfo.second);
}

void OutdoorPvPTarrenMillFun::HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID)
{
    UpdateRankAura(p_Player);
}

void OutdoorPvPTarrenMillFun::HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID)
{
    RankInfo l_RankInfo = GetRankAuraAndMissingKills(p_Player);
    p_Player->RemoveAurasDueToSpell(l_RankInfo.first);
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