////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "BattlegroundDG.h"
#include "Player.h"
#include "Language.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "ScriptedCreature.h"
#include "BattlegroundMgr.hpp"

/************************************************************************/
/*                     STARTING BATTLEGROUND                            */
/************************************************************************/
BattlegroundDG::BattlegroundDG() : Battleground()
{
    m_IsInformedNearVictory = false;
    m_BuffChange = true;

    BgObjects.resize(BG_DG_OBJECT_MAX);
    BgCreatures.resize(BG_DG_CREATURES_MAX);

    bool l_IsBGWeekEnd = sBattlegroundMgr->IsBGWeekend(GetTypeID());
    m_HonorTics = (l_IsBGWeekEnd) ? BG_DG_BBGWeekendHonorTicks : BG_DG_NotBGWeekendHonorTicks;

    StartMessageIds[BG_STARTING_EVENT_FIRST]    = LANG_BG_DG_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND]   = LANG_BG_DG_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]    = LANG_BG_DG_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH]   = LANG_BG_DG_HAS_BEGUN;
}

bool BattlegroundDG::SetupBattleground()
{
    // Nodes related
    for (int i = 0; i < BG_DG_ALL_NODES_COUNT; ++i)
    {
        if (!AddCreature(BG_DG_OBJECTID_CAPT_POINT, BG_DG_OBJECT_CAPT_POINT_START + i, TEAM_NEUTRAL, BG_DG_NodePositions[i][0], BG_DG_NodePositions[i][1], BG_DG_NodePositions[i][2], BG_DG_NodePositions[i][3], RESPAWN_IMMEDIATELY))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "BatteGroundDG: Failed to spawn flags PNJ. Battleground not created!");
            return false;
        }
        if (!AddObject(BG_DG_OBJECT_AURA_ALLY + BG_DG_OBJECT_DYNAMIC_TOTAL * i, BG_DG_OBJECTID_AURA_A, BG_DG_AuraPositions[i][0], BG_DG_AuraPositions[i][1], BG_DG_AuraPositions[i][2], BG_DG_AuraPositions[i][3], 0, 0, std::sin(BG_DG_AuraPositions[i][3] / 2), std::cos(BG_DG_AuraPositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_DG_OBJECT_AURA_HORDE + BG_DG_OBJECT_DYNAMIC_TOTAL * i, BG_DG_OBJECTID_AURA_H, BG_DG_AuraPositions[i][0], BG_DG_AuraPositions[i][1], BG_DG_AuraPositions[i][2], BG_DG_AuraPositions[i][3], 0, 0, std::sin(BG_DG_AuraPositions[i][3] / 2), std::cos(BG_DG_AuraPositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_DG_OBJECT_AURA_CONTESTED + BG_DG_OBJECT_DYNAMIC_TOTAL * i, BG_DG_OBJECTID_AURA_C, BG_DG_AuraPositions[i][0], BG_DG_AuraPositions[i][1], BG_DG_AuraPositions[i][2], BG_DG_AuraPositions[i][3], 0, 0, std::sin(BG_DG_AuraPositions[i][3] / 2), std::cos(BG_DG_AuraPositions[i][3] / 2), RESPAWN_ONE_DAY)
            )
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "BatteGroundDG: Failed to spawn flags aura objects. Battleground not created!");
            return false;
        }
        if (!AddObject(BG_DG_OBJECT_PJ_COLLISION + BG_DG_OBJECT_DYNAMIC_TOTAL * i, BG_DG_OBJECTID_PJ_COLLISION, BG_DG_CollisionPJPositions[i][0], BG_DG_CollisionPJPositions[i][1], BG_DG_CollisionPJPositions[i][2], BG_DG_CollisionPJPositions[i][3], 0, 0, std::sin(BG_DG_CollisionPJPositions[i][3] / 2), std::cos(BG_DG_CollisionPJPositions[i][3] / 2), RESPAWN_ONE_DAY))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "BatteGroundDG: Failed to spawn flags collision objects. Battleground not created!");
            return false;
        }
    }
    // Carts
    if (!AddObject(BG_DG_OBJECT_CART_ALLIANCE, BG_DG_OBJECTID_CART_ALLY, BG_DG_CartPositions[0][0], BG_DG_CartPositions[0][1], BG_DG_CartPositions[0][2], BG_DG_CartPositions[0][3], 0, 0, std::sin(BG_DG_CartPositions[0][3] / 2), std::cos(BG_DG_CartPositions[0][3] / 2), BG_DG_CART_RESPAWN_TIME / 1000)
        || !AddObject(BG_DG_OBJECT_CART_HORDE, BG_DG_OBJECTID_CART_HORDE, BG_DG_CartPositions[1][0], BG_DG_CartPositions[1][1], BG_DG_CartPositions[1][2], BG_DG_CartPositions[1][3], 0, 0, std::sin(BG_DG_CartPositions[1][3] / 2), std::cos(BG_DG_CartPositions[1][3] / 2), BG_DG_CART_RESPAWN_TIME / 1000)
        )
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BatteGroundDG: Failed to spawn carts. Battleground not created!");
        return false;
    }
    // Doors
    if (!AddObject(BG_DG_OBJECT_GATE_1, BG_DG_OBJECTID_GATE, BG_DG_DoorPositions[0][0], BG_DG_DoorPositions[0][1], BG_DG_DoorPositions[0][2], BG_DG_DoorPositions[0][3], BG_DG_DoorPositions[0][4], BG_DG_DoorPositions[0][5], BG_DG_DoorPositions[0][6], BG_DG_DoorPositions[0][7], RESPAWN_IMMEDIATELY)
        || !AddObject(BG_DG_OBJECT_GATE_2, BG_DG_OBJECTID_GATE, BG_DG_DoorPositions[1][0], BG_DG_DoorPositions[1][1], BG_DG_DoorPositions[1][2], BG_DG_DoorPositions[1][3], BG_DG_DoorPositions[1][4], BG_DG_DoorPositions[1][5], BG_DG_DoorPositions[1][6], BG_DG_DoorPositions[1][7], RESPAWN_IMMEDIATELY)
        || !AddObject(BG_DG_OBJECT_GATE_3, BG_DG_OBJECTID_GATE, BG_DG_DoorPositions[2][0], BG_DG_DoorPositions[2][1], BG_DG_DoorPositions[2][2], BG_DG_DoorPositions[2][3], BG_DG_DoorPositions[2][4], BG_DG_DoorPositions[2][5], BG_DG_DoorPositions[2][6], BG_DG_DoorPositions[2][7], RESPAWN_IMMEDIATELY)
        || !AddObject(BG_DG_OBJECT_GATE_4, BG_DG_OBJECTID_GATE, BG_DG_DoorPositions[3][0], BG_DG_DoorPositions[3][1], BG_DG_DoorPositions[3][2], BG_DG_DoorPositions[3][3], BG_DG_DoorPositions[3][4], BG_DG_DoorPositions[3][5], BG_DG_DoorPositions[3][6], BG_DG_DoorPositions[3][7], RESPAWN_IMMEDIATELY)
        )
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BatteGroundDG: Failed to spawn door object. Battleground not created!");
        return false;
    }
    // Buffs
    for (int i = 0; i < MAX_BUFFS; i++)
    {
        if (!AddObject(BG_DG_OBJECT_BUFF_NORTH + i, Buff_Entries[urand(0, 2)], BG_DG_BuffPositions[i][0], BG_DG_BuffPositions[i][1], BG_DG_BuffPositions[i][2], BG_DG_BuffPositions[i][3], 0, 0, std::sin(BG_DG_BuffPositions[i][3] / 2), std::cos(BG_DG_BuffPositions[i][3] / 2), BUFF_RESPAWN_TIME))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "BatteGroundDG: Failed to spawn buff object. Battleground not created!");
            return false;
        }
    }

    WorldSafeLocsEntry const* l_SafeLoc = sWorldSafeLocsStore.LookupEntry(BG_DG_GraveyardIds[TEAM_ALLIANCE][BG_DG_GRAVEYARD_NORTHERN]);
    if (!l_SafeLoc || !AddSpiritGuide(BG_DG_SPIRIT_NORTHERN_ALLIANCE, l_SafeLoc->x, l_SafeLoc->y, l_SafeLoc->z, l_SafeLoc->o + M_PI, ALLIANCE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundDG: Failed to spawn Alliance-Northern spirit guide! Battleground not created!");
        return false;
    }

    l_SafeLoc = sWorldSafeLocsStore.LookupEntry(BG_DG_GraveyardIds[TEAM_ALLIANCE][BG_DG_GRAVEYARD_SOUTHERN]);
    if (!l_SafeLoc || !AddSpiritGuide(BG_DG_SPIRIT_SOUTHERN_ALLIANCE, l_SafeLoc->x, l_SafeLoc->y, l_SafeLoc->z, l_SafeLoc->o + M_PI, ALLIANCE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundDG: Failed to spawn Alliance-Southern spirit guide! Battleground not created!");
        return false;
    }

    l_SafeLoc = sWorldSafeLocsStore.LookupEntry(BG_DG_GraveyardIds[TEAM_HORDE][BG_DG_GRAVEYARD_NORTHERN]);
    if (!l_SafeLoc || !AddSpiritGuide(BG_DG_SPIRIT_NORTHERN_HORDE, l_SafeLoc->x, l_SafeLoc->y, l_SafeLoc->z, l_SafeLoc->o + M_PI, HORDE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundDG: Failed to spawn Horde-Northern spirit guide! Battleground not created!");
        return false;
    }

    l_SafeLoc = sWorldSafeLocsStore.LookupEntry(BG_DG_GraveyardIds[TEAM_HORDE][BG_DG_GRAVEYARD_SOUTHERN]);
    if (!l_SafeLoc || !AddSpiritGuide(BG_DG_SPIRIT_SOUTHERN_HORDE, l_SafeLoc->x, l_SafeLoc->y, l_SafeLoc->z, l_SafeLoc->o + M_PI, HORDE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundDG: Failed to spawn Horde-Southern spirit guide! Battleground not created!");
        return false;
    }

    return true;
}

void BattlegroundDG::Reset()
{
    // Call parent's class reset
    Battleground::Reset();

    m_TeamScores[TEAM_ALLIANCE]     = 0;
    m_TeamScores[TEAM_HORDE]        = 0;
    m_lastTick[TEAM_ALLIANCE]       = 0;
    m_lastTick[TEAM_HORDE]          = 0;
    m_HonorScoreTics[TEAM_ALLIANCE] = 0;
    m_HonorScoreTics[TEAM_HORDE]    = 0;
    m_IsInformedNearVictory         = false;

    for (uint8 i = 0; i < BG_DG_ALL_NODES_COUNT; ++i)
    {
        m_Nodes[i]      = BG_DG_NODE_TYPE_NEUTRAL;
        m_prevNodes[i]  = BG_DG_NODE_TYPE_NEUTRAL;
        m_NodeTimers[i] = 0;
    }

    m_FlagKeepers[TEAM_ALLIANCE]    = 0;
    m_FlagKeepers[TEAM_HORDE]       = 0;
    _flagState[TEAM_ALLIANCE]       = BG_DG_CART_STATE_ON_BASE;
    _flagState[TEAM_HORDE]          = BG_DG_CART_STATE_ON_BASE;
    _flagGold[TEAM_ALLIANCE]        = 0;
    _flagGold[TEAM_HORDE]           = 0;
    m_TeamScores[TEAM_ALLIANCE]     = 0;
    m_TeamScores[TEAM_HORDE]        = 0;
    _lastFlagCaptureTeam            = 0;
    _flagsDropTimer[TEAM_ALLIANCE]  = 0;
    _flagsDropTimer[TEAM_HORDE]     = 0;
    _flagsTimer[TEAM_ALLIANCE]      = 0;
    _flagsTimer[TEAM_HORDE]         = 0;
}

void BattlegroundDG::StartingEventCloseDoors()
{
    // Despawn everything
    for (int l_Iter = BG_DG_OBJECT_AURA_ALLY; l_Iter < BG_DG_OBJECT_MAX; ++l_Iter)
        SpawnBGObject(l_Iter, RESPAWN_ONE_DAY);

    for (int l_Iter = BG_DG_NODE_GOBLIN_MINE; l_Iter < BG_DG_ALL_NODES_COUNT; ++l_Iter)
    {
        // Spawn collision PJ
        SpawnBGObject((l_Iter * BG_DG_OBJECT_DYNAMIC_TOTAL) + BG_DG_OBJECT_PJ_COLLISION, RESPAWN_IMMEDIATELY);
        // Spawn neutral aura
        SpawnBGObject((l_Iter * BG_DG_OBJECT_DYNAMIC_TOTAL) + BG_DG_OBJECT_AURA_CONTESTED, RESPAWN_IMMEDIATELY);
        // Add capt points auras
        if (Creature* l_CapturePoint = GetBgMap()->GetCreature(BgCreatures[BG_DG_OBJECT_CAPT_POINT_START + l_Iter]))
            l_CapturePoint->CastSpell(l_CapturePoint, BG_DG_CAPT_POINT_NEUTRAL, true);
    }

    // Spawn carts
    for (uint32 l_Iter = BG_DG_OBJECT_CART_ALLIANCE; l_Iter <= BG_DG_OBJECT_CART_HORDE; ++l_Iter)
        SpawnBGObject(l_Iter, RESPAWN_IMMEDIATELY);

    // Spawn Buffs
    for (uint32 l_Iter = 0; l_Iter < MAX_BUFFS; l_Iter++)
        SpawnBGObject(BG_DG_OBJECT_BUFF_NORTH + l_Iter, RESPAWN_IMMEDIATELY);

    // Starting doors
    for (int l_Iter = BG_DG_OBJECT_GATE_1; l_Iter <= BG_DG_OBJECT_GATE_4; ++l_Iter)
    {
        DoorClose(l_Iter);
        SpawnBGObject(l_Iter, RESPAWN_IMMEDIATELY);
    }
}

void BattlegroundDG::StartingEventOpenDoors()
{
    // Open doors
    for (int l_Iter = BG_DG_OBJECT_GATE_1; l_Iter <= BG_DG_OBJECT_GATE_4; ++l_Iter)
        DoorOpen(l_Iter);

    // Force update of nodes
    for (int l_Iter = 0; l_Iter < BG_DG_ALL_NODES_COUNT; ++l_Iter)
    {
        if (Creature* l_CapturePoint = GetBgMap()->GetCreature(BgCreatures[BG_DG_OBJECT_CAPT_POINT_START + l_Iter]))
        {
            for (BattlegroundPlayerMap::iterator l_Itr = m_Players.begin(); l_Itr != m_Players.end(); ++l_Itr)
            {
                if (Player* l_Player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(l_Itr->first, 0, HIGHGUID_PLAYER)))
                    l_CapturePoint->SendUpdateToPlayer(l_Player);
            }
        }
    }
}

/************************************************************************/
/*                           CORE UPDATES                               */
/************************************************************************/
void BattlegroundDG::AddPlayer(Player* p_Player)
{
    Battleground::AddPlayer(p_Player);
    BattlegroundDGScore* l_BGScore      = new BattlegroundDGScore();
    PlayerScores[p_Player->GetGUID()]   = l_BGScore;
}

void BattlegroundDG::PostUpdateImpl(uint32 p_Diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        _postUpdateImpl_Flags(p_Diff);
        _postUpdateImpl_Cart(p_Diff);
    }
}

void BattlegroundDG::FillInitialWorldStates(ByteBuffer& p_Data)
{
    for (uint8 l_Node = 0; l_Node < BG_DG_ALL_NODES_COUNT; ++l_Node)
    {
        for (uint8 l_Status = BG_DG_NODE_TYPE_NEUTRAL; l_Status <= BG_DG_NODE_STATUS_HORDE_OCCUPIED; l_Status++)
        {
            if (m_Nodes[l_Node] == l_Status)
                p_Data << uint32(BG_DG_NodesWs[l_Node][l_Status].m_WorldState) << uint32(BG_DG_NodesWs[l_Node][l_Status].m_SentValueToShow);
            else if (l_Status >= BG_DG_NODE_TYPE_OCCUPIED)
            {
                // This is there because the occupied worldstate is the same for ally & horde, so we don't want to worldstate that erase the good value
                // If the node is correct status, send the value, else if status not occupied send 0 (to hide)
                // BUT IF ITS OCCUPED, SEND 0 ONLY IF THE NODE ITSELF ISNT OCCUPED
                if (m_Nodes[l_Node] < BG_DG_NODE_TYPE_OCCUPIED)
                    p_Data << uint32(BG_DG_NodesWs[l_Node][l_Status].m_WorldState) << uint32(0);
            }
            else
                p_Data << uint32(BG_DG_NodesWs[l_Node][l_Status].m_WorldState) << uint32(0);
        }
    }

    // How many bases each team owns
    uint8 l_Ally = 0, l_Horde = 0;
    for (uint8 l_Node = 0; l_Node < BG_DG_ALL_NODES_COUNT; ++l_Node)
    {
        if (m_Nodes[l_Node] == BG_DG_NODE_STATUS_ALLY_OCCUPIED)
            ++l_Ally;
        else if (m_Nodes[l_Node] == BG_DG_NODE_STATUS_HORDE_OCCUPIED)
            ++l_Horde;
    }

    p_Data << uint32(WORLDSTATE_DG_OCCUPIED_BASES_ALLIANCE) << uint32(l_Ally);
    p_Data << uint32(WORLDSTATE_DG_OCCUPIED_BASES_HORDE) << uint32(l_Horde);

    // Team Score
    p_Data << uint32(WORLDSTATE_DG_SCORE_ALLIANCE) << uint32(m_TeamScores[TEAM_ALLIANCE]);
    p_Data << uint32(WORLDSTATE_DG_SCORE_HORDE) << uint32(m_TeamScores[TEAM_HORDE]);

    // Cart state
    p_Data << uint32(WORLDSTATE_DG_CART_STATE_ALLIANCE) << uint32((_flagState[TEAM_ALLIANCE] == BG_DG_CART_STATE_ON_PLAYER) ? 2 : 1);
    p_Data << uint32(WORLDSTATE_DG_CART_STATE_HORDE) << uint32((_flagState[TEAM_HORDE] == BG_DG_CART_STATE_ON_PLAYER) ? 2 : 1);
}

void BattlegroundDG::UpdatePlayerScore(Player* p_Source, uint32 p_Type, uint32 p_Value, bool p_AddHonor)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    BattlegroundScoreMap::iterator l_Iter = PlayerScores.find(p_Source->GetGUID());
    if (l_Iter == PlayerScores.end())
        return;

    uint32 l_TeamIndex = GetTeamIndexByTeamId(p_Source->GetBGTeam()); ///< l_TeamIndex is never read 01/18/16
    switch (p_Type)
    {
        case SCORE_CART_CAPTURES:
            ((BattlegroundDGScore*)l_Iter->second)->m_CapturedCart += p_Value;
            break;
        case SCORE_CART_RETURNS:
            ((BattlegroundDGScore*)l_Iter->second)->m_ReturnedCart += p_Value;
            break;
        case SCORE_BASES_ASSAULTED:
            ((BattlegroundDGScore*)l_Iter->second)->m_AssaultedMines += p_Value;
            break;
        case SCORE_BASES_DEFENDED:
            ((BattlegroundDGScore*)l_Iter->second)->m_DefendedMines += p_Value;
            break;
        default:
            Battleground::UpdatePlayerScore(p_Source, NULL, p_Type, p_Value, p_AddHonor);
            break;
    }
}

