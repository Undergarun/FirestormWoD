/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Battleground.h"
#include "BattlegroundSA.h"
#include "Language.h"
#include "Player.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "ScriptMgr.h"

BattlegroundSA::BattlegroundSA()
{
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_SA_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_SA_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_SA_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_SA_HAS_BEGUN;
    BgObjects.resize(BG_SA_MAXOBJ);
    BgCreatures.resize(BG_SA_MAXNPC + BG_SA_MAX_GY);
    TimerEnabled = false;
    UpdateWaitTimer = 0;
    m_EndTimestamp  = 0;
    SignaledRoundTwo = false;
    SignaledRoundTwoHalfMin = false;
    InitSecondRound = false;

    Attackers = ((urand(0, 1)) ? TEAM_ALLIANCE : TEAM_HORDE);

    //! This is here to prevent an uninitialised variable warning
    //! The warning only occurs when SetUpBattleGround fails though.
    //! In the future this function should be called BEFORE sending initial worldstates.
    memset(&GraveyardStatus, 0, sizeof(GraveyardStatus));
}

BattlegroundSA::~BattlegroundSA()
{
}

void BattlegroundSA::Reset()
{
    TotalTime = 0;
    for (uint8 i = 0; i <= 5; i++)
        GateStatus[i] = BG_SA_GATE_OK;
    ShipsStarted = false;
    gateDestroyed = false;
    _notEvenAScratch[TEAM_ALLIANCE] = true;
    _notEvenAScratch[TEAM_HORDE] = true;
    Status = BG_SA_WARMUP;
}

void BattlegroundSA::GetTeamStartLoc(uint32 p_TeamID, float &p_PositionX, float &p_PositionY, float &p_PositionZ, float &p_Orientation) const
{
    uint32 l_AttackersTeam = Attackers == TEAM_ALLIANCE ? ALLIANCE : HORDE;

    if (GetStatus() != STATUS_IN_PROGRESS)
    {
        if (p_TeamID == l_AttackersTeam)
        {
            p_PositionX = 2956.101f;
            p_PositionY = -513.759f;
            p_PositionZ = 9.245f;
            p_Orientation = 2.932f;
        }
        else
        {
            p_PositionX = 1209.7f;
            p_PositionY = -65.16f;
            p_PositionZ =  70.1f;
            p_Orientation = 0.0f;
        }
    }
    else
    {
        if (p_TeamID != l_AttackersTeam)
        {
            p_PositionX = 1209.7f;
            p_PositionY = -65.16f;
            p_PositionZ = 70.1f;
            p_Orientation = 0.0f;
        }
        else
        {
            if (rand() % 2)
            {
                p_PositionX = 1600.381f;
                p_PositionY = -106.263f;
                p_PositionZ = 8.8745f;
                p_Orientation = 3.78f;
            }
            else
            {
                p_PositionX = 1601.5917f;
                p_PositionY = 51.205f;
                p_PositionZ = 7.822f;
                p_Orientation = 2.61f;
            }
        }
    }
}

bool BattlegroundSA::SetupBattleground()
{
    return ResetObjs();
}

