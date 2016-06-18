////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BATTLEGROUNDTP_H
#define __BATTLEGROUNDTP_H

#include "Battleground.h"

enum BG_TP_TimerOrScore
{
    BG_TP_MAX_TEAM_SCORE        = 3,
    BG_TP_FLAG_RESPAWN_TIME     = 23000,
    BG_TP_FLAG_DROP_TIME        = 10000,
    BG_TP_SPELL_FORCE_TIME      = 600000,
    BG_TP_SPELL_BRUTAL_TIME     = 900000
};

enum BG_TP_Sound
{
    BG_TP_SOUND_FLAG_CAPTURED_ALLIANCE      = 8173,
    BG_TP_SOUND_FLAG_CAPTURED_HORDE         = 8213,
    BG_TP_SOUND_FLAG_PLACED                 = 8232,
    BG_TP_SOUND_FLAG_RETURNED               = 8192,
    BG_TP_SOUND_HORDE_FLAG_PICKED_UP        = 8212,
    BG_TP_SOUND_ALLIANCE_FLAG_PICKED_UP     = 8174,
    BG_TP_SOUND_FLAGS_RESPAWNED             = 8232
};

enum BG_TP_SpellId
{
    BG_TP_SPELL_HORDE_FLAG                  = 23333,
    BG_TP_SPELL_HORDE_FLAG_DROPPED          = 23334,
    BG_TP_SPELL_HORDE_FLAG_PICKED           = 61266,    // fake spell, does not exist but used as timer start event
    BG_TP_SPELL_ALLIANCE_FLAG               = 23335,
    BG_TP_SPELL_ALLIANCE_FLAG_DROPPED       = 23336,
    BG_TP_SPELL_ALLIANCE_FLAG_PICKED        = 61265,    // fake spell, does not exist but used as timer start event
    BG_TP_SPELL_FOCUSED_ASSAULT             = 46392,
    BG_TP_SPELL_BRUTAL_ASSAULT              = 46393
};

enum BG_TP_WorldStates
{
    BG_TP_FLAG_UNK_ALLIANCE                 = 1545,
    BG_TP_FLAG_UNK_HORDE                    = 1546,
    BG_TP_FLAG_CAPTURES_ALLIANCE            = 1581,
    BG_TP_FLAG_CAPTURES_HORDE               = 1582,
    BG_TP_FLAG_CAPTURES_MAX                 = 1601,
    BG_TP_FLAG_STATE_HORDE                  = 2338,
    BG_TP_FLAG_STATE_ALLIANCE               = 2339,
    BG_TP_STATE_TIMER                       = 4248,
    BG_TP_STATE_TIMER_ACTIVE                = 4247
};

enum BG_TP_ObjectTypes
{
    BG_TP_OBJECT_DOOR_A_1        = 0,
    BG_TP_OBJECT_DOOR_A_2        = 1,
    BG_TP_OBJECT_DOOR_A_3        = 2,
    BG_TP_OBJECT_DOOR_A_4        = 3,
    BG_TP_OBJECT_DOOR_H_1        = 4,
    BG_TP_OBJECT_DOOR_H_2        = 5,
    BG_TP_OBJECT_DOOR_H_3        = 6,
    BG_TP_OBJECT_DOOR_H_4        = 7,
    BG_TP_OBJECT_A_FLAG          = 8,
    BG_TP_OBJECT_H_FLAG          = 9,
    BG_TP_OBJECT_SPEEDBUFF_1     = 10,
    BG_TP_OBJECT_SPEEDBUFF_2     = 11,
    BG_TP_OBJECT_REGENBUFF_1     = 12,
    BG_TP_OBJECT_REGENBUFF_2     = 13,
    BG_TP_OBJECT_BERSERKBUFF_1   = 14,
    BG_TP_OBJECT_BERSERKBUFF_2   = 15,
    BG_TP_OBJECT_MAX             = 16
};

