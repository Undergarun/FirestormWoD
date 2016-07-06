////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Scripts for spells with SPELLFAMILY_WARLOCK and SPELLFAMILY_GENERIC spells used by warlock players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warl_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "CreatureAI.h"

enum WarlockSpells
{
    WARLOCK_DEMONIC_CIRCLE_SUMMON           = 48018,
    WARLOCK_DEMONIC_CIRCLE_TELEPORT         = 48020,
    WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST       = 62388,
    WARLOCK_UNSTABLE_AFFLICTION_DISPEL      = 31117,
    WARLOCK_SOULBURN_AURA                   = 74434,
    WARLOCK_CORRUPTION                      = 146739,
    WARLOCK_AGONY                           = 980,
    WARLOCK_UNSTABLE_AFFLICTION             = 30108,
    WARLOCK_IMMOLATE                        = 157736,
    WARLOCK_SHADOWBURN_ENERGIZE             = 125882,
    WARLOCK_CONFLAGRATE                     = 17962,
    WARLOCK_CONFLAGRATE_FIRE_AND_BRIMSTONE  = 108685,
    WARLOCK_IMMOLATE_FIRE_AND_BRIMSTONE     = 108686,
    WARLOCK_BACKDRAFT                       = 117828,
    WARLOCK_RAIN_OF_FIRE                    = 104232,
    WARLOCK_RAIN_OF_FIRE_TRIGGERED          = 104233,
    WARLOCK_SPAWN_PURPLE_DEMONIC_GATEWAY    = 113890,
    WARLOCK_SOUL_SWAP_AURA                  = 86211,
    WARLOCK_SOUL_SWAP_VISUAL                = 92795,
    WARLOCK_GRIMOIRE_OF_SACRIFICE           = 108503,
    WARLOCK_METAMORPHOSIS                   = 103958,
    WARLOCK_DEMONIC_LEAP_JUMP               = 109167,
    WARLOCK_ITEM_S12_TIER_4                 = 131632,
    WARLOCK_TWILIGHT_WARD_S12               = 131623,
    WARLOCK_TWILIGHT_WARD_METAMORPHOSIS_S12 = 131624,
    WARLOCK_SHADOWFLAME                     = 47960,
    WARLOCK_SOUL_LEECH_HEAL                 = 108366,
    WARLOCK_DARK_REGENERATION               = 108359,
    WARLOCK_MOLTEN_CORE                     = 122355,
    WARLOCK_MOLTEN_CORE_AURA                = 122351,
    WARLOCK_WILD_IMP_SUMMON                 = 104317,
    WARLOCK_DEMONIC_CALL                    = 114925,
    WARLOCK_DECIMATE_AURA                   = 108869,
    WARLOCK_SOUL_LEECH_AURA                 = 108370,
    WARLOCK_SOUL_LINK_TALENT                = 108415,
    WARLOCK_SOUL_LINK_DUMMY_AURA            = 108446,
    WARLOCK_SOUL_LINK_HEAL                  = 108447,
    WARLOCK_GLYPH_OF_CONFLAGRATE            = 56235,
    WARLOCK_SHIELD_OF_SHADOW                = 115232,
    WARLOCK_THREATENING_PRESENCE            = 112042,
    WARLOCK_SUPPLANT_DEMONIC_COMMAND        = 119904,
    WARLOCK_AFTERMATH_STUN                  = 85387,
    WARLOCK_IMP_SWARM                       = 104316,
    WARLOCK_DISRUPTED_NETHER                = 114736,
    WARLOCK_GLYPH_OF_SIPHON_LIFE            = 56218,
    WARLOCK_SOULBURN_OVERRIDE_1             = 93313,
    WARLOCK_SOULBURN_OVERRIDE_2             = 104249,
    WARLOCK_SEED_OF_CORRUPTION_DUMMY        = 157697,
    WARLOCK_SOULBURN_DEMONIC_CIRCLE_TELE    = 114794,
    WARLOCK_IMP_SINGLE_MAGIC                = 132411,
    WARLOCK_VOIDWALKER_SHADOW_BULWARK       = 132413,
    WARLOCK_SUCCUBUS_WHIPLASH               = 137706,
    WARLOCK_FELHUNTER_SPELL_LOCK            = 132409,
    WARLOCK_FELGUARD_PURSUIT                = 132410,
    WARLOCK_INFERNAL_METEOR_STRIKE          = 171017,
    WARLOCK_DOOMGUARD_SHADOW_LOCK           = 171139,
    WARLOCK_SHADOW_BOLT                     = 686,
    WARLOCK_TWILIGHT_WARD                   = 6229,
    WARLOCK_TWILIGHT_WARD_METAMORPHOSIS     = 104048,
    WARLOCK_SOUL_SWAP                       = 86121,
    WARLOCK_SOUL_SWAP_EXHALE                = 86213,
    WARLOCK_GLYPH_OF_SOUL_SWAP              = 56226,
    WARLOCK_SOUL_HARVEST                    = 101976,
    WARLOCK_FEAR                            = 5782,
    WARLOCK_SPELL_CORRUPTION                = 172,
    WARLOCK_SPELL_CORRUPTION_DOT            = 146739,
    WARLOCK_SOULSHATTER                     = 32835,
    WARLOCK_HAND_OF_GULDAN_DAMAGE           = 86040,
    WARLOCK_HELLFIRE_DAMAGE                 = 5857,
    WARLOCK_DARK_APOTHEOSIS                 = 114168,
    WARLOCK_DEMON_SINGLE_MAGIC              = 89808,
    WARLOCK_DEMON_SUFFERING                 = 17735,
    WARLOCK_DEMON_SEDUCE                    = 6358,
    WARLOCK_DEMON_SPELL_LOCK                = 19647,
    WARLOCK_DEMON_AXE_TOSS                  = 89766,
    WARLOCK_LIFE_TAP                        = 1454,
    WARLOCK_GLYPH_OF_LIFE_TAP               = 63320,
    WARLOCK_GLYPH_OF_DRAIN_LIFE             = 63302,
    WARLOCK_GLYPH_OF_DARK_SOUL              = 159665,
    WARLOCK_SPELL_SYPHON_LIFE               = 63106,
    WARLOCK_SPELL_SOULBURN_HAUNT            = 157698,
    WARLOCK_SPELL_SOULBURN_HAUNT_AURA       = 152109,
    WARLOCK_WOD_PVP_AFFLICTION_4P_BONUS     = 171379,
    WARLOCK_WOD_PVP_AFFLICTION_4P_BONUS_EFF = 171380
};

enum BurningEmbersSpells
{
    SPELL_WARL_GLYPH_OF_VERDANT_SPHERES = 56241,
    SPELL_WARL_CHARRED_REMAINS          = 157696
};

/// Fire and Brimstone - 108683, with Charred Remains - 157696
class spell_warl_fire_and_brimstone : public SpellScriptLoader
{
    public:
        spell_warl_fire_and_brimstone() : SpellScriptLoader("spell_warl_fire_and_brimstone") { }

        class spell_warl_fire_and_brimstone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_fire_and_brimstone_AuraScript);

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(SPELL_WARL_CHARRED_REMAINS))
                        p_Amount = 0;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_fire_and_brimstone_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_fire_and_brimstone_AuraScript();
        }
};

// Called by Grimoire: Imp - 111859, Grimoire: Voidwalker - 111895, Grimoire: Succubus - 111896
// Grimoire: Felhunter - 111897 and Grimoire: Felguard - 111898
// Grimoire of Service - Demons
class spell_warl_grimoire_of_service: public SpellScriptLoader
{
    public:
        spell_warl_grimoire_of_service() : SpellScriptLoader("spell_warl_grimoire_of_service") { }

        class spell_warl_grimoire_of_service_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_grimoire_of_service_SpellScript);

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                for (Unit::ControlList::const_iterator itr = l_Caster->m_Controlled.begin(); itr != l_Caster->m_Controlled.end(); ++itr)
                {
                    switch ((*itr)->GetEntry())
                    {
                    case ENTRY_IMP:
                        (*itr)->CastSpell(l_Caster, WARLOCK_DEMON_SINGLE_MAGIC, true);
                        if ((*itr)->ToCreature() && (*itr)->ToCreature()->AI())
                            (*itr)->ToCreature()->AI()->AttackStart(l_Target);
                        break;
                    case ENTRY_VOIDWALKER:
                        (*itr)->CastSpell(l_Target, WARLOCK_DEMON_SUFFERING, true);
                        break;
                    case ENTRY_SUCCUBUS:
                        (*itr)->CastSpell(l_Target, WARLOCK_DEMON_SEDUCE, true);
                        break;
                    case ENTRY_FELHUNTER:
                        (*itr)->CastSpell(l_Target, WARLOCK_DEMON_SPELL_LOCK, true);
                        break;
                    case ENTRY_FELGUARD:
                        (*itr)->CastSpell(l_Target, WARLOCK_DEMON_AXE_TOSS, true);
                        break;
                    default:
                        break;
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_warl_grimoire_of_service_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_grimoire_of_service_SpellScript();
        }
};

// Haunt (dispel effect) - 48181
class spell_warl_haunt_dispel: public SpellScriptLoader
{
    public:
        spell_warl_haunt_dispel() : SpellScriptLoader("spell_warl_haunt_dispel") { }

        class spell_warl_haunt_dispel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_haunt_dispel_AuraScript);

            void HandleDispel(DispelInfo* /*dispelInfo*/)
            {
                if (Unit* caster = GetCaster())
                    caster->EnergizeBySpell(caster, GetSpellInfo()->Id, 100, POWER_SOUL_SHARDS);
            }

            void Register()
            {
                AfterDispel += AuraDispelFn(spell_warl_haunt_dispel_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_haunt_dispel_AuraScript();
        }
};

/// Last Update 6.2.3
/// Haunt - 48181
class spell_warl_haunt : public SpellScriptLoader
{
    public:
        spell_warl_haunt() : SpellScriptLoader("spell_warl_haunt") { }

        class spell_warl_haunt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_haunt_SpellScript);

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();

                /// Your Haunt spell also increases the damage of your damage over time effects.
                if (l_Caster->HasAura(WARLOCK_SOULBURN_AURA) && l_Caster->HasAura(WARLOCK_SPELL_SOULBURN_HAUNT_AURA))
                {
                    l_Caster->CastSpell(l_Caster, WARLOCK_SPELL_SOULBURN_HAUNT, true);
                    l_Caster->RemoveAurasDueToSpell(WARLOCK_SOULBURN_AURA);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_warl_haunt_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_haunt_SpellScript();
        }
};

/// Grimoire of Supremacy - 108499
class spell_warl_grimoire_of_supremacy: public SpellScriptLoader
{
    public:
        spell_warl_grimoire_of_supremacy() : SpellScriptLoader("spell_warl_grimoire_of_supremacy") { }

        class spell_warl_grimoire_of_supremacy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_grimoire_of_supremacy_AuraScript);

            enum eSpells
            {
                SummonFelImp      = 112866,
                SummonVoidlord    = 112867,
                SummonShivarra    = 112868,
                SummonObserver    = 112869,
                SummonWrathguard  = 112870,
                SummonAbyssal     = 112921,
                SummonTerrorguard = 112927
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();
                if (!l_Player)
                    return;

                l_Player->learnSpell(eSpells::SummonFelImp, false);
                l_Player->learnSpell(eSpells::SummonVoidlord, false);
                l_Player->learnSpell(eSpells::SummonShivarra, false);
                l_Player->learnSpell(eSpells::SummonObserver, false);
                l_Player->learnSpell(eSpells::SummonAbyssal, false);
                l_Player->learnSpell(eSpells::SummonTerrorguard, false);

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                    l_Player->learnSpell(eSpells::SummonWrathguard, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();
                if (!l_Player)
                    return;

                if (l_Player->HasSpell(eSpells::SummonFelImp))
                    l_Player->removeSpell(eSpells::SummonFelImp, false, false);

                if (l_Player->HasSpell(eSpells::SummonVoidlord))
                    l_Player->removeSpell(eSpells::SummonVoidlord, false, false);

                if (l_Player->HasSpell(eSpells::SummonShivarra))
                    l_Player->removeSpell(eSpells::SummonShivarra, false, false);

                if (l_Player->HasSpell(eSpells::SummonObserver))
                    l_Player->removeSpell(eSpells::SummonObserver, false, false);

                if (l_Player->HasSpell(eSpells::SummonAbyssal))
                    l_Player->removeSpell(eSpells::SummonAbyssal, false, false);

                if (l_Player->HasSpell(eSpells::SummonTerrorguard))
                    l_Player->removeSpell(eSpells::SummonTerrorguard, false, false);

                if (l_Player->HasSpell(eSpells::SummonWrathguard))
                    l_Player->removeSpell(eSpells::SummonWrathguard, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_grimoire_of_supremacy_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warl_grimoire_of_supremacy_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_grimoire_of_supremacy_AuraScript();
        }
};

/// Call by Summon Fel Imp - 112866, Summon Voidlord - 112867, Summon Shivarra - 112868
/// Summon Observer - 112869, Summon Wrathguard - 112870, Summon Abyssal - 112921, Summon Terrorguard - 112927
/// Grimoire of Supremacy - 108499
class spell_warl_grimoire_of_supremacy_effect : public SpellScriptLoader
{
    public:
        spell_warl_grimoire_of_supremacy_effect() : SpellScriptLoader("spell_warl_grimoire_of_supremacy_effect") { }

        class spell_warl_grimoire_of_supremacy_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_grimoire_of_supremacy_effect_SpellScript);

            enum eSpells
            {
                GrimoireOfSupremacyBonus = 115578
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                for (Unit::ControlList::const_iterator itr = l_Caster->m_Controlled.begin(); itr != l_Caster->m_Controlled.end(); ++itr)
                    (*itr)->CastSpell((*itr), eSpells::GrimoireOfSupremacyBonus, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warl_grimoire_of_supremacy_effect_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_grimoire_of_supremacy_effect_SpellScript();
        }
};

/// Call by Avoidance - 32233
/// Grimoire of Supremacy - 108499
class spell_warl_grimoire_of_supremacy_bonus : public SpellScriptLoader
{
    public:
        spell_warl_grimoire_of_supremacy_bonus() : SpellScriptLoader("spell_warl_grimoire_of_supremacy_bonus") { }

        class spell_warl_grimoire_of_supremacy_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_grimoire_of_supremacy_bonus_AuraScript);

            enum eSpells
            {
                GrimoireOfSupremacyBonus    = 115578,
                GrimoireOfSupremacy         = 108499
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Owner = l_Target->GetOwner();

                if (l_Owner->HasAura(eSpells::GrimoireOfSupremacy) && !l_Target->HasAura(eSpells::GrimoireOfSupremacyBonus))
                    l_Target->CastSpell(l_Target, eSpells::GrimoireOfSupremacyBonus, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_grimoire_of_supremacy_bonus_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_grimoire_of_supremacy_bonus_AuraScript();
        }
};


// Soulburn : Seed of Corruption - Damage - 87385
class spell_warl_soulburn_seed_of_corruption_damage: public SpellScriptLoader
{
    public:
        spell_warl_soulburn_seed_of_corruption_damage() : SpellScriptLoader("spell_warl_soulburn_seed_of_corruption_damage") { }

        class spell_warl_soulburn_seed_of_corruption_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_soulburn_seed_of_corruption_damage_SpellScript);

            std::list<uint64> m_Targets;

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                for (std::list<WorldObject*>::const_iterator l_It = p_Targets.begin(); l_It != p_Targets.end(); ++l_It)
                    m_Targets.push_back((*l_It)->GetGUID());
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                /// Your Seed of Corruption detonation effect will afflict Corruption on all enemy targets.
                if (l_Caster->HasAura(WARLOCK_SEED_OF_CORRUPTION_DUMMY))
                    l_Caster->CastSpell(l_Target, WARLOCK_SPELL_CORRUPTION, true);
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                {
                    l_Caster->RemoveAurasDueToSpell(WARLOCK_SEED_OF_CORRUPTION_DUMMY);
                    return;
                }

                if (!m_Targets.empty())
                    m_Targets.remove(l_Target->GetGUID());

                if (m_Targets.empty())
                {
                    /// Remove Soul Burn aura
                    if (l_Caster->HasAura(WARLOCK_SEED_OF_CORRUPTION_DUMMY))
                        l_Caster->RemoveAurasDueToSpell(WARLOCK_SEED_OF_CORRUPTION_DUMMY);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_soulburn_seed_of_corruption_damage_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_soulburn_seed_of_corruption_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                AfterHit += SpellHitFn(spell_warl_soulburn_seed_of_corruption_damage_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_soulburn_seed_of_corruption_damage_SpellScript();
        }
};

// Called by Soulburn : Seed of Corruption - 114790
class spell_warl_soulburn_seed_of_corruption: public SpellScriptLoader
{
    public:
        spell_warl_soulburn_seed_of_corruption() : SpellScriptLoader("spell_warl_soulburn_seed_of_corruption") { }

        class spell_warl_soulburn_seed_of_corruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_soulburn_seed_of_corruption_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, WARLOCK_SEED_OF_CORRUPTION_DUMMY, true);

                    if (caster->HasAura(WARLOCK_SOULBURN_AURA))
                        caster->RemoveAurasDueToSpell(WARLOCK_SOULBURN_AURA);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_soulburn_seed_of_corruption_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_soulburn_seed_of_corruption_SpellScript();
        }
};

