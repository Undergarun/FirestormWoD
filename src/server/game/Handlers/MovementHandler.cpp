////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AnticheatMgr.h"
#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "SpellAuras.h"
#include "MapManager.h"
#include "Transport.h"
#include "Battleground.h"
#include "WaypointMovementGenerator.h"
#include "InstanceSaveMgr.h"
#include "ObjectMgr.h"
#include "MovementStructures.h"
#include "BattlegroundMgr.hpp"
#include "Battleground.h"

#define MOVEMENT_PACKET_TIME_DELAY 0

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket& /*recvPacket*/)
{
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    // ignore unexpected far teleports
    if (!GetPlayer()->IsBeingTeleportedFar())
        return;

    GetPlayer()->SetSemaphoreTeleportFar(false);
    GetPlayer()->SetIgnoreMovementCount(5);

    // get the teleport destination
    WorldLocation const loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check
    if (!MapManager::IsValidMapCoord(loc))
    {
        LogoutPlayer(false);
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.GetMapId());
    InstanceTemplate const* mInstance = sObjectMgr->GetInstanceTemplate(loc.GetMapId());

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    Map* oldMap = GetPlayer()->GetMap();
    if (GetPlayer()->IsInWorld())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Player (Name %s) is still in world when teleported from map %u to new map %u", GetPlayer()->GetName(), oldMap->GetId(), loc.GetMapId());
        oldMap->RemovePlayerFromMap(GetPlayer(), false);
    }

    // relocate the player to the teleport destination
    Map* newMap = sMapMgr->CreateMap(loc.GetMapId(), GetPlayer());
    // the CanEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!newMap || !newMap->CanEnter(GetPlayer()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Map %d could not be created for player %d, porting player to homebind", loc.GetMapId(), GetPlayer()->GetGUIDLow());
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }
    else
        GetPlayer()->Relocate(&loc);

    GetPlayer()->ResetMap();
    GetPlayer()->SetMap(newMap);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    if (!GetPlayer()->GetMap()->AddPlayerToMap(GetPlayer()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: failed to teleport player %s (%d) to map %d (%s) because of unknown reason!",
            GetPlayer()->GetName(), GetPlayer()->GetGUIDLow(), loc.GetMapId(), newMap ? newMap->GetMapName() : "Unknown");
        GetPlayer()->ResetMap();
        GetPlayer()->SetMap(oldMap);
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // only add to bg group and object, if the player was invited (else he entered through command)
    if (m_Player->InBattleground())
    {
        // cleanup setting if outdated
        if (!mEntry->IsBattlegroundOrArena())
        {
            // We're not in BG
            m_Player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            m_Player->SetBGTeam(0);
        }
        // join to bg case
        else if (Battleground* bg = m_Player->GetBattleground())
        {
            if (m_Player->IsInvitedForBattlegroundInstance(m_Player->GetBattlegroundId()))
            {
                bg->AddPlayer(m_Player);

                /// Remove battleground queue status from BGmgr
                sBattlegroundMgr->RemovePlayer(m_Player->GetGUID(), true, MS::Battlegrounds::GetSchedulerType(bg->GetTypeID()));
            }
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // Update position client-side to avoid undermap
    WorldPacket data(SMSG_MOVE_UPDATE);
    m_Player->m_movementInfo.time = getMSTime();
    m_Player->m_movementInfo.pos.m_positionX = m_Player->m_positionX;
    m_Player->m_movementInfo.pos.m_positionY = m_Player->m_positionY;
    m_Player->m_movementInfo.pos.m_positionZ = m_Player->m_positionZ;
    WorldSession::WriteMovementInfo(data, &m_Player->m_movementInfo);
    m_Player->GetSession()->SendPacket(&data);

    // flight fast teleport case
    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!m_Player->InBattleground())
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Initialize(GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->CleanupAfterTaxiFlight();
    }

    // resurrect character at enter into instance where his corpse exist after add to map
    Corpse* corpse = GetPlayer()->GetCorpse();
    if (corpse && corpse->GetType() != CORPSE_BONES && corpse->GetMapId() == GetPlayer()->GetMapId())
    {
        if (mEntry->IsDungeon())
        {
            GetPlayer()->ResurrectPlayer(0.5f, false);
            GetPlayer()->SpawnCorpseBones();
        }
    }

    bool allowMount = !mEntry->IsDungeon() || mEntry->IsBattlegroundOrArena();
    if (mInstance)
    {
        Difficulty diff = GetPlayer()->GetDifficultyID(mEntry);
        if (MapDifficulty const* mapDiff = GetMapDifficultyData(mEntry->MapID, diff))
        {
            if (mapDiff->ResetTime)
            {
                if (time_t timeReset = sInstanceSaveMgr->GetResetTimeFor(mEntry->MapID, diff))
                {
                    uint32 timeleft = uint32(timeReset - time(NULL));
                    m_Player->SendRaidInstanceMessage(mEntry->MapID, diff, timeleft);
                }
            }
        }
        allowMount = mInstance->AllowMount;
    }

    // mount allow check
    if (!allowMount)
        m_Player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    // update zone immediately, otherwise leave channel will cause crash in mtmap
    uint32 newzone, newarea;
    m_Player->GetZoneAndAreaId(newzone, newarea, true);
    m_Player->UpdateZone(newzone, newarea);

    for (uint8 i = 0; i < 9; ++i)
        GetPlayer()->UpdateSpeed(UnitMoveType(i), true);

    // honorless target
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // in friendly area
    else if (GetPlayer()->IsPvP() && !GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
        GetPlayer()->UpdatePvP(false, false);

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
    GetPlayer()->SummonLastSummonedBattlePet();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAck(WorldPacket& recvPacket)
{
    uint64 l_MoverGUID;
    uint32 l_AckIndex, l_MoveTime;

    recvPacket.readPackGUID(l_MoverGUID);
    recvPacket >> l_AckIndex >> l_MoveTime;

    Player* l_MoverPlayer = m_Player->m_mover->ToPlayer();

    if (!l_MoverPlayer || !l_MoverPlayer->IsBeingTeleportedNear())
        return;

    if (l_MoverGUID != l_MoverPlayer->GetGUID())
        return;

    l_MoverPlayer->SetSemaphoreTeleportNear(false);
    l_MoverPlayer->SetIgnoreMovementCount(5);

    uint32 l_OldZone = l_MoverPlayer->GetZoneId();

    WorldLocation const& l_Destination = l_MoverPlayer->GetTeleportDest();

    l_MoverPlayer->UpdatePosition(l_Destination, true);

    uint32 l_NewZone, l_NewArea;

    l_MoverPlayer->GetZoneAndAreaId(l_NewZone, l_NewArea, true);
    l_MoverPlayer->UpdateZone(l_NewZone, l_NewArea);

    // new zone
    if (l_OldZone != l_NewZone)
    {
        // honorless target
        if (l_MoverPlayer->pvpInfo.inHostileArea)
            l_MoverPlayer->CastSpell(l_MoverPlayer, 2479, true);

        // in friendly area
        else if (l_MoverPlayer->IsPvP() && !l_MoverPlayer->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
            l_MoverPlayer->UpdatePvP(false, false);
    }

    // resummon pet
    l_MoverPlayer->ResummonPetTemporaryUnSummonedIfAny();
    l_MoverPlayer->SummonLastSummonedBattlePet();

    //lets process all delayed operations on successful teleport
    l_MoverPlayer->ProcessDelayedOperations();
}

void WorldSession::HandleMovementOpcodes(WorldPacket& p_Packet)
{
    uint16 l_OpCode = p_Packet.GetOpcode();

    Unit * l_Mover = m_Player->m_mover;

    ASSERT(l_Mover != NULL);                      // there must always be a mover

    Player* l_PlayerMover = l_Mover->ToPlayer();

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (l_PlayerMover && l_PlayerMover->IsBeingTeleported())
    {
        p_Packet.rfinish();                     // prevent warnings spam
        return;
    }

    // Sometime, client send movement packet after teleporation with position before teleportation, so we ignore 3 first movement packet after teleporation
    // TODO: find a better way to check that, may be a new CMSG send by client ?
    if (l_PlayerMover && l_PlayerMover->GetIgnoreMovementCount() && l_OpCode != CMSG_CAST_SPELL)
    {
        l_PlayerMover->SetIgnoreMovementCount(l_PlayerMover->GetIgnoreMovementCount() - 1);
        p_Packet.rfinish();                     // prevent warnings spam
        return;
    }

    /* extract packet */
    MovementInfo l_MovementInfo;
    ReadMovementInfo(p_Packet, &l_MovementInfo);

    if (l_OpCode == CMSG_MOVE_FEATHER_FALL_ACK
     || l_OpCode == CMSG_MOVE_WATER_WALK_ACK)
    {
        uint32 l_AckIndex = p_Packet.read<uint32>(); ///< l_AckIndex is never read 01/18/16
    }

    // prevent tampered movement data
    if (l_MovementInfo.guid != l_Mover->GetGUID())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleMovementOpcodes: guid error");
        return;
    }
    if (!l_MovementInfo.pos.IsPositionValid())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleMovementOpcodes: Invalid Position");
        return;
    }

    /* handle special cases */
    if (l_MovementInfo.t_guid)
    {
        // transports size limited
        // (also received at zeppelin leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if (l_MovementInfo.t_pos.GetPositionX() > 50 || l_MovementInfo.t_pos.GetPositionY() > 50 || l_MovementInfo.t_pos.GetPositionZ() > 50)
        {
            p_Packet.rfinish();                 // prevent warnings spam
            return;
        }

        if (!JadeCore::IsValidMapCoord(l_MovementInfo.pos.GetPositionX() + l_MovementInfo.t_pos.GetPositionX(), l_MovementInfo.pos.GetPositionY() + l_MovementInfo.t_pos.GetPositionY(),
            l_MovementInfo.pos.GetPositionZ() + l_MovementInfo.t_pos.GetPositionZ(), l_MovementInfo.pos.GetOrientation() + l_MovementInfo.t_pos.GetOrientation()))
        {
            p_Packet.rfinish();                 // prevent warnings spam
            return;
        }

        // if we boarded a transport, add us to it
        if (l_PlayerMover)
        {
            if (!l_PlayerMover->GetTransport())
            {

                if (Transport* transport = l_PlayerMover->GetMap()->GetTransport(l_MovementInfo.t_guid))
                {
                    l_PlayerMover->m_transport = transport;
                    transport->AddPassenger(l_PlayerMover);
                }
            }
            else if (l_PlayerMover->GetTransport()->GetGUID() != l_MovementInfo.t_guid)
            {
                bool foundNewTransport = false;
                l_PlayerMover->m_transport->RemovePassenger(l_PlayerMover);

                if (Transport* transport = l_PlayerMover->GetMap()->GetTransport(l_MovementInfo.t_guid))
                {
                    foundNewTransport = true;
                    l_PlayerMover->m_transport = transport;
                    transport->AddPassenger(l_PlayerMover);
                }

                if (!foundNewTransport)
                {
                    l_PlayerMover->m_transport = NULL;
                    l_MovementInfo.t_pos.Relocate(0.0f, 0.0f, 0.0f, 0.0f);
                    l_MovementInfo.t_time = 0;
                    l_MovementInfo.t_seat = -1;
                }
            }
        }

        if (!l_Mover->GetTransport() && !l_Mover->GetVehicle())
        {
            GameObject* go = l_Mover->GetMap()->GetGameObject(l_MovementInfo.t_guid);
            if (!go || go->GetGoType() != GAMEOBJECT_TYPE_TRANSPORT)
                l_MovementInfo.t_guid = 0;
        }
    }
    else if (l_PlayerMover && l_PlayerMover->GetTransport())                // if we were on a transport, leave
    {
        l_PlayerMover->m_transport->RemovePassenger(l_PlayerMover);
        l_PlayerMover->m_transport = NULL;
        l_MovementInfo.t_pos.Relocate(0.0f, 0.0f, 0.0f, 0.0f);
        l_MovementInfo.t_time = 0;
        l_MovementInfo.t_seat = -1;
    }

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (l_PlayerMover && l_PlayerMover->m_movementInfo.GetMovementFlags() & MOVEMENTFLAG_FALLING && (l_MovementInfo.GetMovementFlags() & MOVEMENTFLAG_FALLING) == 0 && (l_MovementInfo.GetMovementFlags() & MOVEMENTFLAG_SWIMMING) == 0 && l_PlayerMover && !l_PlayerMover->isInFlight())
    {
        l_PlayerMover->HandleFall(l_MovementInfo);
    }

    if (l_PlayerMover && ((l_MovementInfo.flags & MOVEMENTFLAG_SWIMMING) != 0) != l_PlayerMover->IsInWater())
    {
        // now client not include swimming flag in case jumping under water
        l_PlayerMover->SetInWater(!l_PlayerMover->IsInWater() || l_PlayerMover->GetBaseMap()->IsUnderWater(l_MovementInfo.pos.GetPositionX(), l_MovementInfo.pos.GetPositionY(), l_MovementInfo.pos.GetPositionZ()));
    }

    sScriptMgr->OnPlayerUpdateMovement(l_PlayerMover);

    // Remove emote on movements
    if (l_PlayerMover && l_MovementInfo.HasMovementFlag(MOVEMENTFLAG_MASK_MOVING))
    {
        if (uint32 l_EmoteId = l_PlayerMover->GetUInt32Value(UNIT_FIELD_EMOTE_STATE))
        {
            if (EmotesEntry const* emoteInfo = sEmotesStore.LookupEntry(l_EmoteId))
            {
                if (emoteInfo->EmoteType != EmoteTypes::EmoteLoop)
                    l_PlayerMover->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            }
        }
    }

    if (l_PlayerMover)
        sAnticheatMgr->StartHackDetection(l_PlayerMover, l_MovementInfo, l_OpCode);

    /*----------------------*/

    /* process position-change */
    WorldPacket data(SMSG_MOVE_UPDATE, p_Packet.size() + 4);
    l_MovementInfo.guid = l_Mover->GetGUID();

    uint32 l_MSTime = getMSTime();

    if (m_clientTimeDelay == 0)
        m_clientTimeDelay = l_MSTime - l_MovementInfo.time;

    l_MovementInfo.time = l_MovementInfo.time + m_clientTimeDelay + MOVEMENT_PACKET_TIME_DELAY;

    WorldSession::WriteMovementInfo(data, &l_MovementInfo);
    l_Mover->SendMessageToSet(&data, m_Player);

    l_Mover->m_movementInfo = l_MovementInfo;
    l_Mover->m_movementInfoLastTime = l_MSTime - GetLatency();

    // this is almost never true (not sure why it is sometimes, but it is), normally use mover->IsVehicle()
    if (l_Mover->GetVehicle())
    {
        l_Mover->SetOrientation(l_MovementInfo.pos.GetOrientation());
        return;
    }

    l_Mover->UpdatePosition(l_MovementInfo.pos);

    if (l_PlayerMover)                                            // nothing is charmed, or player charmed
    {
        l_PlayerMover->UpdateFallInformationIfNeed(l_MovementInfo, l_OpCode);

        /*float l_MaxDepth = -500.0f;

        /// Eye of the Cyclone
        if (l_PlayerMover->GetMapId() == 566)
            l_MaxDepth = 1000.0f;

        /// Vash'jir zones
        switch (l_PlayerMover->GetZoneId())
        {
            case 4815:
            case 4816:
            case 5144:
            case 5145:
            case 5146:
                l_MaxDepth = -2000.0f;
                break;
            default:
                break;
        }
        */
        if (l_MovementInfo.pos.GetPositionZ() < l_PlayerMover->GetMap()->GetMinHeight(l_MovementInfo.pos.GetPositionX(), l_MovementInfo.pos.GetPositionY()))
        {
            if (!(l_PlayerMover->GetBattleground() && l_PlayerMover->GetBattleground()->HandlePlayerUnderMap(m_Player)))
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                // TODO: discard movement packets after the player is rooted
                if (l_PlayerMover->isAlive())
                {
                    ///l_PlayerMover->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IS_OUT_OF_BOUNDS);
                    l_PlayerMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, GetPlayer()->GetMaxHealth());
                    // player can be alive if GM/etc
                    // change the death state to CORPSE to prevent the death timer from
                    // starting in the next player update
                    if (!l_PlayerMover->isAlive())
                        l_PlayerMover->KillPlayer();
                }
            }
        }
    }
}

