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
 * Scripts for spells with SPELLFAMILY_HUNTER, SPELLFAMILY_PET and SPELLFAMILY_GENERIC spells used by hunter players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_hun_".
 */

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum HunterSpells
{
    HUNTER_SPELL_BESTIAL_WRATH                      = 19574,
    HUNTER_PET_SPELL_LAST_STAND_TRIGGERED           = 53479,
    HUNTER_PET_HEART_OF_THE_PHOENIX                 = 55709,
    HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED       = 54114,
    HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF          = 55711,
    HUNTER_PET_SPELL_CARRION_FEEDER_TRIGGERED       = 54045,
    HUNTER_SPELL_POSTHASTE                          = 109215,
    HUNTER_SPELL_POSTHASTE_INCREASE_SPEED           = 118922,
    HUNTER_SPELL_NARROW_ESCAPE                      = 109298,
    HUNTER_SPELL_NARROW_ESCAPE_RETS                 = 128405,
    HUNTER_SPELL_SERPENT_STING                      = 118253,
    HUNTER_SPELL_SERPENT_SPREAD                     = 87935,
    HUNTER_SPELL_RAPID_FIRE                         = 3045,
    HUNTER_SPELL_STEADY_SHOT_ENERGIZE               = 77443,
    HUNTER_SPELL_COBRA_SHOT_ENERGIZE                = 91954,
    HUNTER_SPELL_KILL_COMMAND                       = 34026,
    HUNTER_SPELL_KILL_COMMAND_TRIGGER               = 83381,
    HUNTER_SPELL_KILL_COMMAND_CHARGE                = 118171,
    HUNTER_SPELL_SPIRIT_BOND                        = 118694,
    SPELL_MAGE_TEMPORAL_DISPLACEMENT                = 80354,
    HUNTER_SPELL_INSANITY                           = 95809,
    SPELL_SHAMAN_SATED                              = 57724,
    SPELL_SHAMAN_EXHAUSTED                          = 57723,
    HUNTER_SPELL_FATIGUED                           = 160455,
    HUNTER_SPELL_CAMOUFLAGE_VISUAL                  = 80326,
    HUNTER_SPELL_GLYPH_OF_CAMOUFLAGE_VISUAL         = 119450,
    HUNTER_SPELL_CAMOUFLAGE_PERIODIC_HEAL           = 51753,
    HUNTER_SPELL_POWERSHOT                          = 109259,
    HUNTER_SPELL_IMPROVED_SERPENT_STING_AURA        = 82834,
    HUNTER_SPELL_IMPROVED_SERPENT_STING             = 83077,
    HUNTER_SPELL_GLAIVE_TOSS_DAMAGES                = 121414,
    HUNTER_SPELL_GLAIVE_TOSS                        = 117050,
    HUNTER_SPELL_MASTERS_CALL                       = 62305,
    HUNTER_SPELL_MASTERS_CALL_TRIGGERED             = 54216,
    HUNTER_SPELL_COBRA_STRIKES_AURA                 = 53260,
    HUNTER_SPELL_COBRA_STRIKES_STACKS               = 53257,
    HUNTER_SPELL_BEAST_CLEAVE_AURA                  = 115939,
    HUNTER_SPELL_BEAST_CLEAVE_PROC                  = 118455,
    HUNTER_SPELL_LYNX_RUSH_AURA                     = 120697,
    HUNTER_SPELL_LYNX_CRUSH_DAMAGE                  = 120699,
    HUNTER_SPELL_FRENZY_STACKS                      = 19615,
    HUNTER_SPELL_FOCUS_FIRE_AURA                    = 82692,
    HUNTER_SPELL_DIRE_BEAST                         = 120679,
    DIRE_BEAST_JADE_FOREST                          = 121118,
    DIRE_BEAST_KALIMDOR                             = 122802,
    DIRE_BEAST_EASTERN_KINGDOMS                     = 122804,
    DIRE_BEAST_OUTLAND                              = 122806,
    DIRE_BEAST_NORTHREND                            = 122807,
    DIRE_BEAST_KRASARANG_WILDS                      = 122809,
    DIRE_BEAST_VALLEY_OF_THE_FOUR_WINDS             = 122811,
    DIRE_BEAST_VALE_OF_THE_ETERNAL_BLOSSOM          = 126213,
    DIRE_BEAST_KUN_LAI_SUMMIT                       = 126214,
    DIRE_BEAST_TOWNLONG_STEPPES                     = 126215,
    DIRE_BEAST_DREAD_WASTES                         = 126216,
    DIRE_BEAST_DUNGEONS                             = 132764,
    HUNTER_SPELL_GLYPH_OF_COLLAPSE                  = 126095,
    HUNTER_SPELL_HUNTERS_MARK                       = 1130,
    HUNTER_SPELL_TRACK_BEASTS                       = 1494,
    HUNTER_SPELL_TRACK_DEMONS                       = 19878,
    HUNTER_SPELL_TRACK_DRAGONKIN                    = 19879,
    HUNTER_SPELL_TRACK_ELEMENTALS                   = 19880,
    HUNTER_SPELL_TRACK_GIANTS                       = 19882,
    HUNTER_SPELL_TRACK_HUMANOIDS                    = 19883,
    HUNTER_SPELL_TRACK_UNDEAD                       = 19884,
    HUNTER_SPELL_TRACK_HIDDEN                       = 19885,
    HUNTER_SPELL_VIPER_VENOM                        = 118976,
    HUNTER_SPELL_FETCH                              = 125050,
    HUNTER_SPELL_GLAIVE_TOSS_AURA                   = 117050,
    HUNTER_SPELL_GLAIVE_TOSS_RIGHT                  = 120755,
    HUNTER_SPELL_GLAIVE_TOSS_LEFT                   = 120756,
    HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT  = 120761,
    HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT = 121414,
    HUNTER_SPELL_ASPECT_OF_THE_BEAST                = 61648,
    HUNTER_SPELL_SPIRIT_BOND_HEAL                   = 149254,
    HUNTER_SPELL_ARCANE_INTENSITY                   = 142978,
    HUNTER_SPELL_A_MURDER_OF_CROWS_DAMAGE           = 131900,
    HUNTER_SPELL_GLYPH_OF_LIBERATION                = 132106,
    HUNTER_SPELL_GLYPH_OF_LIBERATION_HEAL           = 115927,
    HUNTER_SPELL_GLYPH_OF_ASPECTS                   = 122492,
    HUNTER_SPELL_ASPECT_OF_THE_CHEETAH              = 5118,
    HUNTER_SPELL_ASPECT_OF_THE_CHEETAH_SUMMON       = 122489,
    HUNTER_SPELL_ASPECT_OF_THE_HAWK                 = 13165,
    HUNTER_SPELL_ASPECT_OF_THE_HAWK_SUMMON          = 122487,
    HUNTER_SPELL_ASPECT_OF_THE_PACK                 = 13159,
    HUNTER_SPELL_ASPECT_OF_THE_PACK_SUMMON          = 122490,
    HUNTER_SPELL_FIREWORKS                          = 127933,
    HUNTER_SPELL_KILL_SHOT_HEAL                     = 164851,
    HUNTER_SPELL_ARCANE_INTENSITY_AURA              = 131564,
    HUNTER_SPELL_THRILL_OF_THE_HUNT                 = 109306,
    HUNTER_SPELL_THRILL_OF_THE_HUNT_PROC            = 34720,
    HUNTER_SPELL_MULTI_SHOT                         = 2643,
    HUNTER_SPELL_BOMBARDMENT                        = 82921,
    HUNTER_SPELL_BASIC_ATTACK_COST_MODIFIER         = 62762,
    HUNTER_SPELL_IMPROVED_FOCUS_FIRE                = 157705,
    HUNTER_SPELL_SPIKED_COLLAR                      = 53184,
    HUNTER_SPELL_POISONED_AMMO                      = 162543,
    HUNTER_SPELL_POISONED_AMMO_AURA                 = 170661,
    HUNTER_SPELL_GLYPH_OF_MEND_PET                  = 19573,
    HUNTER_SPELL_GLYPH_OF_MEND_PET_TICK             = 24406,
    HUNTER_SPELL_FRENZY                             = 19623,
    HUNTER_SPELL_COMBAT_EXPERIENCE                  = 20782,
    HUNTER_SPELL_BLINK_STRIKES                      = 130392,
    HUNTER_SPELL_BLINK_STRIKES_TELEPORT             = 130393,
    HUNTER_SPELL_ITEM_WOD_PVP_MM_4P_BONUS           = 170884
};

/// Lesser Proportion - 57894
class spell_hun_lesser_proportion : public SpellScriptLoader
{
    public:
        spell_hun_lesser_proportion() : SpellScriptLoader("spell_hun_lesser_proportion") { }

        class spell_hun_lesser_proportion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_lesser_proportion_AuraScript);

            enum eSpells
            {
                GlyphOfLesserProportion = 57870
            };

            void OnUpdate(uint32, AuraEffectPtr p_AurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (Pet* l_Pet = GetUnitOwner()->ToPet())
                {
                    if (Unit* l_Owner = l_Pet->GetOwner())
                    {
                        if (!l_Owner->HasAura(eSpells::GlyphOfLesserProportion))
                            p_AurEff->GetBase()->Remove();
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_hun_lesser_proportion_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_SCALE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_lesser_proportion_AuraScript();
        }
};

/// Glyph of Lesser Proportion - 57870
class spell_hun_glyph_of_lesser_proportion : public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_lesser_proportion() : SpellScriptLoader("spell_hun_glyph_of_lesser_proportion") { }

        class spell_hun_glyph_of_lesser_proportion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_glyph_of_lesser_proportion_AuraScript);

            enum eSpells
            {
                LesserProportion = 57894
            };

            void OnApply(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                        l_Player->CastSpell(l_Pet, eSpells::LesserProportion, true);
                }
            }

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetTarget())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                        l_Pet->RemoveAura(eSpells::LesserProportion);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hun_glyph_of_lesser_proportion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_glyph_of_lesser_proportion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_glyph_of_lesser_proportion_AuraScript();
        }
};

/// Enhanced Basic Attacks - 157717
class spell_hun_enhanced_basic_attacks : public SpellScriptLoader
{
    public:
        spell_hun_enhanced_basic_attacks() : SpellScriptLoader("spell_hun_enhanced_basic_attacks") { }

        class spell_hun_enhanced_basic_attacks_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_enhanced_basic_attacks_SpellScript);

            enum eBasicAttacks
            {
                Claw  = 16827,
                Bite  = 17253,
                Smack = 49966
            };

            void HandleScript(SpellEffIndex)
            {
                if (Creature* l_Caster = GetCaster()->ToCreature())
                {
                    if (!l_Caster->isPet())
                        return;

                    if (Unit* l_Owner = l_Caster->GetOwner())
                    {
                        if (l_Owner->GetTypeId() != TypeID::TYPEID_PLAYER)
                            return;

                        uint32 l_SpellID = 0;
                        if (l_Caster->HasSpellCooldown(eBasicAttacks::Claw))
                            l_SpellID = eBasicAttacks::Claw;
                        if (l_Caster->HasSpellCooldown(eBasicAttacks::Bite))
                            l_SpellID = eBasicAttacks::Bite;
                        if (l_Caster->HasSpellCooldown(eBasicAttacks::Smack))
                            l_SpellID = eBasicAttacks::Smack;

                        l_Caster->m_CreatureSpellCooldowns.erase(l_SpellID);

                        WorldPacket l_Data(SMSG_SPELL_COOLDOWN, 16 + 2 + 1 + 4 + 4 + 4);
                        l_Data.appendPackGUID(l_Caster->GetGUID());
                        l_Data << uint8(CooldownFlags::CooldownFlagIncludeGCD);
                        l_Data << uint32(1);
                        l_Data << uint32(l_SpellID);
                        l_Data << uint32(0);
                        l_Owner->ToPlayer()->SendDirectMessage(&l_Data);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_enhanced_basic_attacks_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_enhanced_basic_attacks_SpellScript();
        }
};

/// Black Arrow - 3674
class spell_hun_black_arrow : public SpellScriptLoader
{
    public:
        spell_hun_black_arrow() : SpellScriptLoader("spell_hun_black_arrow") { }

        class spell_hun_black_arrow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_black_arrow_AuraScript);

            enum eSpells
            {
                LockAndLoad     = 168980,
                ExplosiveShot   = 53301
            };

            bool m_AlreadyProcLockAndLoad = false;

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (!roll_chance_i(GetSpellInfo()->Effects[EFFECT_1].BasePoints))
                        return;

                    if (m_AlreadyProcLockAndLoad)
                        return;

                    m_AlreadyProcLockAndLoad = true;

                    if (l_Player->HasSpellCooldown(eSpells::ExplosiveShot))
                        l_Player->RemoveSpellCooldown(eSpells::ExplosiveShot, true);

                    l_Player->CastSpell(l_Player, eSpells::LockAndLoad, true);
                }
            }

            void HandleDispel(DispelInfo*)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                        l_Player->RemoveSpellCooldown(GetSpellInfo()->Id);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_black_arrow_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnDispel += AuraDispelFn(spell_hun_black_arrow_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_black_arrow_AuraScript();
        }
};

