////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "UnitAI.h"
#include "Player.h"
#include "Creature.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "Spell.h"
#include "CreatureAIImpl.h"

void UnitAI::AttackStart(Unit* victim)
{
    if (victim && me->Attack(victim, true))
        me->GetMotionMaster()->MoveChase(victim);
}

void UnitAI::AttackStartCaster(Unit* victim, float dist)
{
    if (victim && me->Attack(victim, false))
        me->GetMotionMaster()->MoveChase(victim, dist);
}

void UnitAI::DoMeleeAttackIfReady()
{
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    Unit* victim = me->getVictim();

    if (!me->IsWithinMeleeRange(victim))
        return;

    /// Make sure our attack is ready and we aren't currently casting before checking distance
    if (me->isAttackReady())
    {
        me->AttackerStateUpdate(victim);
        me->resetAttackTimer();
    }

    if (me->haveOffhandWeapon() && me->isAttackReady(WeaponAttackType::OffAttack))
    {
        me->AttackerStateUpdate(victim, WeaponAttackType::OffAttack);
        me->resetAttackTimer(WeaponAttackType::OffAttack);
    }
}

bool UnitAI::DoSpellAttackIfReady(uint32 spell)
{
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return true;

    if (me->isAttackReady())
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell))
        {
            if (me->IsWithinCombatRange(me->getVictim(), spellInfo->GetMaxRange(false)))
            {
                me->CastSpell(me->getVictim(), spell, false);
                me->resetAttackTimer();
            }
            else
                return false;
        }
        else
            return false;
    }
    return true;
}

Unit* UnitAI::SelectTarget(SelectAggroTarget targetType, uint32 position, float dist, bool playerOnly, int32 aura)
{
    return SelectTarget(targetType, position, DefaultTargetSelector(me, dist, playerOnly, aura));
}

void UnitAI::SelectTargetList(std::list<Unit*>& targetList, uint32 num, SelectAggroTarget targetType, float dist, bool playerOnly, int32 aura)
{
    SelectTargetList(targetList, DefaultTargetSelector(me, dist, playerOnly, aura), num, targetType);
}

Player* UnitAI::SelectRangedTarget(bool p_AllowHeal /*= true*/, int32 p_CheckAura /*= 0*/) const
{
    std::list<HostileReference*> const& l_ThreatList = me->getThreatManager().getThreatList();
    if (l_ThreatList.empty())
        return nullptr;

    std::list<Player*> l_TargetList;
    for (HostileReference* l_Iter : l_ThreatList)
    {
        if (l_Iter->getTarget()->IsPlayer())
            l_TargetList.push_back(l_Iter->getTarget()->ToPlayer());
    }

    if (l_TargetList.empty())
        return nullptr;

    l_TargetList.remove_if([&](Player* p_Player) -> bool
    {
        if (!p_Player->IsRangedDamageDealer(p_AllowHeal))
            return true;

        if (p_CheckAura)
        {
            if (p_CheckAura > 0)
            {
                if (!p_Player->HasAura(p_CheckAura))
                    return true;
            }
            else
            {
                if (p_Player->HasAura(-p_CheckAura))
                    return true;
            }
        }

        return false;
    });

    if (l_TargetList.empty())
        return nullptr;

    JadeCore::Containers::RandomResizeList(l_TargetList, 1);

    return l_TargetList.front();
}

Player* UnitAI::SelectMeleeTarget(bool p_AllowTank /*= false*/) const
{
    std::list<HostileReference*> const& l_ThreatList = me->getThreatManager().getThreatList();
    if (l_ThreatList.empty())
        return nullptr;

    std::list<Player*> l_TargetList;
    for (HostileReference* l_Iter : l_ThreatList)
    {
        if (l_Iter->getTarget()->IsPlayer())
            l_TargetList.push_back(l_Iter->getTarget()->ToPlayer());
    }

    if (l_TargetList.empty())
        return nullptr;

    l_TargetList.remove_if([this, p_AllowTank](Player* p_Player) -> bool
    {
        if (!p_Player->IsMeleeDamageDealer(p_AllowTank))
            return true;

        return false;
    });

    if (l_TargetList.empty())
        return nullptr;

    JadeCore::Containers::RandomResizeList(l_TargetList, 1);

    return l_TargetList.front();
}

