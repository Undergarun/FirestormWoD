////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"

enum YseraGiftSpells
{
    SPELL_DRUID_YSERAS_GIFT_HEAL_CASTER = 145109,
    SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY   = 145110
};

/// last update : 6.1.2 19802
/// Ysera's Gift - 145108
class spell_dru_yseras_gift: public SpellScriptLoader
{
    public:
        spell_dru_yseras_gift() : SpellScriptLoader("spell_dru_yseras_gift") { }

        class spell_dru_yseras_gift_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_yseras_gift_AuraScript);

            void OnTick(AuraEffect const* p_AurEff)
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
                    int32 l_HealAmount = CalculatePct(l_Caster->GetMaxHealth(), p_AurEff->GetAmount());
                    l_Caster->CastCustomSpell(l_Caster, SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY, &l_HealAmount, NULL, NULL, true);
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

/// last update : 6.1.2 19802
/// Ysera's Gift (Ally heal) - 145110
class spell_dru_yseras_gift_ally_proc : public SpellScriptLoader
{
    public:
        spell_dru_yseras_gift_ally_proc() : SpellScriptLoader("spell_dru_yseras_gift_ally_proc") { }

        class spell_dru_yseras_gift_ally_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_yseras_gift_ally_proc_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();

                p_Targets.remove_if([l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->ToUnit() == nullptr)
                        return true;

                    if (p_Object->ToUnit()->IsFullHealth() || p_Object->GetGUID() == l_Caster->GetGUID())
                        return true;

                    return false;
                });

                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(1);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_yseras_gift_ally_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_yseras_gift_ally_proc_SpellScript();
        }
};

enum ToothAndClawAbsorbSpells
{
    SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA = 135601
};

/// Tooth and Claw - 135597
class spell_dru_tooth_and_claw_absorb: public SpellScriptLoader
{
    public:
        spell_dru_tooth_and_claw_absorb() : SpellScriptLoader("spell_dru_tooth_and_claw_absorb") { }

        class spell_dru_tooth_and_claw_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_tooth_and_claw_absorb_AuraScript);

            void OnAbsorb(AuraEffect* /*aurEff*/, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                Unit* l_Attacker = p_DmgInfo.GetAttacker();

                if (l_Attacker == nullptr)
                    return;

                if (!l_Attacker->HasAura(SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA))
                    p_AbsorbAmount = 0;
            }

            void AfterAbsorb(AuraEffect* /*aurEff*/, DamageInfo& p_DmgInfo, uint32& /*p_AbsorbAmount*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Attacker = p_DmgInfo.GetAttacker();

                if (l_Attacker == nullptr || l_Caster == nullptr)
                    return;

                l_Attacker->RemoveAura(SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA, l_Caster->GetGUID());
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_tooth_and_claw_absorb_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dru_tooth_and_claw_absorb_AuraScript::AfterAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_tooth_and_claw_absorb_AuraScript();
        }
};

enum GenesisSpells
{
    SPELL_DRUID_REJUVENATION = 774,
    SPELL_DRUID_GENESIS_HEAL = 162359
};

/// last update : 6.1.2 19802
/// Genesis - 145518
class spell_dru_genesis: public SpellScriptLoader
{
    public:
        spell_dru_genesis() : SpellScriptLoader("spell_dru_genesis") { }

        class spell_dru_genesis_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_genesis_SpellScript);
            
            enum eSpells
            {
                RejuvenationAura = 774,
                GerminationAura = 155777,
                GenersisHeal = 162359
            };

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();
                SpellInfo const* l_GenesisHeal = sSpellMgr->GetSpellInfo(eSpells::GenersisHeal);

                if (l_Player == nullptr || l_GenesisHeal == nullptr)
                    return;

                    std::list<Unit*> partyMembers;
                    l_Player->GetRaidMembers(partyMembers);
                    int l_Rejuvenations[2] = { eSpells::RejuvenationAura, eSpells::GerminationAura };

                    for (auto l_Target : partyMembers)
                    {
                        if (!l_Target->IsWithinDist(l_Player, 60.0f) || !l_Target->IsWithinLOSInMap(l_Player))
                            continue;

                        int32 l_HealAmount = 0;
                        bool l_AtListOne = false;
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            if (Aura* l_Rejuvenation = l_Target->GetAura(l_Rejuvenations[i], l_Player->GetGUID()))
                            {
                                int8 l_TicksLeft = uint8(l_Rejuvenation->GetDuration() / l_Rejuvenation->GetEffect(0)->GetAmplitude()) + 1;

                                if (l_GenesisHeal->Effects[0].Amplitude)
                                    l_HealAmount += uint32((l_TicksLeft * l_Rejuvenation->GetEffect(0)->GetAmount()) / (l_GenesisHeal->GetMaxDuration() / l_GenesisHeal->Effects[0].Amplitude));

                                l_Target->RemoveAura(l_Rejuvenations[i]);
                                l_AtListOne = true;
                            }
                        }
                        if (l_AtListOne)
                            l_Player->CastCustomSpell(l_Target, SPELL_DRUID_GENESIS_HEAL, &l_HealAmount, NULL, NULL, true);
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

enum DreamOfCenariusRestorationSpells
{
    SPELL_DRUID_DREAM_OF_CENARIUS_RESTO_TALENT = 158504,
    SPELL_DRUID_DREAM_OF_CENARIUS_HEAL         = 145153
};

/// Wrath - 5176
class spell_dru_wrath : public SpellScriptLoader
{
public:
    spell_dru_wrath() : SpellScriptLoader("spell_dru_wrath") { }

    class spell_dru_wrath_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wrath_SpellScript);

        void HandleBeforeHit()
        {
            /// Dream of Cenarius - 158504
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(SPELL_DRUID_DREAM_OF_CENARIUS_RESTO_TALENT))
                {
                    std::list<Unit*> l_Party;

                    l_Caster->GetRaidMembers(l_Party);

                    if (l_Party.empty())
                        return;

                    if (l_Party.size() > 1)
                    {
                        l_Party.sort(JadeCore::HealthPctOrderPred());
                        l_Party.resize(1);
                    }

                    SpellInfo const* l_DreamOfCenariusSpellInfo = l_Caster->GetAura(SPELL_DRUID_DREAM_OF_CENARIUS_RESTO_TALENT)->GetSpellInfo();
                    SpellInfo const* l_DreamOfCenariusHealSpellInfo = sSpellMgr->GetSpellInfo(SPELL_DRUID_DREAM_OF_CENARIUS_HEAL);

                    if (l_DreamOfCenariusSpellInfo == nullptr || l_DreamOfCenariusHealSpellInfo == nullptr)
                        return;

                    int32 l_HealAmount = CalculatePct(GetHitDamage(), l_DreamOfCenariusSpellInfo->Effects[EFFECT_1].BasePoints);
                    l_Caster->HealBySpell(l_Party.front(), l_DreamOfCenariusHealSpellInfo, l_HealAmount, false, false);
                }
            }
        }

        void Register()
        {
            BeforeHit += SpellHitFn(spell_dru_wrath_SpellScript::HandleBeforeHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_wrath_SpellScript();
    }
};

enum GlyphOfTheTreantSpells
{
    SPELL_DRUID_GLYPH_OF_THE_TREANT = 114282
};

/// Glyph of the Treant - 125047
class spell_dru_glyph_of_the_treant: public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_the_treant() : SpellScriptLoader("spell_dru_glyph_of_the_treant") { }

        class spell_dru_glyph_of_the_treant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_the_treant_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->learnSpell(SPELL_DRUID_GLYPH_OF_THE_TREANT, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    if (l_Player->HasSpell(SPELL_DRUID_GLYPH_OF_THE_TREANT))
                        l_Player->removeSpell(SPELL_DRUID_GLYPH_OF_THE_TREANT, false, false);
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

/// Incarnation : Chosen of Elune - 102560
class spell_dru_incarnation_chosen_of_elune: public SpellScriptLoader
{
    public:
        spell_dru_incarnation_chosen_of_elune() : SpellScriptLoader("spell_dru_incarnation_chosen_of_elune") { }

        class spell_dru_incarnation_chosen_of_elune_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_chosen_of_elune_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->CastSpell(l_Player, SPELL_DRUID_INCARNATION_CHOSEN_OF_ELUNE, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->RemoveAura(SPELL_DRUID_INCARNATION_CHOSEN_OF_ELUNE);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_incarnation_chosen_of_elune_AuraScript();
        }
};

/// Called by Incarnation : Chosen of Elune - 102560, Incarnation : Son of Ursoc - 102558 and Incarnation : King of the Jungle - 102543
/// Incarnation - Skins
class spell_dru_incarnation_skins: public SpellScriptLoader
{
    public:
        spell_dru_incarnation_skins() : SpellScriptLoader("spell_dru_incarnation_skins") { }

        class spell_dru_incarnation_skins_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_skins_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->SetDisplayId(l_Player->GetModelForForm(l_Player->GetShapeshiftForm()));
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->SetDisplayId(l_Player->GetModelForForm(l_Player->GetShapeshiftForm()));
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
                        OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
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

/// Item - PvP Feral 4P Bonus - 131537
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

            void OnUpdate(uint32 diff, AuraEffect* /*aurEff*/)
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
                    if (Player* l_Player = GetCaster()->ToPlayer())
                        l_Player->CastSpell(l_Player, SPELL_DRUID_STAMPEDE, true);

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

/// Wild Charge (Moonkin) - 102383
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

/// Thrash (bear) - 77758
class spell_dru_thrash_bear: public SpellScriptLoader
{
    public:
        spell_dru_thrash_bear() : SpellScriptLoader("spell_dru_thrash_bear") { }

        class spell_dru_thrash_bear_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_thrash_bear_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    // Each tick grant 1 point of rage
                    caster->ModifyPower(POWER_RAGE, 1 * GetCaster()->GetPowerCoeff(POWER_RAGE));
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

/// Swipe - 106785
class spell_dru_swipe: public SpellScriptLoader
{
    public:
        spell_dru_swipe() : SpellScriptLoader("spell_dru_swipe") { }

        class spell_dru_swipe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_swipe_SpellScript);

            void HandleDamage(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                int32 l_Damage = GetHitDamage();

                /// Award 1 combot point
                l_Caster->AddComboPoints(GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                /// Swipe and Maul deals 20% more damage if target is bleeding
                if (l_Target->HasAuraState(AURA_STATE_BLEEDING))
                {
                    AddPct(l_Damage, GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                    SetHitDamage(l_Damage);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_swipe_SpellScript::HandleDamage, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
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

/// Maul - 6807
class spell_dru_maul: public SpellScriptLoader
{
    public:
        spell_dru_maul() : SpellScriptLoader("spell_dru_maul") { }

        class spell_dru_maul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_maul_SpellScript);

            enum eSpells
            {
                T17Guardian4P   = 165423,
                PrimalMending   = 177969
            };

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        int32 l_Damage = GetHitDamage();

                        // Deals 20% more damage if target is bleeding
                        if (l_Target->HasAuraState(AURA_STATE_BLEEDING))
                        {
                            AddPct(l_Damage, GetSpellInfo()->Effects[EFFECT_3].BasePoints);
                            SetHitDamage(l_Damage);
                        }

                        if (l_Caster->HasAura(SPELL_DRUID_TOOTH_AND_CLAW_AURA))
                        {
                            int32 l_Bp = l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 2.4f;

                            l_Caster->CastCustomSpell(l_Caster, SPELL_DRUID_TOOTH_AND_CLAW_ABSORB, &l_Bp, NULL, NULL, true);
                            l_Caster->CastCustomSpell(l_Target, SPELL_DRUID_TOOTH_AND_CLAW_VISUAL_AURA, &l_Bp, NULL, NULL, true);

                            /// Applying Tooth and Claw to a target increases the healing of your next Frenzied Regeneration by 10%. Stacks up to 3 times.
                            if (l_Caster->HasAura(eSpells::T17Guardian4P))
                                l_Caster->CastSpell(l_Caster, eSpells::PrimalMending, true);

                            if (Aura* l_Aura = l_Caster->GetAura(SPELL_DRUID_TOOTH_AND_CLAW_AURA))
                                l_Aura->SetStackAmount(l_Aura->GetStackAmount() - 1);
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

enum NaturesVigilSpells
{
    SPELL_DRUID_NATURES_VIGIL_HEAL   = 124988,
    SPELL_DRUID_NATURES_VIGIL_DAMAGE = 124991
};

/// Nature's Vigil - 124974
class spell_dru_natures_vigil: public SpellScriptLoader
{
    public:
        spell_dru_natures_vigil() : SpellScriptLoader("spell_dru_natures_vigil") { }

        class spell_dru_natures_vigil_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_natures_vigil_AuraScript);

            enum eSpells
            {
                NatureVigilHeal = 124988,
                NatureVigilDamage = 124991
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellProcInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();

                if (l_Caster == nullptr || l_SpellProcInfo == nullptr)
                    return;

                bool l_SingleTarget = false;
                for (uint8 i = 0; i < l_SpellProcInfo->EffectCount; ++i)
                {
                    if ((l_SpellProcInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                        l_SpellProcInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY) &&
                        l_SpellProcInfo->Effects[i].TargetB.GetTarget() == 0)
                        l_SingleTarget = true;
                }

                if (!l_SingleTarget)
                    return;

                int32 l_Bp = CalculatePct(p_EventInfo.GetDamageInfo()->GetDamage(), GetSpellInfo()->Effects[EFFECT_2].BasePoints);

                if (!l_SpellProcInfo->IsPositive())
                    l_Bp = CalculatePct(p_EventInfo.GetDamageInfo()->GetDamage(), GetSpellInfo()->Effects[EFFECT_3].BasePoints);

                l_Caster->CastCustomSpell(l_Caster, eSpells::NatureVigilHeal, &l_Bp, NULL, NULL, true);

                Player *l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_DRUID_RESTORATION && l_SpellProcInfo->IsPositive())
                    l_Caster->CastCustomSpell(l_Caster, eSpells::NatureVigilDamage, &l_Bp, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_natures_vigil_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_natures_vigil_AuraScript();
        }
};

/// Nature's Vigil (heal) - 124988, Nature's Vigil (damage) - 124991
class spell_dru_natures_vigil_proc : public SpellScriptLoader
{
    public:
        spell_dru_natures_vigil_proc() : SpellScriptLoader("spell_dru_natures_vigil_proc") { }

        class spell_dru_natures_vigil_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_natures_vigil_proc_SpellScript);

            enum eSpells
            {
                NatureVigilHeal = 124988,
                NatureVigilDamage = 124991
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(1);
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case eSpells::NatureVigilHeal:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_natures_vigil_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                    break;
                case eSpells::NatureVigilDamage:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_natures_vigil_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                    break;
                default:
                    break;
                }
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_natures_vigil_proc_SpellScript();
        }
};

enum UrsolsVortexSpells
{
    SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER = 102793,
    SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST    = 118283,
    SPELL_DRUID_URSOLS_VORTEX_SNARE        = 127797
};

enum DashSpells
{
    SPELL_DRUID_STAMPEDING_ROAR = 106898
};

/// Dash - 1850
class spell_dru_dash: public SpellScriptLoader
{
    public:
        spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

        class spell_dru_dash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_dash_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(SPELL_DRUID_STAMPEDING_ROAR))
                        l_Player->RemoveAura(SPELL_DRUID_STAMPEDING_ROAR);

                    l_Player->RemoveMovementImpairingAuras();
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

enum GerminationSpells
{
    SPELL_DRUID_GERMINATION = 155777,
    SPELL_DRUID_GERMINATION_PASSIVE_TALENT = 155675
};

enum DruidSoulOfTheForestSpells
{
    SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO_TALENT = 158478,
    SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO = 114108
};

enum LifebloomSpells
{
    SPELL_DRUID_LIFEBLOOM = 33763,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL = 33778,
    SPELL_DRUID_GLYPH_OF_BLOOMING = 121840,
    SPELL_DRUID_OMEN_OF_CLARITY = 113043,
    SPELL_DRUID_MOMENT_OF_CLARITY_TALENT = 155577,
    SPELL_DRUID_MOMENT_OF_CLARITY = 155631,
    SPELL_DRUID_CLEARCASTING = 16870
};

/// last update : 6.2.3
/// Rejuvenation - 774 (germination effect)
class spell_dru_rejuvenation : public SpellScriptLoader
{
public:
    spell_dru_rejuvenation() : SpellScriptLoader("spell_dru_rejuvenation") { }

    class spell_dru_rejuvenation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_rejuvenation_SpellScript);

        int32 m_RejuvenationAura = 0;
        int32 m_RejuvenationAuraAmount = 0;

        void HandleAfterHit()
        {
            Unit* l_Caster = GetCaster();
            if (!l_Caster)
                return;

            Unit* l_Target = GetHitUnit();
            if (!l_Target)
                return;

            Aura* l_RejuvenationAura = l_Target->GetAura(SPELL_DRUID_REJUVENATION);

            if (l_RejuvenationAura && m_RejuvenationAura > 0)
                l_RejuvenationAura->SetDuration(m_RejuvenationAura);
        }

        void HandleBeforeHit()
        {
            Unit* l_Caster = GetCaster();
            if (!l_Caster)
                return;

            Unit* l_Target = GetHitUnit();
            if (!l_Target)
                return;

            ///Germination
            if (l_Caster->HasAura(SPELL_DRUID_GERMINATION_PASSIVE_TALENT) && l_Target->HasAura(SPELL_DRUID_REJUVENATION))
            {
                Aura* l_RejuvenationAura = l_Target->GetAura(SPELL_DRUID_REJUVENATION);
                if (!l_RejuvenationAura)
                    return;

                if (!l_Target->HasAura(SPELL_DRUID_GERMINATION))
                {
                    l_Caster->AddAura(SPELL_DRUID_GERMINATION, l_Target);
                    m_RejuvenationAura = l_RejuvenationAura->GetDuration();
                    if (AuraEffect const* l_AuraEffect = l_RejuvenationAura->GetEffect(EFFECT_0))
                        m_RejuvenationAuraAmount = l_AuraEffect->GetAmount();
                }
                else
                {
                    Aura* l_GerminationAura = l_Target->GetAura(SPELL_DRUID_GERMINATION);
                    Aura* l_RejuvenationAura = l_Target->GetAura(SPELL_DRUID_REJUVENATION);
                    if (l_GerminationAura && l_RejuvenationAura)
                    {
                        int32 l_GerminationDuration = l_GerminationAura->GetDuration();
                        int32 l_RejuvenationDuration = l_RejuvenationAura->GetDuration();
                        if (l_GerminationDuration > l_RejuvenationDuration)
                            l_Caster->AddAura(SPELL_DRUID_REJUVENATION, l_Target);
                        else
                        {
                            l_Caster->AddAura(SPELL_DRUID_GERMINATION, l_Target);
                            m_RejuvenationAura = l_RejuvenationDuration;
                            if (AuraEffect const* l_AuraEffect = l_RejuvenationAura->GetEffect(EFFECT_0))
                                m_RejuvenationAuraAmount = l_AuraEffect->GetAmount();
                        }
                    }
                }
            }
        }

        void Register()
        {
            BeforeHit += SpellHitFn(spell_dru_rejuvenation_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_dru_rejuvenation_SpellScript::HandleAfterHit);
        }
    };

    class spell_dru_rejuvenation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rejuvenation_AuraScript);

