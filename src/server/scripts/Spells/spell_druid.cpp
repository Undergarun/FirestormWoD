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
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"


enum YseraGiftSpells
{
    SPELL_DRUID_YSERAS_GIFT_HEAL_CASTER = 145109,
    SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY   = 145110
};

// Ysera's Gift - 145108
class spell_dru_yseras_gift: public SpellScriptLoader
{
    public:
        spell_dru_yseras_gift() : SpellScriptLoader("spell_dru_yseras_gift") { }

        class spell_dru_yseras_gift_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_yseras_gift_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (!l_Caster->IsFullHealth())
                {
                    int32 l_HealAmount = CalculatePct(l_Caster->GetMaxHealth(), p_AurEff->GetAmount());
                    l_Caster->CastCustomSpell(l_Caster, SPELL_DRUID_YSERAS_GIFT_HEAL_CASTER, &l_HealAmount, NULL, NULL, true);
                }
                else
                {
                    std::list<Unit*> l_Party;

                    l_Caster->GetPartyMembers(l_Party);

                    l_Party.remove_if([l_Caster](Unit* p_Unit) {
                        return (p_Unit->IsFullHealth() || p_Unit->GetDistance(l_Caster) >= 40.0f);
                    });

                    if (l_Party.empty())
                        return;

                    if (l_Party.size() > 1)
                    {
                        l_Party.sort(JadeCore::HealthPctOrderPred());
                        l_Party.resize(1); // Just to be sure
                    }

                    int32 l_HealAmount = CalculatePct(l_Caster->GetMaxHealth(), p_AurEff->GetAmount());
                    l_Caster->CastCustomSpell(l_Party.front(), SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY, &l_HealAmount, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_yseras_gift_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_yseras_gift_AuraScript();
        }
};

enum ToothAndClawAbsorbSpells
{
    SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA = 135601
};

// Tooth and Claw - 135597
class spell_dru_tooth_and_claw_absorb: public SpellScriptLoader
{
    public:
        spell_dru_tooth_and_claw_absorb() : SpellScriptLoader("spell_dru_tooth_and_claw_absorb") { }

        class spell_dru_tooth_and_claw_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_tooth_and_claw_absorb_AuraScript);

            void OnAbsorb(AuraEffectPtr /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (Unit* attacker = dmgInfo.GetAttacker())
                    if (!attacker->HasAura(SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA))
                        absorbAmount = 0;
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_tooth_and_claw_absorb_AuraScript::OnAbsorb, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_tooth_and_claw_absorb_AuraScript();
        }
};

enum GenesisSpells
{
    SPELL_DRUID_REJUVENATION = 774
};

// Genesis - 145518
class spell_dru_genesis: public SpellScriptLoader
{
    public:
        spell_dru_genesis() : SpellScriptLoader("spell_dru_genesis") { }

        class spell_dru_genesis_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_genesis_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                if (Player* plr = GetCaster()->ToPlayer())
                {
                    std::list<Unit*> partyMembers;
                    plr->GetPartyMembers(partyMembers);

                    for (auto itr : partyMembers)
                    {
                        if (!itr->IsWithinDist(plr, 60.0f) || !itr->IsWithinLOSInMap(plr))
                            continue;

                        if (AuraEffectPtr rejuvenation = itr->GetAuraEffect(SPELL_DRUID_REJUVENATION, EFFECT_0))
                        {
                            itr->SetHealth(itr->GetHealth() + plr->GetHealingDoneInPastSecs(3));
                            itr->RemoveAura(SPELL_DRUID_REJUVENATION);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_genesis_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_genesis_SpellScript();
        }
};

enum GlyphOfTheTreantSpells
{
    SPELL_DRUID_GLYPH_OF_THE_TREANT = 114282
};

// Glyph of the Treant - 125047
class spell_dru_glyph_of_the_treant: public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_the_treant() : SpellScriptLoader("spell_dru_glyph_of_the_treant") { }

        class spell_dru_glyph_of_the_treant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_the_treant_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(SPELL_DRUID_GLYPH_OF_THE_TREANT, false);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(SPELL_DRUID_GLYPH_OF_THE_TREANT))
                        _player->removeSpell(SPELL_DRUID_GLYPH_OF_THE_TREANT, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_the_treant_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_the_treant_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_glyph_of_the_treant_AuraScript();
        }
};

enum IncarnationChosenOfElune
{
    SPELL_DRUID_INCARNATION_CHOSEN_OF_ELUNE = 122114
};

// Incarnation : Chosen of Elune - 102560
class spell_dru_incarnation_chosen_of_elune: public SpellScriptLoader
{
    public:
        spell_dru_incarnation_chosen_of_elune() : SpellScriptLoader("spell_dru_incarnation_chosen_of_elune") { }

        class spell_dru_incarnation_chosen_of_elune_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_chosen_of_elune_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->CastSpell(_player, SPELL_DRUID_INCARNATION_CHOSEN_OF_ELUNE, true);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->RemoveAura(SPELL_DRUID_INCARNATION_CHOSEN_OF_ELUNE);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_incarnation_chosen_of_elune_AuraScript();
        }
};

// Called by Incarnation : Chosen of Elune - 102560, Incarnation : Son of Ursoc - 102558 and Incarnation : King of the Jungle - 102543
// Incarnation - Skins
class spell_dru_incarnation_skins: public SpellScriptLoader
{
    public:
        spell_dru_incarnation_skins() : SpellScriptLoader("spell_dru_incarnation_skins") { }

        class spell_dru_incarnation_skins_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_skins_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->SetDisplayId(_player->GetModelForForm(_player->GetShapeshiftForm()));
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->SetDisplayId(_player->GetModelForForm(_player->GetShapeshiftForm()));
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case 102543:// King of the Jungle
                        OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case 102558:// Son of Ursoc
                        OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case 102560:// Chosen of Elune
                        OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_incarnation_skins_AuraScript();
        }
};

enum ItemPVPFeral4PBonus
{
    SPELL_DRUID_STAMPEDE                    = 81022,
    SPELL_DRUID_INCARNATION_KING_OF_JUNGLE  = 102543
};

// Item - PvP Feral 4P Bonus - 131537
class spell_dru_item_pvp_feral_4p: public SpellScriptLoader
{
    public:
        spell_dru_item_pvp_feral_4p() : SpellScriptLoader("spell_dru_item_pvp_feral_4p") { }

        class spell_dru_item_pvp_feral_4p_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_pvp_feral_4p_AuraScript);

            uint32 update;

            bool Validate(SpellInfo const* /*spell*/)
            {
                update = 0;

                if (!sSpellMgr->GetSpellInfo(131537))
                    return false;
                return true;
            }

            void OnUpdate(uint32 diff, AuraEffectPtr /*aurEff*/)
            {
                if (!GetCaster())
                    return;

                if (GetCaster()->HasAura(SPELL_DRUID_STAMPEDE))
                    return;

                update += diff;

                if (GetCaster()->HasAura(SPELL_DRUID_INCARNATION_KING_OF_JUNGLE))
                    return;

                if (update >= 30 * IN_MILLISECONDS)
                {
                    if (Player* _player = GetCaster()->ToPlayer())
                        _player->CastSpell(_player, SPELL_DRUID_STAMPEDE, true);

                    update = 0;
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_item_pvp_feral_4p_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_item_pvp_feral_4p_AuraScript();
        }
};

// Wild Charge (Moonkin) - 102383
class spell_dru_wild_charge_moonkin: public SpellScriptLoader
{
    public:
        spell_dru_wild_charge_moonkin() : SpellScriptLoader("spell_dru_wild_charge_moonkin") { }

        class spell_dru_wild_charge_moonkin_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_charge_moonkin_SpellScript);

            SpellCastResult CheckFight()
            {
                if (GetCaster())
                {
                    if (!GetCaster()->isInCombat())
                        return SPELL_FAILED_DONT_REPORT;
                }
                else
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_wild_charge_moonkin_SpellScript::CheckFight);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_charge_moonkin_SpellScript();
        }
};

// Thrash (bear) - 77758
class spell_dru_thrash_bear: public SpellScriptLoader
{
    public:
        spell_dru_thrash_bear() : SpellScriptLoader("spell_dru_thrash_bear") { }

        class spell_dru_thrash_bear_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_thrash_bear_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    // Each tick grant 1 point of rage
                    caster->SetPower(POWER_RAGE, caster->GetPower(POWER_RAGE) + 1 * caster->GetPowerCoeff(POWER_RAGE));
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_thrash_bear_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_thrash_bear_AuraScript();
        }
};

