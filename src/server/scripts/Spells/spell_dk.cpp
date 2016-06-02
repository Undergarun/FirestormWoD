////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"
#include "GridNotifiersImpl.h"

enum DeathKnightSpells
{
    DK_SPELL_RUNIC_POWER_ENERGIZE               = 49088,
    DK_SPELL_ANTI_MAGIC_SHELL_TALENT            = 51052,
    DK_SPELL_BLOOD_BOIL_TRIGGERED               = 65658,
    DK_SPELL_BLOOD_PLAGUE                       = 55078,
    DK_SPELL_FROST_FEVER                        = 55095,
    DK_SPELL_GHOUL_AS_GUARDIAN                  = 46585,
    DK_SPELL_GHOUL_AS_PET                       = 52150,
    DK_SPELL_BLOOD_BOIL                         = 50842,
    DK_SPELL_CHILBLAINS                         = 50041,
    DK_SPELL_PLAGUE_LEECH                       = 123693,
    DK_SPELL_PERDITION                          = 123981,
    DK_SPELL_SHROUD_OF_PURGATORY                = 116888,
    DK_SPELL_PURGATORY_INSTAKILL                = 123982,
    DK_SPELL_BLOOD_RITES                        = 50034,
    DK_SPELL_DEATH_SIPHON_HEAL                  = 116783,
    DK_SPELL_BLOOD_CHARGE                       = 114851,
    DK_SPELL_PILLAR_OF_FROST                    = 51271,
    DK_SPELL_SOUL_REAPER_HASTE                  = 114868,
    DK_SPELL_SOUL_REAPER_DAMAGE                 = 114867,
    DK_SPELL_REMORSELESS_WINTER_STUN            = 115001,
    DK_SPELL_REMORSELESS_WINTER                 = 115000,
    DK_SPELL_SCENT_OF_BLOOD                     = 49509,
    DK_SPELL_SCENT_OF_BLOOD_AURA                = 50421,
    DK_SPELL_CHAINS_OF_ICE                      = 45524,
    DK_SPELL_EBON_PLAGUEBRINGER                 = 51160,
    DK_SPELL_DESECRATED_GROUND                  = 118009,
    DK_SPELL_DESECRATED_GROUND_IMMUNE           = 115018,
    DK_SPELL_ASPHYXIATE                         = 108194,
    DK_SPELL_DARK_INFUSION_STACKS               = 91342,
    DK_SPELL_DARK_INFUSION_AURA                 = 93426,
    DK_NPC_WILD_MUSHROOM                        = 59172,
    DK_SPELL_GOREFIENDS_GRASP_GRIP_VISUAL       = 114869,
    DK_SPELL_DEATH_GRIP_ONLY_JUMP               = 146599,
    DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION          = 127344,
    DK_SPELL_GLYPH_OF_HORN_OF_WINTER            = 58680,
    DK_SPELL_GLYPH_OF_HORN_OF_WINTER_EFFECT     = 121920,
    DK_SPELL_DEATH_COIL_DAMAGE                  = 47632,
    DK_SPELL_GLYPH_OF_DEATH_AND_DECAY           = 58629,
    DK_SPELL_DEATH_AND_DECAY_DECREASE_SPEED     = 143375,
    DK_SPELL_DEATH_STRIKE_HEAL                  = 45470,
    DK_SPELL_PLAGUEBEARER                       = 161497,
    DK_SPELL_NECROTIC_PLAGUE                    = 152281,
    DK_SPELL_NECROTIC_PLAGUE_APPLY_AURA         = 155159,
    DK_SPELL_DEATH_PACT                         = 48743,
    DK_SPELL_ICY_TOUCH                          = 45477,
    DK_SPELL_CHILBLAINS_TRIGGER                 = 50435,
    DK_SPELL_REAPING                            = 56835,
    DK_SPELL_NECROTIC_PLAGUE_ENERGIZE           = 155165,
    DK_SPELL_EMPOWERED_OBLITERATE               = 157409,
    DK_SPELL_FREEZING_FOG_AURA                  = 59052,
    DK_SPELL_ENHANCED_DEATH_COIL                = 157343,
    DK_SPELL_SHADOW_OF_DEATH                    = 164047,
    DK_SPELL_DEATH_COIL                         = 47541,
    DK_WOD_PVP_UNHOLY_4P_BONUS                  = 166061,
    DK_WOD_PVP_UNHOLY_4P_BONUS_EFFECT           = 166062,
    DK_WOD_PVP_FROST_4P_BONUS                   = 166056,
    DK_WOD_PVP_FROST_4P_BONUS_EFFECT            = 166057
};

/// Glyph of Death and Decay - 58629
/// Call By Death and Decay 43265 & Defile 152280 (hot fix 6.0.3)
class spell_dk_glyph_of_death_and_decay: public SpellScriptLoader
{
    public:
        spell_dk_glyph_of_death_and_decay() : SpellScriptLoader("spell_dk_glyph_of_death_and_decay") { }

        class spell_dk_glyph_of_death_and_decay_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_glyph_of_death_and_decay_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(DeathKnightSpells::DK_SPELL_GLYPH_OF_DEATH_AND_DECAY))
                    {
                        if (WorldLocation const* dest = GetExplTargetDest())
                            l_Caster->CastSpell(*dest, DeathKnightSpells::DK_SPELL_DEATH_AND_DECAY_DECREASE_SPEED, true);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dk_glyph_of_death_and_decay_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_glyph_of_death_and_decay_SpellScript();
        }
};

// Death Barrier - 115635
class spell_dk_death_barrier: public SpellScriptLoader
{
    public:
        spell_dk_death_barrier() : SpellScriptLoader("spell_dk_death_barrier") { }

        class spell_dk_death_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_death_barrier_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                {
                    amount += int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 0.514f);
                    amount = int32(caster->SpellDamageBonusDone(GetUnitOwner(), sSpellMgr->GetSpellInfo(DK_SPELL_DEATH_COIL_DAMAGE), amount, aurEff->GetEffIndex(), SPELL_DIRECT_DAMAGE));
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_death_barrier_AuraScript();
        }
};

// Plague Strike - 45462
class spell_dk_plague_strike: public SpellScriptLoader
{
    public:
        spell_dk_plague_strike() : SpellScriptLoader("spell_dk_plague_strike") { }

        class spell_dk_plague_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_plague_strike_SpellScript);

            enum eSpells
            {
                chilblains = 50041,
                chilblainsAura = 50435
            };

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Target = GetHitUnit();
                Unit* l_Caster = GetCaster();

                if (l_Target == nullptr)
                    return;

                if (l_Caster->HasAura(DK_SPELL_EBON_PLAGUEBRINGER))
                {
                    l_Caster->CastSpell(l_Target, DK_SPELL_FROST_FEVER, true);

                    if (l_Caster->HasAura(eSpells::chilblains))
                        l_Caster->CastSpell(l_Target, eSpells::chilblainsAura, true);
                }
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), DK_SPELL_BLOOD_PLAGUE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_plague_strike_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_NORMALIZED_WEAPON_DMG);
                OnEffectHitTarget += SpellEffectFn(spell_dk_plague_strike_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_plague_strike_SpellScript();
        }
};

// Gorefiend's Grasp - 108199
class spell_dk_gorefiends_grasp: public SpellScriptLoader
{
    public:
        spell_dk_gorefiends_grasp() : SpellScriptLoader("spell_dk_gorefiends_grasp") { }

        class spell_dk_gorefiends_grasp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_gorefiends_grasp_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> tempList;
                        std::list<Unit*> gripList;

                        _player->GetAttackableUnitListInRange(tempList, 20.0f);

                        for (auto itr : tempList)
                        {
                            if (itr->GetGUID() == _player->GetGUID())
                                continue;

                            if (!_player->IsValidAttackTarget(itr))
                                continue;

                            if (itr->IsImmunedToSpell(GetSpellInfo()))
                                continue;

                            if (!itr->IsWithinLOSInMap(_player))
                                continue;

                            gripList.push_back(itr);
                        }

                        for (auto itr : gripList)
                        {
                            itr->CastSpell(target, DK_SPELL_DEATH_GRIP_ONLY_JUMP, true);
                            itr->CastSpell(target, DK_SPELL_GOREFIENDS_GRASP_GRIP_VISUAL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_gorefiends_grasp_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_gorefiends_grasp_SpellScript();
        }
};

enum DarkTransformationSpells
{
    DarkInfusionStacks              = 91342,
    DarkTransformationAuraDummy     = 93426
};

// Dark transformation - transform pet spell - 63560
class spell_dk_dark_transformation_form: public SpellScriptLoader
{
    public:
        spell_dk_dark_transformation_form() : SpellScriptLoader("spell_dk_dark_transformation_form") { }

        class spell_dk_dark_transformation_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_dark_transformation_form_SpellScript);

            enum eSpell
            {
                T17Unholy4P = 165574
            };

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Pet = GetHitUnit())
                    {
                        if (l_Pet->HasAura(DarkTransformationSpells::DarkInfusionStacks))
                        {
                            l_Player->RemoveAura(DarkTransformationSpells::DarkInfusionStacks);
                            l_Player->RemoveAura(DarkTransformationSpells::DarkTransformationAuraDummy);
                            l_Pet->RemoveAura(DarkTransformationSpells::DarkInfusionStacks);
                        }

                        /// When you activate Dark Transformation, your Shadow damage dealt is increased  by 20% for 15 sec.
                        if (l_Player->HasAura(DK_WOD_PVP_UNHOLY_4P_BONUS))
                            l_Player->CastSpell(l_Player, DK_WOD_PVP_UNHOLY_4P_BONUS_EFFECT, true);

                        if (l_Player->HasAura(eSpell::T17Unholy4P))
                        {
                            std::list<uint8> l_LstRunesUsed;

                            for (uint8 i = 0; i < MAX_RUNES; ++i)
                            {
                                if (l_Player->GetRuneCooldown(i))
                                    l_LstRunesUsed.push_back(i);
                            }

                            if (l_LstRunesUsed.empty())
                                return;

                            uint8 l_RuneRandom = JadeCore::Containers::SelectRandomContainerElement(l_LstRunesUsed);

                            l_Player->SetRuneCooldown(l_RuneRandom, 0);
                            l_Player->ConvertRune(l_RuneRandom, RUNE_DEATH);
                            l_Player->ResyncRunes(MAX_RUNES);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_dark_transformation_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_dark_transformation_form_SpellScript();
        }
};

// Desecrated ground - 118009
class spell_dk_desecrated_ground: public SpellScriptLoader
{
    public:
        spell_dk_desecrated_ground() : SpellScriptLoader("spell_dk_desecrated_ground") { }

        class spell_dk_desecrated_ground_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_desecrated_ground_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (GetCaster())
                    if (DynamicObject* dynObj = GetCaster()->GetDynObject(DK_SPELL_DESECRATED_GROUND))
                        if (GetCaster()->GetDistance(dynObj) <= 8.0f)
                            GetCaster()->CastSpell(GetCaster(), DK_SPELL_DESECRATED_GROUND_IMMUNE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_desecrated_ground_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_desecrated_ground_AuraScript();
        }
};

