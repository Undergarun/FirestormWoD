////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "gamePCH.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundBFG.h"
#include "Language.h"
#include "World.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.hpp"
#include "Creature.h"
#include "Object.h"
#include "Util.h"

uint32 GILNEAS_BG_HonorScoreTicks[BG_HONOR_MODE_NUM] = ///< this is unused !
{
    330, // normal honor
    200  // holiday
};

uint32 GILNEAS_BG_Reputation[BG_HONOR_MODE_NUM][GILNEAS_BG_REWARD_NUM] = ///< this is unused !
{
    {200}, // normal honor
    {150}  // holiday
};

BattlegroundBFG::BattlegroundBFG()
{
    m_BuffChange = true;
    BgObjects.resize(GILNEAS_BG_OBJECT_MAX);
    BgCreatures.resize(GILNEAS_BG_ALL_NODES_COUNT + 3); // +3 for aura triggers

    StartMessageIds[BG_STARTING_EVENT_FIRST] = LANG_BG_BFG_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_BFG_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_BFG_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_BFG_HAS_BEGUN;

    for (int l_I = 0; l_I < GILNEAS_BG_DYNAMIC_NODES_COUNT; l_I++)
        m_BannerWorldState[l_I] = BattleForGilneasWorldState::Neutral;
}

BattlegroundBFG::~BattlegroundBFG() { }

void BattlegroundBFG::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    int team_points[MS::Battlegrounds::TeamsCount::Value] = { 0, 0 };

    for (int node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
    {
        // 1-minute cap timer on each node from a contested state.
        if (m_NodeTimers[node])
        {
            if (m_NodeTimers[node] > diff)
                m_NodeTimers[node] -= diff;
            else
            {
                m_NodeTimers[node] = 0;

                // Change from contested to occupied !
                uint8 teamIndex = m_Nodes[node]-1;
                m_prevNodes[node] = m_Nodes[node];
                m_Nodes[node] += 2;

                // create new occupied banner
                _ChangeBanner(node, GILNEAS_BG_NODE_TYPE_OCCUPIED, teamIndex);
                _SendNodeUpdate(node);
                _NodeOccupied(node, (teamIndex == 0) ? ALLIANCE:HORDE);

                // Message to chatlog
                if (teamIndex == 0)
                {
                    // FIXME: need to fix Locales for team and nodes names.
                    SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, LANG_BG_BFG_ALLY, _GetNodeNameId(node));
                    PlaySoundToAll(GILNEAS_BG_SOUND_NODE_CAPTURED_ALLIANCE);
                }
                else
                {
                    // FIXME: team and node names not localized
                    SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_HORDE, NULL, LANG_BG_BFG_HORDE, _GetNodeNameId(node));
                    PlaySoundToAll(GILNEAS_BG_SOUND_NODE_CAPTURED_HORDE);
                }
            }
        }

        for (int team = 0; team < MS::Battlegrounds::TeamsCount::Value; ++team)
            if (m_Nodes[node] == team + GILNEAS_BG_NODE_TYPE_OCCUPIED)
                ++team_points[team];
    }

    // Accumulate points
    for (int team = 0; team < MS::Battlegrounds::TeamsCount::Value; ++team)
    {
        int points = team_points[team];
        if (!points)
            continue;

        m_lastTick[team] += diff;
        if (m_lastTick[team] > GILNEAS_BG_TickIntervals[points])
        {
            m_lastTick[team] -= GILNEAS_BG_TickIntervals[points];
            m_TeamScores[team] += GILNEAS_BG_TickPoints[points];
            m_HonorScoreTicks[team] += GILNEAS_BG_TickPoints[points];
            m_ReputationScoreTicks[team] += GILNEAS_BG_TickPoints[points];

            if (m_ReputationScoreTicks[team] >= m_ReputationTicks)
            {
                (team == BG_TEAM_ALLIANCE) ? RewardReputationToTeam(509, 10, ALLIANCE) : RewardReputationToTeam(510, 10, HORDE);
                m_ReputationScoreTicks[team] -= m_ReputationTicks;
            }

            if (m_HonorScoreTicks[team] >= m_HonorTicks)
            {
                RewardHonorToTeam(GetBonusHonorFromKill(1), (team == BG_TEAM_ALLIANCE) ? ALLIANCE : HORDE, MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundObjectif);
                m_HonorScoreTicks[team] -= m_HonorTicks;
            }

            if (!m_IsInformedNearVictory && m_TeamScores[team] > GILNEAS_BG_WARNING_NEAR_VICTORY_SCORE)
            {
                if (team == BG_TEAM_ALLIANCE)
                    SendMessageToAll(LANG_BG_AB_A_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                else
                    SendMessageToAll(LANG_BG_AB_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);

                PlaySoundToAll(GILNEAS_BG_SOUND_NEAR_VICTORY);
                m_IsInformedNearVictory = true;
            }

            if (m_TeamScores[team] > GILNEAS_BG_MAX_TEAM_SCORE)
                m_TeamScores[team] = GILNEAS_BG_MAX_TEAM_SCORE;

            if (team == BG_TEAM_ALLIANCE)
                UpdateWorldState(GILNEAS_BG_OP_RESOURCES_ALLY, m_TeamScores[team]);

            if (team == BG_TEAM_HORDE)
                UpdateWorldState(GILNEAS_BG_OP_RESOURCES_HORDE, m_TeamScores[team]);
            // update achievement flags
            // we increased m_TeamScores[team] so we just need to check if it is 500 more than other teams resources
            uint8 otherTeam = (team + 1) % MS::Battlegrounds::TeamsCount::Value;
            if (m_TeamScores[team] > m_TeamScores[otherTeam] + 500)
                m_TeamScores500Disadvantage[otherTeam] = true;
        }
    }

    // Test win condition
    if (m_TeamScores[BG_TEAM_ALLIANCE] >= GILNEAS_BG_MAX_TEAM_SCORE)
        EndBattleground(ALLIANCE);

    if (m_TeamScores[BG_TEAM_HORDE] >= GILNEAS_BG_MAX_TEAM_SCORE)
        EndBattleground(HORDE);
}