/// Soulburn - 74434
class spell_warl_soulburn_override: public SpellScriptLoader
{
    public:
        spell_warl_soulburn_override() : SpellScriptLoader("spell_warl_soulburn_override") { }

        class spell_warl_soulburn_override_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_soulburn_override_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                // Overrides Seed of Corruption
                l_Caster->CastSpell(l_Caster, WARLOCK_SOULBURN_OVERRIDE_1, true);
                // Overrides Demonic Circle : Teleport
                l_Caster->CastSpell(l_Caster, WARLOCK_SOULBURN_OVERRIDE_2, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                
                // Overrides Seed of Corruption
                l_Caster->RemoveAura(WARLOCK_SOULBURN_OVERRIDE_1);
                // Overrides Demonic Circle : Teleport
                l_Caster->RemoveAura(WARLOCK_SOULBURN_OVERRIDE_2);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_soulburn_override_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warl_soulburn_override_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_soulburn_override_AuraScript();
        }
};

/// Last Update 6.2.3
/// Imp Swarm - 104316
class spell_warl_imp_swarm: public SpellScriptLoader
{
    public:
        spell_warl_imp_swarm() : SpellScriptLoader("spell_warl_imp_swarm") { }

        class spell_warl_imp_swarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_imp_swarm_SpellScript);

            bool m_AlreadyReduceCooldown = false;

            SpellCastResult CheckSpec()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                if (l_Caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SPEC_WARLOCK_DEMONOLOGY)
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        for (uint8 i = 0; i < GetEffectValue(); i++)
                        {
                            l_Caster->AddAura(WARLOCK_DEMONIC_CALL, l_Caster);
                            l_Caster->CastSpell(l_Target, WARLOCK_WILD_IMP_SUMMON, true);
                        }
                    }
                }
            }

            void HandleAfterHit()
            {
                if (m_AlreadyReduceCooldown)
                    return;

                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                {
                    float l_Haste = 1.0f - l_Player->GetFloatValue(UNIT_FIELD_MOD_HASTE);

                    int32 l_ReduceCooldown = CalculatePct(CalculatePct(GetSpellInfo()->RecoveryTime, (l_Haste * 100)), 100);
                    l_Player->ReduceSpellCooldown(GetSpellInfo()->Id, l_ReduceCooldown);

                    m_AlreadyReduceCooldown = true;
                }
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_imp_swarm_SpellScript::CheckSpec);
                OnEffectHitTarget += SpellEffectFn(spell_warl_imp_swarm_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                AfterHit += SpellHitFn(spell_warl_imp_swarm_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warl_imp_swarm_SpellScript();
        }
};

// Glyph of Imp Swarm - 56242
class spell_warl_glyph_of_imp_swarm: public SpellScriptLoader
{
    public:
        spell_warl_glyph_of_imp_swarm() : SpellScriptLoader("spell_warl_glyph_of_imp_swarm") { }

        class spell_warl_glyph_of_imp_swarm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_glyph_of_imp_swarm_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                        _player->learnSpell(WARLOCK_IMP_SWARM, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(WARLOCK_IMP_SWARM))
                        _player->removeSpell(WARLOCK_IMP_SWARM, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_imp_swarm_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warl_glyph_of_imp_swarm_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_glyph_of_imp_swarm_AuraScript();
        }
};

// Unbound Will - 108482
class spell_warl_unbound_will: public SpellScriptLoader
{
    public:
        spell_warl_unbound_will() : SpellScriptLoader("spell_warl_unbound_will") { }

        class spell_warl_unbound_will_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_unbound_will_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    caster->RemoveAurasWithMechanic(IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK);

                    if (Pet* pet = caster->ToPlayer()->GetPet())
                        pet->RemoveAurasWithMechanic(IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_unbound_will_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_unbound_will_SpellScript();
        }
};

// Rain of Fire (damage) - 42223
class spell_warl_rain_of_fire_damage: public SpellScriptLoader
{
    public:
        spell_warl_rain_of_fire_damage() : SpellScriptLoader("spell_warl_rain_of_fire_damage") { }

        class spell_warl_rain_of_fire_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_rain_of_fire_damage_SpellScript);

            void HandleOnHitUnit(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    // Deals 50% additional damage if the target is immolated.
                    if (l_Target->HasAura(WARLOCK_IMMOLATE) || l_Target->HasAura(WARLOCK_IMMOLATE_FIRE_AND_BRIMSTONE))
                    {
                        int32 l_Damage = GetHitDamage();

                        AddPct(l_Damage, 50);
                        SetHitDamage(l_Damage);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warl_rain_of_fire_damage_SpellScript::HandleOnHitUnit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_rain_of_fire_damage_SpellScript();
        }
};

/// Last Update 6.2.3
/// Agony - 980
class spell_warl_agony: public SpellScriptLoader
{
    public:
        spell_warl_agony() : SpellScriptLoader("spell_warl_agony") { }

        class spell_warl_agony_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_agony_AuraScript);

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (Aura* l_Agony = l_Target->GetAura(p_AurEff->GetSpellInfo()->Id, l_Caster->GetGUID()))
                {
                    l_Agony->ModStackAmount(p_AurEff->GetBaseAmount());

                    /// Patch 6.2.2 (2015-09-01): Now deals 10 % less damage in PvP combat.
                    if (l_Target->IsPlayer() && p_AurEff->GetTickNumber() == 1)
                        l_Agony->GetEffect(EFFECT_0)->ChangeAmount(l_Agony->GetEffect(EFFECT_0)->GetAmount() - CalculatePct(l_Agony->GetEffect(EFFECT_0)->GetAmount(), 10));
                }
            }

            bool CanRefreshProcDummy()
            {
                return false;
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_agony_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                CanRefreshProc += AuraCanRefreshProcFn(spell_warl_agony_AuraScript::CanRefreshProcDummy);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_agony_AuraScript();
        }
};

// Grimoire of Sacrifice - 108503
class spell_warl_grimoire_of_sacrifice: public SpellScriptLoader
{
    public:
        spell_warl_grimoire_of_sacrifice() : SpellScriptLoader("spell_warl_grimoire_of_sacrifice") { }

        class spell_warl_grimoire_of_sacrifice_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_grimoire_of_sacrifice_SpellScript);

            void HandleAfterCast()
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    if (Pet* pet = player->GetPet())
                    {
                        // Supplant Command Demon
                        if (player->getLevel() >= 56)
                        {
                            int32 bp = 0;

                            player->RemoveAura(WARLOCK_SUPPLANT_DEMONIC_COMMAND);

                            switch (pet->GetEntry())
                            {
                                case ENTRY_IMP:
                                    bp = WARLOCK_IMP_SINGLE_MAGIC;
                                    break;
                                case ENTRY_VOIDWALKER:
                                    bp = WARLOCK_VOIDWALKER_SHADOW_BULWARK;
                                    break;
                                case ENTRY_SUCCUBUS:
                                    bp = WARLOCK_SUCCUBUS_WHIPLASH;
                                    break;
                                case ENTRY_FELHUNTER:
                                    bp = WARLOCK_FELHUNTER_SPELL_LOCK;
                                    break;
                                case ENTRY_FELGUARD:
                                    bp = WARLOCK_FELGUARD_PURSUIT;
                                    break;
                                case ENTRY_INFERNAL_PET:
                                    bp = WARLOCK_INFERNAL_METEOR_STRIKE;
                                    break;
                                case ENTRY_DOOMGUARD_PET:
                                    bp = WARLOCK_DOOMGUARD_SHADOW_LOCK;
                                    break;
                                default:
                                    break;
                            }

                            if (bp)
                                player->CastCustomSpell(player, WARLOCK_SUPPLANT_DEMONIC_COMMAND, &bp, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warl_grimoire_of_sacrifice_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_grimoire_of_sacrifice_SpellScript();
        }

        class spell_warl_grimoire_of_sacrifice_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_grimoire_of_sacrifice_AuraScript);

            enum eSpells
            {
                SoulLink = 108415
            };

            void CalculateEffectAmountSoulLink(AuraEffect const* /*aurEff*/, int32 & p_Amount, bool & /*canBeRecalculated*/)
            {
                Unit* l_Owner = GetUnitOwner();

                if (l_Owner == nullptr)
                    return;

                if (!l_Owner->HasSpell(eSpells::SoulLink))
                    p_Amount = 0;
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(WARLOCK_SUPPLANT_DEMONIC_COMMAND))
                    l_Target->RemoveAura(WARLOCK_SUPPLANT_DEMONIC_COMMAND);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_grimoire_of_sacrifice_AuraScript::CalculateEffectAmountSoulLink, EFFECT_2, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
                OnEffectRemove += AuraEffectApplyFn(spell_warl_grimoire_of_sacrifice_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_grimoire_of_sacrifice_AuraScript();
        }
};

/// Flames of Xoroth - 120451
class spell_warl_flames_of_xoroth: public SpellScriptLoader
{
    public:
        spell_warl_flames_of_xoroth() : SpellScriptLoader("spell_warl_flames_of_xoroth") { }

        class spell_warl_flames_of_xoroth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_flames_of_xoroth_SpellScript);

            SpellCastResult CheckPet()
            {
                if (!GetCaster())
                    return SPELL_FAILED_DONT_REPORT;

                Player* _plr = GetCaster()->ToPlayer();
                if (!_plr)
                    return SPELL_FAILED_DONT_REPORT;

                if (_plr->GetPet())
                    return SPELL_FAILED_ALREADY_HAVE_PET;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                Player* player = GetCaster()->ToPlayer();
                if (player && player->GetLastPetNumber())
                {
                    if (Pet* l_NewPet = new Pet(player, SUMMON_PET))
                    {
                        PreparedStatement* l_PetStatement = PetQueryHolder::GenerateFirstLoadStatement(0, player->GetLastPetNumber(), player->GetGUIDLow(), true, PET_SLOT_UNK_SLOT);

                        uint64 l_PlayerGUID = player->GetGUID();
                        uint32 l_PetNumber  = player->GetLastPetNumber();

                        CharacterDatabase.AsyncQuery(l_PetStatement, [l_NewPet, l_PlayerGUID, l_PetNumber](PreparedQueryResult p_Result) -> void
                        {
                            if (!p_Result)
                            {
                                delete l_NewPet;
                                return;
                            }

                            PetQueryHolder* l_PetHolder = new PetQueryHolder(p_Result->Fetch()[0].GetUInt32(), p_Result);
                            l_PetHolder->Initialize();

                            auto l_QueryHolderResultFuture = CharacterDatabase.DelayQueryHolder(l_PetHolder);

                            sWorld->AddQueryHolderCallback(QueryHolderCallback(l_QueryHolderResultFuture, [l_NewPet, l_PlayerGUID, l_PetNumber](SQLQueryHolder* p_QueryHolder) -> void
                            {
                                Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGUID);
                                if (!l_Player || !p_QueryHolder)
                                {
                                    delete l_NewPet;
                                    return;
                                }

                                l_NewPet->LoadPetFromDB(l_Player, 0, l_PetNumber, true, PET_SLOT_UNK_SLOT, false, (PetQueryHolder*)p_QueryHolder, [](Pet* p_Pet, bool p_Result) -> void
                                {
                                    if (!p_Result)
                                    {
                                        delete p_Pet;
                                        return;
                                    }

                                    // revive the pet if it is dead
                                    if (p_Pet->getDeathState() == DEAD || p_Pet->getDeathState() == CORPSE)
                                        p_Pet->setDeathState(ALIVE);

                                    p_Pet->ClearUnitState(uint32(UNIT_STATE_ALL_STATE));
                                    p_Pet->SetFullHealth();
                                    p_Pet->SetPower(p_Pet->getPowerType(), p_Pet->GetMaxPower(p_Pet->getPowerType()));

                                    switch (p_Pet->GetEntry())
                                    {
                                        case ENTRY_DOOMGUARD:
                                        case ENTRY_INFERNAL:
                                            p_Pet->SetEntry(ENTRY_IMP);
                                            break;
                                        default:
                                            break;
                                    }
                                });
                            }));
                        });
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_flames_of_xoroth_SpellScript::CheckPet);
                OnEffectHitTarget += SpellEffectFn(spell_warl_flames_of_xoroth_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_flames_of_xoroth_SpellScript();
        }
};

// Soul Link - 108446
class spell_warl_soul_link_dummy: public SpellScriptLoader
{
    public:
        spell_warl_soul_link_dummy() : SpellScriptLoader("spell_warl_soul_link_dummy") { }

        class spell_warl_soul_link_dummy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_soul_link_dummy_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster() || !GetCaster()->GetOwner())
                    return;

                if (Player* plr = GetCaster()->GetOwner()->ToPlayer())
                {
                    int32 bp = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
                    plr->CastCustomSpell(plr, WARLOCK_SOUL_LINK_HEAL, &bp, &bp, NULL, true);
                }
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster() || !GetTarget())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (GetTarget()->GetGUID() == _player->GetGUID())
                        if (Pet* pet = _player->GetPet())
                            if (pet->HasAura(WARLOCK_SOUL_LINK_DUMMY_AURA))
                                pet->RemoveAura(WARLOCK_SOUL_LINK_DUMMY_AURA);

                    if (_player->HasAura(WARLOCK_SOUL_LINK_DUMMY_AURA))
                        _player->RemoveAura(WARLOCK_SOUL_LINK_DUMMY_AURA);

                    if (Aura* grimoireOfSacrifice = _player->GetAura(WARLOCK_GRIMOIRE_OF_SACRIFICE))
                        if (grimoireOfSacrifice->GetEffect(EFFECT_6))
                                grimoireOfSacrifice->GetEffect(EFFECT_6)->SetAmount(0);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_warl_soul_link_dummy_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectApplyFn(spell_warl_soul_link_dummy_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_SPLIT_DAMAGE_PCT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_soul_link_dummy_AuraScript();
        }
};

// Soul Link - 108415
class spell_warl_soul_link: public SpellScriptLoader
{
    public:
        spell_warl_soul_link() : SpellScriptLoader("spell_warl_soul_link") { }

        class spell_warl_soul_link_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_soul_link_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!target->HasAura(WARLOCK_SOUL_LINK_DUMMY_AURA))
                            caster->CastSpell(caster, WARLOCK_SOUL_LINK_DUMMY_AURA, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_soul_link_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_soul_link_SpellScript();
        }
};

// Called by Hand of Gul'dan - 47960,
// Molten Core - 122351
class spell_warl_molten_core_dot: public SpellScriptLoader
{
    public:
        spell_warl_molten_core_dot() : SpellScriptLoader("spell_warl_molten_core_dot") { }

        class spell_warl_molten_core_dot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_molten_core_dot_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                l_Caster->ModifyPower(POWER_DEMONIC_FURY, 2 * l_Caster->GetPowerCoeff(POWER_DEMONIC_FURY));
                l_Caster->CastSpell(l_Target, 190187, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                l_Caster->ModifyPower(POWER_DEMONIC_FURY, 2 * l_Caster->GetPowerCoeff(POWER_DEMONIC_FURY));
                if (l_Target->HasAura(190187, l_Caster->GetGUID()))
                    l_Target->RemoveAura(190187, l_Caster->GetGUID());
            }

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(WARLOCK_MOLTEN_CORE_AURA))
                {
                    const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(WARLOCK_MOLTEN_CORE_AURA);
                    if (l_SpellInfo != nullptr && roll_chance_i(l_SpellInfo->Effects[EFFECT_0].BasePoints))
                        l_Caster->CastSpell(l_Caster, WARLOCK_MOLTEN_CORE, true);
                }

                uint8 l_StackAmount = p_AurEff->GetBase()->GetStackAmount();

                /// Generates 2 Demonic Fury every time it deals damage.
                l_Caster->ModifyPower(POWER_DEMONIC_FURY, 2 * l_Caster->GetPowerCoeff(POWER_DEMONIC_FURY) * l_StackAmount);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_molten_core_dot_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warl_molten_core_dot_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_molten_core_dot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_molten_core_dot_AuraScript();
        }
};

/// Last Update 6.2.3
/// Molten Core - 122355
class spell_warl_molten_core : public SpellScriptLoader
{
    public:
        spell_warl_molten_core() : SpellScriptLoader("spell_warl_molten_core") { }

