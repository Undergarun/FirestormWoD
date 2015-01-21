/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

/*
 * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mage_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "ScriptedCreature.h"

#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

enum MageSpells
{
    SPELL_MAGE_COLD_SNAP                         = 11958,
    SPELL_MAGE_ALTER_TIME_OVERRIDED              = 127140,
    SPELL_MAGE_ALTER_TIME                        = 110909,
    NPC_PAST_SELF                                = 58542,
    SPELL_MAGE_TEMPORAL_DISPLACEMENT             = 80354,
    HUNTER_SPELL_INSANITY                        = 95809,
    SPELL_SHAMAN_SATED                           = 57724,
    SPELL_SHAMAN_EXHAUSTED                       = 57723,
    SPELL_MAGE_CONJURE_REFRESHMENT_R1            = 92739,
    SPELL_MAGE_CONJURE_REFRESHMENT_R2            = 92799,
    SPELL_MAGE_CONJURE_REFRESHMENT_R3            = 92802,
    SPELL_MAGE_CONJURE_REFRESHMENT_R4            = 92805,
    SPELL_MAGE_CONJURE_REFRESHMENT_R5            = 74625,
    SPELL_MAGE_CONJURE_REFRESHMENT_R6            = 42956,
    SPELL_MAGE_CONJURE_REFRESHMENT_R7            = 92727,
    SPELL_MAGE_CONJURE_REFRESHMENT_R8            = 116130,
    SPELL_MAGE_MANA_GEM_ENERGIZE                 = 10052,
    SPELL_MAGE_INFERNO_BLAST                     = 108853,
    SPELL_MAGE_INFERNO_BLAST_IMPACT              = 118280,
    SPELL_MAGE_IGNITE                            = 12654,
    SPELL_MAGE_PYROBLAST                         = 11366,
    SPELL_MAGE_COMBUSTION_DOT                    = 83853,
    SPELL_MAGE_FROSTJAW                          = 102051,
    SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE      = 114954,
    SPELL_MAGE_NETHER_TEMPEST_MISSILE            = 114956,
    SPELL_MAGE_LIVING_BOMB_TRIGGERED             = 44461,
    SPELL_MAGE_FROST_BOMB_TRIGGERED              = 113092,
    SPELL_MAGE_BRAIN_FREEZE                      = 44549,
    SPELL_MAGE_BRAIN_FREEZE_TRIGGERED            = 57761,
    SPELL_MAGE_SLOW                              = 31589,
    SPELL_MAGE_ARCANE_CHARGE                     = 36032,
    SPELL_MAGE_ARCANE_BARRAGE_TRIGGERED          = 50273,
    SPELL_MAGE_CAUTERIZE                         = 87023,
    SPELL_MAGE_ARCANE_MISSILES                   = 79683,
    SPELL_MAGE_INCANTERS_ABSORBTION              = 116267,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK                = 115723,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK_IMMUNITY       = 115760,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK_FROST_NOVA     = 115757,
    SPELL_MAGE_FINGER_OF_FROST_VISUAL            = 44544,
    SPELL_MAGE_FINGER_OF_FROST_EFFECT            = 126084,
    SPELL_MAGE_GLYPH_OF_SLOW                     = 86209,
    SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE  = 113862,
    SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER = 122293,
    SPELL_MAGE_ICE_BLOCK                         = 45438,
    SPELL_MAGE_CONE_OF_COLD                      = 120,
    SPELL_MAGE_FROST_NOVA                        = 122,
    SPELL_MAGE_FINGERS_OF_FROST_AURA             = 112965,
    SPELL_MAGE_MIRROR_IMAGE_LEFT                 = 58834,
    SPELL_MAGE_MIRROR_IMAGE_RIGHT                = 58833,
    SPELL_MAGE_MIRROR_IMAGE_FRONT                = 58831,
    SPELL_MAGE_ARCANE_BLAST                      = 30451,
    SPELL_MAGE_FIREBALL                          = 133,
    SPELL_MAGE_FROSTBOLT                         = 116,
    SPELL_MAE_FROSTFIRE_BOLT                     = 44614,
    SPELL_MAGE_UNSTABLE_MAGIC                    = 157976,
    SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FIRE        = 157977,
    SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FROST       = 157978,
    SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_ARCANE      = 157979,
    SPELL_MAGE_ARCANE_POWER                      = 12042,
    SPELL_MAGE_OVERPOWERED                       = 155147,
    SPELL_MAGE_ICY_VEINS                         = 12472,
    SPELL_MAGE_THERMAL_VOID                      = 155149,
    SPELL_MAGE_HEATING_UP                        = 48108,
    SPELL_MAGE_KINDKING                          = 155148,
    SPELL_MAGE_COMBUSTION                        = 11129,
    SPELL_MAGE_FROST_BOMB_AURA                   = 112948,
    SPELL_MAGE_FROST_BOMB_VISUAL                 = 69846,
    SPELL_MAGE_RING_OF_FROST_AURA                = 82691,
    SPELL_MAGE_IMPROVED_SCORCH                   = 157632,
    SPELL_MAGE_IMPROVED_SCORCH_AURA              = 157633,
    SPELL_MAGE_ENHANCED_PYROTECHNICS_PROC        = 157644
};


// Greater Invisibility (remove timer) - 122293
class spell_mage_greater_invisibility_removed: public SpellScriptLoader
{
    public:
        spell_mage_greater_invisibility_removed() : SpellScriptLoader("spell_mage_greater_invisibility_removed") { }

        class spell_mage_greater_invisibility_removed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_greater_invisibility_removed_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasAura(SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE))
                        _player->RemoveAurasDueToSpell(SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_greater_invisibility_removed_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_greater_invisibility_removed_AuraScript();
        }
};

// Greater Invisibility (triggered) - 110960
class spell_mage_greater_invisibility_triggered: public SpellScriptLoader
{
    public:
        spell_mage_greater_invisibility_triggered() : SpellScriptLoader("spell_mage_greater_invisibility_triggered") { }

        class spell_mage_greater_invisibility_triggered_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_greater_invisibility_triggered_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->CastSpell(_player, SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE, true);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->CastSpell(_player, SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_greater_invisibility_triggered_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_greater_invisibility_triggered_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_greater_invisibility_triggered_AuraScript();
        }
};

// Called by Arcane Blast - 30451
// Glyph of Slow - 86209
class spell_mage_glyph_of_slow: public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_slow() : SpellScriptLoader("spell_mage_glyph_of_slow") { }

        class spell_mage_glyph_of_slow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_glyph_of_slow_SpellScript);

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->HasAura(SPELL_MAGE_GLYPH_OF_SLOW))
                        return;

                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> targetList;
                        float radius = 50.0f;
                        bool found = false;

                        JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(caster, caster, radius);
                        JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(caster, targetList, u_check);
                        caster->VisitNearbyObject(radius, searcher);

                        for (auto itr : targetList)
                            if (itr->HasAura(SPELL_MAGE_SLOW))
                                found = true;

                        if (found)
                            return;
                        else
                            caster->CastSpell(target, SPELL_MAGE_SLOW, true);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_slow_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_glyph_of_slow_SpellScript();
        }
};

// Frost Nova (Water Elemental) - 33395
class spell_mage_pet_frost_nova: public SpellScriptLoader
{
    public:
        spell_mage_pet_frost_nova() : SpellScriptLoader("spell_mage_pet_frost_nova") { }

        class spell_mage_pet_frost_nova_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_pet_frost_nova_SpellScript);

            bool Load()
            {
                bool result = true;

                if (!GetCaster())
                    return false;

                result &= GetCaster()->GetTypeId() == TYPEID_UNIT;

                if (!GetCaster()->GetOwner())
                    return false;

                result &= GetCaster()->GetOwner()->GetTypeId() == TYPEID_PLAYER;
                result &= GetCaster()->GetOwner()->getLevel() >= 24;

                return result;
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetOwner())
                    {
                        if (Player* _player = caster->GetOwner()->ToPlayer())
                        {
                            if (_player->GetSpecializationId(_player->GetActiveSpec()) != SPEC_MAGE_FROST)
                                return;

                            _player->CastSpell(_player, SPELL_MAGE_FINGER_OF_FROST_VISUAL, true);
                            _player->CastSpell(_player, SPELL_MAGE_FINGER_OF_FROST_EFFECT, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_pet_frost_nova_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_pet_frost_nova_SpellScript();
        }
};

// Called by Ice Block - 45438
// Glyph of Ice Block - 115723
class spell_mage_glyph_of_ice_block: public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_ice_block() : SpellScriptLoader("spell_mage_glyph_of_ice_block") { }

        class spell_mage_glyph_of_ice_block_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_glyph_of_ice_block_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetTarget()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_MAGE_GLYPH_OF_ICE_BLOCK))
                    {
                        _player->CastSpell(_player, SPELL_MAGE_GLYPH_OF_ICE_BLOCK_FROST_NOVA, true);
                        _player->CastSpell(_player, SPELL_MAGE_GLYPH_OF_ICE_BLOCK_IMMUNITY, true);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_ice_block_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_glyph_of_ice_block_AuraScript();
        }
};

// Arcane Missiles - 5143
class spell_mage_arcane_missile: public SpellScriptLoader
{
    public:
        spell_mage_arcane_missile() : SpellScriptLoader("spell_mage_arcane_missile") { }

        class spell_mage_arcane_missile_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_arcane_missile_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_ARCANE_CHARGE, true);

                if (Player* _player = GetCaster()->ToPlayer())
                    if (AuraPtr arcaneMissiles = _player->GetAura(SPELL_MAGE_ARCANE_MISSILES))
                        arcaneMissiles->DropCharge();
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_arcane_missile_AuraScript::OnApply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }

        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_arcane_missile_AuraScript();
        }

        class spell_mage_arcane_missile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_arcane_missile_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    if (l_Caster->HasAura(SPELL_MAGE_OVERPOWERED))
                        if (AuraPtr l_Aura = l_Caster->GetAura(SPELL_MAGE_ARCANE_POWER, l_Caster->GetGUID()))
                            l_Aura->SetDuration(l_Aura->GetMaxDuration() + sSpellMgr->GetSpellInfo(SPELL_MAGE_OVERPOWERED)->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_arcane_missile_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_arcane_missile_SpellScript();
        }
};

// Cauterize - 86949
class spell_mage_cauterize: public SpellScriptLoader
{
    public:
        spell_mage_cauterize() : SpellScriptLoader("spell_mage_cauterize") { }

        class spell_mage_cauterize_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_cauterize_AuraScript);

            uint32 absorbChance;
            uint32 healtPct;

            bool Load()
            {
                absorbChance = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                healtPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
                return GetUnitOwner()->ToPlayer();
            }

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                if (target->ToPlayer()->HasSpellCooldown(SPELL_MAGE_CAUTERIZE))
                    return;

                if (!roll_chance_i(absorbChance))
                    return;

                int bp1 = target->CountPctFromMaxHealth(healtPct);
                target->CastCustomSpell(target, SPELL_MAGE_CAUTERIZE, NULL, &bp1, NULL, true);
                target->ToPlayer()->AddSpellCooldown(SPELL_MAGE_CAUTERIZE, 0, 60 * IN_MILLISECONDS);

                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_cauterize_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_cauterize_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_cauterize_AuraScript();
        }
};

class CheckArcaneBarrageImpactPredicate
{
    public:
        CheckArcaneBarrageImpactPredicate(Unit* caster, Unit* mainTarget) : _caster(caster), _mainTarget(mainTarget) {}

        bool operator()(Unit* target)
        {
            if (!_caster || !_mainTarget)
                return true;

            if (!_caster->IsValidAttackTarget(target))
                return true;

            if (!target->IsWithinLOSInMap(_caster))
                return true;

            if (!_caster->isInFront(target))
                return true;

            if (target->GetGUID() == _caster->GetGUID())
                return true;

            if (target->GetGUID() == _mainTarget->GetGUID())
                return true;

            return false;
        }

    private:
        Unit* _caster;
        Unit* _mainTarget;
};

// Arcane Barrage - 44425
class spell_mage_arcane_barrage: public SpellScriptLoader
{
    public:
        spell_mage_arcane_barrage() : SpellScriptLoader("spell_mage_arcane_barrage") { }

        class spell_mage_arcane_barrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_arcane_barrage_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (Unit* l_MainTarget = ObjectAccessor::FindUnit(l_Player->GetSelection()))
                            if (l_Target != l_MainTarget)
                                SetHitDamage(CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_1].BasePoints));

                        if (AuraPtr arcaneCharge = l_Player->GetAura(SPELL_MAGE_ARCANE_CHARGE))
                            arcaneCharge->DropCharge();
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_arcane_barrage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_arcane_barrage_SpellScript();
        }
};

// Arcane Explosion - 1449
class spell_mage_arcane_explosion: public SpellScriptLoader
{
    public:
        spell_mage_arcane_explosion() : SpellScriptLoader("spell_mage_arcane_explosion") { }

        class spell_mage_arcane_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_arcane_explosion_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_MAGE_ARCANE)
                            if (AuraPtr arcaneCharge = _player->GetAura(SPELL_MAGE_ARCANE_CHARGE))
                                arcaneCharge->RefreshDuration();
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_arcane_explosion_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_arcane_explosion_SpellScript();
        }
};

// Frostbolt - 116
class spell_mage_frostbolt: public SpellScriptLoader
{
    public:
        spell_mage_frostbolt() : SpellScriptLoader("spell_mage_frostbolt") { }

        class spell_mage_frostbolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_frostbolt_SpellScript);

            SpellCastResult CheckTarget()
            {
                if (!GetExplTargetUnit())
                    return SPELL_FAILED_NO_VALID_TARGETS;
                else if (GetExplTargetUnit()->GetGUID() == GetCaster()->GetGUID())
                    return SPELL_FAILED_BAD_TARGETS;
                else if (GetExplTargetUnit()->GetTypeId() == TYPEID_PLAYER && !GetExplTargetUnit()->IsPvP())
                {
                    if (GetCaster()->ToPlayer() && GetCaster()->ToPlayer()->m_Duel)
                    if (GetCaster()->ToPlayer()->m_Duel->opponent->GetGUID() == GetExplTargetUnit()->GetGUID())
                            return SPELL_CAST_OK;

                    return SPELL_FAILED_BAD_TARGETS;
                }
                else if (GetExplTargetUnit()->GetOwner() != GetCaster())
                {
                    if (GetCaster()->IsValidAttackTarget(GetExplTargetUnit()))
                        return SPELL_CAST_OK;

                    return SPELL_FAILED_BAD_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    if (l_Caster->HasAura(SPELL_MAGE_BRAIN_FREEZE) && roll_chance_i(sSpellMgr->GetSpellInfo(SPELL_MAGE_BRAIN_FREEZE)->Effects[EFFECT_0].BasePoints))
                        GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_BRAIN_FREEZE_TRIGGERED, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_frostbolt_SpellScript::HandleOnHit);
                OnCheckCast += SpellCheckCastFn(spell_mage_frostbolt_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_frostbolt_SpellScript();
        }
};

class CheckNetherImpactPredicate
{
    public:
        CheckNetherImpactPredicate(Unit* caster, Unit* mainTarget) : _caster(caster), _mainTarget(mainTarget) {}

        bool operator()(Unit* target)
        {
            if (!_caster || !_mainTarget)
                return true;

            if (!_caster->IsValidAttackTarget(target))
                return true;

            if (!target->IsWithinLOSInMap(_caster))
                return true;

            if (!_caster->isInFront(target))
                return true;

            if (target->GetGUID() == _caster->GetGUID())
                return true;

            if (target->GetGUID() == _mainTarget->GetGUID())
                return true;

            return false;
        }

    private:
        Unit* _caster;
        Unit* _mainTarget;
};

// Nether Tempest - 114923
class spell_mage_nether_tempest: public SpellScriptLoader
{
    public:
        spell_mage_nether_tempest() : SpellScriptLoader("spell_mage_nether_tempest") { }

        class spell_mage_nether_tempest_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_nether_tempest_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (GetCaster())
                {
                    if (Player* _player = GetCaster()->ToPlayer())
                    {
                        std::list<Unit*> targetList;

                        GetTarget()->GetAttackableUnitListInRange(targetList, 10.0f);
                        targetList.remove_if(CheckNetherImpactPredicate(_player, GetTarget()));

                        JadeCore::Containers::RandomResizeList(targetList, 1);

                        for (auto itr : targetList)
                        {
                            GetCaster()->CastSpell(itr, SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE, true);
                            GetTarget()->CastSpell(itr, SPELL_MAGE_NETHER_TEMPEST_MISSILE, true);
                        }

                        if (GetCaster()->HasAura(SPELL_MAGE_BRAIN_FREEZE))
                            if (roll_chance_i(10))
                                GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_BRAIN_FREEZE_TRIGGERED, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_nether_tempest_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_nether_tempest_AuraScript();
        }
};

// Blazing Speed - 108843
class spell_mage_blazing_speed: public SpellScriptLoader
{
    public:
        spell_mage_blazing_speed() : SpellScriptLoader("spell_mage_blazing_speed") { }

        class spell_mage_blazing_speed_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_blazing_speed_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->RemoveMovementImpairingAuras();
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_blazing_speed_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_blazing_speed_SpellScript();
        }
};

// Frostjaw - 102051
class spell_mage_frostjaw: public SpellScriptLoader
{
    public:
        spell_mage_frostjaw() : SpellScriptLoader("spell_mage_frostjaw") { }

        class spell_mage_frostjaw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_frostjaw_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetTypeId() == TYPEID_PLAYER)
                        {
                            if (AuraPtr frostjaw = target->GetAura(SPELL_MAGE_FROSTJAW, _player->GetGUID()))
                            {
                                // Only half time against players
                                frostjaw->SetDuration(frostjaw->GetMaxDuration() / 2);
                                frostjaw->SetMaxDuration(frostjaw->GetDuration());
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_frostjaw_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_frostjaw_SpellScript();
        }
};

// Combustion - 11129
class spell_mage_combustion: public SpellScriptLoader
{
    public:
        spell_mage_combustion() : SpellScriptLoader("spell_mage_combustion") { }

        class spell_mage_combustion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_combustion_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (l_Player->HasSpellCooldown(SPELL_MAGE_INFERNO_BLAST_IMPACT))
                            l_Player->RemoveSpellCooldown(SPELL_MAGE_INFERNO_BLAST_IMPACT, true);
                        //l_Player->SendClearSpellCharges(SPELL_MAGE_INFERNO_BLAST);

                        int32 combustionBp = 0;

                        Unit::AuraEffectList const& aurasPereodic = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
                        for (Unit::AuraEffectList::const_iterator i = aurasPereodic.begin(); i !=  aurasPereodic.end(); ++i)
                        {
                            if ((*i)->GetCasterGUID() != l_Player->GetGUID() || (*i)->GetSpellInfo()->SchoolMask != SPELL_SCHOOL_MASK_FIRE)
                                continue;

                            if (!(*i)->GetAmplitude())
                                continue;

                            combustionBp += l_Player->SpellDamageBonusDone(target, (*i)->GetSpellInfo(), (*i)->GetAmount(), (*i)->GetEffIndex(), DOT) * 1000 / (*i)->GetAmplitude();
                        }

                        if (combustionBp)
                            l_Player->CastCustomSpell(target, SPELL_MAGE_COMBUSTION_DOT, &combustionBp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_combustion_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_combustion_SpellScript();
        }
};

class CheckInfernoBlastImpactPredicate
{
    public:
        CheckInfernoBlastImpactPredicate(Unit* caster, Unit* mainTarget) : _caster(caster), _mainTarget(mainTarget) {}

        bool operator()(Unit* target)
        {
            if (!_caster || !_mainTarget)
                return true;

            if (!_caster->IsValidAttackTarget(target))
                return true;

            if (!target->IsWithinLOSInMap(_caster))
                return true;

            if (!_caster->isInFront(target))
                return true;

            if (target->GetGUID() == _caster->GetGUID())
                return true;

            if (target->GetGUID() == _mainTarget->GetGUID())
                return true;

            return false;
        }

    private:
        Unit* _caster;
        Unit* _mainTarget;
};

// Inferno Blast - 108853
class spell_mage_inferno_blast: public SpellScriptLoader
{
    public:
        spell_mage_inferno_blast() : SpellScriptLoader("spell_mage_inferno_blast") { }

        class spell_mage_inferno_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_inferno_blast_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> targetList;
                        int32 combustionBp = 0;

                        _player->CastSpell(target, SPELL_MAGE_INFERNO_BLAST_IMPACT, true);

                        // Spreads any Pyroblast, Ignite, and Combustion effects to up to 2 nearby enemy targets within 10 yards

                        target->GetAttackableUnitListInRange(targetList, 10.0f);

                        targetList.remove_if(CheckInfernoBlastImpactPredicate(_player, target));

                        if (targetList.size() > (uint32)GetSpellInfo()->Effects[EFFECT_1].BasePoints)
                            JadeCore::Containers::RandomResizeList(targetList, GetSpellInfo()->Effects[EFFECT_1].BasePoints);

                        for (auto itr : targetList)
                        {
                            // 1 : Ignite
                            if (target->HasAura(SPELL_MAGE_IGNITE, _player->GetGUID()))
                            {
                                float value = _player->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.5f;

                                int32 igniteBp = 0;

                                const SpellInfo *l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_IGNITE);

                                if (l_SpellInfo != nullptr)
                                {
                                    if (itr->HasAura(SPELL_MAGE_IGNITE, _player->GetGUID()))
                                        igniteBp += itr->GetRemainingPeriodicAmount(_player->GetGUID(), SPELL_MAGE_IGNITE, SPELL_AURA_PERIODIC_DAMAGE);

                                    igniteBp += int32((CalculatePct(GetHitDamage(), value)) / (l_SpellInfo->GetMaxDuration() / l_SpellInfo->Effects[EFFECT_0].Amplitude));

                                    _player->CastCustomSpell(itr, SPELL_MAGE_IGNITE, &igniteBp, NULL, NULL, true);
                                }
                            }

                            // 2 : Pyroblast
                            if (target->HasAura(SPELL_MAGE_PYROBLAST, _player->GetGUID()))
                                _player->AddAura(SPELL_MAGE_PYROBLAST, itr);

                            // 3 : Combustion
                            if (target->HasAura(SPELL_MAGE_COMBUSTION_DOT, _player->GetGUID()))
                            {
                                if (itr->HasAura(SPELL_MAGE_PYROBLAST, _player->GetGUID()))
                                {
                                    combustionBp += _player->CalculateSpellDamage(target, sSpellMgr->GetSpellInfo(SPELL_MAGE_PYROBLAST), 1);
                                    combustionBp = _player->SpellDamageBonusDone(target, sSpellMgr->GetSpellInfo(SPELL_MAGE_PYROBLAST), combustionBp, EFFECT_0, DOT);
                                }
                                if (itr->HasAura(SPELL_MAGE_IGNITE, _player->GetGUID()))
                                    combustionBp += itr->GetRemainingPeriodicAmount(_player->GetGUID(), SPELL_MAGE_IGNITE, SPELL_AURA_PERIODIC_DAMAGE);

                                if (combustionBp)
                                    _player->CastCustomSpell(itr, SPELL_MAGE_COMBUSTION_DOT, &combustionBp, NULL, NULL, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_inferno_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_inferno_blast_SpellScript();
        }
};

// Replenish Mana - 5405
class spell_mage_replenish_mana: public SpellScriptLoader
{
    public:
        spell_mage_replenish_mana() : SpellScriptLoader("spell_mage_replenish_mana") { }

        class spell_mage_replenish_mana_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_replenish_mana_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, 10052, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_replenish_mana_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_replenish_mana_SpellScript();
        }
};

// Evocation - 12051
class spell_mage_evocation: public SpellScriptLoader
{
    public:
        spell_mage_evocation() : SpellScriptLoader("spell_mage_evocation") { }

        class spell_mage_evocation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_evocation_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->EnergizeBySpell(_player, GetSpellInfo()->Id, int32(_player->GetMaxPower(POWER_MANA) * 0.15), POWER_MANA);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_evocation_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_evocation_SpellScript();
        }
};

// Conjure Refreshment - 42955
class spell_mage_conjure_refreshment: public SpellScriptLoader
{
    public:
        spell_mage_conjure_refreshment() : SpellScriptLoader("spell_mage_conjure_refreshment") { }

        class spell_mage_conjure_refreshment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_conjure_refreshment_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->getLevel() < 44)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R1, true);
                    else if (_player->getLevel() < 54)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R2, true);
                    else if (_player->getLevel() < 64)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R3, true);
                    else if (_player->getLevel() < 74)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R4, true);
                    else if (_player->getLevel() < 80)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R5, true);
                    else if (_player->getLevel() < 85)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R6, true);
                    else if (_player->getLevel() < 90)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R7, true);
                    else
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R8, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_conjure_refreshment_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_conjure_refreshment_SpellScript();
        }
};

// Time Warp - 80353
class spell_mage_time_warp: public SpellScriptLoader
{
    public:
        spell_mage_time_warp() : SpellScriptLoader("spell_mage_time_warp") { }

        class spell_mage_time_warp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_time_warp_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
                targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, SPELL_MAGE_TEMPORAL_DISPLACEMENT, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_mage_time_warp_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_time_warp_SpellScript();
        }
};

// Alter Time - 127140 (overrided)
class spell_mage_alter_time_overrided: public SpellScriptLoader
{
    public:
        spell_mage_alter_time_overrided() : SpellScriptLoader("spell_mage_alter_time_overrided") { }

        class spell_mage_alter_time_overrided_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_alter_time_overrided_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(SPELL_MAGE_ALTER_TIME))
                        _player->RemoveAura(SPELL_MAGE_ALTER_TIME);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_mage_alter_time_overrided_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_alter_time_overrided_SpellScript();
        }
};

// Alter Time - 110909
class spell_mage_alter_time: public SpellScriptLoader
{
    public:
        spell_mage_alter_time() : SpellScriptLoader("spell_mage_alter_time") { }

        class spell_mage_alter_time_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_alter_time_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_DEATH)
                        return;

                    std::list<Creature*> mirrorList;
                    _player->GetCreatureListWithEntryInGrid(mirrorList, NPC_PAST_SELF, 50.0f);

                    if (mirrorList.empty())
                        return;

                    for (std::list<Creature*>::const_iterator itr = mirrorList.begin(); itr != mirrorList.end(); ++itr)
                        if (Creature* pMirror = (*itr)->ToCreature())
                            if (TempSummon* pastSelf = pMirror->ToTempSummon())
                                if (pastSelf->isAlive() && pastSelf->IsInWorld())
                                    if (pastSelf->GetSummoner() && pastSelf->GetSummoner()->GetGUID() == _player->GetGUID())
                                        pastSelf->AI()->DoAction(1);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_alter_time_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_alter_time_AuraScript();
        }
};

// Cold Snap - 11958
class spell_mage_cold_snap: public SpellScriptLoader
{
    public:
        spell_mage_cold_snap() : SpellScriptLoader("spell_mage_cold_snap") { }

        class spell_mage_cold_snap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_cold_snap_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    // Resets cooldown of Ice Block, Frost Nova and Cone of Cold
                    player->RemoveSpellCooldown(SPELL_MAGE_ICE_BLOCK, true);
                    player->RemoveSpellCooldown(SPELL_MAGE_FROST_NOVA, true);
                    player->RemoveSpellCooldown(SPELL_MAGE_CONE_OF_COLD, true);
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Cold Snap
                OnEffectHit += SpellEffectFn(spell_mage_cold_snap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_cold_snap_SpellScript();
        }
};

class spell_mage_incanters_absorbtion_base_AuraScript : public AuraScript
{
    public:
        enum Spells
        {
            SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED = 44413,
            SPELL_MAGE_INCANTERS_ABSORBTION_R1 = 44394,
        };

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED)
                && sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_ABSORBTION_R1);
        }

        void Trigger(AuraEffectPtr aurEff, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
        {
            Unit* target = GetTarget();

            if (AuraEffectPtr talentAurEff = target->GetAuraEffectOfRankedSpell(SPELL_MAGE_INCANTERS_ABSORBTION_R1, EFFECT_0))
            {
                int32 bp = CalculatePct(absorbAmount, talentAurEff->GetAmount());
                target->CastCustomSpell(target, SPELL_MAGE_INCANTERS_ABSORBTION_TRIGGERED, &bp, NULL, NULL, true, NULL, aurEff);
            }
        }
};

// Incanter's Absorption
class spell_mage_incanters_absorbtion_absorb: public SpellScriptLoader
{
    public:
        spell_mage_incanters_absorbtion_absorb() : SpellScriptLoader("spell_mage_incanters_absorbtion_absorb") { }

        class spell_mage_incanters_absorbtion_absorb_AuraScript : public spell_mage_incanters_absorbtion_base_AuraScript
        {
            PrepareAuraScript(spell_mage_incanters_absorbtion_absorb_AuraScript);

            void Register()
            {
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_mage_incanters_absorbtion_absorb_AuraScript::Trigger, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_incanters_absorbtion_absorb_AuraScript();
        }
};

// Incanter's Absorption
class spell_mage_incanters_absorbtion_manashield: public SpellScriptLoader
{
    public:
        spell_mage_incanters_absorbtion_manashield() : SpellScriptLoader("spell_mage_incanters_absorbtion_manashield") { }

        class spell_mage_incanters_absorbtion_manashield_AuraScript : public spell_mage_incanters_absorbtion_base_AuraScript
        {
            PrepareAuraScript(spell_mage_incanters_absorbtion_manashield_AuraScript);

            void Register()
            {
                 AfterEffectManaShield += AuraEffectManaShieldFn(spell_mage_incanters_absorbtion_manashield_AuraScript::Trigger, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_incanters_absorbtion_manashield_AuraScript();
        }
};

// Living Bomb - 44457
class spell_mage_living_bomb: public SpellScriptLoader
{
    public:
        spell_mage_living_bomb() : SpellScriptLoader("spell_mage_living_bomb") { }

        class spell_mage_living_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_living_bomb_AuraScript);

            void AfterRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode != AURA_REMOVE_BY_DEATH && removeMode != AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_LIVING_BOMB_TRIGGERED, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_living_bomb_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_living_bomb_AuraScript();
        }
};

// Mirror Image - 55342
class spell_mage_mirror_image_summon: public SpellScriptLoader
{
    public:
        spell_mage_mirror_image_summon() : SpellScriptLoader("spell_mage_mirror_image_summon") { }

        class spell_mage_mirror_image_summon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_mirror_image_summon_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_LEFT, true);
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_FRONT, true);
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_RIGHT, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_mirror_image_summon_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_mirror_image_summon_SpellScript();
        }
};

// Ice Barrier - 11426
class spell_mage_ice_barrier: public SpellScriptLoader
{
    public:
        spell_mage_ice_barrier() : SpellScriptLoader("spell_mage_ice_barrier") { }

        class spell_mage_ice_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ice_barrier_AuraScript);

            void CalculateAmount(constAuraEffectPtr aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                    amount = l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST) * 4.95f;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_ice_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_ice_barrier_AuraScript();
        }
};

// Call by Arcane Blast 30451 - Fireball 133 - Frostbolt 116
// Unstable Magic - 157976
class spell_mage_unstable_magic: public SpellScriptLoader
{
public:
    spell_mage_unstable_magic() : SpellScriptLoader("spell_mage_unstable_magic") { }

    class spell_mage_unstable_magic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_unstable_magic_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
                if (l_Caster->HasAura(SPELL_MAGE_UNSTABLE_MAGIC))
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (GetSpellInfo()->Id == SPELL_MAGE_ARCANE_BLAST && roll_chance_i(sSpellMgr->GetSpellInfo(SPELL_MAGE_UNSTABLE_MAGIC)->Effects[EFFECT_0].BasePoints))
                            l_Caster->CastSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_ARCANE, true);
                        else if (GetSpellInfo()->Id == SPELL_MAGE_FROSTBOLT && roll_chance_i(sSpellMgr->GetSpellInfo(SPELL_MAGE_UNSTABLE_MAGIC)->Effects[EFFECT_1].BasePoints))
                            l_Caster->CastSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FROST, true);
                        else if (GetSpellInfo()->Id == SPELL_MAGE_FIREBALL && roll_chance_i(sSpellMgr->GetSpellInfo(SPELL_MAGE_UNSTABLE_MAGIC)->Effects[EFFECT_2].BasePoints))
                            l_Caster->CastSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FIRE, true);
                    }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_mage_unstable_magic_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_unstable_magic_SpellScript();
    }
};

// Ice Lance - 30455
class spell_mage_ice_lance: public SpellScriptLoader
{
public:
    spell_mage_ice_lance() : SpellScriptLoader("spell_mage_ice_lance") { }

    class spell_mage_ice_lance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_ice_lance_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasSpell(SPELL_MAGE_THERMAL_VOID))
                    if (AuraPtr l_Aura = l_Caster->GetAura(SPELL_MAGE_ICY_VEINS, l_Caster->GetGUID()))
                        l_Aura->SetDuration(l_Aura->GetDuration() + sSpellMgr->GetSpellInfo(SPELL_MAGE_THERMAL_VOID)->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS);

                if (Unit* l_Target = GetHitUnit())
                    if (l_Target->HasAura(SPELL_MAGE_FROST_BOMB_AURA) && l_Target->isFrozen())
                    {
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_FROST_BOMB_TRIGGERED, true);
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_FROST_BOMB_VISUAL, true);
                    }

            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_mage_ice_lance_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_ice_lance_SpellScript();
    }
};

// Pyroblast - 11366
class spell_mage_pyroblast: public SpellScriptLoader
{
public:
    spell_mage_pyroblast() : SpellScriptLoader("spell_mage_pyroblast") { }

    class spell_mage_pyroblast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_pyroblast_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (AuraPtr l_Aura = l_Caster->GetAura(SPELL_MAGE_HEATING_UP))
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), sSpellMgr->GetSpellInfo(SPELL_MAGE_HEATING_UP)->Effects[EFFECT_2].BasePoints));
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_pyroblast_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_pyroblast_SpellScript();
    }
};

// FrostFire bolt - 44614
class spell_mage_frostfire_bolt: public SpellScriptLoader
{
public:
    spell_mage_frostfire_bolt() : SpellScriptLoader("spell_mage_frostfire_bolt") { }

    class spell_mage_frostfire_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_frostfire_bolt_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (AuraPtr l_Aura = l_Caster->GetAura(SPELL_MAGE_BRAIN_FREEZE))
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), sSpellMgr->GetSpellInfo(SPELL_MAGE_BRAIN_FREEZE)->Effects[EFFECT_2].BasePoints));
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_frostfire_bolt_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_frostfire_bolt_SpellScript();
    }
};

// Call by Fireball 133 - FrostFire Bolt 44614 - Pyroblast 11366 - Inferno Blast 108853
// Kindling - 5405
class spell_mage_kindling: public SpellScriptLoader
{
public:
    spell_mage_kindling() : SpellScriptLoader("spell_mage_kindling") { }

    class spell_mage_kindling_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_kindling_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Unit = GetCaster())
                if (l_Unit->HasAura(SPELL_MAGE_KINDKING))
                    if (Player *l_Player = l_Unit->ToPlayer())
                        if (l_Player->HasSpellCooldown(SPELL_MAGE_COMBUSTION))
                            l_Player->ReduceSpellCooldown(SPELL_MAGE_COMBUSTION, sSpellMgr->GetSpellInfo(SPELL_MAGE_KINDKING)->Effects[EFFECT_0].BasePoints);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_mage_kindling_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_kindling_SpellScript();
    }
};

// Ring of Frost - 136511
class spell_mage_ring_of_frost : public SpellScriptLoader
{
public:
    spell_mage_ring_of_frost() : SpellScriptLoader("spell_mage_ring_of_frost") { }

    class spell_mage_ring_of_frost_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_ring_of_frost_AuraScript);

        void OnTick(constAuraEffectPtr aurEff)
        {
            if (Unit* l_Caster = GetCaster())
            {
                std::list<Creature*> l_TempList;
                std::list<Creature*> l_FrozenRinglist;

                // Get all of the Frozen Ring in Area
                l_Caster->GetCreatureListWithEntryInGrid(l_FrozenRinglist, 44199, 500.0f);

                l_TempList = l_FrozenRinglist;

                // Remove other players Frozen Ring
                for (std::list<Creature*>::iterator i = l_TempList.begin(); i != l_TempList.end(); ++i)
                {
                    Unit* l_Owner = (*i)->GetOwner();
                    if (l_Owner && l_Owner == l_Caster && (*i)->isSummon())
                        continue;

                    l_FrozenRinglist.remove((*i));
                }

                // Check all the Frozen Ring of player in case of more than one
                for (std::list<Creature*>::iterator itr = l_FrozenRinglist.begin(); itr != l_FrozenRinglist.end(); ++itr)
                {
                    std::list<Creature*> l_TempListCreature;
                    std::list<Player*> l_TempListPlayer;

                    // Apply aura on hostile creatures in the grid
                    (*itr)->GetCreatureListInGrid(l_TempListCreature, 6.50);
                    for (std::list<Creature*>::iterator i = l_TempListCreature.begin(); i != l_TempListCreature.end(); ++i)
                    {
                        if ((*i)->IsFriendlyTo(l_Caster) == false && (*i)->HasAura(SPELL_MAGE_RING_OF_FROST_AURA) == false)
                            l_Caster->CastSpell((*i), SPELL_MAGE_RING_OF_FROST_AURA, true);
                    }

                    // Apply aura on hostile players in the grid
                    (*itr)->GetPlayerListInGrid(l_TempListPlayer, 6.50);
                    for (std::list<Player*>::iterator i = l_TempListPlayer.begin(); i != l_TempListPlayer.end(); ++i)
                    {
                        if ((*i)->IsFriendlyTo(l_Caster) == false && (*i)->HasAura(SPELL_MAGE_RING_OF_FROST_AURA) == false)
                            l_Caster->CastSpell((*i), SPELL_MAGE_RING_OF_FROST_AURA, true);
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ring_of_frost_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_mage_ring_of_frost_AuraScript();
    }
};

// Scorch - 2948
class spell_mage_scorch: public SpellScriptLoader
{
public:
    spell_mage_scorch() : SpellScriptLoader("spell_mage_scorch") { }

    class spell_mage_scorch_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_scorch_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(SPELL_MAGE_IMPROVED_SCORCH) && l_Caster->getLevel() >= 92)
                    l_Caster->CastSpell(l_Caster, SPELL_MAGE_IMPROVED_SCORCH_AURA, true);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_mage_scorch_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_scorch_SpellScript();
    }
};

// Enhanced Pyrotechnics - 157642
class spell_mage_enhanced_pyrotechnics : public SpellScriptLoader
{
public:
    spell_mage_enhanced_pyrotechnics() : SpellScriptLoader("spell_mage_enhanced_pyrotechnics") { }

    class spell_mage_enhanced_pyrotechnics_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_enhanced_pyrotechnics_AuraScript);

        void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
        {
            PreventDefaultAction();

            Unit* l_Caster = GetCaster();
            if (!l_Caster)
                return;

            if (p_EventInfo.GetActor()->GetGUID() != l_Caster->GetGUID())
                return;

            if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                return;

            if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_MAGE_FIREBALL && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_MAE_FROSTFIRE_BOLT)
                return;

            if (!(p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                return;

            l_Caster->CastSpell(l_Caster, SPELL_MAGE_ENHANCED_PYROTECHNICS_PROC, true);
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_mage_enhanced_pyrotechnics_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_mage_enhanced_pyrotechnics_AuraScript();
    }
};


// Call by Blast Wave 157981 - Supernova 157980 - Ice Nova 157997
class spell_mage_novas_talent : public SpellScriptLoader
{
public:
    spell_mage_novas_talent() : SpellScriptLoader("spell_mage_novas_talent") { }

    class spell_mage_novas_talent_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_novas_talent_SpellScript);

        int64 m_MainTarget;

        void HandleOnCast()
        {
            m_MainTarget = 0;

            if (GetExplTargetUnit() != nullptr)
                m_MainTarget = GetExplTargetUnit()->GetGUID();
        }

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (m_MainTarget)
                        if (l_Target->GetGUID() == m_MainTarget && !l_Target->IsFriendlyTo(l_Caster))
                        {
                            SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_0].BasePoints));
                        }
                }
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_mage_novas_talent_SpellScript::HandleOnCast);
            OnEffectHitTarget += SpellEffectFn(spell_mage_novas_talent_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_novas_talent_SpellScript();
    }
};

void AddSC_mage_spell_scripts()
{
    new spell_mage_novas_talent();
    new spell_mage_enhanced_pyrotechnics();
    new spell_mage_scorch();
    new spell_mage_ring_of_frost();
    new spell_mage_kindling();
    new spell_mage_frostfire_bolt();
    new spell_mage_pyroblast();
    new spell_mage_ice_lance();
    new spell_mage_unstable_magic();
    new spell_mage_greater_invisibility_removed();
    new spell_mage_greater_invisibility_triggered();
    new spell_mage_glyph_of_slow();
    new spell_mage_pet_frost_nova();
    new spell_mage_glyph_of_ice_block();
    new spell_mage_arcane_missile();
    new spell_mage_cauterize();
    new spell_mage_arcane_barrage();
    new spell_mage_arcane_explosion();
    new spell_mage_frostbolt();
    new spell_mage_nether_tempest();
    new spell_mage_blazing_speed();
    new spell_mage_frostjaw();
    new spell_mage_combustion();
    new spell_mage_inferno_blast();
    new spell_mage_replenish_mana();
    new spell_mage_evocation();
    new spell_mage_conjure_refreshment();
    new spell_mage_time_warp();
    new spell_mage_alter_time_overrided();
    new spell_mage_alter_time();
    new spell_mage_cold_snap();
    new spell_mage_incanters_absorbtion_absorb();
    new spell_mage_incanters_absorbtion_manashield();
    new spell_mage_living_bomb();
    new spell_mage_mirror_image_summon();
    new spell_mage_ice_barrier();
}
