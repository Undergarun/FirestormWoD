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

class TarrenMillFFAEvent : public TarrenMillEvent
{
    public:
        TarrenMillFFAEvent(uint32 p_ID, uint8 p_State, uint32 p_Duration, TimeRanges p_StartRange)
            : TarrenMillEvent(p_ID, p_State, p_Duration, p_StartRange) {}

        void OnPlayerEnter(Player* p_Player) override
        {
            if (!IsInProgress())
                return;

            EnableFFAOnPlayer(p_Player);
        }

        void OnPlayerExit(Player* p_Player) override
        {
            if (!IsInProgress())
                return;

            DisableFFAOnPlayer(p_Player);
        }

        void OnStart() override
        {
            TarrenMillEvent::OnStart();

            ApplyOnAllPlayers([this](Player* p_Player)
            {
                EnableFFAOnPlayer(p_Player);
                ChatHandler(p_Player).SendSysMessage("Tarren Mill: FFA event started for the next 5 minutes!");
            });
        }

        void OnEnd() override
        {
            TarrenMillEvent::OnEnd();

            ApplyOnAllPlayers([this](Player* p_Player)
            {
                DisableFFAOnPlayer(p_Player);
                ChatHandler(p_Player).SendSysMessage("Tarren Mill: FFA event is now finished");
            });
        }

        void EnableFFAOnPlayer(Player* p_Player)
        {
            p_Player->ForceFFA();
            p_Player->UpdatePvPState(true);
        }

        void DisableFFAOnPlayer(Player* p_Player)
        {
            p_Player->DisableForceFFA();
            p_Player->UpdatePvPState(false);
        }
};

class TarrenMillShipEvent : public TarrenMillEvent
{
public:
    TarrenMillShipEvent(uint32 p_ID, uint8 p_State, uint32 p_Duration, TimeRanges p_StartRange)
        : TarrenMillEvent(p_ID, p_State, p_Duration, p_StartRange) {}

    void OnStart() override
    {
        TarrenMillEvent::OnStart();

        OutdoorPvPTarrenMillFun* l_TarrenMillScript = (OutdoorPvPTarrenMillFun*) GetTarrenZoneScript();

        if (!l_TarrenMillScript)
            return;

        bool l_AllianceWon = l_TarrenMillScript->GetCurrentScore(TEAM_ALLIANCE) >= l_TarrenMillScript->GetCurrentScore(TEAM_HORDE);

        l_TarrenMillScript->StartShipEvent(l_AllianceWon);

        ApplyOnAllPlayers([this](Player* p_Player)
        {
            OutdoorPvPTarrenMillFun* l_TarrenMillScript = (OutdoorPvPTarrenMillFun*) GetTarrenZoneScript();

            if (!l_TarrenMillScript)
                return;

            if (l_TarrenMillScript->GetCurrentScore(TEAM_ALLIANCE) >= l_TarrenMillScript->GetCurrentScore(TEAM_HORDE))
                ChatHandler(p_Player).SendSysMessage("Tarren Mill: Alliance WON! A portal to the ships is available in Alliance town for 30 minutes.");
            else
                ChatHandler(p_Player).SendSysMessage("Tarren Mill: Horde WON! A portal to the ships is available in Horde town for 30 minutes.");
        });
    }

    void OnEnd() override
    {
        TarrenMillEvent::OnEnd();

        OutdoorPvPTarrenMillFun* l_TarrenMillScript = (OutdoorPvPTarrenMillFun*) GetTarrenZoneScript();

        if (!l_TarrenMillScript)
            return;

        l_TarrenMillScript->StopShipEvent();

        ApplyOnAllPlayers([this](Player* p_Player)
        {
            ChatHandler(p_Player).SendSysMessage("Tarren Mill: The portal to the ships disapeard.");
        });
    }
};

/// ========================== OutdoorPvPTarrenMillFun ========================== //

OutdoorPvPTarrenMillFun::OutdoorPvPTarrenMillFun()
{
    m_TypeId = OUTDOOR_PVP_TARRENMILL;
}

