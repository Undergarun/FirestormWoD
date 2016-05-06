////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_POINTMOVEMENTGENERATOR_H
#define TRINITY_POINTMOVEMENTGENERATOR_H

#include "MovementGenerator.h"
#include "FollowerReference.h"

template<class T>
class PointMovementGenerator : public MovementGeneratorMedium< T, PointMovementGenerator<T> >
{
    public:
        PointMovementGenerator(uint32 _id, float _x, float _y, float _z, bool _generatePath, float _speed = 0.0f, float p_Angle = -1000.0f) : id(_id),
            i_x(_x), i_y(_y), i_z(_z), speed(_speed), m_Angle(p_Angle), m_generatePath(_generatePath), i_recalculateSpeed(false) { }

        void DoInitialize(T*);
        void DoFinalize(T*);
        void DoReset(T*);
        bool DoUpdate(T*, uint32);

        void MovementInform(T*);

        void unitSpeedChanged() override { i_recalculateSpeed = true; }

        MovementGeneratorType GetMovementGeneratorType() const override { return POINT_MOTION_TYPE; }

        void GetDestination(float& x, float& y, float& z) const { x = i_x; y = i_y; z = i_z; }
    private:
        uint32 id;
        float i_x, i_y, i_z;
        float speed;
        float m_Angle;
        bool m_generatePath;
        bool i_recalculateSpeed;
};

class AssistanceMovementGenerator : public PointMovementGenerator<Creature>
{
    public:
        AssistanceMovementGenerator(float _x, float _y, float _z) :
            PointMovementGenerator<Creature>(0, _x, _y, _z, true) { }

        MovementGeneratorType GetMovementGeneratorType() const override { return ASSISTANCE_MOTION_TYPE; }
        void Finalize(Unit*) override;
};

// Does almost nothing - just doesn't allows previous movegen interrupt current effect.
class EffectMovementGenerator : public MovementGenerator
{
    public:
        EffectMovementGenerator(uint32 id, uint32 arrivalSpellId = 0, uint64 const arrivalSpellTargetGuid = 0)
            : _id(id), _arrivalSpellId(arrivalSpellId), _arrivalSpellTargetGuid(arrivalSpellTargetGuid) { }
        void Initialize(Unit*) override { }
        void Finalize(Unit*) override;
        void Reset(Unit*) override { }
        bool Update(Unit*, uint32) override;
        MovementGeneratorType GetMovementGeneratorType() const override { return EFFECT_MOTION_TYPE; }
        void MovementInform(Unit*);

    private:
        uint32 _id;
        uint32 _arrivalSpellId;
        ObjectGuid _arrivalSpellTargetGuid;
};

#endif