/// Fetch (Glyph) - 125050
class spell_hun_fetch_glyph : public SpellScriptLoader
{
    public:
        spell_hun_fetch_glyph() : SpellScriptLoader("spell_hun_fetch_glyph") { }

        class spell_hun_fetch_glyph_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_fetch_glyph_SpellScript);

            void HandleScript(SpellEffIndex)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target->isAlive())
                            return;

                        if (Pet* l_Pet = l_Player->GetPet())
                        {
                            l_Pet->GetMotionMaster()->MoveCharge(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ());
                            l_Player->SendLoot(l_Target->GetGUID(), LootType::LOOT_CORPSE, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_fetch_glyph_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_fetch_glyph_SpellScript();
        }
};

/// Called by Aspect of the Cheetah - 5118
/// Glyph of Aspect of the Cheetah - 119462
class spell_hun_glyph_of_aspect_of_the_cheetah : public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_aspect_of_the_cheetah() : SpellScriptLoader("spell_hun_glyph_of_aspect_of_the_cheetah") { }

        class spell_hun_glyph_of_aspect_of_the_cheetah_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_glyph_of_aspect_of_the_cheetah_AuraScript);

            enum eSpells
            {
                GlyphOfAspectOfTheCheetah = 119462
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo() && p_EventInfo.GetDamageInfo()->GetSpellInfo() &&
                    p_EventInfo.GetDamageInfo()->GetSpellInfo()->IsPositive())
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::GlyphOfAspectOfTheCheetah))
                        p_AurEff->GetBase()->Remove();
                    else
                        l_Caster->CastSpell(l_Caster, p_AurEff->GetTriggerSpell(), true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_hun_glyph_of_aspect_of_the_cheetah_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_glyph_of_aspect_of_the_cheetah_AuraScript();
        }
};

/// Called by Deterrence (overrided) - 148467
/// Glyph of Mirrored Blades - 83495
class spell_hun_glyph_of_mirrored_blades : public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_mirrored_blades() : SpellScriptLoader("spell_hun_glyph_of_mirrored_blades") { }

        enum eSpells
        {
            GlyphOfMirroredBlades = 83495
        };

        class spell_hun_glyph_of_mirrored_blades_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_glyph_of_mirrored_blades_AuraScript);

            void CalculateAmount(constAuraEffectPtr, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::GlyphOfMirroredBlades))
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfMirroredBlades);
                        if (l_SpellInfo == nullptr)
                            return;

                        p_Amount = l_SpellInfo->Effects[EFFECT_0].BasePoints;
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_glyph_of_mirrored_blades_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_REFLECT_SPELLS);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_glyph_of_mirrored_blades_AuraScript();
        }
};

/// Thunderstomp - 63900
class spell_hun_thunderstomp : public SpellScriptLoader
{
    public:
        spell_hun_thunderstomp() : SpellScriptLoader("spell_hun_thunderstomp") { }

        class spell_hun_thunderstomp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_thunderstomp_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Owner = GetCaster()->GetOwner())
                    SetHitDamage((int32)(1.5f * (l_Owner->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.250f)));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_thunderstomp_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_thunderstomp_SpellScript();
        }
};

/// Burrow Attack  - 95714
class spell_hun_burrow_attack : public SpellScriptLoader
{
    public:
        spell_hun_burrow_attack() : SpellScriptLoader("spell_hun_burrow_attack") { }

        class spell_hun_burrow_attack_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_burrow_attack_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                SetHitDamage((int32)(GetHitDamage() + ((GetCaster()->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.40f) * 0.288) * 8));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_burrow_attack_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_burrow_attack_SpellScript();
        }
};

/// Steady Focus - 177667
class spell_hun_steady_focus: public SpellScriptLoader
{
    public:
        spell_hun_steady_focus() : SpellScriptLoader("spell_hun_steady_focus") { }

        class spell_hun_steady_focus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_steady_focus_AuraScript);

            enum SteadyFocusSpells
            {
                SteadyShot  = 56641,
                CobraShot   = 77767,
                SteadyFocus = 177668
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (p_EventInfo.GetActionTarget() == p_EventInfo.GetActor())
                    return;

                ProcFlagsExLegacy l_ExFlags = ProcFlagsExLegacy(p_EventInfo.GetHitMask());
                uint32 l_SpellID = p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    switch (l_Player->GetSpecializationId(l_Player->GetActiveSpec()))
                    {
                        ///< Marksmanship
                        ///< - Steady Shot twice in a row
                        case SpecIndex::SPEC_HUNTER_MARKSMANSHIP:
                        {
                            ///< Not Steady Shot
                            if (l_SpellID != SteadyFocusSpells::SteadyShot)
                            {
                                ///< Shitty procs
                                if (!(l_ExFlags & (ProcFlagsExLegacy::PROC_EX_INTERNAL_TRIGGERED | ProcFlagsExLegacy::PROC_EX_INTERNAL_CANT_PROC)))
                                    p_AurEff->GetBase()->SetCharges(0);

                                return;
                            }

                            DealWithCharges(p_AurEff, l_Player);
                            break;
                        }
                        case SpecIndex::SPEC_NONE:
                            return;
                        ///< Beast Mastery and Survival (Level 81)
                        ///< - Cobra Shot twice in a row
                        default:
                        {
                            ///< Not Cobra Shot
                            if (l_SpellID != SteadyFocusSpells::CobraShot)
                            {
                                ///< Shitty procs
                                if (!(l_ExFlags & (ProcFlagsExLegacy::PROC_EX_INTERNAL_TRIGGERED | ProcFlagsExLegacy::PROC_EX_INTERNAL_CANT_PROC)))
                                    p_AurEff->GetBase()->SetCharges(0);

                                return;
                            }

                            DealWithCharges(p_AurEff, l_Player);
                            break;
                        }
                    }
                }
            }

            void DealWithCharges(constAuraEffectPtr p_AurEff, Player* p_Player)
            {
                if (p_AurEff->GetBase()->GetCharges() <= 1)
                {
                    p_AurEff->GetBase()->SetCharges(2);
                    return;
                }

                p_AurEff->GetBase()->SetCharges(0);
                p_Player->CastSpell(p_Player, SteadyFocusSpells::SteadyFocus, true);

                if (Pet* l_Pet = p_Player->GetPet())
                    l_Pet->CastSpell(l_Pet, SteadyFocusSpells::SteadyFocus, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_hun_steady_focus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_steady_focus_AuraScript();
        }
};

/// Cornered - 53497
class spell_hun_cornered : public SpellScriptLoader
{
    public:
        spell_hun_cornered() : SpellScriptLoader("spell_hun_cornered") { }

        class spell_hun_cornered_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_cornered_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (l_Target->HasAuraState(AuraStateType::AURA_STATE_HEALTHLESS_35_PERCENT))
                    {
                        p_AurEff->ChangeAmount(GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                        if (AuraEffectPtr l_AurEff = p_AurEff->GetBase()->GetEffect(EFFECT_1))
                            l_AurEff->ChangeAmount(GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                    }
                    else
                    {
                        p_AurEff->ChangeAmount(0);

                        if (AuraEffectPtr l_AurEff = p_AurEff->GetBase()->GetEffect(EFFECT_1))
                            l_AurEff->ChangeAmount(0);
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_hun_cornered_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_cornered_AuraScript();
        }
};

enum LoneWolfes
{
    LoneWolfAura        = 164273,

    ///< Stats auras
    LoneWolfMastery     = 160198,
    LoneWolfStamina     = 160199,
    LoneWolfCritical    = 160200,
    LoneWolfHaste       = 160203,
    LoneWolfSpellPower  = 160205,
    LoneWolfPrimarStats = 160206,
    LoneWolfVersatility = 172967,
    LoneWolfMultistrike = 172968
};

static uint32 const g_BuffSpells[8] =
{
    LoneWolfes::LoneWolfMastery,
    LoneWolfes::LoneWolfStamina,
    LoneWolfes::LoneWolfCritical,
    LoneWolfes::LoneWolfHaste,
    LoneWolfes::LoneWolfSpellPower,
    LoneWolfes::LoneWolfPrimarStats,
    LoneWolfes::LoneWolfVersatility,
    LoneWolfes::LoneWolfMultistrike
};

/// Lone Wolf - 155228
class spell_hun_lone_wolf : public SpellScriptLoader
{
    public:
        spell_hun_lone_wolf() : SpellScriptLoader("spell_hun_lone_wolf") { }

        class spell_hun_lone_wolf_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_lone_wolf_AuraScript);

            void OnApply(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->learnSpell(LoneWolfes::LoneWolfMastery, false);
                    l_Player->learnSpell(LoneWolfes::LoneWolfStamina, false);
                    l_Player->learnSpell(LoneWolfes::LoneWolfCritical, false);
                    l_Player->learnSpell(LoneWolfes::LoneWolfHaste, false);
                    l_Player->learnSpell(LoneWolfes::LoneWolfSpellPower, false);
                    l_Player->learnSpell(LoneWolfes::LoneWolfPrimarStats, false);
                    l_Player->learnSpell(LoneWolfes::LoneWolfVersatility, false);
                    l_Player->learnSpell(LoneWolfes::LoneWolfMultistrike, false);
                }
            }

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->removeSpell(LoneWolfes::LoneWolfMastery);
                    l_Player->removeSpell(LoneWolfes::LoneWolfStamina);
                    l_Player->removeSpell(LoneWolfes::LoneWolfCritical);
                    l_Player->removeSpell(LoneWolfes::LoneWolfHaste);
                    l_Player->removeSpell(LoneWolfes::LoneWolfSpellPower);
                    l_Player->removeSpell(LoneWolfes::LoneWolfPrimarStats);
                    l_Player->removeSpell(LoneWolfes::LoneWolfVersatility);
                    l_Player->removeSpell(LoneWolfes::LoneWolfMultistrike);
                    l_Player->RemoveAura(LoneWolfes::LoneWolfAura);
                }
            }

            void OnUpdate(uint32, AuraEffectPtr p_AurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        l_Player->RemoveAura(LoneWolfes::LoneWolfAura);

                        p_AurEff->ChangeAmount(0, true, true);

                        if (AuraEffectPtr l_AuraEffect = p_AurEff->GetBase()->GetEffect(EFFECT_1))
                            l_AuraEffect->ChangeAmount(0, true, true);

                        for (uint8 l_I = 0; l_I < 8; ++l_I)
                            l_Player->RemoveAura(g_BuffSpells[l_I]);
                    }
                    else
                    {
                        /// We don't need to update values and cast this aura every time on update, just if we don't have it yet
                        if (!l_Player->HasAura(LoneWolfes::LoneWolfAura))
                        {
                            l_Player->CastSpell(l_Player, LoneWolfes::LoneWolfAura, true);

                            p_AurEff->ChangeAmount(GetSpellInfo()->Effects[EFFECT_0].BasePoints, true, true);

                            if (AuraEffectPtr l_AuraEffect = p_AurEff->GetBase()->GetEffect(EFFECT_1))
                                l_AuraEffect->ChangeAmount(GetSpellInfo()->Effects[EFFECT_0].BasePoints, true, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hun_lone_wolf_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_lone_wolf_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                OnEffectUpdate += AuraEffectUpdateFn(spell_hun_lone_wolf_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_lone_wolf_AuraScript();
        }
};

/// Kill Shot - 53351
/// Kill Shot (overrided) - 157708
class spell_hun_kill_shot : public SpellScriptLoader
{
    public:
        spell_hun_kill_shot() : SpellScriptLoader("spell_hun_kill_shot") { }

        class spell_hun_kill_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_kill_shot_SpellScript);

            enum KillShotSpells
            {
                CooldownMarker  = 90967,
                KillShotHealing = 164851
            };

            void HandleAfterHit()
            {
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target->isAlive())
                        {
                            if (l_Caster->HasAura(KillShotSpells::CooldownMarker))
                                return;

                            l_Caster->CastSpell(l_Caster, KillShotSpells::CooldownMarker, true);
                            l_Caster->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                        }
                        else
                            l_Caster->CastSpell(l_Caster, KillShotSpells::KillShotHealing, true);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_hun_kill_shot_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_kill_shot_SpellScript();
        }
};