void WorldSession::HandleForceSpeedChangeAck(WorldPacket& p_Packet)
{
    MovementInfo l_MovementInfo;
    ReadMovementInfo(p_Packet, &l_MovementInfo);

    if (l_MovementInfo.guid != m_Player->GetGUID())
        return;

    uint32 l_AckIndex = p_Packet.read<uint32>();
    float  l_Speed    = p_Packet.read<float>();

    // client ACK send one packet for mounted/run case and need skip all except last from its
    // in other cases anti-cheat check can be fail in false case
    UnitMoveType l_MoveType       = MOVE_WALK;

    Opcodes l_Opcode = (Opcodes)p_Packet.GetOpcode();
    switch (l_Opcode)
    {
        case CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK:        l_MoveType = MOVE_WALK;        break;
        case CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK:         l_MoveType = MOVE_RUN;         break;
        case CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK:    l_MoveType = MOVE_RUN_BACK;    break;
        case CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK:        l_MoveType = MOVE_SWIM;        break;
        case CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:   l_MoveType = MOVE_SWIM_BACK;   break;
        case CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK:         l_MoveType = MOVE_TURN_RATE;   break;
        case CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK:      l_MoveType = MOVE_FLIGHT;      break;
        case CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK: l_MoveType = MOVE_FLIGHT_BACK; break;
        case CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK:        l_MoveType = MOVE_PITCH_RATE;  break;

        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleForceSpeedChangeAck: Unknown move type opcode: %u", l_Opcode);
            return;
    }

    static char const* l_MoveTypeName[MAX_MOVE_TYPE] = {  "Walk", "Run", "RunBack", "Swim", "SwimBack", "TurnRate", "Flight", "FlightBack", "PitchRate" };

    // skip all forced speed changes except last and unexpected
    // in run/mounted case used one ACK and it must be skipped.m_forced_speed_changes[MOVE_RUN} store both.
    if (m_Player->m_forced_speed_changes[l_MoveType] > 0)
    {
        --m_Player->m_forced_speed_changes[l_MoveType];
        if (m_Player->m_forced_speed_changes[l_MoveType] > 0)
            return;
    }

    if (!m_Player->GetTransport() && fabs(m_Player->GetSpeed(l_MoveType) - l_Speed) > 0.01f)
    {
        if (m_Player->GetSpeed(l_MoveType) > l_Speed)         // must be greater - just correct
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "%sSpeedChange player %s is NOT correct (must be %f instead %f), force set to correct value",
                l_MoveTypeName[l_MoveType], m_Player->GetName(), m_Player->GetSpeed(l_MoveType), l_Speed);
            m_Player->SetSpeed(l_MoveType, m_Player->GetSpeedRate(l_MoveType), true);
        }
        else                                                // must be lesser - cheating
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "Player %s from account id %u kicked for incorrect speed (must be %f instead %f)",
                m_Player->GetName(), m_Player->GetSession()->GetAccountId(), m_Player->GetSpeed(l_MoveType), l_Speed);
            /*m_Player->GetSession()->KickPlayer();*/
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPacket& p_RecvPacket)
{
    uint64 l_Guid;
    p_RecvPacket.readPackGUID(l_Guid);
}

