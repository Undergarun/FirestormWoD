////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MOVEMENT_STRUCTURES_H
#define _MOVEMENT_STRUCTURES_H

enum MovementStatusElements
{
    MSEHasTransportData,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSEHasFallData,
    MSEHasFallDirection,
    MSEHasSpline,

    MSEGuid,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSETransportGuid,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSETransportTime2,
    MSETransportTime3,
    MSEPitch,
    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,
    MSESplineElevation,
    MSEAlive32,
    MSEUnkCounter,
    MSEUnkCounterLoop,

    // Special
    MSEFlushBits,   //FlushBits()
    MSEZeroBit,     // writes bit value 1 or skips read bit
    MSEOneBit,      // writes bit value 0 or skips read bit
    MSEEnd,         // marks end of parsing
    MSE_COUNT
};

/// 6.0.1 18716
MovementStatusElements MovementGlobalSequence[] =
{
    MSEGuid,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSEPitch,
    MSESplineElevation,
    MSEUnkCounter,
    MSEAlive32,
    MSEUnkCounterLoop,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSEHasTransportData,
    MSEHasFallData,
    MSEZeroBit,
    MSEZeroBit,
    MSEZeroBit,

    MSETransportGuid,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSETransportTime2,
    MSETransportTime3,

    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEHasFallDirection,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,

    MSEEnd,
};

/// 6.0.1 18716
MovementStatusElements PlayerMoveSequence[] =
{
    MSEGuid,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSEPitch,
    MSESplineElevation,
    MSEUnkCounter,
    MSEAlive32,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSEHasTransportData,
    MSEHasFallData,
    MSEZeroBit,
    MSEZeroBit,
    MSEZeroBit,
    MSEFlushBits,

    MSETransportGuid,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSEFlushBits,
    MSETransportTime2,
    MSETransportTime3,

    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEHasFallDirection,
    MSEFlushBits,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,

    MSEEnd,
};

MovementStatusElements* GetMovementStatusElementsSequence(Opcodes opcode)
{
    // Cast spell has movement data part when castflags & 0x10, patched ClientSide to have same data of CMSG_PLAYER_MOVE
    if (opcode == CMSG_CAST_SPELL
        || opcode == CMSG_USE_ITEM
        || opcode == CMSG_PET_CAST_SPELL

        || opcode == CMSG_MOVE_START_FORWARD
        || opcode == CMSG_MOVE_START_TURN_LEFT
        || opcode == CMSG_MOVE_START_TURN_RIGHT
        || opcode == CMSG_MOVE_START_BACKWARD
        || opcode == CMSG_MOVE_START_STRAFE_LEFT
        || opcode == CMSG_MOVE_START_STRAFE_RIGHT
        || opcode == CMSG_MOVE_START_ASCEND
        || opcode == CMSG_MOVE_START_DESCEND
        || opcode == CMSG_MOVE_START_PITCH_UP
        || opcode == CMSG_MOVE_START_PITCH_DOWN
        || opcode == CMSG_MOVE_START_SWIM

        || opcode == CMSG_MOVE_JUMP
        || opcode == CMSG_MOVE_FALL_LAND
        || opcode == CMSG_MOVE_KNOCK_BACK_ACK

        || opcode == CMSG_MOVE_HEARTBEAT
        || opcode == CMSG_MOVE_SET_FACING
        || opcode == CMSG_MOVE_SET_PITCH
        || opcode == CMSG_MOVE_SET_RUN_MODE
        || opcode == CMSG_MOVE_SET_WALK_MODE
        || opcode == CMSG_MOVE_CHNG_TRANSPORT

        || opcode == CMSG_MOVE_STOP
        || opcode == CMSG_MOVE_STOP_TURN
        || opcode == CMSG_MOVE_STOP_STRAFE
        || opcode == CMSG_MOVE_STOP_SWIM
        || opcode == CMSG_MOVE_STOP_ASCEND
        || opcode == CMSG_MOVE_STOP_PITCH

        || opcode == CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK
        || opcode == CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK
        || opcode == CMSG_MOVE_SET_CAN_FLY_ACK
        || opcode == CMSG_MOVE_FEATHER_FALL_ACK
        || opcode == CMSG_MOVE_WATER_WALK_ACK
        || opcode == CMSG_MOVE_KNOCK_BACK_ACK
        || opcode == CMSG_MOVE_HOVER_ACK
        || opcode == CMSG_MOVE_APPLY_MOVEMENT_FORCE_ACK
        || opcode == CMSG_MOVE_REMOVE_MOVEMENT_FORCE_ACK
        || opcode == CMSG_MOVE_SPLINE_DONE
        || opcode == CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE)
        return MovementGlobalSequence;

    if (opcode == SMSG_MOVE_UPDATE
        || opcode ==  SMSG_MOVE_UPDATE_TELEPORT
        || opcode ==  SMSG_MOVE_UPDATE_RUN_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_FLIGHT_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_PITCH_RATE
        || opcode ==  SMSG_MOVE_UPDATE_RUN_BACK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_SWIM_BACK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_SWIM_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_TURN_RATE
        || opcode ==  SMSG_MOVE_UPDATE_WALK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_KNOCK_BACK)
        return PlayerMoveSequence;

    return NULL;
}

#endif