void BattlegroundBFG::StartingEventCloseDoors()
{
    // Remove banners, auras and buffs
    for (int object = GILNEAS_BG_OBJECT_BANNER_NEUTRAL; object < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++object)
        SpawnBGObject(object, RESPAWN_ONE_DAY);

    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT * 3; ++i)
        SpawnBGObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + i, RESPAWN_ONE_DAY);

    // Starting doors
    DoorClose(GILNEAS_BG_OBJECT_GATE_A_1);
    DoorClose(GILNEAS_BG_OBJECT_GATE_H_1);

    // Starting base spirit guides
    _NodeOccupied(GILNEAS_BG_SPIRIT_ALIANCE, ALLIANCE);
    _NodeOccupied(GILNEAS_BG_SPIRIT_HORDE, HORDE);
}

void BattlegroundBFG::StartingEventOpenDoors()
{
    for (int i = GILNEAS_BG_OBJECT_BANNER_NEUTRAL; i < 3; ++i)
    {
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
        if (GameObject* l_Banner = GetBGObject(i))
             l_Banner->SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, (uint32)BattleForGilneasBannerSpellVisual::Neutral);
    }

    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        uint8 buff = urand(0, 2);
        SpawnBGObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + buff + i * 3, RESPAWN_IMMEDIATELY);
    }

    DoorOpen(GILNEAS_BG_OBJECT_GATE_A_1);
    DoorOpen(GILNEAS_BG_OBJECT_GATE_H_1);

    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, BG_EVENT_START_BATTLE);
}

void BattlegroundBFG::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    // Create score and add it to map, default values are set in constructor
    BattlegroundBFGScore* score = new BattlegroundBFGScore;

    PlayerScores[player->GetGUID()] = score;
}

void BattlegroundBFG::RemovePlayer(Player* /*player*/, uint64 /*guid*/)
{
}

void BattlegroundBFG::HandleAreaTrigger(Player * /*Source*/, uint32 /*Trigger*/)
{
    // this is  wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
}