        enum eSpells
        {
            GlyphofRejuvenation         = 17076,
            GlyphofRejuvenationEffect   = 96206
        };

        void HandleCalculateAmount(AuraEffect const* /*p_AurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                ///If soul of the forest is activated we increase the heal by 100%
                if (l_Caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
                {
                    amount *= 2;
                    l_Caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                }
            }
        }

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            Unit* l_Caster = GetCaster();

            if (l_Caster == nullptr)
                return;

            if (AuraEffect* l_GlyphOfRejuvenation = l_Caster->GetAuraEffect(eSpells::GlyphofRejuvenation, EFFECT_0))
            {
                l_GlyphOfRejuvenation->SetAmount(l_GlyphOfRejuvenation->GetAmount() + 1);
                if (l_GlyphOfRejuvenation->GetAmount() >= 3)
                    l_Caster->CastSpell(l_Caster, eSpells::GlyphofRejuvenationEffect, true);
            }
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            Unit* l_Caster = GetCaster();

            if (l_Caster == nullptr)
                return;

            if (AuraEffect* l_GlyphOfRejuvenation = l_Caster->GetAuraEffect(eSpells::GlyphofRejuvenation, EFFECT_0))
            {
                l_GlyphOfRejuvenation->SetAmount(l_GlyphOfRejuvenation->GetAmount() - 1);
                if (l_GlyphOfRejuvenation->GetAmount() < 3)
                    l_Caster->RemoveAura(eSpells::GlyphofRejuvenationEffect);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_rejuvenation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_rejuvenation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rejuvenation_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_rejuvenation_AuraScript();
    }

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_rejuvenation_SpellScript();
    }
};

/// Regrowth - 8936
class spell_dru_regrowth : public SpellScriptLoader
{
    public:
        spell_dru_regrowth() : SpellScriptLoader("spell_dru_regrowth") { }

        enum eSpells
        {
            GlyphOfRegrowth = 116218
        };

        class spell_dru_regrowth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_regrowth_SpellScript);

            void HandlePeriodic(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || GetCaster()->HasAura(eSpells::GlyphOfRegrowth))
                    PreventHitAura();
            }

            void HandleBeforeHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    ///If soul of the forest is activated we increase the heal by 100%
                    if (l_Caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
                        SetHitHeal(GetHitHeal() * 2);
                }
            }

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_DRUID_CLEARCASTING))
                        l_Caster->RemoveAura(SPELL_DRUID_CLEARCASTING);
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_dru_regrowth_SpellScript::HandleBeforeHit);
                AfterHit += SpellHitFn(spell_dru_regrowth_SpellScript::HandleAfterHit);
                OnEffectHitTarget += SpellEffectFn(spell_dru_regrowth_SpellScript::HandlePeriodic, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        class spell_dru_regrowth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_regrowth_AuraScript);

            void HandleCalculateAmountOnTick(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    uint32 l_TickNumber = aurEff->GetTickNumber();
                    /// If soul of the forest is activated we increase the heal by 100%
                    /// Can be increased just on aura apply, can't remove Soul of the Forest on all periodic ticks
                    if (l_TickNumber == 0 && l_Caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
                    {
                        amount *= 2;
                        l_Caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_regrowth_AuraScript::HandleCalculateAmountOnTick, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_regrowth_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_regrowth_SpellScript();
        }
};

/// Wild growth - 48438
class spell_dru_wild_growth : public SpellScriptLoader
{
    public:
        spell_dru_wild_growth() : SpellScriptLoader("spell_dru_wild_growth") { }

        enum eSpells
        {
            TreeOfLife = 33891,
            T10Resto2PBonus = 70658,
            GlyphofWildGrowth = 62970
        };

        class spell_dru_wild_growth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_growth_SpellScript);

            bool m_HasSoulOfTheForest = false;

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                uint8 l_MaxTargets = GetSpellInfo()->Effects[EFFECT_2].BasePoints + 1; ///< +1 = Main Target
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::TreeOfLife))
                    l_MaxTargets += 2;

                if (l_Caster->HasAura(eSpells::GlyphofWildGrowth))
                    l_MaxTargets += 1;

                if (p_Targets.size() > l_MaxTargets)
                    JadeCore::RandomResizeList(p_Targets, l_MaxTargets);
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                /// If soul of the forest is activated we increase the heal by 50%
                if (AuraEffect* l_AuraEffet = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0, l_Caster->GetGUID()))
                {
                    if (l_Caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO) || m_HasSoulOfTheForest)
                    {
                        l_AuraEffet->SetAmount(l_AuraEffet->GetAmount() + CalculatePct(l_AuraEffet->GetAmount(), 50));
                        m_HasSoulOfTheForest = true;
                        l_Caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_dru_wild_growth_SpellScript::HandleAfterHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        class spell_dru_wild_growth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_wild_growth_AuraScript);

            uint32 m_TooltipAmount;

            void HandleCalculateAmountOnTick(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                m_TooltipAmount = 7*p_Amount; ///< The base healing is split among the ticks with the first tick getting (6%+1/7) of the tooltip heal
                p_Amount += CalculatePct(m_TooltipAmount, 6);
            }

            void DecreaseHealOnTick(AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                float l_SetMod = 1.0f;

                // Item - Druid T10 Restoration 2P Bonus
                if (AuraEffect* l_T10Resto2PBonus = l_Caster->GetAuraEffect(eSpells::T10Resto2PBonus, EFFECT_0))
                    l_SetMod = 1.0f - l_T10Resto2PBonus->GetAmount() / 100.0f;

                int32 l_Amount = p_AurEff->GetAmount();
                l_Amount -= l_SetMod * CalculatePct(m_TooltipAmount, 2);  ///< "each successive tick losing 2% of the tooltip heal" http://wowwiki.wikia.com/Wild_Growth
                p_AurEff->SetAmount(l_Amount);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_wild_growth_AuraScript::HandleCalculateAmountOnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_dru_wild_growth_AuraScript::DecreaseHealOnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_growth_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_wild_growth_AuraScript();
        }
};

enum SavageDefenseSpells
{
    SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT = 132402
};

/// Savage Defense - 62606
class spell_dru_savage_defense: public SpellScriptLoader
{
    public:
        spell_dru_savage_defense() : SpellScriptLoader("spell_dru_savage_defense") { }

        class spell_dru_savage_defense_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_defense_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    l_Player->CastSpell(l_Player, SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT, true);
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

/// Called by Regrowth - 8936 and Healing Touch - 5185
/// Lifebloom - 33763 : Refresh duration
class spell_dru_lifebloom_refresh: public SpellScriptLoader
{
    public:
        spell_dru_lifebloom_refresh() : SpellScriptLoader("spell_dru_lifebloom_refresh") { }

        class spell_dru_lifebloom_refresh_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_lifebloom_refresh_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (!l_Caster->HasAura(SPELL_DRUID_GLYPH_OF_BLOOMING))
                {
                    if (Aura* l_Lifebloom = l_Target->GetAura(SPELL_DRUID_LIFEBLOOM, l_Caster->GetGUID()))
                        l_Lifebloom->RefreshDuration();
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

/// Lifebloom - 33763
class spell_dru_lifebloom: public SpellScriptLoader
{
    public:
        spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

        class spell_dru_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lifebloom_AuraScript);

            void AfterRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                // Final heal only on duration end
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                Unit* l_Target = GetUnitOwner();
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Target)
                    return;

                SpellInfo const* l_LifebloomFinalHeal = sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL);

                // final heal
                int32 l_SpellPower = 0;
                int32 l_HealAmount = 0;

                l_SpellPower = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_SpellPower, p_AurEff->GetEffIndex(), HEAL);
                l_SpellPower = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_SpellPower, HEAL);

                if (l_LifebloomFinalHeal != nullptr)
                    l_HealAmount = l_SpellPower * l_LifebloomFinalHeal->Effects[EFFECT_0].BonusMultiplier;

                l_Target->CastCustomSpell(l_Target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &l_HealAmount, NULL, NULL, true, NULL, p_AurEff, GetCasterGUID());
            }

            void HandleDispel(DispelInfo* p_DispelInfo)
            {
                if (AuraEffect const* aurEff = GetEffect(EFFECT_0))
                {
                    Unit* l_Target = GetUnitOwner();
                    Unit* l_Caster = GetCaster();

                    if (!l_Caster || !l_Target)
                        return;

                    SpellInfo const* l_LifebloomFinalHeal = sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL);

                    // final heal
                    int32 l_SpellPower = 0;
                    int32 l_HealAmount = 0;

                    l_SpellPower = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_SpellPower, aurEff->GetEffIndex(), HEAL, p_DispelInfo->GetRemovedCharges());
                    l_SpellPower = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_SpellPower, HEAL, p_DispelInfo->GetRemovedCharges());

                    if (l_LifebloomFinalHeal != nullptr)
                        l_HealAmount = l_SpellPower * l_LifebloomFinalHeal->Effects[EFFECT_0].BonusMultiplier;

                    l_Target->CastCustomSpell(l_Target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &l_HealAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
                }
            }

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster && l_Caster->HasAura(SPELL_DRUID_OMEN_OF_CLARITY) && roll_chance_i(4))
                {
                    if (l_Caster->HasAura(SPELL_DRUID_MOMENT_OF_CLARITY_TALENT))
                        l_Caster->CastSpell(l_Caster, SPELL_DRUID_MOMENT_OF_CLARITY, true);
                    else
                        l_Caster->CastSpell(l_Caster, SPELL_DRUID_CLEARCASTING, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                OnDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_lifebloom_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_lifebloom_AuraScript();
        }
};

/// Lifebloom (final heal) - 33778
class spell_dru_lifebloom_final_heal : public SpellScriptLoader
{
    public:
        spell_dru_lifebloom_final_heal() : SpellScriptLoader("spell_dru_lifebloom_final_heal") { }

        class spell_dru_lifebloom_final_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_lifebloom_final_heal_SpellScript);

            void HandleHeal(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();

                SpellInfo const* l_GlyphOfBlooming = sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_BLOOMING);

                if (l_GlyphOfBlooming != nullptr && l_Caster->HasAura(SPELL_DRUID_GLYPH_OF_BLOOMING)) ///< Increases the bloom heal of your Lifebloom when it expires by 50%
                    SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), l_GlyphOfBlooming->Effects[EFFECT_0].BasePoints));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_lifebloom_final_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_lifebloom_final_heal_SpellScript();
        }
};

enum KillerInstinct
{
    SPELL_DRUID_KILLER_INSTINCT          = 108299,
    SPELL_DRUID_KILLER_INSTINCT_MOD_STAT = 108300
};

/// Called by Cat Form - 768 and Bear Form - 5487
/// Killer Instinct - 108299
class spell_dru_killer_instinct: public SpellScriptLoader
{
    public:
        spell_dru_killer_instinct() : SpellScriptLoader("spell_dru_killer_instinct") { }

        class spell_dru_killer_instinct_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_killer_instinct_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(SPELL_DRUID_KILLER_INSTINCT))
                    {
                        int32 bp = l_Player->GetStat(STAT_INTELLECT);

                        l_Player->CastCustomSpell(l_Player, SPELL_DRUID_KILLER_INSTINCT_MOD_STAT, &bp, NULL, NULL, true);
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

/// Nature's Cure - 88423 and Remove Corruption - 2782
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
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                for (uint8 i = 0; i < l_SpellInfo->EffectCount; ++i)
                {
                    if (!l_SpellInfo->Effects[i].IsEffect())
                        break;

                    uint32 dispel_type = l_SpellInfo->Effects[i].MiscValue;
                    uint32 dispelMask = l_SpellInfo->GetDispelMask(DispelType(dispel_type));

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

/// Last Update 6.2.3
/// Called by Prowl - 5215, Displacer Beast - 102280 and Dash - 1850
/// Should activate the cat form if not in cat form
class spell_dru_activate_cat_form : public SpellScriptLoader
{
    public:
        spell_dru_activate_cat_form() : SpellScriptLoader("spell_dru_activate_cat_form") { }

        class spell_dru_activate_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_activate_cat_form_SpellScript);

            enum eSpells
            {
                CatForm = 768,
                IncarnationTreeofLife = 33891
            };

            void HandleBeforeHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(eSpells::IncarnationTreeofLife)) ///< Prevent it to be remove by default
                    l_Player->RemoveAura(eSpells::IncarnationTreeofLife, 0, 0, AURA_REMOVE_BY_CANCEL);
                if (l_Player && l_Player->GetShapeshiftForm() != FORM_CAT)
                    l_Player->CastSpell(l_Player, eSpells::CatForm, true);
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_dru_activate_cat_form_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_activate_cat_form_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Cat Form - 768, Cat Form (Claws of Shirvallah) - 171746
class spell_dru_cat_form: public SpellScriptLoader
{
    public:
        spell_dru_cat_form() : SpellScriptLoader("spell_dru_cat_form") { }

        class spell_dru_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_cat_form_SpellScript);

            enum eSpells
            {
                SavageRoar = 52610
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                l_Player->RemoveMovementImpairingAuras();

                /// Fix Berserk - amount of energy after reshift
                l_Player->UpdateMaxPower(POWER_ENERGY);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_cat_form_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_cat_form_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_cat_form_AuraScript);

            enum eSpells
            {
                Dash           = 1850,
                Prowl          = 5215,
                GlyphOfCatForm = 47180,
                DesplacerBeast = 137452,
                BurningEssence = 138927,
                BurningEssenceModel = 38150
            };

            enum eDatas
            {
                FandralsFlamescythe         = 69897,
                FandralsFlamescytheHeroic   = 71466
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                SpellInfo const* l_Dash = sSpellMgr->GetSpellInfo(eSpells::Dash);
                if (!l_Dash)
                    return;

                /// If we have dash, we should restore it to increase movement speed
                if (AuraEffect* l_DashAura = l_Target->GetAuraEffect(eSpells::Dash, EFFECT_0))
                    l_DashAura->SetAmount(l_Dash->Effects[EFFECT_0].BasePoints);

                if (l_Target->ToPlayer()->HasGlyph(eSpells::GlyphOfCatForm) && !l_Target->HasAura(eSpells::GlyphOfCatForm))
                    l_Target->AddAura(eSpells::GlyphOfCatForm, l_Target);
            }

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Item const* l_Weapon = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                if (l_Player->HasAura(eSpells::BurningEssence) || (l_Weapon && l_Weapon->GetTemplate() && (l_Weapon->GetTemplate()->ItemId == eDatas::FandralsFlamescythe || l_Weapon->GetTemplate()->ItemId == eDatas::FandralsFlamescytheHeroic)))
                    l_Player->SetDisplayId(eSpells::BurningEssenceModel);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                if (l_Target->HasAura(eSpells::Prowl))
                    l_Target->RemoveAura(eSpells::Prowl);

                /// When we remove cat form dash shouldn't increase movement speed
                if (AuraEffect* l_DashAura = l_Target->GetAuraEffect(eSpells::Dash, EFFECT_0))
                    l_DashAura->SetAmount(0);

                if (l_Target->HasAura(eSpells::GlyphOfCatForm))
                    l_Target->RemoveAura(eSpells::GlyphOfCatForm);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_cat_form_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_dru_cat_form_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_cat_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_cat_form_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_cat_form_AuraScript();
        }
};

/// Glyph of Cat form - 47180
class spell_dru_glyph_of_cat_form : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_cat_form() : SpellScriptLoader("spell_dru_glyph_of_cat_form") { }

        class spell_dru_glyph_of_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_glyph_of_cat_form_SpellScript);

            void HandleModHealing(SpellEffIndex /*p_EffIndex*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (!l_Player || l_Player->GetShapeshiftForm() != FORM_CAT)
                    PreventHitAura();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_glyph_of_cat_form_SpellScript::HandleModHealing, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_glyph_of_cat_form_SpellScript();
        }
};

enum SkullBashSpells
{
    SPELL_DRUID_SKULL_BASH_INTERUPT = 93985
};

/// Skull Bash - 106839
class spell_dru_skull_bash: public SpellScriptLoader
{
    public:
        spell_dru_skull_bash() : SpellScriptLoader("spell_dru_skull_bash") { }

        class spell_dru_skull_bash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_skull_bash_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        l_Player->CastSpell(target, SPELL_DRUID_SKULL_BASH_INTERUPT, true);
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
    SPELL_DRUID_FAE_SILENCE                 = 114238,
    SPELL_DRUID_FAERIE_SWARM                = 102355
};

/// Faerie Swarm - 102355
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

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetShapeshiftForm() == FORM_BEAR)
                    return;

                PreventHitDamage();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_faerie_swarm_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnHit += SpellHitFn(spell_dru_faerie_swarm_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_faerie_swarm_SpellScript();
        }
};

enum eWildMushroomSpells
{
    Heal                      = 81269,
    HealAura                  = 81262,
    FungalCloudArea           = 164717,
    WildMushroomBalance       = 88747,
    WildMushroomBirthVisual   = 94081,
    WildMushroomRestoration   = 145205,
    T15RestorationBonus       = 138284
};

enum eWildMushroomDatas
{
    NpcWildMushroom = 47649
};

/// Wild Mushroom (Balance) - 88747
/// Wild Mushroom (Restoration) - 145205, Wild Mushroom (Restoration)(override by 146654) - 147349
class spell_dru_wild_mushroom: public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom() : SpellScriptLoader("spell_dru_wild_mushroom") { }

        class spell_dru_wild_mushroom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_SpellScript)

            enum eWildMushroomSpells
            {
                HealAura                        = 81262,
                FungalCloudArea                 = 164717,
                WildMushroomBalance             = 88747,
                WildMushroomBirthVisual         = 94081,
                WildMushroomRestoration         = 145205,
                WildMushroomRestorationGlyph    = 147349
            };
            
            enum eWildMushroomDatas
            {
                NpcWildMushroom = 47649,
            };

            void HandleSummon(SpellEffIndex p_EffIndex)
            {
                PreventHitDefaultEffect(p_EffIndex);

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                std::list<Creature*> l_Mushroomlist;

                l_Player->GetCreatureListWithEntryInGrid(l_Mushroomlist, eWildMushroomDatas::NpcWildMushroom, 200.0f);

                /// Remove other player mushrooms
                for (std::list<Creature*>::iterator i = l_Mushroomlist.begin(); i != l_Mushroomlist.end(); ++i)
                {
                    Unit* l_Owner = (*i)->GetOwner();
                    if (l_Owner && l_Owner == l_Player && (*i)->isSummon())
                        continue;

                    i = l_Mushroomlist.erase(i);
                }

                if (static_cast<int32>(l_Mushroomlist.size()) >= GetSpellInfo()->Effects[p_EffIndex].BasePoints)
                    l_Mushroomlist.back()->ToTempSummon()->UnSummon(); ///< 1 mushroom max for Restoration and 3 for Balance at time

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
                    l_Summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
                    l_Summon->CastSpell(l_Summon, eWildMushroomSpells::WildMushroomBirthVisual, true);
                    if (GetSpellInfo()->Id == eWildMushroomSpells::WildMushroomRestoration || GetSpellInfo()->Id == eWildMushroomSpells::WildMushroomRestorationGlyph)
                        l_Summon->CastSpell(l_Summon, eWildMushroomSpells::HealAura, true);
                    else if (GetSpellInfo()->Id == eWildMushroomSpells::WildMushroomBalance)
                        l_Summon->CastSpell(l_Summon, eWildMushroomSpells::FungalCloudArea, true);
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

/// Wild Mushroom (heal) - 81262
class spell_dru_wild_mushroom_heal : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_heal() : SpellScriptLoader("spell_dru_wild_mushroom_heal") { }

        class spell_dru_wild_mushroom_heal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_wild_mushroom_heal_AuraScript);

            enum eWildMushroomSpells
            {
                Heal = 81269
            };

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                Unit* l_Mushroom = GetCaster();
                Unit* l_Owner = l_Mushroom->GetOwner();

                if (l_Mushroom == nullptr || l_Owner == nullptr)
                    return;

                l_Owner->CastSpell(l_Mushroom, eWildMushroomSpells::Heal, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_wild_mushroom_heal_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_wild_mushroom_heal_AuraScript();
        }
};

/// Wild Mushroom proc (heal) - 81269
class spell_dru_wild_mushroom_heal_proc : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_heal_proc() : SpellScriptLoader("spell_dru_wild_mushroom_heal_proc") { }

        class spell_dru_wild_mushroom_heal_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_heal_proc_SpellScript);

            enum eWildMushroomSpells
            {
                T15RestorationBonus = 138284
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eWildMushroomSpells::T15RestorationBonus);
                uint8 l_MaxTargets = 3;

                if (l_SpellInfo != nullptr && l_Caster->HasAura(eWildMushroomSpells::T15RestorationBonus))
                    l_MaxTargets = l_SpellInfo->Effects[EFFECT_0].BasePoints;

                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(l_MaxTargets);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_mushroom_heal_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_heal_proc_SpellScript();
        }
};

enum StampedingRoarSpells
{
    SPELL_DRUID_BEAR_FORM = 5487,
    SPELL_DRUID_GLYPH_OF_STAMPEDING_ROAR = 114222
};

/// Stampeding Roar - 97993, Stampeding Roar (cat) - 77764 and Stampeding Roar(bear) - 77761
class spell_dru_stampeding_roar: public SpellScriptLoader
{
    public:
        spell_dru_stampeding_roar() : SpellScriptLoader("spell_dru_stampeding_roar") { }

        class spell_dru_stampeding_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_stampeding_roar_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (!l_Player->HasAura(SPELL_DRUID_GLYPH_OF_STAMPEDING_ROAR))
                    {
                        if (l_Player->GetShapeshiftForm() != FORM_CAT && l_Player->GetShapeshiftForm() != FORM_BEAR)
                            l_Player->CastSpell(l_Player, SPELL_DRUID_BEAR_FORM, true);
                    }

                    if (Unit* l_Target = GetHitUnit())
                        l_Target->RemoveMovementImpairingAuras();
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
    SPELL_DRUID_MANGLE = 33917,
    SPELL_DRUID_LACERATE_3_STACKS_MARKER = 158790
};

/// Lacerate - 33745
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
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (roll_chance_i(25) && l_Player->HasSpellCooldown(SPELL_DRUID_MANGLE))
                        l_Player->RemoveSpellCooldown(SPELL_DRUID_MANGLE, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_lacerate_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_lacerate_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lacerate_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster() && aurEff->GetBase()->GetStackAmount() == 3)
                    GetCaster()->CastSpell(GetTarget(), SPELL_DRUID_LACERATE_3_STACKS_MARKER, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_lacerate_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_lacerate_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_lacerate_AuraScript();
        }
};

/// Faerie Fire - 770
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
                {
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

/// Last Update 6.2.3
/// Druid of the flames - 138927
class spell_dru_druid_flames : public SpellScriptLoader
{
    public:
        spell_dru_druid_flames() : SpellScriptLoader("spell_dru_druid_flames") { }

        class spell_dru_druid_flames_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_druid_flames_AuraScript);

            enum eSpells
            {
                DruidOfFlamesModel = 38150,
                DruidOfFlames = 138927
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetShapeshiftForm() == FORM_CAT)
                    l_Player->SetDisplayId(eSpells::DruidOfFlamesModel);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetShapeshiftForm() == FORM_CAT)
                    l_Player->RestoreDisplayId();
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_druid_flames_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_druid_flames_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_druid_flames_AuraScript();
        }
};

namespace Eclipse
{
    uint32 const g_BalanceCycleTime = 40000;
    float  const g_ElipseMaxValue   = 105.0f;

    enum State
    {
        None,
        Lunar,
        Solar,
    };

    enum Spell
    {
        SoundSolar         = 65632,
        Eclipse            = 79577,
        SoundLunar         = 93333,
        VisualSolar        = 93430,
        VisualLunar        = 93431,
        LunarPeak          = 171743,
        SolarPeak          = 171744,
        Sunfire            = 163119,    ///< Trigger to override moonfire with sunfire in solar cycle
        CelestialAlignment = 112071
    };

    enum Scene
    {
        DruidBalanceCycle = 999
    };
}

/// Eclipse power handling
class spell_dru_eclipse : public PlayerScript
{
    public:
        spell_dru_eclipse() : PlayerScript("spell_dru_eclipse")
        {
        }

        struct EclipseData
        {
            bool m_EclipseCycleActive;      ///< Eclipse regen cycle is active ?
            uint8 m_LastEclipseState;       ///< Last eclipse state (reach when amount of eclipse power is >= 100)
            int8  m_EclipseFinishingCount;  ///< Reamining time the amount of eclipse power need to be at 0 before cycle finished

            float  m_LastEclipseAmount;     ///< Amount of eclipse power at last tick
            uint64 m_LastEclipseCheck;      ///< Timestamp at last tick
            uint64 m_BalanceTime;           ///< Time in millisecondes since the current eclipse cycle is started
            uint32 m_ClientUpdateTimer;     ///< Timer in millisecondes before sending update of eclipse timer to client

            uint32 m_SceneInstanceID;       ///< Scene ID for Balance Cycle

            EclipseData()
            {
                m_EclipseCycleActive = false;
                m_EclipseFinishingCount = -1;
                m_LastEclipseState = Eclipse::State::None;

                m_LastEclipseCheck = 0;
                m_BalanceTime = 0;
                m_LastEclipseAmount = 0.0f;
                m_ClientUpdateTimer = 2000;

                m_SceneInstanceID = 0;
            }
        };

        ACE_Based::LockedMap<uint32, EclipseData> m_EclipseData;

        /// This function is internal to script, not a override
        bool CanUseEclipse(Player const* p_Player)
        {
            if (m_EclipseData.find(p_Player->GetGUIDLow()) == m_EclipseData.end())
                m_EclipseData[p_Player->GetGUIDLow()] = EclipseData();

            if (p_Player == nullptr
                || p_Player->getClass() != Classes::CLASS_DRUID
                || p_Player->GetSpecializationId(p_Player->GetActiveSpec()) != SpecIndex::SPEC_DRUID_BALANCE)
                 return false;

            return true;
        }

        /// Override
        void OnEnterInCombat(Player* p_Player)
        {
            if (!CanUseEclipse(p_Player))
                return;

            EclipseData& l_EclipseData = m_EclipseData[p_Player->GetGUIDLow()];

            l_EclipseData.m_EclipseFinishingCount = -1;

            if (l_EclipseData.m_EclipseCycleActive)
                return;

            Position const* l_Pos = p_Player;
            l_EclipseData.m_SceneInstanceID = p_Player->PlayStandaloneScene(Eclipse::Scene::DruidBalanceCycle, 16, *l_Pos);

            l_EclipseData.m_BalanceTime = 0;
            l_EclipseData.m_LastEclipseAmount = 0;
            l_EclipseData.m_LastEclipseState = Eclipse::State::None;
            l_EclipseData.m_ClientUpdateTimer = 2000;

            l_EclipseData.m_EclipseCycleActive = true;
            l_EclipseData.m_EclipseFinishingCount = -1;

            ACE_OS::gettimeofday().msec(l_EclipseData.m_LastEclipseCheck);
       }

        /// Override
        void OnLeaveCombat(Player* p_Player)
        {
            if (!CanUseEclipse(p_Player))
                return;

            EclipseData& l_EclipseData = m_EclipseData[p_Player->GetGUIDLow()];
            l_EclipseData.m_EclipseFinishingCount = l_EclipseData.m_LastEclipseState == Eclipse::State::None ? 1 : 2;
        }

        /// Override
        void OnLogout(Player * p_Player)
        {
            m_EclipseData.erase(p_Player->GetGUIDLow());
        }

        /// Handle regeneration of eclipse
        /// Call at each update tick (100 ms)
        /// Override
        void OnUpdate(Player * p_Player, uint32 p_Diff)
        {
            if (!CanUseEclipse(p_Player))
                return;

            EclipseData& l_EclipseData = m_EclipseData[p_Player->GetGUIDLow()];
            if (!l_EclipseData.m_EclipseCycleActive)
                return;

            uint64 l_ActualTime = 0;
            ACE_OS::gettimeofday().msec(l_ActualTime);

            uint32 l_PowerIndex = p_Player->GetPowerIndex(Powers::POWER_ECLIPSE, p_Player->getClass());
            if (l_PowerIndex == MAX_POWERS)
                return;

            float l_FloatMultiplier = (1.0f + p_Player->GetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER + l_PowerIndex));

            /// Even if we have buff to speedup the cycle, it's freeze because of Celestial Alignment
            if (p_Player->HasAura(Eclipse::Spell::CelestialAlignment))
                l_FloatMultiplier = 0.0f;

            l_EclipseData.m_BalanceTime += (l_ActualTime - l_EclipseData.m_LastEclipseCheck) * l_FloatMultiplier;
            l_EclipseData.m_LastEclipseCheck = l_ActualTime;

            /// Eclipse regen isn't linear, it's a elipse ...
            double l_EclipseAmount = Eclipse::g_ElipseMaxValue * std::sin(2 * M_PI * l_EclipseData.m_BalanceTime / Eclipse::g_BalanceCycleTime);

            if (l_EclipseData.m_EclipseFinishingCount != -1)
            {
                if (int(l_EclipseAmount) == 0 && int(l_EclipseData.m_LastEclipseAmount) != 0)
                {
                    l_EclipseData.m_EclipseFinishingCount--;

                    if (l_EclipseData.m_EclipseFinishingCount == 0)
                    {
                        l_EclipseData.m_EclipseFinishingCount = -1;
                        l_EclipseData.m_EclipseCycleActive = false;

                        l_EclipseData.m_BalanceTime = 0;

                        p_Player->CancelStandaloneScene(l_EclipseData.m_SceneInstanceID);
                        l_EclipseData.m_SceneInstanceID = 0;
                    }
                }
            }

            l_EclipseData.m_LastEclipseAmount = l_EclipseAmount;

            /// We need to send periodic update of server-side balance timer to be sure we are sync with the client timer.
            bool l_ClientUpdate = false;
            if (l_EclipseData.m_ClientUpdateTimer <= p_Diff)
            {
                l_ClientUpdate = true;
                l_EclipseData.m_ClientUpdateTimer = 2000;
            }
            else
                l_EclipseData.m_ClientUpdateTimer -= p_Diff;

            p_Player->SetPower(Powers::POWER_ECLIPSE, l_EclipseData.m_BalanceTime % Eclipse::g_BalanceCycleTime, !l_ClientUpdate);

            /// Very usefull to debug eclipse and see server-side values
            /// Maybe we need to add a command to have it ?
            //ChatHandler(p_Player).PSendSysMessage("Eclipse : %f balance time : %u", l_EclipseAmount, m_BalanceTime % Eclipse::g_BalanceCycleTime);
        }

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 /*p_OldValue*/, int32& p_NewValue, bool /*p_Regen*/)
        {
            if (!CanUseEclipse(p_Player) || p_Power != Powers::POWER_ECLIPSE)
                return;

            EclipseData& l_EclipseData = m_EclipseData[p_Player->GetGUIDLow()];
            if (!l_EclipseData.m_EclipseCycleActive)
            {
                p_NewValue = 0;

                if (p_Player->HasAura(Eclipse::Spell::Sunfire))
                    p_Player->RemoveAurasDueToSpell(Eclipse::Spell::Sunfire);

                return;
            }

            double l_EclipseAmount = Eclipse::g_ElipseMaxValue *std::sin(2 * M_PI * p_Player->GetPower(Powers::POWER_ECLIPSE) / Eclipse::g_BalanceCycleTime);

            /// Overriding of moonfire with sunfire
            {
                if (l_EclipseAmount < 0 && !p_Player->HasAura(Eclipse::Spell::Sunfire))
                    p_Player->CastSpell(p_Player, Eclipse::Spell::Sunfire, true);
                else if (l_EclipseAmount >= 0 && p_Player->HasAura(Eclipse::Spell::Sunfire))
                    p_Player->RemoveAurasDueToSpell(Eclipse::Spell::Sunfire);
            }

            /// Visual effect (don't work ATM, need sniff / research)
            /*{
                if (l_EclipseAmount >= 90  && !p_Player->HasAura(SPELL_DRUID_ECLIPSE_VISUAL_SOLAR)) ///< Solar avantage at -90
                    p_Player->CastSpell(p_Player, SPELL_DRUID_ECLIPSE_VISUAL_SOLAR, true);
                else if (l_EclipseAmount <= 90 && !p_Player->HasAura(SPELL_DRUID_ECLIPSE_VISUAL_LUNAR)) ///< Lunar avantage at 90
                    p_Player->CastSpell(p_Player, SPELL_DRUID_ECLIPSE_VISUAL_LUNAR, true);

                if (l_EclipseAmount == 0)
                {
                    if (m_LastEclipseState == EclipseState::Solar || m_LastEclipseState == EclipseState::Lunar)
                        p_Player->CastSpell(p_Player, SPELL_DRUID_ECLIPSE_SOUND_LUNAR, true);
                    else
                        p_Player->CastSpell(p_Player, SPELL_DRUID_ECLIPSE_SOUND_SOLAR, true);
                }
            }*/

            /// Peak buffs
            {
                if (l_EclipseAmount <= -100 && l_EclipseData.m_LastEclipseState != Eclipse::State::Solar)  ///< Solar Eclipse at -100
                {
                    l_EclipseData.m_LastEclipseState = Eclipse::State::Solar;
                    p_Player->CastSpell(p_Player, Eclipse::Spell::SolarPeak, true);
                }
                else if (l_EclipseAmount >= 100 && l_EclipseData.m_LastEclipseState != Eclipse::State::Lunar)  ///< Lunar Eclipse at 100
                {
                    l_EclipseData.m_LastEclipseState = Eclipse::State::Lunar;
                    p_Player->CastSpell(p_Player, Eclipse::Spell::LunarPeak, true);
                }
            }
       }
};

