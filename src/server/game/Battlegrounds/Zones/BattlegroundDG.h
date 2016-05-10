////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BATTLEGROUND_DG_H
#define __BATTLEGROUND_DG_H

#include "Battleground.h"

#define BG_DG_ALL_NODES_COUNT 3
#define BG_DG_OBJECT_DYNAMIC_TOTAL 4

enum BG_DG_Sound
{
    BG_DG_SOUND_NODE_CLAIMED            = 8192,
    BG_DG_SOUND_NODE_CAPTURED_ALLIANCE  = 8173,
    BG_DG_SOUND_NODE_CAPTURED_HORDE     = 8213,
    BG_DG_SOUND_NODE_ASSAULTED_ALLIANCE = 8212,
    BG_DG_SOUND_NODE_ASSAULTED_HORDE    = 8174,

    BG_DG_SOUND_CART_CAPTURED_ALLIANCE  = 8173,
    BG_DG_SOUND_CART_CAPTURED_HORDE     = 8213,
    BG_DG_SOUND_CART_PLACED             = 8232,
    BG_DG_SOUND_CART_RETURNED           = 8192,
    BG_DG_SOUND_HORDE_CART_PICKED_UP    = 8212,
    BG_DG_SOUND_ALLIANCE_CART_PICKED_UP = 8174,

    BG_DG_SOUND_ALLIANCE_NEAR_VICTORY   = 8456,
    BG_DG_SOUND_HORDE_NEAR_VICTORY      = 8457
};

const float BG_DG_DoorPositions[4][8] =
{
    //      x,          y,          z,          o,        rot0, rot1, rot2, rot3
    { -263.434998f, 218.514999f, 132.054993f, 4.683630f, 0.0f, 0.0f, 0.0f, 0.0f },
    { -213.848007f, 201.164001f, 132.382004f, 3.978350f, 0.0f, 0.0f, 0.0f, 0.0f },
    { -119.559998f, 799.192993f, 132.414001f, 0.793560f, 0.0f, 0.0f, 0.0f, 0.0f },
    { -70.1034010f, 781.851013f, 132.164993f, 1.594670f, 0.0f, 0.0f, 0.0f, 0.0f }
};

enum BG_DG_NodeObjectId
{
    BG_DG_OBJECTID_CAPT_POINT           = 53194,
    BG_DG_OBJECTID_AURA_A               = 180100,
    BG_DG_OBJECTID_AURA_H               = 180101,
    BG_DG_OBJECTID_AURA_C               = 180102,
    BG_DG_OBJECTID_GATE                 = 220163,
    BG_DG_OBJECTID_PJ_COLLISION         = 188215,
    BG_DG_OBJECTID_CART_ALLY            = 220164,
    BG_DG_OBJECTID_CART_HORDE           = 220166,
    BG_DG_OBJECTID_CART_ALLY_GROUND     = 220165,
    BG_DG_OBJECTID_CART_HORDE_GROUND    = 220174
};

enum BG_DG_Spells
{
    BG_DG_ALLIANCE_MINE_CART            = 140876,
    BG_DG_ALLIANCE_CART_HOLDER_AURA     = 141551,
    BG_DG_HORDE_MINE_CART               = 141210,
    BG_DG_HORDE_CART_HOLDER_AURA        = 141555,
    BG_DG_TRACK_MINE_CART               = 52595,
    BG_DG_CAPT_POINT_START_CAPT         = 97388,
    BG_DG_CAPT_POINT_NEUTRAL            = 98554,
    BG_DG_CAPT_POINT_ALLIANCE_CONTEST   = 98543,
    BG_DG_CAPT_POINT_HORDE_CONTEST      = 98545,
    BG_DG_CAPT_POINT_ALLIANCE_CONTROL   = 98519,
    BG_DG_CAPT_POINT_HORDE_CONTROL      = 98527
};