void BattlegroundDG::HandleKillPlayer(Player* p_Player, Player* p_Killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    EventPlayerDroppedFlag(p_Player);

    Battleground::HandleKillPlayer(p_Player, p_Killer);
}

WorldSafeLocsEntry const* BattlegroundDG::GetClosestGraveYard(Player* p_Player)
{
    uint32 l_TeamIndex = GetTeamIndexByTeamId(p_Player->GetBGTeam());

    WorldSafeLocsEntry const* l_North = sWorldSafeLocsStore.LookupEntry(BG_DG_GraveyardIds[l_TeamIndex][0]);
    float l_NorthDist = p_Player->GetExactDist2dSq(l_North->x, l_North->y);
    WorldSafeLocsEntry const* l_South = sWorldSafeLocsStore.LookupEntry(BG_DG_GraveyardIds[l_TeamIndex][1]);
    float l_SouthDist = p_Player->GetExactDist2dSq(l_South->x, l_South->y);

    // If you are on the southern side of the map, respawn at northern (and vice versa)
    // http://us.battle.net/wow/en/game/pvp/battlegrounds/deepwind-gorge
    return l_NorthDist < l_SouthDist ? l_South : l_North;
}

void BattlegroundDG::_UpdateTeamScore(int p_Team, int32 p_Value)
{
    m_TeamScores[p_Team] += p_Value;
    m_TeamScores[p_Team] = std::min(int32(BG_DG_MAX_VICTORY_POINTS), m_TeamScores[p_Team]);
    m_TeamScores[p_Team] = std::max(0, m_TeamScores[p_Team]);

    UpdateWorldState(p_Team == TEAM_ALLIANCE ? WORLDSTATE_DG_SCORE_ALLIANCE : WORLDSTATE_DG_SCORE_HORDE, m_TeamScores[p_Team]);

    if (m_TeamScores[p_Team] == BG_DG_MAX_VICTORY_POINTS)
    {
        UpdateWorldState(WORLDSTATE_DG_CART_STATE_ALLIANCE, 1);
        UpdateWorldState(WORLDSTATE_DG_CART_STATE_HORDE, 1);
        EndBattleground(p_Team == TEAM_ALLIANCE ? ALLIANCE : HORDE);
    }

    if (m_IsInformedNearVictory && m_TeamScores[p_Team] < BG_DG_NEAR_VICTORY_POINTS)
        m_IsInformedNearVictory = false;
    else if (!m_IsInformedNearVictory && m_TeamScores[p_Team] > BG_DG_NEAR_VICTORY_POINTS)
    {
        SendMessageToAll(p_Team == TEAM_ALLIANCE ? LANG_BG_DG_ALLIANCE_NEAR_VICTORY : LANG_BG_TK_HORDE_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
        PlaySoundToAll((p_Team == TEAM_ALLIANCE) ? BG_DG_SOUND_ALLIANCE_NEAR_VICTORY : BG_DG_SOUND_HORDE_NEAR_VICTORY);
        m_IsInformedNearVictory = true;
    }
}

/************************************************************************/
/*                          FLAGS UPDATES                               */
/************************************************************************/
void BattlegroundDG::_ChangeBanner(uint8 p_Node, uint8 p_Type)
{
    // 1. Manage auras objects
    if (p_Type == BG_DG_NODE_TYPE_CONTESTED && (m_prevNodes[p_Node] == BG_DG_NODE_STATUS_ALLY_OCCUPIED || m_prevNodes[p_Node] == BG_DG_NODE_STATUS_HORDE_OCCUPIED))
    {
        // Capt point goes from occupied to contested
        // 1.1. Despawn controlled aura
        SpawnBGObject(p_Node * BG_DG_OBJECT_DYNAMIC_TOTAL + BG_DG_OBJECT_AURA_ALLY + (m_Nodes[p_Node] - 1), RESPAWN_ONE_DAY);
        // 1.2. Spawn contested aura
        SpawnBGObject(p_Node * BG_DG_OBJECT_DYNAMIC_TOTAL + BG_DG_OBJECT_AURA_CONTESTED, RESPAWN_IMMEDIATELY);
    }
    else if (p_Type == BG_DG_NODE_TYPE_OCCUPIED && (m_prevNodes[p_Node] == BG_DG_NODE_STATUS_ALLY_CONTESTED || m_prevNodes[p_Node] == BG_DG_NODE_STATUS_HORDE_CONTESTED))
    {
        // Capt point goes from contested to occupied
        // 1.1. Despawn contested aura
        SpawnBGObject(p_Node * BG_DG_OBJECT_DYNAMIC_TOTAL + BG_DG_OBJECT_AURA_CONTESTED, RESPAWN_ONE_DAY);
        // 1.2. Spawn controlled aura
        SpawnBGObject(p_Node * BG_DG_OBJECT_DYNAMIC_TOTAL + BG_DG_OBJECT_AURA_ALLY + (m_Nodes[p_Node] - 1), RESPAWN_IMMEDIATELY);
    }

    // 2. Manage the capt point
    if (Creature* l_CapturePoint = GetBgMap()->GetCreature(BgCreatures[BG_DG_OBJECT_CAPT_POINT_START + p_Node]))
    {
        // 2.1. Remove the previous aura
        uint32 l_RemoveAura = BG_DG_CAPT_POINT_NEUTRAL;
        switch (m_prevNodes[p_Node])
        {
            case BG_DG_NODE_STATUS_ALLY_CONTESTED:
                l_RemoveAura = BG_DG_CAPT_POINT_ALLIANCE_CONTEST;
                break;
            case BG_DG_NODE_STATUS_HORDE_CONTESTED:
                l_RemoveAura = BG_DG_CAPT_POINT_HORDE_CONTEST;
                break;
            case BG_DG_NODE_STATUS_ALLY_OCCUPIED:
                l_RemoveAura = BG_DG_CAPT_POINT_ALLIANCE_CONTROL;
                break;
            case BG_DG_NODE_STATUS_HORDE_OCCUPIED:
                l_RemoveAura = BG_DG_CAPT_POINT_HORDE_CONTROL;
                break;
            default:
                break;
        }

        l_CapturePoint->RemoveAura(l_RemoveAura);

        // 2.2. Add the new aura
        uint32 l_AddAura = BG_DG_CAPT_POINT_NEUTRAL;
        switch (m_Nodes[p_Node])
        {
            case BG_DG_NODE_STATUS_ALLY_CONTESTED:
                l_AddAura = BG_DG_CAPT_POINT_ALLIANCE_CONTEST;
                break;
            case BG_DG_NODE_STATUS_HORDE_CONTESTED:
                l_AddAura = BG_DG_CAPT_POINT_HORDE_CONTEST;
                break;
            case BG_DG_NODE_STATUS_ALLY_OCCUPIED:
                l_AddAura = BG_DG_CAPT_POINT_ALLIANCE_CONTROL;
                break;
            case BG_DG_NODE_STATUS_HORDE_OCCUPIED:
                l_AddAura = BG_DG_CAPT_POINT_HORDE_CONTROL;
                break;
            default:
                break;
        }

        l_CapturePoint->CastSpell(l_CapturePoint, l_AddAura, true);

        // 2.3 Force update to players
        for (BattlegroundPlayerMap::iterator l_Iter = m_Players.begin(); l_Iter != m_Players.end(); ++l_Iter)
        {
            if (Player* l_Player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(l_Iter->first, 0, HIGHGUID_PLAYER)))
                l_CapturePoint->SendUpdateToPlayer(l_Player);
        }
    }
}