uint32 OutdoorPvPTarrenMillFun::GetCurrentScore(TeamId p_TeamId)
{
    if (p_TeamId == TeamId::TEAM_ALLIANCE)
        return sWorld->getWorldState(eWorldStates::AllianceScore);
    else
        return sWorld->getWorldState(eWorldStates::HordeScore);
}

bool OutdoorPvPTarrenMillFun::SetupOutdoorPvP()
{
    RegisterZone(eTarrenMillFunDatas::ZoneId);
    RegisterScoresResetTime();

    LoadKillsRewards();

    InitializeGraveyards();
    InitializeEvents();

    return true;
}

void OutdoorPvPTarrenMillFun::StartShipEvent(bool p_AllianceWon)
{
    if (p_AllianceWon)
        AddObject(eTarrenMillEvents::EventPortalShip, eGameObjects::TarrenMillShipPortal, eTarrenMillFunDatas::MapId, 1750.73f, 1061.40f, 6.88f, 0.f, 0.f, 0.f, 0.f, 0.f);
    else
        AddObject(eTarrenMillEvents::EventPortalShip, eGameObjects::TarrenMillShipPortal, eTarrenMillFunDatas::MapId, 2609.18f, 671.74f, 56.18f, 0.f, 0.f, 0.f, 0.f, 0.f);
}

void OutdoorPvPTarrenMillFun::StopShipEvent()
{
    DelObject(eTarrenMillEvents::EventPortalShip);
}

void OutdoorPvPTarrenMillFun::InitializeGraveyards()
{
    SetGraveyardNumber(eGraveyards::TotalGraveyards);
    for (uint8 l_I = 0; l_I < eGraveyards::TotalGraveyards; ++l_I)
    {
        OutdoorGraveyard* l_Graveyard = new OutdoorGraveyard(this);
        l_Graveyard->Initialize(g_TarenMillGraveyards[l_I].StartTeam, g_TarenMillGraveyards[l_I].ID);
        m_GraveyardList[l_I] = l_Graveyard;
    }
}

void OutdoorPvPTarrenMillFun::InitializeEvents()
{
    m_Events.resize(eTarrenMillEvents::MaxEvents, nullptr);

    /*m_Events[eTarrenMillEvents::EventFFA] = new TarrenMillFFAEvent
    (
        eTarrenMillEvents::EventFFA,
        eTarrenMillEventStates::NotStarted,
        eTarrenMillEventDurations::EventFFADuration,
        {
            MakeEventTimeRange(11, 0, 12, 0),
            MakeEventTimeRange(18, 0, 19, 0),
            MakeEventTimeRange(23, 0, 23, 59),
        }
    );*/

    m_Events[eTarrenMillEvents::EventPortalShip] = new TarrenMillShipEvent
    (
        eTarrenMillEvents::EventPortalShip,
        eTarrenMillEventStates::NotStarted,
        eTarrenMillEventDurations::EventPortalShipDuration,
        {}
    );

    /// Compute next events start time
    for (TarrenMillEvent* l_Event : m_Events)
        l_Event->ComputeNextStartTime();
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

void OutdoorPvPTarrenMillFun::ScheduleEventsUpdate(uint32 p_Diff)
{
    for (TarrenMillEvent* l_Event : m_Events)
        l_Event->OnUpdate(p_Diff);
}

bool OutdoorPvPTarrenMillFun::Update(uint32 p_Diff)
{
    if (time(nullptr) > m_ResetScoreTimestamp)
        ResetScores();

    ScheduleEventsUpdate(p_Diff);

    OutdoorPvP::Update(p_Diff);

    return true;
}

void OutdoorPvPTarrenMillFun::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << (uint32)eWorldStates::AllianceScore << (uint32)sWorld->getWorldState(eWorldStates::AllianceScore);
    p_Data << (uint32)eWorldStates::HordeScore    << (uint32)sWorld->getWorldState(eWorldStates::HordeScore);
    p_Data << (uint32)eWorldStates::MaxScore      << (uint32)eTarrenMillFunDatas::MaxScoreValue;

    for (TarrenMillEvent* l_Event : m_Events)
        l_Event->OnFillInitialWorldStates(p_Data);
}