/// Festering Strike - 85948
class spell_dk_festering_strike: public SpellScriptLoader
{
    public:
        spell_dk_festering_strike() : SpellScriptLoader("spell_dk_festering_strike") { }

        class spell_dk_festering_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_festering_strike_SpellScript);

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();
                int32 l_extraDuration = GetSpellInfo()->Effects[EFFECT_2].BasePoints * IN_MILLISECONDS;

                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                if (Aura* l_AuraBloodPlague = l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Player->GetGUID()))
                {
                    uint32 l_Dur = std::min(l_AuraBloodPlague->GetDuration() + l_extraDuration, 2 * MINUTE * IN_MILLISECONDS);
                    l_AuraBloodPlague->SetDuration(l_Dur);

                    if (l_Dur > uint32(l_AuraBloodPlague->GetMaxDuration()))
                        l_AuraBloodPlague->SetMaxDuration(l_Dur);
                }

                if (Aura* l_AuraFrostFever = l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Player->GetGUID()))
                {
                    uint32 l_Dur = std::min(l_AuraFrostFever->GetDuration() + l_extraDuration, 2 * MINUTE * IN_MILLISECONDS);
                    l_AuraFrostFever->SetDuration(l_Dur);

                    if (l_Dur > uint32(l_AuraFrostFever->GetMaxDuration()))
                        l_AuraFrostFever->SetMaxDuration(l_Dur);
                }

                if (Aura* l_NecroticPlague = l_Target->GetAura(DK_SPELL_NECROTIC_PLAGUE_APPLY_AURA, l_Player->GetGUID()))
                {
                    uint32 l_Dur = std::min(l_NecroticPlague->GetDuration() + l_extraDuration, 2 * MINUTE * IN_MILLISECONDS);
                    l_NecroticPlague->SetDuration(l_Dur);

                    if (l_Dur > uint32(l_NecroticPlague->GetMaxDuration()))
                        l_NecroticPlague->SetMaxDuration(l_Dur);
                }

                if (Aura* l_AuraChainsOfIce = l_Target->GetAura(DK_SPELL_CHAINS_OF_ICE, l_Player->GetGUID()))
                {
                    uint32 l_Dur = std::min(l_AuraChainsOfIce->GetDuration() + l_extraDuration, 20 * IN_MILLISECONDS);
                    l_AuraChainsOfIce->SetDuration(l_Dur);

                    if (l_Dur > uint32(l_AuraChainsOfIce->GetMaxDuration()))
                        l_AuraChainsOfIce->SetMaxDuration(l_Dur);
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_festering_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_festering_strike_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Death Strike heal - 45470
class spell_dk_death_strike_heal: public SpellScriptLoader
{
    public:
        spell_dk_death_strike_heal() : SpellScriptLoader("spell_dk_death_strike_heal") { }

        class spell_dk_death_strike_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_strike_heal_SpellScript);

            enum eSpells
            {
                ScentOfBloodAura = 50421
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::ScentOfBloodAura))
                    l_Caster->RemoveAura(eSpells::ScentOfBloodAura);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_death_strike_heal_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_strike_heal_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Howling Blast - 49184
class spell_dk_howling_blast: public SpellScriptLoader
{
    public:
        spell_dk_howling_blast() : SpellScriptLoader("spell_dk_howling_blast") { }

        class spell_dk_howling_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_howling_blast_SpellScript);

            enum eSpells
            {
                chilblains = 50041,
                chilblainsAura = 50435
            };

            bool m_HasAuraFrog = false;

            void HandleBeforeCast()
            {
                Unit* l_Target = GetExplTargetUnit();
                Unit* l_Caster = GetCaster();

                if (!l_Target)
                    return;

                if (l_Caster->HasAura(DK_SPELL_FREEZING_FOG_AURA))
                    m_HasAuraFrog = true;
            }

            void HandleOnHit()
            {
                Unit* l_Target = GetHitUnit();
                Unit* l_Caster = GetCaster();

                if (!l_Target)
                    return;

                l_Caster->CastSpell(l_Target, DK_SPELL_FROST_FEVER, true);

                if (l_Caster->HasAura(eSpells::chilblains))
                    l_Caster->CastSpell(l_Target, eSpells::chilblainsAura, true);

                /// When you have Freezing Fog active, your next Howling Blast will increase Frost damage taken on all targets by 10% for 8 sec.
                if (m_HasAuraFrog && l_Caster->HasAura(DK_WOD_PVP_FROST_4P_BONUS))
                    l_Caster->CastSpell(l_Target, DK_WOD_PVP_FROST_4P_BONUS_EFFECT, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dk_howling_blast_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dk_howling_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_howling_blast_SpellScript();
        }
};

// Conversion - 119975
class spell_dk_conversion: public SpellScriptLoader
{
    public:
        spell_dk_conversion() : SpellScriptLoader("spell_dk_conversion") { }

        class spell_dk_conversion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_conversion_AuraScript);

            enum eSpells
            {
                ConversionRegen = 119980
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                int32 l_CurrentPower = l_Caster->GetPower(POWER_RUNIC_POWER) / l_Caster->GetPowerCoeff(POWER_RUNIC_POWER);

                if (l_CurrentPower < p_AurEff->GetAmount())
                    l_Caster->RemoveAura(GetSpellInfo()->Id);

                l_Caster->CastSpell(l_Caster, eSpells::ConversionRegen, true);
                l_Caster->EnergizeBySpell(l_Caster, eSpells::ConversionRegen, -(p_AurEff->GetAmount() * l_Caster->GetPowerCoeff(POWER_RUNIC_POWER)), POWER_RUNIC_POWER);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_conversion_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_conversion_AuraScript();
        }
};

// Remorseless Winter - 115000
class spell_dk_remorseless_winter: public SpellScriptLoader
{
    public:
        spell_dk_remorseless_winter() : SpellScriptLoader("spell_dk_remorseless_winter") { }

        class spell_dk_remorseless_winter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_remorseless_winter_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (Aura* remorselessWinter = target->GetAura(DK_SPELL_REMORSELESS_WINTER))
                            if (remorselessWinter->GetStackAmount() == 5 && !target->HasAura(DK_SPELL_REMORSELESS_WINTER_STUN))
                                _player->CastSpell(target, DK_SPELL_REMORSELESS_WINTER_STUN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_remorseless_winter_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_remorseless_winter_SpellScript();
        }
};

// Soul Reaper - 130736 (unholy) or 130735 (frost) or 114866 (blood)
class spell_dk_soul_reaper: public SpellScriptLoader
{
    public:
        spell_dk_soul_reaper() : SpellScriptLoader("spell_dk_soul_reaper") { }

        class spell_dk_soul_reaper_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_soul_reaper_AuraScript);

            uint8 l_HealthPctMax = 35;

            enum eSpells
            {
                ImprovedSoulReaper  = 157342,
                T17Unholy2P         = 165575,
                DarkTransformation  = 93426
            };

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (Aura* l_ImprovedSoulReaper = l_Caster->GetAura(eSpells::ImprovedSoulReaper))
                    l_HealthPctMax = l_ImprovedSoulReaper->GetEffect(EFFECT_0)->GetAmount();
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();

                if (removeMode == AURA_REMOVE_BY_DEATH)
                    l_Caster->CastSpell(l_Caster, DK_SPELL_SOUL_REAPER_HASTE, true);
                else if (removeMode == AURA_REMOVE_BY_EXPIRE && GetTarget()->GetHealthPct() < (float)l_HealthPctMax)
                {
                    l_Caster->CastSpell(GetTarget(), DK_SPELL_SOUL_REAPER_DAMAGE, true);

                    /// When Soul Reaper deals its bonus damage to a target, you gain 5 stacks of Shadow Infusion.
                    if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::T17Unholy2P, EFFECT_0))
                    {
                        if (Player* l_Player = l_Caster->ToPlayer())
                        {
                            if (Pet* l_Pet = l_Player->GetPet())
                            {
                                for (uint8 l_I = 0; l_I < (uint8)l_AurEff->GetAmount(); ++l_I)
                                {
                                    l_Caster->CastSpell(l_Pet, DK_SPELL_DARK_INFUSION_STACKS, true);
                                }
                                if (Aura* l_Aura = l_Pet->GetAura(DK_SPELL_DARK_INFUSION_STACKS))
                                {
                                    if (l_Aura->GetStackAmount() > 4) /// Apply Dark Transformation
                                        l_Player->CastSpell(l_Player, eSpells::DarkTransformation, true);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_dk_soul_reaper_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_soul_reaper_AuraScript();
        }

        class spell_dk_soul_reaper_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_soul_reaper_SpellScript);

            enum eSpells
            {
                T18Blood2P = 187872
            };

            void HandleAfterHit()
            {
                if (!GetCaster())
                    return;

                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    /// Only in blood spec
                    if (l_Caster->GetSpecializationId(l_Caster->GetActiveSpec()) == SPEC_DK_BLOOD)
                    {
                        l_Caster->CastSpell(l_Caster, DK_SPELL_SCENT_OF_BLOOD_AURA, true);

                        if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::T18Blood2P, EFFECT_1))
                        {
                            if (roll_chance_i(l_AurEff->GetAmount()))
                                l_Caster->CastSpell(l_Caster, DK_SPELL_SCENT_OF_BLOOD_AURA, true);
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_dk_soul_reaper_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_soul_reaper_SpellScript();
        }
};

// Pillar of Frost - 51271
class spell_dk_pillar_of_frost: public SpellScriptLoader
{
    public:
        spell_dk_pillar_of_frost() : SpellScriptLoader("spell_dk_pillar_of_frost") { }

        class spell_dk_pillar_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_pillar_of_frost_AuraScript);

            enum eSpells
            {
                T17Frost2P              = 165547,
                PillarOfFrostEnergize   = 167171,
                T17Frost4P              = 165568,
                T17Frost4PDriver        = 167655,
                T17Frost4PDriverPeriod  = 170205
            };

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Target->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_KNOCKOUT, false);

                    /// When Pillar of Frost fades, your rune weapon discharges the souls at your current target, dealing 30% damage per soul.
                    if (l_Target->HasAura(eSpells::T17Frost4PDriver))
                    {
                        l_Target->RemoveAura(eSpells::T17Frost4PDriver);
                        l_Target->CastSpell(l_Target, eSpells::T17Frost4PDriverPeriod, true);
                    }
                }
            }

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Target->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_KNOCKOUT, true);

                    /// Activating Pillar of Frost generates 30 Runic Power.
                    if (l_Target->HasAura(eSpells::T17Frost2P))
                        l_Target->CastSpell(l_Target, eSpells::PillarOfFrostEnergize, true);

                    /// While Pillar of Frost is active, your special attacks trap a soul in your rune weapon.
                    if (l_Target->HasAura(eSpells::T17Frost4P))
                        l_Target->CastSpell(l_Target, eSpells::T17Frost4PDriver, true);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dk_pillar_of_frost_AuraScript::AfterApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dk_pillar_of_frost_AuraScript::AfterRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_pillar_of_frost_AuraScript();
        }
};

// Blood Tap - 45529
class PlayerScript_Blood_Tap: public PlayerScript
{
    public:
        PlayerScript_Blood_Tap() :PlayerScript("PlayerScript_Blood_Tap") {}

