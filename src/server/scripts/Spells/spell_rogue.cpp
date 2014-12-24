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
 * Scripts for spells with SPELLFAMILY_ROGUE and SPELLFAMILY_GENERIC spells used by rogue players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_rog_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum RogueSpells
{
    ROGUE_SPELL_RECUPERATE                      = 73651,
    ROGUE_SPELL_DEADLY_POISON                   = 2823,
    ROGUE_SPELL_WOUND_POISON                    = 8679,
    ROGUE_SPELL_CRIPPLING_POISON                = 3408,
    ROGUE_SPELL_CRIPPLING_POISON_DEBUFF         = 3409,
    ROGUE_SPELL_LEECHING_POISON                 = 108211,
    ROGUE_SPELL_LEECHING_POISON_DEBUFF          = 112961,
    ROGUE_SPELL_DEBILITATING_POISON             = 115196,
    ROGUE_SPELL_LEECH_VITALITY                  = 116921,
    ROGUE_SPELL_DEADLY_POISON_DOT               = 2818,
    ROGUE_SPELL_DEADLY_POISON_INSTANT_DAMAGE    = 113780,
    ROGUE_SPELL_SLICE_AND_DICE                  = 5171,
    ROGUE_SPELL_SMOKE_BOMB_AREA_DUMMY           = 76577,
    ROGUE_SPELL_SMOKE_BOMB_AURA                 = 88611,
    ROGUE_SPELL_CRIMSON_TEMPEST_DOT             = 122233,
    ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA      = 115834,
    ROGUE_SPELL_VENOMOUS_VIM_ENERGIZE           = 51637,
    ROGUE_SPELL_VENOMOUS_WOUND_DAMAGE           = 79136,
    ROGUE_SPELL_GARROTE_DOT                     = 703,
    ROGUE_SPELL_RUPTURE_DOT                     = 1943,
    ROGUE_SPELL_CUT_TO_THE_CHASE_AURA           = 51667,
    ROGUE_SPELL_ADRENALINE_RUSH                 = 13750,
    ROGUE_SPELL_KILLING_SPREE                   = 51690,
    ROGUE_SPELL_SPRINT                          = 2983,
    ROGUE_SPELL_HEMORRHAGE_DOT                  = 89775,
    ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF          = 124271,
    ROGUE_SPELL_NIGHTSTALKER_AURA               = 14062,
    ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE        = 130493,
    ROGUE_SPELL_SHADOW_FOCUS_AURA               = 108209,
    ROGUE_SPELL_SHADOW_FOCUS_COST_PCT           = 112942,
    ROGUE_SPELL_NERVE_STRIKE_AURA               = 108210,
    ROGUE_SPELL_NERVE_STRIKE_REDUCE_DAMAGE_DONE = 112947,
    ROGUE_SPELL_COMBAT_READINESS                = 74001,
    ROGUE_SPELL_COMBAT_INSIGHT                  = 74002,
    ROGUE_SPELL_BLADE_FLURRY_AURA               = 13877,
    ROGUE_SPELL_BLADE_FLURRY_DAMAGE             = 22482,
    ROGUE_SPELL_CHEAT_DEATH_REDUCE_DAMAGE       = 45182,
    ROGUE_SPELL_CHEATED_DEATH_MARKER            = 45181,
    ROGUE_SPELL_DEADLY_BREW                     = 51626,
    ROGUE_SPELL_GLYPH_OF_HEMORRHAGE             = 56807,
    ROGUE_SPELL_CLOAK_AND_DAGGER                = 138106,
    ROGUE_SPELL_SHADOWSTEP_TELEPORT_ONLY        = 36563,
    ROGUE_SPELL_MARKED_FOR_DEATH                = 137619,
    ROGUE_SPELL_SHURIKEN_TOSS_CHANGE_MELEE      = 137586,
    ROGUE_SPELL_GLYPH_OF_DECOY                  = 56800,
    ROGUE_SPELL_DECOY_SUMMON                    = 89765,
    ROGUE_SPELL_KILLING_SPREE_TELEPORT          = 57840,
    ROGUE_SPELL_KILLING_SPREE_DAMAGES           = 57841,
    ROGUE_SPELL_GLYPH_OF_HEMORRHAGING_VEINS     = 146631,
    ROGUE_SPELL_GLYPH_OF_RECUPERATE             = 56806,
    ROGUE_SPELL_KIDNEY_SHOT                     = 408,
    ROGUE_SPELL_REVEALING_STRIKE                = 84617,
    ROGUE_SPELL_BURST_OF_SPEED                  = 137573,
    ROGUE_SPELL_INTERNAL_BLEEDING               = 154953,
    ROGUE_SPELL_INTERNAL_BLEEDING_AURA          = 154904,
    ROGUE_SPELL_SMOKE_BOMB                      = 88611,
    ROGUE_SPELL_RELTENTLESS_STRIKES_AURA        = 58423,
    ROGUE_SPELL_RELTENTLESS_STRIKES_PROC        = 14181,
    ROGUE_SPELL_COMBO_POINT_DELAYED             = 139569,
    ROGUE_SPELL_RUTHLESSNESS                    = 14161,
    ROGUE_SPELL_EVISCERATE_ENVENOM_BONUS_DAMAGE = 37169,
    ROGUE_SPELL_DEADLY_THROW_INTERRUPT          = 137576
};

// Killing Spree - 51690
class spell_rog_killing_spree: public SpellScriptLoader
{
    public:
        spell_rog_killing_spree() : SpellScriptLoader("spell_rog_killing_spree") { }

