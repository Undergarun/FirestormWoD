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

#include "ObjectAccessor.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "Unit.h"
#include "SpellInfo.h"
#include "Log.h"
#include "AreaTrigger.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

AreaTrigger::AreaTrigger()
    : WorldObject(false),
    m_Duration(0),
    m_Caster(NULL),
    m_VisualRadius(0.0f),
    m_Script(nullptr)
{
    m_objectType |= TYPEMASK_AREATRIGGER;
    m_objectTypeId = TYPEID_AREATRIGGER;

    m_updateFlag = UPDATEFLAG_HAS_POSITION | UPDATEFLAG_HAS_AREATRIGGER;

    m_valuesCount = AREATRIGGER_END;
    m_CreatedTime = 0;

    m_Trajectory = AREATRIGGER_INTERPOLATION_NONE;
    m_Templates.clear();
}

AreaTrigger::~AreaTrigger()
{
    if (m_Script)
        delete m_Script;
}

void AreaTrigger::AddToWorld()
{
    ///- Register the AreaTrigger for guid lookup and for caster
    if (!IsInWorld())
    {
        sObjectAccessor->AddObject(this);
        WorldObject::AddToWorld();
        BindToCaster();
    }
}

void AreaTrigger::RemoveFromWorld()
{
    ///- Remove the AreaTrigger from the accessor and from all lists of objects in world
    if (IsInWorld())
    {
        UnbindFromCaster();
        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool AreaTrigger::CreateAreaTrigger(uint32 guidlow, Unit* caster, SpellInfo const* spell, uint32 p_EffIndex, Position const& pos, Position const& p_Dest)
{
    SetMap(caster->GetMap());
    Relocate(pos);
    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger (spell %u) not created. Invalid coordinates (X: %f Y: %f)", spell->Id, GetPositionX(), GetPositionY());
        return false;
    }

    WorldObject::_Create(guidlow, HIGHGUID_AREATRIGGER, caster->GetPhaseMask());

    const AreaTriggerTemplateList* l_Templates = sObjectMgr->GetAreaTriggerTemplatesForSpell(spell->Id);
    if (l_Templates == nullptr)
        return false;

    for (AreaTriggerTemplateList::const_iterator l_Itr = l_Templates->begin(); l_Itr != l_Templates->end(); l_Itr++)
    {
        const AreaTriggerTemplate l_AreaTriggerTemplate = *l_Itr;
        if (l_AreaTriggerTemplate.m_EffIndex == p_EffIndex)
            m_Templates.push_back(l_AreaTriggerTemplate);
    }

    const AreaTriggerTemplate* l_MainTemplate = GetMainTemplate();

    if (l_MainTemplate == nullptr)
        return false;

    if (l_MainTemplate->m_SpellID == 0 && l_MainTemplate->m_Entry == 0)
        return false;

    SetEntry(l_MainTemplate->m_Entry);
    SetDuration(spell->GetDuration());
    SetObjectScale(1);

    SetGuidValue(AREATRIGGER_FIELD_CASTER, caster->GetGUID());
    SetUInt32Value(AREATRIGGER_FIELD_SPELL_ID, spell->Id);
    SetUInt32Value(AREATRIGGER_FIELD_SPELL_VISUAL_ID, spell->SpellVisual[0]);

    SetSource(pos);
    SetDestination(p_Dest);
    SetTrajectory(pos != p_Dest ? AREATRIGGER_INTERPOLATION_LINEAR : AREATRIGGER_INTERPOLATION_NONE);
    SetUpdateTimerInterval(60);

    if (spell->GetDuration() != -1)
        SetUInt32Value(AREATRIGGER_FIELD_DURATION, spell->GetDuration());

    SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, GetFloatValue(OBJECT_FIELD_SCALE));

    if (!GetMap()->AddToMap(this))
        return false;

    sScriptMgr->OnCreateAreaTriggerEntity(this);

    return true;
}