        uint16 m_RunicPower = 0;

        void OnModifyPower(Player * p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After)
        {
            if (p_After)
                return;

            if (p_Player->getClass() != CLASS_DEATH_KNIGHT || p_Power != POWER_RUNIC_POWER || !p_Player->HasSpell(45529) || p_Regen)
                return;

            // Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_diffValue = p_NewValue - p_OldValue;

            // Only on use runic power
            if (l_diffValue > 0)
                return;

            m_RunicPower += -l_diffValue;

            if (m_RunicPower >= 150)
            {
                uint8 l_Stack = m_RunicPower / 150;

                for (uint8 l_I = 0; l_I < l_Stack; ++l_I)
                {
                    p_Player->CastSpell(p_Player, DK_SPELL_BLOOD_CHARGE, true);
                    m_RunicPower -= 150;
                }
            }
        }
};

// Blood Tap - 45529
class spell_dk_blood_tap: public SpellScriptLoader
{
    public:
        spell_dk_blood_tap() : SpellScriptLoader("spell_dk_blood_tap") { }

        class spell_dk_blood_tap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_tap_SpellScript);

            SpellCastResult CheckBloodCharges()
            {
                if (GetCaster()->ToPlayer())
                {
                    if (Aura* bloodCharges = GetCaster()->ToPlayer()->GetAura(DK_SPELL_BLOOD_CHARGE))
                    {
                        if (bloodCharges->GetStackAmount() < 5)
                            return SPELL_FAILED_DONT_REPORT;
                    }
                    else
                        return SPELL_FAILED_DONT_REPORT;

                    Player* l_Player = GetCaster()->ToPlayer();
                    if (l_Player == nullptr)
                        return SPELL_FAILED_DONT_REPORT;

                    uint8 l_Counter = 0;
                    RuneType l_RuneOnCooldown = RuneType::NUM_RUNE_TYPES;
                    l_Player->SetCurrentRuneForBloodTap(l_RuneOnCooldown);

                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                    {
                        if (!l_Player->GetRuneCooldown(i))
                            continue;

                        /// First rune on cooldown, save it
                        if (l_RuneOnCooldown == RuneType::NUM_RUNE_TYPES)
                            l_RuneOnCooldown = l_Player->GetCurrentRune(i);

                        if (l_Player->GetCurrentRune(i) != l_RuneOnCooldown || !l_Player->GetRuneCooldown(i))
                        {
                            l_RuneOnCooldown = l_Player->GetCurrentRune(i);
                            continue;
                        }

                        l_Counter++;

                        /// If we have already found 2 spent runes - we can use Blood Tap
                        if (l_Counter == 2)
                        {
                            l_Player->SetCurrentRuneForBloodTap(l_RuneOnCooldown);
                            break;
                        }
                    }

                    if (l_Counter < 2)
                        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                }

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* bloodCharges = _player->GetAura(DK_SPELL_BLOOD_CHARGE))
                        {
                            int32 newAmount = bloodCharges->GetStackAmount();

                            if ((newAmount - 5) <= 0)
                                _player->RemoveAura(DK_SPELL_BLOOD_CHARGE);
                            else
                                bloodCharges->SetStackAmount(newAmount - 5);
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_blood_tap_SpellScript::CheckBloodCharges);
                OnHit += SpellHitFn(spell_dk_blood_tap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_tap_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Death Siphon - 108196
class spell_dk_death_siphon: public SpellScriptLoader
{
    public:
        spell_dk_death_siphon() : SpellScriptLoader("spell_dk_death_siphon") { }

        class spell_dk_death_siphon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_siphon_SpellScript);

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                int8 l_DeathRune = -1;

                for (uint8 i = 0; i < MAX_RUNES; ++i)
                {
                    if (l_Player->GetRuneCooldown(i) && l_Player->GetCurrentRune(i) == RUNE_DEATH)
                    {
                        l_DeathRune = i;
                        break;
                    }
                }

                if (l_DeathRune == -1)
                    return;

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SPEC_DK_FROST)
                    l_Player->RestoreBaseRune(uint8(l_DeathRune));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_siphon_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_siphon_SpellScript();
        }
};

// Death Strike - 49998
class spell_dk_death_strike: public SpellScriptLoader
{
    public:
        spell_dk_death_strike() : SpellScriptLoader("spell_dk_death_strike") { }

        class spell_dk_death_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_strike_SpellScript);

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                l_Player->CastSpell(l_Player, DK_SPELL_DEATH_STRIKE_HEAL, true);

                // Apply Blood Rites effects
                if (l_Player->HasAura(DK_SPELL_BLOOD_RITES))
                {
                    bool l_RuneFrost = false;
                    bool l_RuneUnholy = false;

                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                    {
                        if (l_Player->GetCurrentRune(i) == RUNE_DEATH
                            || l_Player->GetCurrentRune(i) == RUNE_BLOOD
                            || l_Player->GetBaseRune(i) == RUNE_BLOOD)
                            continue;

                        if (l_RuneUnholy && l_Player->GetCurrentRune(i) == RUNE_UNHOLY)
                            continue;

                        if (l_RuneFrost && l_Player->GetCurrentRune(i) == RUNE_FROST)
                            continue;

                        if (l_Player->GetRuneCooldown(i))
                        {
                            if (l_Player->GetCurrentRune(i) == RUNE_FROST)
                                l_RuneFrost = true;
                            else
                                l_RuneUnholy = true;

                            l_Player->ConvertRune(i, RUNE_DEATH);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_death_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_strike_SpellScript();
        }
};

// Purgatory - 116888
class spell_dk_purgatory: public SpellScriptLoader
{
    public:
        spell_dk_purgatory() : SpellScriptLoader("spell_dk_purgatory") { }

        class spell_dk_purgatory_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE)
                        _player->CastSpell(_player, DK_SPELL_PURGATORY_INSTAKILL, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_purgatory_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_AuraScript();
        }
};

// Purgatory - 114556
class spell_dk_purgatory_absorb: public SpellScriptLoader
{
    public:
        spell_dk_purgatory_absorb() : SpellScriptLoader("spell_dk_purgatory_absorb") { }

        class spell_dk_purgatory_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_absorb_AuraScript);

            void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                // No damage received under Shroud of Purgatory
                if (target->ToPlayer()->HasAura(DK_SPELL_SHROUD_OF_PURGATORY))
                {
                    absorbAmount = dmgInfo.GetDamage();
                    return;
                }

                if (target->ToPlayer()->HasAura(DK_SPELL_PERDITION))
                    return;

                int32 bp = dmgInfo.GetDamage();

                target->CastCustomSpell(target, DK_SPELL_SHROUD_OF_PURGATORY, &bp, NULL, NULL, true);
                target->CastSpell(target, DK_SPELL_PERDITION, true);
                target->SetHealth(1);
                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_purgatory_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_purgatory_absorb_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_absorb_AuraScript();
        }
};

// Plague Leech - 123693
class spell_dk_plague_leech: public SpellScriptLoader
{
    public:
        spell_dk_plague_leech() : SpellScriptLoader("spell_dk_plague_leech") { }

        class spell_dk_plague_leech_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_plague_leech_SpellScript);

            enum eSpells
            {
                NecroticPlague = 152281,
                NecroticPlagueAura = 155159,
                BloodPlague = 55078,
                FrostEver = 55095
            };

            std::list<uint8> m_LstRunesUsed;

            SpellCastResult CheckRunes()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetExplTargetUnit();

                if (l_Player == nullptr || l_Target == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                for (uint8 i = 0; i < MAX_RUNES; ++i)
                {
                    if (l_Player->GetRuneCooldown(i))
                        m_LstRunesUsed.push_back(i);
                }

                if (m_LstRunesUsed.size() < 2)
                    return SPELL_FAILED_DONT_REPORT;

                if (l_Player->HasAura(eSpells::NecroticPlague) && !l_Target->HasAura(eSpells::NecroticPlagueAura, l_Player->GetGUID()))
                    return SPELL_FAILED_DONT_REPORT;
                else if (!l_Player->HasAura(eSpells::NecroticPlague) && (!l_Target->HasAura(eSpells::BloodPlague, l_Player->GetGUID()) || !l_Target->HasAura(eSpells::FrostEver, l_Player->GetGUID())))
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                if (l_Target->HasAura(eSpells::NecroticPlagueAura, l_Player->GetGUID()))
                    l_Target->RemoveAura(eSpells::NecroticPlagueAura, l_Player->GetGUID());
                else
                {
                    l_Target->RemoveAura(eSpells::FrostEver, l_Player->GetGUID());
                    l_Target->RemoveAura(eSpells::BloodPlague, l_Player->GetGUID());
                }

                for (uint8 l_I = 0; l_I < 2; l_I++)
                {
                    uint8 l_RuneRandom = JadeCore::Containers::SelectRandomContainerElement(m_LstRunesUsed);

                    if (l_Player->GetRuneCooldown(l_RuneRandom))
                    {
                        l_Player->SetRuneCooldown(l_RuneRandom, 0);
                        l_Player->ConvertRune(l_RuneRandom, RUNE_DEATH);
                    }

                    m_LstRunesUsed.remove(l_RuneRandom);
                }
                l_Player->ResyncRunes(MAX_RUNES);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_plague_leech_SpellScript::CheckRunes);
                OnHit += SpellHitFn(spell_dk_plague_leech_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_plague_leech_SpellScript();
        }
};

// Unholy Blight - 115994
class spell_dk_unholy_blight: public SpellScriptLoader
{
    public:
        spell_dk_unholy_blight() : SpellScriptLoader("spell_dk_unholy_blight") { }

        class spell_dk_unholy_blight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_unholy_blight_SpellScript);

            enum eSpells
            {
                chilblains = 50041,
                chilblainsAura = 50435
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, DK_SPELL_BLOOD_PLAGUE, true);
                l_Caster->CastSpell(l_Target, DK_SPELL_FROST_FEVER, true);

                if (l_Caster->HasAura(eSpells::chilblains))
                    l_Caster->CastSpell(l_Target, eSpells::chilblainsAura, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_unholy_blight_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_unholy_blight_SpellScript();
        }
};

/// Outbreak - 77575
class spell_dk_outbreak: public SpellScriptLoader
{
    public:
        spell_dk_outbreak() : SpellScriptLoader("spell_dk_outbreak") { }

        class spell_dk_outbreak_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_outbreak_SpellScript);

            enum eSpells
            {
                chilbrains = 50041,
                chilbrainsAura = 50435
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, DK_SPELL_BLOOD_PLAGUE, true);
                l_Caster->CastSpell(l_Target, DK_SPELL_FROST_FEVER, true);

                if (l_Caster->HasAura(eSpells::chilbrains))
                    l_Caster->CastSpell(l_Target, eSpells::chilbrainsAura, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_outbreak_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_outbreak_SpellScript();
        }
};

// Raise Dead - 46584
class spell_dk_raise_dead: public SpellScriptLoader
{
    public:
        spell_dk_raise_dead() : SpellScriptLoader("spell_dk_raise_dead") { }