void BattlegroundDG::_SendNodeUpdate(uint8 p_Node)
{
    // Node map
    UpdateWorldState(BG_DG_NodesWs[p_Node][m_prevNodes[p_Node]].m_WorldState, 0);
    UpdateWorldState(BG_DG_NodesWs[p_Node][m_Nodes[p_Node]].m_WorldState, BG_DG_NodesWs[p_Node][m_Nodes[p_Node]].m_SentValueToShow);

    // How many bases each team owns
    uint8 l_Ally = 0, l_Horde = 0;
    for (uint8 l_Iter = 0; l_Iter < BG_DG_ALL_NODES_COUNT; ++l_Iter)
    {
        if (m_Nodes[l_Iter] == BG_DG_NODE_STATUS_ALLY_OCCUPIED)
            ++l_Ally;
        else if (m_Nodes[l_Iter] == BG_DG_NODE_STATUS_HORDE_OCCUPIED)
            ++l_Horde;
    }

    UpdateWorldState(WORLDSTATE_DG_OCCUPIED_BASES_ALLIANCE, l_Ally);
    UpdateWorldState(WORLDSTATE_DG_OCCUPIED_BASES_HORDE, l_Horde);
}

bool BattlegroundDG::CanSeeSpellClick(Player const* p_Player, Unit const* p_Clicked)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return false;

    for (int l_Iter = 0; l_Iter < BG_DG_ALL_NODES_COUNT; ++l_Iter)
    {
        if (p_Clicked->GetGUID() == BgCreatures[BG_DG_OBJECT_CAPT_POINT_START + l_Iter])
        {
            BattlegroundTeamId l_TeamIndex = GetTeamIndexByTeamId(p_Player->GetBGTeam());
            // Check if player really could use this banner, not cheated
            // Horde team give 0 as rest of modulo (resp. 1 for ally), but horde is 1 as a team (resp. 0 for ally)
            // So if teamIndex != m_Nodes[node] % 2 means that ur team HAS the flag
            if (m_Nodes[l_Iter] != BG_DG_NODE_TYPE_NEUTRAL && l_TeamIndex != m_Nodes[l_Iter] % 2)
                return false;

            return true;
        }
    }

    return false;
}