enum BG_TP_ObjectEntry
{
    BG_OBJECT_DOOR_A_1_TP_ENTRY        = 206655,
    BG_OBJECT_DOOR_A_2_TP_ENTRY        = 206654,
    BG_OBJECT_DOOR_A_3_TP_ENTRY        = 206653,
    BG_OBJECT_DOOR_A_4_TP_ENTRY        = 206653,
    BG_OBJECT_DOOR_H_1_TP_ENTRY        = 208205,
    BG_OBJECT_DOOR_H_2_TP_ENTRY        = 208206,
    BG_OBJECT_DOOR_H_3_TP_ENTRY        = 208206,
    BG_OBJECT_DOOR_H_4_TP_ENTRY        = 208207,
    BG_OBJECT_A_FLAG_TP_ENTRY          = 179830,
    BG_OBJECT_H_FLAG_TP_ENTRY          = 179831,
    BG_OBJECT_A_FLAG_GROUND_TP_ENTRY   = 179785,
    BG_OBJECT_H_FLAG_GROUND_TP_ENTRY   = 179786
};

enum BG_TP_FlagState
{
    BG_TP_FLAG_STATE_ON_BASE        = 0,
    BG_TP_FLAG_STATE_WAIT_RESPAWN   = 1,
    BG_TP_FLAG_STATE_ON_PLAYER      = 2,
    BG_TP_FLAG_STATE_ON_GROUND      = 3
};

enum BG_TP_Graveyards
{
    TP_GRAVEYARD_FLAGROOM_ALLIANCE  = 1726,
    TP_GRAVEYARD_FLAGROOM_HORDE     = 1727,
    TP_GRAVEYARD_START_ALLIANCE     = 1729,
    TP_GRAVEYARD_START_HORDE        = 1728,
    TP_GRAVEYARD_MIDDLE_ALLIANCE    = 1749,
    TP_GRAVEYARD_MIDDLE_HORDE       = 1750
};

enum BG_TP_CreatureTypes
{
    TP_SPIRIT_ALLIANCE    = 0,
    TP_SPIRIT_HORDE       = 1,

    BG_CREATURES_MAX_TP   = 2
};

enum BG_TP_CarrierDebuffs
{
    TP_SPELL_FOCUSED_ASSAULT   = 46392,
    TP_SPELL_BRUTAL_ASSAULT    = 46393
};

enum BG_TP_Objectives
{
    TP_OBJECTIVE_CAPTURE_FLAG   = 290,
    TP_OBJECTIVE_RETURN_FLAG    = 291
};

#define TP_EVENT_START_BATTLE   8563

class BattlegroundTPScore : public BattlegroundScore
{
    public:
        BattlegroundTPScore() : FlagCaptures(0), FlagReturns(0) {};
        virtual ~BattlegroundTPScore() {};
        uint32 FlagCaptures;
        uint32 FlagReturns;
};

class BattlegroundTP : public Battleground
{
    friend class BattlegroundMgr;