        class spell_dk_raise_dead_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_raise_dead_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_DK_UNHOLY)
                        l_Player->CastSpell(l_Player, DK_SPELL_GHOUL_AS_PET, true);
                    else
                        l_Player->CastSpell(l_Player, DK_SPELL_GHOUL_AS_GUARDIAN, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_raise_dead_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_raise_dead_SpellScript();
        }
};

// 50462 - Anti-Magic Shell (on raid member)
class spell_dk_anti_magic_shell_raid: public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_raid() : SpellScriptLoader("spell_dk_anti_magic_shell_raid") { }

        class spell_dk_anti_magic_shell_raid_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_raid_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // TODO: this should absorb limited amount of damage, but no info on calculation formula
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_raid_AuraScript();
        }
};

/// Last Update 6.2.3
/// Anti-Magic Shell (on self) - 48707
class spell_dk_anti_magic_shell_self: public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_self() : SpellScriptLoader("spell_dk_anti_magic_shell_self") { }

        class spell_dk_anti_magic_shell_self_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_self_AuraScript);

            int32 m_AbsorbPct, m_HpPct = 0;
            uint32 m_Absorbed, m_AmountAbsorb = 0;

            enum eSpells
            {
                AntiMagicShell           = 48707,
                GlyphOfRegenerativeMagic = 146648,
                WoDPvPBlood4PBonus       = 171456
            };

            bool Load()
            {
                m_Absorbed = 0;
                m_AbsorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                m_HpPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
                return true;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_RUNIC_POWER_ENERGIZE))
                    return false;
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (GetCaster())
                {
                    amount = GetCaster()->CountPctFromMaxHealth(m_HpPct);
                    m_AmountAbsorb = amount;
                }
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& /*absorbAmount*/)
            {
                m_Absorbed += dmgInfo.GetDamage();
            }

            void Trigger(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
            {
                Unit* target = GetTarget();
                /// damage absorbed by Anti-Magic Shell energizes the DK with additional runic power.
                /// 1% of lost HP restore DK 2 runic power
                uint32 l_MaxHealth = target->GetMaxHealth();
                uint32 l_AbsorbAmount = absorbAmount;
                float l_Percent = (float)l_AbsorbAmount / (float)l_MaxHealth * 200.0f;
                int32 bp = (int32)(l_Percent * 10);
                target->EnergizeBySpell(target, DK_SPELL_RUNIC_POWER_ENERGIZE, bp, POWER_RUNIC_POWER);
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (!l_Caster->HasAura(eSpells::WoDPvPBlood4PBonus))
                    return;

                if (l_Caster->GetGUID() != l_Target->GetGUID())
                    return;

                std::list<Unit*> l_TargetList;
                float l_Radius = 30.0f;

                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Ucheck(l_Target, l_Target, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Target, l_TargetList, l_Ucheck);
                l_Target->VisitNearbyObject(l_Radius, l_Searcher);

                l_TargetList.remove_if([this, l_Caster, l_Target](Unit* p_Unit) -> bool
                {
                    if (p_Unit == nullptr)
                        return true;

                    if (!l_Caster->IsValidAssistTarget(p_Unit))
                        return true;

                    if (l_Caster->GetGUID() == p_Unit->GetGUID())
                        return true;

                    return false;
                });

                l_TargetList.sort(JadeCore::WorldObjectDistanceCompareOrderPred(l_Caster));

                if (l_TargetList.size() > 2)
                l_TargetList.resize(2);

                for (auto l_Itr : l_TargetList)
                    l_Itr->CastCustomSpell(l_Itr, GetSpellInfo()->Id, 0, NULL, NULL, true, NULL, nullptr, l_Caster->GetGUID());
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode != AURA_REMOVE_BY_EXPIRE)
                    return;

                Unit* l_Caster = GetCaster();
                if (!l_Caster || m_AmountAbsorb == 0)
                    return;

                if (Aura* l_Aura = l_Caster->GetAura(eSpells::GlyphOfRegenerativeMagic))
                {
                    SpellInfo const * l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::AntiMagicShell);
                    if (l_SpellInfo == nullptr)
                        return;

                    float l_RemainingPct = 0.0f;

                    if (m_Absorbed > m_AmountAbsorb)
                        m_Absorbed = m_AmountAbsorb;

                    float l_AbsorbedPct = 100.0f - (m_Absorbed / (m_AmountAbsorb / 100));  ///< Absorbed damage in pct
                    int32 l_Amount = l_Aura->GetEffect(EFFECT_0)->GetAmount();  ///< Maximum absorbed damage is 50%

                    l_RemainingPct = (float)CalculatePct(l_Amount, l_AbsorbedPct);

                    if (l_RemainingPct > l_Aura->GetEffect(EFFECT_0)->GetAmount())
                        l_RemainingPct = (float)l_Aura->GetEffect(EFFECT_0)->GetAmount();

                    int32 l_ReduceTime = int32((l_SpellInfo->GetSpellCooldowns()->CategoryRecoveryTime / 100) * l_RemainingPct) - p_AurEff->GetBase()->GetDuration();

                    if (!l_ReduceTime)
                        return;

                    if (Player* l_Player = l_Caster->ToPlayer())
                        l_Player->ReduceSpellCooldown(eSpells::AntiMagicShell, l_ReduceTime);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_self_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectApply += AuraEffectApplyFn(spell_dk_anti_magic_shell_self_AuraScript::OnApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Trigger, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_anti_magic_shell_self_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_self_AuraScript();
        }
};

// 50461 - Anti-Magic Zone
class spell_dk_anti_magic_zone: public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_zone() : SpellScriptLoader("spell_dk_anti_magic_zone") { }

        class spell_dk_anti_magic_zone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_zone_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                amount = 136800;
                if (Player* player = GetCaster()->ToPlayer())
                     amount += int32(player->GetStat(STAT_STRENGTH) * 4);
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_zone_AuraScript();
        }
};

// Death Gate - 53822
class spell_dk_death_gate_teleport: public SpellScriptLoader
{
    public:
        spell_dk_death_gate_teleport() : SpellScriptLoader("spell_dk_death_gate_teleport") {}

        class spell_dk_death_gate_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_teleport_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }
                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->TeleportTo(0, 2359.41f, -5662.084f, 382.259f, 0.60f);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_teleport_SpellScript::CheckClass);
                AfterCast += SpellCastFn(spell_dk_death_gate_teleport_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_teleport_SpellScript();
        }
};

// Death Gate - 52751
class spell_dk_death_gate: public SpellScriptLoader
{
    public:
        spell_dk_death_gate() : SpellScriptLoader("spell_dk_death_gate") {}

        class spell_dk_death_gate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, GetEffectValue(), false);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_SpellScript::CheckClass);
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_gate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_SpellScript();
        }
};

/// Blood Boil - 50842
class spell_dk_blood_boil: public SpellScriptLoader
{
    public:
        spell_dk_blood_boil() : SpellScriptLoader("spell_dk_blood_boil") { }

        class spell_dk_blood_boil_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_boil_SpellScript);

            int32 m_FrostFever  = 0;
            int32 m_BloodPlague = 0;

            enum eSpells
            {
                T18Blood2P = 187872
            };

            SpellCastResult CheckTarget()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return SPELL_FAILED_SUCCESS;

                return SPELL_CAST_OK;
            }

            void HandleBeforeCast()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Unit* l_Target = l_Player->GetSelectedUnit();

                if (l_Target == nullptr)
                    return;

                if (Aura* l_AuraBloodPlague = l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Player->GetGUID()))
                    m_BloodPlague = l_AuraBloodPlague->GetDuration();

                if (Aura* l_AuraFrostFever = l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Player->GetGUID()))
                    m_FrostFever = l_AuraFrostFever->GetDuration();
            }

            void HandleHitTarget(SpellEffIndex /* effIndex */)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if ((!l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID()) || l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID())->GetDuration() < m_FrostFever) && m_FrostFever > 0)
                {
                    l_Caster->CastSpell(l_Target, DK_SPELL_FROST_FEVER, true);
                    if (Aura* l_AuraFrostFever = l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID()))
                        l_AuraFrostFever->SetDuration(m_FrostFever);
                }

                // Blood plague
                if ((!l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID()) || l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID())->GetDuration() < m_BloodPlague) && m_BloodPlague > 0)
                {
                    l_Caster->CastSpell(l_Target, DK_SPELL_BLOOD_PLAGUE, true);
                    if (Aura* l_AuraBloodPlague = l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID()))
                        l_AuraBloodPlague->SetDuration(m_BloodPlague);
                }

                /// In blood spec we should update duration of DOTs
                if (l_Caster->HasAura(DK_SPELL_SCENT_OF_BLOOD))
                {
                    if (Aura* l_AuraFrostFever = l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID()))
                        l_AuraFrostFever->SetDuration(l_AuraFrostFever->GetMaxDuration());

                    if (Aura* l_AuraBloodPlague = l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID()))
                        l_AuraBloodPlague->SetDuration(l_AuraBloodPlague->GetMaxDuration());
                }
            }

            void HandleAfterCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(DK_SPELL_SCENT_OF_BLOOD))
                    {
                        l_Player->CastSpell(l_Player, DK_SPELL_SCENT_OF_BLOOD_AURA, true);

                        if (AuraEffect* l_AurEff = l_Player->GetAuraEffect(eSpells::T18Blood2P, EFFECT_1))
                        {
                            if (roll_chance_i(l_AurEff->GetAmount()))
                                l_Player->CastSpell(l_Player, DK_SPELL_SCENT_OF_BLOOD_AURA, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_blood_boil_SpellScript::CheckTarget);
                BeforeCast += SpellCastFn(spell_dk_blood_boil_SpellScript::HandleBeforeCast);
                OnEffectHitTarget += SpellEffectFn(spell_dk_blood_boil_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                AfterCast += SpellCastFn(spell_dk_blood_boil_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_boil_SpellScript();
        }
};

// Reaping - 56835
class spell_dk_reaping: public SpellScriptLoader
{
    public:
        spell_dk_reaping() : SpellScriptLoader("spell_dk_reaping") { }

        class spell_dk_reaping_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_reaping_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                    l_Player->RemoveRunesBySpell(DK_SPELL_REAPING);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_reaping_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_reaping_AuraScript();
        }
};

enum DeathGripSpells
{
    SpellImprovedDeathGrip = 157367
};

/// Death Grip - 49560
class spell_dk_death_grip: public SpellScriptLoader
{
    public:
        spell_dk_death_grip() : SpellScriptLoader("spell_dk_death_grip") { }

        class spell_dk_death_grip_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_grip_SpellScript);

            enum ImprovedDeathGrip
            {
                SpellId = 157367,
                ChainsOfIce = 45524
            };

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Target = GetHitUnit();
                Unit* l_Caster = GetCaster();

                int32 l_SpellTrigerID = GetEffectValue();
                Position const* l_Pos = GetExplTargetDest();

                if (l_Target == nullptr)
                    return;

                if (!l_Target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS)) ///< Deterrence
                    l_Target->CastSpell(l_Caster, l_SpellTrigerID, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_grip_SpellScript();
        }
};