/// Eclipse (damage mod) - 79577
/// Linked to the 8 spells of Nature and Arcane (Info here http://wowpedia.org/Eclipse )
class spell_dru_eclipse_mod_damage : public SpellScriptLoader
{
    public:
        spell_dru_eclipse_mod_damage() : SpellScriptLoader("spell_dru_eclipse_mod_damage") { }

        class spell_dru_eclipse_mod_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_eclipse_mod_damage_SpellScript);

            enum eSpells
            {
                Starsurge       = 78674,
                StellarFlare    = 152221,
                MasteryEclipse  = 77492
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || l_Caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;
                if (AuraEffect* l_Aura = l_Caster->GetAuraEffect(Eclipse::Spell::Eclipse, EFFECT_0))
                {
                    float l_BonusSolarSpells = 0.0f;
                    float l_BonusLunarSpells = 0.0f;
                    float l_DamageModPCT = l_Aura->GetAmount();
                    if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::MasteryEclipse, EFFECT_0))
                        l_DamageModPCT += (float)l_AurEff->GetAmount();
                    float l_Eclipse = Eclipse::g_ElipseMaxValue * std::sin(2 * M_PI * l_Caster->GetPower(Powers::POWER_ECLIPSE) / Eclipse::g_BalanceCycleTime);

                    /// Eclipse amount egal 0, each school have the same bonus
                    if ((int)l_Eclipse == 0)
                    {
                        l_BonusLunarSpells = l_DamageModPCT / 2.0f;
                        l_BonusSolarSpells = l_DamageModPCT / 2.0f;
                    }
                    /// We're in lunar phase
                    else if (l_Eclipse > 0)
                    {
                        l_BonusLunarSpells = (l_DamageModPCT / 2.0f) + CalculatePct(l_DamageModPCT / 2.0f, l_Eclipse);
                        l_BonusSolarSpells = l_DamageModPCT - l_BonusLunarSpells;
                    }
                    /// We're in solar phase
                    else if (l_Eclipse < 0)
                    {
                        l_BonusSolarSpells = (l_DamageModPCT / 2.0f) + CalculatePct(l_DamageModPCT / 2.0f, -l_Eclipse);
                        l_BonusLunarSpells = l_DamageModPCT - l_BonusSolarSpells;
                    }

                    /// Celestial alignment : All Lunar and Solar spells benefit from your maximum Eclipse bonus.
                    if (l_Caster->HasAura(Eclipse::Spell::CelestialAlignment))
                    {
                        l_BonusSolarSpells = l_DamageModPCT;
                        l_BonusLunarSpells = l_DamageModPCT;
                    }

                    int32 l_Damage = GetHitDamage();
                    int32 l_BonusDamage = 0;

                    SpellInfo const* l_SpellInfo = GetSpellInfo();
                    if (l_SpellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_NATURE)
                        l_BonusDamage += CalculatePct(GetHitDamage(), l_BonusSolarSpells);
                    else if (l_SpellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_ARCANE)
                        l_BonusDamage += CalculatePct(GetHitDamage(), l_BonusLunarSpells);

                    /// Starsurge has the two schools
                    if (l_SpellInfo->Id == eSpells::Starsurge)
                    {
                        if ((int)l_Eclipse == 0)
                            l_BonusDamage = CalculatePct(GetHitDamage(), (l_DamageModPCT / 2.0f));
                        else if (l_Eclipse > 0)
                            l_BonusDamage = CalculatePct(GetHitDamage(), l_BonusLunarSpells);
                        else if (l_Eclipse < 0)
                            l_BonusDamage = CalculatePct(GetHitDamage(), l_BonusSolarSpells);
                    }

                    /// Stellar Flare has the two schools, dealing the most damage when they are balanced (= 0)
                    if (l_SpellInfo->Id == eSpells::StellarFlare)
                    {
                        if (l_Eclipse < 0)
                            l_Eclipse *= -1;

                        l_BonusDamage = CalculatePct(GetHitDamage(), l_DamageModPCT - CalculatePct(l_DamageModPCT, l_Eclipse));
                    }

                    SetHitDamage(l_Damage + l_BonusDamage);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_eclipse_mod_damage_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_eclipse_mod_damage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_eclipse_mod_damage_AuraScript);

            enum eSpells
            {
                Starsurge       = 78674,
                StellarFlare    = 152221,
                MasteryEclipse  = 77492,
                MoonFireDamage  = 164812,
                SunFireDamage   = 164815
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || l_Caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (AuraEffect* l_Aura = l_Caster->GetAuraEffect(Eclipse::Spell::Eclipse, EFFECT_0))
                {
                    float l_BonusSolarSpells = 0.0f;
                    float l_BonusLunarSpells = 0.0f;
                    float l_DamageModPCT = l_Aura->GetAmount();

                    if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::MasteryEclipse, EFFECT_0))
                        l_DamageModPCT += l_AurEff->GetAmount();

                    float l_Eclipse = Eclipse::g_ElipseMaxValue * std::sin(2 * M_PI * l_Caster->GetPower(Powers::POWER_ECLIPSE) / Eclipse::g_BalanceCycleTime);

                    /// Eclipse amount egal 0, each school have the same bonus
                    if ((int)l_Eclipse == 0)
                    {
                        l_BonusLunarSpells = l_DamageModPCT / 2.0f;
                        l_BonusSolarSpells = l_DamageModPCT / 2.0f;
                    }
                    /// We're in lunar phase
                    else if (l_Eclipse > 0)
                    {
                        l_BonusLunarSpells = (l_DamageModPCT / 2.0f) + CalculatePct(l_DamageModPCT / 2.0f, l_Eclipse);
                        l_BonusSolarSpells = l_DamageModPCT - l_BonusLunarSpells;
                    }
                    /// We're in solar phase
                    else if (l_Eclipse < 0)
                    {
                        l_BonusSolarSpells = (l_DamageModPCT / 2.0f) + CalculatePct(l_DamageModPCT / 2.0f, -l_Eclipse);
                        l_BonusLunarSpells = l_DamageModPCT - l_BonusSolarSpells;
                    }

                    /// Celestial alignment : All Lunar and Solar spells benefit from your maximum Eclipse bonus.
                    if (l_Caster->HasAura(Eclipse::Spell::CelestialAlignment))
                    {
                        l_BonusSolarSpells = l_DamageModPCT;
                        l_BonusLunarSpells = l_DamageModPCT;
                    }

                    int32 l_Damage = p_Amount;
                    int32 l_BonusDamage = 0;

                    SpellInfo const* l_SpellInfo = GetSpellInfo();
                    if (l_SpellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_NATURE)
                        l_BonusDamage += CalculatePct(p_Amount, l_BonusSolarSpells);
                    else if (l_SpellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_ARCANE)
                        l_BonusDamage += CalculatePct(p_Amount, l_BonusLunarSpells);

                    /// Starsurge has the two schools
                    if (l_SpellInfo->Id == eSpells::Starsurge)
                    {
                        if ((int)l_Eclipse == 0)
                            l_BonusDamage = CalculatePct(p_Amount, (l_DamageModPCT / 2.0f));
                        else if (l_Eclipse > 0)
                            l_BonusDamage = CalculatePct(p_Amount, l_BonusLunarSpells);
                        else if (l_Eclipse < 0)
                            l_BonusDamage = CalculatePct(p_Amount, l_BonusSolarSpells);
                    }

                    /// Stellar Flare has the two schools, dealing the most damage when they are balanced (= 0)
                    if (l_SpellInfo->Id == eSpells::StellarFlare)
                    {
                        if (l_Eclipse < 0)
                            l_Eclipse *= -1;

                        l_BonusDamage = CalculatePct(p_Amount, l_DamageModPCT - CalculatePct(l_DamageModPCT, l_Eclipse));
                    }
                    p_Amount = l_Damage + l_BonusDamage;
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case eSpells::MoonFireDamage:
                case eSpells::SunFireDamage:
                case eSpells::StellarFlare:
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_eclipse_mod_damage_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
                    break;
                default:
                    break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_eclipse_mod_damage_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_eclipse_mod_damage_SpellScript();
        }
};

