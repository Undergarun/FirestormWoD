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

AreaTrigger::AreaTrigger() : WorldObject(false), m_Duration(0), m_Caster(NULL), m_VisualRadius(0.0f)
{
    m_objectType |= TYPEMASK_AREATRIGGER;
    m_objectTypeId = TYPEID_AREATRIGGER;

    m_updateFlag = UPDATEFLAG_STATIONARY_POSITION | UPDATEFLAG_AREATRIGGER;

    m_valuesCount = AREATRIGGER_END;
    m_CreatedTime = 0;

    m_Trajectory = AREATRIGGER_INTERPOLATION_NONE;
    m_Templates.clear();
}

AreaTrigger::~AreaTrigger()
{
    ASSERT(!m_Caster);
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

    AreaTriggerTemplateList l_Templates = sObjectMgr->GetAreaTriggerTemplatesForSpell(spell->Id);
    for (AreaTriggerTemplate l_Template : l_Templates)
    {
        if (l_Template.m_EffIndex == p_EffIndex)
            m_Templates.push_back(l_Template);
    }

    AreaTriggerTemplate l_MainTemplate = GetMainTemplate();

    SetEntry(l_MainTemplate.m_Entry);
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

    // Custom MoP Script
    switch (l_SpellInfo->Id)
    {
        case 13810: // Ice Trap
        {
            std::list<Unit*> targetList;
            l_Radius = 10.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            for (auto itr : targetList)
                itr->CastSpell(itr, 135299, true);

            // Glyph of Black Ice
            if (l_Caster->GetDistance(this) <= l_Radius && l_Caster->HasAura(109263) && !l_Caster->HasAura(83559))
                l_Caster->CastSpell(l_Caster, 83559, true);
            else
                l_Caster->RemoveAura(83559);

            break;
        }
        case 62618: // Power Word: Barrier
        {
            std::list<Unit*> targetList;
            l_Radius = 6.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            for (auto itr : targetList)
                itr->CastSpell(itr, 81782, true);

            break;
        }
        case 102793:// Ursol's Vortex
        {
            std::list<Unit*> targetList;
            l_Radius = 8.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
                for (auto itr : targetList)
                    if (!itr->HasAura(127797))
                        l_Caster->CastSpell(itr, 127797, true);

            break;
        }
        case 115460:// Healing Sphere
        {
            std::list<Unit*> targetList;
            l_Radius = 1.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    l_Caster->CastSpell(itr, 115464, true); // Healing Sphere heal
                    SetDuration(0);
                    return;
                }
            }

            break;
        }
        case 115817:// Cancel Barrier
        {
            std::list<Unit*> targetList;
            l_Radius = 6.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
                for (auto itr : targetList)
                    itr->CastSpell(itr, 115856, true);

            break;
        }
        case 116011:// Rune of Power
        {
            std::list<Unit*> targetList;
            l_Radius = 5.0f;

            if (l_Caster->IsWithinDistInMap(this, 5.0f))
            {
                if (!l_Caster->HasAura(116014))
                    l_Caster->CastSpell(l_Caster, 116014, true);
                else if (AuraPtr runeOfPower = l_Caster->GetAura(116014))
                    runeOfPower->RefreshDuration();

                if (l_Caster->ToPlayer())
                    l_Caster->ToPlayer()->UpdateManaRegen();
            }

            break;
        }
        case 116235:// Amethyst Pool
        {
            std::list<Unit*> targetList;
            l_Radius = 5.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    // Amethyst Pool - Periodic Damage
                    if (itr->GetDistance(this) > 3.5f && itr->HasAura(130774))
                        itr->RemoveAura(130774);
                    else if (itr->GetDistance(this) <= 3.5f && !itr->HasAura(130774))
                        l_Caster->CastSpell(itr, 130774, true);
                }
            }
            break;
        }
        case 122731:// Create Cancelling Noise Area trigger
        {
            std::list<Unit*> targetList;
            l_Radius = 10.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    // Periodic absorption for Imperial Vizier Zor'lok's Force and Verve and Sonic Rings
                    if (itr->GetDistance(this) > 2.0f && itr->HasAura(122706))
                        itr->RemoveAura(122706);
                    else if (itr->GetDistance(this) <= 2.0f && !itr->HasAura(122706))
                        l_Caster->AddAura(122706, itr);
                }
            }
            break;
        }
        case 123461:// Get Away!
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, 60.0f);

            Position pos;
            GetPosition(&pos);

            for (auto player : playerList)
            {
                if (player->IsWithinDist(l_Caster, 40.0f, false))
                {
                    if (player->isAlive() && !player->hasForcedMovement)
                        player->SendApplyMovementForce(true, pos, -3.0f);
                    else if (!player->isAlive() && player->hasForcedMovement)
                        player->SendApplyMovementForce(false, pos);
                }
                else if (player->hasForcedMovement)
                    player->SendApplyMovementForce(false, pos);
            }

            break;
        }
        case 116546:// Draw Power
        {
            std::list<Unit*> targetList;
            l_Radius = 30.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            for (auto itr : targetList)
            {
                if (itr->IsInAxe(l_Caster, this, 2.0f))
                {
                    if (!itr->HasAura(116663))
                        l_Caster->AddAura(116663, itr);
                }
                else
                    itr->RemoveAurasDueToSpell(116663);
            }

            break;
        }
        case 117032:// Healing Sphere (Afterlife)
        {
            std::list<Unit*> targetList;
            l_Radius = 1.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    if (itr->GetGUID() == l_Caster->GetGUID())
                    {
                        l_Caster->CastSpell(itr, 125355, true); // Heal for 15% of life
                        SetDuration(0);
                        return;
                    }
                }
            }

            break;
        }
        case 119031:// Gift of the Serpent (Mastery)
        {
            std::list<Unit*> targetList;
            l_Radius = 1.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    l_Caster->CastSpell(itr, 124041, true); // Gift of the Serpent heal
                    SetDuration(0);
                    return;
                }
            }

            break;
        }
        case 121286:// Chi Sphere (Afterlife)
        {
            std::list<Unit*> targetList;
            l_Radius = 1.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    if (itr->GetGUID() == l_Caster->GetGUID())
                    {
                        l_Caster->CastSpell(itr, 121283, true); // Restore 1 Chi
                        SetDuration(0);
                        return;
                    }
                }
            }

            break;
        }
        case 121536:// Angelic Feather
        {
            std::list<Unit*> targetList;
            l_Radius = 1.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    l_Caster->CastSpell(itr, 121557, true); // Angelic Feather increase speed
                    SetDuration(0);
                    return;
                }
            }

            break;
        }
        case 124503:// Gift of the Ox
        case 124506:// Gift of the Ox²
        {
            std::list<Unit*> targetList;
            l_Radius = 1.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targetList, u_check);
            VisitNearbyObject(l_Radius, searcher);

            for (auto itr : targetList)
            {
                if (itr->GetGUID() != l_Caster->GetGUID())
                    continue;

                l_Caster->CastSpell(itr, 124507, true); // Gift of the Ox - Heal
                SetDuration(0);
                return;
            }

            break;
        }
        case 134370:// Down Draft
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, 40.0f);

            Position pos;
            GetPosition(&pos);

            for (auto player : playerList)
            {
                if (player->IsWithinDist(l_Caster, 30.0f, false))
                {
                    if (player->isAlive() && !player->hasForcedMovement)
                        player->SendApplyMovementForce(true, pos, -12.0f);
                    else if (!player->isAlive() && player->hasForcedMovement)
                        player->SendApplyMovementForce(false, pos);
                }
                else if (player->hasForcedMovement)
                    player->SendApplyMovementForce(false, pos);
            }

            break;
        }
        case 161210:// Vileblood Serum
        {
            std::list<Unit*> l_TargetList;
            l_Radius = 2.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(this, l_TargetList, l_Check);
            VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(this) <= l_Radius)
                    l_Unit->CastSpell(l_Unit, 161288, true);
            }

            break;
        }
        case 161827:// Noxious Spit
        {
            std::list<Unit*> l_TargetList;
            l_Radius = 2.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(this, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(this, l_TargetList, l_Check);
            VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetExactDist2d(this) > l_Radius)
                    continue;

                m_Caster->CastSpell(l_Unit, 136962, true);
                SetDuration(0);
                return;
            }

            break;
        }
        default:
            break;
    }

    m_UpdateTimer.Update(p_Time);

    if (m_UpdateTimer.Passed())
    {
        m_UpdateTimer.Reset();

        // Calculate new position
        if (m_Trajectory)
            GetPositionAtTime(m_CreatedTime, this);
    }
}

