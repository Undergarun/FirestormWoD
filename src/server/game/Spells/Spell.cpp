/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Opcodes.h"
#include "Log.h"
#include "UpdateMask.h"
#include "World.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "Pet.h"
#include "Unit.h"
#include "Totem.h"
#include "Spell.h"
#include "DynamicObject.h"
#include "Group.h"
#include "UpdateData.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "CellImpl.h"
#include "SharedDefines.h"
#include "LootMgr.h"
#include "VMapFactory.h"
#include "Battleground.h"
#include "Util.h"
#include "TemporarySummon.h"
#include "Vehicle.h"
#include "SpellAuraEffects.h"
#include "ScriptMgr.h"
#include "ConditionMgr.h"
#include "DisableMgr.h"
#include "SpellScript.h"
#include "InstanceScript.h"
#include "SpellInfo.h"
#include "DB2Stores.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "GuildMgr.h"
#include "GarrisonMgr.hpp"

extern pEffect SpellEffects[TOTAL_SPELL_EFFECTS];

SpellDestination::SpellDestination()
{
    _position.Relocate(0, 0, 0, 0);
    _transportGUID = 0;
    _transportOffset.Relocate(0, 0, 0, 0);
}

SpellDestination::SpellDestination(float x, float y, float z, float orientation, uint32 mapId)
{
    _position.Relocate(x, y, z, orientation);
    _transportGUID = 0;
    _position.m_mapId = mapId;
}

SpellDestination::SpellDestination(Position const& pos)
{
    _position.Relocate(pos);
    _transportGUID = 0;
}

SpellDestination::SpellDestination(WorldObject const& wObj)
{
    _transportGUID = wObj.GetTransGUID();
    _transportOffset.Relocate(wObj.GetTransOffsetX(), wObj.GetTransOffsetY(), wObj.GetTransOffsetZ(), wObj.GetTransOffsetO());
    _position.Relocate(wObj);
    _position.SetOrientation(wObj.GetOrientation());
}


SpellCastTargets::SpellCastTargets() : m_elevation(0), m_speed(0), m_strTarget()
{
    m_objectTarget = nullptr;
    m_itemTarget   = nullptr;

    m_objectTargetGUID   = 0;
    m_itemTargetGUID   = 0;
    m_itemTargetEntry  = 0;

    m_targetMask = 0;
}

SpellCastTargets::~SpellCastTargets()
{
}

void SpellCastTargets::Read(ByteBuffer& data, Unit* caster)
{
    data >> m_targetMask;

    if (m_targetMask == TARGET_FLAG_NONE)
        return;

    if (m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_UNIT_MINIPET | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_CORPSE_ENEMY | TARGET_FLAG_CORPSE_ALLY))
        data.readPackGUID(m_objectTargetGUID);

    if (m_targetMask & (TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM))
        data.readPackGUID(m_itemTargetGUID);

    if (m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
    {
        data.readPackGUID(m_src._transportGUID);
        if (m_src._transportGUID)
            data >> m_src._transportOffset.PositionXYZStream();
        else
            data >> m_src._position.PositionXYZStream();
    }
    else
    {
        m_src._transportGUID = caster->GetTransGUID();
        if (m_src._transportGUID)
            m_src._transportOffset.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            m_src._position.Relocate(caster);
    }

    if (m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        data.readPackGUID(m_dst._transportGUID);
        if (m_dst._transportGUID)
            data >> m_dst._transportOffset.PositionXYZStream();
        else
            data >> m_dst._position.PositionXYZStream();
    }
    else
    {
        m_dst._transportGUID = caster->GetTransGUID();
        if (m_dst._transportGUID)
            m_dst._transportOffset.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            m_dst._position.Relocate(caster);
    }

    if (m_targetMask & TARGET_FLAG_STRING)
        data >> m_strTarget;

    Update(caster);
}

void SpellCastTargets::Write(ByteBuffer& data)
{
    data << uint32(m_targetMask);

    if (m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_CORPSE_ENEMY | TARGET_FLAG_UNIT_MINIPET))
        data.appendPackGUID(m_objectTargetGUID);

    if (m_targetMask & (TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM))
    {
        if (m_itemTarget)
            data.appendPackGUID(m_itemTarget->GetGUID());
        else
            data << uint64(0);
    }

    if (m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
    {
        data.appendPackGUID(m_src._transportGUID); // relative position guid here - transport for example
        if (m_src._transportGUID)
            data << m_src._transportOffset.PositionXYZStream();
        else
            data << m_src._position.PositionXYZStream();
    }

    if (m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        data.appendPackGUID(m_dst._transportGUID); // relative position guid here - transport for example
        if (m_dst._transportGUID)
            data << m_dst._transportOffset.PositionXYZStream();
        else
            data << m_dst._position.PositionXYZStream();
    }

    if (m_targetMask & TARGET_FLAG_STRING)
        data << m_strTarget;
}

uint64 SpellCastTargets::GetUnitTargetGUID() const
{
    switch (GUID_HIPART(m_objectTargetGUID))
    {
        case HIGHGUID_PLAYER:
        case HIGHGUID_VEHICLE:
        case HIGHGUID_UNIT:
        case HIGHGUID_PET:
            return m_objectTargetGUID;
        default:
            return 0LL;
    }
}

Unit* SpellCastTargets::GetUnitTarget() const
{
    if (m_objectTarget)
        return m_objectTarget->ToUnit();
    return NULL;
}

void SpellCastTargets::SetUnitTarget(Unit* target)
{
    if (!target)
        return;

    m_objectTarget = target;
    m_objectTargetGUID = target->GetGUID();
    m_targetMask |= TARGET_FLAG_UNIT;
}

uint64 SpellCastTargets::GetGOTargetGUID() const
{
    switch (GUID_HIPART(m_objectTargetGUID))
    {
        case HIGHGUID_TRANSPORT:
        case HIGHGUID_MO_TRANSPORT:
        case HIGHGUID_GAMEOBJECT:
            return m_objectTargetGUID;
        default:
            return 0LL;
    }
}

GameObject* SpellCastTargets::GetGOTarget() const
{
    if (m_objectTarget)
        return m_objectTarget->ToGameObject();
    return NULL;
}


void SpellCastTargets::SetGOTarget(GameObject* target)
{
    if (!target)
        return;

    m_objectTarget = target;
    m_objectTargetGUID = target->GetGUID();
    m_targetMask |= TARGET_FLAG_GAMEOBJECT;
}

uint64 SpellCastTargets::GetCorpseTargetGUID() const
{
    switch (GUID_HIPART(m_objectTargetGUID))
    {
        case HIGHGUID_CORPSE:
            return m_objectTargetGUID;
        default:
            return 0LL;
    }
}

Corpse* SpellCastTargets::GetCorpseTarget() const
{
    if (m_objectTarget)
        return m_objectTarget->ToCorpse();
    return NULL;
}

WorldObject* SpellCastTargets::GetObjectTarget() const
{
    return m_objectTarget;
}

uint64 SpellCastTargets::GetObjectTargetGUID() const
{
    return m_objectTargetGUID;
}

void SpellCastTargets::RemoveObjectTarget()
{
    m_objectTarget = NULL;
    m_objectTargetGUID = 0LL;
    m_targetMask &= ~(TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK | TARGET_FLAG_GAMEOBJECT_MASK);
}

void SpellCastTargets::SetItemTarget(Item* item)
{
    if (!item)
        return;

    m_itemTarget = item;
    m_itemTargetGUID = item->GetGUID();
    m_itemTargetEntry = item->GetEntry();
    m_targetMask |= TARGET_FLAG_ITEM;
}

void SpellCastTargets::SetTradeItemTarget(Player* caster)
{
    m_itemTargetGUID = uint64(TRADE_SLOT_NONTRADED);
    m_itemTargetEntry = 0;
    m_targetMask |= TARGET_FLAG_TRADE_ITEM;

    Update(caster);
}

void SpellCastTargets::UpdateTradeSlotItem()
{
    if (m_itemTarget && (m_targetMask & TARGET_FLAG_TRADE_ITEM))
    {
        m_itemTargetGUID = m_itemTarget->GetGUID();
        m_itemTargetEntry = m_itemTarget->GetEntry();
    }
}

SpellDestination const* SpellCastTargets::GetSrc() const
{
    return &m_src;
}

Position const* SpellCastTargets::GetSrcPos() const
{
    return &m_src._position;
}

void SpellCastTargets::SetSrc(float x, float y, float z)
{
    m_src = SpellDestination(x, y, z);
    m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
}

void SpellCastTargets::SetSrc(Position const& pos)
{
    m_src = SpellDestination(pos);
    m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
}

void SpellCastTargets::SetSrc(WorldObject const& wObj)
{
    m_src = SpellDestination(wObj);
    m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
}

void SpellCastTargets::ModSrc(Position const& pos)
{
    ASSERT(m_targetMask & TARGET_FLAG_SOURCE_LOCATION);

    if (m_src._transportGUID)
    {
        Position offset;
        m_src._position.GetPositionOffsetTo(pos, offset);
        m_src._transportOffset.RelocateOffset(offset);
    }
    m_src._position.Relocate(pos);
}

void SpellCastTargets::RemoveSrc()
{
    m_targetMask &= ~(TARGET_FLAG_SOURCE_LOCATION);
}

SpellDestination const* SpellCastTargets::GetDst() const
{
    return &m_dst;
}

WorldLocation const* SpellCastTargets::GetDstPos() const
{
    return &m_dst._position;
}

void SpellCastTargets::SetDst(float x, float y, float z, float orientation, uint32 mapId)
{
    m_dst = SpellDestination(x, y, z, orientation, mapId);
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::SetDst(Position const& pos)
{
    m_dst = SpellDestination(pos);
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::SetDst(WorldObject const& wObj)
{
    m_dst = SpellDestination(wObj);
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::SetDst(SpellCastTargets const& spellTargets)
{
    m_dst = spellTargets.m_dst;
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::ModDst(Position const& pos)
{
    ASSERT(m_targetMask & TARGET_FLAG_DEST_LOCATION);

    if (m_dst._transportGUID)
    {
        Position offset;
        m_dst._position.GetPositionOffsetTo(pos, offset);
        m_dst._transportOffset.RelocateOffset(offset);
    }
    m_dst._position.Relocate(pos);
}

void SpellCastTargets::RemoveDst()
{
    m_targetMask &= ~(TARGET_FLAG_DEST_LOCATION);
}

void SpellCastTargets::Update(Unit* p_Caster)
{
    m_objectTarget = m_objectTargetGUID ? ((m_objectTargetGUID == p_Caster->GetGUID()) ? p_Caster : ObjectAccessor::GetWorldObject(*p_Caster, m_objectTargetGUID)) : NULL;

    m_itemTarget = nullptr;
    if (p_Caster->IsPlayer())
    {
        Player* l_Player = p_Caster->ToPlayer();
        if (m_targetMask & TARGET_FLAG_ITEM)
            m_itemTarget = l_Player->GetItemByGuid(m_itemTargetGUID);
        else if (m_targetMask & TARGET_FLAG_TRADE_ITEM)
        {
            if (m_itemTargetGUID == TRADE_SLOT_NONTRADED && l_Player->GetTradeData()) // Here it is not guid but slot. Also prevents hacking slots
            {
                if (Player* l_Trader = l_Player->GetTradeData()->GetTrader())
                {
                    if (TradeData* l_HisTrade = l_Trader->GetTradeData())
                        m_itemTarget = l_HisTrade->GetItem(TRADE_SLOT_TRADED_COUNT);
                }
            }
        }

        if (m_itemTarget)
            m_itemTargetEntry = m_itemTarget->GetEntry();
    }

    // Update positions by transport move
    if (HasSrc() && m_src._transportGUID)
    {
        if (WorldObject* transport = ObjectAccessor::GetWorldObject(*p_Caster, m_src._transportGUID))
        {
            m_src._position.Relocate(transport);
            m_src._position.RelocateOffset(m_src._transportOffset);
        }
    }

    if (HasDst() && m_dst._transportGUID)
    {
        if (WorldObject* transport = ObjectAccessor::GetWorldObject(*p_Caster, m_dst._transportGUID))
        {
            m_dst._position.Relocate(transport);
            m_dst._position.RelocateOffset(m_dst._transportOffset);
        }
    }
}

void SpellCastTargets::OutDebug() const
{
    if (!m_targetMask)
        sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "No targets");

    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "target mask: %u", m_targetMask);
    if (m_targetMask & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK | TARGET_FLAG_GAMEOBJECT_MASK))
        sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "Object target: " UI64FMTD, m_objectTargetGUID);
    if (m_targetMask & TARGET_FLAG_ITEM)
        sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "Item target: " UI64FMTD, m_itemTargetGUID);
    if (m_targetMask & TARGET_FLAG_TRADE_ITEM)
        sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "Trade item target: " UI64FMTD, m_itemTargetGUID);
    if (m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
        sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "Source location: transport guid:" UI64FMTD " trans offset: %s position: %s", m_src._transportGUID, m_src._transportOffset.ToString().c_str(), m_src._position.ToString().c_str());
    if (m_targetMask & TARGET_FLAG_DEST_LOCATION)
        sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "Destination location: transport guid:" UI64FMTD " trans offset: %s position: %s", m_dst._transportGUID, m_dst._transportOffset.ToString().c_str(), m_dst._position.ToString().c_str());
    if (m_targetMask & TARGET_FLAG_STRING)
        sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "String: %s", m_strTarget.c_str());
    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "speed: %f", m_speed);
    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "elevation: %f", m_elevation);
}

SpellValue::SpellValue(SpellInfo const* proto)
{
    for (uint32 i = 0; i < SpellEffIndex::MAX_EFFECTS; ++i)
        EffectBasePoints[i] = proto->Effects[i].BasePoints;
    MaxAffectedTargets = proto->MaxAffectedTargets;
    RadiusMod = 1.0f;
    AuraStackAmount = 1;
}

Spell::Spell(Unit* caster, SpellInfo const* info, TriggerCastFlags triggerFlags, uint64 originalCasterGUID, bool skipCheck) :
m_spellInfo(sSpellMgr->GetSpellForDifficultyFromSpell(info, caster)),
m_caster((info->AttributesEx6 & SPELL_ATTR6_CAST_BY_CHARMER && caster->GetCharmerOrOwner()) ? caster->GetCharmerOrOwner() : caster)
, m_spellValue(new SpellValue(m_spellInfo)), m_preGeneratedPath(PathGenerator(m_caster))
{
    m_customError = SPELL_CUSTOM_ERROR_NONE;
    m_skipCheck = skipCheck;
    m_selfContainer = NULL;
    m_referencedFromCurrentSpell = false;
    m_executedCurrently = false;
    m_needComboPoints = m_spellInfo->NeedsComboPoints();
    m_comboPointGain = 0;
    m_delayStart = 0;
    m_delayAtDamageCount = 0;

    m_applyMultiplierMask = 0;
    m_auraScaleMask = 0;

    memset(m_Misc, 0, sizeof(m_Misc));

    // Get data for type of attack
    switch (m_spellInfo->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_MELEE:
            if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_REQ_OFFHAND)
                m_attackType = WeaponAttackType::OffAttack;
            else
                m_attackType = WeaponAttackType::BaseAttack;
            break;
        case SPELL_DAMAGE_CLASS_RANGED:
            m_attackType = m_spellInfo->IsRangedWeaponSpell() ? WeaponAttackType::RangedAttack : WeaponAttackType::BaseAttack;
            break;
        default:
                                                            // Wands
            if (m_spellInfo->AttributesEx2 & SPELL_ATTR2_AUTOREPEAT_FLAG)
                m_attackType = WeaponAttackType::RangedAttack;
            else
                m_attackType = WeaponAttackType::BaseAttack;
            break;
    }

    m_spellSchoolMask = info->GetSchoolMask();           // Can be override for some spell (wand shoot for example)

    if (m_attackType == WeaponAttackType::RangedAttack)
        // wand case
        if ((m_caster->getClassMask() & CLASSMASK_WAND_USERS) != 0 && m_caster->IsPlayer())
            if (Item* pItem = m_caster->ToPlayer()->GetWeaponForAttack(WeaponAttackType::RangedAttack))
                m_spellSchoolMask = SpellSchoolMask(1 << pItem->GetTemplate()->DamageType);

    if (originalCasterGUID)
        m_originalCasterGUID = originalCasterGUID;
    else
        m_originalCasterGUID = m_caster->GetGUID();

    if (m_originalCasterGUID == m_caster->GetGUID())
        m_originalCaster = m_caster;
    else
    {
        m_originalCaster = ObjectAccessor::GetUnit(*m_caster, m_originalCasterGUID);
        if (m_originalCaster && !m_originalCaster->IsInWorld())
            m_originalCaster = NULL;
    }

    m_spellState = SPELL_STATE_NULL;
    _triggeredCastFlags = triggerFlags;
    if (info->AttributesEx4 & SPELL_ATTR4_CAN_CAST_WHILE_CASTING)
        _triggeredCastFlags = TriggerCastFlags(uint32(_triggeredCastFlags) | TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_CAST_DIRECTLY);

    m_CastItem = NULL;
    m_castItemGUID = 0;

    /// Target used in SpellEffect
    unitTarget    = nullptr;
    itemTarget    = nullptr;
    gameObjTarget = nullptr;
    focusObject   = nullptr;
    m_AreaTrigger = nullptr;

    m_cast_count = 0;
    m_CastItemEntry = 0;
    m_preCastSpell = 0;
    m_triggeredByAuraSpell  = NULL;
    m_spellAura = nullptr;
    isStolen = false;

    m_CustomCritChance = -1.f;

    //Auto Shot & Shoot (wand)
    m_autoRepeat = m_spellInfo->IsAutoRepeatRangedSpell();

    m_runesState = 0;
    m_casttime = 0;                                         // setup to correct value in Spell::prepare, must not be used before.
    m_timer = 0;                                            // will set to castime in prepare
    m_channeledDuration = 0;                                // will be setup in Spell::handle_immediate
    m_periodicDamageModifier = 0.0f;

    m_channelTargetEffectMask = 0;

    m_redirected = false;

    // Determine if spell can be reflected back to the caster
    // Patch 1.2 notes: Spell Reflection no longer reflects abilities
    m_canReflect = m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_MAGIC && !(m_spellInfo->Attributes & SPELL_ATTR0_ABILITY)
        && !(m_spellInfo->AttributesEx & SPELL_ATTR1_CANT_BE_REFLECTED) && !(m_spellInfo->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        && !m_spellInfo->IsPassive() && !m_spellInfo->IsHealingSpell() && !m_spellInfo->IsPositive();

    CleanupTargetList();
    m_effectExecuteData.clear();

    if (m_caster == NULL)
        sLog->outAshran("m_caster is null!! spellId %u", m_spellInfo->Id);

    for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
        m_destTargets[i] = SpellDestination(*m_caster);
}

Spell::~Spell()
{
    // unload scripts
    while (!m_loadedScripts.empty())
    {
        std::list<SpellScript*>::iterator itr = m_loadedScripts.begin();
        (*itr)->_Unload();
        delete (*itr);
        m_loadedScripts.erase(itr);
    }

    if (m_referencedFromCurrentSpell && m_selfContainer && *m_selfContainer == this)
    {
        // Clean the reference to avoid later crash.
        // If this error is repeating, we may have to add an ASSERT to better track down how we get into this case.
        *m_selfContainer = NULL;
    }

    if (m_caster && m_caster->IsPlayer())
        ASSERT(m_caster->ToPlayer()->m_spellModTakingSpell != this);
    delete m_spellValue;

    CheckEffectExecuteData();
}

void Spell::InitExplicitTargets(SpellCastTargets const& targets)
{
    m_targets = targets;
    // this function tries to correct spell explicit targets for spell
    // client doesn't send explicit targets correctly sometimes - we need to fix such spells serverside
    // this also makes sure that we correctly send explicit targets to client (removes redundant data)
    uint32 neededTargets = m_spellInfo->GetExplicitTargetMask();

    if (WorldObject* target = m_targets.GetObjectTarget())
    {
        // check if object target is valid with needed target flags
        // for unit case allow corpse target mask because player with not released corpse is a unit target
        if ((target->ToUnit() && !(neededTargets & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK)))
            || (target->ToGameObject() && !(neededTargets & TARGET_FLAG_GAMEOBJECT_MASK))
            || (target->ToCorpse() && !(neededTargets & TARGET_FLAG_CORPSE_MASK)))
            if (!m_spellInfo->HasPersistenAura() || (m_spellInfo->HasPersistenAura() && !(m_targets.GetTargetMask() & TARGET_FLAG_UNIT_MASK)))
                m_targets.RemoveObjectTarget();
    }
    else
    {
        // try to select correct unit target if not provided by client or by serverside cast
        if (neededTargets & (TARGET_FLAG_UNIT_MASK))
        {
            Unit* unit = NULL;
            // try to use player selection as a target
            if (Player* playerCaster = m_caster->ToPlayer())
            {
                // selection has to be found and to be valid target for the spell
                if (Unit* selectedUnit = ObjectAccessor::GetUnit(*m_caster, playerCaster->GetSelection()))
                    if (m_spellInfo->CheckExplicitTarget(m_caster, selectedUnit) == SPELL_CAST_OK)
                        unit = selectedUnit;
            }
            // try to use attacked unit as a target
            else if ((m_caster->GetTypeId() == TYPEID_UNIT) && neededTargets & (TARGET_FLAG_UNIT_ENEMY | TARGET_FLAG_UNIT))
                unit = m_caster->getVictim();

            // didn't find anything - let's use self as target
            if (!unit && neededTargets & (TARGET_FLAG_UNIT_RAID | TARGET_FLAG_UNIT_PARTY | TARGET_FLAG_UNIT_ALLY))
                unit = m_caster;

            m_targets.SetUnitTarget(unit);
        }
    }

    // check if spell needs dst target
    if (neededTargets & TARGET_FLAG_DEST_LOCATION)
    {
        // and target isn't set
        if (!m_targets.HasDst())
        {
            // try to use unit target if provided
            if (WorldObject* target = targets.GetObjectTarget())
                m_targets.SetDst(*target);
            // or use self if not available
            else
                m_targets.SetDst(*m_caster);
        }
    }
    else
        m_targets.RemoveDst();

    if (neededTargets & TARGET_FLAG_SOURCE_LOCATION)
    {
        if (!targets.HasSrc())
            m_targets.SetSrc(*m_caster);
    }
    else
        m_targets.RemoveSrc();

    // Hack fix for some specific spells
    switch (m_spellInfo->Id)
    {
        case 107145: // Wall of Light Activation
        {
            m_targets.SetSrc(*m_caster);

            WorldLocation firstPos;
            WorldLocation secondPos;

            firstPos.m_positionX = -1045.602f;
            firstPos.m_positionY = -2822.323f;
            firstPos.m_positionZ = 38.25466f;
            secondPos.m_positionX = -989.4236f;
            secondPos.m_positionY = -2821.757f;
            secondPos.m_positionZ = 38.25455f;

            m_targets.AddExtraTarget(0, firstPos);
            m_targets.AddExtraTarget(0, secondPos);
            break;
        }
        default:
            break;
    }
}

void Spell::SelectExplicitTargets()
{
    // here go all explicit target changes made to explicit targets after spell prepare phase is finished
    if (Unit* target = m_targets.GetUnitTarget())
    {
        // check for explicit target redirection, for Grounding Totem for example
        if (m_spellInfo->GetExplicitTargetMask() & TARGET_FLAG_UNIT_ENEMY
            || (m_spellInfo->GetExplicitTargetMask() & TARGET_FLAG_UNIT && !m_spellInfo->IsPositive()))
        {
            Unit* redirect;
            switch (m_spellInfo->DmgClass)
            {
                case SPELL_DAMAGE_CLASS_MAGIC:
                    redirect = m_caster->GetMagicHitRedirectTarget(target, m_spellInfo);
                    break;
                case SPELL_DAMAGE_CLASS_MELEE:
                case SPELL_DAMAGE_CLASS_RANGED:
                    redirect = m_caster->GetMeleeHitRedirectTarget(target, m_spellInfo);
                    break;
                default:
                    redirect = NULL;
                    break;
            }
            if (redirect && (redirect != target))
            {
                m_targets.SetUnitTarget(redirect);
                m_redirected = true;
            }
        }
    }
}

void Spell::SelectSpellTargets()
{
    // select targets for cast phase
    SelectExplicitTargets();

    uint32 processedAreaEffectsMask = 0;
    for (uint32 i = 0; i < m_spellInfo->EffectCount; ++i)
    {
        // not call for empty effect.
        // Also some spells use not used effect targets for store targets for dummy effect in triggered spells
        if (!m_spellInfo->Effects[i].IsEffect())
            continue;

        // set expected type of implicit targets to be sent to client
        uint32 implicitTargetMask = GetTargetFlagMask(m_spellInfo->Effects[i].TargetA.GetObjectType()) | GetTargetFlagMask(m_spellInfo->Effects[i].TargetB.GetObjectType());
        if (implicitTargetMask & TARGET_FLAG_UNIT)
            m_targets.SetTargetFlag(TARGET_FLAG_UNIT);
        if (implicitTargetMask & (TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_GAMEOBJECT_ITEM))
            m_targets.SetTargetFlag(TARGET_FLAG_GAMEOBJECT);

        SelectEffectImplicitTargets(SpellEffIndex(i), m_spellInfo->Effects[i].TargetA, processedAreaEffectsMask);
        SelectEffectImplicitTargets(SpellEffIndex(i), m_spellInfo->Effects[i].TargetB, processedAreaEffectsMask);

        // Select targets of effect based on effect type
        // those are used when no valid target could be added for spell effect based on spell target type
        // some spell effects use explicit target as a default target added to target map (like SPELL_EFFECT_LEARN_SPELL)
        // some spell effects add target to target map only when target type specified (like SPELL_EFFECT_WEAPON)
        // some spell effects don't add anything to target map (confirmed with sniffs) (like SPELL_EFFECT_DESTROY_ALL_TOTEMS)
        SelectEffectTypeImplicitTargets(i);

        if (m_targets.HasDst() && !IsCCSpell(m_spellInfo))
            AddDestTarget(*m_targets.GetDst(), i);

        if (m_spellInfo->IsChanneled())
        {
            uint32 mask = (1 << i);
            for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            {
                if (ihit->effectMask & mask)
                {
                    m_channelTargetEffectMask |= mask;
                    break;
                }
            }
        }
        else if (m_auraScaleMask)
        {
            bool checkLvl = !m_UniqueTargetInfo.empty();
            for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end();)
            {
                // remove targets which did not pass min level check
                if (m_auraScaleMask && ihit->effectMask == m_auraScaleMask)
                {
                    // Do not check for selfcast
                    if (!ihit->scaleAura && ihit->targetGUID != m_caster->GetGUID())
                    {
                        ihit = m_UniqueTargetInfo.erase(ihit);
                        continue;
                    }
                }
                ++ihit;
            }
            if (checkLvl && m_UniqueTargetInfo.empty())
            {
                SendCastResult(SPELL_FAILED_LOWLEVEL);
                finish(false);
            }
        }
    }

    if (m_targets.HasDst())
    {
        if (m_targets.HasTraj())
        {
            float speed = m_targets.GetSpeedXY();
            if (speed > 0.0f)
                m_delayMoment = (uint64)floor(m_targets.GetDist2d() / speed * 1000.0f);
        }
        else if (m_spellInfo->Speed > 0.0f)
        {
            float dist = m_caster->GetDistance(*m_targets.GetDstPos());

            if (!(m_spellInfo->AttributesEx9 & SPELL_ATTR9_SPECIAL_DELAY_CALCULATION))
                m_delayMoment = uint64(floor(dist / m_spellInfo->Speed * 1000.0f));
            else
                m_delayMoment = uint64(m_spellInfo->Speed * 1000.0f);
        }
    }
}

void Spell::SelectEffectImplicitTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32& processedEffectMask)
{
    if (!targetType.GetTarget())
        return;

    uint32 effectMask = 1 << effIndex;
    // set the same target list for all effects
    // some spells appear to need this, however this requires more research
    switch (targetType.GetSelectionCategory())
    {
        case TARGET_SELECT_CATEGORY_NEARBY:
        case TARGET_SELECT_CATEGORY_CONE:
        case TARGET_SELECT_CATEGORY_AREA:
            // targets for effect already selected
            if (effectMask & processedEffectMask)
                return;
            // choose which targets we can select at once
            for (uint32 j = effIndex + 1; j < m_spellInfo->EffectCount; ++j)
            {
                if (m_spellInfo->Effects[effIndex].TargetA.GetTarget() == m_spellInfo->Effects[j].TargetA.GetTarget() &&
                    m_spellInfo->Effects[effIndex].TargetB.GetTarget() == m_spellInfo->Effects[j].TargetB.GetTarget() &&
                    m_spellInfo->Effects[effIndex].ImplicitTargetConditions == m_spellInfo->Effects[j].ImplicitTargetConditions &&
                    m_spellInfo->Effects[effIndex].CalcRadius(m_caster) == m_spellInfo->Effects[j].CalcRadius(m_caster) &&
                    m_spellInfo->Id != 119072)
                    effectMask |= 1 << j;
            }
            processedEffectMask |= effectMask;
            break;
        default:
            break;
    }

    switch (targetType.GetSelectionCategory())
    {
        case TARGET_SELECT_CATEGORY_CHANNEL:
            SelectImplicitChannelTargets(effIndex, targetType);
            break;
        case TARGET_SELECT_CATEGORY_NEARBY:
            SelectImplicitNearbyTargets(effIndex, targetType, effectMask);
            break;
        case TARGET_SELECT_CATEGORY_CONE:
            SelectImplicitConeTargets(effIndex, targetType, effectMask);
            break;
        case TARGET_SELECT_CATEGORY_AREA:
            SelectImplicitAreaTargets(effIndex, targetType, effectMask);
            break;
        case TARGET_SELECT_CATEGORY_DEFAULT:
            switch (targetType.GetObjectType())
            {
                case TARGET_OBJECT_TYPE_SRC:
                    switch (targetType.GetReferenceType())
                    {
                        case TARGET_REFERENCE_TYPE_CASTER:
                            m_targets.SetSrc(*m_caster);
                            break;
                        default:
                            ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target reference type for TARGET_TYPE_OBJECT_SRC");
                            break;
                    }
                    break;
                case TARGET_OBJECT_TYPE_DEST:
                     switch (targetType.GetReferenceType())
                     {
                         case TARGET_REFERENCE_TYPE_CASTER:
                             SelectImplicitCasterDestTargets(effIndex, targetType);
                             break;
                         case TARGET_REFERENCE_TYPE_TARGET:
                             SelectImplicitTargetDestTargets(effIndex, targetType);
                             break;
                         case TARGET_REFERENCE_TYPE_DEST:
                             SelectImplicitDestDestTargets(effIndex, targetType);
                             break;
                         default:
                             ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target reference type for TARGET_TYPE_OBJECT_DEST");
                             break;
                     }
                     break;
                default:
                    switch (targetType.GetReferenceType())
                    {
                        case TARGET_REFERENCE_TYPE_CASTER:
                            SelectImplicitCasterObjectTargets(effIndex, targetType);
                            break;
                        case TARGET_REFERENCE_TYPE_TARGET:
                            SelectImplicitTargetObjectTargets(effIndex, targetType);
                            break;
                        default:
                            ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target reference type for TARGET_TYPE_OBJECT");
                            break;
                    }
                    break;
            }
            break;
        case TARGET_SELECT_CATEGORY_NYI:
            break;
        default:
            printf("Spell::SelectEffectImplicitTargets: received not implemented select target category / Spell ID = %u and Effect = %d and target type = %d \n", m_spellInfo->Id, effIndex, targetType.GetTarget());
            //ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target category");
            break;
    }
}

void Spell::SelectImplicitChannelTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    if (targetType.GetReferenceType() != TARGET_REFERENCE_TYPE_CASTER)
    {
        ASSERT(false && "Spell::SelectImplicitChannelTargets: received not implemented target reference type");
        return;
    }

    Spell* channeledSpell = m_originalCaster->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
    if (!channeledSpell)
        return;

    switch (targetType.GetTarget())
    {
        case TARGET_UNIT_CHANNEL_TARGET:
        {
            WorldObject* target = ObjectAccessor::GetUnit(*m_caster, m_originalCaster->GetGuidValue(UNIT_FIELD_CHANNEL_OBJECT));
            CallScriptObjectTargetSelectHandlers(target, effIndex);
            // unit target may be no longer available - teleported out of map for example
            if (target && target->ToUnit())
                AddUnitTarget(target->ToUnit(), 1 << effIndex);
            break;
        }
        case TARGET_DEST_CHANNEL_TARGET:
            if (channeledSpell->m_targets.HasDst())
                m_targets.SetDst(channeledSpell->m_targets);
            else if (WorldObject* target = ObjectAccessor::GetWorldObject(*m_caster, m_originalCaster->GetGuidValue(UNIT_FIELD_CHANNEL_OBJECT)))
            {
                CallScriptObjectTargetSelectHandlers(target, effIndex);
                if (target)
                    m_targets.SetDst(*target);
            }
            break;
        case TARGET_DEST_CHANNEL_CASTER:
            m_targets.SetDst(*channeledSpell->GetCaster());
            break;
        default:
            ASSERT(false && "Spell::SelectImplicitChannelTargets: received not implemented target type");
            break;
    }
}

void Spell::SelectImplicitNearbyTargets(SpellEffIndex p_EffIndex, SpellImplicitTargetInfo const& p_TargetType, uint32 p_EffMask)
{
    if (p_TargetType.GetReferenceType() != TARGET_REFERENCE_TYPE_CASTER)
    {
        ASSERT(false && "Spell::SelectImplicitNearbyTargets: received not implemented target reference type");
        return;
    }

    float l_Range = 0.0f;
    switch (p_TargetType.GetCheckType())
    {
        case TARGET_CHECK_ENEMY:
            l_Range = m_spellInfo->GetMaxRange(false, m_caster, this);
            break;
        case TARGET_CHECK_ALLY:
        case TARGET_CHECK_PARTY:
        case TARGET_CHECK_RAID:
        case TARGET_CHECK_RAID_CLASS:
        case TARGET_CHECK_ALLY_OR_RAID:
            l_Range = m_spellInfo->GetMaxRange(true, m_caster, this);
            break;
        case TARGET_CHECK_ENTRY:
        case TARGET_CHECK_DEFAULT:
            l_Range = m_spellInfo->GetMaxRange(m_spellInfo->IsPositive(), m_caster, this);
            break;
        default:
            ASSERT(false && "Spell::SelectImplicitNearbyTargets: received not implemented selection check type");
            break;
    }

    ConditionContainer* l_CondList = m_spellInfo->Effects[p_EffIndex].ImplicitTargetConditions;

    // handle emergency case - try to use other provided targets if no conditions provided
    if (p_TargetType.GetCheckType() == TARGET_CHECK_ENTRY && (!l_CondList || l_CondList->empty()))
    {
        switch (p_TargetType.GetObjectType())
        {
            case TARGET_OBJECT_TYPE_GOBJ:
                if (m_spellInfo->RequiresSpellFocus)
                {
                    if (focusObject)
                        AddGOTarget(focusObject, p_EffMask);
                    return;
                }
                break;
            case TARGET_OBJECT_TYPE_DEST:
                if (m_spellInfo->RequiresSpellFocus)
                {
                    if (focusObject)
                        m_targets.SetDst(*focusObject);
                    return;
                }
                break;
            default:
                break;
        }
    }

    WorldObject* l_Target = SearchNearbyTarget(l_Range, p_TargetType.GetObjectType(), p_TargetType.GetCheckType(), l_CondList);
    if (!l_Target)
        return;

    CallScriptObjectTargetSelectHandlers(l_Target, p_EffIndex);

    switch (p_TargetType.GetObjectType())
    {
        case TARGET_OBJECT_TYPE_AREATRIGGER:
            if (AreaTrigger* l_AreaTrigger = l_Target->ToAreaTrigger())
                AddAreaTriggerTarget(l_AreaTrigger, p_EffMask);
        case TARGET_OBJECT_TYPE_UNIT:
            if (Unit* l_UnitTarget = l_Target->ToUnit())
                AddUnitTarget(l_UnitTarget, p_EffMask, true, false);
            break;
        case TARGET_OBJECT_TYPE_GOBJ:
            if (GameObject* l_GobjTarget = l_Target->ToGameObject())
                AddGOTarget(l_GobjTarget, p_EffMask);
            break;
        case TARGET_OBJECT_TYPE_DEST:
            m_targets.SetDst(*l_Target);
            break;
        default:
            ASSERT(false && "Spell::SelectImplicitNearbyTargets: received not implemented target object type");
            break;
    }

    SelectImplicitChainTargets(p_EffIndex, p_TargetType, l_Target, p_EffMask);
}

void Spell::SelectImplicitConeTargets(SpellEffIndex p_EffIndex, SpellImplicitTargetInfo const& p_TargetType, uint32 p_EffMask)
{
    if (p_TargetType.GetReferenceType() != TARGET_REFERENCE_TYPE_CASTER)
    {
        ASSERT(false && "Spell::SelectImplicitConeTargets: received not implemented target reference type");
        return;
    }
    std::list<WorldObject*> l_Targets;
    SpellTargetObjectTypes l_ObjectType = p_TargetType.GetObjectType();
    SpellTargetCheckTypes l_SelectionType = p_TargetType.GetCheckType();
    ConditionContainer* l_ConditionsList = m_spellInfo->Effects[p_EffIndex].ImplicitTargetConditions;
    float l_ConeAngle = ((m_caster->IsPlayer()) ? M_PI * (2.0f / 3.0f) : M_PI / 2.0f);

    switch (m_spellInfo->Effects[p_EffIndex].TargetA.GetTarget())
    {
        case TARGET_UNIT_CONE_ENEMY_24:
            l_ConeAngle = M_PI / 7.5f;
            break;
        case TARGET_UNIT_CONE_ENEMY_54:
            l_ConeAngle = M_PI / 3.33f;
            break;
        case TARGET_UNIT_CONE_ENEMY_104:
            l_ConeAngle = M_PI / 1.73f;
            break;
        case TARGET_UNIT_CONE_ENEMY_110:
            l_ConeAngle = M_PI / 1.64f;
            break;
        case TARGET_UNIT_CONE_ENEMY_129:
            l_ConeAngle = M_PI / 1.40f;
            break;
        case TARGET_UNIT_CONE_ENEMY_130:
            l_ConeAngle = M_PI / 1.385f;
            break;
        default:
            break;
    }

    switch (m_spellInfo->Id)
    {
        case 118094:
            l_ConeAngle = M_PI/2;
            break;
        case 118105:
            l_ConeAngle = M_PI/4;
            break;
        case 118106:
            l_ConeAngle = M_PI/6;
            break;
        case 136740:// Double swipe (back)
            l_ConeAngle += M_PI;
            break;
        default:
            break;
    }

    /// Handle ConeAngle calculation in a generic way, overriding hardcoded values if needed
    if (SpellTargetRestrictionsEntry const* l_Restrictions = m_spellInfo->GetSpellTargetRestrictions())
    {
        if (l_Restrictions->ConeAngle != 0.0f)
            l_ConeAngle = 2.0f * M_PI / 360.0f * l_Restrictions->ConeAngle;
    }

    float l_Radius = m_spellInfo->Effects[p_EffIndex].CalcRadius(m_caster) * m_spellValue->RadiusMod;

    if (uint32 l_ContainerTypeMask = GetSearcherTypeMask(l_ObjectType, l_ConditionsList))
    {
        JadeCore::WorldObjectSpellConeTargetCheck l_Check(l_ConeAngle, l_Radius, m_caster, m_spellInfo, l_SelectionType, l_ConditionsList);
        JadeCore::WorldObjectListSearcher<JadeCore::WorldObjectSpellConeTargetCheck> l_Searcher(m_caster, l_Targets, l_Check, l_ContainerTypeMask);
        SearchTargets<JadeCore::WorldObjectListSearcher<JadeCore::WorldObjectSpellConeTargetCheck> >(l_Searcher, l_ContainerTypeMask, m_caster, m_caster, l_Radius);

        CallScriptObjectAreaTargetSelectHandlers(l_Targets, p_EffIndex);

        if (!l_Targets.empty())
        {
            // Other special target selection goes here
            if (uint32 l_MaxTargets = m_spellValue->MaxAffectedTargets)
                JadeCore::Containers::RandomResizeList(l_Targets, l_MaxTargets);

            // for compability with older code - add only unit and go targets
            // TODO: remove this
            std::list<Unit*>        l_UnitTargets;
            std::list<GameObject*>  l_GObjTargets;
            std::list<AreaTrigger*> l_AreaTriggerTargets;

            for (std::list<WorldObject*>::iterator l_Iterator = l_Targets.begin(); l_Iterator != l_Targets.end(); ++l_Iterator)
            {
                if (Unit* l_UnitTarget = (*l_Iterator)->ToUnit())
                    l_UnitTargets.push_back(l_UnitTarget);
                else if (GameObject* l_GObjTarget = (*l_Iterator)->ToGameObject())
                    l_GObjTargets.push_back(l_GObjTarget);
                else if (AreaTrigger* l_AreaTrigger = (*l_Iterator)->ToAreaTrigger())
                    l_AreaTriggerTargets.push_back(l_AreaTrigger);
            }

            // Light of Dawn
            if (m_spellInfo->Id == 85222)
            {
                uint8 l_MaxSize = m_caster->HasAura(54940) ? 4 : 6; // Glyph of Light of Dawn
                l_UnitTargets.push_back(m_caster);
                if (l_UnitTargets.size() > l_MaxSize)
                {
                    l_UnitTargets.sort(JadeCore::HealthPctOrderPred());
                    l_UnitTargets.resize(l_MaxSize);
                }
            }

            for (std::list<Unit*>::iterator l_Iterator = l_UnitTargets.begin(); l_Iterator != l_UnitTargets.end(); ++l_Iterator)
                AddUnitTarget(*l_Iterator, p_EffMask, false);

            for (std::list<GameObject*>::iterator l_Iterator = l_GObjTargets.begin(); l_Iterator != l_GObjTargets.end(); ++l_Iterator)
                AddGOTarget(*l_Iterator, p_EffMask);

            for (std::list<AreaTrigger*>::iterator l_Iterator = l_AreaTriggerTargets.begin(); l_Iterator != l_AreaTriggerTargets.end(); ++l_Iterator)
                AddAreaTriggerTarget(*l_Iterator, p_EffMask);
        }
    }
}

void Spell::SelectImplicitAreaTargets(SpellEffIndex p_EffIndex, SpellImplicitTargetInfo const& p_TargetType, uint32 p_EffMask)
{
    Unit* l_Referer = nullptr;
    switch (p_TargetType.GetReferenceType())
    {
        case TARGET_REFERENCE_TYPE_SRC:
        case TARGET_REFERENCE_TYPE_DEST:
        case TARGET_REFERENCE_TYPE_CASTER:
            l_Referer = m_caster;
            break;
        case TARGET_REFERENCE_TYPE_TARGET:
            l_Referer = m_targets.GetUnitTarget();
            break;
        case TARGET_REFERENCE_TYPE_LAST:
        {
            // find last added target for this effect
            for (std::list<TargetInfo>::reverse_iterator l_Iterator = m_UniqueTargetInfo.rbegin(); l_Iterator != m_UniqueTargetInfo.rend(); ++l_Iterator)
            {
                if (l_Iterator->effectMask & (1<<p_EffIndex))
                {
                    l_Referer = ObjectAccessor::GetUnit(*m_caster, l_Iterator->targetGUID);
                    break;
                }
            }
            break;
        }
        default:
            ASSERT(false && "Spell::SelectImplicitAreaTargets: received not implemented target reference type");
            return;
    }
    if (!l_Referer)
        return;

    Position const* l_Center = nullptr;
    switch (p_TargetType.GetReferenceType())
    {
        case TARGET_REFERENCE_TYPE_SRC:
            l_Center = m_targets.GetSrcPos();
            break;
        case TARGET_REFERENCE_TYPE_DEST:
            l_Center = m_targets.GetDstPos();
            break;
        case TARGET_REFERENCE_TYPE_CASTER:
        case TARGET_REFERENCE_TYPE_TARGET:
        case TARGET_REFERENCE_TYPE_LAST:
            l_Center = l_Referer;
            break;
         default:
             ASSERT(false && "Spell::SelectImplicitAreaTargets: received not implemented target reference type");
             return;
    }

    std::list<WorldObject*> l_Targets;
    float l_Radius = m_spellInfo->Effects[p_EffIndex].CalcRadius(m_caster) * m_spellValue->RadiusMod;
    SearchAreaTargets(l_Targets, l_Radius, l_Center, l_Referer, p_TargetType.GetObjectType(), p_TargetType.GetCheckType(), m_spellInfo->Effects[p_EffIndex].ImplicitTargetConditions);

    // Custom entries
    // TODO: remove those
    switch (m_spellInfo->Id)
    {
        case 117002:// Tap to the Spirit World
        {
            // remove existing targets
            CleanupTargetList();

            if (!l_Targets.empty())
                for (std::list<WorldObject*>::iterator itr = l_Targets.begin(); itr != l_Targets.end(); ++itr)
                    if ((*itr) && (*itr)->ToUnit())
                        if ((*itr)->GetEntry() == 60512)
                            AddUnitTarget((*itr)->ToUnit(), 1 << p_EffIndex, false);

            return;
        }
        case 117230: // Empowered
        {
            // remove existing targets
            CleanupTargetList();

            if (!l_Targets.empty())
                for (std::list<WorldObject*>::iterator itr = l_Targets.begin(); itr != l_Targets.end(); ++itr)
                    if ((*itr) && (*itr)->ToUnit())
                        if ((*itr)->GetEntry() == 60583 || (*itr)->GetEntry() == 60585 || (*itr)->GetEntry() == 60586)
                            AddUnitTarget((*itr)->ToUnit(), 1 << p_EffIndex, false);

            return;
        }
        case 120764: // Ghost Essence
        {
            // remove existing targets
            CleanupTargetList();

            if (!l_Targets.empty())
                for (std::list<WorldObject*>::iterator itr = l_Targets.begin(); itr != l_Targets.end(); ++itr)
                    if ((*itr) && (*itr)->ToUnit())
                        if ((*itr)->GetEntry() == 61334 || (*itr)->GetEntry() == 61989)
                            AddUnitTarget((*itr)->ToUnit(), 1 << p_EffIndex, false);

            return;
        }
        default:
            break;
    }

    CallScriptObjectAreaTargetSelectHandlers(l_Targets, p_EffIndex);

    std::list<Unit*> l_UnitTargets;
    std::list<GameObject*> l_GObjTargets;
    std::list<AreaTrigger*> l_AreaTriggerTargets;
    // for compability with older code - add only unit and go targets
    // TODO: remove this
    if (!l_Targets.empty())
    {
        for (std::list<WorldObject*>::iterator l_Iterator = l_Targets.begin(); l_Iterator != l_Targets.end(); ++l_Iterator)
        {
            if ((*l_Iterator))
            {
                if (Unit* l_UnitTarget = (*l_Iterator)->ToUnit())
                    l_UnitTargets.push_back(l_UnitTarget);
                else if (GameObject* l_GObjTarget = (*l_Iterator)->ToGameObject())
                    l_GObjTargets.push_back(l_GObjTarget);
                else if (AreaTrigger* l_AreaTrigger = (*l_Iterator)->ToAreaTrigger())
                    l_AreaTriggerTargets.push_back(l_AreaTrigger);
            }
        }
    }

    if (!l_UnitTargets.empty())
    {
        // Special target selection for smart heals and energizes
        uint32 l_MaxSize = 0;
        int32 l_Power = -1;
        switch (m_spellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
                switch (m_spellInfo->Id)
                {
                    case 71610: // Echoes of Light (Althor's Abacus normal version)
                    case 71641: // Echoes of Light (Althor's Abacus heroic version)
                    case 99152: // Cauterizing Flame, Item - Priest Healer T12 4P Bonus
                    case 96966: // Blaze of Life, Eye of Blazing Power (Normal)
                    case 97136: // Blaze of Life, Eye of Blazing Power (Heroic)
                    case 109825: // Nick of Time, Windward Heart (heroic)
                    case 108000: // Nick of Time, Windward Heart (normal)
                    case 109822: // Nick of Time, Windward Heart (lfr)
                        l_MaxSize = 1;
                        l_Power = POWER_HEALTH;
                        break;
                    case 57669: // Replenishment
                        // In arenas Replenishment may only affect the caster
                        if (m_caster->IsPlayer() && m_caster->ToPlayer()->InArena())
                        {
                            l_UnitTargets.clear();
                            l_UnitTargets.push_back(m_caster);
                            break;
                        }
                        l_MaxSize = 10;
                        l_Power = POWER_MANA;
                        break;
                    default:
                        break;
                }
                break;
            case SPELLFAMILY_PRIEST:
                if (m_spellInfo->Id == 34861) // Circle of Healing
                {
                    l_MaxSize = m_caster->HasAura(55675) ? 6 : 5; // Glyph of Circle of Healing
                    l_Power = POWER_HEALTH;
                }
                else if (m_spellInfo->Id == 64844) // Divine Hymn
                {
                    l_MaxSize = 5;
                    l_Power = POWER_HEALTH;
                }
                else
                    break;

                // Remove targets outside caster's raid
                for (std::list<Unit*>::iterator l_Iterator = l_UnitTargets.begin(); l_Iterator != l_UnitTargets.end();)
                {
                    if (!(*l_Iterator)->IsInRaidWith(m_caster))
                        l_Iterator = l_UnitTargets.erase(l_Iterator);
                    else
                        ++l_Iterator;
                }
                break;
            case SPELLFAMILY_PALADIN:
            {
                switch (m_spellInfo->Id)
                {
                    case 119072:// Holy Wrath
                    {
                        static const uint8 s_TypesNoglyph[] = {CREATURE_TYPE_DEMON, CREATURE_TYPE_UNDEAD, 0};
                        static const uint8 sTypeGlyph[] = {CREATURE_TYPE_DEMON, CREATURE_TYPE_UNDEAD, CREATURE_TYPE_ELEMENTAL, CREATURE_TYPE_DRAGONKIN, 0};
                        const uint8 *l_Types = m_caster->HasAura(54923) ? sTypeGlyph: s_TypesNoglyph;

                        // Normal case
                        if (p_EffIndex == 0 && !m_caster->HasAura(115738))
                        {
                            for (std::list<Unit*>::iterator l_Iterator = l_UnitTargets.begin() ; l_Iterator != l_UnitTargets.end();)
                            {
                                bool l_Found = false;
                                uint8 l_TypesI = 0;
                                do
                                {
                                    if ((*l_Iterator)->GetCreatureType() == l_Types[l_TypesI])
                                    {
                                        l_Found = true;
                                        break;
                                    }
                                }
                                while (l_Types[++l_TypesI]);

                                if (l_Found)
                                    l_Iterator++;
                                else
                                    l_Iterator = l_UnitTargets.erase(l_Iterator);
                            }
                        }
                        // Glyph of Focused Wrath
                        else if (m_caster->HasAura(115738))
                        {
                            if (m_caster->GetTypeId() != TYPEID_PLAYER)
                                break;

                            Player* l_Player = m_caster->ToPlayer();
                            if (l_Player == nullptr)
                                break;

                            if (Unit* l_Target = l_Player->GetSelectedUnit())
                            {
                                if (p_EffIndex == 0)
                                {
                                    bool l_Found = false;
                                    uint8 l_TypesI = 0;
                                    do
                                    {
                                        if (l_Target->GetCreatureType() == l_Types[l_TypesI])
                                        {
                                            l_Found = true;
                                            break;
                                        }
                                    }
                                    while (l_Types[++l_TypesI]);

                                    l_UnitTargets.clear();
                                    if (l_Found)
                                        l_UnitTargets.push_back(l_Target);
                                }
                                else
                                {
                                    l_UnitTargets.clear();
                                    l_UnitTargets.push_back(l_Target);
                                }
                            }
                            else
                            {
                                l_UnitTargets.sort(JadeCore::WorldObjectDistanceCompareOrderPred(m_caster));
                                Unit* l_Victim = (*l_UnitTargets.begin())->ToUnit();

                                if (l_Victim)
                                {
                                    if (p_EffIndex == 0)
                                    {
                                        bool l_Found = false;
                                        uint8 l_TypesI = 0;
                                        do
                                        {
                                            if (l_Victim->GetCreatureType() == l_Types[l_TypesI])
                                            {
                                                l_Found = true;
                                                break;
                                            }
                                        }
                                        while (l_Types[++l_TypesI]);

                                        l_UnitTargets.clear();
                                        if (l_Found)
                                            l_UnitTargets.push_back(l_Victim);
                                    }
                                    else
                                    {
                                        l_UnitTargets.clear();
                                        l_UnitTargets.push_back(l_Victim);
                                    }
                                }
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }

                break;
            }
            case SPELLFAMILY_DRUID:
            {
                bool l_RemoveEnemies = true;
                switch(m_spellInfo->Id)
                {
                    /// Firebloom, Item  Druid T12 Restoration 4P Bonus
                    case 99017:
                        l_MaxSize = 1;
                        l_Power = POWER_HEALTH;
                        break;
                    /// Efflorescence
                    case 81269:
                        l_MaxSize = 3;
                        l_Power = POWER_HEALTH;
                        break;
                    /// Tranquility
                    case 157982:
                        l_MaxSize = 5;
                        l_Power = POWER_HEALTH;
                        break;

                    default:
                        l_RemoveEnemies = false;
                        break;
                }
                
                if (l_RemoveEnemies)
                {
                    /// Remove targets outside caster's raid
                    for (std::list<Unit*>::iterator l_Iterator = l_UnitTargets.begin(); l_Iterator != l_UnitTargets.end();)
                    {
                        if (!(*l_Iterator)->IsInRaidWith(m_caster))
                            l_Iterator = l_UnitTargets.erase(l_Iterator);
                        else
                            ++l_Iterator;
                    }
                }

                break;
            }
            default:
                break;
        }

        if (l_MaxSize && l_Power != -1)
        {
            if (Powers(l_Power) == POWER_HEALTH)
            {
                if (l_UnitTargets.size() > l_MaxSize)
                {
                    l_UnitTargets.sort(JadeCore::HealthPctOrderPred());
                    l_UnitTargets.resize(l_MaxSize);
                }
            }
            else
            {
                for (std::list<Unit*>::iterator l_Iterator = l_UnitTargets.begin(); l_Iterator != l_UnitTargets.end();)
                    if ((*l_Iterator)->getPowerType() != (Powers)l_Power)
                        l_Iterator = l_UnitTargets.erase(l_Iterator);
                    else
                        ++l_Iterator;

                if (l_UnitTargets.size() > l_MaxSize)
                {
                    l_UnitTargets.sort(JadeCore::PowerPctOrderPred((Powers)l_Power));
                    l_UnitTargets.resize(l_MaxSize);
                }
            }
        }

        // todo: move to scripts, but we must call it before resize list by MaxAffectedTargets
        // Intimidating Shout
        if (m_spellInfo->Id == 5246 && p_EffIndex != EFFECT_0)
            l_UnitTargets.remove(m_targets.GetUnitTarget());

        // Custom MoP Script
        if (m_caster->IsPlayer())
        {
            switch (m_spellInfo->Id)
            {
                // Spinning Crane Kick / Rushing Jade Wind : Give 1 Chi if the spell hits at least 3 targets
                case 107270:
                case 117640:
                case 148187:
                    if (m_caster->ToPlayer()->HasSpellCooldown(129881) || l_UnitTargets.size() < 3)
                        break;

                    m_caster->CastSpell(m_caster, 129881, true);
                    m_caster->ToPlayer()->AddSpellCooldown(129881, 0, 3 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }

        if (m_caster->IsPlayer() && m_spellInfo->Id == 1449)
            if (m_caster->ToPlayer()->GetSpecializationId() == SPEC_MAGE_ARCANE)
                if (roll_chance_i(30))
                    m_caster->AddAura(36032, m_caster);

        // Other special target selection goes here
        if (uint32 l_MaxTargets = m_spellValue->MaxAffectedTargets)
            JadeCore::Containers::RandomResizeList(l_UnitTargets, l_MaxTargets);

        for (std::list<Unit*>::iterator l_Iterator = l_UnitTargets.begin(); l_Iterator != l_UnitTargets.end(); ++l_Iterator)
            AddUnitTarget(*l_Iterator, p_EffMask, false);
    }

    if (!l_GObjTargets.empty())
    {
        if (uint32 l_MaxTargets = m_spellValue->MaxAffectedTargets)
            JadeCore::Containers::RandomResizeList(l_GObjTargets, l_MaxTargets);

        for (std::list<GameObject*>::iterator l_Iterator = l_GObjTargets.begin(); l_Iterator != l_GObjTargets.end(); ++l_Iterator)
            AddGOTarget(*l_Iterator, p_EffMask);
    }

    if (!l_AreaTriggerTargets.empty())
    {
        if (uint32 l_MaxTargets = m_spellValue->MaxAffectedTargets)
            JadeCore::Containers::RandomResizeList(l_AreaTriggerTargets, l_MaxTargets);

        for (std::list<AreaTrigger*>::iterator l_Iterator = l_AreaTriggerTargets.begin(); l_Iterator != l_AreaTriggerTargets.end(); ++l_Iterator)
            AddAreaTriggerTarget(*l_Iterator, p_EffMask);
    }
}

void Spell::SelectImplicitCasterDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    switch (targetType.GetTarget())
    {
        case TARGET_DEST_CASTER:
            m_targets.SetDst(*m_caster);
            return;
        case TARGET_DEST_HOME:
            if (Player* playerCaster = m_caster->ToPlayer())
                m_targets.SetDst(playerCaster->m_homebindX, playerCaster->m_homebindY, playerCaster->m_homebindZ, playerCaster->GetOrientation(), playerCaster->m_homebindMapId);
            return;
        case TARGET_DEST_DB:
            if (SpellTargetPosition const* st = sSpellMgr->GetSpellTargetPosition(m_spellInfo->Id, effIndex))
            {
                // @TODO: fix this check
                if (m_spellInfo->HasEffect(SPELL_EFFECT_TELEPORT_UNITS))
                    m_targets.SetDst(st->target_X, st->target_Y, st->target_Z, st->target_Orientation, (int32)st->target_mapId);
                else if (st->target_mapId == m_caster->GetMapId())
                    m_targets.SetDst(st->target_X, st->target_Y, st->target_Z, st->target_Orientation);
            }
            else
            {
                WorldObject* target = m_targets.GetObjectTarget();
                m_targets.SetDst(target ? *target : *m_caster);
            }
            return;
        case TARGET_DEST_CASTER_FISHING:
        {
             float min_dis = m_spellInfo->GetMinRange(true);
             float max_dis = m_spellInfo->GetMaxRange(true);
             float dis = (float)rand_norm() * (max_dis - min_dis) + min_dis;
             float x, y, z, angle;
             angle = (float)rand_norm() * static_cast<float>(M_PI * 35.0f / 180.0f) - static_cast<float>(M_PI * 17.5f / 180.0f);
             m_caster->GetClosePoint(x, y, z, DEFAULT_WORLD_OBJECT_SIZE, dis, angle);
             m_targets.SetDst(x, y, z, m_caster->GetOrientation());
             return;
        }
        default:
            break;
    }

    float dist;
    float angle = targetType.CalcDirectionAngle();
    float objSize = m_caster->GetObjectSize();
    if (targetType.GetTarget() == TARGET_DEST_CASTER_SUMMON)
        dist = PET_FOLLOW_DIST;
    else
        dist = m_spellInfo->Effects[effIndex].CalcRadius(m_caster);

    if (dist < objSize)
        dist = objSize;
    else if (targetType.GetTarget() == TARGET_DEST_CASTER_RANDOM)
        dist = objSize + (dist - objSize) * (float)rand_norm();

    Position pos;
    if (targetType.GetTarget() == TARGET_DEST_CASTER_FRONT_LEAP)
        m_caster->GetFirstCollisionPosition(pos, dist, angle);
    else
        m_caster->GetNearPosition(pos, dist, angle);
    m_targets.SetDst(*m_caster);
    m_targets.ModDst(pos);
}

void Spell::SelectImplicitTargetDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    WorldObject* target = m_targets.GetObjectTarget();
    switch (targetType.GetTarget())
    {
        case TARGET_DEST_TARGET_ENEMY:
        case TARGET_DEST_TARGET_ANY:
            m_targets.SetDst(*target);
            return;
        default:
            break;
    }

    float angle = targetType.CalcDirectionAngle();
    float objSize = target->GetObjectSize();
    float dist = m_spellInfo->Effects[effIndex].CalcRadius(m_caster);
    if (dist < objSize)
        dist = objSize;
    else if (targetType.GetTarget() == TARGET_DEST_TARGET_RANDOM)
        dist = objSize + (dist - objSize) * (float)rand_norm();

    Position pos;
    target->GetNearPosition(pos, dist, angle);
    m_targets.SetDst(*target);
    m_targets.ModDst(pos);
}

void Spell::SelectImplicitDestDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    // set destination to caster if no dest provided
    // can only happen if previous destination target could not be set for some reason
    // (not found nearby target, or channel target for example
    // maybe we should abort the spell in such case?
    if (!m_targets.HasDst())
        m_targets.SetDst(*m_caster);

    switch (targetType.GetTarget())
    {
        case TARGET_DEST_DYNOBJ_ENEMY:
        case TARGET_DEST_DYNOBJ_ALLY:
        case TARGET_DEST_DYNOBJ_NONE:
        case TARGET_DEST_DEST:
            return;
        case TARGET_DEST_TRAJ:
            SelectImplicitTrajTargets();
            return;
        default:
            break;
    }

    float angle = targetType.CalcDirectionAngle();
    float dist = m_spellInfo->Effects[effIndex].CalcRadius(m_caster);
    if (targetType.GetTarget() == TARGET_DEST_DEST_RANDOM)
        dist *= (float)rand_norm();

    Position pos = *m_targets.GetDstPos();
    m_caster->MovePosition(pos, dist, angle);
    m_targets.ModDst(pos);
}

void Spell::SelectImplicitCasterObjectTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    WorldObject* target = NULL;
    bool checkIfValid = true;

    switch (targetType.GetTarget())
    {
        case TARGET_UNIT_CASTER:
            target = m_caster;
            checkIfValid = false;
            break;
        case TARGET_UNIT_MASTER:
            target = m_caster->GetCharmerOrOwner();
            if (target == nullptr)
            {
                target = m_caster;
                checkIfValid = false;
            }
            break;
        case TARGET_UNIT_PET:
            target = m_caster->GetGuardianPet();
            break;
        case TARGET_UNIT_SUMMONER:
            if (m_caster->isSummon())
                target = m_caster->ToTempSummon()->GetSummoner();
            break;
        case TARGET_UNIT_VEHICLE:
            target = m_caster->GetVehicleBase();
            break;
        case TARGET_UNIT_PASSENGER_0:
        case TARGET_UNIT_PASSENGER_1:
        case TARGET_UNIT_PASSENGER_2:
        case TARGET_UNIT_PASSENGER_3:
        case TARGET_UNIT_PASSENGER_4:
        case TARGET_UNIT_PASSENGER_5:
        case TARGET_UNIT_PASSENGER_6:
        case TARGET_UNIT_PASSENGER_7:
            if (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->IsVehicle())
                target = m_caster->GetVehicleKit()->GetPassenger(targetType.GetTarget() - TARGET_UNIT_PASSENGER_0);
            break;
        case TARGET_UNIT_CASTER_AREA_RAID: /// Raids buffs
        {
            CleanupTargetList();

            for (uint8 l_I = 0; l_I < m_spellInfo->EffectCount; ++l_I)
            {
                if (!m_spellInfo->Effects[l_I].IsEffect())
                    continue;

                Position const* l_Center = m_caster;
                std::list<WorldObject*> l_Targets;
                float l_Radius = m_spellInfo->Effects[l_I].CalcRadius(m_caster) * m_spellValue->RadiusMod;

                SearchAreaTargets(l_Targets, l_Radius, l_Center, m_caster, TARGET_OBJECT_TYPE_UNIT, SpellTargetCheckTypes::TARGET_CHECK_RAID, m_spellInfo->Effects[l_I].ImplicitTargetConditions);

                std::list<Unit*> l_UnitTargets;

                for (WorldObject* l_Iterator : l_Targets)
                {
                    if (Unit* l_UnitTarget = l_Iterator->ToUnit())
                    {
                        /// Raid buffs work just on Players
                        if (l_UnitTarget->GetTypeId() == TypeID::TYPEID_PLAYER)
                            l_UnitTargets.push_back(l_UnitTarget);
                    }
                }

                if (!l_UnitTargets.empty())
                {
                    if (uint32 l_MaxTargets = m_spellValue->MaxAffectedTargets)
                        JadeCore::Containers::RandomResizeList(l_UnitTargets, l_MaxTargets);

                    for (Unit* l_Iterator : l_UnitTargets)
                        AddUnitTarget(l_Iterator, 1 << l_I, false);
                }
            }
            break;
        }
        default:
            break;
    }

    CallScriptObjectTargetSelectHandlers(target, effIndex);

    if (target && target->ToUnit())
        AddUnitTarget(target->ToUnit(), 1 << effIndex, checkIfValid);
}

void Spell::SelectImplicitTargetObjectTargets(SpellEffIndex p_EffIndex, SpellImplicitTargetInfo const& p_TargetType)
{
    // Persistent auras should cast trigger spells when they is activated on target
    if (m_spellInfo->HasPersistenAura())
        return;

    ASSERT((m_targets.GetObjectTarget() || m_targets.GetItemTarget() || m_targets.GetUnitTarget()) && "Spell::SelectImplicitTargetObjectTargets - no explicit object or item target available!");

    WorldObject* l_Target = m_targets.GetObjectTarget();

    CallScriptObjectTargetSelectHandlers(l_Target, p_EffIndex);

    if (l_Target)
    {
        if (Unit* l_Unit = l_Target->ToUnit())
            AddUnitTarget(l_Unit, 1 << p_EffIndex, m_redirected == true ? false : true, false, p_EffIndex);
        else if (GameObject* l_GameObject = l_Target->ToGameObject())
            AddGOTarget(l_GameObject, 1 << p_EffIndex);
        else if (AreaTrigger* l_AreaTrigger = l_Target->ToAreaTrigger())
            AddAreaTriggerTarget(l_AreaTrigger, 1 << p_EffIndex);

        SelectImplicitChainTargets(p_EffIndex, p_TargetType, l_Target, 1 << p_EffIndex);
    }
    // Script hook can remove object target and we would wrongly land here
    else if (Item* l_Item = m_targets.GetItemTarget())
        AddItemTarget(l_Item, 1 << p_EffIndex);

    switch (p_TargetType.GetTarget())
    {
        case TARGET_UNIT_ALLY_OR_RAID: // Raids buffs
        {
            if (!l_Target || !l_Target->ToUnit() || !m_caster->ToPlayer())
                break;

            bool l_SameRaid = false;
            if (Player* l_Player = l_Target->ToPlayer())
            {
                if (l_Player->IsInSameGroupWith(m_caster->ToPlayer()) || l_Player->IsInSameRaidWith(m_caster->ToPlayer()))
                    l_SameRaid = true;
            }
            else if (Unit* l_Owner = l_Target->ToUnit()->GetOwner())
            {
                if (Player* l_PlayerOwner = l_Owner->ToPlayer())
                {
                    if (l_PlayerOwner->IsInSameGroupWith(m_caster->ToPlayer()) || l_PlayerOwner->IsInSameRaidWith(m_caster->ToPlayer()))
                        l_SameRaid = true;
                }
            }

            if (l_SameRaid)
            {
                CleanupTargetList();

                for (uint8 l_I = 0; l_I < m_spellInfo->EffectCount; ++l_I)
                {
                    // not call for empty effect.
                    // Also some spells use not used effect targets for store targets for dummy effect in triggered spells
                    if (!m_spellInfo->Effects[l_I].IsEffect())
                        continue;

                    Position const* l_Center = m_caster;
                    std::list<WorldObject*> l_Targets;
                    float l_Radius = m_spellInfo->Effects[l_I].CalcRadius(m_caster) * m_spellValue->RadiusMod;

                    SearchAreaTargets(l_Targets, l_Radius, l_Center, m_caster, TARGET_OBJECT_TYPE_UNIT, TARGET_CHECK_RAID, m_spellInfo->Effects[l_I].ImplicitTargetConditions);

                    std::list<Unit*> l_UnitTargets;
                    // for compatibility with older code - add only unit and go targets
                    // TODO: remove this
                    if (!l_Targets.empty())
                    {
                        for (std::list<WorldObject*>::iterator l_Iterator = l_Targets.begin(); l_Iterator != l_Targets.end(); ++l_Iterator)
                        {
                            if ((*l_Iterator))
                            {
                                if (Unit* unitTarget = (*l_Iterator)->ToUnit())
                                {
                                    /// Raid buffs work just on Players
                                    if (unitTarget->ToPlayer())
                                        l_UnitTargets.push_back(unitTarget);
                                }
                            }
                        }
                    }

                    if (!l_UnitTargets.empty())
                    {
                        // Other special target selection goes here
                        if (uint32 l_MaxTargets = m_spellValue->MaxAffectedTargets)
                            JadeCore::Containers::RandomResizeList(l_UnitTargets, l_MaxTargets);

                        for (std::list<Unit*>::iterator l_Iterator = l_UnitTargets.begin(); l_Iterator != l_UnitTargets.end(); ++l_Iterator)
                            AddUnitTarget(*l_Iterator, 1 << l_I, false);
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

void Spell::SelectImplicitChainTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, WorldObject* target, uint32 effMask)
{
    uint32 maxTargets = m_spellInfo->Effects[effIndex].ChainTarget;
    if (Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_JUMP_TARGETS, maxTargets, this);

    // Havoc
    if (Aura* havoc = m_caster->GetAura(80240))
    {
        int8 l_StacksToDrop = GetSpellInfo()->Id == 116858 ? 3 : 1;
        if (GetSpellInfo()->SpellFamilyFlags & flag128(0x00000000, 0x00000000, 0x00000000, 0x00400000) &&
            havoc->GetStackAmount() >= l_StacksToDrop && target->ToUnit() && !target->ToUnit()->HasAura(80240) && effIndex == EFFECT_0)
        {
            std::list<Unit*> targets;
            Unit* secondTarget = NULL;
            m_caster->GetAttackableUnitListInRange(targets, 40.0f);

            targets.remove(target->ToUnit());
            targets.remove(m_caster);

            for (auto itr : targets)
            {
                if (itr->IsWithinLOSInMap(m_caster) && itr->IsWithinDist(m_caster, 40.0f)
                    && target->GetGUID() != itr->GetGUID() && itr->HasAura(80240, m_caster->GetGUID()))
                {
                    secondTarget = itr;
                    break;
                }
            }

            if (secondTarget && target->GetGUID() != secondTarget->GetGUID())
            {
                int8 l_Stacks = havoc->GetStackAmount();

                if (GetSpellInfo()->Id != 157736) ///< Dot of immolate should not drop a stack
                    l_Stacks -= l_StacksToDrop;

                if (l_Stacks > 0)
                {
                    havoc->SetStackAmount(l_Stacks);
                }
                else
                {
                    havoc->Remove();
                    secondTarget->RemoveAurasDueToSpell(havoc->GetId());
                }

                AddUnitTarget(secondTarget, effMask, false);
            }
        }
    }

    if (maxTargets > 1)
    {
        // mark damage multipliers as used
        for (uint32 k = effIndex; k < m_spellInfo->EffectCount; ++k)
            if (effMask & (1 << k))
                m_damageMultipliers[k] = 1.0f;
        m_applyMultiplierMask |= effMask;

        std::list<WorldObject*> targets;
        SearchChainTargets(targets, maxTargets - 1, target, targetType.GetObjectType(), targetType.GetCheckType()
            , m_spellInfo->Effects[effIndex].ImplicitTargetConditions, targetType.GetTarget() == TARGET_UNIT_TARGET_CHAINHEAL_ALLY);

        // Chain primary target is added earlier
        CallScriptObjectAreaTargetSelectHandlers(targets, effIndex);

        // for backward compability
        std::list<Unit*> unitTargets;
        for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
            if (Unit* unitTarget = (*itr)->ToUnit())
                unitTargets.push_back(unitTarget);

        for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end(); ++itr)
            AddUnitTarget(*itr, effMask, false);
    }
}

float tangent(float x)
{
    x = tan(x);
    //if (x < std::numeric_limits<float>::max() && x > -std::numeric_limits<float>::max()) return x;
    //if (x >= std::numeric_limits<float>::max()) return std::numeric_limits<float>::max();
    //if (x <= -std::numeric_limits<float>::max()) return -std::numeric_limits<float>::max();
    if (x < 100000.0f && x > -100000.0f) return x;
    if (x >= 100000.0f) return 100000.0f;
    if (x <= 100000.0f) return -100000.0f;
    return 0.0f;
}

#define DEBUG_TRAJ(a) //a

void Spell::SelectImplicitTrajTargets()
{
    if (!m_targets.HasTraj())
        return;

    float dist2d = m_targets.GetDist2d();
    if (!dist2d)
        return;

    float srcToDestDelta = m_targets.GetDstPos()->m_positionZ - m_targets.GetSrcPos()->m_positionZ;

    std::list<WorldObject*> targets;
    JadeCore::WorldObjectSpellTrajTargetCheck check(dist2d, m_targets.GetSrcPos(), m_caster, m_spellInfo);
    JadeCore::WorldObjectListSearcher<JadeCore::WorldObjectSpellTrajTargetCheck> searcher(m_caster, targets, check, GRID_MAP_TYPE_MASK_ALL);
    SearchTargets<JadeCore::WorldObjectListSearcher<JadeCore::WorldObjectSpellTrajTargetCheck> > (searcher, GRID_MAP_TYPE_MASK_ALL, m_caster, m_targets.GetSrcPos(), dist2d);
    if (targets.empty())
        return;

    targets.sort(JadeCore::ObjectDistanceOrderPred(m_caster));

    float b = tangent(m_targets.GetElevation());
    float a = (srcToDestDelta - dist2d * b) / (dist2d * dist2d);
    if (a > -0.0001f)
        a = 0;
    float bestDist = m_spellInfo->GetMaxRange(false);

    std::list<WorldObject*>::const_iterator itr = targets.begin();
    for (; itr != targets.end(); ++itr)
    {
        if (Unit* unitTarget = (*itr)->ToUnit())
            if (m_caster == *itr || m_caster->IsOnVehicle(unitTarget) || (unitTarget)->GetVehicle())//(*itr)->IsOnVehicle(m_caster))
                continue;

        const float size = std::max((*itr)->GetObjectSize() * 0.7f, 1.0f); // 1/sqrt(3)
        // TODO: all calculation should be based on src instead of m_caster
        const float objDist2d = m_targets.GetSrcPos()->GetExactDist2d(*itr) * std::cos(m_targets.GetSrcPos()->GetRelativeAngle(*itr));
        const float dz = (*itr)->GetPositionZ() - m_targets.GetSrcPos()->m_positionZ;

        float dist = objDist2d - size;
        float height = dist * (a * dist + b);

        if (dist < bestDist && height < dz + size && height > dz - size)
        {
            bestDist = dist > 0 ? dist : 0;
            break;
        }

#define CHECK_DIST {\
            DEBUG_TRAJ(sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell::SelectTrajTargets: dist %f, height %f.", dist, height);)\
            if (dist > bestDist)\
                continue;\
            if (dist < objDist2d + size && dist > objDist2d - size)\
            {\
                bestDist = dist;\
                break;\
            }\
        }

        if (!a)
        {
            height = dz - size;
            dist = height / b;
            CHECK_DIST;

            height = dz + size;
            dist = height / b;
            CHECK_DIST;

            continue;
        }

        height = dz - size;
        float sqrt1 = b * b + 4 * a * height;
        if (sqrt1 > 0)
        {
            sqrt1 = sqrt(sqrt1);
            dist = (sqrt1 - b) / (2 * a);
            CHECK_DIST;
        }

        height = dz + size;
        float sqrt2 = b * b + 4 * a * height;
        if (sqrt2 > 0)
        {
            sqrt2 = sqrt(sqrt2);
            dist = (sqrt2 - b) / (2 * a);
            CHECK_DIST;

            dist = (-sqrt2 - b) / (2 * a);
            CHECK_DIST;
        }

        if (sqrt1 > 0)
        {
            dist = (-sqrt1 - b) / (2 * a);
            CHECK_DIST;
        }
    }

    if (m_targets.GetSrcPos()->GetExactDist2d(m_targets.GetDstPos()) > bestDist)
    {
        float x = m_targets.GetSrcPos()->m_positionX + std::cos(m_caster->GetOrientation()) * bestDist;
        float y = m_targets.GetSrcPos()->m_positionY + std::sin(m_caster->GetOrientation()) * bestDist;
        float z = m_targets.GetSrcPos()->m_positionZ + bestDist * (a * bestDist + b);

        if (itr != targets.end())
        {
            float distSq = (*itr)->GetExactDistSq(x, y, z);
            float sizeSq = (*itr)->GetObjectSize();
            sizeSq *= sizeSq;
            if (distSq > sizeSq)
            {
                float factor = 1 - sqrt(sizeSq / distSq);
                x += factor * ((*itr)->GetPositionX() - x);
                y += factor * ((*itr)->GetPositionY() - y);
                z += factor * ((*itr)->GetPositionZ() - z);

                distSq = (*itr)->GetExactDistSq(x, y, z); ///< distSq is never read 01/18/16
            }
        }

        Position trajDst;
        trajDst.Relocate(x, y, z, m_caster->GetOrientation());
        m_targets.ModDst(trajDst);
    }
}

void Spell::SelectEffectTypeImplicitTargets(uint8 p_EffIndex)
{
    // special case for SPELL_EFFECT_SUMMON_RAF_FRIEND and SPELL_EFFECT_SUMMON_PLAYER
    // TODO: this is a workaround - target shouldn't be stored in target map for those spells
    switch (m_spellInfo->Effects[p_EffIndex].Effect)
    {
        case SPELL_EFFECT_SUMMON_RAF_FRIEND:
        case SPELL_EFFECT_SUMMON_PLAYER:
            if (m_caster->IsPlayer() && m_caster->ToPlayer()->GetSelection())
            {
                WorldObject* l_Target = ObjectAccessor::FindPlayer(m_caster->ToPlayer()->GetSelection());

                CallScriptObjectTargetSelectHandlers(l_Target, SpellEffIndex(p_EffIndex));

                if (l_Target && l_Target->ToPlayer())
                    AddUnitTarget(l_Target->ToUnit(), 1 << p_EffIndex, false);
            }
            return;
        default:
            break;
    }

    // select spell implicit targets based on effect type
    if (!m_spellInfo->Effects[p_EffIndex].GetImplicitTargetType())
        return;

    uint32 l_TargetMask = m_spellInfo->Effects[p_EffIndex].GetMissingTargetMask();
    if (!l_TargetMask)
        return;

    WorldObject* l_Target = NULL;

    switch (m_spellInfo->Effects[p_EffIndex].GetImplicitTargetType())
    {
        // add explicit object target or self to the target map
        case EFFECT_IMPLICIT_TARGET_EXPLICIT:
            // player which not released his spirit is Unit, but target flag for it is TARGET_FLAG_CORPSE_MASK
            if (l_TargetMask & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK))
            {
                if (Unit* l_UnitTarget = m_targets.GetUnitTarget())
                    l_Target = l_UnitTarget;
                else if (l_TargetMask & TARGET_FLAG_CORPSE_MASK)
                {
                    if (Corpse* l_CorpseTarget = m_targets.GetCorpseTarget())
                    {
                        // TODO: this is a workaround - corpses should be added to spell target map too, but we can't do that so we add owner instead
                        if (Player* l_Owner = ObjectAccessor::FindPlayer(l_CorpseTarget->GetOwnerGUID()))
                            l_Target = l_Owner;
                    }
                }
                else //if (targetMask & TARGET_FLAG_UNIT_MASK)
                    l_Target = m_caster;
            }
            if (l_TargetMask & TARGET_FLAG_ITEM_MASK)
            {
                if (Item* l_ItemTarget = m_targets.GetItemTarget())
                    AddItemTarget(l_ItemTarget, 1 << p_EffIndex);
                return;
            }
            if (l_TargetMask & TARGET_FLAG_GAMEOBJECT_MASK)
                l_Target = m_targets.GetGOTarget();
            break;
        // add self to the target map
        case EFFECT_IMPLICIT_TARGET_CASTER:
            if (l_TargetMask & TARGET_FLAG_UNIT_MASK)
                l_Target = m_caster;
            break;
        default:
            break;
    }

    CallScriptObjectTargetSelectHandlers(l_Target, SpellEffIndex(p_EffIndex));

    if (l_Target)
    {
        if (l_Target->ToUnit())
            AddUnitTarget(l_Target->ToUnit(), 1 << p_EffIndex, false);
        else if (l_Target->ToGameObject())
            AddGOTarget(l_Target->ToGameObject(), 1 << p_EffIndex);
        else if (l_Target->ToAreaTrigger())
            AddAreaTriggerTarget(l_Target->ToAreaTrigger(), 1 << p_EffIndex);
    }
}

uint32 Spell::GetSearcherTypeMask(SpellTargetObjectTypes objType, ConditionContainer* condList)
{
    // this function selects which containers need to be searched for spell target
    uint32 retMask = GRID_MAP_TYPE_MASK_ALL;

    // filter searchers based on searched object type
    switch (objType)
    {
        case TARGET_OBJECT_TYPE_UNIT:
        case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
        case TARGET_OBJECT_TYPE_CORPSE:
        case TARGET_OBJECT_TYPE_CORPSE_ENEMY:
        case TARGET_OBJECT_TYPE_CORPSE_ALLY:
            retMask &= GRID_MAP_TYPE_MASK_PLAYER | GRID_MAP_TYPE_MASK_CORPSE | GRID_MAP_TYPE_MASK_CREATURE;
            break;
        case TARGET_OBJECT_TYPE_GOBJ:
        case TARGET_OBJECT_TYPE_GOBJ_ITEM:
            retMask &= GRID_MAP_TYPE_MASK_GAMEOBJECT;
            break;
        case TARGET_OBJECT_TYPE_AREATRIGGER:
            retMask &= GRID_MAP_TYPE_MASK_AREATRIGGER;
            break;
        default:
            break;
    }
    if (!(m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_DEAD))
        retMask &= ~GRID_MAP_TYPE_MASK_CORPSE;
    if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_ONLY_TARGET_PLAYERS)
        retMask &= GRID_MAP_TYPE_MASK_CORPSE | GRID_MAP_TYPE_MASK_PLAYER;
    if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_ONLY_TARGET_GHOSTS)
        retMask &= GRID_MAP_TYPE_MASK_PLAYER;

    if (condList)
        retMask &= sConditionMgr->GetSearcherTypeMaskForConditionList(*condList);
    return retMask;
}

template<class SEARCHER>
void Spell::SearchTargets(SEARCHER& searcher, uint32 containerMask, Unit* referer, Position const* pos, float radius)
{
    if (!containerMask)
        return;

    // search world and grid for possible targets
    bool searchInGrid = containerMask & (GRID_MAP_TYPE_MASK_CREATURE | GRID_MAP_TYPE_MASK_GAMEOBJECT);
    bool searchInWorld = containerMask & (GRID_MAP_TYPE_MASK_CREATURE | GRID_MAP_TYPE_MASK_PLAYER | GRID_MAP_TYPE_MASK_CORPSE);
    if (searchInGrid || searchInWorld)
    {
        float x,y;
        x = pos->GetPositionX();
        y = pos->GetPositionY();

        CellCoord p(JadeCore::ComputeCellCoord(x, y));
        Cell cell(p);
        cell.SetNoCreate();

        Map& map = *(referer->GetMap());

        if (searchInWorld)
        {
            TypeContainerVisitor<SEARCHER, WorldTypeMapContainer> world_object_notifier(searcher);
            cell.Visit(p, world_object_notifier, map, radius, x, y);
        }
        if (searchInGrid)
        {
            TypeContainerVisitor<SEARCHER, GridTypeMapContainer >  grid_object_notifier(searcher);
            cell.Visit(p, grid_object_notifier, map, radius, x , y);
        }
    }
}

WorldObject* Spell::SearchNearbyTarget(float range, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectionType, ConditionContainer* condList)
{
    WorldObject* target = NULL;
    uint32 containerTypeMask = GetSearcherTypeMask(objectType, condList);
    if (!containerTypeMask)
        return NULL;
    JadeCore::WorldObjectSpellNearbyTargetCheck check(range, m_caster, m_spellInfo, selectionType, condList);
    JadeCore::WorldObjectLastSearcher<JadeCore::WorldObjectSpellNearbyTargetCheck> searcher(m_caster, target, check, containerTypeMask);
    SearchTargets<JadeCore::WorldObjectLastSearcher<JadeCore::WorldObjectSpellNearbyTargetCheck> > (searcher, containerTypeMask, m_caster, m_caster, range);
    return target;
}

void Spell::SearchAreaTargets(std::list<WorldObject*>& targets, float range, Position const* position, Unit* referer, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectionType, ConditionContainer* condList)
{
    uint32 containerTypeMask = GetSearcherTypeMask(objectType, condList);
    if (!containerTypeMask)
        return;
    JadeCore::WorldObjectSpellAreaTargetCheck check(range, position, m_caster, referer, m_spellInfo, selectionType, condList);
    JadeCore::WorldObjectListSearcher<JadeCore::WorldObjectSpellAreaTargetCheck> searcher(m_caster, targets, check, containerTypeMask);
    SearchTargets<JadeCore::WorldObjectListSearcher<JadeCore::WorldObjectSpellAreaTargetCheck> > (searcher, containerTypeMask, m_caster, position, range);
}

void Spell::SearchChainTargets(std::list<WorldObject*>& targets, uint32 chainTargets, WorldObject* target, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectType, ConditionContainer* condList, bool isChainHeal)
{
    // max dist for jump target selection
    float jumpRadius = 0.0f;
    switch (m_spellInfo->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_RANGED:
            // 7.5y for multi shot
            jumpRadius = 7.5f;
            if (m_spellInfo->SpellFamilyFlags[2] & 0x1)
                jumpRadius = 2.5f; ///< Chimaera Shot has smaller radius
            break;
        case SPELL_DAMAGE_CLASS_MELEE:
            // 5y for swipe, cleave and similar
            jumpRadius = 5.0f;
            break;
        case SPELL_DAMAGE_CLASS_NONE:
        case SPELL_DAMAGE_CLASS_MAGIC:
            // 12.5y for chain heal spell since 3.2 patch
            if (isChainHeal)
                jumpRadius = 12.5f;
            // 10y as default for magic chain spells
            else
                jumpRadius = 10.0f;
            break;
    }

    if (Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_JUMP_DISTANCE, jumpRadius, this);

    // chain lightning/heal spells and similar - allow to jump at larger distance and go out of los
    bool isBouncingFar = (m_spellInfo->AttributesEx4 & SPELL_ATTR4_AREA_TARGET_CHAIN
        || m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_NONE
        || m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_MAGIC);

    // max dist which spell can reach
    float searchRadius = jumpRadius;
    if (isBouncingFar)
        searchRadius *= chainTargets;

    std::list<WorldObject*> tempTargets;
    SearchAreaTargets(tempTargets, searchRadius, target, m_caster, objectType, selectType, condList);
    tempTargets.remove(target);

    // remove targets which are always invalid for chain spells
    // for some spells allow only chain targets in front of caster (swipe for example)
    if (!isBouncingFar)
    {
        for (std::list<WorldObject*>::iterator itr = tempTargets.begin(); itr != tempTargets.end();)
        {
            std::list<WorldObject*>::iterator checkItr = itr++;
            if (!m_caster->HasInArc(static_cast<float>(M_PI), *checkItr))
                tempTargets.erase(checkItr);
        }
    }

    while (chainTargets)
    {
        // try to get unit for next chain jump
        std::list<WorldObject*>::iterator foundItr = tempTargets.end();
        // get unit with highest hp deficit in dist
        if (isChainHeal)
        {
            uint32 maxHPDeficit = 0;
            for (std::list<WorldObject*>::iterator itr = tempTargets.begin(); itr != tempTargets.end(); ++itr)
            {
                if (Unit* unitTarget = (*itr)->ToUnit())
                {
                    uint32 deficit = unitTarget->GetMaxHealth() - unitTarget->GetHealth();
                    if ((deficit > maxHPDeficit || foundItr == tempTargets.end()) && target->IsWithinDist(unitTarget, jumpRadius) && target->IsWithinLOSInMap(unitTarget))
                    {
                        foundItr = itr;
                        maxHPDeficit = deficit;
                    }
                }
            }
        }
        // get closest object
        else
        {
            for (std::list<WorldObject*>::iterator itr = tempTargets.begin(); itr != tempTargets.end(); ++itr)
            {
                if (foundItr == tempTargets.end())
                {
                    if ((!isBouncingFar || target->IsWithinDist(*itr, jumpRadius)) && target->IsWithinLOSInMap(*itr))
                        foundItr = itr;
                }
                else if (target->GetDistanceOrder(*itr, *foundItr) && target->IsWithinLOSInMap(*itr))
                    foundItr = itr;
            }
        }
        // not found any valid target - chain ends
        if (foundItr == tempTargets.end())
            break;
        target = *foundItr;
        tempTargets.erase(foundItr);
        targets.push_back(target);
        --chainTargets;
    }
}

void Spell::prepareDataForTriggerSystem(AuraEffect const* /*triggeredByAura*/)
{
    //==========================================================================================
    // Now fill data for trigger system, need know:
    // can spell trigger another or not (m_canTrigger)
    // Create base triggers flags for Attacker and Victim (m_procAttacker, m_procVictim and m_procEx)
    //==========================================================================================

    m_procVictim = m_procAttacker = 0;
    // Get data for type of attack and fill base info for trigger
    switch (m_spellInfo->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_MELEE:
            m_procAttacker = PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS;
            if (m_attackType == WeaponAttackType::OffAttack)
                m_procAttacker |= PROC_FLAG_DONE_OFFHAND_ATTACK;
            else
                m_procAttacker |= PROC_FLAG_DONE_MAINHAND_ATTACK;
            m_procVictim   = PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS;
            break;
        case SPELL_DAMAGE_CLASS_RANGED:
            // Auto attack
            if (m_spellInfo->AttributesEx2 & SPELL_ATTR2_AUTOREPEAT_FLAG)
            {
                m_procAttacker = PROC_FLAG_DONE_RANGED_AUTO_ATTACK;
                m_procVictim   = PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK;
            }
            else // Ranged spell attack
            {
                m_procAttacker = PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS;
                m_procVictim   = PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS;
            }
            break;
        default:
            if (m_spellInfo->EquippedItemClass == ITEM_CLASS_WEAPON &&
                m_spellInfo->EquippedItemSubClassMask & (1<<ITEM_SUBCLASS_WEAPON_WAND)
                && m_spellInfo->AttributesEx2 & SPELL_ATTR2_AUTOREPEAT_FLAG) // Wands auto attack
            {
                m_procAttacker = PROC_FLAG_DONE_RANGED_AUTO_ATTACK;
                m_procVictim   = PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK;
            }
            // For other spells trigger procflags are set in Spell::DoAllEffectOnTarget
            // Because spell positivity is dependant on target
    }
    m_procEx = PROC_EX_NONE;

    // Hunter trap spells - activation proc for Lock and Load, Entrapment and Misdirection
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_HUNTER &&
        (m_spellInfo->SpellFamilyFlags[0] & 0x18 ||     // Freezing and Frost Trap, Freezing Arrow
        m_spellInfo->Id == 57879 ||                     // Snake Trap - done this way to avoid double proc
        m_spellInfo->SpellFamilyFlags[2] & 0x00024000)) // Explosive and Immolation Trap

        m_procAttacker |= PROC_FLAG_DONE_TRAP_ACTIVATION;

    /* Effects which are result of aura proc from triggered spell cannot proc
        to prevent chain proc of these spells */

    // Hellfire Effect - trigger as DOT
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_WARLOCK && m_spellInfo->SpellFamilyFlags[0] & 0x00000040)
    {
        m_procAttacker = PROC_FLAG_DONE_PERIODIC;
        m_procVictim   = PROC_FLAG_TAKEN_PERIODIC;
    }

    // Ranged autorepeat attack is set as triggered spell - ignore it
    if (!(m_procAttacker & PROC_FLAG_DONE_RANGED_AUTO_ATTACK))
    {
        if (_triggeredCastFlags & TRIGGERED_DISALLOW_PROC_EVENTS &&
            (m_spellInfo->AttributesEx2 & SPELL_ATTR2_TRIGGERED_CAN_TRIGGER_PROC ||
            m_spellInfo->AttributesEx3 & SPELL_ATTR3_TRIGGERED_CAN_TRIGGER_PROC_2))
            m_procEx |= PROC_EX_INTERNAL_CANT_PROC;
        else if (_triggeredCastFlags & TRIGGERED_DISALLOW_PROC_EVENTS)
            m_procEx |= PROC_EX_INTERNAL_TRIGGERED;
    }
    // Totem casts require spellfamilymask defined in spell_proc_event to proc
    if (m_originalCaster && m_caster != m_originalCaster && m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->isTotem() && m_caster->IsControlledByPlayer())
        m_procEx |= PROC_EX_INTERNAL_REQ_FAMILY;
}

void Spell::CleanupTargetList()
{
    m_UniqueTargetInfo.clear();
    m_UniqueGOTargetInfo.clear();
    m_UniqueItemInfo.clear();
    m_delayMoment = 0;
}

void Spell::AddUnitTarget(Unit* target, uint32 effectMask, bool checkIfValid /*= true*/, bool implicit /*= true*/, uint8 effectIndex /* = EFFECT_0 */)
{
    for (uint32 effIndex = 0; effIndex < m_spellInfo->EffectCount; ++effIndex)
        if (!m_spellInfo->Effects[effIndex].IsEffect() || !CheckEffectTarget(target, effIndex))
            effectMask &= ~(1 << effIndex);

    // no effects left
    if (!effectMask)
        return;

    if (checkIfValid)
        if (m_spellInfo->CheckTarget(m_caster, target, IsTriggered() ? true: implicit) != SPELL_CAST_OK)
            return;

    // Check for effect immune skip if immuned
    for (uint32 effIndex = 0; effIndex < m_spellInfo->EffectCount; ++effIndex)
        if (target->IsImmunedToSpellEffect(m_spellInfo, effIndex))
            effectMask &= ~(1 << effIndex);

    if (m_spellInfo->Id == 44614 && effectIndex != 1)
        if (m_caster->HasAura(61205) == (effectIndex == 0))
            return;

    // Quest item spell 'Kill Golden Stonefish'
    if (m_spellInfo->Id == 80962)
    {
        if (!target->ToCreature() || target->ToCreature()->GetEntry() != 43331)
            return;
    }

    uint64 targetGUID = target->GetGUID();

    // Lookup target in already in list
    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (targetGUID == ihit->targetGUID)             // Found in list
        {
            ihit->effectMask |= effectMask;             // Immune effects removed from mask
            ihit->scaleAura = false;
            if (m_auraScaleMask && ihit->effectMask == m_auraScaleMask && m_caster != target)
            {
                SpellInfo const* auraSpell = sSpellMgr->GetSpellInfo(sSpellMgr->GetFirstSpellInChain(m_spellInfo->Id));
                if (uint32(target->getLevel() + 10) >= auraSpell->SpellLevel)
                    ihit->scaleAura = true;
            }
            return;
        }
    }

    // This is new target calculate data for him

    // Get spell hit result on target
    TargetInfo targetInfo;
    targetInfo.targetGUID = targetGUID;                         // Store target GUID
    targetInfo.effectMask = effectMask;                         // Store all effects not immune
    targetInfo.processed  = false;                              // Effects not apply on target
    targetInfo.alive      = target->isAlive();
    targetInfo.damage     = 0;
    targetInfo.crit       = false;
    targetInfo.scaleAura  = false;
    targetInfo.timeDelay  = 0;

    if (m_auraScaleMask && targetInfo.effectMask == m_auraScaleMask && m_caster != target)
    {
        SpellInfo const* auraSpell = sSpellMgr->GetSpellInfo(sSpellMgr->GetFirstSpellInChain(m_spellInfo->Id));
        if (uint32(target->getLevel() + 10) >= auraSpell->SpellLevel)
            targetInfo.scaleAura = true;
    }

    // Calculate hit result
    if (m_originalCaster)
    {
        targetInfo.missCondition = m_originalCaster->SpellHitResult(target, m_spellInfo, m_canReflect);
        if (m_skipCheck && targetInfo.missCondition != SPELL_MISS_IMMUNE)
            targetInfo.missCondition = SPELL_MISS_NONE;
    }
    else
        targetInfo.missCondition = SPELL_MISS_EVADE; //SPELL_MISS_NONE;

    // Spell have speed - need calculate incoming time
    // Incoming time is zero for self casts. At least I think so.
    if (m_spellInfo->Effects[0].Effect == SPELL_EFFECT_KNOCK_BACK)
    {
        m_delayMoment = 1;
        targetInfo.timeDelay = 0;
    }
    else if (m_spellInfo->Speed > 0.0f && m_caster != target && m_spellInfo->Speed != 12345)
    {
        // calculate spell incoming interval
        // TODO: this is a hack
        float dist = m_caster->GetDistance(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());

        if (dist < 5.0f)
            dist = 5.0f;

        if (!(m_spellInfo->AttributesEx9 & SPELL_ATTR9_SPECIAL_DELAY_CALCULATION))
            targetInfo.timeDelay = uint64(floor(dist / m_spellInfo->Speed * 1000.0f));
        else
            targetInfo.timeDelay = uint64(m_spellInfo->Speed * 1000.0f);

        // Calculate minimum incoming time
        if (m_delayMoment == 0 || m_delayMoment > targetInfo.timeDelay)
            m_delayMoment = targetInfo.timeDelay;
    }
    else if ((m_caster->IsPlayer() || m_caster->ToCreature()->isPet()) && m_caster != target)
    {
        if (!IsTriggered() && m_spellInfo->_IsCrowdControl(0, false))
        {
            targetInfo.timeDelay = 100LL;
            m_delayMoment = 100LL;
        }
        if (m_spellInfo->IsPositive() && (!IsTriggered() || m_spellInfo->SpellIconID == 156) && m_spellInfo->Targets != 0x40 && m_spellInfo->Id != 64382)
        {
            switch (m_spellInfo->Effects[0].Effect)
            {
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                case SPELL_EFFECT_APPLY_AURA:
                case SPELL_EFFECT_APPLY_AURA_ON_PET:
                case SPELL_EFFECT_POWER_BURN:
                case SPELL_EFFECT_DISPEL:
                {
                    targetInfo.timeDelay = 100LL;
                    m_delayMoment = 100LL;
                }
                default:
                    break;
            }
            // Shadowstep
            if (m_spellInfo->Id == 36563)
            {
                targetInfo.timeDelay = 100LL;
                m_delayMoment = 100LL;
            }
        }
    }
    // Removing Death Grip cooldown
    else if (m_spellInfo->Id == 90289)
    {
        targetInfo.timeDelay = 100LL;
        m_delayMoment = 100LL;
    }
    // Misdirection
    else if (m_spellInfo->Id == 35079 || m_spellInfo->Id == 57934)
    {
        targetInfo.timeDelay = 100LL;
        m_delayMoment = 100LL;
    }
    /// Apply delay for CC spells here, can be easily tweaked.
    else if (m_spellInfo->Speed == 12345)
    {
        targetInfo.timeDelay = 150LL;
        m_delayMoment = 150LL;
    }
    else
        targetInfo.timeDelay = 0LL;

    // If target reflect spell back to caster
    if (targetInfo.missCondition == SPELL_MISS_REFLECT)
    {
        // process reflect removal (not delayed)
        if (!targetInfo.timeDelay)
        {
            m_caster->ProcDamageAndSpell(target, PROC_FLAG_NONE, PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG, PROC_EX_REFLECT, 1, 0, WeaponAttackType::BaseAttack, m_spellInfo);
            if (m_spellInfo->Id == 2136) // hack to trigger impact in reflect
            {
                m_caster->ProcDamageAndSpell(m_caster, PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG,
                    PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG, PROC_EX_NORMAL_HIT, 1, 0, WeaponAttackType::BaseAttack, m_spellInfo);
            }
        }
        // Calculate reflected spell result on caster
        targetInfo.reflectResult = m_caster->SpellHitResult(m_caster, m_spellInfo, m_canReflect);

        if (targetInfo.reflectResult == SPELL_MISS_REFLECT)     // Impossible reflect again, so simply deflect spell
            targetInfo.reflectResult = SPELL_MISS_PARRY;

        // Increase time interval for reflected spells by 1.5
        targetInfo.timeDelay += targetInfo.timeDelay >> 1;
    }
    else
        targetInfo.reflectResult = SPELL_MISS_NONE;

    // Add target to list
    m_UniqueTargetInfo.push_back(targetInfo);
}

void Spell::AddGOTarget(GameObject* go, uint32 effectMask)
{
    for (uint32 effIndex = 0; effIndex < m_spellInfo->EffectCount; ++effIndex)
    {
        if (!m_spellInfo->Effects[effIndex].IsEffect())
            effectMask &= ~(1 << effIndex);
        else
        {
            switch (m_spellInfo->Effects[effIndex].Effect)
            {
                case SPELL_EFFECT_GAMEOBJECT_DAMAGE:
                case SPELL_EFFECT_GAMEOBJECT_REPAIR:
                case SPELL_EFFECT_GAMEOBJECT_SET_DESTRUCTION_STATE:
                    if (go->GetGoType() != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
                        effectMask &= ~(1 << effIndex);
                    break;
                default:
                    break;
            }
        }
    }

    if (!effectMask)
        return;

    uint64 targetGUID = go->GetGUID();

    // Lookup target in already in list
    for (std::list<GOTargetInfo>::iterator ihit = m_UniqueGOTargetInfo.begin(); ihit != m_UniqueGOTargetInfo.end(); ++ihit)
    {
        if (targetGUID == ihit->targetGUID)                 // Found in list
        {
            ihit->effectMask |= effectMask;                 // Add only effect mask
            return;
        }
    }

    // This is new target calculate data for him

    GOTargetInfo target;
    target.targetGUID = targetGUID;
    target.effectMask = effectMask;
    target.processed  = false;                              // Effects not apply on target

    // Spell have speed - need calculate incoming time
    if (m_spellInfo->Speed > 0.0f)
    {
        // calculate spell incoming interval
        float dist = m_caster->GetDistance(go->GetPositionX(), go->GetPositionY(), go->GetPositionZ());
        if (dist < 5.0f)
            dist = 5.0f;
       if (!(m_spellInfo->AttributesEx9 & SPELL_ATTR9_SPECIAL_DELAY_CALCULATION))
           target.timeDelay = uint64(floor(dist / m_spellInfo->Speed * 1000.0f));
       else
           target.timeDelay = uint64(m_spellInfo->Speed * 1000.0f);

        if (m_delayMoment == 0 || m_delayMoment > target.timeDelay)
            m_delayMoment = target.timeDelay;
    }
    else
        target.timeDelay = 0LL;

    // Add target to list
    m_UniqueGOTargetInfo.push_back(target);
}

void Spell::AddItemTarget(Item* item, uint32 effectMask)
{
    for (uint32 effIndex = 0; effIndex < m_spellInfo->EffectCount; ++effIndex)
        if (!m_spellInfo->Effects[effIndex].IsEffect())
            effectMask &= ~(1 << effIndex);

    // no effects left
    if (!effectMask)
        return;

    // Lookup target in already in list
    for (std::list<ItemTargetInfo>::iterator ihit = m_UniqueItemInfo.begin(); ihit != m_UniqueItemInfo.end(); ++ihit)
    {
        if (item == ihit->item)                            // Found in list
        {
            ihit->effectMask |= effectMask;                // Add only effect mask
            return;
        }
    }

    // This is new target add data
    ItemTargetInfo target;
    target.item       = item;
    target.effectMask = effectMask;

    m_UniqueItemInfo.push_back(target);
}

void Spell::AddAreaTriggerTarget(AreaTrigger* p_AreaTrigger, uint32 p_EffectMask)
{
    for (uint8 l_EffectIndex = 0; l_EffectIndex < m_spellInfo->EffectCount; ++l_EffectIndex)
    {
        if (!m_spellInfo->Effects[l_EffectIndex].IsEffect())
            p_EffectMask &= ~(1 << l_EffectIndex);
    }

    if (!p_EffectMask)
        return;

    uint64 l_TargetGUID = p_AreaTrigger->GetGUID();

    // Lookup target in already in list
    for (std::list<AreaTriggerTargetInfo>::iterator l_Iterator = m_UniqueAreaTriggerTargetInfo.begin(); l_Iterator != m_UniqueAreaTriggerTargetInfo.end(); ++l_Iterator)
    {
        if (l_TargetGUID == l_Iterator->targetGUID)                 // Found in list
        {
            l_Iterator->effectMask |= p_EffectMask;                 // Add only effect mask
            return;
        }
    }

    AreaTriggerTargetInfo l_AreaTriggerTargetInfo;
    l_AreaTriggerTargetInfo.targetGUID = p_AreaTrigger->GetGUID();
    l_AreaTriggerTargetInfo.processed  = false;
    l_AreaTriggerTargetInfo.effectMask = p_EffectMask;
    l_AreaTriggerTargetInfo.timeDelay  = 0.0f;

    m_UniqueAreaTriggerTargetInfo.push_back(l_AreaTriggerTargetInfo);
}

void Spell::AddDestTarget(SpellDestination const& dest, uint32 effIndex)
{
    m_destTargets[effIndex] = dest;
}

void Spell::DoAllEffectOnTarget(TargetInfo* target)
{
    if (!target || target->processed)
        return;

    target->processed = true;                               // Target checked in apply effects procedure

    // Get mask of effects for target
    uint32 mask = target->effectMask;

    Unit* unit = m_caster->GetGUID() == target->targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, target->targetGUID);
    if (!unit)
    {
        uint32 farMask = 0;
        // create far target mask
        for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
            if (m_spellInfo->Effects[i].IsFarUnitTargetEffect())
                if ((1 << i) & mask)
                    farMask |= (1 << i);

        if (!farMask)
            return;
        // find unit in world
        unit = ObjectAccessor::FindUnit(target->targetGUID);
        if (!unit)
            return;

        // do far effects on the unit
        // can't use default call because of threading, do stuff as fast as possible
        for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
            if (farMask & (1 << i))
                HandleEffects(unit, nullptr, nullptr, nullptr, i, SPELL_EFFECT_HANDLE_HIT_TARGET);
        return;
    }

    if (unit->isAlive() != target->alive)
        return;

    if (getState() == SPELL_STATE_DELAYED && !m_spellInfo->IsPositive() && (getMSTime() - target->timeDelay) <= unit->m_lastSanctuaryTime)
        return;                                             // No missinfo in that case

    // Some spells should remove Camouflage after hit (traps, some spells that have casting time)
    if (target->targetGUID != m_caster->GetGUID() && m_spellInfo && m_spellInfo->IsBreakingCamouflageAfterHit())
    {
        if (TempSummon* summon = m_caster->ToTempSummon())
        {
            if (Unit* owner = summon->GetSummoner())
                owner->RemoveAurasByType(SPELL_AURA_MOD_CAMOUFLAGE);
        }
        else
            m_caster->RemoveAurasByType(SPELL_AURA_MOD_CAMOUFLAGE);
    }

    // Get original caster (if exist) and calculate damage/healing from him data
    Unit* caster = m_originalCaster ? m_originalCaster : m_caster;

    // Skip if m_originalCaster not avaiable
    if (!caster)
        return;

    SpellMissInfo missInfo = target->missCondition;

    // Need init unitTarget by default unit (can changed in code on reflect)
    // Or on missInfo != SPELL_MISS_NONE unitTarget undefined (but need in trigger subsystem)
    unitTarget = unit;

    // Reset damage/healing counter
    m_damage          = target->damage;
    m_healing         = -target->damage;
    m_final_damage    = 0;
    m_absorbed_damage = 0;

    // Fill base trigger info
    uint32 procAttacker = m_procAttacker;
    uint32 procVictim   = m_procVictim;
    uint32 procEx = m_procEx;

    m_spellAura = nullptr; // Set aura to null for every target-make sure that pointer is not used for unit without aura applied

                            //Spells with this flag cannot trigger if effect is casted on self
    bool canEffectTrigger = !(m_spellInfo->AttributesEx3 & SPELL_ATTR3_CANT_TRIGGER_PROC) && unitTarget->CanProc() && (missInfo == SPELL_MISS_IMMUNE2 || CanExecuteTriggersOnHit(mask)) && CanProcOnTarget(unitTarget);
    Unit* spellHitTarget = NULL;

    if (missInfo == SPELL_MISS_NONE)                        // In case spell hit target, do all effect on that target
        spellHitTarget = unit;
    else if (missInfo == SPELL_MISS_REFLECT)                // In case spell reflect from target, do all effect on caster (if hit)
    {
        if (target->reflectResult == SPELL_MISS_NONE)       // If reflected spell hit caster -> do all effect on him
        {
            spellHitTarget = m_caster;
            if (m_caster->GetTypeId() == TYPEID_UNIT)
                m_caster->ToCreature()->LowerPlayerDamageReq(target->damage);
        }
    }

    if (missInfo != SpellMissInfo::SPELL_MISS_NONE)
    {
        if (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->IsAIEnabled)
            m_caster->ToCreature()->AI()->SpellMissTarget(unit, m_spellInfo, missInfo);
    }

    /// Custom WoD Script - Shadowmeld
    if (unitTarget->HasAura(58984) && !m_caster->IsFriendlyTo(unitTarget))
        return;

    /// Custom WoD Script - Death from Above should give immunity to all spells while rogue is in jump effect
    if (unitTarget->GetGUID() != m_caster->GetGUID() && unitTarget->getClass() == CLASS_ROGUE && unitTarget->getLevel() == 100 && unitTarget->HasAura(152150))
        return;

    if (spellHitTarget)
    {
        SpellMissInfo missInfo2 = DoSpellHitOnUnit(spellHitTarget, mask, target->scaleAura);
        if (missInfo2 != SPELL_MISS_NONE)
        {
            if (missInfo2 != SPELL_MISS_MISS)
                m_caster->SendSpellMiss(unit, m_spellInfo->Id, missInfo2);
            m_damage = 0;
            spellHitTarget = NULL;
        }
    }

    // Do not take combo points on dodge and miss
    if (missInfo != SPELL_MISS_NONE && m_needComboPoints &&
            m_targets.GetUnitTargetGUID() == target->targetGUID)
    {
        m_needComboPoints = false;
        // Restore spell mods for a miss/dodge/parry Cold Blood
        // TODO: check how broad this rule should be
        if (m_caster->IsPlayer() && (missInfo == SPELL_MISS_MISS ||
                missInfo == SPELL_MISS_DODGE || missInfo == SPELL_MISS_PARRY))
            m_caster->ToPlayer()->RestoreSpellMods(this, 14177);
    }

    // Trigger info was not filled in spell::preparedatafortriggersystem - we do it now
    if (canEffectTrigger && !procAttacker && !procVictim)
    {
        bool positive = true;
        if (m_damage > 0)
            positive = false;
        else if (!m_healing)
        {
            for (uint8 i = 0; i< m_spellInfo->EffectCount; ++i)
                // If at least one effect negative spell is negative hit
                if (mask & (1<<i) && !m_spellInfo->IsPositiveEffect(i))
                {
                    positive = false;
                    break;
                }
        }
        switch (m_spellInfo->DmgClass)
        {
            case SPELL_DAMAGE_CLASS_MAGIC:
                if (positive)
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS;
                }
                else
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG;
                }
            break;
            case SPELL_DAMAGE_CLASS_NONE:
                if (positive)
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS;
                }
                else
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_NEG;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_NEG;
                }
            break;
        }
    }

    if (caster->IsPlayer() && m_spellInfo->CanTriggerPoisonAdditional())
        caster->ToPlayer()->CastItemCombatSpell(unitTarget, m_attackType, PROC_FLAG_TAKEN_DAMAGE, procEx);

    // All calculated do it!
    // Do healing and triggers
    if (m_healing > 0)
    {
        bool crit = target->crit;
        uint32 addhealth = m_healing;
        if (crit)
        {
            procEx |= PROC_EX_CRITICAL_HIT;
            addhealth = caster->SpellCriticalHealingBonus(m_spellInfo, addhealth, unitTarget);
        }
        else
            procEx |= PROC_EX_NORMAL_HIT;

        m_healing = addhealth;
        CallScriptOnHitHandlers();
        addhealth = m_healing;

        int32 gain = caster->HealBySpell(unitTarget, m_spellInfo, addhealth, crit);
        unitTarget->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, m_spellInfo);
        m_healing = gain;

        // Do triggers for unit (reflect triggers passed on hit phase for correct drop charge)
        if (canEffectTrigger && missInfo != SPELL_MISS_REFLECT)
            caster->ProcDamageAndSpell(unitTarget, procAttacker, procVictim, procEx, addhealth, 0, m_attackType, m_spellInfo, m_triggeredByAuraSpell);
    }
    // Do damage and triggers
    else if (m_damage > 0)
    {
        CallScriptOnHitHandlers();

        // Fill base damage struct (unitTarget - is real spell target)
        SpellNonMeleeDamage damageInfo(caster, unitTarget, m_spellInfo->Id, m_spellSchoolMask);
        damageInfo.damage = m_damage;

        // Add bonuses and fill damageInfo struct
        caster->CalculateSpellDamageTaken(&damageInfo, m_damage, m_spellInfo, m_attackType,  target->crit);
        caster->DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);

        // Send log damage message to client
        if (missInfo == SPELL_MISS_REFLECT)
            damageInfo.attacker = unit;
        caster->SendSpellNonMeleeDamageLog(&damageInfo);

        procEx |= createProcExtendMask(&damageInfo, missInfo);
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;

        // Do triggers for unit (reflect triggers passed on hit phase for correct drop charge)
        if (canEffectTrigger && missInfo != SPELL_MISS_REFLECT)
        {
            caster->ProcDamageAndSpell(unitTarget, procAttacker, procVictim, procEx, damageInfo.damage, damageInfo.absorb, m_attackType, m_spellInfo, m_triggeredByAuraSpell);
            if (caster->IsPlayer() && (m_spellInfo->Attributes & SPELL_ATTR0_STOP_ATTACK_TARGET) == 0 &&
               (m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_MELEE || m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_RANGED))
                caster->ToPlayer()->CastItemCombatSpell(unitTarget, m_attackType, procVictim, procEx);
        }

        m_damage = damageInfo.damage;

        caster->DealSpellDamage(&damageInfo, true);

        // Used in spell scripts
        m_final_damage = damageInfo.damage;
        m_absorbed_damage = damageInfo.absorb;
    }
    // Passive spell hits/misses or active spells only misses (only triggers)
    else
    {
        CallScriptOnHitHandlers();

        // Fill base damage struct (unitTarget - is real spell target)
        SpellNonMeleeDamage damageInfo(caster, unitTarget, m_spellInfo->Id, m_spellSchoolMask);
        procEx |= createProcExtendMask(&damageInfo, missInfo);
        // Do triggers for unit (reflect triggers passed on hit phase for correct drop charge)
        if (canEffectTrigger && missInfo != SPELL_MISS_REFLECT)
            caster->ProcDamageAndSpell(unit, procAttacker, procVictim, procEx, 0, 0, m_attackType, m_spellInfo, m_triggeredByAuraSpell);

        // Failed Pickpocket, reveal rogue
        if (missInfo == SPELL_MISS_RESIST && m_spellInfo->AttributesCu & SPELL_ATTR0_CU_PICKPOCKET && unitTarget->GetTypeId() == TYPEID_UNIT)
        {
            m_caster->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TALK);
            if (unitTarget->ToCreature()->IsAIEnabled)
                unitTarget->ToCreature()->AI()->AttackStart(m_caster);
        }
    }

    if (!m_spellInfo->IsIgnoringCombat() && missInfo != SPELL_MISS_EVADE && !m_caster->IsFriendlyTo(unit) && (!m_spellInfo->IsPositive() || m_spellInfo->HasEffect(SPELL_EFFECT_DISPEL)))
    {
        m_caster->CombatStart(unit, !(m_spellInfo->AttributesEx3 & SPELL_ATTR3_NO_INITIAL_AGGRO));

        if (m_spellInfo->AttributesCu & SPELL_ATTR0_CU_AURA_CC)
            if (!unit->IsStandState())
                unit->SetStandState(UNIT_STAND_STATE_STAND);
    }

    if (spellHitTarget)
    {
        //AI functions
        if (spellHitTarget->GetTypeId() == TYPEID_UNIT)
        {
            if (spellHitTarget->ToCreature()->IsAIEnabled)
                spellHitTarget->ToCreature()->AI()->SpellHit(m_caster, m_spellInfo);

            // cast at creature (or GO) quest objectives update at successful cast finished (+channel finished)
            // ignore pets or autorepeat/melee casts for speed (not exist quest for spells (hm...)
            if (m_originalCaster && m_originalCaster->IsControlledByPlayer() && !spellHitTarget->ToCreature()->isPet() && !IsAutoRepeat() && !IsNextMeleeSwingSpell() && !IsChannelActive())
                if (Player* p = m_originalCaster->GetCharmerOrOwnerPlayerOrPlayerItself())
                    p->CastedCreatureOrGO(spellHitTarget->GetEntry(), spellHitTarget->GetGUID(), m_spellInfo->Id);
        }

        if (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->IsAIEnabled)
            m_caster->ToCreature()->AI()->SpellHitTarget(spellHitTarget, m_spellInfo);

        // Needs to be called after dealing damage/healing to not remove breaking on damage auras
        DoTriggersOnSpellHit(spellHitTarget, mask);

        // if target is fallged for pvp also flag caster if a player
        if (unit->IsPvP() && m_caster->IsPlayer())
            m_caster->ToPlayer()->UpdatePvP(true);

        CallScriptAfterHitHandlers();
    }
}

SpellMissInfo Spell::DoSpellHitOnUnit(Unit* unit, uint32 effectMask, bool scaleAura)
{
    if (!unit || !effectMask)
        return SPELL_MISS_EVADE;

    MeleeHitOutcome l_HitResult = MELEE_HIT_NORMAL;
    SpellMissInfo l_SpellResult = SPELL_MISS_NONE;
    if (unit->ToCreature() && unit->IsAIEnabled)
        unit->ToCreature()->GetAI()->CheckHitResult(l_HitResult, l_SpellResult, m_caster, m_spellInfo);

    if (l_SpellResult != SPELL_MISS_NONE)
        return l_SpellResult;

    // For delayed spells immunity may be applied between missile launch and hit - check immunity for that case
    if (m_spellInfo->Speed && (unit->IsImmunedToDamage(m_spellInfo) || unit->IsImmunedToSpell(m_spellInfo)))
        return SPELL_MISS_IMMUNE;

    // Hack fix for Deterrence and charge stun effect
    if ((m_spellInfo->Id == 105771 || m_spellInfo->Id == 7922) && unit->HasAura(19263))
        return SPELL_MISS_MISS;

    /// Hack fix for Cloak of Shadows (just Blood Plague and Censure (DoT) can hit to Cloak of Shadows)
    if (!m_spellInfo->IsPositive() &&(m_spellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC) && unit->HasAura(31224) && m_spellInfo->Id != 59879 && m_spellInfo->Id != 31803 && m_spellInfo->Id != 157695)
        return SPELL_MISS_MISS;

    // disable effects to which unit is immune
    SpellMissInfo returnVal = SPELL_MISS_IMMUNE;
    for (uint32 effectNumber = 0; effectNumber < m_spellInfo->EffectCount; ++effectNumber)
        if (effectMask & (1 << effectNumber))
            if (unit->IsImmunedToSpellEffect(m_spellInfo, effectNumber))
                effectMask &= ~(1 << effectNumber);

    if (!effectMask)
        return returnVal;

    PrepareScriptHitHandlers();
    CallScriptBeforeHitHandlers();

    if (unit->IsPlayer())
    {
        unit->ToPlayer()->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET, m_spellInfo->Id);
        unit->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, m_spellInfo->Id, 0, 0, m_caster);
        unit->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2, m_spellInfo->Id, 0, 0, m_caster);
    }

    if (m_caster->IsPlayer())
    {
        m_caster->ToPlayer()->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_SPELL_CASTER, m_spellInfo->Id);
        m_caster->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2, m_spellInfo->Id, 0, 0, unit);
    }

    if (m_caster != unit)
    {
        // Recheck  UNIT_FLAG_NON_ATTACKABLE for delayed spells
        if (m_spellInfo->Speed > 0.0f && unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) && unit->GetCharmerOrOwnerGUID() != m_caster->GetGUID())
            return SPELL_MISS_EVADE;

        if (m_caster->_IsValidAttackTarget(unit, m_spellInfo))
        {
            if (m_spellInfo->IsBreakingStealth(m_caster) && !(m_procEx & PROC_EX_INTERNAL_MULTISTRIKE))
            {
                unit->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_HITBYSPELL);
                //TODO: This is a hack. But we do not know what types of stealth should be interrupted by CC
                if (m_spellInfo->HasCustomAttribute(SPELL_ATTR0_CU_AURA_CC) && unit->IsControlledByPlayer())
                    unit->RemoveAurasByType(SPELL_AURA_MOD_STEALTH, 0, NULL, 131361);
            }
            if (m_spellInfo->HasCustomAttribute(SPELL_ATTR0_CU_BINARY) && !m_spellInfo->IsChanneled())
                if (m_originalCaster && m_originalCaster->IsSpellResisted(unit, m_spellSchoolMask, m_spellInfo))
                    return SPELL_MISS_RESIST;
        }
        else if (m_caster->IsFriendlyTo(unit))
        {
            // for delayed spells ignore negative spells (after duel end) for friendly targets
            // TODO: this cause soul transfer bugged
            /// Handle custom flag SPELL_ATTR0_CU_CAN_BE_CASTED_ON_ALLIES, some spells are negative but can be casted on allies
            if (m_spellInfo->Speed > 0.0f && unit->IsPlayer() && !m_spellInfo->IsPositive() && !(m_spellInfo->AttributesCu & SpellCustomAttributes::SPELL_ATTR0_CU_CAN_BE_CASTED_ON_ALLIES))
                return SPELL_MISS_EVADE;

            // assisting case, healing and resurrection
            if (unit->HasUnitState(UNIT_STATE_ATTACK_PLAYER))
            {
                m_caster->SetContestedPvP();
                if (m_caster->IsPlayer())
                    m_caster->ToPlayer()->UpdatePvP(true);
            }
            if (unit->isInCombat() && !(m_spellInfo->AttributesEx3 & SPELL_ATTR3_NO_INITIAL_AGGRO) && !m_spellInfo->IsIgnoringCombat())
            {
                m_caster->SetInCombatState(unit->GetCombatTimer() > 0, unit);
                unit->getHostileRefManager().threatAssist(m_caster, 0.0f);
            }
        }
    }
    else if (!m_spellInfo->IsPositive())
    {
        if (m_spellInfo->HasCustomAttribute(SPELL_ATTR0_CU_BINARY) && !m_spellInfo->IsChanneled())
            if (m_originalCaster && m_originalCaster->IsSpellResisted(unit, m_spellSchoolMask, m_spellInfo))
                return SPELL_MISS_RESIST;
    }

    // Get Data Needed for Diminishing Returns, some effects may have multiple auras, so this must be done on spell hit, not aura add
    m_diminishGroup = GetDiminishingReturnsGroupForSpell(m_spellInfo);
    if (m_diminishGroup)
    {
        m_diminishLevel = unit->GetDiminishing(m_diminishGroup);
        DiminishingReturnsType type = GetDiminishingReturnsGroupType(m_diminishGroup);
        // Increase Diminishing on unit, current informations for actually casts will use values above
        if ((type == DRTYPE_PLAYER &&
            (unit->GetCharmerOrOwnerPlayerOrPlayerItself() || (unit->GetTypeId() == TYPEID_UNIT && unit->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_ALL_DIMINISH))) ||
            type == DRTYPE_ALL)
        {
            unit->IncrDiminishing(m_diminishGroup);

            /// Hack Fix Wod - Hunter WoD PvP Beast Mastery 4P Bonus
            /// Pet and owner are sharing same diminishing return
            if (unit->isPet() && unit->GetOwner() && unit->GetOwner()->HasAura(171478))
                unit->GetOwner()->IncrDiminishing(m_diminishGroup);
            else if (unit->HasAura(171478) && unit->IsPlayer() && unit->ToPlayer()->GetPet())
                unit->ToPlayer()->GetPet()->IncrDiminishing(m_diminishGroup);
        }
    }

    uint32 aura_effmask = 0;
    for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
        if (effectMask & (1 << i) && m_spellInfo->Effects[i].IsUnitOwnedAuraEffect())
            aura_effmask |= 1 << i;

    if (m_spellInfo->Id == 879) // Exorcism
    {
        if (!m_caster->HasAura(54934)) // Glyph of Exorcism
            aura_effmask &= ~(1 << EFFECT_1);
    }

    if (aura_effmask)
    {
        // Select rank for aura with level requirements only in specific cases
        // Unit has to be target only of aura effect, both caster and target have to be players, target has to be other than unit target
        SpellInfo const* aurSpellInfo = m_spellInfo;
        int32 basePoints[32];
        if (scaleAura)
        {
            aurSpellInfo = m_spellInfo->GetAuraRankForLevel(unitTarget->getLevel());
            ASSERT(aurSpellInfo);
            for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
            {
                basePoints[i] = aurSpellInfo->Effects[i].BasePoints;
                if (m_spellInfo->Effects[i].Effect != aurSpellInfo->Effects[i].Effect)
                {
                    aurSpellInfo = m_spellInfo;
                    break;
                }
            }
        }

        if (m_originalCaster)
        {
            bool refresh = false;
            m_spellAura = Aura::TryRefreshStackOrCreate(aurSpellInfo, effectMask, unit,
                m_originalCaster, (aurSpellInfo == m_spellInfo)? &m_spellValue->EffectBasePoints[0] : &basePoints[0], m_CastItem, 0, &refresh);
            if (m_spellAura)
            {
                // Set aura stack amount to desired value
                if (m_spellValue->AuraStackAmount > 1)
                {
                    if (!refresh)
                        m_spellAura->SetStackAmount(m_spellValue->AuraStackAmount);
                    else
                        m_spellAura->ModStackAmount(m_spellValue->AuraStackAmount);
                }

                // Now Reduce spell duration using data received at spell hit
                int32 duration = m_spellAura->GetMaxDuration();
                int32 limitduration = GetDiminishingReturnsLimitDuration(aurSpellInfo);
                float diminishMod = unit->ApplyDiminishingToDuration(m_diminishGroup, duration, m_originalCaster, m_diminishLevel, limitduration);

                // unit is immune to aura if it was diminished to 0 duration
                if (diminishMod == 0.0f)
                {
                    m_spellAura->Remove();
                    bool found = false;
                    for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
                    {
                        if (effectMask & (1 << i) && m_spellInfo->Effects[i].Effect != SPELL_EFFECT_APPLY_AURA && m_spellInfo->Effects[i].Effect != SPELL_EFFECT_APPLY_AURA_ON_PET)
                            found = true;
                    }

                    if (!found)
                        return SPELL_MISS_IMMUNE;
                }
                else
                {
                    UnitAura* unitAura = (UnitAura*)m_spellAura;
                    unitAura->SetDiminishGroup(m_diminishGroup);

                    bool positive = m_spellAura->GetSpellInfo()->IsPositive();
                    if (AuraApplication* aurApp = m_spellAura->GetApplicationOfTarget(m_originalCaster->GetGUID()))
                        positive = aurApp->IsPositive();

                    duration = m_originalCaster->ModSpellDuration(aurSpellInfo, unit, duration, positive, effectMask);

                    if (duration > 0)
                    {
                        // Haste modifies duration of channeled spells
                        if (m_spellInfo->IsChanneled())
                            m_originalCaster->ModSpellCastTime(aurSpellInfo, duration, this);
                        /// If this aura not affected by new wod aura system, we should change amplitude according to amount of haste
                        else if (GetSpellInfo() && !GetSpellInfo()->IsAffectedByWodAuraSystem() && m_spellInfo->AttributesEx5 & SPELL_ATTR5_HASTE_AFFECT_DURATION)
                        {
                            int32 l_OriginalDuration = duration;
                            float l_HastePct = m_originalCaster->GetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED);
                            duration = 0;
                            for (uint8 l_I = 0; l_I < SpellEffIndex::MAX_EFFECTS; ++l_I)
                            {
                                if (AuraEffect const* l_Effect = m_spellAura->GetEffect(l_I))
                                {
                                    if (int32 l_Amplitude = l_Effect->GetAmplitude())  // amplitude is hastened by UNIT_FIELD_MOD_CASTING_SPEED
                                        duration = int32(l_OriginalDuration / (2.0f - l_HastePct));
                                }
                            }

                            // if there is no periodic effect
                            if (!duration)
                                duration = int32(l_OriginalDuration * (2.0f - l_HastePct));
                        }
                    }

                    if (duration != m_spellAura->GetMaxDuration())
                    {
                        bool periodicDamage = false;
                        for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
                            if (m_spellAura->GetEffect(i))
                                if (m_spellAura->GetEffect(i)->GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
                                    periodicDamage = true; ///< periodicDamage is never read 01/18/16
                    }

                    if (duration != m_spellAura->GetMaxDuration())
                    {
                        m_spellAura->SetMaxDuration(duration);
                        m_spellAura->SetDuration(duration);
                    }

                    if (isStolen && (!duration || duration == -1 || duration > 60000))
                    {
                        duration = 60000;
                        m_spellAura->SetMaxDuration(duration);
                        m_spellAura->SetDuration(duration);
                    }

                    m_spellAura->_RegisterForTargets();
                }
            }
        }
    }

    for (uint32 effectNumber = 0; effectNumber < m_spellInfo->EffectCount; ++effectNumber)
        if (effectMask & (1 << effectNumber))
            HandleEffects(unit, nullptr, nullptr, nullptr, effectNumber, SPELL_EFFECT_HANDLE_HIT_TARGET);

    return SPELL_MISS_NONE;
}

void Spell::DoTriggersOnSpellHit(Unit* unit, uint32 effMask)
{
    // Apply additional spell effects to target
    // TODO: move this code to scripts
    // Blizz seems to just apply aura without bothering to cast
    if (m_preCastSpell)
        if (!m_spellInfo->IsWrongPrecastSpell(sSpellMgr->GetSpellInfo(m_preCastSpell)))
            if (sSpellMgr->GetSpellInfo(m_preCastSpell))
                m_caster->AddAura(m_preCastSpell, unit);

    // handle SPELL_AURA_ADD_TARGET_TRIGGER auras
    // this is executed after spell proc spells on target hit
    // spells are triggered for each hit spell target
    // info confirmed with retail sniffs of permafrost and shadow weaving
    if (!m_hitTriggerSpells.empty())
    {
        int _duration = 0;
        for (HitTriggerSpellList::const_iterator i = m_hitTriggerSpells.begin(); i != m_hitTriggerSpells.end(); ++i)
        {
            if (CanExecuteTriggersOnHit(effMask, i->triggeredByAura) && roll_chance_i(i->chance))
            {
                m_caster->CastSpell(unit, i->triggeredSpell, true);

                // SPELL_AURA_ADD_TARGET_TRIGGER auras shouldn't trigger auras without duration
                // set duration of current aura to the triggered spell
                if (i->triggeredSpell->GetDuration() == -1)
                {
                    Aura* triggeredAur = unit->GetAura(i->triggeredSpell->Id, m_caster->GetGUID());
                    if (triggeredAur != nullptr)
                    {
                        // get duration from aura-only once
                        if (!_duration)
                        {
                            Aura* aur = unit->GetAura(m_spellInfo->Id, m_caster->GetGUID());
                            _duration = aur ? aur->GetDuration() : -1;
                        }
                        triggeredAur->SetDuration(_duration);
                    }
                }
            }
        }
    }

    // trigger linked auras remove/apply
    // TODO: remove/cleanup this, as this table is not documented and people are doing stupid things with it
    if (std::vector<int32> const* spellTriggered = sSpellMgr->GetSpellLinked(m_spellInfo->Id + SPELL_LINK_HIT))
    {
        for (std::vector<int32>::const_iterator i = spellTriggered->begin(); i != spellTriggered->end(); ++i)
        {
            if (*i < 0)
                unit->RemoveAurasDueToSpell(-(*i));
            else
                unit->CastSpell(unit, *i, true, 0, nullptr, m_caster->GetGUID());
        }
    }
}

void Spell::DoAllEffectOnTarget(GOTargetInfo* target)
{
    if (target->processed)                                  // Check target
        return;
    target->processed = true;                               // Target checked in apply effects procedure

    uint32 effectMask = target->effectMask;
    if (!effectMask)
        return;

    GameObject* go = m_caster->GetMap()->GetGameObject(target->targetGUID);
    if (!go)
        return;

    PrepareScriptHitHandlers();
    CallScriptBeforeHitHandlers();

    for (uint32 effectNumber = 0; effectNumber < m_spellInfo->EffectCount; ++effectNumber)
        if (effectMask & (1 << effectNumber))
            HandleEffects(nullptr, nullptr, go, nullptr, effectNumber, SPELL_EFFECT_HANDLE_HIT_TARGET);

    CallScriptOnHitHandlers();

    // cast at creature (or GO) quest objectives update at successful cast finished (+channel finished)
    // ignore autorepeat/melee casts for speed (not exist quest for spells (hm...)
    if (m_originalCaster && m_originalCaster->IsControlledByPlayer() && !IsAutoRepeat() && !IsNextMeleeSwingSpell() && !IsChannelActive())
        if (Player* p = m_originalCaster->GetCharmerOrOwnerPlayerOrPlayerItself())
            p->CastedCreatureOrGO(go->GetEntry(), go->GetGUID(), m_spellInfo->Id);
    CallScriptAfterHitHandlers();
}

void Spell::DoAllEffectOnTarget(ItemTargetInfo* target)
{
    uint32 effectMask = target->effectMask;
    if (!target->item || !effectMask)
        return;

    PrepareScriptHitHandlers();
    CallScriptBeforeHitHandlers();

    for (uint32 effectNumber = 0; effectNumber < m_spellInfo->EffectCount; ++effectNumber)
        if (effectMask & (1 << effectNumber))
            HandleEffects(nullptr, target->item, nullptr, nullptr, effectNumber, SPELL_EFFECT_HANDLE_HIT_TARGET);

    CallScriptOnHitHandlers();

    CallScriptAfterHitHandlers();
}

void Spell::DoAllEffectOnTarget(AreaTriggerTargetInfo* p_Target)
{
    uint32 effectMask = p_Target->effectMask;
    if (!p_Target->targetGUID || !effectMask)
        return;

    AreaTrigger* l_AreaTrigger = sObjectAccessor->GetAreaTrigger(*m_caster, p_Target->targetGUID);
    if (l_AreaTrigger == nullptr)
        return;

    PrepareScriptHitHandlers();
    CallScriptBeforeHitHandlers();

    for (uint32 effectNumber = 0; effectNumber < m_spellInfo->EffectCount; ++effectNumber)
    {
        if (effectMask & (1 << effectNumber))
            HandleEffects(nullptr, nullptr, nullptr, l_AreaTrigger, effectNumber, SPELL_EFFECT_HANDLE_HIT_TARGET);
    }

    CallScriptOnHitHandlers();
    CallScriptAfterHitHandlers();
}

bool Spell::UpdateChanneledTargetList()
{
    // Not need check return true
    if (m_channelTargetEffectMask == 0)
        return true;

    uint32 channelTargetEffectMask = m_channelTargetEffectMask;
    uint32 channelAuraMask = 0;
    for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
    {
        if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AURA || m_spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET)
            channelAuraMask |= 1 << i;
    }

    channelAuraMask &= channelTargetEffectMask;

    float range = 0;
    if (channelAuraMask)
    {
        range = m_spellInfo->GetMaxRange(m_spellInfo->IsPositive());
        if (Player* modOwner = m_caster->GetSpellModOwner())
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_RANGE, range, this);
    }

    for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition == SPELL_MISS_NONE && (channelTargetEffectMask & ihit->effectMask))
        {
            Unit* unit = m_caster->GetGUID() == ihit->targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID);

            if (!unit)
                continue;

            if (IsValidDeadOrAliveTarget(unit))
            {
                if (channelAuraMask & ihit->effectMask)
                {
                    if (AuraApplication * aurApp = unit->GetAuraApplication(m_spellInfo->Id, m_originalCasterGUID))
                    {
                        if (m_caster != unit && !m_caster->IsWithinDistInMap(unit, range))
                        {
                            ihit->effectMask &= ~aurApp->GetEffectMask();
                            unit->RemoveAura(aurApp);
                            continue;
                        }
                    }
                    else // aura is dispelled
                        continue;
                }

                channelTargetEffectMask &= ~ihit->effectMask;   // remove from need alive mask effect that have alive target
            }
        }
    }

    // is all effects from m_needAliveTargetMask have alive targets
    return channelTargetEffectMask == 0;
}

void Spell::prepare(SpellCastTargets const* targets, AuraEffect const* triggeredByAura)
{
    if (m_CastItem)
        m_castItemGUID = m_CastItem->GetGUID();
    else
        m_castItemGUID = 0;

    InitExplicitTargets(*targets);

    // Fill aura scaling information
    if (m_caster->IsControlledByPlayer() && !m_spellInfo->IsPassive() && m_spellInfo->SpellLevel && !m_spellInfo->IsChanneled() && !(_triggeredCastFlags & TRIGGERED_IGNORE_AURA_SCALING))
    {
        for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
        {
            if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AURA || m_spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET)
            {
                // Change aura with ranks only if basepoints are taken from spellInfo and aura is positive
                if (m_spellInfo->IsPositiveEffect(i))
                {
                    m_auraScaleMask |= (1 << i);
                    if (m_spellValue->EffectBasePoints[i] != m_spellInfo->Effects[i].BasePoints)
                    {
                        m_auraScaleMask = 0;
                        break;
                    }
                }
            }
        }
    }

    m_spellState = SPELL_STATE_PREPARING;

    if (triggeredByAura)
        m_triggeredByAuraSpell  = triggeredByAura->GetSpellInfo();

    // create and add update event for this spell
    SpellEvent* Event = new SpellEvent(this);
    m_caster->m_Events.AddEvent(Event, m_caster->m_Events.CalculateTime(1));

    //Prevent casting at cast another spell (ServerSide check)
    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CAST_IN_PROGRESS) && m_caster->IsNonMeleeSpellCasted(false, true, true) && m_cast_count &&
        m_caster->GetCurrentSpell(CURRENT_GENERIC_SPELL) &&
        (!(m_spellInfo->AttributesEx9 & SPELL_ATTR9_CASTABLE_WHILE_CAST_IN_PROGRESS) || GetSpellInfo()->CalcCastTime(m_caster)))
    {
        SendCastResult(SPELL_FAILED_SPELL_IN_PROGRESS);
        finish(false);
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_SPELL, m_spellInfo->Id, m_caster))
    {
        SendCastResult(SPELL_FAILED_SPELL_UNAVAILABLE);
        finish(false);
        return;
    }
    LoadScripts();

    if (IsDarkSimulacrum() || (m_triggeredByAuraSpell && m_triggeredByAuraSpell->Id == 101056))
        isStolen = true;

    if (m_caster->IsPlayer())
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);
    // Fill cost data (not use power for item casts
    memset(m_powerCost, 0, sizeof(uint32)* MAX_POWERS_COST);
    m_powerCost[MAX_POWERS_COST - 1] = 0;
    if (m_CastItem == 0)
        m_spellInfo->CalcPowerCost(m_caster, m_spellSchoolMask, m_powerCost);
    if (m_caster->IsPlayer())
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);

    // Set combo point requirement
    if ((_triggeredCastFlags & TRIGGERED_IGNORE_COMBO_POINTS) || m_CastItem || !m_caster->m_movedPlayer)
        m_needComboPoints = false;

    SpellCastResult result = CheckCast(true);
    if (result != SPELL_CAST_OK && !IsAutoRepeat())          //always cast autorepeat dummy for triggering
    {
        // Periodic auras should be interrupted when aura triggers a spell which can't be cast
        // for example bladestorm aura should be removed on disarm as of patch 3.3.5
        // channeled periodic spells should be affected by this (arcane missiles, penance, etc)
        // a possible alternative sollution for those would be validating aura target on unit state change
        if (triggeredByAura && triggeredByAura->IsPeriodic() && !triggeredByAura->GetBase()->IsPassive())
        {
            if (result != SPELL_FAILED_BAD_TARGETS)
            {
                SendChannelUpdate(0);
                triggeredByAura->GetBase()->SetDuration(0);
                SendCastResult(result);
            }
        }
        else
            SendCastResult(result);

        /// Restore SpellMods after spell failed
        if (m_caster->GetTypeId() == TypeID::TYPEID_PLAYER)
            m_caster->ToPlayer()->RestoreSpellMods(this);

        finish(false);
        return;
    }

    // Prepare data for triggers
    prepareDataForTriggerSystem(triggeredByAura);

    if (m_caster->IsPlayer())
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);
    // calculate cast time (calculated after first CheckCast check to prevent charge counting for first CheckCast fail)
    m_casttime = m_spellInfo->CalcCastTime(m_caster, this);

    // Unstable Afflication (30108) with Soulborn: Soul Swap (141931)
    if (m_spellInfo && m_spellInfo->Id == 30108 && _triggeredCastFlags == TRIGGERED_FULL_MASK)
        m_casttime = 0;

    // If spell not channeled and was stolen he have no cast time
    if (isStolen && !m_spellInfo->IsChanneled() && m_spellInfo->Id != 605)
        m_casttime = 0;

    if (m_caster->IsPlayer())
    {
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);

        // Set cast time to 0 if .cheat cast time is enabled.
        if (m_caster->ToPlayer()->GetCommandStatus(CHEAT_CASTTIME))
             m_casttime = 0;
    }

    // don't allow channeled spells / spells with cast time to be casted while moving
    // (even if they are interrupted on moving, spells with almost immediate effect get to have their effect processed before movement interrupter kicks in)
    // don't cancel spells which are affected by a SPELL_AURA_CAST_WHILE_WALKING or SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKINGeffect
    if (((m_spellInfo->IsChanneled() || m_casttime) && m_caster->IsPlayer() && m_caster->IsMoving() &&
        m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_MOVEMENT) && !m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, m_spellInfo) &&
        !m_caster->HasAuraType(SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING))
    {
        SendCastResult(SPELL_FAILED_MOVING);
        finish(false);
        return;
    }

    // set timer base at cast time
    ReSetTimer();

    CallScriptOnPrepareHandlers();
    //Containers for channeled spells have to be set
    //TODO:Apply this to all casted spells if needed
    // Why check duration? 29350: channeled triggers channeled
    if ((_triggeredCastFlags & TRIGGERED_CAST_DIRECTLY) && (!m_spellInfo->IsChanneled() || !m_spellInfo->GetMaxDuration()))
    {
        if (m_spellInfo->HasEffect(SpellEffects::SPELL_EFFECT_LOOT_BONUS))
            SendSpellStart();

        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_GCD) && result == SPELL_CAST_OK) ///< Global cooldown should be trigger before cast
            TriggerGlobalCooldown();

        cast(true);
    }
    else
    {
        // stealth must be removed at cast starting (at show channel bar)
        // skip triggered spell (item equip spell casting and other not explicit character casts/item uses)
        if (CheckInterrupt())
        {
            m_caster->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_CAST);
            for (uint32 i = 0; i < m_spellInfo->EffectCount; ++i)
            {
                if (m_spellInfo->Effects[i].GetUsedTargetObjectType() == TARGET_OBJECT_TYPE_UNIT)
                {
                    m_caster->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_SPELL_ATTACK);
                    break;
                }
            }
        }

        m_caster->SetCurrentCastedSpell(this);
        SendSpellStart();

        // set target for proper facing
        if ((m_casttime || m_spellInfo->IsChanneled()) && !(_triggeredCastFlags & TRIGGERED_IGNORE_SET_FACING))
        {
            if (m_targets.GetObjectTarget() && m_caster != m_targets.GetObjectTarget() && m_caster->GetTypeId() == TYPEID_UNIT)
                m_caster->FocusTarget(this, m_targets.GetObjectTarget());
        }

        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_GCD) && result == SPELL_CAST_OK)
            TriggerGlobalCooldown();

        //item: first cast may destroy item and second cast causes crash
        if (!m_casttime && !m_spellInfo->StartRecoveryTime && !m_castItemGUID && GetCurrentContainer() == CURRENT_GENERIC_SPELL)
            cast(true);
    }
}

bool Spell::CheckInterrupt()
{
    if (CallScriptCheckInterruptHandlers())
        return true;

    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_AURA_INTERRUPT_FLAGS) && m_spellInfo->IsBreakingStealth(m_caster) && (!m_caster->HasAuraType(SPELL_AURA_MOD_CAMOUFLAGE) || m_spellInfo->IsBreakingCamouflage()))
        return true;

    return false;
}

void Spell::cancel()
{
    if (m_spellState == SPELL_STATE_FINISHED)
        return;

    uint32 oldState = m_spellState;
    m_spellState = SPELL_STATE_FINISHED;

    m_autoRepeat = false;
    switch (oldState)
    {
        case SPELL_STATE_PREPARING:
            CancelGlobalCooldown();
            if (m_caster->IsPlayer())
                m_caster->ToPlayer()->RestoreSpellMods(this);
        case SPELL_STATE_DELAYED:
            SendInterrupted(0);
            SendCastResult(SPELL_FAILED_INTERRUPTED);
            break;

        case SPELL_STATE_CASTING:
            for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                if ((*ihit).missCondition == SPELL_MISS_NONE)
                    if (Unit* unit = m_caster->GetGUID() == ihit->targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID))
                        unit->RemoveOwnedAura(m_spellInfo->Id, m_originalCasterGUID, 0, AURA_REMOVE_BY_CANCEL);

            SendChannelUpdate(0);
            SendInterrupted(0);
            SendCastResult(SPELL_FAILED_INTERRUPTED);

            // spell is canceled-take mods and clear list
            if (m_caster->IsPlayer())
                m_caster->ToPlayer()->RemoveSpellMods(this);

            m_appliedMods.clear();
            break;

        default:
            break;
    }

    SetReferencedFromCurrent(false);
    if (m_selfContainer && *m_selfContainer == this)
        *m_selfContainer = NULL;

    m_caster->RemoveDynObject(m_spellInfo->Id);
    if (m_spellInfo->IsChanneled()) // if not channeled then the object for the current cast wasn't summoned yet
        m_caster->RemoveGameObject(m_spellInfo->Id, true);

    //set state back so finish will be processed
    m_spellState = oldState;

    finish(false);
}

void Spell::cast(bool skipCheck)
{
    // update pointers base at GUIDs to prevent access to non-existed already object
    UpdatePointers();

    // cancel at lost explicit target during cast
    if (m_targets.GetObjectTargetGUID() && !m_targets.GetObjectTarget())
    {
        cancel();
        return;
    }

    if (Player* playerCaster = m_caster->ToPlayer())
    {
        // now that we've done the basic check, now run the scripts
        // should be done before the spell is actually executed
        sScriptMgr->OnPlayerSpellCast(playerCaster, this, skipCheck);

        // As of 3.0.2 pets begin attacking their owner's target immediately
        // Let any pets know we've attacked something. Check DmgClass for harmful spells only
        // This prevents spells such as Hunter's Mark from triggering pet attack
        if (this->GetSpellInfo()->DmgClass != SPELL_DAMAGE_CLASS_NONE)
            if (Pet* playerPet = playerCaster->GetPet())
                if (playerPet->ToCreature()->IsAIEnabled && playerPet->isAlive() && playerPet->isControlled() && (m_targets.GetTargetMask() & TARGET_FLAG_UNIT))
                    if (WorldObject* target = m_targets.GetObjectTarget())
                        playerPet->AI()->OwnerAttacked(target->ToUnit());
    }
    SetExecutedCurrently(true);

    if (m_caster->GetTypeId() != TYPEID_PLAYER && m_targets.GetUnitTarget() && m_targets.GetUnitTarget() != m_caster)
        m_caster->SetInFront(m_targets.GetUnitTarget());

    // Should this be done for original caster?
    if (m_caster->IsPlayer())
    {
        // Set spell which will drop charges for triggered cast spells
        // if not successfully casted, will be remove in finish(false)
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);
    }

    switch (m_spellInfo->Id)
    {
        case 101603: // Throw Totem
            if(m_caster->HasAura(107837))
                m_caster->RemoveAura(107837, 0, 0, AURA_REMOVE_BY_CANCEL);
                m_caster->RemoveAura(101601, 0, 0, AURA_REMOVE_BY_CANCEL);
            break;
        case 119393: // Siege Explosive
            if(m_caster->HasAura(119388))
                m_caster->RemoveAura(119388, 0, 0, AURA_REMOVE_BY_CANCEL);
                m_caster->RemoveAura(119386, 0, 0, AURA_REMOVE_BY_CANCEL);
            break;
        case 123057: // Sonic Divebomb
            if(m_caster->HasAura(123021))
                m_caster->RemoveAura(123021, 0, 0, AURA_REMOVE_BY_CANCEL);
                m_caster->RemoveAura(123057, 0, 0, AURA_REMOVE_BY_CANCEL);
            break;
        case 123039: // Player Throw Barrel
            if(m_caster->HasAura(123032))
                m_caster->RemoveAura(123032, 0, 0, AURA_REMOVE_BY_CANCEL);
                m_caster->RemoveAura(123035, 0, 0, AURA_REMOVE_BY_CANCEL);
            break;
        case 127329: // Throw Bomb
            if(m_caster->HasAura(127226))
                m_caster->RemoveAura(127226, 0, 0, AURA_REMOVE_BY_CANCEL);
                m_caster->RemoveAura(127175, 0, 0, AURA_REMOVE_BY_CANCEL);
            break;
    default:
        break;
    }

    CallScriptBeforeCastHandlers();

    // skip check if done already (for instant cast spells for example)
    if (!skipCheck)
    {
        SpellCastResult castResult = CheckCast(false);
        if (castResult != SPELL_CAST_OK)
        {
            SendCastResult(castResult);
            SendInterrupted(0);
            //restore spell mods
            if (m_caster->IsPlayer())
            {
                m_caster->ToPlayer()->RestoreSpellMods(this);
                // cleanup after mod system
                // triggered spell pointer can be not removed in some cases
                m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
            }
            finish(false);
            SetExecutedCurrently(false);
            return;
        }

        // additional check after cast bar completes (must not be in CheckCast)
        // if trade not complete then remember it in trade data
        if (m_targets.GetTargetMask() & TARGET_FLAG_TRADE_ITEM)
        {
            if (m_caster->IsPlayer())
            {
                if (TradeData* my_trade = m_caster->ToPlayer()->GetTradeData())
                {
                    if (!my_trade->IsInAcceptProcess())
                    {
                        // Spell will be casted at completing the trade. Silently ignore at this place
                        my_trade->SetSpell(m_spellInfo->Id, m_CastItem);
                        SendCastResult(SPELL_FAILED_DONT_REPORT);
                        SendInterrupted(0);
                        m_caster->ToPlayer()->RestoreSpellMods(this);
                        // cleanup after mod system
                        // triggered spell pointer can be not removed in some cases
                        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
                        finish(false);
                        SetExecutedCurrently(false);
                        return;
                    }
                }
            }
        }
    }

    SelectSpellTargets();

    // Spell may be finished after target map check
    if (m_spellState == SPELL_STATE_FINISHED)
    {
        SendInterrupted(0);
        //restore spell mods
        if (m_caster->IsPlayer())
        {
            m_caster->ToPlayer()->RestoreSpellMods(this);
            // cleanup after mod system
            // triggered spell pointer can be not removed in some cases
            m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
        }
        finish(false);
        SetExecutedCurrently(false);
        return;
    }

    PrepareTriggersExecutedOnHit();

    if (m_caster->getClass() == CLASS_DEATH_KNIGHT)
        if (IsDarkSimulacrum())
            m_caster->RemoveAurasDueToSpell(77616);

    CallScriptOnCastHandlers();

    // traded items have trade slot instead of guid in m_itemTargetGUID
    // set to real guid to be sent later to the client
    m_targets.UpdateTradeSlotItem();

    if (Player* player = m_caster->ToPlayer())
    {
        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CAST_ITEM) && m_CastItem)
        {
            player->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_ITEM, m_CastItem->GetEntry());
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM, m_CastItem->GetEntry(), 0, 0, player);
        }

        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, m_spellInfo->Id);
    }

    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_POWER_AND_REAGENT_COST))
    {
        // Powers have to be taken before SendSpellGo
        TakePower();
        TakeReagents();                                         // we must remove reagents before HandleEffects to allow place crafted item in same slot
    }
    else if (Item* targetItem = m_targets.GetItemTarget())
    {
        /// Not own traded item (in trader trade slot) req. reagents including triggered spell case
        if (targetItem->GetOwnerGUID() != m_caster->GetGUID())
            TakeReagents();
    }

    // CAST SPELL
    SendSpellCooldown();

    PrepareScriptHitHandlers();

    HandleLaunchPhase();

    // we must send smsg_spell_go packet before m_castItem delete in TakeCastItem()...
    SendSpellGo();

    // Okay, everything is prepared. Now we need to distinguish between immediate and evented delayed spells
    if (((m_spellInfo->Speed > 0.0f || (m_delayMoment && (m_caster->IsPlayer() || m_caster->ToCreature()->isPet())))
        && !m_spellInfo->IsChanneled()) || m_spellInfo->_IsNeedDelay())
    {
        // Remove used for cast item if need (it can be already NULL after TakeReagents call
        // in case delayed spell remove item at cast delay start
        TakeCastItem();

        // Okay, maps created, now prepare flags
        m_immediateHandled = false;
        m_spellState = SPELL_STATE_DELAYED;
        SetDelayStart(0);

        if (IsAutoActionResetSpell())
        {
            bool found = false;
            Unit::AuraEffectList const& vIgnoreReset = m_caster->GetAuraEffectsByType(SPELL_AURA_IGNORE_MELEE_RESET);
            for (Unit::AuraEffectList::const_iterator i = vIgnoreReset.begin(); i != vIgnoreReset.end(); ++i)
            {
                if ((*i)->IsAffectingSpell(m_spellInfo))
                {
                    found = true;
                    break;
                }
            }
            if (!found && !(m_spellInfo->AttributesEx2 & SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS))
            {
                m_caster->resetAttackTimer(WeaponAttackType::BaseAttack);
                if (m_caster->haveOffhandWeapon())
                    m_caster->resetAttackTimer(WeaponAttackType::OffAttack);
                m_caster->resetAttackTimer(WeaponAttackType::RangedAttack);
            }
        }

        if (m_caster->HasUnitState(UNIT_STATE_CASTING) && !m_caster->IsNonMeleeSpellCasted(false, false, true))
            m_caster->ClearUnitState(UNIT_STATE_CASTING);
    }
    else
    {
        // Immediate spell, no big deal
        handle_immediate();
    }

    CallScriptAfterCastHandlers();

    /// Trigger all effects if this spell should do that after cast
    if (IsSpellTriggeredAfterCast())
    {
        // triggered spell pointer can be not set in some cases
        // this is needed for proper apply of triggered spell mods
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);

        // Take mods after trigger spell (needed for 14177 to affect 48664)
        // mods are taken only on succesfull cast and independantly from targets of the spell
        m_caster->ToPlayer()->RemoveSpellMods(this);
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
    }

    if (m_caster->GetTypeId() == TypeID::TYPEID_UNIT && m_caster->ToCreature()->IsAIEnabled)
        m_caster->ToCreature()->AI()->OnSpellCasted(m_spellInfo);

    /// Soul Swap - if we have copied DOTs and cast any other spell, soul swap will refresh duration of all DOTs
    if (m_caster->IsPlayer() && m_caster->getClass() == CLASS_WARLOCK && m_caster->ToPlayer()->GetSpecializationId() == SPEC_WARLOCK_AFFLICTION)
    {
        /// Ignore generic proc spells like Soul Shards (visual), Life Steal etc, also ignore Soul Swap spell
        if (m_spellInfo && m_spellInfo->Id != 86121 && m_spellInfo->SpellFamilyName == SPELLFAMILY_WARLOCK && _triggeredCastFlags == TRIGGERED_NONE && m_caster->HasAura(86211))
            m_caster->SetSoulSwapRefreshDuration(true);
    }

    // Kil'Jaeden's Cunning - 10% speed less for each cast while moving (up to 2 charges) ///< @todo Kil'Jaeden's Cunning  is removed 
    if (m_caster->HasAuraType(SPELL_AURA_KIL_JAEDENS_CUNNING) && m_caster->IsMoving() && !m_caster->HasAura(119048) && m_spellInfo->CalcCastTime(m_caster) > 0)
        m_caster->CastSpell(m_caster, 119050, true);

    if (const std::vector<int32> *spell_triggered = sSpellMgr->GetSpellLinked(m_spellInfo->Id))
    {
        for (std::vector<int32>::const_iterator i = spell_triggered->begin(); i != spell_triggered->end(); ++i)
            if (*i < 0)
                m_caster->RemoveAurasDueToSpell(-(*i));
            else
                m_caster->CastSpell(m_targets.GetUnitTarget() ? m_targets.GetUnitTarget() : m_caster, *i, true);
    }

    if (m_caster->IsPlayer())
    {
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
        //Clear spell cooldowns after every spell is cast if .cheat cooldown is enabled.
        if (m_caster->ToPlayer()->GetCommandStatus(CHEAT_COOLDOWN))
        {
            m_caster->ToPlayer()->RemoveSpellCooldown(m_spellInfo->Id, true);
            m_caster->ToPlayer()->RestoreCharge(m_spellInfo->ChargeCategoryEntry);
        }
    }

    SetExecutedCurrently(false);
}

void Spell::handle_immediate()
{
    // start channeling if applicable
    if (m_spellInfo->IsChanneled())
    {
        int32 duration = m_spellInfo->GetDuration();
        if (duration)
        {
            // First mod_duration then haste - see Missile Barrage
            // Apply duration mod
            if (Player* modOwner = m_caster->GetSpellModOwner())
                modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_DURATION, duration);

            // Apply haste mods
            m_caster->ModSpellCastTime(m_spellInfo, duration, this);

            m_spellState = SPELL_STATE_CASTING;
            m_caster->AddInterruptMask(m_spellInfo->ChannelInterruptFlags);
            m_channeledDuration = duration;
            SendChannelStart(duration);
        }
        else if (duration == -1)
        {
            m_spellState = SPELL_STATE_CASTING;
            m_caster->AddInterruptMask(m_spellInfo->ChannelInterruptFlags);
            SendChannelStart(duration);
        }
    }

    PrepareTargetProcessing();

    // process immediate effects (items, ground, etc.) also initialize some variables
    _handle_immediate_phase();

    for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        DoAllEffectOnTarget(&(*ihit));

    for (std::list<GOTargetInfo>::iterator ihit= m_UniqueGOTargetInfo.begin(); ihit != m_UniqueGOTargetInfo.end(); ++ihit)
        DoAllEffectOnTarget(&(*ihit));

    FinishTargetProcessing();

    // spell is finished, perform some last features of the spell here
    _handle_finish_phase();

    // Remove used for cast item if need (it can be already NULL after TakeReagents call
    TakeCastItem();

    // handle ammo consumption for thrown weapons
    if (m_spellInfo->IsRangedWeaponSpell() && m_spellInfo->IsChanneled())
        TakeAmmo();

    if (m_spellState != SPELL_STATE_CASTING)
        finish(true);                                       // successfully finish spell cast (not last in case autorepeat or channel spell)
}

uint64 Spell::handle_delayed(uint64 t_offset)
{
    UpdatePointers();

    if (m_caster->IsPlayer())
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);

    uint64 next_time = 0;

    PrepareTargetProcessing();

    if (!m_immediateHandled)
    {
        _handle_immediate_phase();
        m_immediateHandled = true;
    }

    bool single_missile = (m_targets.HasDst());

    // now recheck units targeting correctness (need before any effects apply to prevent adding immunity at first effect not allow apply second spell effect and similar cases)
    for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->processed == false)
        {
            if (single_missile || ihit->timeDelay <= t_offset)
            {
                ihit->timeDelay = t_offset;
                DoAllEffectOnTarget(&(*ihit));
            }
            else if (next_time == 0 || ihit->timeDelay < next_time)
                next_time = ihit->timeDelay;
        }
    }

    // now recheck gameobject targeting correctness
    for (std::list<GOTargetInfo>::iterator ighit= m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end(); ++ighit)
    {
        if (ighit->processed == false)
        {
            if (single_missile || ighit->timeDelay <= t_offset)
                DoAllEffectOnTarget(&(*ighit));
            else if (next_time == 0 || ighit->timeDelay < next_time)
                next_time = ighit->timeDelay;
        }
    }

    FinishTargetProcessing();

    if (m_caster->IsPlayer())
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);

    // All targets passed - need finish phase
    if (next_time == 0)
    {
        // spell is finished, perform some last features of the spell here
        _handle_finish_phase();

        finish(true);                                       // successfully finish spell cast

        // return zero, spell is finished now
        return 0;
    }
    else
    {
        // spell is unfinished, return next execution time
        return next_time;
    }
}

void Spell::_handle_immediate_phase()
{
    m_spellAura = nullptr;
    // initialize Diminishing Returns Data
    m_diminishLevel = DIMINISHING_LEVEL_1;
    m_diminishGroup = DIMINISHING_NONE;

    // handle some immediate features of the spell here
    HandleThreatSpells();

    PrepareScriptHitHandlers();

    // handle effects with SPELL_EFFECT_HANDLE_HIT mode
    for (uint8 j = 0; j < m_spellInfo->EffectCount; ++j)
    {
        // don't do anything for empty effect
        if (!m_spellInfo->Effects[j].IsEffect())
            continue;

        // call effect handlers to handle destination hit
        HandleEffects(nullptr, nullptr, nullptr, nullptr, j, SPELL_EFFECT_HANDLE_HIT);
    }

    // process items
    for (std::list<ItemTargetInfo>::iterator ihit= m_UniqueItemInfo.begin(); ihit != m_UniqueItemInfo.end(); ++ihit)
        DoAllEffectOnTarget(&(*ihit));

    if (!m_originalCaster)
        return;

    // Handle procs on cast
    // TODO: finish new proc system:P
    if (m_UniqueTargetInfo.empty() && m_targets.HasDst())
    {
        uint32 procAttacker = m_procAttacker;
        if (!procAttacker)
            procAttacker |= PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS;

        // Proc the spells that have DEST target
        Unit *target = NULL;
        if (procAttacker & PROC_FLAG_DONE_TRAP_ACTIVATION)
            target = m_targets.GetUnitTarget();
        m_originalCaster->ProcDamageAndSpell(target, procAttacker, 0, m_procEx | PROC_EX_NORMAL_HIT, 0, 0, WeaponAttackType::BaseAttack, m_spellInfo, m_triggeredByAuraSpell);
    }
}

void Spell::_handle_finish_phase()
{
    if (m_caster->m_movedPlayer)
    {
        // Take for real after all targets are processed
        if (m_needComboPoints || m_spellInfo->Id == 127538)
        {
            m_caster->ClearComboPoints();

            /// Anticipation
            if (m_caster->HasAura(115189) && (m_spellInfo->Id == 2098 || m_spellInfo->Id == 32645 || m_spellInfo->Id == 152150))
            {
                int32 basepoints0 = m_caster->GetAura(115189)->GetStackAmount();
                m_caster->CastCustomSpell(m_caster->getVictim(), 115190, &basepoints0, NULL, NULL, true);

                if (basepoints0)
                    m_caster->RemoveAura(115189);
            }
        }

        // Real add combo points from effects
        if (m_comboPointGain)
            m_caster->AddComboPoints(m_comboPointGain);

        if (m_spellInfo->PowerType == POWER_HOLY_POWER && m_caster->m_movedPlayer->getClass() == CLASS_PALADIN)
            HandleHolyPower(m_caster->m_movedPlayer);
    }

    if (m_caster->m_extraAttacks && GetSpellInfo()->HasEffect(SPELL_EFFECT_ADD_EXTRA_ATTACKS))
        m_caster->HandleProcExtraAttackFor(m_caster->getVictim());

    // TODO: trigger proc phase finish here
}

void Spell::SendSpellCooldown()
{
    Player* l_Player = m_caster->ToPlayer();
    if (!l_Player)
    {
        // Handle pet cooldowns here if needed instead of in PetAI to avoid hidden cooldown restarts
        Creature* _creature = m_caster->ToCreature();
        if (_creature && (_creature->isPet() || _creature->isGuardian()))
            _creature->AddCreatureSpellCooldown(m_spellInfo->Id);

        return;
    }

    if (l_Player && l_Player->ConsumeCharge(m_spellInfo->ChargeCategoryEntry))
        return;

    // mana/health/etc potions, disabled by client (until combat out as declarate)
    if (m_CastItem && (m_CastItem->IsPotion() || m_CastItem->IsHealthstone() || m_spellInfo->IsCooldownStartedOnEvent()))
    {
        // need in some way provided data for Spell::finish SendCooldownEvent
        l_Player->SetLastPotionId(m_CastItem->GetEntry());
        return;
    }

    // have infinity cooldown but set at aura apply                  // do not set cooldown for triggered spells (needed by reincarnation)
    if (m_spellInfo->IsCooldownStartedOnEvent() || m_spellInfo->IsPassive() || (_triggeredCastFlags & TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD))
        return;

    if (m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_ALLOW_CAST_WHILE_IN_COOLDOWN, m_spellInfo))
        return;

    l_Player->AddSpellAndCategoryCooldowns(m_spellInfo, m_CastItem ? m_CastItem->GetEntry() : m_CastItemEntry, this);
}

void Spell::update(uint32 difftime)
{
    // update pointers based at it's GUIDs
    UpdatePointers();

    if (m_targets.GetUnitTargetGUID() && !m_targets.GetUnitTarget())
    {
        cancel();
        return;
    }

    // check if the player caster has moved before the spell finished
    // with the exception of spells affected with SPELL_AURA_CAST_WHILE_WALKING or SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING effect
    if ((m_caster->IsPlayer() && m_timer != 0) &&
        m_caster->IsMoving() && (m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_MOVEMENT) &&
        (m_spellInfo->Effects[0].Effect != SPELL_EFFECT_STUCK || !m_caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING_FAR)) &&
        !m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, m_spellInfo) && !m_caster->HasAuraType(SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING))
    {
        // don't cancel for melee, autorepeat, triggered and instant spells
        if (!IsNextMeleeSwingSpell() && !IsAutoRepeat() && !IsTriggered())
            cancel();
    }

    switch (m_spellState)
    {
        case SPELL_STATE_PREPARING:
        {
            if (m_timer > 0)
            {
                if (difftime >= (uint32)m_timer)
                    m_timer = 0;
                else
                    m_timer -= difftime;
            }

            if (m_timer == 0 && !IsNextMeleeSwingSpell() && !IsAutoRepeat())
                // don't CheckCast for instant spells - done in spell::prepare, skip duplicate checks, needed for range checks for example
                cast(!m_casttime);
            break;
        }
        case SPELL_STATE_CASTING:
        {
            if (m_timer)
            {
                // check if there are alive targets left
                if (!UpdateChanneledTargetList())
                {
                    SendChannelUpdate(0);
                    finish();
                }

                if (m_timer > 0)
                {
                    if (difftime >= (uint32)m_timer)
                        m_timer = 0;
                    else
                        m_timer -= difftime;
                }
            }

            if (m_timer == 0)
            {
                SendChannelUpdate(0);

                // channeled spell processed independently for quest targeting
                // cast at creature (or GO) quest objectives update at successful cast channel finished
                // ignore autorepeat/melee casts for speed (not exist quest for spells (hm...)
                if (!IsAutoRepeat() && !IsNextMeleeSwingSpell())
                {
                    if (Player* p = m_caster->GetCharmerOrOwnerPlayerOrPlayerItself())
                    {
                        for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                        {
                            TargetInfo* target = &*ihit;
                            if (!IS_CRE_OR_VEH_GUID(target->targetGUID))
                                continue;

                            Unit* unit = m_caster->GetGUID() == target->targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, target->targetGUID);
                            if (unit == NULL)
                                continue;

                            p->CastedCreatureOrGO(unit->GetEntry(), unit->GetGUID(), m_spellInfo->Id);
                        }

                        for (std::list<GOTargetInfo>::iterator ihit = m_UniqueGOTargetInfo.begin(); ihit != m_UniqueGOTargetInfo.end(); ++ihit)
                        {
                            GOTargetInfo* target = &*ihit;

                            GameObject* go = m_caster->GetMap()->GetGameObject(target->targetGUID);
                            if (!go)
                                continue;

                            p->CastedCreatureOrGO(go->GetEntry(), go->GetGUID(), m_spellInfo->Id);
                        }
                    }
                }

                finish();
            }
            break;
        }
        default:
            break;
    }
}

void Spell::finish(bool ok)
{
    if (!m_caster)
        return;

    if (m_spellState == SPELL_STATE_FINISHED)
        return;
    m_spellState = SPELL_STATE_FINISHED;

    if (m_spellInfo->IsChanneled())
        m_caster->UpdateInterruptMask();

    if (m_caster->GetTypeId() == TypeID::TYPEID_UNIT && m_caster->IsAIEnabled)
        m_caster->ToCreature()->AI()->OnSpellFinished(m_spellInfo);

    if (IsAutoActionResetSpell())
    {
        bool found = false;
        Unit::AuraEffectList const& vIgnoreReset = m_caster->GetAuraEffectsByType(SPELL_AURA_IGNORE_MELEE_RESET);
        for (Unit::AuraEffectList::const_iterator i = vIgnoreReset.begin(); i != vIgnoreReset.end(); ++i)
        {
            if ((*i)->IsAffectingSpell(m_spellInfo))
            {
                found = true;
                break;
            }
        }
        if (!found && !(m_spellInfo->AttributesEx2 & SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS))
        {
            m_caster->resetAttackTimer(WeaponAttackType::BaseAttack);
            if (m_caster->haveOffhandWeapon())
                m_caster->resetAttackTimer(WeaponAttackType::OffAttack);
            m_caster->resetAttackTimer(WeaponAttackType::RangedAttack);
        }
    }

    if (m_caster->HasUnitState(UNIT_STATE_CASTING) && !m_caster->IsNonMeleeSpellCasted(false, false, true))
        m_caster->ClearUnitState(UNIT_STATE_CASTING);

    // Unsummon summon as possessed creatures on spell cancel
    if (m_spellInfo->IsChanneled() && m_caster->IsPlayer())
    {
        if (Unit* charm = m_caster->GetCharm())
            if (charm->GetTypeId() == TYPEID_UNIT
                && charm->ToCreature()->HasUnitTypeMask(UNIT_MASK_PUPPET)
                && charm->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL) == m_spellInfo->Id)
                ((Puppet*)charm)->UnSummon();
    }

    if (m_caster->GetTypeId() == TYPEID_UNIT)
        m_caster->ReleaseFocus(this);

    if (!ok)
        return;

    if (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->isSummon())
    {
        // Unsummon statue
        uint32 spell = m_caster->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL);
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell);
        if (spellInfo && spellInfo->SpellIconID == 2056)
        {
            m_caster->setDeathState(JUST_DIED);
            return;
        }
    }

    // potions disabled by client, send event "not in combat" if need
    if (m_caster->IsPlayer() && !IsSpellTriggeredAfterCast())
    {
        /// There is no cooldown to update for triggered spells
        if (!m_triggeredByAuraSpell && !(_triggeredCastFlags & TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD))
            m_caster->ToPlayer()->UpdatePotionCooldown(this);

        // triggered spell pointer can be not set in some cases
        // this is needed for proper apply of triggered spell mods
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);

        // Take mods after trigger spell (needed for 14177 to affect 48664)
        // mods are taken only on succesfull cast and independantly from targets of the spell
        m_caster->ToPlayer()->RemoveSpellMods(this);
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
    }

    // Stop Attack for some spells
    if (m_spellInfo->Attributes & SPELL_ATTR0_STOP_ATTACK_TARGET)
        m_caster->AttackStop();

    if (m_castItemGUID && m_caster->IsPlayer())
        if (Item* item = m_caster->ToPlayer()->GetItemByGuid(m_castItemGUID))
            if (item->IsEquipable() && !item->IsEquipped())
                m_caster->ToPlayer()->ApplyItemEquipSpell(item, false);

    switch (m_spellInfo->Id)
    {
        case 49560: // Death Grip
        case 49576: // Death Grip dummy
        {
            if (m_caster->GetTypeId() != TYPEID_PLAYER)
                break;

            // Only for Death Grip triggered to prevent dysfunctionment of the set bonus
            if (m_spellInfo->Id == 49560)
            {
                bool first = !m_caster->HasAura(131547);

                m_caster->RemoveAurasDueToSpell(131547); // Relentless Grip

                if (m_caster->HasAura(131542) && first) // Item DK - PvP S12 4P Bonus
                    m_caster->CastSpell(m_caster, 131547, true);
            }

            if (m_UniqueTargetInfo.empty())
                break;

            if (m_UniqueTargetInfo.front().missCondition != SPELL_MISS_IMMUNE && m_UniqueTargetInfo.front().missCondition != SPELL_MISS_IMMUNE2)
                break;

            if (m_caster->HasAura(59309)) // Glyph of Resilient Grip
                m_caster->ToPlayer()->RemoveSpellCooldown(m_spellInfo->Id, true);

            break;
        }
        default:
            break;
    }
}

void Spell::SendCastResult(SpellCastResult result)
{
    if (result == SPELL_CAST_OK)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    if (m_caster->ToPlayer()->GetSession()->PlayerLoading())  // don't send cast results at loading time
        return;

    SendCastResult(m_caster->ToPlayer(), m_spellInfo, m_cast_count, result, m_customError);
}

void Spell::SendCastResult(Player* caster, SpellInfo const* p_SpellInfo, uint8 cast_count, SpellCastResult result, SpellCustomErrors customError /*= SPELL_CUSTOM_ERROR_NONE*/)
{
    if (result == SPELL_CAST_OK)
        return;

    WorldPacket l_Data(SMSG_CAST_FAILED, 4 + 4 + 4 + 4 + 1);
    l_Data << uint32(p_SpellInfo->Id);                                  ///< spellId
    l_Data << uint32(result);                                           ///< problem

    switch (result)
    {
        case SPELL_FAILED_NO_POWER:
        {
            l_Data << uint32(p_SpellInfo->GetMainPower());              ///< Power type
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_NOT_READY:
        {
            l_Data << uint32(0);                                        ///< unknown (value 1 update cooldowns on client flag)
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_REQUIRES_SPELL_FOCUS:
        {
            l_Data << uint32(p_SpellInfo->RequiresSpellFocus);          ///< SpellFocusObject.dbc id
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_REQUIRES_AREA:                                ///< AreaTable.dbc id
        {
            // hardcode areas limitation case
            switch (p_SpellInfo->Id)
            {
                case 41617:                                             ///< Cenarion Mana Salve
                case 41619:                                             ///< Cenarion Healing Salve
                    l_Data << uint32(3905);
                    break;
                case 41618:                                             ///< Bottled Nethergon Energy
                case 41620:                                             ///< Bottled Nethergon Vapor
                    l_Data << uint32(3842);
                    break;
                case 45373:                                             ///< Bloodberry Elixir
                    l_Data << uint32(4075);
                    break;
                default:                                                ///< default case (don't must be)
                    l_Data << uint32(0);
                    break;
            }
            l_Data << uint32(0);                                        ///< Arg2 => NULL

            break;
        }
        case SPELL_FAILED_TOTEMS:
        {
            l_Data << uint32(p_SpellInfo->Totem[0]);
            l_Data << uint32(p_SpellInfo->Totem[1]);

            break;
        }
        case SPELL_FAILED_TOTEM_CATEGORY:
        {
            l_Data << uint32(p_SpellInfo->TotemCategory[0]);
            l_Data << uint32(p_SpellInfo->TotemCategory[1]);

            break;
        }
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS:
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND:
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND:
        {
            l_Data << uint32(p_SpellInfo->EquippedItemClass);           ///< Arg1 => NULL
            l_Data << uint32(p_SpellInfo->EquippedItemSubClassMask);    ///< Arg2 => NULL

            break;
        }
        case SPELL_FAILED_TOO_MANY_OF_ITEM:
        {
            uint32 l_ItemID = 0;

            for (uint8 l_EffectIndex = 0; l_EffectIndex < p_SpellInfo->EffectCount; l_EffectIndex++)
            {
                if (p_SpellInfo->Effects[l_EffectIndex].ItemType)
                    l_ItemID = p_SpellInfo->Effects[l_EffectIndex].ItemType;
            }

            ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(l_ItemID);

            if (l_ItemTemplate)
                l_Data << uint32(l_ItemTemplate->ItemLimitCategory);    ///< Arg1 => ItemLimitCategory
            else
                l_Data << uint32(0);                                    ///< Arg1 => NULL

            l_Data << uint32(0);                                        ///< Arg2 => NULL

            break;
        }
        case SPELL_FAILED_PREVENTED_BY_MECHANIC:
        {
            l_Data << uint32(p_SpellInfo->GetAllEffectsMechanicMask()); ///< Arg1 => required fishing skill
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_NEED_EXOTIC_AMMO:
        {
            l_Data << uint32(p_SpellInfo->EquippedItemSubClassMask);    ///< Arg1 => required fishing skill
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_NEED_MORE_ITEMS:
        {
            l_Data << uint32(0);                                        ///< Arg1 => Item id
            l_Data << uint32(0);                                        ///< Arg2 => Item count?
            break;
        }
        case SPELL_FAILED_MIN_SKILL:
        {
            l_Data << uint32(0);                                        ///< Arg1 => SkillLine.dbc id
            l_Data << uint32(0);                                        ///< Arg2 => required skill value
            break;
        }
        case SPELL_FAILED_FISHING_TOO_LOW:
        {
            l_Data << uint32(0);                                        ///< Arg1 => required fishing skill
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_CUSTOM_ERROR:
        {
            l_Data << uint32(customError);                              ///< Arg1 => Custom error code
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_SILENCED:
        {
            l_Data << uint32(0);                                        ///< Arg1 => Unknown
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }
        case SPELL_FAILED_REAGENTS:
        {
            uint32 l_MissingItem = 0;

            for (uint32 l_I = 0; l_I < MAX_SPELL_REAGENTS; l_I++)
            {
                if (p_SpellInfo->Reagent[l_I] <= 0)
                    continue;

                uint32 l_ItemID    = p_SpellInfo->Reagent[l_I];
                uint32 l_ItemCount = p_SpellInfo->ReagentCount[l_I];

                if (!caster->HasItemCount(l_ItemID, l_ItemCount))
                {
                    l_MissingItem = l_ItemID;
                    break;
                }
            }

            l_Data << uint32(l_MissingItem);                            ///< Arg1 => first missing item
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
        }

        // @TODO: SPELL_FAILED_NOT_STANDING
        default:
            l_Data << uint32(0);                                        ///< Arg1 => NULL
            l_Data << uint32(0);                                        ///< Arg2 => NULL
            break;
    }

    l_Data << uint8(cast_count);                              ///< single cast or multi 2.3 (0/1)

    caster->GetSession()->SendPacket(&l_Data);
}

void Spell::SendSpellStart()
{
    if (!IsNeedSendToClient())
        return;

    uint32 l_CastFlags   = CAST_FLAG_HAS_TRAJECTORY;
    uint32 l_CastFlagsEx = CastFlagsEx::CAST_FLAG_EX_NONE;

    if (m_CastItemEntry)
        l_CastFlagsEx |= CastFlagsEx::CAST_FLAG_EX_TOY_COOLDOWN;

    uint32 l_SchoolImmunityMask = m_caster->GetSchoolImmunityMask();
    uint32 l_MechanicImmunityMask = m_caster->GetMechanicImmunityMask();
    if (l_SchoolImmunityMask || l_MechanicImmunityMask)
        l_CastFlags |= CAST_FLAG_IMMUNITY;

    if (m_triggeredByAuraSpell)
        l_CastFlags |= CAST_FLAG_PENDING;

    bool l_IsHealthPowerSpell = false;
    for (auto itr : m_spellInfo->SpellPowers)
    {
        if (itr->PowerType == POWER_HEALTH)
        {
            l_IsHealthPowerSpell = true;
            break;
        }
    }

    if ((m_caster->IsPlayer() ||
        (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->isPet()))
        && !l_IsHealthPowerSpell)
        l_CastFlags |= CAST_FLAG_POWER_LEFT_SELF;

    if (m_spellInfo->RuneCostID && m_spellInfo->GetMainPower() == POWER_RUNES)
        l_CastFlags |= CAST_FLAG_NO_GCD;

    if (m_targets.HasTraj())
        l_CastFlags |= CAST_FLAG_ADJUST_MISSILE;

    if (m_spellInfo->HasEffect(SpellEffects::SPELL_EFFECT_LOOT_BONUS))
        l_CastFlags = SpellCastFlags::CAST_FLAG_HAS_TRAJECTORY | SpellCastFlags::CAST_FLAG_NO_GCD;

    WorldPacket data(SMSG_SPELL_START);

    uint32 unkStringLength = 0;
    uint32 powerCount = 0;
    uint32 unkCounter1 = 0;
    uint32 unkCounter2 = 0;
    uint32 l_ExtraTargetsCount = m_targets.GetExtraTargetsCount();

    uint64 l_PredicOverrideTarget = 0;
    uint32 l_PredictAmount = 0;
    uint8 l_PredicType = 0;

    uint32 unkCounter4 = 0;

    uint64 l_CasterGuid1    = m_CastItem ? m_CastItem->GetGUID() : m_caster->GetGUID();
    uint64 l_CasterGuid2    = m_caster->GetGUID();
    uint64 l_TargetGUID     = m_targets.GetObjectTargetGUID();
    uint64 l_TargetItemGUID = m_targets.GetItemTargetGUID();

    // Initialize predicated heal values
    if (m_spellInfo->HasEffect(SPELL_EFFECT_HEAL))
    {
        if (Unit* target = sObjectAccessor->FindUnit(m_targets.GetObjectTargetGUID()))
        {
            l_CastFlags |= CAST_FLAG_HEAL_PREDICTION;
            l_PredicType = 2;
            l_PredicOverrideTarget = target->GetGUID();

            // Guess spell healing amount
            l_PredictAmount = m_caster->CalculateSpellDamage(target, m_spellInfo, 0);
            l_PredictAmount = m_caster->SpellHealingBonusDone(target, m_spellInfo, l_PredictAmount, m_spellInfo->GetEffectIndex(SPELL_EFFECT_HEAL), HEAL);
            l_PredictAmount = m_caster->SpellHealingBonusTaken(target, m_spellInfo, l_PredictAmount, HEAL);
        }
    }

    data.appendPackGUID(l_CasterGuid1);
    data.appendPackGUID(l_CasterGuid2);
    data << uint8(m_cast_count);
    data << uint32(m_spellInfo->Id);
    data << uint32(m_spellInfo->FirstSpellXSpellVisualID);
    data << uint32(l_CastFlags);
    data << uint32(m_casttime);
    data << uint32(0);                      ///< Hitted target count
    data << uint32(0);                      ///< Missed target count
    data << uint32(0);                      ///< Miss count 2
    data.WriteBits(m_targets.GetTargetMask(), 23);
    data.WriteBit(m_targets.HasSrc());
    data.WriteBit(m_targets.HasDst());
    data.WriteBit(false);
    data.WriteBits(0, 7);                   ///< Src target name
    data.FlushBits();

    data.appendPackGUID(l_TargetGUID);
    data.appendPackGUID(l_TargetItemGUID);

    if (m_targets.HasSrc())
    {
        float l_X, l_Y, l_Z;

        if (m_targets.GetSrc()->_transportGUID)
        {
            l_X = m_targets.GetSrc()->_transportOffset.m_positionX;
            l_Y = m_targets.GetSrc()->_transportOffset.m_positionY;
            l_Z = m_targets.GetSrc()->_transportOffset.m_positionZ;
        }
        else
        {
            l_X = m_targets.GetSrc()->_position.m_positionX;
            l_Y = m_targets.GetSrc()->_position.m_positionY;
            l_Z = m_targets.GetSrc()->_position.m_positionZ;
        }

        data.appendPackGUID(m_targets.GetSrc()->_transportGUID);
        data << float(l_X);
        data << float(l_Y);
        data << float(l_Z);
    }

    if (m_targets.HasDst())
    {
        float l_X, l_Y, l_Z;

        if (m_targets.GetDst()->_transportGUID)
        {
            l_X = m_targets.GetDst()->_transportOffset.m_positionX;
            l_Y = m_targets.GetDst()->_transportOffset.m_positionY;
            l_Z = m_targets.GetDst()->_transportOffset.m_positionZ;
        }
        else
        {
            l_X = m_targets.GetDst()->_position.m_positionX;
            l_Y = m_targets.GetDst()->_position.m_positionY;
            l_Z = m_targets.GetDst()->_position.m_positionZ;
        }

        data.appendPackGUID(m_targets.GetDst()->_transportGUID);
        data << float(l_X);
        data << float(l_Y);
        data << float(l_Z);
    }

    data << uint32(0);                      ///< Remaining power count

    data << uint32(0);                      ///< Travel time
    data << float(m_targets.GetSpeed());

    data << uint32(0);
    data << uint8(0);

    data << uint8(0);

    data << uint32(l_ExtraTargetsCount);

    data << uint32(l_SchoolImmunityMask);
    data << uint32(l_MechanicImmunityMask);

    data << uint32(l_PredictAmount);
    data << uint8(l_PredicType);
    data.appendPackGUID(l_PredicOverrideTarget);

    for (auto l_Itr : m_targets.GetExtraTargets())
    {
        data.appendPackGUID(l_Itr._transportGUID);
        data << float(l_Itr._position.GetPositionX());
        data << float(l_Itr._position.GetPositionY());
        data << float(l_Itr._position.GetPositionZ());
    }

    data.WriteBits(l_CastFlagsEx, 20);  ///< Cast flag ex
    data.WriteBit(false);
    data.FlushBits();

    m_caster->SendMessageToSet(&data, true);
}

void Spell::SendSpellGo()
{
    // not send invisible spell casting
    if (!IsNeedSendToClient() && m_spellInfo->Id != 178236)
        return;

    bool l_IsHealthPowerSpell = false;
    for (auto itr : m_spellInfo->SpellPowers)
    {
        if (itr->PowerType == POWER_HEALTH)
        {
            l_IsHealthPowerSpell = true;
            break;
        }
    }

    uint32 l_CastFlags   = CAST_FLAG_UNKNOWN_9;
    uint32 l_CastFlagsEx = CastFlagsEx::CAST_FLAG_EX_NONE;

    // triggered spells with spell visual != 0
    if (m_triggeredByAuraSpell)
        l_CastFlags |= CAST_FLAG_PENDING;

    if ((m_caster->IsPlayer() ||
        (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->isPet()))
        && !l_IsHealthPowerSpell)
        l_CastFlags |= CAST_FLAG_POWER_LEFT_SELF; // should only be sent to self, but the current messaging doesn't make that possible

    if ((m_caster->IsPlayer())
        && (m_caster->getClass() == CLASS_DEATH_KNIGHT)
        && m_spellInfo->RuneCostID
        && m_spellInfo->GetMainPower() == POWER_RUNES
        && !(_triggeredCastFlags & TRIGGERED_IGNORE_POWER_AND_REAGENT_COST))
    {
        l_CastFlags |= CAST_FLAG_NO_GCD;                    ///< same as in SMSG_SPELL_START
        l_CastFlags |= CAST_FLAG_RUNE_LIST;                 ///< rune cooldowns list
    }

    if (m_spellInfo->HasEffect(SPELL_EFFECT_ACTIVATE_RUNE))
        l_CastFlags |= CAST_FLAG_RUNE_LIST;                 ///< rune cooldowns list

    if (m_targets.HasTraj())
        l_CastFlags |= CAST_FLAG_ADJUST_MISSILE;

    if (!m_spellInfo->StartRecoveryTime)
        l_CastFlags |= CAST_FLAG_NO_GCD;

    if (m_CastItemEntry)
        l_CastFlagsEx |= CastFlagsEx::CAST_FLAG_EX_TOY_COOLDOWN;

    /// Sniffed values - It triggers the bonus roll animation
    if (m_spellInfo->HasEffect(SpellEffects::SPELL_EFFECT_LOOT_BONUS))
    {
        l_CastFlags     = SpellCastFlags::CAST_FLAG_HAS_TRAJECTORY | SpellCastFlags::CAST_FLAG_NO_GCD;
        l_CastFlagsEx   = CastFlagsEx::CAST_FLAG_EX_UNK_5;
    }

    uint32 l_MissCount = 0;
    uint32 l_HitCount = 0;

    // Process targets data
    {
        // ---- Miss target ---- //
        for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if ((*ihit).effectMask == 0)                  // No effect apply - all immuned add state
                ihit->missCondition = SPELL_MISS_IMMUNE2;
        }

        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            if (ihit->missCondition != SPELL_MISS_NONE)        // Add only miss
                ++l_MissCount;

        // ---- Hit target ---- //
        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if ((*ihit).missCondition == SPELL_MISS_NONE)       // Add only hits
            {
                m_channelTargetEffectMask |= ihit->effectMask;
                ++l_HitCount;
            }
        }

        // Reset m_needAliveTargetMask for non channeled spell
        if (!m_spellInfo->IsChanneled())
            m_channelTargetEffectMask = 0;

        for (std::list<GOTargetInfo>::const_iterator ighit = m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end(); ++ighit)
            ++l_HitCount;
    }

    bool l_HasRuneData                  = m_caster->IsPlayer() && m_caster->getClass() == CLASS_DEATH_KNIGHT;

    // @TODO: Find how to trigger it, never find it in sniff (6.0.2)
    bool l_HasSpellCastLogData          = false;// (l_CastFlags & CAST_FLAG_PENDING) == 0 || m_spellInfo->AttributesEx7 & SPELL_ATTR7_SEND_CAST_LOG_DATA;

    Unit::PowerTypeSet l_UsablePowers  = m_caster->GetUsablePowers();
    uint32 l_ExtraTargetsCount         = m_targets.GetExtraTargetsCount();

    // Perdict data are empty in SMSG_SPELL_GO
    uint64 l_PredicOverrideTarget = 0;
    uint32 l_PredictAmount        = 0;
    uint8 l_PredicType            = 0;

    // Guids
    uint64 l_CasterGuid1    = m_castItemGUID ? m_castItemGUID : m_caster->GetGUID();
    uint64 l_CasterGuid2    = m_caster->GetGUID();
    uint64 l_TargetGUID     = m_targets.GetUnitTarget() ? m_targets.GetUnitTarget()->GetGUID() : 0;
    uint64 l_TargetItemGUID = itemTarget ? itemTarget->GetGUID() : 0;

    // Unknown
    bool l_HasUnk1 = m_spellInfo->Id == 178236;

    // Forge the packet !
    WorldPacket l_Data(SMSG_SPELL_GO, 8 * 1024);
    l_Data.appendPackGUID(l_CasterGuid1);
    l_Data.appendPackGUID(l_CasterGuid2);
    l_Data << uint8(m_cast_count);
    l_Data << uint32(m_spellInfo->Id);
    l_Data << uint32(m_spellInfo->FirstSpellXSpellVisualID);
    l_Data << uint32(l_CastFlags);
    l_Data << uint32(getMSTime());
    l_Data << uint32(l_HitCount);
    l_Data << uint32(l_MissCount);
    l_Data << uint32(l_MissCount);
    l_Data.WriteBits(m_targets.GetTargetMask(), 23);
    l_Data.WriteBit(m_targets.HasSrc());
    l_Data.WriteBit(m_targets.HasDst());
    l_Data.WriteBit(l_HasUnk1);
    l_Data.WriteBits(0, 7);                   ///< Src target name
    l_Data.FlushBits();

    l_Data.appendPackGUID(l_TargetGUID);
    l_Data.appendPackGUID(l_TargetItemGUID);

    if (m_targets.HasSrc())
    {
        float l_X, l_Y, l_Z;

        if (m_targets.GetSrc()->_transportGUID)
        {
            l_X = m_targets.GetSrc()->_transportOffset.m_positionX;
            l_Y = m_targets.GetSrc()->_transportOffset.m_positionY;
            l_Z = m_targets.GetSrc()->_transportOffset.m_positionZ;
        }
        else
        {
            l_X = m_targets.GetSrc()->_position.m_positionX;
            l_Y = m_targets.GetSrc()->_position.m_positionY;
            l_Z = m_targets.GetSrc()->_position.m_positionZ;
        }

        l_Data.appendPackGUID(m_targets.GetSrc()->_transportGUID);
        l_Data << float(l_X);
        l_Data << float(l_Y);
        l_Data << float(l_Z);
    }

    if (m_targets.HasDst())
    {
        float l_X, l_Y, l_Z;

        if (m_targets.GetDst()->_transportGUID)
        {
            l_X = m_targets.GetDst()->_transportOffset.m_positionX;
            l_Y = m_targets.GetDst()->_transportOffset.m_positionY;
            l_Z = m_targets.GetDst()->_transportOffset.m_positionZ;
        }
        else
        {
            l_X = m_targets.GetDst()->_position.m_positionX;
            l_Y = m_targets.GetDst()->_position.m_positionY;
            l_Z = m_targets.GetDst()->_position.m_positionZ;
        }

        l_Data.appendPackGUID(m_targets.GetDst()->_transportGUID);
        l_Data << float(l_X);
        l_Data << float(l_Y);
        l_Data << float(l_Z);
    }

    if (l_HasUnk1)
        l_Data << float(5.825109f);

    l_Data << uint32(l_UsablePowers.size());        ///< Remaining power count

    // Used in CMissile::AdjustAllMissileTrajectoryDurations
    // MissileTrajectory
    {
        l_Data << uint32(0);                        ///< Travel time
        l_Data << float(m_targets.GetElevation());  ///< Pitch
    }

    // Ammo_Struct
    {
        l_Data << int32(0);                         ///< DisplayID
        l_Data << int8(0);                          ///< InventoryType
    }

    l_Data << uint8(0);                             ///< DestLocSpellCastIndex (see Spell::AddProcessedDestLocSpellCast)

    l_Data << uint32(l_ExtraTargetsCount);

    // Immunities_Struct
    {
        l_Data << int32(0);                         ///< School
        l_Data << int32(0);                         ///< Value
    }

    l_Data << uint32(l_PredictAmount);
    l_Data << uint8(l_PredicType);
    l_Data.appendPackGUID(l_PredicOverrideTarget);

    // Send hit guid
    {
        // First units ...
        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            if ((*ihit).missCondition == SPELL_MISS_NONE)
                l_Data.appendPackGUID(ihit->targetGUID);

        // And GameObjects
        for (std::list<GOTargetInfo>::const_iterator ighit = m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end(); ++ighit)
            l_Data.appendPackGUID(ighit->targetGUID);
    }

    // Send missed guid
    {
        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            if ((*ihit).missCondition != SPELL_MISS_NONE)
                l_Data.appendPackGUID(ihit->targetGUID);
    }

    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition != SPELL_MISS_NONE)
        {
            l_Data.WriteBits(ihit->missCondition, 4);
            if (ihit->missCondition == SPELL_MISS_REFLECT)
                l_Data.WriteBits(ihit->reflectResult, 4);

            l_Data.FlushBits();
        }
    }

    // Remaining Power
    for (Unit::PowerTypeSet::const_iterator l_Itr = l_UsablePowers.begin(); l_Itr != l_UsablePowers.end(); l_Itr++)
    {
        Powers l_Power = Powers((*l_Itr));
        l_Data << int32(m_caster->GetPower(l_Power));
        l_Data << int8(l_Power);
    }

    // Target Points
    for (auto l_Itr : m_targets.GetExtraTargets())
    {
        l_Data.appendPackGUID(l_Itr._transportGUID);
        l_Data << float(l_Itr._position.GetPositionX());
        l_Data << float(l_Itr._position.GetPositionY());
        l_Data << float(l_Itr._position.GetPositionZ());
    }

    l_Data.WriteBits(l_CastFlagsEx, 20);                        ///< Cast flag ex
    l_Data.WriteBit(l_HasRuneData);                             ///< HasRuneData
    l_Data.FlushBits();

    // JamRuneData
    if (l_HasRuneData)
    {
        Player* l_DeathKnight = m_caster->ToPlayer();

        l_Data << uint8(m_runesState);                          ///< Start
        l_Data << uint8(l_DeathKnight->GetRunesState());        ///< Count
        l_Data.WriteBits(MAX_RUNES, 3);                         ///< Cooldowns lenght
        l_Data.FlushBits();

        for (uint8 l_I = 0; l_I < MAX_RUNES; ++l_I)
        {
            float l_BaseCooldown = float(l_DeathKnight->GetRuneBaseCooldown(l_I));
            uint8 l_Cooldown = uint8((l_BaseCooldown - float(l_DeathKnight->GetRuneCooldown(l_I))) / l_BaseCooldown * 255);

            l_Data << uint8(l_Cooldown);                        ///< Cooldowns
        }
    }

    l_Data.WriteBit(l_HasSpellCastLogData);

    // JamSpellCastLogData
    if (l_HasSpellCastLogData)
    {
        int32 l_Health      = m_caster->GetHealth();
        int32 l_AttackPower = m_caster->GetTotalAttackPowerValue(m_attackType);
        int32 l_SpellPower  = m_caster->SpellBaseDamageBonusDone(m_spellSchoolMask);

        l_Data << int32(l_Health);                              ///< Health
        l_Data << int32(l_AttackPower);                         ///< Attack power
        l_Data << int32(l_SpellPower);                          ///< Spell power

        uint32 l_PowerDataSize = l_UsablePowers.size();

        l_Data << uint32(l_PowerDataSize);
        for (Unit::PowerTypeSet::const_iterator l_Itr = l_UsablePowers.begin(); l_Itr != l_UsablePowers.end(); l_Itr++)
        {
            Powers l_Power = Powers((*l_Itr));
            l_Data << int32(l_Power);                           ///< Power type
            l_Data << int32(m_caster->GetPower(l_Power));       ///< Amount
        }

        bool l_HasUnknowFloat = false;

        l_Data.WriteBit(l_HasUnknowFloat);
        if (l_HasUnknowFloat)
            l_Data << float(0.0f);
    }

    m_caster->SendMessageToSet(&l_Data, true);
}

void Spell::SendLogExecute()
{
    if (m_effectExecuteData.empty())
        return;

    WorldPacket l_Data(SMSG_SPELL_EXECUTE_LOG, 1024);

    l_Data.appendPackGUID(m_caster->GetGUID());
    l_Data << uint32(m_spellInfo->Id);
    l_Data << uint32(m_effectExecuteData.size());

    for (auto l_Iter : m_effectExecuteData)
    {
        auto l_EffIndex = l_Iter.first;
        auto l_Helper = l_Iter.second;

        l_Data << uint32(m_spellInfo->Effects[l_EffIndex].Effect);      ///< Effect
        l_Data << uint32(l_Helper.Energizes.size());                    ///< PowerDrainTargets
        l_Data << uint32(l_Helper.ExtraAttacks.size());                 ///< ExtraAttacksTargets
        l_Data << uint32(l_Helper.DurabilityDamages.size());            ///< DurabilityDamageTargets
        l_Data << uint32(l_Helper.Targets.size());                      ///< GenericVictimTargets
        l_Data << uint32(l_Helper.CreatedItems.size());                 ///< TradeSkillTargets
        l_Data << uint32(0);                                            ///< FeedPetTargets

        for (auto l_Energyze : l_Helper.Energizes)
        {
            l_Data.appendPackGUID(l_Energyze.Guid);
            l_Data << uint32(l_Energyze.Value);
            l_Data << uint32(l_Energyze.PowerType);
            l_Data << float(l_Energyze.Multiplier);
        }

        for (auto l_ExtraAttacksTarget : l_Helper.ExtraAttacks)
        {
            l_Data.appendPackGUID(l_ExtraAttacksTarget.Victim);
            l_Data << uint32(l_ExtraAttacksTarget.NumAttacks);
        }

        for (auto l_DurabilityDamage : l_Helper.DurabilityDamages)
        {
            l_Data.appendPackGUID(l_DurabilityDamage.Victim);
            l_Data << uint32(l_DurabilityDamage.ItemID);
            l_Data << uint32(l_DurabilityDamage.Amount);
        }

        for (auto l_Guid : l_Helper.Targets)
            l_Data.appendPackGUID(l_Guid);

        for (auto l_ItemID : l_Helper.CreatedItems)
            l_Data << uint32(l_ItemID);
    }

    l_Data.WriteBit(false); ///< HasLogData
    l_Data.FlushBits();
    m_caster->SendMessageToSet(&l_Data, true);

    m_effectExecuteData.clear();
}

void Spell::ExecuteLogEffectTakeTargetPower(uint8 effIndex, Unit* target, uint32 powerType, uint32 powerTaken, float gainMultiplier)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddEnergize(target->GetGUID(), gainMultiplier, powerTaken, powerType);
}

void Spell::ExecuteLogEffectExtraAttacks(uint8 effIndex, Unit* victim, uint32 attCount)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddExtraAttack(victim->GetGUID(), attCount);
}

void Spell::ExecuteLogEffectInterruptCast(uint8 effIndex, Unit* victim, uint32 spellId)
{
    /// Why is commented ?
    /*InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex]->append(victim->GetPackGUID());
    *m_effectExecuteData[effIndex] << uint32(spellId);*/
}

void Spell::ExecuteLogEffectDurabilityDamage(uint8 effIndex, Unit* victim, uint32 p_ItemID, uint32 damage)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddDurabilityDamage(victim->GetGUID(), p_ItemID, damage);
}

void Spell::ExecuteLogEffectOpenLock(uint8 effIndex, Object* obj)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(obj->GetGUID());
}

void Spell::ExecuteLogEffectCreateItem(uint8 effIndex, uint32 entry)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddCreatedItem(entry);
}

void Spell::ExecuteLogEffectDestroyItem(uint8 effIndex, uint32 entry)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddCreatedItem(entry);
}

void Spell::ExecuteLogEffectSummonObject(uint8 effIndex, WorldObject* obj)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(obj->GetGUID());
}

void Spell::ExecuteLogEffectUnsummonObject(uint8 effIndex, WorldObject* obj)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(obj->GetGUID());
}

void Spell::ExecuteLogEffectResurrect(uint8 effIndex, Unit* target)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(target->GetGUID());
}

void Spell::SendInterrupted(uint8 p_Result)
{
    WorldPacket l_Data(SMSG_SPELL_FAILURE, 16 + 2 + 1 + 4 + 2);
    l_Data.appendPackGUID(m_caster->GetGUID());
    l_Data << uint8(m_cast_count);
    l_Data << uint32(m_spellInfo->Id);
    l_Data << uint16(p_Result);

    m_caster->SendMessageToSet(&l_Data, true);

    l_Data.Initialize(SMSG_SPELL_FAILED_OTHER, 16 + 2 + 1 + 4 + 1);
    l_Data.appendPackGUID(m_caster->GetGUID());
    l_Data << uint8(m_cast_count);
    l_Data << uint32(m_spellInfo->Id);
    l_Data << uint8(p_Result);

    m_caster->SendMessageToSet(&l_Data, true);
}

void Spell::SendChannelUpdate(uint32 p_Time)
{
    if (p_Time == 0)
    {
        //m_caster->SetGuidValue(UNIT_FIELD_CHANNEL_OBJECT, 0);
        m_caster->SetChannelSpellID(nullptr);
    }

    WorldPacket l_Data(SMSG_SPELL_CHANNEL_UPDATE, 16 + 2 + 4);
    l_Data.appendPackGUID(m_caster->GetGUID());
    l_Data << uint32(p_Time);

    m_caster->SendMessageToSet(&l_Data, true);
}

void Spell::SendChannelStart(uint32 p_Duration)
{
    uint64 l_ChannelTarget = m_targets.GetObjectTargetGUID();

    if (!l_ChannelTarget && !m_spellInfo->NeedsExplicitUnitTarget())
    {
        /// this is for TARGET_SELECT_CATEGORY_NEARBY
        if (m_UniqueTargetInfo.size() + m_UniqueGOTargetInfo.size() == 1)
        {
            if (!m_UniqueTargetInfo.empty())
                l_ChannelTarget = !m_UniqueTargetInfo.empty() ? m_UniqueTargetInfo.front().targetGUID : m_UniqueGOTargetInfo.front().targetGUID;
        }
    }

    bool l_HaveSpellTargetedHealPrediction = false;
    bool l_HaveSpellChannelStartInterruptImmunities = false;

    WorldPacket l_Data(SMSG_SPELL_CHANNEL_START, (16 + 4 + 4 + 1) + (l_HaveSpellTargetedHealPrediction ? 4 + 1 + 16 + 16 : 0) + (l_HaveSpellChannelStartInterruptImmunities ? 4 + 4 : 0));
    l_Data.appendPackGUID(m_caster->GetGUID());
    l_Data << uint32(m_spellInfo->Id);
    l_Data << uint32(p_Duration);
    l_Data.WriteBit(l_HaveSpellChannelStartInterruptImmunities);
    l_Data.WriteBit(l_HaveSpellTargetedHealPrediction);
    l_Data.FlushBits();

    if (l_HaveSpellChannelStartInterruptImmunities)
    {
        l_Data << uint32(0);                  ///< SchoolImmunities
        l_Data << uint32(0);                  ///< Immunities
    }

    if (l_HaveSpellTargetedHealPrediction)
    {
        l_Data.appendPackGUID(0);             ///< TargetGUID
        l_Data << uint32(0);                  ///< Points
        l_Data << uint8(0);                   ///< Type
        l_Data.appendPackGUID(0);             ///< BeaconGUID
    }

    m_caster->SendMessageToSet(&l_Data, true);

    m_timer = p_Duration;

    if (l_ChannelTarget)
        m_caster->SetGuidValue(UNIT_FIELD_CHANNEL_OBJECT, l_ChannelTarget);

    /// 101546 Spinning Crane Kick
    if (m_spellInfo->Id != 101546)
        m_caster->SetChannelSpellID(m_spellInfo->Id);
}

void Spell::SendResurrectRequest(Player* p_Target)
{
    // get ressurector name for creature resurrections, otherwise packet will be not accepted
    // for player resurrections the name is looked up by guid
    char const* l_RessurectorName = m_caster->IsPlayer() ? "" : m_caster->GetNameForLocaleIdx(p_Target->GetSession()->GetSessionDbLocaleIndex());

    WorldPacket l_Data(SMSG_RESURRECT_REQUEST, 16 + 2 + 4 + 4 + 4 + 1 + strlen(l_RessurectorName) + 1);
    l_Data.appendPackGUID(m_caster->GetGUID());
    l_Data << uint32(m_spellInfo->Id);
    l_Data << uint32(0);
    l_Data << uint32(0);
    l_Data.WriteBits(strlen(l_RessurectorName), 6);
    l_Data.WriteBit(false);
    l_Data.WriteBit(m_caster->IsPlayer() ? 0 : 1);
    l_Data.FlushBits();
    l_Data.WriteString(l_RessurectorName);
    p_Target->GetSession()->SendPacket(&l_Data);
}

void Spell::TakeCastItem()
{
    if (!m_CastItem || m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    // not remove cast item at triggered spell (equipping, weapon damage, etc)
    if (_triggeredCastFlags & TRIGGERED_IGNORE_CAST_ITEM)
        return;

    ItemTemplate const* proto = m_CastItem->GetTemplate();

    if (!proto)
    {
        // This code is to avoid a crash
        // I'm not sure, if this is really an error, but I guess every item needs a prototype
        return;
    }

    bool expendable = false;
    bool withoutCharges = false;

    for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
    {
        if (proto->Spells[i].SpellId)
        {
            // item has limited charges
            if (proto->Spells[i].SpellCharges)
            {
                if (proto->Spells[i].SpellCharges < 0)
                    expendable = true;

                int32 charges = m_CastItem->GetSpellCharges(i);

                // item has charges left
                if (charges)
                {
                    (charges > 0) ? --charges : ++charges;  // abs(charges) less at 1 after use
                    if (proto->Stackable == 1)
                        m_CastItem->SetSpellCharges(i, charges);
                    m_CastItem->SetState(ITEM_CHANGED, (Player*)m_caster);
                }

                // all charges used
                withoutCharges = (charges == 0);
            }
        }
    }

    if (expendable && withoutCharges)
    {
        uint32 count = 1;
        m_caster->ToPlayer()->DestroyItemCount(m_CastItem, count, true);

        // prevent crash at access to deleted m_targets.GetItemTarget
        if (m_CastItem == m_targets.GetItemTarget())
            m_targets.SetItemTarget(NULL);

        m_CastItem = NULL;
    }
}

void Spell::TakePower()
{
    if (m_CastItem || m_triggeredByAuraSpell)
        return;

    // Don't take power if the spell is cast while .cheat power is enabled.
    if (m_caster->IsPlayer())
    {
        if (m_caster->ToPlayer()->GetCommandStatus(CHEAT_POWER))
            return;
    }
    else if (m_caster->GetEntry() == 69680 || m_caster->GetEntry() == 69792 || m_caster->GetEntry() == 69791)
        return;

    if (m_spellInfo->SpellPowers.empty())
        return;

    for (auto itr : m_spellInfo->SpellPowers)
    {
        Powers powerType = Powers(itr->PowerType);
        bool hit = true;
        if (m_caster->IsPlayer())
        {
            if (powerType == POWER_HOLY_POWER || powerType == POWER_ENERGY || powerType == POWER_RUNES || powerType == POWER_CHI ||
                powerType == POWER_SHADOW_ORB || powerType == POWER_BURNING_EMBERS || powerType == POWER_DEMONIC_FURY || powerType == POWER_SOUL_SHARDS)
            {
                if (uint64 targetGUID = m_targets.GetUnitTargetGUID())
                {
                    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                    {
                        if (ihit->targetGUID == targetGUID)
                        {
                            if (ihit->missCondition != SPELL_MISS_NONE)
                            {
                                hit = false;
                                //lower spell cost on fail (by talent aura)
                                if (Player* modOwner = m_caster->ToPlayer()->GetSpellModOwner())
                                    modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_SPELL_COST_REFUND_ON_FAIL, m_powerCost[POWER_TO_INDEX(powerType)]);
                            }
                            break;
                        }
                    }
                }
            }
        }

        if (powerType == POWER_RUNES)
        {
            TakeRunePower(hit);
            continue;
        }

        if (!m_powerCost[POWER_TO_INDEX(powerType)])
            continue;

        // Check requiredAura for power which area restricted to this
        if (itr->RequiredAuraSpellId && !m_caster->HasAura(itr->RequiredAuraSpellId))
            continue;

        // health as power used
        if (powerType == POWER_HEALTH)
        {
            m_caster->ModifyHealth(-(int32)m_powerCost[POWER_TO_INDEX(powerType)]);
            continue;
        }

        if (powerType >= MAX_POWERS)
        {
            sLog->outAshran("Spel::TakePower -> Unknown power type [%u]", powerType);
            continue;
        }

        int32 pct = 20;
        if (powerType == POWER_RUNIC_POWER)
            pct = 10;

        /// WoD Custom Script - Arcane Blast mana cost with Arcane Charge and Improved Arcane Power
        if (powerType == POWER_MANA && m_spellInfo && m_spellInfo->Id == 30451 && m_caster->HasAura(157604))
        {
            /// Arcane Charge increase mana cost of Arcane Blast
            if (Aura* l_ArcaneCharge = m_caster->GetAura(36032))
            {
                /// Base Arcane Blast mana cost
                int32 l_PowerCost = 3200;
                /// For every stack of arcane charge +100% of mana cost
                uint8 l_ArcaneChargeStacks = l_ArcaneCharge->GetStackAmount() + 1;
                l_PowerCost = int32(l_PowerCost * l_ArcaneChargeStacks);
                /// Reduce mana cost for 10% by Improved Arcane Power just if we have Arcane Power buff
                if (m_caster->HasAura(12042))
                    l_PowerCost = CalculatePct(l_PowerCost, 90);

                m_powerCost[0] = l_PowerCost;
            }
        }

        if (hit)
            m_caster->ModifyPower(powerType, -m_powerCost[POWER_TO_INDEX(powerType)]);
        else
            m_caster->ModifyPower(powerType, -CalculatePct(m_powerCost[POWER_TO_INDEX(powerType)], pct)); // Refund 80% of power on fail 4.x
    }
}

void Spell::TakeAmmo()
{
    if (m_attackType == WeaponAttackType::RangedAttack && m_caster->IsPlayer())
    {
        Item* pItem = m_caster->ToPlayer()->GetWeaponForAttack(WeaponAttackType::RangedAttack);

        // wands don't have ammo
        if (!pItem  || pItem->CantBeUse() || pItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_WAND)
            return;

        if (pItem->GetTemplate()->InventoryType == INVTYPE_THROWN)
        {
            if (pItem->GetMaxStackCount() == 1)
            {
                // decrease durability for non-stackable throw weapon
                m_caster->ToPlayer()->DurabilityPointLossForEquipSlot(EQUIPMENT_SLOT_MAINHAND);
            }
            else
            {
                // decrease items amount for stackable throw weapon
                uint32 count = 1;
                m_caster->ToPlayer()->DestroyItemCount(pItem, count, true);
            }
        }
    }
}

SpellCastResult Spell::CheckRuneCost(uint32 runeCostID)
{
    bool runesOk = false;
    for (auto itr : m_spellInfo->SpellPowers)
    {
        if (itr->PowerType == POWER_RUNES)
        {
            runesOk = false;
            break;
        }
        else if (itr->PowerType != POWER_RUNES)
            runesOk = true;
    }

    if (runesOk || !runeCostID)
        return SPELL_CAST_OK;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return SPELL_CAST_OK;

    Player* player = (Player*)m_caster;

    if (player->getClass() != CLASS_DEATH_KNIGHT)
        return SPELL_CAST_OK;

    SpellRuneCostEntry const* src = sSpellRuneCostStore.LookupEntry(runeCostID);

    if (!src)
        return SPELL_CAST_OK;

    if (src->NoRuneCost())
        return SPELL_CAST_OK;

    int32 runeCost[NUM_RUNE_TYPES];                         // blood, frost, unholy, death

    for (uint32 i = 0; i < NUM_RUNE_TYPES; ++i)
    {
        runeCost[i] = src->RuneCost[i];
        if (Player* modOwner = m_caster->GetSpellModOwner())
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_COST, runeCost[i], this, false);
    }

    runeCost[RUNE_DEATH] = MAX_RUNES;                       // calculated later

    for (uint32 i = 0; i < MAX_RUNES; ++i)
    {
        RuneType rune = player->GetCurrentRune(i);
        if ((player->GetRuneCooldown(i) == 0) && (runeCost[rune] > 0))
            runeCost[rune]--;
    }

    for (uint32 i = 0; i < RUNE_DEATH; ++i)
        if (runeCost[i] > 0)
            runeCost[RUNE_DEATH] += runeCost[i];

    if (runeCost[RUNE_DEATH] > MAX_RUNES)
        return SPELL_FAILED_NO_POWER;                       // not sure if result code is correct

    return SPELL_CAST_OK;
}

void Spell::TakeRunePower(bool didHit)
{
    if (m_caster->GetTypeId() != TYPEID_PLAYER || m_caster->getClass() != CLASS_DEATH_KNIGHT)
        return;

    SpellRuneCostEntry const* runeCostData = sSpellRuneCostStore.LookupEntry(m_spellInfo->RuneCostID);
    if (!runeCostData || (runeCostData->NoRuneCost() && runeCostData->NoRunicPowerGain()))
        return;

    Player* player = m_caster->ToPlayer();
    m_runesState = player->GetRunesState();                 // store previous state

    int32 runeCost[NUM_RUNE_TYPES];                         // blood, frost, unholy, death
    SpellSchools school = GetFirstSchoolInMask(m_spellSchoolMask);

    for (uint32 i = 0; i < NUM_RUNE_TYPES; ++i)
    {
        runeCost[i] = runeCostData->RuneCost[i];
        if (Player* modOwner = m_caster->GetSpellModOwner())
        {
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_COST, runeCost[i], this);

            // PCT mod from user auras by school
            runeCost[i] = int32(runeCost[i] * (1.0f + m_caster->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + school)));
            if (runeCost[i] < 0)
                runeCost[i] = 0;
        }
        sScriptMgr->OnModifyPower(player, POWER_RUNES, 0, runeCost[i], false);
    }

    /* In MOP there is a some spell, that use death rune, so don't reset it*/
    //runeCost[RUNE_DEATH] = 0;                               // calculated later

    bool gain_runic = runeCostData->NoRuneCost();           //  if spell doesn't have runecost - player can have some runic power, Horn of Winter for example
    for (uint32 i = 0; i < MAX_RUNES; ++i)
    {
        RuneType rune = player->GetCurrentRune(i);
        if (player->GetRuneCooldown(i) || !runeCost[rune])
            continue;

        uint32 cooldown = ((m_spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_DK_DEATH_STRIKE) > 0 || didHit) ? player->GetRuneBaseCooldown(i) : uint32(RUNE_MISS_COOLDOWN);
        player->SetRuneCooldown(i, cooldown);
        player->SetDeathRuneUsed(i, false);

        switch (m_spellInfo->Id)
        {
            case 45477: ///< Icy Touch
            case 50842: ///< Blood Boil
            case 85948: ///< Festering Strike
            {
                // Reaping
                if (player->HasAura(56835))
                    player->AddRuneBySpell(i, RUNE_DEATH, 56835);
                break;
            }
            default:
                break;
        }

        runeCost[rune]--;
        gain_runic = true;
    }

    runeCost[RUNE_DEATH] += runeCost[RUNE_BLOOD] + runeCost[RUNE_UNHOLY] + runeCost[RUNE_FROST];

    if (runeCost[RUNE_DEATH] > 0)
    {
        for (uint32 i = 0; i < MAX_RUNES; ++i)
        {
            RuneType rune = player->GetCurrentRune(i);
            if (!player->GetRuneCooldown(i) && rune == RUNE_DEATH)
            {
                uint32 cooldown = ((m_spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_DK_DEATH_STRIKE) > 0 || didHit) ? player->GetRuneBaseCooldown(i) : uint32(RUNE_MISS_COOLDOWN);
                player->SetRuneCooldown(i, cooldown);
                runeCost[rune]--;

                gain_runic = true;

                bool takePower = didHit;
                if (uint32 spell = player->GetRuneConvertSpell(i))
                    takePower = spell != 54637;

                if (player->IsRunePermanentlyConverted(i))
                    takePower = false;

                // keep Death Rune type if missed or player has Blood of the North or rune is permanently converted
                if (takePower)
                {
                    player->RestoreBaseRune(i);
                    player->SetDeathRuneUsed(i, true);
                }

                switch (m_spellInfo->Id)
                {
                    case 45477: ///< Icy Touch
                    case 50842: ///< Blood Boil
                    case 85948: ///< Festering Strike
                    {
                        // Reaping
                        if (player->HasAura(56835))
                            player->AddRuneBySpell(i, RUNE_DEATH, 56835);
                        break;
                    }
                    default:
                        break;
                }

                if (runeCost[RUNE_DEATH] == 0)
                    break;
            }
        }
    }

    // you can gain some runic power when use runes
    if (didHit && gain_runic)
    {
        if (int32 rp = int32(runeCostData->runePowerGain * sWorld->getRate(RATE_POWER_RUNICPOWER_INCOME)))
        {
            AddPct(rp, player->GetTotalAuraModifier(SPELL_AURA_MOD_RUNE_REGEN_SPEED));
            player->ModifyPower(POWER_RUNIC_POWER, int32(rp));
        }
    }
}

void Spell::TakeReagents()
{
    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    ItemTemplate const* castItemTemplate = m_CastItem ? m_CastItem->GetTemplate() : NULL;

    // do not take reagents for these item casts
    if (castItemTemplate && castItemTemplate->Flags & ITEM_FLAG_TRIGGERED_CAST)
        return;

    Player* p_caster = m_caster->ToPlayer();
    if (p_caster->CanNoReagentCast(m_spellInfo))
        return;

    for (uint32 x = 0; x < MAX_SPELL_REAGENTS; ++x)
    {
        if (m_spellInfo->Reagent[x] <= 0)
            continue;

        uint32 itemid = m_spellInfo->Reagent[x];
        uint32 itemcount = m_spellInfo->ReagentCount[x];

        // if CastItem is also spell reagent
        if (castItemTemplate && castItemTemplate->ItemId == itemid)
        {
            for (int s = 0; s < MAX_ITEM_PROTO_SPELLS; ++s)
            {
                // CastItem will be used up and does not count as reagent
                int32 charges = m_CastItem->GetSpellCharges(s);
                if (castItemTemplate->Spells[s].SpellCharges < 0 && abs(charges) < 2)
                {
                    ++itemcount;
                    break;
                }
            }

            m_CastItem = NULL;
        }

        // if GetItemTarget is also spell reagent
        if (m_targets.GetItemTargetEntry() == itemid)
            m_targets.SetItemTarget(NULL);

        p_caster->DestroyItemCount(itemid, itemcount, true);
    }

    if (m_spellInfo->CurrencyID && m_spellInfo->CurrencyCount > 0)
        p_caster->ModifyCurrency(m_spellInfo->CurrencyID, -int32(m_spellInfo->CurrencyCount));
}

void Spell::HandleThreatSpells()
{
    if (m_UniqueTargetInfo.empty())
        return;

    if ((m_spellInfo->AttributesEx  & SPELL_ATTR1_NO_THREAT) ||
        (m_spellInfo->AttributesEx3 & SPELL_ATTR3_NO_INITIAL_AGGRO))
        return;

    float threat = 0.0f;
    if (SpellThreatEntry const* threatEntry = sSpellMgr->GetSpellThreatEntry(m_spellInfo->Id))
    {
        if (threatEntry->apPctMod != 0.0f)
            threat += threatEntry->apPctMod * m_caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);

        threat += threatEntry->flatMod;
    }
    else if ((m_spellInfo->AttributesCu & SPELL_ATTR0_CU_NO_INITIAL_THREAT) == 0)
        threat += m_spellInfo->SpellLevel;

    // past this point only multiplicative effects occur
    if (threat == 0.0f)
        return;

    // since 2.0.1 threat from positive effects also is distributed among all targets, so the overall caused threat is at most the defined bonus
    threat /= m_UniqueTargetInfo.size();

    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition != SPELL_MISS_NONE)
            continue;

        Unit* target = ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID);
        if (!target)
            continue;

        // positive spells distribute threat among all units that are in combat with target, like healing
        if (m_spellInfo->_IsPositiveSpell())
            target->getHostileRefManager().threatAssist(m_caster, threat, m_spellInfo);
        // for negative spells threat gets distributed among affected targets
        else
        {
            if (!target->CanHaveThreatList())
                continue;

            target->AddThreat(m_caster, threat, m_spellInfo->GetSchoolMask(), m_spellInfo);
        }
    }
}

void Spell::HandleHolyPower(Player* caster)
{
    if (!caster)
        return;

    // Templar's Verdict and Eternal Flame - Don't remove power twice
    if (m_spellInfo->Id == 85256 || m_spellInfo->Id == 114163)
        return;

    // Word of Glory - Don't remove power twice (idem for Light of Dawn)
    if (m_spellInfo->Id == 85673 || m_spellInfo->Id == 85222)
        return;

    // Shield of the Righteous - Don't remove power twice
    if (m_spellInfo->Id == 53600)
        return;

    bool hit = true;
    Player* modOwner = caster->GetSpellModOwner();
    m_powerCost[POWER_TO_INDEX(POWER_HOLY_POWER)] = caster->GetPower(POWER_HOLY_POWER); // Always use all the holy power we have
    if (!m_powerCost[POWER_TO_INDEX(POWER_HOLY_POWER)] || !modOwner)
        return;

    if (uint64 targetGUID = m_targets.GetUnitTargetGUID())
    {
        for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if (ihit->targetGUID == targetGUID)
            {
                if (ihit->missCondition != SPELL_MISS_NONE && ihit->missCondition != SPELL_MISS_MISS)
                    hit = false;
                break;
            }
        }

        // The spell did hit the target, apply aura cost mods if there are any.
        if (hit)
        {
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_COST, m_powerCost[POWER_TO_INDEX(POWER_HOLY_POWER)]);
            m_caster->ModifyPower(POWER_HOLY_POWER, -m_powerCost[POWER_TO_INDEX(POWER_HOLY_POWER)]);
        }
    }
}

void Spell::HandleEffects(Unit* p_UnitTarget, Item* p_ItemTarget, GameObject* p_GOTarget, AreaTrigger* p_AreaTrigger, uint32 p_I, SpellEffectHandleMode p_Mode)
{
    effectHandleMode = p_Mode;
    unitTarget       = p_UnitTarget;
    itemTarget       = p_ItemTarget;
    gameObjTarget    = p_GOTarget;
    m_AreaTrigger    = p_AreaTrigger;
    destTarget       = &m_destTargets[p_I]._position;

    uint8 l_Effect = m_spellInfo->Effects[p_I].Effect;
    damage = CalculateDamage(p_I, unitTarget);

    /// Prevent recalculating base damage for every posible target in case of AoE spells 
    /// @TODO_SOVAK: Rewrite this shit! :P
    /*if (p_Mode == SPELL_EFFECT_HANDLE_HIT || p_Mode == SPELL_EFFECT_HANDLE_LAUNCH)
        damage = CalculateDamage(p_I, unitTarget, p_Mode == SPELL_EFFECT_HANDLE_LAUNCH_TARGET);*/

    bool l_PreventDefault = CallScriptEffectHandlers((SpellEffIndex)p_I, p_Mode);
    if (!l_PreventDefault && l_Effect < TOTAL_SPELL_EFFECTS)
        (this->*SpellEffects[l_Effect])((SpellEffIndex)p_I);
}

SpellCastResult Spell::CheckCast(bool strict)
{
    // Hack fixing Skulloc LOS issue with the bridge sequence
    if (m_spellInfo->Id == 168539 || m_spellInfo->Id == 168540)
        return SPELL_CAST_OK;

    /// Hacky fix for Herb Gathering exploit
    if (m_spellInfo->Id == 2368 && m_targets.GetGOTarget() && m_targets.GetGOTarget()->GetEntry() == 209059)
        return SPELL_FAILED_BAD_TARGETS;

    // Custom Spell_failed
    if (m_spellInfo->IsCustomCastCanceled(m_caster))
        return SPELL_FAILED_DONT_REPORT;

    /// Combat Resurrection spell
    if (m_spellInfo->IsBattleResurrection())
    {
        if (InstanceScript* l_InstanceScript = m_caster->GetInstanceScript())
        {
            if (!l_InstanceScript->CanUseCombatResurrection())
                return SPELL_FAILED_IN_COMBAT_RES_LIMIT_REACHED;
        }

        if (m_targets.GetUnitTarget() != nullptr)
        {
            if (Player* l_Target = m_targets.GetUnitTarget()->ToPlayer())
            {
                if (l_Target->IsRessurectRequested())
                    return SPELL_FAILED_TARGET_HAS_RESURRECT_PENDING;
            }
        }
    }

    /// Check specialization
    if (!IsTriggered() && !sWorld->getBoolConfig(CONFIG_DISABLE_SPELL_SPECIALIZATION_CHECK) && !IsDarkSimulacrum())
    {
        if (Player* l_Player = m_caster->ToPlayer())
        {
            uint32 l_CasterSpecialization = l_Player->GetSpecializationId();
            
            if (!m_spellInfo->SpecializationIdList.empty())
            {
                bool l_Found = false;
                for (uint32 l_Specialization : m_spellInfo->SpecializationIdList)
                {
                    if (l_CasterSpecialization == l_Specialization)
                    {
                        l_Found = true;
                        break;
                    }
                }

                if (!l_Found)
                    return SpellCastResult::SPELL_FAILED_SPELL_UNAVAILABLE;
            }
        }
    }

    // Check death state
    if (!m_caster->isAlive() && !(m_spellInfo->Attributes & SPELL_ATTR0_PASSIVE) && !((m_spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD) || (IsTriggered() && !m_triggeredByAuraSpell)))
        return SPELL_FAILED_CASTER_DEAD;

    if (m_spellInfo->HasEffect(SPELL_EFFECT_RESURRECT_WITH_AURA) && m_caster->GetInstanceScript() && m_caster->GetInstanceScript()->IsEncounterInProgress())
        return SPELL_FAILED_TARGET_IN_COMBAT;

    // Check cooldowns to prevent cheating
    if (m_caster->IsPlayer() && !(m_spellInfo->Attributes & SPELL_ATTR0_PASSIVE))
    {
        Player* l_Player = m_caster->ToPlayer();

        SpellInfo const* l_SpellInfo = nullptr;
        if (m_caster->HasAura(46924)) ///< Bladestorm
            l_SpellInfo = sSpellMgr->GetSpellInfo(46924);

        Unit::AuraEffectList const& l_AuraEffects = m_caster->GetAuraEffectsByType(SPELL_AURA_ALLOW_ONLY_ABILITY);
        for (Unit::AuraEffectList::const_iterator l_AuraEffect = l_AuraEffects.begin(); l_AuraEffect != l_AuraEffects.end(); ++l_AuraEffect)
        {
            if (l_SpellInfo && (*l_AuraEffect)->IsAffectingSpell(m_spellInfo))
                _triggeredCastFlags = TriggerCastFlags(uint32(_triggeredCastFlags) | TRIGGERED_IGNORE_CASTER_AURASTATE);
        }

        //can cast triggered (by aura only?) spells while have this flag
        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_AURASTATE) && !(_triggeredCastFlags & TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD) && l_Player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_ALLOW_ONLY_ABILITY) &&
            m_caster->GetCurrentSpell(CURRENT_GENERIC_SPELL) &&
            (!(m_spellInfo->AttributesEx9 & SPELL_ATTR9_CASTABLE_WHILE_CAST_IN_PROGRESS) || GetSpellInfo()->CalcCastTime(m_caster)))
            return SPELL_FAILED_SPELL_IN_PROGRESS;

        if (l_Player->HasSpellCooldown(m_spellInfo->Id) && !l_Player->HasAuraTypeWithAffectMask(SPELL_AURA_ALLOW_CAST_WHILE_IN_COOLDOWN, m_spellInfo))
        {
            if (m_triggeredByAuraSpell)
                return SPELL_FAILED_DONT_REPORT;
            else
                return SPELL_FAILED_NOT_READY;
        }

        if (!l_Player->HasCharge(m_spellInfo->ChargeCategoryEntry))
            return m_triggeredByAuraSpell ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_NOT_READY;

        // check if we are using a potion in combat for the 2nd+ time. Cooldown is added only after caster gets out of combat
        if (l_Player->GetLastPotionId() && m_CastItem && (m_CastItem->IsPotion() || m_spellInfo->IsCooldownStartedOnEvent()))
            return SPELL_FAILED_NOT_READY;
    }

    if (m_spellInfo->AttributesEx7 & SPELL_ATTR7_IS_CHEAT_SPELL && !m_caster->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_CHEAT_SPELLS))
    {
        m_customError = SPELL_CUSTOM_ERROR_GM_ONLY;
        return SPELL_FAILED_CUSTOM_ERROR;
    }

    // Check global cooldown
    if (strict && !(_triggeredCastFlags & TRIGGERED_IGNORE_GCD) && !(_triggeredCastFlags & TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD) && HasGlobalCooldown() && !m_spellInfo->DoesIgnoreGlobalCooldown(m_caster))
        return SPELL_FAILED_NOT_READY;

    // only triggered spells can be processed an ended battleground
    if (!IsTriggered() && m_caster->IsPlayer())
        if (Battleground* bg = m_caster->ToPlayer()->GetBattleground())
            if (bg->GetStatus() == STATUS_WAIT_LEAVE)
                return SPELL_FAILED_DONT_REPORT;

    if (m_caster->IsPlayer() && VMAP::VMapFactory::createOrGetVMapManager()->isLineOfSightCalcEnabled())
    {
        if (m_spellInfo->Attributes & SPELL_ATTR0_OUTDOORS_ONLY &&
            !m_caster->IsOutdoors())
            return SPELL_FAILED_ONLY_OUTDOORS;

        if (m_spellInfo->Attributes & SPELL_ATTR0_INDOORS_ONLY &&
                m_caster->IsOutdoors())
            return SPELL_FAILED_ONLY_INDOORS;
    }

    // only check at first call, Stealth auras are already removed at second call
    // for now, ignore triggered spells
    if (strict && !(_triggeredCastFlags & TRIGGERED_IGNORE_SHAPESHIFT))
    {
        bool checkForm = true;
        // Ignore form req aura
        Unit::AuraEffectList const& ignore = m_caster->GetAuraEffectsByType(SPELL_AURA_MOD_IGNORE_SHAPESHIFT);
        for (Unit::AuraEffectList::const_iterator i = ignore.begin(); i != ignore.end(); ++i)
        {
            if (!(*i)->IsAffectingSpell(m_spellInfo))
                continue;
            checkForm = false;
            break;
        }
        if (checkForm)
        {
            // Cannot be used in this stance/form
            SpellCastResult shapeError = m_spellInfo->CheckShapeshift(m_caster->GetShapeshiftForm());
            if (shapeError != SPELL_CAST_OK)
                return shapeError;

            if ((m_spellInfo->Attributes & SPELL_ATTR0_ONLY_STEALTHED) && !(m_caster->HasStealthAura()))
                return SPELL_FAILED_ONLY_STEALTHED;
        }
    }

    Unit::AuraEffectList const& blockSpells = m_caster->GetAuraEffectsByType(SPELL_AURA_BLOCK_SPELL_FAMILY);
    for (Unit::AuraEffectList::const_iterator blockItr = blockSpells.begin(); blockItr != blockSpells.end(); ++blockItr)
        if (uint32((*blockItr)->GetMiscValue()) == m_spellInfo->SpellFamilyName)
            return SPELL_FAILED_SPELL_UNAVAILABLE;

    bool reqCombat = true;
    Unit::AuraEffectList const& stateAuras = m_caster->GetAuraEffectsByType(SPELL_AURA_ABILITY_IGNORE_AURASTATE);
    for (Unit::AuraEffectList::const_iterator j = stateAuras.begin(); j != stateAuras.end(); ++j)
    {
        if ((*j)->IsAffectingSpell(m_spellInfo))
        {
            m_needComboPoints = false;
            if ((*j)->GetMiscValue() == 1)
            {
                reqCombat=false;
                break;
            }
        }
    }

    // caster state requirements
    // not for triggered spells (needed by execute)
    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_AURASTATE))
    {
        if (m_spellInfo->CasterAuraState && !m_caster->HasAuraState(AuraStateType(m_spellInfo->CasterAuraState), m_spellInfo, m_caster))
            return SPELL_FAILED_CASTER_AURASTATE;
        if (m_spellInfo->CasterAuraStateNot && m_caster->HasAuraState(AuraStateType(m_spellInfo->CasterAuraStateNot), m_spellInfo, m_caster))
            return SPELL_FAILED_CASTER_AURASTATE;

        // Note: spell 62473 requres casterAuraSpell = triggering spell
        if (!((m_spellInfo->Id == 104136 || m_spellInfo->Id == 48020 || m_spellInfo->Id == 114794) && m_spellInfo->CasterAuraSpell == 62388) && m_spellInfo->CasterAuraSpell && !m_caster->HasAura(m_spellInfo->CasterAuraSpell))
            return SPELL_FAILED_CASTER_AURASTATE;
        if (m_spellInfo->ExcludeCasterAuraSpell && m_caster->HasAura(m_spellInfo->ExcludeCasterAuraSpell))
            return SPELL_FAILED_CASTER_AURASTATE;

        if (reqCombat && m_caster->isInCombat() && !m_spellInfo->CanBeUsedInCombat())
            return SPELL_FAILED_AFFECTING_COMBAT;
    }

    // cancel autorepeat spells if cast start when moving
    // (not wand currently autorepeat cast delayed to moving stop anyway in spell update code)
    // Do not cancel spells which are affected by a SPELL_AURA_CAST_WHILE_WALKING or SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING effect
    if (m_caster->IsPlayer() && m_caster->ToPlayer()->IsMoving() && !m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, m_spellInfo) &&
        !m_caster->HasAuraType(SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING))
    {
        // skip stuck spell to allow use it in falling case and apply spell limitations at movement
        if ((!m_caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING_FAR) || m_spellInfo->Effects[0].Effect != SPELL_EFFECT_STUCK) &&
            (IsAutoRepeat() || (m_spellInfo->AuraInterruptFlags & AURA_INTERRUPT_FLAG_NOT_SEATED) != 0))
            return SPELL_FAILED_MOVING;
    }

    // Check vehicle flags
    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE))
    {
        SpellCastResult vehicleCheck = m_spellInfo->CheckVehicle(m_caster);
        if (vehicleCheck != SPELL_CAST_OK)
            return vehicleCheck;
    }

    // check spell cast conditions from database
    {
        ConditionSourceInfo condInfo = ConditionSourceInfo(m_caster, m_targets.GetObjectTarget());
        if (!sConditionMgr->IsObjectMeetingNotGroupedConditions(CONDITION_SOURCE_TYPE_SPELL, m_spellInfo->Id, condInfo))
        {
            // send error msg to player if condition failed and text message available
            // @TODO: using WorldSession::SendNotification is not blizzlike
            if (Player* playerCaster = m_caster->ToPlayer())
            {
                // mLastFailedCondition can be NULL if there was an error processing the condition in Condition::Meets (i.e. wrong data for ConditionTarget or others)
                if (playerCaster->GetSession() && condInfo.mLastFailedCondition
                    && condInfo.mLastFailedCondition->ErrorTextId)
                {
                    playerCaster->GetSession()->SendNotification(condInfo.mLastFailedCondition->ErrorTextId);
                    return SPELL_FAILED_DONT_REPORT;
                }
            }
            if (!condInfo.mLastFailedCondition || !condInfo.mLastFailedCondition->ConditionTarget)
                return SPELL_FAILED_CASTER_AURASTATE;
            return SPELL_FAILED_BAD_TARGETS;
        }
    }

    // Don't check explicit target for passive spells (workaround) (check should be skipped only for learn case)
    // those spells may have incorrect target entries or not filled at all (for example 15332)
    // such spells when learned are not targeting anyone using targeting system, they should apply directly to caster instead
    // also, such casts shouldn't be sent to client
    if (!IsTriggered() || !m_targets.GetUnitTarget())
    {
        if (!((m_spellInfo->Attributes & SPELL_ATTR0_PASSIVE) && (!m_targets.GetUnitTarget() || m_targets.GetUnitTarget() == m_caster)))
        {
            // Check explicit target for m_originalCaster - todo: get rid of such workarounds
            SpellCastResult castResult = m_spellInfo->CheckExplicitTarget(m_originalCaster ? m_originalCaster : m_caster, m_targets.GetObjectTarget(), m_targets.GetItemTarget());
            if (castResult != SPELL_CAST_OK)
                return castResult;
        }
    }

    if (Unit* target = m_targets.GetUnitTarget())
    {
        SpellCastResult castResult = m_spellInfo->CheckTarget(m_caster, target, IsTriggered() ? true: false);
        if (castResult != SPELL_CAST_OK)
            return castResult;

        if (strict && IsMorePowerfulAura(target))
            return IsTriggered() ? SPELL_FAILED_DONT_REPORT: SPELL_FAILED_AURA_BOUNCED;

        if (target != m_caster)
        {
            // Must be behind the target
            if ((m_spellInfo->AttributesCu & SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET) && target->HasInArc(static_cast<float>(M_PI), m_caster))
                return SPELL_FAILED_NOT_BEHIND;

            // Target must be facing you
            if ((m_spellInfo->AttributesCu & SPELL_ATTR0_CU_REQ_TARGET_FACING_CASTER) && !target->HasInArc(static_cast<float>(M_PI), m_caster))
                return SPELL_FAILED_NOT_INFRONT;

            // Gouge and Glyph of Gouge
            if (m_spellInfo->Id == 1776 && !m_caster->HasAura(56809) && !target->HasInArc(static_cast<float>(M_PI), m_caster))
                return SPELL_FAILED_NOT_INFRONT;

            if (!IsTriggered() || IsCommandDemonSpell())  ///< Comand Demon spells is triggered to allow cast while casting other spell, but should be checked for LoS rules too
            {
                // Ignore LOS for gameobjects casts (wrongly casted by a trigger)
                if (m_caster->GetEntry() != WORLD_TRIGGER)
                {
                    if (!(m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS) && !m_caster->IsWithinLOSInMap(target))
                        return SPELL_FAILED_LINE_OF_SIGHT;
                }

                if (m_caster->IsVisionObscured(target, m_spellInfo))
                    return SPELL_FAILED_VISION_OBSCURED; // smoke bomb, camouflage...
            }
        }
    }

    // Check for line of sight for spells with dest
    if (m_targets.HasDst())
    {
        float x, y, z;
        m_targets.GetDstPos()->GetPosition(x, y, z);

        if (!(m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS) && !m_caster->IsWithinLOS(x, y, z))
            return SPELL_FAILED_LINE_OF_SIGHT;
    }

    // check pet presence
    for (uint8 j = 0; j < m_spellInfo->EffectCount; ++j)
    {
        if (m_spellInfo->Effects[j].TargetA.GetTarget() == TARGET_UNIT_PET)
        {
            if (!m_caster->GetGuardianPet())
            {
                if (m_triggeredByAuraSpell)              // not report pet not existence for triggered spells
                    return SPELL_FAILED_DONT_REPORT;
                else
                    return SPELL_FAILED_NO_PET;
            }
            break;
        }

        Unit* l_Target = m_targets.GetUnitTarget();

        /// Check mini pet target
        if (m_spellInfo->Effects[j].TargetA.GetTarget() == TARGET_UNIT_TARGET_MINIPET && l_Target != nullptr && !l_Target->GetCritterGUID())
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
    }

    // Spell casted only on battleground
    if ((m_spellInfo->AttributesEx3 & SPELL_ATTR3_BATTLEGROUND) &&  m_caster->IsPlayer())
        if (!m_caster->ToPlayer()->InBattleground())
            return SPELL_FAILED_ONLY_BATTLEGROUNDS;

    // do not allow spells to be cast in arenas or rated battlegrounds
    if (Player* l_Player = m_caster->GetCharmerOrOwnerPlayerOrPlayerItself())
    {
        if (l_Player->InArena() || l_Player->InRatedBattleGround())
        {
            SpellCastResult castResult = CheckArenaAndRatedBattlegroundCastRules(l_Player->GetBattleground());
            if (castResult != SPELL_CAST_OK)
                return castResult;
        }
    }

    // zone check
    if (m_caster->GetTypeId() == TYPEID_UNIT || !m_caster->ToPlayer()->isGameMaster())
    {
        uint32 zone, area;
        m_caster->GetZoneAndAreaId(zone, area);

        SpellCastResult locRes= m_spellInfo->CheckLocation(m_caster->GetMapId(), zone, area,
            m_caster->IsPlayer() ? m_caster->ToPlayer() : NULL);
        if (locRes != SPELL_CAST_OK)
            return locRes;
    }

    // not let players cast spells at mount (and let do it to creatures)
    if (m_caster->IsMounted() && m_caster->IsPlayer() && !(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE) &&
        !m_spellInfo->IsPassive() && !(m_spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_MOUNTED))
    {
        /// Herb Gathering
        if (m_spellInfo->Id == 2369)
        {
            /// This is used for Sky Golem (Flying Mount) which allow you to gather herbs (and only herbs) without being dismounted.
            if (!m_caster->HasFlag(EPlayerFields::PLAYER_FIELD_LOCAL_FLAGS, PlayerLocalFlags::PLAYER_LOCAL_FLAG_CAN_USE_OBJECTS_MOUNTED))
            {
                if (m_caster->isInFlight())
                    return SPELL_FAILED_NOT_ON_TAXI;
                else
                    return SPELL_FAILED_NOT_MOUNTED;
            }
        }
        else
        {
            if (m_caster->isInFlight())
                return SPELL_FAILED_NOT_ON_TAXI;
            else
                return SPELL_FAILED_NOT_MOUNTED;
        }
    }

    SpellCastResult castResult = SPELL_CAST_OK;

    // always (except passive spells) check items (focus object can be required for any type casts)
    if (!m_spellInfo->IsPassive())
    {
        castResult = CheckItems();
        if (castResult != SPELL_CAST_OK)
            return castResult;
    }

    // Triggered spells also have range check
    // TODO: determine if there is some flag to enable/disable the check
    castResult = CheckRange(strict);
    if (castResult != SPELL_CAST_OK)
        return castResult;

    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_POWER_AND_REAGENT_COST))
    {
        castResult = CheckPower();
        if (castResult != SPELL_CAST_OK)
            return castResult;
    }

    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_AURAS))
    {
        castResult = CheckCasterAuras();
        if (castResult != SPELL_CAST_OK)
            return castResult;
    }

    // script hook
    castResult = CallScriptCheckCastHandlers();
    if (castResult != SPELL_CAST_OK)
        return castResult;

    for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
    {
        // for effects of spells that have only one target
        switch (m_spellInfo->Effects[i].Effect)
        {
            case SPELL_EFFECT_KNOCK_BACK_DEST:
            {
                switch (m_spellInfo->Id)
                {
                    case 68645: // Rocket Pack
                        if (!m_caster->GetTransport())
                            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                        break;
                }
                break;
            }
            case SPELL_EFFECT_DUMMY:
            {
                // Death Coil
                if (m_spellInfo->SpellFamilyName == SPELLFAMILY_DEATHKNIGHT && m_spellInfo->SpellFamilyFlags[0] == 0x2000)
                {
                    Unit* target = m_targets.GetUnitTarget();
                    // Glyph of Death Coil
                    if (!target || (target->IsFriendlyTo(m_caster) && target->GetCreatureType() != CREATURE_TYPE_UNDEAD && !m_caster->HasAura(63333)))
                        return SPELL_FAILED_BAD_TARGETS;

                    if (!target->IsFriendlyTo(m_caster))
                    {
                        if (!m_caster->HasInArc(static_cast<float>(M_PI), target))
                            return SPELL_FAILED_UNIT_NOT_INFRONT;
                        if (!m_caster->_IsValidAttackTarget(target, GetSpellInfo()))
                            return SPELL_FAILED_BAD_TARGETS;
                    }
                    else if (m_caster->HasAura(63333) && target == m_caster)
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else if (m_spellInfo->Id == 19938)          // Awaken Peon
                {
                    Unit* unit = m_targets.GetUnitTarget();
                    if (!unit || !unit->HasAura(17743))
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else if (m_spellInfo->Id == 31789)          // Righteous Defense
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_DONT_REPORT;

                    Unit* target = m_targets.GetUnitTarget();
                    if (!target || !target->IsFriendlyTo(m_caster) || target->getAttackers().empty())
                        return SPELL_FAILED_BAD_TARGETS;

                }
                break;
            }
            case SPELL_EFFECT_LEARN_SPELL:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;

                if (m_spellInfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_PET)
                    break;

                Pet* pet = m_caster->ToPlayer()->GetPet();

                if (!pet)
                    return SPELL_FAILED_NO_PET;

                SpellInfo const* learn_spellproto = sSpellMgr->GetSpellInfo(m_spellInfo->Effects[i].TriggerSpell);

                if (!learn_spellproto)
                    return SPELL_FAILED_NOT_KNOWN;

                if (m_spellInfo->SpellLevel > pet->getLevel())
                    return SPELL_FAILED_LOWLEVEL;

                break;
            }
            case SPELL_EFFECT_LEARN_PET_SPELL:
            {
                // check target only for unit target case
                if (Unit* unitTarget = m_targets.GetUnitTarget())
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_BAD_TARGETS;

                    Pet* pet = unitTarget->ToPet();
                    if (!pet || pet->GetOwner() != m_caster)
                        return SPELL_FAILED_BAD_TARGETS;

                    SpellInfo const* learn_spellproto = sSpellMgr->GetSpellInfo(m_spellInfo->Effects[i].TriggerSpell);

                    if (!learn_spellproto)
                        return SPELL_FAILED_NOT_KNOWN;

                    if (m_spellInfo->SpellLevel > pet->getLevel())
                        return SPELL_FAILED_LOWLEVEL;
                }
                break;
            }
            case SPELL_EFFECT_APPLY_GLYPH:
            {
                uint32 glyphId = m_spellInfo->Effects[i].MiscValue;
                if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyphId))
                    if (m_caster->HasAura(gp->SpellId))
                        return SPELL_FAILED_UNIQUE_GLYPH;
                break;
            }
            case SPELL_EFFECT_FEED_PET:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;

                Item* foodItem = m_targets.GetItemTarget();
                if (!foodItem)
                    return SPELL_FAILED_BAD_TARGETS;

                Pet* pet = m_caster->ToPlayer()->GetPet();

                if (!pet)
                    return SPELL_FAILED_NO_PET;

                if (!pet->HaveInDiet(foodItem->GetTemplate()))
                    return SPELL_FAILED_WRONG_PET_FOOD;

                if (!pet->GetCurrentFoodBenefitLevel(foodItem->GetTemplate()->ItemLevel))
                    return SPELL_FAILED_FOOD_LOWLEVEL;

                if (m_caster->isInCombat() || pet->isInCombat())
                    return SPELL_FAILED_AFFECTING_COMBAT;

                break;
            }
            case SPELL_EFFECT_POWER_BURN:
            case SPELL_EFFECT_POWER_DRAIN:
            {
                // Can be area effect, Check only for players and not check if target - caster (spell can have multiply drain/burn effects)
                if (m_caster->IsPlayer())
                    if (Unit* target = m_targets.GetUnitTarget())
                        if (target != m_caster && target->getPowerType() != Powers(m_spellInfo->Effects[i].MiscValue))
                            return SPELL_FAILED_BAD_TARGETS;
                break;
            }
            case SPELL_EFFECT_CHARGE:
            {
                if (m_spellInfo->SpellFamilyName == SPELLFAMILY_WARRIOR)
                {
                    // Warbringer - can't be handled in proc system - should be done before checkcast root check and charge effect process
                    if (strict && m_caster->IsScriptOverriden(m_spellInfo, 6953))
                        m_caster->RemoveMovementImpairingAuras();
                    // Intervene can be casted in root effects, so we need to remove movement impairing auras before check cast result
                    if (m_spellInfo->Id == 34784)
                        m_caster->RemoveMovementImpairingAuras();
                }
                if (m_caster->HasUnitState(UNIT_STATE_ROOT))
                    return SPELL_FAILED_ROOTED;
                if (m_caster->IsPlayer())
                    if (Unit* target = m_targets.GetUnitTarget())
                        if (!target->isAlive())
                            return SPELL_FAILED_BAD_TARGETS;

                Unit* target = m_targets.GetUnitTarget();

                if (!target)
                    return SPELL_FAILED_DONT_REPORT;

                Position pos;
                target->GetContactPoint(m_caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ);
                target->GetFirstCollisionPosition(pos, CONTACT_DISTANCE, target->GetRelativeAngle(m_caster));

                m_preGeneratedPath.SetPathLengthLimit(m_spellInfo->GetMaxRange(true) * 4.0f);

                bool result = m_preGeneratedPath.CalculatePath(pos.m_positionX, pos.m_positionY, pos.m_positionZ + target->GetObjectSize());
                if (m_preGeneratedPath.GetPathType() & PATHFIND_SHORT)
                    return SPELL_FAILED_OUT_OF_RANGE; 
                else if (!result)
                    return SPELL_FAILED_NOPATH;

                break;
            }
            case SPELL_EFFECT_SKINNING:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER || !m_targets.GetUnitTarget() || m_targets.GetUnitTarget()->GetTypeId() != TYPEID_UNIT)
                    return SPELL_FAILED_BAD_TARGETS;

                if (!(m_targets.GetUnitTarget()->GetUInt32Value(UNIT_FIELD_FLAGS) & UNIT_FLAG_SKINNABLE))
                    return SPELL_FAILED_TARGET_UNSKINNABLE;

                Creature* creature = m_targets.GetUnitTarget()->ToCreature();
                if (creature->GetCreatureType() != CREATURE_TYPE_CRITTER && !creature->loot.isLooted())
                    return SPELL_FAILED_TARGET_NOT_LOOTED;

                uint32 skill = creature->GetCreatureTemplate()->GetRequiredLootSkill();

                int32 skillValue = m_caster->ToPlayer()->GetSkillValue(skill);
                int32 TargetLevel = creature->getLevel();
                int32 ReqValue = (skillValue < 100 ? (TargetLevel-10) * 10 : TargetLevel * 5);

                /// Skinning in Draenor doesn't require to have a specific skill level, 1 is enough
                if (skillValue && creature->GetCreatureTemplate()->expansion >= Expansion::EXPANSION_WARLORDS_OF_DRAENOR)
                    break;

                if (ReqValue > skillValue)
                    return SPELL_FAILED_LOW_CASTLEVEL;

                // chance for fail at orange skinning attempt
                if ((m_selfContainer && (*m_selfContainer) == this) &&
                    skillValue < sWorld->GetConfigMaxSkillValue() &&
                    (ReqValue < 0 ? 0 : ReqValue) > irand(skillValue - 25, skillValue + 37))
                    return SPELL_FAILED_TRY_AGAIN;

                break;
            }
            case SPELL_EFFECT_OPEN_LOCK:
            {
                if (m_spellInfo->Effects[i].TargetA.GetTarget() != TARGET_GAMEOBJECT_TARGET &&
                    m_spellInfo->Effects[i].TargetA.GetTarget() != TARGET_GAMEOBJECT_ITEM_TARGET)
                    break;

                if (m_caster->GetTypeId() != TYPEID_PLAYER  // only players can open locks, gather etc.
                    // we need a go target in case of TARGET_GAMEOBJECT_TARGET
                    || (m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_GAMEOBJECT_TARGET && !m_targets.GetGOTarget()))
                    return SPELL_FAILED_BAD_TARGETS;

                Item* pTempItem = NULL;
                if (m_targets.GetTargetMask() & TARGET_FLAG_TRADE_ITEM)
                {
                    if (TradeData* pTrade = m_caster->ToPlayer()->GetTradeData())
                    {
                        if (m_targets.GetItemTargetGUID() < TRADE_SLOT_COUNT)
                            pTempItem = pTrade->GetTraderData()->GetItem(TradeSlots(m_targets.GetItemTargetGUID()));
                    }
                }
                else if (m_targets.GetTargetMask() & TARGET_FLAG_ITEM)
                    pTempItem = m_caster->ToPlayer()->GetItemByGuid(m_targets.GetItemTargetGUID());

                // we need a go target, or an openable item target in case of TARGET_GAMEOBJECT_ITEM_TARGET
                if (m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_GAMEOBJECT_ITEM_TARGET &&
                    !m_targets.GetGOTarget() &&
                    (!pTempItem || !pTempItem->GetTemplate()->LockID || !pTempItem->IsLocked()))
                    return SPELL_FAILED_BAD_TARGETS;

                if (m_spellInfo->Id != 1842 || (m_targets.GetGOTarget() &&
                    m_targets.GetGOTarget()->GetGOInfo()->type != GAMEOBJECT_TYPE_TRAP))
                    if (m_caster->ToPlayer()->InBattleground() && // In Battleground players can use only flags and banners
                        !m_caster->ToPlayer()->CanUseBattlegroundObject())
                        return SPELL_FAILED_TRY_AGAIN;

                // get the lock entry
                uint32 lockId = 0;
                bool l_ResultOverridedByPlayerCondition = false;
                bool l_PlayerConditionFailed = false;
                if (GameObject* go = m_targets.GetGOTarget())
                {
                    lockId = go->GetGOInfo()->GetLockId();
                    if (!lockId)
                        return SPELL_FAILED_BAD_TARGETS;

                    GameObjectTemplate const* l_Template = sObjectMgr->GetGameObjectTemplate(go->GetEntry());

                    if (l_Template && l_Template->type == GAMEOBJECT_TYPE_CHEST && m_caster->IsPlayer())
                    {
                        uint32 l_PlayerConditionID = l_Template->chest.conditionID1;
                        bool l_HasPlayerCondition = l_PlayerConditionID != 0 && (sPlayerConditionStore.LookupEntry(l_Template->chest.conditionID1) != nullptr || sScriptMgr->HasPlayerConditionScript(l_Template->chest.conditionID1));

                        if (l_HasPlayerCondition && m_caster->ToPlayer()->EvalPlayerCondition(l_PlayerConditionID).first)
                            l_ResultOverridedByPlayerCondition = true;
                        else if (l_HasPlayerCondition)
                            l_PlayerConditionFailed = true;
                    }
                }
                else if (Item* itm = m_targets.GetItemTarget())
                    lockId = itm->GetTemplate()->LockID;

                SkillType skillId = SKILL_NONE;
                int32 reqSkillValue = 0;
                int32 skillValue = 0;

                if (l_PlayerConditionFailed)
                    return SPELL_FAILED_ERROR;

                // check lock compatibility
                SpellCastResult res = CanOpenLock(i, lockId, skillId, reqSkillValue, skillValue);
                if (res != SPELL_CAST_OK && !l_ResultOverridedByPlayerCondition)
                    return res;

                if (l_ResultOverridedByPlayerCondition)
                    res = SPELL_CAST_OK;

                // chance for fail at orange mining/herb/LockPicking gathering attempt
                // second check prevent fail at rechecks
                if (skillId != SKILL_NONE && (!m_selfContainer || ((*m_selfContainer) != this)))
                {
                    bool canFailAtMax = skillId != SKILL_HERBALISM && skillId != SKILL_MINING;

                    // chance for failure in orange gather / lockpick (gathering skill can't fail at maxskill)
                    if ((canFailAtMax || skillValue < sWorld->GetConfigMaxSkillValue()) && reqSkillValue > irand(skillValue - 25, skillValue + 37))
                        return SPELL_FAILED_TRY_AGAIN;
                }
                break;
            }
            case SPELL_EFFECT_SUMMON_DEAD_PET:
            {
                Creature* pet = m_caster->GetGuardianPet();

                if (pet && pet->isAlive())
                    return SPELL_FAILED_ALREADY_HAVE_SUMMON;

                break;
            }
            // This is generic summon effect
            case SPELL_EFFECT_SUMMON:
            {
                SummonPropertiesEntry const* SummonProperties = sSummonPropertiesStore.LookupEntry(m_spellInfo->Effects[i].MiscValueB);
                if (!SummonProperties)
                    break;
                switch (SummonProperties->Category)
                {
                    case SUMMON_CATEGORY_PET:
                        if (m_caster->GetPetGUID())
                            return SPELL_FAILED_ALREADY_HAVE_SUMMON;
                    case SUMMON_CATEGORY_PUPPET:
                        if (m_caster->GetCharmGUID())
                            return SPELL_FAILED_ALREADY_HAVE_CHARM;
                        break;
                }
                break;
            }
            case SPELL_EFFECT_CREATE_TAMED_PET:
            {
                if (m_targets.GetUnitTarget())
                {
                    if (m_targets.GetUnitTarget()->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_BAD_TARGETS;
                    if (m_targets.GetUnitTarget()->GetPetGUID())
                        return SPELL_FAILED_ALREADY_HAVE_SUMMON;
                }
                break;
            }
            case SPELL_EFFECT_SUMMON_PET:
            {
                if (m_caster->GetPetGUID())                  //let warlock do a replacement summon
                {
                    if (m_caster->IsPlayer() && m_caster->getClass() == CLASS_WARLOCK)
                    {
                        if (strict)                         //starting cast, trigger pet stun (cast by pet so it doesn't attack player)
                            if (Pet* pet = m_caster->ToPlayer()->GetPet())
                                pet->CastSpell(pet, 32752, true, NULL, nullptr, pet->GetGUID());
                    }
                    else
                        return SPELL_FAILED_ALREADY_HAVE_SUMMON;
                }

                if (m_caster->GetCharmGUID())
                    return SPELL_FAILED_ALREADY_HAVE_CHARM;
                break;
            }
            case SPELL_EFFECT_SUMMON_PLAYER:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;
                if (!m_caster->ToPlayer()->GetSelection())
                    return SPELL_FAILED_BAD_TARGETS;

                Player* target = ObjectAccessor::FindPlayer(m_caster->ToPlayer()->GetSelection());
                if (!target || m_caster->ToPlayer() == target || (!target->IsInSameRaidWith(m_caster->ToPlayer()) && m_spellInfo->Id != 48955)) // refer-a-friend spell
                    return SPELL_FAILED_BAD_TARGETS;

                // check if our map is dungeon
                MapEntry const* map = sMapStore.LookupEntry(m_caster->GetMapId());
                if (map->IsDungeon())
                {
                    uint32 mapId = m_caster->GetMap()->GetId();
                    Difficulty difficulty = m_caster->GetMap()->GetDifficultyID();
                    if (map->IsRaid())
                        if (InstancePlayerBind* targetBind = target->GetBoundInstance(mapId, difficulty))
                            if (InstancePlayerBind* casterBind = m_caster->ToPlayer()->GetBoundInstance(mapId, difficulty))
                                if (targetBind->perm && targetBind->save != casterBind->save)
                                    return SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE;

                    InstanceTemplate const* instance = sObjectMgr->GetInstanceTemplate(mapId);
                    if (!instance)
                        return SPELL_FAILED_TARGET_NOT_IN_INSTANCE;
                    if (!target->Satisfy(sObjectMgr->GetAccessRequirement(mapId, difficulty), mapId))
                        return SPELL_FAILED_BAD_TARGETS;
                }
                break;
            }
            // RETURN HERE
            case SPELL_EFFECT_SUMMON_RAF_FRIEND:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;

                Player* playerCaster = m_caster->ToPlayer();
                    //
                if (!(playerCaster->GetSelection()))
                    return SPELL_FAILED_BAD_TARGETS;

                Player* target = ObjectAccessor::FindPlayer(playerCaster->GetSelection());

                if (!target ||
                    !(target->GetSession()->GetRecruiterId() == playerCaster->GetSession()->GetAccountId() || target->GetSession()->GetAccountId() == playerCaster->GetSession()->GetRecruiterId()))
                    return SPELL_FAILED_BAD_TARGETS;

                break;
            }
            case SPELL_EFFECT_LEAP:
            case SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER:
            {
              //Do not allow to cast it before BG starts.
                if (m_caster->IsPlayer())
                    if (Battleground const* bg = m_caster->ToPlayer()->GetBattleground())
                        if (bg->GetStatus() != STATUS_IN_PROGRESS)
                            return SPELL_FAILED_TRY_AGAIN;
                break;
            }
            case SPELL_EFFECT_STEAL_BENEFICIAL_BUFF:
            {
                if (m_targets.GetUnitTarget() == m_caster)
                    return SPELL_FAILED_BAD_TARGETS;
                break;
            }
            case SPELL_EFFECT_LEAP_BACK:
            {
                if (m_caster->HasUnitState(UNIT_STATE_ROOT))
                {
                    if (m_caster->IsPlayer())
                    {
                        switch (m_spellInfo->Id)
                        {
                            case 781:   // Disengage
                                if (m_caster->HasAura(109215)) // Posthaste
                                    break;
                                return SPELL_FAILED_ROOTED;
                            default:
                                return SPELL_FAILED_ROOTED;
                        }
                    }
                    else
                        return SPELL_FAILED_DONT_REPORT;
                }
                break;
            }
            case SPELL_EFFECT_TALENT_SPEC_SELECT:
                // can't change during already started arena/battleground
                if (m_caster->IsPlayer())
                    if (Battleground const* bg = m_caster->ToPlayer()->GetBattleground())
                        if (bg->GetStatus() == STATUS_IN_PROGRESS)
                            return SPELL_FAILED_NOT_IN_BATTLEGROUND;
                break;
            case SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;
                if (Guild* guild = m_caster->ToPlayer()->GetGuild())
                    if (guild->GetLeaderGUID() != m_caster->ToPlayer()->GetGUID())
                        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                break;
            }
            case SPELL_EFFECT_CREATE_HEIRLOOM:
            {
                Player* l_Player = m_caster->ToPlayer();

                if (!l_Player)
                    return SPELL_FAILED_BAD_TARGETS;

                if (!l_Player->HasHeirloom(m_Misc[0]))
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                break;
            }
            case SPELL_EFFECT_UPGRADE_HEIRLOOM:
            {
                Player* l_Player = m_caster->ToPlayer();
                HeirloomEntry const* l_Heirloom = GetHeirloomEntryByItemID(m_Misc[0]);

                if (!l_Player || !l_Heirloom || !m_CastItem)
                    return SPELL_FAILED_BAD_TARGETS;

                if (!l_Player->HasHeirloom(l_Heirloom))
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                if (!l_Player->CanUpgradeHeirloomWith(l_Heirloom, m_CastItem->GetTemplate()->ItemId))
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                break;
            }
            case SPELL_EFFECT_TEACH_FOLLOWER_ABILITY:
            {
                Player* l_Player = m_caster->ToPlayer();

                if (!l_Player)
                    return SPELL_FAILED_BAD_TARGETS;

                MS::Garrison::Manager* l_Garrison = l_Player->GetGarrison();

                if (!l_Garrison)
                    return SPELL_FAILED_BAD_TARGETS;

                SpellCastResult l_Result = l_Garrison->CanLearnTrait(m_Misc[0], m_Misc[1], GetSpellInfo(), i);
                if (l_Result != SPELL_CAST_OK)
                    return l_Result;

                break;
            }
            case SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL:
            {
                Player* l_Player = m_caster->ToPlayer();

                if (!l_Player)
                    return SPELL_FAILED_BAD_TARGETS;

                MS::Garrison::Manager* l_Garrison = l_Player->GetGarrison();

                if (!l_Garrison)
                    return SPELL_FAILED_BAD_TARGETS;

                SpellCastResult l_Result = l_Garrison->CanUpgradeItemLevelWith(m_Misc[0], GetSpellInfo());
                if (l_Result != SPELL_CAST_OK)
                    return l_Result;

                break;
            }
            default:
                break;
        }

        switch (m_spellInfo->Effects[i].ApplyAuraName)
        {
            case SPELL_AURA_MOD_STEALTH:
            {
                // Flare ///< @todo spell id removed && Smoke Bomb
                if (m_caster->HasAura(94528) || m_caster->HasAuraWithNegativeCaster(88611))
                    return SPELL_FAILED_CASTER_AURASTATE;

                break;
            }
            case SPELL_AURA_MOD_RANGED_HASTE:
            {
                // Focus Fire
                if (m_spellInfo->Id == 82692)
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_CASTER_AURASTATE;

                    Pet* pet = m_caster->ToPlayer()->GetPet();
                    if (!pet)
                        return SPELL_FAILED_CASTER_AURASTATE;

                    if (!pet->HasAura(19615))
                        return SPELL_FAILED_CASTER_AURASTATE;
                }
                break;
            }
            case SPELL_AURA_MOD_POSSESS_PET:
            {
                // Storm, Earth and Fire (for spirits)
                if (m_spellInfo->Id == 138130)
                    break;

                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_NO_PET;

                Pet* pet = m_caster->ToPlayer()->GetPet();
                if (!pet)
                    return SPELL_FAILED_NO_PET;

                if (pet->GetCharmerGUID())
                    return SPELL_FAILED_CHARMED;
                break;
            }
            case SPELL_AURA_MOD_POSSESS:
            case SPELL_AURA_MOD_CHARM:
            case SPELL_AURA_AOE_CHARM:
            {
                if (m_caster->GetCharmerGUID())
                    return SPELL_FAILED_CHARMED;

                if (m_spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_CHARM
                    || m_spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_POSSESS)
                {
                    if (m_caster->GetPetGUID())
                        return SPELL_FAILED_ALREADY_HAVE_SUMMON;

                    if (m_caster->GetCharmGUID())
                        return SPELL_FAILED_ALREADY_HAVE_CHARM;
                }

                if (Unit* target = m_targets.GetUnitTarget())
                {
                    if (target->GetTypeId() == TYPEID_UNIT && target->ToCreature()->IsVehicle())
                        return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                    if (target->IsMounted())
                        return SPELL_FAILED_CANT_BE_CHARMED;

                    if (target->GetCharmerGUID())
                        return SPELL_FAILED_CHARMED;

                    int32 damage = CalculateDamage(i, target);
                    if (damage && int32(target->getLevel()) > damage)
                        return SPELL_FAILED_HIGHLEVEL;
                }

                break;
            }
            case SPELL_AURA_MOUNTED:
            {
                // Ignore map check if spell have AreaId. AreaId already checked and this prevent special mount spells
                bool allowMount = !m_caster->GetMap()->IsDungeon() || m_caster->GetMap()->IsBattlegroundOrArena();
                InstanceTemplate const* it = sObjectMgr->GetInstanceTemplate(m_caster->GetMapId());
                if (it)
                    allowMount = it->AllowMount;
                if (m_caster->IsPlayer() && !allowMount && !m_spellInfo->AreaGroupId)
                    return SPELL_FAILED_NO_MOUNTS_ALLOWED;

                if (m_caster->IsInDisallowedMountForm())
                    return SPELL_FAILED_NOT_SHAPESHIFT;

                // hex & mount
                if (m_caster->HasAura(51514))
                    return SPELL_FAILED_CONFUSED;

                break;
            }
            case SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS:
            {
                if (!m_targets.GetUnitTarget())
                    return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                // can be casted at non-friendly unit or own pet/charm
                if (m_caster->IsFriendlyTo(m_targets.GetUnitTarget()))
                    return SPELL_FAILED_TARGET_FRIENDLY;

                break;
            }
            case SPELL_AURA_FLY:
            case SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED:
            {
                // not allow cast fly spells if not have req. skills  (all spells is self target)
                // allow always ghost flight spells
                if (m_originalCaster && m_originalCaster->IsPlayer() && m_originalCaster->isAlive())
                {
                    Battlefield* Bf = sBattlefieldMgr->GetBattlefieldToZoneId(m_originalCaster->GetZoneId());
                    if (AreaTableEntry const* area = GetAreaEntryByAreaID(m_originalCaster->GetAreaId()))
                        if (area->Flags & AREA_FLAG_NO_FLY_ZONE  || (Bf && !Bf->CanFlyIn()))
                            return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_NOT_HERE;
                }
                break;
            }
            case SPELL_AURA_PERIODIC_MANA_LEECH:
            {
                if (m_spellInfo->Effects[i].IsTargetingArea())
                    break;

                if (!m_targets.GetUnitTarget())
                    return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                if (m_caster->GetTypeId() != TYPEID_PLAYER || m_CastItem)
                    break;

                if (m_targets.GetUnitTarget()->getPowerType() != POWER_MANA)
                    return SPELL_FAILED_BAD_TARGETS;

                break;
            }
            case SPELL_AURA_MOD_HEALTH_REGEN_PERCENT:
            {
                // Health Funnel
                if (m_spellInfo->Id == 755)
                {
                    if (m_caster->IsPlayer())
                        if (Pet * pet = m_caster->ToPlayer()->GetPet())
                            if (pet->IsFullHealth())
                                return SPELL_FAILED_ALREADY_AT_FULL_HEALTH;
                }
                break;
            }
            default:
                break;
        }
    }

    switch(m_spellInfo->Id)
    {
        case 23517: ///< Create Healthstone
        {
            if (m_caster->IsPlayer() && m_caster->ToPlayer()->HasItemCount(5512, 1))
                m_caster->ToPlayer()->DestroyItemCount(5512, 1, true);
            break;
        }
        case 50334: // Berserk
        case 61336: // Survival Instincts
        {
            if (m_caster->GetTypeId() != TYPEID_PLAYER || !m_caster->ToPlayer()->IsInFeralForm())
                return SPELL_FAILED_ONLY_SHAPESHIFT;
        }
        default: break;
    }

    /// Fix a bug when spells can be casted in fear
    if (m_caster->HasAuraType(SPELL_AURA_MOD_FEAR) || m_caster->HasAuraType(SPELL_AURA_MOD_FEAR_2))
    {
        if (!m_spellInfo->IsRemoveLossControlEffects() && !m_spellInfo->IsRemoveFear() && m_spellInfo->Id != 1022) ///< Specific case of Hand of Protection
            return SPELL_FAILED_FLEEING;
    }

    // hex
    if (m_caster->HasAuraWithMechanic(1 << MECHANIC_POLYMORPH))
    {
        // Shapeshift
        if (m_spellInfo->HasAura(SPELL_AURA_MOD_SHAPESHIFT) && m_spellInfo->Id != 33891)    // Tree of Life is exception
            return SPELL_FAILED_CHARMED;

        // Teleport
        if (m_spellInfo->HasEffect(SPELL_EFFECT_TELEPORT_UNITS))
            return SPELL_FAILED_CHARMED;

        // Summon totem
        if (m_spellInfo->HasAttribute(SPELL_ATTR7_SUMMON_TOTEM) || m_spellInfo->HasEffect(SPELL_EFFECT_CAST_BUTTON))
            return SPELL_FAILED_CHARMED;

        // Fishing
        if (m_spellInfo->HasAttribute(SPELL_ATTR1_IS_FISHING))
            return SPELL_FAILED_CHARMED;

        // Death Knight - Outbreak
        if (m_spellInfo->Id == 77575)
            return SPELL_FAILED_CHARMED;

        // Drink
        switch (m_spellInfo->GetSpellSpecific())
        {
            case SpellSpecificType::SpellSpecificDrink:
            case SpellSpecificType::SpellSpecificFood:
            case SpellSpecificType::SpellSpecificFoodAndDrink:
                return SPELL_FAILED_CHARMED;
        default:
            break;
        }
    }

    // check trade slot case (last, for allow catch any another cast problems)
    if (m_targets.GetTargetMask() & TARGET_FLAG_TRADE_ITEM)
    {
        if (m_CastItem)
            return SPELL_FAILED_ITEM_ENCHANT_TRADE_WINDOW;

        if (m_caster->GetTypeId() != TYPEID_PLAYER)
            return SPELL_FAILED_NOT_TRADING;

        TradeData* l_MyTrade = m_caster->ToPlayer()->GetTradeData();
        if (!l_MyTrade)
            return SPELL_FAILED_NOT_TRADING;

        // Slot: TRADE_SLOT_NONTRADED is only for non tradable items, enchantment case, open lock ...
        TradeSlots l_Slot = TradeSlots(m_targets.GetItemTargetGUID());
        if (l_Slot != TRADE_SLOT_NONTRADED)
            return SPELL_FAILED_BAD_TARGETS;

        if (!IsTriggered())
        {
            if (l_MyTrade->GetSpell())
                return SPELL_FAILED_ITEM_ALREADY_ENCHANTED;
        }
    }

    /// Check if caster has at least 1 combo point for spells that require combo points
    if (m_needComboPoints && !m_caster->GetPower(Powers::POWER_COMBO_POINT))
        return SPELL_FAILED_NO_COMBO_POINTS;

    // all ok
    return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckPetCast(Unit* target)
{
    // Prevent spellcast interruption by another spellcast
    if (m_caster->HasUnitState(UNIT_STATE_CASTING) && !(_triggeredCastFlags & TRIGGERED_IGNORE_CAST_IN_PROGRESS) && !(m_spellInfo->AttributesEx9 & SPELL_ATTR9_CASTABLE_WHILE_CAST_IN_PROGRESS))
        return SPELL_FAILED_SPELL_IN_PROGRESS;

    // Prevent using of ability if is already casting an ability that has aura type SPELL_AURA_ALLOW_ONLY_ABILITY
    if (m_caster->HasAuraType(SPELL_AURA_ALLOW_ONLY_ABILITY) && !(_triggeredCastFlags & TRIGGERED_IGNORE_CAST_IN_PROGRESS) && !(m_spellInfo->AttributesEx9 & SPELL_ATTR9_CASTABLE_WHILE_CAST_IN_PROGRESS))
        return SPELL_FAILED_SPELL_IN_PROGRESS;

    // dead owner (pets still alive when owners ressed?)
    if (Unit* owner = m_caster->GetCharmerOrOwner())
        if (!owner->isAlive())
            return SPELL_FAILED_CASTER_DEAD;

    if (!target && m_targets.GetUnitTarget())
        target = m_targets.GetUnitTarget();

    if (m_spellInfo->NeedsExplicitUnitTarget())
    {
        if (!target)
            return SPELL_FAILED_BAD_IMPLICIT_TARGETS;
        m_targets.SetUnitTarget(target);
    }

    // cooldown
    if (Creature const* creatureCaster = m_caster->ToCreature())
        if (creatureCaster->HasSpellCooldown(m_spellInfo->Id))
            return SPELL_FAILED_NOT_READY;

    return CheckCast(true);
}

SpellCastResult Spell::CheckCasterAuras() const
{
    // spells totally immuned to caster auras (wsg flag drop, give marks etc)
    if (m_spellInfo->AttributesEx6 & SPELL_ATTR6_IGNORE_CASTER_AURAS)
        return SPELL_CAST_OK;

    uint8 school_immune = 0;
    uint32 mechanic_immune = 0;
    uint32 dispel_immune = 0;

    // Check if the spell grants school or mechanic immunity.
    // We use bitmasks so the loop is done only once and not on every aura check below.
    if (m_spellInfo->AttributesEx & SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY)
    {
        for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
        {
            if (m_spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_SCHOOL_IMMUNITY)
                school_immune |= uint32(m_spellInfo->Effects[i].MiscValue);
            else if (m_spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MECHANIC_IMMUNITY)
                mechanic_immune |= 1 << uint32(m_spellInfo->Effects[i].MiscValue);
            else if (m_spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_DISPEL_IMMUNITY)
                dispel_immune |= SpellInfo::GetDispelMask(DispelType(m_spellInfo->Effects[i].MiscValue));
        }

        // immune movement impairment and loss of control
        if (m_spellInfo->IsRemoveLossControlEffects())
            mechanic_immune = IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;
    }

    bool usableInStun = m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_STUNNED;

    // Life Cocoon is usable while stunned with Glyph of life cocoon
    if (m_spellInfo->Id == 116849 && m_caster->HasAura(124989))
        usableInStun = true;

    // Check whether the cast should be prevented by any state you might have.
    SpellCastResult prevented_reason = SPELL_CAST_OK;
    // Have to check if there is a stun aura. Otherwise will have problems with ghost aura apply while logging out
    uint32 unitflag = m_caster->GetUInt32Value(UNIT_FIELD_FLAGS);     // Get unit state

    if (unitflag & UNIT_FLAG_STUNNED && !usableInStun)
        prevented_reason = SPELL_FAILED_STUNNED;
    else if (unitflag & UNIT_FLAG_CONFUSED && !m_spellInfo->HasAttribute(SPELL_ATTR5_USABLE_WHILE_CONFUSED))
        prevented_reason = SPELL_FAILED_CONFUSED;
    else if (unitflag & UNIT_FLAG_FLEEING && !m_spellInfo->HasAttribute(SPELL_ATTR5_USABLE_WHILE_FEARED))
        prevented_reason = SPELL_FAILED_FLEEING;
    else if (unitflag & UNIT_FLAG_SILENCED && m_spellInfo->PreventionType & (SpellPreventionMask::Silence))
        prevented_reason = SPELL_FAILED_SILENCED;
    else if (unitflag & UNIT_FLAG_PACIFIED && m_spellInfo->PreventionType & (SpellPreventionMask::Pacify))
        prevented_reason = SPELL_FAILED_PACIFIED;

    // Barkskin & Hex hotfix 4.3 patch http://eu.battle.net/wow/ru/blog/10037151
    if (m_spellInfo->Id == 22812 && m_caster->HasAura(51514))
        prevented_reason = SPELL_FAILED_PACIFIED;

    // Attr must make flag drop spell totally immune from all effects
    if (prevented_reason != SPELL_CAST_OK)
    {
        if (school_immune || mechanic_immune || dispel_immune)
        {
            //Checking auras is needed now, because you are prevented by some state but the spell grants immunity.
            Unit::AuraApplicationMap const& auras = m_caster->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                Aura const* aura = itr->second->GetBase();
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (auraInfo->GetAllEffectsMechanicMask() & mechanic_immune)
                    continue;
                if (auraInfo->GetSchoolMask() & school_immune && !(auraInfo->AttributesEx & SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE))
                    continue;
                if (auraInfo->GetDispelMask() & dispel_immune)
                    continue;

                //Make a second check for spell failed so the right SPELL_FAILED message is returned.
                //That is needed when your casting is prevented by multiple states and you are only immune to some of them.
                for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
                {
                    if (AuraEffect* part = aura->GetEffect(i))
                    {
                        switch (part->GetAuraType())
                        {
                            case SPELL_AURA_MOD_STUN:
                                if (!usableInStun || !(auraInfo->GetAllEffectsMechanicMask() & (1<<MECHANIC_STUN)))
                                    return SPELL_FAILED_STUNNED;
                                break;
                            case SPELL_AURA_MOD_CONFUSE:
                                if (!(m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_CONFUSED))
                                    return SPELL_FAILED_CONFUSED;
                                break;
                            case SPELL_AURA_MOD_FEAR:
                            case SPELL_AURA_MOD_FEAR_2:
                                if (!(m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_FEARED))
                                    return SPELL_FAILED_FLEEING;
                                break;
                            case SPELL_AURA_MOD_SILENCE:
                            case SPELL_AURA_MOD_PACIFY:
                            case SPELL_AURA_MOD_PACIFY_SILENCE:
                                if (m_spellInfo->PreventionType & (SpellPreventionMask::Pacify))
                                    return SPELL_FAILED_PACIFIED;
                                else if (m_spellInfo->PreventionType & (SpellPreventionMask::Silence))
                                    return SPELL_FAILED_SILENCED;
                                break;
                            default: break;
                        }
                    }
                }
            }
        }
        // You are prevented from casting and the spell casted does not grant immunity. Return a failed error.
        else
            return prevented_reason;
    }
    return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckArenaAndRatedBattlegroundCastRules(Battleground const* p_Battleground)
{
    bool isRatedBG = p_Battleground ? p_Battleground->IsRatedBG() : false;
    bool isArena = p_Battleground ? p_Battleground->isArena() : false;

    // check USABLE attributes
    // USABLE takes precedence over NOT_USABLE
    if (isRatedBG && m_spellInfo->AttributesEx9 & SPELL_ATTR9_USABLE_IN_RATED_BATTLEGROUNDS)
        return SPELL_CAST_OK;

    if (isArena && m_spellInfo->AttributesEx4 & SPELL_ATTR4_USABLE_IN_ARENA)
        return SPELL_CAST_OK;

    // check NOT_USABLE attributes
    if (m_spellInfo->AttributesEx4 & SPELL_ATTR4_NOT_USABLE_IN_ARENA_OR_RATED_BG)
    {
        if (isArena)
            return SPELL_FAILED_NOT_IN_ARENA;
        else if (isRatedBG)
            return SPELL_FAILED_NOT_IN_RATED_BATTLEGROUND;
    }

    if (isArena && m_spellInfo->AttributesEx9 & SPELL_ATTR9_NOT_USABLE_IN_ARENA)
        return SPELL_FAILED_NOT_IN_ARENA;

    // check cooldowns
    uint32 spellCooldown = m_spellInfo->GetRecoveryTime();
    if (isArena && spellCooldown > 10 * MINUTE * IN_MILLISECONDS) // not sure if still needed
        return SPELL_FAILED_NOT_IN_ARENA;

    if (isRatedBG && spellCooldown > 15 * MINUTE * IN_MILLISECONDS)
        return SPELL_FAILED_NOT_IN_RATED_BATTLEGROUND;

    return SPELL_CAST_OK;
}

bool Spell::CanAutoCast(Unit* target)
{
    uint64 targetguid = target->GetGUID();

    for (uint32 j = 0; j < m_spellInfo->EffectCount; ++j)
    {
        if (m_spellInfo->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA || m_spellInfo->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET)
        {
            if (m_spellInfo->StackAmount <= 1)
            {
                if (target->HasAuraEffect(m_spellInfo->Id, j))
                    return false;
            }
            else
            {
                if (AuraEffect* aureff = target->GetAuraEffect(m_spellInfo->Id, j))
                    if (aureff->GetBase()->GetStackAmount() >= m_spellInfo->StackAmount)
                        return false;
            }
        }
        else if (m_spellInfo->Effects[j].IsAreaAuraEffect())
        {
            if (target->HasAuraEffect(m_spellInfo->Id, j))
                return false;
        }
    }

    SpellCastResult result = CheckPetCast(target);

    if (result == SPELL_CAST_OK || result == SPELL_FAILED_UNIT_NOT_INFRONT)
    {
        SelectSpellTargets();
        //check if among target units, our WANTED target is as well (->only self cast spells return false)
        for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            if (ihit->targetGUID == targetguid)
                return true;
    }
    return false;                                           //target invalid
}

SpellCastResult Spell::CheckRange(bool strict)
{
    // Don't check for instant cast spells
    if (!strict && m_casttime == 0)
        return SPELL_CAST_OK;

    uint32 range_type = 0;

    if (m_spellInfo->RangeEntry)
    {
        // check needed by 68766 51693 - both spells are cast on enemies and have 0 max range
        // these are triggered by other spells - possibly we should omit range check in that case?
        if (m_spellInfo->RangeEntry->ID == 1)
            return SPELL_CAST_OK;

        range_type = m_spellInfo->RangeEntry->type;
    }

    Unit* target = m_targets.GetUnitTarget();
    float max_range = m_caster->GetSpellMaxRangeForTarget(target, m_spellInfo);
    float min_range = m_caster->GetSpellMinRangeForTarget(target, m_spellInfo);

    if (Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_RANGE, max_range, this);

    if (target && target != m_caster)
    {
        if (range_type == SPELL_RANGE_MELEE)
        {
            // Because of lag, we can not check too strictly here.
            if (!m_caster->IsWithinMeleeRange(target, max_range))
                return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_OUT_OF_RANGE : SPELL_FAILED_DONT_REPORT;
        }
        else if (!m_caster->IsWithinCombatRange(target, max_range))
            return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_OUT_OF_RANGE : SPELL_FAILED_DONT_REPORT; //0x5A;

        if (range_type == SPELL_RANGE_RANGED)
        {
            if (m_caster->IsWithinMeleeRange(target))
                return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_TOO_CLOSE : SPELL_FAILED_DONT_REPORT;
        }
        else if (min_range && m_caster->IsWithinCombatRange(target, min_range)) // skip this check if min_range = 0
            return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_TOO_CLOSE : SPELL_FAILED_DONT_REPORT;

        /// Allow spells in any direction if the target is own vehicle
        if (m_caster->IsPlayer() && (m_spellInfo->FacingCasterFlags & SPELL_FACING_FLAG_INFRONT) && !m_caster->HasInArc(static_cast<float>(M_PI), target))
        {
            Vehicle* l_Vehicle = m_caster->GetVehicle();
            if (l_Vehicle == nullptr || (!m_caster->IsOnVehicle(target) && !(l_Vehicle->GetBase() && l_Vehicle->GetBase()->IsOnVehicle(target))))
                return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_UNIT_NOT_INFRONT : SPELL_FAILED_DONT_REPORT;
        }
    }

    if (m_targets.HasDst() && !m_targets.HasTraj())
    {
        if (!m_caster->IsWithinDist3d(m_targets.GetDstPos(), max_range, target))
            return SPELL_FAILED_OUT_OF_RANGE;
        if (min_range && m_caster->IsWithinDist3d(m_targets.GetDstPos(), min_range))
            return SPELL_FAILED_TOO_CLOSE;
    }

    return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckPower()
{
    // item cast not used power
    if (m_CastItem)
        return SPELL_CAST_OK;

    // Dark Simulacrum case
    if (isStolen)
        return SPELL_CAST_OK;

    // health as power used - need check health amount
    for (auto itr : m_spellInfo->SpellPowers)
    {
        if (itr->PowerType == POWER_HEALTH)
        {
            if (int32(m_caster->GetHealth()) <= m_powerCost[POWER_TO_INDEX(POWER_HEALTH)])
                return SPELL_FAILED_CASTER_AURASTATE;
            return SPELL_CAST_OK;
        }
    }

    // Check valid power type
    for (auto itr : m_spellInfo->SpellPowers)
    {
        if (itr->PowerType >= MAX_POWERS)
            return SPELL_FAILED_UNKNOWN;
    }

    // Check rune cost only if a spell has PowerType == POWER_RUNES
    for (auto itr : m_spellInfo->SpellPowers)
    {
        if (itr->PowerType == POWER_RUNES)
        {
            SpellCastResult failReason = CheckRuneCost(m_spellInfo->RuneCostID);
            if (failReason != SPELL_CAST_OK)
                return failReason;
        }
    }

    switch (m_spellInfo->Id)
    {
        case 53385:  ///< Divine Storm
        {
            if (m_powerCost[POWER_HOLY_POWER] != 0 && m_caster->HasAura(144595))
            {
                m_powerCost[POWER_HOLY_POWER] = 0;
                m_caster->RemoveAuraFromStack(144595);
            }

            break;
        }
        default:
            break;
    }

    // Check power amount
    for (auto itr : m_spellInfo->SpellPowers)
    {
        if (itr->RequiredAuraSpellId && !m_caster->HasAura(itr->RequiredAuraSpellId))
            continue;

        Powers powerType = Powers(itr->PowerType);
        if (int32(m_caster->GetPower(powerType)) < m_powerCost[POWER_TO_INDEX(powerType)])
            return SPELL_FAILED_NO_POWER;
    }

    return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckItems()
{
    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return SPELL_CAST_OK;

    Player* p_caster = (Player*)m_caster;

    if (!m_CastItem)
    {
        if (m_castItemGUID)
            return SPELL_FAILED_ITEM_NOT_READY;
    }
    else
    {
        uint32 itemid = m_CastItem->GetEntry();
        if (!p_caster->HasItemCount(itemid))
            return SPELL_FAILED_ITEM_NOT_READY;

        ItemTemplate const* proto = m_CastItem->GetTemplate();
        if (!proto)
            return SPELL_FAILED_ITEM_NOT_READY;

        for (int i = 0; i < MAX_ITEM_SPELLS; ++i)
            if (proto->Spells[i].SpellCharges)
                if (m_CastItem->GetSpellCharges(i) == 0)
                    return SPELL_FAILED_NO_CHARGES_REMAIN;

        // consumable cast item checks
        if (proto->Class == ITEM_CLASS_CONSUMABLE && m_targets.GetUnitTarget())
        {
            // such items should only fail if there is no suitable effect at all - see Rejuvenation Potions for example
            SpellCastResult failReason = SPELL_CAST_OK;
            for (int i = 0; i < m_spellInfo->EffectCount; i++)
            {
                    // skip check, pet not required like checks, and for TARGET_UNIT_PET m_targets.GetUnitTarget() is not the real target but the caster
                    if (m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_PET)
                    continue;

                if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_HEAL)
                {
                    if (m_targets.GetUnitTarget()->IsFullHealth())
                    {
                        failReason = SPELL_FAILED_ALREADY_AT_FULL_HEALTH;
                        continue;
                    }
                    else
                    {
                        failReason = SPELL_CAST_OK;
                        break;
                    }
                }

                // Mana Potion, Rage Potion, Thistle Tea(Rogue), ...
                if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_ENERGIZE)
                {
                    if (m_spellInfo->Effects[i].MiscValue < 0 || m_spellInfo->Effects[i].MiscValue >= int8(MAX_POWERS))
                    {
                        failReason = SPELL_FAILED_ALREADY_AT_FULL_POWER;
                        continue;
                    }

                    Powers power = Powers(m_spellInfo->Effects[i].MiscValue);
                    if (m_targets.GetUnitTarget()->GetPower(power) == m_targets.GetUnitTarget()->GetMaxPower(power))
                    {
                        failReason = SPELL_FAILED_ALREADY_AT_FULL_POWER;
                        continue;
                    }
                    else
                    {
                        failReason = SPELL_CAST_OK;
                        break;
                    }
                }
            }
            if (failReason != SPELL_CAST_OK)
                return failReason;
        }
    }

    // check target item
    if (m_targets.GetItemTargetGUID())
    {
        if (m_caster->GetTypeId() != TYPEID_PLAYER)
            return SPELL_FAILED_BAD_TARGETS;

        if (!m_targets.GetItemTarget())
            return SPELL_FAILED_ITEM_GONE;

        if (!m_targets.GetItemTarget()->IsFitToSpellRequirements(m_spellInfo))
            return SPELL_FAILED_EQUIPPED_ITEM_CLASS;
    }
    // if not item target then required item must be equipped
    else
    {
        if (m_caster->IsPlayer() && !m_caster->ToPlayer()->HasItemFitToSpellRequirements(m_spellInfo))
            return SPELL_FAILED_EQUIPPED_ITEM_CLASS;
    }

    // check spell focus object
    if (m_spellInfo->RequiresSpellFocus)
    {
        CellCoord p(JadeCore::ComputeCellCoord(m_caster->GetPositionX(), m_caster->GetPositionY()));
        Cell cell(p);

        GameObject* ok = NULL;
        JadeCore::GameObjectFocusCheck go_check(m_caster, m_spellInfo->RequiresSpellFocus);
        JadeCore::GameObjectSearcher<JadeCore::GameObjectFocusCheck> checker(m_caster, ok, go_check);

        TypeContainerVisitor<JadeCore::GameObjectSearcher<JadeCore::GameObjectFocusCheck>, GridTypeMapContainer > object_checker(checker);
        Map& map = *m_caster->GetMap();
        cell.Visit(p, object_checker, map, *m_caster, m_caster->GetVisibilityRange());

        if (!ok)
            return SPELL_FAILED_REQUIRES_SPELL_FOCUS;

        focusObject = ok;                                   // game object found in range
    }

    // do not take reagents for these item casts
    if (!(m_CastItem && m_CastItem->GetTemplate()->Flags & ITEM_FLAG_TRIGGERED_CAST))
    {
        bool checkReagents = !(_triggeredCastFlags & TRIGGERED_IGNORE_POWER_AND_REAGENT_COST) && !p_caster->CanNoReagentCast(m_spellInfo);
        // Not own traded item (in trader trade slot) requires reagents even if triggered spell
        if (!checkReagents)
            if (Item* targetItem = m_targets.GetItemTarget())
                if (targetItem->GetOwnerGUID() != m_caster->GetGUID())
                    checkReagents = true;

        // check reagents (ignore triggered spells with reagents processed by original spell) and special reagent ignore case.
        if (checkReagents)
        {
            for (uint32 i = 0; i < MAX_SPELL_REAGENTS; i++)
            {
                if (m_spellInfo->Reagent[i] <= 0)
                    continue;

                uint32 itemid    = m_spellInfo->Reagent[i];
                uint32 itemcount = m_spellInfo->ReagentCount[i];

                // if CastItem is also spell reagent
                if (m_CastItem && m_CastItem->GetEntry() == itemid)
                {
                    ItemTemplate const* proto = m_CastItem->GetTemplate();
                    if (!proto)
                        return SPELL_FAILED_ITEM_NOT_READY;
                    for (int s=0; s < MAX_ITEM_PROTO_SPELLS; ++s)
                    {
                        // CastItem will be used up and does not count as reagent
                        int32 charges = m_CastItem->GetSpellCharges(s);
                        if (proto->Spells[s].SpellCharges < 0 && abs(charges) < 2)
                        {
                            ++itemcount;
                            break;
                        }
                    }
                }
                if (!p_caster->HasItemCount(itemid, itemcount))
                    return SPELL_FAILED_REAGENTS;
            }
        }

        // check totem-item requirements (items presence in inventory)
        uint32 totems = 2;
        for (int i = 0; i < 2; ++i)
        {
            if (m_spellInfo->Totem[i] != 0)
            {
                if (p_caster->HasItemCount(m_spellInfo->Totem[i]))
                {
                    totems -= 1;
                    continue;
                }
            }
            else
            totems -= 1;
        }
        if (totems != 0)
            return SPELL_FAILED_TOTEMS;
    }

    // special checks for spell effects
    for (int i = 0; i < m_spellInfo->EffectCount; i++)
    {
        switch (m_spellInfo->Effects[i].Effect)
        {
            case SPELL_EFFECT_CREATE_ITEM:
            case SPELL_EFFECT_CREATE_ITEM_2:
            {
                if (!IsTriggered() && m_spellInfo->Effects[i].ItemType)
                {
                    ItemPosCountVec dest;
                    InventoryResult msg = p_caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, m_spellInfo->Effects[i].ItemType, 1);
                    if (msg != EQUIP_ERR_OK)
                    {
                        ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(m_spellInfo->Effects[i].ItemType);
                        // TODO: Needs review
                        if (pProto && !(pProto->ItemLimitCategory))
                        {
                            p_caster->SendEquipError(msg, NULL, NULL, m_spellInfo->Effects[i].ItemType);
                            return SPELL_FAILED_DONT_REPORT;
                        }
                        else
                        {
                            if (!(m_spellInfo->Id == 759)) // Conjure Mana Gem
                                return SPELL_FAILED_TOO_MANY_OF_ITEM;
                            else if (!(p_caster->HasItemCount(m_spellInfo->Effects[i].ItemType)))
                                return SPELL_FAILED_TOO_MANY_OF_ITEM;
                            else if (!(m_spellInfo->Id == 759))
                                p_caster->CastSpell(m_caster, m_spellInfo->Effects[EFFECT_1].CalcValue(), false);        // move this to anywhere
                            return SPELL_FAILED_DONT_REPORT;
                        }
                    }
                }
                break;
            }
            case SPELL_EFFECT_ENCHANT_ITEM:
                if (m_spellInfo->Effects[i].ItemType && m_targets.GetItemTarget()
                    && (m_targets.GetItemTarget()->IsVellum()))
                {
                    // cannot enchant vellum for other player
                    if (m_targets.GetItemTarget()->GetOwner() != m_caster)
                        return SPELL_FAILED_NOT_TRADEABLE;
                    // do not allow to enchant vellum from scroll made by vellum-prevent exploit
                    if (m_CastItem && m_CastItem->GetTemplate()->Flags & ITEM_FLAG_TRIGGERED_CAST)
                        return SPELL_FAILED_TOTEM_CATEGORY;
                    ItemPosCountVec dest;
                    InventoryResult msg = p_caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, m_spellInfo->Effects[i].ItemType, 1);
                    if (msg != EQUIP_ERR_OK)
                    {
                        p_caster->SendEquipError(msg, NULL, NULL, m_spellInfo->Effects[i].ItemType);
                        return SPELL_FAILED_DONT_REPORT;
                    }
                }
            case SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC:
            {
                Item* targetItem = m_targets.GetItemTarget();
                if (!targetItem)
                    return SPELL_FAILED_ITEM_NOT_FOUND;

                uint32 l_ItemLevel = 0;
                if (Player* l_Owner = targetItem->GetOwner())
                    l_ItemLevel = l_Owner->GetEquipItemLevelFor(targetItem->GetTemplate(), targetItem);
                else
                    l_ItemLevel = targetItem->GetTemplate()->ItemLevel;

                if (l_ItemLevel < m_spellInfo->BaseLevel)
                    return SPELL_FAILED_LOWLEVEL;

                if (m_spellInfo->MaxLevel > 0 && l_ItemLevel > m_spellInfo->MaxLevel)
                    return SPELL_FAILED_HIGHLEVEL;

                bool isItemUsable = false; m_spellInfo->MaxLevel;
                for (uint8 e = 0; e < MAX_ITEM_PROTO_SPELLS; ++e)
                {
                    ItemTemplate const* proto = targetItem->GetTemplate();
                    if (proto->Spells[e].SpellId && (
                        proto->Spells[e].SpellTrigger == ITEM_SPELLTRIGGER_ON_USE ||
                        proto->Spells[e].SpellTrigger == ITEM_SPELLTRIGGER_ON_NO_DELAY_USE))
                    {
                        isItemUsable = true;
                        break;
                    }
                }

                SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(m_spellInfo->Effects[i].MiscValue);
                // do not allow adding usable enchantments to items that have use effect already
                if (pEnchant && isItemUsable)
                    for (uint8 s = 0; s < MAX_ENCHANTMENT_SPELLS; ++s)
                        if (pEnchant->type[s] == ITEM_ENCHANTMENT_TYPE_USE_SPELL)
                            return SPELL_FAILED_ON_USE_ENCHANT;

                // Not allow enchant in trade slot for some enchant type
                if (targetItem->GetOwner() != m_caster)
                {
                    if (!pEnchant)
                        return SPELL_FAILED_ERROR;
                    if (pEnchant->slot & ENCHANTMENT_CAN_SOULBOUND)
                        return SPELL_FAILED_NOT_TRADEABLE;
                }
                break;
            }
            case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
            {
                Item* item = m_targets.GetItemTarget();
                if (!item)
                    return SPELL_FAILED_ITEM_NOT_FOUND;
                // Not allow enchant in trade slot for some enchant type
                if (item->GetOwner() != m_caster)
                {
                    uint32 enchant_id = m_spellInfo->Effects[i].MiscValue;
                    SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
                    if (!pEnchant)
                        return SPELL_FAILED_ERROR;
                    if (pEnchant->slot & ENCHANTMENT_CAN_SOULBOUND)
                        return SPELL_FAILED_NOT_TRADEABLE;
                }
                break;
            }
            case SPELL_EFFECT_ENCHANT_HELD_ITEM:
                // check item existence in effect code (not output errors at offhand hold item effect to main hand for example
                break;
            case SPELL_EFFECT_DISENCHANT:
            {
                if (!m_targets.GetItemTarget())
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;

                // prevent disenchanting in trade slot
                if (m_targets.GetItemTarget()->GetOwnerGUID() != m_caster->GetGUID())
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;

                ItemTemplate const* itemProto = m_targets.GetItemTarget()->GetTemplate();
                if (!itemProto)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;

                uint32 item_quality = itemProto->Quality;
                // 2.0.x addon: Check player enchanting level against the item disenchanting requirements
                uint32 item_disenchantskilllevel = itemProto->RequiredDisenchantSkill;
                if (item_disenchantskilllevel == uint32(-1))
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                if (item_disenchantskilllevel > p_caster->GetSkillValue(SKILL_ENCHANTING))
                    return SPELL_FAILED_LOW_CASTLEVEL;
                if (item_quality > 4 || item_quality < 2)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                if (itemProto->Class != ITEM_CLASS_WEAPON && itemProto->Class != ITEM_CLASS_ARMOR)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                if (!itemProto->DisenchantID)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                break;
            }
            case SPELL_EFFECT_PROSPECTING:
            {
                if (!m_targets.GetItemTarget())
                    return SPELL_FAILED_CANT_BE_PROSPECTED;
                //ensure item is a prospectable ore
                if (!(m_targets.GetItemTarget()->GetTemplate()->Flags & ITEM_FLAG_PROSPECTABLE))
                    return SPELL_FAILED_CANT_BE_PROSPECTED;
                //prevent prospecting in trade slot
                if (m_targets.GetItemTarget()->GetOwnerGUID() != m_caster->GetGUID())
                    return SPELL_FAILED_CANT_BE_PROSPECTED;
                //Check for enough skill in jewelcrafting
                uint32 item_prospectingskilllevel = m_targets.GetItemTarget()->GetTemplate()->RequiredSkillRank;
                if (item_prospectingskilllevel >p_caster->GetSkillValue(SKILL_JEWELCRAFTING))
                    return SPELL_FAILED_LOW_CASTLEVEL;
                //make sure the player has the required ores in inventory
                if (m_targets.GetItemTarget()->GetCount() < 5)
                    return SPELL_FAILED_NEED_MORE_ITEMS;

                if (!LootTemplates_Prospecting.HaveLootFor(m_targets.GetItemTargetEntry()))
                    return SPELL_FAILED_CANT_BE_PROSPECTED;

                break;
            }
            case SPELL_EFFECT_MILLING:
            {
                if (!m_targets.GetItemTarget())
                    return SPELL_FAILED_CANT_BE_MILLED;
                //ensure item is a millable herb
                if (!(m_targets.GetItemTarget()->GetTemplate()->Flags & ITEM_FLAG_MILLABLE))
                    return SPELL_FAILED_CANT_BE_MILLED;
                //prevent milling in trade slot
                if (m_targets.GetItemTarget()->GetOwnerGUID() != m_caster->GetGUID())
                    return SPELL_FAILED_CANT_BE_MILLED;
                //Check for enough skill in inscription
                uint32 item_millingskilllevel = m_targets.GetItemTarget()->GetTemplate()->RequiredSkillRank;
                if (item_millingskilllevel >p_caster->GetSkillValue(SKILL_INSCRIPTION))
                    return SPELL_FAILED_LOW_CASTLEVEL;
                //make sure the player has the required herbs in inventory
                if (m_targets.GetItemTarget()->GetCount() < 5)
                    return SPELL_FAILED_NEED_MORE_ITEMS;

                if (!LootTemplates_Milling.HaveLootFor(m_targets.GetItemTargetEntry()))
                    return SPELL_FAILED_CANT_BE_MILLED;

                break;
            }
            case SPELL_EFFECT_WEAPON_DAMAGE:
            case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_TARGET_NOT_PLAYER;

                if (m_attackType != WeaponAttackType::RangedAttack)
                    break;

                Item* pItem = m_caster->ToPlayer()->GetWeaponForAttack(m_attackType);
                if (!pItem || pItem->CantBeUse())
                    return SPELL_FAILED_EQUIPPED_ITEM;

                switch (pItem->GetTemplate()->SubClass)
                {
                    case ITEM_SUBCLASS_WEAPON_THROWN:
                    {
                        uint32 ammo = pItem->GetEntry();
                        if (!m_caster->ToPlayer()->HasItemCount(ammo))
                            return SPELL_FAILED_NO_AMMO;
                    };
                    break;
                    case ITEM_SUBCLASS_WEAPON_GUN:
                    case ITEM_SUBCLASS_WEAPON_BOW:
                    case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                    case ITEM_SUBCLASS_WEAPON_WAND:
                        break;
                    default:
                        break;
                }
                break;
            }
            case SPELL_EFFECT_CREATE_MANA_GEM:
            {
                 uint32 item_id = m_spellInfo->Effects[i].ItemType;
                 ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item_id);

                 if (!pProto)
                     return SPELL_FAILED_ITEM_AT_MAX_CHARGES;

                 if (Item* pitem = p_caster->GetItemByEntry(item_id))
                 {
                     for (int x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
                         if (pProto->Spells[x].SpellCharges != 0 && pitem->GetSpellCharges(x) == pProto->Spells[x].SpellCharges)
                             return SPELL_FAILED_ITEM_AT_MAX_CHARGES;
                 }
                 break;
            }
            case SPELL_EFFECT_CHANGE_ITEM_BONUSES:
            {
                Item* l_ItemTarget = m_targets.GetItemTarget();
                if (l_ItemTarget == nullptr)
                    return SPELL_FAILED_NO_VALID_TARGETS;

                uint32 l_OldItemBonusTreeCategory = m_spellInfo->Effects[i].MiscValue;
                uint32 l_NewItemBonusTreeCategory = m_spellInfo->Effects[i].MiscValueB;

                std::vector<uint32> const& l_CurrentItemBonus = l_ItemTarget->GetAllItemBonuses();
                if (l_OldItemBonusTreeCategory == l_NewItemBonusTreeCategory)
                {
                    uint32 l_MaxIlevel = 0;
                    bool   l_Found = false;

                    auto& l_ItemStageUpgradeRules = sSpellMgr->GetSpellUpgradeItemStage(l_OldItemBonusTreeCategory);
                    if (!l_ItemStageUpgradeRules.empty())
                    {
                        for (auto l_Itr : l_ItemStageUpgradeRules)
                        {
                            if (l_Itr.ItemClass != l_ItemTarget->GetTemplate()->Class)
                                continue;

                            if (l_Itr.ItemSubclassMask != 0)
                            {
                                if ((l_Itr.ItemSubclassMask & (1 << l_ItemTarget->GetTemplate()->SubClass)) == 0)
                                    continue;
                            }

                            if (l_Itr.InventoryTypeMask != 0)
                            {
                                if ((l_Itr.InventoryTypeMask & (1 << l_ItemTarget->GetTemplate()->InventoryType)) == 0)
                                    continue;
                            }

                            if (int32(l_ItemTarget->GetTemplate()->ItemLevel + l_ItemTarget->GetItemLevelBonusFromItemBonuses()) >= l_Itr.MaxIlevel)
                                continue;

                            l_Found = true;
                            l_MaxIlevel = l_Itr.MaxIlevel;
                            break;
                        }

                        if (!l_Found)
                            return SPELL_FAILED_NO_VALID_TARGETS;

                        std::vector<uint32> l_UpgradeBonusStages;

                        switch (l_ItemTarget->GetTemplate()->ItemLevel)
                        {
                            case 630:
                                l_UpgradeBonusStages = { 525, 558, 559, 594, 619, 620 };
                                break;
                            case 640:
                                l_UpgradeBonusStages = { 525, 526, 527, 593, 617, 618 };
                            default:
                                break;
                        }

                        if (l_UpgradeBonusStages.empty())
                            return SPELL_FAILED_NO_VALID_TARGETS;

                        int32 l_CurrentIdx = -1;

                        for (int l_Idx = 0; l_Idx < (int)l_UpgradeBonusStages.size(); l_Idx++)
                        {
                            for (auto l_BonusId : l_CurrentItemBonus)
                            {
                                if (l_BonusId == l_UpgradeBonusStages[l_Idx])
                                {
                                    l_CurrentIdx = l_Idx;
                                    break;
                                }
                            }
                        }

                        if (l_CurrentIdx == -1 || l_CurrentIdx == l_UpgradeBonusStages.size() - 1)
                            return SPELL_FAILED_NO_VALID_TARGETS;
                    }
                }
                break;
            }
            default:
                break;
        }
    }

    // check weapon presence in slots for main/offhand weapons
    if (m_spellInfo->EquippedItemClass >=0)
    {
        // main hand weapon required
        if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_MAIN_HAND)
        {
            Item* item = m_caster->ToPlayer()->GetWeaponForAttack(WeaponAttackType::BaseAttack);

            // skip spell if no weapon in slot or broken
            if (!item || item->CantBeUse())
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;

            // skip spell if weapon not fit to triggered spell
            if (!item->IsFitToSpellRequirements(m_spellInfo))
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;
        }

        // offhand hand weapon required
        if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_REQ_OFFHAND)
        {
            Item* item = m_caster->ToPlayer()->GetWeaponForAttack(WeaponAttackType::OffAttack);

            // skip spell if no weapon in slot or broken
            if (!item || item->CantBeUse())
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;

            // skip spell if weapon not fit to triggered spell
            if (!item->IsFitToSpellRequirements(m_spellInfo))
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;
        }
    }

    return SPELL_CAST_OK;
}

/// Called only in Unit::DealDamage
void Spell::Delayed()
{
    /// Spell is active and can't be time-backed
    if (!m_caster)
        return;

    /// Spells may only be delayed twice
    if (isDelayableNoMore())
        return;

    /// Check pushback reduce
    /// Spellcasting delay is normally 150ms since WOD
    int32 l_DelayTime = 150;

    /// Must be initialized to 100 for percent modifiers
    int32 l_DelayReduce = 100;

    m_caster->ToPlayer()->ApplySpellMod(m_spellInfo->Id, SPELLMOD_NOT_LOSE_CASTING_TIME, l_DelayReduce, this);

    l_DelayReduce += m_caster->GetTotalAuraModifier(SPELL_AURA_REDUCE_PUSHBACK) - 100;

    if (l_DelayReduce >= 100)
        return;

    AddPct(l_DelayTime, -l_DelayReduce);

    if (m_timer + l_DelayTime > m_casttime)
    {
        l_DelayTime = m_casttime - m_timer;
        m_timer = m_casttime;
    }
    else
        m_timer += l_DelayTime;

    WorldPacket l_Data(SMSG_SPELL_DELAYED, 8 + 4);
    l_Data.appendPackGUID(m_caster->GetGUID());
    l_Data << uint32(l_DelayTime);

    m_caster->SendMessageToSet(&l_Data, true);
}

void Spell::DelayedChannel()
{
    if (!m_caster || m_caster->GetTypeId() != TYPEID_PLAYER || getState() != SPELL_STATE_CASTING)
        return;

    if (isDelayableNoMore())                                    // Spells may only be delayed twice
        return;

    //check pushback reduce
    // should be affected by modifiers, not take the dbc duration.
    int32 duration = ((m_channeledDuration > 0) ? m_channeledDuration : m_spellInfo->GetDuration());

    int32 delaytime = CalculatePct(duration, 25); // channeling delay is normally 25% of its time per hit
    int32 delayReduce = 100;                                    // must be initialized to 100 for percent modifiers
    m_caster->ToPlayer()->ApplySpellMod(m_spellInfo->Id, SPELLMOD_NOT_LOSE_CASTING_TIME, delayReduce, this);
    delayReduce += m_caster->GetTotalAuraModifier(SPELL_AURA_REDUCE_PUSHBACK) - 100;
    if (delayReduce >= 100)
        return;

    AddPct(delaytime, -delayReduce);

    if (m_timer <= delaytime)
    {
        delaytime = m_timer;
        m_timer = 0;
    }
    else
        m_timer -= delaytime;

    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        if ((*ihit).missCondition == SPELL_MISS_NONE)
            if (Unit* unit = (m_caster->GetGUID() == ihit->targetGUID) ? m_caster : ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID))
                unit->DelayOwnedAuras(m_spellInfo->Id, m_originalCasterGUID, delaytime);

    // partially interrupt persistent area auras
    if (DynamicObject* dynObj = m_caster->GetDynObject(m_spellInfo->Id))
        dynObj->Delay(delaytime);

    SendChannelUpdate(m_timer);
}

void Spell::UpdatePointers()
{
    if (m_originalCasterGUID == m_caster->GetGUID())
        m_originalCaster = m_caster;
    else
    {
        m_originalCaster = ObjectAccessor::GetUnit(*m_caster, m_originalCasterGUID);
        if (m_originalCaster && !m_originalCaster->IsInWorld())
            m_originalCaster = NULL;
    }

    if (m_castItemGUID && m_caster->IsPlayer())
        m_CastItem = m_caster->ToPlayer()->GetItemByGuid(m_castItemGUID);

    m_targets.Update(m_caster);

    // further actions done only for dest targets
    if (!m_targets.HasDst())
        return;

    // cache last transport
    WorldObject* transport = NULL;

    // update effect destinations (in case of moved transport dest target)
    for (uint8 effIndex = 0; effIndex < m_spellInfo->EffectCount; ++effIndex)
    {
        SpellDestination& dest = m_destTargets[effIndex];
        if (!dest._transportGUID)
            continue;

        if (!transport || transport->GetGUID() != dest._transportGUID)
            transport = ObjectAccessor::GetWorldObject(*m_caster, dest._transportGUID);

        if (transport)
        {
            dest._position.Relocate(transport);
            dest._position.RelocateOffset(dest._transportOffset);
        }
    }
}

CurrentSpellTypes Spell::GetCurrentContainer() const
{
    if (IsNextMeleeSwingSpell())
        return(CURRENT_MELEE_SPELL);
    else if (IsAutoRepeat())
        return(CURRENT_AUTOREPEAT_SPELL);
    else if (m_spellInfo->IsChanneled())
        return(CURRENT_CHANNELED_SPELL);
    else
        return(CURRENT_GENERIC_SPELL);
}

bool Spell::CheckEffectTarget(Unit const* target, uint32 eff) const
{
    switch (m_spellInfo->Effects[eff].ApplyAuraName)
    {
        case SPELL_AURA_MOD_POSSESS:
        case SPELL_AURA_MOD_CHARM:
        case SPELL_AURA_MOD_POSSESS_PET:
        case SPELL_AURA_AOE_CHARM:
            if (target->GetTypeId() == TYPEID_UNIT && target->IsVehicle())
                return false;
            if (target->IsMounted())
                return false;
            if (target->GetCharmerGUID())
                return false;
            if (int32 damage = CalculateDamage(eff, target))
                if ((int32)target->getLevel() > damage)
                    return false;
            break;
        default:
            break;
    }

    // Hour of Twilight
    if (m_spellInfo->Id == 103327)
    {
        // Deterrence
        if (target->HasAura(19263))
            return false;
    }

    if (!m_spellInfo->IsNeedAdditionalLosChecks() && (IsTriggered() || m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS))
        return true;

    // Hack fix for Ice Tombs (Sindragosa encounter)
    if (target->GetTypeId() == TYPEID_UNIT)
        if (target->GetEntry() == 36980 || target->GetEntry() == 38320 || target->GetEntry() == 38321 || target->GetEntry() == 38322)
            return true;

    // todo: shit below shouldn't be here, but it's temporary (21/04/2015: look "temporary for life")
    //Check targets for LOS visibility (except spells without range limitations)
    switch (m_spellInfo->Effects[eff].Effect)
    {
        case SPELL_EFFECT_RESURRECT_NEW:
            // player far away, maybe his corpse near?
            if (target != m_caster && !target->IsWithinLOSInMap(m_caster))
            {
                if (!m_targets.GetCorpseTargetGUID())
                    return false;

                Corpse* corpse = ObjectAccessor::GetCorpse(*m_caster, m_targets.GetCorpseTargetGUID());
                if (!corpse)
                    return false;

                if (target->GetGUID() != corpse->GetOwnerGUID())
                    return false;

                if (!corpse->IsWithinLOSInMap(m_caster))
                    return false;
            }

            // all ok by some way or another, skip normal check
            break;
        default:                                            // normal case
            // Get GO cast coordinates if original caster -> GO
            WorldObject* caster = NULL;
            if (IS_GAMEOBJECT_GUID(m_originalCasterGUID))
                caster = m_caster->GetMap()->GetGameObject(m_originalCasterGUID);
            if (!caster)
                caster = m_caster;

            // Sin and Punishment from duel bug
            if (m_spellInfo->Id == 87204)
            {
                if (caster != target && caster->IsPlayer() && caster->ToPlayer()->m_Duel && target->IsPlayer())
                    return false;
            }
            // Glyph of Concussive Shot
            else if (m_spellInfo->Id == 5116)
            {
                if (eff == EFFECT_1 && !m_caster->HasAura(56851))
                    return false;
            }

            if (target->GetEntry() == 5925)
                return true;
            if (LOSAdditionalRules(target))
                return true;

            if (m_targets.HasDst())
            {
                float x, y, z;
                m_targets.GetDstPos()->GetPosition(x, y, z);

                if (!target->IsWithinLOS(x, y, z))
                    return false;
            }
            else if (target != m_caster && !target->IsWithinLOSInMap(caster))
                return false;
            break;
    }

    return true;
}

bool Spell::IsNextMeleeSwingSpell() const
{
    return m_spellInfo->Attributes & SPELL_ATTR0_ON_NEXT_SWING;
}

bool Spell::IsAutoActionResetSpell() const
{
    // TODO: changed SPELL_INTERRUPT_FLAG_AUTOATTACK -> SPELL_INTERRUPT_FLAG_INTERRUPT to fix compile - is this check correct at all?
    return !IsTriggered() && (m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_INTERRUPT);
}

bool Spell::IsNeedSendToClient() const
{
    return m_spellInfo->SpellVisual[0] || m_spellInfo->SpellVisual[1] || m_spellInfo->IsChanneled() || m_spellInfo->HasEffect(SpellEffects::SPELL_EFFECT_LOOT_BONUS) ||
        (m_spellInfo->AttributesEx8 & SPELL_ATTR8_AURA_SEND_AMOUNT) || m_spellInfo->Speed > 0.0f || (!m_triggeredByAuraSpell && !IsTriggered());
}

bool Spell::HaveTargetsForEffect(uint8 effect) const
{
    for (std::list<TargetInfo>::const_iterator itr = m_UniqueTargetInfo.begin(); itr != m_UniqueTargetInfo.end(); ++itr)
        if (itr->effectMask & (1 << effect))
            return true;

    for (std::list<GOTargetInfo>::const_iterator itr = m_UniqueGOTargetInfo.begin(); itr != m_UniqueGOTargetInfo.end(); ++itr)
        if (itr->effectMask & (1 << effect))
            return true;

    for (std::list<ItemTargetInfo>::const_iterator itr = m_UniqueItemInfo.begin(); itr != m_UniqueItemInfo.end(); ++itr)
        if (itr->effectMask & (1 << effect))
            return true;

    return false;
}

SpellEvent::SpellEvent(Spell* spell) : BasicEvent()
{
    m_Spell = spell;
}

SpellEvent::~SpellEvent()
{
    if (m_Spell->getState() != SPELL_STATE_FINISHED)
        m_Spell->cancel();

    if (m_Spell->IsDeletable())
    {
        delete m_Spell;
    }
    else
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "~SpellEvent: %s %u tried to delete non-deletable spell %u. Was not deleted, causes memory leak.",
            (m_Spell->GetCaster()->IsPlayer() ? "Player" : "Creature"), m_Spell->GetCaster()->GetGUIDLow(), m_Spell->m_spellInfo->Id);
        ASSERT(false);
    }
}

bool SpellEvent::Execute(uint64 e_time, uint32 p_time)
{
    // update spell if it is not finished
    if (m_Spell->getState() != SPELL_STATE_FINISHED)
        m_Spell->update(p_time);

    // check spell state to process
    switch (m_Spell->getState())
    {
        case SPELL_STATE_FINISHED:
        {
            // spell was finished, check deletable state
            if (m_Spell->IsDeletable())
            {
                // check, if we do have unfinished triggered spells
                return true;                                // spell is deletable, finish event
            }
            // event will be re-added automatically at the end of routine)
        } break;

        case SPELL_STATE_DELAYED:
        {
            // first, check, if we have just started
            if (m_Spell->GetDelayStart() != 0)
            {
                {
                    // run the spell handler and think about what we can do next
                    uint64 t_offset = e_time - m_Spell->GetDelayStart();
                    uint64 n_offset = m_Spell->handle_delayed(t_offset);
                    if (n_offset)
                    {
                        // re-add us to the queue
                        m_Spell->GetCaster()->m_Events.AddEvent(this, m_Spell->GetDelayStart() + n_offset, false);
                        return false;                       // event not complete
                    }
                    // event complete
                    // finish update event will be re-added automatically at the end of routine)
                }
            }
            else
            {
                // delaying had just started, record the moment
                m_Spell->SetDelayStart(e_time);
                // re-plan the event for the delay moment
                m_Spell->GetCaster()->m_Events.AddEvent(this, e_time + m_Spell->GetDelayMoment(), false);
                return false;                               // event not complete
            }
        } break;

        default:
        {
            // all other states
            // event will be re-added automatically at the end of routine)
        } break;
    }

    // spell processing not complete, plan event on the next update interval
    m_Spell->GetCaster()->m_Events.AddEvent(this, e_time + 1, false);
    return false;                                           // event not complete
}

void SpellEvent::Abort(uint64 /*e_time*/)
{
    // oops, the spell we try to do is aborted
    if (m_Spell->getState() != SPELL_STATE_FINISHED)
        m_Spell->cancel();
}

bool SpellEvent::IsDeletable() const
{
    return m_Spell->IsDeletable();
}

bool Spell::IsValidDeadOrAliveTarget(Unit const* target) const
{
    if (target->isAlive())
        return !m_spellInfo->IsRequiringDeadTarget();
    if (m_spellInfo->IsAllowingDeadTarget())
        return true;
    return false;
}

void Spell::HandleLaunchPhase()
{
    // handle effects with SPELL_EFFECT_HANDLE_LAUNCH mode
    for (uint32 i = 0; i < m_spellInfo->EffectCount; ++i)
    {
        // don't do anything for empty effect
        if (!m_spellInfo->Effects[i].IsEffect())
            continue;

        HandleEffects(nullptr, nullptr, nullptr, nullptr, i, SPELL_EFFECT_HANDLE_LAUNCH);
    }

    float multiplier[SpellEffIndex::MAX_EFFECTS];
    for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
        if (m_applyMultiplierMask & (1 << i))
            multiplier[i] = m_spellInfo->Effects[i].CalcDamageMultiplier(m_originalCaster, this);

    bool usesAmmo = m_spellInfo->AttributesCu & SPELL_ATTR0_CU_DIRECT_DAMAGE;

    for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        TargetInfo& target = *ihit;

        uint32 mask = target.effectMask;
        if (!mask)
            continue;

        // do not consume ammo anymore for Hunter's volley spell
        if (IsTriggered() && m_spellInfo->SpellFamilyName == SPELLFAMILY_HUNTER && m_spellInfo->IsTargetingArea())
            usesAmmo = false;

        if (usesAmmo)
        {
            bool ammoTaken = false;
            for (uint8 i = 0; i < m_spellInfo->EffectCount; i++)
            {
                if (!(mask & 1<<i))
                    continue;

                switch (m_spellInfo->Effects[i].Effect)
                {
                    case SPELL_EFFECT_SCHOOL_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                    case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                    case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                    ammoTaken=true;
                    TakeAmmo();
                }
                if (ammoTaken)
                    break;
            }
        }
        DoAllEffectOnLaunchTarget(target, multiplier);
    }
}

void Spell::DoAllEffectOnLaunchTarget(TargetInfo& targetInfo, float* multiplier)
{
    Unit* unit = NULL;
    // In case spell hit target, do all effect on that target
    if (targetInfo.missCondition == SPELL_MISS_NONE)
        unit = m_caster->GetGUID() == targetInfo.targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, targetInfo.targetGUID);
    // In case spell reflect from target, do all effect on caster (if hit)
    else if (targetInfo.missCondition == SPELL_MISS_REFLECT && targetInfo.reflectResult == SPELL_MISS_NONE)
        unit = m_caster;
    if (!unit)
        return;

    for (uint32 i = 0; i < m_spellInfo->EffectCount; ++i)
    {
        if (targetInfo.effectMask & (1<<i))
        {
            m_damage = 0;
            m_healing = 0;

            HandleEffects(unit, nullptr, nullptr, nullptr, i, SPELL_EFFECT_HANDLE_LAUNCH_TARGET);

            if (m_damage > 0)
            {
                if (m_spellInfo->Effects[i].IsTargetingArea())
                {
                    if (m_caster->IsPlayer())
                    {
                        uint32 targetAmount = m_UniqueTargetInfo.size();
                        if (targetAmount > 20)
                            m_damage = m_damage * 20 / targetAmount;

                        // Hack Fix Frost Bomb, Beast Cleave : Doesn't add AoE damage to main target
                        if (m_spellInfo->Id == 113092 || m_spellInfo->Id == 118459)
                        {
                            if (targetInfo.targetGUID == (*m_UniqueTargetInfo.begin()).targetGUID)
                                continue;
                        }
                    }
                }
            }
            else if (m_damage < 0)
            {
                switch (m_spellInfo->Id)
                {
                    case 73921: // Healing Rain
                    case 88686: // Holy Word : Sanctuary
                    {
                        if (m_caster->IsPlayer())
                        {
                            uint32 targetAmount = m_UniqueTargetInfo.size();
                            if (targetAmount > 6)
                                m_damage = (m_damage * 6) / int32(targetAmount);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            if (m_applyMultiplierMask & (1 << i))
            {
                m_damage = int32(m_damage * m_damageMultipliers[i]);
                m_damageMultipliers[i] *= multiplier[i];
            }
            targetInfo.damage += m_damage;
        }
    }
    
    if (m_CustomCritChance < 0.f)
        targetInfo.crit = m_caster->IsSpellCrit(unit, m_spellInfo, m_spellSchoolMask, m_attackType);
    else
        targetInfo.crit = roll_chance_f(m_CustomCritChance);
}

SpellCastResult Spell::CanOpenLock(uint32 effIndex, uint32 lockId, SkillType& skillId, int32& reqSkillValue, int32& skillValue)
{
    if (!lockId)                                             // possible case for GO and maybe for items.
        return SPELL_CAST_OK;

    // Get LockInfo
    LockEntry const* lockInfo = sLockStore.LookupEntry(lockId);

    if (!lockInfo)
        return SPELL_FAILED_BAD_TARGETS;

    bool reqKey = false;                                    // some locks not have reqs

    for (int j = 0; j < MAX_LOCK_CASE; ++j)
    {
        switch (lockInfo->Type[j])
        {
            // check key item (many fit cases can be)
            case LOCK_KEY_ITEM:
                if (lockInfo->Index[j] && m_CastItem && m_CastItem->GetEntry() == lockInfo->Index[j])
                    return SPELL_CAST_OK;
                reqKey = true;
                break;
                // check key skill (only single first fit case can be)
            case LOCK_KEY_SKILL:
            {
                reqKey = true;

                // wrong locktype, skip
                if (uint32(m_spellInfo->Effects[effIndex].MiscValue) != lockInfo->Index[j])
                    continue;

                skillId = SkillByLockType(LockType(lockInfo->Index[j]));

                if (skillId != SKILL_NONE)
                {
                    reqSkillValue = lockInfo->Skill[j];

                    // castitem check: rogue using skeleton keys. the skill values should not be added in this case.
                    skillValue = m_CastItem || m_caster->GetTypeId()!= TYPEID_PLAYER ?
                        0 : m_caster->ToPlayer()->GetSkillValue(skillId);

                    // skill bonus provided by casting spell (mostly item spells)
                    // add the effect base points modifier from the spell casted (cheat lock / skeleton key etc.)
                    if (m_spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_GAMEOBJECT_ITEM_TARGET || m_spellInfo->Effects[effIndex].TargetB.GetTarget() == TARGET_GAMEOBJECT_ITEM_TARGET)
                        skillValue += m_spellInfo->Effects[effIndex].CalcValue(m_caster, 0);

                    if (skillValue < reqSkillValue)
                        return SPELL_FAILED_LOW_CASTLEVEL;
                }

                return SPELL_CAST_OK;
            }
        }
    }

    if (reqKey)
        return SPELL_FAILED_BAD_TARGETS;

    return SPELL_CAST_OK;
}

void Spell::SetSpellValue(SpellValueMod mod, int32 value)
{
    switch (mod)
    {
        case SPELLVALUE_BASE_POINT0:
            m_spellValue->EffectBasePoints[0] = m_spellInfo->Effects[EFFECT_0].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT1:
            m_spellValue->EffectBasePoints[1] = m_spellInfo->Effects[EFFECT_1].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT2:
            m_spellValue->EffectBasePoints[2] = m_spellInfo->Effects[EFFECT_2].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT3:
            m_spellValue->EffectBasePoints[3] = m_spellInfo->Effects[EFFECT_3].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT4:
            m_spellValue->EffectBasePoints[4] = m_spellInfo->Effects[EFFECT_4].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT5:
            m_spellValue->EffectBasePoints[5] = m_spellInfo->Effects[EFFECT_5].CalcBaseValue(value);
            break;
        case SPELLVALUE_RADIUS_MOD:
            m_spellValue->RadiusMod = (float)value / 10000;
            break;
        case SPELLVALUE_MAX_TARGETS:
            m_spellValue->MaxAffectedTargets = (uint32)value;
            break;
        case SPELLVALUE_AURA_STACK:
            m_spellValue->AuraStackAmount = uint8(value);
            break;
    }
}

int32 Spell::GetSpellValue(SpellValueMod p_Mod) const
{
    switch (p_Mod)
    {
        case SPELLVALUE_BASE_POINT0:
            return m_spellValue->EffectBasePoints[0];
        case SPELLVALUE_BASE_POINT1:
            return m_spellValue->EffectBasePoints[1];
        case SPELLVALUE_BASE_POINT2:
            return m_spellValue->EffectBasePoints[2];
        case SPELLVALUE_BASE_POINT3:
            return m_spellValue->EffectBasePoints[3];
        case SPELLVALUE_BASE_POINT4:
            return m_spellValue->EffectBasePoints[4];
        case SPELLVALUE_BASE_POINT5:
            return m_spellValue->EffectBasePoints[5];
        case SPELLVALUE_RADIUS_MOD:
            return m_spellValue->RadiusMod;
        case SPELLVALUE_MAX_TARGETS:
            return m_spellValue->MaxAffectedTargets;
        case SPELLVALUE_AURA_STACK:
            return m_spellValue->AuraStackAmount;
        default:
            return 0;
    }
}

void Spell::PrepareTargetProcessing()
{
    CheckEffectExecuteData();
}

void Spell::FinishTargetProcessing()
{
    SendLogExecute();
}

void Spell::InitEffectExecuteData(uint8 effIndex)
{
}

void Spell::CheckEffectExecuteData()
{
}

void Spell::LoadScripts()
{
    sScriptMgr->CreateSpellScripts(m_spellInfo->Id, m_loadedScripts);
    for (std::list<SpellScript*>::iterator itr = m_loadedScripts.begin(); itr != m_loadedScripts.end();)
    {
        if (!(*itr)->_Load(this))
        {
            std::list<SpellScript*>::iterator bitr = itr;
            ++itr;
            delete (*bitr);
            m_loadedScripts.erase(bitr);
            continue;
        }
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::LoadScripts: Script `%s` for spell `%u` is loaded now", (*itr)->_GetScriptName()->c_str(), m_spellInfo->Id);
        (*itr)->Register();
        ++itr;
    }
}

void Spell::CallScriptBeforeCastHandlers()
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_BEFORE_CAST);
        std::list<SpellScript::CastHandler>::iterator hookItrEnd = (*scritr)->BeforeCast.end(), hookItr = (*scritr)->BeforeCast.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            (*hookItr).Call(*scritr);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

void Spell::CallScriptOnCastHandlers()
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_ON_CAST);
        std::list<SpellScript::CastHandler>::iterator hookItrEnd = (*scritr)->OnCast.end(), hookItr = (*scritr)->OnCast.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            (*hookItr).Call(*scritr);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

void Spell::CallScriptAfterCastHandlers()
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_AFTER_CAST);
        std::list<SpellScript::CastHandler>::iterator hookItrEnd = (*scritr)->AfterCast.end(), hookItr = (*scritr)->AfterCast.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            (*hookItr).Call(*scritr);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

bool Spell::CallScriptCheckInterruptHandlers()
{
    uint32 l_ScriptExecuteTime = getMSTime();
    bool l_CanInterrupt = false;

    for (std::list<SpellScript*>::iterator l_Scritr = m_loadedScripts.begin(); l_Scritr != m_loadedScripts.end(); ++l_Scritr)
    {
        (*l_Scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_CHECK_INTERRUPT);
        std::list<SpellScript::CheckInterruptHandler>::iterator l_HookItrEnd = (*l_Scritr)->OnCheckInterrupt.end(), l_HookItr = (*l_Scritr)->OnCheckInterrupt.begin();
        for (; l_HookItr != l_HookItrEnd; ++l_HookItr)
        {
            bool l_TempResult = (*l_HookItr).Call(*l_Scritr);
            if (l_TempResult == true)
                l_CanInterrupt = l_TempResult;
        }

        (*l_Scritr)->_FinishScriptCall();
    }

    l_ScriptExecuteTime = getMSTime() - l_ScriptExecuteTime;
    if (l_ScriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, l_ScriptExecuteTime);
    return l_CanInterrupt;
}

void Spell::CallScriptOnPrepareHandlers()
{
    uint32 scriptExecuteTime = getMSTime();

    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_ON_PREPARE);

        std::list<SpellScript::OnPrepareHandler>::iterator hookItrEnd = (*scritr)->OnPrepare.end(), hookItr = (*scritr)->OnPrepare.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            (*hookItr).Call(*scritr);

        (*scritr)->_FinishScriptCall();
    }
    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}


SpellCastResult Spell::CallScriptCheckCastHandlers()
{
    uint32 scriptExecuteTime = getMSTime();
    SpellCastResult retVal = SPELL_CAST_OK;
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_CHECK_CAST);
        std::list<SpellScript::CheckCastHandler>::iterator hookItrEnd = (*scritr)->OnCheckCast.end(), hookItr = (*scritr)->OnCheckCast.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
        {
            SpellCastResult tempResult = (*hookItr).Call(*scritr);
            if (retVal == SPELL_CAST_OK)
                retVal = tempResult;
        }

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
    return retVal;
}

void Spell::PrepareScriptHitHandlers()
{
    uint32 scriptExecuteTime = getMSTime();

    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
        (*scritr)->_InitHit();

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

bool Spell::CallScriptEffectHandlers(SpellEffIndex effIndex, SpellEffectHandleMode mode)
{
    uint32 scriptExecuteTime = getMSTime();

    // execute script effect handler hooks and check if effects was prevented
    bool preventDefault = false;
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        std::list<SpellScript::EffectHandler>::iterator effItr, effEndItr;
        SpellScriptHookType hookType;
        switch (mode)
        {
            case SPELL_EFFECT_HANDLE_LAUNCH:
                effItr = (*scritr)->OnEffectLaunch.begin();
                effEndItr = (*scritr)->OnEffectLaunch.end();
                hookType = SPELL_SCRIPT_HOOK_EFFECT_LAUNCH;
                break;
            case SPELL_EFFECT_HANDLE_LAUNCH_TARGET:
                effItr = (*scritr)->OnEffectLaunchTarget.begin();
                effEndItr = (*scritr)->OnEffectLaunchTarget.end();
                hookType = SPELL_SCRIPT_HOOK_EFFECT_LAUNCH_TARGET;
                break;
            case SPELL_EFFECT_HANDLE_HIT:
                effItr = (*scritr)->OnEffectHit.begin();
                effEndItr = (*scritr)->OnEffectHit.end();
                hookType = SPELL_SCRIPT_HOOK_EFFECT_HIT;
                break;
            case SPELL_EFFECT_HANDLE_HIT_TARGET:
                effItr = (*scritr)->OnEffectHitTarget.begin();
                effEndItr = (*scritr)->OnEffectHitTarget.end();
                hookType = SPELL_SCRIPT_HOOK_EFFECT_HIT_TARGET;
                break;
            default:
                ASSERT(false);
                return false;
        }
        (*scritr)->_PrepareScriptCall(hookType);
        for (; effItr != effEndItr; ++effItr)
            // effect execution can be prevented
            if (!(*scritr)->_IsEffectPrevented(effIndex) && (*effItr).IsEffectAffected(m_spellInfo, effIndex))
                (*effItr).Call(*scritr, effIndex);

        if (!preventDefault)
            preventDefault = (*scritr)->_IsDefaultEffectPrevented(effIndex);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);

    return preventDefault;
}

void Spell::CallScriptBeforeHitHandlers()
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_BEFORE_HIT);
        std::list<SpellScript::HitHandler>::iterator hookItrEnd = (*scritr)->BeforeHit.end(), hookItr = (*scritr)->BeforeHit.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            (*hookItr).Call(*scritr);

        (*scritr)->_FinishScriptCall();
    }
    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

void Spell::CallScriptOnHitHandlers()
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_HIT);
        std::list<SpellScript::HitHandler>::iterator hookItrEnd = (*scritr)->OnHit.end(), hookItr = (*scritr)->OnHit.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            (*hookItr).Call(*scritr);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

void Spell::CallScriptAfterHitHandlers()
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_AFTER_HIT);
        std::list<SpellScript::HitHandler>::iterator hookItrEnd = (*scritr)->AfterHit.end(), hookItr = (*scritr)->AfterHit.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            (*hookItr).Call(*scritr);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

void Spell::CallScriptObjectAreaTargetSelectHandlers(std::list<WorldObject*>& targets, SpellEffIndex effIndex)
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_OBJECT_AREA_TARGET_SELECT);
        std::list<SpellScript::ObjectAreaTargetSelectHandler>::iterator hookItrEnd = (*scritr)->OnObjectAreaTargetSelect.end(), hookItr = (*scritr)->OnObjectAreaTargetSelect.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            if ((*hookItr).IsEffectAffected(m_spellInfo, effIndex))
                (*hookItr).Call(*scritr, targets);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

void Spell::CallScriptObjectTargetSelectHandlers(WorldObject*& target, SpellEffIndex effIndex)
{
    uint32 scriptExecuteTime = getMSTime();
    for (std::list<SpellScript*>::iterator scritr = m_loadedScripts.begin(); scritr != m_loadedScripts.end(); ++scritr)
    {
        (*scritr)->_PrepareScriptCall(SPELL_SCRIPT_HOOK_OBJECT_TARGET_SELECT);
        std::list<SpellScript::ObjectTargetSelectHandler>::iterator hookItrEnd = (*scritr)->OnObjectTargetSelect.end(), hookItr = (*scritr)->OnObjectTargetSelect.begin();
        for (; hookItr != hookItrEnd; ++hookItr)
            if ((*hookItr).IsEffectAffected(m_spellInfo, effIndex))
                (*hookItr).Call(*scritr, target);

        (*scritr)->_FinishScriptCall();
    }

    scriptExecuteTime = getMSTime() - scriptExecuteTime;
    if (scriptExecuteTime > 10)
        sLog->outAshran("SpellScript [%u] take more than 10 ms to execute (%u ms)", m_spellInfo->Id, scriptExecuteTime);
}

bool Spell::CanExecuteTriggersOnHit(uint32 effMask, SpellInfo const* triggeredByAura) const
{
    bool only_on_caster = (triggeredByAura && (triggeredByAura->AttributesEx4 & SPELL_ATTR4_PROC_ONLY_ON_CASTER));
    // If triggeredByAura has SPELL_ATTR4_PROC_ONLY_ON_CASTER then it can only proc on a casted spell with TARGET_UNIT_CASTER
    for (uint8 i = 0; i < m_spellInfo->EffectCount; ++i)
    {
        if ((effMask & (1 << i)) && (!only_on_caster || (m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_CASTER)))
            return true;
    }
    return false;
}

bool Spell::CanProcOnTarget(Unit *target) const
{
    // Sudden Eclipse - PVP Druid Set Bonus
    if (m_spellInfo->Id == 95746)
        return true;

    /// Arcane Charge
    if (m_spellInfo->Id == 36032)
        return true;

    // swd for priest, for other it does bugs
    if (m_spellInfo->SpellFamilyName != SPELLFAMILY_PRIEST && !m_spellInfo->IsPositive() && m_caster->IsFriendlyTo(target))
        return false;

    // Distract can't be proced
    if (m_spellInfo->HasEffect(SPELL_EFFECT_DISTRACT))
        return false;

    return true;
}

void Spell::PrepareTriggersExecutedOnHit()
{
    // todo: move this to scripts
    if (m_spellInfo->SpellFamilyName)
    {
        SpellInfo const* excludeCasterSpellInfo = sSpellMgr->GetSpellInfo(m_spellInfo->ExcludeCasterAuraSpell);
        if (excludeCasterSpellInfo && !excludeCasterSpellInfo->IsPositive())
            m_preCastSpell = m_spellInfo->ExcludeCasterAuraSpell;
        SpellInfo const* excludeTargetSpellInfo = sSpellMgr->GetSpellInfo(m_spellInfo->ExcludeTargetAuraSpell);
        if (excludeTargetSpellInfo && !excludeTargetSpellInfo->IsPositive())
            m_preCastSpell = m_spellInfo->ExcludeTargetAuraSpell;
    }

    // todo: move this to scripts
    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_MAGE:
        {
             // Permafrost
             if (m_spellInfo->SpellFamilyFlags[1] & 0x00001000 ||  m_spellInfo->SpellFamilyFlags[0] & 0x00100220)
                 m_preCastSpell = 68391; ///< @todo SpellID removed 
             break;
        }
    }

    if (m_spellInfo->IsPassive())
        return;

    // handle SPELL_AURA_ADD_TARGET_TRIGGER auras:
    // save auras which were present on spell caster on cast, to prevent triggered auras from affecting caster
    // and to correctly calculate proc chance when combopoints are present
    Unit::AuraEffectList const& targetTriggers = m_caster->GetAuraEffectsByType(SPELL_AURA_ADD_TARGET_TRIGGER);
    for (Unit::AuraEffectList::const_iterator i = targetTriggers.begin(); i != targetTriggers.end(); ++i)
    {
        if (!(*i)->IsAffectingSpell(m_spellInfo))
            continue;
        SpellInfo const* auraSpellInfo = (*i)->GetSpellInfo();
        uint32 auraSpellIdx = (*i)->GetEffIndex();
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(auraSpellInfo->Effects[auraSpellIdx].TriggerSpell))
        {
            // calculate the chance using spell base amount, because aura amount is not updated on combo-points change
            // this possibly needs fixing
            int32 auraBaseAmount = (*i)->GetBaseAmount();
            // proc chance is stored in effect amount
            int32 chance = m_caster->CalculateSpellDamage(NULL, auraSpellInfo, auraSpellIdx, &auraBaseAmount);
            // build trigger and add to the list
            HitTriggerSpell spellTriggerInfo;
            spellTriggerInfo.triggeredSpell = spellInfo;
            spellTriggerInfo.triggeredByAura = auraSpellInfo;
            spellTriggerInfo.chance = chance * (*i)->GetBase()->GetStackAmount();
            m_hitTriggerSpells.push_back(spellTriggerInfo);
        }
    }
}

// Global cooldowns management
enum GCDLimits
{
    MIN_GCD = 1000,
    MAX_GCD = 1500
};

bool Spell::HasGlobalCooldown() const
{
    // Only player or controlled units have global cooldown
    if (m_caster->GetCharmInfo())
        return m_caster->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(m_spellInfo);
    else if (m_caster->IsPlayer())
        return m_caster->ToPlayer()->GetGlobalCooldownMgr().HasGlobalCooldown(m_spellInfo);
    else
        return false;
}

void Spell::TriggerGlobalCooldown()
{
    int32 l_Gcd = m_spellInfo->StartRecoveryTime;
    if (!l_Gcd)
        return;

    // Only players or controlled units have global cooldown
    if (m_caster->GetTypeId() != TYPEID_PLAYER && !m_caster->GetCharmInfo())
        return;

     if (m_caster->IsPlayer())
          if (m_caster->ToPlayer()->GetCommandStatus(CHEAT_COOLDOWN))
               return;

    // Global cooldown can't leave range 1..1.5 secs
    // There are some spells (mostly not casted directly by player) that have < 1 sec and > 1.5 sec global cooldowns
    // but as tests show are not affected by any spell mods.
    if (m_spellInfo->StartRecoveryTime >= MIN_GCD && m_spellInfo->StartRecoveryTime <= MAX_GCD)
    {
        // gcd modifier auras are applied only to own spells and only players have such mods
        if (m_caster->IsPlayer())
        {
            m_caster->ToPlayer()->ApplySpellMod(m_spellInfo->Id, SPELLMOD_GLOBAL_COOLDOWN, l_Gcd, this);

            if (int32 l_CooldownMod = m_caster->ToPlayer()->GetTotalAuraModifier(SPELL_AURA_MOD_GLOBAL_COOLDOWN_BY_HASTE))
            {
                float l_Haste = m_caster->ToPlayer()->GetFloatValue(UNIT_FIELD_MOD_HASTE);
                l_Gcd *= ApplyPct(l_Haste, l_CooldownMod);
            }
        }

        if (l_Gcd == 0)
            return;

        // Apply haste rating
        l_Gcd = int32(float(l_Gcd) * m_caster->GetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED));
        if (l_Gcd < MIN_GCD)
            l_Gcd = MIN_GCD;
        else if (l_Gcd > MAX_GCD)
            l_Gcd = MAX_GCD;
    }

    // Only players or controlled units have global cooldown
    if (m_caster->GetCharmInfo())
        m_caster->GetCharmInfo()->GetGlobalCooldownMgr().AddGlobalCooldown(m_spellInfo, l_Gcd);
    else if (m_caster->IsPlayer())
        m_caster->ToPlayer()->GetGlobalCooldownMgr().AddGlobalCooldown(m_spellInfo, l_Gcd);
}

void Spell::CancelGlobalCooldown()
{
    if (!m_spellInfo->StartRecoveryTime)
        return;

    // Cancel global cooldown when interrupting current cast
    if (m_caster->GetCurrentSpell(CURRENT_GENERIC_SPELL) != this)
        return;

    // Only players or controlled units have global cooldown
    if (m_caster->GetCharmInfo())
        m_caster->GetCharmInfo()->GetGlobalCooldownMgr().CancelGlobalCooldown(m_spellInfo);
    else if (m_caster->IsPlayer())
        m_caster->ToPlayer()->GetGlobalCooldownMgr().CancelGlobalCooldown(m_spellInfo);
}

bool Spell::IsCritForTarget(Unit* target) const
{
    if (!target)
        return false;

    for (auto itr : m_UniqueTargetInfo)
        if (itr.targetGUID == target->GetGUID() && itr.crit)
            return true;

    return false;
}

bool Spell::IsDarkSimulacrum() const
{
    // Dark Simulacrum
    if (AuraEffect* darkSimulacrum = m_caster->GetAuraEffect(77616, 0))
    {
        if (m_spellInfo->Id == darkSimulacrum->GetAmount())
            return true;
        else
        {
            SpellInfo const* amountSpell = sSpellMgr->GetSpellInfo(darkSimulacrum->GetAmount());
            if (!amountSpell)
                return false;

            SpellInfo const* triggerSpell = sSpellMgr->GetSpellInfo(amountSpell->Effects[0].BasePoints);
            if (!triggerSpell)
                return false;

            if (m_spellInfo->Id == triggerSpell->Id)
                return true;
        }
    }

    return false;
}

bool Spell::IsCommandDemonSpell() const
{
    switch (m_spellInfo->Id)
    {
        case 119905:
        case 119907:
        case 119909:
        case 119910:
        case 119911:
        case 119913:
        case 119914:
        case 119915:
            return true;
        default:
            return false;
    }

    return false;
}

bool Spell::LOSAdditionalRules(Unit const* target, int8 eff) const
{
    // Okay, custom rules for LoS
    for (uint8 x = (eff == -1 ? 0 : eff); x < (eff == -1 ? m_spellInfo->EffectCount : eff + 1); ++x)
    {
        // like paladin auras
        if (m_spellInfo->Effects[x].Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID)
            return true;

        // like bloodlust / prayers
        if (m_spellInfo->Effects[x].ApplyAuraName && (m_spellInfo->Effects[x].TargetB.GetTarget() == TARGET_UNIT_SRC_AREA_ALLY ||
            m_spellInfo->Effects[x].TargetA.GetTarget() == TARGET_UNIT_CASTER_AREA_RAID))
            return !IsMorePowerfulAura(target);

        if (m_spellInfo->IsChanneled())
            continue;

        switch (m_spellInfo->Effects[x].TargetA.GetTarget())
        {
            case TARGET_UNIT_PET:
            case TARGET_UNIT_MASTER:
                return true;
            default:
                break;
        }
    }

    return false;
}

bool Spell::IsMorePowerfulAura(Unit const* target) const
{
    if (m_spellInfo->GetDuration() >= 2 * MINUTE * IN_MILLISECONDS)
    {
        switch (m_spellInfo->Effects[0].ApplyAuraName)
        {
            case SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE:
            case SPELL_AURA_MOD_STAT:
            case SPELL_AURA_MOD_RANGED_ATTACK_POWER:
            {
                Unit::VisibleAuraMap const *visibleAuras = const_cast<Unit*>(target)->GetVisibleAuras();
                for (Unit::VisibleAuraMap::const_iterator itr = visibleAuras->begin(); itr != visibleAuras->end(); ++itr)
                    if (AuraEffect* auraeff = itr->second->GetBase()->GetEffect(0))
                    {
                        if (auraeff->GetBase()->GetDuration() <= 2*MINUTE*IN_MILLISECONDS)
                            continue;

                        if (auraeff->GetSpellInfo()->SpellFamilyName == SPELLFAMILY_POTION)
                            continue;

                        if (auraeff->GetAuraType() == m_spellInfo->Effects[0].ApplyAuraName &&
                            (m_spellInfo->Effects[0].ApplyAuraName == SPELL_AURA_MOD_RANGED_ATTACK_POWER || m_spellInfo->Effects[0].MiscValue == auraeff->GetMiscValue()))
                        {
                            uint32 dmg = abs(CalculateDamage(0, target));
                            uint32 amount = abs(auraeff->GetAmount());
                            if (amount < dmg)
                                continue;

                            else if (amount == dmg && m_spellInfo->GetDuration() > auraeff->GetBase()->GetDuration())
                                continue;

                            return true;
                        }
                    }
            }
            default:
                break;
        }
    }
    return false;
}

bool Spell::IsSpellTriggeredAfterCast() const
{
    switch (m_spellInfo->Id)
    {
        case 29722:  ///< Incinerate
            return true;
        default:
            return false;
    }

    return false;
}

namespace JadeCore
{

WorldObjectSpellTargetCheck::WorldObjectSpellTargetCheck(Unit* caster, Unit* referer, SpellInfo const* spellInfo,
    SpellTargetCheckTypes selectionType, ConditionContainer* condList) : _caster(caster), _referer(referer), _spellInfo(spellInfo),
    _targetSelectionType(selectionType), _condList(condList)
{
    if (condList)
        _condSrcInfo = new ConditionSourceInfo(NULL, caster);
    else
        _condSrcInfo = NULL;
}

WorldObjectSpellTargetCheck::~WorldObjectSpellTargetCheck()
{
    if (_condSrcInfo)
        delete _condSrcInfo;
}

bool WorldObjectSpellTargetCheck::operator()(WorldObject* target)
{
    if (_spellInfo->CheckTarget(_caster, target, true) != SPELL_CAST_OK)
        return false;
    Unit* unitTarget = target->ToUnit();
    if (Corpse* corpseTarget = target->ToCorpse())
    {
        // use ofter for party/assistance checks
        if (Player* owner = ObjectAccessor::FindPlayer(corpseTarget->GetOwnerGUID()))
            unitTarget = owner;
        else
            return false;
    }
    if (unitTarget)
    {
        switch (_targetSelectionType)
        {
            case TARGET_CHECK_ENEMY:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAttackTarget(unitTarget, _spellInfo))
                    return false;
                break;
            case TARGET_CHECK_ALLY:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAssistTarget(unitTarget, _spellInfo, false))
                    return false;
                break;
            case TARGET_CHECK_PARTY:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAssistTarget(unitTarget, _spellInfo))
                    return false;
                if (!_referer->IsInPartyWith(unitTarget))
                    return false;
                break;
            case TARGET_CHECK_RAID_CLASS:
                if (_referer->getClass() != unitTarget->getClass())
                    return false;
                // nobreak;
            case TARGET_CHECK_RAID:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAssistTarget(unitTarget, _spellInfo))
                    return false;
                if (!_referer->IsInRaidWith(unitTarget) && !_referer->IsInPartyWith(unitTarget))
                    return false;
                break;
            case TARGET_CHECK_ALLY_OR_RAID:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAssistTarget(unitTarget, _spellInfo) &&
                    !_referer->IsInRaidWith(unitTarget) && !_referer->IsInPartyWith(unitTarget))
                    return false;
                break;
            default:
                break;
        }
    }
    if (!_condSrcInfo)
        return true;
    _condSrcInfo->mConditionTargets[0] = target;
    return sConditionMgr->IsObjectMeetToConditions(*_condSrcInfo, *_condList);
}

WorldObjectSpellNearbyTargetCheck::WorldObjectSpellNearbyTargetCheck(float range, Unit* caster, SpellInfo const* spellInfo,
    SpellTargetCheckTypes selectionType, ConditionContainer* condList)
    : WorldObjectSpellTargetCheck(caster, caster, spellInfo, selectionType, condList), _range(range), _position(caster)
{
}

bool WorldObjectSpellNearbyTargetCheck::operator()(WorldObject* target)
{
    float dist = target->GetDistance(*_position);
    if (dist < _range && WorldObjectSpellTargetCheck::operator ()(target) && _caster->GetGUID() != target->GetGUID())
    {
        _range = dist;
        return true;
    }
    return false;
}

WorldObjectSpellAreaTargetCheck::WorldObjectSpellAreaTargetCheck(float range, Position const* position, Unit* caster,
    Unit* referer, SpellInfo const* spellInfo, SpellTargetCheckTypes selectionType, ConditionContainer* condList)
    : WorldObjectSpellTargetCheck(caster, referer, spellInfo, selectionType, condList), _range(range), _position(position)
{
}

bool WorldObjectSpellAreaTargetCheck::operator()(WorldObject* target)
{
    if (!target->IsWithinDist3d(_position, _range))
        return false;
    return WorldObjectSpellTargetCheck::operator ()(target);
}

WorldObjectSpellConeTargetCheck::WorldObjectSpellConeTargetCheck(float coneAngle, float range, Unit* caster,
    SpellInfo const* spellInfo, SpellTargetCheckTypes selectionType, ConditionContainer* condList)
    : WorldObjectSpellAreaTargetCheck(range, caster, caster, caster, spellInfo, selectionType, condList), _coneAngle(coneAngle)
{
}

bool WorldObjectSpellConeTargetCheck::operator()(WorldObject* target)
{
    if (_spellInfo->AttributesCu & SPELL_ATTR0_CU_CONE_BACK)
    {
        if (!_caster->isInBack(target, _coneAngle))
            return false;
    }
    else if (_spellInfo->AttributesCu & SPELL_ATTR0_CU_CONE_LINE)
    {
        if (!_caster->HasInLine(target, _caster->GetObjectSize()))
            return false;
    }
    else
    {
        if (!_caster->isInFront(target, _coneAngle))
        {
            if (_caster->GetTypeId() != TYPEID_PLAYER || _caster->GetDistance2d(target) > 3.0f || !_caster->isInFront(target, M_PI))
                return false;
        }
    }
    return WorldObjectSpellAreaTargetCheck::operator ()(target);
}

WorldObjectSpellTrajTargetCheck::WorldObjectSpellTrajTargetCheck(float range, Position const* position, Unit* caster, SpellInfo const* spellInfo)
    : WorldObjectSpellAreaTargetCheck(range, position, caster, caster, spellInfo, TARGET_CHECK_DEFAULT, NULL)
{
}

bool WorldObjectSpellTrajTargetCheck::operator()(WorldObject* target)
{
    // return all targets on missile trajectory (0 - size of a missile)
    if (!_caster->HasInLine(target, 0))
        return false;
    return WorldObjectSpellAreaTargetCheck::operator ()(target);
}

} //namespace JadeCore
