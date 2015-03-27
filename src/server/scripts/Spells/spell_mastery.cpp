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
 * Scripts for spells with MASTERY.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mastery_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum MasterySpells
{
    MASTERY_SPELL_LIGHTNING_BOLT        = 45284,
    MASTERY_SPELL_CHAIN_LIGHTNING       = 45297,
    MASTERY_SPELL_LAVA_BURST            = 77451,
    MASTERY_SPELL_ELEMENTAL_BLAST       = 120588,
    MASTERY_SPELL_HAND_OF_LIGHT         = 96172,
    MASTERY_SPELL_IGNITE_AURA           = 12654,
    MASTERY_SPELL_BLOOD_SHIELD          = 77535,
    MASTERY_SPELL_DISCIPLINE_SHIELD     = 77484,
    SPELL_DK_SCENT_OF_BLOOD             = 50421,
    SPELL_MAGE_MASTERY_ICICLES          = 76613,
    SPELL_MAGE_ICICLE_DAMAGE            = 148022,
    SPELL_MAGE_ICICLE_PERIODIC_TRIGGER  = 148023,
    SPELL_PRIEST_ECHO_OF_LIGHT          = 77489,
    SPELL_WARRIOR_WEAPONS_MASTER        = 76838,
    SPELL_WARLOCK_METAMORPHIS           = 103958,
    SPELL_WARLOCK_MASTER_DEMONOLOGIST   = 77219,
    SPELL_PRIEST_MENTAL_ANGUISH         = 77486,
    MASTERY_SPELL_IGNITE                = 12846
};

enum MoltenEarthSpells
{
    MoltenEarthDamage   = 170379,
    MoltenEarthPeriodic = 170377,
    MoltenEarthAura     = 170374
};

/// Molten Earth - 170374
class spell_mastery_molten_earth : public SpellScriptLoader
{
    public:
        spell_mastery_molten_earth() : SpellScriptLoader("spell_mastery_molten_earth") { }

        class spell_mastery_molten_earth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_molten_earth_AuraScript);

            void OnProc(constAuraEffectPtr, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == MoltenEarthSpells::MoltenEarthDamage)
                    return;

                Unit* l_Caster = GetCaster();
                Unit* l_Target = p_EventInfo.GetDamageInfo()->GetVictim();
                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                /// Assuming it's a 33.33% proc chance
                if (!roll_chance_f(33.33f))
                    return;

                if (l_Target->HasAura(MoltenEarthSpells::MoltenEarthPeriodic))
                {
                    if (AuraPtr l_PeriodicAura = l_Target->GetAura(MoltenEarthSpells::MoltenEarthPeriodic))
                        l_PeriodicAura->RefreshDuration();
                }
                else
                    l_Caster->AddAura(MoltenEarthSpells::MoltenEarthPeriodic, l_Target);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mastery_molten_earth_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_molten_earth_AuraScript();
        }
};

/// Molten Earth Periodic - 170377
class spell_mastery_molten_earth_periodic: public SpellScriptLoader
{
    public:
        spell_mastery_molten_earth_periodic() : SpellScriptLoader("spell_mastery_molten_earth_periodic") { }

        class spell_mastery_molten_earth_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_molten_earth_periodic_AuraScript);

            void HandleEffectPeriodic(constAuraEffectPtr)
            {
                PreventDefaultAction();

                if (GetOwner() == nullptr)
                    return;

                if (Unit* l_Owner = GetOwner()->ToUnit())
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        uint8 l_Count = irand(1, 2);

                        for (uint8 l_I = 0; l_I < l_Count; l_I++)
                            l_Caster->CastSpell(l_Owner, MoltenEarthSpells::MoltenEarthDamage, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_molten_earth_periodic_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_molten_earth_periodic_AuraScript();
        }
};

/// Molten Earth Damage - 170379
class spell_mastery_molten_earth_damage: public SpellScriptLoader
{
    public:
        spell_mastery_molten_earth_damage() : SpellScriptLoader("spell_mastery_molten_earth_damage") { }