enum MoonfireSpells
{
    SPELL_DRUID_MOONFIRE_DAMAGE          = 164812,
    SPELL_DRUID_DREAM_OF_CENARIUS_TALENT = 108373,
    SPELL_DRUID_DREAM_OF_CENARIUS_RESTO  = 145153
};

enum SunfireSpells
{
    SPELL_DRUID_SUNFIRE_DAMAGE = 164815
};

namespace Sunfire
{
    enum
    {
        SpellDamage = 164815
    };
}

/// Last Update 6.2.1
/// Moonfire - 8921
class spell_dru_moonfire : public SpellScriptLoader
{
    public:
        spell_dru_moonfire() : SpellScriptLoader("spell_dru_moonfire") { }

        class spell_dru_moonfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonfire_SpellScript);

            bool m_IsLunarPeak = false;

            enum eSpells
            {
                LunarPeak = 171743,
                DreamOfCenarius = 108373,
                DreamOfCenariusHeal = 172176
            };

            void HandleBeforeCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::LunarPeak))
                    m_IsLunarPeak = true;
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, SPELL_DRUID_MOONFIRE_DAMAGE, true);

                if (l_Caster->HasAura(eSpells::DreamOfCenarius) && m_IsLunarPeak)
                    l_Caster->CastSpell(l_Caster, eSpells::DreamOfCenariusHeal, true);

                /// Celestial Alignment : causes your Moonfire and Sunfire spells to also apply the other's damage over time effect.
                if (l_Caster->HasAura(Eclipse::Spell::CelestialAlignment))
                    l_Caster->AddAura(Sunfire::SpellDamage, l_Target);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dru_moonfire_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dru_moonfire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_moonfire_SpellScript();
        }
};

/// Last Update 6.2.1
/// Sunfire - 93402
class spell_dru_sunfire : public SpellScriptLoader
{
    public:
        spell_dru_sunfire() : SpellScriptLoader("spell_dru_sunfire") { }

        class spell_dru_sunfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_sunfire_SpellScript);

            bool m_IsSolarPeak = false;

            enum eSpells
            {
                SolarPeak = 171744,
                DreamOfCenarius = 108373,
                DreamOfCenariusHeal = 172176
            };

            void HandleBeforeCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::SolarPeak))
                    m_IsSolarPeak = true;
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, Sunfire::SpellDamage, true);

                if (l_Caster->HasAura(eSpells::DreamOfCenarius) && m_IsSolarPeak)
                    l_Caster->CastSpell(l_Caster, eSpells::DreamOfCenariusHeal, true);

                /// Celestial Alignment : causes your Moonfire and Sunfire spells to also apply the other's damage over time effect.
                if (l_Caster->HasAura(Eclipse::Spell::CelestialAlignment))
                    l_Caster->AddAura(MoonfireSpells::SPELL_DRUID_MOONFIRE_DAMAGE, l_Target);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dru_sunfire_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dru_sunfire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_sunfire_SpellScript();
        }
};

/// Moonfire - 164812, Sunfire - 164815
class spell_dru_moonfire_sunfire_damage : public SpellScriptLoader
{
    public:
        spell_dru_moonfire_sunfire_damage() : SpellScriptLoader("spell_dru_moonfire_sunfire_damage") { }

        class spell_dru_moonfire_sunfire_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonfire_sunfire_damage_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(SPELL_DRUID_DREAM_OF_CENARIUS_TALENT) && l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_DRUID_RESTORATION &&
                        (l_Player->HasAura(Eclipse::Spell::LunarPeak) || l_Player->HasAura(Eclipse::Spell::SolarPeak)))
                    {
                        if (Unit* l_Target = l_Player->GetNextRandomRaidMember(15.0f))
                            l_Player->CastSpell(l_Target, SPELL_DRUID_DREAM_OF_CENARIUS_RESTO, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_moonfire_sunfire_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_moonfire_sunfire_damage_SpellScript();
        }
};

/// Dream of cenarius (heal) - 172176
class spell_dru_dream_of_cenarius_balance : public SpellScriptLoader
{
    public:
        spell_dru_dream_of_cenarius_balance() : SpellScriptLoader("spell_dru_dream_of_cenarius_balance") { }

        class spell_dru_dream_of_cenarius_balance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_dream_of_cenarius_balance_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(1);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_dream_of_cenarius_balance_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_dream_of_cenarius_balance_SpellScript();
        }
};

/// Shooting Stars - 93399
class spell_dru_shooting_stars_proc : public SpellScriptLoader
{
    public:
        spell_dru_shooting_stars_proc() : SpellScriptLoader("spell_dru_shooting_stars_proc") { }

        class spell_dru_shooting_stars_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_shooting_stars_proc_AuraScript);

            enum eSpells
            {
                Moonfire            = 8921,
                Sunfire             = 93402,
                MoonfireOverrided   = 164812,
                SunfireOverrided    = 164815,
                ShootingStars       = 93400
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                    if (l_SpellInfo == nullptr)
                        return;

                    if (l_SpellInfo->Id != eSpells::Moonfire && l_SpellInfo->Id != eSpells::Sunfire &&
                        l_SpellInfo->Id != eSpells::MoonfireOverrided && l_SpellInfo->Id != eSpells::SunfireOverrided)
                        return;

                    int32 l_Chance = p_AurEff->GetAmount();
                    if (p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
                        l_Chance *= 2;

                    if (!roll_chance_i(l_Chance))
                        return;

                    l_Caster->CastSpell(l_Caster, eSpells::ShootingStars, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_shooting_stars_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_shooting_stars_proc_AuraScript();
        }
};

/// Shooting Stars - 93400
class spell_dru_shooting_stars : public SpellScriptLoader
{
    public:
        spell_dru_shooting_stars() : SpellScriptLoader("spell_dru_shooting_stars") { }

        class spell_dru_shooting_stars_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_shooting_stars_SpellScript);

            enum eSpells
            {
                Starsurge = 78674
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                /// Shooting Stars restores 1 charge of Starsurge and Starfall. (share same charges category)
                if (SpellInfo const* l_Starsurge = sSpellMgr->GetSpellInfo(eSpells::Starsurge))
                    l_Player->RestoreCharge(l_Starsurge->ChargeCategoryEntry);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_shooting_stars_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_shooting_stars_SpellScript();
        }
};

/// T10 Restoration 4P - 70691
class spell_dru_t10_restoration_4p_bonus: public SpellScriptLoader
{
    public:
        spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

        class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
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
                        if ((*itr)->IsPlayer() && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
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
    SPELL_DRUID_MOONFIRE              = 8921,
    SPELL_DRUID_SUNFIRE               = 93402,
    SPELL_DRUID_GLYPH_OF_UNTAMED_STARS = 146655
};

/// Last Update 6.2.1
/// Starfall (triggered) - 50286 and Sunfall (triggered) - 155598
class spell_dru_starfall_dummy: public SpellScriptLoader
{
    public:
        spell_dru_starfall_dummy() : SpellScriptLoader("spell_dru_starfall_dummy") { }

        class spell_dru_starfall_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starfall_dummy_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Unit* l_Target = GetHitUnit();
                if (!l_Target)
                    return;

                /// Shapeshifting into an animal form or mounting cancels the effect
                if (l_Caster->GetCreatureType() == CREATURE_TYPE_BEAST || l_Caster->IsMounted())
                {
                    if (SpellInfo const* l_SpellInfo = GetTriggeringSpell())
                        l_Caster->RemoveAurasDueToSpell(l_SpellInfo->Id);
                    return;
                }

                /// Any effect which causes you to lose control of your character will supress the starfall effect.
                if (l_Caster->HasUnitState(UNIT_STATE_CONTROLLED))
                    return;

                /// Can't hit a target in stealth
                if (l_Target->HasAuraType(SPELL_AURA_MOD_STEALTH))
                    return;

                /// Can't hit a target in LoS
                if (!l_Target->IsWithinLOSInMap(l_Caster))
                    return;

                /// Starfall can only hit targets with Moonfire or Sunfire by default
                /// Glyph of Untamed Stars expands Starfall to hit all targets within range.
                if (!l_Caster->HasAura(SPELL_DRUID_GLYPH_OF_UNTAMED_STARS))
                {
                    if (!l_Target->HasAura(SPELL_DRUID_MOONFIRE_DAMAGE, l_Caster->GetGUID()) && !l_Target->HasAura(SPELL_DRUID_SUNFIRE_DAMAGE, l_Caster->GetGUID()))
                        return;
                }