enum BG_DG_Worldstate
{
    WORLDSTATE_DG_SCORE_ALLIANCE            = 7880,
    WORLDSTATE_DG_SCORE_HORDE               = 7881,
    WORLDSTATE_DG_OCCUPIED_BASES_ALLIANCE   = 8230,
    WORLDSTATE_DG_OCCUPIED_BASES_HORDE      = 8231,
    WORLDSTATE_DG_CART_STATE_ALLIANCE       = 7904,
    WORLDSTATE_DG_CART_STATE_HORDE          = 7887,
    WORLDSTATE_DG_PANDA_NEUTRAL             = 7935,
    WORLDSTATE_DG_CENTER_NEUTRAL            = 7939,
    WORLDSTATE_DG_GOBLIN_NEUTRAL            = 7938,
    WORLDSTATE_DG_PANDA_ALLY_CONTEST        = 7857,
    WORLDSTATE_DG_CENTER_ALLY_CONTEST       = 7934,
    WORLDSTATE_DG_GOBLIN_ALLY_CONTEST       = 7864,
    WORLDSTATE_DG_PANDA_HORDE_CONTEST       = 7861,
    WORLDSTATE_DG_CENTER_HORDE_CONTEST      = 7936,
    WORLDSTATE_DG_GOBLIN_HORDE_CONTEST      = 7865,
    WORLDSTATE_DG_PANDA_ALLY_CONTROL        = 7855, // Has to be set to 2
    WORLDSTATE_DG_CENTER_ALLY_CONTROL       = 7932, // Has to be set to 2
    WORLDSTATE_DG_GOBLIN_ALLY_CONTROL       = 7856, // Has to be set to 2
    WORLDSTATE_DG_PANDA_HORDE_CONTROL       = 7855, // Has to be set to 1
    WORLDSTATE_DG_CENTER_HORDE_CONTROL      = 7932, // Has to be set to 1
    WORLDSTATE_DG_GOBLIN_HORDE_CONTROL      = 7856  // Has to be set to 1
};

struct WorldStateIcon
{
    uint32 m_WorldState;
    uint32 m_SentValueToShow;
    uint32 m_SentValueToHide;

    WorldStateIcon(uint32 p_WorldState, uint32 p_Value) : m_WorldState(p_WorldState), m_SentValueToShow(p_Value), m_SentValueToHide(0) { }
};

const WorldStateIcon BG_DG_NodesWs[BG_DG_ALL_NODES_COUNT][5] =
{
    { 
        WorldStateIcon(WORLDSTATE_DG_GOBLIN_NEUTRAL,        1), 
        WorldStateIcon(WORLDSTATE_DG_GOBLIN_ALLY_CONTEST,   1), 
        WorldStateIcon(WORLDSTATE_DG_GOBLIN_HORDE_CONTEST,  1), 
        WorldStateIcon(WORLDSTATE_DG_GOBLIN_ALLY_CONTROL,   2), 
        WorldStateIcon(WORLDSTATE_DG_GOBLIN_HORDE_CONTROL,  1) 
    },
    { 
        WorldStateIcon(WORLDSTATE_DG_CENTER_NEUTRAL,        1), 
        WorldStateIcon(WORLDSTATE_DG_CENTER_ALLY_CONTEST,   1), 
        WorldStateIcon(WORLDSTATE_DG_CENTER_HORDE_CONTEST,  1), 
        WorldStateIcon(WORLDSTATE_DG_CENTER_ALLY_CONTROL,   2), 
        WorldStateIcon(WORLDSTATE_DG_CENTER_HORDE_CONTROL,  1) 
    },
    { 
        WorldStateIcon(WORLDSTATE_DG_PANDA_NEUTRAL,         1), 
        WorldStateIcon(WORLDSTATE_DG_PANDA_ALLY_CONTEST,    1), 
        WorldStateIcon(WORLDSTATE_DG_PANDA_HORDE_CONTEST,   1), 
        WorldStateIcon(WORLDSTATE_DG_PANDA_ALLY_CONTROL,    2), 
        WorldStateIcon(WORLDSTATE_DG_PANDA_HORDE_CONTROL,   1) 
    }
};