        class spell_mastery_molten_earth_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_molten_earth_damage_SpellScript);

            void HandleDamage(SpellEffIndex)
            {
                Unit* l_Target = GetHitUnit();
                Unit* l_Caster = GetCaster();
                if (!l_Target || !l_Caster)
                    return;

                if (AuraPtr l_Aura = l_Caster->GetAura(MoltenEarthSpells::MoltenEarthAura))
                {
                    if (Player* l_Player = l_Caster->ToPlayer())
                        SetHitDamage(GetHitDamage() * l_Player->GetFloatValue(PLAYER_FIELD_MASTERY) * l_Aura->GetSpellInfo()->Effects[EFFECT_0].BonusMultiplier / 100);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mastery_molten_earth_damage_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_molten_earth_damage_SpellScript();
        }
};

/// Mastery: Razor Claws - 77493
class spell_mastery_razor_claws : public SpellScriptLoader
{
    public:
        spell_mastery_razor_claws() : SpellScriptLoader("spell_mastery_razor_claws") { }

        class spell_mastery_razor_claws_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_razor_claws_AuraScript);

            void CalculateAmount(constAuraEffectPtr, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    float l_Mastery = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * float(GetSpellInfo()->Effects[EFFECT_1].BonusMultiplier);
                    p_Amount = l_Mastery;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_razor_claws_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_razor_claws_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_razor_claws_AuraScript();
        }
};

/// 76671 - Mastery : Divine Bulwark
class spell_mastery_divine_bulwark : public SpellScriptLoader
{
    public:
        spell_mastery_divine_bulwark() : SpellScriptLoader("spell_mastery_divine_bulwark") { }

        class spell_mastery_divine_bulwark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_divine_bulwark_AuraScript);

            void CalculateAmount(constAuraEffectPtr p_AuraPtr, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    float l_Mastery = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * float(GetSpellInfo()->Effects[p_AuraPtr->GetEffIndex()].BonusMultiplier);
                    p_Amount = l_Mastery;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_divine_bulwark_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_ADD_FLAT_MODIFIER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_divine_bulwark_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_ADD_FLAT_MODIFIER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_divine_bulwark_AuraScript::CalculateAmount, EFFECT_4, SPELL_AURA_MOD_ATTACK_POWER_PCT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_divine_bulwark_AuraScript();
        }
};

/// Mastery: Executioner - 76808
class spell_mastery_executioner : public SpellScriptLoader
{
    public:
        spell_mastery_executioner() : SpellScriptLoader("spell_mastery_executioner") { }

        class spell_mastery_executioner_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_executioner_AuraScript);

            void CalculateAmount(constAuraEffectPtr p_AuraPtr, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    float l_Mastery = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * float(GetSpellInfo()->Effects[p_AuraPtr->GetEffIndex()].BonusMultiplier);
                    p_Amount = l_Mastery;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_executioner_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_executioner_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_executioner_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_executioner_AuraScript();
        }
};

/// Mastery: Potent Poisons - 76803
class spell_mastery_potent_poisons : public SpellScriptLoader
{
    public:
        spell_mastery_potent_poisons() : SpellScriptLoader("spell_mastery_potent_poisons") { }

        class spell_mastery_potent_poisons_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_potent_poisons_AuraScript);

            void CalculateAmount(constAuraEffectPtr, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    float l_Mastery = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * (float(GetSpellInfo()->Effects[EFFECT_2].BasePoints) / 100.0f);
                    p_Amount = l_Mastery;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_potent_poisons_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_potent_poisons_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_potent_poisons_AuraScript();
        }
};

/// Mastery: Sniper Training - 76659
class spell_mastery_sniper_training : public SpellScriptLoader
{
    public:
        spell_mastery_sniper_training() : SpellScriptLoader("spell_mastery_sniper_training") { }

        enum Masteries
        {
            RecentlyMoved       = 168809,
            SniperTrainingAura  = 168811
        };