void WorldSession::HandleMoveNotActiveMover(WorldPacket &recvData)
{
    uint64 old_mover_guid;
    recvData.readPackGUID(old_mover_guid);

    MovementInfo mi;
    ReadMovementInfo(recvData, &mi);

    mi.guid = old_mover_guid;

    m_Player->m_movementInfo = mi;
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*p_RecvData*/)
{
    WorldPacket l_Data(SMSG_SPECIAL_MOUNT_ANIM, 8);
    l_Data.appendPackGUID(m_Player->GetGUID());
    GetPlayer()->SendMessageToSet(&l_Data, false);
}

void WorldSession::HandleMoveKnockBackAck(WorldPacket & recvData)
{
    HandleMovementOpcodes(recvData);

    if (recvData.rpos() != recvData.size())
        recvData.read_skip<uint32>();                          /// ACK Index
}

void WorldSession::HandleMoveHoverAck(WorldPacket& recvData)
{
    uint64 guid;                                            // guid - unused
    recvData.readPackGUID(guid);

    recvData.read_skip<uint32>();                          // unk

    HandleMovementOpcodes(recvData);

    recvData.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& p_Packet)
{
    if (!m_Player->isAlive() || m_Player->isInCombat())
        return;

    uint64 l_SummonerGuid = 0;
    bool l_Agree = false;

    p_Packet.readPackGUID(l_SummonerGuid);
    l_Agree = p_Packet.ReadBit();

    m_Player->SummonIfPossible(l_Agree);
}