        class spell_rog_killing_spree_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_killing_spree_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->HasAura(ROGUE_SPELL_BLADE_FLURRY_AURA))
                    {
                        Unit* target = caster->getVictim();
                        if (!target && caster->ToPlayer())
                            target = caster->ToPlayer()->GetSelectedUnit();
                        if (!target)
                            return;

                        caster->CastSpell(target, ROGUE_SPELL_KILLING_SPREE_TELEPORT, true);
                        caster->CastSpell(target, ROGUE_SPELL_KILLING_SPREE_DAMAGES, true);
                    }
                    else
                    {
                        UnitList targets;
                        {
                            //// eff_radius == 0
                            //float radius = GetSpellInfo()->GetMaxRange(false);

                            //CellCoord p(JadeCore::ComputeCellCoord(caster->GetPositionX(), caster->GetPositionY()));
                            //Cell cell(p);

                            //JadeCore::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck u_check(caster, radius);
                            //JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck> checker(caster, targets, u_check);

                            //TypeContainerVisitor<JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck>, GridTypeMapContainer > grid_object_checker(checker);
                            //TypeContainerVisitor<JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck>, WorldTypeMapContainer > world_object_checker(checker);

                            //cell.Visit(p, grid_object_checker,  *caster->GetMap(), *caster, radius);
                            //cell.Visit(p, world_object_checker, *caster->GetMap(), *caster, radius);
                        }

                        std::vector<uint64> validTargets;

                        for (auto itr : targets)
                        {
                             if (itr->HasAuraType(SPELL_AURA_MOD_CONFUSE) ||
                                 itr->HasAuraType(SPELL_AURA_MOD_CHARM) ||
                                 itr->HasAuraType(SPELL_AURA_MOD_FEAR) ||
                                 itr->HasAuraType(SPELL_AURA_MOD_CONFUSE) ||
                                 itr->HasAuraType(SPELL_AURA_MOD_STUN))
                                continue;

                             validTargets.push_back(itr->GetGUID());
                        }

                        if (validTargets.empty())
                            return;

                        Unit* spellTarget = sObjectAccessor->FindUnit(JadeCore::Containers::SelectRandomContainerElement(validTargets));
                        if (!spellTarget)
                            return;

                        caster->CastSpell(spellTarget, ROGUE_SPELL_KILLING_SPREE_TELEPORT, true);
                        caster->CastSpell(spellTarget, ROGUE_SPELL_KILLING_SPREE_DAMAGES, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_killing_spree_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_killing_spree_AuraScript();
        }
};

// Called by Vanish - 1856
// Glyph of Decoy - 56800
class spell_rog_glyph_of_decoy: public SpellScriptLoader
{
    public:
        spell_rog_glyph_of_decoy() : SpellScriptLoader("spell_rog_glyph_of_decoy") { }

        class spell_rog_glyph_of_decoy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_glyph_of_decoy_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->HasAura(ROGUE_SPELL_GLYPH_OF_DECOY))
                        return;

                    caster->CastSpell(caster, ROGUE_SPELL_DECOY_SUMMON, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_glyph_of_decoy_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_glyph_of_decoy_SpellScript();
        }
};

// Shuriken Toss - 114014
class spell_rog_shuriken_toss: public SpellScriptLoader
{
    public:
        spell_rog_shuriken_toss() : SpellScriptLoader("spell_rog_shuriken_toss") { }

        class spell_rog_shuriken_toss_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shuriken_toss_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        if (caster->GetDistance(target) > 10.0f)
                            caster->CastSpell(caster, ROGUE_SPELL_SHURIKEN_TOSS_CHANGE_MELEE, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_shuriken_toss_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shuriken_toss_SpellScript();
        }
};

// Marked for Death - 137619
class spell_rog_marked_for_death: public SpellScriptLoader
{
    public:
        spell_rog_marked_for_death() : SpellScriptLoader("spell_rog_marked_for_death") { }

        class spell_rog_marked_for_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_marked_for_death_AuraScript);

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster())
                    return;

                Player* plr = GetCaster()->ToPlayer();
                if (!plr)
                    return;

                if (!GetTargetApplication())
                    return;

                if (!GetTargetApplication()->GetBase())
                    return;

                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode == AURA_REMOVE_BY_DEATH)
                {
                    int32 baseDuration = sSpellMgr->GetSpellInfo(ROGUE_SPELL_MARKED_FOR_DEATH)->GetMaxDuration();
                    int32 remainingDur = GetTargetApplication()->GetBase()->GetDuration();

                    if ((baseDuration - remainingDur) <= (60 * IN_MILLISECONDS))
                    {
                        if (plr->HasSpellCooldown(ROGUE_SPELL_MARKED_FOR_DEATH))
                            plr->RemoveSpellCooldown(ROGUE_SPELL_MARKED_FOR_DEATH, true);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_marked_for_death_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_marked_for_death_AuraScript();
        }
};

// Called by Ambush - 8676, Garrote - 703 and Cheap Shot - 1833
// Cloak and Dagger - 138106
class spell_rog_cloak_and_dagger: public SpellScriptLoader
{
    public:
        spell_rog_cloak_and_dagger() : SpellScriptLoader("spell_rog_cloak_and_dagger") { }

        class spell_rog_cloak_and_dagger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_cloak_and_dagger_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasUnitState(UNIT_STATE_ROOT))
                        return SPELL_FAILED_ROOTED;
                    if (Unit* l_Target = GetHitUnit())
                        if (l_Target == l_Caster)
                        return SPELL_FAILED_BAD_TARGETS;
                    return SPELL_CAST_OK;
                }
                return SPELL_FAILED_TRY_AGAIN;
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    if (Unit* l_Target = GetHitUnit())
                        if (l_Caster->HasSpell(ROGUE_SPELL_CLOAK_AND_DAGGER))
                            l_Caster->CastSpell(l_Target, 36563, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_rog_cloak_and_dagger_SpellScript::CheckCast);
                OnHit += SpellHitFn(spell_rog_cloak_and_dagger_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_cloak_and_dagger_SpellScript();
        }
};

// Cheat Death - 31230
class spell_rog_cheat_death: public SpellScriptLoader
{
    public:
        spell_rog_cheat_death() : SpellScriptLoader("spell_rog_cheat_death") { }

        class spell_rog_cheat_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_cheat_death_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (!GetTarget())
                    return;

                if (Player* target = GetTarget()->ToPlayer())
                {
                    if (dmgInfo.GetDamage() < target->GetHealth())
                        return;

                    if (target->HasAura(ROGUE_SPELL_CHEATED_DEATH_MARKER))
                        return;

                    target->CastSpell(target, ROGUE_SPELL_CHEAT_DEATH_REDUCE_DAMAGE, true);
                    target->CastSpell(target, ROGUE_SPELL_CHEATED_DEATH_MARKER, true);

                    uint32 health10 = target->CountPctFromMaxHealth(10);

                    // hp > 10% - absorb hp till 10%
                    if (target->GetHealth() > health10)
                        absorbAmount = dmgInfo.GetDamage() - target->GetHealth() + health10;
                    // hp lower than 10% - absorb everything
                    else
                        absorbAmount = dmgInfo.GetDamage();
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_cheat_death_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_rog_cheat_death_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_cheat_death_AuraScript();
        }
};