        class spell_warl_molten_core_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_molten_core_AuraScript);

            enum eSpells
            {
                MoltenCoreVisual = 126090
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                l_Target->CastSpell(l_Target, eSpells::MoltenCoreVisual, true);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                l_Target->RemoveAura(eSpells::MoltenCoreVisual);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_warl_molten_core_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_warl_molten_core_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_molten_core_AuraScript();
        }
};

// Called by Shadow Bolt - 686 and Soul Fire - 6353
// Decimate - 108869
class spell_warl_decimate: public SpellScriptLoader
{
    public:
        spell_warl_decimate() : SpellScriptLoader("spell_warl_decimate") { }

        class spell_warl_decimate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_decimate_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(WARLOCK_MOLTEN_CORE_AURA);
                        if (l_Caster->HasAura(WARLOCK_MOLTEN_CORE_AURA) && l_SpellInfo != nullptr && l_Target->GetHealthPct() < l_SpellInfo->Effects[EFFECT_1].BasePoints)
                            l_Caster->CastSpell(l_Caster, WARLOCK_MOLTEN_CORE, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_decimate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_decimate_SpellScript();
        }
};

// Called by Shadow Bolt - 686, Soul Fire - 6353, Touch of Chaos - 103964 and Demonic Slash - 114175
// Soul Fire (metamorphosis) - 104027
// Demonic Call - 114925
class spell_warl_demonic_call: public SpellScriptLoader
{
    public:
        spell_warl_demonic_call() : SpellScriptLoader("spell_warl_demonic_call") { }

        class spell_warl_demonic_call_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_demonic_call_SpellScript);

            void HandleOnHit()
            {
                if (!GetHitUnit())
                    return;

                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(WARLOCK_DEMONIC_CALL) && !caster->HasAura(WARLOCK_DISRUPTED_NETHER))
                    {
                        caster->CastSpell(caster, WARLOCK_WILD_IMP_SUMMON, true);
                        caster->RemoveAura(WARLOCK_DEMONIC_CALL);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_demonic_call_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_demonic_call_SpellScript();
        }
};

// Touch of Chaos - 103964
class spell_warl_touch_of_chaos: public SpellScriptLoader
{
    public:
        spell_warl_touch_of_chaos() : SpellScriptLoader("spell_warl_touch_of_chaos") { }

        class spell_warl_touch_of_chaos_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_touch_of_chaos_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* corruption = target->GetAura(WARLOCK_CORRUPTION, caster->GetGUID()))
                        {
                            corruption->SetDuration(corruption->GetDuration() + 6 * IN_MILLISECONDS); // Not sure for the 6 seconds
                            corruption->SetNeedClientUpdateForTargets();
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_touch_of_chaos_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_touch_of_chaos_SpellScript();
        }
};

// Immolation Aura - 104025
class spell_warl_immolation_aura: public SpellScriptLoader
{
    public:
        spell_warl_immolation_aura() : SpellScriptLoader("spell_warl_immolation_aura") { }

        class spell_warl_immolation_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_immolation_aura_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, WARLOCK_HELLFIRE_DAMAGE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_immolation_aura_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_immolation_aura_AuraScript();
        }
};

enum SpellsDarkBargain
{
    WARLOCK_DARK_BARGAIN_DOT = 110914
};

// Dark Bargain - 110913
class spell_warl_dark_bargain: public SpellScriptLoader
{
    public:
        spell_warl_dark_bargain() : SpellScriptLoader("spell_warl_dark_bargain") { }

        class spell_warl_dark_bargain_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_dark_bargain_AuraScript);

            uint32 absorbedAmount;

            bool Load()
            {
                absorbedAmount = 0;
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool & /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* auraEffect, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                absorbedAmount += dmgInfo.GetDamage();
                absorbAmount = dmgInfo.GetDamage();
                auraEffect->SetAmount(absorbAmount + absorbedAmount);
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                uint32 l_Bp0 = CalculatePct(absorbedAmount, aurEff->GetBaseAmount()) / (sSpellMgr->GetSpellInfo(WARLOCK_DARK_BARGAIN_DOT)->GetDuration() / IN_MILLISECONDS);
                if (l_Bp0)
                    GetTarget()->CastCustomSpell(WARLOCK_DARK_BARGAIN_DOT, SPELLVALUE_BASE_POINT0, l_Bp0, GetTarget(), true);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_dark_bargain_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_warl_dark_bargain_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectRemove += AuraEffectRemoveFn(spell_warl_dark_bargain_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_dark_bargain_AuraScript();
        }
};

// Dark Regeneration - 108359
class spell_warl_dark_regeneration: public SpellScriptLoader
{
    public:
        spell_warl_dark_regeneration() : SpellScriptLoader("spell_warl_dark_regeneration") { }

        class spell_warl_dark_regeneration_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_dark_regeneration_AuraScript);

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget())
                    if (Guardian* pet = GetTarget()->GetGuardianPet())
                        pet->CastSpell(pet, WARLOCK_DARK_REGENERATION, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_dark_regeneration_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_dark_regeneration_AuraScript();
        }
};

/// Called by Haunt - 48181, Drain soul - 103103,
/// Shadow Bolt - 686, Soul Fire - 6353, Soul Fire (Metamorphosis) - 104027, Touch of Chaos - 103964,
/// Soul Fire - 6353, Soul Fire (Metamorphosis) - 104027, Incinerate - 29722, ShadowBurn - 17877, Chaos bolt - 116858
/// Soul Leech - 108370
class spell_warl_soul_leech: public SpellScriptLoader
{
    public:
        spell_warl_soul_leech() : SpellScriptLoader("spell_warl_soul_leech") { }

        class spell_warl_soul_leech_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_soul_leech_SpellScript);

            enum eSpells
            {
                DrainSoul = 103103
            };

            int32 l_SpellID[12] = {48181 , 103103, 686, 6353, 104027, 103964, 6353, 104027, 29722, 17877, 116858, 108370};
            void HandleAfterHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(WARLOCK_SOUL_LEECH_AURA);

                if (l_Player == nullptr || l_SpellInfo == nullptr)
                    return;

                if (!l_Player->HasAura(WARLOCK_SOUL_LEECH_AURA))
                    return;

                int32 l_Bp = 0;
                
                for (int i = 0; i < 12; ++i)
                    l_Bp += l_Player->GetDamageDoneInPastSecsBySpell(l_SpellInfo->Effects[EFFECT_0].BasePoints, l_SpellID[i]);
                /// Affliction - 30%
                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SpecIndex::SPEC_WARLOCK_AFFLICTION)
                    l_Bp = CalculatePct(l_Bp, 30);
                /// Demonology and Destruction - 15%
                else
                    l_Bp = CalculatePct(l_Bp, 15);

                if (Pet* l_Pet = l_Player->GetPet())
                    l_Player->CastCustomSpell(l_Pet, WARLOCK_SOUL_LEECH_HEAL, &l_Bp, NULL, NULL, true);

                l_Player->CastCustomSpell(l_Player, WARLOCK_SOUL_LEECH_HEAL, &l_Bp, NULL, NULL, true);
            }

            void Register() override
            {
                if (m_scriptSpellId != eSpells::DrainSoul)
                    AfterHit += SpellHitFn(spell_warl_soul_leech_SpellScript::HandleAfterHit);
            }
        };

        class spell_warl_soul_leech_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_soul_leech_AuraScript);

            enum eSpells
            {
                DrainSoul = 103103
            };

            int32 l_SpellID[12] = { 48181 , 103103, 686, 6353, 104027, 103964, 6353, 104027, 29722, 17877, 116858, 108370 };
            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (GetCaster() == nullptr)
                    return;

                Player* l_Player = GetCaster()->ToPlayer();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(WARLOCK_SOUL_LEECH_AURA);

                if (l_Player == nullptr || l_SpellInfo == nullptr)
                    return;

                if (!l_Player->HasAura(WARLOCK_SOUL_LEECH_AURA))
                    return;

                int l_Bp = 0;
                for (int i = 0; i < 12; ++i)
                    l_Bp += l_Player->GetDamageDoneInPastSecsBySpell(l_SpellInfo->Effects[EFFECT_0].BasePoints, l_SpellID[i]);
                /// Affliction - 30%
                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SpecIndex::SPEC_WARLOCK_AFFLICTION)
                    l_Bp = CalculatePct(l_Bp, 30);
                /// Demonology and Destruction - 15%
                else
                    l_Bp = CalculatePct(l_Bp, 15);

                if (Pet* l_Pet = l_Player->GetPet())
                    l_Player->CastCustomSpell(l_Pet, WARLOCK_SOUL_LEECH_HEAL, &l_Bp, NULL, NULL, true);

                l_Player->CastCustomSpell(l_Player, WARLOCK_SOUL_LEECH_HEAL, &l_Bp, NULL, NULL, true);
            }

            void Register() override
            {
                if (m_scriptSpellId == eSpells::DrainSoul)
                    AfterEffectPeriodic += AuraEffectPeriodicFn(spell_warl_soul_leech_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warl_soul_leech_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_warl_soul_leech_AuraScript();
        }
};

// Sacrificial Pact - 108416
class spell_warl_sacrificial_pact: public SpellScriptLoader
{
    public:
        spell_warl_sacrificial_pact() : SpellScriptLoader("spell_warl_sacrificial_pact") { }

        class spell_warl_sacrificial_pact_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_sacrificial_pact_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                {
                    int32 sacrifiedHealth = 0;

                    if (Guardian* guardian = caster->GetGuardianPet())
                    {
                        sacrifiedHealth = guardian->CountPctFromCurHealth(GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                        guardian->ModifyHealth(-sacrifiedHealth);
                    }
                    else
                    {
                        sacrifiedHealth = caster->CountPctFromCurHealth(GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                        caster->ModifyHealth(-sacrifiedHealth);
                    }

                    amount = CalculatePct(sacrifiedHealth, aurEff->GetBaseAmount());
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_sacrificial_pact_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_sacrificial_pact_AuraScript();
        }
};

// Hand of Gul'Dan - 143381
class spell_warl_hand_of_guldan: public SpellScriptLoader
{
    public:
        spell_warl_hand_of_guldan() : SpellScriptLoader("spell_warl_hand_of_guldan") { }

        class spell_warl_hand_of_guldan_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_hand_of_guldan_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, WARLOCK_HAND_OF_GULDAN_DAMAGE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_hand_of_guldan_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_hand_of_guldan_SpellScript();
        }
};

// Hand of Gul'Dan (damage) - 86040
class spell_warl_hand_of_guldan_damage: public SpellScriptLoader
{
    public:
        spell_warl_hand_of_guldan_damage() : SpellScriptLoader("spell_warl_hand_of_guldan_damage") { }

        class spell_warl_hand_of_guldan_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_hand_of_guldan_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, WARLOCK_SHADOWFLAME, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_hand_of_guldan_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_hand_of_guldan_damage_SpellScript();
        }
};

/// last Update 6.2.3
/// Call by Hand of Gul'Dan - 143381, Chaos Wave - 124915
class spell_warl_t17_Demonology_2p : public SpellScriptLoader
{
    public:
        spell_warl_t17_Demonology_2p() : SpellScriptLoader("spell_warl_t17_Demonology_2p") { }

        class  spell_warl_t17_Demonology_2p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_t17_Demonology_2p_SpellScript);

            enum eSpells
            {
                T17Demonology2P = 165450,
                InnerDemon = 166862
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                /// Hand of Gul'dan has a 50% chance empower your inner demon, temporarily bringing your Metamorphosis form out even while you are not transformed.
                if (!l_Player->HasSpellCooldown(eSpells::InnerDemon) && l_Player->HasAura(eSpells::T17Demonology2P) && roll_chance_i(50))
                {
                    l_Player->CastSpell(l_Player, eSpells::InnerDemon, true);

                    /// 45 seconds of internal cooldown
                    l_Player->AddSpellCooldown(eSpells::InnerDemon, 0, 45 * TimeConstants::IN_MILLISECONDS);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_t17_Demonology_2p_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new  spell_warl_t17_Demonology_2p_SpellScript();
        }
};

// Twilight Ward - 6229 and Twilight Ward (Metamorphosis) - 104048
class spell_warl_twilight_ward_s12: public SpellScriptLoader
{
    public:
        spell_warl_twilight_ward_s12() : SpellScriptLoader("spell_warl_twilight_ward_s12") { }

        class spell_warl_twilight_ward_s12_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_twilight_ward_s12_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(WARLOCK_ITEM_S12_TIER_4))
                    {
                        if (GetSpellInfo()->Id == WARLOCK_TWILIGHT_WARD)
                        {
                            if (caster->HasAura(GetSpellInfo()->Id))
                                caster->RemoveAura(GetSpellInfo()->Id);

                            caster->CastSpell(caster, WARLOCK_TWILIGHT_WARD_S12, true);
                        }
                        else if (GetSpellInfo()->Id == WARLOCK_TWILIGHT_WARD_METAMORPHOSIS)
                        {
                            if (caster->HasAura(GetSpellInfo()->Id))
                                caster->RemoveAura(GetSpellInfo()->Id);

                            caster->CastSpell(caster, WARLOCK_TWILIGHT_WARD_METAMORPHOSIS_S12, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_twilight_ward_s12_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_twilight_ward_s12_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Hellfire - 1949
class spell_warl_hellfire_periodic: public SpellScriptLoader
{
    public:
        spell_warl_hellfire_periodic() : SpellScriptLoader("spell_warl_hellfire_periodic") { }

        class spell_warl_hellfire_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_hellfire_periodic_AuraScript);

            void OnTickOnCaster(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->EnergizeBySpell(l_Caster, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_2].BasePoints, POWER_DEMONIC_FURY);
            }

            void OnTickOnAreaEnemy(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, WARLOCK_HELLFIRE_DAMAGE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_hellfire_periodic_AuraScript::OnTickOnAreaEnemy, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_hellfire_periodic_AuraScript::OnTickOnCaster, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_hellfire_periodic_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Hellfire (area enemy damage)- 5857
class spell_warl_hellfire_damage_area : public SpellScriptLoader
{
    public:
        spell_warl_hellfire_damage_area() : SpellScriptLoader("spell_warl_hellfire_damage_area") { }

        class spell_warl_hellfire_damage_area_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_hellfire_damage_area_SpellScript);

            enum eSpells
            {
                hellfireTick = 1949
            };

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::hellfireTick);

                if (l_SpellInfo == nullptr)
                    return;

                l_Caster->EnergizeBySpell(l_Caster, l_SpellInfo->Id, l_SpellInfo->Effects[EFFECT_3].BasePoints, POWER_DEMONIC_FURY);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warl_hellfire_damage_area_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_hellfire_damage_area_SpellScript();
        }
};

// Demonic Leap (jump) - 109167
class spell_warl_demonic_leap_jump: public SpellScriptLoader
{
    public:
        spell_warl_demonic_leap_jump() : SpellScriptLoader("spell_warl_demonic_leap_jump") { }

        class spell_warl_demonic_leap_jump_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_demonic_leap_jump_SpellScript);

            SpellCastResult CheckElevation()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return SPELL_FAILED_DONT_REPORT;

                Position pos;
                caster->GetFirstCollisionPosition(pos, GetSpellInfo()->Effects[EFFECT_0].CalcRadius(caster), caster->GetOrientation());

                if (!caster->IsWithinLOS(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ()))
                {
                    caster->RemoveAura(WARLOCK_METAMORPHOSIS);
                    return SPELL_FAILED_NOPATH;
                }
                else if (caster->HasAuraType(SPELL_AURA_MOD_ROOT) || caster->HasAuraType(SPELL_AURA_MOD_ROOT_2))
                {
                    caster->RemoveAura(WARLOCK_METAMORPHOSIS);
                    return SPELL_FAILED_ROOTED;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_demonic_leap_jump_SpellScript::CheckElevation);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_demonic_leap_jump_SpellScript();
        }
};

// Demonic Leap - 109151
class spell_warl_demonic_leap: public SpellScriptLoader
{
    public:
        spell_warl_demonic_leap() : SpellScriptLoader("spell_warl_demonic_leap") { }

        class spell_warl_demonic_leap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_demonic_leap_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->HasAura(WARLOCK_DARK_APOTHEOSIS))
                        caster->CastSpell(caster, WARLOCK_METAMORPHOSIS, true);
                    caster->CastSpell(caster, WARLOCK_DEMONIC_LEAP_JUMP, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warl_demonic_leap_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_demonic_leap_SpellScript();
        }
};

// Soul Swap : Soulburn - 119678
class spell_warl_soul_swap_soulburn: public SpellScriptLoader
{
    public:
        spell_warl_soul_swap_soulburn() : SpellScriptLoader("spell_warl_soul_swap_soulburn") { }

