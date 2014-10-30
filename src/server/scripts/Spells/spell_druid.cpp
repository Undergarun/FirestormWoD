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
class spell_dru_yseras_gift : public SpellScriptLoader
{
    public:
        spell_dru_yseras_gift() : SpellScriptLoader("spell_dru_yseras_gift") { }

        class spell_dru_yseras_gift_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_yseras_gift_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->IsFullHealth())
                        caster->CastSpell(caster, SPELL_DRUID_YSERAS_GIFT_HEAL_CASTER, true);
                    else
                    {
                        std::list<Unit*> party;
                        caster->GetPartyMembers(party);

                        if (party.empty())
                            return;

                        std::list<Unit*> tempList;
                        for (auto itr : party)
                        {
                            if (itr->IsFullHealth() || itr->GetDistance(caster) >= 40.0f)
                                continue;

                            tempList.push_back(itr);
                        }

                        if (tempList.empty())
                            return;

                        tempList.sort(JadeCore::HealthPctOrderPred());
                        caster->CastSpell(tempList.front(), SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY, true);
                    }
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
class spell_dru_tooth_and_claw_absorb : public SpellScriptLoader
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
class spell_dru_genesis : public SpellScriptLoader
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
class spell_dru_glyph_of_the_treant : public SpellScriptLoader
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
class spell_dru_incarnation_chosen_of_elune : public SpellScriptLoader
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
class spell_dru_incarnation_skins : public SpellScriptLoader
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
class spell_dru_item_pvp_feral_4p : public SpellScriptLoader
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
class spell_dru_wild_charge_moonkin : public SpellScriptLoader
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
class spell_dru_thrash_bear : public SpellScriptLoader
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
                    caster->SetPower(POWER_RAGE, caster->GetPower(POWER_RAGE) + 1 * 10);
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
class spell_dru_swipe : public SpellScriptLoader
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
                            plr->AddComboPoints(plr, GetSpellInfo()->Effects[EFFECT_0].BasePoints);

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
class spell_dru_maul : public SpellScriptLoader
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
                            int32 bp = caster->GetTotalAttackPowerValue(BASE_ATTACK) * 2.4f;

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
class spell_dru_moonfire : public SpellScriptLoader
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
class spell_dru_moonfire_damage : public SpellScriptLoader
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
class spell_dru_natures_vigil : public SpellScriptLoader
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
class spell_dru_ursols_vortex : public SpellScriptLoader
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
class spell_dru_ursols_vortex_snare : public SpellScriptLoader
{
    public:
        spell_dru_ursols_vortex_snare() : SpellScriptLoader("spell_dru_ursols_vortex_snare") { }

        class spell_dru_ursols_vortex_snare_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursols_vortex_snare_AuraScript);

            std::list<Unit*> targetList;

            void OnUpdate(uint32 diff, AuraEffectPtr aurEff)
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
class spell_dru_solar_beam : public SpellScriptLoader
{
    public:
        spell_dru_solar_beam() : SpellScriptLoader("spell_dru_solar_beam") { }

        class spell_dru_solar_beam_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_solar_beam_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
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
class spell_dru_dash : public SpellScriptLoader
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
class spell_dru_savage_defense : public SpellScriptLoader
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
class spell_dru_lifebloom_refresh : public SpellScriptLoader
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

// Lifebloom - 33763 : Final heal
class spell_dru_lifebloom : public SpellScriptLoader
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

                if (!GetCaster())
                    return;

                if (GetCaster()->ToPlayer()->HasSpellCooldown(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL))
                    return;

                // final heal
                int32 healAmount = aurEff->GetAmount();