// Corpse Explosion (Glyph) - 127344
class spell_dk_corpse_explosion: public SpellScriptLoader
{
    public:
        spell_dk_corpse_explosion() : SpellScriptLoader("spell_dk_corpse_explosion") { }

        class spell_dk_corpse_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_corpse_explosion_SpellScript);

            SpellCastResult CheckTarget()
            {
                // Any effect on Mechanical or Elemental units
                if (Unit* caster = GetCaster())
                {
                    Unit* target = GetExplTargetUnit();
                    if (!target)
                        return SPELL_FAILED_NO_VALID_TARGETS;

                    if (Creature* c = target->ToCreature())
                    {
                        if (c->GetCreatureTemplate() && (c->GetCreatureTemplate()->type == CREATURE_TYPE_MECHANICAL ||
                                                         c->GetCreatureTemplate()->type == CREATURE_TYPE_ELEMENTAL))
                            return SPELL_FAILED_BAD_TARGETS;
                        else if (c->IsDungeonBoss())
                            return SPELL_FAILED_BAD_TARGETS;
                    }
                    else if (target->GetGUID() == caster->GetGUID())
                        return SPELL_FAILED_BAD_TARGETS;

                    if (target->isAlive())
                        return SPELL_FAILED_BAD_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_corpse_explosion_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_corpse_explosion_SpellScript();
        }
};

// Glyph of Corpse Explosion - 59336
class spell_dk_glyph_of_corpse_explosion: public SpellScriptLoader
{
    public:
        spell_dk_glyph_of_corpse_explosion() : SpellScriptLoader("spell_dk_glyph_of_corpse_explosion") { }

        class spell_dk_glyph_of_corpse_explosion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_glyph_of_corpse_explosion_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION))
                        _player->removeSpell(DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_glyph_of_corpse_explosion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_glyph_of_corpse_explosion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_glyph_of_corpse_explosion_AuraScript();
        }
};

// Glyph of Horn of Winter - 58680
// Called by Horn of Winter - 57330
class spell_dk_glyph_of_horn_of_winter: public SpellScriptLoader
{
    public:
        spell_dk_glyph_of_horn_of_winter() : SpellScriptLoader("spell_dk_glyph_of_horn_of_winter") { }

        class spell_dk_glyph_of_horn_of_winter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_glyph_of_horn_of_winter_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (!_player->isInCombat() && _player->HasAura(DK_SPELL_GLYPH_OF_HORN_OF_WINTER))
                        _player->CastSpell(_player, DK_SPELL_GLYPH_OF_HORN_OF_WINTER_EFFECT, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dk_glyph_of_horn_of_winter_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_glyph_of_horn_of_winter_SpellScript();
        }
};

/// Icy touch - 45477
class spell_dk_icy_touch: public SpellScriptLoader
{
    public:
        spell_dk_icy_touch() : SpellScriptLoader("spell_dk_icy_touch") { }

        class spell_dk_icy_touch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_icy_touch_SpellScript);

            enum eSpells
            {
                chilblains = 50041,
                chilblainsAura = 50435
            };

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                GetCaster()->CastSpell(l_Target, DK_SPELL_FROST_FEVER, true);

                if (l_Caster->HasAura(eSpells::chilblains))
                    l_Caster->CastSpell(l_Target, eSpells::chilblainsAura, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_icy_touch_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_icy_touch_SpellScript();
        }
};

// Empowered Obliterate - 157409
// Called by Icy Touch - 45477
class spell_dk_empowered_obliterate_icy_touch : public SpellScriptLoader
{
    public:
        spell_dk_empowered_obliterate_icy_touch() : SpellScriptLoader("spell_dk_empowered_obliterate_icy_touch") { }

        class spell_dk_empowered_obliterate_icy_touch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_empowered_obliterate_icy_touch_SpellScript);

            bool m_HasAura = false;

            void HandleOnPrepare()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(DK_SPELL_EMPOWERED_OBLITERATE) && l_Caster->HasAura(DK_SPELL_FREEZING_FOG_AURA))
                        m_HasAura = true;
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                SpellInfo const * l_SpellInfo = sSpellMgr->GetSpellInfo(DK_SPELL_EMPOWERED_OBLITERATE);
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (m_HasAura && l_SpellInfo != nullptr)
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_SpellInfo->Effects[EFFECT_0].BasePoints));
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_dk_empowered_obliterate_icy_touch_SpellScript::HandleOnPrepare);
                OnEffectHitTarget += SpellEffectFn(spell_dk_empowered_obliterate_icy_touch_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_empowered_obliterate_icy_touch_SpellScript();
        }
};

/// Empowered Obliterate - 157409
/// Howling Blast - 49184
class spell_dk_empowered_obliterate_howling_blast : public SpellScriptLoader
{
    public:
        spell_dk_empowered_obliterate_howling_blast() : SpellScriptLoader("spell_dk_empowered_obliterate_howling_blast") { }

        class spell_dk_empowered_obliterate_howling_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_empowered_obliterate_howling_blast_SpellScript);

            bool m_HasAura = false;

            void HandleOnPrepare()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(DK_SPELL_EMPOWERED_OBLITERATE) && l_Caster->HasAura(DK_SPELL_FREEZING_FOG_AURA))
                        m_HasAura = true;
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                SpellInfo const * l_SpellInfo = sSpellMgr->GetSpellInfo(DK_SPELL_EMPOWERED_OBLITERATE);

                if (m_HasAura && l_SpellInfo != nullptr)
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_SpellInfo->Effects[EFFECT_0].BasePoints));
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_dk_empowered_obliterate_howling_blast_SpellScript::HandleOnPrepare);
                OnEffectHitTarget += SpellEffectFn(spell_dk_empowered_obliterate_howling_blast_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_empowered_obliterate_howling_blast_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Necrotic Plague - 155159
class spell_dk_necrotic_plague_aura: public SpellScriptLoader
{
    public:
        spell_dk_necrotic_plague_aura() : SpellScriptLoader("spell_dk_necrotic_plague_aura") { }

        class spell_dk_necrotic_plague_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_necrotic_plague_aura_AuraScript);

            enum eSpells
            {
                NecroticPlagueAura = 155159,
                NecroticPlagueEnergize = 155165
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Target == nullptr || l_Caster == nullptr)
                    return;

                int32 l_CurrentDuration = 0;
                uint8 l_CurrentStacks = 0;

                if (Aura* l_AuraNecroticPlague = l_Target->GetAura(NecroticPlagueAura, l_Caster->GetGUID()))
                {
                    if (l_AuraNecroticPlague->GetEffect(EFFECT_0))
                    {
                        uint32 l_LeftDuration = l_AuraNecroticPlague->GetDuration();
                        uint32 l_MaxDuration = l_AuraNecroticPlague->GetMaxDuration();
                        uint32 l_Amplitude = l_AuraNecroticPlague->GetEffect(EFFECT_0)->GetAmplitude();
                        int8 l_TicksLeft = int8(l_LeftDuration / l_Amplitude);
                        int8 l_MaxTicks = int8(l_MaxDuration / l_Amplitude);
                        /// Shouldn't add 1 tick on the first tick, just after second
                        if (l_MaxTicks - l_TicksLeft != 2)
                            l_AuraNecroticPlague->ModStackAmount(1);
                    }

                    l_CurrentDuration = l_AuraNecroticPlague->GetDuration();
                    l_CurrentStacks = l_AuraNecroticPlague->GetStackAmount();
                }

                std::list<Unit*> l_TargetList;
                float l_Radius = 8.0f;

                /// Friendly target for the target (your target) not for you
                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Ucheck(l_Target, l_Target, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Target, l_TargetList, l_Ucheck);
                l_Target->VisitNearbyObject(l_Radius, l_Searcher);

                l_TargetList.remove_if([this, l_Caster, l_Target](Unit* p_Unit) -> bool
                {
                    if (p_Unit == nullptr)
                        return true;

                    if (!l_Caster->IsValidAttackTarget(p_Unit))
                        return true;

                    if (l_Target->GetGUID() == p_Unit->GetGUID())
                        return true;

                    return false;
                });

                if (l_TargetList.empty())
                    return;

                if (Unit* l_NewTarget = JadeCore::Containers::SelectRandomContainerElement(l_TargetList))
                {
                    l_Caster->CastSpell(l_NewTarget, NecroticPlagueAura, true);

                    /// Copy aura data
                    if (Aura* l_NewNecroticPlague = l_NewTarget->GetAura(NecroticPlagueAura, l_Caster->GetGUID()))
                    {
                        l_NewNecroticPlague->SetDuration(l_CurrentDuration);
                        l_NewNecroticPlague->SetMaxDuration(l_CurrentDuration);
                        l_NewNecroticPlague->SetStackAmount(l_CurrentStacks);
                    }
                }
            }

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                Unit* l_Target = p_EventInfo.GetActionTarget();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                if (l_Caster->GetGUID() != l_Target->GetGUID())
                    return;

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_DK_BLOOD)
                    l_Caster->CastSpell(l_Caster, NecroticPlagueEnergize, true);
            }

            bool CanRefreshProcDummy()
            {
                return false;
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_necrotic_plague_aura_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectProc += AuraEffectProcFn(spell_dk_necrotic_plague_aura_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                CanRefreshProc += AuraCanRefreshProcFn(spell_dk_necrotic_plague_aura_AuraScript::CanRefreshProcDummy);
            }
        };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_necrotic_plague_aura_AuraScript();
    }
};

/// Runic Empowerment - 81229
class spell_dk_runic_empowerment : public PlayerScript
{
    public:
        spell_dk_runic_empowerment() : PlayerScript("spell_dk_runic_empowerment") {}

        enum eSpells
        {
           RunicEmpowerment = 81229,
        };

        void OnModifyPower(Player * p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After)
        {
            if (p_After)
                return;

            if (p_Player->getClass() != CLASS_DEATH_KNIGHT || p_Power != POWER_RUNIC_POWER || p_Regen || p_NewValue > p_OldValue)
                return;

            int32 l_PowerSpent = p_OldValue - p_NewValue;

            if (AuraEffect* l_RunicEmpowerment = p_Player->GetAuraEffect(eSpells::RunicEmpowerment, EFFECT_0))
            {
                /// 1.50% chance per Runic Power spent
                float l_Chance = (l_RunicEmpowerment->GetAmount() / 100.0f) * (l_PowerSpent / p_Player->GetPowerCoeff(p_Power));

                if (roll_chance_f(l_Chance))
                {
                    std::list<uint8> l_LstRunesUsed;

                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                    {
                        if (p_Player->GetRuneCooldown(i))
                            l_LstRunesUsed.push_back(i);
                    }

                    if (l_LstRunesUsed.empty())
                        return;

                    uint8 l_RuneRandom = JadeCore::Containers::SelectRandomContainerElement(l_LstRunesUsed);

                    p_Player->SetRuneCooldown(l_RuneRandom, 0);
                    p_Player->ResyncRunes(MAX_RUNES);
                }
            }
        }
};

/// Runic Corruption - 51462
class spell_dk_runic_corruption : public PlayerScript
{
    public:
        spell_dk_runic_corruption() : PlayerScript("spell_dk_runic_corruption") {}

        enum eSpells
        {
            RunicCorruption     = 51460,
            RunicCorruptionAura = 51462,
        };

