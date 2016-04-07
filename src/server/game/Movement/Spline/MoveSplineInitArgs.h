////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITYSERVER_MOVESPLINEINIT_ARGS_H
#define TRINITYSERVER_MOVESPLINEINIT_ARGS_H

#include "MoveSplineFlag.h"
#include <G3D/Vector3.h>

class Unit;

namespace Movement
{
    typedef std::vector<Vector3> PointsArray;

    struct FacingInfo
    {
        G3D::Vector3 f;
        uint64 target;
        float angle;

        FacingInfo() : angle(0.0f) { }
    };

    struct MoveSplineInitArgs
    {
        MoveSplineInitArgs(size_t path_capacity = 16) : path_Idx_offset(0), velocity(0.0f),
            parabolic_amplitude(0.0f), time_perc(0.0f), splineId(0), initialOrientation(0.0f),
            HasVelocity(false), TransformForTransport(true)
        {
            path.reserve(path_capacity);
        }

        PointsArray path;
        FacingInfo facing;
        MoveSplineFlag flags;
        int32 path_Idx_offset;
        float velocity;
        float parabolic_amplitude;
        float time_perc;
        uint32 splineId;
        float initialOrientation;
        bool HasVelocity;
        bool TransformForTransport;

        /** Returns true to show that the arguments were configured correctly and MoveSpline initialization will succeed. */
        bool Validate(Unit* unit) const;

    private:
        bool _checkPathBounds() const;
    };
}

#endif // TRINITYSERVER_MOVESPLINEINIT_ARGS_H
