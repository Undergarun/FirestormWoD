////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BATTLEGROUNDSM_H
#define __BATTLEGROUNDSM_H

#include "Language.h"
#include "Battleground.h"

enum BG_SM_WorldStates
{
    SM_MINE_CARTS_DISPLAY            = 6436,
    SM_ALLIANCE_RESOURCES            = 6437,
    SM_HORDE_RESOURCES               = 6438,
    SM_MINE_CART_1                   = 6439,
    SM_MINE_CART_2                   = 6440,
    SM_DISPLAY_ALLIANCE_RESSOURCES   = 6441,
    SM_MINE_CART_3                   = 6442,
    SM_DISPLAY_HORDE_RESSOURCES      = 6443,
    SM_DISPLAY_PROGRESS_BAR          = 6875, // 0 = false, 1 = true
    SM_PROGRESS_BAR_STATUS           = 6876, // 0 = Horde max, 50 = Neutral, 100 = Alliance max
    SM_UNK                           = 6877
};

enum BG_SM_MineCarts
{
    BG_SM_MINE_CART_1 = 1,
    BG_SM_MINE_CART_2 = 2,
    BG_SM_MINE_CART_3 = 3
};

enum BG_SM_ProgressBarConsts
{
    BG_SM_PROGRESS_BAR_DONT_SHOW  = 0,
    BG_SM_PROGRESS_BAR_SHOW       = 1,
    BG_SM_PROGRESS_BAR_NEUTRAL    = 50
};

enum BG_SM_Sounds
{
    BG_SM_SOUND_MINE_CART_CAPTURED_HORDE     = 8213,
    BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE  = 8173,
    BG_SM_SOUND_NEAR_VICTORY                 = 8456,
    BG_SM_SOUND_MINE_CART_SPAWNED            = 8174
};

enum BG_SM_Spells
{
    BG_SM_CONTROL_VISUAL_ALLIANCE   = 116086,
    BG_SM_CONTROL_VISUAL_HORDE      = 116085,
    BG_SM_CONTROL_VISUAL_NEUTRAL    = 118001,
    BG_SM_TRACK_SWITCH_OPENED       = 120228,
    BG_SM_TRACK_SWITCH_CLOSED       = 120229,
    BG_SM_FEIGN_DEATH_STUN          = 135781, // Prevent from turn moves
    BG_SM_PREVENTION_AURA           = 135846  // Prevent from using multiple times track switches
};

enum SMBattlegroundObjectEntry
{
    BG_SM_WATERFALL_DEPOT   = 212080,
    BG_SM_LAVA_DEPOT        = 212081,
    BG_SM_DIAMOND_DEPOT     = 212082,
    BG_SM_TROLL_DEPOT       = 212083,
    BG_SM_DOOR              = 157819
};

enum SMBattlegroundGaveyards
{
    SM_GRAVEYARD_MAIN_ALLIANCE     = 4062,
    SM_GRAVEYARD_MAIN_HORDE        = 4061
};

enum SMBattlegroundCreaturesTypes
{
    SM_SPIRIT_ALLIANCE        = 0,
    SM_SPIRIT_HORDE           = 1,
    NPC_SM_MINE_CART_1        = 2,
    NPC_SM_MINE_CART_2        = 3,
    NPC_SM_MINE_CART_3        = 4,
    SM_TRACK_SWITCH_EAST      = 5,
    SM_TRACK_SWITCH_NORTH     = 6,

    BG_SM_CREATURES_MAX       = 7
};

enum SMDepots
{
    SM_WATERFALL_DEPOT,
    SM_LAVA_DEPOT,
    SM_DIAMOND_DEPOT,
    SM_TROLL_DEPOT
};

enum SMTracks
{
    SM_EAST_TRACK_SWITCH,
    SM_NORTH_TRACK_SWITCH,
    SM_MAX_TRACK_SWITCH
};