// Swipe - 106785
class spell_dru_swipe: public SpellScriptLoader
{
    public:
        spell_dru_swipe() : SpellScriptLoader("spell_dru_swipe") { }

        class spell_dru_swipe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_swipe_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 damage = GetHitDamage();

                        // Award 1 combot point
                        if (Player* plr = caster->ToPlayer())
                            plr->AddComboPoints(GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                        // Swipe and Maul deals 20% more damage if target is bleeding
                        if (target->HasAuraState(AURA_STATE_BLEEDING))
                        {
                            AddPct(damage, GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                            SetHitDamage(damage);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_swipe_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_swipe_SpellScript();
        }
};

enum MaulSpells
{
    SPELL_DRUID_TOOTH_AND_CLAW_AURA         = 135286,
    SPELL_DRUID_TOOTH_AND_CLAW_ABSORB       = 135597,
    //SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA  = 135601
};

// Maul - 6807
class spell_dru_maul: public SpellScriptLoader
{
    public:
        spell_dru_maul() : SpellScriptLoader("spell_dru_maul") { }

        class spell_dru_maul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_maul_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 damage = GetHitDamage();

                        // Deals 20% more damage if target is bleeding
                        if (target->HasAuraState(AURA_STATE_BLEEDING))
                        {
                            AddPct(damage, GetSpellInfo()->Effects[EFFECT_3].BasePoints);
                            SetHitDamage(damage);
                        }

                        if (caster->HasAura(SPELL_DRUID_TOOTH_AND_CLAW_AURA))
                        {
                            int32 bp = caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 2.4f;

                            caster->RemoveAura(SPELL_DRUID_TOOTH_AND_CLAW_AURA);
                            caster->CastCustomSpell(caster, SPELL_DRUID_TOOTH_AND_CLAW_ABSORB, &bp, NULL, NULL, true);
                            caster->CastCustomSpell(target, SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA, &bp, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_maul_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_maul_SpellScript();
        }
};

enum MoonfireSpells
{
    SPELL_DRUID_MOONFIRE_DAMAGE = 164812
};

// Moonfire - 8921
class spell_dru_moonfire: public SpellScriptLoader
{
    public:
        spell_dru_moonfire() : SpellScriptLoader("spell_dru_moonfire") { }

        class spell_dru_moonfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonfire_SpellScript);

            void HandleOnHit()
            {
                if (Player* caster = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_DRUID_MOONFIRE_DAMAGE);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_moonfire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_moonfire_SpellScript();
        }
};

// Moonfire - 164812
class spell_dru_moonfire_damage: public SpellScriptLoader
{
    public:
        spell_dru_moonfire_damage() : SpellScriptLoader("spell_dru_moonfire_damage") { }

        class spell_dru_moonfire_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonfire_damage_SpellScript);

            void HandleOnHit()
            {
                if (Player* caster = GetCaster()->ToPlayer())
                {
                    int32 damage =  caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * GetSpellInfo()->Effects[EFFECT_0].BonusMultiplier;
                    SetHitDamage(damage);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_moonfire_damage_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_moonfire_damage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_moonfire_damage_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Player* caster = GetCaster()->ToPlayer())
                    amount = caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * GetSpellInfo()->Effects[EFFECT_1].BonusMultiplier;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_moonfire_damage_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_moonfire_damage_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_moonfire_damage_AuraScript();
        }
};

enum NaturesVigilSpells
{
    SPELL_DRUID_NATURES_VIGIL_HEAL   = 124988,
    SPELL_DRUID_NATURES_VIGIL_DAMAGE = 124991
};

// Nature's Vigil - 124974
class spell_dru_natures_vigil: public SpellScriptLoader
{
    public:
        spell_dru_natures_vigil() : SpellScriptLoader("spell_dru_natures_vigil") { }

        class spell_dru_natures_vigil_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_natures_vigil_AuraScript);

            void OnProcHeal(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                Unit* caster = GetCaster();
                if (!caster)
                    return;

                if (eventInfo.GetActor()->GetGUID() != caster->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                bool singleTarget = false;
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    if ((eventInfo.GetDamageInfo()->GetSpellInfo()->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                         eventInfo.GetDamageInfo()->GetSpellInfo()->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY) &&
                        eventInfo.GetDamageInfo()->GetSpellInfo()->Effects[i].TargetB.GetTarget() == 0)
                        singleTarget = true;

                if (!singleTarget)
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_DRUID_NATURES_VIGIL_HEAL ||
                    eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_DRUID_NATURES_VIGIL_DAMAGE)
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamage()) && !(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE) && !(eventInfo.GetDamageInfo()->GetDamageType() == HEAL))
                    return;

                int32 bp = 0;
                Unit* target = NULL;
                uint32 spellId = 0;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo()->IsPositive())
                {
                    bp = CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount());
                    spellId = SPELL_DRUID_NATURES_VIGIL_DAMAGE;
                    target = caster->SelectNearbyTarget(caster, 25.0f);
                    if (!target)
                        target = eventInfo.GetActionTarget();
                }

                if (!target || !spellId || !bp)
                    return;

                caster->CastCustomSpell(target, spellId, &bp, NULL, NULL, true);
            }

            void OnProcDamage(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                Unit* caster = GetCaster();
                if (!caster)
                    return;

                if (eventInfo.GetActor()->GetGUID() != caster->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                bool singleTarget = false;
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    if ((eventInfo.GetDamageInfo()->GetSpellInfo()->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                         eventInfo.GetDamageInfo()->GetSpellInfo()->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY) &&
                        eventInfo.GetDamageInfo()->GetSpellInfo()->Effects[i].TargetB.GetTarget() == 0)
                        singleTarget = true;

                if (!singleTarget)
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_DRUID_NATURES_VIGIL_HEAL ||
                    eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_DRUID_NATURES_VIGIL_DAMAGE)
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamage()) && !(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE) && !(eventInfo.GetDamageInfo()->GetDamageType() == HEAL))
                    return;

                int32 bp = 0;
                Unit* target = NULL;
                uint32 spellId = 0;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->IsPositive())
                {
                    bp = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
                    spellId = SPELL_DRUID_NATURES_VIGIL_HEAL;
                    target = caster->SelectNearbyAlly(caster, 25.0f);
                    if (!target)
                        target = caster;
                }

                if (!target || !spellId || !bp)
                    return;

                caster->CastCustomSpell(target, spellId, &bp, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_natures_vigil_AuraScript::OnProcHeal, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectProc += AuraEffectProcFn(spell_dru_natures_vigil_AuraScript::OnProcDamage, EFFECT_3, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_natures_vigil_AuraScript();
        }
};

enum UrsolsVortexSpells
{
    SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER = 102793,
    SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST    = 118283,
    SPELL_DRUID_URSOLS_VORTEX_SNARE        = 127797
};

// Ursol's Vortex - 102793
class spell_dru_ursols_vortex: public SpellScriptLoader
{
public:
    spell_dru_ursols_vortex() : SpellScriptLoader("spell_dru_ursols_vortex") { }

    class spell_dru_ursols_vortex_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_ursols_vortex_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (!target->HasAura(SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER))
                        _player->CastSpell(target, SPELL_DRUID_URSOLS_VORTEX_SNARE, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_dru_ursols_vortex_SpellScript::HandleOnHit);
        }
    };

    class spell_dru_ursols_vortex_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_ursols_vortex_AuraScript);

        std::list<Unit*> targetList;

        void OnUpdate(uint32 /*diff*/, AuraEffectPtr aurEff)
        {
            aurEff->GetTargetList(targetList);

            for (auto itr : targetList)
            {
                if (Unit* caster = GetCaster())
                    if (DynamicObject* dynObj = caster->GetDynObject(SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER))
                        if (itr->GetDistance(dynObj) > 8.0f && !itr->HasAura(SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST))
                            itr->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST, true);
            }

            targetList.clear();
        }

        void Register()
        {
            OnEffectUpdate += AuraEffectUpdateFn(spell_dru_ursols_vortex_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_ursols_vortex_SpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_ursols_vortex_AuraScript();
    }
};

// Ursol's Vortex (snare) - 127797
class spell_dru_ursols_vortex_snare: public SpellScriptLoader
{
    public:
        spell_dru_ursols_vortex_snare() : SpellScriptLoader("spell_dru_ursols_vortex_snare") { }