void OutdoorPvPTarrenMillFun::HandlePlayerKilled(Player* p_Player)
{
    for (TarrenMillEvent* l_Event : m_Events)
        l_Event->OnPlayerKilled(p_Player);

    bool l_CanUpdateScoreAtSkill = true;// !m_Events[eTarrenMillEvents::EventFFA]->IsInProgress();
    if (l_CanUpdateScoreAtSkill)
        UpdateScoreAtKill(p_Player);
}

void OutdoorPvPTarrenMillFun::HandleKill(Player* p_Killer, Unit* p_Killed)
{
//     if (p_Killed->GetTypeId() != TypeID::TYPEID_PLAYER)
//         return;
// 
//     if (p_Killed->ToPlayer()->GetTeamId() == p_Killer->GetTeamId())
//         return;
// 
//     uint32 l_Kills = p_Killer->GetCharacterWorldStateValue(eCharacterWorldStates::TarrenMillFunKill);
//     l_Kills++;
// 
//     p_Killer->SetCharacterWorldState(eCharacterWorldStates::TarrenMillFunKill, l_Kills);
//     UpdateRankAura(p_Killer);
//     CheckKillRewardConditions(p_Killer);
}

void OutdoorPvPTarrenMillFun::HandleRewardHonor(Player* p_Player)
{
    uint32 l_Kills = p_Player->GetCharacterWorldStateValue(eCharacterWorldStates::TarrenMillFunKill);
    l_Kills++;

    p_Player->SetCharacterWorldState(eCharacterWorldStates::TarrenMillFunKill, l_Kills);
    UpdateRankAura(p_Player);
    CheckKillRewardConditions(p_Player);

    p_Player->ModifyMoney(20000);
}

void OutdoorPvPTarrenMillFun::CheckKillRewardConditions(Player* p_Player)
{
    uint32 l_Kills     = p_Player->GetCharacterWorldStateValue(eCharacterWorldStates::TarrenMillFunKill);
    uint32 l_KillCount = 0;

    /// Decrease lowers ranks
    for (auto l_Iterator : m_KillRewards)
    {
        if (l_KillCount + l_Iterator.Kills == l_Kills)
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
                p_Player->AddItem(l_ItemTemplate->ItemId, 1);

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
        l_KillCount += l_Iterator.Kills;
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

    if (AuraEffect* l_RankAura = p_Player->GetAuraEffect(l_RankInfo.first, 0))
        l_RankAura->ChangeAmount(l_RankInfo.second);
}

void OutdoorPvPTarrenMillFun::UpdateScoreAtKill(Player* p_Player)
{
    uint32 l_WorldState = 0;

    switch (p_Player->GetTeamId())
    {
        case TeamId::TEAM_ALLIANCE:
            l_WorldState = eWorldStates::HordeScore;
            break;
        case TeamId::TEAM_HORDE:
            l_WorldState = eWorldStates::AllianceScore;
            break;
    }

    /// Neutral case
    if (l_WorldState == 0)
        return;

    uint32 l_Value = sWorld->getWorldState(l_WorldState);
    l_Value++;

    if (l_Value >= eTarrenMillFunDatas::MaxScoreValue)
    {
        if (!m_Events[eTarrenMillEvents::EventPortalShip]->IsInProgress())
            m_Events[eTarrenMillEvents::EventPortalShip]->Start();
        ResetScores();
    }
    else
    {
        sWorld->setWorldState(l_WorldState, l_Value);
        SendUpdateWorldState(l_WorldState, l_Value);
    }
}

void OutdoorPvPTarrenMillFun::HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID)
{
    for (TarrenMillEvent* l_Event : m_Events)
        l_Event->OnPlayerEnter(p_Player);

    UpdateRankAura(p_Player);

    p_Player->ModifyAuraState(AURA_STATE_PVP_RAID_PREPARE, true);
}

void OutdoorPvPTarrenMillFun::HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID)
{
    for (TarrenMillEvent* l_Event : m_Events)
        l_Event->OnPlayerExit(p_Player);

    p_Player->ModifyAuraState(AURA_STATE_PVP_RAID_PREPARE, false);

    RankInfo l_RankInfo = GetRankAuraAndMissingKills(p_Player);
    p_Player->RemoveAurasDueToSpell(l_RankInfo.first);
}