bool AreaTrigger::CreateAreaTrigger(uint32 p_Entry, uint32 p_GuidLow, uint32 p_PhaseMask, uint32 p_SpellVisualID, Position const& p_Pos, uint32 p_Duration, Map* p_Map)
{
    ASSERT(p_Map != nullptr);

    SetMap(p_Map);
    Relocate(p_Pos);
    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger (entry %u) not created. Invalid coordinates (X: %f Y: %f)", p_Entry, GetPositionX(), GetPositionY());
        return false;
    }

    WorldObject::_Create(p_GuidLow, HIGHGUID_AREATRIGGER, p_PhaseMask);

    SetEntry(p_Entry);
    SetDuration(p_Duration);
    SetObjectScale(1);

    SetGuidValue(AREATRIGGER_FIELD_CASTER, 0);
    SetUInt32Value(AREATRIGGER_FIELD_SPELL_ID, 0);
    SetUInt32Value(AREATRIGGER_FIELD_SPELL_VISUAL_ID, p_SpellVisualID);

    SetSource(p_Pos);
    SetTrajectory(AREATRIGGER_INTERPOLATION_NONE);
    SetUpdateTimerInterval(60);

    if (p_Duration != -1)
        SetUInt32Value(AREATRIGGER_FIELD_DURATION, p_Duration);

    SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, GetFloatValue(OBJECT_FIELD_SCALE));

    if (!GetMap()->AddToMap(this))
        return false;

    sScriptMgr->OnCreateAreaTriggerEntity(this);

    return true;
}

void AreaTrigger::Update(uint32 p_Time)
{
    // Don't decrease infinite durations
    if (GetDuration() > int32(p_Time))
        m_Duration -= p_Time;
    else if (GetDuration() != -1 && int32(m_CreatedTime) > GetDuration())
        Remove(p_Time); // expired

    m_CreatedTime += p_Time;
    WorldObject::Update(p_Time);

    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(GetUInt32Value(AREATRIGGER_FIELD_SPELL_ID));
    if (!l_SpellInfo)
        return;

    if (!GetCaster())
    {
        Remove(p_Time);
        return;
    }

    Unit* l_Caster = GetCaster();
    float l_Radius = 0.0f;

    sScriptMgr->OnUpdateAreaTriggerEntity(this, p_Time);

    m_UpdateTimer.Update(p_Time);

    if (m_UpdateTimer.Passed())
    {
        m_UpdateTimer.Reset();

        // Calculate new position
        if (GetMainTemplate()->m_MoveCurveID != 0)
        {
            UpdatePositionWithPathId(m_CreatedTime, this);
        }
        else if (m_Trajectory)
        {
            GetPositionAtTime(m_CreatedTime, this);
        }
    }
}

void AreaTrigger::Remove(uint32 p_time)
{
    if (IsInWorld())
    {
        SpellInfo const* m_spellInfo = sSpellMgr->GetSpellInfo(GetUInt32Value(AREATRIGGER_FIELD_SPELL_ID));
        if (!m_spellInfo)
            return;

        sScriptMgr->OnRemoveAreaTriggerEntity(this, p_time);

        SendObjectDeSpawnAnim(GetGUID());
        RemoveFromWorld();
        AddObjectToRemoveList();
    }
}

void AreaTrigger::BindToCaster()
{
    m_Caster = ObjectAccessor::GetUnit(*this, GetCasterGUID());

    if (m_Caster)
        m_Caster->_RegisterAreaTrigger(this);
}

void AreaTrigger::UnbindFromCaster()
{
    if (m_Caster)
    {
        m_Caster->_UnregisterAreaTrigger(this);
        m_Caster = NULL;
    }
}

void AreaTrigger::SendMovementUpdate()
{
    WorldPacket l_Data(SMSG_AREA_TRIGGER_MOVEMENT_UPDATE, 24);

    // startX
    // startY
    // endY
    // endZ
    // startZ
    // endX
}