void WorldSession::ReadMovementInfo(WorldPacket& p_Data, MovementInfo* p_MovementInformation)
{
    uint32 l_UnkLoopCounter = 0;

    bool l_HasTransportData     = false;
    bool l_HasTransportTime2    = false;
    bool l_HasTransportTime3    = false;
    bool l_HasSpline            = false;

    MovementStatusElements* l_Sequence = GetMovementStatusElementsSequence(p_Data.GetOpcode());

    if (l_Sequence == nullptr)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::ReadMovementInfo: No movement sequence found for opcode 0x%04X", uint32(p_Data.GetOpcode()));
        return;
    }

    uint64 l_MoverGuid      = 0;
    uint64 l_TransportGuid  = 0;

    for (uint32 l_I = 0; l_I < MSE_COUNT; ++l_I)
    {
        MovementStatusElements l_Element = l_Sequence[l_I];

        if (l_Element == MSEEnd)
            break;

        switch (l_Element)
        {
            case MSEGuid:
                p_Data.readPackGUID(l_MoverGuid);
                break;

            case MSEMovementFlags:
                p_MovementInformation->flags = p_Data.ReadBits(30);
                break;

            case MSEMovementFlags2:
                p_MovementInformation->flags2 = p_Data.ReadBits(16);
                break;

            case MSETimestamp:
                p_Data >> p_MovementInformation->time;
                break;

            case MSEPositionX:
                p_Data >> p_MovementInformation->pos.m_positionX;
                break;

            case MSEPositionY:
                p_Data >> p_MovementInformation->pos.m_positionY;
                break;

            case MSEPositionZ:
                p_Data >> p_MovementInformation->pos.m_positionZ;
                break;

            case MSEOrientation:
                p_MovementInformation->pos.SetOrientation(p_Data.read<float>());
                break;

            case MSETransportGuid:
                if (l_HasTransportData)
                    p_Data.readPackGUID(l_TransportGuid);
                break;

            case MSETransportPositionX:
                if (l_HasTransportData)
                    p_Data >> p_MovementInformation->t_pos.m_positionX;
                break;

            case MSETransportPositionY:
                if (l_HasTransportData)
                    p_Data >> p_MovementInformation->t_pos.m_positionY;
                break;

            case MSETransportPositionZ:
                if (l_HasTransportData)
                    p_Data >> p_MovementInformation->t_pos.m_positionZ;
                break;

            case MSETransportOrientation:
                if (l_HasTransportData)
                    p_MovementInformation->t_pos.SetOrientation(p_Data.read<float>());
                break;

            case MSETransportSeat:
                if (l_HasTransportData)
                    p_Data >> p_MovementInformation->t_seat;
                break;

            case MSETransportTime:
                if (l_HasTransportData)
                    p_Data >> p_MovementInformation->t_time;
                break;

            case MSEHasTransportTime2:
                if (l_HasTransportData)
                    l_HasTransportTime2 = p_Data.ReadBit();
                break;

            case MSEHasTransportTime3:
                if (l_HasTransportData)
                    l_HasTransportTime3 = p_Data.ReadBit();
                break;

            case MSETransportTime2:
                if (l_HasTransportData && l_HasTransportTime2)
                    p_Data >> p_MovementInformation->PrevMoveTime;
                break;

            case MSETransportTime3:
                if (l_HasTransportData && l_HasTransportTime3)
                    p_Data >> p_MovementInformation->VehicleRecID;
                break;

            case MSEPitch:
                p_MovementInformation->HavePitch = 1;
                p_Data >> p_MovementInformation->pitch;
                break;

            case MSEFallTime:
                if (p_MovementInformation->HasFallData)
                    p_Data >> p_MovementInformation->fallTime;
                break;

            case MSEFallVerticalSpeed:
                if (p_MovementInformation->HasFallData)
                    p_Data >> p_MovementInformation->JumpVelocity;
                break;

            case MSEFallCosAngle:
                if (p_MovementInformation->HasFallData && p_MovementInformation->hasFallDirection)
                    p_Data >> p_MovementInformation->j_cosAngle;
                break;

            case MSEFallSinAngle:
                if (p_MovementInformation->HasFallData && p_MovementInformation->hasFallDirection)
                    p_Data >> p_MovementInformation->j_sinAngle;
                break;

            case MSEFallHorizontalSpeed:
                if (p_MovementInformation->HasFallData && p_MovementInformation->hasFallDirection)
                    p_Data >> p_MovementInformation->j_xyspeed;
                break;

            case MSESplineElevation:
                p_MovementInformation->HaveSplineElevation = 1;
                p_Data >> p_MovementInformation->splineElevation;
                break;

            case MSEAlive32:
                p_Data >> p_MovementInformation->Alive32;
                break;

            case MSEUnkCounter:
                p_Data >> l_UnkLoopCounter;
                break;

            case MSEUnkCounterLoop:
                for (uint32 l_I = 0; l_I < l_UnkLoopCounter; ++l_I)
                {
                    uint64 l_UnkGuid;
                    p_Data.readPackGUID(l_UnkGuid);
                }
                break;

            case MSEHasSpline:
                l_HasSpline = p_Data.ReadBit(); ///< l_HasSpline is never read 01/18/16
                break;

            case MSEFlushBits:
                p_Data.FlushBits();
                break;

            case MSEZeroBit:
            case MSEOneBit:
                p_Data.ReadBit();
                break;

            case MSEHasTransportData:
                l_HasTransportData = p_Data.ReadBit();
                break;

            case MSEHasFallData:
                p_MovementInformation->HasFallData = p_Data.ReadBit();
                break;
            case MSEHasFallDirection:
                if (p_MovementInformation->HasFallData)
                    p_MovementInformation->hasFallDirection = p_Data.ReadBit();
                break;

            default:
                ASSERT(false && "Incorrect sequence element detected at ReadMovementInfo");
                break;
        }
    }

    p_MovementInformation->guid     = l_MoverGuid;
    p_MovementInformation->t_guid   = l_TransportGuid;

    //! Anti-cheat checks. Please keep them in seperate if() blocks to maintain a clear overview.
    //! Might be subject to latency, so just remove improper flags.
    #ifdef TRINITY_DEBUG
        #define REMOVE_VIOLATING_FLAGS(check, maskToRemove) \
        { \
            if (check) \
            { \
                sLog->outDebug(LOG_FILTER_UNITS, "WorldSession::ReadMovementInfo: Violation of MovementFlags found (%s). " \
                    "MovementFlags: %u, MovementFlags2: %u for player GUID: %u. Mask %u will be removed.", \
                    STRINGIZE(check), p_MovementInformation->GetMovementFlags(), p_MovementInformation->GetExtraMovementFlags(), GetPlayer()->GetGUIDLow(), maskToRemove); \
                p_MovementInformation->RemoveMovementFlag((maskToRemove)); \
            } \
        }
    #else
        #define REMOVE_VIOLATING_FLAGS(check, maskToRemove) \
                if (check) \
                    p_MovementInformation->RemoveMovementFlag((maskToRemove));
    #endif


    /*! This must be a packet spoofing attempt. MOVEMENTFLAG_ROOT sent from the client is not valid
        in conjunction with any of the moving movement flags such as MOVEMENTFLAG_FORWARD.
        It will freeze clients that receive this player's movement info.*/

    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_ROOT),
        MOVEMENTFLAG_ROOT);

    //! Cannot hover without SPELL_AURA_HOVER
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_HOVER) && !GetPlayer()->HasAuraType(SPELL_AURA_HOVER),
        MOVEMENTFLAG_HOVER);

    //! Cannot ascend and descend at the same time
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_ASCENDING) && p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_DESCENDING),
        MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING);

    //! Cannot move left and right at the same time
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_LEFT) && p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_RIGHT),
        MOVEMENTFLAG_LEFT | MOVEMENTFLAG_RIGHT);

    //! Cannot strafe left and right at the same time
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_STRAFE_LEFT) && p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_STRAFE_RIGHT),
        MOVEMENTFLAG_STRAFE_LEFT | MOVEMENTFLAG_STRAFE_RIGHT);

    //! Cannot pitch up and down at the same time
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_PITCH_UP) && p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_PITCH_DOWN),
        MOVEMENTFLAG_PITCH_UP | MOVEMENTFLAG_PITCH_DOWN);

    //! Cannot move forwards and backwards at the same time
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_FORWARD) && p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_BACKWARD),
        MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_BACKWARD);

    //! Cannot walk on water without SPELL_AURA_WATER_WALK
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_WATERWALKING) && !GetPlayer()->HasAuraType(SPELL_AURA_WATER_WALK),
        MOVEMENTFLAG_WATERWALKING);

    //! Cannot feather fall without SPELL_AURA_FEATHER_FALL
    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_FALLING_SLOW) && !GetPlayer()->HasAuraType(SPELL_AURA_FEATHER_FALL),
        MOVEMENTFLAG_FALLING_SLOW);

    /*! Cannot fly if no fly auras present. Exception is being a GM.
        Note that we check for account level instead of Player::IsGameMaster() because in some
        situations it may be feasable to use .gm fly on as a GM without having .gm on,
        e.g. aerial combat.
    */

    REMOVE_VIOLATING_FLAGS(p_MovementInformation->HasMovementFlag(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY) && GetSecurity() == SEC_PLAYER &&
        !GetPlayer()->m_mover->HasAuraType(SPELL_AURA_FLY) &&
        !GetPlayer()->m_mover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED),
        MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY);

    #undef REMOVE_VIOLATING_FLAGS
}

