////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
    HUNTER_SPELL_SPIRIT_BOND_BUFF                   = 118694,
    HUNTER_SPELL_SPIRIT_BOND                        = 109212,
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

            void OnUpdate(uint32, AuraEffect* p_AurEff)
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

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                        l_Player->CastSpell(l_Pet, eSpells::LesserProportion, true);
                }
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
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

/// Last Update 6.2.3
/// Black Arrow - 3674
class spell_hun_black_arrow : public SpellScriptLoader
{
    public:
        spell_hun_black_arrow() : SpellScriptLoader("spell_hun_black_arrow") { }

        class spell_hun_black_arrow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_black_arrow_SpellScript);

            enum eSpells
            {
                T17Survival2P   = 165544,
                LockAndLoad     = 168980,
                ExplosiveShot   = 53301
            };

            void HandleApplyDoT(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (!l_Caster->HasAura(eSpells::T17Survival2P))
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasSpellCooldown(eSpells::ExplosiveShot))
                    l_Player->RemoveSpellCooldown(eSpells::ExplosiveShot, true);
                l_Player->CastSpell(l_Player, eSpells::LockAndLoad, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_black_arrow_SpellScript::HandleApplyDoT, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_hun_black_arrow_SpellScript();
        }

        class spell_hun_black_arrow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_black_arrow_AuraScript);

            enum eSpells
            {
                LockAndLoad     = 168980,
                ExplosiveShot   = 53301
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (!roll_chance_i(GetSpellInfo()->Effects[EFFECT_1].BasePoints))
                        return;

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
                        l_Player->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_black_arrow_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnDispel += AuraDispelFn(spell_hun_black_arrow_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const override
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

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
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

            void CalculateAmount(AuraEffect const*, int32& p_Amount, bool&)
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

/// last update : 6.1.2 19802
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
                Unit* l_Caster = GetCaster();
                Unit* l_Owner = GetCaster()->GetOwner();
                Unit* l_Target = GetHitUnit();

                if (l_Owner == nullptr || l_Target == nullptr)
                    return;

                int32 l_Damage = (int32)(l_Owner->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.250f);

                l_Damage = l_Caster->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                l_Damage = l_Target->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                SetHitDamage(l_Damage);
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

/// Last Update 6.2.3
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
                SteadyShot      = 56641,
                CobraShot       = 77767,
                SteadyFocus     = 177668
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
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

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                switch (l_Player->GetSpecializationId(l_Player->GetActiveSpec()))
                {
                    ///< Beast Mastery and Survival (Level 81)
                    ///< - Cobra Shot twice in a row
                    case SpecIndex::SPEC_HUNTER_BEASTMASTERY:
                    case SpecIndex::SPEC_HUNTER_SURVIVAL:
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
                    default:
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
                }
            }

            void DealWithCharges(AuraEffect const* p_AurEff, Player* p_Player)
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

            void OnUpdate(uint32, AuraEffect* p_AurEff)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (l_Target->HasAuraState(AuraStateType::AURA_STATE_HEALTHLESS_35_PERCENT))
                    {
                        p_AurEff->ChangeAmount(GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                        if (AuraEffect* l_AurEff = p_AurEff->GetBase()->GetEffect(EFFECT_1))
                            l_AurEff->ChangeAmount(GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                    }
                    else
                    {
                        p_AurEff->ChangeAmount(0);

                        if (AuraEffect* l_AurEff = p_AurEff->GetBase()->GetEffect(EFFECT_1))
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

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
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

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
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

            void OnUpdate(uint32, AuraEffect* p_AurEff)
            {
                if (!GetUnitOwner())
                    return;

                Player* l_Player = GetUnitOwner()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Pet* l_Pet = l_Player->GetPet();

                if (l_Pet != nullptr && !l_Pet->m_Stampeded)
                {
                    l_Player->RemoveAura(LoneWolfes::LoneWolfAura);

                    p_AurEff->ChangeAmount(0, true, true);

                    if (AuraEffect* l_AuraEffect = p_AurEff->GetBase()->GetEffect(EFFECT_1))
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

                        if (AuraEffect* l_AuraEffect = p_AurEff->GetBase()->GetEffect(EFFECT_1))
                            l_AuraEffect->ChangeAmount(GetSpellInfo()->Effects[EFFECT_0].BasePoints, true, true);
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

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
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

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
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

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(HUNTER_SPELL_FIREWORKS, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

            void OnUpdate(uint32, AuraEffect* /*p_AurEff*/)
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

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
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

/// Last Update 6.2.3
/// Bestial Wrath - 19574 and The Beast Within - 34471
class spell_hun_bestial_wrath_dispel: public SpellScriptLoader
{
    public:
        spell_hun_bestial_wrath_dispel() : SpellScriptLoader("spell_hun_bestial_wrath_dispel") { }

        class spell_hun_bestial_wrath_dispel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_bestial_wrath_dispel_AuraScript);

            enum eSpells
            {
                T17BeastMaster4P        = 165518,
                BestialWrathStampede    = 167135,
                BestialWrath            = 19574
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Pet* l_Pet = l_Target->ToPet())
                    {
                        if (l_Pet->m_Stampeded)
                            return;
                    }
                    uint32 l_Mechanic = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT)
                                        | (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                                        | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                                        | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                                        | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                                        | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN);

                    l_Target->RemoveAurasWithMechanic(l_Mechanic, AURA_REMOVE_BY_DEFAULT, GetSpellInfo()->Id);

                    /// While Bestial Wrath is active, one additional pet is summoned to fight with you.
                    if (Unit* l_Caster = GetCaster())
                    {
                        if (l_Caster->GetTypeId() == TypeID::TYPEID_PLAYER && m_scriptSpellId == eSpells::BestialWrath && l_Caster->HasAura(eSpells::T17BeastMaster4P))
                        {
                            Unit* l_Victim = l_Caster->ToPlayer()->GetSelectedUnit();
                            if (!l_Victim || !l_Caster->IsValidAttackTarget(l_Victim))
                                l_Victim = l_Caster->getVictim();
                            if (!l_Victim || !l_Caster->IsValidAttackTarget(l_Victim))
                                l_Victim = l_Caster;

                            l_Caster->CastSpell(l_Victim, eSpells::BestialWrathStampede, true);
                        }
                    }
                }
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case 19574: /// Bestial Wrath
                        AfterEffectApply += AuraEffectApplyFn(spell_hun_bestial_wrath_dispel_AuraScript::OnApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case 34471: /// The Beast Within
                        AfterEffectApply += AuraEffectApplyFn(spell_hun_bestial_wrath_dispel_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
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

            void OnTick(AuraEffect const*)
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
                        l_Player->RemoveAura(HunterSpells::HUNTER_SPELL_SPIRIT_BOND_BUFF);
                    else if (!l_Player->HasAura(HunterSpells::HUNTER_SPELL_SPIRIT_BOND_BUFF))
                        l_Player->CastSpell(l_Player, HunterSpells::HUNTER_SPELL_SPIRIT_BOND_BUFF, true);
                }
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->CastSpell(l_Caster, HunterSpells::HUNTER_SPELL_SPIRIT_BOND_BUFF, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->RemoveAura(HunterSpells::HUNTER_SPELL_SPIRIT_BOND_BUFF);
                if (l_Caster->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (Unit* l_Pet = l_Caster->ToPlayer()->GetPet())
                        l_Pet->RemoveAura(HunterSpells::HUNTER_SPELL_SPIRIT_BOND);
                }
            }

            void Register()
            {
                OnAuraUpdate += AuraUpdateFn(spell_hun_spirit_bond_apply_AuraScript::OnUpdate);
                OnEffectApply += AuraEffectApplyFn(spell_hun_spirit_bond_apply_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_spirit_bond_apply_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_REAL);
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

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(HUNTER_SPELL_ASPECT_OF_THE_BEAST, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

/// Last Update 6.2.3
/// Glaive Toss (damage) - 120761 and 121414
class spell_hun_glaive_toss_damage: public SpellScriptLoader
{
    public:
        spell_hun_glaive_toss_damage() : SpellScriptLoader("spell_hun_glaive_toss_damage") { }

        class spell_hun_glaive_toss_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_glaive_toss_damage_SpellScript);

            uint64 mainTargetGUID = 0;

            void CorrectRange(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_OriginalCaster = GetOriginalCaster();
                Unit* l_MainTarget = nullptr;

                for (auto itr : p_Targets)
                {
                    Unit* l_Target = itr->ToUnit();

                    if (l_Target->GetGUID() == l_OriginalCaster->GetGlaiveOfTossTargetGUID())
                    {
                        mainTargetGUID = itr->GetGUID();
                        l_OriginalCaster->removeGlaiveTossTarget();
                        break;
                    }
                }

                if (!mainTargetGUID)
                    mainTargetGUID = l_OriginalCaster->GetGUID();

                l_MainTarget = ObjectAccessor::FindUnit(mainTargetGUID);

                if (l_MainTarget == nullptr)
                {
                    p_Targets.clear();
                    return;
                }

                p_Targets.remove_if([this, l_Caster, l_MainTarget](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || !p_Object->IsWithinLOSInMap(l_Caster))
                        return true;

                    if (p_Object->ToUnit() && !l_Caster->IsValidAttackTarget(p_Object->ToUnit()))
                        return true;

                    if (p_Object->GetGUID() == l_MainTarget->GetGUID())
                        return false;

                    if (p_Object->IsInElipse(l_Caster, l_MainTarget, 7.0f, 4.0f))
                        return false;
                    return true;
                });
            }

            void OnDamage()
            {
                Unit* l_Target = GetHitUnit();

                if (!mainTargetGUID || l_Target == nullptr)
                    return;

                Unit* l_MainTarget = ObjectAccessor::FindUnit(mainTargetGUID);

                if (l_MainTarget != nullptr && l_MainTarget->GetGUID() == l_Target->GetGUID())
                    SetHitDamage(GetHitDamage() * 4);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectRange, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
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
                Unit* l_Target = GetExplTargetUnit();
                Unit* l_Caster = GetCaster();
                Unit* l_OriginalCaster = GetOriginalCaster();

                if (l_Target != nullptr)
                {
                    if (l_Caster->GetGUID() == GetOriginalCaster()->GetGUID())
                        l_Caster->SetGlaiveTossTarget(l_Target->GetGUID());
                    else
                        l_OriginalCaster->SetGlaiveTossTarget(l_Caster->GetGUID());
                }

                if (GetSpellInfo()->Id == HUNTER_SPELL_GLAIVE_TOSS_RIGHT)
                {
                    if (l_OriginalCaster != nullptr)
                        l_OriginalCaster->CastSpell(l_OriginalCaster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                    else
                        l_Caster->CastSpell(l_Caster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                }
                if (GetSpellInfo()->Id == HUNTER_SPELL_GLAIVE_TOSS_LEFT)
                {
                    if (l_OriginalCaster != nullptr)
                        l_OriginalCaster->CastSpell(l_OriginalCaster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                    else
                        l_Caster->CastSpell(l_Caster, HUNTER_SPELL_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                }
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                if (GetSpellInfo()->Id == HUNTER_SPELL_GLAIVE_TOSS_RIGHT)
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster == GetOriginalCaster())
                            l_Target->CastSpell(l_Caster, HUNTER_SPELL_GLAIVE_TOSS_LEFT, true, NULL, nullptr, l_Caster->GetGUID());
                    }
                }
                else
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster == GetOriginalCaster())
                            l_Target->CastSpell(l_Caster, HUNTER_SPELL_GLAIVE_TOSS_RIGHT, true, NULL, nullptr, l_Caster->GetGUID());
                    }
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

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(HUNTER_SPELL_FETCH, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

/// Last Update 6.2.3
/// Dire Beast - 120679
class spell_hun_dire_beast: public SpellScriptLoader
{
    public:
        spell_hun_dire_beast() : SpellScriptLoader("spell_hun_dire_beast") { }

        class spell_hun_dire_beast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_dire_beast_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        // Summon's skin is different function of Map or Zone ID
                        switch (l_Player->GetZoneId())
                        {
                            case 5785: // The Jade Forest
                                l_Player->CastSpell(l_Target, DIRE_BEAST_JADE_FOREST, true);
                                break;
                            case 5805: // Valley of the Four Winds
                                l_Player->CastSpell(l_Target, DIRE_BEAST_VALLEY_OF_THE_FOUR_WINDS, true);
                                break;
                            case 5840: // Vale of Eternal Blossoms
                                l_Player->CastSpell(l_Target, DIRE_BEAST_VALE_OF_THE_ETERNAL_BLOSSOM, true);
                                break;
                            case 5841: // Kun-Lai Summit
                                l_Player->CastSpell(l_Target, DIRE_BEAST_KUN_LAI_SUMMIT, true);
                                break;
                            case 5842: // Townlong Steppes
                                l_Player->CastSpell(l_Target, DIRE_BEAST_TOWNLONG_STEPPES, true);
                                break;
                            case 6134: // Krasarang Wilds
                                l_Player->CastSpell(l_Target, DIRE_BEAST_KRASARANG_WILDS, true);
                                break;
                            case 6138: // Dread Wastes
                                l_Player->CastSpell(l_Target, DIRE_BEAST_DREAD_WASTES, true);
                                break;
                            default:
                            {
                                switch (l_Player->GetMapId())
                                {
                                    case 0: // Eastern Kingdoms
                                        l_Player->CastSpell(l_Target, DIRE_BEAST_EASTERN_KINGDOMS, true);
                                        break;
                                    case 1: // Kalimdor
                                        l_Player->CastSpell(l_Target, DIRE_BEAST_KALIMDOR, true);
                                        break;
                                    case 8: // Outland
                                        l_Player->CastSpell(l_Target, DIRE_BEAST_OUTLAND, true);
                                        break;
                                    case 10: // Northrend
                                        l_Player->CastSpell(l_Target, DIRE_BEAST_NORTHREND, true);
                                        break;
                                    default:
                                        if (l_Player->GetMap()->IsDungeon() || l_Player->GetMap()->IsBattlegroundOrArena())
                                            l_Player->CastSpell(l_Target, DIRE_BEAST_DUNGEONS, true);
                                        else ///< Default beast in case there is not
                                            l_Player->CastSpell(l_Target, DIRE_BEAST_KALIMDOR, true);
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

/// last update : 6.1.2
/// A Murder of Crows - 131894
class spell_hun_a_murder_of_crows: public SpellScriptLoader
{
    public:
        spell_hun_a_murder_of_crows() : SpellScriptLoader("spell_hun_a_murder_of_crows") { }

        class spell_hun_a_murder_of_crows_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_a_murder_of_crows_AuraScript);

            enum eSpells
            {
                FreezingTrap = 3355,
                MurderOfCrowsVisualFirst = 131951,
                MurderOfCrowsVisualSecond = 131952
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr)
                    return;

                /// Visual effect
                /// Four crows fall from the sky to target
                for (int8 i = 0; i < 2; ++i)
                {
                    l_Target->CastSpell(l_Target, eSpells::MurderOfCrowsVisualFirst, true);
                    l_Target->CastSpell(l_Target, eSpells::MurderOfCrowsVisualSecond, true);
                }

                if (l_Caster->IsValidAttackTarget(l_Target))
                    l_Caster->CastSpell(l_Target, HUNTER_SPELL_A_MURDER_OF_CROWS_DAMAGE, true);
                else
                    p_AurEff->GetBase()->Remove();

                if (l_Target->HasAura(eSpells::FreezingTrap))
                    l_Target->RemoveAura(eSpells::FreezingTrap);
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                
                if (!l_Caster)
                    return;
                
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                {
                    if (Player* l_Player = l_Caster->ToPlayer())
                    {
                        if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                            l_Player->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                    }
                }
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

/// last update : 6.2.3
/// A Murder of Crows (damage) - 131900
class spell_hun_a_murder_of_crows_damage : public SpellScriptLoader
{
    public:
        spell_hun_a_murder_of_crows_damage() : SpellScriptLoader("spell_hun_a_murder_of_crows_damage") { }

        class spell_hun_a_murder_of_crows_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_a_murder_of_crows_damage_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Target = GetHitUnit();
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Target == nullptr || l_Player == nullptr)
                    return;

                /// A Murder of Crows now deals only 75% of normal damage against player-controlled targets.
                if (l_Target->GetSpellModOwner() && l_Player->GetSpecializationId() == SpecIndex::SPEC_HUNTER_BEASTMASTERY)
                    SetHitDamage(CalculatePct(GetHitDamage(), 75));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_a_murder_of_crows_damage_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_a_murder_of_crows_damage_SpellScript();
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

            void CalculateAmount(AuraEffect const*, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(HUNTER_SPELL_IMPROVED_FOCUS_FIRE))
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(HUNTER_SPELL_IMPROVED_FOCUS_FIRE);
                        if (l_SpellInfo == nullptr)
                            return;

                        if (Aura* l_Frenzy = l_Caster->GetAura(HUNTER_SPELL_FRENZY_STACKS))
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
                    if (AuraEffect* l_FocusFire = l_Player->GetAuraEffect(HUNTER_SPELL_FOCUS_FIRE_AURA, EFFECT_0))
                    {
                        if (Aura* l_Frenzy = l_Player->GetAura(HUNTER_SPELL_FRENZY_STACKS))
                        {
                            if (Pet* l_Pet = l_Player->GetPet())
                            {
                                l_FocusFire->ChangeAmount(l_FocusFire->GetAmount() * l_Frenzy->GetStackAmount());

                                if (Aura* l_FrenzyPet = l_Pet->GetAura(HUNTER_SPELL_FRENZY_STACKS))
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
                        if (Aura* l_Frenzy = l_Caster->GetAura(HUNTER_SPELL_FRENZY_STACKS))
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

            void HandleRemove(AuraEffect const*, AuraEffectHandleModes)
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

            void OnTick(AuraEffect const* /*aurEff*/)
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

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
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
                        if (AuraEffect* l_AurEff = l_Player->GetAuraEffect(eSpells::BeastCleaveAura, EFFECT_0))
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

/// Last Update 6.2.3
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

                    if (p_Object->ToUnit() && !l_Caster->IsValidAttackTarget(p_Object->ToUnit()))
                        return true;

                    return false;
                });
            }

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                float l_Mindamage = 0.0f;
                float l_Maxdamage = 0.0f;

                l_Player->CalculateMinMaxDamage(WeaponAttackType::RangedAttack, true, true, l_Mindamage, l_Maxdamage);
               
                int32 l_Damage = (l_Mindamage + l_Maxdamage) / 2 * 0.7f;

                if (!l_Target->HasAura(eSpells::BarrageTalent, l_Player->GetGUID()))
                    l_Damage /= 2;

                l_Damage = l_Player->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                l_Damage = l_Target->SpellDamageBonusTaken(l_Player, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                /// Barrage now deals only 80% of normal damage against player-controlled targets.
                if (l_Target->GetSpellModOwner())
                    l_Damage = CalculatePct(l_Damage, 80);

                SetHitDamage(l_Damage);
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

            void OnUpdate(uint32, AuraEffect*)
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
                                if (Aura* l_Stun = l_Target->GetAura(eSpells::BindingShotStun))
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

                    if (itr->IsPlayer())
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

                    if (itr->IsPlayer())
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
class spell_hun_ancient_hysteria : public SpellScriptLoader
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

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (GetSpellInfo() && l_Target->HasAura(GetSpellInfo()->Id))
                        l_Target->CastSpell(l_Target, HunterSpells::HUNTER_SPELL_INSANITY, true);
                }
            }

            void HandleImmunity(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(SPELL_SHAMAN_EXHAUSTED) || l_Target->HasAura(HUNTER_SPELL_INSANITY) ||
                    l_Target->HasAura(SPELL_SHAMAN_SATED) || l_Target->HasAura(SPELL_MAGE_TEMPORAL_DISPLACEMENT) ||
                    l_Target->HasAura(HUNTER_SPELL_FATIGUED))
                    PreventHitAura();
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_hun_ancient_hysteria_SpellScript::ApplyDebuff);
                OnEffectHitTarget += SpellEffectFn(spell_hun_ancient_hysteria_SpellScript::HandleImmunity, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_hun_ancient_hysteria_SpellScript::HandleImmunity, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
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

            enum eSpells
            {
                T17BeastMaster2P    = 165517,
                BestialWrath        = 19574
            };

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
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    /// Kill Command has a chance to reset the cooldown of Bestial Wrath.
                    if (AuraEffect* l_AuraEffect = l_Player->GetAuraEffect(eSpells::T17BeastMaster2P, EFFECT_0))
                    {
                        if (l_Player->HasSpellCooldown(eSpells::BestialWrath) && roll_chance_i(12))
                            l_Player->RemoveSpellCooldown(eSpells::BestialWrath, true);
                    }

                    if (Unit* l_Pet = GetCaster()->GetGuardianPet())
                    {
                        if (!l_Pet)
                            return;

                        if (!GetExplTargetUnit())
                            return;

                        l_Pet->CastSpell(GetExplTargetUnit(), HUNTER_SPELL_KILL_COMMAND_TRIGGER, true);

                        if (l_Pet->getVictim())
                        {
                            l_Pet->AttackStop();
                            l_Pet->ToCreature()->AI()->AttackStart(GetExplTargetUnit());
                        }
                        else
                            l_Pet->ToCreature()->AI()->AttackStart(GetExplTargetUnit());

                        l_Pet->CastSpell(GetExplTargetUnit(), HUNTER_SPELL_KILL_COMMAND_CHARGE, true);
                    }
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

/// last update : 6.2.3
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
                Unit* l_Caster = GetCaster();
                Unit* l_Owner = l_Caster->GetOwner();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Owner == nullptr)
                    return;

                int32 l_Damage = int32(l_Owner->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 1.632f);
                l_Damage *= l_Caster->GetModifierValue(UNIT_MOD_DAMAGE_MAINHAND, TOTAL_PCT);

                l_Damage = l_Caster->MeleeDamageBonusDone(l_Target, l_Damage, WeaponAttackType::BaseAttack, GetSpellInfo());
                l_Damage = l_Target->MeleeDamageBonusTaken(l_Caster, l_Damage, WeaponAttackType::BaseAttack, GetSpellInfo());

                if (l_Target->GetTypeId() == TYPEID_UNIT)
                    l_Damage *= l_Caster->CalculateDamageDealtFactor(l_Caster, l_Target->ToCreature());

                SetHitDamage(l_Damage);
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

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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
                        _player->RemoveMovementImpairingAuras();
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
                    else if ((l_Target->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_EXOTIC_PET) && !l_Player->CanTameExoticPets())
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
            EnhancedBasicAttacksProc = 157717,
            Invigoration             = 53253,
            InvigorationEffect       = 53398
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
                        Unit* l_Target = GetHitUnit();

                        if (l_Target == nullptr)
                            return;

                        int32 l_Damage = int32(l_Hunter->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.333f);

                        SpellInfo const* l_SpikedCollar = sSpellMgr->GetSpellInfo(HUNTER_SPELL_SPIKED_COLLAR);
                        SpellInfo const* l_EnhancedBasicAttacks = sSpellMgr->GetSpellInfo(eSpells::EnhancedBasicAttacksAura);
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(HUNTER_SPELL_BASIC_ATTACK_COST_MODIFIER);
                        SpellInfo const* l_Frenzy = sSpellMgr->GetSpellInfo(HUNTER_SPELL_FRENZY);

                        /// Increases the damage done by your pet's Basic Attacks by 10%
                        if (l_Pet->HasAura(HUNTER_SPELL_SPIKED_COLLAR) && l_SpikedCollar != nullptr)
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

                        /// Frenzy - 19623
                        if (l_Hunter->HasAura(HUNTER_SPELL_FRENZY) && roll_chance_i(l_Frenzy->Effects[EFFECT_1].BasePoints))
                            l_Pet->CastSpell(l_Pet, HUNTER_SPELL_FRENZY_STACKS, true);

                        l_Damage *= l_Pet->GetModifierValue(UNIT_MOD_DAMAGE_MAINHAND, TOTAL_PCT);

                        l_Damage = l_Pet->MeleeDamageBonusDone(l_Target, l_Damage, WeaponAttackType::BaseAttack, GetSpellInfo());
                        l_Damage = l_Target->MeleeDamageBonusTaken(l_Pet, l_Damage, WeaponAttackType::BaseAttack, GetSpellInfo());

                        if (l_Target->GetTypeId() == TYPEID_UNIT)
                            l_Damage *= l_Pet->CalculateDamageDealtFactor(l_Pet, l_Target->ToCreature());

                        SetHitDamage(l_Damage);

                        /// Invigoration - 53253
                        if (l_Hunter->HasAura(eSpells::Invigoration))
                        if (roll_chance_i(15))
                            l_Hunter->CastSpell(l_Hunter, eSpells::InvigorationEffect, true);
                    }
                }
            }

            void HandleAfterHit()
            {
                Pet* l_Pet = GetCaster()->ToPet();
                Unit* l_Hunter = GetCaster()->GetOwner();
                
                if (l_Hunter == nullptr || l_Pet == nullptr)
                    return;

                Player* l_Player = l_Hunter->ToPlayer();

                if (l_Player == nullptr)
                    return;

                SpellInfo const* l_EnhancedBasicAttacks = sSpellMgr->GetSpellInfo(eSpells::EnhancedBasicAttacksAura);

                if (Aura* l_CobraStrike = l_Hunter->GetAura(HUNTER_SPELL_COBRA_STRIKES_STACKS))
                    l_CobraStrike->ModStackAmount(-1);
                if (Aura* l_CobraStrikePet = l_Pet->GetAura(HUNTER_SPELL_COBRA_STRIKES_STACKS))
                    l_CobraStrikePet->ModStackAmount(-1);

                if (l_EnhancedBasicAttacks != nullptr && l_Hunter->HasAura(eSpells::EnhancedBasicAttacksAura) && roll_chance_i(l_EnhancedBasicAttacks->Effects[EFFECT_0].BasePoints))
                {
                    l_Pet->CastSpell(l_Pet, eSpells::EnhancedBasicAttacksProc, true);
                    l_Pet->_AddCreatureSpellCooldown(GetSpellInfo()->Id, time(nullptr));
                    l_Player->SendClearCooldown(GetSpellInfo()->Id, l_Pet);
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

            void CalculateAmount(AuraEffect const* l_AuraEffect, int32& l_Amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Owner = l_Caster->GetOwner())
                    {
                        if (l_AuraEffect->GetAmplitude() && GetMaxDuration())
                            l_Amount = int32(l_Owner->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.6f * 2.0f);
                    }
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
                Unit* l_Owner  = l_Caster->GetOwner();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Owner == nullptr)
                    return;

                int32 l_Heal = int32(l_Owner->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack) * 0.6f * 3.0f);
                l_Heal = l_Owner->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_Heal, l_Idx, SPELL_DIRECT_DAMAGE);
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

            void OnTick(AuraEffect const* /*aurEff*/)
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

enum ThrilloftheHunt
{
    VisualEffect1 = 170620,
    VisualEffect2 = 170621,
    VisualEffect3 = 170622
};

static uint32 const g_VisualSpells[3] =
{
    ThrilloftheHunt::VisualEffect3,
    ThrilloftheHunt::VisualEffect2,
    ThrilloftheHunt::VisualEffect1
};

/// last update : 6.1.2 19802
/// Thrill of the Hunt - 109396
class PlayerScript_thrill_of_the_hunt: public PlayerScript
{
    public:
        PlayerScript_thrill_of_the_hunt() :PlayerScript("PlayerScript_thrill_of_the_hunt") {}

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After)
        {
            if (p_After)
                return;

            if (p_Regen)
                return;

            // Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_diffValue = p_NewValue - p_OldValue;

            if (p_Player->getClass() == CLASS_HUNTER && p_Power == POWER_FOCUS && p_Player->HasAura(HUNTER_SPELL_THRILL_OF_THE_HUNT) && l_diffValue < 0)
            {
                for (int8 i = 0; i < ((l_diffValue / 10) * -1); ++i)
                {
                    if (roll_chance_i(sSpellMgr->GetSpellInfo(HUNTER_SPELL_THRILL_OF_THE_HUNT)->Effects[EFFECT_0].BasePoints))
                    {
                        p_Player->CastSpell(p_Player, HUNTER_SPELL_THRILL_OF_THE_HUNT_PROC, true);
                        for (uint8 l_I = 0; l_I < 3; ++l_I)
                            p_Player->CastSpell(p_Player, g_VisualSpells[l_I], true);
                        break;
                    }
                }
            }
        }
};

/// last update : 6.1.2 19802
/// Thrill of the Hunt - 34720
class spell_hun_thrill_of_the_hunt : public SpellScriptLoader
{
    public:
        spell_hun_thrill_of_the_hunt() : SpellScriptLoader("spell_hun_thrill_of_the_hunt") { }

        class spell_hun_exotic_munitions_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_exotic_munitions_AuraScript);

            uint8 m_ActualCharges = 3;

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                for (int8 l_I = 2; l_I >= 0; l_I--)
                    l_Caster->RemoveAura(g_VisualSpells[l_I]);
            }

            void OnUpdate(uint32, AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                for (; p_AurEff->GetBase()->GetCharges() < m_ActualCharges; m_ActualCharges--)
                {
                    for (int8 l_I = 2; l_I >= 0; l_I--)
                    {
                        if (l_Caster->HasAura(g_VisualSpells[l_I]))
                        {
                            l_Caster->RemoveAura(g_VisualSpells[l_I]);
                            break;
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_hun_exotic_munitions_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_exotic_munitions_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_exotic_munitions_AuraScript();
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

            void HandlePeriodicDamage(SpellEffIndex /*p_EffIndex*/)
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

/// last update : 6.2.3
/// Explosive Shot - 53301
class spell_hun_explosive_shot : public SpellScriptLoader
{
    public:
        spell_hun_explosive_shot() : SpellScriptLoader("spell_hun_explosive_shot") { }

        class spell_hun_explosive_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_explosive_shot_SpellScript);

            enum eSpells
            {
                T17Survival4P   = 165545,
                HeavyShot       = 167165
            };

            uint32 l_ReminingAmount = 0;

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_1, l_Caster->GetGUID()))
                {
                    if (l_AuraEffect->GetAmplitude() && l_AuraEffect->GetBase()->GetMaxDuration())
                        l_ReminingAmount = (l_AuraEffect->GetAmount() * (l_AuraEffect->GetBase()->GetDuration() / l_AuraEffect->GetAmplitude()) / (l_AuraEffect->GetBase()->GetMaxDuration() / l_AuraEffect->GetAmplitude()));
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                int32 l_Damage = int32(0.47f * l_Caster->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack));
                l_Damage = l_Caster->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                l_Damage = l_Target->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                SetHitDamage(l_Damage);
                if (l_Caster->HasAura(eSpells::T17Survival4P))
                    l_Caster->CastSpell(l_Caster, eSpells::HeavyShot, true);
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_1, l_Caster->GetGUID()))
                {
                    int32 l_Damage = int32(0.47f * l_Caster->GetTotalAttackPowerValue(WeaponAttackType::RangedAttack));
                    l_Damage = l_Caster->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_Damage, 0, DOT, l_AuraEffect->GetBase()->GetStackAmount());
                    l_Damage = l_Target->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, DOT, l_AuraEffect->GetBase()->GetStackAmount());

                    l_AuraEffect->SetAmount(l_Damage + l_ReminingAmount);
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_hun_explosive_shot_SpellScript::HandleBeforeHit);
                OnEffectHitTarget += SpellEffectFn(spell_hun_explosive_shot_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                AfterHit += SpellHitFn(spell_hun_explosive_shot_SpellScript::HandleAfterHit);
            }
        };

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
                    if (Aura* l_PoisonedAmmo = l_Target->GetAura(HUNTER_SPELL_POISONED_AMMO))
                    {
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
                if (Aura* l_PoisonedAmmo = l_Target->GetAura(HUNTER_SPELL_POISONED_AMMO))
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

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();
                 
                if (l_Caster == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (l_Pet->HasSpell(eSpells::CombatExperience))
                        {
                            l_Pet->RemoveAura(eSpells::CombatExperience);
                            l_Pet->CastSpell(l_Pet, eSpells::CombatExperienceAdaptation, true);
                        }
                    }
                }
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        l_Pet->RemoveAura(eSpells::CombatExperienceAdaptation);
                        if (l_Pet->HasSpell(eSpells::CombatExperience))
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

            enum eSpells
            {
                T17Marksmanship2P   = 165519,
                AimedShotEnergize   = 167153
            };

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

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                /// Aimed Shot critical strikes restore 8 additional Focus.
                if (Unit* l_Caster = GetCaster())
                {
                    if (GetSpell()->IsCritForTarget(GetHitUnit()) && l_Caster->HasAura(eSpells::T17Marksmanship2P))
                        l_Caster->CastSpell(l_Caster, eSpells::AimedShotEnergize, true);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_hun_aimed_shot_SpellScript::HandleAfterCast);
                OnEffectHitTarget += SpellEffectFn(spell_hun_aimed_shot_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_NORMALIZED_WEAPON_DMG);
            }
        };

        SpellScript* GetSpellScript() const override
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

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
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