        void OnModifyPower(Player * p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After)
        {
            if (p_After)
                return;

            if (p_Player->getClass() != CLASS_DEATH_KNIGHT || p_Power != POWER_RUNIC_POWER || p_Regen || p_NewValue > p_OldValue)
                return;

            int32 l_PowerSpent = p_OldValue - p_NewValue;

            if (AuraEffect* l_RunicCorruption = p_Player->GetAuraEffect(eSpells::RunicCorruptionAura, EFFECT_1))
            {
                /// 1.50% chance per Runic Power spent
                float l_Chance = (l_RunicCorruption->GetAmount() / 100.0f) * (l_PowerSpent / p_Player->GetPowerCoeff(p_Power));

                if (roll_chance_f(l_Chance))
                {
                    if (Aura* l_AuraRunicCorruption = p_Player->GetAura(eSpells::RunicCorruption))
                        l_AuraRunicCorruption->SetDuration(l_AuraRunicCorruption->GetDuration() + l_AuraRunicCorruption->GetMaxDuration());
                    else
                        p_Player->CastSpell(p_Player, eSpells::RunicCorruption, true);
                }
            }
        }
};

// Death Pact - 48743
class spell_dk_death_pact: public SpellScriptLoader
{
    public:
        spell_dk_death_pact() : SpellScriptLoader("spell_dk_death_pact") { }

        class spell_dk_death_pact_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_death_pact_AuraScript);

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                    p_Amount = l_Caster->CountPctFromMaxHealth(p_Amount);
            }

            void OnAbsorb(AuraEffect* /*p_AurEff*/, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (p_DmgInfo.GetSpellInfo() && p_DmgInfo.GetSpellInfo()->Id == GetSpellInfo()->Id)
                        p_AbsorbAmount = 0;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_pact_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_HEAL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_death_pact_AuraScript::OnAbsorb, EFFECT_1, SPELL_AURA_SCHOOL_HEAL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_death_pact_AuraScript();
        }
};

/// Last Update 6.2.3
/// Chains of Ice - 45524
class spell_dk_chain_of_ice: public SpellScriptLoader
{
    public:
        spell_dk_chain_of_ice() : SpellScriptLoader("spell_dk_chain_of_ice") { }

        class spell_dk_chain_of_ice_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_chain_of_ice_SpellScript);

            enum eSpells
            {
                ChainOfIceRoot = 96294,
                chilblains = 50041,
                chilblainsAura = 50435,
                GlyphoftheIceReaper = 159416
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Player == nullptr)
                    return;

                if (l_Player->HasAura(eSpells::chilblains))
                    l_Player->CastSpell(l_Target, eSpells::chilblainsAura, true);

                if (l_Player->HasAura(DK_SPELL_CHILBLAINS))
                    l_Player->CastSpell(l_Target, eSpells::ChainOfIceRoot, true);

                if (l_Player->HasAura(eSpells::GlyphoftheIceReaper))
                {
                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                    {
                        if (l_Player->GetCurrentRune(i) == RUNE_DEATH
                            || l_Player->GetCurrentRune(i) == RUNE_BLOOD
                            || l_Player->GetCurrentRune(i) == RUNE_UNHOLY)
                            continue;

                        if (l_Player->GetRuneCooldown(i))
                        {
                            if (l_Player->GetCurrentRune(i) == RUNE_FROST)
                                l_Player->ConvertRune(i, RUNE_DEATH);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_chain_of_ice_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_chain_of_ice_SpellScript();
        }
};

/// Chilblains - 50041
class spell_dk_chilblains_aura : public SpellScriptLoader
{
    public:
        spell_dk_chilblains_aura() : SpellScriptLoader("spell_dk_chilblains_aura") { }

        class spell_dk_chilblains_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_chilblains_aura_AuraScript);

            enum eSpell
            {
                HowlingBlast = 49184
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
            {
                PreventDefaultAction();
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_chilblains_aura_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_chilblains_aura_AuraScript();
        }
};

/// Will of the Necropolis - 81164
class spell_dk_will_of_the_necropolis : public SpellScriptLoader
{
    public:
        spell_dk_will_of_the_necropolis() : SpellScriptLoader("spell_dk_will_of_the_necropolis") { }

        class spell_dk_will_of_the_necropolis_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_will_of_the_necropolis_AuraScript);

            enum eSpell
            {
                TimerAura   = 157335, ///< 30s
                RuneTap     = 171049
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpell::TimerAura))
                    return;

                if (!l_Caster->HealthBelowPctDamaged(GetSpellInfo()->Effects[EFFECT_0].BasePoints, p_EventInfo.GetDamageInfo()->GetDamage()))
                    return;

                /// This effect cannot occur more than once every 30 sec.
                l_Caster->CastSpell(l_Caster, eSpell::TimerAura, true);

                l_Caster->CastSpell(l_Caster, eSpell::RuneTap, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_will_of_the_necropolis_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_will_of_the_necropolis_AuraScript();
        }
};

/// Dark Succor - 178819
class spell_dk_dark_succor : public SpellScriptLoader
{
    public:
        spell_dk_dark_succor() : SpellScriptLoader("spell_dk_dark_succor") { }

        class spell_dk_dark_succor_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_dark_succor_AuraScript);

            enum eSpells
            {
                UnholyPresence = 48265,
                FrostPresence  = 48266
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                   if (l_Caster->HasAura(FrostPresence) || l_Caster->HasAura(UnholyPresence))
                        l_Caster->CastSpell(l_Caster, 101568, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_dark_succor_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_dark_succor_AuraScript();
        }
};

/// Mark of Sindragosa - 178819
class spell_dk_mark_of_sindragosa : public SpellScriptLoader
{
    public:
        spell_dk_mark_of_sindragosa() : SpellScriptLoader("spell_dk_mark_of_sindragosa") { }

        class spell_dk_mark_of_sindragosa_AuraScript : public AuraScript
        {

            enum BloodBathSpells
            {
                SPELL_MARK_OF_SINDRAGOSA_HEAL = 155168
            };

            PrepareAuraScript(spell_dk_mark_of_sindragosa_AuraScript);

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!p_EventInfo.GetDamageInfo() || !p_EventInfo.GetDamageInfo()->GetDamage())
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    int32 l_Damage = CalculatePct(p_EventInfo.GetDamageInfo()->GetDamage(), p_AurEff->GetAmount());
                    l_Caster->CastCustomSpell(l_Caster, SPELL_MARK_OF_SINDRAGOSA_HEAL, &l_Damage, nullptr, nullptr, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_mark_of_sindragosa_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_mark_of_sindragosa_AuraScript();
        }
};

enum DeathCoilSpells
{
    SpellGlyphOfDeathsEmbrace = 58679
};

/// 58677 - Glyph of Death's Embrace
class spell_dk_glyph_of_deaths_embrace : public SpellScriptLoader
{
    public:
        spell_dk_glyph_of_deaths_embrace() : SpellScriptLoader("spell_dk_glyph_of_deaths_embrace") { }

        class spell_dk_glyph_of_deaths_embrace_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_glyph_of_deaths_embrace_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                Unit* l_Target = p_EventInfo.GetDamageInfo()->GetVictim();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;


                if (l_Target->IsHostileTo(l_Caster))
                    return;


                if (l_Target->GetOwner() && l_Target->isGuardian() && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == DK_SPELL_DEATH_COIL)
                    l_Caster->CastSpell(l_Caster, DeathCoilSpells::SpellGlyphOfDeathsEmbrace, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_glyph_of_deaths_embrace_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_glyph_of_deaths_embrace_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Death Coil - 47541
class spell_dk_death_coil : public SpellScriptLoader
{
    public:
        spell_dk_death_coil() : SpellScriptLoader("spell_dk_death_coil") { }

        class spell_dk_death_coil_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_coil_SpellScript);

            enum eSpells
            {
                GlyphofDeathCoil    = 63333,
                Lichborne            = 49039,
                DeathBarrier        = 115635,
                DeathCoilAlly       = 47633,
                DeathCoilEnemy      = 47632
            };

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Caster->IsFriendlyTo(l_Target))
                {
                    /// Glyph of Death Coil
                    if (l_Caster->HasAura(eSpells::GlyphofDeathCoil) && l_Target->GetCreatureType() != CREATURE_TYPE_UNDEAD && !l_Target->HasAura(eSpells::Lichborne))
                        l_Caster->CastSpell(l_Target, eSpells::DeathBarrier, true); ///< Death Barrier
                    else
                    {
                        int32 l_Healing = int32(l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 0.80f * 5.0f);
                        l_Caster->CastCustomSpell(l_Target, eSpells::DeathCoilAlly, &l_Healing, NULL, NULL, true);
                    }
                }
                else
                {
                    int32 l_Damage = int32(l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 0.80f);
                    l_Caster->CastCustomSpell(l_Target, eSpells::DeathCoilEnemy, &l_Damage, NULL, NULL, true);
                }
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(DK_SPELL_ENHANCED_DEATH_COIL))
                    {
                        l_Caster->CastSpell(l_Caster, DK_SPELL_SHADOW_OF_DEATH, true);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_coil_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnHit += SpellHitFn(spell_dk_death_coil_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_coil_SpellScript();
        }
};

/// Enhanced Death Coil (aura) - 164047
class spell_dk_enhanced_death_coil : public SpellScriptLoader
{
    public:
        spell_dk_enhanced_death_coil() : SpellScriptLoader("spell_dk_enhanced_death_coil") { }

        class spell_dk_enhanced_death_coil_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_enhanced_death_coil_AuraScript);

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
                    l_Caster->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, (float)p_AurEff->GetAmount(), false);
                    l_Caster->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, (float)l_Stack->GetTotalAmount(), true);
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
                OnEffectApply += AuraEffectApplyFn(spell_dk_enhanced_death_coil_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY));
                OnEffectUpdate += AuraEffectUpdateFn(spell_dk_enhanced_death_coil_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dk_enhanced_death_coil_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL));
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_enhanced_death_coil_AuraScript();
        }
};

enum SkeletonSpells
{
    SpellSkeletonForm = 147157
};

enum GeistSpells
{
    SpellGeistForm = 121916
};

/// Called on removing Glyph of the Geist - 58640
class spell_dk_glyph_of_the_geist : public SpellScriptLoader
{
    public:
        spell_dk_glyph_of_the_geist() : SpellScriptLoader("spell_dk_glyph_of_the_geist") { }

        class spell_dk_glyph_of_the_geist_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_glyph_of_the_geist_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (!l_Pet->HasAura(SkeletonSpells::SpellSkeletonForm))
                            l_Pet->CastSpell(l_Pet, GeistSpells::SpellGeistForm, true);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (l_Pet->HasAura(GeistSpells::SpellGeistForm))
                            l_Pet->RemoveAura(GeistSpells::SpellGeistForm);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_glyph_of_the_geist_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_glyph_of_the_geist_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_glyph_of_the_geist_AuraScript();
        }
};

/// Called on removing Glyph of the Skeleton - 146652
class spell_dk_glyph_of_the_skeleton : public SpellScriptLoader
{
    public:
        spell_dk_glyph_of_the_skeleton() : SpellScriptLoader("spell_dk_glyph_of_the_skeleton") { }