void OutdoorPvPTarrenMillFun::OnCreatureCreate(Creature* p_Creature)
{
    switch (p_Creature->GetEntry())
    {
        case eCreatures::AllianceSpiritGuide:
        case eCreatures::HordeSpiritGuide:
        {
            TeamId l_TeamID = p_Creature->GetEntry() == eCreatures::HordeSpiritGuide ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE;
            uint8 l_GraveyardID = p_Creature->GetEntry() == eCreatures::HordeSpiritGuide ? eGraveyards::HordeCity : eGraveyards::AllianceCity;
            if (m_GraveyardList[l_GraveyardID] != nullptr)
                m_GraveyardList[l_GraveyardID]->SetSpirit(p_Creature, l_TeamID);
            break;
        }
    }
}
/// ========================================================================= //

/// ============================ TarrenMillEvent ============================ //

TarrenMillEvent::TarrenMillEvent(uint32 p_ID, uint8 p_State, uint32 p_Duration, TimeRanges p_StartRange)
    : ID(p_ID), State(p_State), Duration(p_Duration), StartRanges(p_StartRange)
{
    ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(eTarrenMillFunDatas::ZoneId);

    if (!l_ZoneScript)
        return;

    OutdoorPvPTarrenMillFun* l_TarrenMillScript = (OutdoorPvPTarrenMillFun*)l_ZoneScript;

    m_TarrenZoneScript = l_TarrenMillScript;
}

void TarrenMillEvent::Start()
{
    OnStart();
}

void TarrenMillEvent::OnStart()
{
    sLog->outAshran("TarrenMill: Start event [%u]", ID);

    State = eTarrenMillEventStates::Started;
    NextStartTimestamp = time(nullptr);
}

void TarrenMillEvent::OnEnd()
{
    sLog->outAshran("TarrenMill: End event [%u]", ID);

    State = eTarrenMillEventStates::NotStarted;
    ComputeNextStartTime();
}

void TarrenMillEvent::OnUpdate(const uint32 p_Diff)
{
    time_t l_Now = time(nullptr);

    if (State == eTarrenMillEventStates::NotStarted && NextStartTimestamp <= l_Now)
        Start();

    if (State == eTarrenMillEventStates::Started && l_Now > (NextStartTimestamp + Duration))
        OnEnd();
}

void TarrenMillEvent::ComputeNextStartTime()
{
    NextStartTimestamp = 0;

    if (StartRanges.empty())
        return;

    time_t l_StartTimestamp = 0;
    time_t l_CurTime = time(nullptr);
    tm     l_LocalTm = *localtime(&l_CurTime);

    /// Find next range
    for (TarrenMillEvent::TimeRange l_Range : StartRanges)
    {
        l_LocalTm.tm_hour = l_Range.first.Hour;
        l_LocalTm.tm_min = l_Range.first.Minute;

        l_StartTimestamp = mktime(&l_LocalTm);
        if (l_StartTimestamp > l_CurTime)
        {
            /// Get range time
            l_LocalTm.tm_hour = l_Range.second.Hour;
            l_LocalTm.tm_min = l_Range.second.Minute;

            time_t l_EndTimestamp = mktime(&l_LocalTm);
            uint32 l_RangeInSecs = l_EndTimestamp - l_StartTimestamp;

            /// Choose random time in the range
            l_StartTimestamp += rand() % l_RangeInSecs;
            break;
        }
    }

    /// Can happen if we have past the last event range today.
    if (l_StartTimestamp <= l_CurTime)
    {
        l_LocalTm.tm_hour = 0;
        l_LocalTm.tm_min  = 0;
        l_LocalTm.tm_sec  = 0;

        time_t l_Tomorrow = mktime(&l_LocalTm) + DAY;
        l_LocalTm         = *localtime(&l_Tomorrow);

        for (TarrenMillEvent::TimeRange l_Range : StartRanges)
        {
            l_LocalTm.tm_hour = l_Range.first.Hour;
            l_LocalTm.tm_min = l_Range.first.Minute;

            l_StartTimestamp = mktime(&l_LocalTm);
            if (l_StartTimestamp > l_CurTime)
            {
                /// Get range time
                l_LocalTm.tm_hour = l_Range.second.Hour;
                l_LocalTm.tm_min = l_Range.second.Minute;

                time_t l_EndTimestamp = mktime(&l_LocalTm);
                uint32 l_RangeInSecs = l_EndTimestamp - l_StartTimestamp;

                /// Choose random time in the range
                l_StartTimestamp += rand() % l_RangeInSecs;
                break;
            }
        }
    }

    NextStartTimestamp = l_StartTimestamp;
    sLog->outAshran("TarrenMill::ComputeNextStartTime: NextStartTimestamp : [%u] for event [%u]", NextStartTimestamp, ID);
}