// Blade Flurry - 13877
class spell_rog_blade_flurry: public SpellScriptLoader
{
    public:
        spell_rog_blade_flurry() : SpellScriptLoader("spell_rog_blade_flurry") { }

        class spell_rog_blade_flurry_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_blade_flurry_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster() || !GetTarget())
                    return;

                if (eventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    int32 damage = eventInfo.GetDamageInfo()->GetDamage();
                    SpellInfo const* spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo();

                    if (!damage || eventInfo.GetDamageInfo()->GetDamageType() == DOT)
                        return;

                    if (spellInfo && !spellInfo->CanTriggerBladeFlurry())
                        return;

                    if (Unit* target = _player->SelectNearbyTarget(eventInfo.GetActionTarget()))
                        _player->CastCustomSpell(target, ROGUE_SPELL_BLADE_FLURRY_DAMAGE, &damage, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_blade_flurry_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_blade_flurry_AuraScript();
        }
};

// Growl - 113613
class spell_rog_growl: public SpellScriptLoader
{
    public:
        spell_rog_growl() : SpellScriptLoader("spell_rog_growl") { }

        class spell_rog_growl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_growl_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        if (caster->IsValidAttackTarget(target))
                            caster->CastSpell(target, 355, true); // Taunt
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_growl_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_growl_SpellScript();
        }
};

// Cloak of Shadows - 31224
class spell_rog_cloak_of_shadows: public SpellScriptLoader
{
    public:
        spell_rog_cloak_of_shadows() : SpellScriptLoader("spell_rog_cloak_of_shadows") { }

        class spell_rog_cloak_of_shadows_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_cloak_of_shadows_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    uint32 dispelMask = SpellInfo::GetDispelMask(DISPEL_ALL);
                    Unit::AuraApplicationMap& Auras = caster->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator iter = Auras.begin(); iter != Auras.end();)
                    {
                        // remove all harmful spells on you...
                        SpellInfo const* spell = iter->second->GetBase()->GetSpellInfo();
                        if ((spell->DmgClass == SPELL_DAMAGE_CLASS_MAGIC // only affect magic spells
                            || (spell->GetDispelMask() & dispelMask) || (spell->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC))
                            // ignore positive and passive auras
                            && !iter->second->IsPositive() && !iter->second->GetBase()->IsPassive() && GetSpellInfo()->CanDispelAura(spell))
                        {
                            caster->RemoveAura(iter);
                        }
                        else
                            ++iter;
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_cloak_of_shadows_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_cloak_of_shadows_SpellScript();
        }
};

// Combat Readiness - 74001
class spell_rog_combat_readiness: public SpellScriptLoader
{
    public:
        spell_rog_combat_readiness() : SpellScriptLoader("spell_rog_combat_readiness") { }

        class spell_rog_combat_readiness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_combat_readiness_AuraScript);

            uint32 update;
            bool hit;

            void HandleApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (GetCaster())
                {
                    update = 10000;
                    hit = false;
                }
            }

            void OnUpdate(uint32 diff)
            {
                update -= diff;

                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(ROGUE_SPELL_COMBAT_INSIGHT))
                        hit = true;

                    if (update <= 0)
                        if (!hit)
                            caster->RemoveAura(ROGUE_SPELL_COMBAT_READINESS);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_combat_readiness_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnAuraUpdate += AuraUpdateFn(spell_rog_combat_readiness_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_combat_readiness_AuraScript();
        }
};

// Called by Kidney Shot - 408 and Cheap Shot - 1833
// Nerve Strike - 108210
class spell_rog_nerve_strike: public SpellScriptLoader
{
    public:
        spell_rog_nerve_strike() : SpellScriptLoader("spell_rog_nerve_strike") { }

        class spell_rog_combat_readiness_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_combat_readiness_SpellScript);

            void HandleOnHit()
            {
                if (GetSpellInfo()->Id != ROGUE_SPELL_KIDNEY_SHOT)
                    return;

                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->HasAura(ROGUE_SPELL_REVEALING_STRIKE, caster->GetGUID()))
                        {
                            if (AuraPtr kidney = target->GetAura(ROGUE_SPELL_KIDNEY_SHOT, caster->GetGUID()))
                            {
                                int32 duration = kidney->GetMaxDuration();
                                AddPct(duration, 35);
                                kidney->SetMaxDuration(duration);
                                kidney->RefreshDuration(true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_combat_readiness_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_combat_readiness_SpellScript();
        }

        class spell_rog_combat_readiness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_combat_readiness_AuraScript);

            void HandleRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        if (caster->HasAura(ROGUE_SPELL_NERVE_STRIKE_AURA))
                            caster->CastSpell(target, ROGUE_SPELL_NERVE_STRIKE_REDUCE_DAMAGE_DONE, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_combat_readiness_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_combat_readiness_AuraScript();
        }
};

// Called by Stealth - 1784
// Nightstalker - 14062
class spell_rog_nightstalker: public SpellScriptLoader
{
    public:
        spell_rog_nightstalker() : SpellScriptLoader("spell_rog_nightstalker") { }

        class spell_rog_nightstalker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_nightstalker_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(ROGUE_SPELL_NIGHTSTALKER_AURA))
                        caster->CastSpell(caster, ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE, true);

                    if (caster->HasAura(ROGUE_SPELL_SHADOW_FOCUS_AURA))
                        caster->CastSpell(caster, ROGUE_SPELL_SHADOW_FOCUS_COST_PCT, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_nightstalker_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_nightstalker_SpellScript();
        }

        class spell_rog_nightstalker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_nightstalker_AuraScript);

            void HandleRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (GetCaster())
                {
                    if (GetCaster()->HasAura(ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE))
                        GetCaster()->RemoveAura(ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE);

                    if (GetCaster()->HasAura(ROGUE_SPELL_SHADOW_FOCUS_COST_PCT))
                        GetCaster()->RemoveAura(ROGUE_SPELL_SHADOW_FOCUS_COST_PCT);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_rog_nightstalker_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_nightstalker_AuraScript();
        }
};

// Called by Rupture - 1943, Garrote - 703, Hemorrhage (DoT) - 89775 and Crimson Tempest - 121411
// Sanguinary Vein - 79147
class spell_rog_sanguinary_vein: public SpellScriptLoader
{
    public:
        spell_rog_sanguinary_vein() : SpellScriptLoader("spell_rog_sanguinary_vein") { }

