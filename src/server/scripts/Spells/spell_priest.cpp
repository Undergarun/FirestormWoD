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
 * Scripts for spells with SPELLFAMILY_PRIEST and SPELLFAMILY_GENERIC spells used by priest players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pri_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "Player.h"

enum PriestSpells
{
    PRIEST_SPELL_GUARDIAN_SPIRIT_HEAL               = 48153,
    PRIEST_SPELL_PENANCE                            = 47540,
    PRIEST_SPELL_PENANCE_DAMAGE                     = 47758,
    PRIEST_SPELL_PENANCE_HEAL                       = 47757,
    PRIEST_SPELL_REFLECTIVE_SHIELD_DAMAGE           = 33619,
    PRIEST_SPELL_GLYPH_OF_REFLECTIVE_SHIELD         = 33202,
    PRIEST_SHADOW_WORD_DEATH                        = 32409,
    PRIEST_LEAP_OF_FAITH                            = 73325,
    PRIEST_LEAP_OF_FAITH_JUMP                       = 97817,
    PRIEST_SPELL_HALO_HEAL_SHADOW                   = 120696,
    PRIEST_SPELL_HALO_HEAL_HOLY                     = 120692,

    // Cascade
    PRIEST_CASCADE_HOLY_DAMAGE                      = 120785,
    PRIEST_CASCADE_HOLY_TRIGGER                     = 120786,
    PRIEST_CASCADE_INVISIBLE_AURA                   = 120840,
    PRIEST_CASCADE_HOLY_TALENT                      = 121135,
    PRIEST_CASCADE_HOLY_MISSILE                     = 121146,
    PRIEST_CASCADE_HOLY_HEAL                        = 121148,
    PRIEST_CASCADE_SHADOW_MISSILE                   = 127627,
    PRIEST_CASCADE_SHADOW_DAMAGE                    = 127628,
    PRIEST_CASCADE_SHADOW_HEAL                      = 127629,
    PRIEST_CASCADE_DAMAGE_TRIGGER                   = 127630,
    PRIEST_CASCADE_INVISIBLE_AURA_2                 = 127631,
    PRIEST_CASCADE_SHADOW_TALENT                    = 127632,

    PRIEST_SHADOWFORM_STANCE                        = 15473,
    PRIEST_SHADOW_WORD_PAIN                         = 589,
    PRIEST_DEVOURING_PLAGUE                         = 2944,
    PRIEST_DEVOURING_PLAGUE_HEAL                    = 127626,
    PRIEST_DEVOURING_PLAGUE_AURA                    = 158831,
    PRIEST_VAMPIRIC_TOUCH                           = 34914,
    PRIEST_PHANTASM_AURA                            = 108942,
    PRIEST_PHANTASM_PROC                            = 114239,
    PRIEST_ATONEMENT_AURA                           = 81749,
    PRIEST_ATONEMENT_HEAL                           = 81751,
    PRIEST_RAPTURE_ENERGIZE                         = 47755,
    PRIEST_WEAKENED_SOUL                            = 6788,
    PRIEST_EVANGELISM_AURA                          = 81662,
    PRIEST_EVANGELISM_STACK                         = 81661,
    PRIEST_ARCHANGEL                                = 81700,
    LIGHTWELL_CHARGES                               = 59907,
    LIGHTSPRING_RENEW                               = 126154,
    PRIEST_SMITE                                    = 585,
    PRIEST_HOLY_WORD_CHASTISE                       = 88625,
    PRIEST_HOLY_WORD_SANCTUARY_AREA                 = 88685,
    PRIEST_HOLY_WORD_SANCTUARY_HEAL                 = 88686,
    PRIEST_RAPID_RENEWAL_AURA                       = 95649,
    PRIEST_SPELL_EMPOWERED_RENEW                    = 63544,
    PRIEST_SPELL_DIVINE_INSIGHT_TALENT              = 109175,
    PRIEST_SPELL_DIVINE_INSIGHT_DISCIPLINE          = 123266,
    PRIEST_SPELL_POWER_WORD_SHIELD_OVERRIDED        = 123258,
    PRIEST_SPELL_DIVINE_INSIGHT_HOLY                = 123267,
    PRIEST_PRAYER_OF_MENDING                        = 33076,
    PRIEST_PRAYER_OF_MENDING_HEAL                   = 33110,
    PRIEST_PRAYER_OF_MENDING_RADIUS                 = 123262,
    PRIEST_PRAYER_OF_MENDING_AURA                   = 44586,
    PRIEST_BODY_AND_SOUL_AURA                       = 64129,
    PRIEST_BODY_AND_SOUL_INCREASE_SPEED             = 65081,
    PRIEST_SURGE_OF_LIGHT_AURA                      = 109186,
    PRIEST_SURGE_OF_LIGHT                           = 114255,
    PRIEST_SURGE_OF_DARKNESS                        = 87160,
    PRIEST_SURGE_OF_DARKNESS_AURA                   = 162448,
    PRIEST_SHADOW_WORD_INSANITY_DAMAGE              = 129249,
    PRIEST_SPELL_MIND_BLAST                         = 8092,
    PRIEST_SPELL_2P_S12_SHADOW                      = 92711,
    PRIEST_SPELL_DISPERSION_SPRINT                  = 129960,
    PRIEST_SPELL_4P_S12_SHADOW                      = 131556,
    PRIEST_SPELL_SIN_AND_PUNISHMENT                 = 87204,
    PRIEST_SPELL_2P_S12_HEAL                        = 33333,
    PRIEST_SPELL_SOUL_OF_DIAMOND                    = 96219,
    PRIEST_SPELL_4P_S12_HEAL                        = 131566,
    PRIEST_SPELL_HOLY_SPARK                         = 131567,
    PRIEST_SPELL_SPIRIT_OF_REDEMPTION_IMMUNITY      = 62371,
    PRIEST_SPELL_SPIRIT_OF_REDEMPTION_FORM          = 27795,
    PRIEST_SPELL_SPIRIT_OF_REDEMPTION_TALENT        = 20711,
    PRIEST_SPELL_SPIRIT_OF_REDEMPTION_ROOT          = 27792,
    PRIEST_SPELL_SPIRIT_OF_REDEMPTION_SHAPESHIFT    = 27827,
    PRIEST_SPELL_LEVITATE                           = 111758,
    PRIEST_SPELL_VOID_TENDRILS_SUMMON               = 127665,
    PRIEST_NPC_PSYFIEND                             = 59190,
    PRIEST_SPELL_SPECTRAL_GUISE_CHARGES             = 119030,
    PRIEST_SPELL_POWER_WORD_SHIELD                  = 17,
    PRIEST_SPELL_INNER_FOCUS_IMMUNITY               = 96267,
    PRIEST_SPELL_HOLY_NOVA                          = 132157,
    PRIEST_SPELL_HOLY_NOVA_HEAL                     = 23455,
    PRIEST_SPELL_CONFESSION                         = 126123,
    PRIEST_TRAIN_OF_THOUGHT                         = 92297,
    PRIEST_INNER_FOCUS                              = 89485,
    PRIEST_SHADOW_ORB_AURA                          = 77487,
    PRIEST_SHADOW_ORB_DUMMY                         = 127850,
    PRIEST_GLYPH_OF_SHADOW_RAVENS                   = 57985,
    PRIEST_NPC_VOID_TENDRILS                        = 65282,
    PRIEST_SPELL_SAVING_GRACE                       = 155274,
    PRIEST_SPELL_CLARITY_OF_POWER                   = 155246,
    PRIEST_SPELL_SPIRIT_SHELL_AURA                  = 109964,
    PRIEST_SPELL_SPIRIT_SHELL_PROC                  = 114908,
    PRIEST_SPELL_HALO_AREA_DAMAGE                   = 120644,
    PRIEST_SPELL_HALO_AREA_HEAL                     = 120517,
    PRIEST_SPELL_INSANITY_AURA                      = 139139,
    PRIEST_SPELL_INSANITY                           = 132573,
    PRIEST_SPELL_SHADOW_INSIGHT                     = 162452,
    PRIEST_SPELL_SHADOW_INSIGHT_PROC                = 124430,
    PRIEST_GLYPH_OF_POWER_WORD_SHIELD               = 55672,
    PRIEST_GLYPH_OF_POWER_WORD_SHIELD_PROC          = 56160,
    PRIEST_GLYPH_OF_MIND_HARVEST                    = 162532,
    PRIEST_SPELL_VOID_ENTROPY                       = 155361,
    PRIEST_GLYPH_OF_MIND_BLAST                      = 87194
};

// Shadow Orb - 77487 & Glyph od Shadow ravens - 57985
class PlayerScript_Shadow_Orb: public PlayerScript
{
    public:
        PlayerScript_Shadow_Orb() :PlayerScript("PlayerScript_Shadow_Orb") {}

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
        {
            if (p_Regen)
                return;

            if (p_Power == POWER_SHADOW_ORB && p_Player->GetPower(POWER_SHADOW_ORB) > 0)
            {
                // Shadow Orb visual
                if (!p_Player->HasAura(PRIEST_SHADOW_ORB_AURA) && !p_Player->HasAura(PRIEST_GLYPH_OF_SHADOW_RAVENS))
                    p_Player->CastSpell(p_Player, PRIEST_SHADOW_ORB_AURA, true);
                // Glyph of Shadow Ravens
                else if (!p_Player->HasAura(PRIEST_SHADOW_ORB_AURA) && p_Player->HasAura(PRIEST_GLYPH_OF_SHADOW_RAVENS))
                    p_Player->CastSpell(p_Player, PRIEST_SHADOW_ORB_DUMMY, true);
            }
            else
            {
                p_Player->RemoveAurasDueToSpell(PRIEST_SHADOW_ORB_AURA);
                p_Player->RemoveAurasDueToSpell(PRIEST_SHADOW_ORB_DUMMY);
            }
        }
};

// Confession (Glyph) - 126123
class spell_pri_confession: public SpellScriptLoader
{
    public:
        spell_pri_confession() : SpellScriptLoader("spell_pri_confession") { }