void BattlegroundDG::EventPlayerClickedOnFlag(Player* p_Source, Unit* /*p_Flag*/)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 l_Node = BG_DG_NODE_GOBLIN_MINE;
    Creature* l_CapturePoint = GetBgMap()->GetCreature(BgCreatures[BG_DG_OBJECT_CAPT_POINT_START + l_Node]);
    while ((l_Node < BG_DG_ALL_NODES_COUNT) && ((!l_CapturePoint) || (!p_Source->IsWithinDistInMap(l_CapturePoint, 10))))
    {
        ++l_Node;
        l_CapturePoint = GetBgMap()->GetCreature(BgCreatures[BG_DG_OBJECT_CAPT_POINT_START + l_Node]);
    }

    // This means our player isn't close to any of banners - maybe cheater ??
    if (l_Node == BG_DG_ALL_NODES_COUNT)
        return;

    BattlegroundTeamId l_TeamIndex = GetTeamIndexByTeamId(p_Source->GetBGTeam());

    // Check if player really could use this banner, not cheated
    // Horde team give 0 as rest of modulo (resp. 1 for ally), but horde is 1 as a team (resp. 0 for ally)
    // So if teamIndex != m_Nodes[node] % 2 means that ur team HAS the flag
    if (m_Nodes[l_Node] != BG_DG_NODE_TYPE_NEUTRAL && l_TeamIndex != m_Nodes[l_Node] % 2)
        return;

    p_Source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    uint32 l_SoundID = 0;
    // If node is neutral, change to contested
    if (m_Nodes[l_Node] == BG_DG_NODE_TYPE_NEUTRAL)
    {
        UpdatePlayerScore(p_Source, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[l_Node] = m_Nodes[l_Node];
        m_Nodes[l_Node] = BG_DG_NODE_TYPE_CONTESTED + l_TeamIndex;
        // Create new contested banner
        _ChangeBanner(l_Node, BG_DG_NODE_TYPE_CONTESTED);
        _SendNodeUpdate(l_Node);
        m_NodeTimers[l_Node] = BG_DG_FLAG_CAPTURING_TIME;
        // Warn players
        SendMessage2ToAll(LANG_BG_DG_NODE_CLAIMED, ChatMsg(CHAT_MSG_BG_SYSTEM_ALLIANCE + l_TeamIndex), p_Source, _GetNodeNameId(l_Node), LANG_BG_DG_ALLY + l_TeamIndex);
        l_SoundID = BG_DG_SOUND_NODE_CLAIMED;
    }
    // If node is contested
    else if ((m_Nodes[l_Node] == BG_DG_NODE_STATUS_ALLY_CONTESTED) || (m_Nodes[l_Node] == BG_DG_NODE_STATUS_HORDE_CONTESTED))
    {
        // If last state is NOT occupied, change node to enemy-contested
        if (m_prevNodes[l_Node] < BG_DG_NODE_TYPE_OCCUPIED)
        {
            UpdatePlayerScore(p_Source, SCORE_BASES_ASSAULTED, 1);
            m_prevNodes[l_Node] = m_Nodes[l_Node];
            m_Nodes[l_Node] = BG_DG_NODE_TYPE_CONTESTED + l_TeamIndex;
            // Create new contested banner
            _ChangeBanner(l_Node, BG_DG_NODE_TYPE_CONTESTED);
            _SendNodeUpdate(l_Node);
            m_NodeTimers[l_Node] = BG_DG_FLAG_CAPTURING_TIME;

            SendMessage2ToAll(LANG_BG_DG_NODE_CLAIMED, ChatMsg(CHAT_MSG_BG_SYSTEM_ALLIANCE + l_TeamIndex), p_Source, _GetNodeNameId(l_Node), LANG_BG_DG_ALLY + l_TeamIndex);
        }
        // If contested, change back to occupied
        else
        {
            UpdatePlayerScore(p_Source, SCORE_BASES_DEFENDED, 1);
            m_prevNodes[l_Node] = m_Nodes[l_Node];
            m_Nodes[l_Node] = BG_DG_NODE_TYPE_OCCUPIED + l_TeamIndex;
            // Create new occupied banner
            _ChangeBanner(l_Node, BG_DG_NODE_TYPE_OCCUPIED);
            _SendNodeUpdate(l_Node);
            m_NodeTimers[l_Node] = 0;

            SendMessage2ToAll(LANG_BG_DG_NODE_DEFENDED, ChatMsg(CHAT_MSG_BG_SYSTEM_ALLIANCE + l_TeamIndex), p_Source, _GetNodeNameId(l_Node), LANG_BG_DG_ALLY + l_TeamIndex);
        }
        l_SoundID = (l_TeamIndex == BG_TEAM_ALLIANCE) ? BG_DG_SOUND_NODE_ASSAULTED_ALLIANCE : BG_DG_SOUND_NODE_ASSAULTED_HORDE;
    }
    // If node is occupied, change to enemy-contested
    else
    {
        UpdatePlayerScore(p_Source, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[l_Node] = m_Nodes[l_Node];
        m_Nodes[l_Node] = l_TeamIndex + BG_DG_NODE_TYPE_CONTESTED;
        // Create new contested banner
        _ChangeBanner(l_Node, BG_DG_NODE_TYPE_CONTESTED);
        _SendNodeUpdate(l_Node);
        m_NodeTimers[l_Node] = BG_DG_FLAG_CAPTURING_TIME;

        SendMessage2ToAll(LANG_BG_DG_NODE_ASSAULTED, ChatMsg(CHAT_MSG_BG_SYSTEM_ALLIANCE + l_TeamIndex), p_Source, _GetNodeNameId(l_Node), LANG_BG_DG_ALLY + l_TeamIndex);

        l_SoundID = (l_TeamIndex == BG_TEAM_ALLIANCE) ? BG_DG_SOUND_NODE_ASSAULTED_ALLIANCE : BG_DG_SOUND_NODE_ASSAULTED_HORDE;
    }

    // If node is occupied again, send "X has taken the Y" msg.
    if (m_Nodes[l_Node] >= BG_DG_NODE_TYPE_OCCUPIED)
        SendMessage2ToAll(LANG_BG_DG_NODE_TAKEN, ChatMsg(CHAT_MSG_BG_SYSTEM_ALLIANCE + l_TeamIndex), p_Source, _GetNodeNameId(l_Node), LANG_BG_DG_ALLY + l_TeamIndex);

    PlaySoundToAll(l_SoundID);
}

void BattlegroundDG::_postUpdateImpl_Flags(uint32 p_Diff)
{
    int l_TeamPoints[MS::Battlegrounds::TeamsCount::Value] = { 0, 0 };

    for (int l_Node = 0; l_Node < BG_DG_ALL_NODES_COUNT; ++l_Node)
    {
        // 1-minute to occupy a node from contested state
        _contestedTime(l_Node, p_Diff);

        for (int l_Team = 0; l_Team < MS::Battlegrounds::TeamsCount::Value; ++l_Team)
        {
            if (m_Nodes[l_Node] == l_Team + BG_DG_NODE_TYPE_OCCUPIED)
                ++l_TeamPoints[l_Team];
        }
    }

    // Accumulate points
    for (int l_Team = 0; l_Team < MS::Battlegrounds::TeamsCount::Value; ++l_Team)
    {
        int l_Points = l_TeamPoints[l_Team];
        if (!l_Points)
            continue;

        m_lastTick[l_Team] += p_Diff;

        if (m_lastTick[l_Team] > BG_DG_TickIntervals[l_Points])
        {
            m_lastTick[l_Team] -= BG_DG_TickIntervals[l_Points];
            m_HonorScoreTics[l_Team] += BG_DG_TickPoints[l_Points];
            if (m_HonorScoreTics[l_Team] >= m_HonorTics)
            {
                RewardHonorToTeam(GetBonusHonorFromKill(6), (l_Team == TEAM_ALLIANCE) ? ALLIANCE : HORDE, MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundObjectif);
                m_HonorScoreTics[l_Team] -= m_HonorTics;
            }

            // Need to be last because there can be a last tick for honor before end (cart give 200, it's a lot)
            _UpdateTeamScore(l_Team, BG_DG_TickPoints[l_Points]);
        }
    }
}

void BattlegroundDG::_contestedTime(int p_Node, uint32 p_Diff)
{
    if (m_NodeTimers[p_Node] == 0)
        return;

    if (m_NodeTimers[p_Node] > p_Diff)
        m_NodeTimers[p_Node] -= p_Diff;
    else
    {
        m_NodeTimers[p_Node] = 0;
        // Change from contested to occupied !
        uint8 l_TeamIndex = m_Nodes[p_Node] - 1;
        m_prevNodes[p_Node] = m_Nodes[p_Node];
        m_Nodes[p_Node] += 2;
        // Create new occupied banner
        _ChangeBanner(p_Node, BG_DG_NODE_TYPE_OCCUPIED);
        _SendNodeUpdate(p_Node);
        // Message to chat log
        SendMessage2ToAll(LANG_BG_DG_NODE_TAKEN, ChatMsg(CHAT_MSG_BG_SYSTEM_ALLIANCE + l_TeamIndex), NULL, LANG_BG_DG_ALLY + l_TeamIndex, _GetNodeNameId(p_Node));
        PlaySoundToAll((l_TeamIndex == TEAM_ALLIANCE) ? BG_DG_SOUND_NODE_CAPTURED_ALLIANCE : BG_DG_SOUND_NODE_CAPTURED_HORDE);
    }
}

int32 BattlegroundDG::_GetNodeNameId(uint8 p_Node)
{
    switch (p_Node)
    {
        case BG_DG_NODE_GOBLIN_MINE:
            return LANG_BG_DG_NODE_SOUTH_MINE;
        case BG_DG_NODE_CENTER_MINE:
            return LANG_BG_DG_NODE_CENTER;
        case BG_DG_NODE_PANDAREN_MINE:
            return LANG_BG_DG_NODE_NORTH_MINE;
        default:
            break;
    }

    return 0;
}

/************************************************************************/
/*                          CARTS UPDATES                               */
/************************************************************************/
void BattlegroundDG::EventPlayerClickedOnFlag(Player* p_Player, GameObject* p_GameObject)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!p_Player->CanUseBattlegroundObject())
        return;

    int32 l_MessageID = 0;
    ChatMsg l_ChatType = CHAT_MSG_BG_SYSTEM_NEUTRAL;

    // Alliance flag picked up from base
    if (p_Player->GetBGTeam() == HORDE && GetFlagState(ALLIANCE) == BG_DG_CART_STATE_ON_BASE
        && BgObjects[BG_DG_OBJECT_CART_ALLIANCE] == p_GameObject->GetGUID())
    {
        l_MessageID = LANG_BG_DG_PICKEDUP_ALLIANCE_CART;
        l_ChatType = CHAT_MSG_BG_SYSTEM_HORDE;
        PlaySoundToAll(BG_DG_SOUND_ALLIANCE_CART_PICKED_UP);
        SpawnBGObject(BG_DG_OBJECT_CART_ALLIANCE, RESPAWN_ONE_DAY);
        SetAllianceCartPicker(p_Player->GetGUID());
        _flagState[TEAM_ALLIANCE] = BG_DG_CART_STATE_ON_PLAYER;
        // Update world state to show correct flag carrier
        UpdateCartState(HORDE, BG_DG_CART_STATE_ON_PLAYER);
        //UpdateWorldState(BG_WS_FLAG_UNK_ALLIANCE, 1);
        p_Player->CastSpell(p_Player, BG_DG_ALLIANCE_MINE_CART, true);
        p_Player->CastSpell(p_Player, BG_DG_ALLIANCE_CART_HOLDER_AURA, true);
        _flagGold[TEAM_ALLIANCE] = (m_TeamScores[TEAM_ALLIANCE] > 200) ? 200 : m_TeamScores[TEAM_ALLIANCE];
        _UpdateTeamScore(TEAM_ALLIANCE, -_flagGold[TEAM_ALLIANCE]);
    }

    // Horde flag picked up from base
    if (p_Player->GetBGTeam() == ALLIANCE && GetFlagState(HORDE) == BG_DG_CART_STATE_ON_BASE
        && BgObjects[BG_DG_OBJECT_CART_HORDE] == p_GameObject->GetGUID())
    {
        l_MessageID = LANG_BG_DG_PICKEDUP_HORDE_CART;
        l_ChatType = CHAT_MSG_BG_SYSTEM_ALLIANCE;
        PlaySoundToAll(BG_DG_SOUND_HORDE_CART_PICKED_UP);
        SpawnBGObject(BG_DG_OBJECT_CART_HORDE, RESPAWN_ONE_DAY);
        SetHordeFlagPicker(p_Player->GetGUID());
        _flagState[TEAM_HORDE] = BG_DG_CART_STATE_ON_PLAYER;
        // Update world state to show correct flag carrier
        UpdateCartState(ALLIANCE, BG_DG_CART_STATE_ON_PLAYER);
        //UpdateWorldState(BG_WS_FLAG_UNK_HORDE, 1);
        p_Player->CastSpell(p_Player, BG_DG_HORDE_MINE_CART, true);
        p_Player->CastSpell(p_Player, BG_DG_HORDE_CART_HOLDER_AURA, true);
        _flagGold[TEAM_HORDE] = (m_TeamScores[TEAM_HORDE] > 200) ? 200 : m_TeamScores[TEAM_HORDE];
        _UpdateTeamScore(TEAM_HORDE, -_flagGold[TEAM_HORDE]);
    }

    // Alliance flag on ground(not in base) (returned or picked up again from ground!)
    if (GetFlagState(ALLIANCE) == BG_DG_CART_STATE_ON_GROUND && p_Player->IsWithinDistInMap(p_GameObject, 10)
        && p_GameObject->GetGOInfo()->entry == BG_DG_OBJECTID_CART_ALLY_GROUND)
    {
        if (p_Player->GetBGTeam() == ALLIANCE)
        {
            l_MessageID = LANG_BG_DG_RETURNED_ALLIANCE_CART;
            l_ChatType = CHAT_MSG_BG_SYSTEM_ALLIANCE;
            UpdateCartState(HORDE, BG_DG_CART_STATE_ON_BASE);
            _flagState[TEAM_ALLIANCE] = BG_DG_CART_STATE_ON_BASE;
            SpawnBGObject(BG_DG_OBJECT_CART_ALLIANCE, RESPAWN_IMMEDIATELY);
            PlaySoundToAll(BG_DG_SOUND_CART_RETURNED);
            UpdatePlayerScore(p_Player, SCORE_CART_RETURNS, 1);
            _UpdateTeamScore(TEAM_ALLIANCE, _flagGold[TEAM_ALLIANCE]);
            _flagGold[TEAM_ALLIANCE] = 0;
        }
        else
        {
            l_MessageID = LANG_BG_DG_PICKEDUP_ALLIANCE_CART;
            l_ChatType = CHAT_MSG_BG_SYSTEM_HORDE;
            PlaySoundToAll(BG_DG_SOUND_ALLIANCE_CART_PICKED_UP);
            SpawnBGObject(BG_DG_OBJECT_CART_ALLIANCE, RESPAWN_ONE_DAY);
            SetAllianceCartPicker(p_Player->GetGUID());
            p_Player->CastSpell(p_Player, BG_DG_ALLIANCE_MINE_CART, true);
            p_Player->CastSpell(p_Player, BG_DG_ALLIANCE_CART_HOLDER_AURA, true);
            _flagState[TEAM_ALLIANCE] = BG_DG_CART_STATE_ON_PLAYER;
            UpdateCartState(HORDE, BG_DG_CART_STATE_ON_PLAYER);
            //UpdateWorldState(BG_WS_FLAG_UNK_ALLIANCE, 1);
        }
        p_GameObject->Delete();
    }

    // Horde flag on ground(not in base) (returned or picked up again)
    if (GetFlagState(HORDE) == BG_DG_CART_STATE_ON_GROUND && p_Player->IsWithinDistInMap(p_GameObject, 10)
        && p_GameObject->GetGOInfo()->entry == BG_DG_OBJECTID_CART_HORDE_GROUND)
    {
        if (p_Player->GetBGTeam() == HORDE)
        {
            l_MessageID = LANG_BG_DG_RETURNED_HORDE_CART;
            l_ChatType = CHAT_MSG_BG_SYSTEM_HORDE;
            UpdateCartState(ALLIANCE, BG_DG_CART_STATE_ON_BASE);
            _flagState[TEAM_HORDE] = BG_DG_CART_STATE_ON_BASE;
            SpawnBGObject(BG_DG_OBJECT_CART_HORDE, RESPAWN_IMMEDIATELY);
            PlaySoundToAll(BG_DG_SOUND_CART_RETURNED);
            UpdatePlayerScore(p_Player, SCORE_CART_RETURNS, 1);
            _UpdateTeamScore(TEAM_HORDE, _flagGold[TEAM_HORDE]);
            _flagGold[TEAM_HORDE] = 0;
        }
        else
        {
            l_MessageID = LANG_BG_DG_PICKEDUP_HORDE_CART;
            l_ChatType = CHAT_MSG_BG_SYSTEM_ALLIANCE;
            PlaySoundToAll(BG_DG_SOUND_HORDE_CART_PICKED_UP);
            SpawnBGObject(BG_DG_OBJECT_CART_HORDE, RESPAWN_ONE_DAY);
            SetHordeFlagPicker(p_Player->GetGUID());
            p_Player->CastSpell(p_Player, BG_DG_HORDE_MINE_CART, true);
            p_Player->CastSpell(p_Player, BG_DG_HORDE_CART_HOLDER_AURA, true);
            _flagState[TEAM_HORDE] = BG_DG_CART_STATE_ON_PLAYER;
            UpdateCartState(ALLIANCE, BG_DG_CART_STATE_ON_PLAYER);
            //UpdateWorldState(BG_WS_FLAG_UNK_HORDE, 1);
        }
        p_GameObject->Delete();
    }

    if (!l_MessageID)
        return;

    SendMessageToAll(l_MessageID, l_ChatType, p_Player);
    p_Player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundDG::HandleAreaTrigger(Player* p_Player, uint32 p_TriggerID)
{
    switch (p_TriggerID)
    {
        case 9012: // Alliance cart spawn
            if (_flagState[TEAM_HORDE] && !_flagState[TEAM_ALLIANCE])
            {
                if (GetFlagPickerGUID(TEAM_HORDE) == p_Player->GetGUID())
                    EventPlayerCapturedFlag(p_Player);
            }
            break;
        case 9013: // Horde cart spawn
            if (_flagState[TEAM_ALLIANCE] && !_flagState[TEAM_HORDE])
            {
                if (GetFlagPickerGUID(TEAM_ALLIANCE) == p_Player->GetGUID())
                    EventPlayerCapturedFlag(p_Player);
            }
            break;
        case 9139: // behind the wood on the spawn building alliance on the right
        case 9140: // inside building
        case 9159: // buff location
        case 9160: // buff location
        case 9161: // buff location
        case 9162: // buff location
        case 9299: // on the roof
        case 9301: // on the roof
        case 9302: // flying => should tp outside the mine when triggered
        case 9303: // flying => should tp outside the mine when triggered
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundDG : Handled AreaTrigger(ID : %u) have been activated by Player %s (ID : %u)",
                p_TriggerID, p_Player->GetName(), GUID_LOPART(p_Player->GetGUID()));
            break;
        default:
            Battleground::HandleAreaTrigger(p_Player, p_TriggerID);
            break;
    }
}