/// Exotic Munitions - 162534
class spell_hun_exotic_munitions : public SpellScriptLoader
{
    public:
        spell_hun_exotic_munitions() : SpellScriptLoader("spell_hun_exotic_munitions") { }

        enum ExoticSpells
        {
            IncendiaryAmmo = 162536,
            PoisonedAmmo   = 162537,
            FrozenAmmo     = 162539
        };

        class spell_hun_exotic_munitions_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_exotic_munitions_AuraScript);

            void OnApply(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->learnSpell(ExoticSpells::IncendiaryAmmo, false);
                    l_Player->learnSpell(ExoticSpells::PoisonedAmmo, false);
                    l_Player->learnSpell(ExoticSpells::FrozenAmmo, false);
                }
            }

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->removeSpell(ExoticSpells::IncendiaryAmmo);
                    l_Player->removeSpell(ExoticSpells::PoisonedAmmo);
                    l_Player->removeSpell(ExoticSpells::FrozenAmmo);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hun_exotic_munitions_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_exotic_munitions_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_exotic_munitions_AuraScript();
        }
};

const uint32 fireworksSpells[4] = { 127937, 127936, 127961, 127951 };

// Fireworks - 127933
class spell_hun_fireworks: public SpellScriptLoader
{
    public:
        spell_hun_fireworks() : SpellScriptLoader("spell_hun_fireworks") { }

        class spell_hun_fireworks_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_fireworks_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, fireworksSpells[urand(0, 3)], true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_hun_fireworks_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_fireworks_SpellScript();
        }
};

// Glyph of Fireworks - 57903
class spell_hun_glyph_of_fireworks: public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_fireworks() : SpellScriptLoader("spell_hun_glyph_of_fireworks") { }

        class spell_hun_glyph_of_fireworks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_glyph_of_fireworks_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(HUNTER_SPELL_FIREWORKS, false);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(HUNTER_SPELL_FIREWORKS))
                        _player->removeSpell(HUNTER_SPELL_FIREWORKS, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hun_glyph_of_fireworks_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_glyph_of_fireworks_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_glyph_of_fireworks_AuraScript();
        }
};

// Called by Aspect of the Pack - 13159, Aspect of the Hawk - 13165 and Aspect of the Cheetah - 5118
// Glyph of Aspects - 122492
class spell_hun_glyph_of_aspects: public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_aspects() : SpellScriptLoader("spell_hun_glyph_of_aspects") { }

        class spell_hun_glyph_of_aspects_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_glyph_of_aspects_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->HasAura(HUNTER_SPELL_GLYPH_OF_ASPECTS))
                        return;

                    switch (GetSpellInfo()->Id)
                    {
                        case HUNTER_SPELL_ASPECT_OF_THE_CHEETAH:
                            caster->CastSpell(caster, HUNTER_SPELL_ASPECT_OF_THE_CHEETAH_SUMMON, true);
                            break;
                        case HUNTER_SPELL_ASPECT_OF_THE_HAWK:
                            caster->CastSpell(caster, HUNTER_SPELL_ASPECT_OF_THE_HAWK_SUMMON, true);
                            break;
                        case HUNTER_SPELL_ASPECT_OF_THE_PACK:
                            caster->CastSpell(caster, HUNTER_SPELL_ASPECT_OF_THE_PACK_SUMMON, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_glyph_of_aspects_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_glyph_of_aspects_SpellScript();
        }
};

/// Glyph of animal bond - 20895
class spell_hun_glyph_of_animal_bond : public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_animal_bond() : SpellScriptLoader("spell_hun_glyph_of_animal_bond") { }

        enum eSpells
        {
            GlyphOfAnimalBond = 24529
        };

        class spell_hun_glyph_of_animal_bond_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_glyph_of_animal_bond_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eSpells::GlyphOfAnimalBond, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_glyph_of_animal_bond_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_glyph_of_animal_bond_SpellScript();
        }

        class spell_hun_glyph_of_animal_bond_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_glyph_of_animal_bond_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr p_AurEff)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (!l_Player->HasAura(eSpells::GlyphOfAnimalBond))
                            l_Player->CastSpell(l_Player, eSpells::GlyphOfAnimalBond, true);
                    }
                    else
                        l_Player->RemoveAura(eSpells::GlyphOfAnimalBond);
                }
            }

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                        l_Pet->RemoveAura(eSpells::GlyphOfAnimalBond);

                    l_Player->RemoveAura(eSpells::GlyphOfAnimalBond);
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_hun_glyph_of_animal_bond_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_glyph_of_animal_bond_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_glyph_of_animal_bond_AuraScript();
        }
};

// Bestial Wrath - 19574 and The Beast Within - 34471
class spell_hun_bestial_wrath_dispel: public SpellScriptLoader
{
    public:
        spell_hun_bestial_wrath_dispel() : SpellScriptLoader("spell_hun_bestial_wrath_dispel") { }

        class spell_hun_bestial_wrath_dispel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_bestial_wrath_dispel_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    uint32 mechanic = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT)
                                        | (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                                        | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                                        | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                                        | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                                        | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN);

                    target->RemoveAurasWithMechanic(mechanic, AURA_REMOVE_BY_DEFAULT, GetSpellInfo()->Id);
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case 19574: // Bestial Wrath
                        OnEffectApply += AuraEffectApplyFn(spell_hun_bestial_wrath_dispel_AuraScript::OnApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case 34471: // The Beast Within
                        OnEffectApply += AuraEffectApplyFn(spell_hun_bestial_wrath_dispel_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_bestial_wrath_dispel_AuraScript();
        }
};

// Called by Arcane Shot - 3044
// Item PvP - Hunter S13 2P - 131564
class spell_hun_item_pvp_s13_2p: public SpellScriptLoader
{
    public:
        spell_hun_item_pvp_s13_2p() : SpellScriptLoader("spell_hun_item_pvp_s13_2p") { }

        class spell_hun_item_pvp_s13_2p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_item_pvp_s13_2p_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    if (l_Caster->HasAura(HUNTER_SPELL_ARCANE_INTENSITY_AURA))
                        l_Caster->CastSpell(l_Caster, HUNTER_SPELL_ARCANE_INTENSITY, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_item_pvp_s13_2p_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_item_pvp_s13_2p_SpellScript();
        }
};

/// Spirit Bond - 118694
class spell_hun_spirit_bond: public SpellScriptLoader
{
    public:
        spell_hun_spirit_bond() : SpellScriptLoader("spell_hun_spirit_bond") { }

        class spell_hun_spirit_bond_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_spirit_bond_AuraScript);

            void OnTick(constAuraEffectPtr)
            {
                if (!GetTarget())
                    return;

                if (Player* l_Player = GetTarget()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                        l_Pet->CastSpell(l_Pet, HUNTER_SPELL_SPIRIT_BOND_HEAL, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_spirit_bond_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_spirit_bond_AuraScript();
        }
};

/// Spirit Bond - 109212
class spell_hun_spirit_bond_apply: public SpellScriptLoader
{
    public:
        spell_hun_spirit_bond_apply() : SpellScriptLoader("spell_hun_spirit_bond_apply") { }

        class spell_hun_spirit_bond_apply_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_spirit_bond_apply_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, HUNTER_SPELL_SPIRIT_BOND, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_spirit_bond_apply_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_spirit_bond_apply_SpellScript();
        }

        class spell_hun_spirit_bond_apply_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_spirit_bond_apply_AuraScript);

            void OnUpdate(uint32)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->GetPet() == nullptr)
                        l_Player->RemoveAura(HUNTER_SPELL_SPIRIT_BOND);
                    else if (!l_Player->HasAura(HUNTER_SPELL_SPIRIT_BOND))
                        l_Player->CastSpell(l_Player, HUNTER_SPELL_SPIRIT_BOND, true);
                }
            }

            void Register()
            {
                OnAuraUpdate += AuraUpdateFn(spell_hun_spirit_bond_apply_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_spirit_bond_apply_AuraScript();
        }
};

// Glyph of Aspect of the Beast - 125042
class spell_hun_glyph_of_aspect_of_the_beast: public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_aspect_of_the_beast() : SpellScriptLoader("spell_hun_glyph_of_aspect_of_the_beast") { }

        class spell_hun_glyph_of_aspect_of_the_beast_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_glyph_of_aspect_of_the_beast_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(HUNTER_SPELL_ASPECT_OF_THE_BEAST, false);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(HUNTER_SPELL_ASPECT_OF_THE_BEAST))
                        _player->removeSpell(HUNTER_SPELL_ASPECT_OF_THE_BEAST, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hun_glyph_of_aspect_of_the_beast_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_glyph_of_aspect_of_the_beast_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_glyph_of_aspect_of_the_beast_AuraScript();
        }
};

// Glaive Toss (damage) - 120761 and 121414
class spell_hun_glaive_toss_damage: public SpellScriptLoader
{
    public:
        spell_hun_glaive_toss_damage() : SpellScriptLoader("spell_hun_glaive_toss_damage") { }

        class spell_hun_glaive_toss_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_glaive_toss_damage_SpellScript);

            uint64 mainTargetGUID;

            bool Load()
            {
                mainTargetGUID = 0;
                return true;
            }

            void CorrectDamageRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                std::list<Unit*> targetList;
                float radius = 50.0f;

                JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(GetCaster(), GetCaster(), radius);
                JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(GetCaster(), targetList, u_check);
                GetCaster()->VisitNearbyObject(radius, searcher);

                for (auto itr : targetList)
                {
                    if (itr->HasAura(HUNTER_SPELL_GLAIVE_TOSS_AURA))
                    {
                        mainTargetGUID = itr->GetGUID();
                        break;
                    }
                }

                if (!mainTargetGUID)
                    return;

                Unit* target = ObjectAccessor::FindUnit(mainTargetGUID);
                if (!target)
                    return;

                targets.push_back(target);

                for (auto itr : targetList)
                    if (itr->IsInBetween(GetCaster(), target, 5.0f))
                        targets.push_back(itr);
            }

            void CorrectSnareRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                std::list<Unit*> targetList;
                float radius = 50.0f;

                JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(GetCaster(), GetCaster(), radius);
                JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(GetCaster(), targetList, u_check);
                GetCaster()->VisitNearbyObject(radius, searcher);

                for (auto itr : targetList)
                {
                    if (itr->HasAura(HUNTER_SPELL_GLAIVE_TOSS_AURA))
                    {
                        mainTargetGUID = itr->GetGUID();
                        break;
                    }
                }

                if (!mainTargetGUID)
                    return;

                if (!mainTargetGUID)
                    return;

                Unit* target = ObjectAccessor::FindUnit(mainTargetGUID);
                if (!target)
                    return;

                targets.push_back(target);

                for (auto itr : targetList)
                    if (itr->IsInBetween(GetCaster(), target, 5.0f))
                        targets.push_back(itr);
            }

            void OnDamage()
            {
                if (!mainTargetGUID)
                    return;

                Unit* target = ObjectAccessor::FindUnit(mainTargetGUID);
                if (!target)
                    return;

                if (GetHitUnit())
                    if (GetHitUnit() == target)
                        SetHitDamage(GetHitDamage() * 4);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectDamageRange, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectSnareRange, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
                OnHit += SpellHitFn(spell_hun_glaive_toss_damage_SpellScript::OnDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_glaive_toss_damage_SpellScript();
        }
};