/// Called by Rapid Fire - 3045
/// Item - Hunter T17 Marksmanship 4P Bonus - 165525
class spell_hun_t17_marksmanship_4p : public SpellScriptLoader
{
    public:
        spell_hun_t17_marksmanship_4p() : SpellScriptLoader("spell_hun_t17_marksmanship_4p") { }

        class spell_hun_t17_marksmanship_4p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_t17_marksmanship_4p_SpellScript);

            enum eSpells
            {
                T17Marksmanship4P   = 165525,
                DeadlyAimDriver     = 170186
            };

            void HandleOnHit()
            {
                /// While Rapid Fire is active, your critical strike damage increases by 3% per second.
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::T17Marksmanship4P))
                        l_Caster->CastSpell(l_Caster, eSpells::DeadlyAimDriver, true);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_hun_t17_marksmanship_4p_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_hun_t17_marksmanship_4p_SpellScript();
        }
};

/// Trap Launcher - 77769
class spell_hun_trap_launcher : public SpellScriptLoader
{
    public:
        spell_hun_trap_launcher() : SpellScriptLoader("spell_hun_trap_launcher") { }

        class spell_hun_trap_launcher_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_trap_launcher_AuraScript);

            enum eSpells
            {
                GLYPH_OF_SNAKE_TRAP = 159470
            };

            void CalculateAmount(AuraEffect const*, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (AuraEffect* l_GlyphOfSnakeTrapEff = l_Caster->GetAuraEffect(GLYPH_OF_SNAKE_TRAP, EFFECT_0))
                        l_GlyphOfSnakeTrapEff->ChangeAmount(p_Amount, true, true);
                }
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_GlyphOfSnakeTrap = sSpellMgr->GetSpellInfo(159470);
                    if (!l_GlyphOfSnakeTrap)
                        return;

                    int32 l_SpellId = l_GlyphOfSnakeTrap->Effects[EFFECT_0].BasePoints;
                    if (AuraEffect* l_GlyphOfSnakeTrapEff = l_Caster->GetAuraEffect(GLYPH_OF_SNAKE_TRAP, EFFECT_0))
                        l_GlyphOfSnakeTrapEff->ChangeAmount(l_SpellId, true, true);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_trap_launcher_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_trap_launcher_AuraScript::OnRemove, EFFECT_3, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hun_trap_launcher_AuraScript();
        }
};