        class spell_dru_ursols_vortex_snare_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursols_vortex_snare_AuraScript);

            std::list<Unit*> targetList;

            void OnUpdate(uint32 /*diff*/, AuraEffectPtr aurEff)
            {
                aurEff->GetTargetList(targetList);

                for (auto itr : targetList)
                {
                    if (Unit* caster = GetCaster())
                        if (AreaTrigger* areaTrigger = caster->GetAreaTrigger(SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER))
                            if (itr->GetDistance(areaTrigger) > 8.0f && !itr->HasAura(SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST))
                                itr->CastSpell(areaTrigger->GetPositionX(), areaTrigger->GetPositionY(), areaTrigger->GetPositionZ(), SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST, true);
                }

                targetList.clear();
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_ursols_vortex_snare_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_ursols_vortex_snare_AuraScript();
        }
};

enum SolarBeamSpells
{
    SPELL_DRUID_SOLAR_BEAM         = 78675,
    SPELL_DRUID_SOLAR_BEAM_SILENCE = 81261
};

// Solar beam - 78675
class spell_dru_solar_beam: public SpellScriptLoader
{
    public:
        spell_dru_solar_beam() : SpellScriptLoader("spell_dru_solar_beam") { }

        class spell_dru_solar_beam_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_solar_beam_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_DRUID_SOLAR_BEAM))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_DRUID_SOLAR_BEAM_SILENCE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_solar_beam_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_solar_beam_AuraScript();
        }
};

enum DashSpells
{
    SPELL_DRUID_STAMPEDING_ROAR = 106898
};

// Dash - 1850
class spell_dru_dash: public SpellScriptLoader
{
    public:
        spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

        class spell_dru_dash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_dash_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_DRUID_STAMPEDING_ROAR))
                        _player->RemoveAura(SPELL_DRUID_STAMPEDING_ROAR);

                    _player->RemoveMovementImpairingAuras();
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_dash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_dash_SpellScript();
        }
};

enum SavageDefenseSpells
{
    SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT = 132402
};

// Savage Defense - 62606
class spell_dru_savage_defense: public SpellScriptLoader
{
    public:
        spell_dru_savage_defense() : SpellScriptLoader("spell_dru_savage_defense") { }

        class spell_dru_savage_defense_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_defense_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_savage_defense_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_savage_defense_SpellScript();
        }
};

enum LifebloomSpells
{
    SPELL_DRUID_LIFEBLOOM                 = 33763,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL      = 33778,
    SPELL_DRUID_GLYPH_OF_BLOOMING         = 121840
};

// Called by Regrowth - 8936 and Healing Touch - 5185
// Lifebloom - 33763 : Refresh duration
class spell_dru_lifebloom_refresh: public SpellScriptLoader
{
    public:
        spell_dru_lifebloom_refresh() : SpellScriptLoader("spell_dru_lifebloom_refresh") { }

        class spell_dru_lifebloom_refresh_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_lifebloom_refresh_SpellScript);

            void HandleOnHit()
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!player->HasAura(SPELL_DRUID_GLYPH_OF_BLOOMING))
                            if (AuraPtr lifebloom = target->GetAura(SPELL_DRUID_LIFEBLOOM, player->GetGUID()))
                                lifebloom->RefreshDuration();
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_lifebloom_refresh_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_lifebloom_refresh_SpellScript();
        }
};

// Lifebloom - 33763
class spell_dru_lifebloom: public SpellScriptLoader
{
    public:
        spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

        class spell_dru_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lifebloom_AuraScript);

            void AfterRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                // Final heal only on duration end
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                // final heal
                int32 l_SpellPower = 0;
                int32 l_HealAmount = 0;

                l_SpellPower = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_SpellPower, aurEff->GetEffIndex(), HEAL);
                l_SpellPower = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_SpellPower, HEAL);

                l_HealAmount = l_SpellPower * sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL)->Effects[EFFECT_0].BonusMultiplier;

                l_Target->CastCustomSpell(l_Target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &l_HealAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (constAuraEffectPtr aurEff = GetEffect(EFFECT_0))
                {
                    Unit* l_Target = dispelInfo->GetDispeller();
                    Unit* l_Caster = GetCaster();
                    if (!l_Caster)
                        return;

                    // final heal
                    int32 l_SpellPower = 0;
                    int32 l_HealAmount = 0;

                    l_SpellPower = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_SpellPower, aurEff->GetEffIndex(), HEAL, dispelInfo->GetRemovedCharges());
                    l_SpellPower = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_SpellPower, HEAL, dispelInfo->GetRemovedCharges());

                    l_HealAmount = l_SpellPower * sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL)->Effects[EFFECT_0].BonusMultiplier;

                    l_Target->CastCustomSpell(l_Target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &l_HealAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                AfterDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_lifebloom_AuraScript();
        }
};

enum KillerInstinct
{
    SPELL_DRUID_KILLER_INSTINCT          = 108299,
    SPELL_DRUID_KILLER_INSTINCT_MOD_STAT = 108300
};

// Called by Cat Form - 768 and Bear Form - 5487
// Killer Instinct - 108299
class spell_dru_killer_instinct: public SpellScriptLoader
{
    public:
        spell_dru_killer_instinct() : SpellScriptLoader("spell_dru_killer_instinct") { }

        class spell_dru_killer_instinct_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_killer_instinct_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_DRUID_KILLER_INSTINCT))
                    {
                        int32 bp = _player->GetStat(STAT_INTELLECT);

                        _player->CastCustomSpell(_player, SPELL_DRUID_KILLER_INSTINCT_MOD_STAT, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_killer_instinct_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_killer_instinct_SpellScript();
        }
};

// Nature's Cure - 88423 and Remove Corruption - 2782
class spell_dru_natures_cure: public SpellScriptLoader
{
    public:
        spell_dru_natures_cure() : SpellScriptLoader("spell_dru_natures_cure") { }

        class spell_dru_natures_cure_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_natures_cure_SpellScript);

            SpellCastResult CheckCleansing()
            {
                Unit* caster = GetCaster();
                Unit* target = GetExplTargetUnit();
                if (!caster || !target)
                    return SPELL_FAILED_ERROR;

                DispelChargesList dispelList;

                // Create dispel mask by dispel type
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                {
                    if (!GetSpellInfo()->Effects[i].IsEffect())
                        break;

                    uint32 dispel_type = GetSpellInfo()->Effects[i].MiscValue;
                    uint32 dispelMask  = GetSpellInfo()->GetDispelMask(DispelType(dispel_type));

                    target->GetDispellableAuraList(caster, dispelMask, dispelList);
                }

                if (dispelList.empty())
                    return SPELL_FAILED_NOTHING_TO_DISPEL;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_natures_cure_SpellScript::CheckCleansing);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_natures_cure_SpellScript();
        }
};

// Cat Form - 768
class spell_dru_cat_form: public SpellScriptLoader
{
    public:
        spell_dru_cat_form() : SpellScriptLoader("spell_dru_cat_form") { }

        class spell_dru_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_cat_form_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveMovementImpairingAuras();
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_cat_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_cat_form_SpellScript();
        }
};

enum SkullBashSpells
{
    SPELL_DRUID_SKULL_BASH_INTERUPT = 93985
};

// Skull Bash - 106839
class spell_dru_skull_bash: public SpellScriptLoader
{
    public:
        spell_dru_skull_bash() : SpellScriptLoader("spell_dru_skull_bash") { }

        class spell_dru_skull_bash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_skull_bash_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_DRUID_SKULL_BASH_INTERUPT, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_skull_bash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_skull_bash_SpellScript();
        }
};

enum FaerieSwarmSpells
{
    SPELL_DRUID_FAERIE_DECREASE_SPEED       = 102354,
    SPELL_DRUID_GLYPH_OF_FAE_SILENCE        = 114237,
    SPELL_DRUID_FAE_SILENCE                 = 114238
};

// Faerie Swarm - 102355
class spell_dru_faerie_swarm: public SpellScriptLoader
{
    public:
        spell_dru_faerie_swarm() : SpellScriptLoader("spell_dru_faerie_swarm") { }

        class spell_dru_faerie_swarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_faerie_swarm_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        l_Player->CastSpell(l_Target, SPELL_DRUID_FAERIE_DECREASE_SPEED, true);