void BattlegroundDG::_postUpdateImpl_Cart(uint32 p_Diff)
{
    if (_flagState[TEAM_ALLIANCE] == BG_DG_CART_STATE_ON_GROUND)
    {
        _flagsDropTimer[TEAM_ALLIANCE] -= p_Diff;

        if (_flagsDropTimer[TEAM_ALLIANCE] <= 0)
        {
            _flagsDropTimer[TEAM_ALLIANCE] = 0;
            RespawnFlagAfterDrop(TEAM_ALLIANCE);
        }
    }

    if (_flagState[TEAM_HORDE] == BG_DG_CART_STATE_ON_GROUND)
    {
        _flagsDropTimer[TEAM_HORDE] -= p_Diff;

        if (_flagsDropTimer[TEAM_HORDE] <= 0)
        {
            _flagsDropTimer[TEAM_HORDE] = 0;
            RespawnFlagAfterDrop(TEAM_HORDE);
        }
    }
}

void BattlegroundDG::RespawnFlagAfterDrop(uint32 p_Team)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    _flagState[p_Team] = BG_DG_CART_STATE_ON_BASE;

    SpawnBGObject(BG_DG_OBJECT_CART_ALLIANCE + p_Team, RESPAWN_IMMEDIATELY);
    SendMessageToAll(LANG_BG_DG_ALLIANCE_FLAG_RESPAWNED + p_Team, CHAT_MSG_BG_SYSTEM_NEUTRAL);

    //PlaySoundToAll(BG_DG_SOUND_CARTS_RESPAWNED);

    if (GameObject* l_Gob = GetBGObject(BG_DG_OBJECT_CART_ALLY_GROUND + p_Team))
        l_Gob->Delete();
    else
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground DG : unknown dropped flag bg");
}

