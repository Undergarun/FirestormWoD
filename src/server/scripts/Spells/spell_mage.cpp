////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mage_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

enum MageSpells
{
    SPELL_MAGE_COLD_SNAP                         = 11958,
    SPELL_MAGE_ALTER_TIME_OVERRIDED              = 127140,
    SPELL_MAGE_ALTER_TIME                        = 110909,
    NPC_PAST_SELF                                = 58542,
    SPELL_MAGE_TEMPORAL_DISPLACEMENT             = 80354,
    HUNTER_SPELL_INSANITY                        = 95809,
    SPELL_SHAMAN_SATED                           = 57724,
    SPELL_SHAMAN_EXHAUSTED                       = 57723,
    HUNTER_SPELL_FATIGUED                        = 160455,
    SPELL_MAGE_CONJURE_REFRESHMENT_R1            = 92739,
    SPELL_MAGE_CONJURE_REFRESHMENT_R2            = 92799,
    SPELL_MAGE_CONJURE_REFRESHMENT_R3            = 92802,
    SPELL_MAGE_CONJURE_REFRESHMENT_R4            = 92805,
    SPELL_MAGE_CONJURE_REFRESHMENT_R5            = 74625,
    SPELL_MAGE_CONJURE_REFRESHMENT_R6            = 42956,
    SPELL_MAGE_CONJURE_REFRESHMENT_R7            = 92727,
    SPELL_MAGE_CONJURE_REFRESHMENT_R8            = 116130,
    SPELL_MAGE_MANA_GEM_ENERGIZE                 = 10052,
    SPELL_MAGE_INFERNO_BLAST                     = 108853,
    SPELL_MAGE_INFERNO_BLAST_IMPACT              = 118280,
    SPELL_MAGE_IGNITE                            = 12654,
    SPELL_MAGE_PYROBLAST                         = 11366,
    SPELL_MAGE_COMBUSTION_DOT                    = 83853,
    SPELL_MAGE_FROSTJAW                          = 102051,
    SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE      = 114954,
    SPELL_MAGE_NETHER_TEMPEST_MISSILE            = 114956,
    SPELL_MAGE_LIVING_BOMB_TRIGGERED             = 44461,
    SPELL_MAGE_FROST_BOMB_TRIGGERED              = 113092,
    SPELL_MAGE_BRAIN_FREEZE                      = 44549,
    SPELL_MAGE_BRAIN_FREEZE_TRIGGERED            = 57761,
    SPELL_MAGE_SLOW                              = 31589,
    SPELL_MAGE_ARCANE_CHARGE                     = 36032,
    SPELL_MAGE_ARCANE_BARRAGE_TRIGGERED          = 50273,
    SPELL_MAGE_CAUTERIZE                         = 87023,
    SPELL_MAGE_ARCANE_MISSILES                   = 79683,
    SPELL_MAGE_INCANTERS_FLOW                    = 116267,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK                = 115723,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK_IMMUNITY       = 115760,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK_FROST_NOVA     = 115757,
    SPELL_MAGE_FINGER_OF_FROST_VISUAL            = 44544,
    SPELL_MAGE_FINGER_OF_FROST_EFFECT            = 126084,
    SPELL_MAGE_GLYPH_OF_SLOW                     = 86209,
    SPELL_MAGE_GREATED_INVISIBILITY              = 110960,
    SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE  = 113862,
    SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER = 122293,
    SPELL_MAGE_ICE_BLOCK                         = 45438,
    SPELL_MAGE_CONE_OF_COLD                      = 120,
    SPELL_MAGE_FROST_NOVA                        = 122,
    SPELL_MAGE_FINGERS_OF_FROST_AURA             = 112965,
    SPELL_MAGE_MIRROR_IMAGE_LEFT                 = 58834,
    SPELL_MAGE_MIRROR_IMAGE_RIGHT                = 58833,
    SPELL_MAGE_MIRROR_IMAGE_FRONT                = 58831,
    SPELL_MAGE_ARCANE_BLAST                      = 30451,
    SPELL_MAGE_FIREBALL                          = 133,
    SPELL_MAGE_FROSTBOLT                         = 116,
    SPELL_MAGE_FROSTFIRE_BOLT                    = 44614,
    SPELL_MAGE_UNSTABLE_MAGIC                    = 157976,
    SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FIRE        = 157977,
    SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FROST       = 157978,
    SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_ARCANE      = 157979,
    SPELL_MAGE_ARCANE_POWER                      = 12042,
    SPELL_MAGE_OVERPOWERED                       = 155147,
    SPELL_MAGE_ICY_VEINS                         = 12472,
    SPELL_MAGE_THERMAL_VOID                      = 155149,
    SPELL_MAGE_HEATING_UP                        = 48108,
    SPELL_MAGE_KINDLING                          = 155148,
    SPELL_MAGE_COMBUSTION                        = 11129,
    SPELL_MAGE_FROST_BOMB_AURA                   = 112948,
    SPELL_MAGE_FROST_BOMB_VISUAL                 = 64627,
    SPELL_MAGE_FROST_BOMB_VISUAL_TARGETING       = 70022,
    SPELL_MAGE_RING_OF_FROST_AURA                = 82691,
    SPELL_MAGE_IMPROVED_SCORCH                   = 157632,
    SPELL_MAGE_IMPROVED_SCORCH_AURA              = 157633,
    SPELL_MAGE_ENHANCED_PYROTECHNICS_PROC        = 157644,
    SPELL_MAGE_ENHANCED_ARCANE_BLAST             = 157595,
    SPELL_MAGE_IMPROVED_BLINK                    = 157606,
    SPELL_MAGE_IMPROVED_BLINK_PROC               = 157610,
    SPELL_MAGE_IMPROVED_BLIZZARD                 = 157727,
    SPELL_MAGE_FORZEN_ORB                        = 84714,   /// @TODO: This spell doesn't have visual effect, need to sniff
    SPELL_MAGE_ENHANCED_FROSTBOLT                = 157646,
    SPELL_MAGE_ENHANCED_FROSTBOLT_PROC           = 157648,
    SPELL_MAGE_FLAMEGLOW                         = 140468,
    SPELL_MAGE_RAPID_TELEPORTATION               = 46989,
    SPELL_MAGE_RAPID_TELEPORTATION_AURA          = 89749,
    SPELL_MAGE_RING_OF_FROST_IMMUNATE            = 91264,
    SPELL_MAGE_LIVING_BOMB                       = 44457,
    SPELL_MAGE_CHILLED                           = 12486,
    SPELL_SHAMAN_HEX                             = 51514,
    SPELL_MAGE_WOD_PVP_FIRE_2P_BONUS             = 165977,
    SPELL_MAGE_WOD_PVP_FIRE_2P_BONUS_EFFECT      = 165979,
    SPELL_MAGE_WOD_PVP_FIRE_4P_BONUS             = 171169,
    SPELL_MAGE_WOD_PVP_FIRE_4P_BONUS_EFFECT      = 171170,
    SPELL_MAGE_POLYMORPH_CRITTERMORPH            = 120091,
    SPELL_MAGE_DRAGON_BREATH                     = 31661,
    SPELL_MAGE_PRESENCE_OF_MIND                  = 12043
};

/// Item - Mage WoD PvP Frost 2P Bonus - 180723
class spell_areatrigger_mage_wod_frost_2p_bonus : public AreaTriggerEntityScript
{
public:
    spell_areatrigger_mage_wod_frost_2p_bonus()
        : AreaTriggerEntityScript("at_mage_wod_frost_2p_bonus")
    {
    }

    enum eSpells
    {
        SlickIce = 180724
    };

    AreaTriggerEntityScript* GetAI() const
    {
        return new spell_areatrigger_mage_wod_frost_2p_bonus();
    }

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
    {
        std::list<Unit*> targetList;
        float l_Radius = 20.0f;
        Unit* l_Caster = p_AreaTrigger->GetCaster();

        JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
        JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(p_AreaTrigger, targetList, u_check);
        p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

        for (auto itr : targetList)
        {
            if (itr->GetDistance(p_AreaTrigger) <= 6.0f)
                l_Caster->CastSpell(itr, eSpells::SlickIce, true);
            else
                itr->RemoveAura(eSpells::SlickIce, l_Caster->GetGUID());
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
    {
        std::list<Unit*> targetList;
        float l_Radius = 10.0f;
        Unit* l_Caster = p_AreaTrigger->GetCaster();

        JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
        JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(p_AreaTrigger, targetList, u_check);
        p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

        for (auto itr : targetList)
        {
            if (itr->HasAura(eSpells::SlickIce, l_Caster->GetGUID()))
                itr->RemoveAura(eSpells::SlickIce, l_Caster->GetGUID());
        }
    }
};

/// Arcane Charge - 36032
class spell_mage_arcane_charge : public SpellScriptLoader
{
    public:
        spell_mage_arcane_charge() : SpellScriptLoader("spell_mage_arcane_charge") { }

        class spell_mage_arcane_charge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_arcane_charge_AuraScript);

            enum eDatas
            {
                EnhancedArcaneBlast = 157595
            };

            void CalulcateSpellMod(AuraEffect const* p_AurEff, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eDatas::EnhancedArcaneBlast))
                    {
                        SpellInfo const* l_EnhancedArcaneBlast = sSpellMgr->GetSpellInfo(eDatas::EnhancedArcaneBlast);
                        if (l_EnhancedArcaneBlast == nullptr)
                            return;

                        if (Aura* l_Aura = p_AurEff->GetBase())
                            p_Amount = -int32(l_EnhancedArcaneBlast->Effects[EFFECT_0].BasePoints);
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_arcane_charge_AuraScript::CalulcateSpellMod, EFFECT_4, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_arcane_charge_AuraScript();
        }
};

/// Meteor - 153561
class spell_mage_meteor : public SpellScriptLoader
{
    public:
        spell_mage_meteor() : SpellScriptLoader("spell_mage_meteor") { }

        class spell_mage_meteor_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_meteor_SpellScript);

            enum eMeteorDatas
            {
                MeteorTimeStamp = 177345
            };

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (WorldLocation const* l_Dest = GetExplTargetDest())
                        l_Caster->CastSpell(l_Dest->GetPositionX(), l_Dest->GetPositionY(), l_Dest->GetPositionZ(), eMeteorDatas::MeteorTimeStamp, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_mage_meteor_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_meteor_SpellScript();
        }
};

/// Prismatic Crystal - 155152
class spell_mage_prysmatic_crystal_damage : public SpellScriptLoader
{
    public:
        spell_mage_prysmatic_crystal_damage() : SpellScriptLoader("spell_mage_prysmatic_crystal_damage") { }