                        // Glyph of Fae Silence
                        if (l_Player->GetShapeshiftForm() == FORM_BEAR && l_Player->HasAura(SPELL_DRUID_GLYPH_OF_FAE_SILENCE))
                        {
                            l_Player->CastSpell(l_Target, SPELL_DRUID_FAE_SILENCE, true);
                            l_Player->ToPlayer()->AddSpellCooldown(GetSpellInfo()->Id, 0, 15 * IN_MILLISECONDS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_faerie_swarm_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_faerie_swarm_SpellScript();
        }
};

enum WildMushroomSpells
{
    DRUID_NPC_WILD_MUSHROOM                = 47649,
    SPELL_DRUID_WILD_MUSHROOM_HEAL         = 81269,
    SPELL_DRUID_WILD_MUSHROOM_FUNGAL_CLOUD = 81281,
    SPELL_DRUID_WILD_MUSHROOM_BALANCE      = 88747,
    SPELL_DRUID_MUSHROOM_BIRTH_VISUAL      = 94081,
    SPELL_DRUID_WILD_MUSHROOM_RESTORATION  = 145205
};

// Wild Mushroom (Balance) - 88747
// Wild Mushroom (Restoration) - 145205
class spell_dru_wild_mushroom: public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom() : SpellScriptLoader("spell_dru_wild_mushroom") { }

        class spell_dru_wild_mushroom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_SpellScript)

            void HandleSummon(SpellEffIndex p_EffIndex)
            {
                PreventHitDefaultEffect(p_EffIndex);

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                std::list<Creature*> l_Mushroomlist;
                l_Player->GetCreatureListWithEntryInGrid(l_Mushroomlist, DRUID_NPC_WILD_MUSHROOM, 500.0f);

                // Remove other player mushrooms
                for (std::list<Creature*>::iterator i = l_Mushroomlist.begin(); i != l_Mushroomlist.end(); ++i)
                {
                    Unit* l_Owner = (*i)->GetOwner();
                    if (l_Owner && l_Owner == l_Player && (*i)->isSummon())
                        continue;

                    l_Mushroomlist.remove((*i));
                }

                if (static_cast<int32>(l_Mushroomlist.size()) >= GetSpellInfo()->Effects[p_EffIndex].BasePoints)
                {
                    // 1 mushrooms max
                    if (GetSpellInfo()->Id == SPELL_DRUID_WILD_MUSHROOM_RESTORATION)
                    {
                        Creature* l_Mushroom = l_Mushroomlist.back();

                        // Recasting Wild Mushroom will move the Mushroom without losing this accumulated healing.
                        if (WorldLocation* l_Dest = const_cast<WorldLocation*>(GetExplTargetDest()))
                        {
                            l_Mushroom->NearTeleportTo(l_Dest->GetPositionX(), l_Dest->GetPositionY(), l_Dest->GetPositionZ(), l_Mushroom->GetOrientation());
                            return;
                        }
                    }
                    else if (GetSpellInfo()->Id == SPELL_DRUID_WILD_MUSHROOM_BALANCE) // 3 mushrooms max
                    {
                        if (static_cast<int32>(l_Mushroomlist.size()) >= GetSpellInfo()->Effects[p_EffIndex].BasePoints)
                            l_Mushroomlist.back()->ToTempSummon()->UnSummon();
                    }
                }

                Position l_Pos;
                GetExplTargetDest()->GetPosition(&l_Pos);
                TempSummon* l_Summon = l_Player->SummonCreature(GetSpellInfo()->Effects[p_EffIndex].MiscValue, l_Pos, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, GetSpellInfo()->GetDuration());
                if (l_Summon)
                {
                    l_Summon->SetGuidValue(UNIT_FIELD_SUMMONED_BY, l_Player->GetGUID());
                    l_Summon->setFaction(l_Player->getFaction());
                    l_Summon->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, GetSpellInfo()->Id);
                    l_Summon->SetMaxHealth(GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                    l_Summon->SetFullHealth();
                    l_Summon->CastSpell(l_Summon, SPELL_DRUID_MUSHROOM_BIRTH_VISUAL, true);

                    if (GetSpellInfo()->Id == SPELL_DRUID_WILD_MUSHROOM_RESTORATION)
                        l_Summon->CastSpell(l_Summon, SPELL_DRUID_WILD_MUSHROOM_HEAL, true);
                    else if (GetSpellInfo()->Id == SPELL_DRUID_WILD_MUSHROOM_BALANCE)
                        l_Summon->CastSpell(l_Summon, SPELL_DRUID_WILD_MUSHROOM_FUNGAL_CLOUD, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_dru_wild_mushroom_SpellScript::HandleSummon, EFFECT_1, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_SpellScript();
        }
};

enum CelestialAlignementSpells
{
    SPELL_DRUID_NATURES_GRACE            = 16886,
    SPELL_DRUID_STARFALL                 = 48505,
    SPELL_DRUID_SOLAR_ECLIPSE            = 48517,
    SPELL_DRUID_LUNAR_ECLIPSE            = 48518,
    SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE = 81070,
    SPELL_DRUID_STARSURGE_ENERGIZE       = 86605,
    SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE   = 107095
};

// Celestial Alignment - 112071
class spell_dru_celestial_alignment: public SpellScriptLoader
{
    public:
        spell_dru_celestial_alignment() : SpellScriptLoader("spell_dru_celestial_alignment") { }

        class spell_dru_celestial_alignment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_celestial_alignment_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    _player->SetEclipsePower(0);

                    _player->CastSpell(_player, SPELL_DRUID_SOLAR_ECLIPSE, true, 0); // Cast Solar Eclipse
                    _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE, true, 0); // Cast Lunar Eclipse
                    _player->m_lastEclipseState = ECLIPSE_NONE;
                    _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                    _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                    _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);

                    if (_player->HasSpellCooldown(SPELL_DRUID_STARFALL))
                        _player->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_celestial_alignment_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_celestial_alignment_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_celestial_alignment_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->RemoveAura(SPELL_DRUID_SOLAR_ECLIPSE);
                    caster->RemoveAura(SPELL_DRUID_LUNAR_ECLIPSE);
                    caster->RemoveAura(SPELL_DRUID_NATURES_GRACE);
                    caster->RemoveAura(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_celestial_alignment_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_celestial_alignment_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_celestial_alignment_AuraScript();
        }
};

enum StampedingRoarSpells
{
    SPELL_DRUID_BEAR_FORM = 5487
};

// Stampeding Roar - 97993, Stampeding Roar (cat) - 77764 and Stampeding Roar(bear) - 77761
class spell_dru_stampeding_roar: public SpellScriptLoader
{
    public:
        spell_dru_stampeding_roar() : SpellScriptLoader("spell_dru_stampeding_roar") { }

        class spell_dru_stampeding_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_stampeding_roar_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetShapeshiftForm() != FORM_CAT && _player->GetShapeshiftForm() != FORM_BEAR)
                        _player->CastSpell(_player, SPELL_DRUID_BEAR_FORM, true);

                    if (Unit* target = GetHitUnit())
                        target->RemoveMovementImpairingAuras();
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_stampeding_roar_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_stampeding_roar_SpellScript();
        }
};

enum LacerateSpells
{
    SPELL_DRUID_MANGLE = 33917
};

// Lacerate - 33745
class spell_dru_lacerate: public SpellScriptLoader
{
    public:
        spell_dru_lacerate() : SpellScriptLoader("spell_dru_lacerate") { }

        class spell_dru_lacerate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_lacerate_SpellScript);

            void HandleOnHit()
            {
                // 25% chance to reset the cooldown on Mangle.
                if (Player* _player = GetCaster()->ToPlayer())
                    if (roll_chance_i(25) && _player->HasSpellCooldown(SPELL_DRUID_MANGLE))
                        _player->RemoveSpellCooldown(SPELL_DRUID_MANGLE, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_lacerate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_lacerate_SpellScript();
        }
};

// Faerie Fire - 770
class spell_dru_faerie_fire: public SpellScriptLoader
{
    public:
        spell_dru_faerie_fire() : SpellScriptLoader("spell_dru_faerie_fire") { }