                if (Player* _plr = GetCaster()->ToPlayer())
                {
                    healAmount = _plr->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), healAmount, aurEff->GetEffIndex(), HEAL);
                    healAmount = GetTarget()->SpellHealingBonusTaken(_plr, GetSpellInfo(), healAmount, HEAL);

                    // Increase final heal by 50%
                    if (_plr->HasAura(SPELL_DRUID_GLYPH_OF_BLOOMING))
                        AddPct(healAmount, 50);

                    GetTarget()->CastCustomSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());

                    _plr->AddSpellCooldown(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, 0, time(NULL) + 1);

                    return;
                }

                // Increase final heal by 50%
                if (GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_BLOOMING))
                    AddPct(healAmount, 50);

                GetTarget()->CastCustomSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
                GetCaster()->ToPlayer()->AddSpellCooldown(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, 0, time(NULL) + 1);
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* target = GetUnitOwner())
                {
                    if (constAuraEffectPtr aurEff = GetEffect(EFFECT_1))
                    {
                        // final heal
                        int32 healAmount = aurEff->GetAmount();

                        if (Unit* caster = GetCaster())
                        {
                            healAmount = caster->SpellHealingBonusDone(target, GetSpellInfo(), healAmount, aurEff->GetEffIndex(), HEAL, dispelInfo->GetRemovedCharges());
                            healAmount = target->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, dispelInfo->GetRemovedCharges());

                            target->CastCustomSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, NULLAURA_EFFECT, GetCasterGUID());

                            return;
                        }
                        target->CastCustomSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, NULLAURA_EFFECT, GetCasterGUID());
                    }
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
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
class spell_dru_killer_instinct : public SpellScriptLoader
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
class spell_dru_natures_cure : public SpellScriptLoader
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
class spell_dru_cat_form : public SpellScriptLoader
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
class spell_dru_skull_bash : public SpellScriptLoader
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
    SPELL_DRUID_FAERIE_DECREASE_SPEED       = 102354
};

// Faerie Swarm - 102355
class spell_dru_faerie_swarm : public SpellScriptLoader
{
    public:
        spell_dru_faerie_swarm() : SpellScriptLoader("spell_dru_faerie_swarm") { }

        class spell_dru_faerie_swarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_faerie_swarm_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_DRUID_FAERIE_DECREASE_SPEED, true);
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
    SPELL_DRUID_WILD_MUSHROOM_DAMAGE       = 78777,
    SPELL_DRUID_SWIFTMEND                  = 81262,
    SPELL_DRUID_FUNGAL_GROWTH_SUMMON       = 81283,
    SPELL_DRUID_WILD_MUSHROOM_DEATH_VISUAL = 92701,
    SPELL_DRUID_WILD_MUSHROOM_SUICIDE      = 92853,
    SPELL_DRUID_MUSHROOM_BIRTH_VISUAL      = 94081,
    SPELL_DRUID_WILD_MUSHROOM_HEAL         = 102792,
    SPELL_DRUID_WILD_MUSHROOM_GROWING      = 138611,
    SPELL_DRUID_WILD_MUSHROOM_MOD_SCALE    = 138616,
    SPELL_DRUID_GLYPH_OF_EFFLORESCENCE     = 145529
};