void AreaTrigger::Remove(uint32 p_time)
{
    if (IsInWorld())
    {
        SpellInfo const* m_spellInfo = sSpellMgr->GetSpellInfo(GetUInt32Value(AREATRIGGER_FIELD_SPELL_ID));
        if (!m_spellInfo)
            return;

        switch (m_spellInfo->Id)
        {
            case 115460: // zen sphere
            {
                if (int32(GetDuration()) - int32(p_time) > 0 || GetDuration() == 0)
                    break;

                if (!m_Caster)
                    break;

                m_Caster->CastSpell(GetPositionX(), GetPositionY(), GetPositionZ(), 135914, true);
                break;
            }
            case 119031: // zen sphere
            {
                if (int32(GetDuration()) - int32(p_time) > 0 || GetDuration() == 0)
                    break;

                if (!m_Caster)
                    break;

                m_Caster->CastSpell(GetPositionX(), GetPositionY(), GetPositionZ(), 135920, true);
                break;
            }
            case 116011:// Rune of Power : Remove the buff if caster is still in radius
                if (m_Caster && m_Caster->HasAura(116014))
                    m_Caster->RemoveAura(116014);
                break;
            case 122731:// Create Noise Cancelling Area Trigger
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, 200.0f);

                for (auto player : playerList)
                    if (player->HasAura(122706))
                        player->RemoveAura(122706);
                break;
            }
            case 123461:// Get Away!
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, 100.0f);

                Position pos;
                GetPosition(&pos);

                for (auto player : playerList)
                    player->SendApplyMovementForce(false, pos);

                break;
            }
            case 134370:// Down Draft
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, 100.0f);

                Position pos;
                GetPosition(&pos);

                for (auto player : playerList)
                    player->SendApplyMovementForce(false, pos);

                break;
            }
            default:
                break;
        }

        SendObjectDeSpawnAnim(GetGUID());
        RemoveFromWorld();
        AddObjectToRemoveList();
    }
}

void AreaTrigger::BindToCaster()
{
    //ASSERT(!m_caster);
    m_Caster = ObjectAccessor::GetUnit(*this, GetCasterGUID());
    //ASSERT(GetCaster());
    //ASSERT(GetCaster()->GetMap() == GetMap());
    if (m_Caster)
        m_Caster->_RegisterAreaTrigger(this);
}

void AreaTrigger::UnbindFromCaster()
{
    ASSERT(m_Caster);
    m_Caster->_UnregisterAreaTrigger(this);
    m_Caster = NULL;
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