        class spell_mastery_sniper_training_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_sniper_training_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr)
            {
                if (!GetUnitOwner())
                    return;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    if (AuraPtr l_Aura = l_Player->GetAura(Masteries::SniperTrainingAura))
                    {
                        if (l_Player->isMoving() && l_Aura->GetDuration() == -1)
                        {
                            l_Aura->SetDuration(6000);
                            l_Aura->SetMaxDuration(6000);
                        }
                        else if (!l_Player->isMoving() && l_Aura->GetDuration() != -1 && !l_Player->HasAura(Masteries::RecentlyMoved))
                            l_Player->CastSpell(l_Player, Masteries::RecentlyMoved, true);
                    }
                    else if (!l_Player->isMoving() && !l_Player->HasAura(Masteries::RecentlyMoved))
                        l_Player->CastSpell(l_Player, Masteries::RecentlyMoved, true);
                }
            }

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->RemoveAura(Masteries::RecentlyMoved);
                    l_Caster->RemoveAura(Masteries::SniperTrainingAura);
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_mastery_sniper_training_AuraScript::OnUpdate, EFFECT_2, SPELL_AURA_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_mastery_sniper_training_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_sniper_training_AuraScript();
        }
};

/// Sniper Training: Recently Moved - 168809
class spell_mastery_recently_moved : public SpellScriptLoader
{
    public:
        spell_mastery_recently_moved() : SpellScriptLoader("spell_mastery_recently_moved") { }

        enum Masteries
        {
            SniperTrainingAura = 168811
        };