        class spell_dru_faerie_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_faerie_fire_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (l_Player->GetShapeshiftForm() == FORM_BEAR)
                    {
                        if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                            l_Player->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                        
                        // Glyph of Fae Silence
                        if (l_Player->HasAura(SPELL_DRUID_GLYPH_OF_FAE_SILENCE))
                        {
                            if (Unit* l_Target = GetHitUnit())
                                l_Player->CastSpell(l_Target, SPELL_DRUID_FAE_SILENCE, true);
                            l_Player->ToPlayer()->AddSpellCooldown(GetSpellInfo()->Id, 0, 15 * IN_MILLISECONDS);
                        }
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_faerie_fire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_faerie_fire_SpellScript();
        }
};

// Teleport : Moonglade - 18960
class spell_dru_teleport_moonglade: public SpellScriptLoader
{
    public:
        spell_dru_teleport_moonglade() : SpellScriptLoader("spell_dru_teleport_moonglade") { }

        class spell_dru_teleport_moonglade_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_teleport_moonglade_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->TeleportTo(1, 7964.063f, -2491.099f, 487.83f, _player->GetOrientation());
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dru_teleport_moonglade_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_teleport_moonglade_SpellScript();
        }
};

enum ActivateCatFormSpells
{
    SPELL_DRUID_CAT_FORM = 768
};

// Prowl - 5215, Displacer Beast - 102280 and Dash - 1850
class spell_dru_activate_cat_form: public SpellScriptLoader
{
    public:
        spell_dru_activate_cat_form() : SpellScriptLoader("spell_dru_activate_cat_form") { }

        class spell_dru_activate_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_activate_cat_form_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_DRUID_CAT_FORM, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_activate_cat_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_activate_cat_form_SpellScript();
        }
};

enum EclipseSpells
{
    SPELL_DRUID_WRATH                    = 5176,
    SPELL_DRUID_STARFIRE                 = 2912,
    SPELL_DRUID_MOONFIRE                 = 8921,
    //SPELL_DRUID_NATURES_GRACE            = 16886,
    //SPELL_DRUID_STARFALL                 = 48505,
    //SPELL_DRUID_LUNAR_ECLIPSE            = 48518,
    SPELL_DRUID_STARSURGE                = 78674,
    SPELL_DRUID_EUPHORIA                 = 81062,
    //SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE = 81070,
    SPELL_DRUID_SHOOTING_STARS           = 93400,
    SPELL_DRUID_SUNFIRE                  = 93402,
    //SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE   = 107095,
    SPELL_DRUID_DREAM_OF_CENARIUS_TALENT = 108373,
    SPELL_DRUID_CELESTIAL_ALIGNMENT      = 112071,
    SPELL_DRUID_DREAM_OF_CENARIUS_RESTO  = 145153
};

// 5176 - Wrath, 2912 - Starfire and 78674 - Starsurge
class spell_dru_eclipse: public SpellScriptLoader
{
    public:
        spell_dru_eclipse() : SpellScriptLoader("spell_dru_eclipse") { }

        class spell_dru_eclipse_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_eclipse_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _plr = GetCaster()->ToPlayer())
                    if (GetSpellInfo()->Id == SPELL_DRUID_STARSURGE)
                        _plr->RemoveAura(SPELL_DRUID_SHOOTING_STARS);
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!_player->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT) && _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DRUID_BALANCE)
                        {
                            float modifier = 1.0f;

                            switch (GetSpellInfo()->Id)
                            {
                                case SPELL_DRUID_WRATH:
                                {
                                    int32 eclipse = 15 * modifier; // X Lunar energy

                                    _player->SetEclipsePower(int32(_player->GetEclipsePower() - eclipse));

                                    if (_player->GetEclipsePower() == -100 && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                    {
                                        _player->m_lastEclipseState = ECLIPSE_LUNAR;
                                        _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE, true, 0); // Cast Lunar Eclipse
                                        _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                                        _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                                        _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);

                                        if (_player->HasSpellCooldown(SPELL_DRUID_STARFALL))
                                            _player->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
                                    }

                                    // Your crits with Wrath also increase sunfire duration by 2s
                                    if (GetSpell()->IsCritForTarget(target))
                                    {
                                        if (AuraPtr aura = target->GetAura(SPELL_DRUID_SUNFIRE))
                                        {
                                            aura->SetDuration(aura->GetDuration() + 2 * IN_MILLISECONDS);
                                            if (aura->GetMaxDuration() < aura->GetDuration())
                                                aura->SetMaxDuration(aura->GetDuration());
                                        }
                                    }

                                    break;
                                }
                                case SPELL_DRUID_STARFIRE:
                                {
                                    int32 eclipse = 20 * modifier; // X Solar energy

                                    _player->SetEclipsePower(int32(_player->GetEclipsePower() + eclipse));

                                    if (_player->GetEclipsePower() == 100 && !_player->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
                                    {
                                        _player->m_lastEclipseState = ECLIPSE_SOLAR;
                                        _player->CastSpell(_player, SPELL_DRUID_SOLAR_ECLIPSE, true, 0); // Cast Solar Eclipse
                                        _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                                        _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                                    }

                                    // Your crits with Starfire also increase moonfire duration by 2s
                                    if (GetSpell()->IsCritForTarget(target))
                                    {
                                        if (AuraPtr aura = target->GetAura(SPELL_DRUID_MOONFIRE))
                                        {
                                            aura->SetDuration(aura->GetDuration() + 2 * IN_MILLISECONDS);
                                            if (aura->GetMaxDuration() < aura->GetDuration())
                                                aura->SetMaxDuration(aura->GetDuration());
                                        }
                                    }

                                    break;
                                }
                                case SPELL_DRUID_STARSURGE:
                                {
                                    int32 eclipse = 20 * modifier;

                                    switch (_player->m_lastEclipseState)
                                    {
                                        case ECLIPSE_NONE:
                                        case ECLIPSE_SOLAR:
                                            // If last eclipse is solar, set lunar power ...
                                            // .. or if no eclipse since in game, set lunar power
                                            _player->SetEclipsePower(int32(_player->GetEclipsePower() - eclipse));
                                            break;
                                        case ECLIPSE_LUNAR:
                                            // If last eclipse is lunar, set solar power
                                            _player->SetEclipsePower(int32(_player->GetEclipsePower() + eclipse));
                                            break;
                                        default:
                                            break;
                                    }

                                    if (_player->GetEclipsePower() == 100 && !_player->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
                                    {
                                        _player->m_lastEclipseState = ECLIPSE_SOLAR;
                                        _player->CastSpell(_player, SPELL_DRUID_SOLAR_ECLIPSE, true, 0); // Cast Solar Eclipse
                                        _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                                        _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                                    }
                                    else if (_player->GetEclipsePower() == -100 && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                    {
                                        _player->m_lastEclipseState = ECLIPSE_LUNAR;
                                        _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE, true, 0); // Cast Lunar Eclipse
                                        _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                                        _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                                        _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);

                                        if (_player->HasSpellCooldown(SPELL_DRUID_STARFALL))
                                            _player->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
                                    }

                                    // Your crits with Starsurge also increase sunfire and moonfire duration by 2s
                                    if (GetSpell()->IsCritForTarget(target))
                                    {
                                        if (AuraPtr aura = target->GetAura(SPELL_DRUID_SUNFIRE))
                                        {
                                            aura->SetDuration(aura->GetDuration() + 2 * IN_MILLISECONDS);
                                            if (aura->GetMaxDuration() < aura->GetDuration())
                                                aura->SetMaxDuration(aura->GetDuration());
                                        }

                                        if (AuraPtr aura = target->GetAura(SPELL_DRUID_MOONFIRE))
                                        {
                                            aura->SetDuration(aura->GetDuration() + 2 * IN_MILLISECONDS);
                                            if (aura->GetMaxDuration() < aura->GetDuration())
                                                aura->SetMaxDuration(aura->GetDuration());
                                        }
                                    }

                                    break;
                                }
                            }
                        }
                    }
                }
            }

            void HandleAfterHit()
            {
                if (Player* plr = GetCaster()->ToPlayer())
                {
                    if (plr->HasAura(SPELL_DRUID_DREAM_OF_CENARIUS_TALENT) && plr->GetSpecializationId(plr->GetActiveSpec()) == SPEC_DRUID_RESTORATION &&
                        GetSpellInfo()->Id == SPELL_DRUID_WRATH)
                    {
                        int32 bp = CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_1].BasePoints / 100);

                        if (Unit* target = plr->GetNextRandomRaidMember(15.0f))
                            plr->CastCustomSpell(target, SPELL_DRUID_DREAM_OF_CENARIUS_RESTO, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_eclipse_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_dru_eclipse_SpellScript::HandleAfterHit);
                AfterCast += SpellCastFn(spell_dru_eclipse_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_eclipse_SpellScript();
        }
};
// T10 Restoration 4P - 70691
class spell_dru_t10_restoration_4p_bonus: public SpellScriptLoader
{
    public:
        spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