        class spell_rog_sanguinary_vein_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_sanguinary_vein_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        if (GetSpellInfo()->Id == ROGUE_SPELL_HEMORRHAGE_DOT && !caster->HasAura(ROGUE_SPELL_GLYPH_OF_HEMORRHAGING_VEINS))
                            return;

                        caster->CastSpell(target, ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, true);
                    }
                }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        if (GetSpellInfo()->Id == ROGUE_SPELL_HEMORRHAGE_DOT && !caster->HasAura(ROGUE_SPELL_GLYPH_OF_HEMORRHAGING_VEINS))
                            return;

                        if (target->HasAura(ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, caster->GetGUID()))
                            caster->CastSpell(target, ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, true);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_sanguinary_vein_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_sanguinary_vein_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_sanguinary_vein_AuraScript();
        }
};

// Hemorrhage - 16511
class spell_rog_hemorrhage: public SpellScriptLoader
{
    public:
        spell_rog_hemorrhage() : SpellScriptLoader("spell_rog_hemorrhage") { }

        class spell_rog_hemorrhage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_hemorrhage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (caster->HasAura(ROGUE_SPELL_GLYPH_OF_HEMORRHAGE))
                            if (!target->HasAuraState(AURA_STATE_BLEEDING))
                                return;

                        int32 bp = int32(GetHitDamage() / 2 / 8);

                        caster->CastCustomSpell(target, ROGUE_SPELL_HEMORRHAGE_DOT, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_hemorrhage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_hemorrhage_SpellScript();
        }
};

// Called by Envenom - 32645
class spell_rog_envenom: public SpellScriptLoader
{
public:
    spell_rog_envenom() : SpellScriptLoader("spell_rog_envenom") { }

    class spell_rog_envenom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_envenom_SpellScript);

        void HandleOnHit()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                uint8 l_ComboPoint = l_Player->GetComboPoints();
                int32 l_Damage = 0;

                if (l_ComboPoint)
                {
                    float l_Ap = l_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);

                    l_Damage += int32(1.05 * 1 * l_Ap * 0.306 * l_ComboPoint + (l_ComboPoint * GetSpellInfo()->Effects[EFFECT_0].BasePoints));

                    // Eviscerate and Envenom Bonus Damage (item set effect)
                    if (l_Player->HasAura(ROGUE_SPELL_EVISCERATE_ENVENOM_BONUS_DAMAGE))
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(ROGUE_SPELL_EVISCERATE_ENVENOM_BONUS_DAMAGE);

                        l_Damage += l_ComboPoint * l_SpellInfo->Effects[EFFECT_0].BasePoints;
                    }
                }
                SetHitDamage(l_Damage);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_envenom_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_envenom_SpellScript();
    }
};


// Called by Envenom - 32645 and Eviscerate - 2098
// Cut to the Chase - 51667
class spell_rog_cut_to_the_chase: public SpellScriptLoader
{
    public:
        spell_rog_cut_to_the_chase() : SpellScriptLoader("spell_rog_cut_to_the_chase") { }

        class spell_rog_cut_to_the_chase_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_cut_to_the_chase_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(ROGUE_SPELL_CUT_TO_THE_CHASE_AURA))
                    {
                        if (AuraPtr l_SliceAndDice = l_Caster->GetAura(ROGUE_SPELL_SLICE_AND_DICE, l_Caster->GetGUID()))
                        {
                            l_SliceAndDice->SetDuration(36 * IN_MILLISECONDS);
                            l_SliceAndDice->SetMaxDuration(36 * IN_MILLISECONDS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_cut_to_the_chase_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_cut_to_the_chase_SpellScript();
        }
};

// Called by Garrote - 703 and Rupture - 1943
// Venomous Wounds - 79134
class spell_rog_venomous_wounds: public SpellScriptLoader
{
    public:
        spell_rog_venomous_wounds() : SpellScriptLoader("spell_rog_venomous_wounds") { }

        class spell_rog_venomous_wounds_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_venomous_wounds_AuraScript);

            void HandleEffectPeriodic(constAuraEffectPtr /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        if (caster->HasAura(79134))
                        {
                            // Each time your Rupture or Garrote deals damage to an enemy that you have poisoned ...
                            if (target->HasAura(8680, caster->GetGUID())
                                || target->HasAura(2818, caster->GetGUID())
                                || target->HasAura(5760, caster->GetGUID())
                                || target->HasAura(3409, caster->GetGUID())
                                || target->HasAura(113952, caster->GetGUID())
                                || target->HasAura(112961, caster->GetGUID()))
                            {
                                if (AuraPtr rupture = target->GetAura(ROGUE_SPELL_RUPTURE_DOT, caster->GetGUID()))
                                {
                                    // ... you have a 75% chance ...
                                    if (roll_chance_i(75))
                                    {
                                        // ... to deal [ X + 16% of AP ] additional Nature damage and to regain 10 Energy
                                        caster->CastSpell(target, ROGUE_SPELL_VENOMOUS_WOUND_DAMAGE, true);
                                        int32 bp = 10;
                                        caster->CastCustomSpell(caster, ROGUE_SPELL_VENOMOUS_VIM_ENERGIZE, &bp, NULL, NULL, true);
                                    }
                                }
                                // Garrote will not trigger this effect if the enemy is also afflicted by your Rupture
                                else if (AuraPtr garrote = target->GetAura(ROGUE_SPELL_GARROTE_DOT, caster->GetGUID()))
                                {
                                    // ... you have a 75% chance ...
                                    if (roll_chance_i(75))
                                    {
                                        // ... to deal [ X + 16% of AP ] additional Nature damage and to regain 10 Energy
                                        caster->CastSpell(target, ROGUE_SPELL_VENOMOUS_WOUND_DAMAGE, true);
                                        int32 bp = 10;
                                        caster->CastCustomSpell(caster, ROGUE_SPELL_VENOMOUS_VIM_ENERGIZE, &bp, NULL, NULL, true);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_DEATH)
                    {
                        if (AuraPtr rupture = aurEff->GetBase())
                        {
                            // If an enemy dies while afflicted by your Rupture, you regain energy proportional to the remaining Rupture duration
                            int32 duration = int32(rupture->GetDuration() / 1000);
                            caster->CastCustomSpell(caster, ROGUE_SPELL_VENOMOUS_VIM_ENERGIZE, &duration, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_venomous_wounds_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_venomous_wounds_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_venomous_wounds_AuraScript();
        }
};

// Redirect - 73981 and Redirect - 110730
class spell_rog_redirect: public SpellScriptLoader
{
    public:
        spell_rog_redirect() : SpellScriptLoader("spell_rog_redirect") { }

        class spell_rog_redirect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_redirect_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_DONT_REPORT;

                    if (!caster->ToPlayer()->GetComboPoints())
                        return SPELL_FAILED_NO_COMBO_POINTS;
                }
                else
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    uint8 cp = _player->GetComboPoints();

                    if (cp > 5)
                        cp = 5;

                    _player->ClearComboPoints();
                    _player->AddComboPoints(cp, GetSpell());
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_rog_redirect_SpellScript::CheckCast);
                OnHit += SpellHitFn(spell_rog_redirect_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_redirect_SpellScript();
        }
};

enum battleGroundsFlagsSpells
{
    BG_WS_SPELL_WARSONG_FLAG    = 23333,
    BG_WS_SPELL_SILVERWING_FLAG = 23335,
    BG_KT_SPELL_ORB_PICKED_UP_1 = 121164,
    BG_KT_SPELL_ORB_PICKED_UP_2 = 121175,
    BG_KT_SPELL_ORB_PICKED_UP_3 = 121176,
    BG_KT_SPELL_ORB_PICKED_UP_4 = 121177,
    BG_KT_ALLIANCE_INSIGNIA     = 131527,
    BG_KT_HORDE_INSIGNIA        = 131528
};

// Shroud of Concealment - 115834
class spell_rog_shroud_of_concealment: public SpellScriptLoader
{
    public:
        spell_rog_shroud_of_concealment() : SpellScriptLoader("spell_rog_shroud_of_concealment") { }

        class spell_rog_shroud_of_concealment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shroud_of_concealment_SpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                std::list<WorldObject*> targetsToRemove;
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_WS_SPELL_WARSONG_FLAG));
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_WS_SPELL_SILVERWING_FLAG));
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_1));
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_2));
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_3));
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_4));
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_ALLIANCE_INSIGNIA));
                targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_HORDE_INSIGNIA));

                for (auto itr : targets)
                {
                    if (Unit* target = itr->ToUnit())
                    {
                        if ((!target->IsInRaidWith(caster) && !target->IsInPartyWith(caster)) ||
                            target->isInCombat() || target->HasUnitState(UNIT_STATE_CASTING))
                            targetsToRemove.push_back(itr);
                    }
                }

                for (auto itr : targetsToRemove)
                    targets.remove(itr);
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (AuraPtr shroudOfConcealment = target->GetAura(ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA, caster->GetGUID()))
                        {
                            if ((!target->IsInRaidWith(caster) && !target->IsInPartyWith(caster)) ||
                                target->isInCombat() || target->HasUnitState(UNIT_STATE_CASTING) ||
                                target->HasAura(BG_WS_SPELL_WARSONG_FLAG) || target->HasAura(BG_WS_SPELL_SILVERWING_FLAG) ||
                                target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_1) ||target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_2) ||
                                target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_3) ||target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_4))
                            {
                                target->RemoveAura(ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA, caster->GetGUID());
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_shroud_of_concealment_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnHit += SpellHitFn(spell_rog_shroud_of_concealment_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shroud_of_concealment_SpellScript();
        }
};