        class spell_pri_confession_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_confession_SpellScript);

            void HandleOnHit()
            {
                if (Player* caster = GetCaster()->ToPlayer())
                {
                    if (!GetHitUnit())
                        return;

                    if (Player* target = GetHitUnit()->ToPlayer())
                    {
                        std::string name = target->GetName();
                        std::string text = "[" + name + "]" + caster->GetSession()->GetTrinityString(LANG_CONFESSION_EMOTE);
                        text += caster->GetSession()->GetTrinityString(urand(LANG_CONFESSION_START, LANG_CONFESSION_END));
                        WorldPacket data;
                        target->BuildPlayerChat(&data, CHAT_MSG_TEXT_EMOTE, text.c_str(), LANG_UNIVERSAL);
                        target->SendMessageToSetInRange(&data, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_confession_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_confession_SpellScript();
        }
};

// Glyph of Confession - 126152
class spell_pri_glyph_of_confession: public SpellScriptLoader
{
    public:
        spell_pri_glyph_of_confession() : SpellScriptLoader("spell_pri_glyph_of_confession") { }

        class spell_pri_glyph_of_confession_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_glyph_of_confession_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(PRIEST_SPELL_CONFESSION, false);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(PRIEST_SPELL_CONFESSION))
                        _player->removeSpell(PRIEST_SPELL_CONFESSION, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_pri_glyph_of_confession_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_pri_glyph_of_confession_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_glyph_of_confession_AuraScript();
        }
};

enum ShadowWordDeath
{
    ShadowWordDeathGlyph            = 129176,
    EnhancedShadowWordDeath         = 157218,
    ShadowWordDeathOrbEnergize      = 125927,
    ShadowWordDeathCooldownMarker   = 95652
};

///Shadow Word: Death - 32379 and Shadow Word: Death (overrided by Glyph) - 129176
class spell_pri_shadow_word_death: public SpellScriptLoader
{
    public:
        spell_pri_shadow_word_death() : SpellScriptLoader("spell_pri_shadow_word_death") { }

        class spell_pri_shadow_word_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_shadow_word_death_SpellScript);

            bool m_Flag = false;

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (GetSpellInfo()->Id == ShadowWordDeath::ShadowWordDeathGlyph && l_Target->GetHealthPct() < 20.0f)
                {
                    m_Flag = true;
                    SetHitDamage(GetHitDamage() * 4); ///< If the target is below 20% health, the target takes four times as much damage
                }
            }

            void HandleAfterHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                int32 l_Damage = GetHitDamage();


                if (GetSpellInfo()->Id == ShadowWordDeath::ShadowWordDeathGlyph)
                {
                    if (m_Flag)
                    {
                        l_Damage /= 4;

                        /// Grants a Shadow Orb if the target is below 20% health
                        l_Player->CastSpell(l_Player, ShadowWordDeath::ShadowWordDeathOrbEnergize, true); ///< Shadow Orb energize

                        /// Gain an additional Shadow Orb if the cooldown of Shadow Word: Death had not been reset in the last 6 seconds
                        /// If the target dies
                        if (!l_Target->isAlive())
                        {
                            int32 l_Duration = 0;

                            if (AuraPtr l_CooldownMarker = l_Player->GetAura(ShadowWordDeath::ShadowWordDeathCooldownMarker))
                                l_Duration = l_CooldownMarker->GetDuration();
                            
                            if (l_Duration <= 3 * IN_MILLISECONDS)
                                l_Player->CastSpell(l_Player, ShadowWordDeath::ShadowWordDeathOrbEnergize, true); ///< Shadow Orb energize
                        }
                    }
                    if (l_Target->isAlive())
                    {
                        /// The cooldown is reset if the target does not die.
                        /// This reset cannot happen more often than once every 9 sec.
                        /// If the target is below 20% health
                        if (!l_Player->HasAura(ShadowWordDeath::ShadowWordDeathCooldownMarker) && m_Flag)
                        {
                            l_Player->CastSpell(l_Player, ShadowWordDeath::ShadowWordDeathCooldownMarker, true);
                            l_Player->ToPlayer()->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                        }

                        /// If the target not dies, you take damage
                        l_Player->CastCustomSpell(l_Player, PRIEST_SHADOW_WORD_DEATH, &l_Damage, NULL, NULL, true);
                    }
                }
                else
                {
                    if (!l_Target->isAlive() || l_Player->HasAura(ShadowWordDeath::EnhancedShadowWordDeath))
                        l_Player->CastSpell(l_Player, ShadowWordDeath::ShadowWordDeathOrbEnergize, true); ///< Shadow Orb energize
                    if (l_Target->isAlive())
                        l_Player->ToPlayer()->RemoveSpellCooldown(GetSpellInfo()->Id, true); ///< If the target does not die, the cooldown is reset, 
                }

            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pri_shadow_word_death_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                AfterHit += SpellHitFn(spell_pri_shadow_word_death_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_shadow_word_death_SpellScript();
        }
};

// Holy Nova (heal) - 23455
class spell_pri_holy_nova_heal: public SpellScriptLoader
{
    public:
        spell_pri_holy_nova_heal() : SpellScriptLoader("spell_pri_holy_nova_heal") { }

        class spell_pri_holy_nova_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_holy_nova_heal_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.size() < uint32(sSpellMgr->GetSpellInfo(PRIEST_SPELL_HOLY_NOVA)->Effects[EFFECT_1].BasePoints) + 1)
                    return;

                JadeCore::RandomResizeList(targets, sSpellMgr->GetSpellInfo(PRIEST_SPELL_HOLY_NOVA)->Effects[EFFECT_1].BasePoints);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_holy_nova_heal_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_holy_nova_heal_SpellScript();
        }
};

// Holy Nova - 132157
class spell_pri_holy_nova: public SpellScriptLoader
{
    public:
        spell_pri_holy_nova() : SpellScriptLoader("spell_pri_holy_nova") { }

        class spell_pri_holy_nova_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_holy_nova_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, PRIEST_SPELL_HOLY_NOVA_HEAL, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pri_holy_nova_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_holy_nova_SpellScript();
        }
};

// Glyph of Holy Nova - 125045
class spell_pri_glyph_of_holy_nova: public SpellScriptLoader
{
    public:
        spell_pri_glyph_of_holy_nova() : SpellScriptLoader("spell_pri_glyph_of_holy_nova") { }

        class spell_pri_glyph_of_holy_nova_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_glyph_of_holy_nova_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget())
                    return;

                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(PRIEST_SPELL_HOLY_NOVA, false);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget())
                    return;

                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(PRIEST_SPELL_HOLY_NOVA))
                        _player->removeSpell(PRIEST_SPELL_HOLY_NOVA, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_pri_glyph_of_holy_nova_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_pri_glyph_of_holy_nova_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_glyph_of_holy_nova_AuraScript();
        }
};

// Spectral Guise Charges - 119030
class spell_pri_spectral_guise_charges: public SpellScriptLoader
{
    public:
        spell_pri_spectral_guise_charges() : SpellScriptLoader("spell_pri_spectral_guise_charges") { }

        class spell_pri_spectral_guise_charges_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_spectral_guise_charges_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                Unit* spectralGuise = GetCaster();
                if (!spectralGuise)
                    return;

                Unit* attacker = eventInfo.GetActor();
                if (!attacker)
                    return;

                if (eventInfo.GetActor()->GetGUID() != spectralGuise->GetGUID())
                    return;

                if (eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE || eventInfo.GetDamageInfo()->GetDamageType() == DIRECT_DAMAGE)
                    if (AuraPtr spectralGuiseCharges = spectralGuise->GetAura(PRIEST_SPELL_SPECTRAL_GUISE_CHARGES))
                        spectralGuiseCharges->DropCharge();
            }

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->ToCreature())
                        caster->ToCreature()->DespawnOrUnsummon();
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pri_spectral_guise_charges_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_pri_spectral_guise_charges_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_spectral_guise_charges_AuraScript();
        }
};

// Spirit of Redemption (Shapeshift) - 27827
class spell_pri_spirit_of_redemption_form: public SpellScriptLoader
{
    public:
        spell_pri_spirit_of_redemption_form() : SpellScriptLoader("spell_pri_spirit_of_redemption_form") { }

        class spell_pri_spirit_of_redemption_form_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_spirit_of_redemption_form_AuraScript);

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->RemoveAura(PRIEST_SPELL_SPIRIT_OF_REDEMPTION_IMMUNITY);
                    caster->RemoveAura(PRIEST_SPELL_SPIRIT_OF_REDEMPTION_FORM);
                    caster->RemoveAura(PRIEST_SPELL_SPIRIT_OF_REDEMPTION_ROOT);
                    caster->setDeathState(JUST_DIED);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_pri_spirit_of_redemption_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_WATER_BREATHING, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_spirit_of_redemption_form_AuraScript();
        }
};

// Spirit of Redemption - 20711
class spell_pri_spirit_of_redemption: public SpellScriptLoader
{
    public:
        spell_pri_spirit_of_redemption() : SpellScriptLoader("spell_pri_spirit_of_redemption") { }

        class spell_pri_spirit_of_redemption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_spirit_of_redemption_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (Unit* caster = GetCaster())
                {
                    if (dmgInfo.GetDamage() < caster->GetHealth())
                        return;

                    if (caster->HasAura(PRIEST_SPELL_SPIRIT_OF_REDEMPTION_SHAPESHIFT))
                        return;

                    caster->CastSpell(caster, PRIEST_SPELL_SPIRIT_OF_REDEMPTION_FORM, true);
                    caster->CastSpell(caster, PRIEST_SPELL_SPIRIT_OF_REDEMPTION_IMMUNITY, true);
                    caster->CastSpell(caster, PRIEST_SPELL_SPIRIT_OF_REDEMPTION_ROOT, true);
                    caster->CastSpell(caster, PRIEST_SPELL_SPIRIT_OF_REDEMPTION_SHAPESHIFT, true);

                    absorbAmount = caster->GetHealth() - 1;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_spirit_of_redemption_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_spirit_of_redemption_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_spirit_of_redemption_AuraScript();
        }
};

// Called by Prayer of Mending - 33076
// Item : S12 4P bonus - Heal
class spell_pri_item_s12_4p_heal: public SpellScriptLoader
{
    public:
        spell_pri_item_s12_4p_heal() : SpellScriptLoader("spell_pri_item_s12_4p_heal") { }

        class spell_pri_item_s12_4p_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_item_s12_4p_heal_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(PRIEST_SPELL_4P_S12_HEAL))
                            _player->CastSpell(target, PRIEST_SPELL_HOLY_SPARK, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_item_s12_4p_heal_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_item_s12_4p_heal_SpellScript();
        }
};

// Called by Dispersion - 47585
// Item : S12 2P bonus - Shadow
class spell_pri_item_s12_2p_shadow: public SpellScriptLoader
{
    public:
        spell_pri_item_s12_2p_shadow() : SpellScriptLoader("spell_pri_item_s12_2p_shadow") { }

        class spell_pri_item_s12_2p_shadow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_item_s12_2p_shadow_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(PRIEST_SPELL_2P_S12_SHADOW))
                        _player->CastSpell(_player, PRIEST_SPELL_DISPERSION_SPRINT, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_item_s12_2p_shadow_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_item_s12_2p_shadow_SpellScript();
        }
};

// Divine Insight - 124430
class spell_pri_divine_insight_shadow: public SpellScriptLoader
{
    public:
        spell_pri_divine_insight_shadow() : SpellScriptLoader("spell_pri_divine_insight_shadow") { }

        class spell_pri_divine_insight_shadow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_divine_insight_shadow_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasSpellCooldown(PRIEST_SPELL_MIND_BLAST))
                        _player->RemoveSpellCooldown(PRIEST_SPELL_MIND_BLAST, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_divine_insight_shadow_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_divine_insight_shadow_SpellScript();
        }
};

// Power Word - Solace - 129250
class spell_pri_power_word_solace: public SpellScriptLoader
{
    public:
        spell_pri_power_word_solace() : SpellScriptLoader("spell_pri_power_word_solace") { }

        class spell_pri_power_word_solace_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_power_word_solace_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->EnergizeBySpell(l_Player, GetSpellInfo()->Id, int32(l_Player->GetMaxPower(POWER_MANA) / 100) * (GetSpellInfo()->Effects[EFFECT_2].BasePoints / 100), POWER_MANA);
                    l_Player->CastSpell(l_Player, PRIEST_ATONEMENT_AURA, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_power_word_solace_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_power_word_solace_SpellScript();
        }
};