bool BattlegroundSA::ResetObjs()
{
    uint32 atF = BG_SA_Factions[Attackers];
    uint32 defF = BG_SA_Factions[Attackers ? TEAM_ALLIANCE : TEAM_HORDE];

    for (uint8 i = 0; i < BG_SA_MAXOBJ; i++)
        DelObject(i);

    for (uint8 i = 0; i < BG_SA_MAXNPC; i++)
        DelCreature(i);

    for (uint8 i = BG_SA_MAXNPC; i < BG_SA_MAXNPC + BG_SA_MAX_GY; i++)
        DelCreature(i);

    for (uint8 i = 0; i < 6; i++)
        GateStatus[i] = BG_SA_GATE_OK;

    for (uint8 i = 0; i <= BG_SA_PORTAL_DEFFENDER_RED; i++)
    {
        if (!AddObject(i, BG_SA_ObjEntries[i], BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1], BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3], 0, 0, 0, 0, RESPAWN_ONE_DAY))
            return false;
    }

    /*for (uint8 i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
    {
        uint32 l_BoatId = 0;
        switch (i)
        {
            case BG_SA_BOAT_ONE:
                l_BoatId = Attackers ? BG_SA_BOAT_ONE_H : BG_SA_BOAT_ONE_A;
                break;
            case BG_SA_BOAT_TWO:
                l_BoatId = Attackers ? BG_SA_BOAT_TWO_H : BG_SA_BOAT_TWO_A;
                break;
            default:
                break;
        }

        if (!AddObject(i, l_BoatId, BG_SA_ObjSpawnlocs[i][0],
          BG_SA_ObjSpawnlocs[i][1],
          BG_SA_ObjSpawnlocs[i][2] + (Attackers ? -3.750f : 0),
          BG_SA_ObjSpawnlocs[i][3], 0, 0, 0, 0, RESPAWN_ONE_DAY))
            return false;
    }*/

    for (uint8 i = BG_SA_SIGIL_1; i <= BG_SA_LEFT_FLAGPOLE; i++)
    {
        if (!AddObject(i, BG_SA_ObjEntries[i],
            BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1],
            BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3],
            0, 0, 0, 0, RESPAWN_ONE_DAY))
        return false;
    }

    /// MAD props for Kiper for discovering those values - 4 hours of his work.
    /*if (GameObject* l_BoatOne = GetBGObject(BG_SA_BOAT_ONE))
    {
        if (l_BoatOne->GetCustomFlags() & eGoBCustomFlags::CustomFlagUseQuaternion)
            l_BoatOne->SetRotationAngles(acosf(0.0002f), 0.f, 0.f);
        else
            l_BoatOne->UpdateRotationFields(1.0f, 0.0002f);

        /// - Don't know why we need to set transport 'active' to stop them, and stopped to move them ...
        l_BoatOne->SetGoState(GO_STATE_TRANSPORT_ACTIVE);
    }

    if (GameObject* l_BoatTwo = GetBGObject(BG_SA_BOAT_ONE))
    {
        if (l_BoatTwo->GetCustomFlags() & eGoBCustomFlags::CustomFlagUseQuaternion)
            l_BoatTwo->SetRotationAngles(acosf(0.00001f), 0.f, 0.f);
        else
            l_BoatTwo->UpdateRotationFields(1.0f, 0.00001f);

        /// - Don't know why we need to set transport 'active' to stop them, and stopped to move them ...
        l_BoatTwo->SetGoState(GO_STATE_TRANSPORT_ACTIVE);
    }

    SpawnBGObject(BG_SA_BOAT_ONE, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_SA_BOAT_TWO, RESPAWN_IMMEDIATELY);*/

    //Cannons and demolishers - NPCs are spawned
    //By capturing GYs.
    for (uint8 i = 0; i < BG_SA_DEMOLISHER_5; i++)
    {
        if (!AddCreature(BG_SA_NpcEntries[i], i, (Attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
              BG_SA_NpcSpawnlocs[i][0], BG_SA_NpcSpawnlocs[i][1],
              BG_SA_NpcSpawnlocs[i][2], BG_SA_NpcSpawnlocs[i][3], 600))
        return false;
    }

    OverrideGunFaction();
    DemolisherStartState(true);

    for (uint8 i = 0; i <= BG_SA_PORTAL_DEFFENDER_RED; i++)
    {
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, defF);
    }

    GetBGObject(BG_SA_TITAN_RELIC)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, atF);
    GetBGObject(BG_SA_TITAN_RELIC)->Refresh();

    for (uint8 i = 0; i <= 5; i++)
        GateStatus[i] = BG_SA_GATE_OK;

    TotalTime = 0;
    ShipsStarted = false;
    gateDestroyed = false;

    //Graveyards
    for (uint8 i = 0; i < BG_SA_MAX_GY; i++)
    {
        WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(BG_SA_GYEntries[i]);
        if (!sg)
            return false;

        if (i == BG_SA_BEACH_GY)
        {
            GraveyardStatus[i] = Attackers;
            AddSpiritGuide(i + BG_SA_MAXNPC, sg->x, sg->y, sg->z, BG_SA_GYOrientation[i], ((Attackers == TEAM_HORDE)? HORDE : ALLIANCE));
        }
        else
        {
            GraveyardStatus[i] = ((Attackers == TEAM_HORDE)? TEAM_ALLIANCE : TEAM_HORDE);
            AddSpiritGuide(i + BG_SA_MAXNPC, sg->x, sg->y, sg->z, BG_SA_GYOrientation[i], ((Attackers == TEAM_HORDE)? ALLIANCE : HORDE));
        }
    }

    //GY capture points
    for (uint8 i = BG_SA_CENTRAL_FLAG; i <= BG_SA_LEFT_FLAG; i++)
    {
        AddObject(i, (BG_SA_ObjEntries[i] - (Attackers == TEAM_ALLIANCE ? 1 : 0)),
            BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1],
            BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3],
            0, 0, 0, 0, RESPAWN_ONE_DAY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, atF);
    }

    for (uint8 i = BG_SA_BOMB; i < BG_SA_MAXOBJ; i++)
    {
        AddObject(i, BG_SA_ObjEntries[BG_SA_BOMB],
            BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1],
            BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3],
            0, 0, 0, 0, RESPAWN_ONE_DAY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, atF);
        if (i > 46)
            GetBGObject(i)->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
    }

    //Player may enter BEFORE we set up BG - lets update his worldstates anyway...
    UpdateWorldState(BG_SA_RIGHT_GY_HORDE, GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    UpdateWorldState(BG_SA_LEFT_GY_HORDE, GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    UpdateWorldState(BG_SA_CENTER_GY_HORDE, GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);

    UpdateWorldState(BG_SA_RIGHT_GY_ALLIANCE, GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_LEFT_GY_ALLIANCE, GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_CENTER_GY_ALLIANCE, GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);

    if (Attackers == TEAM_ALLIANCE)
    {
        UpdateWorldState(BG_SA_ALLY_ATTACKS, 1);
        UpdateWorldState(BG_SA_HORDE_ATTACKS, 0);

        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_ALL, 1);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_ALL, 1);
        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_HRD, 0);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_HRD, 0);

        UpdateWorldState(BG_SA_HORDE_DEFENCE_TOKEN, 1);
        UpdateWorldState(BG_SA_ALLIANCE_DEFENCE_TOKEN, 0);
    }
    else
    {
        UpdateWorldState(BG_SA_HORDE_ATTACKS, 1);
        UpdateWorldState(BG_SA_ALLY_ATTACKS, 0);

        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_ALL, 0);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_ALL, 0);
        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_HRD, 1);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_HRD, 1);

        UpdateWorldState(BG_SA_HORDE_DEFENCE_TOKEN, 0);
        UpdateWorldState(BG_SA_ALLIANCE_DEFENCE_TOKEN, 1);
    }

    UpdateWorldState(BG_SA_PURPLE_GATEWS, 1);
    UpdateWorldState(BG_SA_RED_GATEWS, 1);
    UpdateWorldState(BG_SA_BLUE_GATEWS, 1);
    UpdateWorldState(BG_SA_GREEN_GATEWS, 1);
    UpdateWorldState(BG_SA_YELLOW_GATEWS, 1);
    UpdateWorldState(BG_SA_ANCIENT_GATEWS, 1);

    // set status manually so preparation is cast correctly in 2nd round too
    SetStatus(STATUS_WAIT_JOIN);

    TeleportPlayers();
    return true;
}