// Glaive Toss (Missile data) - 120755 and 120756
class spell_hun_glaive_toss_missile: public SpellScriptLoader
{
    public:
        spell_hun_glaive_toss_missile() : SpellScriptLoader("spell_hun_glaive_toss_missile") { }

        class spell_hun_glaive_toss_missile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_glaive_toss_missile_SpellScript);

            void HandleAfterCast()
            {
                if (GetSpellInfo()->Id == HUNTER_SPELL_GLAIVE_TOSS_RIGHT)
                {
                    if (Player* plr = GetCaster()->ToPlayer())
                        plr->CastSpell(plr, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                    else if (GetOriginalCaster())
                    {
                        if (Player* caster = GetOriginalCaster()->ToPlayer())
                            caster->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                    }
                }
                else
                {
                    if (Player* plr = GetCaster()->ToPlayer())
                        plr->CastSpell(plr, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                    else if (GetOriginalCaster())
                    {
                        if (Player* caster = GetOriginalCaster()->ToPlayer())
                            caster->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                    }
                }

                if (Unit* target = GetExplTargetUnit())
                    if (GetCaster() == GetOriginalCaster())
                        GetCaster()->AddAura(HUNTER_SPELL_GLAIVE_TOSS_AURA, target);
            }

            void HandleOnHit()
            {
                if (GetSpellInfo()->Id == HUNTER_SPELL_GLAIVE_TOSS_RIGHT)
                {
                    if (Unit* caster = GetCaster())
                        if (Unit* target = GetHitUnit())
                            if (caster == GetOriginalCaster())
                                target->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_LEFT, true, NULL, NULLAURA_EFFECT, caster->GetGUID());
                }
                else
                {
                    if (Unit* caster = GetCaster())
                        if (Unit* target = GetHitUnit())
                            if (caster == GetOriginalCaster())
                                target->CastSpell(caster, HUNTER_SPELL_GLAIVE_TOSS_RIGHT, true, NULL, NULLAURA_EFFECT, caster->GetGUID());
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_hun_glaive_toss_missile_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_hun_glaive_toss_missile_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_glaive_toss_missile_SpellScript();
        }
};

// Glyph of Fetch - 126746
class spell_hun_glyph_of_fetch: public SpellScriptLoader
{
    public:
        spell_hun_glyph_of_fetch() : SpellScriptLoader("spell_hun_glyph_of_fetch") { }

        class spell_hun_glyph_of_fetch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_glyph_of_fetch_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(HUNTER_SPELL_FETCH, false);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(HUNTER_SPELL_FETCH))
                        _player->removeSpell(HUNTER_SPELL_FETCH, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hun_glyph_of_fetch_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_glyph_of_fetch_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_glyph_of_fetch_AuraScript();
        }
};

// Dire Beast - 120679
class spell_hun_dire_beast: public SpellScriptLoader
{
    public:
        spell_hun_dire_beast() : SpellScriptLoader("spell_hun_dire_beast") { }

        class spell_hun_dire_beast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_dire_beast_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Summon's skin is different function of Map or Zone ID
                        switch (_player->GetZoneId())
                        {
                            case 5785: // The Jade Forest
                                _player->CastSpell(target, DIRE_BEAST_JADE_FOREST, true);
                                break;
                            case 5805: // Valley of the Four Winds
                                _player->CastSpell(target, DIRE_BEAST_VALLEY_OF_THE_FOUR_WINDS, true);
                                break;
                            case 5840: // Vale of Eternal Blossoms
                                _player->CastSpell(target, DIRE_BEAST_VALE_OF_THE_ETERNAL_BLOSSOM, true);
                                break;
                            case 5841: // Kun-Lai Summit
                                _player->CastSpell(target, DIRE_BEAST_KUN_LAI_SUMMIT, true);
                                break;
                            case 5842: // Townlong Steppes
                                _player->CastSpell(target, DIRE_BEAST_TOWNLONG_STEPPES, true);
                                break;
                            case 6134: // Krasarang Wilds
                                _player->CastSpell(target, DIRE_BEAST_KRASARANG_WILDS, true);
                                break;
                            case 6138: // Dread Wastes
                                _player->CastSpell(target, DIRE_BEAST_DREAD_WASTES, true);
                                break;
                            default:
                            {
                                switch (_player->GetMapId())
                                {
                                    case 0: // Eastern Kingdoms
                                        _player->CastSpell(target, DIRE_BEAST_EASTERN_KINGDOMS, true);
                                        break;
                                    case 1: // Kalimdor
                                        _player->CastSpell(target, DIRE_BEAST_KALIMDOR, true);
                                        break;
                                    case 8: // Outland
                                        _player->CastSpell(target, DIRE_BEAST_OUTLAND, true);
                                        break;
                                    case 10: // Northrend
                                        _player->CastSpell(target, DIRE_BEAST_NORTHREND, true);
                                        break;
                                    default:
                                        if (_player->GetMap()->IsDungeon() || _player->GetMap()->IsBattlegroundOrArena())
                                            _player->CastSpell(target, DIRE_BEAST_DUNGEONS, true);
                                        break;
                                }
                                break;
                            }
                        }
                    }
                }
            }

            void Register()
            {
               OnHit += SpellHitFn(spell_hun_dire_beast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_dire_beast_SpellScript();
        }
};

// A Murder of Crows - 131894
class spell_hun_a_murder_of_crows: public SpellScriptLoader
{
    public:
        spell_hun_a_murder_of_crows() : SpellScriptLoader("spell_hun_a_murder_of_crows") { }

        class spell_hun_a_murder_of_crows_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_a_murder_of_crows_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                    if (Unit* l_Caster = GetCaster())
                        l_Caster->CastSpell(l_Target, HUNTER_SPELL_A_MURDER_OF_CROWS_DAMAGE, true);
            }

            void HandleRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                
                if (!l_Caster)
                    return;

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                    if (Player* l_Player = l_Caster->ToPlayer())
                    if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                        l_Player->RemoveSpellCooldown(GetSpellInfo()->Id, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_hun_a_murder_of_crows_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_a_murder_of_crows_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_a_murder_of_crows_AuraScript();
        }
};

/// Focus Fire - 82692
class spell_hun_focus_fire : public SpellScriptLoader
{
    public:
        spell_hun_focus_fire() : SpellScriptLoader("spell_hun_focus_fire") { }

        enum eFrenzy
        {
            FrenzyReady = 88843
        };

        class spell_hun_focus_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_focus_fire_AuraScript);

            void CalculateAmount(constAuraEffectPtr, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(HUNTER_SPELL_IMPROVED_FOCUS_FIRE))
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(HUNTER_SPELL_IMPROVED_FOCUS_FIRE);
                        if (l_SpellInfo == nullptr)
                            return;

                        if (AuraPtr l_Frenzy = l_Caster->GetAura(HUNTER_SPELL_FRENZY_STACKS))
                            p_Amount = l_Frenzy->GetStackAmount() * l_SpellInfo->Effects[EFFECT_0].BasePoints;
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_focus_fire_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_focus_fire_AuraScript();
        }

        class spell_hun_focus_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_focus_fire_SpellScript);

            SpellCastResult CheckFrenzy()
            {
                if (!GetCaster()->HasAura(HUNTER_SPELL_FRENZY_STACKS))
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (AuraEffectPtr l_FocusFire = l_Player->GetAuraEffect(HUNTER_SPELL_FOCUS_FIRE_AURA, EFFECT_0))
                    {
                        if (AuraPtr l_Frenzy = l_Player->GetAura(HUNTER_SPELL_FRENZY_STACKS))
                        {
                            if (Pet* l_Pet = l_Player->GetPet())
                            {
                                l_FocusFire->ChangeAmount(l_FocusFire->GetAmount() * l_Frenzy->GetStackAmount());

                                if (AuraPtr l_FrenzyPet = l_Pet->GetAura(HUNTER_SPELL_FRENZY_STACKS))
                                {
                                    l_Pet->EnergizeBySpell(l_Pet, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_1].BasePoints * l_FrenzyPet->GetStackAmount(), POWER_FOCUS);
                                    l_Pet->RemoveAura(HUNTER_SPELL_FRENZY_STACKS);
                                }

                                l_Player->RemoveAura(HUNTER_SPELL_FRENZY_STACKS);
                                l_Player->RemoveAura(eFrenzy::FrenzyReady);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_focus_fire_SpellScript::CheckFrenzy);
                OnHit += SpellHitFn(spell_hun_focus_fire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_focus_fire_SpellScript();
        }
};

/// Frenzy - 19615
class spell_hun_frenzy : public SpellScriptLoader
{
    public:
        spell_hun_frenzy() : SpellScriptLoader("spell_hun_frenzy") { }

        enum eFrenzy
        {
            FrenzyReady = 88843
        };

        class spell_hun_frenzy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_frenzy_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Owner = l_Caster->GetOwner())
                    {
                        if (AuraPtr l_Frenzy = l_Caster->GetAura(HUNTER_SPELL_FRENZY_STACKS))
                        {
                            if (l_Frenzy->GetStackAmount() >= 5)
                                l_Owner->CastSpell(l_Owner, eFrenzy::FrenzyReady, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_frenzy_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_frenzy_SpellScript();
        }

        class spell_hun_frenzy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_frenzy_AuraScript);

            void HandleRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAura(eFrenzy::FrenzyReady);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_hun_frenzy_AuraScript::HandleRemove, EFFECT_2, SPELL_AURA_408, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_frenzy_AuraScript();
        }
};

// Lynx Rush - 120697
class spell_hun_lynx_rush: public SpellScriptLoader
{
    public:
        spell_hun_lynx_rush() : SpellScriptLoader("spell_hun_lynx_rush") { }

        class spell_hun_lynx_rush_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_lynx_rush_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                std::list<Unit*> tempList;
                std::list<Unit*> targetList;
                Unit* unitTarget = NULL;

                GetTarget()->GetAttackableUnitListInRange(tempList, 10.0f);

                for (auto itr : tempList)
                {
                    if (itr->GetGUID() == GetTarget()->GetGUID())
                        continue;

                    if (GetTarget()->GetOwner() && GetTarget()->GetOwner()->GetGUID() == itr->GetGUID())
                        continue;

                    if (!GetTarget()->IsValidAttackTarget(itr))
                        continue;

                    targetList.push_back(itr);
                }

                tempList.clear();

                if (targetList.empty())
                    return;

                JadeCore::Containers::RandomResizeList(targetList, 1);

                for (auto itr : targetList)
                {
                    unitTarget = itr;
                    break;
                }

                if (!unitTarget)
                    return;

                float angle = unitTarget->GetRelativeAngle(GetTarget());
                Position pos;