        class spell_dk_glyph_of_the_skeleton_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_glyph_of_the_skeleton_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (!l_Pet->HasAura(GeistSpells::SpellGeistForm))
                            l_Pet->CastSpell(l_Pet, SkeletonSpells::SpellSkeletonForm, true);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                {
                    if (Pet* l_Pet = l_Player->GetPet())
                    {
                        if (l_Pet->HasAura(SkeletonSpells::SpellSkeletonForm))
                            l_Pet->RemoveAura(SkeletonSpells::SpellSkeletonForm);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_glyph_of_the_skeleton_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_dk_glyph_of_the_skeleton_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_glyph_of_the_skeleton_AuraScript();
        }
};

/// Army Transform - 127517
class spell_dk_army_transform : public SpellScriptLoader
{
    public:
        spell_dk_army_transform() : SpellScriptLoader("spell_dk_army_transform") { }

        class spell_dk_army_transform_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_army_transform_SpellScript);

            enum eArmyTransformSpells
            {
                GlyphOfFullMenagerie        = 58642,
                TransformFleahBeast         = 127533,
                TransformGeist              = 127534,
                TransformNothrendSkeleton   = 127528,
                TransformSkeleton           = 127527,
                TransformSpikedGhoul        = 127525,
                TransformSuperZombie        = 127526
            };

            bool Load() override
            {
                return GetCaster()->isGuardian();
            }

            SpellCastResult CheckCast()
            {
                Unit* l_Owner = GetCaster()->GetOwner();

                if (l_Owner == nullptr)
                    return SPELL_FAILED_SPELL_UNAVAILABLE;

                if (l_Owner->HasAura(eArmyTransformSpells::GlyphOfFullMenagerie))
                    return SPELL_CAST_OK;

                return SPELL_FAILED_SPELL_UNAVAILABLE;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                static uint32 const ArmyTransforms[6] = { TransformFleahBeast, TransformGeist, TransformNothrendSkeleton, TransformSkeleton, TransformSpikedGhoul, TransformSuperZombie };
                Unit *l_Caster = GetCaster();

                l_Caster->CastSpell(l_Caster, ArmyTransforms[urand(0, 5)], true);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_army_transform_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dk_army_transform_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dk_army_transform_SpellScript();
        }
};


/// Gargoyle Strike - 51963
class spell_dk_gargoyle_strike : public SpellScriptLoader
{
public:
    spell_dk_gargoyle_strike() : SpellScriptLoader("spell_dk_gargoyle_strike") { }

    class spell_dk_gargoyle_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_gargoyle_strike_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Owner = l_Caster->GetOwner())
                {
                    /// Gargoyle Strike damage is increased by DK mastery
                    int32 l_HitDamage = GetHitDamage();
                    float l_Mastery = l_Owner->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.5f;

                    l_HitDamage += CalculatePct(l_HitDamage, l_Mastery);

                    SetHitDamage(l_HitDamage);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_gargoyle_strike_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_gargoyle_strike_SpellScript();
    }
};

/// last update : 6.1.2 19802
/// Blood rites - 163952
class spell_dk_blood_rites : public SpellScriptLoader
{
    public:
        spell_dk_blood_rites() : SpellScriptLoader("spell_dk_blood_rites") { }

        class spell_dk_blood_rites_Aurascript : public AuraScript
        {
            PrepareAuraScript(spell_dk_blood_rites_Aurascript);

            enum eSpells
            {
                BloodRitesEnergize = 163948
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_ProcInfos)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (p_ProcInfos.GetDamageInfo()->GetSpellInfo() != nullptr)
                    return;

                if (!(p_ProcInfos.GetHitMask() & PROC_EX_INTERNAL_MULTISTRIKE))
                    return;

                Item* l_MainItem = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                Item* l_OffHandItem = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                if ((l_MainItem && l_MainItem->GetTemplate()->IsTwoHandedWeapon()) || (l_OffHandItem && l_OffHandItem->GetTemplate()->IsTwoHandedWeapon()))
                {
                    l_Caster->CastSpell(l_Caster, eSpells::BloodRitesEnergize, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_blood_rites_Aurascript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_blood_rites_Aurascript();
        }
};

/// Control Undead - 111673
class spell_dk_control_undead : public SpellScriptLoader
{
    public:
        spell_dk_control_undead() : SpellScriptLoader("spell_dk_control_undead") {}

        class spell_dk_control_undead_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_control_undead_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* l_Target = GetExplTargetUnit();

                if (l_Target == nullptr)
                    return SPELL_FAILED_SUCCESS;

                if (l_Target->IsPlayer())
                    return SPELL_FAILED_BAD_TARGETS;

                if (Unit* l_Owner = l_Target->GetOwner())
                {
                    if (l_Owner->IsPlayer())
                        return SPELL_FAILED_BAD_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_control_undead_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_control_undead_SpellScript();
        }
};

/// DK Presences
/// Called by Blood Presence (48263), Unholy Presence (48265), Frost Presence (48266)
class spell_dk_presences : public SpellScriptLoader
{
    public:
        spell_dk_presences() : SpellScriptLoader("spell_dk_presences") { }

        class spell_dk_presences_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_presences_AuraScript);

            enum eSpells
            {
                ImprovedBloodPresence  = 50371,
                ImprovedFrostPresence  = 50385,
                ImprovedUnholyPresence = 50392
            };

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (GetCaster() == nullptr)
                    return;

                Player* l_Target = GetCaster()->ToPlayer();
                if (!l_Target)
                    return;

                if (l_Target->GetSpecializationId() == SPEC_DK_BLOOD && !l_Target->HasAura(ImprovedBloodPresence))
                    l_Target->CastSpell(l_Target, ImprovedBloodPresence, true, nullptr, p_AurEff);
                if (l_Target->GetSpecializationId() == SPEC_DK_UNHOLY && !l_Target->HasAura(ImprovedUnholyPresence))
                    l_Target->CastSpell(l_Target, ImprovedUnholyPresence, true, nullptr, p_AurEff);
                if (l_Target->GetSpecializationId() == SPEC_DK_FROST && !l_Target->HasAura(ImprovedFrostPresence))
                    l_Target->CastSpell(l_Target, ImprovedFrostPresence, true, nullptr, p_AurEff);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_presences_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_presences_AuraScript();
        }
};

/// Breath of Sindragosa - 152279
class spell_dk_breath_of_sindragosa : public SpellScriptLoader
{
public:
    spell_dk_breath_of_sindragosa() : SpellScriptLoader("spell_dk_breath_of_sindragosa") { }

    class spell_dk_breath_of_sindragosa_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_breath_of_sindragosa_AuraScript);

        enum eSpells
        {
            DarkTransformation = 63560
        };

        void OnTick(AuraEffect const* p_AurEff)
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            Player* l_Player = l_Caster->ToPlayer();
            if (l_Player == nullptr)
                return;

            if (l_Player->GetSpecializationId() == SPEC_DK_UNHOLY)
            {
                /// Receive Dark Infusion for every 30 runic power
                if (p_AurEff->GetTickNumber() % 2 == 0)
                    if (Pet* l_Pet = l_Player->GetPet())
                        if (!l_Pet->HasAura(eSpells::DarkTransformation))
                            l_Player->CastSpell(l_Pet, DK_SPELL_DARK_INFUSION_STACKS, true);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_breath_of_sindragosa_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_breath_of_sindragosa_AuraScript();
    }
};

/// Called by Scent of Blood - 50421
/// Item - Death Knight T17 Blood 2P Bonus - 167192
class spell_dk_item_t17_blood_2p_bonus : public SpellScriptLoader
{
    public:
        spell_dk_item_t17_blood_2p_bonus() : SpellScriptLoader("spell_dk_item_t17_blood_2p_bonus") { }

        class spell_dk_item_t17_blood_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_item_t17_blood_2p_bonus_AuraScript);

            enum eSpells
            {
                T17Blood2P      = 167192,
                VampiricBlood   = 55233
            };

            void AfterRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    /// Each stack of Scent of Blood consumed reduces the cooldown of Vampiric Blood by 5 sec.
                    if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::T17Blood2P, EFFECT_0))
                    {
                        uint8 l_Stacks = p_AurEff->GetBase()->GetStackAmount();

                        if (l_Caster->GetTypeId() == TypeID::TYPEID_PLAYER)
                            l_Caster->ToPlayer()->ReduceSpellCooldown(eSpells::VampiricBlood, l_Stacks * l_AurEff->GetAmount());
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectApplyFn(spell_dk_item_t17_blood_2p_bonus_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_item_t17_blood_2p_bonus_AuraScript();
        }
};

/// Blood Shield - 77535
class spell_dk_blood_shield : public SpellScriptLoader
{
    public:
        spell_dk_blood_shield() : SpellScriptLoader("spell_dk_blood_shield") { }

        class spell_dk_blood_shield_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_blood_shield_AuraScript);

            enum eSpells
            {
                T17Blood4P = 165571
            };

            void AfterAbsorb(AuraEffect* p_AurEff, DamageInfo& /*p_DmgInfo*/, uint32& p_AbsorbAmount)
            {
                if (Unit* l_Target = GetTarget())
                {
                    /// While Vampiric Blood is active, your Blood Shield cannot be reduced below 3% of your maximum health.
                    if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(eSpells::T17Blood4P, EFFECT_0))
                    {
                        int32 l_FutureAbsorb = p_AurEff->GetAmount() - p_AbsorbAmount;
                        int32 l_MinimaAbsorb = l_Target->CountPctFromMaxHealth(l_AurEff->GetAmount());

                        /// We need to add some absorb amount to correct the absorb amount after that, and set it to 3% of max health
                        if (l_FutureAbsorb < l_MinimaAbsorb)
                        {
                            int32 l_AddedAbsorb = l_MinimaAbsorb - l_FutureAbsorb;
                            p_AurEff->ChangeAmount(p_AurEff->GetAmount() + l_AddedAbsorb);
                        }
                    }
                }
            }

            void Register() override
            {
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_blood_shield_AuraScript::AfterAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_blood_shield_AuraScript();
        }
};

/// Item - Death Knight T17 Frost 4P Driver - 167655
class spell_dk_item_t17_frost_4p_driver : public SpellScriptLoader
{
    public:
        spell_dk_item_t17_frost_4p_driver() : SpellScriptLoader("spell_dk_item_t17_frost_4p_driver") { }

        class spell_dk_item_t17_frost_4p_driver_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_item_t17_frost_4p_driver_AuraScript);

            enum eSpells
            {
                FrozenRuneblade = 170202
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (l_ProcSpell == nullptr)
                    return;

                Unit* l_Target = p_EventInfo.GetActionTarget();
                if (l_Target == nullptr || l_Target == l_Caster)
                    return;

                /// While Pillar of Frost is active, your special attacks trap a soul in your rune weapon.
                l_Caster->CastSpell(l_Target, eSpells::FrozenRuneblade, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_item_t17_frost_4p_driver_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_item_t17_frost_4p_driver_AuraScript();
        }
};