void BattlegroundSA::StartShips()
{
    if (ShipsStarted)
        return;

    /// - Don't ask why
    //GetBGObject(BG_SA_BOAT_ONE)->SetTransportState(GO_STATE_TRANSPORT_STOPPED);
    //GetBGObject(BG_SA_BOAT_TWO)->SetTransportState(GO_STATE_TRANSPORT_STOPPED);

    ShipsStarted = true;
}

void BattlegroundSA::PostUpdateImpl(uint32 diff)
{
    if (InitSecondRound)
    {
        if (UpdateWaitTimer < diff)
        {
            if (!SignaledRoundTwo)
            {
                SignaledRoundTwo = true;
                InitSecondRound = false;
                SendMessageToAll(LANG_BG_SA_ROUND_TWO_ONE_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                StartShips();
            }
        }
        else
        {
            UpdateWaitTimer -= diff;
            return;
        }
    }

    TotalTime += diff;

    if (Status == BG_SA_WARMUP )
    {
        EndRoundTimer = BG_SA_ROUNDLENGTH;
        if (TotalTime >= BG_SA_WARMUPLENGTH)
        {
            TotalTime = 0;
            ToggleTimer();
            DemolisherStartState(false);
            Status = BG_SA_ROUND_ONE;
            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, (Attackers == TEAM_ALLIANCE) ? 23748 : 21702);
            SetStatus(STATUS_IN_PROGRESS);

            /// Teleports players on boat to beach
            for (BattlegroundPlayerMap::const_iterator l_Itr = GetPlayers().begin(); l_Itr != GetPlayers().end(); ++l_Itr)
            {
                if (Player* l_Player = ObjectAccessor::FindPlayer(l_Itr->first))
                {
                    uint32 l_AttackerTeam = Attackers == TEAM_ALLIANCE ? ALLIANCE : HORDE;
                    if (l_Player->GetBGTeam() != l_AttackerTeam)
                        continue;

                    float l_PositionX = 0.0f;
                    float l_PositionY = 0.0f;
                    float l_PositionZ = 0.0f;
                    float l_Orientation = 0.0f;
                    GetTeamStartLoc(l_AttackerTeam, l_PositionX, l_PositionY, l_PositionZ, l_Orientation);

                    l_Player->TeleportTo(607, l_PositionX, l_PositionY, l_PositionZ, l_Orientation);
                }
            }

            m_EndTimestamp = time(nullptr) + BG_SA_ROUNDLENGTH / IN_MILLISECONDS;
            UpdateWorldState(BG_SA_ENABLE_TIMER, 1);
            UpdateWorldState(BG_SA_TIMER, m_EndTimestamp);
        }

        if (TotalTime >= BG_SA_BOAT_START)
            StartShips();
        return;
    }
    else if (Status == BG_SA_SECOND_WARMUP)
    {
        if (RoundScores[0].time < BG_SA_ROUNDLENGTH)
            EndRoundTimer = RoundScores[0].time;
        else
            EndRoundTimer = BG_SA_ROUNDLENGTH;

        if (TotalTime >= 60000)
        {
            SendWarningToAll(LANG_BG_SA_HAS_BEGUN);
            TotalTime = 0;
            ToggleTimer();
            DemolisherStartState(false);
            Status = BG_SA_ROUND_TWO;
            m_EndTimestamp = time(nullptr) + EndRoundTimer / IN_MILLISECONDS;

            UpdateWorldState(BG_SA_ENABLE_TIMER, 1);
            UpdateWorldState(BG_SA_TIMER, m_EndTimestamp);
            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, (Attackers == TEAM_ALLIANCE) ? 23748 : 21702);
            // status was set to STATUS_WAIT_JOIN manually for Preparation, set it back now
            SetStatus(STATUS_IN_PROGRESS);

            /// Teleports players on boat to beach
            for (BattlegroundPlayerMap::const_iterator l_Itr = GetPlayers().begin(); l_Itr != GetPlayers().end(); ++l_Itr)
            {
                if (Player* l_Player = ObjectAccessor::FindPlayer(l_Itr->first))
                {
                    uint32 l_AttackerTeam = Attackers == TEAM_ALLIANCE ? ALLIANCE : HORDE;
                    if (l_Player->GetBGTeam() != l_AttackerTeam)
                        continue;

                    float l_PositionX = 0.0f;
                    float l_PositionY = 0.0f;
                    float l_PositionZ = 0.0f;
                    float l_Orientation = 0.0f;
                    GetTeamStartLoc(l_AttackerTeam, l_PositionX, l_PositionY, l_PositionZ, l_Orientation);

                    l_Player->TeleportTo(607, l_PositionX, l_PositionY, l_PositionZ, l_Orientation);
                }
            }

            for (BattlegroundPlayerMap::const_iterator l_Itr = GetPlayers().begin(); l_Itr != GetPlayers().end(); ++l_Itr)
                if (Player* p = ObjectAccessor::FindPlayer(l_Itr->first))
                {
                    p->ModifyAuraState(AURA_STATE_PVP_RAID_PREPARE, false);
                    p->RemoveAurasDueToSpell(SPELL_PREPARATION);
                }
        }
        if (TotalTime >= 30000)
        {
            if (!SignaledRoundTwoHalfMin)
            {
                SignaledRoundTwoHalfMin = true;
                SendMessageToAll(LANG_BG_SA_ROUND_TWO_START_HALF_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            }
        }

        return;
    }
    else if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (Status == BG_SA_ROUND_ONE)
        {
            if (TotalTime >= BG_SA_ROUNDLENGTH)
            {
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        player->CastSpell(player, 52459, true);

                RoundScores[0].winner = Attackers;
                RoundScores[0].time = BG_SA_ROUNDLENGTH;
                TotalTime = 0;
                Status = BG_SA_SECOND_WARMUP;
                m_EndTimestamp = 0;
                UpdateWorldState(BG_SA_ENABLE_TIMER, 0);
                UpdateWorldState(BG_SA_TIMER, 0);
                Attackers = (Attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
                UpdateWaitTimer = 5000;
                SignaledRoundTwo = false;
                SignaledRoundTwoHalfMin = false;
                InitSecondRound = true;
                ToggleTimer();
                ResetObjs();
                return;
            }
        }
        else if (Status == BG_SA_ROUND_TWO)
        {
            if (TotalTime >= EndRoundTimer)
            {
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        player->CastSpell(player, 52459, true);

                m_EndTimestamp = 0;
                UpdateWorldState(BG_SA_ENABLE_TIMER, 0);
                UpdateWorldState(BG_SA_TIMER, 0);

                RoundScores[1].time = BG_SA_ROUNDLENGTH;
                RoundScores[1].winner = (Attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
                if (RoundScores[0].time == RoundScores[1].time)
                    EndBattleground(0);
                else if (RoundScores[0].time < RoundScores[1].time)
                    EndBattleground(RoundScores[0].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
                else
                    EndBattleground(RoundScores[1].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
                return;
            }
        }
        if (Status == BG_SA_ROUND_ONE || Status == BG_SA_ROUND_TWO)
        {
            SendTime();
            UpdateDemolisherSpawns();
        }
    }
}

void BattlegroundSA::StartingEventCloseDoors()
{
}

void BattlegroundSA::StartingEventOpenDoors()
{
}

void BattlegroundSA::FillInitialWorldStates(ByteBuffer& p_Data)
{
    uint32 l_AllyAttacks  = uint32(Attackers == TEAM_ALLIANCE ? 1 : 0);
    uint32 l_HordeAttacks = uint32(Attackers == TEAM_HORDE ? 1 : 0);

    p_Data << uint32(BG_SA_ANCIENT_GATEWS)         << uint32(GateStatus[BG_SA_ANCIENT_GATE]);
    p_Data << uint32(BG_SA_YELLOW_GATEWS)          << uint32(GateStatus[BG_SA_YELLOW_GATE]);
    p_Data << uint32(BG_SA_GREEN_GATEWS)           << uint32(GateStatus[BG_SA_GREEN_GATE]);
    p_Data << uint32(BG_SA_BLUE_GATEWS)            << uint32(GateStatus[BG_SA_BLUE_GATE]);
    p_Data << uint32(BG_SA_RED_GATEWS)             << uint32(GateStatus[BG_SA_RED_GATE]);
    p_Data << uint32(BG_SA_PURPLE_GATEWS)          << uint32(GateStatus[BG_SA_PURPLE_GATE]);

    p_Data << uint32(BG_SA_HORDE_ATTACKS)          << uint32(l_HordeAttacks);
    p_Data << uint32(BG_SA_ALLY_ATTACKS)           << uint32(l_AllyAttacks);

    p_Data << uint32(BG_SA_RIGHT_GY_HORDE)         << uint32(GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_HORDE?1:0);
    p_Data << uint32(BG_SA_LEFT_GY_HORDE)          << uint32(GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_HORDE?1:0);
    p_Data << uint32(BG_SA_CENTER_GY_HORDE)        << uint32(GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_HORDE?1:0);

    p_Data << uint32(BG_SA_RIGHT_GY_ALLIANCE)      << uint32(GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_ALLIANCE?1:0);
    p_Data << uint32(BG_SA_LEFT_GY_ALLIANCE)       << uint32(GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_ALLIANCE?1:0);
    p_Data << uint32(BG_SA_CENTER_GY_ALLIANCE)     << uint32(GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_ALLIANCE?1:0);

    p_Data << uint32(BG_SA_HORDE_DEFENCE_TOKEN)    << uint32(l_AllyAttacks);
    p_Data << uint32(BG_SA_ALLIANCE_DEFENCE_TOKEN) << uint32(l_HordeAttacks);

    p_Data << uint32(BG_SA_LEFT_ATT_TOKEN_HRD)     << uint32(l_HordeAttacks);
    p_Data << uint32(BG_SA_RIGHT_ATT_TOKEN_HRD)    << uint32(l_HordeAttacks);
    p_Data << uint32(BG_SA_RIGHT_ATT_TOKEN_ALL)    << uint32(l_AllyAttacks);
    p_Data << uint32(BG_SA_LEFT_ATT_TOKEN_ALL)     << uint32(l_AllyAttacks);

    if (m_EndTimestamp)
    {
        p_Data << uint32(BG_SA_ENABLE_TIMER) << uint32(1);
        p_Data << uint32(BG_SA_TIMER) << uint32(m_EndTimestamp);
    }
}

void BattlegroundSA::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    //create score and add it to map, default values are set in constructor
    BattlegroundSAScore* sc = new BattlegroundSAScore;

    PlayerScores[player->GetGUID()] = sc;

    // Set player on transport if needed
    /*uint32 l_AttackersTeam = Attackers == TEAM_ALLIANCE ? ALLIANCE : HORDE;

    if (l_AttackersTeam == player->GetBGTeam() && Status != BG_SA_ROUND_ONE && Status != BG_SA_ROUND_TWO)
    {
        if (GetBGObject(BG_SA_BOAT_ONE) == nullptr || GetBGObject(BG_SA_BOAT_TWO) == nullptr)
            return;

        float l_BoatOneDistance = player->GetDistance(GetBGObject(BG_SA_BOAT_ONE));
        float l_BoatTwoDistance = player->GetDistance(GetBGObject(BG_SA_BOAT_TWO));

        GameObject* l_Boat = GetBGObject(BG_SA_BOAT_TWO);
        if (l_BoatOneDistance < l_BoatTwoDistance)
            l_Boat = GetBGObject(BG_SA_BOAT_ONE);

        player->m_movementInfo.t_guid = l_Boat->GetGUID();
        player->m_movementInfo.t_seat = -1;
        player->m_movementInfo.t_pos.Relocate(13.9f, 1.23f, 17.22f, 3.1f);
    }*/
}

void BattlegroundSA::RemovePlayer(Player* /*player*/, uint64 guid, uint32 /*team*/)
{
}

void BattlegroundSA::HandleAreaTrigger(Player* /*Source*/, uint32 /*Trigger*/)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
}

void BattlegroundSA::UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return;

    if (type == SCORE_DESTROYED_DEMOLISHER)
        ((BattlegroundSAScore*)itr->second)->demolishers_destroyed += value;
    else if (type == SCORE_DESTROYED_WALL)
        ((BattlegroundSAScore*)itr->second)->gates_destroyed += value;
    else
        Battleground::UpdatePlayerScore(Source, NULL, type, value, doAddHonor);
}

