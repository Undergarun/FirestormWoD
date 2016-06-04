////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITYSERVER_TYPEDEFS_H
#define TRINITYSERVER_TYPEDEFS_H

#include "Common.h"

namespace G3D
{
    class Vector3;
    class Vector4;
}

namespace Movement
{
    using G3D::Vector3;
    using G3D::Vector4;

    enum MonsterMoveType
    {
        MONSTER_MOVE_NORMAL         = 0,
        MONSTER_MOVE_FACING_SPOT    = 1,
        MONSTER_MOVE_FACING_TARGET  = 2,
        MONSTER_MOVE_FACING_ANGLE   = 3
    };

    inline uint32 SecToMS(float sec)
    {
        return static_cast<uint32>(sec * 1000.0f);
    }

    inline float MSToSec(uint32 ms)
    {
        return ms / 1000.0f;
    }

    float computeFallTime(float path_length, bool isSafeFall);
    float computeFallElevation(float t_passed, bool isSafeFall, float start_velocity = 0.0f);

    template<class T, T limit>
    class counter
    {
    public:
        counter() { init(); }

        void Increase()
        {
            if (m_counter == limit)
                init();
            else
                ++m_counter;
        }

        T NewId() { Increase(); return m_counter; }
        T getCurrent() const { return m_counter; }

    private:
        void init() { m_counter = 0; }
        T m_counter;
    };

    typedef counter<uint32, 0xFFFFFFFF> UInt32Counter;

    extern float gravity;
    extern UInt32Counter splineIdGen;
    extern std::string MovementFlags_ToString(uint32 flags);
    extern std::string MovementFlagsExtra_ToString(uint32 flags);
}

#endif // TRINITYSERVER_TYPEDEFS_H