enum BG_DG_CreatureTypes
{
    BG_DG_SPIRIT_NORTHERN_ALLIANCE,
    BG_DG_SPIRIT_NORTHERN_HORDE,
    BG_DG_SPIRIT_SOUTHERN_ALLIANCE,
    BG_DG_SPIRIT_SOUTHERN_HORDE,
    BG_DG_OBJECT_CAPT_POINT_GOBLIN,
    BG_DG_OBJECT_CAPT_POINT_CENTER,
    BG_DG_OBJECT_CAPT_POINT_PANDAREN,
    BG_DG_CREATURES_MAX,

    BG_DG_OBJECT_CAPT_POINT_START   = BG_DG_OBJECT_CAPT_POINT_GOBLIN,
    BG_DG_OBJECT_CAPT_POINT_END     = BG_DG_OBJECT_CAPT_POINT_PANDAREN
};

enum BG_DG_NodeStatus
{
    BG_DG_NODE_TYPE_NEUTRAL             = 0,
    BG_DG_NODE_TYPE_CONTESTED           = 1,
    BG_DG_NODE_STATUS_ALLY_CONTESTED    = 1,
    BG_DG_NODE_STATUS_HORDE_CONTESTED   = 2,
    BG_DG_NODE_TYPE_OCCUPIED            = 3,
    BG_DG_NODE_STATUS_ALLY_OCCUPIED     = 3,
    BG_DG_NODE_STATUS_HORDE_OCCUPIED    = 4
};

enum BG_DG_CartState
{
    BG_DG_CART_STATE_ON_BASE    = 0,
    BG_DG_CART_STATE_ON_PLAYER  = 2,
    BG_DG_CART_STATE_ON_GROUND  = 3
};

enum BG_DG_Scores
{
    BG_DG_NEAR_VICTORY_POINTS   = 1200,
    BG_DG_MAX_VICTORY_POINTS    = 1500
};

enum BG_DG_Nodes
{
    BG_DG_NODE_GOBLIN_MINE   = 0,
    BG_DG_NODE_CENTER_MINE   = 1,
    BG_DG_NODE_PANDAREN_MINE = 2
};

enum BG_DG_Timers
{
    BG_DG_FLAG_CAPTURING_TIME   = 60000,
    BG_DG_CART_RESPAWN_TIME     = 23000,
    BG_DG_CART_DROP_TIME        = 10000
};

enum BG_DG_ObjectTypes
{
    // for all 3 node points 4 * 3 = 12 objects
    BG_DG_OBJECT_AURA_ALLY          = 0,
    BG_DG_OBJECT_AURA_HORDE         = 1,
    BG_DG_OBJECT_AURA_CONTESTED     = 2,
    BG_DG_OBJECT_PJ_COLLISION       = 3,
    //Carts
    BG_DG_OBJECT_CART_ALLIANCE      = 12,
    BG_DG_OBJECT_CART_HORDE         = 13,
    //gates
    BG_DG_OBJECT_GATE_1             = 14,
    BG_DG_OBJECT_GATE_2             = 15,
    BG_DG_OBJECT_GATE_3             = 16,
    BG_DG_OBJECT_GATE_4             = 17,
    //Buffs
    BG_DG_OBJECT_BUFF_NORTH        = 18,
    BG_DG_OBJECT_BUFF_SOUTH        = 19,
    BG_DG_OBJECT_BUFF_EAST         = 20,
    BG_DG_OBJECT_BUFF_WEST         = 21,
    //Carts ground
    BG_DG_OBJECT_CART_ALLY_GROUND  = 22,
    BG_DG_OBJECT_CART_HORDE_GROUND = 23,
    //total
    BG_DG_OBJECT_MAX               = 24
};