// Wild Mushroom (Heal effect with growing) - 102792
class spell_dru_wild_mushroom_heal : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_heal() : SpellScriptLoader("spell_dru_wild_mushroom_heal") { }

        class spell_dru_wild_mushroom_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_heal_SpellScript);

            uint8 count;

            bool Load()
            {
                count = 0;
                return true;
            }

            void HandleTargets(std::list<WorldObject*>& targets)
            {
                count = targets.size();
            }

            void HandleHeal()
            {
                if (!count)
                    return;

                if (Unit* mushroom = GetCaster())
                {
                    if (AuraEffectPtr growing = mushroom->GetAuraEffect(SPELL_DRUID_WILD_MUSHROOM_MOD_SCALE, EFFECT_1))
                    {
                        int32 bonus = growing->GetAmount() / count;
                        SetHitHeal(GetHitHeal() + bonus);
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_mushroom_heal_SpellScript::HandleTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                OnHit += SpellHitFn(spell_dru_wild_mushroom_heal_SpellScript::HandleHeal);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_heal_SpellScript();
        }
};

// Wild Mushroom (Growing effect) - 138611
class spell_dru_wild_mushroom_growing : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_growing() : SpellScriptLoader("spell_dru_wild_mushroom_growing") { }

        class spell_dru_wild_mushroom_growing_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_wild_mushroom_growing_AuraScript);

            uint32 currAmount;

            bool Load()
            {
                currAmount = 0;
                return true;
            }

            void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32 &amount, bool & /*canBeRecalculated*/)
            {
                // Max amount : 200% of caster's health
                amount = GetUnitOwner()->CountPctFromMaxHealth(amount);
            }

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                Unit* target = eventInfo.GetActor();
                if (!target)
                    return;

                uint32 overHeal = eventInfo.GetHealInfo()->GetHeal();
                uint32 maxAmount = aurEff->GetAmount();
                currAmount += overHeal;
                currAmount = std::min(currAmount, maxAmount);

                int32 newPct = float(currAmount) / float(maxAmount) * 100.0f;
                int32 bp2 = currAmount;

                std::list<Creature*> tempList;
                std::list<Creature*> mushroomlist;

                target->GetCreatureListWithEntryInGrid(tempList, DRUID_NPC_WILD_MUSHROOM, 500.0f);

                mushroomlist = tempList;

                // Remove other players mushrooms
                for (std::list<Creature*>::iterator i = tempList.begin(); i != tempList.end(); ++i)
                {
                    Unit* owner = (*i)->GetOwner();
                    if (owner && owner == target && (*i)->isSummon())
                        continue;

                    mushroomlist.remove((*i));
                }

                if (mushroomlist.empty() || mushroomlist.size() > 1)
                    return;

                Creature* mushroom = mushroomlist.back();
                mushroom->RemoveAura(SPELL_DRUID_WILD_MUSHROOM_MOD_SCALE);
                target->CastCustomSpell(mushroom, SPELL_DRUID_WILD_MUSHROOM_MOD_SCALE, &newPct, &bp2, NULL, true);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_wild_mushroom_growing_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_DUMMY);
                OnEffectProc += AuraEffectProcFn(spell_dru_wild_mushroom_growing_AuraScript::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_wild_mushroom_growing_AuraScript();
        }
};

// Wild Mushroom (Restoration) - 145205
class spell_dru_wild_mushroom_resto : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_resto() : SpellScriptLoader("spell_dru_wild_mushroom_resto") { }

        class spell_dru_wild_mushroom_resto_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_resto_SpellScript)

            void HandleSummon(SpellEffIndex effIndex)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    PreventHitDefaultEffect(effIndex);

                    const SpellInfo* spell = GetSpellInfo();
                    std::list<Creature*> tempList;
                    std::list<Creature*> mushroomlist;

                    player->GetCreatureListWithEntryInGrid(tempList, DRUID_NPC_WILD_MUSHROOM, 500.0f);

                    mushroomlist = tempList;

                    // Remove other players mushrooms
                    for (std::list<Creature*>::iterator i = tempList.begin(); i != tempList.end(); ++i)
                    {
                        Unit* owner = (*i)->GetOwner();
                        if (owner && owner == player && (*i)->isSummon())
                            continue;

                        mushroomlist.remove((*i));
                    }

                    // 1 mushrooms max
                    if ((int32)mushroomlist.size() >= spell->Effects[effIndex].BasePoints)
                    {
                        Creature* mushroom = mushroomlist.back();

                        // Recasting Wild Mushroom will move the Mushroom without losing this accumulated healing.
                        if (WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest()))
                        {
                            mushroom->NearTeleportTo(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), mushroom->GetOrientation());

                            if (player->HasAura(SPELL_DRUID_GLYPH_OF_EFFLORESCENCE))
                            {
                                mushroom->RemoveDynObject(SPELL_DRUID_SWIFTMEND);
                                mushroom->RemoveAura(SPELL_DRUID_SWIFTMEND);
                            }

                            return;
                        }
                    }

                    Position pos;
                    GetExplTargetDest()->GetPosition(&pos);
                    const SummonPropertiesEntry* properties = sSummonPropertiesStore.LookupEntry(spell->Effects[effIndex].MiscValueB);
                    TempSummon* summon = player->SummonCreature(spell->Effects[effIndex].MiscValue, pos, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, spell->GetDuration());
                    if (!summon)
                        return;

                    summon->SetGuidValue(UNIT_FIELD_SUMMONED_BY, player->GetGUID());
                    summon->setFaction(player->getFaction());
                    summon->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, GetSpellInfo()->Id);
                    summon->SetMaxHealth(5);
                    summon->SetFullHealth();
                    summon->CastSpell(summon, SPELL_DRUID_MUSHROOM_BIRTH_VISUAL, true); // Wild Mushroom : Detonate Birth Visual
                    player->CastSpell(player, SPELL_DRUID_WILD_MUSHROOM_GROWING, true);

                    if (player->HasAura(SPELL_DRUID_GLYPH_OF_EFFLORESCENCE))
                        summon->CastSpell(summon, SPELL_DRUID_SWIFTMEND, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_dru_wild_mushroom_resto_SpellScript::HandleSummon, EFFECT_1, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_resto_SpellScript();
        }
};