        class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster()->ToPlayer()->GetGroup())
                {
                    targets.clear();
                    targets.push_back(GetCaster());
                }
                else
                {
                    targets.remove(GetExplTargetUnit());
                    std::list<Unit*> tempTargets;
                    for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        if ((*itr)->GetTypeId() == TYPEID_PLAYER && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
                            tempTargets.push_back((*itr)->ToUnit());

                    if (tempTargets.empty())
                    {
                        targets.clear();
                        FinishCast(SPELL_FAILED_DONT_REPORT);
                        return;
                    }

                    Unit* target = JadeCore::Containers::SelectRandomContainerElement(tempTargets);
                    targets.clear();
                    targets.push_back(target);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_t10_restoration_4p_bonus_SpellScript();
        }
};

enum StarfallSpells
{
    //SPELL_DRUID_MOONFIRE              = 8921,
    //SPELL_DRUID_STARFALL              = 48505,
    //SPELL_DRUID_SUNFIRE               = 93402,
    SPELL_DRUID_GLYPH_OF_GUIDED_STARS = 146655
};
// Starfall (triggered) - 50286
class spell_dru_starfall_dummy: public SpellScriptLoader
{
    public:
        spell_dru_starfall_dummy() : SpellScriptLoader("spell_dru_starfall_dummy") { }

        class spell_dru_starfall_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starfall_dummy_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Unit* target = GetHitUnit();
                if (!target)
                    return;

                // Shapeshifting into an animal form or mounting cancels the effect
                if (caster->GetCreatureType() == CREATURE_TYPE_BEAST || caster->IsMounted())
                {
                    if (SpellInfo const* spellInfo = GetTriggeringSpell())
                        caster->RemoveAurasDueToSpell(spellInfo->Id);
                    return;
                }

                // Any effect which causes you to lose control of your character will supress the starfall effect.
                if (caster->HasUnitState(UNIT_STATE_CONTROLLED))
                    return;

                // Glyph of Guided Stars - Starfall can only hit targets with Moonfire or Sunfire
                if (caster->HasAura(SPELL_DRUID_GLYPH_OF_GUIDED_STARS))
                    if (!target->HasAura(SPELL_DRUID_MOONFIRE) && !target->HasAura(SPELL_DRUID_SUNFIRE))
                        return;

                caster->CastSpell(target, uint32(GetEffectValue()), true);

                // Starfall can only launch 20 stars
                if (AuraEffectPtr starfall = caster->GetAuraEffect(SPELL_DRUID_STARFALL, EFFECT_1))
                {
                    int32 amount = starfall->GetAmount();
                    if (amount > 1)
                        starfall->SetAmount(amount - 1);
                    else
                        caster->RemoveAura(SPELL_DRUID_STARFALL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_starfall_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_starfall_dummy_SpellScript();
        }
};

enum SavageRoarSpells
{
    SPELL_DRUID_SAVAGE_ROAR = 52610,
    SPELL_DRUID_SAVAGE_ROAR_CAST = 62071
};

// Savage Roar - 52610
class spell_dru_savage_roar: public SpellScriptLoader
{
    public:
        spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

        class spell_dru_savage_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_roar_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster->GetShapeshiftForm() != FORM_CAT)
                    return SPELL_FAILED_ONLY_SHAPESHIFT;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
            }
        };

        class spell_dru_savage_roar_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_savage_roar_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SAVAGE_ROAR_CAST))
                    return false;
                return true;
            }

            void AfterApply(constAuraEffectPtr p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, SPELL_DRUID_SAVAGE_ROAR_CAST, true, NULL, p_AurEff, GetCasterGUID());
            }

            void AfterRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR_CAST);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_savage_roar_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_savage_roar_AuraScript();
        }
};

enum SurvivalInstinctSpells
{
    SPELL_DRUID_SURVIVAL_INSTINCTS = 50322
};

// Survival Instincts - 61336
class spell_dru_survival_instincts: public SpellScriptLoader
{
    public:
        spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

        class spell_dru_survival_instincts_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

            void AfterApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_DRUID_SURVIVAL_INSTINCTS, true);
            }

            void AfterRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->RemoveAurasDueToSpell(SPELL_DRUID_SURVIVAL_INSTINCTS);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_survival_instincts_AuraScript();
        }
};

// Swiftmend - 18562
class spell_dru_swiftmend: public SpellScriptLoader
{
    public:
        spell_dru_swiftmend() : SpellScriptLoader("spell_dru_swiftmend") { }

        class spell_dru_swiftmend_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_swiftmend_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    int32 heal = GetHitHeal() + CalculatePct(caster->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL), GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                    SetHitHeal(heal);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_swiftmend_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_swiftmend_SpellScript();
        }
};

enum DruidFormsSpells
{
    SPELL_DRUID_TRAVEL_FORM       = 783,
    SPELL_DRUID_AQUATIC_FORM      = 1066,
    SPELL_DRUID_FLIGHT_FORM       = 33943,
    SPELL_DRUID_SWIFT_FLIGHT_FORM = 40120,
    SPELL_DRUID_GLYPH_OF_THE_STAG = 114338,
    SPELL_DRUID_STAG_FORM         = 165961,

    ///< Extra spells
    SPELL_COLD_WEATHER_FLYING      = 54197,
    SPELL_MASTER_FLYING            = 90265,
    SPELL_FLIGHT_MASTERS_LICENSE   = 90267,
    SPELL_WISDOM_OF_THE_FOUR_WINDS = 115913
};

// Travel form - 783
class spell_dru_travel_form: public SpellScriptLoader
{
public:
    spell_dru_travel_form() : SpellScriptLoader("spell_dru_travel_form") { }

    class spell_dru_travel_form_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_travel_form_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetCaster()->GetTypeId() == TYPEID_PLAYER)
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (!l_Player->GetMap()->IsOutdoors(l_Player->GetPositionX(), l_Player->GetPositionY(), l_Player->GetPositionZ()))
                        return SPELL_FAILED_ONLY_OUTDOORS;

            return SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_travel_form_SpellScript::CheckCast);
        }
    };

    class spell_dru_travel_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_travel_form_AuraScript);

        void AfterApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            //Call the player script "spell_dru_travel_form_playerscript" below to avoid code duplication
            if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                sScriptMgr->OnPlayerUpdateMovement(GetTarget()->ToPlayer());
        }

        void AfterRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* l_Target = GetTarget();

            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_CANCEL)
                l_Target->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_travel_form_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_travel_form_SpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_travel_form_AuraScript();
    }
};

class spell_dru_travel_form_playerscript: public PlayerScript
{
    public:
        spell_dru_travel_form_playerscript() : PlayerScript("spell_dru_travel_form_playerscript") {}

        bool CheckIfCanFlyInLoc(Player* p_Player)
        {
            bool l_CanFly = false;

            ///< First check by map...
            uint32 l_Map = GetVirtualMapForMapAndZone(p_Player->GetMapId(), p_Player->GetZoneId());
            switch (l_Map)
            {
                case 530: ///< Outland
                    l_CanFly = true;
                    break;
                case 571: ///< Northrend
                {
                    if (p_Player->HasSpell(SPELL_COLD_WEATHER_FLYING))
                        l_CanFly = true;
                    break;
                }
                case 0:   ///< Eastern Kingdoms
                case 1:   ///< Kalimdor
                case 646: ///< Deepholm
                {
                    if (p_Player->HasSpell(SPELL_FLIGHT_MASTERS_LICENSE))
                        l_CanFly = true;
                    break;
                }
                case 870: ///< Pandaria
                {
                    if (p_Player->HasSpell(SPELL_WISDOM_OF_THE_FOUR_WINDS))
                        l_CanFly = true;
                    break;
                }
            }

            ///< ...and then by area
            AreaTableEntry const* l_Area = GetAreaEntryByAreaID(p_Player->GetAreaId());
            if (!l_Area || l_Area->Flags & AREA_FLAG_NO_FLY_ZONE)
                l_CanFly = false;

            return l_CanFly;
        }