void BattlegroundDG::EventPlayerCapturedFlag(Player* p_Player)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint32 l_WinnerID = 0;

    p_Player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    if (p_Player->GetBGTeam() == ALLIANCE)
    {
        if (!IsHordeFlagPickedup())
            return;

        SetHordeFlagPicker(0);                              // must be before aura remove to prevent 2 events (drop+capture) at the same time
        // horde flag in base (but not respawned yet)
        _flagState[TEAM_HORDE] = BG_DG_CART_STATE_ON_BASE;
        _UpdateTeamScore(TEAM_ALLIANCE, _flagGold[TEAM_HORDE]);
        _flagGold[TEAM_HORDE] = 0;
        // Drop Horde Flag from Player
        p_Player->RemoveAurasDueToSpell(BG_DG_HORDE_MINE_CART);
        p_Player->RemoveAurasDueToSpell(BG_DG_HORDE_CART_HOLDER_AURA);
        PlaySoundToAll(BG_DG_SOUND_CART_CAPTURED_ALLIANCE);
        SpawnBGObject(BG_DG_OBJECT_CART_HORDE, RESPAWN_IMMEDIATELY);
    }
    else
    {
        if (!IsAllianceFlagPickedup())
            return;

        SetAllianceCartPicker(0);                           // must be before aura remove to prevent 2 events (drop+capture) at the same time

        // alliance flag in base (but not respawned yet)
        _flagState[TEAM_ALLIANCE] = BG_DG_CART_STATE_ON_BASE;
        _UpdateTeamScore(TEAM_HORDE, _flagGold[TEAM_ALLIANCE]);
        _flagGold[TEAM_ALLIANCE] = 0;

        // Drop Alliance Flag from Player
        p_Player->RemoveAurasDueToSpell(BG_DG_ALLIANCE_MINE_CART);
        p_Player->RemoveAurasDueToSpell(BG_DG_ALLIANCE_CART_HOLDER_AURA);

        PlaySoundToAll(BG_DG_SOUND_CART_CAPTURED_HORDE);
        SpawnBGObject(BG_DG_OBJECT_CART_ALLIANCE, RESPAWN_IMMEDIATELY);
    }
    // For flag capture is reward 2 honorable kills
    RewardHonorToTeam(GetBonusHonorFromKill(12), p_Player->GetBGTeam(), MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundObjectif);

    if (p_Player->GetBGTeam() == ALLIANCE)
        SendMessageToAll(LANG_BG_DG_CAPTURED_HORDE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE, p_Player);
    else
        SendMessageToAll(LANG_BG_DG_CAPTURED_ALLIANCE_CART, CHAT_MSG_BG_SYSTEM_HORDE, p_Player);

    UpdateCartState(p_Player->GetBGTeam(), 1);                  // Flag state none
    // Only flag capture should be updated
    UpdatePlayerScore(p_Player, SCORE_CART_CAPTURES, 1);      // +1 flag captures

    // Update last flag capture to be used if team score is equal
    SetLastFlagCapture(p_Player->GetBGTeam());

    _flagsTimer[GetTeamIndexByTeamId(p_Player->GetBGTeam()) ? 0 : 1] = BG_DG_CART_RESPAWN_TIME;
}