        class spell_warl_soul_swap_soulburn_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_soul_swap_soulburn_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Apply instantly corruption, unstable affliction and agony on the target
                        caster->CastSpell(target, WARLOCK_CORRUPTION, true);
                        caster->CastSpell(target, WARLOCK_UNSTABLE_AFFLICTION, true);
                        caster->CastSpell(target, WARLOCK_AGONY, true);
                        caster->RemoveAura(WARLOCK_SOULBURN_AURA);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_soul_swap_soulburn_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_soul_swap_soulburn_SpellScript();
        }
};

// Soul Swap - 86121 or Soul Swap : Exhale - 86213
class spell_warl_soul_swap: public SpellScriptLoader
{
    public:
        spell_warl_soul_swap() : SpellScriptLoader("spell_warl_soul_swap") { }

        class spell_warl_soul_swap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_soul_swap_SpellScript);

            SpellCastResult CheckConditions()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (!l_Caster || !l_Target)
                    return SPELL_FAILED_DONT_REPORT;

                /// Target should have at least one DOT from the list
                if (GetSpellInfo()->Id == WARLOCK_SOUL_SWAP)
                {
                    if (!l_Target->HasAura(WARLOCK_AGONY, l_Caster->GetGUID()) && !l_Target->HasAura(WARLOCK_SPELL_CORRUPTION_DOT, l_Caster->GetGUID()) &&
                        !l_Target->HasAura(WARLOCK_UNSTABLE_AFFLICTION, l_Caster->GetGUID()) && !l_Caster->HasAura(WARLOCK_SOULBURN_AURA))
                        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                }

                /// Can't copy DOTs to the same target
                if (GetSpellInfo()->Id == WARLOCK_SOUL_SWAP_EXHALE)
                {
                    if (Unit* l_SavedTarget = l_Caster->GetSoulSwapDotTarget())
                    {
                        if (l_SavedTarget == l_Target)
                            return SPELL_FAILED_BAD_TARGETS;
                    }
                }
                    

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                bool l_HasSoulburnAura = l_Caster->HasAura(WARLOCK_SOULBURN_AURA);

                /// Soul Swap launch spell
                if (GetSpellInfo()->Id == WARLOCK_SOUL_SWAP)
                {
                    /// Soulburn: Applies Corruption, Unstable Affliction, and Agony without requiring a previous copy.
                    if (l_HasSoulburnAura)
                    {
                        l_Caster->CastSpell(l_Target, WARLOCK_SOUL_SWAP_VISUAL, true);
                        l_Caster->CastSpell(l_Target, WARLOCK_AGONY, true);
                        l_Caster->CastSpell(l_Target, WARLOCK_SPELL_CORRUPTION, true);
                        l_Caster->CastSpell(l_Target, WARLOCK_UNSTABLE_AFFLICTION, true);

                        l_Caster->RemoveAurasDueToSpell(WARLOCK_SOULBURN_AURA);
                    }
                    /// Simple cast of Soul Swap, without Soulburn
                    else
                    {
                        l_Target->CastSpell(l_Caster, WARLOCK_SOUL_SWAP_VISUAL, true);
                        // Soul Swap override spell
                        l_Caster->CastSpell(l_Caster, WARLOCK_SOUL_SWAP_AURA, true);
                        l_Caster->RemoveSoulSwapDOT(l_Target);

                        /// Store Soul Swap target GUID
                        l_Caster->SetSoulSwapDotTarget(l_Target->GetGUID());
                        /// Save that at the moment we don't need refresh duration
                        l_Caster->SetSoulSwapRefreshDuration(false);
                    }
                }
                /// Soul Swap finish (exhale) spell
                else if (GetSpellInfo()->Id == WARLOCK_SOUL_SWAP_EXHALE)
                {
                    l_Caster->ApplySoulSwapDOT(l_Caster, l_Target);
                    l_Caster->RemoveAurasDueToSpell(WARLOCK_SOUL_SWAP_AURA);

                    if (l_Caster->HasAura(WARLOCK_GLYPH_OF_SOUL_SWAP) && l_Caster->ToPlayer())
                        l_Caster->ToPlayer()->AddSpellCooldown(WARLOCK_SOUL_SWAP, 0, 30 * IN_MILLISECONDS);

                    /// Set Soul Swap target GUID to NULL
                    l_Caster->RemoveSoulSwapDotTarget();
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_soul_swap_SpellScript::CheckConditions);
                OnHit += SpellHitFn(spell_warl_soul_swap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_soul_swap_SpellScript();
        }
};

enum DrainSoulSpells
{
    SPELL_WARL_IMPROVED_DRAIN_SOUL = 157077
};

// Drain Soul - 103103
class spell_warl_drain_soul: public SpellScriptLoader
{
    public:
        spell_warl_drain_soul() : SpellScriptLoader("spell_warl_drain_soul") { }

        enum eSpells
        {
            GrimoireOfSacrifice         = 108503,
            AgonyDoT                    = 980,
            AgonyTriggered              = 131737,
            SeedOfCorruptionDoT         = 27243,
            SeedOfCorruptionTriggered   = 132566,
            UnstableAfflictionDoT       = 30108,
            UnstableAfflictionTriggered = 131736,
            CorruptionDoT               = 146739,
            CorruptionTriggered         = 131740
        };

        class spell_warl_drain_soul_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_drain_soul_AuraScript)

            bool m_UnderImproved = false;

            void HandlePeriodicDamage(AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                std::list<Unit*> l_TargetList;

                p_AurEff->GetTargetList(l_TargetList);
                for (auto l_Target : l_TargetList)
                {
                    if (l_Caster->getLevel() >= 92 && l_Caster->HasSpell(DrainSoulSpells::SPELL_WARL_IMPROVED_DRAIN_SOUL) && l_Target->GetHealthPct() >= 20)
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(DrainSoulSpells::SPELL_WARL_IMPROVED_DRAIN_SOUL);
                        if (l_SpellInfo != nullptr && m_UnderImproved)
                        {
                            m_UnderImproved = false;
                            p_AurEff->SetAmount(p_AurEff->GetAmount() - CalculatePct(p_AurEff->GetAmount(), l_SpellInfo->Effects[SpellEffIndex::EFFECT_0].BasePoints));
                        }
                    }

                    if (l_Caster->getLevel() >= 92 && l_Caster->HasSpell(DrainSoulSpells::SPELL_WARL_IMPROVED_DRAIN_SOUL) && l_Target->GetHealthPct() < 20)
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(DrainSoulSpells::SPELL_WARL_IMPROVED_DRAIN_SOUL);
                        if (l_SpellInfo != nullptr && !m_UnderImproved)
                        {
                            m_UnderImproved = true;
                            p_AurEff->SetAmount(p_AurEff->GetAmount() + CalculatePct(p_AurEff->GetAmount(), l_SpellInfo->Effects[SpellEffIndex::EFFECT_0].BasePoints));
                        }
                    }

                    /// Associate DoT spells to their damage spells
                    std::map<uint32, uint32> l_DotAurasMap =
                    {
                        { eSpells::AgonyDoT,                eSpells::AgonyTriggered                 },
                        { eSpells::SeedOfCorruptionDoT,     eSpells::SeedOfCorruptionTriggered      },
                        { eSpells::UnstableAfflictionDoT,   eSpells::UnstableAfflictionTriggered    },
                        { eSpells::CorruptionDoT,           eSpells::CorruptionTriggered            }
                    };

                    for (auto l_DotAura = l_DotAurasMap.begin(); l_DotAura != l_DotAurasMap.end(); ++l_DotAura)
                    {
                        if (Aura* l_Aura = l_Target->GetAura((*l_DotAura).first, l_Caster->GetGUID()))
                        {
                            if (AuraEffect* l_AuraEffect = l_Aura->GetEffect(l_Aura->GetEffectIndexByType(AuraType::SPELL_AURA_PERIODIC_DAMAGE)))
                            {
                                int32 l_Bp0 = CalculatePct(l_AuraEffect->GetAmount(), GetSpellInfo()->Effects[SpellEffIndex::EFFECT_2].BasePoints);

                                SpellInfo const* l_BaseSpell = sSpellMgr->GetSpellInfo((*l_DotAura).first);
                                if (l_BaseSpell == nullptr)
                                    continue;

                                /// Apply bonuses from the base DoT
                                l_Bp0 = l_Caster->SpellDamageBonusDone(l_Target, l_BaseSpell, l_Bp0, 0, DamageEffectType::DOT);
                                l_Bp0 = l_Target->SpellDamageBonusTaken(l_Caster, l_BaseSpell, l_Bp0, DamageEffectType::DOT);

                                if (Aura* l_AuraGoS = l_Caster->GetAura(eSpells::GrimoireOfSacrifice))
                                {
                                    if (l_AuraGoS->GetEffect(SpellEffIndex::EFFECT_4))
                                        AddPct(l_Bp0, l_AuraGoS->GetEffect(SpellEffIndex::EFFECT_4)->GetAmount());
                                }

                                l_Caster->CastCustomSpell(l_Target, (*l_DotAura).second, &l_Bp0, nullptr, nullptr, true);

                                /// Agony stack refresh
                                if ((*l_DotAura).second == eSpells::AgonyTriggered)
                                {
                                    if (Aura* l_Agony = l_Target->GetAura((*l_DotAura).first, GetCaster()->GetGUID()))
                                    {
                                        if (AuraEffect const* l_AgonyDmgEffect = l_Target->GetAuraEffect((*l_DotAura).first, 0))
                                            l_Agony->ModStackAmount(l_AgonyDmgEffect->GetBaseAmount());
                                    }
                                }

                                /// Glyph of Siphon Life - 56218
                                if ((*l_DotAura).second == eSpells::CorruptionTriggered)
                                {
                                    if (l_Caster->HasAura(WarlockSpells::WARLOCK_GLYPH_OF_SIPHON_LIFE))
                                        l_Caster->CastSpell(l_Caster, WarlockSpells::WARLOCK_SPELL_SYPHON_LIFE, true);
                                }
                            }
                        }
                    }
                }
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        if (l_Caster->IsPlayer())
                        {
                            if (l_Caster->ToPlayer()->isHonorOrXPTarget(l_Target))
                                l_Caster->ModifyPower(POWER_SOUL_SHARDS, 1 * l_Caster->GetPowerCoeff(POWER_SOUL_SHARDS));
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_warl_drain_soul_AuraScript::HandlePeriodicDamage, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectRemove += AuraEffectApplyFn(spell_warl_drain_soul_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_warl_drain_soul_AuraScript();
        }
};

/// Last Update 6.2.3
/// Demonic Gateway - 111771
class spell_warl_demonic_gateway: public SpellScriptLoader
{
    public:
        spell_warl_demonic_gateway() : SpellScriptLoader("spell_warl_demonic_gateway") { }

        class spell_warl_demonic_gateway_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_demonic_gateway_SpellScript);

            SpellCastResult CheckElevation()
            {
                if (!GetCaster() || !GetCaster()->ToPlayer())
                    return SPELL_FAILED_DONT_REPORT;

                Player* player = GetCaster()->ToPlayer();

                WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
                if (!dest)
                    return SPELL_FAILED_DONT_REPORT;

                if (dest->GetPositionZ() > player->GetPositionZ() + 5.0f || dest->GetPositionZ() < player->GetPositionZ() - 5.0f)
                    return SPELL_FAILED_NOPATH;

                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, WARLOCK_SPAWN_PURPLE_DEMONIC_GATEWAY, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_demonic_gateway_SpellScript::CheckElevation);
                AfterCast += SpellCastFn(spell_warl_demonic_gateway_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_demonic_gateway_SpellScript();
        }
};

// Rain of Fire - 104232
class spell_warl_rain_of_fire: public SpellScriptLoader
{
    public:
        spell_warl_rain_of_fire() : SpellScriptLoader("spell_warl_rain_of_fire") { }

        class spell_warl_rain_of_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_rain_of_fire_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (!GetTarget() || GetTarget()->GetTypeId() == TYPEID_UNIT)
                    return;

                // Prevent multiple ticks bug
                if (GetTarget() != GetCaster())
                    return;

                if (Unit* caster = GetCaster())
                    if (DynamicObject* dynObj = caster->GetDynObject(WARLOCK_RAIN_OF_FIRE))
                        caster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), WARLOCK_RAIN_OF_FIRE_TRIGGERED, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_rain_of_fire_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_rain_of_fire_AuraScript();
        }
};

// Rain of Fire - 104232 and Rain of Fire - 5740
class spell_warl_rain_of_fire_despawn: public SpellScriptLoader
{
    public:
        spell_warl_rain_of_fire_despawn() : SpellScriptLoader("spell_warl_rain_of_fire_despawn") { }

        class spell_warl_rain_of_fire_despawn_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_rain_of_fire_despawn_SpellScript);

            void HandleAreaAura(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    // Casting a second Rain of Fire will replace the old Rain of Fire
                    if (caster->GetDynObject(GetSpellInfo()->Id))
                        caster->RemoveDynObject(GetSpellInfo()->Id);
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_warl_rain_of_fire_despawn_SpellScript::HandleAreaAura, EFFECT_0, SPELL_EFFECT_PERSISTENT_AREA_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_rain_of_fire_despawn_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Ember Tap - 114635
class spell_warl_ember_tap: public SpellScriptLoader
{
    public:
        spell_warl_ember_tap() : SpellScriptLoader("spell_warl_ember_tap") { }

        class spell_warl_ember_tap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_ember_tap_SpellScript);

            int32 m_RemainingAmout = 0;

            enum eSpells
            {
                GlyphOfEmberTap   = 63304,
                MasteryEmberstorm = 77220,
                EnhancedEmberTap  = 157121,
                SearingFlames     = 174848,
                Hex               = 51514
            };

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(Hex))
                        return SPELL_FAILED_SILENCED;
                    else
                        return SPELL_CAST_OK;
                }
                else
                    return SPELL_FAILED_SUCCESS;
            }

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster->HasAura(eSpells::GlyphOfEmberTap))
                    return;

                if (AuraEffect* l_PreviousEffect = l_Caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_2))
                    m_RemainingAmout += (l_PreviousEffect->GetAmount() * (l_PreviousEffect->GetBase()->GetDuration() / l_PreviousEffect->GetAmplitude()));
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                if (AuraEffect* l_SearingFlames = l_Caster->GetAuraEffect(eSpells::SearingFlames, EFFECT_0))
                    l_Caster->ModifyPower(POWER_BURNING_EMBERS, 5);

                /// No instant heal with Glyph of Ember tap
                if (Aura* l_GlyphOfEmberTap = l_Caster->GetAura(eSpells::GlyphOfEmberTap))
                {
                    SetHitHeal(0);
                    return;
                }

                if (!GetSpellInfo() || !GetSpellInfo()->Effects[0].BasePoints)
                    return;

                int32 l_HealAmount = l_Caster->CountPctFromMaxHealth(GetSpellInfo()->Effects[0].BasePoints);
                
                if (AuraEffect* l_MasteryEmberstorm = l_Caster->GetAuraEffect(eSpells::MasteryEmberstorm, EFFECT_0))
                {
                    float l_MasteryPct = l_MasteryEmberstorm->GetSpellEffectInfo()->BonusMultiplier * l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY);
                    l_HealAmount += CalculatePct(l_HealAmount, l_MasteryPct);
                }

                if (AuraEffect* l_SearingFlames = l_Caster->GetAuraEffect(eSpells::SearingFlames, EFFECT_0))
                    l_HealAmount += CalculatePct(l_HealAmount, l_SearingFlames->GetAmount());

                l_HealAmount = l_Caster->SpellHealingBonusDone(l_Caster, GetSpellInfo(), l_HealAmount, EFFECT_0, HEAL);
                l_HealAmount = l_Caster->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_HealAmount, HEAL);

                SetHitHeal(l_HealAmount);
                /// Your Ember Tap also heals your pet demon for 20% as much.
                if (AuraEffect* l_EnhancedEmberTap = l_Caster->GetAuraEffect(eSpells::EnhancedEmberTap, EFFECT_0))
                {
                    Player* l_Player = l_Caster->ToPlayer();
                    if (!l_Player)
                        return;

                    if (Pet* l_Pet = l_Player->GetPet())
                        l_Player->HealBySpell(l_Pet, sSpellMgr->GetSpellInfo(139967), CalculatePct(l_HealAmount, l_EnhancedEmberTap->GetAmount()), false, false);
                }
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster->HasAura(eSpells::GlyphOfEmberTap))
                    return;

                if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_2))
                    l_AuraEffect->ChangeAmount(l_AuraEffect->GetAmount() + (m_RemainingAmout / (l_AuraEffect->GetBase()->GetDuration() / l_AuraEffect->GetAmplitude())));
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_ember_tap_SpellScript::CheckCast);
                BeforeHit += SpellHitFn(spell_warl_ember_tap_SpellScript::HandleBeforeHit);
                OnHit += SpellHitFn(spell_warl_ember_tap_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_warl_ember_tap_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_ember_tap_SpellScript();
        }
};