        class spell_mage_prysmatic_crystal_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_prysmatic_crystal_damage_SpellScript);

            enum eCreature
            {
                PrismaticCrystalNpc = 76933
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->ToUnit() == nullptr)
                        return true;

                    if (p_Object->ToUnit()->GetEntry() == eCreature::PrismaticCrystalNpc)
                        return true;

                    return false;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_prysmatic_crystal_damage_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_prysmatic_crystal_damage_SpellScript();
        }
};

/// Last Update 6.2.3
/// Comet Storm - 153595 and Comet Storm damage spel - 153596
class spell_mage_comet_storm : public SpellScriptLoader
{
    public:
        spell_mage_comet_storm() : SpellScriptLoader("spell_mage_comet_storm") { }

        class spell_mage_comet_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_comet_storm_SpellScript);

            enum eCometDatas
            {
                MaxComets   = 7,
                CometStorm  = 153596
            };

            uint16 m_CountTarget = 0;
            bool m_AlreadyLaunch = false;

            void OnCometFalls(SpellEffIndex p_Idx)
            {
                if (m_AlreadyLaunch) ///< Prevent to enter on each targets hit
                    return;

                m_AlreadyLaunch = true;

                Unit* l_Caster = GetCaster();

                if (WorldLocation const* l_Dest = GetExplTargetDest())
                {
                    int32 l_AmountOfUsedComets = l_Caster->GetAmountOfComets();
                    if (GetSpellInfo()->Id == 153596 && l_AmountOfUsedComets >= 1 && l_AmountOfUsedComets <= 7)
                    {
                        l_Caster->SetAmountOfComets(l_AmountOfUsedComets + 1);
                        /// It's done, all comets are used
                        if (l_AmountOfUsedComets >= 7)
                            l_Caster->SetAmountOfComets(0);
                        else
                        {
                            float l_X = l_Caster->GetCometStartCoordinateX() + frand(-4.0f, 4.0f);
                            float l_Y = l_Caster->GetCometStartCoordinateY() + frand(-4.0f, 4.0f);

                            l_Caster->CastSpell(l_X, l_Y, l_Dest->m_positionZ, eCometDatas::CometStorm, true);
                        }
                    }
                }
            }
            
            void HandleAfterHit()
            {
                if (GetSpellInfo()->Id == eCometDatas::CometStorm)
                {
                    int32 l_Damage = GetHitDamage();

                    if (Unit* l_Target = GetHitUnit())
                    {
                        /// Damage split between all enemies
                        if (GetSpell()->GetUnitTargetCount() > 1)
                            l_Damage = int32(GetHitDamage() / GetSpell()->GetUnitTargetCount());

                        /// Comet Storm (Frost) damage has increased by 94% but deals 33.3% less damage in PvP combat. - 6.1
                        if (l_Target && l_Target->IsPlayer())
                            l_Damage = l_Damage - CalculatePct(l_Damage, 33.3f);

                        SetHitDamage(l_Damage);

                        /// Polymorph
                        if (l_Target->IsPolymorphed())
                            l_Target->RemoveAurasDueToSpell(l_Target->getTransForm());
                    }
                }
            }

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (WorldLocation const* l_Dest = GetExplTargetDest())
                    {
                        if (GetSpellInfo()->Id == 153595)
                        {
                            /// Store launch information
                            l_Caster->SetAmountOfComets(1);
                            l_Caster->SetCometStartCoordinateX(l_Dest->m_positionX);
                            l_Caster->SetCometStartCoordinateY(l_Dest->m_positionY);
                            /// First comet
                            l_Caster->CastSpell(l_Dest->m_positionX, l_Dest->m_positionY, l_Dest->m_positionZ, eCometDatas::CometStorm, true);
                        }
                    }
                }
            }

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                m_CountTarget = p_Targets.size();
            }

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                if (m_CountTarget)
                    SetHitDamage(GetHitDamage() / m_CountTarget);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case 153596:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_comet_storm_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                    OnEffectHitTarget += SpellEffectFn(spell_mage_comet_storm_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                    OnEffectHit += SpellEffectFn(spell_mage_comet_storm_SpellScript::OnCometFalls, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                    break;
                default:
                    break;
                }
                AfterCast += SpellCastFn(spell_mage_comet_storm_SpellScript::HandleAfterCast);
                AfterHit += SpellHitFn(spell_mage_comet_storm_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_comet_storm_SpellScript();
        }
};

/// Greater Invisibility (remove timer) - 122293
class spell_mage_greater_invisibility_removed: public SpellScriptLoader
{
    public:
        spell_mage_greater_invisibility_removed() : SpellScriptLoader("spell_mage_greater_invisibility_removed") { }

        class spell_mage_greater_invisibility_removed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_greater_invisibility_removed_AuraScript);

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAura(SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_greater_invisibility_removed_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_greater_invisibility_removed_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Greater Invisibility (triggered) - 110960
class spell_mage_greater_invisibility_triggered: public SpellScriptLoader
{
    public:
        spell_mage_greater_invisibility_triggered() : SpellScriptLoader("spell_mage_greater_invisibility_triggered") { }

        class spell_mage_greater_invisibility_triggered_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_greater_invisibility_triggered_AuraScript);

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Target->CastSpell(l_Target, SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE, true);
                    l_Target->CombatStop();

                    Unit::AuraEffectList const& l_AuraListDamage = l_Target->GetAuraEffectsByType(AuraType::SPELL_AURA_PERIODIC_DAMAGE);
                    Unit::AuraEffectList const& l_AuraListDummy = l_Target->GetAuraEffectsByType(AuraType::SPELL_AURA_PERIODIC_DUMMY);
                    std::list<uint32> l_ListID;

                    for (AuraEffect* l_AuraDummy : l_AuraListDummy)
                    {
                        if (!l_AuraDummy->GetSpellInfo()->IsPositive())
                            l_ListID.push_back(l_AuraDummy->GetId());
                    }
                    for (AuraEffect* l_AuraDamage : l_AuraListDamage)
                    {
                        l_ListID.push_back(l_AuraDamage->GetId());
                    }

                    JadeCore::Containers::RandomResizeList(l_ListID, 2);
                    for (uint32 l_ID : l_ListID)
                        l_Target->RemoveAura(l_ID);
                }
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_greater_invisibility_triggered_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_greater_invisibility_triggered_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_greater_invisibility_triggered_AuraScript();
        }
};

// Called by Arcane Blast - 30451
// Glyph of Slow - 86209
class spell_mage_glyph_of_slow: public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_slow() : SpellScriptLoader("spell_mage_glyph_of_slow") { }

        class spell_mage_glyph_of_slow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_glyph_of_slow_SpellScript);

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(SPELL_MAGE_GLYPH_OF_SLOW))
                        return;

                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_SLOW, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_slow_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_glyph_of_slow_SpellScript();
        }
};

/// Last Update 6.2.3
/// Frost Nova (Water Elemental) - 33395
class spell_mage_pet_frost_nova: public SpellScriptLoader
{
    public:
        spell_mage_pet_frost_nova() : SpellScriptLoader("spell_mage_pet_frost_nova") { }

        class spell_mage_pet_frost_nova_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_pet_frost_nova_AuraScript);

            enum eSpells
            {
                FingerFrost = 44544,
                FingerFrostVisual = 126084
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetOwner())
                    {
                        if (Player* l_Player = l_Caster->GetOwner()->ToPlayer())
                        {
                            if (l_Player->HasAura(eSpells::FingerFrost))
                                l_Player->CastSpell(l_Player, eSpells::FingerFrostVisual, true); ///< Fingers of frost visual 2 procs
                            l_Player->CastSpell(l_Player, eSpells::FingerFrost, true);  ///< Fingers of frost proc
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_pet_frost_nova_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_ROOT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_pet_frost_nova_AuraScript();
        }
};

// Called by Ice Block - 45438
class spell_mage_ice_block : public SpellScriptLoader
{
    public:
        spell_mage_ice_block() : SpellScriptLoader("spell_mage_ice_block") { }
        
        class spell_mage_ice_block_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_ice_block_SpellScript);
              
            enum eSpells
            {
                Cyclone = 33786
            };

            SpellCastResult CheckCyclone()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(Cyclone))
                        l_Caster->RemoveAura(Cyclone);
                }

                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_mage_ice_block_SpellScript::CheckCyclone);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_ice_block_SpellScript();
        }
};


// Called by Ice Block - 45438
// Glyph of Ice Block - 115723
class spell_mage_glyph_of_ice_block: public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_ice_block() : SpellScriptLoader("spell_mage_glyph_of_ice_block") { }

        class spell_mage_glyph_of_ice_block_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_glyph_of_ice_block_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetTarget()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_MAGE_GLYPH_OF_ICE_BLOCK))
                    {
                        _player->CastSpell(_player, SPELL_MAGE_GLYPH_OF_ICE_BLOCK_FROST_NOVA, true);
                        _player->CastSpell(_player, SPELL_MAGE_GLYPH_OF_ICE_BLOCK_IMMUNITY, true);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_ice_block_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_glyph_of_ice_block_AuraScript();
        }
};

// Arcane Missiles - 5143
class spell_mage_arcane_missile: public SpellScriptLoader
{
    public:
        spell_mage_arcane_missile() : SpellScriptLoader("spell_mage_arcane_missile") { }

        class spell_mage_arcane_missile_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_arcane_missile_AuraScript);

            enum eSpell
            {
                ArcaneInstability = 166872
            };

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_ARCANE_CHARGE, true);

                if (Player* _player = GetCaster()->ToPlayer())
                    if (Aura* arcaneMissiles = _player->GetAura(SPELL_MAGE_ARCANE_MISSILES))
                        arcaneMissiles->DropCharge();
                
                if (l_Caster->HasAura(eSpell::ArcaneInstability))
                    l_Caster->RemoveAura(eSpell::ArcaneInstability);
            }


            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_arcane_missile_AuraScript::OnApply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }

        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_arcane_missile_AuraScript();
        }

        class spell_mage_arcane_missile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_arcane_missile_SpellScript);



            void HandleOnCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_OVERPOWERED);

                    if (l_Caster->HasSpell(SPELL_MAGE_OVERPOWERED) && l_SpellInfo != nullptr)
                        if (Aura* l_Aura = l_Caster->GetAura(SPELL_MAGE_ARCANE_POWER, l_Caster->GetGUID()))
                            l_Aura->SetDuration(l_Aura->GetDuration() + l_SpellInfo->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_mage_arcane_missile_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_arcane_missile_SpellScript();
        }
};

// Cauterize - 86949
class spell_mage_cauterize: public SpellScriptLoader
{
    public:
        spell_mage_cauterize() : SpellScriptLoader("spell_mage_cauterize") { }