// Shadowfiend - 34433 or Mindbender - 123040
class spell_pri_shadowfiend: public SpellScriptLoader
{
    public:
        spell_pri_shadowfiend() : SpellScriptLoader("spell_pri_shadowfiend") { }

        class spell_pri_shadowfiend_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_shadowfiend_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        if (Guardian* pet = _player->GetGuardianPet())
                        {
                            pet->InitCharmInfo();
                            pet->SetReactState(REACT_AGGRESSIVE);

                            if (pet->IsValidAttackTarget(target))
                                pet->ToCreature()->AI()->AttackStart(target);
                            else
                            {
                                Unit* victim = _player->GetSelectedUnit();
                                if (victim && pet->IsValidAttackTarget(target))
                                    pet->ToCreature()->AI()->AttackStart(target);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_pri_shadowfiend_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_shadowfiend_SpellScript();
        }
};

// Surge of Light (Discipline, Holy) - 109186 
// Surge of Light - 114255
class spell_pri_surge_of_light: public SpellScriptLoader
{
    public:
        spell_pri_surge_of_light() : SpellScriptLoader("spell_pri_surge_of_light") { }

        class spell_pri_surge_of_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_surge_of_light_SpellScript);

            int32 m_Duration = 0;

            void HandleOnPrepare()
            {
                if (Unit* l_Caster = GetCaster())
                    if (AuraPtr l_SurgeOfLight = l_Caster->GetAura(PRIEST_SURGE_OF_LIGHT))
                        m_Duration = l_SurgeOfLight->GetDuration();
            }

            void HandleOnCast()
            {
                if (Unit* l_Caster = GetCaster())
                    if (AuraPtr l_SurgeOfLight = l_Caster->GetAura(PRIEST_SURGE_OF_LIGHT))
                    {
                        l_SurgeOfLight->SetDuration(m_Duration);
                        if (l_SurgeOfLight->GetStackAmount() > 1)
                            l_SurgeOfLight->SetStackAmount(1);
                    }
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_pri_surge_of_light_SpellScript::HandleOnPrepare);
                OnCast += SpellCastFn(spell_pri_surge_of_light_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_surge_of_light_SpellScript();
        }

        class spell_pri_surge_of_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_surge_of_light_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& procInfo)
            {
                PreventDefaultAction();

                if (!procInfo.GetHealInfo() || !procInfo.GetHealInfo()->GetHeal() || !procInfo.GetActor())
                    return;

                if (Player* l_Player = procInfo.GetActor()->ToPlayer())
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PRIEST_SHADOW)
                        return;

                        if (roll_chance_i(GetSpellInfo()->Effects[EFFECT_0].BasePoints))
                        {
                            if (AuraPtr l_SurgeOfLight = l_Player->GetAura(PRIEST_SURGE_OF_LIGHT))
                            {
                                if (l_SurgeOfLight->GetStackAmount() == 2)
                                    l_SurgeOfLight->SetDuration(l_SurgeOfLight->GetMaxDuration());
                                else
                                    l_Player->CastSpell(l_Player, PRIEST_SURGE_OF_LIGHT, true);
                            }
                            else
                                l_Player->CastSpell(l_Player, PRIEST_SURGE_OF_LIGHT, true);
                        }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pri_surge_of_light_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_surge_of_light_AuraScript();
        }
};

// Called by Leap of Faith - 73325 and Power Word : Shield - 17
// Body and Soul - 64129
class spell_pri_body_and_soul: public SpellScriptLoader
{
    public:
        spell_pri_body_and_soul() : SpellScriptLoader("spell_pri_body_and_soul") { }

        class spell_pri_body_and_soul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_body_and_soul_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(PRIEST_BODY_AND_SOUL_AURA))
                            _player->CastSpell(target, PRIEST_BODY_AND_SOUL_INCREASE_SPEED, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_body_and_soul_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_body_and_soul_SpellScript();
        }
};

// Prayer of Mending (Divine Insight) - 123259
class spell_pri_prayer_of_mending_divine_insight: public SpellScriptLoader
{
    public:
        spell_pri_prayer_of_mending_divine_insight() : SpellScriptLoader("spell_pri_prayer_of_mending_divine_insight") { }

        class spell_pri_prayer_of_mending_divine_insight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_prayer_of_mending_divine_insight_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (AuraPtr prayerOfMending = target->GetAura(PRIEST_PRAYER_OF_MENDING_RADIUS, _player->GetGUID()))
                        {
                            int32 value = prayerOfMending->GetEffect(0)->GetAmount();

                            if (_player->HasAura(PRIEST_SPELL_DIVINE_INSIGHT_HOLY))
                                _player->RemoveAura(PRIEST_SPELL_DIVINE_INSIGHT_HOLY);

                            target->CastCustomSpell(target, PRIEST_PRAYER_OF_MENDING_HEAL, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());
                            if (target->HasAura(GetSpellInfo()->Id))
                                target->RemoveAura(GetSpellInfo()->Id);

                            float radius = sSpellMgr->GetSpellInfo(PRIEST_PRAYER_OF_MENDING_RADIUS)->Effects[0].CalcRadius(_player);

                            if (Unit* secondTarget = target->GetNextRandomRaidMemberOrPet(radius))
                            {
                                target->CastCustomSpell(secondTarget, PRIEST_PRAYER_OF_MENDING, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());
                                if (secondTarget->HasAura(PRIEST_PRAYER_OF_MENDING))
                                    secondTarget->RemoveAura(PRIEST_PRAYER_OF_MENDING);

                                secondTarget->CastCustomSpell(secondTarget, PRIEST_PRAYER_OF_MENDING_HEAL, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());

                                if (Unit* thirdTarget = target->GetNextRandomRaidMemberOrPet(radius))
                                {
                                    secondTarget->CastCustomSpell(thirdTarget, PRIEST_PRAYER_OF_MENDING, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());
                                    if (thirdTarget->HasAura(PRIEST_PRAYER_OF_MENDING))
                                        thirdTarget->RemoveAura(PRIEST_PRAYER_OF_MENDING);

                                    thirdTarget->CastCustomSpell(thirdTarget, PRIEST_PRAYER_OF_MENDING_HEAL, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());

                                    if (Unit* fourthTarget = target->GetNextRandomRaidMemberOrPet(radius))
                                    {
                                        thirdTarget->CastCustomSpell(fourthTarget, PRIEST_PRAYER_OF_MENDING, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());
                                        if (fourthTarget->HasAura(PRIEST_PRAYER_OF_MENDING))
                                            fourthTarget->RemoveAura(PRIEST_PRAYER_OF_MENDING);

                                        fourthTarget->CastCustomSpell(fourthTarget, PRIEST_PRAYER_OF_MENDING_HEAL, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());

                                        if (Unit* fifthTarget = target->GetNextRandomRaidMemberOrPet(radius))
                                        {
                                            fourthTarget->CastCustomSpell(fifthTarget, PRIEST_PRAYER_OF_MENDING, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());
                                            if (fifthTarget->HasAura(PRIEST_PRAYER_OF_MENDING))
                                                fifthTarget->RemoveAura(PRIEST_PRAYER_OF_MENDING);

                                            fifthTarget->CastCustomSpell(fifthTarget, PRIEST_PRAYER_OF_MENDING_HEAL, &value, NULL, NULL, true, NULL, NULLAURA_EFFECT, _player->GetGUID());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_prayer_of_mending_divine_insight_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_prayer_of_mending_divine_insight_SpellScript();
        }
};

// Called by Greater Heal - 2060 and Prayer of Healing - 596
// Divine Insight (Holy) - 109175
class spell_pri_divine_insight_holy: public SpellScriptLoader
{
    public:
        spell_pri_divine_insight_holy() : SpellScriptLoader("spell_pri_divine_insight_holy") { }

        class spell_pri_divine_insight_holy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_divine_insight_holy_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(PRIEST_SPELL_DIVINE_INSIGHT_TALENT))
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_PRIEST_HOLY)
                        if (roll_chance_i(GetSpellInfo()->Effects[EFFECT_0].BasePoints))
                                _player->CastSpell(_player, PRIEST_SPELL_DIVINE_INSIGHT_HOLY, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_divine_insight_holy_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_divine_insight_holy_SpellScript();
        }
};

// Called by Power Word: Shield (Divine Insight) - 123258 and Power Word: Shield - 17
// Divine Insight (Discipline) - 123266
class spell_pri_divine_insight_discipline: public SpellScriptLoader
{
    public:
        spell_pri_divine_insight_discipline() : SpellScriptLoader("spell_pri_divine_insight_discipline") { }

        class spell_pri_divine_insight_discipline_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_divine_insight_discipline_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    if (l_Caster->HasAura(PRIEST_SPELL_DIVINE_INSIGHT_DISCIPLINE))
                        l_Caster->RemoveAura(PRIEST_SPELL_DIVINE_INSIGHT_DISCIPLINE);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_divine_insight_discipline_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_divine_insight_discipline_SpellScript();
        }
};

// Holy Word : Sanctuary - 88685
class spell_pri_holy_word_sanctuary: public SpellScriptLoader
{
    public:
        spell_pri_holy_word_sanctuary() : SpellScriptLoader("spell_pri_holy_word_sanctuary") { }

        class spell_pri_holy_word_sanctuary_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_holy_word_sanctuary_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                if (DynamicObject* dynObj = caster->GetDynObject(PRIEST_HOLY_WORD_SANCTUARY_AREA))
                    caster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), PRIEST_HOLY_WORD_SANCTUARY_HEAL, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_holy_word_sanctuary_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_holy_word_sanctuary_AuraScript();
        }
};

enum MasterySpells
{
    MASTERY_SPELL_DISCIPLINE_SHIELD = 77484
};

// Power Word: Shield - 17
class spell_pri_power_word_shield: public SpellScriptLoader
{
    public:
        spell_pri_power_word_shield() : SpellScriptLoader("spell_pri_power_word_shield") { }

