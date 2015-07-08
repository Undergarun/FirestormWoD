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
        MonsterMoveFacingSpot   = 1,
        MonsterMoveFacingTarget = 2,
        MonsterMoveFacingAngle  = 3,
        MonsterMoveStop         = 4
    };

    void MoveSplineInit::Launch()
    {
        MoveSpline& l_MoveSpline = *m_Unit.movespline;

        Location l_RealPosition(m_Unit.GetPositionX(), m_Unit.GetPositionY(), m_Unit.GetPositionZMinusOffset(), m_Unit.GetOrientation());

        /// Elevators also use MOVEMENTFLAG_ONTRANSPORT but we do not keep track of their position changes
        if (m_Unit.GetTransGUID())
        {
            l_RealPosition.x = m_Unit.GetTransOffsetX();
            l_RealPosition.y = m_Unit.GetTransOffsetY();
            l_RealPosition.z = m_Unit.GetTransOffsetZ();
            l_RealPosition.orientation = m_Unit.GetTransOffsetO();
        }

        /// There is a big chance that current position is unknown if current state is not finalized, need compute it
        /// this also allows calculate spline position and update map position in much greater intervals
        /// Don't compute for transport movement if the unit is in a motion between two transports
        /// If the movement to enter on the vehicle isn't finish and we are exit a vehicle, transport guid will be null but we will still have transport position in unit.movespline
        /// So we have to check if current movespline is a entervehicle too
        if (!l_MoveSpline.Finalized() && l_MoveSpline.onTransport == (m_Unit.GetTransGUID() != 0)
            && !(l_MoveSpline.splineflags.transportEnter && args.flags.transportExit))
            l_RealPosition = l_MoveSpline.ComputePosition();

        /// Should i do the things that user should do? - no.
        if (args.path.empty())
            return;

        /// Correct first vertex
        args.path[0] = l_RealPosition;
        args.initialOrientation = l_RealPosition.orientation;
        l_MoveSpline.onTransport = (m_Unit.GetTransGUID() != 0);

        uint32 l_MoveFlags = m_Unit.m_movementInfo.GetMovementFlags();
        
        if (args.flags.walkmode)
            l_MoveFlags |= MOVEMENTFLAG_WALKING;
        else
            l_MoveFlags &= ~MOVEMENTFLAG_WALKING;

        l_MoveFlags |= MOVEMENTFLAG_FORWARD;

        if (!args.HasVelocity)
            args.velocity = m_Unit.GetSpeed(SelectSpeedType(l_MoveFlags));

        if (!args.Validate())
        {
            if (!(args.velocity > 0.1f))
                sLog->outError(LOG_FILTER_GENERAL, "MoveSplineInitArgs::Validate: args.velocity '%f', flags: %u, speedType: %u, speed %f, hasVelocity %u", args.velocity, l_MoveFlags, SelectSpeedType(l_MoveFlags), m_Unit.GetSpeed(SelectSpeedType(l_MoveFlags)), args.HasVelocity);

            return;
        }

        if (l_MoveFlags & MOVEMENTFLAG_ROOT)
            l_MoveFlags &= ~MOVEMENTFLAG_MASK_MOVING;

        m_Unit.m_movementInfo.SetMovementFlags(l_MoveFlags);
        l_MoveSpline.Initialize(args);

        //////////////////////////////////////////////////////////////////////////

        MoveSplineFlag l_Splineflags    = l_MoveSpline.splineflags;
        l_Splineflags.enter_cycle       = l_MoveSpline.isCyclic();

        uint64 l_MoverGUID      = m_Unit.GetGUID();
        uint64 l_TransportGUID  = m_Unit.GetTransGUID();

        uint32 l_CompressedWayPointCount    = l_Splineflags & MoveSplineFlag::UncompressedPath ? 0 : l_MoveSpline.spline.getPointCount() - 3;
        uint32 l_UncompressedWayPointCount  = !l_CompressedWayPointCount ? l_MoveSpline.spline.getPointCount() - 2 : 1;

        uint8   l_FinalFacingMode   = 0;
        int8    l_TransportSeat     = m_Unit.GetTransSeat();

        switch (l_Splineflags & MoveSplineFlag::Mask_Final_Facing)
        {
            case MoveSplineFlag::Final_Target:
                l_FinalFacingMode = MonsterMoveFacingTarget;
                break;
            case MoveSplineFlag::Final_Angle:
                l_FinalFacingMode = MonsterMoveFacingAngle;
                break;
            case MoveSplineFlag::Final_Point:
                l_FinalFacingMode = MonsterMoveFacingSpot;
                break;
            default:
                l_FinalFacingMode = MonsterMoveNormal;
                break;
        }

        float l_SplineDestinationX = 0.0f;
        float l_SplineDestinationY = 0.0f;
        float l_SplineDestinationZ = 0.0f;

        if ((l_Splineflags & MoveSplineFlag::UncompressedPath) == 0)
        {
            uint32 l_LastIndex = l_MoveSpline.spline.getPointCount() - 2;
            const Vector3 * l_RealPath = &l_MoveSpline.spline.getPoint(1);

            /// Add a salt in points because the client doesn't like to have 2 time the same points
            l_SplineDestinationX = l_RealPath[l_LastIndex].x + (float(50) / 1000.f);
            l_SplineDestinationY = l_RealPath[l_LastIndex].y;
            l_SplineDestinationZ = l_RealPath[l_LastIndex].z;
        }

        Movement::Location  l_Position = l_MoveSpline.spline.getPoint(l_MoveSpline.spline.first());

        WorldPacket l_Data(SMSG_MONSTER_MOVE, 2 * 1024);
        l_Data.appendPackGUID(l_MoverGUID);
        l_Data << float(l_Position.x);                                                              ///< Spline current position X
        l_Data << float(l_Position.y);                                                              ///< Spline current position Y
        l_Data << float(l_Position.z);                                                              ///< Spline current position Z
        l_Data << uint32(l_MoveSpline.GetId());                                                     ///< Move Spline ID
        l_Data << float(0);                                                                         ///< Spline destination X
        l_Data << float(0);                                                                         ///< Spline destination Y
        l_Data << float(0);                                                                         ///< Spline destination Z
        l_Data << uint32(l_Splineflags & ~MoveSplineFlag::Mask_No_Monster_Move);                    ///< Spline raw flags
        l_Data << uint8(l_Splineflags.getAnimationId());                                            ///< Animation ID
        l_Data << int32(l_MoveSpline.effect_start_time);                                            ///< Animation Time
        l_Data << uint32(l_MoveSpline.TimePassed());                                                ///< Elapsed time
        l_Data << uint32(l_MoveSpline.Duration());                                                  ///< Duration
        l_Data << float(l_MoveSpline.vertical_acceleration);                                        ///< Vertical Acceleration (AKA Jump gravity)
        l_Data << int32(l_MoveSpline.effect_start_time);                                            ///< Parabolic Time (AKA Special time)
        l_Data << uint32(l_UncompressedWayPointCount);                                              ///< Uncompressed waypoint count
        l_Data << uint8(MonsterMoveNormal);                                                         ///< Mode
        l_Data << uint8(0);                                                                         ///< Vehicle exit Voluntary
        l_Data.appendPackGUID(l_TransportGUID);                                                     ///< Transport guid
        l_Data << int8(l_TransportSeat);                                                            ///< Transport seat
        l_Data << uint32(l_CompressedWayPointCount);                                                ///< Compressed waypoint count

        if (l_Splineflags & MoveSplineFlag::UncompressedPath)
        {
            if (l_Splineflags.cyclic)
            {
                uint32 p_Count = l_MoveSpline.spline.getPointCount() - 2;

                ///< fake point, client will erase it from the spline after first cycle done
                l_Data << (l_MoveSpline.spline.getPoint(1).x + (float(50) / 1000.f)) << l_MoveSpline.spline.getPoint(1).y << l_MoveSpline.spline.getPoint(1).z;

                for (uint32 l_I = 0; l_I < p_Count; l_I++)
                {
                    /// Add a salt in points because the client doesn't like to have 2 time the same points
                    l_Data << (l_MoveSpline.spline.getPoint(l_I + 1).x + (float(l_I) / 1000.f))
                        << l_MoveSpline.spline.getPoint(l_I + 1).y
                        << l_MoveSpline.spline.getPoint(l_I + 1).z;
                }
            }
            else
            {
                uint32 l_Count = l_MoveSpline.spline.getPointCount() - 2;

                for (uint32 l_I = 0; l_I < l_Count; l_I++)
                {
                    /// Add a salt in points because the client doesn't like to have 2 time the same points
                    l_Data << (l_MoveSpline.spline.getPoint(l_I + 2).x + (float(l_I) / 1000.f)) << l_MoveSpline.spline.getPoint(l_I + 2).y << l_MoveSpline.spline.getPoint(l_I + 2).z;
                }
            }
        }
        else
        {
            /// Fake waypoints
            l_Data << float(l_SplineDestinationX);                                                  ///< Spline destination X
            l_Data << float(l_SplineDestinationY);                                                  ///< Spline destination Y
            l_Data << float(l_SplineDestinationZ);                                                  ///< Spline destination Z
        }

        if ((l_Splineflags & MoveSplineFlag::UncompressedPath) == 0)
        {
            uint32 l_LastIndex = l_MoveSpline.spline.getPointCount() - 3;
            const Vector3 * l_RealPath = &l_MoveSpline.spline.getPoint(1);

            if (l_LastIndex > 0)
            {
                Vector3 l_Middle = (l_RealPath[0] + l_RealPath[l_LastIndex]) / 2.f;
                Vector3 l_Offset;

                /// end points already appended
                for (uint32 i = 0; i < l_LastIndex; ++i)
                {
                    l_Offset = l_Middle - l_RealPath[i];
                    l_Data.appendPackXYZ(l_Offset.x, l_Offset.y, l_Offset.z);
                }
            }
        }

        l_Data.WriteBits(l_FinalFacingMode, 2);                                                     ///< Facing
        l_Data.WriteBit(0);                                                                         ///< Has monster spline filter
        l_Data.FlushBits();

        if (l_FinalFacingMode == MonsterMoveFacingAngle)
            l_Data << l_MoveSpline.facing.angle;                                                    ///< Facing angle
        else if (l_FinalFacingMode == MonsterMoveFacingTarget)
        {
            l_Data << float(l_MoveSpline.facing.angle);                                             ///< Face direction
            l_Data.appendPackGUID(l_MoveSpline.facing.target);                                      ///< Facing target
        }
        else if (l_FinalFacingMode == MonsterMoveFacingSpot)
            l_Data << l_MoveSpline.facing.f.x << l_MoveSpline.facing.f.y << l_MoveSpline.facing.f.z;///< Facing position

        l_Data.WriteBit(0);                                                                         ///< Crz teleport
        l_Data.WriteBits(0, 2);
        l_Data.FlushBits();

        m_Unit.SendMessageToSet(&l_Data, true);
    }

    void MoveSplineInit::Stop(bool force)
    {
        MoveSpline& l_MoveSpline = *m_Unit.movespline;

        if (force)
        {
            args.flags = MoveSplineFlag::Done;
            m_Unit.m_movementInfo.RemoveMovementFlag(MOVEMENTFLAG_FORWARD);
            l_MoveSpline.Initialize(args);
            return;
        }

        // No need to stop if we are not moving
        if (l_MoveSpline.Finalized())
            return;

        Location loc = l_MoveSpline.ComputePosition();
        args.flags = MoveSplineFlag::Done;
        m_Unit.m_movementInfo.RemoveMovementFlag(MOVEMENTFLAG_FORWARD);
        l_MoveSpline.Initialize(args);
        
        uint64 l_MoverGUID = m_Unit.GetGUID();
        uint64 l_TransportGUID = m_Unit.GetTransGUID();

        uint8   l_FinalFacingMode   = MonsterMoveStop;
        int8    l_TransportSeat     = m_Unit.GetTransSeat();

        WorldPacket l_Data(SMSG_MONSTER_MOVE, 2 * 1024);

        l_Data.appendPackGUID(l_MoverGUID);
        l_Data << float(m_Unit.GetPositionX());                                                     ///< Spline start X
        l_Data << float(m_Unit.GetPositionY());                                                     ///< Spline start Y
        l_Data << float(m_Unit.GetPositionZ());                                                     ///< Spline start Z
        l_Data << uint32(getMSTime());                                                              ///< Move Ticks
        l_Data << float(0);                                                                         ///< Spline destination X
        l_Data << float(0);                                                                         ///< Spline destination Y
        l_Data << float(0);                                                                         ///< Spline destination Z
        l_Data << uint32(0);                                                                        ///< Spline raw flags
        l_Data << uint8(0);                                                                         ///< Animation ID
        l_Data << int32(0);                                                                         ///< Animation Time
        l_Data << uint32(0);                                                                        ///< Elapsed time
        l_Data << uint32(0);                                                                        ///< Duration
        l_Data << float(0);                                                                         ///< Vertical Acceleration
        l_Data << int32(0);                                                                         ///< Parabolic Time
        l_Data << uint32(1);                                                                        ///< Uncompressed waypoint count
        l_Data << uint8(MonsterMoveStop);                                                           ///< Mode
        l_Data << uint8(0);                                                                         ///< Vehicle exit Voluntary
        l_Data.appendPackGUID(l_TransportGUID);                                                     ///< Transport guid
        l_Data << int8(l_TransportSeat);                                                            ///< Transport seat
        l_Data << uint32(0);                                                                        ///< Compressed waypoint count

        l_Data << float(loc.x);                                                                     ///< Fake WAYPOINT
        l_Data << float(loc.y);                                                                     ///< Fake WAYPOINT
        l_Data << float(loc.z);                                                                     ///< Fake WAYPOINT

        l_Data.WriteBits(0, 2);
        l_Data.WriteBit(0);
        l_Data.FlushBits();

        l_Data.WriteBit(0);
        l_Data.WriteBits(0, 2);
        l_Data.FlushBits();

        m_Unit.SendMessageToSet(&l_Data, true);
    }

    MoveSplineInit::MoveSplineInit(Unit& m) : m_Unit(m)
    {
        args.splineId = splineIdGen.NewId();
        // Elevators also use MOVEMENTFLAG_ONTRANSPORT but we do not keep track of their position changes
        args.TransformForTransport = m_Unit.GetTransGUID();
        // mix existing state into new
        args.flags.walkmode = m_Unit.m_movementInfo.HasMovementFlag(MOVEMENTFLAG_WALKING);
        args.flags.flying = m_Unit.m_movementInfo.HasMovementFlag(MovementFlags(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_DISABLE_GRAVITY));
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
            if (Unit* vehicle = m_Unit.GetVehicleBase())
                angle -= vehicle->GetOrientation();
            else if (Transport* transport = m_Unit.GetTransport())
                angle -= transport->GetOrientation();
        }

        args.facing.angle = G3D::wrap(angle, 0.f, (float)G3D::twoPi());
        args.flags.EnableFacingAngle();
    }

    void MoveSplineInit::MoveTo(Vector3 const& dest)
    {
        args.path_Idx_offset = 0;
        args.path.resize(2);
        TransportPathTransform transform(m_Unit, args.TransformForTransport);
        args.path[1] = transform(dest);
    }

    void MoveSplineInit::SetFall()
    {
        args.flags.EnableFalling();
        args.flags.fallingSlow = m_Unit.HasUnitMovementFlag(MOVEMENTFLAG_FALLING_SLOW);
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