        class spell_mage_cauterize_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_cauterize_AuraScript);

            int32 absorbChance;
            int32 healtPct;

            bool Load()
            {
                absorbChance = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                healtPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
                return GetUnitOwner()->ToPlayer();
            }

            void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                if (target->ToPlayer()->HasSpellCooldown(SPELL_MAGE_CAUTERIZE))
                    return;

                if (!roll_chance_i(absorbChance))
                    return;

                int bp1 = target->CountPctFromMaxHealth(healtPct);
                target->CastCustomSpell(target, SPELL_MAGE_CAUTERIZE, NULL, &bp1, NULL, true);
                target->ToPlayer()->AddSpellCooldown(SPELL_MAGE_CAUTERIZE, 0, 60 * IN_MILLISECONDS);

                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_cauterize_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_cauterize_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_cauterize_AuraScript();
        }
};

class CheckArcaneBarrageImpactPredicate
{
    public:
        CheckArcaneBarrageImpactPredicate(Unit* caster, Unit* mainTarget) : _caster(caster), _mainTarget(mainTarget) {}

        bool operator()(Unit* target)
        {
            if (!_caster || !_mainTarget)
                return true;

            if (!_caster->IsValidAttackTarget(target))
                return true;

            if (!target->IsWithinLOSInMap(_caster))
                return true;

            if (!_caster->isInFront(target))
                return true;

            if (target->GetGUID() == _caster->GetGUID())
                return true;

            if (target->GetGUID() == _mainTarget->GetGUID())
                return true;

            return false;
        }

    private:
        Unit* _caster;
        Unit* _mainTarget;
};

/// Arcane Barrage - 44425
class spell_mage_arcane_barrage: public SpellScriptLoader
{
    public:
        spell_mage_arcane_barrage() : SpellScriptLoader("spell_mage_arcane_barrage") { }

        class spell_mage_arcane_barrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_arcane_barrage_SpellScript);

            uint64 m_MainTargetGUID = 0;

            void HandleBeforeCast()
            {
                if (Unit* l_MainTarget = GetExplTargetUnit())
                    m_MainTargetGUID = l_MainTarget->GetGUID();
            }

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Target = GetHitUnit();
                Unit* l_Caster = GetCaster();

                if (l_Target == nullptr)
                    return;

                if (l_Target->GetGUID() != m_MainTargetGUID)
                    SetHitDamage(CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_1].BasePoints));

                if (l_Caster->HasAura(SPELL_MAGE_ARCANE_CHARGE))
                    l_Caster->RemoveAura(SPELL_MAGE_ARCANE_CHARGE);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_mage_arcane_barrage_SpellScript::HandleBeforeCast);
                OnEffectHitTarget += SpellEffectFn(spell_mage_arcane_barrage_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_arcane_barrage_SpellScript();
        }
};

// Arcane Explosion - 1449
class spell_mage_arcane_explosion: public SpellScriptLoader
{
    public:
        spell_mage_arcane_explosion() : SpellScriptLoader("spell_mage_arcane_explosion") { }

        class spell_mage_arcane_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_arcane_explosion_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_MAGE_ARCANE)
                            if (Aura* arcaneCharge = _player->GetAura(SPELL_MAGE_ARCANE_CHARGE))
                                arcaneCharge->RefreshDuration();
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_arcane_explosion_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_arcane_explosion_SpellScript();
        }
};

/// Last Update 6.1.2
/// Frostbolt - 116
class spell_mage_frostbolt: public SpellScriptLoader
{
    public:
        spell_mage_frostbolt() : SpellScriptLoader("spell_mage_frostbolt") { }

        class spell_mage_frostbolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_frostbolt_SpellScript);

            bool m_TargetIsSnare = false;

            enum eSpells
            {
                WoDPvPFrost4PBonus = 180741,
                WaterJet           = 135029,
                FingerFrost        = 44544,
                FingerFrostVisual = 126084
            };

            SpellCastResult CheckTarget()
            {
                if (!GetExplTargetUnit())
                    return SPELL_FAILED_NO_VALID_TARGETS;
                else if (GetExplTargetUnit()->GetGUID() == GetCaster()->GetGUID())
                    return SPELL_FAILED_BAD_TARGETS;
                else if (GetExplTargetUnit()->IsPlayer() && !GetExplTargetUnit()->IsPvP())
                {
                    if (GetCaster()->IsPlayer() && GetCaster()->ToPlayer()->m_Duel)
                    if (GetCaster()->ToPlayer()->m_Duel->opponent->GetGUID() == GetExplTargetUnit()->GetGUID())
                            return SPELL_CAST_OK;

                    return SPELL_FAILED_BAD_TARGETS;
                }
                else if (GetExplTargetUnit()->GetOwner() != GetCaster())
                {
                    if (GetCaster()->IsValidAttackTarget(GetExplTargetUnit()))
                        return SPELL_CAST_OK;

                    return SPELL_FAILED_BAD_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void HandleOnPrepare()
            {
                Unit* l_Target = GetExplTargetUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED) || l_Target->HasAuraType(SPELL_AURA_MOD_ROOT))
                    m_TargetIsSnare = true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::WoDPvPFrost4PBonus);

                if (l_Target == nullptr || l_SpellInfo == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::WoDPvPFrost4PBonus))
                {
                    if (m_TargetIsSnare)
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_SpellInfo->Effects[EFFECT_0].BasePoints));
                }
            }

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_BRAIN_FREEZE);

                if (l_SpellInfo == nullptr)
                    return;

                Pet* l_Pet = l_Player->GetPet();

                if (l_Pet == nullptr)
                    return;

                if (l_Target->HasAura(eSpells::WaterJet, l_Pet->GetGUID()))
                {
                    if (l_Player->HasAura(eSpells::FingerFrost))
                        l_Player->CastSpell(l_Player, eSpells::FingerFrostVisual, true); ///< Fingers of frost visual 2 procs
                    l_Player->CastSpell(l_Player, eSpells::FingerFrost, true);  ///< Fingers of frost proc
                }
            }

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    
                    if (l_Caster->HasAura(SPELL_MAGE_ENHANCED_FROSTBOLT) && l_Caster->getLevel() >= 92 && !l_Caster->HasAura(SPELL_MAGE_ENHANCED_FROSTBOLT_PROC))
                        l_Caster->CastSpell(l_Caster, SPELL_MAGE_ENHANCED_FROSTBOLT_PROC, true);
                }
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_mage_frostbolt_SpellScript::HandleOnPrepare);
                OnEffectHitTarget += SpellEffectFn(spell_mage_frostbolt_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnHit += SpellHitFn(spell_mage_frostbolt_SpellScript::HandleOnHit);
                OnCheckCast += SpellCheckCastFn(spell_mage_frostbolt_SpellScript::CheckTarget);
                AfterCast += SpellCastFn(spell_mage_frostbolt_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_frostbolt_SpellScript();
        }
};

class CheckNetherImpactPredicate
{
    public:
        CheckNetherImpactPredicate(Unit* caster, Unit* mainTarget) : _caster(caster), _mainTarget(mainTarget) {}

        bool operator()(Unit* target)
        {
            if (!_caster || !_mainTarget)
                return true;

            if (!_caster->IsValidAttackTarget(target))
                return true;

            if (!target->IsWithinLOSInMap(_caster))
                return true;

            if (!_caster->isInFront(target))
                return true;

            if (target->GetGUID() == _caster->GetGUID())
                return true;

            if (target->GetGUID() == _mainTarget->GetGUID())
                return true;

            return false;
        }

    private:
        Unit* _caster;
        Unit* _mainTarget;
};

/// Last Update 6.2.3
/// Nether Tempest - 114954
class spell_mage_nether_tempest_damage : public SpellScriptLoader
{
    public:
        spell_mage_nether_tempest_damage() : SpellScriptLoader("spell_mage_nether_tempest_damage") { }

        class spell_mage_nether_tempest_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_nether_tempest_damage_SpellScript);

            enum eSpells
            {
                NetherTempestAura           = 114923,
                NetherTempestLuncherVisual  = 114956
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_FirstTarget = nullptr;
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                for (WorldObject* l_Unit : p_Targets)
                {
                    if (l_Unit->ToUnit() && l_Unit->ToUnit()->HasAura(eSpells::NetherTempestAura, l_Caster->GetGUID()))
                        l_FirstTarget = l_Unit->ToUnit();
                }

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->ToUnit() == nullptr)
                        return true;

                    if (p_Object->ToUnit()->HasAura(eSpells::NetherTempestAura, l_Caster->GetGUID()))
                        return true;

                    return false;
                });

                if (l_FirstTarget == nullptr)
                    return;

                for (WorldObject* l_Unit : p_Targets)
                {
                    l_FirstTarget->CastSpell(l_Unit->ToUnit(), eSpells::NetherTempestLuncherVisual, true);
                }
            }

            void HandleDamage()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_ARCANE_CHARGE);

                    if (l_Caster->HasAura(SPELL_MAGE_ARCANE_CHARGE) && l_SpellInfo != nullptr)
                    {
                        if (Aura* l_ArcaneCharge = l_Caster->GetAura(SPELL_MAGE_ARCANE_CHARGE))
                        {
                            int32 l_Damage = GetHitDamage();
                            SetHitDamage(AddPct(l_Damage, int32(l_SpellInfo->Effects[EFFECT_0].BasePoints * l_ArcaneCharge->GetStackAmount())));
                        }
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_nether_tempest_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnHit += SpellHitFn(spell_mage_nether_tempest_damage_SpellScript::HandleDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_nether_tempest_damage_SpellScript();
        }
};

/// Nether Tempest - 114923
class spell_mage_nether_tempest : public SpellScriptLoader
{
    public:
        spell_mage_nether_tempest() : SpellScriptLoader("spell_mage_nether_tempest") { }

        class spell_mage_nether_tempest_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_nether_tempest_AuraScript);

            void CalculateAmount(AuraEffect const*, int32& p_Amount, bool&)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_ARCANE_CHARGE);

                    if (l_Caster->HasAura(SPELL_MAGE_ARCANE_CHARGE) && l_SpellInfo != nullptr)
                    {
                        if (Aura* l_ArcaneCharge = l_Caster->GetAura(SPELL_MAGE_ARCANE_CHARGE))
                            p_Amount += CalculatePct(p_Amount, l_SpellInfo->Effects[EFFECT_0].BasePoints) * l_ArcaneCharge->GetStackAmount();
                    }
                }
            }

            void OnTick(AuraEffect const*)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_nether_tempest_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_nether_tempest_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_nether_tempest_AuraScript();
        }
};

// Blazing Speed - 108843
class spell_mage_blazing_speed: public SpellScriptLoader
{
    public:
        spell_mage_blazing_speed() : SpellScriptLoader("spell_mage_blazing_speed") { }

        class spell_mage_blazing_speed_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_blazing_speed_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->RemoveMovementImpairingAuras();