// Wild Mushroom - 88747
class spell_dru_wild_mushroom : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom() : SpellScriptLoader("spell_dru_wild_mushroom") { }

        class spell_dru_wild_mushroom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_SpellScript)

            void HandleSummon(SpellEffIndex effIndex)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    PreventHitDefaultEffect(effIndex);

                    const SpellInfo* spell = GetSpellInfo();
                    std::list<Creature*> tempList;
                    std::list<Creature*> mushroomlist;

                    player->GetCreatureListWithEntryInGrid(tempList, DRUID_NPC_WILD_MUSHROOM, 500.0f);

                    mushroomlist = tempList;

                    // Remove other players mushrooms
                    for (std::list<Creature*>::iterator i = tempList.begin(); i != tempList.end(); ++i)
                    {
                        Unit* owner = (*i)->GetOwner();
                        if (owner && owner == player && (*i)->isSummon())
                            continue;

                        mushroomlist.remove((*i));
                    }

                    // 3 mushrooms max
                    if ((int32)mushroomlist.size() >= spell->Effects[effIndex].BasePoints)
                        mushroomlist.back()->ToTempSummon()->UnSummon();

                    Position pos;
                    GetExplTargetDest()->GetPosition(&pos);
                    const SummonPropertiesEntry* properties = sSummonPropertiesStore.LookupEntry(spell->Effects[effIndex].MiscValueB);
                    TempSummon* summon = player->SummonCreature(spell->Effects[effIndex].MiscValue, pos, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, spell->GetDuration());
                    if (!summon)
                        return;

                    summon->SetGuidValue(UNIT_FIELD_SUMMONED_BY, player->GetGUID());
                    summon->setFaction(player->getFaction());
                    summon->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, GetSpellInfo()->Id);
                    summon->SetMaxHealth(5);
                    summon->SetFullHealth();
                    summon->CastSpell(summon, SPELL_DRUID_MUSHROOM_BIRTH_VISUAL, true); // Wild Mushroom : Detonate Birth Visual
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