/// last update : 6.1.2 19802
/// Ember Tap - 114635
/// With Glyph of Ember Tap - 63304
class spell_warl_ember_tap_glyph : public SpellScriptLoader
{
public:
    spell_warl_ember_tap_glyph() : SpellScriptLoader("spell_warl_ember_tap_glyph") { }

    enum eSpells
    {
        MasteryEmberstorm = 77220,
        GlyphOfEmberTap = 63304,
        EmberTap = 114635,
        SearingFlames = 174848
    };

    class spell_warl_ember_tap_glyph_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_ember_tap_glyph_AuraScript);

        void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
        {
            Unit* l_Caster = GetCaster();
            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfEmberTap);

            if (l_Caster == nullptr || l_SpellInfo == nullptr)
                return;

            if (!l_Caster->HasAura(eSpells::GlyphOfEmberTap))
            {
                p_Amount = 0;
                return;
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_ember_tap_glyph_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_ember_tap_glyph_AuraScript();
    }

    class spell_warl_ember_tap_glyph_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_ember_tap_glyph_SpellScript);

        uint32 m_PreviousTotalHeal = 0;

        void HitTarget(SpellEffIndex)
        {
            Unit* l_Caster = GetCaster();

            if (!l_Caster)
                return;

            if (!l_Caster->HasAura(eSpells::GlyphOfEmberTap))
            {
                PreventHitAura();
                return;
            }
        }

        void HandleBeforeHit()
        {
            Player* l_Caster = GetCaster()->ToPlayer();
            SpellInfo const* l_SpellInfo = GetSpellInfo();
            if (!l_Caster || !l_SpellInfo)
                return;

            if (!l_Caster->HasAura(eSpells::GlyphOfEmberTap))
                return;

            if (AuraEffect* l_PreviousEmberTap = l_Caster->GetAuraEffect(l_SpellInfo->Id, EFFECT_2, l_Caster->GetGUID()))
            {
                int32 l_Duration = 0;
                int32 l_MaxDuration = 0;
                int32 l_PeriodicDamage = l_PreviousEmberTap->GetAmount();
                int32 l_Amplitude = l_PreviousEmberTap->GetAmplitude();

                if (Aura* l_EmberTap = l_Caster->GetAura(GetSpellInfo()->Id))
                {
                    l_Duration = l_EmberTap->GetDuration();
                    l_MaxDuration = l_EmberTap->GetMaxDuration();
                }

                if (l_Amplitude && l_Duration && l_MaxDuration)
                {
                    m_PreviousTotalHeal = l_PeriodicDamage * (l_Duration / l_Amplitude + 1);
                    ///m_PreviousTotalHeal = m_PreviousTotalHeal / (l_MaxDuration / l_Amplitude);
                }
            }
        }

        void HandleAfterHit()
        {
            Player* l_Caster = GetCaster()->ToPlayer();
            SpellInfo const* l_SpellInfo = GetSpellInfo();
            SpellInfo const* l_GlyphSpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfEmberTap);
            if (!l_Caster || !l_SpellInfo)
                return;

            Aura* l_Aura = l_Caster->GetAura(l_SpellInfo->Id);
            if (!l_Aura)
                return;

            if (!l_Caster->HasAura(eSpells::GlyphOfEmberTap) || l_GlyphSpellInfo == nullptr)
                return;

            uint8 l_AdditionalTick = 0;
            float l_Pct = l_SpellInfo->Effects[EFFECT_0].BasePoints + l_GlyphSpellInfo->Effects[EFFECT_2].BasePoints;
            if (AuraEffect* l_SearingFlames = l_Caster->GetAuraEffect(eSpells::SearingFlames, EFFECT_0))
            {
                l_Pct += CalculatePct(l_Pct, l_SearingFlames->GetAmount());
            }

            int32 l_TotalHeal = CalculatePct(l_Caster->GetMaxHealth(), l_Pct);

            l_TotalHeal += m_PreviousTotalHeal;

            if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(l_SpellInfo->Id, EFFECT_2))
            {
                l_TotalHeal = l_TotalHeal / (l_Aura->GetMaxDuration() / l_AuraEffect->GetAmplitude());
                l_AuraEffect->SetAmount(l_TotalHeal);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_ember_tap_glyph_SpellScript::HitTarget, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH);
            BeforeHit += SpellHitFn(spell_warl_ember_tap_glyph_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_warl_ember_tap_glyph_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_ember_tap_glyph_SpellScript();
    }
};

/// Last Update 6.2.3
/// Conflagrate - 17962 and Conflagrate (Fire and Brimstone) - 108685
class spell_warl_conflagrate_aura: public SpellScriptLoader
{
    public:
        spell_warl_conflagrate_aura() : SpellScriptLoader("spell_warl_conflagrate_aura") { }

        class spell_warl_conflagrate_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_conflagrate_aura_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Player* l_Player = l_Caster->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                if (!l_Target->HasAura(WARLOCK_IMMOLATE) && !l_Player->HasGlyph(WARLOCK_GLYPH_OF_CONFLAGRATE))
                {
                    if (Aura* conflagrate = l_Target->GetAura(WARLOCK_CONFLAGRATE))
                        l_Target->RemoveAura(WARLOCK_CONFLAGRATE);
                }
                if (!l_Target->HasAura(WARLOCK_IMMOLATE))
                {
                    if (Aura* conflagrate = l_Target->GetAura(WARLOCK_CONFLAGRATE_FIRE_AND_BRIMSTONE))
                        l_Target->RemoveAura(WARLOCK_CONFLAGRATE_FIRE_AND_BRIMSTONE);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_conflagrate_aura_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_conflagrate_aura_SpellScript();
        }
};

enum GlyphofNightmareSpells
{
    SPELL_WARL_GLYPH_OF_NIGHTMARE_AURA      = 56232,
    SPELL_WARL_GLYPH_OF_NIGHTMARE           = 143314
};

/// Call by : Felsteed - 5784, Dreadsteed - 23161
/// Glyph of Nightmares - 56232
class spell_warl_glyph_of_nightmares : public SpellScriptLoader
{
    public:
        spell_warl_glyph_of_nightmares() : SpellScriptLoader("spell_warl_glyph_of_nightmares") { }

        class spell_warl_glyph_of_nightmares_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_glyph_of_nightmares_AuraScript);

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_WARL_GLYPH_OF_NIGHTMARE_AURA))
                        l_Caster->CastSpell(l_Caster, SPELL_WARL_GLYPH_OF_NIGHTMARE, true);
                }
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_WARL_GLYPH_OF_NIGHTMARE))
                        l_Caster->RemoveAura(SPELL_WARL_GLYPH_OF_NIGHTMARE);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_nightmares_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectApplyFn(spell_warl_glyph_of_nightmares_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_glyph_of_nightmares_AuraScript();
        }
};

/// Shadowburn - 29341
class spell_warl_shadowburn: public SpellScriptLoader
{
    public:
        spell_warl_shadowburn() : SpellScriptLoader("spell_warl_shadowburn") { }

        class spell_warl_shadowburn_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_shadowburn_AuraScript);

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode == AURA_REMOVE_BY_DEATH)
                    l_Caster->ModifyPower(POWER_BURNING_EMBERS, 2 * l_Caster->GetPowerCoeff(POWER_BURNING_EMBERS)); ///< Give 2 Burning Ember
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_warl_shadowburn_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_shadowburn_AuraScript();
        }
};

/// Last Upadte 6.2.3
/// Called By : Incinerate - 29722 and Incinerate (Fire and Brimstone) - 114654
/// Conflagrate - 17962 and Conflagrate (Fire and Brimstone) - 108685
/// Burning Embers generate
class spell_warl_burning_embers: public SpellScriptLoader
{
    public:
        spell_warl_burning_embers() : SpellScriptLoader("spell_warl_burning_embers") { }

        class spell_warl_burning_embers_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_burning_embers_SpellScript);

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (!l_Target)
                    return;

                // 10% of a Burning Embers
                float l_BurningEmbersPct = 0.1f;

                if (GetSpell()->IsCritForTarget(l_Target))
                    l_BurningEmbersPct = 0.2f;

                if (Aura* l_CharredRemains = l_Caster->GetAura(SPELL_WARL_CHARRED_REMAINS))
                    AddPct(l_BurningEmbersPct, l_CharredRemains->GetEffect(EFFECT_1)->GetBaseAmount());

                l_Caster->ModifyPower(POWER_BURNING_EMBERS, l_BurningEmbersPct * l_Caster->GetPowerCoeff(POWER_BURNING_EMBERS));
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_warl_burning_embers_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warl_burning_embers_SpellScript();
        }
};

class spell_warl_burning_embers_regen : public PlayerScript
{
    public:
        spell_warl_burning_embers_regen() : PlayerScript("spell_warl_burning_embers_regen")
        {
        }

        struct BuringEmbersData
        {
            uint64 m_LastCombat; ///< Timestamp at when leaving last combat
            uint32 m_RegenTimer; ///< Timer in millisecondes we regenate the burnign embers

            BuringEmbersData()
            {
                m_LastCombat = 0;
                m_RegenTimer = 2000;
            }
        };

        ACE_Based::LockedMap<uint32, BuringEmbersData> m_BurningEmbersData;

        /// Internal script function
        bool CanUseBuringEmbers(Player* p_Player, Powers p_Power = Powers::POWER_BURNING_EMBERS)
        {
            if (m_BurningEmbersData.find(p_Player->GetGUIDLow()) == m_BurningEmbersData.end())
                m_BurningEmbersData[p_Player->GetGUIDLow()] = BuringEmbersData();

            if (p_Player == nullptr
                || p_Player->getClass() != Classes::CLASS_WARLOCK
                || p_Player->GetSpecializationId(p_Player->GetActiveSpec()) != SpecIndex::SPEC_WARLOCK_DESTRUCTION
                || p_Power != POWER_BURNING_EMBERS)
                return false;

            return true;
        }

        /// Override
        void OnLogout(Player * p_Player)
        {
            m_BurningEmbersData.erase(p_Player->GetGUIDLow());
        }

        /// Override
        void OnLeaveCombat(Player* p_Player)
        {
            if (!CanUseBuringEmbers(p_Player))
                return;

            BuringEmbersData& l_BuringEmbersData = m_BurningEmbersData[p_Player->GetGUIDLow()];
            l_BuringEmbersData.m_LastCombat = getMSTime();
        }

        /// Handle regeneration of burning embers
        /// Call at each update tick (100 ms)
        /// Override
        void OnUpdate(Player * p_Player, uint32 p_Diff)
        {
            if (!CanUseBuringEmbers(p_Player))
                return;

            BuringEmbersData& l_BuringEmbersData = m_BurningEmbersData[p_Player->GetGUIDLow()];

            if (l_BuringEmbersData.m_RegenTimer <= p_Diff)
            {
                /// After 25s out of combat...
                if (p_Player->isInCombat() || (l_BuringEmbersData.m_LastCombat != 0 && GetMSTimeDiffToNow(l_BuringEmbersData.m_LastCombat) < (25 * IN_MILLISECONDS)))
                    return;

                int32 l_CurrentPower = p_Player->GetPower(POWER_BURNING_EMBERS);

                /// ...return to one embers if no one
                /// or return to one if more than one
                if (l_CurrentPower < (1 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                    p_Player->SetPower(POWER_BURNING_EMBERS, l_CurrentPower + 1, true);
                else if (l_CurrentPower > (1 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                    p_Player->SetPower(POWER_BURNING_EMBERS, l_CurrentPower - 1, true);

                l_BuringEmbersData.m_RegenTimer = 2 * IN_MILLISECONDS;
            }
            else
                l_BuringEmbersData.m_RegenTimer -= p_Diff;
        }

        /// Override
        void OnModifyPower(Player* p_Player, Powers p_Power, int32 /*p_OldValue*/, int32& p_NewValue, bool /*p_Regen*/, bool p_After)
        {
            if (p_After)
                return;

            if (!CanUseBuringEmbers(p_Player, p_Power))
                return;

            if (p_Player->HasAura(SPELL_WARL_GLYPH_OF_VERDANT_SPHERES))
            {
                if (p_NewValue < (2 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->RemoveAura(123728); ///< 2 shards visual
                    p_Player->RemoveAura(123730); ///< 3 shards visual
                    p_Player->RemoveAura(123731); ///< 4 shards visual
                }
                else if (p_NewValue < (3 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->CastSpell(p_Player, 123728, true); ///< 2 shards visual
                    p_Player->RemoveAura(123730);                ///< 3 shards visual
                    p_Player->RemoveAura(123731);                ///< 4 shards visual
                }
                else if (p_NewValue < (4 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->CastSpell(p_Player, 123728, true); ///< 2 shards visual
                    p_Player->CastSpell(p_Player, 123730, true); ///< 3 shards visual
                    p_Player->RemoveAura(123731);                ///< 4 shards visual
                }
                else if (p_NewValue >= (4 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->CastSpell(p_Player, 123728, true); ///< 2 shards visual
                    p_Player->CastSpell(p_Player, 123730, true); ///< 3 shards visual
                    p_Player->CastSpell(p_Player, 123731, true); ///< 4 shards visual
                }
            }
            else
            {
                if (p_NewValue < (2 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->CastSpell(p_Player, 116856, true); ///< Remove visual for 2,3,4 shards
                }
                else if (p_NewValue < (3 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->CastSpell(p_Player, 116854, true);  ///< 2 shards visual
                    p_Player->RemoveAura(116855);                 ///< 3 shards visual
                    p_Player->RemoveAura(116920);                 ///< 4 shards visual
                }
                else if (p_NewValue < (4 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->CastSpell(p_Player, 116854, true); ///< 2 shards visual
                    p_Player->CastSpell(p_Player, 116855, true); ///< 3 shards visual
                    p_Player->RemoveAura(116920);                ///< 4 shards visual
                }
                else if (p_NewValue >= (4 * p_Player->GetPowerCoeff(POWER_BURNING_EMBERS)))
                {
                    p_Player->CastSpell(p_Player, 116854, true); ///< 2 shards visual
                    p_Player->CastSpell(p_Player, 116855, true); ///< 3 shards visual
                    p_Player->CastSpell(p_Player, 116920, true); ///< 4 shards visual
                }
            }

        }
};

// Drain Life - 689
class spell_warl_drain_life: public SpellScriptLoader
{
    public:
        spell_warl_drain_life() : SpellScriptLoader("spell_warl_drain_life") { }

        class spell_warl_drain_life_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_drain_life_AuraScript);

            enum eSpells
            {
                DrainLifeHeal      = 89653,
                EmpoweredDrainLife = 157069
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                int32 l_Bp0 = l_Caster->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_1].BasePoints) / p_AurEff->GetTotalTicks();
                int32 l_BaseBonus = 0;

                if (Aura* l_EmpoweredDrainLife = l_Caster->GetAura(eSpells::EmpoweredDrainLife))
                    l_BaseBonus += CalculatePct(l_Bp0, l_EmpoweredDrainLife->GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                l_BaseBonus *= p_AurEff->GetTickNumber();
                l_Bp0 += l_BaseBonus;

                l_Caster->CastCustomSpell(l_Caster, eSpells::DrainLifeHeal, &l_Bp0, NULL, NULL, true);

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                // In Demonology spec : Generates 10 Demonic Fury per second
                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                    l_Player->EnergizeBySpell(l_Player, GetSpellInfo()->Id, 10 * l_Player->GetPowerCoeff(POWER_DEMONIC_FURY), POWER_DEMONIC_FURY);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_drain_life_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_drain_life_AuraScript();
        }
};

// Soul Harvest - 101976
class spell_warl_soul_harverst: public SpellScriptLoader
{
    public:
        spell_warl_soul_harverst() : SpellScriptLoader("spell_warl_soul_harverst") { }

        class spell_warl_soul_harverst_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_soul_harverst_AuraScript);

            uint32 update;

            bool Validate(SpellInfo const* /*spell*/)
            {
                update = 0;

                if (!sSpellMgr->GetSpellInfo(WARLOCK_SOUL_HARVEST))
                    return false;
                return true;
            }

            void OnUpdate(uint32 diff)
            {
                update += diff;

                if (update >= 1000)
                {
                    if (Player* _player = GetCaster()->ToPlayer())
                    {
                        if (!_player->isInCombat() && !_player->InArena() && _player->isAlive())
                        {
                            _player->SetHealth(_player->GetHealth() + CalculatePct(_player->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_0].BasePoints / 1000));

                            if (Pet* pet = _player->GetPet())
                                pet->SetHealth(pet->GetHealth() + CalculatePct(_player->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_0].BasePoints / 1000));
                        }
                    }

                    update = 0;
                }
            }

            void Register()
            {
                OnAuraUpdate += AuraUpdateFn(spell_warl_soul_harverst_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_soul_harverst_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Life Tap - 1454
class spell_warl_life_tap: public SpellScriptLoader
{
    public:
        spell_warl_life_tap() : SpellScriptLoader("spell_warl_life_tap") { }

        class spell_warl_life_tap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_life_tap_SpellScript);

            SpellCastResult CheckLife()
            {
                if (GetCaster()->GetHealthPct() > 15.0f)
                    return SPELL_CAST_OK;
                return SPELL_FAILED_FIZZLE;
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                int32 l_Amount = int32(l_Caster->GetMaxHealth() * GetSpellInfo()->Effects[EFFECT_0].BasePoints / 100);
                l_Caster->EnergizeBySpell(l_Caster, WARLOCK_LIFE_TAP, l_Amount, POWER_MANA);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_life_tap_SpellScript::CheckLife);
                AfterHit += SpellHitFn(spell_warl_life_tap_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_life_tap_SpellScript();
        }

        class spell_warl_life_tap_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_life_tap_AuraScript);

            void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                    if (l_Caster->HasAura(WARLOCK_GLYPH_OF_LIFE_TAP))
                        p_Amount = l_Caster->CountPctFromMaxHealth(p_Amount);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_life_tap_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_HEAL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_life_tap_AuraScript();
        }
};

// Fear - 5782
class spell_warl_fear: public SpellScriptLoader
{
    public:
        spell_warl_fear() : SpellScriptLoader("spell_warl_fear") { }

        class spell_warl_fear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_fear_SpellScript);

            enum eSpells
            {
                GlyphOfFear       = 56244,
                FearEffect        = 118699,
                GlyphOfFearEffect = 130616,
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                if (!l_Caster || !l_Target)
                    return;

                if (l_Caster->HasAura(eSpells::GlyphOfFear))
                    l_Caster->CastSpell(l_Target, eSpells::GlyphOfFearEffect, true);
                else
                    l_Caster->CastSpell(l_Target, eSpells::FearEffect, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_fear_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_fear_SpellScript();
        }
};

// Banish - 710
class spell_warl_banish: public SpellScriptLoader
{
    public:
        spell_warl_banish() : SpellScriptLoader("spell_warl_banish") { }

        class spell_warl_banish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_banish_SpellScript);

            bool Load()
            {
                _removed = false;
                return true;
            }

            void HandleBanish()
            {
                // Casting Banish on a banished target will cancel the effect
                // Check if the target already has Banish, if so, do nothing.

                if (Unit* target = GetHitUnit())
                {
                    if (target->GetAuraEffect(SPELL_AURA_SCHOOL_IMMUNITY, SPELLFAMILY_WARLOCK, 0, 0x08000000, 0))
                    {
                        // No need to remove old aura since its removed due to not stack by current Banish aura
                        PreventHitDefaultEffect(EFFECT_0);
                        PreventHitDefaultEffect(EFFECT_1);
                        PreventHitDefaultEffect(EFFECT_2);
                        _removed = true;
                    }
                }
            }

            void RemoveAura()
            {
                if (_removed)
                    PreventHitAura();
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_warl_banish_SpellScript::HandleBanish);
                AfterHit += SpellHitFn(spell_warl_banish_SpellScript::RemoveAura);
            }

            bool _removed;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_banish_SpellScript();
        }
};

// Seed of Corruption (detonation) - 27285
class spell_warl_seed_of_corruption: public SpellScriptLoader
{
    public:
        spell_warl_seed_of_corruption() : SpellScriptLoader("spell_warl_seed_of_corruption") { }

        class spell_warl_seed_of_corruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_seed_of_corruption_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (GetExplTargetUnit())
                    targets.remove(GetExplTargetUnit());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_seed_of_corruption_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_seed_of_corruption_SpellScript();
        }
};

// Soulshatter - 29858
class spell_warl_soulshatter: public SpellScriptLoader
{
    public:
        spell_warl_soulshatter() : SpellScriptLoader("spell_warl_soulshatter") { }

        class spell_warl_soulshatter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_soulshatter_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARLOCK_SOULSHATTER))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->CanHaveThreatList() && target->getThreatManager().getThreat(caster) > 0.0f)
                            caster->CastSpell(target, WARLOCK_SOULSHATTER, true);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warl_soulshatter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_soulshatter_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Demonic Circle : Summon - 48018