const uint32 BG_DG_TickIntervals[BG_DG_ALL_NODES_COUNT + 1] = { 0, 12000, 6000, 1000 };
const int32 BG_DG_TickPoints[BG_DG_ALL_NODES_COUNT + 1]     = { 0, 10,    10,   30   };

enum BG_DG_GraveyardsIds
{
    BG_DG_GRAVEYARD_NORTHERN    = 0,
    BG_DG_GRAVEYARD_SOUTHERN    = 1,
    BG_DG_MAX_GRAVEYARD         = 2
};

const uint32 BG_DG_GraveyardIds[MS::Battlegrounds::TeamsCount::Value][BG_DG_MAX_GRAVEYARD] =
{
    { 4546, 4488 },
    { 4489, 4545 }
};

const float BG_DG_CartPositions[2][4] =
{
    //      x,      y,          z,          o
    { -241.7413f, 208.6111f, 133.7474f, 0.8427795f },   // Alliance
    { -91.61632f, 791.3611f, 133.7473f, 4.0235610f }    // Horde
};

const float BG_DG_CollisionPJPositions[BG_DG_ALL_NODES_COUNT][4] =
{
    { -398.1615f, 574.5243f, 111.5271f, 0.0f },          // Goblin mine
    { -167.3698f, 499.1858f, 92.83675f, 0.0f },          // Central mine
    { 68.421880f, 431.1945f, 111.7609f, 0.0f }           // Pandaren mine
};

const float BG_DG_NodePositions[BG_DG_ALL_NODES_COUNT][4] =
{
    { -397.7726f, 574.3680f, 111.0529f, 1.437866f },    // Goblin mine
    { -167.5035f, 499.0590f, 92.62903f, 1.249549f },    // Central mine
    { 68.392360f, 431.1771f, 111.4928f, 1.229253f }     // Pandaren mine
};

const float BG_DG_AuraPositions[BG_DG_ALL_NODES_COUNT][4] =
{
    { -397.7730f, 574.3680f, 110.9700f, 6.245294f },    // Goblin mine
    { -167.5035f, 499.0590f, 92.83793f, 6.245294f },    // Central mine
    {  68.04340f, 430.7587f, 111.7523f, 0.1778332f }    // Pandaren mine
};

#define MAX_BUFFS 4

const float BG_DG_BuffPositions[MAX_BUFFS][4] =
{
    {  96.05209f, 426.0920f, 111.1858f, 4.059507f },    // Nourrish
    { -93.55556f, 375.3403f, 135.5808f, 5.516745f },    // Berserk
    { -428.2292f, 581.0121f, 110.9582f, 6.058313f },    // Nourrish
    { -239.4358f, 624.6111f, 135.6253f, 2.562323f }     // Berserk
};

class BattlegroundDGScore : public BattlegroundScore
{
    public:
        BattlegroundDGScore() :
            BattlegroundScore(), m_CapturedCart(0), m_ReturnedCart(0), m_AssaultedMines(0), m_DefendedMines(0) { }

        virtual ~BattlegroundDGScore() { }

        uint32 m_CapturedCart;
        uint32 m_ReturnedCart;

        uint32 m_AssaultedMines;
        uint32 m_DefendedMines;
};

#define BG_DG_NotBGWeekendHonorTicks      260
#define BG_DG_BBGWeekendHonorTicks        200

class BattlegroundDG : public Battleground
{
    public:
        BattlegroundDG();
        ~BattlegroundDG();