Player* UnitAI::SelectMainTank() const
{
    std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
    if (l_ThreatList.empty())
        return nullptr;

    l_ThreatList.remove_if([this](HostileReference* p_HostileRef) -> bool
    {
        Player* l_Player = p_HostileRef->getTarget()->ToPlayer();
        if (l_Player == nullptr)
            return true;

        if (l_Player->GetRoleForGroup() != Roles::ROLE_TANK)
            return true;

        return false;
    });

    if (l_ThreatList.empty())
        return nullptr;

    l_ThreatList.sort(JadeCore::ThreatOrderPred());

    return l_ThreatList.front()->getTarget()->ToPlayer();
}

Player* UnitAI::SelectOffTank() const
{
    std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
    if (l_ThreatList.empty())
        return nullptr;

    l_ThreatList.remove_if([this](HostileReference* p_HostileRef) -> bool
    {
        Player* l_Player = p_HostileRef->getTarget()->ToPlayer();
        if (l_Player == nullptr)
            return true;

        if (l_Player->GetRoleForGroup() != Roles::ROLE_TANK)
            return true;

        return false;
    });

    if (l_ThreatList.empty())
        return nullptr;

    l_ThreatList.sort(JadeCore::ThreatOrderPred());

    return l_ThreatList.back()->getTarget()->ToPlayer();
}

float UnitAI::DoGetSpellMaxRange(uint32 spellId, bool positive)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    return spellInfo ? spellInfo->GetMaxRange(positive) : 0;
}

void UnitAI::DoAddAuraToAllHostilePlayers(uint32 spellid)
{
    if (me->isInCombat())
    {
        std::list<HostileReference*>& threatlist = me->getThreatManager().getThreatList();
        for (std::list<HostileReference*>::iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
        {
            if (Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                if (unit->IsPlayer())
                    me->AddAura(spellid, unit);
        }
    }
}

void UnitAI::DoCastToAllHostilePlayers(uint32 spellid, bool triggered)
{
    if (me->isInCombat())
    {
        std::list<HostileReference*>& threatlist = me->getThreatManager().getThreatList();
        for (std::list<HostileReference*>::iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
        {
            if (Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                if (unit->IsPlayer())
                    me->CastSpell(unit, spellid, triggered);
        }
    }
}

void UnitAI::DoCast(uint32 spellId)
{
    Unit* target = NULL;
    //sLog->outError(LOG_FILTER_GENERAL, "aggre %u %u", spellId, (uint32)AISpellInfo[spellId].target);
    switch (AISpellInfo[spellId].target)
    {
        default:
        case AITARGET_SELF:
           target = me;
           break;
        case AITARGET_VICTIM:
            target = me->getVictim();
            break;
        case AITARGET_ENEMY:
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
            {
                bool playerOnly = spellInfo->AttributesEx3 & SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                target = SelectTarget(SELECT_TARGET_RANDOM, 0, spellInfo->GetMaxRange(false), playerOnly);
            }
            break;
        }
        case AITARGET_ALLY:
            target = me;
            break;
        case AITARGET_BUFF:
            target = me;
                break;
        case AITARGET_DEBUFF:
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
            {
                bool playerOnly = spellInfo->AttributesEx3 & SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                float range = spellInfo->GetMaxRange(false);

                DefaultTargetSelector targetSelector(me, range, playerOnly, -(int32)spellId);
                if (!(spellInfo->AuraInterruptFlags & AURA_INTERRUPT_FLAG_NOT_VICTIM)
                    && targetSelector(me->getVictim()))
                    target = me->getVictim();
                else
                    target = SelectTarget(SELECT_TARGET_RANDOM, 0, targetSelector);
            }
            break;
        }
    }

    if (target)
        me->CastSpell(target, spellId, false);
}

#define UPDATE_TARGET(a) {if (AIInfo->target<a) AIInfo->target=a;}

void UnitAI::FillAISpellInfo()
{
    AISpellInfo = new AISpellInfoType[sSpellMgr->GetSpellInfoStoreSize()];

    AISpellInfoType* AIInfo = AISpellInfo;
    const SpellInfo* spellInfo;

    for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); ++i, ++AIInfo)
    {
        spellInfo = sSpellMgr->GetSpellInfo(i);
        if (!spellInfo)
            continue;

        if (spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD)
            AIInfo->condition = AICOND_DIE;
        else if (spellInfo->IsPassive() || spellInfo->GetDuration() == -1)
            AIInfo->condition = AICOND_AGGRO;
        else
            AIInfo->condition = AICOND_COMBAT;

        if (AIInfo->cooldown < spellInfo->RecoveryTime)
            AIInfo->cooldown = spellInfo->RecoveryTime;

        if (!spellInfo->GetMaxRange(false))
            UPDATE_TARGET(AITARGET_SELF)
        else
        {
            for (uint8 j = 0; j < spellInfo->EffectCount; ++j)
            {
                uint32 targetType = spellInfo->Effects[j].TargetA.GetTarget();

                if (targetType == TARGET_UNIT_TARGET_ENEMY
                    || targetType == TARGET_DEST_TARGET_ENEMY)
                    UPDATE_TARGET(AITARGET_VICTIM)
                else if (targetType == TARGET_UNIT_DEST_AREA_ENEMY)
                    UPDATE_TARGET(AITARGET_ENEMY)

                if (spellInfo->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA ||
                    spellInfo->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET)
                {
                    if (targetType == TARGET_UNIT_TARGET_ENEMY)
                        UPDATE_TARGET(AITARGET_DEBUFF)
                    else if (spellInfo->IsPositive())
                        UPDATE_TARGET(AITARGET_BUFF)
                }
            }
        }
        AIInfo->realCooldown = spellInfo->RecoveryTime + spellInfo->StartRecoveryTime;
        AIInfo->maxRange = spellInfo->GetMaxRange(false) * 3 / 4;
    }
}