                l_Caster->CastSpell(l_Target, uint32(GetEffectValue()), true);
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

/// Savage Roar - 52610
class spell_dru_savage_roar: public SpellScriptLoader
{
    public:
        spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

        class spell_dru_savage_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_roar_SpellScript);

            int32 m_ComboPoint = 0;
            int32 m_OldDuration = 0;

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster->GetShapeshiftForm() != FORM_CAT)
                    return SPELL_FAILED_ONLY_SHAPESHIFT;

                return SPELL_CAST_OK;
            }

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                m_ComboPoint = l_Caster->GetPower(Powers::POWER_COMBO_POINT);

                if (Aura* l_SavageRoar = l_Caster->GetAura(GetSpellInfo()->Id))
                    m_OldDuration = l_SavageRoar->GetDuration();
            }


            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;
                
                int32 l_BonusDuration = m_ComboPoint * 6 * IN_MILLISECONDS;
                int32 l_MaxDuration = GetSpellInfo()->GetMaxDuration();
                int32 l_NewDuration = m_OldDuration + GetSpellInfo()->GetDuration() + l_BonusDuration;

                if (Aura* l_SavageRoar = l_Caster->GetAura(GetSpellInfo()->Id))
                {
                    /// Can't be more then 130% of max duration
                    if (l_NewDuration > l_MaxDuration)
                        l_NewDuration = l_MaxDuration;

                    l_SavageRoar->SetDuration(l_NewDuration);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
                OnPrepare += SpellOnPrepareFn(spell_dru_savage_roar_SpellScript::HandleOnPrepare);
                AfterHit += SpellHitFn(spell_dru_savage_roar_SpellScript::HandleAfterHit);
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

            void AfterApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, SPELL_DRUID_SAVAGE_ROAR_CAST, true, NULL, p_AurEff, GetCasterGUID());
            }

            void OnUpdate(uint32 /*p_Diff*/, AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->GetShapeshiftForm() == FORM_CAT && !l_Caster->HasAura(SPELL_DRUID_SAVAGE_ROAR_CAST))
                    l_Caster->CastSpell(l_Caster, SPELL_DRUID_SAVAGE_ROAR_CAST, true, NULL, p_AurEff, GetCasterGUID());
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR_CAST);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_savage_roar_AuraScript::OnUpdate, EFFECT_1, SPELL_AURA_DUMMY);
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

/// Survival Instincts - 61336
class spell_dru_survival_instincts: public SpellScriptLoader
{
    public:
        spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

        class spell_dru_survival_instincts_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_DRUID_SURVIVAL_INSTINCTS, true);
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

enum SwiftmendSpells
{
    SPELL_DRUID_RAMPANTH_GROWTH   = 155834,
    SPELL_DRUID_REGROWTH          = 8936
};

/// Last Update 6.2.3
/// Swiftmend - 18562
class spell_dru_swiftmend: public SpellScriptLoader
{
    public:
        spell_dru_swiftmend() : SpellScriptLoader("spell_dru_swiftmend") { }

        class spell_dru_swiftmend_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_swiftmend_SpellScript);

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                /// Restoration soul of the forest - 114108
                if (l_Caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO_TALENT))
                    l_Caster->CastSpell(l_Caster, SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO, false);
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    int32 l_HealAmount = GetHitHeal() + CalculatePct(l_Caster->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL), GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                    SetHitHeal(l_HealAmount);
                }
            }

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_DRUID_RAMPANTH_GROWTH))
                    {
                        if (Unit* l_Target = GetHitUnit())
                        {
                            Aura* l_Aura = l_Target->GetAura(SPELL_DRUID_REGROWTH, l_Caster->GetGUID());
                            if (!l_Aura)
                                l_Aura = l_Target->GetAura(SPELL_DRUID_REJUVENATION, l_Caster->GetGUID());

                            if (l_Aura)
                                l_Aura->Remove();
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dru_swiftmend_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_dru_swiftmend_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_dru_swiftmend_SpellScript::HandleAfterHit);
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

    /// Extra spells
    SPELL_COLD_WEATHER_FLYING      = 54197,
    SPELL_MASTER_FLYING            = 90265,
    SPELL_FLIGHT_MASTERS_LICENSE   = 90267,
    SPELL_WISDOM_OF_THE_FOUR_WINDS = 115913
};

/// Called by the cancel of Aquatic Form - 1066, Swift Flight Form - 40120
/// Flight Form - 33943 and Stag Form - 165961
/// Shapeshift removal handling
class spell_dru_travel_form_removal : public SpellScriptLoader
{
    public:
        spell_dru_travel_form_removal() : SpellScriptLoader("spell_dru_travel_form_removal") { }

        class spell_dru_travel_form_removal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_travel_form_removal_AuraScript);

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                /// This should only be called by client, if player cancel aura itself
                /// Or if client send CMSG_CANCEL_AURA when casting incompatible spell with Shapeshift
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_CANCEL)
                    l_Target->RemoveAura(SPELL_DRUID_TRAVEL_FORM);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_removal_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_travel_form_removal_AuraScript();
        }
};

/// Travel form - 783
class spell_dru_travel_form: public SpellScriptLoader
{
    public:
        spell_dru_travel_form() : SpellScriptLoader("spell_dru_travel_form") { }

        class spell_dru_travel_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_travel_form_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (!l_Player->GetMap()->IsOutdoors(l_Player->GetPositionX(), l_Player->GetPositionY(), l_Player->GetPositionZ()))
                        return SPELL_FAILED_ONLY_OUTDOORS;
                }

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

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // Call the player script "spell_dru_travel_form_playerscript" below to avoid code duplication
                if (GetTarget()->IsPlayer())
                    sScriptMgr->OnPlayerUpdateMovement(GetTarget()->ToPlayer());
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

        void OnUpdateMovement(Player* p_Player)
        {
            if (!p_Player || p_Player->getClass() != CLASS_DRUID || !p_Player->HasAura(SPELL_DRUID_TRAVEL_FORM))
                return;

            if (p_Player->IsInWater() && p_Player->GetShapeshiftForm() != FORM_AQUA)
                p_Player->CastSpell(p_Player, SPELL_DRUID_AQUATIC_FORM, true);
            else if (!p_Player->IsInWater() && p_Player->getLevel() >= 71 && CheckIfCanFlyInLoc(p_Player) && !p_Player->isInCombat() &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT_EPIC && !p_Player->HasAura(SPELL_DRUID_GLYPH_OF_THE_STAG))
                p_Player->CastSpell(p_Player, SPELL_DRUID_SWIFT_FLIGHT_FORM, true);
            else if (!p_Player->IsInWater() && p_Player->getLevel() >= 60 && CheckIfCanFlyInLoc(p_Player) && !p_Player->isInCombat() &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT_EPIC && p_Player->GetShapeshiftForm() != FORM_FLIGHT && !p_Player->HasAura(SPELL_DRUID_GLYPH_OF_THE_STAG))
                p_Player->CastSpell(p_Player, SPELL_DRUID_FLIGHT_FORM, true);
            else if (!p_Player->IsInWater() && !p_Player->IsFlying() && p_Player->GetShapeshiftForm() != FORM_FLIGHT_EPIC &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT && p_Player->GetShapeshiftForm() != FORM_STAG)
                p_Player->CastSpell(p_Player, SPELL_DRUID_STAG_FORM, true);
        }

        void OnChangeShapeshift(Player* p_Player, ShapeshiftForm p_Form)
        {
            if (!p_Player || p_Player->getClass() != CLASS_DRUID)
                return;

            if (p_Player->HasAura(SPELL_DRUID_TRAVEL_FORM) && !p_Player->IsTravelForm(p_Form))
                p_Player->RemoveAura(SPELL_DRUID_TRAVEL_FORM);
        }
};

enum GlyphOfTheShapemender
{
    SPELL_DRUID_GLYPH_OF_THE_SHAPEMENDER_AURA = 159453,
    SPELL_DRUID_GLYPH_OF_THE_SHAPEMENDER = 159454
};

/// 159453 - Glyph of the Shapemender
class spell_dru_glyph_of_the_shapemender_playerscript : public PlayerScript
{
    public:
        spell_dru_glyph_of_the_shapemender_playerscript() : PlayerScript("spell_dru_glyph_of_the_shapemender_playerscript") {}

        void OnChangeShapeshift(Player* p_Player, ShapeshiftForm /*p_Form*/)
        {
            if (!p_Player || p_Player->getClass() != CLASS_DRUID)
                return;

            if (p_Player->HasAura(SPELL_DRUID_GLYPH_OF_THE_SHAPEMENDER_AURA) && p_Player->HasAuraType(SPELL_AURA_MOD_SHAPESHIFT) && !p_Player->HasSpellCooldown(SPELL_DRUID_GLYPH_OF_THE_SHAPEMENDER))
            {
                p_Player->CastSpell(p_Player, SPELL_DRUID_GLYPH_OF_THE_SHAPEMENDER, true);
                p_Player->AddSpellCooldown(SPELL_DRUID_GLYPH_OF_THE_SHAPEMENDER, 0, 5 * IN_MILLISECONDS); ///< This effect cannot occur more often than once every 5 sec.
            }
        }
};

/// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive: public SpellScriptLoader
{
    public:
        spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

        class spell_dru_swift_flight_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
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

/// 114338 - Glyph of the Stag
class spell_dru_glyph_of_the_stag: public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_the_stag() : SpellScriptLoader("spell_dru_glyph_of_the_stag") { }

        class spell_dru_glyph_of_the_stag_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_the_stag_AuraScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

/// 159456 - Glyph of Travel
/// Called by Travel Form - 165961
class spell_dru_glyph_of_travel: public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_travel() : SpellScriptLoader("spell_dru_glyph_of_travel") { }

        class spell_dru_glyph_of_travel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_travel_AuraScript);

            enum eSpells
            {
                GlyphOfTravel = 159456
            };

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                if (AuraEffect* l_GlyphOfTravel = l_Caster->GetAuraEffect(eSpells::GlyphOfTravel, EFFECT_0))
                    if (!l_Caster->isInCombat() && !l_Player->InBattleground() && !l_Player->InArena())
                        amount += l_GlyphOfTravel->GetAmount();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_glyph_of_travel_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_SPEED_ALWAYS);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_glyph_of_travel_AuraScript();
        }
};

enum SpellsRake
{
    SPELL_DRU_RAKE_STUNT = 163505,
    SPELL_DRU_GLYPH_OF_SAVAGE_ROAR = 127540,
    SPELL_DRU_IMPROVED_RAKE = 157276,
    SPELL_DRU_BLOODTALONS = 145152
};

/// last update : 6.2.3
/// Rake - 1822
class spell_dru_rake: public SpellScriptLoader
{
    public:
        spell_dru_rake() : SpellScriptLoader("spell_dru_rake") { }

        class spell_dru_rake_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rake_SpellScript);

            enum eSpells
            {
                Prowl           = 5215,
                GlyphOfRake     = 54821,
                KingOfTheJungle = 102543
            };

            bool m_isStealthedOrKingOfTheJungle = false;

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::KingOfTheJungle) || l_Caster->HasStealthAura())
                    m_isStealthedOrKingOfTheJungle = true;

                if (Aura* l_ImprovedRake = l_Caster->GetAura(SPELL_DRU_IMPROVED_RAKE))
                {
                    if (m_isStealthedOrKingOfTheJungle)
                        l_ImprovedRake->GetEffect(EFFECT_1)->SetAmount(1);
                    else
                        l_ImprovedRake->GetEffect(EFFECT_1)->SetAmount(0);
                }
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                if (!l_Caster || !l_Target)
                    return;

                if (Aura* l_BloodTalons = l_Caster->GetAura(SPELL_DRU_BLOODTALONS)) ///< Charge is drop on Rake periodic damage
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_BloodTalons->GetEffect(EFFECT_0)->GetAmount()));

                if (Aura* l_ImprovedRake = l_Caster->GetAura(SPELL_DRU_IMPROVED_RAKE))
                {
                    if (m_isStealthedOrKingOfTheJungle)
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_ImprovedRake->GetEffect(EFFECT_0)->GetAmount()));
                }

                if (m_isStealthedOrKingOfTheJungle)
                {
                    l_Caster->CastSpell(l_Target, SPELL_DRU_RAKE_STUNT, true);

                    if (AuraEffect const* l_GlyphOfSavageRoar = l_Caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_SAVAGE_ROAR, EFFECT_0))
                        if (Aura* l_SavageRoar = l_Caster->AddAura(SPELL_DRUID_SAVAGE_ROAR, l_Caster))
                            l_SavageRoar->SetDuration((l_GlyphOfSavageRoar->GetAmount() * 6 * IN_MILLISECONDS) + 12 * IN_MILLISECONDS);
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

/// last update : 6.1.2 19802
/// Rake (triggered) - 155722
class spell_dru_rake_triggered : public SpellScriptLoader
{
public:
    spell_dru_rake_triggered() : SpellScriptLoader("spell_dru_rake_triggered") { }

    class spell_dru_rake_triggered_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rake_triggered_AuraScript);

        void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
        {
            Unit* l_Caster = GetCaster();

            if (l_Caster == nullptr)
                return;


            if (Aura* l_ImprovedRake = l_Caster->GetAura(SPELL_DRU_IMPROVED_RAKE))
            {
                if (l_ImprovedRake->GetEffect(1)->GetAmount() == 1)
                {
                    int32 l_Amount = p_Amount;
                    l_Amount = (l_Amount + CalculatePct(l_Amount, l_ImprovedRake->GetEffect(0)->GetAmount()));
                    p_Amount = l_Amount;
                    l_ImprovedRake->GetEffect(1)->SetAmount(0);
                }
            }

            if (Aura* l_BloodTalons = l_Caster->GetAura(SPELL_DRU_BLOODTALONS))
            {
                p_Amount += CalculatePct(p_Amount, l_BloodTalons->GetEffect(EFFECT_0)->GetAmount());
                l_BloodTalons->DropCharge();
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rake_triggered_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_rake_triggered_AuraScript();
    }
};

enum SpellsBarkskin
{
    SPELL_DRUID_GLYPH_OF_BARKSKIN_AURA  = 63057,
    SPELL_DRUID_GLYPH_OF_BARKSKIN       = 63058,
    SPELL_DRUID_GLYPH_OF_ENCHANTED_BARK_AURA = 159436,
    SPELL_DRUID_GLYPH_OF_ENCHANTED_BARK = 159438
};

/// Call by Barkskin - 22812
/// Glyph of Barkskin - 114338
class spell_dru_glyph_of_barkskin : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_barkskin() : SpellScriptLoader("spell_dru_glyph_of_barkskin") { }

        class spell_dru_glyph_of_barkskin_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_barkskin_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(SpellsBarkskin::SPELL_DRUID_GLYPH_OF_BARKSKIN_AURA))
                    l_Caster->CastSpell(l_Caster, SpellsBarkskin::SPELL_DRUID_GLYPH_OF_BARKSKIN, true);
                if (l_Caster->HasAura(SpellsBarkskin::SPELL_DRUID_GLYPH_OF_ENCHANTED_BARK_AURA))
                    l_Caster->CastSpell(l_Caster, SpellsBarkskin::SPELL_DRUID_GLYPH_OF_ENCHANTED_BARK, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(SpellsBarkskin::SPELL_DRUID_GLYPH_OF_BARKSKIN))
                    l_Caster->RemoveAura(SpellsBarkskin::SPELL_DRUID_GLYPH_OF_BARKSKIN);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_barkskin_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_barkskin_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_glyph_of_barkskin_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Shred - 5221
class spell_dru_shred: public SpellScriptLoader
{
    public:
        spell_dru_shred() : SpellScriptLoader("spell_dru_shred") { }

        class spell_dru_shred_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_shred_SpellScript);

            enum eSpells
            {
                Prowl           = 5215,
                KingOfTheJungle = 102543,
                Swipe           = 106785
            };

            bool m_isStealthedOrKingOfTheJungle = false;

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::KingOfTheJungle) || l_Caster->HasStealthAura())
                    m_isStealthedOrKingOfTheJungle = true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                int32 l_Damage = GetHitDamage();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::Prowl);

                if (l_Target == nullptr)
                    return;

                if (m_isStealthedOrKingOfTheJungle)
                {
                    if (l_SpellInfo != nullptr)
                        l_Damage += CalculatePct(l_Damage, l_SpellInfo->Effects[EFFECT_3].BasePoints);

                    if (AuraEffect const* l_GlyphOfSavageRoar = l_Caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_SAVAGE_ROAR, EFFECT_0))
                        if (Aura* l_SavageRoar = l_Caster->AddAura(SPELL_DRUID_SAVAGE_ROAR, l_Caster))
                            l_SavageRoar->SetDuration((l_GlyphOfSavageRoar->GetAmount() * 6 * IN_MILLISECONDS) + 12 * IN_MILLISECONDS);
                }

                if (l_Target && l_Target->HasAuraState(AURA_STATE_BLEEDING) && sSpellMgr->GetSpellInfo(eSpells::Swipe))
                    l_Damage += CalculatePct(l_Damage, sSpellMgr->GetSpellInfo(eSpells::Swipe)->Effects[EFFECT_1].BasePoints);

                SetHitDamage(l_Damage);
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_shred_SpellScript::HandleOnPrepare);
                OnEffectHitTarget += SpellEffectFn(spell_dru_shred_SpellScript::HandleDamage, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_shred_SpellScript();
        }
};