bool TarrenMillEvent::IsInProgress() const
{
    return State == eTarrenMillEventStates::Started;
}

OutdoorPvP* TarrenMillEvent::GetTarrenZoneScript()
{
    return m_TarrenZoneScript;
}

void TarrenMillEvent::ApplyOnAllPlayers(std::function<void(Player*)> p_Lambda)
{
    OutdoorPvP* l_TarrenMill = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(eTarrenMillFunDatas::ZoneId);
    if (l_TarrenMill == nullptr)
        return;

    for (uint8 l_Team = 0; l_Team < MS::Battlegrounds::TeamsCount::Value; l_Team++)
    {
        GuidSet const& l_PlayerGuids = l_TarrenMill->GetPlayers(l_Team);
        for (uint64 l_Guid : l_PlayerGuids)
        {
            Player* l_Player = sObjectAccessor->FindPlayer(l_Guid);
            if (l_Player == nullptr)
                continue;

            p_Lambda(l_Player);
        }
    }
}

/// ========================================================================= //

/// ========================== gameobjects scripts ========================== //

class OutdoorPvP_TarrenMillFun : public OutdoorPvPScript
{
    public:

        OutdoorPvP_TarrenMillFun() : OutdoorPvPScript("OutdoorPvP_TarrenMillFun") { }

        OutdoorPvP* GetOutdoorPvP() const
        {
            return new OutdoorPvPTarrenMillFun();
        }
};

class go_tarren_mill_portal : public GameObjectScript
{
    public:
        go_tarren_mill_portal() : GameObjectScript("go_tarren_mill_portal") { }

        bool OnGossipHello(Player* p_Player, GameObject* p_Go)
        {
            Position l_Position;

            if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
            {
                l_Position.m_positionX = 1752.77f;
                l_Position.m_positionY = 1063.11f;
                l_Position.m_positionZ = 6.9f;
            }
            else
            {
                l_Position.m_positionX = 2621.11f;
                l_Position.m_positionY = 636.17f;
                l_Position.m_positionZ = 55.97f;
                l_Position.m_orientation = 4.31f;
            }

            p_Player->TeleportTo(eTarrenMillFunDatas::MapId, l_Position.m_positionX, l_Position.m_positionY, l_Position.m_positionZ, l_Position.m_orientation);

            return true;
        }
};

class go_tarren_mill_ship_portal : public GameObjectScript
{
public:
    go_tarren_mill_ship_portal() : GameObjectScript("go_tarren_mill_ship_portal") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Go)
    {
        Position l_Position;

        if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
        {
            l_Position.m_positionX = 1807.79f;
            l_Position.m_positionY = 1025.76f;
            l_Position.m_positionZ = 160.6f;
            l_Position.m_orientation = 1.51f;
        }
        else
        {
            l_Position.m_positionX = 2644.08f;
            l_Position.m_positionY = 655.103f;
            l_Position.m_positionZ = 177.64f;
            l_Position.m_orientation = 2.03f;
        }

        p_Player->NearTeleportTo(l_Position.m_positionX, l_Position.m_positionY, l_Position.m_positionZ, l_Position.m_orientation);

        return true;
    }
};

/// ========================================================================= //

void AddSC_TarrenMillFun()
{
    new OutdoorPvP_TarrenMillFun();
    new go_tarren_mill_portal();
    new go_tarren_mill_ship_portal();
} 