// Crimson Tempest - 121411
class spell_rog_crimson_tempest: public SpellScriptLoader
{
    public:
        spell_rog_crimson_tempest() : SpellScriptLoader("spell_rog_crimson_tempest") { }

        class spell_rog_crimson_tempest_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_crimson_tempest_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Player->GetGUID() == l_Target->GetGUID())
                            return;

                        uint8 l_ComboPoint = l_Player->GetComboPoints();
                        int32 l_Damage = 0;

                        if (l_ComboPoint)
                        {
                            float l_Ap = l_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);
                            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(ROGUE_SPELL_CRIMSON_TEMPEST_DOT);
                            int32 l_DamageDot = 0;

                            l_Damage += int32(3 + ((l_Ap * 0.0602f) * l_ComboPoint * 1.5f));

                            if (l_SpellInfo != nullptr)
                                l_DamageDot += CalculatePct(l_Damage, l_SpellInfo->Effects[EFFECT_0].BasePoints) / 6;

                            l_Player->CastCustomSpell(l_Target, ROGUE_SPELL_CRIMSON_TEMPEST_DOT, &l_DamageDot, NULL, NULL, true);
                        }
                        SetHitDamage(l_Damage);
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_crimson_tempest_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_crimson_tempest_SpellScript();
        }
};

// Slice and Dice - 5171
class spell_rog_slice_and_dice: public SpellScriptLoader
{
    public:
        spell_rog_slice_and_dice() : SpellScriptLoader("spell_rog_slice_and_dice") { }

        class spell_rog_slice_and_dice_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_slice_and_dice_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (AuraPtr sliceAndDice = caster->GetAura(ROGUE_SPELL_SLICE_AND_DICE))
                    {
                        int32 duration = sliceAndDice->GetDuration();
                        int32 maxDuration = sliceAndDice->GetMaxDuration();

                        // Replace old duration of Slice and Dice by the new duration ...
                        // ... five combo points : 36s instead of 30s
                        if (maxDuration >= 30000)
                        {
                            sliceAndDice->SetDuration(duration + 6000);
                            sliceAndDice->SetMaxDuration(maxDuration + 6000);
                        }
                        // ... four combo points : 30s instead of 25s
                        else if (maxDuration >= 25000)
                        {
                            sliceAndDice->SetDuration(duration + 5000);
                            sliceAndDice->SetMaxDuration(maxDuration + 5000);
                        }
                        // ... three combo points : 24s instead of 20s
                        else if (maxDuration >= 20000)
                        {
                            sliceAndDice->SetDuration(duration + 4000);
                            sliceAndDice->SetMaxDuration(maxDuration + 4000);
                        }
                        // ... two combo points : 18s instead of 15s
                        else if (maxDuration >= 15000)
                        {
                            sliceAndDice->SetDuration(duration + 3000);
                            sliceAndDice->SetMaxDuration(maxDuration + 3000);
                        }
                        // ... one combo point : 12s instead of 10s
                        else
                        {
                            sliceAndDice->SetDuration(duration + 2000);
                            sliceAndDice->SetMaxDuration(maxDuration + 2000);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_slice_and_dice_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_slice_and_dice_SpellScript();
        }
};

// Called by Deadly Poison - 2818
// Deadly Poison : Instant damage - 113780
class spell_rog_deadly_poison_instant_damage: public SpellScriptLoader
{
    public:
        spell_rog_deadly_poison_instant_damage() : SpellScriptLoader("spell_rog_deadly_poison_instant_damage") { }