/// last update : 6.2.3
/// Camouflage - 51753
class spell_hun_camouflage_triggered : public SpellScriptLoader
{
    public:
        spell_hun_camouflage_triggered() : SpellScriptLoader("spell_hun_camouflage_triggered") { }

        class spell_hun_camouflage_triggered_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_camouflage_triggered_AuraScript);

            enum eSpells
            {
                Camouflage = 51755
            };

            void OnApply(AuraEffect const* /*p_AuraEffect*/, AuraEffectHandleModes)
            {
                Unit* l_Target = GetTarget();

                l_Target->CastSpell(l_Target, eSpells::Camouflage, true);
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Target = GetTarget();

                if (l_Target->HasAura(eSpells::Camouflage))
                    l_Target->RemoveAura(eSpells::Camouflage);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_hun_camouflage_triggered_AuraScript::OnApply, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_triggered_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_hun_camouflage_triggered_AuraScript();
        }
};

/// last update : 6.2.3
/// Camouflage - 51755
class spell_hun_camouflage : public SpellScriptLoader
{
    public:
        spell_hun_camouflage() : SpellScriptLoader("spell_hun_camouflage") { }

        class spell_hun_camouflage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_camouflage_AuraScript);

            enum eSpells
            {
                CamouflageBuff  = 80326,
                HealthBuff      = 51753
            };

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->CastSpell(l_Player, eSpells::CamouflageBuff, true);

                if (l_Player->isInCombat())
                {
                    /// Should have 6s duration in Pvp
                    Aura* l_Aura = l_Player->GetAura(GetSpellInfo()->Id);
                    if (l_Aura != nullptr)
                        l_Aura->SetDuration(6 * IN_MILLISECONDS);
                }

                Pet* l_Pet = l_Player->GetPet();

                if (l_Pet != nullptr)
                {
                    l_Pet->CastSpell(l_Pet, GetSpellInfo()->Id, true);
                    l_Pet->CastSpell(l_Pet, eSpells::CamouflageBuff, true);
                    
                    /// Should have 6s duration in Pvp
                    Aura* l_Aura = l_Pet->GetAura(GetSpellInfo()->Id);
                    if (l_Aura != nullptr)
                        l_Aura->SetDuration(6 * IN_MILLISECONDS);
                }
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->RemoveAura(eSpells::CamouflageBuff);
                l_Player->RemoveAura(eSpells::HealthBuff);

                Pet* l_Pet = l_Player->GetPet();

                if (l_Pet)
                {
                    l_Player->RemoveAura(GetSpellInfo()->Id);
                    l_Player->RemoveAura(eSpells::CamouflageBuff);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_hun_camouflage_AuraScript::OnApply, EFFECT_2, SPELL_AURA_MOD_CAMOUFLAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_MOD_CAMOUFLAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_hun_camouflage_AuraScript();
        }
};