void BattlegroundBFG::_ChangeBanner(uint8 node, uint8 type, uint8 teamIndex)
{
    GameObject* l_Banner = GetBGObject(node);
    if (l_Banner == nullptr)
        return;

    BattleForGilneasWorldState l_WorldStateValue = BattleForGilneasWorldState::Neutral;

    uint32 l_SpellVisualId = (uint32)BattleForGilneasBannerSpellVisual::Neutral;

    if (type == GILNEAS_BG_NODE_TYPE_CONTESTED)
    {
        if (teamIndex == 1)
        {
            l_SpellVisualId = (uint32)BattleForGilneasBannerSpellVisual::HordeContested;
            l_WorldStateValue = BattleForGilneasWorldState::HordeContested;
        }
        else
        {
            l_SpellVisualId = (uint32)BattleForGilneasBannerSpellVisual::AllianceContested;
            l_WorldStateValue = BattleForGilneasWorldState::AllianceContested;
        }

    }

    if (type == GILNEAS_BG_NODE_TYPE_OCCUPIED)
    {
        if (teamIndex == 1)
        {
            l_SpellVisualId = (uint32)BattleForGilneasBannerSpellVisual::HordeOccupied;
            l_WorldStateValue = BattleForGilneasWorldState::HordeOccupied;
        }
        else
        {
            l_SpellVisualId = (uint32)BattleForGilneasBannerSpellVisual::AllianceOccupied;
            l_WorldStateValue = BattleForGilneasWorldState::AllianceOccupied;
        }
    }


    // Update the visual of the banner
    l_Banner->SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, l_SpellVisualId);

    // Update the worldstate
    m_BannerWorldState[node] = l_WorldStateValue;
    UpdateWorldState(l_Banner->GetGOInfo()->capturePoint.worldState1, (uint8)l_WorldStateValue);
}

int32 BattlegroundBFG::_GetNodeNameId(uint8 node)
{
    switch (node)
    {
        case GILNEAS_BG_NODE_LIGHTHOUSE: return LANG_BG_BFG_NODE_LIGHTHOUSE;
        case GILNEAS_BG_NODE_WATERWORKS: return LANG_BG_BFG_NODE_WATERWORKS;
        case GILNEAS_BG_NODE_MINE: return LANG_BG_BFG_NODE_MINE;
        default:
            ASSERT(0);
    }
    return 0;
}