// Wild Mushroom : Detonate - 88751
class spell_dru_wild_mushroom_detonate : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_detonate() : SpellScriptLoader("spell_dru_wild_mushroom_detonate") { }

        class spell_dru_wild_mushroom_detonate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_detonate_SpellScript)

            // Globals variables
            float spellRange;
            std::list<uint64> mushroomList;

            bool Load()
            {
                spellRange = GetSpellInfo()->GetMaxRange(true);

                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return false;

                std::list<Creature*> list;
                player->GetCreatureListWithEntryInGrid(list, DRUID_NPC_WILD_MUSHROOM, 50.0f);

                for (std::list<Creature*>::const_iterator i = list.begin(); i != list.end(); ++i)
                {
                    Unit* owner = (*i)->GetOwner();
                    if (owner && owner == player && (*i)->isSummon())
                    {
                        mushroomList.push_back((*i)->GetGUID());
                        continue;
                    }
                }

                if (!spellRange)
                    return false;

                return true;
            }

            SpellCastResult CheckCast()
            {
                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return SPELL_FAILED_CASTER_DEAD;

                if (mushroomList.empty())
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                bool inRange = false;

                for (std::list<uint64>::const_iterator i = mushroomList.begin(); i != mushroomList.end(); ++i)
                {
                    Creature* mushroom = player->GetMap()->GetCreature(*i);
                    if (!mushroom)
                        continue;
                    Position shroomPos;
                    mushroom->GetPosition(&shroomPos);
                    if (player->IsWithinDist3d(&shroomPos, spellRange)) // Must have at least one mushroom within 40 yards
                    {
                        inRange = true;
                        break;
                    }
                }

                if (!inRange)
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    for (std::list<uint64>::const_iterator i = mushroomList.begin(); i != mushroomList.end(); ++i)
                    {
                        Creature* mushroom = player->GetMap()->GetCreature(*i);
                        if (!mushroom)
                            continue;
                        Position shroomPos;
                        mushroom->GetPosition(&shroomPos);
                        if (!player->IsWithinDist3d(&shroomPos, spellRange))
                            continue;

                        mushroom->SetVisible(true);

                        player->CastSpell(mushroom, SPELL_DRUID_WILD_MUSHROOM_DAMAGE, true);    // Damage
                        player->CastSpell(mushroom, SPELL_DRUID_FUNGAL_GROWTH_SUMMON, true);    // Fungal Growth

                        mushroom->CastSpell(mushroom, SPELL_DRUID_WILD_MUSHROOM_DEATH_VISUAL, true);// Explosion visual
                        mushroom->CastSpell(mushroom, SPELL_DRUID_WILD_MUSHROOM_SUICIDE, true);     // Suicide
                        mushroom->DespawnOrUnsummon(500);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_detonate_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_wild_mushroom_detonate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_detonate_SpellScript();
        }
};

// Wild Mushroom : Bloom - 102791
class spell_dru_wild_mushroom_bloom : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_bloom() : SpellScriptLoader("spell_dru_wild_mushroom_bloom") { }

        class spell_dru_wild_mushroom_bloom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_bloom_SpellScript)

            // Globals variables
            float spellRange;
            std::list<uint64> mushroomList;

            bool Load()
            {
                spellRange = GetSpellInfo()->GetMaxRange(true);

                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return false;

                std::list<Creature*> list;
                std::list<uint64> summonList;
                player->GetCreatureListWithEntryInGrid(list, DRUID_NPC_WILD_MUSHROOM, 500.0f);

                for (std::list<Creature*>::const_iterator i = list.begin(); i != list.end(); ++i)
                {
                    Unit* owner = (*i)->GetOwner();
                    if (owner && owner == player && (*i)->isSummon())
                    {
                        summonList.push_back((*i)->GetGUID());
                        continue;
                    }
                }
                mushroomList = summonList;

                if (!spellRange)
                    return false;

                return true;
            }

            SpellCastResult CheckCast()
            {
                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return SPELL_FAILED_CASTER_DEAD;

                if (mushroomList.empty())
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                bool inRange = false;

                for (std::list<uint64>::const_iterator i = mushroomList.begin(); i != mushroomList.end(); ++i)
                {
                    Creature* mushroom = player->GetMap()->GetCreature(*i);
                    if (!mushroom)
                        continue;
                    Position shroomPos;
                    mushroom->GetPosition(&shroomPos);
                    if (player->IsWithinDist3d(&shroomPos, spellRange)) // Must have at least one mushroom within 40 yards
                    {
                        inRange = true;
                        break;
                    }
                }

                if (!inRange)
                    return SPELL_FAILED_CUSTOM_ERROR;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    for (std::list<uint64>::const_iterator i = mushroomList.begin(); i != mushroomList.end(); ++i)
                    {
                        Creature* mushroom = player->GetMap()->GetCreature(*i);
                        if (!mushroom)
                            continue;
                        Position shroomPos;
                        mushroom->GetPosition(&shroomPos);
                        if (!player->IsWithinDist3d(&shroomPos, spellRange))
                            continue;

                        mushroom->CastSpell(mushroom, SPELL_DRUID_WILD_MUSHROOM_SUICIDE, true); // Explosion visual and suicide
                        mushroom->CastSpell(mushroom, SPELL_DRUID_WILD_MUSHROOM_HEAL, true, NULL, NULLAURA_EFFECT, player->GetGUID()); // heal
                        mushroom->RemoveDynObject(SPELL_DRUID_SWIFTMEND);
                        mushroom->RemoveAura(SPELL_DRUID_SWIFTMEND);
                        player->RemoveAura(SPELL_DRUID_WILD_MUSHROOM_GROWING);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_bloom_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_wild_mushroom_bloom_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_bloom_SpellScript();
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
class spell_dru_celestial_alignment : public SpellScriptLoader
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
                    if (Unit* target = GetHitUnit())
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
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_celestial_alignment_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_celestial_alignment_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_celestial_alignment_AuraScript);

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
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
class spell_dru_stampeding_roar : public SpellScriptLoader
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
class spell_dru_lacerate : public SpellScriptLoader
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
                    if (Unit* target = GetHitUnit())
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
class spell_dru_faerie_fire : public SpellScriptLoader
{
    public:
        spell_dru_faerie_fire() : SpellScriptLoader("spell_dru_faerie_fire") { }

        class spell_dru_faerie_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_faerie_fire_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->GetShapeshiftForm() == FORM_BEAR)
                        {
                            if (_player->HasSpellCooldown(GetSpellInfo()->Id))
                                _player->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                        }
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
class spell_dru_teleport_moonglade : public SpellScriptLoader
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

// Prowl - 102547, Displacer Beast - 102280 and Dash - 1850
class spell_dru_activate_cat_form : public SpellScriptLoader
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
    SPELL_DRUID_WRATH          = 5176,
    SPELL_DRUID_STARFIRE       = 2912,
    SPELL_DRUID_STARSURGE      = 78674,
    SPELL_DRUID_EUPHORIA       = 81062,
    SPELL_DRUID_SHOOTING_STARS = 93400,
};