        class spell_pri_power_word_shield_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_power_word_shield_AuraScript);

            std::map<uint64, uint32> m_DmgByAttackerList;

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                p_Amount = ((l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL) * 5) + GetSpellInfo()->Effects[EFFECT_0].BasePoints) * 1;

                if (l_Caster->HasAura(MASTERY_SPELL_DISCIPLINE_SHIELD) && l_Caster->getLevel() >= 80)
                {
                    float l_Mastery = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.625f;
                    p_Amount += CalculatePct(p_Amount, l_Mastery);
                }
            }

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*mode*/) // Case of PRIEST_GLYPH_OF_REFLECTIVE_SHIELD
            {
                if (Unit* l_Owner = GetUnitOwner())
                    if (Unit* l_Target = GetTarget())
                        if (l_Owner == l_Target && l_Owner->HasAura(PRIEST_SPELL_GLYPH_OF_REFLECTIVE_SHIELD))
                            for (std::map<uint64, uint32>::iterator it = m_DmgByAttackerList.begin(); it != m_DmgByAttackerList.end(); ++it)
                            {
                                if ((*it).first == l_Target->GetGUID())
                                    return;
                                int32 l_Damage = CalculatePct((*it).second, sSpellMgr->GetSpellInfo(PRIEST_SPELL_GLYPH_OF_REFLECTIVE_SHIELD)->Effects[EFFECT_0].BasePoints);
                                l_Owner->CastCustomSpell(l_Target->GetUnit(*l_Target, (*it).first), PRIEST_SPELL_REFLECTIVE_SHIELD_DAMAGE, &l_Damage, NULL, NULL, true);
                            }
            }

            void OnAbsorb(AuraEffectPtr p_AurEff, DamageInfo & p_DmgInfo, uint32 & p_AbsorbAmount)
            {
                if (Unit* l_Owner = GetUnitOwner())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        if (Unit* l_Attacker = p_DmgInfo.GetAttacker())
                            if (l_Owner == l_Target && l_Owner->HasAura(PRIEST_SPELL_GLYPH_OF_REFLECTIVE_SHIELD)) // Case of PRIEST_GLYPH_OF_REFLECTIVE_SHIELD
                            {
                                if (m_DmgByAttackerList.find(l_Attacker->GetGUID()) != m_DmgByAttackerList.end())
                                    m_DmgByAttackerList.find(l_Attacker->GetGUID())->second += p_DmgInfo.GetDamage();
                                else
                                    m_DmgByAttackerList[l_Attacker->GetGUID()] = p_DmgInfo.GetDamage();
                            }

                        if (l_Owner->HasAura(PRIEST_GLYPH_OF_POWER_WORD_SHIELD)) // Case of PRIEST_GLYPH_OF_POWER_WORD_SHIELD
                        {
                            int32 l_Heal = CalculatePct(p_DmgInfo.GetDamage(), sSpellMgr->GetSpellInfo(PRIEST_GLYPH_OF_POWER_WORD_SHIELD)->Effects[EFFECT_0].BasePoints);
                            l_Owner->CastCustomSpell(l_Target, PRIEST_GLYPH_OF_POWER_WORD_SHIELD_PROC, &l_Heal, NULL, NULL, true, NULL, p_AurEff);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_power_word_shield_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_power_word_shield_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectRemove += AuraEffectRemoveFn(spell_pri_power_word_shield_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_power_word_shield_AuraScript();
        }
};

// Smite - 585
class spell_pri_smite: public SpellScriptLoader
{
    public:
        spell_pri_smite() : SpellScriptLoader("spell_pri_smite") { }

        class spell_pri_smite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_smite_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                    {
                        // Surge of light 
                        const SpellInfo * l_SpellInfo = sSpellMgr->GetSpellInfo(PRIEST_SURGE_OF_LIGHT_AURA);

                        if (l_SpellInfo != nullptr && l_Player->HasSpell(PRIEST_SURGE_OF_LIGHT_AURA) && roll_chance_i(l_SpellInfo->Effects[EFFECT_0].BasePoints))
                        {
                            if (AuraPtr l_SurgeOfLight = l_Player->GetAura(PRIEST_SURGE_OF_LIGHT))
                            {
                                if (l_SurgeOfLight->GetStackAmount() == 2)
                                    l_SurgeOfLight->SetDuration(l_SurgeOfLight->GetMaxDuration());
                                else
                                    l_Player->CastSpell(l_Player, PRIEST_SURGE_OF_LIGHT, true);
                            }
                            else
                                l_Player->CastSpell(l_Player, PRIEST_SURGE_OF_LIGHT, true);
                        }
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_smite_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_smite_SpellScript();
        }
};

// Lightwell Renew - 60123
class spell_pri_lightwell_renew: public SpellScriptLoader
{
    public:
        spell_pri_lightwell_renew() : SpellScriptLoader("spell_pri_lightwell_renew") { }

        class spell_pri_lightwell_renew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_lightwell_renew_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->GetTypeId() != TYPEID_UNIT || !l_Caster->ToCreature()->isSummon())
                            return;

                        // proc a spellcast
                        if (AuraPtr l_ChargesAura = l_Caster->GetAura(LIGHTWELL_CHARGES))
                        {
                            if (!l_Target->HasAura(LIGHTSPRING_RENEW))
                            {
                                l_Caster->CastSpell(l_Target, LIGHTSPRING_RENEW, true, NULL, NULLAURA_EFFECT, l_Caster->ToTempSummon()->GetSummonerGUID());
                                if (l_ChargesAura->ModCharges(-1))
                                    l_Caster->ToTempSummon()->UnSummon();
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_lightwell_renew_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_lightwell_renew_SpellScript();
        }
};

// Called by Smite - 585, Holy Fire - 14914 and Penance - 47666
// Atonement - 81749
class spell_pri_atonement: public SpellScriptLoader
{
    public:
        spell_pri_atonement() : SpellScriptLoader("spell_pri_atonement") { }

        class spell_pri_atonement_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_atonement_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                SpellInfo const* l_SpellInfoAtonement = sSpellMgr->GetSpellInfo(PRIEST_ATONEMENT_AURA);
                if (!l_SpellInfoAtonement && !l_Caster->HasAura(PRIEST_ATONEMENT_AURA))
                    return;

                std::list<Unit*> l_GroupList;
                l_Caster->GetRaidMembers(l_GroupList);

                l_GroupList.remove_if([this, l_Caster, l_SpellInfoAtonement](Unit* p_Unit)
                {
                    return l_Caster->GetDistance(p_Unit->GetPositionX(), p_Unit->GetPositionY(), p_Unit->GetPositionZ()) > l_SpellInfoAtonement->Effects[EFFECT_1].BasePoints;
                });

                if (l_GroupList.size() > 1)
                {
                    l_GroupList.sort(JadeCore::HealthPctOrderPred());
                    l_GroupList.resize(1);
                }

                int32 l_Heal = CalculatePct(GetHitDamage(), l_SpellInfoAtonement->Effects[EFFECT_0].BasePoints);
                for (auto itr : l_GroupList)
                {
                    if (itr->GetGUID() == l_Caster->GetGUID())
                        l_Heal /= 2;

                    l_Caster->CastCustomSpell(itr, PRIEST_ATONEMENT_HEAL, &l_Heal, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_atonement_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_atonement_SpellScript();
        }
};

enum Purify_Spell
{
    PRIEST_SPELL_PURIFY = 527
};

// Purify - 527
class spell_pri_purify: public SpellScriptLoader
{
    public:
        spell_pri_purify() : SpellScriptLoader("spell_pri_purify") { }

        class spell_pri_purify_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_purify_SpellScript);

            SpellCastResult CheckCleansing()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        DispelChargesList dispelList;

                        // Create dispel mask by dispel type
                        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        {
                            if (GetSpellInfo()->Effects[i].IsEffect())
                            {
                                uint32 dispel_type = GetSpellInfo()->Effects[i].MiscValue;
                                uint32 dispelMask = GetSpellInfo()->GetDispelMask(DispelType(dispel_type));
                                if (GetSpellInfo()->Id == PRIEST_SPELL_PURIFY)
                                target->GetDispellableAuraList(caster, dispelMask, dispelList);
                            }
                        }
                        if (dispelList.empty())
                            return SPELL_FAILED_NOTHING_TO_DISPEL;

                        return SPELL_CAST_OK;
                    }
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pri_purify_SpellScript::CheckCleansing);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_purify_SpellScript();
        }
};

// Devouring Plague - 2944
class spell_pri_devouring_plague: public SpellScriptLoader
{
    public:
        spell_pri_devouring_plague() : SpellScriptLoader("spell_pri_devouring_plague") { }

        class spell_pri_devouring_plague_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_devouring_plague_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetPower(POWER_SHADOW_ORB) < 3)
                        return SPELL_FAILED_NO_POWER;
                }
                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PRIEST_SHADOW)
                        {
                            if (l_Player->HasAura(PRIEST_SURGE_OF_DARKNESS_AURA))
                            {
                                if (roll_chance_i(sSpellMgr->GetSpellInfo(PRIEST_SURGE_OF_DARKNESS_AURA)->Effects[EFFECT_0].BasePoints))
                                    l_Player->CastSpell(l_Player, PRIEST_SURGE_OF_DARKNESS, true);
                            }
                            // Shadow Orb visual
                            if (l_Player->HasAura(PRIEST_SHADOW_ORB_AURA))
                                l_Player->RemoveAura(PRIEST_SHADOW_ORB_AURA);
                            // Glyph of Shadow Ravens
                            else if (l_Player->HasAura(PRIEST_SHADOW_ORB_DUMMY))
                                l_Player->RemoveAura(PRIEST_SHADOW_ORB_DUMMY);
                            
                            // Case of Void Entropy running on Target -> refresh this effect to its full 1 min duration.
                            if (l_Target->HasAura(PRIEST_SPELL_VOID_ENTROPY))
                            {
                                if (AuraPtr l_VoidEntropy = l_Target->GetAura(PRIEST_SPELL_VOID_ENTROPY))
                                    l_VoidEntropy->RefreshDuration();
                            }

                            int32 l_Heal = GetHitDamage();
                            l_Player->CastCustomSpell(l_Player, PRIEST_DEVOURING_PLAGUE_HEAL, &l_Heal, NULL, NULL, true);
                            l_Player->CastSpell(l_Target, PRIEST_DEVOURING_PLAGUE_AURA, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pri_devouring_plague_SpellScript::CheckCast);
                OnHit += SpellHitFn(spell_pri_devouring_plague_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_devouring_plague_SpellScript;
        }
};

// Devouring Plague Periodic Damage - 158831
class spell_pri_devouring_plague_aura: public SpellScriptLoader
{
public:
    spell_pri_devouring_plague_aura() : SpellScriptLoader("spell_pri_devouring_plague_aura") { }

    class spell_pri_devouring_plague_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_devouring_plague_aura_AuraScript);

        void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* l_Caster = GetCaster())
                amount = CalculatePct(l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL), sSpellMgr->GetSpellInfo(2944)->Effects[EFFECT_1].BasePoints);
        }

        void OnTick(constAuraEffectPtr aurEff)
        {
            if (Unit* l_Caster = GetCaster())
            {
                int32 l_TickDamage = 1;

                if (constAuraEffectPtr aurEff2 = aurEff->GetBase()->GetEffect(0))
                    l_TickDamage *= aurEff2->GetAmount();

                l_Caster->CastCustomSpell(l_Caster, PRIEST_DEVOURING_PLAGUE_HEAL, &l_TickDamage, NULL, NULL, true);

                if (l_Caster->HasAura(PRIEST_SURGE_OF_DARKNESS_AURA))
                {
                    if (roll_chance_i(sSpellMgr->GetSpellInfo(PRIEST_SURGE_OF_DARKNESS_AURA)->Effects[EFFECT_0].BasePoints))
                        l_Caster->CastSpell(l_Caster, PRIEST_SURGE_OF_DARKNESS, true);
                }
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_devouring_plague_aura_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_devouring_plague_aura_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pri_devouring_plague_aura_AuraScript();
    }
};

// Called by Fade - 586
// Phantasm - 108942
class spell_pri_phantasm: public SpellScriptLoader
{
    public:
        spell_pri_phantasm() : SpellScriptLoader("spell_pri_phantasm") { }