        class spell_mastery_recently_moved_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_recently_moved_AuraScript);

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                Unit* l_Caster = GetCaster();
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                {
                    if (Player* l_Player = l_Caster->ToPlayer())
                    {
                        float l_Mastery = l_Player->GetFloatValue(EPlayerFields::PLAYER_FIELD_MASTERY) * 0.5f;
                        int32 l_BasePoints = l_Mastery;

                        l_Player->CastCustomSpell(l_Player, Masteries::SniperTrainingAura, &l_BasePoints, &l_BasePoints, &l_BasePoints, &l_BasePoints, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mastery_recently_moved_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_recently_moved_AuraScript();
        }
};

/// Sniper Training - 168811
class spell_mastery_sniper_training_aura : public SpellScriptLoader
{
    public:
        spell_mastery_sniper_training_aura() : SpellScriptLoader("spell_mastery_sniper_training_aura") { }

        class spell_mastery_sniper_training_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_sniper_training_aura_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr p_AurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SpecIndex::SPEC_HUNTER_MARKSMANSHIP)
                        p_AurEff->GetBase()->Remove();

                    float l_Mastery = l_Player->GetFloatValue(EPlayerFields::PLAYER_FIELD_MASTERY) * 0.5f;
                    int32 l_BasePoints = l_Mastery;

                    if (AuraPtr l_Aura = p_AurEff->GetBase())
                    {
                        for (uint8 l_I = 0; l_I < 4; ++l_I)
                        {
                            if (AuraEffectPtr l_Effect = l_Aura->GetEffect(l_I))
                                l_Effect->ChangeAmount(l_BasePoints);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_mastery_sniper_training_aura_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_sniper_training_aura_AuraScript();
        }
};

// Mastery: Echo of Light - 77485
class spell_mastery_echo_of_light: public SpellScriptLoader
{
    public:
        spell_mastery_echo_of_light() : SpellScriptLoader("spell_mastery_echo_of_light") { }

        class spell_mastery_echo_of_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_echo_of_light_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (!eventInfo.GetHealInfo() || !eventInfo.GetHealInfo()->GetHeal())
                    return;

                Unit* unitTarget = eventInfo.GetActionTarget();
                Player* plr = GetCaster()->ToPlayer();
                if (!unitTarget || !plr)
                    return;

                float Mastery = plr->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.25f / 100.0f;
                int32 bp = (Mastery * eventInfo.GetHealInfo()->GetHeal()) / 6;

                bp += unitTarget->GetRemainingPeriodicAmount(plr->GetGUID(), SPELL_PRIEST_ECHO_OF_LIGHT, SPELL_AURA_PERIODIC_HEAL);
                plr->CastCustomSpell(unitTarget, SPELL_PRIEST_ECHO_OF_LIGHT, &bp, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mastery_echo_of_light_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_echo_of_light_AuraScript();
        }
};

const int IcicleAuras[5] = { 148012, 148013, 148014, 148015, 148016 };
const int IcicleHits[5] = { 148017, 148018, 148019, 148020, 148021 };
bool IcicleOverstack = false;

// Called by Frostbolt - 116 and Frostfire bolt - 44614
// Mastery: Icicles - 76613
class spell_mastery_icicles: public SpellScriptLoader
{
    public:
        spell_mastery_icicles() : SpellScriptLoader("spell_mastery_icicles") { }

        class spell_mastery_icicles_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_icicles_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        // Calculate damage
                        int32 l_Damage = GetHitDamage();
                        if (GetSpell()->IsCritForTarget(l_Target))
                            l_Damage *= 2;

                        /// Prevent huge hits on player after hitting low level creatures
                        if (l_Player->getLevel() > l_Target->getLevel())
                            l_Damage = std::min((uint32)l_Damage, l_Target->GetMaxHealth());

                        l_Damage *= ((l_Player->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f) / 100.0f);

                        int8 l_CurrMaxAura = 0;

                        // if hitDamage == 0 we have a miss, so we need to except this variant
                        if (l_Player->HasAura(SPELL_MAGE_MASTERY_ICICLES) && l_Damage != 0)
                        {
                            // We need to find how much icicles we have, and which is the last.
                            for (int l_I = 0; l_I < 5; l_I++)
                            {
                                if (l_Player->HasAura(IcicleAuras[l_I]))
                                    l_CurrMaxAura = l_I + 1;
                            }

                            switch (l_CurrMaxAura)
                            {
                                case 0:
                                    l_Player->AddAura(IcicleAuras[0], l_Player);
                                    break;
                                case 1:
                                    l_Player->AddAura(IcicleAuras[1], l_Player);
                                    break;
                                case 2:
                                    l_Player->AddAura(IcicleAuras[2], l_Player);
                                    break;
                                case 3:
                                    l_Player->AddAura(IcicleAuras[3], l_Player);
                                    break;
                                case 4:
                                    l_Player->AddAura(IcicleAuras[4], l_Player);
                                    break;
                                case 5:
                                {
                                    // We need to find an icicle, with the smallest duration.
                                    int32 l_MinDur = 0;
                                    int32 l_SmallestIcicle = 0;
                                    if (AuraPtr l_CurrAura = l_Player->GetAura(IcicleAuras[l_SmallestIcicle]))
                                        l_MinDur = l_Player->GetAura(IcicleAuras[0])->GetDuration();

                                    for (int l_I = 1; l_I < 5; l_I++)
                                    {
                                        if (AuraPtr l_TmpCurrAura = l_Player->GetAura(IcicleAuras[l_I]))
                                        {
                                            if (l_MinDur > l_TmpCurrAura->GetDuration())
                                            {
                                                l_MinDur = l_TmpCurrAura->GetDuration();
                                                l_SmallestIcicle = l_I;
                                            }
                                        }
                                    }

                                    if (AuraPtr l_CurrAura = l_Player->GetAura(IcicleAuras[l_SmallestIcicle]))
                                    {
                                        int32 l_BP = l_CurrAura->GetEffect(0)->GetAmount();
                                        l_Player->CastSpell(l_Target, IcicleHits[l_SmallestIcicle], true);
                                        l_Player->CastCustomSpell(l_Target, SPELL_MAGE_ICICLE_DAMAGE, &l_BP, NULL, NULL, true);
                                        l_Player->RemoveAura(IcicleAuras[l_SmallestIcicle]);
                                    }

                                    l_Player->AddAura(IcicleAuras[l_SmallestIcicle], l_Player);

                                    if (AuraPtr l_CurrAura = l_Player->GetAura(IcicleAuras[l_SmallestIcicle]))
                                        l_CurrAura->GetEffect(0)->SetAmount(int32(l_Damage));

                                    IcicleOverstack = true;
                                    break;
                                }
                            }

                            if (IcicleOverstack == false)
                            {
                                if (AuraPtr l_CurrAura = l_Player->GetAura(IcicleAuras[l_CurrMaxAura]))
                                    l_CurrAura->GetEffect(0)->SetAmount(l_Damage);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_icicles_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_icicles_SpellScript();
        }
};

// Ice Lance - 30455
class spell_mastery_icicles_trigger: public SpellScriptLoader
{
    public:
        spell_mastery_icicles_trigger() : SpellScriptLoader("spell_mastery_icicles_trigger") { }

        class spell_mastery_icicles_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_icicles_trigger_SpellScript);

            void HandleOnHit(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(SPELL_MAGE_ICICLE_PERIODIC_TRIGGER))
                        return;

                    if (GetHitUnit())
                        caster->SetIciclesTarget(GetHitUnit()->GetGUID());

                    caster->CastSpell(caster, SPELL_MAGE_ICICLE_PERIODIC_TRIGGER, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mastery_icicles_trigger_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_icicles_trigger_SpellScript();
        }
};

// Icicles (periodic) - 148023
class spell_mastery_icicles_periodic: public SpellScriptLoader
{
    public:
        spell_mastery_icicles_periodic() : SpellScriptLoader("spell_mastery_icicles_periodic") { }

        class spell_mastery_icicles_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_icicles_periodic_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (AuraEffectPtr aura = caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                    {
                        if (aura->GetAmount() > 4)
                            caster->RemoveAura(GetSpellInfo()->Id);

                        // Maybe not the good target selection ...
                        if (Unit* target = ObjectAccessor::FindUnit(caster->GetIciclesTarget()))
                        {
                            if (aura->GetAmount() < 5)
                            {
                                if (AuraPtr icicleCurrentAura = caster->GetAura(IcicleAuras[aura->GetAmount()]))
                                {
                                    int32 basepoints = icicleCurrentAura->GetEffect(0)->GetAmount();
                                    caster->CastSpell(target, IcicleHits[aura->GetAmount()], true);
                                    caster->CastCustomSpell(target, SPELL_MAGE_ICICLE_DAMAGE, &basepoints, NULL, NULL, true);
                                    caster->RemoveAura(IcicleAuras[aura->GetAmount()]);
                                    aura->SetAmount(aura->GetAmount() + 1);
                                }
                            }

                            IcicleOverstack = false;
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_icicles_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_icicles_periodic_AuraScript();
        }
};

// Icicles: 148017, 148018, 148019, 148020, 148021
class spell_mastery_icicles_hit: public SpellScriptLoader
{
    public:
        spell_mastery_icicles_hit() : SpellScriptLoader("spell_mastery_icicles_hit") { }

        class spell_mastery_icicles_hit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_icicles_hit_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetGUID() != _player->GetGUID())
                        {
                            int8 currentMinAura = 0;
                            if (IcicleOverstack == false)
                            {
                                // We need to find the first icicle and if we found it - break, because function will be called one more time.
                                for (int i = 4; i >= 0; i--)
                                {
                                    if (AuraPtr icicleCurrentAura = _player->GetAura(IcicleAuras[i]))
                                    {
                                        int32 basepoints = icicleCurrentAura->GetEffect(0)->GetAmount();
                                        _player->CastSpell(target, IcicleHits[i], true);
                                        _player->CastCustomSpell(target, SPELL_MAGE_ICICLE_DAMAGE, &basepoints, NULL, NULL, true);
                                        _player->RemoveAura(IcicleAuras[i]);
                                        break;
                                    }
                                }
                            }

                            IcicleOverstack = false;
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_icicles_hit_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_icicles_hit_SpellScript();
        }
};

// Called by Clarity of Will - 152118, Power Word : Shield - 17, Power Word : Shield (Divine Insight) - 123258, Spirit Shell - 114908, Angelic Bulwark - 114214 and Divine Aegis - 47753
// Mastery : Shield Discipline - 77484
class spell_mastery_shield_discipline: public SpellScriptLoader
{
    public:
        spell_mastery_shield_discipline() : SpellScriptLoader("spell_mastery_shield_discipline") { }

        class spell_mastery_shield_discipline_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_shield_discipline_AuraScript);

            void CalculateAmount(constAuraEffectPtr , int32 & p_Amount, bool & )
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(MASTERY_SPELL_DISCIPLINE_SHIELD) && caster->getLevel() >= 80)
                    {
                        float l_Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.625f;
                        p_Amount += CalculatePct(p_Amount, l_Mastery);
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_shield_discipline_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_shield_discipline_AuraScript();
        }
};