void BattlegroundSA::TeleportPlayers()
{
    for (BattlegroundPlayerMap::const_iterator l_Itr = GetPlayers().begin(); l_Itr != GetPlayers().end(); ++l_Itr)
    {
        if (Player* l_Player = ObjectAccessor::FindPlayer(l_Itr->first))
        {
            l_Player->RemoveAurasDueToSpell(52459);

            // should remove spirit of redemption
            if (l_Player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
                l_Player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

            l_Player->RemoveAurasByType(SPELL_AURA_MOD_FEAR);

            if (!l_Player->isAlive())
            {
                l_Player->ResurrectPlayer(1.0f);
                l_Player->SpawnCorpseBones();
            }

            l_Player->ResetAllPowers();
            l_Player->CombatStopWithPets(true);

            l_Player->ModifyAuraState(AURA_STATE_PVP_RAID_PREPARE, true);
            l_Player->CastSpell(l_Player, SPELL_PREPARATION, true);

            float l_PositionX = 0.0f;
            float l_PositionY = 0.0f;
            float l_PositionZ = 0.0f;
            float l_Orientation = 0.0f;
            GetTeamStartLoc(l_Player->GetBGTeam(), l_PositionX, l_PositionY, l_PositionZ, l_Orientation);

            l_Player->TeleportTo(607, l_PositionX, l_PositionY, l_PositionZ, l_Orientation);
        }
    }
}

void BattlegroundSA::EventPlayerDamagedGO(Player* /*player*/, GameObject* go, uint32 eventType)
{
    if (!go || !go->GetGOInfo())
        return;

    if (eventType == go->GetGOInfo()->building.DamagedEvent)
    {
        uint32 i = getGateIdFromDamagedOrDestroyEventId(eventType);
        GateStatus[i] = BG_SA_GATE_DAMAGED;
        uint32 uws = getWorldStateFromGateId(i);
        if (uws)
            UpdateWorldState(uws, GateStatus[i]);

        SendWarningToAll(LANG_BG_SA_IS_UNDER_ATTACK, go->GetNameForLocaleIdx(sObjectMgr->GetDBCLocaleIndex()));
    }
    else if (eventType == go->GetGOInfo()->building.DestroyedEvent)
    {
        if (go->GetGOInfo()->building.DestroyedEvent == BG_SA_EVENT_ANCIENT_GATE_DESTROYED)
            SendWarningToAll(LANG_BG_SA_CHAMBER_BREACHED);
        else
            SendWarningToAll(LANG_BG_SA_WAS_DESTROYED, go->GetNameForLocaleIdx(sObjectMgr->GetDBCLocaleIndex()));
    }
}

void BattlegroundSA::HandleKillUnit(Creature* creature, Player* killer)
{
    if (creature->GetEntry() == NPC_DEMOLISHER_SA)
    {
        UpdatePlayerScore(killer, SCORE_DESTROYED_DEMOLISHER, 1);
        _notEvenAScratch[Attackers] = false;
    }
}

/*
  You may ask what the fuck does it do?
  Prevents owner overwriting guns faction with own.
 */
void BattlegroundSA::OverrideGunFaction()
{
    if (!BgCreatures[0])
        return;

    for (uint8 i = BG_SA_GUN_1; i <= BG_SA_GUN_10;i++)
    {
        if (Creature* gun = GetBGCreature(i))
            gun->setFaction(BG_SA_Factions[Attackers? TEAM_ALLIANCE : TEAM_HORDE]);
    }

    for (uint8 i = BG_SA_DEMOLISHER_1; i <= BG_SA_DEMOLISHER_4;i++)
    {
        if (Creature* dem = GetBGCreature(i))
            dem->setFaction(BG_SA_Factions[Attackers]);
    }
}

void BattlegroundSA::DemolisherStartState(bool start)
{
    if (!BgCreatures[0])
        return;

    // set flags only for the demolishers on the beach, factory ones dont need it
    for (uint8 i = BG_SA_DEMOLISHER_1; i <= BG_SA_DEMOLISHER_4; i++)
    {
        if (Creature* dem = GetBGCreature(i))
        {
            if (start)
                dem->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            else
                dem->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }
    }
}

void BattlegroundSA::DestroyGate(Player* player, GameObject* go)
{
    uint32 i = getGateIdFromDamagedOrDestroyEventId(go->GetGOInfo()->building.DestroyedEvent);
    if (!GateStatus[i])
        return;

    if (GameObject* g = GetBGObject(i))
    {
        if (g->GetGOValue()->Building.Health == 0)
        {
            GateStatus[i] = BG_SA_GATE_DESTROYED;
            uint32 uws = getWorldStateFromGateId(i);
            if (uws)
                UpdateWorldState(uws, GateStatus[i]);
            bool rewardHonor = true;
            gateDestroyed = true;
            switch (i)
            {
                case BG_SA_GREEN_GATE:
                    if (GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    for (uint8 bombIndex = 47; bombIndex < 59; bombIndex++)
                    {
                        if (GetBGObject(bombIndex))
                            GetBGObject(bombIndex)->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                    }
                    break;
                case BG_SA_BLUE_GATE:
                    if (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    for (uint8 bombIndex = 59; bombIndex < 68; bombIndex++)
                    {
                        if (GetBGObject(bombIndex))
                            GetBGObject(bombIndex)->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                    }
                    break;
                case BG_SA_RED_GATE:
                    if (GateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    for (uint8 bombIndex = 68; bombIndex < 77; bombIndex++)
                    {
                        if (GetBGObject(bombIndex))
                            GetBGObject(bombIndex)->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                    }
                    break;
                case BG_SA_PURPLE_GATE:
                    if (GateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    for (uint8 bombIndex = 68; bombIndex < 77; bombIndex++)
                    {
                        if (GetBGObject(bombIndex))
                            GetBGObject(bombIndex)->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                    }
                    break;
                case BG_SA_YELLOW_GATE:
                    for (uint8 bombIndex = 77; bombIndex < 93; bombIndex++)
                    {
                        if (GetBGObject(bombIndex))
                            GetBGObject(bombIndex)->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                    }
                    break;
                default:
                    break;
            }

            if (i < 5)
                DelObject(i + 14);
            UpdatePlayerScore(player, SCORE_DESTROYED_WALL, 1);
            if (rewardHonor)
                UpdatePlayerScore(player, SCORE_BONUS_HONOR, GetBonusHonorFromKill(1));
        }
    }
}

WorldSafeLocsEntry const* BattlegroundSA::GetClosestGraveYard(Player* player)
{
    uint32 safeloc = 0;
    WorldSafeLocsEntry const* ret;
    WorldSafeLocsEntry const* closest;
    float dist, nearest;
    float x, y, z;

    player->GetPosition(x, y, z);

    if (player->GetTeamId() == Attackers)
        safeloc = BG_SA_GYEntries[BG_SA_BEACH_GY];
    else
        safeloc = BG_SA_GYEntries[BG_SA_DEFENDER_LAST_GY];

    closest = sWorldSafeLocsStore.LookupEntry(safeloc);
    nearest = sqrt((closest->x - x)*(closest->x - x) + (closest->y - y)*(closest->y - y)+(closest->z - z)*(closest->z - z));

    for (uint8 i = BG_SA_RIGHT_CAPTURABLE_GY; i < BG_SA_MAX_GY; i++)
    {
        if (GraveyardStatus[i] != player->GetTeamId())
            continue;

        ret = sWorldSafeLocsStore.LookupEntry(BG_SA_GYEntries[i]);
        dist = sqrt((ret->x - x)*(ret->x - x) + (ret->y - y)*(ret->y - y)+(ret->z - z)*(ret->z - z));
        if (dist < nearest)
        {
            closest = ret;
            nearest = dist;
        }
    }

    return closest;
}

void BattlegroundSA::SendTime()
{
    /*uint32 end_of_round = (EndRoundTimer - TotalTime);
    UpdateWorldState(BG_SA_TIMER_MINS, end_of_round/60000);
    UpdateWorldState(BG_SA_TIMER_SEC_TENS, (end_of_round%60000)/10000);
    UpdateWorldState(BG_SA_TIMER_SEC_DECS, ((end_of_round%60000)%10000)/1000);*/
}

void BattlegroundSA::EventPlayerClickedOnFlag(Player* Source, GameObject* target_obj)
{
    switch (target_obj->GetEntry())
    {
        case 191307:
        case 191308:
            if (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                CaptureGraveyard(BG_SA_LEFT_CAPTURABLE_GY, Source);
            break;
        case 191305:
        case 191306:
            if (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                CaptureGraveyard(BG_SA_RIGHT_CAPTURABLE_GY, Source);
            break;
        case 191310:
        case 191309:
            if ((GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED) && (GateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED))
            {
                CaptureGraveyard(BG_SA_LEFT_CAPTURABLE_GY, Source);
                CaptureGraveyard(BG_SA_RIGHT_CAPTURABLE_GY, Source);
                CaptureGraveyard(BG_SA_CENTRAL_CAPTURABLE_GY, Source);
            }
            break;
        default:
            return;
    };
}

void BattlegroundSA::CaptureGraveyard(BG_SA_Graveyards i, Player* Source)
{
    if (GraveyardStatus[i] == Attackers)
        return;

    DelCreature(BG_SA_MAXNPC + i);
    GraveyardStatus[i] = Source->GetTeamId();
    WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(BG_SA_GYEntries[i]);
    if (!sg)
        return;

    AddSpiritGuide(i + BG_SA_MAXNPC, sg->x, sg->y, sg->z, BG_SA_GYOrientation[i], (GraveyardStatus[i] == TEAM_ALLIANCE?  ALLIANCE : HORDE));
    uint32 npc = 0;
    uint32 flag = 0;

    switch (i)
    {
        case BG_SA_LEFT_CAPTURABLE_GY:
            flag = BG_SA_LEFT_FLAG;
            DelObject(flag);
            AddObject(flag, (BG_SA_ObjEntries[flag] - (Source->GetTeamId() == TEAM_ALLIANCE ? 0:1)),
            BG_SA_ObjSpawnlocs[flag][0], BG_SA_ObjSpawnlocs[flag][1],
            BG_SA_ObjSpawnlocs[flag][2], BG_SA_ObjSpawnlocs[flag][3], 0, 0, 0, 0, RESPAWN_ONE_DAY);

            npc = BG_SA_NPC_RIGSPARK;
            AddCreature(BG_SA_NpcEntries[npc], npc, Attackers,
              BG_SA_NpcSpawnlocs[npc][0], BG_SA_NpcSpawnlocs[npc][1],
              BG_SA_NpcSpawnlocs[npc][2], BG_SA_NpcSpawnlocs[npc][3]);

            for (uint8 j = BG_SA_DEMOLISHER_7; j <= BG_SA_DEMOLISHER_8; j++)
            {
                AddCreature(BG_SA_NpcEntries[j], j, (Attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
                    BG_SA_NpcSpawnlocs[j][0], BG_SA_NpcSpawnlocs[j][1],
                    BG_SA_NpcSpawnlocs[j][2], BG_SA_NpcSpawnlocs[j][3], 600);

                if (Creature* dem = GetBGCreature(j))
                    dem->setFaction(BG_SA_Factions[Attackers]);
            }

            UpdateWorldState(BG_SA_LEFT_GY_ALLIANCE, (GraveyardStatus[i] == TEAM_ALLIANCE ? 1 : 0));
            UpdateWorldState(BG_SA_LEFT_GY_HORDE, (GraveyardStatus[i] == TEAM_ALLIANCE ? 0 : 1));
            if (Source->GetTeamId() == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_WEST);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_WEST);
            break;
        case BG_SA_RIGHT_CAPTURABLE_GY:
            flag = BG_SA_RIGHT_FLAG;
            DelObject(flag);
            AddObject(flag, (BG_SA_ObjEntries[flag] - (Source->GetTeamId() == TEAM_ALLIANCE ? 0:1)),
              BG_SA_ObjSpawnlocs[flag][0], BG_SA_ObjSpawnlocs[flag][1],
              BG_SA_ObjSpawnlocs[flag][2], BG_SA_ObjSpawnlocs[flag][3], 0, 0, 0, 0, RESPAWN_ONE_DAY);

            npc = BG_SA_NPC_SPARKLIGHT;
            AddCreature(BG_SA_NpcEntries[npc], npc, Attackers,
              BG_SA_NpcSpawnlocs[npc][0], BG_SA_NpcSpawnlocs[npc][1],
              BG_SA_NpcSpawnlocs[npc][2], BG_SA_NpcSpawnlocs[npc][3]);

            for (uint8 j = BG_SA_DEMOLISHER_5; j <= BG_SA_DEMOLISHER_6; j++)
            {
                AddCreature(BG_SA_NpcEntries[j], j, (Attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
                    BG_SA_NpcSpawnlocs[j][0], BG_SA_NpcSpawnlocs[j][1],
                    BG_SA_NpcSpawnlocs[j][2], BG_SA_NpcSpawnlocs[j][3], 600);

                if (Creature* dem = GetBGCreature(j))
                    dem->setFaction(BG_SA_Factions[Attackers]);
            }

            UpdateWorldState(BG_SA_RIGHT_GY_ALLIANCE, (GraveyardStatus[i] == TEAM_ALLIANCE ? 1 : 0));
            UpdateWorldState(BG_SA_RIGHT_GY_HORDE, (GraveyardStatus[i] == TEAM_ALLIANCE ? 0 : 1));
            if (Source->GetTeamId() == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_EAST);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_EAST);
            break;
        case BG_SA_CENTRAL_CAPTURABLE_GY:
            flag = BG_SA_CENTRAL_FLAG;
            DelObject(flag);
            AddObject(flag, (BG_SA_ObjEntries[flag] - (Source->GetTeamId() == TEAM_ALLIANCE ? 0:1)),
              BG_SA_ObjSpawnlocs[flag][0], BG_SA_ObjSpawnlocs[flag][1],
              BG_SA_ObjSpawnlocs[flag][2], BG_SA_ObjSpawnlocs[flag][3], 0, 0, 0, 0, RESPAWN_ONE_DAY);

            UpdateWorldState(BG_SA_CENTER_GY_ALLIANCE, (GraveyardStatus[i] == TEAM_ALLIANCE? 1:0));
            UpdateWorldState(BG_SA_CENTER_GY_HORDE, (GraveyardStatus[i] == TEAM_ALLIANCE? 0:1));
            if (Source->GetTeamId() == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_SOUTH);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_SOUTH);
            break;
        default:
            ASSERT(false);
            break;
    };
}

void BattlegroundSA::EventPlayerUsedGO(Player* Source, GameObject* object)
{
    if (object->GetEntry() == BG_SA_ObjEntries[BG_SA_TITAN_RELIC] && GateStatus[BG_SA_ANCIENT_GATE] == BG_SA_GATE_DESTROYED && GateStatus[BG_SA_YELLOW_GATE] == BG_SA_GATE_DESTROYED && (GateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED) && (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED))
    {
        if (Source->GetTeamId() == Attackers)
        {
            if (Source->GetTeamId() == TEAM_ALLIANCE)
                SendMessageToAll(LANG_BG_SA_ALLIANCE_CAPTURED_RELIC, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            else SendMessageToAll(LANG_BG_SA_HORDE_CAPTURED_RELIC, CHAT_MSG_BG_SYSTEM_NEUTRAL);

            if (Status == BG_SA_ROUND_ONE)
            {
                RoundScores[0].winner = Attackers;
                RoundScores[0].time = TotalTime;
                //Achievement Storm the Beach (1310)
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    {
                        player->CastSpell(player, 52459, true);
                        if (player->GetTeamId() == Attackers)
                            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 65246);
                    }
                }

                Attackers = (Attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
                Status = BG_SA_SECOND_WARMUP;
                m_EndTimestamp = 0;
                UpdateWorldState(BG_SA_ENABLE_TIMER, 0);
                UpdateWorldState(BG_SA_TIMER, 0);
                TotalTime = 0;
                ToggleTimer();
                SendWarningToAll(LANG_BG_SA_ROUND_ONE_END);
                UpdateWaitTimer = 5000;
                SignaledRoundTwo = false;
                SignaledRoundTwoHalfMin = false;
                InitSecondRound = true;
                ResetObjs();
            }
            else if (Status == BG_SA_ROUND_TWO)
            {
                Status = BG_SA_END;

                RoundScores[1].winner = Attackers;
                RoundScores[1].time = TotalTime;
                ToggleTimer();

                //Achievement Storm the Beach (1310)
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        if (player->GetTeamId() == Attackers && RoundScores[1].winner == Attackers)
                            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 65246);
                }

                m_EndTimestamp = 0;
                UpdateWorldState(BG_SA_ENABLE_TIMER, 0);
                UpdateWorldState(BG_SA_TIMER, 0);

                if (RoundScores[0].time == RoundScores[1].time)
                    EndBattleground(0);
                else if (RoundScores[0].time < RoundScores[1].time)
                    EndBattleground(RoundScores[0].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
                else
                    EndBattleground(RoundScores[1].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
            }
        }
    }
}

void BattlegroundSA::ToggleTimer()
{
    TimerEnabled = !TimerEnabled;
    //UpdateWorldState(BG_SA_ENABLE_TIMER, (TimerEnabled) ? 1 : 0);
}

void BattlegroundSA::EndBattleground(uint32 winner)
{
    //honor reward for winning
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    else if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);

    //complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(2), ALLIANCE);
    RewardHonorToTeam(GetBonusHonorFromKill(2), HORDE);

    Battleground::EndBattleground(winner);
}

void BattlegroundSA::UpdateDemolisherSpawns()
{
    for (uint8 i = BG_SA_DEMOLISHER_1; i <= BG_SA_DEMOLISHER_8; i++)
    {
        if (BgCreatures[i])
        {
            if (Creature* Demolisher = GetBGCreature(i))
            {
                if (Demolisher->isDead())
                {
                    // Demolisher is not in list
                    if (DemoliserRespawnList.find(i) == DemoliserRespawnList.end())
                    {
                        DemoliserRespawnList[i] = getMSTime()+30000;
                    }
                    else
                    {
                        if (DemoliserRespawnList[i] < getMSTime())
                        {
                            Demolisher->Relocate(BG_SA_NpcSpawnlocs[i][0], BG_SA_NpcSpawnlocs[i][1],
                                BG_SA_NpcSpawnlocs[i][2], BG_SA_NpcSpawnlocs[i][3]);

                            Demolisher->Respawn();
                            DemoliserRespawnList.erase(i);
                        }
                    }
                }
            }
        }
    }
}

/// - The Graceful Maiden - 193182
/// - The Blightbringer   - 193183
/// - The Coffin Carrier  - 193184
/// - The Frostbreaker    - 193185
class go_sa_boat : public GameObjectScript
{
    public:
        go_sa_boat() : GameObjectScript("go_sa_boat") {}

        bool OnGameObjectElevatorCheck(GameObject const* p_GameObject) const
        {
            return false;
        }
};

void AddSC_BattlegroundSAScripts()
{
    new go_sa_boat();
}