        void OnPlayerUpdateMovement(Player* p_Player)
        {
            if (!p_Player || p_Player->getClass() != CLASS_DRUID || !p_Player->HasAura(SPELL_DRUID_TRAVEL_FORM))
                return;

            if (p_Player->IsInWater() && p_Player->GetShapeshiftForm() != FORM_AQUA)
            {
                p_Player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
                p_Player->CastSpell(p_Player, SPELL_DRUID_AQUATIC_FORM, true);
            }
            else if (!p_Player->IsInWater() && p_Player->getLevel() >= 71 && CheckIfCanFlyInLoc(p_Player) && !p_Player->isInCombat() &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT_EPIC && !p_Player->HasAura(SPELL_DRUID_GLYPH_OF_THE_STAG))
            {
                p_Player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
                p_Player->CastSpell(p_Player, SPELL_DRUID_SWIFT_FLIGHT_FORM, true);
            }
            else if (!p_Player->IsInWater() && p_Player->getLevel() >= 60 && CheckIfCanFlyInLoc(p_Player) && !p_Player->isInCombat() &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT_EPIC && p_Player->GetShapeshiftForm() != FORM_FLIGHT &&
                     !p_Player->HasAura(SPELL_DRUID_GLYPH_OF_THE_STAG))
            {
                p_Player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
                p_Player->CastSpell(p_Player, SPELL_DRUID_FLIGHT_FORM, true);
            }
            else if (!p_Player->IsInWater() && !p_Player->IsFlying() && p_Player->GetShapeshiftForm() != FORM_FLIGHT_EPIC && p_Player->GetShapeshiftForm() != FORM_FLIGHT && p_Player->GetShapeshiftForm() != FORM_STAG)
            {
                p_Player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
                p_Player->CastSpell(p_Player, SPELL_DRUID_STAG_FORM, true);
            }
        }

        void OnChangeShapeshift(Player* p_Player, ShapeshiftForm p_Form)
        {
            if (!p_Player || p_Player->getClass() != CLASS_DRUID)
                return;

            if (p_Player->HasAura(SPELL_DRUID_TRAVEL_FORM) && !p_Player->IsTravelForm(p_Form))
                p_Player->RemoveAura(SPELL_DRUID_TRAVEL_FORM);
        }
};

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive: public SpellScriptLoader
{
public:
    spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

    class spell_dru_swift_flight_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (l_Caster->HasAura(SPELL_MASTER_FLYING))
                    amount = 310; // 310% instead of 280% by default
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_swift_flight_passive_AuraScript();
    }
};

// 114338 - Glyph of the Stag
class spell_dru_glyph_of_the_stag: public SpellScriptLoader
{
public:
    spell_dru_glyph_of_the_stag() : SpellScriptLoader("spell_dru_glyph_of_the_stag") { }

    class spell_dru_glyph_of_the_stag_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_the_stag_AuraScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* l_Target = GetTarget();
            if (!l_Target)
                return;

            Player* l_Player = l_Target->ToPlayer();
            if (!l_Player)
                return;

            if (l_Player->getLevel() >= 71 && !l_Player->HasSpell(SPELL_DRUID_SWIFT_FLIGHT_FORM))
                l_Player->learnSpell(SPELL_DRUID_SWIFT_FLIGHT_FORM, false);
            else if (l_Player->getLevel() >= 60 && !l_Player->HasSpell(SPELL_DRUID_FLIGHT_FORM))
                l_Player->learnSpell(SPELL_DRUID_FLIGHT_FORM, false);
        }

        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* l_Target = GetTarget();
            if (!l_Target)
                return;

            Player* l_Player = l_Target->ToPlayer();
            if (!l_Player)
                return;

            if (l_Player->getLevel() >= 71 && l_Player->HasSpell(SPELL_DRUID_SWIFT_FLIGHT_FORM))
                l_Player->removeSpell(SPELL_DRUID_SWIFT_FLIGHT_FORM, false, false);
            else if (l_Player->getLevel() >= 60 && l_Player->HasSpell(SPELL_DRUID_FLIGHT_FORM))
                l_Player->removeSpell(SPELL_DRUID_FLIGHT_FORM, false, false);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_the_stag_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_the_stag_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_glyph_of_the_stag_AuraScript();
    }
};

enum SpellsRake
{
    SPELL_DRU_RAKE_STUNT = 163505,
    SPELL_DRU_GLYPH_OF_SAVAGE_ROAR = 127540
};

// Rake - 1822
class spell_dru_rake: public SpellScriptLoader
{
public:
    spell_dru_rake() : SpellScriptLoader("spell_dru_rake") { }

    class spell_dru_rake_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_rake_SpellScript);

        bool m_isStealthed = false;

        void HandleOnPrepare()
        {
            if (Unit* l_Caster = GetCaster())
                m_isStealthed = l_Caster->HasStealthAura();
        }

        void HandleOnHit()
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();

            if (l_Target &&  l_Caster && m_isStealthed)
            {
                l_Caster->CastSpell(l_Target, SPELL_DRU_RAKE_STUNT, true);

                if (constAuraEffectPtr l_GlyphOfSavageRoar = l_Caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_SAVAGE_ROAR, EFFECT_0))
                {
                        if (Player* l_Player = l_Caster->ToPlayer())
                        {
                            uint8 l_ComboPointsBefore = l_Player->GetComboPoints();
                            l_Player->AddComboPoints(l_GlyphOfSavageRoar->GetAmount(), nullptr);

                            l_Caster->CastSpell(l_Target, SPELL_DRUID_SAVAGE_ROAR, true);

                            l_Player->ClearComboPoints();
                            l_Player->AddComboPoints(l_ComboPointsBefore, nullptr);
                        }
                }
            }

        }

        void Register()
        {
            OnPrepare += SpellOnPrepareFn(spell_dru_rake_SpellScript::HandleOnPrepare);
            OnHit += SpellHitFn(spell_dru_rake_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_rake_SpellScript();
    }
};

enum SpellsShred
{
    SPELL_DRUID_PROWL = 5215,
    SPELL_DRUID_SWIPE = 106785
};

// Shred - 5221
class spell_dru_shred: public SpellScriptLoader
{
public:
    spell_dru_shred() : SpellScriptLoader("spell_dru_shred") { }

    class spell_dru_shred_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_shred_SpellScript);

        bool m_isStealthed = false;

        void HandleOnPrepare()
        {
            m_isStealthed = GetCaster()->HasStealthAura();
        }

        void HandleOnHit()
        {
            int32 l_Damage = GetHitDamage();
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();

            if (m_isStealthed)
            {
                if (sSpellMgr->GetSpellInfo(SPELL_DRUID_PROWL))
                    l_Damage += CalculatePct(l_Damage, sSpellMgr->GetSpellInfo(SPELL_DRUID_PROWL)->Effects[EFFECT_3].BasePoints);

                if (l_Target)
                {
                    if (constAuraEffectPtr l_GlyphOfSavageRoar = l_Caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_SAVAGE_ROAR, EFFECT_0))
                    {
                        if (l_Caster->GetTypeId() == TYPEID_PLAYER)
                        {
                            uint8 l_ComboPointsBefore = l_Caster->ToPlayer()->GetComboPoints();
                            l_Caster->ToPlayer()->AddComboPoints(l_GlyphOfSavageRoar->GetAmount(), nullptr);

                            l_Caster->CastSpell(l_Target, SPELL_DRUID_SAVAGE_ROAR, true);

                            l_Caster->ToPlayer()->ClearComboPoints();
                            l_Caster->ToPlayer()->AddComboPoints(l_ComboPointsBefore, nullptr);
                        }
                    }
                }
            }

            if (l_Target && l_Target->HasAuraState(AURA_STATE_BLEEDING) && sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE))
                l_Damage += CalculatePct(l_Damage, sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE)->Effects[EFFECT_1].BasePoints);

            SetHitDamage(l_Damage);
        }

        void Register()
        {
            OnPrepare += SpellOnPrepareFn(spell_dru_shred_SpellScript::HandleOnPrepare);
            OnHit += SpellHitFn(spell_dru_shred_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_shred_SpellScript();
    }
};

enum SpellsFerociousBite
{
    SPELL_DRUID_RAKE = 1822,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE = 67598,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL = 101024
};

// Ferocious Bite - 22568
class spell_dru_ferocious_bite: public SpellScriptLoader
{
public:
    spell_dru_ferocious_bite() : SpellScriptLoader("spell_dru_ferocious_bite") { }