// Called by 45470 - Death Strike (Heal)
// 77513 - Mastery : Blood Shield
class spell_mastery_blood_shield: public SpellScriptLoader
{
    public:
        spell_mastery_blood_shield() : SpellScriptLoader("spell_mastery_blood_shield") { }

        class spell_mastery_blood_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_blood_shield_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _plr = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_plr->GetTypeId() == TYPEID_PLAYER && _plr->HasAura(77513) && _plr->getLevel() >= 80)
                        {
                            // Check the Mastery aura while in Blood presence
                            if (_plr->HasAura(77513) && _plr->HasAura(48263))
                            {
                                float Mastery = _plr->GetFloatValue(PLAYER_FIELD_MASTERY) * 6.25f / 100.0f;

                                int32 bp = -int32(GetHitDamage() * Mastery);

                                if (AuraPtr scentOfBlood = _plr->GetAura(SPELL_DK_SCENT_OF_BLOOD))
                                    AddPct(bp, (scentOfBlood->GetStackAmount() * 20));

                                if (AuraEffectPtr bloodShield = target->GetAuraEffect(MASTERY_SPELL_BLOOD_SHIELD, EFFECT_0))
                                    bp += bloodShield->GetAmount();

                                bp = std::min(uint32(bp), target->GetMaxHealth());

                                _plr->CastCustomSpell(target, MASTERY_SPELL_BLOOD_SHIELD, &bp, NULL, NULL, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_blood_shield_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_blood_shield_SpellScript();
        }
};

// Called by 133 - Fireball, 44614 - Frostfire Bolt, 108853 - Inferno Blast, 2948 - Scorch and 11366 - Pyroblast
// 12846 - Mastery : Ignite
class spell_mastery_ignite: public SpellScriptLoader
{
    public:
        spell_mastery_ignite() : SpellScriptLoader("spell_mastery_ignite") { }

        class spell_mastery_ignite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_ignite_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->GetTypeId() == TYPEID_PLAYER && l_Caster->HasAura(MASTERY_SPELL_IGNITE) && l_Caster->getLevel() >= 80)
                        {
                            const SpellInfo *l_SpellInfo = sSpellMgr->GetSpellInfo(MASTERY_SPELL_IGNITE_AURA);
                            if (GetSpellInfo()->Id != MASTERY_SPELL_IGNITE_AURA && l_SpellInfo != nullptr)
                            {
                                float l_Value = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.5f;

                                int32 l_Bp = GetHitDamage();

                                if (l_Bp)
                                {
                                    l_Bp = int32(CalculatePct(l_Bp, l_Value));

                                    if (l_SpellInfo->Effects[EFFECT_0].Amplitude > 0)
                                        l_Bp = l_Bp / (l_SpellInfo->GetMaxDuration() / l_SpellInfo->Effects[EFFECT_0].Amplitude);
                                    
                                    if (l_Target->HasAura(MASTERY_SPELL_IGNITE_AURA, l_Caster->GetGUID()))
                                        l_Bp += l_Target->GetRemainingPeriodicAmount(l_Caster->GetGUID(), MASTERY_SPELL_IGNITE_AURA, SPELL_AURA_PERIODIC_DAMAGE);

                                    l_Caster->CastCustomSpell(l_Target, MASTERY_SPELL_IGNITE_AURA, &l_Bp, NULL, NULL, true);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_ignite_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_ignite_SpellScript();
        }
};

// Called by 35395 - Crusader Strike, 53595 - Hammer of the Righteous, 24275 - Hammer of Wrath, 85256 - Templar's Verdict, 53385 - Divine Storm, 157048 - Final Verdict
// 76672 - Mastery : Hand of Light
class spell_mastery_hand_of_light: public SpellScriptLoader
{
    public:
        spell_mastery_hand_of_light() : SpellScriptLoader("spell_mastery_hand_of_light") { }

        class spell_mastery_hand_of_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_hand_of_light_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->GetTypeId() == TYPEID_PLAYER && l_Caster->HasAura(76672) && l_Caster->getLevel() >= 80)
                        {
                            uint32 l_ProcSpellId = GetSpellInfo()->Id ? GetSpellInfo()->Id : 0;
                            if (l_ProcSpellId != MASTERY_SPELL_HAND_OF_LIGHT)
                            {
                                int32 l_Bp = int32(CalculatePct(GetHitDamage(), l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.25f));
                                l_Caster->CastCustomSpell(l_Target, MASTERY_SPELL_HAND_OF_LIGHT, &l_Bp, NULL, NULL, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_hand_of_light_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_hand_of_light_SpellScript();
        }
};

// Called by 403 - Lightning Bolt, 421 - Chain Lightning, 51505 - Lava Burst and 117014 - Elemental Blast
// 77222 - Mastery : Elemental Overload
class spell_mastery_elemental_overload: public SpellScriptLoader
{
    public:
        spell_mastery_elemental_overload() : SpellScriptLoader("spell_mastery_elemental_overload") { }

        class spell_mastery_elemental_overload_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_elemental_overload_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(403) || !sSpellMgr->GetSpellInfo(421) || !sSpellMgr->GetSpellInfo(51505) || !sSpellMgr->GetSpellInfo(117014))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                SpellInfo const* procSpell = GetSpellInfo();

                if (procSpell)
                {
                    if (Unit* caster = GetCaster())
                    {
                        if (Unit* unitTarget = GetHitUnit())
                        {
                            // Lavaburst deals 50% more damage with Flame Shock on target
                            if (procSpell->Id == 51505 && unitTarget->HasAura(8050))
                                SetHitDamage(int32(float(GetHitDamage()) * 1.5f));

                            if (caster->GetTypeId() == TYPEID_PLAYER && caster->HasAura(77222))
                            {
                                // Every Lightning Bolt, Chain Lightning and Lava Burst spells have duplicate vs 75% damage and no cost
                                switch (procSpell->Id)
                                {
                                    // Lava Burst
                                    case 51505:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_LAVA_BURST, true);

                                        break;
                                    }
                                    // Lightning Bolt
                                    case 403:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_LIGHTNING_BOLT, true);

                                        break;
                                    }
                                    // Chain Lightning
                                    case 421:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_CHAIN_LIGHTNING, true);

                                        break;
                                    }
                                    // Elemental Blast
                                    case 117014:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                        {
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_ELEMENTAL_BLAST, true);
                                            caster->CastSpell(unitTarget, 118517, true); // Nature visual
                                            caster->CastSpell(unitTarget, 118515, true); // Frost visual
                                        }

                                        break;
                                    }
                                    default: break;
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_elemental_overload_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_elemental_overload_SpellScript();
        }
};