class spell_warl_demonic_circle_summon: public SpellScriptLoader
{
    public:
        spell_warl_demonic_circle_summon() : SpellScriptLoader("spell_warl_demonic_circle_summon") { }

        class spell_warl_demonic_circle_summon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_demonic_circle_summon_AuraScript);

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Target = GetTarget())
                {
                    // If effect is removed by expire remove the summoned demonic circle too.
                    if (!(p_Mode & AURA_EFFECT_HANDLE_REAPPLY))
                        l_Target->RemoveGameObject(WARLOCK_DEMONIC_CIRCLE_SUMMON, true);

                    if (AuraApplication* l_Circle = l_Target->GetAuraApplication(WARLOCK_DEMONIC_CIRCLE_SUMMON, l_Target->GetGUID()))
                        l_Circle->SendFakeAuraUpdate(WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
                }
            }

            void HandleDummyTick(AuraEffect const* /*aurEff*/)
            {
                Unit* l_Target = GetTarget();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(WARLOCK_DEMONIC_CIRCLE_TELEPORT);

                if (l_Target == nullptr || l_SpellInfo == nullptr)
                    return;

                GameObject* l_Circle = l_Target->GetGameObject(GetId());

                if (l_Circle == nullptr)
                    return;

                // Here we check if player is in demonic circle teleport range, if so add
                // WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST; allowing him to cast the WARLOCK_DEMONIC_CIRCLE_TELEPORT.
                // If not in range remove the WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST.
                if (AuraApplication* l_CircleSummon = l_Target->GetAuraApplication(WARLOCK_DEMONIC_CIRCLE_SUMMON, l_Target->GetGUID()))
                {
                    if (l_Target->IsWithinDist(l_Circle, l_SpellInfo->GetMaxRange(true)))
                        l_CircleSummon->SendFakeAuraUpdate(WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, false);
                    else
                        l_CircleSummon->SendFakeAuraUpdate(WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_warl_demonic_circle_summon_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_circle_summon_AuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_demonic_circle_summon_AuraScript();
        }
};

// Demonic Circle : Teleport - 48020, Soulburn : Demonic Circle : Teleport - 114794 and Metamorphosis : Demonic Circle : Teleport - 104136
class spell_warl_demonic_circle_teleport: public SpellScriptLoader
{
    public:
        spell_warl_demonic_circle_teleport() : SpellScriptLoader("spell_warl_demonic_circle_teleport") { }

        class spell_warl_demonic_circle_teleport_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_demonic_circle_teleport_AuraScript);

            void HandleTeleport(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetTarget()->ToPlayer())
                {
                    if (GameObject* circle = player->GetGameObject(WARLOCK_DEMONIC_CIRCLE_SUMMON))
                    {
                        /// Item - Warlock WoD PvP Affliction 4P Bonus
                        if (player->getLevel() == 100 && player->HasAura(WARLOCK_WOD_PVP_AFFLICTION_4P_BONUS))
                            player->CastSpell(player, WARLOCK_WOD_PVP_AFFLICTION_4P_BONUS_EFF, true);
                        player->NearTeleportTo(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation());
                        player->RemoveMovementImpairingAuras();
                        player->Relocate(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation());

                        if (aurEff->GetSpellInfo()->Id == WARLOCK_SOULBURN_DEMONIC_CIRCLE_TELE)
                            if (player->HasAura(WARLOCK_SOULBURN_AURA))
                                player->RemoveAurasDueToSpell(WARLOCK_SOULBURN_AURA);

                        WorldPacket data(SMSG_MOVE_UPDATE);
                        player->m_movementInfo.pos.m_positionX = player->GetPositionX();
                        player->m_movementInfo.pos.m_positionY = player->GetPositionY();
                        player->m_movementInfo.pos.m_positionZ = player->GetPositionZ();
                        player->m_movementInfo.pos.m_orientation = player->GetOrientation();
                        player->m_movementInfo.time = getMSTime();
                        WorldSession::WriteMovementInfo(data, &player->m_movementInfo);
                        player->SendMessageToSet(&data, player);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_circle_teleport_AuraScript::HandleTeleport, EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_demonic_circle_teleport_AuraScript();
        }
};

// Unstable Affliction - 30108
class spell_warl_unstable_affliction: public SpellScriptLoader
{
    public:
        spell_warl_unstable_affliction() : SpellScriptLoader("spell_warl_unstable_affliction") { }

        class spell_warl_unstable_affliction_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_unstable_affliction_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARLOCK_UNSTABLE_AFFLICTION_DISPEL))
                    return false;
                return true;
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (AuraEffect const* aurEff = GetEffect(EFFECT_0))
                    {
                        Unit* l_Dispeller = dispelInfo->GetDispeller();
                        int32 l_Damage = int32(aurEff->GetAmount() * 8);
                        l_Damage = l_Caster->SpellDamageBonusDone(l_Dispeller, GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                        l_Damage = l_Dispeller->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);
                        // backfire damage and silence
                        l_Caster->CastCustomSpell(dispelInfo->GetDispeller(), WARLOCK_UNSTABLE_AFFLICTION_DISPEL, &l_Damage, &l_Damage, NULL, true);
                    }
                }
            }

            void Register()
            {
                AfterDispel += AuraDispelFn(spell_warl_unstable_affliction_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_unstable_affliction_AuraScript();
        }
};

enum ImmolateSpells
{
    SpellImmolateMarker = 16003,
    SpellEmpoweredImmolate = 157114
};

/// last update : 6.1.2 19802
/// Soulburn : Immolate - 348, Immolate (Fire and Brimstone override) - 108686
class spell_warl_immolate : public SpellScriptLoader
{
    public:
        spell_warl_immolate() : SpellScriptLoader("spell_warl_immolate") { }

        class spell_warl_immolate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_immolate_SpellScript);

            bool m_HasMarker = false;

            void HandleBeforeCast()
            {
                Unit* l_Target = GetExplTargetUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(ImmolateSpells::SpellImmolateMarker))
                    m_HasMarker = true;
            }

            void HandleDamage(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(ImmolateSpells::SpellEmpoweredImmolate);

                if (l_Target == nullptr || l_SpellInfo == nullptr)
                    return;

                if (l_Caster->HasAura(ImmolateSpells::SpellEmpoweredImmolate))
                {
                    if (m_HasMarker)
                        SetHitDamage(GetHitDamage() - CalculatePct(GetHitDamage(), l_SpellInfo->Effects[EFFECT_0].BasePoints / 2));
                    else
                        l_Target->CastSpell(l_Target, ImmolateSpells::SpellImmolateMarker, true);
                }
            }

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Player == nullptr)
                    return;

                if (l_Player->HasAura(WARLOCK_GLYPH_OF_SIPHON_LIFE) && l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARLOCK_DESTRUCTION)
                    l_Player->CastSpell(l_Player, WARLOCK_SPELL_SYPHON_LIFE, true);

                l_Player->CastSpell(l_Target, WARLOCK_IMMOLATE, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_warl_immolate_SpellScript::HandleBeforeCast);
                OnEffectHitTarget += SpellEffectFn(spell_warl_immolate_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnHit += SpellHitFn(spell_warl_immolate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_immolate_SpellScript();
        }
};

// Call by Soulburn : Immolate (Periodic damage) - 157736
// Corruption (Periodic Damage) - 146739
// Siphon Life
class spell_warl_siphon_life : public SpellScriptLoader
{
    public:
        spell_warl_siphon_life() : SpellScriptLoader("spell_warl_siphon_life") { }

        class spell_warl_siphon_life_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_siphon_life_AuraScript);

            enum eSpells
            {
                T17Demonology4P = 165451,
                CorruptionDoT   = 146739,
                HandOfGuldan    = 105174
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(WARLOCK_GLYPH_OF_SIPHON_LIFE))
                        l_Caster->CastSpell(l_Caster, WARLOCK_SPELL_SYPHON_LIFE, true);

                    /// Hand of Gul'dan now has 1 additional maximum charge, and Corruption has a 2% chance to generate 1 charge of Hand of Gul'dan when dealing damage.
                    if (GetSpellInfo()->Id == eSpells::CorruptionDoT && l_Caster->HasAura(eSpells::T17Demonology4P) && roll_chance_i(2))
                    {
                        if (Player* l_Player = l_Caster->ToPlayer())
                        {
                            if (SpellInfo const* l_HandOfGuldan = sSpellMgr->GetSpellInfo(eSpells::HandOfGuldan))
                                l_Player->RestoreCharge(l_HandOfGuldan->ChargeCategoryEntry);
                        }
                    }
                }
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (l_Target == nullptr)
                    return;

                /// Just for Immolation
                if (!p_AurEff->GetBase() || p_AurEff->GetBase()->GetId() != WARLOCK_IMMOLATE)
                    return;

                if (l_Target->HasAura(ImmolateSpells::SpellImmolateMarker))
                    l_Target->RemoveAura(ImmolateSpells::SpellImmolateMarker);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_siphon_life_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectRemove += AuraEffectRemoveFn(spell_warl_siphon_life_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_warl_siphon_life_AuraScript();
        }
};

// Siphon Life - 63106
class spell_warl_siphon_life_heal : public SpellScriptLoader
{
public:
    spell_warl_siphon_life_heal() : SpellScriptLoader("spell_warl_siphon_life_heal") { }

    class spell_warl_siphon_life_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_siphon_life_heal_SpellScript);

        void HandleOnHit()
        {
            SetHitHeal(GetHitHeal() / 100);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warl_siphon_life_heal_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_siphon_life_heal_SpellScript();
    }
};

/// last update : 6.1.2 19802
/// Call by Corruption - 146739 and Doom - 603
/// Item - Warlock WoD PvP Demonology 4P Bonus
class spell_warl_WodPvPDemonology4PBonus : public SpellScriptLoader
{
    public:
        spell_warl_WodPvPDemonology4PBonus() : SpellScriptLoader("spell_warl_WodPvPDemonology4PBonus") { }

