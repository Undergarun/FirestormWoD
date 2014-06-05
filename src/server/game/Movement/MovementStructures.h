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

/// 6.0.1 18322
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

//5.0.5 16048
MovementStatusElements PlayerMoveSequence[] =
{
    MSEEnd,
};

MovementStatusElements* GetMovementStatusElementsSequence(Opcodes opcode)
{
    switch (opcode)
    {
        case CMSG_CAST_SPELL:       // Cast spell has movement data part when castflags & 0x10, patched ClientSide to have same data of CMSG_PLAYER_MOVE

        case CMSG_MOVE_START_FORWARD:
        case CMSG_MOVE_START_TURN_LEFT:
        case CMSG_MOVE_START_TURN_RIGHT:
        case CMSG_MOVE_START_BACKWARD:

        case CMSG_MOVE_JUMP:

        case CMSG_MOVE_HEARTBEAT:
        case CMSG_MOVE_SET_FACING:

        case CMSG_MOVE_STOP:
        case CMSG_MOVE_STOP_TURN:
            return MovementGlobalSequence;

        case SMSG_MOVE_UPDATE:
            return PlayerMoveSequence;

        default:
            break;
    }

    return NULL;
}

#endif