enum SpellsFerociousBite
{
    SPELL_DRUID_RAKE_TRIGGERED = 155722,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE = 67598,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL = 101024,
    SPELL_DRUID_BERSEK = 106951,
    SPELL_DRUID_RIP = 1079
};

/// Last Update 6.2.3
/// Ferocious Bite - 22568
class spell_dru_ferocious_bite: public SpellScriptLoader
{
    public:
        spell_dru_ferocious_bite() : SpellScriptLoader("spell_dru_ferocious_bite") { }

        class spell_dru_ferocious_bite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_ferocious_bite_SpellScript);

            int32 m_SpellCost = 25;
            int32 m_EnergyConsumedExtra = 0;
            bool m_IsFreeCost = false;

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(SpellsFerociousBite::SPELL_DRUID_BERSEK))
                    m_SpellCost = 12;

                if (l_Caster->HasAura(135700))
                    m_IsFreeCost = true;
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                int32 l_Damage = GetHitDamage();


                /// Prevent double call
                if (l_Damage == 0)
                    return;

                if (l_Caster->IsPlayer())
                    l_Damage = (l_Damage / 5) * l_Caster->GetPower(Powers::POWER_COMBO_POINT);

                /// converts each extra point of energy ( up to 25 energy ) into additional damage
                int32 l_EnergyConsumed = l_Caster->GetPower(POWER_ENERGY) > m_SpellCost ? m_SpellCost : l_Caster->GetPower(POWER_ENERGY);

                AddPct(l_Damage, l_EnergyConsumed * (100 / m_SpellCost));
                SetHitDamage(l_Damage);

                m_EnergyConsumedExtra = l_EnergyConsumed;
                /// Glyph of Ferocious Bite
                if (Aura* l_GlyphOfFerociousBite = l_Caster->GetAura(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE))
                {
                    l_EnergyConsumed += m_SpellCost; ///< Add the basic cost of Ferocious Bite to the extra cost;
                    int l_HealPct = (l_GlyphOfFerociousBite->GetEffect(EFFECT_0)->GetAmount() * floor(l_EnergyConsumed / 10) / 10);
                    l_Caster->CastCustomSpell(l_Caster, SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL, &l_HealPct, 0, 0, true);
                }

                /// if target is under 25% of life, also reset rake duration
                if (l_Target && l_Target->GetHealthPct() <= 25.0f)
                if (Aura* l_Rip = l_Target->GetAura(SPELL_DRUID_RIP))
                    l_Rip->RefreshDuration();
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                if (m_IsFreeCost)
                {
                    l_Caster->SetPower(POWER_ENERGY, l_Caster->GetPower(POWER_ENERGY) + m_EnergyConsumedExtra);
                    m_IsFreeCost = false;
                }
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_ferocious_bite_SpellScript::HandleOnPrepare);
                OnHit += SpellHitFn(spell_dru_ferocious_bite_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_dru_ferocious_bite_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_ferocious_bite_SpellScript();
        }
};

/// Frenzied Regeneration - 22842
class spell_dru_frenzied_regeneration: public SpellScriptLoader
{
    public:
        spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration") { }

        class spell_dru_frenzied_regeneration_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_frenzied_regeneration_SpellScript)

            int32 m_RageForSpell;

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster)
                    m_RageForSpell = l_Caster->GetPower(POWER_RAGE) > GetSpellInfo()->Effects[EFFECT_1].BasePoints * l_Caster->GetPowerCoeff(POWER_RAGE) ? GetSpellInfo()->Effects[EFFECT_1].BasePoints * l_Caster->GetPowerCoeff(POWER_RAGE) : l_Caster->GetPower(POWER_RAGE);

                return SPELL_CAST_OK;
            }

            void HandleHeal(SpellEffIndex p_EffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                /// Maximum we can reach (attack power * 6) health by 60 rage
                int32 l_AttackPower = l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * GetSpellInfo()->Effects[EFFECT_0].AttackPowerMultiplier;
                /// For every 1 rage we reach ((attack power * 6) / spent rage) health
                float l_AttackPowerPerRage = l_AttackPower / GetSpellInfo()->Effects[EFFECT_1].BasePoints;
                /// Calculate our heal, according to spent rage
                int32 l_Heal = l_AttackPowerPerRage * (m_RageForSpell / l_Caster->GetPowerCoeff(POWER_RAGE));

                l_Heal = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_Heal, p_EffIndex, HEAL);
                l_Heal = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_Heal, HEAL);

                SetHitHeal(l_Heal);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->ModifyPower(POWER_RAGE, -m_RageForSpell);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_frenzied_regeneration_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_frenzied_regeneration_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
                OnEffectHitTarget += SpellEffectFn(spell_dru_frenzied_regeneration_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_frenzied_regeneration_SpellScript();
        }
};

/// last update : 6.2.3
/// Rip - 1079
class spell_dru_rip: public SpellScriptLoader
{
    public:
        spell_dru_rip() : SpellScriptLoader("spell_dru_rip") { }

        class spell_dru_rip_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_rip_AuraScript);

            uint32 m_PreviousTick = 0;

            void OnReApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0, l_Caster->GetGUID()))
                    m_PreviousTick = ((l_AurEff->GetAmount() * (l_AurEff->GetBase()->GetDuration() / l_AurEff->GetAmplitude())) / (p_AurEff->GetBase()->GetMaxDuration() / p_AurEff->GetAmplitude()));

            }

            void AfterReApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0, l_Caster->GetGUID()))
                    l_AurEff->SetAmount(l_AurEff->GetAmount() + m_PreviousTick);
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                int32 l_Combo = l_Caster->GetPower(Powers::POWER_COMBO_POINT);

                if (l_Combo > 0)
                    p_Amount *= l_Combo;

                if (Aura* l_BloodTalons = l_Caster->GetAura(SPELL_DRU_BLOODTALONS))
                {
                    p_Amount += CalculatePct(p_Amount, l_BloodTalons->GetEffect(EFFECT_0)->GetAmount());
                    l_BloodTalons->DropCharge();
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_rip_AuraScript::OnReApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_rip_AuraScript::AfterReApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
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

/// Dream of Cenarius - 158501
class spell_dru_dream_of_cenarius: public SpellScriptLoader
{
    public:
        spell_dru_dream_of_cenarius() : SpellScriptLoader("spell_dru_dream_of_cenarius") { }

        class spell_dru_dream_of_cenarius_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_dream_of_cenarius_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
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

/// Primal Fury - 16961 159286
class spell_dru_primal_fury: public SpellScriptLoader
{
    public:
        spell_dru_primal_fury() : SpellScriptLoader("spell_dru_primal_fury") { }

        class spell_dru_primal_fury_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_primal_fury_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
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

enum EntanglingEnergySpells
{
    SPELL_DRUID_ENTANGLING_ROOTS = 339,
    SPELL_DRUID_ENTANGLING_ENERGY = 164991
};

enum BalanceOfPowerSpells
{
    SPELL_DRUID_BALANCE_OF_POWER = 152220,
    SPELL_DRUID_STARFIRE = 2912,
    SPELL_DRUID_WRATH = 5176
};

/// Call by Starfire - 2912, Wrath - 5176
/// Entangling Energy - 164991
class spell_dru_entangling_energy : public SpellScriptLoader
{
    public:
        spell_dru_entangling_energy() : SpellScriptLoader("spell_dru_entangling_energy") { }

        class spell_dru_entangling_energy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_entangling_energy_SpellScript);

            enum eSpells
            {
                T17Balance4P        = 166677,
                CelestialAlignment  = 112071
            };

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    /// Wrath and Starfire casts reduce the cooldown of Celestial Alignment by 3 sec.
                    if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::T17Balance4P, EFFECT_0))
                        l_Caster->ToPlayer()->ReduceSpellCooldown(eSpells::CelestialAlignment, l_AurEff->GetAmount());
                }
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Caster->HasAura(EntanglingEnergySpells::SPELL_DRUID_ENTANGLING_ENERGY))
                {
                    l_Caster->CastSpell(l_Target, EntanglingEnergySpells::SPELL_DRUID_ENTANGLING_ROOTS, true);
                    l_Caster->RemoveAura(EntanglingEnergySpells::SPELL_DRUID_ENTANGLING_ENERGY);
                }

                /// Balance of Power - 152220
                if (l_Caster->HasAura(BalanceOfPowerSpells::SPELL_DRUID_BALANCE_OF_POWER))
                {
                    SpellInfo const* l_BalanceOfPowerSpells = sSpellMgr->GetSpellInfo(BalanceOfPowerSpells::SPELL_DRUID_BALANCE_OF_POWER);
                    /// Your Starfires extend the duration of Moonfire by 6 sec.
                    if (GetSpellInfo()->Id == BalanceOfPowerSpells::SPELL_DRUID_STARFIRE)
                    {
                        if (Aura* l_Moonfire = l_Target->GetAura(SPELL_DRUID_MOONFIRE_DAMAGE, l_Caster->GetGUID()))
                        {
                            int32 l_CurrentDuration = l_Moonfire->GetDuration();
                            int32 l_MaxDuration = l_Moonfire->GetMaxDuration();
                            int32 l_NewDuration = l_CurrentDuration + l_BalanceOfPowerSpells->Effects[0].BasePoints * IN_MILLISECONDS;

                            if (l_NewDuration > l_MaxDuration)
                                l_NewDuration = l_MaxDuration;

                            l_Moonfire->SetDuration(l_NewDuration);
                        }
                    }
                    /// Your Wraths extend the duration of Sunfire by 4 sec.
                    else if (GetSpellInfo()->Id == BalanceOfPowerSpells::SPELL_DRUID_WRATH)
                    {
                        if (Aura* l_Sunfire = l_Target->GetAura(SPELL_DRUID_SUNFIRE_DAMAGE, l_Caster->GetGUID()))
                        {
                            int32 l_CurrentDuration = l_Sunfire->GetDuration();
                            int32 l_MaxDuration = l_Sunfire->GetMaxDuration();
                            int32 l_NewDuration = l_CurrentDuration + l_BalanceOfPowerSpells->Effects[1].BasePoints * IN_MILLISECONDS;

                            if (l_NewDuration > l_MaxDuration)
                                l_NewDuration = l_MaxDuration;

                            l_Sunfire->SetDuration(l_NewDuration);
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dru_entangling_energy_SpellScript::HandleAfterCast);
                AfterHit += SpellHitFn(spell_dru_entangling_energy_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_entangling_energy_SpellScript();
        }
};

enum UrsaMajor
{
    SPELL_DRU_URSA_MAJOR_PROC   = 159233,
    SPELL_DRU_MANGLE            = 33917,
    SPELL_DRU_LACERATE          = 33745

};

/// Ursa Major - 159232
class spell_dru_ursa_major : public SpellScriptLoader
{
    public:
        spell_dru_ursa_major() : SpellScriptLoader("spell_dru_ursa_major") { }

        class spell_dru_ursa_major_Aurascript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursa_major_Aurascript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_ProcInfos)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (p_ProcInfos.GetDamageInfo()->GetSpellInfo() != nullptr
                    && p_ProcInfos.GetDamageInfo()->GetSpellInfo()->Id != UrsaMajor::SPELL_DRU_MANGLE
                    && p_ProcInfos.GetDamageInfo()->GetSpellInfo()->Id != UrsaMajor::SPELL_DRU_LACERATE)
                    return;

                if (!(p_ProcInfos.GetHitMask() & PROC_EX_INTERNAL_MULTISTRIKE))
                    return;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_DRU_URSA_MAJOR_PROC);

                if (l_SpellInfo == nullptr || !l_SpellInfo->GetDuration())
                    return;

                /// Increases your maximum health by 2 % for 25 sec
                l_Caster->CastSpell(l_Caster, UrsaMajor::SPELL_DRU_URSA_MAJOR_PROC, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_ursa_major_Aurascript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_ursa_major_Aurascript();
        }
};

/// Ursa Major (aura) - 159233
class spell_dru_ursa_major_aura : public SpellScriptLoader
{
    public:
        spell_dru_ursa_major_aura() : SpellScriptLoader("spell_dru_ursa_major_aura") { }

        class spell_dru_ursa_major_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursa_major_aura_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->AddToStackOnDuration(GetSpellInfo()->Id, GetSpellInfo()->GetMaxDuration(), GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                StackOnDuration* l_Stack = l_Caster->GetStackOnDuration(GetSpellInfo()->Id);

                if (l_Stack == nullptr)
                    return;

                GetEffect(EFFECT_0)->SetAmount(l_Stack->GetTotalAmount());
            }

            void OnUpdate(uint32 /*p_Diff*/, AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                StackOnDuration* l_Stack = l_Caster->GetStackOnDuration(GetSpellInfo()->Id);

                if (l_Stack == nullptr)
                    return;

                if (p_AurEff->GetAmount() > l_Stack->GetTotalAmount())
                {
                    float l_Percent = l_Caster->GetHealthPct();
                    l_Caster->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, p_AurEff->GetAmount(), false);
                    l_Caster->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, l_Stack->GetTotalAmount(), true);
                    if (l_Caster->isAlive())
                        l_Caster->SetHealth(l_Caster->CountPctFromMaxHealth(int32(l_Percent)));
                }

                p_AurEff->SetAmount(l_Stack->GetTotalAmount());
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->RemoveStackOnDuration(GetSpellInfo()->Id);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_ursa_major_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY));
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_ursa_major_aura_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_ursa_major_aura_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_ursa_major_aura_AuraScript();
        }
};

enum HealingTouchSpells
{
    SPELL_DRU_BLOODTALONS_MOD_DAMAGE = 145152,
    SPELL_DRU_BLOODTALONS_TALENT = 155672
};

/// Last Uptade 6.1.2
/// Healing Touch - 5185
class spell_dru_healing_touch: public SpellScriptLoader
{
    public:
        spell_dru_healing_touch() : SpellScriptLoader("spell_dru_healing_touch") { }

        class spell_dru_healing_touch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_healing_touch_SpellScript);

            enum eSpells
            {
                BearForm = 5487
            };

            void HandleOnPrepare()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                /// Healing Touch no longer cancels Bear Form for Guardian Druids.
                if (l_Player->HasAura(eSpells::BearForm) && l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SPEC_DRUID_GUARDIAN)
                    l_Player->RemoveAura(eSpells::BearForm);
            }

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasAura(SPELL_DRU_BLOODTALONS_TALENT))
                    l_Caster->CastSpell(l_Caster, SPELL_DRU_BLOODTALONS_MOD_DAMAGE, true);
            }

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
                    l_Caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_healing_touch_SpellScript::HandleOnPrepare);
                OnCast += SpellCastFn(spell_dru_healing_touch_SpellScript::HandleOnCast);
                AfterCast += SpellCastFn(spell_dru_healing_touch_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_healing_touch_SpellScript();
        }
};

/// Last Update - 6.1.2 19802
/// Starsurge - 78674
class spell_dru_starsurge : public SpellScriptLoader
{
    public:
        spell_dru_starsurge() : SpellScriptLoader("spell_dru_starsurge") { }

        class spell_dru_starsurge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starsurge_SpellScript);

            enum eSpells
            {
                LunarEmpowerment = 164547,
                SolarEmpowerment = 164545
            };

            void HandleBeforeCast()
            {
                Unit* l_Caster = GetCaster();

                double l_EclipseAmount = Eclipse::g_ElipseMaxValue * std::sin(2 * M_PI * l_Caster->GetPower(Powers::POWER_ECLIPSE) / Eclipse::g_BalanceCycleTime);

                if (l_EclipseAmount < 0)
                    l_Caster->CastSpell(l_Caster, eSpells::SolarEmpowerment, true);
                else if (l_EclipseAmount >= 0)
                    l_Caster->CastSpell(l_Caster, eSpells::LunarEmpowerment, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dru_starsurge_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_starsurge_SpellScript();
        }
};