                unitTarget->GetContactPoint(GetTarget(), pos.m_positionX, pos.m_positionY, pos.m_positionZ);
                unitTarget->GetFirstCollisionPosition(pos, unitTarget->GetObjectSize(), angle);
                GetTarget()->GetMotionMaster()->MoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ + unitTarget->GetObjectSize());

                GetTarget()->CastSpell(unitTarget, HUNTER_SPELL_LYNX_CRUSH_DAMAGE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_lynx_rush_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_lynx_rush_AuraScript();
        }

        class spell_hun_lynx_rush_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_lynx_rush_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (GetHitUnit())
                    {
                        if (Pet* pet = _player->GetPet())
                        {
                            if (pet->GetGUID() == GetHitUnit()->GetGUID())
                            {
                                std::list<Unit*> tempList;
                                std::list<Unit*> targetList;
                                Unit* unitTarget = NULL;

                                pet->GetAttackableUnitListInRange(tempList, 10.0f);

                                for (auto itr : tempList)
                                {
                                    if (itr->GetGUID() == pet->GetGUID())
                                        continue;

                                    if (_player->GetGUID() == itr->GetGUID())
                                        continue;

                                    if (!pet->IsValidAttackTarget(itr))
                                        continue;

                                    targetList.push_back(itr);
                                }

                                tempList.clear();

                                if (targetList.empty())
                                    return;

                                JadeCore::Containers::RandomResizeList(targetList, 1);

                                for (auto itr : targetList)
                                {
                                    unitTarget = itr;
                                    break;
                                }

                                if (!unitTarget)
                                    return;

                                float angle = unitTarget->GetRelativeAngle(pet);
                                Position pos;

                                unitTarget->GetContactPoint(pet, pos.m_positionX, pos.m_positionY, pos.m_positionZ);
                                unitTarget->GetFirstCollisionPosition(pos, unitTarget->GetObjectSize(), angle);
                                pet->GetMotionMaster()->MoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ + unitTarget->GetObjectSize());

                                pet->CastSpell(unitTarget, HUNTER_SPELL_LYNX_CRUSH_DAMAGE, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_lynx_rush_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_lynx_rush_SpellScript();
        }
};

/// Beast Cleave - 118455
class spell_hun_beast_cleave_proc: public SpellScriptLoader
{
    public:
        spell_hun_beast_cleave_proc() : SpellScriptLoader("spell_hun_beast_cleave_proc") { }

        class spell_hun_beast_cleave_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_beast_cleave_proc_AuraScript);

            enum eSpells
            {
                BeastCleaveAura     = 115939,
                BeastCleaveDamage   = 118459
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                if (p_EventInfo.GetActor()->GetGUID() != l_Target->GetGUID())
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == eSpells::BeastCleaveDamage)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Target->HasAura(p_AurEff->GetSpellInfo()->Id, l_Player->GetGUID()))
                    {
                        if (AuraEffectPtr l_AurEff = l_Player->GetAuraEffect(eSpells::BeastCleaveAura, EFFECT_0))
                        {
                            int32 l_BP = CalculatePct(p_EventInfo.GetDamageInfo()->GetDamage(), l_AurEff->GetAmount());
                            l_Target->CastCustomSpell(l_Target, eSpells::BeastCleaveDamage, &l_BP, nullptr, nullptr, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_hun_beast_cleave_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_beast_cleave_proc_AuraScript();
        }
};

// Called by Multi Shot - 2643
// Beast Cleave - 115939
class spell_hun_beast_cleave: public SpellScriptLoader
{
    public:
        spell_hun_beast_cleave() : SpellScriptLoader("spell_hun_beast_cleave") { }

        class spell_hun_beast_cleave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_beast_cleave_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(HUNTER_SPELL_BEAST_CLEAVE_AURA))
                        if (Pet* pet = _player->GetPet())
                            _player->CastSpell(pet, HUNTER_SPELL_BEAST_CLEAVE_PROC, true);
            }

            void Register()
            {
               AfterCast += SpellCastFn(spell_hun_beast_cleave_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_beast_cleave_SpellScript();
        }
};

// Called by Arcane Shot - 3044
// Cobra Strikes - 53260
class spell_hun_cobra_strikes: public SpellScriptLoader
{
    public:
        spell_hun_cobra_strikes() : SpellScriptLoader("spell_hun_cobra_strikes") { }

        class spell_hun_cobra_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_cobra_strikes_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(HUNTER_SPELL_COBRA_STRIKES_AURA))
                {
                    const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(HUNTER_SPELL_COBRA_STRIKES_AURA);

                    if (l_SpellInfo == nullptr)
                        return;

                    if (roll_chance_i(l_SpellInfo->ProcChance))
                        l_Caster->CastSpell(l_Caster, HUNTER_SPELL_COBRA_STRIKES_STACKS, true);
                }
            }

            void Register()
            {
               OnHit += SpellHitFn(spell_hun_cobra_strikes_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_cobra_strikes_SpellScript();
        }
};

/// Barrage damage - 120361
class spell_hun_barrage : public SpellScriptLoader
{
    public:
        spell_hun_barrage() : SpellScriptLoader("spell_hun_barrage") { }

        class spell_hun_barrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_barrage_SpellScript);

            enum eSpells
            {
                BarrageTalent = 120360
            };

            void CheckLOS(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || !p_Object->IsWithinLOSInMap(l_Caster))
                        return true;

                    return false;
                });
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (!l_Target->HasAura(eSpells::BarrageTalent))
                            SetHitDamage(GetHitDamage() / 2);
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_barrage_SpellScript::CheckLOS, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_barrage_SpellScript::CheckLOS, EFFECT_2, TARGET_UNIT_CONE_ENEMY_104);
                OnHit += SpellHitFn(spell_hun_barrage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_barrage_SpellScript();
        }
};

/// Binding Shot - 117405
class spell_hun_binding_shot : public SpellScriptLoader
{
    public:
        spell_hun_binding_shot() : SpellScriptLoader("spell_hun_binding_shot") { }

        class spell_hun_binding_shot_zone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_binding_shot_zone_AuraScript);

            enum eSpells
            {
                BindingShotArrow    = 109248,
                BindingShotImmune   = 117553,
                BindingShotStun     = 117526,
                BindingShotLink     = 117405
            };

            void OnUpdate(uint32, AuraEffectPtr)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetUnitOwner())
                    {
                        AreaTrigger* l_BindingShot = l_Caster->GetAreaTrigger(eSpells::BindingShotArrow);
                        if (l_BindingShot == nullptr)
                            return;

                        /// If AreaTrigger exists, spellInfo exists too
                        float l_Radius = sSpellMgr->GetSpellInfo(eSpells::BindingShotArrow)->Effects[EFFECT_1].CalcRadius(l_Caster);

                        if (l_Target->GetDistance(l_BindingShot) > l_Radius)
                        {
                            l_Target->CastSpell(l_Target, eSpells::BindingShotStun, true);
                            l_Target->CastSpell(l_Target, eSpells::BindingShotImmune, true);
                            l_Target->RemoveAura(eSpells::BindingShotLink);

                            /// 3s duration on PvP targets
                            if (l_Target->GetTypeId() == TypeID::TYPEID_PLAYER)
                            {
                                if (AuraPtr l_Stun = l_Target->GetAura(eSpells::BindingShotStun))
                                {
                                    l_Stun->SetDuration(3000);
                                    l_Stun->SetMaxDuration(3000);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_hun_binding_shot_zone_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_binding_shot_zone_AuraScript();
        }
};

/// Binding Shot - 109248
class spell_hun_binding_shot_zone : public SpellScriptLoader
{
    public:
        spell_hun_binding_shot_zone() : SpellScriptLoader("spell_hun_binding_shot_zone") { }

        class spell_hun_binding_shot_zone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_binding_shot_zone_SpellScript);

            enum eSpells
            {
                BindingShotArrow = 118306
            };

            void HandleHitDest(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (WorldLocation const* l_Dest = GetExplTargetDest())
                        l_Caster->CastSpell(l_Dest->m_positionX, l_Dest->m_positionY, l_Dest->m_positionZ, eSpells::BindingShotArrow, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_hun_binding_shot_zone_SpellScript::HandleHitDest, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_binding_shot_zone_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Powershot - 109259
class spell_hun_powershot: public SpellScriptLoader
{
    public:
        spell_hun_powershot() : SpellScriptLoader("spell_hun_powershot") { }

        class spell_hun_powershot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_powershot_SpellScript);

            enum eSpells
            {
                PowerShotMainTarget = 177571,
                PowerShotSecondaryTarget = 181741
            };

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::PowerShotMainTarget, true);

                std::list<Unit*> l_TempUnitMap;
                l_Caster->GetAttackableUnitListInRange(l_TempUnitMap, l_Caster->GetDistance(l_Target));

                for (auto itr : l_TempUnitMap)
                {
                    if (!itr->IsValidAttackTarget(l_Caster))
                        continue;

                    if (itr->GetGUID() == l_Caster->GetGUID() || itr->GetGUID() == l_Target->GetGUID())
                        continue;

                    if (!itr->IsInBetween(l_Caster, l_Target, 1.0f))
                        continue;

                    l_Caster->CastSpell(itr, eSpells::PowerShotSecondaryTarget, true);

                    if (Creature* creatureTarget = itr->ToCreature())
                    if (creatureTarget->isWorldBoss() || creatureTarget->IsDungeonBoss())
                        continue;

                    if (itr->GetTypeId() == TYPEID_PLAYER)
                    if (itr->ToPlayer()->GetKnockBackTime())
                        continue;

                    // Instantly interrupt non melee spells being casted
                    if (itr->IsNonMeleeSpellCasted(true))
                        itr->InterruptNonMeleeSpells(true);

                    float l_Ratio = 0.1f;
                    float l_Speedxy = float(GetSpellInfo()->Effects[EFFECT_1].MiscValue) * l_Ratio;
                    float l_Speedz = float(GetSpellInfo()->Effects[EFFECT_1].BasePoints) * l_Ratio;
                    if (l_Speedxy < 0.1f && l_Speedz < 0.1f)
                        return;

                    float l_X, l_Y;
                    l_Caster->GetPosition(l_X, l_Y);

                    itr->KnockbackFrom(l_X, l_Y, l_Speedxy, l_Speedz);

                    if (itr->GetTypeId() == TYPEID_PLAYER)
                        itr->ToPlayer()->SetKnockBackTime(getMSTime());
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_powershot_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_powershot_SpellScript();
        }
};

// Feign Death - 5384
class spell_hun_feign_death: public SpellScriptLoader
{
    public:
        spell_hun_feign_death() : SpellScriptLoader("spell_hun_feign_death") { }

        class spell_hun_feign_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_feign_death_AuraScript);

            int32 health;
            int32 focus;

            void HandleEffectApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                health = GetTarget()->GetHealth();
                focus = GetTarget()->GetPower(POWER_FOCUS);
            }

            void HandleEffectRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (health && focus)
                {
                    GetTarget()->SetHealth(health);
                    GetTarget()->SetPower(POWER_FOCUS, focus);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_hun_feign_death_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_feign_death_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_feign_death_AuraScript();
        }
};

// Camouflage - 51755
class spell_hun_camouflage_visual: public SpellScriptLoader
{
    public:
        spell_hun_camouflage_visual() : SpellScriptLoader("spell_hun_camouflage_visual") { }

        class spell_hun_camouflage_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_camouflage_visual_AuraScript);

            void HandleEffectRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                {
                    GetCaster()->RemoveAura(HUNTER_SPELL_CAMOUFLAGE_VISUAL);
                    GetCaster()->RemoveAura(HUNTER_SPELL_GLYPH_OF_CAMOUFLAGE_VISUAL);
                    GetCaster()->RemoveAura(HUNTER_SPELL_CAMOUFLAGE_PERIODIC_HEAL);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_visual_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_INTERFERE_TARGETTING, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_camouflage_visual_AuraScript();
        }
};

// Called by Multi Shot - 2643
// Serpent Spread - 87935
class spell_hun_serpent_spread: public SpellScriptLoader
{
    public:
        spell_hun_serpent_spread() : SpellScriptLoader("spell_hun_serpent_spread") { }

        class spell_hun_serpent_spread_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_serpent_spread_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(HUNTER_SPELL_SERPENT_SPREAD))
                            _player->CastSpell(target, HUNTER_SPELL_SERPENT_STING, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_serpent_spread_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_serpent_spread_SpellScript();
        }
};

/// Ancient Hysteria - 90355 - last update: 6.1.2 19802
class spell_hun_ancient_hysteria: public SpellScriptLoader
{
    public:
        spell_hun_ancient_hysteria() : SpellScriptLoader("spell_hun_ancient_hysteria") { }

        class spell_hun_ancient_hysteria_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_ancient_hysteria_SpellScript);

            bool Validate(SpellInfo const* /*p_SpellEntry*/) override
            {
                if (!sSpellMgr->GetSpellInfo(HunterSpells::HUNTER_SPELL_INSANITY))
                    return false;

                return true;
            }

            SpellCastResult CheckMap()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(HunterSpells::SPELL_SHAMAN_EXHAUSTED))
                    return SpellCastResult::SPELL_FAILED_DONT_REPORT;

                return SpellCastResult::SPELL_CAST_OK;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::HUNTER_SPELL_INSANITY));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::SPELL_SHAMAN_EXHAUSTED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::SPELL_SHAMAN_SATED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::SPELL_MAGE_TEMPORAL_DISPLACEMENT));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::HUNTER_SPELL_FATIGUED));
            }

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                    l_Target->CastSpell(l_Target, HunterSpells::HUNTER_SPELL_INSANITY, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_ancient_hysteria_SpellScript::CheckMap);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_ancient_hysteria_SpellScript::RemoveInvalidTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_ancient_hysteria_SpellScript::RemoveInvalidTargets, SpellEffIndex::EFFECT_1, Targets::TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_hun_ancient_hysteria_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_ancient_hysteria_SpellScript();
        }
};