        class spell_pri_phantasm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_phantasm_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(PRIEST_PHANTASM_AURA))
                    {
                        _player->CastSpell(_player, PRIEST_PHANTASM_PROC, true);
                        _player->RemoveMovementImpairingAuras();
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_phantasm_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_phantasm_SpellScript;
        }
};

// Mind Spike - 73510
class spell_pri_mind_spike: public SpellScriptLoader
{
    public:
        spell_pri_mind_spike() : SpellScriptLoader("spell_pri_mind_spike") { }

        class spell_pri_mind_spike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_mind_spike_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(PRIEST_SURGE_OF_DARKNESS))
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(PRIEST_SURGE_OF_DARKNESS);
                        if (l_SpellInfo)
                            SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_SpellInfo->Effects[EFFECT_3].BasePoints));
                    }
                }
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->HasAura(PRIEST_SPELL_SHADOW_INSIGHT))
                        {
                            if (roll_chance_i(sSpellMgr->GetSpellInfo(PRIEST_SPELL_SHADOW_INSIGHT)->Effects[EFFECT_3].BasePoints))
                                l_Caster->CastSpell(l_Caster, PRIEST_SPELL_SHADOW_INSIGHT_PROC, true);
                        }
                        // Surge of Darkness - Your next Mind Spike will not consume your damage-over-time effects ...
                        if (!l_Caster->HasAura(PRIEST_SURGE_OF_DARKNESS))
                        {
                            // Mind Spike remove all DoT on the target's
                            if (l_Target->HasAura(PRIEST_SHADOW_WORD_PAIN, l_Caster->GetGUID()))
                                l_Target->RemoveAura(PRIEST_SHADOW_WORD_PAIN, l_Caster->GetGUID());
                            if (l_Target->HasAura(PRIEST_DEVOURING_PLAGUE, l_Caster->GetGUID()))
                                l_Target->RemoveAura(PRIEST_DEVOURING_PLAGUE, l_Caster->GetGUID());
                            if (l_Target->HasAura(PRIEST_VAMPIRIC_TOUCH, l_Caster->GetGUID()))
                                l_Target->RemoveAura(PRIEST_VAMPIRIC_TOUCH, l_Caster->GetGUID());
                        }
                    }
                }
            }


            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pri_mind_spike_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnHit += SpellHitFn(spell_pri_mind_spike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_mind_spike_SpellScript;
        }
};

// Called by Holy Fire - 14914, Smite - 585 and Penance - 47666
// Evangelism - 81662
class spell_pri_evangelism: public SpellScriptLoader
{
    public:
        spell_pri_evangelism() : SpellScriptLoader("spell_pri_evangelism") { }

        class spell_pri_evangelism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_evangelism_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(PRIEST_EVANGELISM_AURA))
                    {
                        if (GetHitDamage())
                            l_Player->CastSpell(l_Player, PRIEST_EVANGELISM_STACK, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_evangelism_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_evangelism_SpellScript;
        }
};

// Cascade - 127630 (damage trigger) or Cascade - 120786 (heal trigger)
class spell_pri_cascade_second: public SpellScriptLoader
{
    public:
        spell_pri_cascade_second() : SpellScriptLoader("spell_pri_cascade_second") { }

        class spell_pri_cascade_second_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_cascade_second_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> checkAuras;
                        std::list<Unit*> targetList;
                        int32 affectedUnits = 0;

                        _player->GetAttackableUnitListInRange(targetList, GetSpellInfo()->Effects[EFFECT_1].RadiusEntry->radiusHostile);

                        for (auto itr : targetList)
                        {
                            if (itr->HasAura(PRIEST_CASCADE_INVISIBLE_AURA))
                                if (Unit* caster = itr->GetAura(PRIEST_CASCADE_INVISIBLE_AURA)->GetCaster())
                                    if (caster->GetGUID() == _player->GetGUID())
                                        affectedUnits++;
                        }

                        // Stop the script if the max targets is reached ...
                        if (affectedUnits >= 15)
                            return;

                        if (AuraPtr boundNumber = _player->GetAura(PRIEST_CASCADE_INVISIBLE_AURA_2))
                            if (boundNumber->GetCharges() >= 3)
                                return;

                        for (auto itr : targetList)
                            checkAuras.push_back(itr);

                        for (auto itr : checkAuras)
                        {
                            if (itr->HasAura(PRIEST_CASCADE_INVISIBLE_AURA))
                                if (Unit* caster = itr->GetAura(PRIEST_CASCADE_INVISIBLE_AURA)->GetCaster())
                                    if (caster->GetGUID() == _player->GetGUID())
                                        targetList.remove(itr);

                            if (!itr->IsWithinLOSInMap(_player))
                                targetList.remove(itr);

                            if (!itr->isInFront(_player))
                                targetList.remove(itr);

                            if (itr->GetGUID() == _player->GetGUID())
                                targetList.remove(itr);

                            // damage
                            if (GetSpellInfo()->Id == 127630)
                                if (!_player->IsValidAttackTarget(itr))
                                    targetList.remove(itr);

                            // heal
                            if (GetSpellInfo()->Id == 120786)
                                if (_player->IsValidAttackTarget(itr))
                                    targetList.remove(itr);
                        }

                        // ... or if there are no targets reachable
                        if (targetList.size() == 0)
                            return;

                        // Each bound hit twice more targets up to 8 for the same bound
                        JadeCore::Containers::RandomResizeList(targetList, (affectedUnits * 2));

                        for (auto itr : targetList)
                        {
                            if (_player->HasAura(PRIEST_SHADOWFORM_STANCE))
                            {
                                switch (GetSpellInfo()->Id)
                                {
                                    // damage
                                    case 127630:
                                        target->CastSpell(itr, PRIEST_CASCADE_SHADOW_DAMAGE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                        break;
                                    // heal
                                    case 120786:
                                        target->CastSpell(itr, PRIEST_CASCADE_SHADOW_MISSILE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                        target->CastSpell(itr, PRIEST_CASCADE_SHADOW_HEAL, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                        break;
                                    default:
                                        break;
                                }
                            }
                            else
                            {
                                switch (GetSpellInfo()->Id)
                                {
                                    // damage
                                    case 127630:
                                        target->CastSpell(itr, PRIEST_CASCADE_HOLY_DAMAGE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                        break;
                                    // heal
                                    case 120786:
                                        target->CastSpell(itr, PRIEST_CASCADE_HOLY_MISSILE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                        target->CastSpell(itr, PRIEST_CASCADE_HOLY_HEAL, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                        break;
                                    default:
                                        break;
                                }
                            }

                            _player->CastSpell(itr, PRIEST_CASCADE_INVISIBLE_AURA, true);
                        }

                        if (AuraPtr boundNumber = _player->GetAura(PRIEST_CASCADE_INVISIBLE_AURA_2))
                        {
                            boundNumber->RefreshDuration();
                            boundNumber->SetCharges(boundNumber->GetCharges() + 1);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_cascade_second_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_cascade_second_SpellScript;
        }
};

// Cascade - 120785 (holy damage) or Cascade - 127628 (shadow damage) or Cascade - 127627 (shadow missile) or Cascade - 121146 (holy missile)
class spell_pri_cascade_trigger: public SpellScriptLoader
{
    public:
        spell_pri_cascade_trigger() : SpellScriptLoader("spell_pri_cascade_trigger") { }

        class spell_pri_cascade_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_cascade_trigger_SpellScript);

            void HandleOnHit()
            {
                if (GetOriginalCaster())
                {
                    if (Player* _player = GetOriginalCaster()->ToPlayer())
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            // Trigger for SpellScript
                            if (_player->IsValidAttackTarget(target))
                                _player->CastSpell(target, PRIEST_CASCADE_DAMAGE_TRIGGER, true); // Only damage
                            else
                                _player->CastSpell(target, PRIEST_CASCADE_HOLY_TRIGGER, true); // Only heal
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_cascade_trigger_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_cascade_trigger_SpellScript;
        }
};

// Cascade (shadow) - 127632 and Cascade - 121135
class spell_pri_cascade_first: public SpellScriptLoader
{
    public:
        spell_pri_cascade_first() : SpellScriptLoader("spell_pri_cascade_first") { }

        class spell_pri_cascade_first_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_cascade_first_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        switch (GetSpellInfo()->Id)
                        {
                            case 127632:
                            {
                                // First missile
                                if (_player->IsValidAttackTarget(target))
                                    _player->CastSpell(target, PRIEST_CASCADE_SHADOW_DAMAGE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                else
                                {
                                    _player->CastSpell(target, PRIEST_CASCADE_SHADOW_MISSILE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                    _player->CastSpell(target, PRIEST_CASCADE_SHADOW_HEAL, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                }

                                break;
                            }
                            case 121135:
                            {
                                // Cast shaow cascade if needed
                                if (_player->HasAura(15473))
                                {
                                    _player->CastSpell(target, 127632, true);
                                    return;
                                }

                                // First missile
                                if (_player->IsValidAttackTarget(target))
                                    _player->CastSpell(target, PRIEST_CASCADE_HOLY_DAMAGE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                else
                                {
                                    _player->CastSpell(target, PRIEST_CASCADE_HOLY_MISSILE, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                    _player->CastSpell(target, PRIEST_CASCADE_HOLY_HEAL, true, 0, NULLAURA_EFFECT, _player->GetGUID());
                                }

                                break;
                            }
                        }

                        // Invisible aura : Each target cannot be hit more than once time [...]
                        _player->CastSpell(target, PRIEST_CASCADE_INVISIBLE_AURA, true);
                        // Invisible aura 2 : [...] or Cascade can bound three times
                        _player->CastSpell(_player, PRIEST_CASCADE_INVISIBLE_AURA_2, true); // First bound

                        if (AuraPtr boundNumber = _player->GetAura(PRIEST_CASCADE_INVISIBLE_AURA_2))
                            boundNumber->SetCharges(1);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_cascade_first_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_cascade_first_SpellScript;
        }
};

// Halo - 120692 : Heal
class spell_pri_halo_heal: public SpellScriptLoader
{
    public:
        spell_pri_halo_heal() : SpellScriptLoader("spell_pri_halo_heal") { }

        class spell_pri_halo_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_halo_heal_SpellScript);

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Player = GetCaster())
                    if (Unit* l_Target = GetHitUnit())
                        if (l_Target->IsFriendlyTo(l_Player))
                        {
                            int32 l_Heal = GetHitHeal();

                            if (l_Player->GetDistance(l_Target) < 25.0f)
                                l_Heal = (l_Player->GetDistance(l_Target) / 25.0f) * GetHitHeal();

                            SetHitHeal(l_Heal);
                        }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pri_halo_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_halo_heal_SpellScript;
        }
};

enum Halo_Spell
{
    PRIEST_SPELL_HALO_DAMAGE = 120696
};

// Halo Damage (shadow) - 120696
class spell_pri_halo_damage: public SpellScriptLoader
{
    public:
        spell_pri_halo_damage() : SpellScriptLoader("spell_pri_halo_damage") { }

        class spell_pri_halo_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_halo_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Player = GetCaster()->ToPlayer())
                    if (Unit*   l_Target = GetHitUnit())
                        if (!l_Target->IsFriendlyTo(l_Player))
                        {
                            int32 l_Damage = GetHitDamage();
                            
                            if (l_Player->GetDistance(l_Target) < 25.0f)
                                l_Damage = (l_Player->GetDistance(l_Target) / 25.0f) * GetHitDamage();
                            
                            SetHitDamage(l_Damage);
                        }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_halo_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_halo_damage_SpellScript;
        }
};

// Halo - 120644 (damage)
// Halo - 120517 (heal)
class spell_pri_halo: public SpellScriptLoader
{
public:
    spell_pri_halo() : SpellScriptLoader("spell_pri_halo") { }

    class spell_pri_halo_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_halo_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Player = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    std::list<Creature*> l_TempListCreature;
                    std::list<Player*> l_TempListPlayer;

                    AreaTrigger* l_Area = l_Player->GetAreaTrigger(GetSpellInfo()->Id);

                    if (l_Area)
                    {
                        l_Area->GetCreatureListInGrid(l_TempListCreature, GetSpellInfo()->RangeEntry->maxRangeHostile);
                        for (std::list<Creature*>::iterator i = l_TempListCreature.begin(); i != l_TempListCreature.end(); ++i)
                        {
                            if (GetSpellInfo()->Id == PRIEST_SPELL_HALO_AREA_DAMAGE && !(*i)->IsFriendlyTo(l_Player))
                                l_Player->CastSpell((*i), PRIEST_SPELL_HALO_DAMAGE, true);
                            if (GetSpellInfo()->Id == PRIEST_SPELL_HALO_AREA_HEAL && (*i)->IsFriendlyTo(l_Player))
                                l_Player->CastSpell((*i), PRIEST_SPELL_HALO_HEAL_HOLY, true);
                        }

                        l_Area->GetPlayerListInGrid(l_TempListPlayer, GetSpellInfo()->RangeEntry->maxRangeHostile);
                        for (std::list<Player*>::iterator i = l_TempListPlayer.begin(); i != l_TempListPlayer.end(); ++i)
                        {
                            if (GetSpellInfo()->Id == PRIEST_SPELL_HALO_AREA_DAMAGE && (*i)->IsHostileTo(l_Player))
                                l_Player->CastSpell((*i), PRIEST_SPELL_HALO_DAMAGE, true);
                            if (GetSpellInfo()->Id == PRIEST_SPELL_HALO_AREA_HEAL && (*i)->IsFriendlyTo(l_Player))
                                l_Player->CastSpell((*i), PRIEST_SPELL_HALO_HEAL_HOLY, true);
                        }
                    }
                }
            }
        }

    void Register()
    {
        OnHit += SpellHitFn(spell_pri_halo_SpellScript::HandleOnHit);
    }
};

SpellScript* GetSpellScript() const
{
    return new spell_pri_halo_SpellScript;
}
};

// Leap of Faith - 73325
class spell_pri_leap_of_faith: public SpellScriptLoader
{
    public:
        spell_pri_leap_of_faith() : SpellScriptLoader("spell_pri_leap_of_faith") { }