void BattlegroundBFG::FillInitialWorldStates(ByteBuffer& data)
{
    const uint8 plusArray[] = { 0, 2, 3, 0, 1 };

    // Node icons
    for (uint8 node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
        data << uint32(GILNEAS_BG_OP_NODEICONS[node]) << uint32((m_Nodes[node] == 0) ? 1 : 0);

    // Node occupied states
    for (uint8 node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
        for (uint8 i = 1; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
            data << uint32(GILNEAS_BG_OP_NODESTATES[node] + plusArray[i]) << uint32((m_Nodes[node] == i) ? 1 : 0);

    // How many bases each team owns
    uint8 ally = 0, horde = 0;
    for (uint8 node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
        if (m_Nodes[node] == GILNEAS_BG_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[node] == GILNEAS_BG_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    data << uint32(GILNEAS_BG_OP_OCCUPIED_BASES_ALLY)  << uint32(ally);
    data << uint32(GILNEAS_BG_OP_OCCUPIED_BASES_HORDE) << uint32(horde);

    // Team scores
    data << uint32(GILNEAS_BG_OP_RESOURCES_ALLY)     << uint32(m_TeamScores[BG_TEAM_ALLIANCE]);
    data << uint32(GILNEAS_BG_OP_RESOURCES_HORDE)    << uint32(m_TeamScores[BG_TEAM_HORDE]);
    data << uint32(GILNEAS_BG_OP_RESOURCES_MAX)      << uint32(GILNEAS_BG_MAX_TEAM_SCORE);
    data << uint32(GILNEAS_BG_OP_RESOURCES_WARNING)  << uint32(GILNEAS_BG_WARNING_NEAR_VICTORY_SCORE);

    // Banner world states
    for (int obj = GILNEAS_BG_NODE_LIGHTHOUSE; obj < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++obj)
    {
        GameObject* l_Banner = GetBGObject(obj);
        if (l_Banner == nullptr)
            continue;

        data << uint32(l_Banner->GetGOInfo()->capturePoint.worldState1) << uint32(m_BannerWorldState[obj]);
    }

    // other unknown
    //data << uint32(0x745) << uint32(0x2);           // 37 1861 unk
}

void BattlegroundBFG::_SendNodeUpdate(uint8 node)
{
    // Send to client owner node state updates to refresh map icons.
    const uint8 plusArray[] = { 0, 2, 3, 0, 1 };

    if (m_prevNodes[node])
        UpdateWorldState(GILNEAS_BG_OP_NODESTATES[node] + plusArray[m_prevNodes[node]], 0);
    else
        UpdateWorldState(GILNEAS_BG_OP_NODEICONS[node], 0);

    UpdateWorldState(GILNEAS_BG_OP_NODESTATES[node] + plusArray[m_Nodes[node]], 1);

    // How many bases each team owns
    uint8 ally = 0, horde = 0;

    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == GILNEAS_BG_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == GILNEAS_BG_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    UpdateWorldState(GILNEAS_BG_OP_OCCUPIED_BASES_ALLY, ally);
    UpdateWorldState(GILNEAS_BG_OP_OCCUPIED_BASES_HORDE, horde);
}

void BattlegroundBFG::_NodeOccupied(uint8 node, Team team)
{
    if (node >= GILNEAS_BG_DYNAMIC_NODES_COUNT)
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundBFG::_NodeOccupied node(%u) > GILNEAS_BG_DYNAMIC_NODES_COUNT(%u)", node, GILNEAS_BG_DYNAMIC_NODES_COUNT);

    if (!AddSpiritGuide(node, GILNEAS_BG_SpiritGuidePos[node][0], GILNEAS_BG_SpiritGuidePos[node][1], GILNEAS_BG_SpiritGuidePos[node][2], GILNEAS_BG_SpiritGuidePos[node][3], team))
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Failed to spawn spirit guide! point: %u, team: %u, ", node, team);

    if (node >= GILNEAS_BG_DYNAMIC_NODES_COUNT) // only dynamic nodes, no start points
        return;

    Creature* trigger = GetBGCreature(node+5); // 0-5 spirit guides

    if (!trigger)
       trigger = AddCreature(WORLD_TRIGGER, node+5, team, GILNEAS_BG_NodePositions[node][0], GILNEAS_BG_NodePositions[node][1], GILNEAS_BG_NodePositions[node][2], GILNEAS_BG_NodePositions[node][3]);

    // Add bonus honor aura trigger creature when node is occupied
    // Cast bonus aura (+50% honor in 25yards)
    // aura should only apply to players who have occupied the node, set correct faction for trigger
    if (trigger)
    {
        trigger->setFaction(team == ALLIANCE ? 84 : 83);
        trigger->CastSpell(trigger, SPELL_HONORABLE_DEFENDER_25Y, false);
    }
}

void BattlegroundBFG::_NodeDeOccupied(uint8 node)
{
    if (node >= GILNEAS_BG_DYNAMIC_NODES_COUNT)
        return;

    // Remove bonus honor aura trigger bunny when node is lost
    if (node < GILNEAS_BG_DYNAMIC_NODES_COUNT)  // Only dynamic nodes, no start points
        DelCreature(node+5);                    // NULL checks are in DelCreature! 0-5 spirit guides

    // Players waiting to resurrect at this node are sent to closest owned graveyard
    std::vector<uint64> ghost_list = m_ReviveQueue[BgCreatures[node]];
    if (!ghost_list.empty())
    {
        WorldSafeLocsEntry const *ClosestGrave = NULL;
        for (std::vector<uint64>::const_iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;

            if (!ClosestGrave)
                ClosestGrave = GetClosestGraveYard(player);

            if (ClosestGrave)
                player->TeleportTo(GetMapId(), ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, player->GetOrientation());
        }
    }

    if (BgCreatures[node])
        DelCreature(node);

    // Buff object is not removed
}

/* Invoked if a player used a banner as a GameObject */
void BattlegroundBFG::EventPlayerClickedOnFlag(Player* source, GameObject* /*target_obj*/)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 node = GILNEAS_BG_NODE_LIGHTHOUSE;
    GameObject* object = GetBgMap()->GetGameObject(BgObjects[node]);
    while ((node < GILNEAS_BG_DYNAMIC_NODES_COUNT) && ((!object) || (!source->IsWithinDistInMap(object, 10))))
    {
        ++node;
        object=GetBgMap()->GetGameObject(BgObjects[node]);
    }

    if (node == GILNEAS_BG_DYNAMIC_NODES_COUNT)
    {
        // this means our player isn't close to any of banners - maybe cheater ??
        return;
    }

    BattlegroundTeamId teamIndex = GetTeamIndexByTeamId(source->GetBGTeam());

    // Check if player really could use this banner, and has not cheated
    if (!(m_Nodes[node] == 0 || teamIndex == m_Nodes[node]%2))
        return;

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    uint32 sound = 0;
    // If node is neutral, change to contested
    if (m_Nodes[node] == GILNEAS_BG_NODE_TYPE_NEUTRAL)
    {
        UpdatePlayerScore(source, nullptr, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + 1;

        // create new contested banner
        _ChangeBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, teamIndex);
        _SendNodeUpdate(node);
        m_NodeTimers[node] = GILNEAS_BG_FLAG_CAPTURING_TIME;

        // FIXME: need to fix Locales for team and node names.
        if (teamIndex == 0)
            SendMessage2ToAll(LANG_BG_BFG_NODE_CLAIMED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node), LANG_BG_BFG_ALLY);
        else
            SendMessage2ToAll(LANG_BG_BFG_NODE_CLAIMED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node), LANG_BG_BFG_HORDE);

        sound = GILNEAS_BG_SOUND_NODE_CLAIMED;
    }
    // If node is contested
    else if ((m_Nodes[node] == GILNEAS_BG_NODE_STATUS_ALLY_CONTESTED) || (m_Nodes[node] == GILNEAS_BG_NODE_STATUS_HORDE_CONTESTED))
    {
        // If last state is NOT occupied, change node to enemy-contested
        if (m_prevNodes[node] < GILNEAS_BG_NODE_TYPE_OCCUPIED)
        {
            UpdatePlayerScore(source, nullptr, SCORE_BASES_ASSAULTED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + GILNEAS_BG_NODE_TYPE_CONTESTED;

            // create new contested banner
            _ChangeBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, teamIndex);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = GILNEAS_BG_FLAG_CAPTURING_TIME;

            // FIXME: need to fix Locales for team and node names.
            if (teamIndex == BG_TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        // If contested, change back to occupied
        else
        {
            UpdatePlayerScore(source, nullptr, SCORE_BASES_DEFENDED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + GILNEAS_BG_NODE_TYPE_OCCUPIED;

            // create new occupied banner
            _ChangeBanner(node, GILNEAS_BG_NODE_TYPE_OCCUPIED, teamIndex);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = 0;
            _NodeOccupied(node, (teamIndex == BG_TEAM_ALLIANCE) ? ALLIANCE:HORDE);

            // FIXME: need to fix Locales for team and node names.
            if (teamIndex == BG_TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_BFG_NODE_DEFENDED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_BFG_NODE_DEFENDED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        sound = (teamIndex == BG_TEAM_ALLIANCE) ? GILNEAS_BG_SOUND_NODE_ASSAULTED_ALLIANCE : GILNEAS_BG_SOUND_NODE_ASSAULTED_HORDE;
    }
    // If node is occupied, change to enemy-contested
    else
    {
        UpdatePlayerScore(source, nullptr, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + GILNEAS_BG_NODE_TYPE_CONTESTED;

        // create new contested banner
        _ChangeBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, teamIndex);
        _SendNodeUpdate(node);
        _NodeDeOccupied(node);
        m_NodeTimers[node] = GILNEAS_BG_FLAG_CAPTURING_TIME;

        // FIXME: need to fix Locales for team and node names.
        if (teamIndex == BG_TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));

        sound = (teamIndex == BG_TEAM_ALLIANCE) ? GILNEAS_BG_SOUND_NODE_ASSAULTED_ALLIANCE : GILNEAS_BG_SOUND_NODE_ASSAULTED_HORDE;
    }

    // If node is occupied again, send "X has taken the Y" msg.
    if (m_Nodes[node] >= GILNEAS_BG_NODE_TYPE_OCCUPIED)
    {
        // FIXME: need to fix Locales for team and node names.
        if (teamIndex == BG_TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, LANG_BG_BFG_ALLY, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_HORDE, NULL, LANG_BG_BFG_HORDE, _GetNodeNameId(node));
    }
    PlaySoundToAll(sound);
}

bool BattlegroundBFG::SetupBattleground()
{
    if (!AddObject(GILNEAS_BG_OBJECT_BANNER_NEUTRAL, GILNEAS_BG_OBJECTID_NODE_BANNER_0,  GILNEAS_BG_NodePositions[0][0], GILNEAS_BG_NodePositions[0][1], GILNEAS_BG_NodePositions[0][2], GILNEAS_BG_NodePositions[0][3], 0, 0, sin(GILNEAS_BG_NodePositions[0][3] / 2), cos(GILNEAS_BG_NodePositions[0][3] / 2), RESPAWN_ONE_DAY)
        || !AddObject(GILNEAS_BG_OBJECT_BANNER_NEUTRAL + 1, GILNEAS_BG_OBJECTID_NODE_BANNER_1, GILNEAS_BG_NodePositions[1][0], GILNEAS_BG_NodePositions[1][1], GILNEAS_BG_NodePositions[1][2], GILNEAS_BG_NodePositions[1][3], 0, 0, sin(GILNEAS_BG_NodePositions[1][3] / 2), cos(GILNEAS_BG_NodePositions[1][3] / 2), RESPAWN_ONE_DAY)
        || !AddObject(GILNEAS_BG_OBJECT_BANNER_NEUTRAL + 2, GILNEAS_BG_OBJECTID_NODE_BANNER_2, GILNEAS_BG_NodePositions[2][0], GILNEAS_BG_NodePositions[2][1], GILNEAS_BG_NodePositions[2][2], GILNEAS_BG_NodePositions[2][3], 0, 0, sin(GILNEAS_BG_NodePositions[2][3] / 2), cos(GILNEAS_BG_NodePositions[2][3] / 2), RESPAWN_ONE_DAY))
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattleForGilneas: Can't Create Some Object");
        return false;
    }

    if (!AddObject(GILNEAS_BG_OBJECT_GATE_A_1, GILNEAS_BG_OBJECTID_GATE_A_1, GILNEAS_BG_DoorPositions[0][0], GILNEAS_BG_DoorPositions[0][1], GILNEAS_BG_DoorPositions[0][2], GILNEAS_BG_DoorPositions[0][3], GILNEAS_BG_DoorPositions[0][4], GILNEAS_BG_DoorPositions[0][5], GILNEAS_BG_DoorPositions[0][6], GILNEAS_BG_DoorPositions[0][7], RESPAWN_IMMEDIATELY)
        || !AddObject(GILNEAS_BG_OBJECT_GATE_H_1, GILNEAS_BG_OBJECTID_GATE_H_1, GILNEAS_BG_DoorPositions[2][0], GILNEAS_BG_DoorPositions[2][1], GILNEAS_BG_DoorPositions[2][2], GILNEAS_BG_DoorPositions[2][3], GILNEAS_BG_DoorPositions[2][4], GILNEAS_BG_DoorPositions[2][5], GILNEAS_BG_DoorPositions[2][6], GILNEAS_BG_DoorPositions[2][7], RESPAWN_IMMEDIATELY))
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattleForGilneas: Can't Create Doors");
        return false;
    }

    // Buffs
    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        if (!AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i, Buff_Entries[0], GILNEAS_BG_BuffPositions[i][0], GILNEAS_BG_BuffPositions[i][1], GILNEAS_BG_BuffPositions[i][2], GILNEAS_BG_BuffPositions[i][3], 0, 0, sin(GILNEAS_BG_BuffPositions[i][3]/2), cos(GILNEAS_BG_BuffPositions[i][3]/2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i + 1, Buff_Entries[1], GILNEAS_BG_BuffPositions[i][0], GILNEAS_BG_BuffPositions[i][1], GILNEAS_BG_BuffPositions[i][2], GILNEAS_BG_BuffPositions[i][3], 0, 0, sin(GILNEAS_BG_BuffPositions[i][3]/2), cos(GILNEAS_BG_BuffPositions[i][3]/2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i + 2, Buff_Entries[2], GILNEAS_BG_BuffPositions[i][0], GILNEAS_BG_BuffPositions[i][1], GILNEAS_BG_BuffPositions[i][2], GILNEAS_BG_BuffPositions[i][3], 0, 0, sin(GILNEAS_BG_BuffPositions[i][3]/2), cos(GILNEAS_BG_BuffPositions[i][3]/2), RESPAWN_ONE_DAY))
            sLog->outError(LOG_FILTER_BATTLEGROUND, "BattleForGilneas: Can't Create Buffs");
    }

    return true;
}

void BattlegroundBFG::Reset()
{
    //call parent's class reset
    Battleground::Reset();

    m_TeamScores[BG_TEAM_ALLIANCE]               = 0;
    m_TeamScores[BG_TEAM_HORDE]                  = 0;
    m_lastTick[BG_TEAM_ALLIANCE]                 = 0;
    m_lastTick[BG_TEAM_HORDE]                    = 0;
    m_HonorScoreTicks[BG_TEAM_ALLIANCE]          = 0;
    m_HonorScoreTicks[BG_TEAM_HORDE]             = 0;
    m_ReputationScoreTicks[BG_TEAM_ALLIANCE]     = 0;
    m_ReputationScoreTicks[BG_TEAM_HORDE]        = 0;
    m_IsInformedNearVictory                      = false;
    bool isBGWeekend = sBattlegroundMgr->IsBGWeekend(GetTypeID());
    m_HonorTicks = (isBGWeekend) ? GILNEAS_BG_BGWeekendHonorTicks : GILNEAS_BG_NotBGWeekendHonorTicks;
    m_ReputationTicks = (isBGWeekend) ? GILNEAS_BG_BGWeekendRepTicks : GILNEAS_BG_NotBGWeekendRepTicks;
    m_TeamScores500Disadvantage[BG_TEAM_ALLIANCE] = false;
    m_TeamScores500Disadvantage[BG_TEAM_HORDE]    = false;

    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        m_Nodes[i]       = 0;
        m_prevNodes[i]   = 0;
        m_NodeTimers[i]  = 0;
    }

    for (uint8 i = 0; i < GILNEAS_BG_ALL_NODES_COUNT + 3; ++i)// +3 for aura triggers
        if (BgCreatures[i])
            DelCreature(i);
}

void BattlegroundBFG::UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(p_Source->GetGUID());

    if (itr == PlayerScores.end())      // player was not found...
        return;

    switch (p_Type)
    {
        case SCORE_BASES_ASSAULTED:
            ((BattlegroundBFGScore*)itr->second)->BasesAssaulted += p_Value;
            p_Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, BG_OBJECTIVE_ASSAULT_BASE);
            break;
        case SCORE_BASES_DEFENDED:
            ((BattlegroundBFGScore*)itr->second)->BasesDefended += p_Value;
            p_Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, BG_OBJECTIVE_DEFEND_BASE);
            break;
        default:
            Battleground::UpdatePlayerScore(p_Source, p_Victim, p_Type, p_Value, p_DoAddHonor, p_RewardType);
            break;
    }
}

