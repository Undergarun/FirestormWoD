////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Battleground.h"
#include "BattlegroundTV.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundTV::BattlegroundTV()
{
    BgObjects.resize(BG_TV_OBJECT_MAX);

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

BattlegroundTV::~BattlegroundTV()
{

}

void BattlegroundTV::StartingEventCloseDoors()
{
    for (uint32 i = BG_TV_OBJECT_DOOR_1; i <= BG_TV_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundTV::StartingEventOpenDoors()
{
    for (uint32 i = BG_TV_OBJECT_DOOR_1; i <= BG_TV_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for (uint32 i = BG_TV_OBJECT_BUFF_1; i <= BG_TV_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 60);
}

void BattlegroundTV::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    //create score and add it to map, default values are set in constructor
    BattlegroundTVScore* sc = new BattlegroundTVScore;

    PlayerScores[player->GetGUID()] = sc;

    UpdateArenaWorldState();
}

void BattlegroundTV::RemovePlayer(Player* /*player*/, uint64 guid, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTV::HandleKillPlayer(Player* player, Player* killer)
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

bool BattlegroundTV::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), -10716.234375f, 429.321808f, 24.411819f, player->GetOrientation(), false);
    return true;
}

void BattlegroundTV::HandleAreaTrigger(Player* Source, uint32 Trigger)
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

void BattlegroundTV::FillInitialWorldStates(ByteBuffer &data)
{
    data << uint32(0xE1A) << uint32(1);
    UpdateArenaWorldState();
}

void BattlegroundTV::Reset()
{
    // Call parent's class reset
    Battleground::Reset();
}

bool BattlegroundTV::SetupBattleground()
{
    // Gates
    if (!AddObject(BG_TV_OBJECT_DOOR_1, BG_TV_OBJECT_TYPE_DOOR_1, -10774.6f, 430.992f, 24.41076f, 0.0156f, 0.0f, 0.0f, 0.0078f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TV_OBJECT_DOOR_2, BG_TV_OBJECT_TYPE_DOOR_2, -10655.0f, 428.117f, 24.416f, 3.148f, 0.0f, 0.0f, 1.0f, RESPAWN_IMMEDIATELY)
    // Buffs
        || !AddObject(BG_TV_OBJECT_BUFF_1, BG_TV_OBJECT_TYPE_BUFF_1, -10717.63f, 383.8223f, 24.412825f, 1.555f, 0.0f, 0.0f, 0.70154f, 120)
        || !AddObject(BG_TV_OBJECT_BUFF_2, BG_TV_OBJECT_TYPE_BUFF_2, -10716.6f, 475.364f, 24.4131f, 0.0f, 0.0f, 0.70068f, -0.713476f, 120))
    {
        sLog->outError(LOG_FILTER_SQL, "BattlegroundTV: Failed to spawn some object!");
        return false;
    }

    return true;
}

/*
5.4.8 - 18291 - SMSG_INIT_WORLD_STATES (5472)
D4030000981800009818000000035000000000C72000000000000051210
0000000000050210000000000004F210000000000004E21000000000000
4D210000000000004C2100000F00000067200000010000004C1F0000000
00000751E000000000000741E0000140000007320000014000000722000
0000000000571D0000000000005E1E0000000000003A1E000001000000F
71D000000000000481E000005000000C11D000005000000C21D00000100
00004D1C0000590000004C1C0000010000004B1C0000590000004A1C000
0000000006E1B0000000000002419000001000000F319000000000000F2
19000000000000F119000000000000E01400000A000000F21A00000A000
000F11A00000A000000EF1A000000000000BE17000000000000CF170000
0100000084150000640000000013000000000000A2180000230000001E1
800002300000014180000190000007B180000000000004B140000000000
00491400000000000034160000000000002F160000000000002E1600000
00000002D1600000000000043110000010000004111000001C184530211
0000010000001711000000000000F114000000000000F01400000100000
0D614000000000000D51400006CBD8453D4140000000000000C02000000
0000000A02000000000000B80F000000000000B50F00000000000095070
00003000000620D000000000000FF0C00000F000000770C000001000000
7E0E000032000000421100003C000000231000000000000086110000000
000008511000006000000AD1300002C010000FF120000E8030000FE1200
00000000004C140000000000004A140000000000004814000000000000F
B13000000000000D608000000000000D3080000000000006F0E00000600
0000CF1300007B0100000D0C000000000000B110000000000000100E000
000000000DE0F0000010000001A0E000000000000230B0000000000000B
0200000000000097070000000000009607000000000000B90F000000000
000B70F000000000000B60F000000000000B40F000000000000110E0000
00000000BC06000000000000BB06000000000000D808000000000000D70
8000000000000D50800008E000000050A00000100000009020000E1A484
53C50E0000030000003D0F000001000000F20E000000000000D90E00000
0000000D4080000
*/