/// Glyph of the Shapemender - 159453
class spell_dru_glyph_of_the_shapemender : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_the_shapemender() : SpellScriptLoader("spell_dru_glyph_of_the_shapemender") { }

        class glyph_of_the_shapemender_AuraScript : public AuraScript
        {
            PrepareAuraScript(glyph_of_the_shapemender_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
            {
                /// Glyph of the Shapemender proc only on shapeshift form.
                /// Glyph of the Shapemender has is playerscript.
                PreventDefaultAction();
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(glyph_of_the_shapemender_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new glyph_of_the_shapemender_AuraScript();
        }
};

enum DreamOfCenariusFeralSpells
{
    SPELL_DRU_DREAM_OF_CENARIUS_FERAL = 158497,
    SPELL_DRU_HEALING_TOUCH = 5185,
    SPELL_DRU_REJUVENATION = 774

};

/// Last Update - 6.1.2
/// Dream of Cenarius (feral) - 158497
/// Called by Healing Touch (5185) and Rejuvenation (774)
class spell_dru_dream_of_cenarius_feral : public SpellScriptLoader
{
public:
    spell_dru_dream_of_cenarius_feral() : SpellScriptLoader("spell_dru_dream_of_cenarius_feral") { }

    class spell_dru_dream_of_cenarius_feral_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_dream_of_cenarius_feral_SpellScript);

        enum eSpells
        {
            GlyphOfCatForm = 47180
        };

        void HandleOnHit()
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();

            if (l_Caster == nullptr || l_Target == nullptr)
                return;

            if (GetSpellInfo()->Id == SPELL_DRU_HEALING_TOUCH)
            {
                if (l_Caster != l_Target && l_Caster->HasAura(SPELL_DRU_DREAM_OF_CENARIUS_FERAL))
                {
                    uint32 l_HealAmount = GetHitHeal();
                    SpellInfo const* l_GlyphOfCatForm = sSpellMgr->GetSpellInfo(eSpells::GlyphOfCatForm);

                    /// Glyph of Cat Form
                    if (l_Caster->GetShapeshiftForm() == FORM_CAT && l_Caster->ToPlayer()->HasGlyph(l_GlyphOfCatForm->Id))
                        AddPct(l_HealAmount, l_GlyphOfCatForm->Effects[EFFECT_0].BasePoints);

                        l_Caster->HealBySpell(l_Caster, GetSpellInfo(), l_HealAmount, GetSpell()->IsCritForTarget(l_Target));
                }
            }
            else if (GetSpellInfo()->Id == SPELL_DRU_REJUVENATION)
            {
                if (l_Caster != l_Target && l_Caster->HasAura(SPELL_DRU_DREAM_OF_CENARIUS_FERAL))
                    l_Caster->AddAura(SPELL_DRUID_REJUVENATION, l_Caster);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_dru_dream_of_cenarius_feral_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_dream_of_cenarius_feral_SpellScript();
    }
};

enum WodPvpRestoration2pSpells
{
    SPELL_DRU_WOD_PVP_2P_BONUS = 170853,
    SPELL_DRU_WOD_PVP_2P_BONUS_EFFECT = 170856

};

/// Last Update - 6.1.2
/// Item - Druid WoD PvP Restoration 2P Bonus - 170853
/// Called by Barskin (22812) and Ironbark (102342)
class spell_dru_wod_pvp_2p_restoration : public SpellScriptLoader
{
public:
    spell_dru_wod_pvp_2p_restoration() : SpellScriptLoader("spell_dru_wod_pvp_2p_restoration") { }

    class spell_dru_wod_pvp_2p_restoration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wod_pvp_2p_restoration_SpellScript);


        void HandleOnHit()
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();

            if (l_Caster == nullptr || l_Target == nullptr)
                return;

            if (l_Caster->HasAura(SPELL_DRU_WOD_PVP_2P_BONUS))
                l_Caster->CastSpell(l_Target, SPELL_DRU_WOD_PVP_2P_BONUS_EFFECT, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_dru_wod_pvp_2p_restoration_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_wod_pvp_2p_restoration_SpellScript();
    }
};

/// Pulverize - 80313
class spell_dru_pulverize : public SpellScriptLoader
{
    public:
        spell_dru_pulverize() : SpellScriptLoader("spell_dru_pulverize") { }

        class spell_dru_pulverize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_pulverize_SpellScript);

            enum eSpells
            {
                Lacerate = 33745,
                AllowPulverize = 158790,
                PulverizeAura = 158792
            };

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (!l_Caster || !l_Target)
                    return SPELL_FAILED_ERROR;

                if (Aura* l_Lacerete = l_Target->GetAura(eSpells::Lacerate, l_Caster->GetGUID()))
                {
                    if (l_Lacerete->GetStackAmount() >= 3)
                        return SPELL_CAST_OK;
                }

                return SPELL_FAILED_ERROR;
            }

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (Aura* l_Lacerate = l_Target->GetAura(eSpells::Lacerate, l_Caster->GetGUID()))
                    l_Lacerate->ModStackAmount(-3);

                l_Caster->CastSpell(l_Caster, eSpells::PulverizeAura, true);
                l_Target->RemoveAura(eSpells::AllowPulverize);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_pulverize_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_pulverize_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_pulverize_SpellScript();
        }
};

/// Last Update - 6.1.2 19802
/// Empowered Moonkin - 157228
/// Called by Wrath - 5176, Stellar Flare - 152221, Starfire - 2912, Starsurge - 78674
class spell_dru_empowered_moonkin : public SpellScriptLoader
{
    public:
        spell_dru_empowered_moonkin() : SpellScriptLoader("spell_dru_empowered_moonkin") { }

        class spell_dru_empowered_moonkin_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_empowered_moonkin_SpellScript);

            enum eSpells
            {
                EmpoweredMoonkin = 157228,
                EnhancedStarsurge = 157232,
                Starsurge = 78674
            };

            bool m_HasAuraBeforeCast = false;

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::EmpoweredMoonkin))
                    m_HasAuraBeforeCast = true;
            }

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                /// Starsurge is already instant with Enhanced Starsurge, so we will not consume the aura
                if (l_Caster->HasAura(eSpells::EnhancedStarsurge) && GetSpellInfo()->Id == eSpells::Starsurge)
                    return;

                if (l_Caster->HasAura(eSpells::EmpoweredMoonkin) && m_HasAuraBeforeCast)
                    l_Caster->RemoveAura(eSpells::EmpoweredMoonkin);
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_empowered_moonkin_SpellScript::HandleOnPrepare);
                AfterCast += SpellCastFn(spell_dru_empowered_moonkin_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_empowered_moonkin_SpellScript();
        }
};

/// Glyph of Enchanted Bark - 159436
class spell_dru_glyph_of_enchanted_bark : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_enchanted_bark() : SpellScriptLoader("spell_dru_glyph_of_enchanted_bark") { }

        class spell_dru_glyph_of_enchanted_bark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_enchanted_bark_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
            {
                PreventDefaultAction();
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_glyph_of_enchanted_bark_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_glyph_of_enchanted_bark_AuraScript();
        }
};

/// Last Update 6.2.3
/// WoD PvP Balance 4P Bonus - 180717
class spell_dru_WodPvpBalance4pBonus : public SpellScriptLoader
{
    public:
        spell_dru_WodPvpBalance4pBonus() : SpellScriptLoader("spell_dru_WodPvpBalance4pBonus") { }

        class spell_dru_WodPvpBalance4pBonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_WodPvpBalance4pBonus_AuraScript);

            enum eSpells
            {
                CelestialFury = 180719,
                Starsurge = 78674
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    Unit* l_Victim = p_EventInfo.GetDamageInfo()->GetVictim();
                    SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();

                    if (l_SpellInfo == nullptr || l_Victim == nullptr)
                        return;

                    if (l_SpellInfo->Id != eSpells::Starsurge)
                        return;

                    if (l_Victim->GetTypeId() == TYPEID_PLAYER)
                        l_Caster->CastSpell(l_Caster, eSpells::CelestialFury, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_WodPvpBalance4pBonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_WodPvpBalance4pBonus_AuraScript();
        }
};

/// Item - Druid WoD PvP Balance 2P Bonus - 165701
/// Called by Entangling Roots - 339
class spell_dru_wod_pvp_balance_2p : public SpellScriptLoader
{
public:
    spell_dru_wod_pvp_balance_2p() : SpellScriptLoader("spell_dru_wod_pvp_balance_2p") { }

    class spell_dru_wod_pvp_balance_2p_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_wod_pvp_balance_2p_AuraScript);

        enum eSpells
        {
            Starsurge = 78674,
            WoDPvPBalance2PBonus = 165701
        };

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* l_Player = GetCaster()->ToPlayer();
            if (!l_Player)
                return;

            if (!l_Player->HasAura(eSpells::WoDPvPBalance2PBonus))
                return;

            /// When Entangling Roots is dispelled or broken by damage, you gain 1 charge of Starsurge.
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                return;

            if (SpellInfo const* l_Starsurge = sSpellMgr->GetSpellInfo(eSpells::Starsurge))
                l_Player->RestoreCharge(l_Starsurge->ChargeCategoryEntry);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectApplyFn(spell_dru_wod_pvp_balance_2p_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_ROOT_2, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_wod_pvp_balance_2p_AuraScript();
    }
};

/// last update : 6.1.2 19802
/// Lunar Inspiration - 155580
class spell_dru_lunar_inspiration : public SpellScriptLoader
{
    public:
        spell_dru_lunar_inspiration() : SpellScriptLoader("spell_dru_lunar_inspiration") { }

        class spell_dru_lunar_inspiration_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lunar_inspiration_AuraScript);

            enum eSpells
            {
                LunarInspirationOverride = 155627
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->AddAura(eSpells::LunarInspirationOverride, l_Caster);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->RemoveAura(eSpells::LunarInspirationOverride);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_lunar_inspiration_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_lunar_inspiration_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_lunar_inspiration_AuraScript();
        }
};

/// 16914 - Hurricane
class spell_dru_hurricane: public SpellScriptLoader
{
    public:
        spell_dru_hurricane() : SpellScriptLoader("spell_dru_hurricane") { }

        class spell_dru_hurricane_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_hurricane_AuraScript);

            enum eSpells
            {
                GlyphOfHurricane = 54831
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32 & p_Amount, bool & /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (AuraEffect* l_GlyphOfHurricane = l_Caster->GetAuraEffect(eSpells::GlyphOfHurricane, EFFECT_0))
                    p_Amount += l_GlyphOfHurricane->GetAmount();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_hurricane_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_hurricane_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Glyph of the Flapping Owl - 164997
class spell_dru_gyph_of_the_flapping_owl : public SpellScriptLoader
{
    public:
        spell_dru_gyph_of_the_flapping_owl() : SpellScriptLoader("spell_dru_gyph_of_the_flapping_owl") { }

        class spell_dru_gyph_of_the_flapping_owl_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_gyph_of_the_flapping_owl_AuraScript);

            enum eSpells
            {
                Flap = 164862
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->HasSpell(eSpells::Flap))
                    l_Player->learnSpell(eSpells::Flap, false);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasSpell(eSpells::Flap))
                    l_Player->removeSpell(eSpells::Flap, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_gyph_of_the_flapping_owl_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_gyph_of_the_flapping_owl_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_gyph_of_the_flapping_owl_AuraScript();
        }
};

/// Glyph of Rake - 54821
/// Called by Prowl - 5215
class spell_dru_glyph_of_rake: public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_rake() : SpellScriptLoader("spell_dru_glyph_of_rake") { }

        class spell_dru_glyph_of_rake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_rake_AuraScript);

            enum eSpells
            {
                GlyphOfRake         = 54821,
                GlyphOfRakeModRange = 164020
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (l_Target->HasAura(eSpells::GlyphOfRake))
                    l_Target->AddAura(eSpells::GlyphOfRakeModRange, l_Target);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (l_Target->HasAura(eSpells::GlyphOfRake))
                    l_Target->RemoveAura(eSpells::GlyphOfRakeModRange);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_rake_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_rake_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_glyph_of_rake_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Celestial Alignment - 112071
class spell_dru_celestial_alignement : public SpellScriptLoader
{
    public:
        spell_dru_celestial_alignement() : SpellScriptLoader("spell_dru_celestial_alignement") { }

        class spell_dru_celestial_alignement_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_celestial_alignement_SpellScript);

            enum eSpells
            {
                GlyphofCelestialAlignment = 159445,
                GlyphofCelestialAlignmentMarker = 160373
            };

            void HandleBeforeCast()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->HasGlyph(eSpells::GlyphofCelestialAlignment))
                    return;

                l_Player->CastSpell(l_Player, eSpells::GlyphofCelestialAlignment, true);
            }

            void HandleApplyAura(SpellEffIndex /*p_EffIndex*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->HasGlyph(eSpells::GlyphofCelestialAlignment))
                    return;

                /// Reinit charges of marker if it's already apply
                if (l_Player->HasAura(eSpells::GlyphofCelestialAlignmentMarker))
                    l_Player->RemoveAurasDueToSpell(GlyphofCelestialAlignmentMarker);

                /// Apply Marker of 8 charges
                l_Player->CastSpell(l_Player, eSpells::GlyphofCelestialAlignmentMarker, true);

            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dru_celestial_alignement_SpellScript::HandleBeforeCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_celestial_alignement_SpellScript::HandleApplyAura, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_celestial_alignement_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Celestial Alignment (Marker) - 160373
class spell_dru_celestial_alignement_marker : public SpellScriptLoader
{
    public:
        spell_dru_celestial_alignement_marker() : SpellScriptLoader("spell_dru_celestial_alignement_marker") { }

        class spell_dru_celestial_alignement_marker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_celestial_alignement_marker_AuraScript);

            enum eSpells
            {
                GlyphofCelestialAlignment = 159445,
                GlyphofCelestialAlignmentMarker = 160373,
                CelestialAlignment = 112071
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& /*p_EventInfo*/)
            {
                Unit* l_Target = GetTarget();

                if (p_AurEff->GetBase()->GetCharges() <= 1)
                {
                    l_Target->RemoveAurasDueToSpell(eSpells::GlyphofCelestialAlignmentMarker);
                    l_Target->RemoveAurasDueToSpell(eSpells::GlyphofCelestialAlignment);
                    l_Target->RemoveAurasDueToSpell(eSpells::CelestialAlignment);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_celestial_alignement_marker_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_celestial_alignement_marker_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Incarnation: Tree of Life - 33891
class spell_dru_incarnation_tree_of_life : public SpellScriptLoader
{
    public:
        spell_dru_incarnation_tree_of_life() : SpellScriptLoader("spell_dru_incarnation_tree_of_life") { }

        class spell_dru_incarnation_tree_of_life_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_tree_of_life_AuraScript);

            enum eSpells
            {
                Incarnation = 117679
            };

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (Aura* l_Aura = l_Target->GetAura(eSpells::Incarnation))
                {
                    p_AurEff->GetBase()->SetDuration(l_Aura->GetDuration());
                    l_Target->RemoveAura(eSpells::Incarnation);
                }
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                AuraRemoveMode RemoveMod = GetTargetApplication()->GetRemoveMode();

                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_CANCEL)
                    return;

                if (p_AurEff->GetBase()->GetDuration())
                    l_Target->CastSpell(l_Target, eSpells::Incarnation, true);

                if (Aura* l_Aura = l_Target->GetAura(eSpells::Incarnation))
                    l_Aura->SetDuration(p_AurEff->GetBase()->GetDuration());
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_tree_of_life_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_tree_of_life_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_incarnation_tree_of_life_AuraScript();
        }
};

/// 113769 - Warth
class spell_dru_treant_wrath : public SpellScriptLoader
{
    public:
        spell_dru_treant_wrath() : SpellScriptLoader("spell_dru_treant_wrath") { }

        class spell_dru_treant_wrath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_treant_wrath_SpellScript);

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Owner = l_Caster->GetOwner();
                Unit* l_Target = GetHitUnit();

                if (l_Owner == nullptr || l_Target == nullptr)
                    return;

                int32 l_Damage = GetSpellInfo()->Effects[EFFECT_0].BonusMultiplier * l_Owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL);

                l_Damage = l_Caster->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                l_Damage = l_Target->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                SetHitDamage(l_Damage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_treant_wrath_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_treant_wrath_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Soul of the forest - 158476
class PlayerScript_soul_of_the_forest : public PlayerScript
{
    public:
        PlayerScript_soul_of_the_forest() : PlayerScript("PlayerScript_soul_of_the_forest") { }

        enum eSpells
        {
            SoulOfTheForestAura = 158476,
            SoulOfTheForestTrigger = 114113
        };

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
        {
            if (p_Regen || p_Power != POWER_COMBO_POINT || p_Player->getClass() != CLASS_DRUID || !p_Player->HasAura(eSpells::SoulOfTheForestAura))
                return;

            if (p_Player->GetSpecializationId(p_Player->GetActiveSpec()) != SPEC_DRUID_FERAL)
                return;

            /// Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_DiffVal = p_NewValue - p_OldValue;

            if (l_DiffVal < 0)
            {
                int32 l_Bp = 4 * abs(l_DiffVal);
                p_Player->EnergizeBySpell(p_Player, eSpells::SoulOfTheForestTrigger, l_Bp, POWER_ENERGY);
            }
        }
};

/// last update : 6.1.2 19802
/// Moonkin Form - 24858, Incarnation: Chosen of Elune 102560
class spell_dru_astral_form : public SpellScriptLoader
{
    public:
        spell_dru_astral_form() : SpellScriptLoader("spell_dru_astral_form") { }

        class spell_dru_astral_form_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_astral_form_AuraScript);

            enum eSpells
            {
                GlyphOfStars    = 114301,
                MoonkinForm     = 24858,
                ChosenofElune   = 102560
            };

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfStars);

                if (l_Target->HasAura(eSpells::GlyphOfStars) && l_SpellInfo != nullptr)
                {
                    l_Target->SetDisplayId(l_Target->GetNativeDisplayId());
                    l_Target->CastSpell(l_Target, l_SpellInfo->Effects[EFFECT_0].BasePoints, true);
                }
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfStars);

                if (l_Target->HasAura(eSpells::MoonkinForm) || l_Target->HasAura(eSpells::ChosenofElune))
                    return;

                if (l_SpellInfo != nullptr && l_Target->HasAura(l_SpellInfo->Effects[EFFECT_0].BasePoints))
                    l_Target->RemoveAura(l_SpellInfo->Effects[EFFECT_0].BasePoints);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case eSpells::MoonkinForm:
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_astral_form_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_dru_astral_form_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                    break;
                case eSpells::ChosenofElune:
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_astral_form_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_dru_astral_form_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                    break;
                default:
                    break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_astral_form_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Glyph of Savagery - 171752
