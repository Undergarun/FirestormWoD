////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Battleground.h"
#include "BattlegroundTTP.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundTTP::BattlegroundTTP()
{
    BgObjects.resize(BG_TTP_OBJECT_MAX);

    StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_1M;
    StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_30S;
    StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_15S;
    StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    //we must set messageIds
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

BattlegroundTTP::~BattlegroundTTP()
{

}

void BattlegroundTTP::StartingEventCloseDoors()
{
    for (uint32 i = BG_TTP_OBJECT_DOOR_1; i <= BG_TTP_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundTTP::StartingEventOpenDoors()
{
    for (uint32 i = BG_TTP_OBJECT_DOOR_1; i <= BG_TTP_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for (uint32 i = BG_TTP_OBJECT_BUFF_1; i <= BG_TTP_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 60);
}

void BattlegroundTTP::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    //create score and add it to map, default values are set in constructor
    BattlegroundTTPScore* sc = new BattlegroundTTPScore;

    PlayerScores[player->GetGUID()] = sc;

    UpdateArenaWorldState();
}

void BattlegroundTTP::RemovePlayer(Player* /*player*/, uint64 /*guid*/, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTTP::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!killer)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundTV: Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

bool BattlegroundTTP::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), 567.313843f, 632.106384f, 380.703339f, player->GetOrientation(), false);
    return true;
}

void BattlegroundTTP::HandleAreaTrigger(Player* Source, uint32 Trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (Trigger)
    {
        case 4536:                                          // buff trigger?
        case 4537:                                          // buff trigger?
            break;
        default:
            break;
    }
}

void BattlegroundTTP::FillInitialWorldStates(ByteBuffer &data)
{
    data << uint32(0xE1A) << uint32(1);
    UpdateArenaWorldState();
}

void BattlegroundTTP::Reset()
{
    // Call parent's class reset
    Battleground::Reset();
}

bool BattlegroundTTP::SetupBattleground()
{
    // Gates
    if (!AddObject(BG_TTP_OBJECT_DOOR_1, BG_TTP_OBJECT_TYPE_DOOR_1, 502.414f, 633.099f, 380.706f, 0.0308292f, 0.0f, 0.0f, 0.015414f, 0.999881f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TTP_OBJECT_DOOR_2, BG_TTP_OBJECT_TYPE_DOOR_2, 632.891f, 633.059f, 380.705f, 3.12778f, 0.0f, 0.0f, 0.999976f, 0.0069045f, RESPAWN_IMMEDIATELY)
    // Buffs
        || !AddObject(BG_TTP_OBJECT_BUFF_1, BG_TTP_OBJECT_TYPE_BUFF_1, 566.788f, 602.743f, 383.68f, 1.5724f, 0.0f, 0.0f, 0.707673f, 0.70654f, 120)
        || !AddObject(BG_TTP_OBJECT_BUFF_2, BG_TTP_OBJECT_TYPE_BUFF_2, 566.661f, 664.311f, 383.681f, 4.66374f, 0.0f, 0.0f, 0.724097f, -0.689698f, 120))
    {
        sLog->outError(LOG_FILTER_SQL, "BattlegroundTTP: Failed to spawn some object!");
        return false;
    }

    return true;
}

/*
5.4.8 - 18291 - SMSG_INIT_WORLD_STATES (5472)
6E0400004C1A00004C1A000000033800000000C72000000000000051210
0000000000050210000000000004F210000000000004E21000000000000
4D210000000000004C2100000F00000067200000010000004C1F0000000
00000751E000000000000741E0000140000007320000014000000722000
0000000000571D0000000000005E1E0000000000003A1E000001000000F
71D000000000000481E000005000000C11D000005000000C21D00000100
00004D1C0000A90000004C1C0000010000004B1C0000A90000004A1C000
0000000006E1B0000000000002419000000000000E01400000A000000F2
1A00000A000000F11A00000A000000EF1A000000000000BE17000000000
000CF1700000100000084150000640000000013000000000000A2180000
230000001E1800002300000014180000190000007B180000000000004B1
4000000000000491400000000000034160000000000002F160000000000
002E160000000000002D160000000000004311000001000000411100000
5C1845302110000000000001711000000000000F114000000000000F014
000002000000D614000000000000D51400006EBD8453D41400000000000
00C020000000000000A02000000000000B80F000000000000B50F000000
0000009507000003000000620D000000000000FF0C00000F000000770C0
000000000007E0E000032000000421100003C0000002310000000000000
86110000000000008511000006000000AD1300002C010000FF120000E80
30000FE120000000000004C140000000000004A14000000000000481400
0000000000FB13000000000000D608000000000000D3080000000000006
F0E000006000000CF1300007B0100000D0C000000000000B11000000000
0000100E000000000000DE0F0000010000001A0E000000000000230B000
0000000000B0200000000000097070000000000009607000000000000B9
0F000000000000B70F000000000000B60F000000000000B40F000000000
000110E000000000000BC06000000000000BB06000000000000D8080000
00000000D708000000000000D50800008E000000050A000001000000090
2000000000000C50E0000030000003D0F000001000000F20E0000010000
00D90E000000000000D4080000
*/
