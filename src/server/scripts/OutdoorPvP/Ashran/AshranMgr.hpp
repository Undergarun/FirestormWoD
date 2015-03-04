////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "OutdoorPvP.h"
#include "OutdoorPvPMgr.h"
#include "AshranDatas.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "Player.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "ScriptedCosmeticAI.hpp"

#ifndef ASHRAN_MGR_HPP_ASHRAN
#define ASHRAN_MGR_HPP_ASHRAN

class OutdoorPvPAshran;

class OutdoorGraveyardAshran : public OutdoorGraveyard
{
    public:
        OutdoorGraveyardAshran(OutdoorPvPAshran* p_OutdoorPvP);
};

class OPvPCapturePoint_Middle : public OPvPCapturePoint
{
    public:

        OPvPCapturePoint_Middle(OutdoorPvP* p_Outdoor, eBattleType p_Type, uint8 p_Faction);

        void ChangeState();

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void UpdateTowerState();

        bool Update(uint32 p_Diff);

        void SpawnFactionGuards(eBattleType p_BattleID, uint8 p_Faction);

        void SetBattleFaction(uint32 p_Faction) { m_BattleFaction = p_Faction; }
        uint32 GetBattleFaction() const { return m_BattleFaction; }
        eBattleType GetBattleType() const { return m_BattleType; }

    protected:

        eBattleType m_BattleType;
        uint32 m_BattleFaction;
};

class OPvPCapturePoint_Graveyard : public OPvPCapturePoint
{
    public:

        OPvPCapturePoint_Graveyard(OutdoorPvP* p_Outdoor);

        void ChangeState();

        void SendChangePhase();

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void UpdateTowerState();

        // Used when player is activated/inactivated in the area
        bool HandlePlayerEnter(Player* p_Player);
        void HandlePlayerLeave(Player* p_Player);

        void SpawnFactionFlags(uint8 p_Faction);

        bool Update(uint32 p_Diff);
        void ScheduleNextControl(uint32 p_Diff);

        uint8 GetGraveyardState() const { return m_GraveyardState; }

    protected:

        uint8 m_GraveyardState;
        uint32 m_ControlTime;
};

class OutdoorPvPAshran : public OutdoorPvP
{
    using PlayerTimerMap = std::map<uint64, uint32>;
    using PlayerCurrencyLoot = std::map<uint64, uint32>;
    using AshranEventsMap = std::map<uint32, uint32>;

    public:
        OutdoorPvPAshran();

        bool SetupOutdoorPvP();

        void HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID);
        void HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID);
        void HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID);
        void HandlePlayerLeaveArea(Player* p_Player, uint32 p_AreaID);
        void HandlePlayerResurrects(Player* p_Player, uint32 p_ZoneID);

        void HandlePlayerKilled(Player* p_Player);
        void HandleKill(Player* p_Killer, Unit* p_Killed);

        bool IsFactionGuard(Unit* p_Unit) const;

        void FillCustomPvPLoots(Player* p_Looter, Loot& p_Loot, uint64 p_Container);

        bool Update(uint32 p_Diff);
        void ScheduleNextBattle(uint32 p_Diff);
        void ScheduleEndOfBattle(uint32 p_Diff);
        void ScheduleInitPoints(uint32 p_Diff);
        void ScheduleEventsUpdate(uint32 p_Diff);

        void StartEvent(uint8 p_EventID);
        void EndEvent(uint8 p_EventID, bool p_ScheduleNext = true);
        void SendEventWarningToPlayers(uint32 p_LangID);

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void SendRemoveWorldStates(Player* p_Player);

        void HandleBFMGREntryInviteResponse(bool p_Accepted, Player* p_Player);

        void OnCreatureCreate(Creature* p_Creature);
        Creature* GetHerald() const;

        void ResetControlPoints();
        void InitializeControlPoints();
        void InitializeEvents();
        bool IsInitialized() const { return m_IsInitialized; }

        void SetBattleState(uint32 p_NewState);
        void SetNextBattleTimer(uint32 p_Time) { m_NextBattleTimer = p_Time * TimeConstants::IN_MILLISECONDS; }

        void AddGenericMoPGuid(uint8 p_Type, uint64 p_Guid) { m_GenericMoPGuids[p_Type] = p_Guid; }
        uint64 GetGenericMoPGuid(uint8 p_Type) const { return m_GenericMoPGuids[p_Type]; }
        uint64 GetFactionGenericMoP(uint8 p_Faction) const { return m_FactionGenericMoP[p_Faction]; }

        uint32 GetCurrentBattleType() const;

        void HandleFactionBossDeath(uint8 p_Faction);

        OPvPCapturePoint_Middle* GetCapturePoint(uint8 p_Index) const { return m_ControlPoints[p_Index]; }

        WorldSafeLocsEntry const* GetClosestGraveyard(Player* p_Player);
        uint8 GetSpiritGraveyardID(uint32 p_AreaID, TeamId p_Team) const;

    private:

        OPvPCapturePoint_Graveyard* m_GraveYard;
        OPvPCapturePoint_Middle* m_ControlPoints[eBattleType::MaxBattleType];
        uint64 m_GenericMoPGuids[eBattleType::MaxBattleType];
        uint64 m_FactionGenericMoP[MS::Battlegrounds::TeamsCount::Value];
        uint32 m_InitPointsTimer;
        bool m_IsInitialized;
        bool m_WillBeReset;

        uint64 m_Guid;
        uint64 m_HeraldGuid;
        uint64 m_HighWarlordVolrath;
        uint64 m_GrandMasrhalTremblade;
        uint32 m_WorldPvPAreaId;

        GuidSet m_PlayersInWar[MS::Battlegrounds::TeamsCount::Value];
        PlayerTimerMap m_InvitedPlayers[MS::Battlegrounds::TeamsCount::Value];
        PlayerTimerMap m_PlayersWillBeKick[MS::Battlegrounds::TeamsCount::Value];
        PlayerCurrencyLoot m_PlayerCurrencyLoots;

        uint32 m_EnnemiesKilled[MS::Battlegrounds::TeamsCount::Value];
        uint32 m_EnnemiesKilledMax[MS::Battlegrounds::TeamsCount::Value];

        uint32 m_AshranEvents[eAshranEvents::MaxEvents];
        bool m_AshranEventsWarned[eAshranEvents::MaxEvents];

        uint32 m_CurrentBattleState;
        uint32 m_NextBattleTimer;
        uint32 m_MaxBattleTime;
};

#endif