        void Reset();
        bool SetupBattleground();
        void EndBattleground(uint32 winner);
        void StartingEventOpenDoors();
        void StartingEventCloseDoors();
        void FillInitialWorldStates(ByteBuffer& data);
        void PostUpdateImpl(uint32 diff);
        void HandleAreaTrigger(Player* player, uint32 trigger);
        bool CanSeeSpellClick(Player const* player, Unit const* clicked);
        void EventPlayerClickedOnFlag(Player* source, GameObject* target_obj);
        void EventPlayerClickedOnFlag(Player* source, Unit* target);
        void UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor = true, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType = MS::Battlegrounds::RewardCurrencyType::Type::None);
        void AddPlayer(Player* player);
        void RemovePlayer(Player* player, uint64 guid, uint32 team);
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);
        void HandleKillPlayer(Player* player, Player* killer);
        void EventPlayerDroppedFlag(Player* player);

        uint32 GetTeamScore(uint32 p_TeamID) const { return m_TeamScores[GetTeamIndexByTeamId(p_TeamID)]; }
        uint32 GetMaxScore() const { return BG_DG_MAX_VICTORY_POINTS; }
        bool IsScoreIncremental() const { return true; }

    private:
        uint8 GetFlagState(uint32 team)             { return _flagState[GetTeamIndexByTeamId(team)]; };

        uint64 GetFlagPickerGUID(int32 p_Team) const
        {
            if (p_Team != TEAM_ALLIANCE && p_Team != TEAM_HORDE)
                return 0;

            return m_FlagKeepers[p_Team];
        }

        std::set<uint64> const GetFlagPickersGUID(int32 p_Team) const
        {
            if (p_Team != TEAM_ALLIANCE && p_Team != TEAM_HORDE)
                return std::set<uint64>();

            std::set<uint64> l_FlagPickers { m_FlagKeepers[p_Team] };
            return l_FlagPickers;
        }

        void SetAllianceCartPicker(uint64 guid)     { m_FlagKeepers[TEAM_ALLIANCE] = guid; };
        void SetHordeFlagPicker(uint64 guid)        { m_FlagKeepers[TEAM_HORDE] = guid; };
        bool IsAllianceFlagPickedup() const         { return m_FlagKeepers[TEAM_ALLIANCE] != 0; };
        bool IsHordeFlagPickedup() const            { return m_FlagKeepers[TEAM_HORDE] != 0; };
        void SetLastFlagCapture(uint32 team)        { _lastFlagCaptureTeam = team; };

        void _UpdateTeamScore(int team, int32 value);
        void UpdateCartState(uint32 team, uint32 value);
        void _postUpdateImpl_Flags(uint32 diff);
        void _postUpdateImpl_Cart(uint32 diff);
        void RespawnFlagAfterDrop(uint32 team);
        void EventPlayerCapturedFlag(Player* player);
        int32 _GetNodeNameId(uint8 node);
        void _contestedTime(int node, uint32 diff);
        /* Gameobject spawning/despawning */
        void _ChangeBanner(uint8 node, uint8 type);
        void _SendNodeUpdate(uint8 node);

    private:
        /************************************************************************/
        /*                           Nodes info:                                */
        /************************************************************************/
        /*                      0: neutral                                      */
        /*                      1: ally contested                               */
        /*                      2: horde contested                              */
        /*                      3: ally occupied                                */
        /*                      4: horde occupied                               */
        /************************************************************************/  
        uint8               m_Nodes[BG_DG_ALL_NODES_COUNT];
        uint8               m_prevNodes[BG_DG_ALL_NODES_COUNT];
        uint32              m_NodeTimers[BG_DG_ALL_NODES_COUNT];
        uint32              m_lastTick[MS::Battlegrounds::TeamsCount::Value];
        uint32              m_HonorScoreTics[MS::Battlegrounds::TeamsCount::Value];
        bool                m_IsInformedNearVictory;
        uint32              m_HonorTics;

        uint32 _lastFlagCaptureTeam;                       // Winner is based on this if score is equal
        uint64 m_FlagKeepers[2];                            // 0 - alliance, 1 - horde
        uint8 _flagState[2];                               // for checking flag state
        int32 _flagGold[2];
        int32 _flagsTimer[2];
        int32 _flagsDropTimer[2];
};

#endif