//Enable PlayerAI when charmed
void PlayerAI::OnCharmed(bool apply) { me->IsAIEnabled = apply; }

void SimpleCharmedAI::UpdateAI(const uint32 /*diff*/)
{
  Creature* charmer = me->GetCharmer()->ToCreature();

    //kill self if charm aura has infinite duration
    if (charmer->IsInEvadeMode())
    {
        Unit::AuraEffectList const& auras = me->GetAuraEffectsByType(SPELL_AURA_MOD_CHARM);
        for (Unit::AuraEffectList::const_iterator iter = auras.begin(); iter != auras.end(); ++iter)
            if ((*iter)->GetCasterGUID() == charmer->GetGUID() && (*iter)->GetBase()->IsPermanent())
            {
                charmer->Kill(me);
                return;
            }
    }

    if (!charmer->isInCombat())
        me->GetMotionMaster()->MoveFollow(charmer, PET_FOLLOW_DIST, me->GetFollowAngle());

    Unit* target = me->getVictim();
    if (!target || !charmer->IsValidAttackTarget(target))
        AttackStart(charmer->SelectNearestTargetInAttackDistance());
}

SpellTargetSelector::SpellTargetSelector(Unit* caster, uint32 spellId) :
    _caster(caster), _spellInfo(sSpellMgr->GetSpellForDifficultyFromSpell(sSpellMgr->GetSpellInfo(spellId), caster))
{
    ASSERT(_spellInfo);
}

bool SpellTargetSelector::operator()(Unit const* target) const
{
    if (!target)
        return false;

    if (_spellInfo->CheckTarget(_caster, target) != SPELL_CAST_OK)
        return false;

    // copypasta from Spell::CheckRange
    uint32 range_type = _spellInfo->RangeEntry ? _spellInfo->RangeEntry->type : 0;
    float max_range = _caster->GetSpellMaxRangeForTarget(target, _spellInfo);
    float min_range = _caster->GetSpellMinRangeForTarget(target, _spellInfo);


    if (target && target != _caster)
    {
        if (range_type == SPELL_RANGE_MELEE)
        {
            // Because of lag, we can not check too strictly here.
            if (!_caster->IsWithinMeleeRange(target, max_range))
                return false;
        }
        else if (!_caster->IsWithinCombatRange(target, max_range))
            return false;

        if (range_type == SPELL_RANGE_RANGED)
        {
            if (_caster->IsWithinMeleeRange(target))
                return false;
        }
        else if (min_range && _caster->IsWithinCombatRange(target, min_range)) // skip this check if min_range = 0
            return false;
    }

    return true;
}

bool NonTankTargetSelector::operator()(Unit const* target) const
{
    if (!target)
        return false;

    if (_playerOnly && target->GetTypeId() != TYPEID_PLAYER)
        return false;

    return target != _source->getVictim();
}