                    if (l_Player->HasAura(SPELL_MAGE_GREATED_INVISIBILITY))
                    {
                        l_Player->RemoveAura(SPELL_MAGE_GREATED_INVISIBILITY);
                        l_Player->RemoveAura(SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_blazing_speed_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_blazing_speed_SpellScript();
        }
};

// Frostjaw - 102051
class spell_mage_frostjaw: public SpellScriptLoader
{
    public:
        spell_mage_frostjaw() : SpellScriptLoader("spell_mage_frostjaw") { }

        class spell_mage_frostjaw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_frostjaw_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->IsPlayer())
                        {
                            if (Aura* frostjaw = target->GetAura(SPELL_MAGE_FROSTJAW, _player->GetGUID()))
                            {
                                // Only half time against players
                                frostjaw->SetDuration(frostjaw->GetMaxDuration() / 2);
                                frostjaw->SetMaxDuration(frostjaw->GetDuration());
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_frostjaw_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_frostjaw_SpellScript();
        }
};

/// Last Update 6.2.3
/// Combustion - 11129
class spell_mage_combustion: public SpellScriptLoader
{
    public:
        spell_mage_combustion() : SpellScriptLoader("spell_mage_combustion") { }

        class spell_mage_combustion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_combustion_SpellScript);

            enum eSpells
            {
                InfernoBlast = 108853,
                Combustion = 11129
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();
                SpellInfo const* l_SpellInfoCombustionDoT = sSpellMgr->GetSpellInfo(SPELL_MAGE_COMBUSTION_DOT);

                if (l_Player == nullptr || l_Target == nullptr || l_SpellInfoCombustionDoT == nullptr)
                    return;

                if (SpellInfo const* l_InfernoBlast = sSpellMgr->GetSpellInfo(eSpells::InfernoBlast))
                    l_Player->RestoreCharge(l_InfernoBlast->ChargeCategoryEntry);

                int32 combustionBp = 0;

                Unit::AuraEffectList const& aurasPereodic = l_Target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
                for (Unit::AuraEffectList::const_iterator i = aurasPereodic.begin(); i !=  aurasPereodic.end(); ++i)
                {
                    if ((*i)->GetCasterGUID() != l_Player->GetGUID() || (*i)->GetSpellInfo()->SchoolMask != SPELL_SCHOOL_MASK_FIRE)
                        continue;

                    if (!(*i)->GetAmplitude())
                        continue;

                    if ((*i)->GetId() != 12654)
                        continue;

                    int32 l_Amount = l_Player->SpellDamageBonusDone(l_Target, (*i)->GetSpellInfo(), (*i)->GetAmount(), (*i)->GetEffIndex(), DOT) * 1000 / (*i)->GetAmplitude();
                    l_Amount = l_Target->SpellDamageBonusTaken(l_Player, GetSpellInfo(), l_Amount, DOT);

                    combustionBp = (l_Amount * (sSpellMgr->GetSpellInfo(eSpells::Combustion))->Effects[EFFECT_0].BasePoints) / (l_SpellInfoCombustionDoT->GetMaxDuration() / l_SpellInfoCombustionDoT->Effects[EFFECT_0].Amplitude);
                }

                if (combustionBp)
                    l_Player->CastCustomSpell(l_Target, SPELL_MAGE_COMBUSTION_DOT, &combustionBp, NULL, NULL, true);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_mage_combustion_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_combustion_SpellScript();
        }
};

class CheckInfernoBlastImpactPredicate
{
    public:
        CheckInfernoBlastImpactPredicate(Unit* caster, Unit* mainTarget) : _caster(caster), _mainTarget(mainTarget) {}

        bool operator()(Unit* target)
        {
            if (!_caster || !_mainTarget)
                return true;

            if (!_caster->IsValidAttackTarget(target))
                return true;

            if (!target->IsWithinLOSInMap(_caster))
                return true;

            if (!_caster->isInFront(target))
                return true;

            if (target->GetGUID() == _caster->GetGUID())
                return true;

            if (target->GetGUID() == _mainTarget->GetGUID())
                return true;

            return false;
        }

    private:
        Unit* _caster;
        Unit* _mainTarget;
};

/// Inferno Blast - 108853
class spell_mage_inferno_blast: public SpellScriptLoader
{
    public:
        spell_mage_inferno_blast() : SpellScriptLoader("spell_mage_inferno_blast") { }

        class spell_mage_inferno_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_inferno_blast_SpellScript);

            enum eSpell
            {
                ImprovedInfernoBlast = 157629
            };

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        std::list<Unit*> l_TargetList;

                        l_Caster->CastSpell(l_Target, SPELL_MAGE_INFERNO_BLAST_IMPACT, true);

                        /// Spreads any Pyroblast, Ignite, Living Bomb and Combustion effects to up to 2 nearby enemy targets within 10 yards
                        l_Target->GetAttackableUnitListInRange(l_TargetList, 10.0f);

                        l_TargetList.remove_if(CheckInfernoBlastImpactPredicate(l_Caster, l_Target));

                        uint8 l_TargetCount = GetSpellInfo()->Effects[EFFECT_1].BasePoints;
                        if (SpellInfo const* l_ImprovedBlast = sSpellMgr->GetSpellInfo(eSpell::ImprovedInfernoBlast))
                        {
                            if (l_Caster->HasAura(l_ImprovedBlast->Id))
                                l_TargetCount += l_ImprovedBlast->Effects[EFFECT_0].BasePoints;
                        }

                        if (l_TargetList.size() > l_TargetCount)
                            JadeCore::Containers::RandomResizeList(l_TargetList, l_TargetCount);

                        for (Unit* l_Unit : l_TargetList)
                        {
                            /// 1 : Ignite
                            if (Aura* l_Aura = l_Target->GetAura(SPELL_MAGE_IGNITE, l_Caster->GetGUID()))
                            {
                                float l_Value = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.5f;

                                int32 l_Ignite = l_Aura->GetEffect(EFFECT_0)->GetAmount();
                                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_IGNITE);

                                if (l_SpellInfo != nullptr)
                                {
                                    if (l_Unit->HasAura(SPELL_MAGE_IGNITE, l_Caster->GetGUID()))
                                        l_Ignite += l_Unit->GetRemainingPeriodicAmount(l_Caster->GetGUID(), SPELL_MAGE_IGNITE, SPELL_AURA_PERIODIC_DAMAGE);

                                    l_Caster->CastCustomSpell(l_Unit, SPELL_MAGE_IGNITE, &l_Ignite, NULL, NULL, true);
                                }
                            }

                            /// 2 : Pyroblast
                            if (Aura* l_AuraPyroblast = l_Target->GetAura(SPELL_MAGE_PYROBLAST, l_Caster->GetGUID()))
                            {
                                l_Caster->AddAura(SPELL_MAGE_PYROBLAST, l_Unit);
                                if (Aura* l_NewAuraPyroblast = l_Unit->GetAura(SPELL_MAGE_PYROBLAST, l_Caster->GetGUID()))
                                    l_NewAuraPyroblast->SetDuration(l_AuraPyroblast->GetDuration());
                            }

                            /// 3 : Living Bomb
                            if (Aura* l_AuraLivingBomb = l_Target->GetAura(SPELL_MAGE_LIVING_BOMB, l_Caster->GetGUID()))
                            {
                                l_Caster->AddAura(SPELL_MAGE_LIVING_BOMB, l_Unit);
                                if (Aura* l_NewAuraLivingBomb = l_Unit->GetAura(SPELL_MAGE_LIVING_BOMB, l_Caster->GetGUID()))
                                    l_NewAuraLivingBomb->SetDuration(l_AuraLivingBomb->GetDuration());
                            }

                            /// 4 : Combustion
                            if (AuraEffect* l_AuraCombustion = l_Target->GetAuraEffect(SPELL_MAGE_COMBUSTION_DOT, EFFECT_0, l_Caster->GetGUID()))
                            {
                                int32 l_PreviousCombustion = 0;

                                if (l_Unit->HasAura(SPELL_MAGE_COMBUSTION_DOT, l_Caster->GetGUID()))
                                    l_PreviousCombustion = l_Unit->GetRemainingPeriodicAmount(l_Caster->GetGUID(), SPELL_MAGE_COMBUSTION_DOT, SPELL_AURA_PERIODIC_DAMAGE);

                                if (Aura* l_Combustion = l_Caster->AddAura(SPELL_MAGE_COMBUSTION_DOT, l_Unit))
                                {
                                    l_Combustion->SetDuration(l_AuraCombustion->GetBase()->GetDuration());
                                    l_Combustion->GetEffect(EFFECT_0)->SetAmount(l_AuraCombustion->GetAmount() + l_PreviousCombustion);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_inferno_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_inferno_blast_SpellScript();
        }
};

// Evocation - 12051
class spell_mage_evocation : public SpellScriptLoader
{
    public:
        spell_mage_evocation() : SpellScriptLoader("spell_mage_evocation") { }

        class spell_mage_evocation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_evocation_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->EnergizeBySpell(l_Player, GetSpellInfo()->Id, int32(l_Player->GetMaxPower(POWER_MANA) * 0.15), POWER_MANA);

                    if (l_Player->HasAura(SPELL_MAGE_ARCANE_CHARGE))
                        l_Player->RemoveAura(SPELL_MAGE_ARCANE_CHARGE);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_evocation_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_evocation_SpellScript();
        }

        class spell_mage_evocation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_evocation_AuraScript);

            enum eSpells
            {
                T17Arcane2P     = 165475,
                ArcaneAffinity  = 166871
            };

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::T17Arcane2P))
                        l_Caster->CastSpell(l_Caster, eSpells::ArcaneAffinity, true);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_evocation_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_OBS_MOD_POWER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_evocation_AuraScript();
        }
};

// Conjure Refreshment - 42955
class spell_mage_conjure_refreshment: public SpellScriptLoader
{
    public:
        spell_mage_conjure_refreshment() : SpellScriptLoader("spell_mage_conjure_refreshment") { }

        class spell_mage_conjure_refreshment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_conjure_refreshment_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->getLevel() < 44)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R1, true);
                    else if (_player->getLevel() < 54)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R2, true);
                    else if (_player->getLevel() < 64)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R3, true);
                    else if (_player->getLevel() < 74)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R4, true);
                    else if (_player->getLevel() < 80)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R5, true);
                    else if (_player->getLevel() < 85)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R6, true);
                    else if (_player->getLevel() < 90)
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R7, true);
                    else
                        _player->CastSpell(_player, SPELL_MAGE_CONJURE_REFRESHMENT_R8, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_conjure_refreshment_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_conjure_refreshment_SpellScript();
        }
};

/// Time Warp - 80353
class spell_mage_time_warp : public SpellScriptLoader
{
    public:
        spell_mage_time_warp() : SpellScriptLoader("spell_mage_time_warp") { }