/// Netherwinds - 160452 - last update: 6.1.2 19802
class spell_hun_netherwinds : public SpellScriptLoader
{
    public:
        spell_hun_netherwinds() : SpellScriptLoader("spell_hun_netherwinds") { }

        class spell_hun_netherwinds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_netherwinds_SpellScript);

            bool Validate(SpellInfo const* /*p_SpellEntry*/) override
            {
                if (!sSpellMgr->GetSpellInfo(HunterSpells::HUNTER_SPELL_FATIGUED))
                    return false;

                return true;
            }

            SpellCastResult CheckMap()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(HunterSpells::SPELL_SHAMAN_EXHAUSTED))
                    return SpellCastResult::SPELL_FAILED_DONT_REPORT;

                return SpellCastResult::SPELL_CAST_OK;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::HUNTER_SPELL_INSANITY));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::SPELL_SHAMAN_EXHAUSTED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::SPELL_SHAMAN_SATED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::SPELL_MAGE_TEMPORAL_DISPLACEMENT));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HunterSpells::HUNTER_SPELL_FATIGUED));
            }

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                    l_Target->CastSpell(l_Target, HunterSpells::HUNTER_SPELL_FATIGUED, true);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_netherwinds_SpellScript::CheckMap);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_netherwinds_SpellScript::RemoveInvalidTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_netherwinds_SpellScript::RemoveInvalidTargets, SpellEffIndex::EFFECT_1, Targets::TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_hun_netherwinds_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_netherwinds_SpellScript();
        }
};

// Kill Command - 34026
class spell_hun_kill_command: public SpellScriptLoader
{
    public:
        spell_hun_kill_command() : SpellScriptLoader("spell_hun_kill_command") { }

        class spell_hun_kill_command_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_kill_command_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(HUNTER_SPELL_KILL_COMMAND))
                    return false;
                return true;
            }

            SpellCastResult CheckCastMeet()
            {
                Unit* pet = GetCaster()->GetGuardianPet();
                Unit* petTarget = GetExplTargetUnit();

                if (!pet || pet->isDead())
                    return SPELL_FAILED_NO_PET;

                // pet has a target and target is within 5 yards and target is in line of sight
                if (!petTarget || !pet->IsWithinDist(petTarget, 25.0f, true) || !petTarget->IsWithinLOSInMap(pet))
                    return SPELL_FAILED_DONT_REPORT;

                if (pet->HasAuraType(SPELL_AURA_MOD_STUN) || pet->HasAuraType(SPELL_AURA_MOD_CONFUSE) || pet->HasAuraType(SPELL_AURA_MOD_SILENCE) ||
                    pet->HasAuraType(SPELL_AURA_MOD_FEAR) || pet->HasAuraType(SPELL_AURA_MOD_FEAR_2))
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* pet = GetCaster()->GetGuardianPet())
                {
                    if (!pet)
                        return;

                    if (!GetExplTargetUnit())
                        return;

                    pet->CastSpell(GetExplTargetUnit(), HUNTER_SPELL_KILL_COMMAND_TRIGGER, true);

                    if (pet->getVictim())
                    {
                        pet->AttackStop();
                        pet->ToCreature()->AI()->AttackStart(GetExplTargetUnit());
                    }
                    else
                        pet->ToCreature()->AI()->AttackStart(GetExplTargetUnit());

                    pet->CastSpell(GetExplTargetUnit(), HUNTER_SPELL_KILL_COMMAND_CHARGE, true);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_kill_command_SpellScript::CheckCastMeet);
                OnEffectHit += SpellEffectFn(spell_hun_kill_command_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_kill_command_SpellScript();
        }
};

/// Kill Command - 83381
class spell_hun_kill_command_proc : public SpellScriptLoader
{
    public:
        spell_hun_kill_command_proc() : SpellScriptLoader("spell_hun_kill_command_proc") { }

        class spell_hun_kill_command_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_kill_command_proc_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Owner = GetCaster()->GetOwner())
                {
                    int32 l_Damage = int32(l_Owner->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 1.632f);
                    l_Damage = GetCaster()->SpellDamageBonusDone(GetHitUnit(), GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                    l_Damage = GetHitUnit()->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                    SetHitDamage(l_Damage);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_kill_command_proc_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_kill_command_proc_SpellScript();
        }
};

// Cobra Shot - 77767
class spell_hun_cobra_shot: public SpellScriptLoader
{
    public:
        spell_hun_cobra_shot() : SpellScriptLoader("spell_hun_cobra_shot") { }

        class spell_hun_cobra_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_cobra_shot_SpellScript);

            void HandleOnCast()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, HUNTER_SPELL_COBRA_SHOT_ENERGIZE, true);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_hun_cobra_shot_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_cobra_shot_SpellScript();
        }
};

// Steady Shot - 56641
class spell_hun_steady_shot: public SpellScriptLoader
{
    public:
        spell_hun_steady_shot() : SpellScriptLoader("spell_hun_steady_shot") { }

        class spell_hun_steady_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_steady_shot_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, HUNTER_SPELL_STEADY_SHOT_ENERGIZE, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_hun_steady_shot_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_steady_shot_SpellScript();
        }
};

// Chimaera Shot - 53209
class spell_hun_chimaera_shot: public SpellScriptLoader
{
    public:
        spell_hun_chimaera_shot() : SpellScriptLoader("spell_hun_chimaera_shot") { }

        class spell_hun_chimaera_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_chimaera_shot_SpellScript);

            enum ChimaeraSpells
            {
                SpellChimaeraFrost  = 171454,
                SpellChimaeraNature = 171457,
                GlyphOfChimaeraShot = 119447,
                ChimaeraShotHealing = 53353
            };

            bool m_Healed;

            bool Load()
            {
                m_Healed = false;
                return true;
            }

            void HandleOnHit(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        ///< 50% chance to deals frost damage or nature damage
                        if (urand(0, 1))
                            l_Caster->CastSpell(l_Target, ChimaeraSpells::SpellChimaeraFrost, true);
                        else
                            l_Caster->CastSpell(l_Target, ChimaeraSpells::SpellChimaeraNature, true);

                        SpellInfo const* l_GlyphOfChimaera = sSpellMgr->GetSpellInfo(ChimaeraSpells::GlyphOfChimaeraShot);
                        if (l_Caster->HasAura(ChimaeraSpells::GlyphOfChimaeraShot) && l_GlyphOfChimaera != nullptr && !m_Healed)
                        {
                            m_Healed = true;

                            int32 l_Value = l_GlyphOfChimaera->Effects[EFFECT_0].BasePoints;
                            l_Caster->CastCustomSpell(l_Caster, ChimaeraSpells::ChimaeraShotHealing, &l_Value, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_chimaera_shot_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_chimaera_shot_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Chimaera Shot (damage), Frost - 171454, Nature - 171457
class spell_hun_chimaera_shot_damage : public SpellScriptLoader
{
    public:
        spell_hun_chimaera_shot_damage() : SpellScriptLoader("spell_hun_chimaera_shot_damage") { }

        class spell_hun_chimaera_shot_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_chimaera_shot_damage_SpellScript);

            void HandleDamage(SpellEffIndex)
            {
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                /// HotFixe February 27, 2015 : Chimaera Shot now deals 20% less damage in PvP combat.
                if (l_Target->GetTypeId() == TYPEID_PLAYER)
                    SetHitDamage(GetHitDamage() - CalculatePct(GetHitDamage(), 20));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_chimaera_shot_damage_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_chimaera_shot_damage_SpellScript();
        }
};

class spell_hun_last_stand_pet: public SpellScriptLoader
{
    public:
        spell_hun_last_stand_pet() : SpellScriptLoader("spell_hun_last_stand_pet") { }

        class spell_hun_last_stand_pet_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_last_stand_pet_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(HUNTER_PET_SPELL_LAST_STAND_TRIGGERED))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(30));
                caster->CastCustomSpell(caster, HUNTER_PET_SPELL_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
            }

            void Register()
            {
                // add dummy effect spell handler to pet's Last Stand
                OnEffectHitTarget += SpellEffectFn(spell_hun_last_stand_pet_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_last_stand_pet_SpellScript();
        }
};

// Master's Call - 53271
class spell_hun_masters_call: public SpellScriptLoader
{
    public:
        spell_hun_masters_call() : SpellScriptLoader("spell_hun_masters_call") { }

        class spell_hun_masters_call_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_masters_call_SpellScript);

            SpellCastResult CheckIfPetInLOS()
            {
                Unit* l_Target = GetExplTargetUnit();

                if (l_Target == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (l_Pet->isDead())
                            return SPELL_FAILED_NO_PET;

                        float x, y, z;
                        l_Pet->GetPosition(x, y, z);
                        
                        if (!l_Pet->IsWithinDist(l_Target, 40.0f, true))
                            return SPELL_FAILED_OUT_OF_RANGE;

                        if (l_Pet->HasAuraType(SPELL_AURA_MOD_STUN) || l_Pet->HasAuraType(SPELL_AURA_MOD_CONFUSE) || l_Pet->HasAuraType(SPELL_AURA_MOD_SILENCE) ||
                            l_Pet->HasAuraType(SPELL_AURA_MOD_FEAR) || l_Pet->HasAuraType(SPELL_AURA_MOD_FEAR_2))
                            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                        if (l_Player->IsWithinLOS(x, y, z))
                            return SPELL_CAST_OK;
                    }
                }
                return SPELL_FAILED_LINE_OF_SIGHT;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (Pet* l_Pet = l_Caster->GetPet())
                        {
                            l_Pet->RemoveMovementImpairingAuras();
                            l_Pet->CastSpell(l_Target, HUNTER_SPELL_MASTERS_CALL_TRIGGERED, true);
                        }
                    }
                }
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    l_Target->RemoveMovementImpairingAuras();
                    l_Target->CastSpell(l_Target, HUNTER_SPELL_MASTERS_CALL, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
                OnCheckCast += SpellCheckCastFn(spell_hun_masters_call_SpellScript::CheckIfPetInLOS);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_masters_call_SpellScript();
        }
};

class spell_hun_pet_heart_of_the_phoenix: public SpellScriptLoader
{
    public:
        spell_hun_pet_heart_of_the_phoenix() : SpellScriptLoader("spell_hun_pet_heart_of_the_phoenix") { }

        class spell_hun_pet_heart_of_the_phoenix_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_pet_heart_of_the_phoenix_SpellScript);

            bool Load()
            {
                if (!GetCaster()->isPet())
                    return false;
                return true;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED) || !sSpellMgr->GetSpellInfo(HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* owner = caster->GetOwner())
                    if (!caster->HasAura(HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                    {
                        owner->CastCustomSpell(HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED, SPELLVALUE_BASE_POINT0, 100, caster, true);
                        caster->CastSpell(caster, HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF, true);
                    }
            }

            void Register()
            {
                // add dummy effect spell handler to pet's Last Stand
                OnEffectHitTarget += SpellEffectFn(spell_hun_pet_heart_of_the_phoenix_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_pet_heart_of_the_phoenix_SpellScript();
        }
};

// Misdirection - 34477
class spell_hun_misdirection: public SpellScriptLoader
{
    public:
        spell_hun_misdirection() : SpellScriptLoader("spell_hun_misdirection") { }

        enum MisdirectionSpells
        {
            GlyphOfMisdirection = 56829,
            MisdirectionSpell = 34477,
            MisdirectionProc = 35079
        };


        class spell_hun_misdirection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_misdirection_SpellScript);

            void HandleAfterHit()
            {
                Player* l_Caster = GetCaster()->ToPlayer();
                Unit* l_Target = GetExplTargetUnit();

                if (l_Caster == nullptr)
                    return;
                if (l_Target == nullptr)
                    return;

                Pet* l_Pet = l_Caster->GetPet();

                if (l_Pet == nullptr)
                    return;

                /// Self proc
                l_Caster->CastSpell(l_Caster, MisdirectionSpells::MisdirectionProc, true);

                if (l_Pet->GetGUID() == l_Target->GetGUID())
                {
                    if (l_Caster->HasAura(MisdirectionSpells::GlyphOfMisdirection))
                        l_Caster->RemoveSpellCooldown(MisdirectionSpells::MisdirectionSpell, true);
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_hun_misdirection_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_misdirection_SpellScript;
        }

        class spell_hun_misdirection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_misdirection_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Caster = GetCaster()->ToPlayer();

                if (l_Caster == nullptr)
                    return;

                if (!GetDuration())
                    l_Caster->SetReducedThreatPercent(0, 0);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_misdirection_AuraScript();
        }
};

