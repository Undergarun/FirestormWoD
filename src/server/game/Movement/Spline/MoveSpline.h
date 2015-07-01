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

#ifndef TRINITYSERVER_MOVEPLINE_H
#define TRINITYSERVER_MOVEPLINE_H

#include "Spline.h"
#include "MoveSplineInitArgs.h"

namespace Movement
{
    struct Location : public Vector3
    {
        Location() : orientation(0) {}
        Location(float x, float y, float z, float o) : Vector3(x,y,z), orientation(o) {}
        Location(const Vector3& v) : Vector3(v), orientation(0) {}
        Location(const Vector3& v, float o) : Vector3(v), orientation(o) {}

        float orientation;
    };

    // MoveSpline represents smooth catmullrom or linear curve and point that moves belong it
    // curve can be cyclic - in this case movement will be cyclic
    // point can have vertical acceleration motion componemt(used in fall, parabolic movement)
    class MoveSpline
    {
    public:
        typedef Spline<int32> MySpline;
        enum UpdateResult
        {
            Result_None         = 0x01,
            Result_Arrived      = 0x02,
            Result_NextCycle    = 0x04,
            Result_NextSegment  = 0x08,
        };
        friend class PacketBuilder;
        friend class MoveSplineInit;
        friend class Object;

//    protected:
        MySpline        spline;

        FacingInfo      facing;

        uint32          m_Id;

        MoveSplineFlag  splineflags;

        int32           m_TimePassed;
        // currently duration mods are unused, but its _currently_
        //float           duration_mod;
        //float           duration_mod_next;
        float           vertical_acceleration;
        float           initialOrientation;
        float           m_Velocity;
        int32           effect_start_time;
        int32           point_Idx;
        int32           point_Idx_offset;

        void init_spline(MoveSplineInitArgs const& args);

    public:

        MySpline::ControlArray const& getPath() const { return spline.getPoints(); }
        void computeParabolicElevation(float& el) const;
        void computeFallElevation(float& el) const;

        UpdateResult _UpdateState(int32& p_Diff);
        int32 NextTime() const { return spline.length(point_Idx+1); }
        int32 NextSegTime() const { return NextTime() - m_TimePassed; }
        int32 Duration() const { return spline.length(); }
        int32 TimeElapsed() const { return Duration() - m_TimePassed; }
        int32 TimePassed() const { return m_TimePassed; }

    public:
        const MySpline& _Spline() const { return spline; }
        int32 _currentSplineIdx() const { return point_Idx; }
        void _Finalize();
        void _Interrupt() { splineflags.done = true;}

    public:

        void Initialize(MoveSplineInitArgs const&);
        bool Initialized() const { return !spline.empty(); }

        explicit MoveSpline();

        template<class UpdateHandler>
        void updateState(int32 p_Diff, UpdateHandler& p_Handler)
        {
            ASSERT(Initialized());
            do
                p_Handler(_UpdateState(p_Diff));
            while(p_Diff > 0);
        }

        void updateState(int32 p_Diff)
        {
            ASSERT(Initialized());
            do
                _UpdateState(p_Diff);
            while(p_Diff > 0);
        }

        Location ComputePosition() const;

        uint32 GetId() const { return m_Id; }
        bool Finalized() const { return splineflags.done; }
        bool isCyclic() const { return splineflags.cyclic; }
        Vector3 const FinalDestination() const { return Initialized() ? spline.getPoint(spline.last()) : Vector3(); }
        Vector3 const CurrentDestination() const { return Initialized() ? spline.getPoint(point_Idx + 1) : Vector3(); }
        int32 currentPathIdx() const;

        bool onTransport;
        std::string ToString() const;
    };
}
#endif // TRINITYSERVER_MOVEPLINE_H