        class spell_mage_time_warp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_time_warp_SpellScript);

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (GetSpellInfo() && l_Target->HasAura(GetSpellInfo()->Id))
                        l_Target->CastSpell(l_Target, SPELL_MAGE_TEMPORAL_DISPLACEMENT, true);
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

            void Register()
            {
                AfterHit += SpellHitFn(spell_mage_time_warp_SpellScript::ApplyDebuff);
                OnEffectHitTarget += SpellEffectFn(spell_mage_time_warp_SpellScript::HandleImmunity, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_mage_time_warp_SpellScript::HandleImmunity, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_time_warp_SpellScript();
        }
};

// Alter Time - 127140 (overrided)
class spell_mage_alter_time_overrided: public SpellScriptLoader
{
    public:
        spell_mage_alter_time_overrided() : SpellScriptLoader("spell_mage_alter_time_overrided") { }

        class spell_mage_alter_time_overrided_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_alter_time_overrided_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(SPELL_MAGE_ALTER_TIME))
                        _player->RemoveAura(SPELL_MAGE_ALTER_TIME);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_mage_alter_time_overrided_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_alter_time_overrided_SpellScript();
        }
};

/// Last Update 6.2.3
/// Alter Time - 110909
class spell_mage_alter_time: public SpellScriptLoader
{
    public:
        spell_mage_alter_time() : SpellScriptLoader("spell_mage_alter_time") { }

        class spell_mage_alter_time_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_alter_time_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode != AURA_REMOVE_BY_EXPIRE && removeMode != AURA_REMOVE_BY_DEFAULT)
                        return;

                    std::list<Creature*> mirrorList;
                    _player->GetCreatureListWithEntryInGrid(mirrorList, NPC_PAST_SELF, 100.0f);

                    if (mirrorList.empty())
                        return;

                    for (std::list<Creature*>::const_iterator itr = mirrorList.begin(); itr != mirrorList.end(); ++itr)
                        if (Creature* pMirror = (*itr)->ToCreature())
                            if (TempSummon* pastSelf = pMirror->ToTempSummon())
                                if (pastSelf->isAlive() && pastSelf->IsInWorld())
                                    if (pastSelf->GetSummoner() && pastSelf->GetSummoner()->GetGUID() == _player->GetGUID())
                                        pastSelf->AI()->DoAction(1);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_alter_time_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_alter_time_AuraScript();
        }
};

/// last update : 6.1.2
/// Cold Snap - 11958
class spell_mage_cold_snap: public SpellScriptLoader
{
    public:
        spell_mage_cold_snap() : SpellScriptLoader("spell_mage_cold_snap") { }

        class spell_mage_cold_snap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_cold_snap_SpellScript);

            enum eSpells
            {
                Evanesce        = 157913,
                IceBlock        = 45438,
                FrostNova       = 122,
                ConeOfCold      = 120,
                DragonBreath    = 31661,
                PresenceOfMind  = 12043
            };

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                /// Resets cooldown of Ice Block, Frost Nova, Evanesce and Cone of Cold
                l_Player->RemoveSpellCooldown(eSpells::IceBlock, true);
                l_Player->RemoveSpellCooldown(eSpells::FrostNova, true);
                l_Player->RemoveSpellCooldown(eSpells::Evanesce, true);

                if (l_Player->GetSpecializationId() == SPEC_MAGE_FROST)
                    l_Player->RemoveSpellCooldown(eSpells::ConeOfCold, true);
                if (l_Player->GetSpecializationId() == SPEC_MAGE_FIRE)
                    l_Player->RemoveSpellCooldown(eSpells::DragonBreath, true);
                if (l_Player->GetSpecializationId() == SPEC_MAGE_ARCANE)
                    l_Player->RemoveSpellCooldown(eSpells::PresenceOfMind, true);
            }

            void Register()
            {
                // add dummy effect spell handler to Cold Snap
                OnEffectHit += SpellEffectFn(spell_mage_cold_snap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_cold_snap_SpellScript();
        }
};

// Living Bomb - 44457
class spell_mage_living_bomb: public SpellScriptLoader
{
    public:
        spell_mage_living_bomb() : SpellScriptLoader("spell_mage_living_bomb") { }

        class spell_mage_living_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_living_bomb_AuraScript);

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode != AURA_REMOVE_BY_DEATH && removeMode != AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_LIVING_BOMB_TRIGGERED, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_living_bomb_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_living_bomb_AuraScript();
        }
};

// Mirror Image - 55342
class spell_mage_mirror_image_summon: public SpellScriptLoader
{
    public:
        spell_mage_mirror_image_summon() : SpellScriptLoader("spell_mage_mirror_image_summon") { }

        class spell_mage_mirror_image_summon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_mirror_image_summon_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_LEFT, true);
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_FRONT, true);
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_RIGHT, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_mirror_image_summon_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_mirror_image_summon_SpellScript();
        }
};

// Ice Barrier - 11426
class spell_mage_ice_barrier: public SpellScriptLoader
{
    public:
        spell_mage_ice_barrier() : SpellScriptLoader("spell_mage_ice_barrier") { }

        class spell_mage_ice_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ice_barrier_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                    amount = l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST) * 4.95f;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_ice_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_ice_barrier_AuraScript();
        }
};

/// Call by Arcane Blast - 30451, Fireball - 133, Frostbolt - 116 and Frostfire Bolt - 44614
/// Unstable Magic - 157976
class spell_mage_unstable_magic: public SpellScriptLoader
{
    public:
        spell_mage_unstable_magic() : SpellScriptLoader("spell_mage_unstable_magic") { }

        class spell_mage_unstable_magic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_unstable_magic_SpellScript);

            void HandleDamage(SpellEffIndex)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Aura* l_Aura = l_Player->GetAura(SPELL_MAGE_UNSTABLE_MAGIC))
                    {
                        int32 l_Chance = 0;
                        switch (l_Player->GetSpecializationId(l_Player->GetActiveSpec()))
                        {
                            case SpecIndex::SPEC_MAGE_ARCANE:
                                l_Chance = l_Aura->GetEffect(EFFECT_0) ? l_Aura->GetEffect(EFFECT_0)->GetAmount() : 0;
                                break;
                            case SpecIndex::SPEC_MAGE_FIRE:
                                l_Chance = l_Aura->GetEffect(EFFECT_2) ? l_Aura->GetEffect(EFFECT_2)->GetAmount() : 0;
                                break;
                            case SpecIndex::SPEC_MAGE_FROST:
                                l_Chance = l_Aura->GetEffect(EFFECT_1) ? l_Aura->GetEffect(EFFECT_1)->GetAmount() : 0;
                                break;
                            default:
                                break;
                        }

                        if (!roll_chance_i(l_Chance))
                            return;

                        if (Unit* l_Target = GetHitUnit())
                        {
                            int32 l_Pct = l_Aura->GetEffect(EFFECT_3) ? l_Aura->GetEffect(EFFECT_3)->GetAmount() : 0;
                            int32 l_BasePoints = CalculatePct(GetHitDamage(), l_Pct);
                            uint32 l_SpellID = GetSpellInfo()->Id;
                            switch (l_SpellID)
                            {
                                case SPELL_MAGE_ARCANE_BLAST:
                                    l_Player->CastCustomSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_ARCANE, &l_BasePoints, nullptr, nullptr, true);
                                    break;
                                case SPELL_MAGE_FROSTBOLT:
                                    l_Player->CastCustomSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FROST, &l_BasePoints, nullptr, nullptr, true);
                                    break;
                                case SPELL_MAGE_FIREBALL:
                                    l_Player->CastCustomSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FIRE, &l_BasePoints, nullptr, nullptr, true);
                                    break;
                                case SPELL_MAGE_FROSTFIRE_BOLT:
                                    if (urand(0, 1))
                                        l_Player->CastCustomSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FROST, &l_BasePoints, nullptr, nullptr, true);
                                    else
                                        l_Player->CastCustomSpell(l_Target, SPELL_MAGE_UNSTABLE_MAGIC_DAMAGE_FIRE, &l_BasePoints, nullptr, nullptr, true);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case SPELL_MAGE_FROSTBOLT:
                    case SPELL_MAGE_FROSTFIRE_BOLT:
                        OnEffectHitTarget += SpellEffectFn(spell_mage_unstable_magic_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                        break;
                    default:
                        OnEffectHitTarget += SpellEffectFn(spell_mage_unstable_magic_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                        break;
                }
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_unstable_magic_SpellScript();
        }
};

/// Last Update 6.2.3
/// Ice Lance - 30455
class spell_mage_ice_lance: public SpellScriptLoader
{
    public:
        spell_mage_ice_lance() : SpellScriptLoader("spell_mage_ice_lance") { }

        class spell_mage_ice_lance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_ice_lance_SpellScript);

            enum eSpells
            {
                T17Frost4P      = 165469,
                IceShard        = 166869,
                FingerOfFrost   = 44544,
                ThermalVoid     = 155149,
                IciVeins        = 12472
            };

            enum eCreature
            {
                FrozenOrb = 45322
            };

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasSpell(eSpells::ThermalVoid))
                {
                    if (Aura* l_Aura = l_Caster->GetAura(eSpells::IciVeins, l_Caster->GetGUID()))
                    {
                        int32 l_IncreaseDuration = sSpellMgr->GetSpellInfo(eSpells::ThermalVoid)->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS;
                        int32 l_NewDuration = (l_Aura->GetDuration() + l_IncreaseDuration);
                        l_Aura->SetDuration(l_NewDuration);
                    }
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                srand(time(NULL));
                int32 l_RandomScale =  rand() % 100 + 1;
                float l_Multiplier = GetSpellInfo()->Effects[EFFECT_0].DeltaScalingMultiplier * 100;
                int32 l_Damage = GetHitDamage();
                int32 l_Scale = CalculatePct(l_Damage, l_Multiplier);

                int32 l_ScaleValue = (l_Scale / 100) * l_RandomScale;

                l_Damage -= (l_Scale / 2);
                l_Damage += l_ScaleValue;
                SetHitDamage(l_Damage);
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                /// Each Ice Lance cast while Frozen Orb is active reduces the cast time of Frostbolt by 2% and increases the damage of Frostbolt by 2% for 10 sec. Stacks up to 10 times.
                if (l_Caster->HasAura(eSpells::T17Frost4P))
                {
                    Creature* l_FrostOrb = nullptr;
                    for (auto l_Iter : l_Caster->m_Controlled)
                    {
                        if (l_Iter->GetEntry() == eCreature::FrozenOrb)
                            l_FrostOrb = l_Iter->ToCreature();
                    }

                    if (l_FrostOrb != nullptr)
                        l_Caster->CastSpell(l_Caster, eSpells::IceShard, true);
                }

                if (Unit* l_Target = GetHitUnit())
                {
                    if (l_Target->HasAura(SPELL_MAGE_FROST_BOMB_AURA, l_Caster->GetGUID()) &&
                        (l_Target->HasAuraState(AURA_STATE_FROZEN) || l_Caster->HasAura(eSpells::FingerOfFrost)))
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_FROST_BOMB_TRIGGERED, true);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_mage_ice_lance_SpellScript::HandleOnCast);
                OnHit += SpellHitFn(spell_mage_ice_lance_SpellScript::HandleOnHit);
                OnEffectHitTarget += SpellEffectFn(spell_mage_ice_lance_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_ice_lance_SpellScript();
        }
};