// Call by Mortal Strike - 12294, Colossus Smash - 167105, Execute - 5308
// Mastery: Weapons Master - 76338
class spell_mastery_weapons_master : public SpellScriptLoader
{
    public:
        spell_mastery_weapons_master() : SpellScriptLoader("spell_mastery_weapons_master") { }

        class spell_mastery_weapons_master_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_weapons_master_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_WARRIOR_WEAPONS_MASTER))
                    {
                        float l_MasteryValue = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 3.5f;

                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_MasteryValue));
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_weapons_master_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_weapons_master_SpellScript();
        }
};

/// Called by Doom - 603, Immolation Aura - 140720
/// Mastery: Master Demonologist - 77219
class spell_mastery_master_demonologist_aura : public SpellScriptLoader
{
    public:
        spell_mastery_master_demonologist_aura() : SpellScriptLoader("spell_mastery_master_demonologist_aura") { }

        class spell_mastery_master_demonologist_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_master_demonologist_aura_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*p_AuraEffect*/, int32& p_Amount, bool& /*p_CanBeRecalculate*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                /// Further increases the damage of your Touch of Chaos, Chaos Wave, Doom, Immolation Aura, and Soul Fire while in Metamorphosis by 12%.
                if (l_Caster->HasAura(SPELL_WARLOCK_METAMORPHIS) && l_Caster->HasAura(SPELL_WARLOCK_MASTER_DEMONOLOGIST))
                {
                    float l_MasteryValue = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.5f;
                    p_Amount += CalculatePct(p_Amount, l_MasteryValue);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_master_demonologist_aura_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_master_demonologist_aura_AuraScript();
        }
};

/// Called by Chaos wave - 124915, Touch of Chao - 103964, Soul Fire - 6353
/// Mastery: Master Demonologist - 77219
class spell_mastery_master_demonologist : public SpellScriptLoader
{
    public:
        spell_mastery_master_demonologist() : SpellScriptLoader("spell_mastery_master_demonologist") { }

        class spell_mastery_master_demonologist_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_master_demonologist_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();

                /// Further increases the damage of your Touch of Chaos, Chaos Wave, Doom, Immolation Aura, and Soul Fire while in Metamorphosis by 12%.
                if (l_Caster->HasAura(SPELL_WARLOCK_METAMORPHIS) && l_Caster->HasAura(SPELL_WARLOCK_MASTER_DEMONOLOGIST))
                {
                    float l_MasteryValue = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.5f;
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_MasteryValue));
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mastery_master_demonologist_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_master_demonologist_SpellScript();
        }
};

enum MasterAnguish
{
    SPELL_PRIEST_MIND_BLAST = 8092,
    SPELL_PRIEST_MIND_SPIKE = 73510,
    SPELL_PRIEST_MIND_FLAY = 15407
};

// Called by 8092 - Mind Blast, 73510 - Mind Spike, 15407 - Mind Flay
// 77486 - Mastery: Mental Anguish
class spell_mastery_master_mental_anguish : public SpellScriptLoader
{
    public:
        spell_mastery_master_mental_anguish() : SpellScriptLoader("spell_mastery_master_mental_anguish") { }

        class spell_mastery_master_mental_anguish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_master_mental_anguish_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (GetSpellInfo()->Id != SPELL_PRIEST_MIND_BLAST && GetSpellInfo()->Id != SPELL_PRIEST_MIND_SPIKE)
                        return;

                    /// Increases the damage of Mind Blast, Mind Spike, and Mind Flay by 20 %.
                    if (l_Caster->HasAura(SPELL_PRIEST_MENTAL_ANGUISH))
                    {
                        float l_MasteryValue = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.5f;
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_MasteryValue));
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mastery_master_mental_anguish_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        class spell_mastery_master_mental_anguish_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_master_mental_anguish_AuraScript);

            void CalculateAmount(constAuraEffectPtr, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (GetSpellInfo()->Id != SPELL_PRIEST_MIND_FLAY)
                        return;

                    if (l_Caster->HasAura(SPELL_PRIEST_MENTAL_ANGUISH))
                    {
                        float l_MasteryValue = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.5f;
                        p_Amount += CalculatePct(p_Amount, l_MasteryValue);
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_master_mental_anguish_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_master_mental_anguish_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_master_mental_anguish_SpellScript();
        }
};

void AddSC_mastery_spell_scripts()
{
    new spell_mastery_molten_earth();
    new spell_mastery_molten_earth_periodic();
    new spell_mastery_molten_earth_damage();
    new spell_mastery_razor_claws();
    new spell_mastery_executioner();
    new spell_mastery_potent_poisons();
    new spell_mastery_sniper_training();
    new spell_mastery_recently_moved();
    new spell_mastery_sniper_training_aura();
    new spell_mastery_echo_of_light();
    new spell_mastery_icicles();
    new spell_mastery_icicles_trigger();
    new spell_mastery_icicles_periodic();
    new spell_mastery_icicles_hit();
    new spell_mastery_shield_discipline();
    new spell_mastery_blood_shield();
    new spell_mastery_ignite();
    new spell_mastery_hand_of_light();
    new spell_mastery_elemental_overload();
    new spell_mastery_weapons_master();
    new spell_mastery_master_demonologist();
    new spell_mastery_master_demonologist_aura();
    new spell_mastery_master_mental_anguish();
    new spell_mastery_divine_bulwark();
}