    class spell_dru_ferocious_bite_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_ferocious_bite_SpellScript);

        void HandleOnHit()
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();
            int32 l_Damage = GetHitDamage();

            if (l_Caster->GetTypeId() == TYPEID_PLAYER)
                l_Damage = (l_Damage / 5) * l_Caster->ToPlayer()->GetComboPoints();

            // converts each extra point of energy ( up to 25 energy ) into additional damage
            int32 l_EnergyConsumed = -l_Caster->ModifyPower(POWER_ENERGY, -GetSpellInfo()->Effects[EFFECT_1].BasePoints);
            // 25 energy = 100% more damage
            AddPct(l_Damage, l_EnergyConsumed * 4);

            SetHitDamage(l_Damage);

            // Glyph of Ferocious Bite
            if (AuraPtr l_GlyphOfFerociousBite = l_Caster->GetAura(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE))
            {
                int32 l_HealPct = (l_GlyphOfFerociousBite->GetEffect(EFFECT_0)->GetAmount() / 10) * (l_EnergyConsumed / 10);
                l_Caster->CastCustomSpell(l_Caster, SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL, &l_HealPct, 0, 0, true);
            }

            // if target is under 25% of life, also reset rake duration
            if (l_Target && l_Target->GetHealthPct() <= 25.0f)
                if (AuraPtr l_Rake = l_Target->GetAura(SPELL_DRUID_RAKE))
                    l_Rake->RefreshDuration();
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_dru_ferocious_bite_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_ferocious_bite_SpellScript();
    }
};

// Frenzied Regeneration - 22842
class spell_dru_frenzied_regeneration: public SpellScriptLoader
{
public:
    spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration") { }

    class spell_dru_frenzied_regeneration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_frenzied_regeneration_SpellScript)

        SpellCastResult CheckCast()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster->GetPower(POWER_RAGE) < GetSpellInfo()->Effects[EFFECT_1].BasePoints * l_Caster->GetPowerCoeff(POWER_RAGE))
                return SPELL_FAILED_NO_POWER;

            return SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->ModifyPower(POWER_RAGE, -GetEffectValue() * GetCaster()->GetPowerCoeff(POWER_RAGE));
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_frenzied_regeneration_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_dru_frenzied_regeneration_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_frenzied_regeneration_SpellScript();
    }
};

// Rip - 1079
class spell_dru_rip: public SpellScriptLoader
{
public:
    spell_dru_rip() : SpellScriptLoader("spell_dru_rip") { }

    class spell_dru_rip_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rip_AuraScript);

        void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
        {
            Unit* l_Caster = GetCaster();

            if (l_Caster && l_Caster->GetTypeId() == TYPEID_PLAYER && GetEffect(EFFECT_0))
                p_Amount = (p_Amount * l_Caster->ToPlayer()->GetComboPoints() * 8) / (GetMaxDuration() / GetEffect(EFFECT_0)->GetAmplitude());
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rip_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_rip_AuraScript();
    }
};

enum DreamOfCenariusSpells
{
    //SPELL_DRUID_MANGLE = 33917
    SPELL_DRUID_DREAM_OF_CENARIUS_PROC = 145162
};

// Dream of Cenarius - 158501
class spell_dru_dream_of_cenarius: public SpellScriptLoader
{
public:
    spell_dru_dream_of_cenarius() : SpellScriptLoader("spell_dru_dream_of_cenarius") { }

    class spell_dru_dream_of_cenarius_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_dream_of_cenarius_AuraScript);

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

            if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_DRUID_MANGLE)
                return;

            if (!(p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                return;

            l_Caster->CastSpell(l_Caster, SPELL_DRUID_DREAM_OF_CENARIUS_PROC, true);
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_dream_of_cenarius_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_dream_of_cenarius_AuraScript();
    }
};

enum PrimalFurySpells
{
    SPELL_DRUID_PRIMAL_FURY_CP_PROC = 16953,
    SPELL_DRUID_PRIMAL_FURY_RAGE = 16961,
    SPELL_DRUID_PRIMAL_FURY_RAGE_PROC = 16959,
    SPELL_DRUID_PRIMAL_FURY_CP = 159286
};

// Primal Fury - 16961 159286
class spell_dru_primal_fury: public SpellScriptLoader
{
public:
    spell_dru_primal_fury() : SpellScriptLoader("spell_dru_primal_fury") { }

    class spell_dru_primal_fury_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_primal_fury_AuraScript);

        void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
        {
            PreventDefaultAction();

            Unit* l_Caster = GetCaster();
            if (!l_Caster)
                return;

            if (p_EventInfo.GetActor()->GetGUID() != l_Caster->GetGUID())
                return;

            if (!p_EventInfo.GetDamageInfo()->GetSpellInfo() || !GetSpellInfo())
                return;

            if (!(p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                return;

            if (GetSpellInfo()->Id == SPELL_DRUID_PRIMAL_FURY_RAGE && l_Caster->GetShapeshiftForm() == FORM_BEAR)
                l_Caster->CastSpell(l_Caster, SPELL_DRUID_PRIMAL_FURY_RAGE_PROC, true);
            else if (GetSpellInfo()->Id == SPELL_DRUID_PRIMAL_FURY_CP && l_Caster->GetShapeshiftForm() == FORM_CAT &&
                     p_EventInfo.GetDamageInfo()->GetSpellInfo()->HasEffect(SPELL_EFFECT_ADD_COMBO_POINTS))
                    l_Caster->CastSpell(l_Caster, SPELL_DRUID_PRIMAL_FURY_CP_PROC, true);
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_primal_fury_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_primal_fury_AuraScript();
    }
};

enum HealingTouchSpells
{
    SPELL_DRU_BLOODTALONS_MOD_DAMAGE = 145152,
    SPELL_DRU_BLOODTALONS_TALENT = 155672
};

// Healing Touch - 5185
class spell_dru_healing_touch: public SpellScriptLoader
{
public:
    spell_dru_healing_touch() : SpellScriptLoader("spell_dru_healing_touch") { }

    class spell_dru_healing_touch_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_healing_touch_SpellScript);

        void HandleOnCast()
        {
            Unit* l_Caster = GetCaster();
            if (!l_Caster)
                return;

            if (l_Caster->HasAura(SPELL_DRU_BLOODTALONS_TALENT))
                l_Caster->CastSpell(l_Caster, SPELL_DRU_BLOODTALONS_MOD_DAMAGE, true);
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_dru_healing_touch_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_healing_touch_SpellScript();
    }
};

void AddSC_druid_spell_scripts()
{
    new spell_dru_yseras_gift();
    new spell_dru_tooth_and_claw_absorb();
    new spell_dru_genesis();
    new spell_dru_glyph_of_the_treant();
    new spell_dru_incarnation_chosen_of_elune();
    new spell_dru_incarnation_skins();
    new spell_dru_item_pvp_feral_4p();
    new spell_dru_wild_charge_moonkin();
    new spell_dru_thrash_bear();
    new spell_dru_swipe();
    new spell_dru_maul();
    new spell_dru_moonfire();
    new spell_dru_natures_vigil();
    new spell_dru_ursols_vortex_snare();
    new spell_dru_ursols_vortex();
    new spell_dru_solar_beam();
    new spell_dru_dash();
    new spell_dru_savage_defense();
    new spell_dru_lifebloom();
    new spell_dru_killer_instinct();
    new spell_dru_lifebloom_refresh();
    new spell_dru_natures_cure();
    new spell_dru_cat_form();
    new spell_dru_skull_bash();
    new spell_dru_faerie_swarm();
    new spell_dru_wild_mushroom();
    new spell_dru_celestial_alignment();
    new spell_dru_stampeding_roar();
    new spell_dru_lacerate();
    new spell_dru_faerie_fire();
    new spell_dru_teleport_moonglade();
    new spell_dru_activate_cat_form();
    new spell_dru_eclipse();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_starfall_dummy();
    new spell_dru_savage_roar();
    new spell_dru_survival_instincts();
    new spell_dru_swiftmend();
    new spell_dru_travel_form();
    new spell_dru_travel_form_playerscript();
    new spell_dru_swift_flight_passive();
    new spell_dru_glyph_of_the_stag();
    new spell_dru_rake();
    new spell_dru_shred();
    new spell_dru_ferocious_bite();
    new spell_dru_frenzied_regeneration();
    new spell_dru_rip();
    new spell_dru_dream_of_cenarius();
    new spell_dru_primal_fury();
    new spell_dru_healing_touch();
}