        class spell_rog_deadly_poison_instant_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_deadly_poison_instant_damage_SpellScript);

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetExplTargetUnit())
                        if (target->HasAura(ROGUE_SPELL_DEADLY_POISON_DOT, caster->GetGUID()))
                            caster->CastSpell(target, ROGUE_SPELL_DEADLY_POISON_INSTANT_DAMAGE, true);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_rog_deadly_poison_instant_damage_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_deadly_poison_instant_damage_SpellScript();
        }
};

// Shiv - 5938
class spell_rog_shiv: public SpellScriptLoader
{
    public:
        spell_rog_shiv() : SpellScriptLoader("spell_rog_shiv") { }

        class spell_rog_shiv_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shiv_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (caster->HasAura(ROGUE_SPELL_CRIPPLING_POISON))
                            caster->CastSpell(target, ROGUE_SPELL_DEBILITATING_POISON, true);
                        else if (caster->HasAura(ROGUE_SPELL_LEECHING_POISON))
                            caster->CastSpell(caster, ROGUE_SPELL_LEECH_VITALITY, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_shiv_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shiv_SpellScript();
        }
};

// All Poisons
// Deadly Poison - 2823, Wound Poison - 8679, Mind-numbing Poison - 5761, Leeching Poison - 108211, Paralytic Poison - 108215 or Crippling Poison - 3408
class spell_rog_poisons: public SpellScriptLoader
{
    public:
        spell_rog_poisons() : SpellScriptLoader("spell_rog_poisons") { }

        class spell_rog_poisons_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_poisons_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    switch (GetSpellInfo()->Id)
                    {
                        case ROGUE_SPELL_WOUND_POISON:
                        {
                            if (caster->HasAura(ROGUE_SPELL_DEADLY_POISON))
                                caster->RemoveAura(ROGUE_SPELL_DEADLY_POISON);
                            break;
                        }
                        case ROGUE_SPELL_CRIPPLING_POISON:
                        {
                            if (caster->HasAura(ROGUE_SPELL_LEECHING_POISON))
                                caster->RemoveAura(ROGUE_SPELL_LEECHING_POISON);
                            break;
                        }
                        case ROGUE_SPELL_LEECHING_POISON:
                        {
                            if (caster->HasAura(ROGUE_SPELL_CRIPPLING_POISON))
                                caster->RemoveAura(ROGUE_SPELL_CRIPPLING_POISON);
                            break;
                        }
                        case ROGUE_SPELL_DEADLY_POISON:
                        {
                            if (caster->HasAura(ROGUE_SPELL_WOUND_POISON))
                                caster->RemoveAura(ROGUE_SPELL_WOUND_POISON);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_poisons_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_poisons_SpellScript();
        }
};

// Recuperate - 73651
class spell_rog_recuperate: public SpellScriptLoader
{
    public:
        spell_rog_recuperate() : SpellScriptLoader("spell_rog_recuperate") { }

        class spell_rog_recuperate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_recuperate_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (AuraPtr recuperate = caster->GetAura(ROGUE_SPELL_RECUPERATE))
                    {
                        int32 bp = caster->CountPctFromMaxHealth(caster->HasAura(ROGUE_SPELL_GLYPH_OF_RECUPERATE) ? 4 : 3);
                        bp = caster->SpellHealingBonusDone(caster, GetSpellInfo(), bp, EFFECT_0, HEAL);
                        bp = caster->SpellHealingBonusTaken(caster, GetSpellInfo(), bp, HEAL);
                        recuperate->GetEffect(0)->ChangeAmount(bp);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_recuperate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_recuperate_SpellScript();
        }
};

// Preparation - 14185
class spell_rog_preparation: public SpellScriptLoader
{
    public:
        spell_rog_preparation() : SpellScriptLoader("spell_rog_preparation") { }

        class spell_rog_preparation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_preparation_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();

                //immediately finishes the cooldown on certain Rogue abilities
                const SpellCooldowns& cm = caster->GetSpellCooldownMap();
                for (SpellCooldowns::const_iterator itr = cm.begin(); itr != cm.end();)
                {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);

                    if (spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE)
                    {
                        if (spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_ROGUE_VAN_EVAS_SPRINT ||   // Vanish, Evasion, Sprint
                            spellInfo->SpellFamilyFlags[1] & SPELLFAMILYFLAG1_ROGUE_DISMANTLE)          // Dismantle
                            caster->RemoveSpellCooldown((itr++)->first, true);
                        else
                            ++itr;
                    }
                    else
                        ++itr;
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Preparation
                OnEffectHitTarget += SpellEffectFn(spell_rog_preparation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_preparation_SpellScript();
        }
};

class spell_rog_deadly_poison: public SpellScriptLoader
{
    public:
        spell_rog_deadly_poison() : SpellScriptLoader("spell_rog_deadly_poison") { }

        class spell_rog_deadly_poison_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_deadly_poison_SpellScript);

            bool Load()
            {
                _stackAmount = 0;
                // at this point CastItem must already be initialized
                return GetCaster()->GetTypeId() == TYPEID_PLAYER && GetCastItem();
            }

