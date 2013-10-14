/*
 * Copyright (C) 2005-2011 MaNGOS <http://www.getmangos.com/>
 *
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __BattlegroundBFG_H
#define __BattlegroundBFG_H

class Battleground;

#define SPELL_ROOT_BEGIN                75215

const float BG_BFG_NodeObjectId[3] = {205557,208785,208782};

enum BG_BFG_ObjectType
{
    BG_BFG_OBJECT_BANNER_NEUTRAL          = 0,
    BG_BFG_OBJECT_BANNER_CONT_A           = 1,
    BG_BFG_OBJECT_BANNER_CONT_H           = 2,
    BG_BFG_OBJECT_BANNER_ALLY             = 3,
    BG_BFG_OBJECT_BANNER_HORDE            = 4,
    BG_BFG_OBJECT_AURA_ALLY               = 5,
    BG_BFG_OBJECT_AURA_HORDE              = 6,
    BG_BFG_OBJECT_AURA_CONTESTED          = 7,
    BG_BFG_OBJECT_GATE_A                  = 24,
    BG_BFG_OBJECT_GATE_H                  = 25,
    BG_BFG_OBJECT_MAX                     = 26,
};

/* Object id templates from DB */
enum BG_BFG_ObjectTypes
{
    BG_BFG_OBJECTID_BANNER_A             = 180058,
    BG_BFG_OBJECTID_BANNER_CONT_A        = 180059,
    BG_BFG_OBJECTID_BANNER_H             = 180060,
    BG_BFG_OBJECTID_BANNER_CONT_H        = 180061,
    
    BG_BFG_OBJECTID_AURA_A               = 180100,
    BG_BFG_OBJECTID_AURA_H               = 180101,
    BG_BFG_OBJECTID_AURA_C               = 180102,

    BG_BFG_OBJECTID_GATE_A               = 205496,
    BG_BFG_OBJECTID_GATE_H               = 207178
};

enum BG_BFG_Timers
{
    BG_BFG_FLAG_CAPTURING_TIME   = 60000,
};

enum BG_BFG_Score
{
    BG_BFG_WARNING_NEAR_VICTORY_SCORE    = 1800,
    BG_BFG_MAX_TEAM_SCORE                = 2000
};

/* do NOT change the order, else wrong behaviour */
enum BG_BFG_BattlegroundNodes
{
    BG_BFG_NODE_LIGHTHOUSE       = 0,
    BG_BFG_NODE_WATERWORKS       = 1,
    BG_BFG_NODE_MINE             = 2,

    BG_BFG_DYNAMIC_NODES_COUNT   = 3,                        // dynamic nodes that can be captured

    BG_BFG_SPIRIT_ALIANCE        = 3,
    BG_BFG_SPIRIT_HORDE          = 4,

    BG_BFG_ALL_NODES_COUNT       = 5,                        // all nodes (dynamic and static)
};

enum BG_BFG_NodeStatus
{
    BG_BFG_NODE_TYPE_NEUTRAL             = 0,
    BG_BFG_NODE_TYPE_CONTESTED           = 1,
    BG_BFG_NODE_STATUS_ALLY_CONTESTED    = 1,
    BG_BFG_NODE_STATUS_HORDE_CONTESTED   = 2,
    BG_BFG_NODE_TYPE_OCCUPIED            = 3,
    BG_BFG_NODE_STATUS_ALLY_OCCUPIED     = 3,
    BG_BFG_NODE_STATUS_HORDE_OCCUPIED    = 4
};

enum BG_BFG_Sounds
{
    BG_BFG_SOUND_NODE_CLAIMED            = 8192,
    BG_BFG_SOUND_NODE_CAPTURED_ALLIANCE  = 8173,
    BG_BFG_SOUND_NODE_CAPTURED_HORDE     = 8213,
    BG_BFG_SOUND_NODE_ASSAULTED_ALLIANCE = 8212,
    BG_BFG_SOUND_NODE_ASSAULTED_HORDE    = 8174,
    BG_BFG_SOUND_NEAR_VICTORY            = 8456
};

enum BG_BFG_Objectives
{
    BG_OBJECTIVE_ASSAULT_BASE           = 122,
    BG_OBJECTIVE_DEFEND_BASE            = 123
};

const float BG_BFG_SpiritGuidePos[5][4] = {
    {1252.22f,  836.54f,    27.78f, 0}, //CB Gilnéas - Cimetière (H - cent.) 1735
    {1034.81f,  1335.57f,   12.0f,  0}, //CB Gilnéas - Cimetière (A - cent.) 1736
    {887.57f,   937.33f,    23.77f, 0}, //CB Gilnéas - Cimetière (cent.) 1738
    {908.27f,   1338.59f,   227.64f,0}, //CB Gilnéas - Cimetière (zone de départ Alliance) 1740
    {1401.38f,  977.12f,    7.44f,  0}  //CB Gilnéas - Cimetière (zone de départ Horde) 1739
};