// Pyroblast - 11366
class spell_mage_pyroblast: public SpellScriptLoader
{
    public:
        spell_mage_pyroblast() : SpellScriptLoader("spell_mage_pyroblast") { }

        class spell_mage_pyroblast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_pyroblast_SpellScript);

            bool m_InstantPyroblast;

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                m_InstantPyroblast = false;

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(SPELL_MAGE_HEATING_UP))
                {
                    m_InstantPyroblast = true;
                    l_Caster->RemoveAura(SPELL_MAGE_HEATING_UP);
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (m_InstantPyroblast)
                {
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), sSpellMgr->GetSpellInfo(SPELL_MAGE_HEATING_UP)->Effects[EFFECT_2].BasePoints));

                    /// Item - Mage WoD PvP Fire 4P Bonus
                    if (l_Caster->HasAura(SPELL_MAGE_WOD_PVP_FIRE_4P_BONUS))
                        l_Caster->CastSpell(l_Target, SPELL_MAGE_WOD_PVP_FIRE_4P_BONUS_EFFECT, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_mage_pyroblast_SpellScript::HandleAfterCast);
                OnEffectHitTarget += SpellEffectFn(spell_mage_pyroblast_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_pyroblast_SpellScript();
        }
};

// FrostFire bolt - 44614
class spell_mage_frostfire_bolt: public SpellScriptLoader
{
    public:
        spell_mage_frostfire_bolt() : SpellScriptLoader("spell_mage_frostfire_bolt") { }

        class spell_mage_frostfire_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_frostfire_bolt_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Aura* l_Aura = l_Caster->GetAura(SPELL_MAGE_BRAIN_FREEZE_TRIGGERED))
                    {
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), sSpellMgr->GetSpellInfo(SPELL_MAGE_BRAIN_FREEZE)->Effects[EFFECT_2].BasePoints));
                        l_Aura->ModStackAmount(-1);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_frostfire_bolt_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_frostfire_bolt_SpellScript();
        }
};

/// Call by Fireball - 133, FrostFire Bolt - 44614, Pyroblast 11366 and Inferno Blast 108853
/// Kindling - 155148
class spell_mage_kindling : public SpellScriptLoader
{
    public:
        spell_mage_kindling() : SpellScriptLoader("spell_mage_kindling") { }

        class spell_mage_kindling_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_kindling_SpellScript);

            enum eSpells
            {
                Ignition = 165979,
                Fireball = 133
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();
                if (l_Player == nullptr || l_Target == nullptr)
                    return;

                if (l_Player->HasAura(SPELL_MAGE_KINDLING) && GetSpell()->IsCritForTarget(l_Target))
                {
                    if (l_Player->HasSpellCooldown(SPELL_MAGE_COMBUSTION))
                        l_Player->ReduceSpellCooldown(SPELL_MAGE_COMBUSTION, sSpellMgr->GetSpellInfo(SPELL_MAGE_KINDLING)->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS);
                }
                if (GetSpellInfo()->Id == eSpells::Fireball && l_Player->HasAura(eSpells::Ignition))
                    l_Player->RemoveAura(eSpells::Ignition);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_mage_kindling_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_kindling_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Ring of Frost - 136511
class spell_mage_ring_of_frost : public SpellScriptLoader
{
    public:
        spell_mage_ring_of_frost() : SpellScriptLoader("spell_mage_ring_of_frost") { }

        class spell_mage_ring_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ring_of_frost_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Creature*> l_TempList;
                    std::list<Creature*> l_FrozenRinglist;

                    // Get all of the Frozen Ring in Area
                    l_Caster->GetCreatureListWithEntryInGrid(l_FrozenRinglist, 44199, 200.0f);

                    l_TempList = l_FrozenRinglist;

                    // Remove other players Frozen Ring
                    for (std::list<Creature*>::iterator i = l_TempList.begin(); i != l_TempList.end(); ++i)
                    {
                        Unit* l_Owner = (*i)->GetOwner();
                        if (l_Owner && l_Owner == l_Caster && (*i)->isSummon())
                            continue;

                        l_FrozenRinglist.remove((*i));
                    }

                    // Check all the Frozen Ring of player in case of more than one
                    for (std::list<Creature*>::iterator itr = l_FrozenRinglist.begin(); itr != l_FrozenRinglist.end(); ++itr)
                    {
                        std::list<Creature*> l_TempListCreature;
                        std::list<Player*> l_TempListPlayer;

                        /// Save ring position, to add some additional range checks
                        Position l_Position;
                        (*itr)->GetPosition(&l_Position);

                        // Apply aura on hostile creatures in the grid
                        (*itr)->GetCreatureListInGrid(l_TempListCreature, -2.0f);
                        for (std::list<Creature*>::iterator i = l_TempListCreature.begin(); i != l_TempListCreature.end(); ++i)
                        {
                            if (!(*i)->IsWithinDist3d(&l_Position, 1.0f) && (*i)->IsHostileTo(l_Caster) && !(*i)->HasAura(SPELL_MAGE_RING_OF_FROST_AURA) && !(*i)->HasAura(SPELL_MAGE_RING_OF_FROST_IMMUNATE) && l_Caster->IsValidAttackTarget(*i))
                                l_Caster->CastSpell((*i), SPELL_MAGE_RING_OF_FROST_AURA, true);
                        }

                        // Apply aura on hostile players in the grid
                        (*itr)->GetPlayerListInGrid(l_TempListPlayer, -2.0f);
                        for (std::list<Player*>::iterator i = l_TempListPlayer.begin(); i != l_TempListPlayer.end(); ++i)
                        {
                            if (!(*i)->IsWithinDist3d(&l_Position, 1.0f) && (*i)->IsHostileTo(l_Caster) && !(*i)->HasAura(SPELL_MAGE_RING_OF_FROST_AURA) && !(*i)->HasAura(SPELL_MAGE_RING_OF_FROST_IMMUNATE) && l_Caster->IsValidAttackTarget(*i))
                                l_Caster->CastSpell((*i), SPELL_MAGE_RING_OF_FROST_AURA, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ring_of_frost_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_ring_of_frost_AuraScript();
        }
};

// Call by Ring of Frost (Aura) - 82691
// Ring of Frost (immunity 2.5s) - 91264
class spell_mage_ring_of_frost_immunity : public SpellScriptLoader
{
    public:
        spell_mage_ring_of_frost_immunity() : SpellScriptLoader("spell_mage_ring_of_frost_immunity") { }

        class spell_mage_ring_of_frost_immunity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_ring_of_frost_immunity_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, SPELL_MAGE_RING_OF_FROST_IMMUNATE, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_ring_of_frost_immunity_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_ring_of_frost_immunity_AuraScript();
        }
};

// Scorch - 2948
class spell_mage_scorch: public SpellScriptLoader
{
    public:
        spell_mage_scorch() : SpellScriptLoader("spell_mage_scorch") { }

        class spell_mage_scorch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_scorch_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_MAGE_IMPROVED_SCORCH) && l_Caster->getLevel() >= 92)
                        l_Caster->CastSpell(l_Caster, SPELL_MAGE_IMPROVED_SCORCH_AURA, true);

                    /// Item  Mage WoD PvP Fire 2P Bonus
                    if (l_Caster->HasAura(SPELL_MAGE_WOD_PVP_FIRE_2P_BONUS))
                        l_Caster->CastSpell(l_Caster, SPELL_MAGE_WOD_PVP_FIRE_2P_BONUS_EFFECT, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_scorch_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_scorch_SpellScript();
        }
};

/// Enhanced Pyrotechnics - 157642
class spell_mage_enhanced_pyrotechnics : public SpellScriptLoader
{
    public:
        spell_mage_enhanced_pyrotechnics() : SpellScriptLoader("spell_mage_enhanced_pyrotechnics") { }

        class spell_mage_enhanced_pyrotechnics_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_enhanced_pyrotechnics_AuraScript);

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

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_MAGE_FIREBALL && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_MAGE_FROSTFIRE_BOLT)
                    return;

                if (p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
                    l_Caster->RemoveAura(SPELL_MAGE_ENHANCED_PYROTECHNICS_PROC);
                else
                    l_Caster->CastSpell(l_Caster, SPELL_MAGE_ENHANCED_PYROTECHNICS_PROC, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mage_enhanced_pyrotechnics_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_enhanced_pyrotechnics_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Mage WoD PvP Frost 2P Bonus - 180721
class spell_mage_WoDPvPFrost2PBonus : public SpellScriptLoader
{
    public:
        spell_mage_WoDPvPFrost2PBonus() : SpellScriptLoader("spell_mage_WoDPvPFrost2PBonus") { }

        class spell_mage_WoDPvPFrost2PBonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_WoDPvPFrost2PBonus_AuraScript);

            enum eSpells
            {
                PvpFrost2PBonusTrigger  = 180723,
                ConeOfCold              = 120
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
            {
                PreventDefaultAction();
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mage_WoDPvPFrost2PBonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_WoDPvPFrost2PBonus_AuraScript();
        }
};

/// Cone of Frost - 120
class spell_mage_cone_of_frost : public SpellScriptLoader
{
    public:
        spell_mage_cone_of_frost() : SpellScriptLoader("spell_mage_cone_of_frost") { }

        class spell_mage_cone_of_frost_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_cone_of_frost_SpellScript);

            enum eSpells
            {
                PvpFrost2PBonusTrigger = 180723,
                WoDPvPFrost2PBonus = 180721
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::WoDPvPFrost2PBonus))
                    l_Caster->CastSpell(l_Caster, eSpells::PvpFrost2PBonusTrigger, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_mage_cone_of_frost_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_cone_of_frost_SpellScript();
        }
};

// Call by Blast Wave 157981 - Supernova 157980 - Ice Nova 157997
class spell_mage_novas_talent : public SpellScriptLoader
{
    public:
        spell_mage_novas_talent() : SpellScriptLoader("spell_mage_novas_talent") { }

        class spell_mage_novas_talent_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_novas_talent_SpellScript);

            uint64 m_MainTarget;

            void HandleOnCast()
            {
                m_MainTarget = 0;

                if (GetExplTargetUnit() != nullptr)
                    m_MainTarget = GetExplTargetUnit()->GetGUID();
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target->GetGUID() == m_MainTarget && !l_Target->IsFriendlyTo(l_Caster))
                        {
                            SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_0].BasePoints));
                        }
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_mage_novas_talent_SpellScript::HandleOnCast);
                OnEffectHitTarget += SpellEffectFn(spell_mage_novas_talent_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_novas_talent_SpellScript();
        }
};