void WorldSession::WriteMovementInfo(WorldPacket & p_Data, MovementInfo* p_MovementInformation)
{
    MovementStatusElements* l_Sequence = GetMovementStatusElementsSequence(p_Data.GetOpcode());

    if (!l_Sequence)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::WriteMovementInfo: No movement sequence found for opcode 0x%04X", uint32(p_Data.GetOpcode()));
        return;
    }

    bool l_HasTransportData = p_MovementInformation->t_guid != 0LL;

    p_MovementInformation->Normalize();

    for (uint32 l_I = 0; l_I < MSE_COUNT; ++l_I)
    {
        MovementStatusElements l_Element = l_Sequence[l_I];

        if (l_Element == MSEEnd)
            break;

        switch (l_Element)
        {
            case MSEHasTransportData:
                p_Data.WriteBit(l_HasTransportData);
                break;

            case MSEHasTransportTime2:
                if (l_HasTransportData)
                    p_Data.WriteBit(p_MovementInformation->PrevMoveTime);
                break;

            case MSEHasTransportTime3:
                if (l_HasTransportData)
                    p_Data.WriteBit(p_MovementInformation->VehicleRecID);
                break;

            case MSEHasFallData:
                p_Data.WriteBit(p_MovementInformation->HasFallData);
                break;

            case MSEHasFallDirection:
                if (p_MovementInformation->HasFallData)
                    p_Data.WriteBit(p_MovementInformation->hasFallDirection);
                break;

            case MSEHasSpline:
                p_Data.WriteBit(false);
                break;

            case MSEGuid:
                p_Data.appendPackGUID(p_MovementInformation->guid);
                break;

            case MSEMovementFlags:
                p_Data.WriteBits(p_MovementInformation->flags, 30);
                break;

            case MSEMovementFlags2:
                p_Data.WriteBits(p_MovementInformation->flags2, 16);
                break;

            case MSETimestamp:
                p_Data << p_MovementInformation->time;
                break;

            case MSEPositionX:
                p_Data << p_MovementInformation->pos.m_positionX;
                break;

            case MSEPositionY:
                p_Data << p_MovementInformation->pos.m_positionY;
                break;

            case MSEPositionZ:
                p_Data << p_MovementInformation->pos.m_positionZ;
                break;

            case MSEOrientation:
                 p_Data << p_MovementInformation->pos.GetOrientation();
                break;

            case MSETransportGuid:
                if (l_HasTransportData)
                    p_Data.appendPackGUID(p_MovementInformation->t_guid);
                break;

            case MSETransportPositionX:
                if (l_HasTransportData)
                    p_Data << p_MovementInformation->t_pos.m_positionX;
                break;

            case MSETransportPositionY:
                if (l_HasTransportData)
                    p_Data << p_MovementInformation->t_pos.m_positionY;
                break;

            case MSETransportPositionZ:
                if (l_HasTransportData)
                    p_Data << p_MovementInformation->t_pos.m_positionZ;
                break;

            case MSETransportOrientation:
                if (l_HasTransportData)
                    p_Data << p_MovementInformation->t_pos.GetOrientation();
                break;

            case MSETransportSeat:
                if (l_HasTransportData)
                    p_Data << p_MovementInformation->t_seat;
                break;

            case MSETransportTime:
                if (l_HasTransportData)
                    p_Data << p_MovementInformation->t_time;
                break;

            case MSETransportTime2:
                if (l_HasTransportData && p_MovementInformation->PrevMoveTime)
                    p_Data << p_MovementInformation->PrevMoveTime;
                break;

            case MSETransportTime3:
                if (l_HasTransportData && p_MovementInformation->VehicleRecID)
                    p_Data << p_MovementInformation->VehicleRecID;
                break;

            case MSEPitch:
                p_Data << p_MovementInformation->pitch;
                break;

            case MSEFallTime:
                if (p_MovementInformation->HasFallData)
                    p_Data << p_MovementInformation->fallTime;
                break;

            case MSEFallVerticalSpeed:
                if (p_MovementInformation->HasFallData)
                    p_Data << p_MovementInformation->JumpVelocity;
                break;

            case MSEFallCosAngle:
                if (p_MovementInformation->HasFallData && p_MovementInformation->hasFallDirection)
                    p_Data << p_MovementInformation->j_cosAngle;
                break;

            case MSEFallSinAngle:
                if (p_MovementInformation->HasFallData && p_MovementInformation->hasFallDirection)
                    p_Data << p_MovementInformation->j_sinAngle;
                break;

            case MSEFallHorizontalSpeed:
                if (p_MovementInformation->HasFallData && p_MovementInformation->hasFallDirection)
                    p_Data << p_MovementInformation->j_xyspeed;
                break;

            case MSESplineElevation:
                p_Data << p_MovementInformation->splineElevation;
                break;

            case MSEAlive32:
                p_Data << uint32(0);
                break;

            case MSEUnkCounter:
                p_Data << uint32(0);
                break;

            case MSEUnkCounterLoop:
                break;

            case MSEFlushBits:
                p_Data.FlushBits();
                break;

            case MSEZeroBit:
                p_Data.WriteBit(0);
                break;
            case MSEOneBit:
                p_Data.WriteBit(1);
                break;

            default:
                ASSERT(false && "Incorrect sequence element detected at ReadMovementInfo");
                break;
        }
    }
}