void BattlegroundBFG::EndBattleground(uint32 winner)
{
    // Win reward
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);

    // Complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    Battleground::EndBattleground(winner);
}

WorldSafeLocsEntry const* BattlegroundBFG::GetClosestGraveYard(Player* player)
{
    BattlegroundTeamId teamIndex = GetTeamIndexByTeamId(player->GetBGTeam());

    // Is there any occupied node for this team?
    std::vector<uint8> nodes;
    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == teamIndex + 3)
            nodes.push_back(i);

    WorldSafeLocsEntry const* good_entry = NULL;

    // If so, select the closest node to place ghost on
    if (!nodes.empty())
    {
        float player_x = player->GetPositionX();
        float player_y = player->GetPositionY();

        float mindist = 999999.0f; // Temp Hack
        for (uint8 i = 0; i < nodes.size(); ++i)
        {
            WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(GILNEAS_BG_GraveyardIds[nodes[i]]);

            if (!entry)
                continue;

            float dist = (entry->x - player_x)*(entry->x - player_x)+(entry->y - player_y)*(entry->y - player_y);

            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
        nodes.clear();
    }

    // If not, place ghost on starting location
    if (!good_entry)
        good_entry = sWorldSafeLocsStore.LookupEntry(GILNEAS_BG_GraveyardIds[teamIndex+3]);

    return good_entry;
}

bool BattlegroundBFG::IsAllNodesControlledByTeam(uint32 team) const
{
    uint32 count = 0;
    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if ((team == ALLIANCE && m_Nodes[i] == GILNEAS_BG_NODE_STATUS_ALLY_OCCUPIED) || (team == HORDE && m_Nodes[i] == GILNEAS_BG_NODE_STATUS_HORDE_OCCUPIED))
            ++count;

    return count == GILNEAS_BG_DYNAMIC_NODES_COUNT;
}