// 5176 - Wrath, 2912 - Starfire and 78674 - Starsurge
class spell_dru_eclipse : public SpellScriptLoader
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
                        if (_player->HasAura(SPELL_DRUID_DREAM_OF_CENARIUS_TALENT) && _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DROOD_RESTORATION &&
                            GetSpellInfo()->Id == SPELL_DRUID_WRATH)
                        {
                            int32 damage = GetHitDamage();
                            AddPct(damage, 20);
                            SetHitDamage(damage);
                        }

                        if (!_player->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT) && _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DROOD_BALANCE)
                        {
                            float modifier = 1.0f;
                            if (_player->HasAura(SPELL_DRUID_EUPHORIA) && !_player->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                modifier *= 2;

                            switch (GetSpellInfo()->Id)
                            {
                                case SPELL_DRUID_WRATH:
                                {
                                    int32 eclipse = 15 * modifier; // X Lunar energy

                                    _player->SetEclipsePower(int32(_player->GetEclipsePower() - eclipse));

                                    if (_player->GetEclipsePower() == -100 && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                    {
                                        _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE, true, 0); // Cast Lunar Eclipse
                                        _player->m_lastEclipseState = ECLIPSE_LUNAR;
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
                                            aura->SetDuration(aura->GetDuration() + 2000);
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
                                        _player->CastSpell(_player, SPELL_DRUID_SOLAR_ECLIPSE, true, 0); // Cast Solar Eclipse
                                        _player->m_lastEclipseState = ECLIPSE_SOLAR;
                                        _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                                        _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                                    }

                                    // Your crits with Starfire also increase moonfire duration by 2s
                                    if (GetSpell()->IsCritForTarget(target))
                                    {
                                        if (AuraPtr aura = target->GetAura(SPELL_DRUID_MOONFIRE))
                                        {
                                            aura->SetDuration(aura->GetDuration() + 2000);
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
                                        _player->CastSpell(_player, SPELL_DRUID_SOLAR_ECLIPSE, true, 0); // Cast Solar Eclipse
                                        _player->m_lastEclipseState = ECLIPSE_SOLAR;
                                        _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                                        _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                                    }
                                    else if (_player->GetEclipsePower() == -100 && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                    {
                                        _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE, true, 0); // Cast Lunar Eclipse
                                        _player->m_lastEclipseState = ECLIPSE_LUNAR;
                                        _player->CastSpell(_player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                                        _player->CastSpell(_player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true); // Cast Eclipse - Give 35% of POWER_MANA
                                        _player->CastSpell(_player, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);

                                        if (_player->HasSpellCooldown(SPELL_DRUID_STARFALL))
                                            _player->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
                                    }

                                    // Your crits with Starsurge also increase sunfire duration by 2s
                                    if (GetSpell()->IsCritForTarget(target))
                                    {
                                        if (AuraPtr aura = target->GetAura(SPELL_DRUID_SUNFIRE))
                                        {
                                            aura->SetDuration(aura->GetDuration() + 2000);
                                            if (aura->GetMaxDuration() < aura->GetDuration())
                                                aura->SetMaxDuration(aura->GetDuration());
                                        }
                                    }
                                    // Your crits with Starsurge also increase moonfire duration by 2s
                                    if (GetSpell()->IsCritForTarget(target))
                                    {
                                        if (AuraPtr aura = target->GetAura(SPELL_DRUID_MOONFIRE))
                                        {
                                            aura->SetDuration(aura->GetDuration() + 2000);
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
                    if (plr->HasAura(SPELL_DRUID_DREAM_OF_CENARIUS_TALENT) && plr->GetSpecializationId(plr->GetActiveSpec()) == SPEC_DROOD_RESTORATION &&
                        GetSpellInfo()->Id == SPELL_DRUID_WRATH)
                    {
                        int32 bp = GetHitDamage();

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
class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
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

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public SpellScriptLoader
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
                if (Player* caster = GetCaster()->ToPlayer())
                    if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                        amount = 310;
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

enum StarfallSpells
{
    SPELL_DRUID_MOONFIRE              = 8921,
    //SPELL_DRUID_STARFALL              = 48505,
    SPELL_DRUID_SUNFIRE               = 93402,
    SPELL_DRUID_GLYPH_OF_GUIDED_STARS = 146655
};
// Starfall (triggered) - 50286
class spell_dru_starfall_dummy : public SpellScriptLoader
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
    SPELL_DRUID_SAVAGE_ROAR = 62071,
};

// Savage Roar - 52610
class spell_dru_savage_roar : public SpellScriptLoader
{
    public:
        spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

        class spell_dru_savage_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_roar_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetShapeshiftForm() != FORM_CAT)
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
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SAVAGE_ROAR))
                    return false;
                return true;
            }

            void AfterApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->CastSpell(target, SPELL_DRUID_SAVAGE_ROAR, true, NULL, aurEff, GetCasterGUID());
            }

            void AfterRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR);
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
class spell_dru_survival_instincts : public SpellScriptLoader
{
    public:
        spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

        class spell_dru_survival_instincts_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

            void AfterApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_SURVIVAL_INSTINCTS, true);
            }

            void AfterRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SURVIVAL_INSTINCTS);
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
    new spell_dru_wild_mushroom_heal();
    new spell_dru_wild_mushroom_growing();
    new spell_dru_wild_mushroom_resto();
    new spell_dru_wild_mushroom_bloom();
    new spell_dru_wild_mushroom_detonate();
    new spell_dru_wild_mushroom();
    new spell_dru_celestial_alignment();
    new spell_dru_stampeding_roar();
    new spell_dru_lacerate();
    new spell_dru_faerie_fire();
    new spell_dru_teleport_moonglade();
    new spell_dru_activate_cat_form();
    new spell_dru_eclipse();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_swift_flight_passive();
    new spell_dru_starfall_dummy();
    new spell_dru_savage_roar();
    new spell_dru_survival_instincts();
}