        class spell_pri_leap_of_faith_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_leap_of_faith_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Player = GetCaster())
                    if (Unit* l_Target = GetHitUnit())
                        l_Target->CastSpell(l_Player, PRIEST_LEAP_OF_FAITH_JUMP, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_leap_of_faith_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_leap_of_faith_SpellScript;
        }
};

enum PsychicHorror_Spell
{
    PRIEST_SPELL_PSYCHIC_HORROR = 64044
};

// Psychic Horror - 64044
class spell_pri_psychic_horror: public SpellScriptLoader
{
    public:
        spell_pri_psychic_horror() : SpellScriptLoader("spell_pri_psychic_horror") { }

        class spell_pri_psychic_horror_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_psychic_horror_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->ToPlayer() && l_Caster->ToPlayer()->GetSpecializationId(l_Caster->ToPlayer()->GetActiveSpec()) == SPEC_PRIEST_SHADOW)
                        {
                            int32 l_CurrentPowerUsed = l_Caster->GetPower(POWER_SHADOW_ORB);
                            if (l_CurrentPowerUsed > 3) // Maximum 3 Shadow Orb can be consumed
                                l_CurrentPowerUsed = 3;
                            l_Caster->ModifyPower(POWER_SHADOW_ORB, -l_CurrentPowerUsed);

                            // +1s per Shadow Orb consumed
                            if (AuraPtr l_PsychicHorror = l_Target->GetAura(PRIEST_SPELL_PSYCHIC_HORROR))
                            {
                                int32 l_MaxDuration = l_PsychicHorror->GetMaxDuration();
                                int32 l_NewDuration = l_MaxDuration + GetSpellInfo()->Effects[EFFECT_0].BasePoints + l_CurrentPowerUsed * IN_MILLISECONDS;
                                l_PsychicHorror->SetDuration(l_NewDuration);

                                if (l_NewDuration > l_MaxDuration)
                                    l_PsychicHorror->SetMaxDuration(l_NewDuration);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_psychic_horror_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_psychic_horror_SpellScript();
        }
};

// Guardian Spirit - 47788
class spell_pri_guardian_spirit: public SpellScriptLoader
{
    public:
        spell_pri_guardian_spirit() : SpellScriptLoader("spell_pri_guardian_spirit") { }

        class spell_pri_guardian_spirit_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_guardian_spirit_AuraScript);

            uint32 healPct;

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(PRIEST_SPELL_GUARDIAN_SPIRIT_HEAL))
                    return false;
                return true;
            }

            bool Load()
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                return true;
            }

            void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                Unit* target = GetTarget();
                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                int32 healAmount = int32(target->CountPctFromMaxHealth(healPct));
                // remove the aura now, we don't want 40% healing bonus
                Remove(AURA_REMOVE_BY_ENEMY_SPELL);
                target->CastCustomSpell(target, PRIEST_SPELL_GUARDIAN_SPIRIT_HEAL, &healAmount, NULL, NULL, true);
                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_guardian_spirit_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_guardian_spirit_AuraScript::Absorb, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_guardian_spirit_AuraScript();
        }
};

// Penance - 47540
class spell_pri_penance: public SpellScriptLoader
{
    public:
        spell_pri_penance() : SpellScriptLoader("spell_pri_penance") { }

        class spell_pri_penance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_penance_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(PRIEST_SPELL_PENANCE))
                    return false;
                // can't use other spell than this penance due to spell_ranks dependency
                if (sSpellMgr->GetFirstSpellInChain(PRIEST_SPELL_PENANCE) != sSpellMgr->GetFirstSpellInChain(spellEntry->Id))
                    return false;

                uint8 rank = sSpellMgr->GetSpellRank(spellEntry->Id);
                if (!sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_DAMAGE, rank, true))
                    return false;
                if (!sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_HEAL, rank, true))
                    return false;

                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (!unitTarget->isAlive())
                            return;

                        uint8 rank = sSpellMgr->GetSpellRank(GetSpellInfo()->Id);

                        if (_player->IsFriendlyTo(unitTarget))
                            _player->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_HEAL, rank), false, 0);
                        else
                            _player->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_DAMAGE, rank), false, 0);

                        // Divine Insight (Discipline)
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_PRIEST_DISCIPLINE)
                            if (_player->HasAura(PRIEST_SPELL_DIVINE_INSIGHT_TALENT))
                                _player->CastSpell(_player, PRIEST_SPELL_DIVINE_INSIGHT_DISCIPLINE, true);
                    }
                }
            }

            SpellCastResult CheckCast()
            {
                Player* caster = GetCaster()->ToPlayer();
                if (Unit* target = GetExplTargetUnit())
                    if (!caster->IsFriendlyTo(target) && !caster->IsValidAttackTarget(target))
                        return SPELL_FAILED_BAD_TARGETS;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                // add dummy effect spell handler to Penance
                OnEffectHitTarget += SpellEffectFn(spell_pri_penance_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_pri_penance_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_penance_SpellScript;
        }
};

// Vampiric Touch - 34914
class spell_pri_vampiric_touch: public SpellScriptLoader
{
    public:
        spell_pri_vampiric_touch() : SpellScriptLoader("spell_pri_vampiric_touch") { }

        class spell_pri_vampiric_touch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_vampiric_touch_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->EnergizeBySpell(GetCaster(), GetSpellInfo()->Id, l_Caster->CountPctFromMaxMana(2), POWER_MANA);

                    if (l_Caster->HasAura(PRIEST_SURGE_OF_DARKNESS_AURA))
                        if (roll_chance_i(sSpellMgr->GetSpellInfo(PRIEST_SURGE_OF_DARKNESS_AURA)->Effects[EFFECT_0].BasePoints))
                            l_Caster->CastSpell(l_Caster, PRIEST_SURGE_OF_DARKNESS, true);
                }
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Dispeller = dispelInfo->GetDispeller())
                        l_Dispeller->CastSpell(l_Dispeller, PRIEST_SPELL_SIN_AND_PUNISHMENT, true, 0, NULLAURA_EFFECT, l_Caster->GetGUID());
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_vampiric_touch_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
                AfterDispel += AuraDispelFn(spell_pri_vampiric_touch_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_vampiric_touch_AuraScript();
        }
};

// Called by Renew - 139
// Rapid Renew - 95649
class spell_pri_renew: public SpellScriptLoader
{
    public:
        spell_pri_renew() : SpellScriptLoader("spell_pri_renew") { }

        class spell_pri_renew_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_renew_AuraScript);

            void HandleApplyEffect(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    // Empowered Renew
                    if (AuraPtr empoweredRenew = caster->GetAura(PRIEST_RAPID_RENEWAL_AURA))
                    {
                        uint32 heal = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), GetEffect(EFFECT_0)->GetAmount(), EFFECT_0, DOT);
                        heal = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, DOT);

                        int32 basepoints0 = empoweredRenew->GetEffect(EFFECT_2)->GetAmount() * GetEffect(EFFECT_0)->GetTotalTicks() * int32(heal) / 100;
                        caster->CastCustomSpell(GetTarget(), PRIEST_SPELL_EMPOWERED_RENEW, &basepoints0, NULL, NULL, true, NULL, aurEff);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_pri_renew_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_renew_AuraScript();
        }
};

// Levitate - 1706
class spell_pri_levitate: public SpellScriptLoader
{
    public:
        spell_pri_levitate() : SpellScriptLoader("spell_pri_levitate") { }

        class spell_pri_levitate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_levitate_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster())
                    if (GetHitUnit())
                        GetCaster()->CastSpell(GetHitUnit(), PRIEST_SPELL_LEVITATE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pri_levitate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_levitate_SpellScript;
        }
};

// Call by Flah Heal 2061 - Heal 2060 - Prayer of healing 596
// Spirit Shell - 109964
class spell_pri_spirit_shell: public SpellScriptLoader
{
public:
    spell_pri_spirit_shell() : SpellScriptLoader("spell_pri_spirit_shell") { }