    public:
        /* Construction */
        BattlegroundTP();
        ~BattlegroundTP();

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player *player);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();

        /* BG Flags */
        uint64 GetAllianceFlagPickerGUID() const    { return _flagKeepers[TEAM_ALLIANCE]; }
        uint64 GetHordeFlagPickerGUID() const       { return _flagKeepers[TEAM_HORDE]; }
        void SetAllianceFlagPicker(uint64 guid)     { _flagKeepers[TEAM_ALLIANCE] = guid; }
        void SetHordeFlagPicker(uint64 guid)        { _flagKeepers[TEAM_HORDE] = guid; }
        bool IsAllianceFlagPickedup() const         { return _flagKeepers[TEAM_ALLIANCE] != 0; }
        bool IsHordeFlagPickedup() const            { return _flagKeepers[TEAM_HORDE] != 0; }
        void RespawnFlag(uint32 Team, bool captured);
        void RespawnFlagAfterDrop(uint32 Team);
        uint8 GetFlagState(uint32 team)             { return m_FlagState[GetTeamIndexByTeamId(team)]; }
        void AddTimedAura(uint32 aura);
        void RemoveTimedAura(uint32 aura);
        bool IsBrutalTimerDone;
        bool IsForceTimerDone;

        /* Battleground Events */
        virtual void EventPlayerDroppedFlag(Player *Source);
        virtual void EventPlayerClickedOnFlag(Player *Source, GameObject* target_obj);
        virtual void EventPlayerCapturedFlag(Player *Source);

        void RemovePlayer(Player *player, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        void HandleKillPlayer(Player *player, Player *killer);
        bool SetupBattleground();
        virtual void Reset();
        void EndBattleground(uint32 winner);
        virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        void UpdateFlagState(uint32 team, uint32 value);
        void SetLastFlagCapture(uint32 team)                { m_LastFlagCaptureTeam = team; }
        void UpdateTeamScore(uint32 team);
        void UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor = true, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType = MS::Battlegrounds::RewardCurrencyType::Type::None);
        void SetDroppedFlagGUID(uint64 guid, uint32 TeamID)  { m_DroppedFlagGUID[GetTeamIndexByTeamId(TeamID)] = guid;}
        uint64 GetDroppedFlagGUID(uint32 TeamID)             { return m_DroppedFlagGUID[GetTeamIndexByTeamId(TeamID)];}
        virtual void FillInitialWorldStates(ByteBuffer& data);

        /* Scorekeeping */
        uint32 GetTeamScore(uint32 p_TeamID) const   { return m_TeamScores[GetTeamIndexByTeamId(p_TeamID)]; }
        uint32 GetMaxScore() const { return BG_TP_MAX_TEAM_SCORE; }
        bool IsScoreIncremental() const { return true; }

        void AddPoint(uint32 TeamID, uint32 Points = 1)     { m_TeamScores[GetTeamIndexByTeamId(TeamID)] += Points; }
        void SetTeamPoint(uint32 TeamID, uint32 Points = 0) { m_TeamScores[GetTeamIndexByTeamId(TeamID)] = Points; }
        void RemovePoint(uint32 TeamID, uint32 Points = 1)  { m_TeamScores[GetTeamIndexByTeamId(TeamID)] -= Points; }
#ifdef CROSS

        uint32 GetZoneId() const { return BATTLEGROUND_TTP_ZONEID; }
#endif /* CROSS */
    private:
        uint64 _flagKeepers[2];                            // 0 - alliance, 1 - horde
        uint64 m_DroppedFlagGUID[2];
        uint8 m_FlagState[2];                               // for checking flag state
        int32 m_FlagsTimer[2];
        int32 m_FlagsDropTimer[2];
        uint32 m_LastFlagCaptureTeam;                       // Winner is based on this if score is equal

        uint32 m_ReputationCapture;
        uint32 m_HonorWinKills;
        uint32 m_HonorEndKills;
        int32 m_FlagSpellForceTimer;
        bool m_BothFlagsKept;
        uint8 m_FlagDebuffState;                            // 0 - no debuffs, 1 - focused assault, 2 - brutal assault
        uint32 m_EndTimestamp;

        virtual void PostUpdateImpl(uint32 diff);
        int32 m_CheatersCheckTimer;

        uint64 GetFlagPickerGUID(int32 p_Team) const
        {
            if (p_Team != TEAM_ALLIANCE && p_Team != TEAM_HORDE)
                return 0;

            return _flagKeepers[p_Team];
        }

        std::set<uint64> const GetFlagPickersGUID(int32 p_Team) const
        {
            if (p_Team != TEAM_ALLIANCE && p_Team != TEAM_HORDE)
                return std::set<uint64>();

            std::set<uint64> l_FlagPickers{ _flagKeepers[p_Team] };
            return l_FlagPickers;
        }
};

#endif