            void HandleBeforeHit()
            {
                if (Unit* target = GetHitUnit())
                    // Deadly Poison
                    if (constAuraEffectPtr aurEff = target->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_ROGUE, 0x10000, 0x80000, 0, GetCaster()->GetGUID()))
                        _stackAmount = aurEff->GetBase()->GetStackAmount();
            }

            void HandleAfterHit()
            {
                if (_stackAmount < 5)
                    return;

                Player* player = GetCaster()->ToPlayer();

                if (Unit* target = GetHitUnit())
                {
                    Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                    if (item == GetCastItem())
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                    if (!item)
                        return;

                    // item combat enchantments
                    for (uint8 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot)
                    {
                        if (slot > PRISMATIC_ENCHANTMENT_SLOT || slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
                            continue;

                        SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(item->GetEnchantmentId(EnchantmentSlot(slot)));
                        if (!enchant)
                            continue;

                        for (uint8 s = 0; s < 3; ++s)
                        {
                            if (enchant->type[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                                continue;

                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(enchant->spellid[s]);
                            if (!spellInfo)
                            {
                                sLog->outError(LOG_FILTER_SPELLS_AURAS, "Player::CastItemCombatSpell Enchant %i, player (Name: %s, GUID: %u) cast unknown spell %i", enchant->ID, player->GetName(), player->GetGUIDLow(), enchant->spellid[s]);
                                continue;
                            }

                            // Proc only rogue poisons
                            if (spellInfo->SpellFamilyName != SPELLFAMILY_ROGUE || spellInfo->Dispel != DISPEL_POISON)
                                continue;

                            // Do not reproc deadly
                            if (spellInfo->SpellFamilyFlags.IsEqual(0x10000, 0x80000, 0))
                                continue;

                            if (spellInfo->IsPositive())
                                player->CastSpell(player, enchant->spellid[s], true, item);
                            else
                                player->CastSpell(target, enchant->spellid[s], true, item);
                        }
                    }
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_rog_deadly_poison_SpellScript::HandleBeforeHit);
                AfterHit += SpellHitFn(spell_rog_deadly_poison_SpellScript::HandleAfterHit);
            }

            uint8 _stackAmount;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_deadly_poison_SpellScript();
        }
};

// Shadowstep - 36554
class spell_rog_shadowstep: public SpellScriptLoader
{
    public:
        spell_rog_shadowstep() : SpellScriptLoader("spell_rog_shadowstep") { }

        class spell_rog_shadowstep_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shadowstep_SpellScript);

            SpellCastResult CheckCast()
            {
                if (GetCaster()->HasUnitState(UNIT_STATE_ROOT))
                    return SPELL_FAILED_ROOTED;
                if (GetExplTargetUnit() && GetExplTargetUnit() == GetCaster())
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_rog_shadowstep_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shadowstep_SpellScript();
        }
};

// Stealth - 1784
class spell_rog_stealth: public SpellScriptLoader
{
    public:
        spell_rog_stealth() : SpellScriptLoader("spell_rog_stealth") { }

        class spell_rog_stealth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_stealth_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->CastSpell(l_Caster, 158188, true);
                    l_Caster->CastSpell(l_Caster, 158185, true);
                    if (l_Caster->HasSpell(ROGUE_SPELL_SHADOW_FOCUS_AURA))
                        l_Caster->CastSpell(l_Caster, ROGUE_SPELL_SHADOW_FOCUS_COST_PCT, true);
                }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->RemoveAura(158188);
                    l_Caster->RemoveAura(158185);
                    if (l_Caster->HasAura(ROGUE_SPELL_SHADOW_FOCUS_COST_PCT))
                        l_Caster->RemoveAura(ROGUE_SPELL_SHADOW_FOCUS_COST_PCT);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_stealth_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_stealth_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

    AuraScript* GetAuraScript() const
    {
        return new spell_rog_stealth_AuraScript();
    }
};

// Burst of Speed - 108212
class spell_rog_burst_of_speed: public SpellScriptLoader
{
public:
    spell_rog_burst_of_speed() : SpellScriptLoader("spell_rog_burst_of_speed") { }

    class spell_rog_burst_of_speed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_burst_of_speed_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
                l_Caster->CastSpell(l_Caster, ROGUE_SPELL_BURST_OF_SPEED, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_burst_of_speed_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_burst_of_speed_SpellScript();
    }
};

// Call by Kidney Shot 408
// Internal Bleeding - 154904
class spell_rog_internal_bleeding: public SpellScriptLoader
{
public:
    spell_rog_internal_bleeding() : SpellScriptLoader("spell_rog_internal_bleeding") { }

    class spell_rog_internal_bleeding_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_internal_bleeding_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
                if (Unit* l_Target = GetHitUnit())
                    if (l_Caster->HasAura(ROGUE_SPELL_INTERNAL_BLEEDING_AURA))
                        l_Caster->CastSpell(l_Target, ROGUE_SPELL_INTERNAL_BLEEDING, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_internal_bleeding_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_internal_bleeding_SpellScript();
    }
};

// Smoke Bomb - 76577
class spell_rog_smoke_bomb: public SpellScriptLoader
{
public:
    spell_rog_smoke_bomb() : SpellScriptLoader("spell_rog_smoke_bomb") { }

    class spell_rog_smoke_bomb_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_smoke_bomb_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
            {
                AreaTrigger* l_Area = l_Caster->GetAreaTrigger(GetSpellInfo()->Id);
                l_Caster->CastSpell(l_Area->GetPositionX(), l_Area->GetPositionY(), l_Area->GetPositionZ(), ROGUE_SPELL_SMOKE_BOMB, true);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_smoke_bomb_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_smoke_bomb_SpellScript();
    }
};

// Fan of Knives - 51723
class spell_rog_fan_of_knives: public SpellScriptLoader
{
public:
    spell_rog_fan_of_knives() : SpellScriptLoader("spell_rog_fan_of_knives") { }

    class spell_rog_fan_of_knives_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_fan_of_knives_SpellScript);

        void HandleAfterHit()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->EnergizeBySpell(l_Player, GetSpellInfo()->Id, l_Player->GetPower(POWER_COMBO_POINT) + 1, POWER_COMBO_POINT);
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_rog_fan_of_knives_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_fan_of_knives_SpellScript();
    }
};

// Call by Kidney Shot 408 - Eviscerate 2098 - Recuperate 73651 - Slice and Dice 5171 - Deadly Throw 26679 - Rupture 1943
// Relentless Strikes - 58423
class spell_rog_retenless_strikes: public SpellScriptLoader
{
public:
    spell_rog_retenless_strikes() : SpellScriptLoader("spell_rog_retenless_strikes") { }

    class spell_rog_retenless_strikes_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_retenless_strikes_SpellScript);

        void HandleOnHit()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                if (l_Player->HasAura(ROGUE_SPELL_RELTENTLESS_STRIKES_AURA))
                {
                    if (roll_chance_i(20 * l_Player->GetPower(POWER_COMBO_POINT)))
                        l_Player->CastSpell(l_Player, ROGUE_SPELL_RELTENTLESS_STRIKES_PROC, true);
                }
                if (l_Player->HasAura(ROGUE_SPELL_RUTHLESSNESS))
                {
                    if (roll_chance_i(20 * l_Player->GetPower(POWER_COMBO_POINT)))
                    {
                        l_Player->CastSpell(l_Player, ROGUE_SPELL_COMBO_POINT_DELAYED, true);
                        l_Player->CastSpell(l_Player, ROGUE_SPELL_RELTENTLESS_STRIKES_PROC, true);
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_retenless_strikes_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_retenless_strikes_SpellScript();
    }
};