// Arcane Blast - 30451
class spell_mage_arcane_blast : public SpellScriptLoader
{
    public:
        spell_mage_arcane_blast() : SpellScriptLoader("spell_mage_arcane_blast") { }

        class spell_mage_arcane_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_arcane_blast_SpellScript);

            void HandleOnPrepare()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_MAGE_ENHANCED_ARCANE_BLAST) && l_Caster->getLevel() >= 92)
                    {
                        const SpellInfo *l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_ENHANCED_ARCANE_BLAST);

                        if (l_SpellInfo == nullptr)
                            return;

                        if (Aura* l_ArcaneCharge = l_Caster->GetAura(SPELL_MAGE_ARCANE_CHARGE))
                        {
                            if (AuraEffect* l_EffectCastSpeed = l_ArcaneCharge->GetEffect(4))
                                l_EffectCastSpeed->SetAmount(l_SpellInfo->Effects[EFFECT_0].BasePoints * l_ArcaneCharge->GetCharges() * -1);
                        }
                    }
                }
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_mage_arcane_blast_SpellScript::HandleOnPrepare);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_arcane_blast_SpellScript();
        }
};

/// Blink - 1953
class spell_mage_blink : public SpellScriptLoader
{
    public:
        spell_mage_blink() : SpellScriptLoader("spell_mage_blink") { }

        enum eSpells
        {
            GlyphOfRapidDisplacement = 146659
        };

        class spell_mage_blink_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_blink_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::GlyphOfRapidDisplacement) && l_Caster->HasAuraType(SPELL_AURA_MOD_STUN))
                    return SPELL_FAILED_STUNNED;

                return SPELL_CAST_OK;
            }

            void HandleImmunity(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::GlyphOfRapidDisplacement))
                    PreventHitAura();
            }

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_MAGE_IMPROVED_BLINK) && l_Caster->getLevel() >= 92)
                        l_Caster->CastSpell(l_Caster, SPELL_MAGE_IMPROVED_BLINK_PROC, true);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_mage_blink_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_mage_blink_SpellScript::HandleImmunity, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_mage_blink_SpellScript::HandleImmunity, EFFECT_2, SPELL_EFFECT_APPLY_AURA);
                AfterHit += SpellHitFn(spell_mage_blink_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_blink_SpellScript();
        }
};

/// Blizzard - 42208
class spell_mage_blizzard : public SpellScriptLoader
{
    public:
        spell_mage_blizzard() : SpellScriptLoader("spell_mage_blizzard") { }

        class spell_mage_blizzard_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_blizzard_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->GetGUID() == l_Caster->GetGUID())
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                /// Slowing enemies by 50%
                l_Caster->CastSpell(l_Target, SPELL_MAGE_CHILLED, true);

                /// Improved Blizzard
                if (l_Player->HasSpellCooldown(SPELL_MAGE_FORZEN_ORB))
                    l_Player->ReduceSpellCooldown(SPELL_MAGE_FORZEN_ORB, 500);
            }

            void HandleAfterHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_IMPROVED_BLIZZARD);

                if (l_Player == nullptr || l_SpellInfo == nullptr)
                    return;

                if (l_Player->HasAura(SPELL_MAGE_IMPROVED_BLIZZARD) && l_Player->getLevel() >= 92 && l_SpellInfo->Effects[EFFECT_0].BasePoints > 0)
                {
                    if (l_Player->HasSpellCooldown(SPELL_MAGE_FORZEN_ORB))
                        l_Player->ReduceSpellCooldown(SPELL_MAGE_FORZEN_ORB, l_SpellInfo->Effects[EFFECT_0].BasePoints * 10);   ///< BasePoint is 50, should reduce CD by 500ms
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mage_blizzard_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_mage_blizzard_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_blizzard_SpellScript();
        }
};

/// Incanter's Flow - 1463
class spell_mage_incanters_flow : public SpellScriptLoader
{
    public:
        spell_mage_incanters_flow() : SpellScriptLoader("spell_mage_incanters_flow") { }

        class spell_mage_incanters_flow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_incanters_flow_AuraScript);

            bool m_Up = true;
            bool m_Changed = false;

            void OnTick(AuraEffect const*)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    /// Break the cycle if caster is out of combat
                    if (!l_Caster->isInCombat())
                        return;

                    if (l_Caster->HasAura(SPELL_MAGE_INCANTERS_FLOW))
                    {
                        if (Aura* l_IncantersFlow = l_Caster->GetAura(SPELL_MAGE_INCANTERS_FLOW))
                        {
                            m_Changed = false;

                            if (l_IncantersFlow->GetStackAmount() == 5 && m_Up)
                            {
                                m_Up = false;
                                m_Changed = true;
                            }
                            else if (l_IncantersFlow->GetStackAmount() == 1 && !m_Up)
                            {
                                m_Up = true;
                                m_Changed = true;
                            }

                            if (!m_Changed)
                                l_IncantersFlow->ModStackAmount(m_Up ? 1 : -1);
                        }
                    }
                    else if (l_Caster->isInCombat())
                    {
                        l_Caster->CastSpell(l_Caster, SPELL_MAGE_INCANTERS_FLOW, true);
                        m_Up = true;
                        m_Changed = false;
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_incanters_flow_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_incanters_flow_AuraScript();
        }
};

/// Glyph of the Unbound Elemental - 146976
class spell_mage_glyph_of_the_unbound_elemental : public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_the_unbound_elemental() : SpellScriptLoader("spell_mage_glyph_of_the_unbound_elemental") { }

        class spell_mage_glyph_of_the_unbound_elemental_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_glyph_of_the_unbound_elemental_AuraScript);

            enum eUnboundWaterElemental
            {
                UnboundWaterElementalTransform = 147358
            };

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    Pet* l_Pet = l_Player->GetPet();

                    if (l_Pet == nullptr)
                        return;

                    l_Pet->CastSpell(l_Pet, eUnboundWaterElemental::UnboundWaterElementalTransform, true);
                }
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    Pet* l_Pet = l_Player->GetPet();

                    if (l_Pet == nullptr)
                        return;

                    if (l_Pet->HasAura(eUnboundWaterElemental::UnboundWaterElementalTransform))
                        l_Pet->RemoveAurasDueToSpell(eUnboundWaterElemental::UnboundWaterElementalTransform);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_glyph_of_the_unbound_elemental_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_the_unbound_elemental_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_glyph_of_the_unbound_elemental_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Arcane Power - 12042
class spell_mage_arcane_power : public SpellScriptLoader
{
    public:
        spell_mage_arcane_power() : SpellScriptLoader("spell_mage_arcane_power") { }

        class spell_mage_arcane_power_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_arcane_power_AuraScript);

            enum eSpells
            {
                WodPvpArcane4pBonusAura = 171351,
                WodPvpArcane4pBonus = 171375
            };

            void OnApply(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::WodPvpArcane4pBonusAura))
                    l_Caster->CastSpell(l_Caster, eSpells::WodPvpArcane4pBonus, true);
            }

            void OnRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::WodPvpArcane4pBonus))
                    l_Caster->RemoveAurasDueToSpell(eSpells::WodPvpArcane4pBonus);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_arcane_power_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_arcane_power_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_arcane_power_AuraScript();
        }
};

/// Flameglow - 140468
class spell_mage_flameglow : public SpellScriptLoader
{
    public:
        spell_mage_flameglow() : SpellScriptLoader("spell_mage_flameglow") { }

        class spell_mage_flameglow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_flameglow_AuraScript);

            void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                if (Unit *l_Caster = GetCaster())
                    p_Amount = (l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL) * GetSpellInfo()->Effects[EFFECT_1].BasePoints) / 100;
            }

            void AfterAbsorb(AuraEffect* p_AurEff, DamageInfo & /*p_DmgInfo*/, uint32 & p_AbsorbAmount)
            {
                if (Unit *l_Caster = GetCaster())
                if (l_Caster->HasSpell(SPELL_MAGE_FLAMEGLOW))
                    p_AurEff->SetAmount(p_AbsorbAmount + ((l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL) * GetSpellInfo()->Effects[EFFECT_1].BasePoints) / 100));
            }

            void OnAbsorb(AuraEffect* /*p_AurEff*/, DamageInfo & p_DmgInfo, uint32 & p_AbsorbAmount)
            {
                if (Unit* l_Attacker = p_DmgInfo.GetAttacker())
                    p_AbsorbAmount = std::min(p_AbsorbAmount, CalculatePct(p_DmgInfo.GetDamage(), GetSpellInfo()->Effects[EFFECT_2].BasePoints));
            }


            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_flameglow_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_flameglow_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_mage_flameglow_AuraScript::AfterAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_flameglow_AuraScript();
        }
};

/// Polymorph - 118 - last update 5.4.2 17688
class spell_mage_polymorph : public SpellScriptLoader
{
    public:
        spell_mage_polymorph() : SpellScriptLoader("spell_mage_polymorph") { }

        class spell_mage_polymorph_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_polymorph_SpellScript);

            void HandleTarget(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(56382))
                        return;

                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target->GetCreatureType() == CreatureType::CREATURE_TYPE_CRITTER)
                        {
                            PreventHitAura();

                            l_Caster->CastSpell(l_Target, SPELL_MAGE_POLYMORPH_CRITTERMORPH, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_polymorph_SpellScript::HandleTarget, SpellEffIndex::EFFECT_1, Targets::TARGET_UNIT_TARGET_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_polymorph_SpellScript();
        }
};

class PlayerScript_rapid_teleportation : public PlayerScript
{
    public:
        PlayerScript_rapid_teleportation() :PlayerScript("PlayerScript_rapid_teleportation") {}

        void OnTeleport(Player* p_Player, const SpellInfo * p_SpellInfo)
        {
            if (p_Player->getClass() == CLASS_MAGE &&  p_SpellInfo->SpellFamilyName == SPELLFAMILY_MAGE && p_Player->HasAura(SPELL_MAGE_RAPID_TELEPORTATION_AURA))
            {
                p_Player->CastSpell(p_Player, SPELL_MAGE_RAPID_TELEPORTATION, true);
            }
        }
};

/// Ring of Frost (Freeze) - 82691 - last update: 6.1.2 19865
class spell_ring_of_frost_freeze : public SpellScriptLoader
{
    public:
        spell_ring_of_frost_freeze() : SpellScriptLoader("spell_ring_of_frost_freeze") { }

        enum Spells
        {
            RingOfFrostImmune = 91264
        };

        class spell_ring_of_frost_freeze_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ring_of_frost_freeze_AuraScript);

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_DEATH)
                    return;

                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(GetTarget(), Spells::RingOfFrostImmune, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_ring_of_frost_freeze_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ring_of_frost_freeze_AuraScript();
        }
};