        class spell_warl_WodPvPDemonology4PBonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_WodPvPDemonology4PBonus_AuraScript);

            enum eSpells
            {
                WoDPvPDemonology4PBonusAura = 165967,
                WoDPvPDemonology4PBonus     = 165968
            };

            void HandleDispel(DispelInfo* /*dispelInfo*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::WoDPvPDemonology4PBonusAura))
                    l_Caster->CastSpell(l_Caster, eSpells::WoDPvPDemonology4PBonus, true);
            }

            void Register()
            {
                AfterDispel += AuraDispelFn(spell_warl_WodPvPDemonology4PBonus_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_WodPvPDemonology4PBonus_AuraScript();
        }
};

/// Corruption - 172
class spell_warl_corruption : public SpellScriptLoader
{
    public:
        spell_warl_corruption() : SpellScriptLoader("spell_warl_corruption") { }

        class spell_warl_corruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_corruption_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(WARLOCK_GLYPH_OF_SIPHON_LIFE) && l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SPEC_WARLOCK_DESTRUCTION)
                        l_Player->CastSpell(l_Player, WARLOCK_SPELL_SYPHON_LIFE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_corruption_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_corruption_SpellScript();
        }
};

/// Dark Soul Knowledge - 113861
/// Dark Soul Misery - 113860
/// Dark Soul Instability - 113858
class spell_warl_dark_soul_charges : public SpellScriptLoader
{
public:
    spell_warl_dark_soul_charges() : SpellScriptLoader("spell_warl_dark_soul_charges") { }

    class spell_warl_dark_soul_charges_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_dark_soul_charges_SpellScript);

        SpellCastResult CheckAura()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(GetSpellInfo()->Id))
                    return SPELL_FAILED_CASTER_AURASTATE;
            }

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_dark_soul_charges_SpellScript::CheckAura);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_dark_soul_charges_SpellScript();
    }
};


/// Dark Soul - 77801
class spell_warl_dark_soul : public SpellScriptLoader
{
    public:
        spell_warl_dark_soul() : SpellScriptLoader("spell_warl_dark_soul") { }

        class spell_warl_dark_soul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_dark_soul_SpellScript);

            void HandleAfterCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    uint32 l_OldCooldown = l_Player->GetSpellCooldownDelay(GetSpellInfo()->Id);
                    uint32 l_NewCooldown = l_OldCooldown - CalculatePct(l_OldCooldown, 50);

                    l_Player->RemoveSpellCooldown(GetSpellInfo()->Id, true);

                    if (!l_Player->HasAura(WARLOCK_GLYPH_OF_DARK_SOUL))
                        l_Player->AddSpellCooldown(GetSpellInfo()->Id, 0, l_OldCooldown, true);
                    else ///< Case of GLYPH_OF_DARK_SOUL
                        l_Player->AddSpellCooldown(GetSpellInfo()->Id, 0, l_NewCooldown, true);

                    if (Aura* l_DarkSoul = l_Player->GetAura(GetSpellInfo()->Id))
                        l_DarkSoul->SetDuration(CalculatePct(l_DarkSoul->GetDuration(), 50));

                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warl_dark_soul_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_dark_soul_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Cataclysm - 152108
class spell_warl_cataclysm : public SpellScriptLoader
{
    public:
        spell_warl_cataclysm() : SpellScriptLoader("spell_warl_cataclysm") { }

        class spell_warl_cataclysm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_cataclysm_SpellScript);

            enum eSpells
            {
                Immolate = 157736,
                Agony = 980,
                Corrupation = 172,
                UnstableAffliction = 30108
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARLOCK_DESTRUCTION)
                    l_Player->CastSpell(l_Target, eSpells::Immolate, true);
                else if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARLOCK_AFFLICTION)
                {
                    l_Player->CastSpell(l_Target, eSpells::Agony, true);
                    l_Player->CastSpell(l_Target, eSpells::UnstableAffliction, true);
                }
                else if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                    l_Player->CastSpell(l_Target, eSpells::Corrupation, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warl_cataclysm_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_cataclysm_SpellScript();
        }
};

enum HavocSpells
{
    SPELL_GLYPH_OF_HAVOC    = 146962
};

// Havoc - 80240
class spell_warl_havoc: public SpellScriptLoader
{
    public:
        spell_warl_havoc() : SpellScriptLoader("spell_warl_havoc") { }

        class spell_warl_havoc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_havoc_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                // Set to max at apply
                uint8 l_Charges = GetCaster()->HasAura(SPELL_GLYPH_OF_HAVOC) ? 6 : 3;
                aurEff->GetBase()->SetStackAmount(l_Charges);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_havoc_AuraScript::OnApply, EFFECT_1, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_havoc_AuraScript();
        }
};

/// Called by Corruption - 146739
/// Nightfall - 108558
/// last update : 6.2.3
class spell_warl_nightfall : public SpellScriptLoader
{
    public:
        spell_warl_nightfall() : SpellScriptLoader("spell_warl_nightfall") { }

        class spell_warl_nightfall_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_nightfall_AuraScript);

            enum eSpells
            {
                Nightfall           = 108558,
                NightfallGain       = 17941,
                T17Affliction2P     = 165448,
                DrainSoul           = 103103,
                Corruption          = 146739,
                UnstableAffliction  = 30108,
                Agony               = 980,
                DarkSoulMisery      = 113860,
                T17Affliction4P     = 165449
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                if (!l_Caster)
                    return;

                if (!l_Caster->HasAura(eSpells::Nightfall))
                    return;

                SpellInfo const* l_SpellInfoNightfall = sSpellMgr->GetSpellInfo(eSpells::Nightfall);
                if (!l_SpellInfoNightfall)
                    return;

                float l_Chance = 0.0f;

                if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::Nightfall, EFFECT_0))
                    l_Chance = (l_AuraEffect->GetAmount() / 10);

                /// While channeling Drain Soul on a target afflicted by your Corruption, Unstable Affliction, and Agony, the chance for Nightfall to activate is increased by 5%.
                if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::T17Affliction2P, EFFECT_0))
                {
                    if (Spell* l_Spell = l_Caster->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                    {
                        if (l_Spell->GetSpellInfo()->Id == eSpells::DrainSoul)
                        {
                            if (Unit* l_Victim = l_Spell->GetUnitTarget())
                            {
                                if (l_Victim->HasAura(eSpells::Corruption, l_Caster->GetGUID()) &&
                                    l_Victim->HasAura(eSpells::UnstableAffliction, l_Caster->GetGUID()) &&
                                    l_Victim->HasAura(eSpells::Agony, l_Caster->GetGUID()))
                                    l_Chance += l_AuraEffect->GetAmount();
                            }
                        }
                    }
                }

                if (roll_chance_f(l_Chance))
                {
                    l_Caster->CastSpell(l_Caster, eSpells::NightfallGain, true);

                    /// When Nightfall activates, the cooldown of Dark Soul is reduced by 10 sec.
                    if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::T17Affliction4P, EFFECT_0))
                    {
                        if (Player* l_Player = l_Caster->ToPlayer())
                            l_Player->ReduceSpellCooldown(eSpells::DarkSoulMisery, l_AuraEffect->GetAmount());
                    }
                }

                if (Aura* l_Corruption = l_Target->GetAura(p_AurEff->GetSpellInfo()->Id, l_Caster->GetGUID()))
                {
                    /// Patch 6.2.2 (2015-09-01): Now deals 10 % less damage in PvP combat.
                    if (l_Target->IsPlayer() && p_AurEff->GetTickNumber() == 1)
                        l_Corruption->GetEffect(EFFECT_0)->ChangeAmount(l_Corruption->GetEffect(EFFECT_0)->GetAmount() - CalculatePct(l_Corruption->GetEffect(EFFECT_0)->GetAmount(), 10));
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_nightfall_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_nightfall_AuraScript();
        }
};

/// Last Update : 6.2.3
/// Chaos Bolt - 116858 and Chaos Bolt (Fire and Brimstone) - 157701
class spell_warl_chaos_bolt : public SpellScriptLoader
{
    public:
        spell_warl_chaos_bolt() : SpellScriptLoader("spell_warl_chaos_bolt") { }

        class spell_warl_chaos_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_chaos_bolt_SpellScript);

            enum eSpells
            {
                Backdraft = 117828
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                Aura* l_Backdraft = l_Caster->GetAura(eSpells::Backdraft);

                if (l_Backdraft == nullptr)
                    return;

                if (l_Backdraft->GetCharges() < 3)
                    return;

                l_Backdraft->ModCharges(-3);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warl_chaos_bolt_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_chaos_bolt_SpellScript();
        }
};

/// Chaos Wave - 124916
class spell_warl_chaos_wave : public SpellScriptLoader
{
public:
    spell_warl_chaos_wave() : SpellScriptLoader("spell_warl_chaos_wave") { }

    class spell_warl_chaos_wave_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_chaos_wave_SpellScript);

        void HandleAfterCast()
        {
            Unit* l_Caster = GetCaster();

            if (l_Caster == nullptr)
                return;

            if (l_Caster->HasAura(WARLOCK_MOLTEN_CORE_AURA))
                l_Caster->CastSpell(l_Caster, WARLOCK_MOLTEN_CORE, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_warl_chaos_wave_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_chaos_wave_SpellScript();
    }
};

/// last update : 6.1.2 19802
/// Fel Firebolt - 104318, Firebolt - 3110
class spell_warl_fel_firebolt : public SpellScriptLoader
{
    public:
        spell_warl_fel_firebolt() : SpellScriptLoader("spell_warl_fel_firebolt") { }

        class spell_warl_fel_firebolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_fel_firebolt_SpellScript);

            enum eSpells
            {
                Firebolt        = 104318,
                MoltenCore      = 122351,
                MoltenCoreAura  = 122355
            };

            enum eDatas
            {
                WildImp = 55659
            };

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

                if (m_scriptSpellId == eSpells::Firebolt)
                {
                    Creature* l_Creature = l_Caster->ToCreature();

                    if (l_Creature == nullptr)
                        return;

                    if (l_Creature->GetEntry() == eDatas::WildImp)
                    {
                        l_Creature->AI()->DropCharge();

                        if (Unit* l_Owner = l_Creature->GetOwner())
                        {
                            if (AuraEffect* l_MoltenCore = l_Owner->GetAuraEffect(eSpells::MoltenCore, EFFECT_0))
                                if (roll_chance_i(l_MoltenCore->GetAmount()))
                                    l_Owner->CastSpell(l_Owner, eSpells::MoltenCoreAura, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warl_fel_firebolt_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_fel_firebolt_SpellScript();
        }
};

/// last update : 6.2.3
/// Doom Bolt - 85692
class spell_warl_doom_bolt : public SpellScriptLoader
{
    public:
        spell_warl_doom_bolt() : SpellScriptLoader("spell_warl_doom_bolt") { }

        class spell_warl_doom_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_doom_bolt_SpellScript);

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                int32 l_Damage = GetHitDamage();

                /// If target has less then 20% damage we should increase damage by 20%
                if (l_Target->GetHealthPct() <= (float)GetSpellInfo()->Effects[EFFECT_1].BasePoints)
                    AddPct(l_Damage, GetSpellInfo()->Effects[EFFECT_1].BasePoints);

                SetHitDamage(l_Damage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warl_doom_bolt_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_doom_bolt_SpellScript();
        }
};

/// last update : 6.2.3
/// Demonic Servitude - 152107
class spell_warl_demonic_servitude : public SpellScriptLoader
{
    public:
        spell_warl_demonic_servitude() : SpellScriptLoader("spell_warl_demonic_servitude") { }

        class spell_warl_demonic_servitude_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_demonic_servitude_AuraScript);

            enum eSpells
            {
                GrimoireOfService   = 108501,
                GrimoireDoomguard   = 157900,
                GrimoireInfernal    = 157901,
                GrimoireofSupremacy = 108499,
                SummonAbyssal       = 157899
            };

            enum eNPCs
            {
                Infernal    = 78217,
                Abyssal     = 78216,
                Terrorguard = 78215,
                DoomGuard   = 78158
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasSpell(eSpells::GrimoireOfService)) ///< Grimoire of Service
                {
                    if (!l_Player->HasSpell(eSpells::GrimoireDoomguard))
                        l_Player->learnSpell(eSpells::GrimoireDoomguard, false);
                    if (!l_Player->HasSpell(eSpells::GrimoireInfernal))
                        l_Player->learnSpell(eSpells::GrimoireInfernal, false);
                }
                if (l_Player->HasAura(eSpells::GrimoireofSupremacy)) ///< Grimoire of Supremacy
                {
                    if (!l_Player->HasSpell(eSpells::SummonAbyssal))
                        l_Player->learnSpell(eSpells::SummonAbyssal, false);
                }

            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasSpell(eSpells::GrimoireDoomguard))
                    l_Player->removeSpell(eSpells::GrimoireDoomguard, false, false);
                if (l_Player->HasSpell(eSpells::GrimoireInfernal))
                    l_Player->removeSpell(eSpells::GrimoireInfernal, false, false);
                if (l_Player->HasSpell(eSpells::SummonAbyssal))
                    l_Player->removeSpell(eSpells::SummonAbyssal, false, false);

                Pet* l_Pet = l_Player->GetPet();

                if (l_Pet == nullptr)
                    return;

                /// Prevent Doomguard/Infernal/Abyssal to keep summon after removing the talent
                if (l_Pet->GetEntry() == eNPCs::Abyssal || l_Pet->GetEntry() == eNPCs::Infernal || l_Pet->GetEntry() == eNPCs::Terrorguard || l_Pet->GetEntry() == eNPCs::DoomGuard)
                    l_Player->GetPet()->UnSummon();
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_servitude_AuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warl_demonic_servitude_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_demonic_servitude_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Grimoire of Synergy - 171975
class spell_warl_grimoire_of_synergy : public SpellScriptLoader
{
    public:
        spell_warl_grimoire_of_synergy() : SpellScriptLoader("spell_warl_grimoire_of_synergy") { }

        class spell_warl_grimoire_of_synergy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_grimoire_of_synergy_AuraScript);

            enum eSpells
            {
                GrimoireofSynergyBuff = 171982
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
            {
                Unit* l_Target = GetTarget();

                /// If procs on warlock, we should add buff on pet
                if (l_Target->IsPlayer() && l_Target->ToPlayer())
                {
                    if (Pet* l_Pet = l_Target->ToPlayer()->GetPet())
                    {
                        if (!l_Pet)
                            return;

                        l_Target->CastSpell(l_Pet, eSpells::GrimoireofSynergyBuff, true);
                    }
                }
                /// If procs on pet, we should add buff on warlock
                else if (l_Target->GetTypeId() == TYPEID_UNIT)
                {
                    if (Unit* l_Owner = l_Target->GetOwner())
                    {
                        if (!l_Owner)
                            return;

                        l_Target->CastSpell(l_Owner, eSpells::GrimoireofSynergyBuff, true);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_warl_grimoire_of_synergy_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_grimoire_of_synergy_AuraScript();
        }
};

enum WoDPvPDemonology2PBonusSpells
{
    WoDPvPDemonology2PBonusAura = 171393,
    WoDPvPDemonology2PBonus = 171397
};

/// WoD PvP Demonology 2P Bonus - 171393
class PlayerScript_WoDPvPDemonology2PBonus : public PlayerScript
{
    public:
        PlayerScript_WoDPvPDemonology2PBonus() :PlayerScript("PlayerScript_WoDPvPDemonology2PBonus") { }

        enum eSpells
        {
            T17Destruction4P    = 165452,
            ChaoticInfusion     = 170000
        };

        void OnModifyHealth(Player* p_Player, int32 /*p_Value*/) override
        {
            if (p_Player->getClass() == CLASS_WARLOCK && p_Player->HasAura(WoDPvPDemonology2PBonusSpells::WoDPvPDemonology2PBonusAura))
            {
                if (p_Player->GetHealthPct() < 20.0f && !p_Player->HasAura(WoDPvPDemonology2PBonusSpells::WoDPvPDemonology2PBonus))
                    p_Player->CastSpell(p_Player, WoDPvPDemonology2PBonusSpells::WoDPvPDemonology2PBonus, true);

                /// Remove aura if player has more than 20% life
                if (p_Player->GetHealthPct() >= 20.0f && !(p_Player->GetPet() && p_Player->GetPet()->GetHealthPct() < 20.0f))
                    p_Player->RemoveAura(WoDPvPDemonology2PBonusSpells::WoDPvPDemonology2PBonus);
            }
        }

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After) override
        {
            if (p_After)
                return;

            if (p_Regen || p_OldValue > p_NewValue || p_Power != Powers::POWER_BURNING_EMBERS)
                return;

            /// Item - Warlock T17 Destruction 4P Bonus
            for (uint8 l_I = 0; l_I < EFFECT_4; ++l_I)
            {
                if (AuraEffect* l_AuraEffect = p_Player->GetAuraEffect(eSpells::T17Destruction4P, l_I))
                {
                    /// When a Burning Ember is filled up, you have a chance to cause your next Chaos Bolt to multistrike 3 additional times.
                    if (p_OldValue < l_AuraEffect->GetAmount() && p_NewValue >= l_AuraEffect->GetAmount() && roll_chance_i(15))
                        p_Player->CastSpell(p_Player, eSpells::ChaoticInfusion, true);
                }
            }
        }
};

/// Demonic Fury regeneration by kill
class PlayerScript_DemonicFury_On_Kill : public PlayerScript
{
    public:
        PlayerScript_DemonicFury_On_Kill() :PlayerScript("PlayerScript_DemonicFury_On_Kill") { }

        enum eSpells
        {
            Metamorphis = 103958
        };

        void OnKill(Player* p_Player, Unit* p_Victim) override
        {
            if (p_Player->GetSpecializationId() != SPEC_WARLOCK_DEMONOLOGY)
                return;

            if (p_Player->isHonorOrXPTarget(p_Victim) && p_Player->HasAura(eSpells::Metamorphis))
                p_Player->ModifyPower(POWER_DEMONIC_FURY, 100 * p_Player->GetPowerCoeff(POWER_DEMONIC_FURY));
        }
};

/// Create Healthstone - 6201
class spell_warl_create_healthstone: public SpellScriptLoader
{
    public:
        spell_warl_create_healthstone() : SpellScriptLoader("spell_warl_create_healthstone") { }