enum SMBattlegroundObjectTypes
{
    BG_SM_OBJECT_DOOR_A_1                   = 0,
    BG_SM_OBJECT_DOOR_H_1                   = 1,
    BG_SM_OBJECT_DOOR_A_2                   = 2,
    BG_SM_OBJECT_DOOR_H_2                   = 3,
    BG_SM_OBJECT_WATERFALL_DEPOT            = 4,
    BG_SM_OBJECT_LAVA_DEPOT                 = 5,
    BG_SM_OBJECT_DIAMOND_DEPOT              = 6,
    BG_SM_OBJECT_TROLL_DEPOT                = 7,
    BG_SM_OBJECT_BERSERKING_BUFF_EAST       = 8,
    BG_SM_OBJECT_BERSERKING_BUFF_WEST       = 9,
    BG_SM_OBJECT_RESTORATION_BUFF_WATERFALL = 10,
    BG_SM_OBJECT_RESTORATION_BUFF_LAVA      = 11,

    BG_SM_OBJECT_MAX                        = 12
};

enum BG_SM_Score
{
    BG_SM_WARNING_NEAR_VICTORY_SCORE    = 1200,
    BG_SM_MAX_TEAM_SCORE                = 1500
};

enum SMBattlegroundMineCartState
{
    SM_MINE_CART_CONTROL_NEUTRAL    = 0,
    SM_MINE_CART_CONTROL_ALLIANCE   = 1,
    SM_MINE_CART_CONTROL_HORDE      = 2
};

enum BG_SM_CreatureIds
{
    NPC_TRACK_SWITCH_EAST    = 60283,
    NPC_TRACK_SWITCH_NORTH   = 60309,
    NPC_MINE_CART_1          = 60378,
    NPC_MINE_CART_2          = 60379,
    NPC_MINE_CART_3          = 60380,
    NPC_MINE_CART_TRIGGER    = 400464
};

enum BG_SM_Paths
{
    SM_EAST_PATH,
    SM_NORTH_PATH
};

const float BG_SM_BuffPos[4][4] =
{
    { 749.444153f, 64.338188f, 369.535797f, 6.058259f },   // Berserking buff East
    { 789.979431f, 281.883575f, 355.389984f, 0.652173f },  // Berserking buff West
    { 539.873596f, 396.386749f, 345.722412f, 3.994188f },  // Restoration buff Waterfall
    { 614.202698f, 120.924660f, 294.430908f, 4.241807f }   // Restoration buff Lava
};

const float BG_SM_DepotPos[4][4] =
{
    { 566.950989f, 337.05801f, 347.295013f, 1.559089f },   // Waterfall
    { 619.469971f, 79.719597f, 299.067993f, 1.625564f },   // Lava
    { 895.974426f, 27.210802f, 364.390991f, 3.445790f },   // Diamond
    { 778.444946f, 500.949707f, 359.738983f, 0.737040f }   // Troll
};

const float BG_SM_DoorPos[4][4] =
{
    { 852.0289952f, 158.216003f, 326.761011f, 0.150758f }, // Alliance 1
    { 830.092102f, 143.925507f,  326.5f,      3.130245f},       // Alliance 2
    { 652.177612f, 228.493423f,  326.917480f, 0.163844f },  // Horde 1
    { 635.622925f, 208.220886f,  326.648315f, 3.717332f }   // Horde 2
};

const float BG_SM_TrackPos[2][4] =
{
    { 715.585388f, 101.272034f, 319.994690f, 4.647377f }, // East
    { 847.481689f, 308.032562f, 346.573242f, 0.587086f }  // North
};

const float BG_SM_StartPos[3][4] =
{
    { 744.542053f, 183.545883f, 319.658203f, 4.356342f },
    { 739.400330f, 203.598511f, 319.603333f, 2.308198f },
    { 760.184509f, 198.844742f, 319.446655f, 0.351249f }
};

const uint32 BG_SM_CartIDs[3]
{
    NPC_MINE_CART_1,
    NPC_MINE_CART_2,
    NPC_MINE_CART_3
};

const uint32 BG_SM_CartTypes[3]
{
    NPC_SM_MINE_CART_1,
    NPC_SM_MINE_CART_2,
    NPC_SM_MINE_CART_3
};

#define MINE_CART_AT_DEPOT_POINTS       200
#define POINTS_PER_MINE_CART            150
#define DISPLAY_WORLDSTATE              1
#define BG_SM_NotSMWeekendHonorTicks    200
#define BG_SM_SMWeekendHonorTicks       130
#define SM_MINE_CART_MAX                3
#define SM_MAX_PATHS                    2