/// Call by Cat Form - 3025
class spell_dru_glyph_of_savagery : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_savagery() : SpellScriptLoader("spell_dru_glyph_of_savagery") { }

        class spell_dru_glyph_of_savagery_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_savagery_AuraScript);

            enum eSpells
            {
                CatForm             = 3025,
                GlyphOfSavagery     = 171752,
                SavageRoarEffect    = 62071
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfSavagery);

                if (l_SpellInfo == nullptr)
                    return;

                if ((GetSpellInfo()->Id == eSpells::CatForm && l_Target->HasAura(eSpells::GlyphOfSavagery)) || (GetSpellInfo()->Id == eSpells::GlyphOfSavagery && l_Target->HasAura(eSpells::CatForm)))
                {
                    l_Target->CastSpell(l_Target, eSpells::SavageRoarEffect, true);

                    if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(eSpells::SavageRoarEffect, EFFECT_0))
                        l_AuraEffect->ChangeAmount(l_SpellInfo->Effects[EFFECT_2].BasePoints);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (l_Target->HasAura(eSpells::SavageRoarEffect))
                    l_Target->RemoveAura(eSpells::SavageRoarEffect);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case eSpells::CatForm:
                    OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_savagery_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                    OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_savagery_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                    break;
                case eSpells::GlyphOfSavagery:
                    OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_savagery_AuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                    OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_savagery_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                    break;
                default:
                    break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_glyph_of_savagery_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Guardian of Elune - 155578
class spell_dru_guardian_of_elune : public SpellScriptLoader
{
    public:
        spell_dru_guardian_of_elune() : SpellScriptLoader("spell_dru_guardian_of_elune") { }

        class spell_dru_guardian_of_elune_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_guardian_of_elune_AuraScript);

            void AfterApplyRecovery(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                int32 l_DodgeChance = (int32)l_Target->GetFloatValue(PLAYER_FIELD_DODGE_PERCENTAGE);
                if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_2))
                    l_AurEff->SetAmount(l_DodgeChance * -1);
            }

            void AfterApplyModifier(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                int32 l_DodgeChance = (int32)l_Target->GetFloatValue(PLAYER_FIELD_DODGE_PERCENTAGE);
                if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_3))
                    l_AurEff->SetAmount(l_DodgeChance * -1);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_guardian_of_elune_AuraScript::AfterApplyRecovery, EFFECT_2, SPELL_AURA_CHARGE_RECOVERY_MULTIPLIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_guardian_of_elune_AuraScript::AfterApplyModifier, EFFECT_3, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_guardian_of_elune_AuraScript();
        }
};

/// Item - Druid T17 Feral 2P Bonus - 165431
class spell_dru_item_t17_feral_2p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_feral_2p_bonus() : SpellScriptLoader("spell_dru_item_t17_feral_2p_bonus") { }

        class spell_dru_item_t17_feral_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_feral_2p_bonus_AuraScript);

            enum eSpell
            {
                ScentOfBlood = 169752
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                /// Dealing bleed damage generates 1 Energy.
                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell || l_ProcSpell->Mechanic != Mechanics::MECHANIC_BLEED)
                    return;

                l_Caster->CastSpell(l_Caster, eSpell::ScentOfBlood, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_item_t17_feral_2p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_feral_2p_bonus_AuraScript();
        }
};

/// Called by Berserk (for Cat Form) - 106951
/// Item - Druid T17 Feral 4P Bonus - 165432
class spell_dru_item_t17_feral_4p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_feral_4p_bonus() : SpellScriptLoader("spell_dru_item_t17_feral_4p_bonus") { }

        class spell_dru_item_t17_feral_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_feral_4p_bonus_AuraScript);

            enum eSpells
            {
                T17Feral4P              = 165432,
                T17Feral4PProcDriver    = 166639
            };

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::T17Feral4P))
                        l_Caster->CastSpell(l_Caster, eSpells::T17Feral4PProcDriver, true);
                }
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->RemoveAura(eSpells::T17Feral4PProcDriver);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_item_t17_feral_4p_bonus_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectApplyFn(spell_dru_item_t17_feral_4p_bonus_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_feral_4p_bonus_AuraScript();
        }
};

/// Item - Druid T17 Feral 2P Bonus Proc Driver - 166639
class spell_dru_item_t17_feral_4p_bonus_proc_driver : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_feral_4p_bonus_proc_driver() : SpellScriptLoader("spell_dru_item_t17_feral_4p_bonus_proc_driver") { }

        class spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript);

            enum eSpells
            {
                T17Feral4P      = 165432,
                GushingWound    = 166638
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                /// While Berserk is active, special attacks deal an additional 30% bleed damage to the target over 6 sec.
                AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::T17Feral4P, EFFECT_0);
                if (l_AurEff == nullptr)
                {
                    p_AurEff->GetBase()->Remove();
                    return;
                }

                Unit* l_Target = p_EventInfo.GetActionTarget();
                if (l_Target == nullptr || l_Target == l_Caster)
                    return;

                SpellInfo const* l_Trigger  = sSpellMgr->GetSpellInfo(eSpells::GushingWound);
                int32 l_Damage              = CalculatePct(p_EventInfo.GetDamageInfo()->GetDamage(), l_AurEff->GetAmount());
                int32 l_PreviousTotalDamage = 0;

                if (AuraEffect* l_OldWounds = l_Target->GetAuraEffect(eSpells::GushingWound, EFFECT_0, l_Caster->GetGUID()))
                {
                    int32 l_PeriodicDamage  = l_OldWounds->GetAmount();
                    int32 l_Duration        = l_OldWounds->GetBase()->GetDuration();
                    int32 l_Amplitude       = l_OldWounds->GetAmplitude();

                    if (l_Amplitude)
                        l_PreviousTotalDamage = l_PeriodicDamage * (l_Duration / l_Amplitude);

                    l_PreviousTotalDamage /= (l_Trigger->GetMaxDuration() / l_Trigger->Effects[EFFECT_0].Amplitude);
                }

                if (l_Trigger->Effects[EFFECT_0].Amplitude)
                    l_Damage /= (l_Trigger->GetMaxDuration() / l_Trigger->Effects[EFFECT_0].Amplitude);

                l_Damage += l_PreviousTotalDamage;

                if (Aura* l_Wounds = l_Target->GetAura(eSpells::GushingWound, l_Caster->GetGUID()))
                    l_Wounds->RefreshDuration();
                else
                    l_Caster->CastSpell(l_Target, eSpells::GushingWound, true);

                if (AuraEffect* l_NewWounds = l_Target->GetAuraEffect(eSpells::GushingWound, EFFECT_0, l_Caster->GetGUID()))
                    l_NewWounds->SetAmount(l_Damage);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript();
        }
};

/// Called by Tooth and Claw buff - 135286
/// Item - Druid T17 Guardian 2P Bonus - 165410
class spell_dru_item_t17_guardian_2p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_guardian_2p_bonus() : SpellScriptLoader("spell_dru_item_t17_guardian_2p_bonus") { }

        class spell_dru_item_t17_guardian_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_guardian_2p_bonus_AuraScript);

            enum eSpells
            {
                T17Guardian2P       = 165432,
                ToothAndClawProc    = 166639
            };

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::T17Guardian2P))
                        l_Caster->CastSpell(l_Caster, eSpells::ToothAndClawProc, true);
                }
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->RemoveAura(eSpells::ToothAndClawProc);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_item_t17_guardian_2p_bonus_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectApplyFn(spell_dru_item_t17_guardian_2p_bonus_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_guardian_2p_bonus_AuraScript();
        }
};

/// Item - Druid T17 Restoration 4P Bonus - 167714
class spell_dru_item_t17_restoration_4p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_restoration_4p_bonus() : SpellScriptLoader("spell_dru_item_t17_restoration_4p_bonus") { }

        class spell_dru_item_t17_restoration_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_restoration_4p_bonus_AuraScript);

            enum eSpells
            {
                HealingTouch    = 5185,
                NaturesWisdom   = 167715
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                /// When you cast 2 sequential Healing Touch spells, the mana cost of your next Wild Growth is reduced by 25%.
                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell)
                    return;

                uint32 l_FlagsNot = ProcFlagsExLegacy::PROC_EX_INTERNAL_DOT | ProcFlagsExLegacy::PROC_EX_INTERNAL_HOT;
                l_FlagsNot |= ProcFlagsExLegacy::PROC_EX_INTERNAL_TRIGGERED | ProcFlagsExLegacy::PROC_EX_INTERNAL_MULTISTRIKE;

                if (p_EventInfo.GetHitMask() & l_FlagsNot)
                    return;

                if (AuraEffect* l_AurEff = p_AurEff->GetBase()->GetEffect(EFFECT_0))
                {
                    if (l_ProcSpell->Id != eSpells::HealingTouch)
                    {
                        l_AurEff->ChangeAmount(2);
                        return;
                    }

                    l_AurEff->ChangeAmount(l_AurEff->GetAmount() - 1);

                    if (l_AurEff->GetAmount() <= 0)
                    {
                        l_AurEff->ChangeAmount(2);
                        l_Caster->CastSpell(l_Caster, eSpells::NaturesWisdom, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_item_t17_restoration_4p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_restoration_4p_bonus_AuraScript();
        }
};

/// Last Update 6.2.3
/// Living Seed - 48500
class spell_dru_living_seed : public SpellScriptLoader
{
    public:
        spell_dru_living_seed() : SpellScriptLoader("spell_dru_living_seed") { }

        class spell_dru_living_seed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_living_seed_AuraScript);

            enum eSpells
            {
                LivingSeedAura = 48504
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();

                if (p_EventInfo.GetDamageInfo() == nullptr || l_Caster == nullptr)
                    return;

                int32 l_HealAmount = p_EventInfo.GetDamageInfo()->GetDamage();
                
                Unit* l_Target = p_EventInfo.GetDamageInfo()->GetVictim();

                if (!l_HealAmount || l_Target == nullptr)
                    return;

                l_HealAmount = CalculatePct(l_HealAmount, p_AurEff->GetAmount());
                if (AuraEffect* l_LivingSeed = l_Caster->GetAuraEffect(eSpells::LivingSeedAura, EFFECT_0))
                    l_HealAmount += l_LivingSeed->GetAmount();

                l_Caster->CastCustomSpell(l_Target, eSpells::LivingSeedAura, &l_HealAmount, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_living_seed_AuraScript();
        }
};

/// Last Update 6.2.3
/// One With Nature - 147420
class spell_dru_one_with_nature : public SpellScriptLoader
{
public:
    spell_dru_one_with_nature() : SpellScriptLoader("spell_dru_one_with_nature") { }

    class spell_dru_one_with_nature_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_one_with_nature_SpellScript);

        enum eSpells
        {
            OneWithNature = 147420
        };

        void HandleAfterHit()
        {
            Player* l_Player = GetCaster()->ToPlayer();

            if (l_Player == nullptr)
                return;
            
            int ZoneId[8] = { 85, 4, 493, 148, 331, 3711, 5805, 5785 };
            uint8 l_nbrDest = 0;
            uint32 l_minLevel = 0;
            for (uint8 i = 0; i < 8; ++i)
            {
                if (WorldMapAreaEntry const* l_wma = sWorldMapAreaStore.LookupEntry(ZoneId[i]))
                {
                    switch (ZoneId[i])
                    {
                        case 4: ///< minRecommendedLevel for Blasted land is 0
                            l_minLevel = 55;
                            break;
                        case 493: ///< minRecommendedLevel for Moonglade is 55 
                            l_minLevel = 15;
                            break;
                        default:
                            l_minLevel = l_wma->minRecommendedLevel;
                            break;
                    }
                    if (l_Player->getLevel() >= l_minLevel)
                        ++l_nbrDest;
                }
            }

            uint8 l_dest = urand(0, l_nbrDest-1);

            switch (l_dest)
            {
                case 0: ///< Tirisfal Glades
                    l_Player->TeleportTo(0, 1800.0f, 2239.0f, 148.88f, 0.0f);
                    break;
                case 1: ///< Darkshore
                    l_Player->TeleportTo(1, 4987.0f, 143.0f, 50.32f, 4.86f);
                    break;
                case 2: ///< Moonglade
                    l_Player->TeleportTo(1, 7372.0f, -2630.0f, 464.67f, 6.17f);
                    break;
                case 3: ///< Ashenvale
                    l_Player->TeleportTo(1, 3824.0f, 88.0f, 13.1f, 1.6f);
                    break;
                case 4: ///< Blasted Lands
                    l_Player->TeleportTo(0, -12338.0f, -2376.0f, 21.79f, 3.38f);
                    break;
                case 5: ///< Sholazar Basin
                    l_Player->TeleportTo(571, 6338.0f, 5189.0f, -75.7f, 2.78f);
                    break;
                case 6: ///< The Jade Forest
                    l_Player->TeleportTo(870, 262.0f, 1981.0f, 162.67f, 5.45f);
                    break;
                case 7: ///< Valley Of The Four Winds
                    l_Player->TeleportTo(870, 1104.0f, -1896.0f, 132.96f, 3.11f);
                    break;
                default:
                    break;
            }
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_dru_one_with_nature_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_one_with_nature_SpellScript();
    }
};

class spell_dru_one_with_nature_glyph : public SpellScriptLoader
{
public:
    spell_dru_one_with_nature_glyph() : SpellScriptLoader("spell_dru_one_with_nature_glyph") { }

    class spell_dru_one_with_nature_glyph_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_one_with_nature_glyph_AuraScript);

        enum eSpells
        {
            OneWithNatureGlyph = 146656,
            OneWithNature = 147420
        };

        void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->learnSpell(OneWithNature, true);
        }

        void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->removeSpell(OneWithNature);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_one_with_nature_glyph_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectApplyFn(spell_dru_one_with_nature_glyph_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_one_with_nature_glyph_AuraScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_druid_spell_scripts()
{
    new spell_dru_one_with_nature_glyph();
    new spell_dru_one_with_nature();
    new spell_dru_living_seed();
    new spell_dru_guardian_of_elune();
    new spell_dru_glyph_of_savagery();
    new spell_dru_astral_form();
    new spell_dru_incarnation_tree_of_life();
    new spell_dru_celestial_alignement_marker();
    new spell_dru_celestial_alignement();
    new spell_dru_yseras_gift_ally_proc();
    new spell_dru_glyph_of_enchanted_bark();
    new spell_dru_pulverize();
    new spell_dru_lifebloom_final_heal();
    new spell_dru_entangling_energy();
    new spell_dru_ursa_major();
    new spell_dru_glyph_of_barkskin();
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
    new spell_dru_natures_vigil();
    new spell_dru_natures_vigil_proc();
    new spell_dru_dash();
    new spell_dru_savage_defense();
    new spell_dru_lifebloom();
    new spell_dru_killer_instinct();
    new spell_dru_lifebloom_refresh();
    new spell_dru_natures_cure();
    new spell_dru_activate_cat_form();
    new spell_dru_cat_form();
    new spell_dru_glyph_of_cat_form();
    new spell_dru_skull_bash();
    new spell_dru_faerie_swarm();
    new spell_dru_wild_mushroom();
    new spell_dru_stampeding_roar();
    new spell_dru_lacerate();
    new spell_dru_faerie_fire();
    new spell_dru_eclipse();
    new spell_dru_eclipse_mod_damage();
    new spell_dru_moonfire();
    new spell_dru_moonfire_sunfire_damage();
    new spell_dru_shooting_stars_proc();
    new spell_dru_shooting_stars();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_starfall_dummy();
    new spell_dru_savage_roar();
    new spell_dru_survival_instincts();
    new spell_dru_swiftmend();
    new spell_dru_travel_form_removal();
    new spell_dru_travel_form();
    new spell_dru_travel_form_playerscript();
    new spell_dru_swift_flight_passive();
    new spell_dru_glyph_of_the_stag();
    new spell_dru_glyph_of_travel();
    new spell_dru_rake();
    new spell_dru_rake_triggered();
    new spell_dru_shred();
    new spell_dru_ferocious_bite();
    new spell_dru_frenzied_regeneration();
    new spell_dru_rip();
    new spell_dru_dream_of_cenarius();
    new spell_dru_primal_fury();
    new spell_dru_healing_touch();
    new spell_dru_rejuvenation();
    new spell_dru_sunfire();
    new spell_dru_regrowth();
    new spell_dru_wild_growth();
    new spell_dru_druid_flames();
    new spell_dru_glyph_of_the_shapemender_playerscript();
    new spell_dru_glyph_of_the_shapemender();
    new spell_dru_starsurge();
    new spell_dru_wrath();
    new spell_dru_wild_mushroom_heal();
    new spell_dru_wild_mushroom_heal_proc();
    new spell_dru_dream_of_cenarius_feral();
    new spell_dru_dream_of_cenarius_balance();
    new spell_dru_wod_pvp_2p_restoration();
    new spell_dru_WodPvpBalance4pBonus();
    new spell_dru_empowered_moonkin();
    new spell_dru_ursa_major_aura();
    new spell_dru_hurricane();
    new spell_dru_wod_pvp_balance_2p();
    new spell_dru_lunar_inspiration();
    new spell_dru_gyph_of_the_flapping_owl();
    new spell_dru_glyph_of_rake();
    new spell_dru_treant_wrath();
    new spell_dru_item_t17_feral_2p_bonus();
    new spell_dru_item_t17_feral_4p_bonus();
    new spell_dru_item_t17_feral_4p_bonus_proc_driver();
    new spell_dru_item_t17_guardian_2p_bonus();
    new spell_dru_item_t17_restoration_4p_bonus();

    /// PlayerScript
    new PlayerScript_soul_of_the_forest();
}
#endif