    class spell_pri_spirit_shell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_spirit_shell_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
            if (l_Caster->GetAura(PRIEST_SPELL_SPIRIT_SHELL_AURA))
            {
                int32 l_Absorb = GetHitHeal();
                l_Caster->CastCustomSpell(GetHitUnit(), PRIEST_SPELL_SPIRIT_SHELL_PROC, &l_Absorb, NULL, NULL, true);
                SetHitHeal(0);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_spirit_shell_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pri_spirit_shell_SpellScript();
    }
};

// Flash heal - 2061
class spell_pri_flash_heal: public SpellScriptLoader
{
public:
    spell_pri_flash_heal() : SpellScriptLoader("spell_pri_flash_heal") { }

    class spell_pri_flash_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_flash_heal_SpellScript);

        void HandleBeforeCast()
        {
            if (Unit* l_Caster = GetCaster())
                if (AuraPtr surgeOfLight = l_Caster->GetAura(PRIEST_SURGE_OF_LIGHT))
                    surgeOfLight->ModStackAmount(-1);
        }

        void Register()
        {
            BeforeCast += SpellCastFn(spell_pri_flash_heal_SpellScript::HandleBeforeCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pri_flash_heal_SpellScript();
    }
};

// Clarity of will - 152118
class spell_pri_clarity_of_will: public SpellScriptLoader
{
public:
    spell_pri_clarity_of_will() : SpellScriptLoader("spell_pri_clarity_of_will") { }

    class spell_pri_clarity_of_will_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_clarity_of_will_AuraScript);

        void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (Player* l_Player = l_Caster->ToPlayer())
                    amount = l_Player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL) * 6 * 1.1;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_clarity_of_will_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pri_clarity_of_will_AuraScript();
    }
};

// Void Tendrils - 108920
class spell_pri_void_tendrils: public SpellScriptLoader
{
    public:
        spell_pri_void_tendrils() : SpellScriptLoader("spell_pri_void_tendrils") { }

        class spell_pri_void_tendrils_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_void_tendrils_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, PRIEST_SPELL_VOID_TENDRILS_SUMMON, true);

                        if (Creature* voidTendrils = target->FindNearestCreature(PRIEST_NPC_VOID_TENDRILS, GetSpellInfo()->Effects[EFFECT_0].RadiusEntry->radiusHostile))
                            if (voidTendrils->AI())
                                voidTendrils->AI()->SetGUID(target->GetGUID());

                        if (AuraPtr voidTendrils = target->GetAura(GetSpellInfo()->Id, _player->GetGUID()))
                        {
                            if (target->GetTypeId() == TYPEID_PLAYER)
                                voidTendrils->SetMaxDuration(8000);
                            else
                                voidTendrils->SetMaxDuration(20000);
                            voidTendrils->SetDuration(voidTendrils->GetMaxDuration());
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_void_tendrils_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_void_tendrils_SpellScript();
        }
};

// Archangel - 81700
class spell_pri_archangel: public SpellScriptLoader
{
public:
    spell_pri_archangel() : SpellScriptLoader("spell_pri_archangel") { }

    class spell_pri_archangel_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_archangel_AuraScript);

        void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (AuraPtr l_Aura = l_Caster->GetAura(PRIEST_EVANGELISM_STACK))
                {
                    amount = l_Aura->GetStackAmount() * GetSpellInfo()->Effects[0].BasePoints;
                    l_Caster->RemoveAura(l_Aura);
                }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_archangel_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_HEALING_DONE_PERCENT);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pri_archangel_AuraScript();
    }
};

enum PrayerOfMendingSpells
{
    PrayerOfMendingAura     = 41635,
    PrayerOfMendingHeal     = 33110,
    T9Healing2Pieces        = 67201
};

// Prayer of Mending - 33076
class spell_pri_prayer_of_mending: public SpellScriptLoader
{
    public:
        spell_pri_prayer_of_mending() : SpellScriptLoader("spell_pri_prayer_of_mending") {}

        class spell_pri_prayer_of_mending_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_prayer_of_mending_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        l_Caster->CastSpell(l_Target, PrayerOfMendingSpells::PrayerOfMendingAura, true);
                        if (AuraPtr l_PrayerOfMendingAura = l_Target->GetAura(PrayerOfMendingSpells::PrayerOfMendingAura))
                            l_PrayerOfMendingAura->SetStackAmount(5);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pri_prayer_of_mending_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_prayer_of_mending_SpellScript();
        }
};

/// Prayer of mending aura - 41635
class spell_pri_prayer_of_mending_aura : public SpellScriptLoader
{
    public:
        spell_pri_prayer_of_mending_aura() : SpellScriptLoader("spell_pri_prayer_of_mending_aura") { }

        class spell_pri_prayer_of_mending_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_prayer_of_mending_aura_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetHealInfo())
                {
                    if (p_EventInfo.GetHealInfo()->GetHeal() > 0)
                        return;
                }

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = p_EventInfo.GetActionTarget())
                    {
                        l_Caster->CastSpell(l_Target, PrayerOfMendingSpells::PrayerOfMendingHeal, true);

                        uint8 l_CurrentStackAmount = p_AurEff->GetBase()->GetStackAmount();

                        if (l_CurrentStackAmount >= 1)
                        {
                            std::list<Unit*> l_FriendlyUnitListTemp;
                            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, 20.0f);
                            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_FriendlyUnitListTemp, l_Check);
                            l_Caster->VisitNearbyObject(20.0f, l_Searcher);

                            if (!l_FriendlyUnitListTemp.empty())
                            {
                                std::list<Unit*> l_FriendlyUnitList;

                                for (auto l_Itr : l_FriendlyUnitListTemp)
                                {
                                    if (l_Caster->IsValidAssistTarget(l_Itr))
                                        l_FriendlyUnitList.push_back(l_Itr);
                                }

                                if (!l_FriendlyUnitList.empty())
                                {
                                    JadeCore::Containers::RandomResizeList(l_FriendlyUnitList, 1);

                                    for (auto l_Itr : l_FriendlyUnitList)
                                    {
                                        l_Caster->CastSpell(l_Itr, PrayerOfMendingSpells::PrayerOfMendingAura, true);
                                        if (AuraPtr l_PrayerOfMendingAura = l_Itr->GetAura(PrayerOfMendingSpells::PrayerOfMendingAura, l_Caster->GetGUID()))
                                            l_PrayerOfMendingAura->SetStackAmount(l_CurrentStackAmount - 1);
                                    }
                                }
                            }
                        }

                        l_Target->RemoveAura(PrayerOfMendingSpells::PrayerOfMendingAura);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pri_prayer_of_mending_aura_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_prayer_of_mending_aura_AuraScript();
        }
};

/// Prayer of Mending Heal
class spell_pri_prayer_of_mending_heal : public SpellScriptLoader
{
    public:
        spell_pri_prayer_of_mending_heal() : SpellScriptLoader("spell_pri_prayer_of_mending_heal") { }

        class spell_pri_prayer_of_mending_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_prayer_of_mending_heal_SpellScript);

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetOriginalCaster())
                {
                    if (AuraEffectPtr l_AurEff = l_Caster->GetAuraEffect(PrayerOfMendingSpells::T9Healing2Pieces, EFFECT_0))
                    {
                        int32 l_Heal = GetHitHeal();
                        AddPct(l_Heal, l_AurEff->GetAmount());
                        SetHitHeal(l_Heal);
                    }
                }

            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pri_prayer_of_mending_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_prayer_of_mending_heal_SpellScript();
        }
};

// Call by Mind Spike 73510 - Mind Sear 48045 - Shadow Word: Death 32379
// Clarity of Power - 155246
class spell_pri_clarity_of_power: public SpellScriptLoader
{
public:
    spell_pri_clarity_of_power() : SpellScriptLoader("spell_pri_clarity_of_power") {}

    class spell_pri_clarity_of_power_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_clarity_of_power_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
                if (Unit *l_Target = GetHitUnit())
                    if (l_Caster->HasAura(PRIEST_SPELL_CLARITY_OF_POWER))
                        if (!(l_Target->HasAura(PRIEST_SHADOW_WORD_PAIN)) && !(l_Target->HasAura(PRIEST_VAMPIRIC_TOUCH))) // Shadow word: pain / Vampiric touch
                            SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), sSpellMgr->GetSpellInfo(PRIEST_SPELL_CLARITY_OF_POWER)->Effects[EFFECT_0].BasePoints));
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_pri_clarity_of_power_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pri_clarity_of_power_SpellScript();
    }
};

// Angelic Feather - 121536
class spell_pri_angelic_feather: public SpellScriptLoader
{
public:
    spell_pri_angelic_feather() : SpellScriptLoader("spell_pri_angelic_feather") {}

    class spell_pri_angelic_feather_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_angelic_feather_SpellScript);

        void HandleOnCast()
        {
            if (Unit *l_Caster = GetCaster())
                if (WorldLocation* l_Dest = const_cast<WorldLocation*>(GetExplTargetDest()))
                    l_Caster->CastSpell(l_Dest->GetPositionX(), l_Dest->GetPositionY(), l_Dest->GetPositionZ(), 158624, true);
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_pri_angelic_feather_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pri_angelic_feather_SpellScript();
    }
};

// Shadow Word: Pain - 589
class spell_pri_shadow_word_pain: public SpellScriptLoader
{
public:
    spell_pri_shadow_word_pain() : SpellScriptLoader("spell_pri_shadow_word_pain") { }

    class spell_pri_shadow_word_pain_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_shadow_word_pain_AuraScript);

        void OnTick(constAuraEffectPtr aurEff)
        {
            if (Unit *l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(PRIEST_SPELL_SHADOW_INSIGHT))
                    if (roll_chance_i(sSpellMgr->GetSpellInfo(PRIEST_SPELL_SHADOW_INSIGHT)->Effects[EFFECT_3].BasePoints))
                        l_Caster->CastSpell(l_Caster, PRIEST_SPELL_SHADOW_INSIGHT_PROC, true);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_shadow_word_pain_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pri_shadow_word_pain_AuraScript();
    }
};

// Mind Blast - 8092
class spell_pri_mind_blast: public SpellScriptLoader
{
    public:
        spell_pri_mind_blast() : SpellScriptLoader("spell_pri_mind_blast") { }

        class spell_pri_mind_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_mind_blast_SpellScript);

            void HandleAdditionalsOrbs(SpellEffIndex p_EffIndex)
            {
                PreventHitDefaultEffect(p_EffIndex);

                if (!GetCaster()->HasAura(PRIEST_GLYPH_OF_MIND_HARVEST))
                    return;

                GetSpell()->EffectEnergize(p_EffIndex);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pri_mind_blast_SpellScript::HandleAdditionalsOrbs, EFFECT_3, SPELL_EFFECT_ENERGIZE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_mind_blast_SpellScript();
        }
};

// Glyphe of Mind Blast - 87195
class spell_pri_glyphe_of_mind_blast : public SpellScriptLoader
{
public:
    spell_pri_glyphe_of_mind_blast() : SpellScriptLoader("spell_pri_glyphe_of_mind_blast") { }