// Misdirection (proc) - 35079
class spell_hun_misdirection_proc: public SpellScriptLoader
{
    public:
        spell_hun_misdirection_proc() : SpellScriptLoader("spell_hun_misdirection_proc") { }

        class spell_hun_misdirection_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_misdirection_proc_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                    GetCaster()->SetReducedThreatPercent(0, 0);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_proc_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_misdirection_proc_AuraScript();
        }
};

/// Disengage - 781
class spell_hun_disengage: public SpellScriptLoader
{
    public:
        spell_hun_disengage() : SpellScriptLoader("spell_hun_disengage") { }

        class spell_hun_disengage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_disengage_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(HUNTER_SPELL_POSTHASTE))
                    {
                        _player->RemoveMovementImpairingAuras();
                        _player->CastSpell(_player, HUNTER_SPELL_POSTHASTE_INCREASE_SPEED, true);
                    }
                    else if (_player->HasAura(HUNTER_SPELL_NARROW_ESCAPE))
                    {
                        std::list<Unit*> unitList;
                        std::list<Unit*> retsList;

                        _player->GetAttackableUnitListInRange(unitList, 8.0f);

                        for (auto itr : unitList)
                            if (_player->IsValidAttackTarget(itr))
                                retsList.push_back(itr);

                        for (auto itr : retsList)
                            _player->CastSpell(itr, HUNTER_SPELL_NARROW_ESCAPE_RETS, true);
                    }

                    if (_player->HasAura(HUNTER_SPELL_GLYPH_OF_LIBERATION))
                        _player->CastSpell(_player, HUNTER_SPELL_GLYPH_OF_LIBERATION_HEAL, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_hun_disengage_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_disengage_SpellScript();
        }
};

// Tame Beast - 1515
class spell_hun_tame_beast: public SpellScriptLoader
{
    public:
        spell_hun_tame_beast() : SpellScriptLoader("spell_hun_tame_beast") { }

        class spell_hun_tame_beast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_tame_beast_SpellScript);

            enum eCreature
            {
                Chimaeron = 43296
            };

            SpellCastResult CheckCast()
            {
                int8 l_ResultId = -1;
                Player* l_Player = GetCaster()->ToPlayer();

                if (!GetExplTargetUnit() || GetExplTargetUnit()->GetTypeId() != TYPEID_UNIT)
                    l_ResultId = PET_TAME_ERROR_INVALID_CREATURE;

                Creature* l_Target = GetExplTargetUnit()->ToCreature();

                // No errors until now, time to do more checks
                if (l_ResultId == -1)
                {
                    if (l_Player->getSlotForNewPet() == PET_SLOT_FULL_LIST)
                        l_ResultId = PET_TAME_ERROR_TOO_MANY_PETS;
                    else if (l_Player->GetPetGUID() || l_Player->GetCharmGUID())
                        l_ResultId = PET_TAME_ERROR_ANOTHER_SUMMON_ACTIVE;
                    else if (l_Target->isPet())
                        l_ResultId = PET_TAME_ERROR_CREATURE_ALREADY_OWNED;
                    else if (l_Target->getLevel() > l_Player->getLevel())
                        l_ResultId = PET_TAME_ERROR_TOO_HIGH_LEVEL;
                    else if ((l_Target->GetCreatureTemplate()->type_flags & CREATURE_TYPEFLAGS_EXOTIC) && !l_Player->CanTameExoticPets())
                        l_ResultId = PET_TAME_ERROR_CANT_CONTROL_EXOTIC;
                    else if (!l_Target->GetCreatureTemplate()->isTameable(l_Player->CanTameExoticPets()))
                        l_ResultId = PET_TAME_ERROR_NOT_TAMEABLE;
                }

                if (l_ResultId >= 0)
                {
                    l_Player->SendPetTameResult((PetTameResult) l_ResultId);
                    return SPELL_FAILED_DONT_REPORT;
                }

                /// Chimaeron can be tamed but only at 20%
                if (l_Target->GetEntry() == eCreature::Chimaeron && l_Target->GetHealthPct() > 20.0f)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CHIMAERON_IS_TOO_CALM_TO_TAME);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_tame_beast_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_tame_beast_SpellScript();
        }
};

/// Claw - 16827 / Bite - 17253 / Smack - 49966
class spell_hun_claw_bite : public SpellScriptLoader
{
    public:
        spell_hun_claw_bite() : SpellScriptLoader("spell_hun_claw_bite") { }

        enum eSpells
        {
            EnhancedBasicAttacksAura = 157715,
            EnhancedBasicAttacksProc = 157717
        };

        class spell_hun_claw_bite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_claw_bite_SpellScript);

            SpellCastResult CheckCastRange()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                Unit* l_Owner = l_Caster->GetOwner();

                if (l_Owner == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                Unit* l_Target = GetExplTargetUnit();

                if (l_Target == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                if (l_Owner->HasAura(HUNTER_SPELL_BLINK_STRIKES))
                {
                    if (l_Owner->ToPlayer()->HasSpellCooldown(HUNTER_SPELL_BLINK_STRIKES) && l_Caster->GetDistance(l_Target) > 10.0f)
                        return SPELL_FAILED_OUT_OF_RANGE;

                    if ((l_Caster->isInRoots() || l_Caster->isInStun()) && l_Caster->GetDistance(l_Target) > 5.0f)
                        return SPELL_FAILED_ROOTED;

                    /// Blinking Strikes
                    if (!l_Owner->ToPlayer()->HasSpellCooldown(HUNTER_SPELL_BLINK_STRIKES) && l_Target->IsWithinLOSInMap(l_Caster) && 
                        l_Caster->GetDistance(l_Target) > 10.0f && l_Caster->GetDistance(l_Target) < 30.0f && !l_Caster->isInRoots() && !l_Caster->isInStun())
                    {
                        l_Caster->CastSpell(l_Target, HUNTER_SPELL_BLINK_STRIKES_TELEPORT, true);

                        if (l_Caster->ToCreature()->IsAIEnabled && l_Caster->ToPet())
                        {
                            l_Caster->ToPet()->ClearUnitState(UNIT_STATE_FOLLOW);
                            if (l_Caster->ToPet()->getVictim())
                                l_Caster->ToPet()->AttackStop();
                            l_Caster->GetMotionMaster()->Clear();
                            l_Caster->ToPet()->GetCharmInfo()->SetIsCommandAttack(true);
                            l_Caster->ToPet()->GetCharmInfo()->SetIsAtStay(false);
                            l_Caster->ToPet()->GetCharmInfo()->SetIsReturning(false);
                            l_Caster->ToPet()->GetCharmInfo()->SetIsFollowing(false);

                            l_Caster->ToCreature()->AI()->AttackStart(l_Target);
                        }

                        l_Owner->ToPlayer()->AddSpellCooldown(HUNTER_SPELL_BLINK_STRIKES, 0, 20 * IN_MILLISECONDS, true);
                    }
                }

                return SPELL_CAST_OK;
            }


            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Pet* l_Pet = GetCaster()->ToPet())
                {
                    if (Unit* l_Hunter = GetCaster()->GetOwner())
                    {
                        int32 l_Damage = int32(l_Hunter->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.333f);
                        l_Damage = l_Pet->SpellDamageBonusDone(GetHitUnit(), GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                        l_Damage = GetHitUnit()->SpellDamageBonusTaken(l_Pet, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                        SpellInfo const* l_SpikedCollar = sSpellMgr->GetSpellInfo(HUNTER_SPELL_SPIKED_COLLAR);
                        SpellInfo const* l_EnhancedBasicAttacks = sSpellMgr->GetSpellInfo(eSpells::EnhancedBasicAttacksAura);
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(HUNTER_SPELL_BASIC_ATTACK_COST_MODIFIER);
                        SpellInfo const* l_Frenzy = sSpellMgr->GetSpellInfo(HUNTER_SPELL_FRENZY);

                        /// Increases the damage done by your pet's Basic Attacks by 10%
                        if (l_Hunter->HasAura(HUNTER_SPELL_SPIKED_COLLAR) && l_SpikedCollar != nullptr)
                            AddPct(l_Damage, l_SpikedCollar->Effects[EFFECT_0].BasePoints);

                        bool l_FreeCostSpell = l_Pet->HasAura(eSpells::EnhancedBasicAttacksProc);

                        /// Deals 100% more damage and costs 100% more Focus when your pet has 50 or more Focus.
                        if (l_Pet->GetPower(POWER_FOCUS) + 25 >= 50 && !l_FreeCostSpell)
                        {
                            if (l_SpellInfo != nullptr)
                                l_Damage += CalculatePct(l_Damage, l_SpellInfo->Effects[EFFECT_1].BasePoints);
                            l_Pet->EnergizeBySpell(l_Pet, GetSpellInfo()->Id, -25, POWER_FOCUS);
                        }

                        if (l_FreeCostSpell)
                            l_Pet->RemoveAura(eSpells::EnhancedBasicAttacksProc);

                        if (l_EnhancedBasicAttacks != nullptr && l_Hunter->HasAura(eSpells::EnhancedBasicAttacksAura) && roll_chance_i(l_EnhancedBasicAttacks->Effects[EFFECT_0].BasePoints))
                            l_Pet->CastSpell(l_Pet, eSpells::EnhancedBasicAttacksProc, true);

                        /// Frenzy - 19623
                        if (l_Hunter->HasAura(HUNTER_SPELL_FRENZY) && roll_chance_i(l_Frenzy->Effects[EFFECT_1].BasePoints))
                            l_Pet->CastSpell(l_Pet, HUNTER_SPELL_FRENZY_STACKS, true);

                        // WoD: Apply factor on damages depending on creature level and expansion
                        if (l_Pet->IsPetGuardianStuff() && GetHitUnit()->GetTypeId() == TYPEID_UNIT)
                            l_Damage *= l_Pet->CalculateDamageDealtFactor(l_Pet, GetHitUnit()->ToCreature());
                        else if (l_Pet->GetTypeId() == TYPEID_UNIT && (GetHitUnit()->GetTypeId() == TYPEID_PLAYER || GetHitUnit()->IsPetGuardianStuff()))
                            l_Damage *= l_Pet->CalculateDamageTakenFactor(GetHitUnit(), l_Pet->ToCreature());

                        /// Reduce damage by armor
                        if (l_Pet->IsDamageReducedByArmor(SPELL_SCHOOL_MASK_NORMAL, GetSpellInfo()))
                            l_Damage = l_Pet->CalcArmorReducedDamage(GetHitUnit(), l_Damage, GetSpellInfo(), BaseAttack);
                        SetHitDamage(l_Damage);
                    }
                }
            }

            void HandleAfterHit()
            {
                if (Unit* l_Pet = GetCaster())
                {
                    if (Unit* l_Hunter = GetCaster()->GetOwner())
                    {
                        if (AuraPtr l_CobraStrike = l_Hunter->GetAura(HUNTER_SPELL_COBRA_STRIKES_STACKS))
                            l_CobraStrike->ModStackAmount(-1);
                        if (AuraPtr l_CobraStrikePet = l_Pet->GetAura(HUNTER_SPELL_COBRA_STRIKES_STACKS))
                            l_CobraStrikePet->ModStackAmount(-1);
                    }
                }
            }

            void HandleBeforeHit()
            {
                SetHitDamage(1);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_hun_claw_bite_SpellScript::CheckCastRange);
                BeforeHit += SpellHitFn(spell_hun_claw_bite_SpellScript::HandleBeforeHit);
                OnEffectHitTarget += SpellEffectFn(spell_hun_claw_bite_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                AfterHit += SpellHitFn(spell_hun_claw_bite_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_claw_bite_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Spirit Mend - 90361
class spell_hun_spirit_mend : public SpellScriptLoader
{
    public:
        spell_hun_spirit_mend() : SpellScriptLoader("spell_hun_spirit_mend") { }

        class spell_hun_spirit_mend_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_spirit_mend_AuraScript);

            void CalculateAmount(constAuraEffectPtr l_AuraEffect, int32& l_Amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_AuraEffect->GetAmplitude() && GetMaxDuration())
                        l_Amount = int32(l_Caster->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.35f * 2.0f) / (GetMaxDuration() / l_AuraEffect->GetAmplitude());
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_spirit_mend_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_spirit_mend_AuraScript();
        }

        class spell_hun_spirit_mend_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_spirit_mend_SpellScript);

            void HandleHeal(SpellEffIndex l_Idx)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                int32 l_Heal = int32(l_Caster->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.35f * 3.0f);
                l_Heal = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_Heal, l_Idx, SPELL_DIRECT_DAMAGE);
                l_Heal = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_Heal, SPELL_DIRECT_DAMAGE);
                SetHitHeal(l_Heal);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_spirit_mend_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_spirit_mend_SpellScript();
        }
};