/// last update : 6.2.3
/// Camouflage - 80326
class spell_hun_camouflage_visual : public SpellScriptLoader
{
    public:
        spell_hun_camouflage_visual() : SpellScriptLoader("spell_hun_camouflage_visual") { }

        class spell_hun_camouflage_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_camouflage_visual_AuraScript);

            enum eSpells
            {
                CamouflageBuffVisual        = 80325,
                GlyphOfCamouflage           = 119449,
                GlyphOfCamouflageBuff       = 119450,
            };

           void OnApply(AuraEffect const*, AuraEffectHandleModes)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Pet* l_Pet = l_Player->GetPet();

                if (l_Player->HasAura(eSpells::GlyphOfCamouflage))
                {
                    if (l_Pet != nullptr)
                        l_Pet->CastSpell(l_Pet, eSpells::GlyphOfCamouflageBuff, true);
                    l_Player->CastSpell(l_Player, eSpells::GlyphOfCamouflageBuff, true);
                }
                else
                {
                    if (l_Pet != nullptr)
                        l_Pet->CastSpell(l_Pet, eSpells::CamouflageBuffVisual, true);
                    l_Player->CastSpell(l_Player, eSpells::CamouflageBuffVisual, true);
                }
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
               Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Pet* l_Pet = l_Player->GetPet();

                l_Player->RemoveAura(eSpells::GlyphOfCamouflageBuff);
                l_Player->RemoveAura(eSpells::CamouflageBuffVisual);
                if (l_Pet != nullptr)
                {
                    l_Pet->RemoveAura(GetSpellInfo()->Id);
                    l_Pet->RemoveAura(eSpells::GlyphOfCamouflageBuff);
                    l_Pet->RemoveAura(eSpells::CamouflageBuffVisual);
                }
            }

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Pet* l_Pet = l_Player->GetPet();

                if (!l_Player->IsMoving())
                {
                    if (l_Player->HasAura(eSpells::GlyphOfCamouflage))
                    {
                        if (l_Pet != nullptr)
                            l_Pet->CastSpell(l_Pet, eSpells::GlyphOfCamouflageBuff, true);
                        l_Player->CastSpell(l_Player, eSpells::GlyphOfCamouflageBuff, true);
                    }
                    else
                    {
                        if (l_Pet != nullptr)
                            l_Pet->CastSpell(l_Pet, eSpells::CamouflageBuffVisual, true);
                        l_Player->CastSpell(l_Player, eSpells::CamouflageBuffVisual, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_camouflage_visual_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectApply += AuraEffectApplyFn(spell_hun_camouflage_visual_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_visual_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_hun_camouflage_visual_AuraScript();
        }
};

/// Last Update 6.2.3
/// Focusing Shot - 152245
class spell_hun_focusing_shot : public SpellScriptLoader
{
    public:
        spell_hun_focusing_shot() : SpellScriptLoader("spell_hun_focusing_shot") { }

        class spell_hun_focusing_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_focusing_shot_SpellScript);

            enum eSpells
            {
                SteadyFocus     = 177667,
                SteadyFocusProc = 177668
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(eSpells::SteadyFocus))
                {
                    l_Player->CastSpell(l_Player, eSpells::SteadyFocusProc, true);

                    if (Pet* l_Pet = l_Player->GetPet())
                        l_Pet->CastSpell(l_Pet, eSpells::SteadyFocusProc, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_hun_focusing_shot_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hun_focusing_shot_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_hunter_spell_scripts()
{
    new spell_hun_focusing_shot();
    new spell_hun_camouflage_visual();
    new spell_hun_camouflage();
    new spell_hun_camouflage_triggered();
    new spell_hun_a_murder_of_crows_damage();
    new spell_hun_thrill_of_the_hunt();
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
    new spell_hun_serpent_spread();
    new spell_hun_ancient_hysteria();
    new spell_hun_netherwinds();
    new spell_hun_kill_command();
    new spell_hun_cobra_shot();
    new spell_hun_steady_shot();
    new spell_hun_chimaera_shot();
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
    new spell_hun_t17_marksmanship_4p();
    new spell_hun_trap_launcher();

    // Player Script
    new PlayerScript_thrill_of_the_hunt();
}
#endif
