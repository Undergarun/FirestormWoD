/*
 * Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#ifndef TRINITYCORE_AREATRIGGER_H
#define TRINITYCORE_AREATRIGGER_H

#include "Object.h"
#include "Timer.h"

class Unit;
class SpellInfo;

enum AreatriggerInterpolation
{
    AREATRIGGER_INTERPOLATION_NONE,
    AREATRIGGER_INTERPOLATION_LINEAR
};

enum eAreaTriggerFlags
{
    AREATRIGGER_FLAG_NONE                   = 0x0000,
    AREATRIGGER_FLAG_ABSOLUTE_ORIENTATION   = 0x0001,
    AREATRIGGER_FLAG_DYNAMIC_SHAPE          = 0x0002,
    AREATRIGGER_FLAG_ATTACHED               = 0x0004,
    AREATRIGGER_FLAG_FACE_MOVEMENT_DIR      = 0x0008,
    AREATRIGGER_FLAG_FOLLOWS_TERRAIN        = 0x0010,
    AREATRIGGER_FLAG_HAS_TARGET_ROLL_PITCH  = 0x0020,
    AREATRIGGER_FLAG_HAS_SCALE_CURVE        = 0x0040,
    AREATRIGGER_FLAG_HAS_MORPH_CURVE        = 0x0080,
    AREATRIGGER_FLAG_HAS_FACING_CURVE       = 0x0100,
    AREATRIGGER_FLAG_HAS_MOVE_CURVE         = 0x0200,
    AREATRIGGER_FLAG_UNUSED_0400            = 0x0400,
    AREATRIGGER_FLAG_AREATRIGGER_BOX        = 0x0800,
    AREATRIGGER_FLAG_AREATRIGGER_POLYGON    = 0x1000,
    AREATRIGGER_FLAG_AREATRIGGER_CYLINDER   = 0x2000,
    AREATRIGGER_FLAG_AREATRIGGER_SPHERE     = 0x4000
};

enum eAreaTriggerTypes
{
    AREATRIGGER_TYPE_POLYGON,
    AREATRIGGER_TYPE_BOX,
    AREATRIGGER_TYPE_SPHERE,
    AREATRIGGER_TYPE_CYLINDER,
    AREATRIGGER_TYPE_SPLINE
};

#define MAX_AREATRIGGER_DATA 8

struct AreaTriggerTemplate
{
    uint32 m_SpellID;
    uint32 m_EffIndex;
    uint32 m_Entry;
    uint8  m_Type;
    float  m_ScaleX;
    float  m_ScaleY;
    uint32 m_Flags;
    uint32 m_MoveCurveID;
    uint32 m_ScaleCurveID;
    uint32 m_MorphCurveID;
    uint32 m_FacingCurveID;

    union
    {
        // AREATRIGGER_TYPE_POLYGON
        struct
        {
            float  m_Vertices[2];
            float  m_VerticesTarget[2];
            float  m_Height;
            float  m_HeightTarget;
            uint32 m_VerticesCount;
            uint32 m_VerticesTargetCount;
        } m_PolygonDatas;

        // AREATRIGGER_TYPE_BOX
        struct
        {
            float m_Extent[3];
            float m_ExtentTarget[3];
        } m_BoxDatas;

        // Not use for specific field access (only for output with loop by all filed), also this determinate max union size
        struct
        {
            float m_Data[MAX_AREATRIGGER_DATA];
        } m_Raw;

        // AREATRIGGER_TYPE_SPHERE

        // AREATRIGGER_TYPE_CYLINDER
        struct
        {
            float m_Extent[3];
            float m_ExtentTarget[3];
        } m_CylinderDatas;

        // AREATRIGGER_TYPE_SPLINE
        // Implemented in WoD
    };
};

typedef std::list<AreaTriggerTemplate> AreaTriggerTemplateList;
typedef std::map<uint32, AreaTriggerTemplateList> AreaTriggerTemplateContainer;

class AreaTrigger : public WorldObject, public GridObject<AreaTrigger>
{
    public:
        AreaTrigger();
        ~AreaTrigger();

        void AddToWorld();
        void RemoveFromWorld();

        bool CreateAreaTrigger(uint32 guidlow, Unit* caster, SpellInfo const* spell, uint32 p_EffIndex, Position const& pos, Position const& p_Dest);
        void Update(uint32 p_time);

        void Remove(uint32 p_time);
        uint32 GetSpellId() const { return GetUInt32Value(AREATRIGGER_FIELD_SPELL_ID); }
        int32 GetDuration() const { return m_Duration; }
        void SetDuration(int32 newDuration) { m_Duration = newDuration; }
        void Delay(int32 delaytime) { SetDuration(GetDuration() - delaytime); }
        Unit* GetCaster() const { return m_Caster; }
        uint64 GetCasterGUID() const { return GetGuidValue(AREATRIGGER_FIELD_CASTER); }

        void BindToCaster();
        void UnbindFromCaster();

        uint32 GetCreatedTime() const { return m_CreatedTime; }

        float GetVisualRadius() const { return m_VisualRadius; }
        void SetVisualRadius(float radius) { m_VisualRadius = radius; }

        uint32 GetUpdateInterval() const { return m_UpdateTimer.GetInterval(); }
        AreatriggerInterpolation GetTrajectory() const { return m_Trajectory; }

        AreaTriggerTemplateList GetTemplates() const { return m_Templates; }
        AreaTriggerTemplate GetMainTemplate() const { return !m_Templates.empty() ? m_Templates.front() : AreaTriggerTemplate(); }

        void SendMovementUpdate();

        void GetPositionAtTime(uint32 p_Time, Position* p_OutPos) const;

        void SetSource(Position p_Source) { m_Source = p_Source; }
        void SetDestination(Position p_Dest) { m_Destination = p_Dest; }
        void SetTrajectory(AreatriggerInterpolation p_Trajectory) { m_Trajectory = p_Trajectory; }
        void SetUpdateTimerInterval(uint32 p_Timer) { m_UpdateTimer.SetInterval(p_Timer); }

    protected:
        int32 m_Duration;
        Unit* m_Caster;
        float m_VisualRadius;
        uint32 m_CreatedTime;
        Position m_Source;
        Position m_Destination;
        AreatriggerInterpolation m_Trajectory;
        IntervalTimer m_UpdateTimer;
        AreaTriggerTemplateList m_Templates;

};
#endif