const float BG_BFG_NodePositions[3][4] = {
    {1057.86f,  1278.28f, 3.13f,    5.06f}, // Lighthouse
    {980.08f,   948.707f, 12.7478f, 5.92f}, // waterworks
    {1250.99f,  958.311f, 5.66513f, 5.93f}  // mine
};

// x, y, z, o, rot0, rot1, rot2, rot3
const float BG_BFG_DoorPositions[2][8] = {
    {918.876f, 1336.56f, 27.6195f, 2.77481f, 0.0f, 0.0f, 0.983231f, 0.182367f},
    {1396.15f, 977.014f, 7.43169f, 6.27043f, 0.0f, 0.0f, 0.006378f, -0.99998f}
};

const uint32 BG_BFG_TickIntervals[4] = {0, 12000, 6000, 1000};
const uint32 BG_BFG_TickPoints[4] = {0, 10, 10, 30};

// WorldSafeLocs ids for 3 nodes, and for ally, and horde starting location
const uint32 BG_BFG_GraveyardIds[BG_BFG_ALL_NODES_COUNT] = {1735, 1736, 1738, 1740, 1739};

enum BG_BFG_WorldStates
{
    //ok.
    BG_BFG_OP_OCCUPIED_BASES_HORDE       = 1778,
    BG_BFG_OP_OCCUPIED_BASES_ALLY        = 1779,
    BG_BFG_OP_RESOURCES_ALLY             = 1776,
    BG_BFG_OP_RESOURCES_HORDE            = 1777,
    BG_BFG_OP_RESOURCES_MAX              = 1780,
    BG_BFG_OP_RESOURCES_WARNING          = 1955
};

//ok.
const uint32 BG_BFG_OP_NODESTATES[3] =    {1767, 1782, 1772};

const uint32 BG_BFG_OP_NODEICONS[3]  =    {1842, 1846, 1845};

struct BG_BFG_BannerTimer
{
    uint32      timer;
    uint8       type;
    uint8       teamIndex;
};

class BattlegroundBFGScore : public BattlegroundScore
{
    public:
        BattlegroundBFGScore(): BasesAssaulted(0), BasesDefended(0) {};
        virtual ~BattlegroundBFGScore() {};
        uint32 BasesAssaulted;
        uint32 BasesDefended;
};

class BattlegroundBFG : public Battleground
{
    friend class BattlegroundMgr;

public:
    BattlegroundBFG();
    ~BattlegroundBFG();

    void PostUpdateImpl(uint32 diff);
    void AddPlayer(Player *plr);
    virtual void StartingEventCloseDoors();
    virtual void StartingEventOpenDoors();
    void RemovePlayer(Player *plr,uint64 guid);
    void HandleAreaTrigger(Player *Source, uint32 Trigger);
    virtual bool SetupBattleground();
    virtual void Reset();
    void EndBattleground(uint32 winner);
    virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

    /* Scorekeeping */
    virtual void UpdatePlayerScore(Player *Source, uint32 type, uint32 value, bool doAddHonor = true);

    virtual void FillInitialWorldStates(WorldPacket& data);

    /* Nodes occupying */
    virtual void EventPlayerClickedOnFlag(Player *source, GameObject* target_obj);

    /* achievement req. */
    bool IsAllNodesConrolledByTeam(uint32 team) const;  // overwrited
    bool IsTeamScores500Disadvantage(uint32 team) const { return m_TeamScores500Disadvantage[GetTeamIndexByTeamId(team)]; }
private:
    /* Gameobject spawning/despawning */
    void _CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay);
    void _DelBanner(uint8 node, uint8 type, uint8 teamIndex);
    void _SendNodeUpdate(uint8 node);

    /* Creature spawning/despawning */
    // TODO: working, scripted peons spawning
    void _NodeOccupied(uint8 node,Team team);
    void _NodeDeOccupied(uint8 node);

    int32 _GetNodeNameId(uint8 node);

    /* Nodes info:
     0: neutral
     1: ally contested
     2: horde contested
     3: ally occupied
     4: horde occupied     */
    uint8               m_Nodes[BG_BFG_DYNAMIC_NODES_COUNT];
    uint8               m_prevNodes[BG_BFG_DYNAMIC_NODES_COUNT];
    BG_BFG_BannerTimer  m_BannerTimers[BG_BFG_DYNAMIC_NODES_COUNT];
    uint32              m_NodeTimers[BG_BFG_DYNAMIC_NODES_COUNT];
    uint32              m_lastTick[BG_TEAMS_COUNT];
    uint32              m_HonorScoreTics[BG_TEAMS_COUNT];
    uint32              m_ReputationScoreTics[BG_TEAMS_COUNT];
    bool                m_IsInformedNearVictory;
    uint32              m_HonorTics;
    uint32              m_ReputationTics;
    // need for achievements
    bool                m_TeamScores500Disadvantage[BG_TEAMS_COUNT];
};
#endif