/// Ring of Frost - 113724, Ring of Frost - 140376 (override)
class spell_mage_ring_of_frost_trigger : public SpellScriptLoader
{
    public:
        spell_mage_ring_of_frost_trigger() : SpellScriptLoader("spell_mage_ring_of_frost_trigger") { }

        class spell_mage_ring_of_frost_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_ring_of_frost_trigger_SpellScript);

            enum eSpells
            {
                PresenceOfMind = 12043
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::PresenceOfMind))
                    l_Caster->RemoveAura(eSpells::PresenceOfMind);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_mage_ring_of_frost_trigger_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_ring_of_frost_trigger_SpellScript();
        }
};

/// Item - Mage T17 Fire 4P Bonus - 165459
class spell_mage_item_t17_fire_4p_bonus : public SpellScriptLoader
{
    public:
        spell_mage_item_t17_fire_4p_bonus() : SpellScriptLoader("spell_mage_item_t17_fire_4p_bonus") { }

        class spell_mage_item_t17_fire_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_item_t17_fire_4p_bonus_AuraScript);

            enum eSpells
            {
                PyroblastInstantProc    = 48108,
                Pyromaniac              = 166868
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell || l_ProcSpell->Id != eSpells::PyroblastInstantProc)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::Pyromaniac, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_mage_item_t17_fire_4p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_item_t17_fire_4p_bonus_AuraScript();
        }
};

/// Item - Mage T17 Arcane 4P Bonus - 165476
class spell_mage_item_t17_arcane_4p_bonus : public SpellScriptLoader
{
    public:
        spell_mage_item_t17_arcane_4p_bonus() : SpellScriptLoader("spell_mage_item_t17_arcane_4p_bonus") { }

        class spell_mage_item_t17_arcane_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_item_t17_arcane_4p_bonus_AuraScript);

            enum eSpells
            {
                ArcaneCharge        = 36032,
                ArcaneInstability   = 166872
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell || l_ProcSpell->Id != eSpells::ArcaneCharge)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::ArcaneInstability, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_mage_item_t17_arcane_4p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mage_item_t17_arcane_4p_bonus_AuraScript();
        }
};

/// last update : 6.2.3
/// Glyph of Illusion - 63092
class spell_mage_glyph_of_illusion : public SpellScriptLoader
{
    public:
        spell_mage_glyph_of_illusion() : SpellScriptLoader("spell_mage_glyph_of_illusion") { }

        class spell_mage_glyph_of_illusion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_glyph_of_illusion_AuraScript);

            enum eSpells
            {
                Illusion = 131784
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->HasSpell(eSpells::Illusion))
                    l_Player->learnSpell(eSpells::Illusion, false);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasSpell(eSpells::Illusion))
                    l_Player->removeSpell(eSpells::Illusion, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mage_glyph_of_illusion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_illusion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_glyph_of_illusion_AuraScript();
        }
};

/// last update : 6.2.3
/// Illusion - 131784
class spell_mage_illusion : public SpellScriptLoader
{
    public:
        spell_mage_illusion() : SpellScriptLoader("spell_mage_illusion") { }

        class spell_mage_illusion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_illusion_SpellScript);

            enum eSpells
            {
                IllusionEffect = 80396,
                IllusionBasic = 94632
            };

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                if (Unit* l_Target = l_Player->GetSelectedPlayer())
                    l_Target->CastSpell(l_Player, eSpells::IllusionEffect, true);

                else
                    l_Player->CastSpell(l_Player, eSpells::IllusionBasic);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_mage_illusion_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_illusion_SpellScript();
        }
};

/// Last Update 6.2.3
/// Arcane Missiles! - 79683
class spell_mage_arcane_missiles_visual : public SpellScriptLoader
{
    public:
        spell_mage_arcane_missiles_visual() : SpellScriptLoader("spell_mage_arcane_missiles_visual") { }

        class spell_mage_arcane_missiles_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mage_arcane_missiles_visual_AuraScript);

            enum eSpells
            {
                ArcaneMissilesVisualUI = 79808
            };

            void OnRemove(AuraEffect const* /*p_Aura*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->RemoveAura(eSpells::ArcaneMissilesVisualUI);
            }

            void OnUpdate(uint32 /*p_Diff*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (GetCharges() > 0 || GetStackAmount() > 0)
                    l_Caster->CastSpell(l_Caster, eSpells::ArcaneMissilesVisualUI, true);
            }

            void Register()
            {
                OnAuraUpdate += AuraUpdateFn(spell_mage_arcane_missiles_visual_AuraScript::OnUpdate);
                AfterEffectRemove += AuraEffectRemoveFn(spell_mage_arcane_missiles_visual_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mage_arcane_missiles_visual_AuraScript();
        }
};

/// Last Update 6.2.3
/// Call by Ice Lance - 30455, Deep Freeze - 44572
/// Fingers of Frost - 44544
class spell_mage_finger_of_frost : public SpellScriptLoader
{
    public:
        spell_mage_finger_of_frost() : SpellScriptLoader("spell_mage_finger_of_frost") { }

        class spell_mage_finger_of_frost_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_finger_of_frost_SpellScript);

            bool m_AlreadyDrop = false;

            enum eSpells
            {
                FingerOfFrost           = 44544,
                FingerOfFrostVisualUi   = 126084
            };

            void HandleAfterHit()
            {
                if (m_AlreadyDrop)
                    return;

                Unit* l_Caster = GetCaster();

                m_AlreadyDrop = true; ///< With Glyph of Splitting Ice, Ice Lance hit too times, with should not consume 2 stack of Finger of Frost
                if (Aura* l_FingersOfFrost = l_Caster->GetAura(eSpells::FingerOfFrost, l_Caster->GetGUID()))
                    l_FingersOfFrost->ModStackAmount(-1);
                if (Aura* l_FingersVisual = l_Caster->GetAura(eSpells::FingerOfFrostVisualUi, l_Caster->GetGUID()))
                    l_FingersVisual->Remove();
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mage_finger_of_frost_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_finger_of_frost_SpellScript();
        }
};

/// Glyph of Arcane Language - 57925
/// Called by Arcane Brilliance: 1459
class spell_mage_glyph_of_arcane_language : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_arcane_language() : SpellScriptLoader("spell_mage_glyph_of_arcane_language") { }

    class spell_mage_glyph_of_arcane_language_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_glyph_of_arcane_language_AuraScript);

        enum eSpells
        {
            SpellMageGlyphOfArcaneLanguage = 57925,
            ArcaneLanguageAlliance = 122998,
            ArcaneLanguageHorde = 122999,
        };

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (GetCaster() == nullptr)
                return;

            Player* l_Player = GetCaster()->ToPlayer();

            if (l_Player == nullptr)
                return;

            if (l_Player->HasAura(eSpells::SpellMageGlyphOfArcaneLanguage))
            {
                if (l_Player->GetTeamId() == TEAM_ALLIANCE)
                    l_Player->CastSpell(l_Player, eSpells::ArcaneLanguageAlliance, true);
                else if (l_Player->GetTeamId() == TEAM_HORDE)
                    l_Player->CastSpell(l_Player, eSpells::ArcaneLanguageHorde, true);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_mage_glyph_of_arcane_language_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SPELL_POWER_PCT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_mage_glyph_of_arcane_language_AuraScript();
    }
};

/// Last update 6.2.3
/// Glyph of Conjure Familiar - 126748
class spell_mage_conjure_familiar_glyph : public SpellScriptLoader
{
public:
    spell_mage_conjure_familiar_glyph() : SpellScriptLoader("spell_mage_conjure_familiar_glyph") { }

    class spell_mage_conjure_familiar_glyph_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_conjure_familiar_glyph_AuraScript);

        enum eSpells
        {
            GlyphOfConjureFamiliar = 126748,
            ConjureFamiliar = 126578
        };

        void AfterApply(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->learnSpell(ConjureFamiliar, true);
        }

        void AfterRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->removeSpell(ConjureFamiliar);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_conjure_familiar_glyph_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectApplyFn(spell_mage_conjure_familiar_glyph_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_mage_conjure_familiar_glyph_AuraScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_mage_spell_scripts()
{
    /// AreaTriggers
    new spell_areatrigger_mage_wod_frost_2p_bonus();

    /// Spells
    new spell_mage_conjure_familiar_glyph();
    new spell_mage_ice_block();
    new spell_mage_finger_of_frost();
    new spell_mage_arcane_missiles_visual();
    new spell_mage_illusion();
    new spell_mage_glyph_of_illusion();
    new spell_mage_ring_of_frost_trigger();
    new spell_mage_arcane_charge();
    new spell_mage_meteor();
    new spell_mage_comet_storm();
    new spell_mage_ring_of_frost_immunity();
    new spell_mage_flameglow();
    new spell_mage_incanters_flow();
    new spell_mage_blizzard();
    new spell_mage_blink();
    new spell_mage_arcane_blast();
    new spell_mage_novas_talent();
    new spell_mage_enhanced_pyrotechnics();
    new spell_mage_scorch();
    new spell_mage_ring_of_frost();
    new spell_mage_kindling();
    new spell_mage_frostfire_bolt();
    new spell_mage_pyroblast();
    new spell_mage_ice_lance();
    new spell_mage_unstable_magic();
    new spell_mage_greater_invisibility_removed();
    new spell_mage_greater_invisibility_triggered();
    new spell_mage_glyph_of_slow();
    new spell_mage_pet_frost_nova();
    new spell_mage_glyph_of_ice_block();
    new spell_mage_arcane_missile();
    new spell_mage_cauterize();
    new spell_mage_arcane_barrage();
    new spell_mage_arcane_explosion();
    new spell_mage_frostbolt();
    new spell_mage_nether_tempest_damage();
    new spell_mage_nether_tempest();
    new spell_mage_blazing_speed();
    new spell_mage_frostjaw();
    new spell_mage_combustion();
    new spell_mage_inferno_blast();
    new spell_mage_evocation();
    new spell_mage_conjure_refreshment();
    new spell_mage_time_warp();
    new spell_mage_alter_time_overrided();
    new spell_mage_alter_time();
    new spell_mage_cold_snap();
    new spell_mage_living_bomb();
    new spell_mage_mirror_image_summon();
    new spell_mage_ice_barrier();
    new spell_mage_prysmatic_crystal_damage();
    new spell_mage_glyph_of_the_unbound_elemental();
    new spell_mage_WoDPvPFrost2PBonus();
    new spell_mage_arcane_power();
    new spell_mage_polymorph();
    new spell_ring_of_frost_freeze();
    new spell_mage_cone_of_frost();
    new spell_mage_item_t17_fire_4p_bonus();
    new spell_mage_item_t17_arcane_4p_bonus();
    new spell_mage_glyph_of_arcane_language();

    /// Player Script
    new PlayerScript_rapid_teleportation();
}
#endif