        class spell_warl_create_healthstone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_create_healthstone_SpellScript);

            enum eSpells
            {
                CreateHealthstone = 23517
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                l_Caster->CastSpell(l_Caster, CreateHealthstone, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warl_create_healthstone_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_create_healthstone_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Healthstone - 6262
class spell_warl_healthstone : public SpellScriptLoader
{
    public:
        spell_warl_healthstone() : SpellScriptLoader("spell_warl_healthstone") { }

        class spell_warl_healthstone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_healthstone_SpellScript);

            enum eSpells
            {
                GlyphOfHealthstone = 56224
            };

            void HandleHeal(SpellEffIndex /*p_EffIndex*/)
            {
                if (GetCaster()->HasAura(eSpells::GlyphOfHealthstone))
                    PreventHitHeal();
            }

            void HandlePeriodicHeal(SpellEffIndex /*p_EffIndex*/)
            {
                if (!GetCaster()->HasAura(eSpells::GlyphOfHealthstone))
                    PreventHitAura();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warl_healthstone_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
                OnEffectHitTarget += SpellEffectFn(spell_warl_healthstone_SpellScript::HandlePeriodicHeal, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_healthstone_SpellScript();
        }
};

/// Visual handler for Soul Shards
class spell_warl_soul_shards_visual : public PlayerScript
{
public:
    spell_warl_soul_shards_visual() : PlayerScript("spell_warl_soul_shards_visual")
    {
    }

    enum eSpells
    {
        OneMainSoulShard = 104756,
        TwoMainSoulShards = 123171,
        TwoSoulShards = 104759,
        GlyphOfSubtlety = 56217
    };

    void OnModifySpec(Player* p_Player, int32 p_NewSpec)
    {
        if (p_Player->getClass() == Classes::CLASS_WARLOCK && p_NewSpec != SPEC_WARLOCK_AFFLICTION)
        {
            p_Player->RemoveAura(eSpells::OneMainSoulShard);  ///< 1 center shard visual
            p_Player->RemoveAura(eSpells::TwoMainSoulShards); ///< 2 center shards visual
            p_Player->RemoveAura(eSpells::TwoSoulShards);     ///< 2 shards visual
        }
    }

    /// Override
    void OnModifyPower(Player* p_Player, Powers p_Power, int32 /*p_OldValue*/, int32& p_NewValue, bool /*p_Regen*/, bool p_After)
    {
        if (p_After)
            return;

        ///< Works only in Afflication spec and if warlock doesn't have Glyph of Subtlety
        if (p_Power == POWER_SOUL_SHARDS && p_Player->GetSpecializationId() == SPEC_WARLOCK_AFFLICTION)
        {
            p_Player->RemoveAura(eSpells::OneMainSoulShard);  ///< 1 center shard visual
            p_Player->RemoveAura(eSpells::TwoMainSoulShards); ///< 2 center shards visual
            p_Player->RemoveAura(eSpells::TwoSoulShards);     ///< 2 shards visual
            
            /// Glyph of Subtlety
            if (!p_Player->HasAura(eSpells::GlyphOfSubtlety))
            {
                if ((p_NewValue > (1 * p_Player->GetPowerCoeff(p_Power))) && (p_NewValue < (2 * p_Player->GetPowerCoeff(p_Power))))
                {
                    p_Player->CastSpell(p_Player, eSpells::OneMainSoulShard, true);
                }
                else if (p_NewValue < (3 * p_Player->GetPowerCoeff(p_Power)))
                {
                    p_Player->CastSpell(p_Player, eSpells::TwoMainSoulShards, true);
                }
                else if (p_NewValue < (4 * p_Player->GetPowerCoeff(p_Power)))
                {
                    p_Player->CastSpell(p_Player, eSpells::OneMainSoulShard, true);
                    p_Player->CastSpell(p_Player, eSpells::TwoMainSoulShards, true);
                }
                else if (p_NewValue >= (4 * p_Player->GetPowerCoeff(p_Power)))
                {
                    p_Player->CastSpell(p_Player, eSpells::TwoMainSoulShards, true);
                    p_Player->CastSpell(p_Player, eSpells::TwoSoulShards, true);
                }
            }
        }
    }
};

/// Command Demon
/// 119905 - (Cauterize Master), 119907 - (Disarm), 119909 - (Whiplash), 119913 - (Fellash), 119910 - (Spell Lock), 119911 - (Optical Blast), 119914 - (Felstorm), 119915 - (Wrathstorm)
class spell_warl_command_demon_spells : public SpellScriptLoader
{
    public:
        spell_warl_command_demon_spells() : SpellScriptLoader("spell_warl_command_demon_spells") { }

        class spell_warl_command_demon_spells_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_command_demon_spells_SpellScript);

            SpellCastResult CheckConditions()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr || !l_Caster->ToPlayer())
                    return SPELL_FAILED_DONT_REPORT;

                Pet* l_Pet = l_Caster->ToPlayer()->GetPet();

                if (l_Pet == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                if (l_Pet->HasAurasPreventCasting())
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                return SPELL_CAST_OK;
            }


            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warl_command_demon_spells_SpellScript::CheckConditions);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_command_demon_spells_SpellScript();
        }
};

/// Glyph of Life Pact - 159669
class spell_warl_glyph_of_life_tap : public SpellScriptLoader
{
    public:
        spell_warl_glyph_of_life_tap() : SpellScriptLoader("spell_warl_glyph_of_life_tap") { }

        class spell_warl_glyph_of_life_tap_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_glyph_of_life_tap_AuraScript);

            enum eSpells
            {
                GlyphofLifePactPeriodic = 162640
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->HasAura(eSpells::GlyphofLifePactPeriodic))
                    l_Player->CastSpell(l_Player, eSpells::GlyphofLifePactPeriodic, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(eSpells::GlyphofLifePactPeriodic))
                    l_Player->RemoveAura(eSpells::GlyphofLifePactPeriodic);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_life_tap_AuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warl_glyph_of_life_tap_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_glyph_of_life_tap_AuraScript();
        }
};

/// Glyph of Life Pact  - 162640
class spell_warl_glyph_of_life_tap_periodic : public SpellScriptLoader
{
    public:
        spell_warl_glyph_of_life_tap_periodic() : SpellScriptLoader("spell_warl_glyph_of_life_tap_periodic") { }

        class spell_warl_glyph_of_life_tap_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_glyph_of_life_tap_periodic_AuraScript);

            enum eSpells
            {
                LifePactDamage = 162647
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Target = GetTarget();

                if (l_Target->GetHealthPct() > (float)p_AurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount())
                    l_Target->CastSpell(l_Target, eSpells::LifePactDamage, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_glyph_of_life_tap_periodic_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_glyph_of_life_tap_periodic_AuraScript();
        }
};

/// Cripple - 170995
class spell_warl_cripple_doomguard : public SpellScriptLoader
{
public:
    spell_warl_cripple_doomguard() : SpellScriptLoader("spell_warl_cripple_doomguard") { }

    class spell_warl_cripple_doomguard_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_cripple_doomguard_SpellScript);

        void HandleAfterHit()
        {
            if (!GetCaster() || !GetCaster()->ToPet())
                return;

            if (Pet* l_Pet = GetCaster()->ToPet())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    /// 4 seconds duration in PVP
                    if (l_Target->IsPlayer())
                        if (Aura* l_Cripple = l_Target->GetAura(GetSpellInfo()->Id))
                            l_Cripple->SetDuration(4000);
                }
            }
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_warl_cripple_doomguard_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warl_cripple_doomguard_SpellScript();
    }
};

/// last update : 6.2.3
/// Incinerate - 29722, Incinerate (Fire and Brimstone) - 114654
class spell_warl_incinerate : public SpellScriptLoader
{
    public:
        spell_warl_incinerate() : SpellScriptLoader("spell_warl_incinerate") { }

        class spell_warl_incinerate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_incinerate_SpellScript);

            enum eSpells
            {
                WarlockWoDPvPDestruction4PBonus = 189209,
                ImmolateDamage                  = 157736
            };

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::WarlockWoDPvPDestruction4PBonus, EFFECT_0))
                {
                    if (l_Target->HasAura(eSpells::ImmolateDamage, l_Caster->GetGUID()))
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), 100)); ///< Patch 6.2.3 Hotfixes: November 17 - 23 : 4-piece PvP set bonus for Destruction Warlocks now increases damage dealt by Incinerate on targets afflicted by the casting Warlock's Immolate by 100% (up from 50%)
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warl_incinerate_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_incinerate_SpellScript();
        }
};

/// Last Update 6.2.3
/// Demonbolt - 157695
class spell_warl_demonbolt : public SpellScriptLoader
{
    public:
        spell_warl_demonbolt() : SpellScriptLoader("spell_warl_demonbolt") { }

        class spell_warl_demonbolt_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_demonbolt_AuraScript);

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& /*p_Amount*/, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                /* Wainting for more information */
                /*float l_HastePct = 2.0f - l_Caster->GetFloatValue(UNIT_FIELD_MOD_HASTE);

                p_Amount *= l_HastePct;*/
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_demonbolt_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        class spell_warl_demonbolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warl_demonbolt_SpellScript);

            uint32 m_Duration = 0;
            bool m_Flag = false;

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                float l_HastePct = l_Caster->GetFloatValue(UNIT_FIELD_MOD_HASTE);

                l_Caster->AddAura(GetSpellInfo()->Id, l_Caster);
                if (Aura* l_Aura = l_Caster->GetAura(GetSpellInfo()->Id, l_Caster->GetGUID()))
                    l_Aura->SetDuration(l_Aura->GetDuration() * l_HastePct);
            }

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();

                if (Aura* l_Aura = l_Caster->GetAura(GetSpellInfo()->Id, l_Caster->GetGUID()))
                    m_Duration = l_Aura->GetDuration();
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (Aura* l_Aura = l_Caster->GetAura(GetSpellInfo()->Id, l_Caster->GetGUID()))
                {
                    if (m_Flag)
                        l_Aura->DropStack();
                    l_Aura->SetDuration(m_Duration);
                    m_Flag = true;
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warl_demonbolt_SpellScript::HandleAfterCast);
                AfterHit += SpellHitFn(spell_warl_demonbolt_SpellScript::HandleAfterHit);
                BeforeHit += SpellHitFn(spell_warl_demonbolt_SpellScript::HandleBeforeHit);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warl_demonbolt_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_warl_demonbolt_SpellScript();
        }
};

/// Last Update 6.2.3
/// Glyph of soul consumption - 58070
class spell_warl_glyph_of_soul_consumption : public SpellScriptLoader
{
    public:
        spell_warl_glyph_of_soul_consumption() : SpellScriptLoader("spell_warl_glyph_of_soul_consumption") { }

        class spell_warl_glyph_of_soul_consumption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warl_glyph_of_soul_consumption_AuraScript);

            enum eSpells
            {
                ChaosBolt = 116858,
                DrainSoul = 103103,
                Metamorphosis = 103958,
                GlyphOfSoulConsumption = 58068
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                if (p_EventInfo.GetDamageInfo() == nullptr)
                    return;

                uint32 l_Spec = l_Player->GetSpecializationId(l_Player->GetActiveSpec());
                SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                Unit* l_Victim = p_EventInfo.GetDamageInfo()->GetVictim();

                if (l_Victim == nullptr)
                    return;

                if (!l_Player->isHonorOrXPTarget(l_Victim))
                    return;

                if (l_Victim->GetHealth() > p_EventInfo.GetDamageInfo()->GetDamage())
                    return;

                if ((l_Spec == SPEC_WARLOCK_DEMONOLOGY && l_Player->HasAura(eSpells::Metamorphosis)) ||
                    (l_SpellInfo != nullptr && ((l_Spec == SPEC_WARLOCK_DESTRUCTION && l_SpellInfo->Id == eSpells::ChaosBolt) ||
                    (l_Spec == SPEC_WARLOCK_AFFLICTION && l_SpellInfo->Id == eSpells::DrainSoul))))
                {
                    l_Caster->CastSpell(l_Caster, eSpells::GlyphOfSoulConsumption, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_warl_glyph_of_soul_consumption_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_warl_glyph_of_soul_consumption_AuraScript();
        }
};

/// last update 6.2.3
/// Drain life doesn't cost 1% of maximum mana while the player is in metamorphosis
class PlayerScript_spell_warl_drain_life_meta_cost : public PlayerScript
{
public:
    PlayerScript_spell_warl_drain_life_meta_cost() : PlayerScript("PlayerScript_spell_warl_drain_life_meta_cost") { }

    enum eSpells
    {
        Metomorphosis = 103958,
        DrainLife = 689
    };

    void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool /*p_Regen*/, bool p_After)
    {
        if (p_After)
            return;

        if (p_Power == POWER_MANA && p_Player->HasAura(eSpells::Metomorphosis) && p_Player->HasAura(eSpells::DrainLife))
        {
            if (p_NewValue < p_OldValue)
                p_NewValue = p_OldValue;
        }

    }
};

/// Last Update 6.2.3
/// Drain Fel Energy - 139200
class spell_warl_drain_fel_energy : public SpellScriptLoader
{
public:
    spell_warl_drain_fel_energy() : SpellScriptLoader("spell_warl_drain_fel_energy") { }

    class spell_warl_drain_fel_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_drain_fel_energy_AuraScript);

        enum eSpells
        {
            CodexOfXerrath1 = 101508,
            CodexOfXerrath2 = 137206
        };

        void OnRemoveAura(AuraEffect const* p_AuraEffect, AuraEffectHandleModes /*mode*/)
        {
            Unit* l_Caster = GetCaster();
            if (!l_Caster)
                return;

            Player* l_Player = l_Caster->ToPlayer();
            if (!l_Player)
                return;

            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            l_Player->learnSpell(eSpells::CodexOfXerrath1, false);
            l_Player->learnSpell(eSpells::CodexOfXerrath2, false);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_warl_drain_fel_energy_AuraScript::OnRemoveAura, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warl_drain_fel_energy_AuraScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_warlock_spell_scripts()
{
    new spell_warl_drain_fel_energy();
    new spell_warl_dark_soul_charges();
    new spell_warl_glyph_of_soul_consumption();
    new spell_warl_t17_Demonology_2p();
    new spell_warl_grimoire_of_supremacy_bonus();
    new spell_warl_molten_core();
    new spell_warl_demonbolt();
    new spell_warl_incinerate();
    new spell_warl_glyph_of_life_tap_periodic();
    new spell_warl_glyph_of_life_tap();
    new spell_warl_demonic_servitude();
    new spell_warl_fire_and_brimstone();
    new spell_warl_haunt();
    new spell_warl_glyph_of_nightmares();
    new spell_warl_cataclysm();
    new spell_warl_siphon_life();
    new spell_warl_corruption();
    new spell_warl_siphon_life_heal();
    new spell_warl_dark_soul();
    new spell_warl_immolate();
    new spell_warl_grimoire_of_service();
    new spell_warl_haunt_dispel();
    new spell_warl_grimoire_of_supremacy();
    new spell_warl_soulburn_seed_of_corruption_damage();
    new spell_warl_soulburn_seed_of_corruption();
    new spell_warl_soulburn_override();
    new spell_warl_imp_swarm();
    new spell_warl_glyph_of_imp_swarm();
    new spell_warl_unbound_will();
    new spell_warl_rain_of_fire_damage();
    new spell_warl_agony();
    new spell_warl_grimoire_of_sacrifice();
    new spell_warl_flames_of_xoroth();
    new spell_warl_soul_link_dummy();
    new spell_warl_soul_link();
    new spell_warl_molten_core_dot();
    new spell_warl_decimate();
    new spell_warl_demonic_call();
    new spell_warl_touch_of_chaos();
    new spell_warl_immolation_aura();
    new spell_warl_dark_bargain();
    new spell_warl_dark_regeneration();
    new spell_warl_soul_leech();
    new spell_warl_sacrificial_pact();
    new spell_warl_hand_of_guldan();
    new spell_warl_hand_of_guldan_damage();
    new spell_warl_twilight_ward_s12();
    new spell_warl_hellfire_periodic();
    new spell_warl_hellfire_damage_area();
    new spell_warl_demonic_leap_jump();
    new spell_warl_demonic_leap();
    new spell_warl_soul_swap_soulburn();
    new spell_warl_soul_swap();
    new spell_warl_drain_soul();
    new spell_warl_demonic_gateway();
    new spell_warl_rain_of_fire();
    new spell_warl_rain_of_fire_despawn();
    new spell_warl_ember_tap();
    new spell_warl_ember_tap_glyph();
    new spell_warl_conflagrate_aura();
    new spell_warl_shadowburn();
    new spell_warl_burning_embers();
    new spell_warl_burning_embers_regen();
    new spell_warl_drain_life();
    new spell_warl_soul_harverst();
    new spell_warl_life_tap();
    new spell_warl_fear();
    new spell_warl_banish();
    new spell_warl_seed_of_corruption();
    new spell_warl_soulshatter();
    new spell_warl_demonic_circle_summon();
    new spell_warl_demonic_circle_teleport();
    new spell_warl_unstable_affliction();
    new spell_warl_havoc();
    new spell_warl_nightfall();
    new spell_warl_chaos_bolt();
    new spell_warl_chaos_wave();
    new spell_warl_WodPvPDemonology4PBonus();
    new spell_warl_fel_firebolt();
    new spell_warl_doom_bolt();
    new spell_warl_grimoire_of_synergy();
    new spell_warl_grimoire_of_supremacy_effect();
    new PlayerScript_WoDPvPDemonology2PBonus();
    new spell_warl_create_healthstone();
    new spell_warl_healthstone();
    new spell_warl_soul_shards_visual();
    new spell_warl_command_demon_spells();
    new spell_warl_cripple_doomguard();
    new PlayerScript_DemonicFury_On_Kill();
    new PlayerScript_spell_warl_drain_life_meta_cost();
}
#endif