void BattlegroundDG::EventPlayerDroppedFlag(Player* p_Player)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
    {
        // If not running, do not cast things at the dropper player (prevent spawning the "dropped" flag), neither send unnecessary messages
        // Just take off the aura
        if (p_Player->GetBGTeam() == ALLIANCE)
        {
            if (!IsHordeFlagPickedup())
                return;

            if (GetFlagPickerGUID(TEAM_HORDE) == p_Player->GetGUID())
            {
                SetHordeFlagPicker(0);
                p_Player->RemoveAurasDueToSpell(BG_DG_HORDE_MINE_CART);
                p_Player->RemoveAurasDueToSpell(BG_DG_HORDE_CART_HOLDER_AURA);
            }
        }
        else
        {
            if (!IsAllianceFlagPickedup())
                return;

            if (GetFlagPickerGUID(TEAM_ALLIANCE) == p_Player->GetGUID())
            {
                SetAllianceCartPicker(0);
                p_Player->RemoveAurasDueToSpell(BG_DG_ALLIANCE_MINE_CART);
                p_Player->RemoveAurasDueToSpell(BG_DG_ALLIANCE_CART_HOLDER_AURA);
            }
        }
        return;
    }

    bool l_Set = false;

    if (p_Player->GetBGTeam() == ALLIANCE)
    {
        if (!IsHordeFlagPickedup())
            return;

        if (GetFlagPickerGUID(TEAM_HORDE) == p_Player->GetGUID())
        {
            SetHordeFlagPicker(0);
            p_Player->RemoveAurasDueToSpell(BG_DG_HORDE_MINE_CART);
            p_Player->RemoveAurasDueToSpell(BG_DG_HORDE_CART_HOLDER_AURA);
            _flagState[TEAM_HORDE] = BG_DG_CART_STATE_ON_GROUND;
            if (AddObject(BG_DG_OBJECT_CART_HORDE_GROUND, BG_DG_OBJECTID_CART_HORDE_GROUND, p_Player->GetPositionX(), p_Player->GetPositionY(), p_Player->GetPositionZ(), p_Player->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f))
            {
                SpawnBGObject(BG_DG_OBJECT_CART_HORDE_GROUND, RESPAWN_IMMEDIATELY);
                // Dunno how to cast spell on gob :/
                //GetBGObject(BG_DG_OBJECT_CART_HORDE_GROUND)->CastSpell(GetBGObject(BG_DG_OBJECT_CART_HORDE_GROUND), true);
            }
            _flagsDropTimer[TEAM_HORDE] = BG_DG_CART_DROP_TIME;
            l_Set = true;
        }
    }
    else
    {
        if (!IsAllianceFlagPickedup())
            return;

        if (GetFlagPickerGUID(TEAM_ALLIANCE) == p_Player->GetGUID())
        {
            SetAllianceCartPicker(0);
            p_Player->RemoveAurasDueToSpell(BG_DG_ALLIANCE_MINE_CART);
            p_Player->RemoveAurasDueToSpell(BG_DG_ALLIANCE_CART_HOLDER_AURA);
            _flagState[TEAM_ALLIANCE] = BG_DG_CART_STATE_ON_GROUND;
            if (AddObject(BG_DG_OBJECT_CART_ALLY_GROUND, BG_DG_OBJECTID_CART_ALLY_GROUND, p_Player->GetPositionX(), p_Player->GetPositionY(), p_Player->GetPositionZ(), p_Player->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f))
            {
                SpawnBGObject(BG_DG_OBJECT_CART_ALLY_GROUND, RESPAWN_IMMEDIATELY);
                // Dunno how to cast spell on gob :/
                //GetBGObject(BG_DG_OBJECT_CART_ALLY_GROUND)->CastSpell(GetBGObject(BG_DG_OBJECT_CART_ALLY_GROUND), true);
            }
            _flagsDropTimer[TEAM_ALLIANCE] = BG_DG_CART_DROP_TIME;
            l_Set = true;
        }
    }

    if (l_Set)
    {
        p_Player->CastSpell(p_Player, SPELL_RECENTLY_DROPPED_FLAG, true);
        UpdateCartState(p_Player->GetBGTeam(), 1);

        if (p_Player->GetBGTeam() == ALLIANCE)
        {
            SendMessageToAll(LANG_BG_DG_DROPPED_HORDE_CART, CHAT_MSG_BG_SYSTEM_HORDE, p_Player);
            //UpdateWorldState(BG_WS_FLAG_UNK_HORDE, uint32(-1));
        }
        else
        {
            SendMessageToAll(LANG_BG_DG_DROPPED_ALLIANCE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE, p_Player);
            //UpdateWorldState(BG_WS_FLAG_UNK_ALLIANCE, uint32(-1));
        }
    }
}

