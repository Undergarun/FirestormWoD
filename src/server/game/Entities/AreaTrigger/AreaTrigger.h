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
# define TRINITYCORE_AREATRIGGER_H

# include "Object.h"
# include "Timer.h"

class Unit;
class SpellInfo;
class AreaTriggerEntityScript;

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
    AreaTriggerTemplate()
    {
        m_SpellID               = 0;
        m_EffIndex              = 0;
        m_Entry                 = 0;
        m_Type                  = 0;
        m_ScaleX                = 0.0f;
        m_ScaleY                = 0.0f;
        m_Flags                 = 0;
        m_MoveCurveID           = 0;
        m_ScaleCurveID          = 0;
        m_MorphCurveID          = 0;
        m_FacingCurveID         = 0;

        m_ScriptId              = 0;
        m_CreatureVisualEntry   = 0;

        for (uint32 l_I = 0; l_I < MAX_AREATRIGGER_DATA; l_I++)
            m_Raw.m_Data[l_I]   = 0;
    }

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

    uint32 m_ScriptId;
    uint32 m_CreatureVisualEntry;

    union
    {
        // Not use for specific field access (only for output with loop by all filed), also this determinate max union size
        struct
        {
            uint32 m_Data[MAX_AREATRIGGER_DATA];
        } m_Raw;

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

        // AREATRIGGER_TYPE_SPHERE

        // AREATRIGGER_TYPE_CYLINDER
        struct
        {
            float Radius;
            float RadiusTarget;
            float Height;
            float HeightTarget;
            float Unk1;
            float Unk2;
        } m_CylinderDatas;

        // AREATRIGGER_TYPE_SPLINE
        struct
        {
            uint32 TimeToTarget;
        } m_SplineDatas;

        // Implemented in WoD
    };

    bool HasAbsoluteOrientation() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_ABSOLUTE_ORIENTATION;
    }

    bool HasDynamicShape() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_DYNAMIC_SHAPE;
    }

    bool HasAttached() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_ATTACHED;
    }

    bool HasFaceMovementDir() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_FACE_MOVEMENT_DIR;
    }

    bool HasFollowsTerrain() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_FOLLOWS_TERRAIN;
    }

    bool HasTargetRollPitchYaw() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_HAS_TARGET_ROLL_PITCH;
    }

    bool HasScaleCurveID() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_HAS_SCALE_CURVE;
    }

    bool HasMorphCurveID() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_HAS_MORPH_CURVE;
    }

    bool HasFacingCurveID() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_HAS_FACING_CURVE;
    }

    bool HasMoveCurveID() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_HAS_MOVE_CURVE;
    }

    bool HasAreaTriggerSphere() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_AREATRIGGER_SPHERE;
    }

    bool HasAreaTriggerBox() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_AREATRIGGER_BOX;
    }

    bool HasAreaTriggerPolygon() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_AREATRIGGER_POLYGON;
    }

    bool HasAreaTriggerCylinder() const
    {
        return m_Flags & eAreaTriggerFlags::AREATRIGGER_FLAG_AREATRIGGER_CYLINDER;
    }
};

struct AreaTriggerMoveTemplate
{
    AreaTriggerMoveTemplate()
    {
        memset(this, 0, sizeof (AreaTriggerMoveTemplate));
    }

    // Maybe we can get those information an other way because duration is in the spellInfos and path_size is the number of MoveSplines in DB.
    uint32 m_move_id;
    uint32 m_path_size;
    uint32 m_duration;
};

struct AreaTriggerMoveSplines
{
    AreaTriggerMoveSplines()
    {
        memset(this, 0, sizeof (AreaTriggerMoveSplines));
    }

    uint32 m_move_id;
    uint32 m_path_id;
    float m_path_x;
    float m_path_y;
    float m_path_z;
};

typedef std::map<uint32, AreaTriggerMoveTemplate> AreaTriggerMoveTemplateContainer;
typedef std::map<std::pair<uint32, uint32>, AreaTriggerMoveSplines> AreaTriggerMoveSplinesContainer;

typedef std::list<AreaTriggerTemplate> AreaTriggerTemplateList;
typedef std::map<uint32, AreaTriggerTemplateList> AreaTriggerTemplateContainer;