// Relentless Strikes proc - 14181 
class spell_rog_retenless_strikes_proc: public SpellScriptLoader
{
public:
    spell_rog_retenless_strikes_proc() : SpellScriptLoader("spell_rog_retenless_strikes_proc") { }

    class spell_rog_retenless_strikes_proc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_retenless_strikes_proc_SpellScript);

        void HandleOnHit()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->EnergizeBySpell(l_Player, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_0].BasePoints, POWER_ENERGY);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_retenless_strikes_proc_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_retenless_strikes_proc_SpellScript();
    }
};

// Combo Point Delayed - 139569
class spell_rog_combo_point_delayed: public SpellScriptLoader
{
public:
    spell_rog_combo_point_delayed() : SpellScriptLoader("spell_rog_combo_point_delayed") { }

    class spell_rog_combo_point_delayed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_combo_point_delayed_SpellScript);

        void HandleOnHit()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->EnergizeBySpell(l_Player, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_0].BasePoints, POWER_COMBO_POINT);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_combo_point_delayed_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_combo_point_delayed_SpellScript();
    }
};

// Eviscerate - 2098
class spell_rog_evicerate : public SpellScriptLoader
{
public:
    spell_rog_evicerate() : SpellScriptLoader("spell_rog_evicerate") { }

    class spell_rog_evicerate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_evicerate_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                uint8 l_ComboPoint = l_Player->GetComboPoints();
                int32 l_Damage = 0;

                if (l_ComboPoint)
                {
                    float l_Ap = l_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);

                    l_Damage += int32((l_Ap * 0.577f) * 0.88f * l_ComboPoint);

                    // Eviscerate and Envenom Bonus Damage (item set effect)
                    if (l_Player->HasAura(ROGUE_SPELL_EVISCERATE_ENVENOM_BONUS_DAMAGE))
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(ROGUE_SPELL_EVISCERATE_ENVENOM_BONUS_DAMAGE);

                        if (l_SpellInfo != nullptr)
                            l_Damage += l_ComboPoint * l_SpellInfo->Effects[EFFECT_0].BasePoints;
                    }
                }
                SetHitDamage(l_Damage);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_rog_evicerate_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_evicerate_SpellScript();
    }
};

// Deadly Throw - 26679
class spell_rog_deadly_throw : public SpellScriptLoader
{
public:
    spell_rog_deadly_throw() : SpellScriptLoader("spell_rog_deadly_throw") { }

    class spell_rog_deadly_throw_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_deadly_throw_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                uint8 l_ComboPoint = l_Player->GetComboPoints();
                int32 l_Damage = 0;

                if (l_ComboPoint)
                {
                    float l_Ap = l_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);

                    l_Damage += int32((l_Ap * 0.178f) * 1 * l_ComboPoint);
                }
                if (l_ComboPoint >= 5)
                    if (Unit* l_Target = GetHitUnit())
                        l_Player->CastSpell(l_Target, ROGUE_SPELL_DEADLY_THROW_INTERRUPT, true);
                SetHitDamage(l_Damage);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_rog_deadly_throw_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_deadly_throw_SpellScript();
    }
};

class PlayerScript_ruthlessness : public PlayerScript
{
public:
    PlayerScript_ruthlessness() : PlayerScript("PlayerScript_ruthlessness") {}

    void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_Value)
    {
        if (p_Player->getClass() == CLASS_ROGUE && p_Player->GetSpecializationId(p_Player->GetActiveSpec()) == SPEC_ROGUE_COMBAT && p_Power == POWER_COMBO_POINT)
        if (p_Value < 0 && p_Player->HasAura(ROGUE_SPELL_RUTHLESSNESS))
        {
            if (p_Player->HasSpellCooldown(ROGUE_SPELL_ADRENALINE_RUSH))
                p_Player->ReduceSpellCooldown(ROGUE_SPELL_ADRENALINE_RUSH, sSpellMgr->GetSpellInfo(ROGUE_SPELL_RUTHLESSNESS)->Effects[EFFECT_2].BasePoints * p_Value * -1);
            else if (p_Player->HasSpellCooldown(ROGUE_SPELL_KILLING_SPREE))
                p_Player->ReduceSpellCooldown(ROGUE_SPELL_KILLING_SPREE, sSpellMgr->GetSpellInfo(ROGUE_SPELL_RUTHLESSNESS)->Effects[EFFECT_2].BasePoints * p_Value * -1);
            else if (p_Player->HasSpellCooldown(ROGUE_SPELL_SPRINT))
                p_Player->ReduceSpellCooldown(ROGUE_SPELL_SPRINT, sSpellMgr->GetSpellInfo(ROGUE_SPELL_RUTHLESSNESS)->Effects[EFFECT_2].BasePoints * p_Value * -1);
        }
    }
};

void AddSC_rogue_spell_scripts()
{
    new spell_rog_deadly_throw();
    new spell_rog_evicerate();
    new spell_rog_envenom();
    new spell_rog_combo_point_delayed();
    new spell_rog_retenless_strikes_proc();
    new spell_rog_retenless_strikes();
    new spell_rog_fan_of_knives();
    new spell_rog_smoke_bomb();
    new spell_rog_internal_bleeding();
    new spell_rog_burst_of_speed();
    new spell_rog_killing_spree();
    new spell_rog_glyph_of_decoy();
    new spell_rog_shuriken_toss();
    new spell_rog_marked_for_death();
    new spell_rog_cloak_and_dagger();
    new spell_rog_cheat_death();
    new spell_rog_blade_flurry();
    new spell_rog_growl();
    new spell_rog_cloak_of_shadows();
    new spell_rog_combat_readiness();
    new spell_rog_nerve_strike();
    new spell_rog_nightstalker();
    new spell_rog_sanguinary_vein();
    new spell_rog_hemorrhage();
    new spell_rog_cut_to_the_chase();
    new spell_rog_venomous_wounds();
    new spell_rog_redirect();
    new spell_rog_shroud_of_concealment();
    new spell_rog_crimson_tempest();
    new spell_rog_slice_and_dice();
    new spell_rog_deadly_poison_instant_damage();
    new spell_rog_shiv();
    new spell_rog_poisons();
    new spell_rog_recuperate();
    new spell_rog_preparation();
    new spell_rog_deadly_poison();
    new spell_rog_shadowstep();
    new spell_rog_stealth();

    // Player Script
    new PlayerScript_ruthlessness();
}