/// Mend Pet - 136
class spell_hun_mend_pet : public SpellScriptLoader
{
    public:
        spell_hun_mend_pet() : SpellScriptLoader("spell_hun_mend_pet") { }

        class spell_hun_mend_pet_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_mend_pet_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(HUNTER_SPELL_GLYPH_OF_MEND_PET);

                if (l_Caster == nullptr || l_Target == nullptr || l_SpellInfo == nullptr)
                    return;

                if (l_Caster->HasAura(HUNTER_SPELL_GLYPH_OF_MEND_PET)) ///< Glyph of Mend Pet
                {
                    if (roll_chance_i(l_SpellInfo->Effects[EFFECT_0].BasePoints))
                        l_Caster->CastSpell(l_Target, HUNTER_SPELL_GLYPH_OF_MEND_PET_TICK, true); ///< Dispel
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_mend_pet_AuraScript::OnTick, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_mend_pet_AuraScript();
        }
};

// Thrill of the Hunt - 109396
class PlayerScript_thrill_of_the_hunt: public PlayerScript
{
    public:
        PlayerScript_thrill_of_the_hunt() :PlayerScript("PlayerScript_thrill_of_the_hunt") {}

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
        {
            if (p_Regen)
                return;

            // Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_diffValue = p_NewValue - p_OldValue;

            if (p_Player->getClass() == CLASS_HUNTER && p_Power == POWER_FOCUS && p_Player->HasAura(HUNTER_SPELL_THRILL_OF_THE_HUNT) && l_diffValue < 0)
            {
                for (int8 i = 0; i < ((l_diffValue / 10) * -1); ++i)
                {
                    if (roll_chance_i(sSpellMgr->GetSpellInfo(HUNTER_SPELL_THRILL_OF_THE_HUNT)->Effects[EFFECT_0].BasePoints))
                        p_Player->CastSpell(p_Player, HUNTER_SPELL_THRILL_OF_THE_HUNT_PROC, true);
                }
            }
        }
};

// Explosive Trap (damage) - 13812
class spell_hun_explosive_trap : public SpellScriptLoader
{
    public:
        spell_hun_explosive_trap() : SpellScriptLoader("spell_hun_explosive_trap") { }

        class spell_hun_explosive_trap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_explosive_trap_SpellScript);

            enum HunterExplosiveTrap
            {
                SpellGlyphOfExplosiveTrap = 119403
            };

            void HandlePeriodicDamage(SpellEffIndex p_EffIndex)
            {
                if (GetCaster()->HasAura(HunterExplosiveTrap::SpellGlyphOfExplosiveTrap))
                    PreventHitAura();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_explosive_trap_SpellScript::HandlePeriodicDamage, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_explosive_trap_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Explosive Shot - 53301
class spell_hun_explosive_shot : public SpellScriptLoader
{
    public:
        spell_hun_explosive_shot() : SpellScriptLoader("spell_hun_explosive_shot") { }

        class spell_hun_explosive_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_explosive_shot_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();

                SetHitDamage((int32)(l_Caster->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.553f * 1.08f));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_explosive_shot_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        class spell_hun_explosive_shot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_explosive_shot_AuraScript);

            void CalculateAmount(constAuraEffectPtr p_AuraEffect, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (p_AuraEffect->GetAmplitude() <= 0)
                    return;

                if (p_AuraEffect->GetBase() == nullptr || p_AuraEffect->GetBase()->GetDuration() == 0)
                    return;

                p_Amount = (int32)((l_Caster->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.553f * 1.08f) / float(p_AuraEffect->GetBase()->GetDuration() / p_AuraEffect->GetAmplitude()));
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_explosive_shot_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_explosive_shot_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_explosive_shot_SpellScript();
        }
};

/// Poisoned Ammo - 162543
class spell_hun_poisoned_ammo : public SpellScriptLoader
{
public:
    spell_hun_poisoned_ammo() : SpellScriptLoader("spell_hun_poisoned_ammo") { }

    class spell_hun_poisoned_ammo_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_poisoned_ammo_SpellScript);

        int32 m_Damage = 0;

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (AuraPtr l_PoisonedAmmo = l_Target->GetAura(HUNTER_SPELL_POISONED_AMMO))
                    {
                        const SpellInfo *l_SpellInfo = sSpellMgr->GetSpellInfo(HUNTER_SPELL_POISONED_AMMO_AURA);

                        if (m_Damage > 0)
                            m_Damage = m_Damage / (l_PoisonedAmmo->GetDuration() / l_PoisonedAmmo->GetEffect(0)->GetAmplitude());

                        int32 l_BasicDamage = int32(0.046f * l_Caster->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack));
                        l_BasicDamage = l_Caster->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_BasicDamage, 0, DOT, l_PoisonedAmmo->GetEffect(0)->GetBase()->GetStackAmount());
                        l_BasicDamage = l_Target->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_BasicDamage, DOT, l_PoisonedAmmo->GetEffect(0)->GetBase()->GetStackAmount());

                        m_Damage += l_BasicDamage;

                        l_PoisonedAmmo->GetEffect(0)->SetAmount(m_Damage);
                    }
                }
            }
        }

        void HandleBeforeHit()
        {
            if (Unit* l_Target = GetHitUnit())
            {
                if (AuraPtr l_PoisonedAmmo = l_Target->GetAura(HUNTER_SPELL_POISONED_AMMO))
                    m_Damage = l_PoisonedAmmo->GetEffect(0)->GetAmount() * (l_PoisonedAmmo->GetDuration() / l_PoisonedAmmo->GetEffect(0)->GetAmplitude());
            }
        }

        void Register()
        {
            BeforeHit += SpellHitFn(spell_hun_poisoned_ammo_SpellScript::HandleBeforeHit);
            OnHit += SpellHitFn(spell_hun_poisoned_ammo_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_hun_poisoned_ammo_SpellScript();
    }
};

/// last update : 6.1.2 19802
/// Adaptation - 152244
class spell_hun_adaptation : public SpellScriptLoader
{
    public:
        spell_hun_adaptation() : SpellScriptLoader("spell_hun_adaptation") { }

        class spell_hun_adaptation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_adaptation_AuraScript);

            enum eSpells
            {
                CombatExperienceAdaptation = 156843,
                CombatExperience = 20782
            };

            void OnApply(constAuraEffectPtr, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();
                 
                if (l_Caster == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (l_Pet->HasAura(eSpells::CombatExperience))
                            l_Pet->RemoveAura(eSpells::CombatExperience);
                        l_Pet->CastSpell(l_Pet, eSpells::CombatExperienceAdaptation, true);
                    }
                }
            }

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        l_Pet->RemoveAura(eSpells::CombatExperienceAdaptation);
                        l_Pet->CastSpell(l_Pet, eSpells::CombatExperience, true);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hun_adaptation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_PET_SPECS, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_adaptation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_PET_SPECS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_adaptation_AuraScript();
        }
};


/// Aimed Shot - 19434
class spell_hun_aimed_shot : public SpellScriptLoader
{
    public:
        spell_hun_aimed_shot() : SpellScriptLoader("spell_hun_aimed_shot") { }

        class spell_hun_aimed_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_aimed_shot_SpellScript);

            void HandleAfterCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(HUNTER_SPELL_ITEM_WOD_PVP_MM_4P_BONUS))
                    {
                        if (l_Player->HasSpellCooldown(HUNTER_SPELL_RAPID_FIRE))
                            l_Player->ReduceSpellCooldown(HUNTER_SPELL_RAPID_FIRE, 5000);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_hun_aimed_shot_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_aimed_shot_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Thick Hide - 160057
class spell_hun_thick_hide : public SpellScriptLoader
{
    public:
        spell_hun_thick_hide() : SpellScriptLoader("spell_hun_thick_hide") { }

        class spell_hun_thick_hide_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_thick_hide_AuraScript);

            enum eSpells
            {
                thickHideEffect = 160058
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Pet* l_Pet = l_Caster->ToPet();

                if (l_Pet == nullptr)
                    return;

                if (l_Pet->HasSpellCooldown(eSpells::thickHideEffect))
                    return;

                if (l_Pet->GetHealthPct() > (float)GetSpellInfo()->Effects[EFFECT_1].BasePoints)
                    return;

                l_Pet->CastSpell(l_Pet, eSpells::thickHideEffect, true);
                l_Pet->_AddCreatureSpellCooldown(eSpells::thickHideEffect, time(nullptr) + GetSpellInfo()->Effects[EFFECT_2].BasePoints);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_hun_thick_hide_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_thick_hide_AuraScript();
        }
};

void AddSC_hunter_spell_scripts()
{
    new spell_hun_thick_hide();
    new spell_hun_lesser_proportion();
    new spell_hun_glyph_of_lesser_proportion();
    new spell_hun_enhanced_basic_attacks();
    new spell_hun_black_arrow();
    new spell_hun_fetch_glyph();
    new spell_hun_glyph_of_aspect_of_the_cheetah();
    new spell_hun_glyph_of_mirrored_blades();
    new spell_hun_mend_pet();
    new spell_hun_kill_command_proc();
    new spell_hun_spirit_mend();
    new spell_hun_thunderstomp();
    new spell_hun_steady_focus();
    new spell_hun_cornered();
    new spell_hun_lone_wolf();
    new spell_hun_kill_shot();
    new spell_hun_exotic_munitions();
    new spell_hun_claw_bite();
    new spell_hun_glyph_of_animal_bond();
    new spell_hun_spirit_bond_apply();
    new spell_hun_fireworks();
    new spell_hun_glyph_of_fireworks();
    new spell_hun_glyph_of_aspects();
    new spell_hun_bestial_wrath_dispel();
    new spell_hun_bestial_wrath_dispel();
    new spell_hun_item_pvp_s13_2p();
    new spell_hun_spirit_bond();
    new spell_hun_glyph_of_aspect_of_the_beast();
    new spell_hun_glaive_toss_damage();
    new spell_hun_glaive_toss_missile();
    new spell_hun_glyph_of_fetch();
    new spell_hun_dire_beast();
    new spell_hun_a_murder_of_crows();
    new spell_hun_focus_fire();
    new spell_hun_frenzy();
    new spell_hun_lynx_rush();
    new spell_hun_beast_cleave_proc();
    new spell_hun_beast_cleave();
    new spell_hun_cobra_strikes();
    new spell_hun_barrage();
    new spell_hun_binding_shot();
    new spell_hun_binding_shot_zone();
    new spell_hun_powershot();
    new spell_hun_feign_death();
    new spell_hun_camouflage_visual();
    new spell_hun_serpent_spread();
    new spell_hun_ancient_hysteria();
    new spell_hun_netherwinds();
    new spell_hun_kill_command();
    new spell_hun_cobra_shot();
    new spell_hun_steady_shot();
    new spell_hun_chimaera_shot();
    new spell_hun_chimaera_shot_damage();
    new spell_hun_last_stand_pet();
    new spell_hun_masters_call();
    new spell_hun_pet_heart_of_the_phoenix();
    new spell_hun_misdirection();
    new spell_hun_misdirection_proc();
    new spell_hun_disengage();
    new spell_hun_tame_beast();
    new spell_hun_explosive_trap();
    new spell_hun_burrow_attack();
    new spell_hun_explosive_shot();
    new spell_hun_aimed_shot();
    new spell_hun_poisoned_ammo();
    new spell_hun_adaptation();

    // Player Script
    new PlayerScript_thrill_of_the_hunt();
}