/// Item - Death Knight T17 Frost 4P Driver (Periodic) - 170205
class spell_dk_item_t17_frost_4p_driver_periodic : public SpellScriptLoader
{
    public:
        spell_dk_item_t17_frost_4p_driver_periodic() : SpellScriptLoader("spell_dk_item_t17_frost_4p_driver_periodic") { }

        class spell_dk_item_t17_frost_4p_driver_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_item_t17_frost_4p_driver_periodic_AuraScript);

            enum eSpells
            {
                FrozenRunebladeMainHand = 165569,
                FrozenRunebladeOffHand  = 178808,
                FrozenRunebladeStacks   = 170202
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Unit* l_Target = l_Caster->getVictim();
                if (l_Target == nullptr)
                    return;

                if (Player* l_Player = l_Caster->ToPlayer())
                {
                    if (Aura* l_Aura = l_Player->GetAura(eSpells::FrozenRunebladeStacks))
                    {
                        if (Item* l_MainHand = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND))
                            l_Player->CastSpell(l_Target, eSpells::FrozenRunebladeMainHand, true);

                        if (Item* l_OffHand = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND))
                            l_Player->CastSpell(l_Target, eSpells::FrozenRunebladeOffHand, true);

                        l_Aura->DropStack();
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_item_t17_frost_4p_driver_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_item_t17_frost_4p_driver_periodic_AuraScript();
        }
};

/// Last Update 6.2.3
/// Soul Reaper (Bonus) - 114868
class spell_dk_soul_reaper_bonus : public SpellScriptLoader
{
    public:
        spell_dk_soul_reaper_bonus() : SpellScriptLoader("spell_dk_soul_reaper_bonus") {}

        class  spell_dk_soul_reaper_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_soul_reaper_bonus_AuraScript);

            enum eSpells
            {
                GlyphofSwiftDeath = 146645
            };

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & p_Amount, bool & /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster->HasAura(eSpells::GlyphofSwiftDeath))
                    p_Amount = 0;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_soul_reaper_bonus_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new  spell_dk_soul_reaper_bonus_AuraScript();
        }
};

/// last update : 6.2.3
/// Defile - 152280
class spell_dk_defile_absorb_effect : public SpellScriptLoader
{
    public:
        spell_dk_defile_absorb_effect() : SpellScriptLoader("spell_dk_defile_absorb_effect") { }

        class spell_dk_defile_absorb_effect_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_defile_absorb_effect_AuraScript);

            bool IsInDefileArea(Unit* p_Victim, Unit *p_Attacker)
            {
                if (p_Attacker == nullptr || p_Victim == nullptr)
                    return false;

                std::list<AreaTrigger*> l_DefileList;

                p_Victim->GetAreaTriggerList(l_DefileList, GetSpellInfo()->Id);

                if (!l_DefileList.empty())
                {
                    for (auto itr : l_DefileList)
                    {
                        if (itr->IsInRange(p_Attacker, 0, itr->GetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE) * 8.0f))
                            return true;
                    }
                }
                return false;
            }

            void OnAbsorb(AuraEffect* /*p_AurEff*/, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                Unit* l_Victim = p_DmgInfo.GetVictim();
                Unit* l_Attacker = p_DmgInfo.GetAttacker();

                if (l_Victim == nullptr || l_Attacker == nullptr)
                    return;

                if (IsInDefileArea(l_Victim, l_Attacker))
                    p_AbsorbAmount = CalculatePct(p_DmgInfo.GetDamage(), GetSpellInfo()->Effects[EFFECT_3].BasePoints);
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                /// Only in blood spec
                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SPEC_DK_BLOOD)
                    amount = 0;
                else
                    amount = -1;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_defile_absorb_effect_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_defile_absorb_effect_AuraScript::OnAbsorb, EFFECT_3, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_defile_absorb_effect_AuraScript();
        }
};

/// last update : 6.2.3
/// Periodic Taunt - 43264
class spell_dk_army_of_the_death_taunt : public SpellScriptLoader
{
    public:
        spell_dk_army_of_the_death_taunt() : SpellScriptLoader("spell_dk_army_of_the_death_taunt") { }

        class spell_dk_army_of_the_death_taunt_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_army_of_the_death_taunt_AuraScript);

            enum eSpells
            {
                GlyphofArmyoftheDead = 58669
            };

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Owner = l_Target->GetOwner();

                if (l_Owner == nullptr)
                    return;

                if (l_Owner->HasAura(eSpells::GlyphofArmyoftheDead))
                    l_Target->RemoveAura(GetSpellInfo()->Id);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dk_army_of_the_death_taunt_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_army_of_the_death_taunt_AuraScript();
        }
};

/// Last Update 6.2.3
/// Shadow Infusion - 49572
class spell_dk_shadow_infusion : public SpellScriptLoader
{
    public:
        spell_dk_shadow_infusion() : SpellScriptLoader("spell_dk_shadow_infusion") { }

        class spell_dk_shadow_infusion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_shadow_infusion_AuraScript);

            enum eSpells
            {
                DeathCoilDamage         = 47632,
                DeathCoilHeal           = 47633,
                ShadowInfusion          = 91342,
                DarkTransformation      = 93426,
                DarkTranformationAura   = 63560
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Player* l_Player = GetTarget()->ToPlayer();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() == nullptr)
                    return;
                
                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != eSpells::DeathCoilDamage && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != eSpells::DeathCoilHeal)
                    return;

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(eSpells::DarkTransformation))
                    return;

                Pet* l_Pet = l_Player->GetPet();

                if (l_Pet != nullptr && l_Pet->HasAura(eSpells::DarkTranformationAura))
                    return;

                l_Player->CastSpell(l_Player, eSpells::ShadowInfusion, true);

                if (Pet* l_Pet = l_Player->GetPet())
                {
                    if (Aura* l_Aura = l_Pet->GetAura(eSpells::ShadowInfusion))
                    {
                        if (l_Aura->GetStackAmount() > 4) /// Apply Dark Transformation
                            l_Player->CastSpell(l_Player, eSpells::DarkTransformation, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_shadow_infusion_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_shadow_infusion_AuraScript();
        }
};

/// Last Update 6.2.3
/// Might of the Frozen Wastes - 81333
class spell_dk_might_of_the_frozen_wastes : public SpellScriptLoader
{
    public:
        spell_dk_might_of_the_frozen_wastes() : SpellScriptLoader("spell_dk_might_of_the_frozen_wastes") { }

        class spell_dk_might_of_the_frozen_wastes_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_might_of_the_frozen_wastes_AuraScript);

            void CalculateEffect(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    bool l_TwoHand = false;
                    Item* l_MainItem = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                    Item* l_OffHandItem = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                    if (l_MainItem)
                    if (l_MainItem->GetTemplate() && l_MainItem->GetTemplate()->IsTwoHandedWeapon())
                        l_TwoHand = true;

                    if (l_OffHandItem)
                    if (l_OffHandItem->GetTemplate() && l_OffHandItem->GetTemplate()->IsTwoHandedWeapon())
                        l_TwoHand = true;

                    if (!l_TwoHand)
                        p_Amount = 0;
                }
            }


            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_might_of_the_frozen_wastes_AuraScript::CalculateEffect, EFFECT_2, SPELL_AURA_MOD_AUTOATTACK_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_might_of_the_frozen_wastes_AuraScript();
        }
};

/// Last Update 6.2.3
/// Improved Death Grip - 157367
class spell_dk_improved_death_grip : public PlayerScript
{
    public:
        spell_dk_improved_death_grip() : PlayerScript("spell_dk_improved_death_grip") {}

        enum ImprovedDeathGrip
        {
            Spell = 157367,
            ChainsOfIce = 45524,
            Jump = 49575
        };

        void OnFinishMovement(Player * p_Player, uint32 p_SpellID, uint64 const p_TargetGUID)
        {
            if (!(p_SpellID == ImprovedDeathGrip::Jump))
                return;

            Unit* l_Target = ObjectAccessor::FindUnit(p_TargetGUID);
            if (l_Target && l_Target->getClass() == CLASS_DEATH_KNIGHT)
            {
                if (l_Target->HasSpell(ImprovedDeathGrip::Spell))
                    l_Target->CastSpell(p_Player, ImprovedDeathGrip::ChainsOfIce, true);
            }
        }
};

#ifndef __clang_analyzer__
void AddSC_deathknight_spell_scripts()
{
    new spell_dk_shadow_infusion();
    new spell_dk_army_of_the_death_taunt();
    new spell_dk_defile_absorb_effect();
    new spell_dk_soul_reaper_bonus();
    new spell_dk_death_coil();
    new spell_dk_empowered_obliterate_icy_touch();
    new spell_dk_empowered_obliterate_howling_blast();
    new spell_dk_glyph_of_death_and_decay();
    new spell_dk_death_barrier();
    new spell_dk_plague_strike();
    new spell_dk_gorefiends_grasp();
    new spell_dk_dark_transformation_form();
    new spell_dk_desecrated_ground();
    new spell_dk_festering_strike();
    new spell_dk_death_strike_heal();
    new spell_dk_howling_blast();
    new spell_dk_conversion();
    new spell_dk_remorseless_winter();
    new spell_dk_soul_reaper();
    new spell_dk_pillar_of_frost();
    new spell_dk_blood_tap();
    new spell_dk_death_siphon();
    new spell_dk_death_strike();
    new spell_dk_purgatory();
    new spell_dk_purgatory_absorb();
    new spell_dk_plague_leech();
    new spell_dk_unholy_blight();
    new spell_dk_outbreak();
    new spell_dk_raise_dead();
    new spell_dk_anti_magic_shell_raid();
    new spell_dk_anti_magic_shell_self();
    new spell_dk_anti_magic_zone();
    new spell_dk_death_gate_teleport();
    new spell_dk_death_gate();
    new spell_dk_blood_boil();
    new spell_dk_death_grip();
    new spell_dk_corpse_explosion();
    new spell_dk_glyph_of_corpse_explosion();
    new spell_dk_glyph_of_horn_of_winter();
    new spell_dk_icy_touch();
    new spell_dk_necrotic_plague_aura();
    new spell_dk_runic_empowerment();
    new spell_dk_runic_corruption();
    new spell_dk_death_pact();
    new spell_dk_chain_of_ice();
    new spell_dk_chilblains_aura();
    new spell_dk_reaping();
    new spell_dk_mark_of_sindragosa();
    new spell_dk_dark_succor();
    new spell_dk_glyph_of_the_geist();
    new spell_dk_glyph_of_the_skeleton();
    new spell_dk_improved_death_grip();
    new spell_dk_glyph_of_deaths_embrace();
    new spell_dk_will_of_the_necropolis();
    new spell_dk_enhanced_death_coil();
    new spell_dk_gargoyle_strike();
    new spell_dk_blood_rites();
    new spell_dk_control_undead();
    new spell_dk_presences();
    new spell_dk_breath_of_sindragosa();
    new spell_dk_item_t17_blood_2p_bonus();
    new spell_dk_blood_shield();
    new spell_dk_item_t17_frost_4p_driver();
    new spell_dk_item_t17_frost_4p_driver_periodic();
    new spell_dk_might_of_the_frozen_wastes();

    new PlayerScript_Blood_Tap();
}
#endif
