/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "MovementPacketBuilder.h"
#include "Unit.h"
#include "Transport.h"
#include "Vehicle.h"

namespace Movement
{
    UnitMoveType SelectSpeedType(uint32 moveFlags)
    {
        /*! Not sure about MOVEMENTFLAG_CAN_FLY here - do creatures that can fly
            but are on ground right now also have it? If yes, this needs a more
            dynamic check, such as is flying now
        */
        if (moveFlags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_DISABLE_GRAVITY))
        {
            if (moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.flight >= speed_obj.flight_back*/)
                return MOVE_FLIGHT_BACK;
            else
                return MOVE_FLIGHT;
        }
        else if (moveFlags & MOVEMENTFLAG_SWIMMING)
        {
            if (moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.swim >= speed_obj.swim_back*/)
                return MOVE_SWIM_BACK;
            else
                return MOVE_SWIM;
        }
        else if (moveFlags & MOVEMENTFLAG_WALKING)
        {
            //if (speed_obj.run > speed_obj.walk)
            return MOVE_WALK;
        }
        else if (moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.run >= speed_obj.run_back*/)
            return MOVE_RUN_BACK;

        return MOVE_RUN;
    }

    enum MonsterMoveType
    {
        MonsterMoveNormal       = 0,
        MonsterMoveStop         = 1,
        MonsterMoveFacingSpot   = 2,
        MonsterMoveFacingTarget = 3,
        MonsterMoveFacingAngle  = 4
    };

    inline void operator << (ByteBuffer& b, const Vector3& v)
    {
        b << v.x << v.y << v.z;
    }

    inline void operator >> (ByteBuffer& b, Vector3& v)
    {
        b >> v.x >> v.y >> v.z;
    }

    void WriteLinearPath(const Spline<int32>& spline, ByteBuffer& data)
    {
        uint32 last_idx = spline.getPointCount() - 3;
        const Vector3 * real_path = &spline.getPoint(1);

        data << real_path[last_idx];   // destination
        if (last_idx > 1)
        {
            Vector3 middle = (real_path[0] + real_path[last_idx]) / 2.f;
            Vector3 offset;
            // first and last points already appended
            for (uint32 i = 1; i < last_idx; ++i)
            {
                offset = middle - real_path[i];
                data.appendPackXYZ(offset.x, offset.y, offset.z);
            }
        }
    }

    void WriteCatmullRomPath(const Spline<int32>& spline, ByteBuffer& data)
    {
        uint32 count = spline.getPointCount() - 2;

        for (uint32 i = 0; i < count; i++)
            data << spline.getPoint(i+2).y << spline.getPoint(i+2).x << spline.getPoint(i+2).z;

        //data.append<Vector3>(&spline.getPoint(2), count);
    }

    void WriteCatmullRomCyclicPath(const Spline<int32>& spline, ByteBuffer& data)
    {
        uint32 count = spline.getPointCount() - 2;
        data << spline.getPoint(1).y << spline.getPoint(1).z << spline.getPoint(1).x ; // fake point, client will erase it from the spline after first cycle done
        for (uint32 i = 0; i < count; i++)
            data << spline.getPoint(i+1).y << spline.getPoint(i+1).x << spline.getPoint(i+1).z;
        //data.append<Vector3>(&spline.getPoint(1), count);
    }

    void MoveSplineInit::Launch()
    {
        MoveSpline& move_spline = *unit.movespline;

        Location real_position(unit.GetPositionX(), unit.GetPositionY(), unit.GetPositionZMinusOffset(), unit.GetOrientation());
        // Elevators also use MOVEMENTFLAG_ONTRANSPORT but we do not keep track of their position changes
        if (unit.GetTransGUID())
        {
            real_position.x = unit.GetTransOffsetX();
            real_position.y = unit.GetTransOffsetY();
            real_position.z = unit.GetTransOffsetZ();
            real_position.orientation = unit.GetTransOffsetO();
        }

        // there is a big chance that current position is unknown if current state is not finalized, need compute it
        // this also allows calculate spline position and update map position in much greater intervals
        // Don't compute for transport movement if the unit is in a motion between two transports
        if (!move_spline.Finalized() && move_spline.onTransport == (unit.GetTransGUID() != 0))
            real_position = move_spline.ComputePosition();

        // should i do the things that user should do? - no.
        if (args.path.empty())
            return;

        // correct first vertex
        args.path[0] = real_position;
        args.initialOrientation = real_position.orientation;
        move_spline.onTransport = (unit.GetTransGUID() != 0);

        uint32 moveFlags = unit.m_movementInfo.GetMovementFlags();
        if (args.flags.walkmode)
            moveFlags |= MOVEMENTFLAG_WALKING;
        else
            moveFlags &= ~MOVEMENTFLAG_WALKING;

        moveFlags |= MOVEMENTFLAG_FORWARD;

        if (!args.HasVelocity)
            args.velocity = unit.GetSpeed(SelectSpeedType(moveFlags));

        if (!args.Validate())
            return;

        if (moveFlags & MOVEMENTFLAG_ROOT)
            moveFlags &= ~MOVEMENTFLAG_MASK_MOVING;

        unit.m_movementInfo.SetMovementFlags(moveFlags);
        move_spline.Initialize(args);

        WorldPacket data(SMSG_MONSTER_MOVE, 64);
        ObjectGuid moverGUID = unit.GetGUID();
        ObjectGuid transportGUID = unit.GetTransGUID();
        MoveSplineFlag splineflags =  move_spline.splineflags;
        splineflags.enter_cycle = move_spline.isCyclic();
        uint32 sendSplineFlags = splineflags & ~MoveSplineFlag::Mask_No_Monster_Move;
        int8 seat = unit.GetTransSeat();

        bool hasUnk1 = false;
        bool hasUnk2 = false;
        bool hasUnk3 = false;
        bool unk4 = false;
        uint32 unkCounter = 0;
        uint32 packedWPcount = splineflags & MoveSplineFlag::UncompressedPath ? 0 : move_spline.spline.getPointCount() - 3;
        uint32 WPcount = !packedWPcount ? move_spline.spline.getPointCount() - 2 : 1;
        uint8 splineType = 0;

        switch (splineflags & MoveSplineFlag::Mask_Final_Facing)
        {
            case MoveSplineFlag::Final_Target:
                splineType = MonsterMoveFacingTarget;
                break;
            case MoveSplineFlag::Final_Angle:
                splineType = MonsterMoveFacingAngle;
                break;
            case MoveSplineFlag::Final_Point:
                splineType = MonsterMoveFacingSpot;
                break;
            default:
                splineType = MonsterMoveNormal;
                break;
        }

        // Writes bits
        data.WriteBit(hasUnk1);                     // unk, has counter + 2 bits & somes uint16/float
        data.WriteBit(moverGUID[5]);
        data.WriteBit(!splineflags.animation);      // !hasAnimationTime
        data.WriteBit(false);                       // !has duration
        data.WriteBit(moverGUID[4]);
        data.WriteBit(moverGUID[3]);
        data.WriteBit(true);                        // !unk, send uint32
        data.WriteBit(seat == -1);                  // !has seat
        data.WriteBit(moverGUID[2]);
        data.WriteBit(sendSplineFlags == 0);        // !hasFlags
        data.WriteBit(moverGUID[0]);

        if (hasUnk1)
        {
            data.WriteBits(0, 2);
            data.WriteBits(unkCounter, 22);
        }

        data.WriteBits(packedWPcount, 22);          // packed waypoint count
        data.WriteBit(!hasUnk2);                    // !hasUnk2, unk byte
        data.WriteBit(moverGUID[7]);
        data.WriteBit(false);                       // fake bit

        uint8 transportBitsOrder[8] = {5, 3, 4, 6, 2, 1, 7, 0};
        data.WriteBitInOrder(transportGUID, transportBitsOrder);

        data.WriteBit(!splineflags.animation);      // animation state
        data.WriteBit(!splineflags.parabolic);      // !hasParabolicTime
        data.WriteBit(!splineflags.parabolic);      // !hasParabolicSpeed
        data.WriteBit(moverGUID[6]);
        data.WriteBits(WPcount, 20);
        data.WriteBit(moverGUID[1]);
        data.WriteBit(!hasUnk3);                    // !hasUnk3
        data.WriteBits(splineType, 3);              // splineType

        if (splineType == MonsterMoveFacingTarget)
        {
            ObjectGuid facingTargetGUID = move_spline.facing.target;
            uint8 facingTargetBitsOrder[8] = {4, 6, 5, 1, 0, 7, 3, 2};
            data.WriteBitInOrder(facingTargetGUID, facingTargetBitsOrder);
        }

        data.WriteBit(unk4);                       // unk bit 38
        
        data.FlushBits();

        // Write bytes       
        if (hasUnk1)
        {
            data << float(0.0f);
            data << float(0.0f);
            data << uint16(0);

            for (uint32 i = 0; i < unkCounter; i++)
            {
                data << uint16(0);
                data << uint16(0);
            }

            data << uint16(0);
        }

        uint8 transportByteOrder[8] = {0, 1, 2, 7, 3, 4, 6, 5};
        data.WriteBytesSeq(transportGUID, transportByteOrder);

        if (splineType == MonsterMoveFacingTarget)
        {
            ObjectGuid facingTargetGUID = move_spline.facing.target;
            uint8 facingTargetBitsOrder[8] = {2, 1, 7, 0, 5, 3, 4, 6};
            data.WriteBytesSeq(facingTargetGUID, facingTargetBitsOrder);
        }

        data << float(move_spline.spline.getPoint(move_spline.spline.first()).y);
        data.WriteByteSeq(moverGUID[7]);

        if (splineflags.animation)
            data << int32(move_spline.effect_start_time);

        if ((splineflags & MoveSplineFlag::UncompressedPath) == 0)
        {
            uint32 last_idx = move_spline.spline.getPointCount() - 3;
            const Vector3 * real_path = &move_spline.spline.getPoint(1);
            if (last_idx > 0)
            {
                Vector3 middle = (real_path[0] + real_path[last_idx]) / 2.f;
                Vector3 offset;
                // end points already appended
                for (uint32 i = 0; i < last_idx; ++i)
                {
                    offset = middle - real_path[i];
                    data.appendPackXYZ(offset.x, offset.y, offset.z);
                }
            }
        }
         //   WriteLinearPath(move_spline.spline, data);

        data << float(0.0f);                    // unk float
        data << float(move_spline.spline.getPoint(move_spline.spline.first()).z);
        data << float(0.0f);                    // unk float

        if (splineType == MonsterMoveFacingSpot)
            data << move_spline.facing.f.z << move_spline.facing.f.x << move_spline.facing.f.y;

        if (splineflags & MoveSplineFlag::UncompressedPath)
        {
            if (splineflags.cyclic)
                WriteCatmullRomCyclicPath(move_spline.spline, data);
            else
                WriteCatmullRomPath(move_spline.spline, data);
        }
        // Append end point for packed waypoints
        else
        {
            uint32 last_idx = move_spline.spline.getPointCount() - 2;
            const Vector3 * real_path = &move_spline.spline.getPoint(1);
            data << real_path[last_idx].y << real_path[last_idx].z << real_path[last_idx].x; // destination
        }

        data.WriteByteSeq(moverGUID[5]);
        data << uint32(getMSTime());            // Move Ticks

        if (sendSplineFlags)
            data << uint32(sendSplineFlags);

        if (splineflags.animation)
            data << uint8(splineflags.getAnimationId());

        data.WriteByteSeq(moverGUID[0]);

        if (hasUnk2)
            data << uint8(0);                   // unk byte

        if (splineflags.parabolic)
            data << move_spline.effect_start_time;

        data << float(move_spline.spline.getPoint(move_spline.spline.first()).x);
        data << move_spline.Duration();
        data.WriteByteSeq(moverGUID[4]);

        if (splineflags.parabolic)
            data << move_spline.vertical_acceleration;

        if (seat != -1)
            data << int8(seat);

        if (splineType == MonsterMoveFacingAngle)
            data << move_spline.facing.angle;

        if (hasUnk3)
            data << uint8(0);                   // unk byte

        data << float(0.0f);                    // unk float

        if (false)
            data << uint32(0);                  // unk uint32

        data.WriteByteSeq(moverGUID[6]);
        data.WriteByteSeq(moverGUID[2]);
        data.WriteByteSeq(moverGUID[3]);
        data.WriteByteSeq(moverGUID[1]);

        unit.SendMessageToSet(&data, true);
    }

    void MoveSplineInit::Stop(bool force)
    {
        MoveSpline& move_spline = *unit.movespline;

        if (force)
        {
            args.flags = MoveSplineFlag::Done;
            unit.m_movementInfo.RemoveMovementFlag(MOVEMENTFLAG_FORWARD);
            move_spline.Initialize(args);
            return;
        }

        // No need to stop if we are not moving
        if (move_spline.Finalized())
            return;

        Location loc = move_spline.ComputePosition();
        args.flags = MoveSplineFlag::Done;
        unit.m_movementInfo.RemoveMovementFlag(MOVEMENTFLAG_FORWARD);
        move_spline.Initialize(args);

            WorldPacket data(SMSG_MONSTER_MOVE, 64);
        ObjectGuid moverGUID = unit.GetGUID();
        ObjectGuid transportGUID = unit.GetTransGUID();

        int8 seat = unit.GetTransSeat();

        bool hasUnk1 = false;
        bool hasUnk2 = false;
        bool hasUnk3 = false;
        bool unk4 = false;
        uint32 unkCounter = 0;
        uint32 packedWPcount = 0;
        uint32 WPcount = 0;
        uint8 splineType = MonsterMoveStop;

        // Writes bits
        data.WriteBit(hasUnk1);                     // unk, has counter + 2 bits & somes uint16/float
        data.WriteBit(moverGUID[5]);
        data.WriteBit(true);      					// !hasAnimationTime
        data.WriteBit(true);                        // has duration
        data.WriteBit(moverGUID[4]);
        data.WriteBit(moverGUID[3]);
        data.WriteBit(true);                        // !unk, send uint32
        data.WriteBit(seat == -1);                  // !has seat
        data.WriteBit(moverGUID[2]);
        data.WriteBit(true);        // !hasFlags
        data.WriteBit(moverGUID[0]);

        if (hasUnk1)
        {
            data.WriteBits(0, 2);
            data.WriteBits(unkCounter, 22);
        }

        data.WriteBits(packedWPcount, 22);          // packed waypoint count
        data.WriteBit(!hasUnk2);                    // !hasUnk2, unk byte
        data.WriteBit(moverGUID[7]);
        data.WriteBit(false);                       // fake bit

        uint8 transportBitsOrder[8] = {5, 3, 4, 6, 2, 1, 7, 0};
        data.WriteBitInOrder(transportGUID, transportBitsOrder);

        data.WriteBit(true);      // animation state
        data.WriteBit(true);      // !hasParabolicTime
        data.WriteBit(true);      // !hasParabolicSpeed
        data.WriteBit(moverGUID[6]);
        data.WriteBits(WPcount, 20);
        data.WriteBit(moverGUID[1]);
        data.WriteBit(!hasUnk3);                    // !hasUnk3
        data.WriteBits(splineType, 3);              // splineType

        data.WriteBit(unk4);                       // unk bit 38
        
        data.FlushBits();

        // Write bytes       
        if (hasUnk1)
        {
            data << float(0.0f);
            data << float(0.0f);
            data << uint16(0);

            for (uint32 i = 0; i < unkCounter; i++)
            {
                data << uint16(0);
                data << uint16(0);
            }

            data << uint16(0);
        }

        uint8 transportByteOrder[8] = {0, 1, 2, 7, 3, 4, 6, 5};
        data.WriteBytesSeq(transportGUID, transportByteOrder);

        data << float(unit.GetPositionY());
        data.WriteByteSeq(moverGUID[7]);

        data << float(0.0f);                    // unk float
        data << float(unit.GetPositionZ());
        data << float(0.0f);                    // unk float

        data.WriteByteSeq(moverGUID[5]);
        data << uint32(getMSTime());            // Move Ticks

        data.WriteByteSeq(moverGUID[0]);

        if (hasUnk2)
            data << uint8(0);                   // unk byte

        data << float(unit.GetPositionX());

        data.WriteByteSeq(moverGUID[4]);

        if (seat != -1)
            data << int8(seat);

        if (hasUnk3)
            data << uint8(0);                   // unk byte

        data << float(0.0f);                    // unk float

        if (false)
            data << uint32(0);                  // unk uint32

        data.WriteByteSeq(moverGUID[6]);
        data.WriteByteSeq(moverGUID[2]);
        data.WriteByteSeq(moverGUID[3]);
        data.WriteByteSeq(moverGUID[1]);

        //PacketBuilder::WriteStopMovement(loc, args.splineId, data);
        unit.SendMessageToSet(&data, true);
    }

    MoveSplineInit::MoveSplineInit(Unit& m) : unit(m)
    {
        args.splineId = splineIdGen.NewId();
        // Elevators also use MOVEMENTFLAG_ONTRANSPORT but we do not keep track of their position changes
        args.TransformForTransport = unit.GetTransGUID();
        // mix existing state into new
        args.flags.walkmode = unit.m_movementInfo.HasMovementFlag(MOVEMENTFLAG_WALKING);
        args.flags.flying = unit.m_movementInfo.HasMovementFlag(MovementFlags(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_DISABLE_GRAVITY));
        args.flags.smoothGroundPath = true; // enabled by default, CatmullRom mode or client config "pathSmoothing" will disable this
    }

    void MoveSplineInit::SetFacing(const Unit * target)
    {
        args.flags.EnableFacingTarget();
        args.facing.target = target->GetGUID();
    }

    void MoveSplineInit::SetFacing(float angle)
    {
        if (args.TransformForTransport)
        {
            if (Unit* vehicle = unit.GetVehicleBase())
                angle -= vehicle->GetOrientation();
            else if (Transport* transport = unit.GetTransport())
                angle -= transport->GetOrientation();
        }

        args.facing.angle = G3D::wrap(angle, 0.f, (float)G3D::twoPi());
        args.flags.EnableFacingAngle();
    }

    void MoveSplineInit::MoveTo(Vector3 const& dest)
    {
        args.path_Idx_offset = 0;
        args.path.resize(2);
        TransportPathTransform transform(unit, args.TransformForTransport);
        args.path[1] = transform(dest);
    }

    void MoveSplineInit::SetFall()
    {
        args.flags.EnableFalling();
        args.flags.fallingSlow = unit.HasUnitMovementFlag(MOVEMENTFLAG_FALLING_SLOW);
    }

    Vector3 TransportPathTransform::operator()(Vector3 input)
    {
        if (_transformForTransport)
        {
            float unused = 0.0f;
            if (TransportBase* transport = _owner.GetDirectTransport())
                transport->CalculatePassengerOffset(input.x, input.y, input.z, unused);

        }

        return input;
    }
}