class AreaTrigger : public WorldObject, public GridObject<AreaTrigger>
{
    public:
        AreaTrigger();
        ~AreaTrigger();

        void AddToWorld();
        void RemoveFromWorld();

        bool CreateAreaTriggerFromSpell(uint32 guidlow, Unit* caster, SpellInfo const* spell, uint32 p_EffIndex, Position const& pos, Position const& p_Dest);
        bool CreateAreaTrigger(uint32 p_Entry, uint32 p_GuidLow, uint32 p_PhaseMask, uint32 p_SpellVisualID, Position const& p_Pos, uint32 p_Duration, Map* p_Map);

        void Update(uint32 p_time);

        void Remove(uint32 p_time);
        uint32 GetSpellId() const { return GetUInt32Value(AREATRIGGER_FIELD_SPELL_ID); }
        int32 GetDuration() const { return m_Duration; }
        void SetDuration(int32 newDuration) { m_Duration = newDuration; }
        void Delay(int32 delaytime) { SetDuration(GetDuration() - delaytime); }
        Unit* GetCaster() const { return m_Caster; }
        uint64 GetCasterGUID() const { return GetGuidValue(AREATRIGGER_FIELD_CASTER); }

        void SetTimeToTarget(uint32 p_Time) { m_TimeToTarget = p_Time; }
        uint32 GetTimeToTarget() const { return m_TimeToTarget; }

        void BindToCaster();
        void UnbindFromCaster();

        uint32 GetCreatedTime() const { return m_CreatedTime; }

        uint32 GetUpdateInterval() const { return m_UpdateTimer.GetInterval(); }
        AreatriggerInterpolation GetTrajectory() const { return m_Trajectory; }

        AreaTriggerTemplateList GetTemplates() const { return m_Templates; }
        AreaTriggerTemplate const* GetMainTemplate() const { return !m_Templates.empty() ? &m_Templates.front() : nullptr; }

        AreaTriggerEntityScript* GetScript() const { return m_Script; }
        void SetScript(AreaTriggerEntityScript* p_Script) { m_Script = p_Script; }

        void SendMovementUpdate();

        void GetPositionAtTime(uint32 p_Time, Position* p_OutPos) const;
        void GetPositionFromPathId(uint32 p_MoveCurveId, Position* p_OutPos) const;
        void UpdatePositionWithPathId(uint32 p_Time, Position* p_OutPos);

        uint64 GetGUIDCreatureVisual() { return m_CreatureVisualGUID; };

        void SetSource(Position p_Source) { m_Source = p_Source; }
        void SetDestination(Position p_Dest) { m_Destination = p_Dest; }
        void SetTrajectory(AreatriggerInterpolation p_Trajectory) { m_Trajectory = p_Trajectory; }
        void SetUpdateTimerInterval(uint32 p_Timer) { m_UpdateTimer.SetInterval(p_Timer); }
        void SetPathToLinearDestination(std::list<Position>& l_List) { m_PathToLinearDestination = l_List; }
        Position const& GetDestination() const { return m_Destination; };

        static AreaTrigger* GetAreaTrigger(WorldObject const& p_Object, uint64 p_Guid);

        /*
         * Cast spell by using unit trigger/dummy at areatrigger posiiton
         * Use areatrigger owner (caster) for ennemies/friends selection
         * @param p_Target  : Target of the spell we cast
         * @param p_SpellId : Id of spell to cast
         */
        void CastSpell(Unit* p_Target, uint32 p_SpellId);
        bool IsInAreaTriggerPolygon(std::vector<G3D::Vector2> p_Polygon, G3D::Vector2 p_Point, float p_Radius = 0.5f) const;

        void SendAreaTriggerRePath(uint32 p_TimeToTarget, uint32 p_OldTime);

    protected:
        int32 m_Duration;
        Unit* m_Caster;
        uint32 m_CreatedTime;
        uint32 m_Flags;
        Position m_Source;
        Position m_Destination;
        AreatriggerInterpolation m_Trajectory;
        IntervalTimer m_UpdateTimer;
        AreaTriggerTemplateList m_Templates;
        AreaTriggerEntityScript* m_Script;

        uint32 m_TimeToTarget;

        uint64 m_CreatureVisualGUID;
        std::list<Position> m_PathToLinearDestination;
};
#endif