struct BattlegroundSMScore : public BattlegroundScore
{
    BattlegroundSMScore() : BattlegroundScore(), MineCartCaptures(0) { }
    virtual ~BattlegroundSMScore() { }

    uint32 MineCartCaptures;
};

class BattlegroundSM : public Battleground
{
    public:
        BattlegroundSM();
        ~BattlegroundSM();

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* p_Player);
        void StartingEventCloseDoors();
        void StartingEventOpenDoors();

        void HandleKillPlayer(Player* p_Player, Player* p_Killer);
        bool SetupBattleground();
        void Reset();
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* p_Player);
        void PostUpdateImpl(uint32 p_Diff);
        void UpdateTeamScore(uint32 p_Team);
        void EndBattleground(uint32 p_Winner);
        void UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor = true, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType = MS::Battlegrounds::RewardCurrencyType::Type::None);
        void FillInitialWorldStates(ByteBuffer& p_Data);
        void EventPlayerClickedOnFlag(Player* p_Player, Unit* p_Target);

        /* Battleground Events */
        void EventReopenDepot(uint32 p_Diff);

#ifdef CROSS
        uint32 GetZoneId() const { return BATTLEGROUND_SSM_ZONEID; }

#endif /* CROSS */
    private:
        void EventTeamCapturedMineCart(uint32 p_Team, uint8 p_MineCart);
        void SummonMineCart(uint32 p_Diff);
        void FirstMineCartSummon(uint32 p_Diff);
        void MineCartsMoves();
        void CheckPlayerNearMineCart(uint32 p_Diff);
        void CheckMineCartNearDepot();
        void MineCartAddPoints(uint32 p_Diff);
        void ResetDepotsAndMineCarts(uint8 p_Depot, uint8 p_MineCart);
        void CheckTrackSwitch();

        /* Scorekeeping */
        uint32 GetTeamScore(uint32 p_TeamID) const { return m_TeamScores[GetTeamIndexByTeamId(p_TeamID)]; }
        uint32 GetMaxScore() const { return BG_SM_MAX_TEAM_SCORE; }
        bool IsScoreIncremental() const { return true; }

        void AddPoints(uint32 p_Team, uint32 p_Points);

        void RemovePoint(uint32 p_TeamID, uint32 p_Points = 1) { m_TeamScores[GetTeamIndexByTeamId(p_TeamID)] -= p_Points; }
        void SetTeamPoint(uint32 p_TeamID, uint32 p_Points = 0) { m_TeamScores[GetTeamIndexByTeamId(p_TeamID)] = p_Points; }
        
        uint32 GetMineCartTeamKeeper(uint8 p_MineCart);
        uint32 m_HonorScoreTics[2];

        uint32 m_MineCartsTrigger[SM_MINE_CART_MAX]; ///< m_MineCartsTrigger is not used
        int32 m_MineCartsProgressBar[SM_MINE_CART_MAX];
        uint32 m_MineCartTeamKeeper[SM_MINE_CART_MAX]; // keepers team ///< m_MineCartTeamKeeper is not used
        int32 m_FirstMineCartSummonTimer;
        int32 m_MineCartCheckTimer;
        int32 m_DepotCloseTimer[4];
        uint32 m_MineCartAddPointsTimer;
        int32 m_TrackSwitchClickTimer[SM_MAX_TRACK_SWITCH];
        bool m_Depot[4]; // 0 = Waterfall, 1 = Lava, 2 = Diamond, 3 = Troll
        bool m_MineCartReachedDepot[SM_MINE_CART_MAX];
        bool m_MineCartNearDepot[SM_MINE_CART_MAX];
        bool m_MineCartSpawned[SM_MINE_CART_MAX];
        bool m_FirstMineCartSpawned;
        bool m_PathDone[SM_MINE_CART_MAX - 1][SM_MAX_PATHS]; // Only for first and third mine cart
        bool m_WaterfallPathDone; // Waterfall path
        bool m_TrackSwitch[SM_MAX_TRACK_SWITCH]; // East : true = open, false = close | North : true = close, false = open
        bool m_TrackSwitchCanInterract[SM_MAX_TRACK_SWITCH];

        uint32 m_HonorTics;
        bool m_IsInformedNearVictory;
};

#endif