void AreaTrigger::UpdatePositionWithPathId(uint32 p_Time, Position* p_OutPos)
{
    const AreaTriggerTemplate* l_template = GetMainTemplate();

    if (l_template->m_Flags & AREATRIGGER_FLAG_HAS_MOVE_CURVE)
    {
        AreaTriggerMoveTemplate l_MoveTemplate = sObjectMgr->GetAreaTriggerMoveTemplate(l_template->m_MoveCurveID);
        if (l_MoveTemplate.m_path_size == 0)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger Move Template (entry %u) not in DB.", l_template->m_MoveCurveID);
            return; // ERROR.
        }

        int32 l_LocalDuration = l_MoveTemplate.m_duration / l_MoveTemplate.m_path_size;
        float l_Progress = float(p_Time % l_LocalDuration) / l_LocalDuration;
        uint32 l_PathId = float(p_Time) / l_LocalDuration;

        // We want to interpolate so that to get true position on server side.
        AreaTriggerMoveSplines l_spline0 = sObjectMgr->GetAreaTriggerMoveSplines(l_template->m_MoveCurveID, l_PathId);
        AreaTriggerMoveSplines l_spline1 = sObjectMgr->GetAreaTriggerMoveSplines(l_template->m_MoveCurveID, l_PathId + 1);

        if (l_spline0.m_move_id == 0 || l_spline1.m_move_id == 0)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger Move Splines (entry %u) not in DB.", l_template->m_MoveCurveID);
            return; // ERROR.
        }

        float l_Cos = cos(m_Source.m_orientation - M_PI / 2);
        float l_Sin = sin(m_Source.m_orientation - M_PI / 2);
        float l_X = l_spline0.m_path_x + l_Progress * (l_spline1.m_path_x - l_spline0.m_path_x);
        float l_Y = l_spline0.m_path_y + l_Progress * (l_spline1.m_path_y - l_spline0.m_path_y);

        p_OutPos->m_positionX = m_Source.m_positionX + l_X * l_Cos - l_Y * l_Sin;
        p_OutPos->m_positionY = m_Source.m_positionY + l_X * l_Sin + l_Y * l_Cos;
        p_OutPos->m_positionZ = m_Source.m_positionZ + l_spline0.m_path_z + l_Progress * (l_spline1.m_path_z - l_spline0.m_path_z);
        p_OutPos->m_orientation = m_Source.m_orientation;
    }
}

void AreaTrigger::GetPositionFromPathId(uint32 p_PathId, Position* p_OutPos) const
{
    const AreaTriggerTemplate* l_template = GetMainTemplate();

    if (l_template->m_Flags & AREATRIGGER_FLAG_HAS_MOVE_CURVE)
    {
        AreaTriggerMoveSplines l_spline = sObjectMgr->GetAreaTriggerMoveSplines(l_template->m_MoveCurveID, p_PathId);

        if (l_spline.m_move_id == 0)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AreaTrigger Move Splines (entry %u) not in DB.", l_template->m_MoveCurveID);
            return; // ERROR.
        }

        float l_Cos = cos(m_Source.m_orientation - M_PI / 2);
        float l_Sin = sin(m_Source.m_orientation - M_PI / 2);

        p_OutPos->m_positionX = m_Source.m_positionX + l_spline.m_path_x * l_Cos - l_spline.m_path_y * l_Sin;
        p_OutPos->m_positionY = m_Source.m_positionY + l_spline.m_path_x * l_Sin + l_spline.m_path_y * l_Cos;
        p_OutPos->m_positionZ = m_Source.m_positionZ + l_spline.m_path_z;
        p_OutPos->m_orientation = m_Source.m_orientation;
    }
}

void AreaTrigger::GetPositionAtTime(uint32 p_Time, Position* p_OutPos) const
{
    switch (m_Trajectory)
    {
    case AREATRIGGER_INTERPOLATION_LINEAR:
    {
        int32 l_Duration = GetDuration();
        float l_Progress = float(p_Time % l_Duration) / l_Duration;
        p_OutPos->m_positionX = m_Source.m_positionX + l_Progress * (m_Destination.m_positionX - m_Source.m_positionX);
        p_OutPos->m_positionY = m_Source.m_positionY + l_Progress * (m_Destination.m_positionY - m_Source.m_positionY);
        p_OutPos->m_positionZ = m_Source.m_positionZ + l_Progress * (m_Destination.m_positionZ - m_Source.m_positionZ);
        p_OutPos->m_orientation = m_Source.m_orientation + l_Progress * (m_Destination.m_orientation - m_Source.m_orientation);
        break;
    }
    default:
        *p_OutPos = m_Source;
        break;
    }
}