void BattlegroundDG::UpdateCartState(uint32 p_Team, uint32 p_Value)
{
    if (p_Team == ALLIANCE)
        UpdateWorldState(WORLDSTATE_DG_CART_STATE_ALLIANCE, p_Value);
    else
        UpdateWorldState(WORLDSTATE_DG_CART_STATE_HORDE, p_Value);
}

/************************************************************************/
/*                        ENDING BATTLEGROUND                           */
/************************************************************************/
void BattlegroundDG::RemovePlayer(Player* p_Player, uint64 p_Guid, uint32 /*p_Team*/)
{
    // Sometimes flag aura not removed :(
    if (IsAllianceFlagPickedup() && m_FlagKeepers[TEAM_ALLIANCE] == p_Guid)
    {
        if (!p_Player)
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundWS: Removing offline player who has the FLAG!!");
            SetAllianceCartPicker(0);
            _flagState[TEAM_ALLIANCE] = BG_DG_CART_STATE_ON_BASE;
            SpawnBGObject(BG_DG_OBJECT_CART_ALLIANCE, RESPAWN_IMMEDIATELY);
        }
        else
            EventPlayerDroppedFlag(p_Player);
    }

    if (IsHordeFlagPickedup() && m_FlagKeepers[TEAM_HORDE] == p_Guid)
    {
        if (!p_Player)
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundWS: Removing offline player who has the FLAG!!");
            SetHordeFlagPicker(0);
            _flagState[TEAM_HORDE] = BG_DG_CART_STATE_ON_BASE;
            SpawnBGObject(BG_DG_OBJECT_CART_HORDE, RESPAWN_IMMEDIATELY);
        }
        else
            EventPlayerDroppedFlag(p_Player);
    }
}

void BattlegroundDG::EndBattleground(uint32 p_Winner)
{
    Battleground::EndBattleground(p_Winner);
}

BattlegroundDG::~BattlegroundDG()
{
}

/************************************************************************/
/*                     CART SCRIPTS (CREATURE)                          */
/************************************************************************/

// Horde mine cart - 141210
// Alliance mine cart - 140876
class spell_mine_cart : public SpellScriptLoader
{
    public:
        spell_mine_cart() : SpellScriptLoader("spell_mine_cart") { }

        class spell_mine_cart_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mine_cart_AuraScript);

            void HandleOnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                uint32 l_Entry = 0;
                if (GetSpellInfo()->Id == BG_DG_HORDE_MINE_CART)
                    l_Entry = 71073;
                else if (GetSpellInfo()->Id == BG_DG_ALLIANCE_MINE_CART)
                    l_Entry = 71071;

                if (l_Entry != 0)
                {
                    std::list<Creature*> l_Carts;
                    GetCaster()->GetCreatureListWithEntryInGrid(l_Carts, l_Entry, 500.0f);
                    for (std::list<Creature*>::iterator l_Iter = l_Carts.begin(); l_Iter != l_Carts.end(); ++l_Iter)
                    {
                        if (TempSummon* l_TempSummon = (*l_Iter)->ToTempSummon())
                            l_TempSummon->UnSummon();
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mine_cart_AuraScript::HandleOnRemove, EFFECT_4, SPELL_AURA_LINKED_2, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mine_cart_AuraScript();
        }
};

// Mine cart - 71071 / 71073
class npc_dg_cart : public CreatureScript
{
    public:
        npc_dg_cart() : CreatureScript("npc_dg_cart") { }

        enum
        {
            EVENT_NEW_WAYPOINT = 1,
        };

        struct npc_dg_cartAI : public ScriptedAI
        {
            npc_dg_cartAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            EventMap m_Events;

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                if (me->GetOwner())
                {
                    me->SetSpeed(MOVE_RUN, (me->GetOwner()->GetSpeed(MOVE_RUN) + 1.0f) / playerBaseMoveSpeed[MOVE_RUN], true);
                    if (!me->IsMoving() && m_Events.Empty())
                        m_Events.ScheduleEvent(EVENT_NEW_WAYPOINT, 700);
                    if (m_Events.ExecuteEvent() == EVENT_NEW_WAYPOINT)
                        me->GetMotionMaster()->MoveChase(me->GetOwner(), frand(0.5f, 1.0f), frand(0.0f, 2.0f * M_PI));
                    if (!me->HasAura(52595))
                        me->GetOwner()->CastSpell(me, 52595, true);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_dg_cartAI(p_Creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_BattlegroundDGScripts()
{
    new spell_mine_cart();
    new npc_dg_cart();
}
#endif