    class spell_pri_glyphe_of_mind_blast_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_glyphe_of_mind_blast_AuraScript);

        void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
        {
            PreventDefaultAction();

            Unit* l_Caster = GetCaster();

            if (!l_Caster)
                return;

            if (p_EventInfo.GetActor()->GetGUID() != l_Caster->GetGUID())
                return;

            if (!p_EventInfo.GetDamageInfo()->GetSpellInfo() || p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != PRIEST_SPELL_MIND_BLAST || !GetSpellInfo())
                return;

            if (!(p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                return;

            l_Caster->CastSpell(p_EventInfo.GetDamageInfo()->GetVictim(), PRIEST_GLYPH_OF_MIND_BLAST, true);
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_pri_glyphe_of_mind_blast_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pri_glyphe_of_mind_blast_AuraScript();
    }
};

enum MindFlay
{
    GlyphOfMindFlay = 120585,
    GlyphOfMindFlayTick = 120587
};

/// Mind Flay - 15407
class spell_pri_mind_flay : public SpellScriptLoader
{
    public:
        spell_pri_mind_flay() : SpellScriptLoader("spell_pri_mind_flay") { }

        class spell_pri_mind_flay_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_mind_flay_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                // Glyph of Mind Flay
                if (l_Caster->HasAura(MindFlay::GlyphOfMindFlay))
                    l_Caster->CastSpell(l_Caster, MindFlay::GlyphOfMindFlayTick, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_mind_flay_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_mind_flay_AuraScript();
        }
};


enum WordsOfMendingSpells
{
    WordsOfMendingAuraStack     = 155362,
    WordsOfMendingAuraFinal     = 155363,
    PrayerOfMendingSpell         = 33076
};

/// Words of mending - 152117
class spell_pri_words_of_mending : public SpellScriptLoader
{
    public:
        spell_pri_words_of_mending() : SpellScriptLoader("spell_pri_words_of_mending") { }

        class spell_pri_words_of_mending_Aurascript : public AuraScript
        {
            PrepareAuraScript(spell_pri_words_of_mending_Aurascript);

            void HandleOnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_ProcInfos)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(WordsOfMendingSpells::WordsOfMendingAuraFinal))
                    {
                        if (Unit* l_Target = p_ProcInfos.GetProcTarget())
                        {
                            l_Caster->CastSpell(l_Target, WordsOfMendingSpells::PrayerOfMendingSpell, true);
                            l_Caster->RemoveAura(WordsOfMendingSpells::WordsOfMendingAuraFinal);
                        }
                    }
                    else if (AuraPtr l_WordsOfMendingAura = l_Caster->GetAura(WordsOfMendingSpells::WordsOfMendingAuraStack))
                    {
                        if (l_WordsOfMendingAura->GetStackAmount() > 9)
                        {
                            l_Caster->AddAura(WordsOfMendingSpells::WordsOfMendingAuraFinal, l_Caster);
                            l_Caster->RemoveAura(WordsOfMendingSpells::WordsOfMendingAuraStack);
                        }
                        else
                            l_Caster->AddAura(WordsOfMendingSpells::WordsOfMendingAuraStack, l_Caster);
                    }
                    else
                    {
                        l_Caster->AddAura(WordsOfMendingSpells::WordsOfMendingAuraStack, l_Caster);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pri_words_of_mending_Aurascript::HandleOnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_words_of_mending_Aurascript();
        }
};

// Insanity - 132573
class PlayerScript_insanity: public PlayerScript
{
    public:
        PlayerScript_insanity() :PlayerScript("PlayerScript_insanity") {}

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
        {
            if (p_Regen)
                return;

            // Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_diffValue = p_NewValue - p_OldValue;

            if (p_Player->getClass() == CLASS_PRIEST && p_Player->GetSpecializationId(p_Player->GetActiveSpec()) == SPEC_PRIEST_SHADOW && p_Power == POWER_SHADOW_ORB)
                if (l_diffValue < 0 && p_Player->HasAura(PRIEST_SPELL_INSANITY_AURA))
                {
                    p_Player->CastSpell(p_Player, PRIEST_SPELL_INSANITY, true);
                    if (AuraPtr l_Insanity = p_Player->GetAura(PRIEST_SPELL_INSANITY))
                        l_Insanity->SetDuration(l_Insanity->GetMaxDuration() * (l_diffValue * -1));
                }
        }
};

enum AreaTriggerSpells
{
    SPELL_DIVINE_STAR_HOLY = 110744,
    SPELL_DIVINE_STAR_HEAL = 110745,
    SPELL_DIVINE_STAR_DAMAGE = 122128,
};

class at_pri_divine_star : public AreaTriggerEntityScript
{
    public:
        at_pri_divine_star()
            : AreaTriggerEntityScript("at_pri_divine_star") { }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_pri_divine_star();
        }

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination)
        {
            Position l_Position;
            float l_Dist = 24.f; // Hardcoded in the tooltip;

            l_Position.m_positionX = p_SourcePosition.m_positionX + (l_Dist * cos(p_Caster->GetOrientation()));
            l_Position.m_positionY = p_SourcePosition.m_positionY + (l_Dist * sin(p_Caster->GetOrientation()));
            l_Position.m_positionZ = p_SourcePosition.m_positionZ;
            p_Caster->UpdateGroundPositionZ(l_Position.m_positionX, l_Position.m_positionY, l_Position.m_positionZ);

            p_PathToLinearDestination.push_back(l_Position);
            p_DestinationPosition = p_SourcePosition; // Return back
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (!l_Caster)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 3.f;
            bool friendly = p_AreaTrigger->GetSpellId() == SPELL_DIVINE_STAR_HOLY;
            uint32 l_SpellID = friendly ? SPELL_DIVINE_STAR_HEAL : SPELL_DIVINE_STAR_DAMAGE;

            for (std::map<uint64, uint32>::iterator iter = m_Cooldows.begin(); iter != m_Cooldows.end();)
            {
                if (iter->second < p_Time)
                    iter = m_Cooldows.erase(iter);
                else
                {
                    iter->second -= p_Time;
                    iter++;
                }
            }

            if (friendly)
            {
                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);
            }
            else
            {
                JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
                JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);
            }

            for (auto l_Unit : l_TargetList)
            {
                if (m_Cooldows.find(l_Unit->GetGUID()) != m_Cooldows.end())
                    continue;

                m_Cooldows.insert({ l_Unit->GetGUID(), 500 });
                l_Caster->CastSpell(l_Unit, l_SpellID, true);
            }
        }

        std::map<uint64, uint32> m_Cooldows;
};

enum TwistOfFateSpells
{
    SPELL_PRI_TWIST_OF_FATE_PROC = 123254
};

/// Twist of Fate - 109142
class spell_pri_twist_of_fate : public SpellScriptLoader
{
    public:
        spell_pri_twist_of_fate() : SpellScriptLoader("spell_pri_twist_of_fate") { }

        class spell_pri_twist_of_fate_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_twist_of_fate_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* l_Caster = GetCaster()->ToPlayer();
                if (!l_Caster)
                    return;

                /// GetDamageInfo and GetHealInfo contain the same infos no matter if it's a heal or damage spell
                SpellInfo const* l_SpellInfoProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_SpellInfoProcSpell)
                    return;

                if ((l_Caster->GetSpecializationId(l_Caster->GetActiveSpec()) == SPEC_PRIEST_SHADOW && l_SpellInfoProcSpell->IsPositive()) ||
                    (l_Caster->GetSpecializationId(l_Caster->GetActiveSpec()) != SPEC_PRIEST_SHADOW && !l_SpellInfoProcSpell->IsPositive()))
                    return;

                Unit* l_Target = p_EventInfo.GetActionTarget();
                if (!l_Target || l_Target == l_Caster || l_Target->GetHealthPct() > p_AurEff->GetAmount())
                    return;

                l_Caster->CastSpell(l_Caster, SPELL_PRI_TWIST_OF_FATE_PROC, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pri_twist_of_fate_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_twist_of_fate_AuraScript();
        }
};

/// Power Word: Barrier - 62618
class spell_areatrigger_power_word_barrier : public AreaTriggerEntityScript
{
    public:
        spell_areatrigger_power_word_barrier() : AreaTriggerEntityScript("spell_areatrigger_power_word_barrier") { }

        enum ePowerWordBarrierSpell
        {
            PowerWordBarrierAura     = 81782
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_FriendListInRadius;
                float l_Radius = 6.5f;

                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_FriendListInRadius, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                if (!l_FriendListInRadius.empty())
                {
                    for (Unit* l_Unit : l_FriendListInRadius)
                    {
                        if (l_Caster->IsValidAssistTarget(l_Unit))
                            l_Caster->CastSpell(l_Unit, ePowerWordBarrierSpell::PowerWordBarrierAura, true);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_areatrigger_power_word_barrier();
        }
};

void AddSC_priest_spell_scripts()
{
    new spell_pri_mind_flay();
    new spell_pri_glyphe_of_mind_blast();
    new spell_pri_mind_blast();
    new spell_pri_shadow_word_pain();
    new spell_pri_angelic_feather();
    new spell_pri_spirit_shell();
    new spell_pri_clarity_of_power();
    new spell_pri_prayer_of_mending();
    new spell_pri_prayer_of_mending_heal();
    new spell_pri_prayer_of_mending_aura();
    new spell_pri_archangel();
    new spell_pri_void_tendrils();
    new spell_pri_clarity_of_will();
    new spell_pri_confession();
    new spell_pri_glyph_of_confession();
    new spell_pri_shadow_word_death();
    new spell_pri_psychic_horror();
    new spell_pri_holy_nova_heal();
    new spell_pri_holy_nova();
    new spell_pri_glyph_of_holy_nova();
    new spell_pri_spectral_guise_charges();
    new spell_pri_spirit_of_redemption_form();
    new spell_pri_spirit_of_redemption();
    new spell_pri_item_s12_4p_heal();
    new spell_pri_item_s12_2p_shadow();
    new spell_pri_divine_insight_shadow();
    new spell_pri_power_word_solace();
    new spell_pri_shadowfiend();
    new spell_pri_surge_of_light();
    new spell_pri_power_word_shield();
    new spell_pri_body_and_soul();
    new spell_pri_prayer_of_mending_divine_insight();
    new spell_pri_divine_insight_holy();
    new spell_pri_divine_insight_discipline();
    new spell_pri_holy_word_sanctuary();
    new spell_pri_smite();
    new spell_pri_lightwell_renew();
    new spell_pri_atonement();
    new spell_pri_purify();
    new spell_pri_devouring_plague();
    new spell_pri_devouring_plague_aura();
    new spell_pri_phantasm();
    new spell_pri_mind_spike();
    new spell_pri_cascade_second();
    new spell_pri_cascade_trigger();
    new spell_pri_cascade_first();
    new spell_pri_halo();
    new spell_pri_halo_heal();
    new spell_pri_halo_damage();
    new spell_pri_leap_of_faith();
    new spell_pri_psychic_horror();
    new spell_pri_guardian_spirit();
    new spell_pri_penance();
    new spell_pri_vampiric_touch();
    new spell_pri_renew();
    new spell_pri_evangelism();
    new spell_pri_levitate();
    new spell_pri_flash_heal();
    new spell_pri_words_of_mending();
    new spell_pri_twist_of_fate();

    /// Player Script
    new PlayerScript_Shadow_Orb();
    new PlayerScript_insanity();

    /// Areatrigger scripts
    new at_pri_divine_star();
    new spell_areatrigger_power_word_barrier();
}
