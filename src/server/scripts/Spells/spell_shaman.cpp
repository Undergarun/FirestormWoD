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
 * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_sha_".
 */

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "GridNotifiers.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum ShamanSpells
{
    SHAMAN_SPELL_SATED                          = 57724,
    SHAMAN_SPELL_EXHAUSTION                     = 57723,
    HUNTER_SPELL_INSANITY                       = 95809,
    HUNTER_SPELL_FATIGUED                       = 160455,
    MAGE_SPELL_TEMPORAL_DISPLACEMENT            = 80354,
    SPELL_SHA_LIGHTNING_SHIELD_AURA             = 324,
    SPELL_SHA_ELEMENTAL_BLAST                   = 117014,
    SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL     = 118517,
    SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL      = 118515,
    SPELL_SHA_LAVA_LASH                         = 60103,
    SPELL_SHA_FLAME_SHOCK                       = 8050,
    SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE       = 26364,
    SPELL_SHA_HEALING_STREAM                    = 52042,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM           = 119523,
    SPELL_SHA_FULMINATION                       = 88766,
    SPELL_SHA_FULMINATION_TRIGGERED             = 88767,
    SPELL_SHA_FULMINATION_INFO                  = 95774,
    SPELL_SHA_ROLLING_THUNDER_AURA              = 88764,
    SPELL_SHA_ROLLING_THUNDER_ENERGIZE          = 88765,
    SPELL_SHA_UNLEASH_ELEMENTS                  = 73680,
    SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE        = 77661,
    SPELL_SHA_FIRE_NOVA                         = 1535,
    SPELL_SHA_FIRE_NOVA_TRIGGERED               = 8349,
    SPELL_SHA_TIDAL_WAVES                       = 51564,
    SPELL_SHA_TIDAL_WAVES_PROC                  = 53390,
    SPELL_SHA_MANA_TIDE                         = 16191,
    SPELL_SHA_FROST_SHOCK_FREEZE                = 63685,
    SPELL_SHA_FROZEN_POWER                      = 63374,
    SPELL_SHA_MAIL_SPECIALIZATION_AGI           = 86099,
    SPELL_SHA_MAIL_SPECIALISATION_INT           = 86100,
    SPELL_SHA_UNLEASHED_FURY_TALENT             = 117012,
    SPELL_SHA_UNLEASHED_FURY_FLAMETONGUE        = 118470,
    SPELL_SHA_UNLEASHED_FURY_WINDFURY           = 118472,
    SPELL_SHA_UNLEASHED_FURY_EARTHLIVING        = 118473,
    SPELL_SHA_STONE_BULWARK_ABSORB              = 114893,
    SPELL_SHA_EARTHGRAB_IMMUNITY                = 116946,
    SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM     = 116947,
    SPELL_SHA_EARTHGRAB                         = 64695,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS              = 108283,
    SPELL_SHA_ANCESTRAL_GUIDANCE                = 114911,
    SPELL_SHA_CONDUCTIVITY_TALENT               = 108282,
    SPELL_SHA_CONDUCTIVITY_HEAL                 = 118800,
    SPELL_SHA_GLYPH_OF_LAKESTRIDER              = 55448,
    SPELL_SHA_WATER_WALKING                     = 546,
    SPELL_SHA_SOLAR_BEAM                        = 113286,
    SPELL_SHA_SOLAR_BEAM_SILENCE                = 113288,
    SPELL_SHA_GHOST_WOLF                        = 2645,
    SPELL_SHA_ITEM_T14_4P                       = 123124,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM_TOTEM     = 55456,
    SPELL_SHA_ITEM_S12_4P_ENHANCEMENT_BONUS     = 131554,
    SPELL_SHA_RESTORATIVE_MISTS                 = 114083,
    SPELL_SHA_FROST_SHOCK                       = 8056,
    SPELL_SHA_LAVA_SURGE_AURA                   = 77756,
    SPELL_SHA_LAVA_BURST                        = 51505,
    SPELL_SHA_WINDFURY_ATTACK                   = 25504,
    SPELL_SHA_LAMETONGUE_ATTACK                 = 10444,
    SPELL_SHA_PVP_BONUS_WOD_2                   = 166103,
    SPELL_SHA_PVP_BONUS_WOD_4                   = 171121,
    SPELL_SHA_LIGHTNING_SHIELD                  = 324,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS_ELEMENTAL    = 159101,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS_ENHANCEMENT  = 159103,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS_RESTORATION  = 159105,
    SPELL_SHA_LAVA_LASH_SPREAD                  = 105792,
    SPELL_SHA_LIQUID_MAGMA_DAMAGE               = 177601,
    SPELL_SHA_GLYPH_OF_GHOSTLY_SPEED            = 159642,
    SPELL_SHA_ELEMENTAL_BLAST_CRIT_BONUS        = 118522,
    SPELL_SHA_ELEMENTAL_BLAST_HASTE_BONUS       = 173183,
    SPELL_SHA_ELEMENTAL_BLAST_MASTERY_BONUS     = 173184,
    SPELL_SHA_ELEMENTAL_BLAST_MULTISTRIKE_BONUS = 173185,
    SPELL_SHA_ELEMENTAL_BLAST_AGILITY_BONUS     = 173186,
    SPELL_SHA_ELEMENTAL_BLAST_SPIRIT_BONUS      = 173187,
    SPELL_SHA_ELEMENTAL_FUSION                  = 152257,
    SPELL_SHA_ELEMENTAL_FUSION_PROC             = 157174,
    SPELL_SHA_IMPROVED_LIGHTNING_SHIELD         = 157774,
    SPELL_SHA_UNLEASH_FLAME_AURA_ENCHANCEMENT   = 73683,
    SPELL_SHA_UNLEASH_FLAME_AURA_ELEMENTAL      = 165462,
    SPELL_SHA_GLYPH_OF_LAVA_LASH                = 55444
};

/// Called by Unleash Flame - 165462, Unleash Life - 73685 and Unleash Elements - 73680
/// Unleashed Fury - 165477
class spell_sha_unleashed_fury : public SpellScriptLoader
{
    public:
        spell_sha_unleashed_fury() : SpellScriptLoader("spell_sha_unleashed_fury") { }

        class spell_sha_unleashed_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_unleashed_fury_SpellScript);

            enum eSpells
            {
                UnleashedFuryTalentElemental    = 165477,
                UnleashedFuryTalentEnhancement  = 117012,
                UnleashedFuryTalentRestauration = 165479,
                /// Increases the damage dealt by your Lightning Bolt by 30% and Lava Burst by 10% for 10 sec.
                UnleashedFuryElemental          = 118470,
                /// Increases the damage dealt by your Lightning Bolt by 30% and increases your multistrike chance by 5% for 8 sec.
                UnleashedFuryEnhancement        = 118472,
                /// Increases the healing done by your next single-target heal by 50%, for 10 sec.
                UnleashedRestauration           = 118473,

                UnleashElements                 = 73680,
                UnleashLife                     = 73685,
                UnleashFlame                    = 165462
            };

            void HandleDummy(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::UnleashedFuryTalentElemental) && !l_Caster->HasAura(eSpells::UnleashedFuryTalentEnhancement) && !l_Caster->HasAura(eSpells::UnleashedFuryTalentRestauration))
                        return;

                    switch (GetSpellInfo()->Id)
                    {
                        case eSpells::UnleashElements:
                            l_Caster->CastSpell(l_Caster, eSpells::UnleashedFuryEnhancement, true);
                            break;
                        case eSpells::UnleashLife:
                            l_Caster->CastSpell(l_Caster, eSpells::UnleashedRestauration, true);
                            break;
                        case eSpells::UnleashFlame:
                            l_Caster->CastSpell(l_Caster, eSpells::UnleashedFuryElemental, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::UnleashElements:
                    case eSpells::UnleashFlame:
                        OnEffectHitTarget += SpellEffectFn(spell_sha_unleashed_fury_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                        break;
                    case eSpells::UnleashLife:
                        OnEffectHitTarget += SpellEffectFn(spell_sha_unleashed_fury_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_HEAL);
                        break;
                    default:
                        break;
                }
            }
        };

        class spell_sha_unleashed_fury_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_unleashed_fury_AuraScript);

            enum eSpells
            {
                UnleashLife = 73685,
                UnleashedFuryTalentRestauration = 165479,
                UnleashedRestauration = 118473
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::UnleashedRestauration);

                if (l_Caster->HasAura(eSpells::UnleashedFuryTalentRestauration))
                    p_Amount += l_SpellInfo->Effects[EFFECT_0].BasePoints;
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::UnleashedRestauration))
                    l_Caster->RemoveAura(eSpells::UnleashedRestauration);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::UnleashLife:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_unleashed_fury_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_unleashed_fury_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_DUMMY);
                        OnEffectRemove += AuraEffectRemoveFn(spell_sha_unleashed_fury_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                    break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_unleashed_fury_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_unleashed_fury_SpellScript();
        }
};

/// last update: 6.1.2 19865
/// Called by Chain Heal - 1064 and Chain Heal (T17 Proc) - 177972
/// High Tide - 157154
class spell_sha_high_tide : public SpellScriptLoader
{
    public:
        spell_sha_high_tide() : SpellScriptLoader("spell_sha_high_tide") { }

        class spell_sha_high_tide_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_high_tide_SpellScript);

            enum eSpells
            {
                SpellHighTide   = 157154,
                SpellRiptide    = 61295
            };

            uint64 m_MainTargetGUID = 0;

            void HandleBeforeCast()
            {
                if (Unit* l_MainTarget = GetExplTargetUnit())
                    m_MainTargetGUID = l_MainTarget->GetGUID();
            }

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::SpellHighTide))
                        return;

                    std::map<uint64, WorldObject*> l_TargetMap;
                    for (WorldObject* l_Object : p_Targets)
                        l_TargetMap.insert(std::make_pair(l_Object->GetGUID(), l_Object));

                    if (!m_MainTargetGUID)
                        return;

                    Unit* l_FirstTarget = ObjectAccessor::FindUnit(m_MainTargetGUID);

                    if (l_FirstTarget == nullptr)
                        return;

                    std::list<Unit*> l_TempList;
                    JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_FirstTarget, l_Caster, GetSpellInfo()->RangeEntry->maxRangeFriend);
                    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_FirstTarget, l_TempList, l_Check);
                    l_Caster->VisitNearbyObject(GetSpellInfo()->RangeEntry->maxRangeFriend, l_Searcher);
                    
                    if (l_TempList.empty())
                        return;

                    l_TempList.remove_if([this, l_TargetMap, l_Caster](Unit* p_Unit) -> bool
                    {
                        if (p_Unit == nullptr || !p_Unit->HasAura(eSpells::SpellRiptide) || p_Unit == l_Caster)
                            return true;

                        /// Already in list
                        if (l_TargetMap.find(p_Unit->GetGUID()) != l_TargetMap.end())
                            return true;

                        return false;
                    });

                    if (l_TempList.empty())
                        return;

                    l_TempList.sort(JadeCore::HealthPctOrderPred());
                    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::SpellHighTide);

                    if (l_SpellInfo == nullptr)
                        return;

                    uint8 l_TargetCount = l_SpellInfo->Effects[EFFECT_1].BasePoints;

                    for (Unit* l_Unit : l_TempList)
                    {
                        if (!l_TargetCount)
                            break;

                        p_Targets.push_back(l_Unit);
                        --l_TargetCount;
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_sha_high_tide_SpellScript::HandleBeforeCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_high_tide_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_CHAINHEAL_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_high_tide_SpellScript();
        }
};

/// Totemic Projection - 108287
class spell_sha_totemic_projection: public SpellScriptLoader
{
    public:
        spell_sha_totemic_projection() : SpellScriptLoader("spell_sha_totemic_projection") { }

        class spell_sha_totemic_projection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_totemic_projection_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Creature*> leapList;
                    Unit* leapTarget = NULL;

                    caster->GetCreatureListWithEntryInGrid(leapList, 47319, 45.0f);

                    for (auto itr : leapList)
                    {
                        if (!itr->ToUnit() || !itr->ToTempSummon())
                            continue;

                        if (!itr->ToTempSummon()->GetSummoner())
                            continue;

                        if (itr->ToTempSummon()->GetSummoner()->GetGUID() != caster->GetGUID())
                            continue;

                        leapTarget = itr->ToUnit();
                        break;
                    }

                    if (!leapTarget)
                        return;

                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->isTotem())
                        {
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo((*itr)->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL));
                            if (!spellInfo)
                                continue;

                            float objSize = caster->GetObjectSize();
                            float angle = spellInfo->Effects[0].TargetA.CalcDirectionAngle();
                            float dist = spellInfo->Effects[0].CalcRadius(caster);

                            if (dist < objSize)
                                dist = objSize;

                            Position pos;
                            leapTarget->GetPosition(&pos);

                            angle += (*itr)->GetOrientation();
                            float destx, desty, destz, ground, floor;
                            destx = pos.m_positionX + dist * std::cos(angle);
                            desty = pos.m_positionY + dist * std::sin(angle);
                            ground = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
                            floor = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, pos.m_positionZ, true);
                            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

                            (*itr)->NearTeleportTo(destx, desty, destz, (*itr)->GetOrientation());
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_totemic_projection_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_totemic_projection_SpellScript();
        }
};

/// Last Build 6.2.3
/// Call by Ascendance - 114050, Ascendance - 114051, Ascendance - 114052
/// Glyph of Ascendance - 186198
class spell_sha_glyph_of_ascendance : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_ascendance() : SpellScriptLoader("spell_sha_glyph_of_ascendance") { }

        class spell_sha_glyph_of_ascendance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_glyph_of_ascendance_AuraScript);

            enum eSpells
            {
                GlyphofAscendance               = 186198,
                GlyphofAscendanceVisualElem     = 186199,
                GlyphofAscendanceVisualResto    = 186200,
                GlyphofAscendanceVisualEnh       = 186201
            };

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->HasAura(eSpells::GlyphofAscendance))
                    return;

                l_Player->SetDisplayId(l_Player->GetNativeDisplayId());

                switch (l_Player->GetSpecializationId(l_Player->GetActiveSpec()))
                {
                case SPEC_SHAMAN_ELEMENTAL:
                    l_Player->CastSpell(l_Player, eSpells::GlyphofAscendanceVisualElem, true);
                    break;
                case SPEC_SHAMAN_ENHANCEMENT:
                    l_Player->CastSpell(l_Player, eSpells::GlyphofAscendanceVisualEnh, true);
                    break;
                case SPEC_SHAMAN_RESTORATION:
                    l_Player->CastSpell(l_Player, eSpells::GlyphofAscendanceVisualResto, true);
                    break;
                default:
                    break;
                }
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                l_Target->RemoveAura(eSpells::GlyphofAscendanceVisualElem);
                l_Target->RemoveAura(eSpells::GlyphofAscendanceVisualEnh);
                l_Target->RemoveAura(eSpells::GlyphofAscendanceVisualResto);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_sha_glyph_of_ascendance_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_sha_glyph_of_ascendance_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_glyph_of_ascendance_AuraScript();
        }
};

/// Ascendance (Flame) - 114050
/// last update: 6.1.2 19865
class spell_sha_ascendance_flame : public SpellScriptLoader
{
    public:
        spell_sha_ascendance_flame() : SpellScriptLoader("spell_sha_ascendance_flame") { }

        class spell_sha_ascendance_flame_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ascendance_flame_SpellScript);

            enum eSpells
            {
                LavaBurst                           = 51505
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;
                
                if (SpellInfo const* l_LavaBurst = sSpellMgr->GetSpellInfo(eSpells::LavaBurst))
                    l_Player->RestoreCharge(l_LavaBurst->ChargeCategoryEntry);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_ascendance_flame_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ascendance_flame_SpellScript();
        }
};

/// Ascendance (Air) - 114051
/// last update: 6.1.2 19865
class spell_sha_ascendance_air : public SpellScriptLoader
{
    public:
        spell_sha_ascendance_air() : SpellScriptLoader("spell_sha_ascendance_air") { }

        class spell_sha_ascendance_air_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ascendance_air_SpellScript);

            enum eSpells
            {
                Stormstrike = 17364
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                if (SpellInfo const* l_Stormstrike = sSpellMgr->GetSpellInfo(eSpells::Stormstrike))
                    l_Player->RestoreCharge(l_Stormstrike->ChargeCategoryEntry);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_ascendance_air_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ascendance_air_SpellScript();
        }
};

/// Ascendance (Water) - 114052
class spell_sha_ascendance_water : public SpellScriptLoader
{
    public:
        spell_sha_ascendance_water() : SpellScriptLoader("spell_sha_ascendance_water") { }

        class spell_sha_ascendance_water_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ascendance_water_AuraScript);

            enum eSpells
            {
                RestorativeMists = 114083
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                if (p_EventInfo.GetActor()->GetGUID() != l_Player->GetGUID())
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == eSpells::RestorativeMists)
                    return;

                if (!(p_EventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(p_EventInfo.GetDamageInfo()->GetSpellInfo()->IsPositive()))
                    return;

                if (Unit* l_Target = p_EventInfo.GetActionTarget())
                {
                    int32 l_Bp = p_EventInfo.GetHealInfo()->GetHeal();

                    if (l_Bp > 0)
                        l_Player->CastCustomSpell(l_Target, eSpells::RestorativeMists, &l_Bp, NULL, NULL, true); //< Restorative Mists
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_ascendance_water_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ascendance_water_AuraScript();
        }
};

/// Ascendance (Water)(heal) - 114083
class spell_sha_ascendance_water_heal : public SpellScriptLoader
{
    public:
        spell_sha_ascendance_water_heal() : SpellScriptLoader("spell_sha_ascendance_water_heal") { }

        class spell_sha_ascendance_water_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ascendance_water_heal_SpellScript);

            uint32 m_TargetSize = 0;

            void OnEffectHeal(SpellEffIndex)
            {
                SetHitHeal(GetHitHeal() / m_TargetSize);
            }

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                m_TargetSize = p_Targets.size();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_ascendance_water_heal_SpellScript::OnEffectHeal, EFFECT_0, SPELL_EFFECT_HEAL);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ascendance_water_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ascendance_water_heal_SpellScript();
        }
};

/// Called by Shamanistic Rage - 30823
/// Glyph of Shamanistic Rage - 63280
class spell_sha_glyph_of_shamanistic_rage: public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_shamanistic_rage() : SpellScriptLoader("spell_sha_glyph_of_shamanistic_rage") { }

        class spell_sha_glyph_of_shamanistic_rage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_shamanistic_rage_SpellScript);

            enum eSpells
            {
                GlyphOfShamanisticRage = 63280
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasAura(eSpells::GlyphOfShamanisticRage))
                {
                    DispelChargesList l_DispelList;
                    l_Caster->GetDispellableAuraList(l_Caster, DISPEL_MAGIC, l_DispelList);

                    for (auto itr : l_DispelList)
                    {
                        if (!itr.first->GetSpellInfo()->IsPositive() && GetSpellInfo()->CanDispelAura(itr.first->GetSpellInfo()) && GetSpellInfo()->SchoolMask & SPELL_SCHOOL_MASK_MAGIC)
                            l_Caster->RemoveAura(itr.first);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_shamanistic_rage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_shamanistic_rage_SpellScript();
        }
};

/// Called by Ghost Wolf - 2645
/// Glyph of Lakestrider - 55448
class spell_sha_glyph_of_lakestrider: public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_lakestrider() : SpellScriptLoader("spell_sha_glyph_of_lakestrider") { }

        class spell_sha_glyph_of_lakestrider_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_lakestrider_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(SPELL_SHA_GLYPH_OF_LAKESTRIDER))
                        caster->CastSpell(caster, SPELL_SHA_WATER_WALKING, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_lakestrider_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_lakestrider_SpellScript();
        }
};

uint32 g_resetTotemCdSpells[] =
{
    108269, // Capacitor Totem
    2484,   // Earthbind Totem
    8177,   // Grounding Totem
    5394,   // Healing Stream Totem
    8143,   // Tremor Totem
    108270, // Stone Bulwark Totem
    51485,  // Earthgrab Totem
    108273  // Windwalk Totem
};

/// 108285 - Call of the Elements
class spell_sha_call_of_the_elements: public SpellScriptLoader
{
    public:
        spell_sha_call_of_the_elements() : SpellScriptLoader("spell_sha_call_of_the_elements") { }

        class spell_sha_call_of_the_elements_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_call_of_the_elements_SpellScript);

            void OnSpellHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (!l_Player)
                    return;

                for (int l_I = 0; l_I < sizeof(g_resetTotemCdSpells) / sizeof(uint32); l_I++)
                    l_Player->RemoveSpellCooldown(g_resetTotemCdSpells[l_I], true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_call_of_the_elements_SpellScript::OnSpellHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_call_of_the_elements_SpellScript();
        }
};

/// Called by Healing Wave - 331, Greater Healing Wave - 77472 and Healing Surge - 8004
/// Called by Lightning Bolt - 403, Chain Lightning - 421, Earth Shock - 8042 and Stormstrike - 17364
/// Called by Lightning Bolt - 45284, Chain Lightning - 45297
/// Conductivity - 108282
class spell_sha_conductivity: public SpellScriptLoader
{
    public:
        spell_sha_conductivity() : SpellScriptLoader("spell_sha_conductivity") { }

        class spell_sha_conductivity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_conductivity_SpellScript);

            enum eSpells
            {
                HealingRainAura     = 73920,
                Stormstrike         = 17364,
                T17Enhancement2P    = 165605,
                FeralSpirits        = 51533
            };

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (Aura* l_Conductivity = l_Caster->GetAura(SPELL_SHA_CONDUCTIVITY_TALENT))
                {
                    if (Aura* l_Aura = l_Caster->GetAura(eSpells::HealingRainAura))
                    {
                        int32 l_RemainingDuration = l_Conductivity->GetEffect(EFFECT_0)->GetAmount() * 10;
                        int32 l_AddDuration = std::min(l_RemainingDuration, 4000);

                        if (!l_AddDuration)
                            return;

                        l_Aura->SetDuration(l_Aura->GetDuration() + l_AddDuration);
                        l_Aura->SetMaxDuration(l_Aura->GetMaxDuration() + l_AddDuration);
                        l_Conductivity->GetEffect(EFFECT_0)->SetAmount(std::max(0, l_Conductivity->GetEffect(EFFECT_0)->GetAmount() - (l_AddDuration / 10)));
                    }
                }

                /// Stormstrike reduces the cooldown of Feral Spirits by 5 sec.
                if (GetSpellInfo()->Id == eSpells::Stormstrike && l_Caster->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::T17Enhancement2P, EFFECT_0))
                        l_Caster->ToPlayer()->ReduceSpellCooldown(eSpells::FeralSpirits, l_AuraEffect->GetAmount());
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_conductivity_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_conductivity_SpellScript();
        }
};

/// Ancestral Guidance - 108281
class spell_sha_ancestral_guidance: public SpellScriptLoader
{
    public:
        spell_sha_ancestral_guidance() : SpellScriptLoader("spell_sha_ancestral_guidance") { }

        class spell_sha_ancestral_guidance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ancestral_guidance_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                // Prevent inifinite loop
                if (eventInfo.GetDamageInfo()->GetSpellInfo() &&
                    (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == GetSpellInfo()->Id ||
                    eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_RESTORATIVE_MISTS ||
                    eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_ANCESTRAL_GUIDANCE))
                    return;

                Player* l_Player = GetCaster()->ToPlayer();
                if (l_Player == nullptr)
                    return;

                if (Unit* l_Target = eventInfo.GetActionTarget())
                {
                    int32 l_Bp = eventInfo.GetDamageInfo()->GetDamage() > eventInfo.GetHealInfo()->GetHeal() ? eventInfo.GetDamageInfo()->GetDamage() : eventInfo.GetHealInfo()->GetHeal();
                    if (!l_Bp)
                        return;

                    l_Bp = int32(l_Bp * aurEff->GetAmount() / 100);

                    l_Player->CastCustomSpell(l_Target, SPELL_SHA_ANCESTRAL_GUIDANCE, &l_Bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_guidance_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ancestral_guidance_AuraScript();
        }
};

/// last update : 6.1.2
/// Ancestral Guidance (heal) - 114911
class spell_sha_ancestral_guidance_heal : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_guidance_heal() : SpellScriptLoader("spell_sha_ancestral_guidance_heal") { }

        class spell_sha_ancestral_guidance_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ancestral_guidance_heal_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.size() > 3)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(3);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ancestral_guidance_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ancestral_guidance_heal_SpellScript();
        }
};

/// 64695 - Earthgrab
class spell_sha_earthgrab: public SpellScriptLoader
{
    public:
        spell_sha_earthgrab() : SpellScriptLoader("spell_sha_earthgrab") { }

        class spell_sha_earthgrab_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthgrab_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                Unit* target = GetHitUnit();

                if (target->HasAura(SPELL_SHA_EARTHGRAB_IMMUNITY))
                {
                    GetCaster()->CastSpell(target, SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM, true);
                    PreventHitAura();
                    return;
                }

                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, SPELL_SHA_EARTHGRAB_IMMUNITY, true);
            }

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                for (std::list<WorldObject*>::iterator iter = unitList.begin(); iter != unitList.end();)
                {
                    if (Unit* target = (*iter)->ToUnit())
                    {
                        if (target->HasAura(SPELL_SHA_EARTHGRAB))
                            iter = unitList.erase(iter);
                        else
                            iter++;
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_earthgrab_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_earthgrab_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthgrab_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Stone Bulwark Totem - 114889
class spell_sha_stone_bulwark: public SpellScriptLoader
{
    public:
        spell_sha_stone_bulwark() : SpellScriptLoader("spell_sha_stone_bulwark") { }

        class spell_sha_stone_bulwark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_stone_bulwark_AuraScript);

            void OnTick(AuraEffect const* p_AurEff)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                Unit* l_Owner = l_Caster->GetOwner();

                if (l_Caster == nullptr || l_Owner == nullptr)
                    return;

                Player* l_Player = l_Owner->ToPlayer();

                if (l_Player == nullptr)
                    return;

                float spellPower = spellPower = l_Player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL);
                int32 l_Amount = 0.875f * spellPower;

                if (Aura* aura = l_Player->GetAura(SPELL_SHA_STONE_BULWARK_ABSORB))
                {
                    aura->GetEffect(EFFECT_0)->SetAmount(aura->GetEffect(EFFECT_0)->GetAmount() + l_Amount);
                    aura->RefreshDuration();
                }
                else if (p_AurEff->GetTickNumber() == 1)
                {
                    l_Amount *= 4.0f;
                    if (Aura* aura = l_Caster->AddAura(SPELL_SHA_STONE_BULWARK_ABSORB, l_Player))
                        aura->GetEffect(EFFECT_0)->SetAmount(l_Amount);
                }
                else
                {
                    if (Aura* aura = l_Caster->AddAura(SPELL_SHA_STONE_BULWARK_ABSORB, l_Player))
                        aura->GetEffect(EFFECT_0)->SetAmount(l_Amount);
                }

            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_stone_bulwark_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_stone_bulwark_AuraScript();
        }
};

/// Frost Shock - 8056
class spell_sha_frozen_power: public SpellScriptLoader
{
    public:
        spell_sha_frozen_power() : SpellScriptLoader("spell_sha_frozen_power") { }

        class spell_sha_frozen_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_frozen_power_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_FROST_SHOCK))
                    return false;
                return true;
            }

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        if (caster->HasAura(SPELL_SHA_FROZEN_POWER))
                            caster->CastSpell(target, SPELL_SHA_FROST_SHOCK_FREEZE, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_frozen_power_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_frozen_power_SpellScript();
        }
};

/// Spirit Link - 98020 : triggered by 98017
/// Spirit Link Totem
class spell_sha_spirit_link: public SpellScriptLoader
{
    public:
        spell_sha_spirit_link() : SpellScriptLoader("spell_sha_spirit_link") { }

        class spell_sha_spirit_link_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_spirit_link_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetEntry() == 53006)
                    {
                        if (l_Caster->GetOwner())
                        {
                            if (Player* _player = l_Caster->GetOwner()->ToPlayer())
                            {
                                std::list<Unit*> l_MemberList;
                                _player->GetRaidMembers(l_MemberList);

                                float l_Radius = GetSpellInfo()->Effects[EFFECT_0].CalcRadius(l_Caster, GetSpell());

                                l_MemberList.remove_if([l_Caster, l_Radius](Unit const* p_Unit) -> bool
                                {
                                    if (p_Unit->GetDistance(*l_Caster) > l_Radius)
                                        return true;

                                    return false;
                                });

                                float l_TotalRaidHealthPct = 0;

                                for (auto l_Itr : l_MemberList)
                                    l_TotalRaidHealthPct += l_Itr->GetHealthPct();

                                l_TotalRaidHealthPct /= l_MemberList.size() * 100.0f;

                                for (auto l_Itr : l_MemberList)
                                    l_Itr->SetHealth(uint32(l_TotalRaidHealthPct * l_Itr->GetMaxHealth()));
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_spirit_link_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_spirit_link_SpellScript();
        }
};

/// Last Update 6.2.3
/// Fire Nova - 1535
class spell_sha_fire_nova: public SpellScriptLoader
{
    public:
        spell_sha_fire_nova() : SpellScriptLoader("spell_sha_fire_nova") { }

        class spell_sha_fire_nova_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fire_nova_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(SPELL_SHA_FLAME_SHOCK, l_Caster->GetGUID()))
                    l_Caster->CastSpell(l_Target, SPELL_SHA_FIRE_NOVA_TRIGGERED, true);
            }

            SpellCastResult HandleCheckCast()
            {
                UnitList targets;
                JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(GetCaster(), GetCaster(), 100, SPELL_SHA_FLAME_SHOCK, false);
                JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(GetCaster(), targets, u_check);
                GetCaster()->VisitNearbyObject(100, searcher);

                return targets.size() == 0 ? SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW : SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_fire_nova_SpellScript::HandleCheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_fire_nova_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_fire_nova_SpellScript();
        }
};

/// 88766 Fulmination handled in 8042 Earth Shock
class spell_sha_fulmination: public SpellScriptLoader
{
    public:
        spell_sha_fulmination() : SpellScriptLoader("spell_sha_fulmination") { }

        class spell_sha_fulmination_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fulmination_SpellScript);

            enum eSpells
            {
                T17Elemental2P      = 165577,
                FocusOfTheElements  = 167205,
                T17Elemental4P      = 165580,
                LavaSurgeProc       = 77762
            };

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        Aura* l_Shield = l_Caster->GetAura(SPELL_SHA_LIGHTNING_SHIELD_AURA);
                        if (!l_Shield)
                            return;

                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE);
                        if (!l_SpellInfo)
                            return;

                        uint8 l_Charges = l_Shield->GetCharges();

                        /// Each stack of Lightning Shield consumed by Fulmination increases your multistrike damage by 1% for 10 sec.
                        if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::T17Elemental2P, EFFECT_0))
                        {
                            int32 l_Pct = l_AuraEffect->GetAmount() * l_Charges;
                            l_Caster->CastCustomSpell(eSpells::FocusOfTheElements, SpellValueMod::SPELLVALUE_BASE_POINT0, l_Pct, l_Caster, true);
                        }

                        /// When you consume more than 12 Lightning Shield charges with Fulmination, you automatically gain Lava Surge.
                        if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::T17Elemental4P, EFFECT_0))
                        {
                            if (l_AuraEffect->GetAmount() < (int32)l_Charges)
                                l_Caster->CastSpell(l_Caster, eSpells::LavaSurgeProc, true);
                        }

                        l_Charges = l_Charges - 1;
                        if (!l_Charges)
                            return;

                        int32 l_BP = l_Caster->CalculateSpellDamage(l_Target, l_SpellInfo, EFFECT_0);
                        uint32 l_Damage = l_Charges * l_Caster->SpellDamageBonusDone(l_Target, l_SpellInfo, l_BP, EFFECT_0, SPELL_DIRECT_DAMAGE);

                        l_Caster->CastCustomSpell(SPELL_SHA_FULMINATION_TRIGGERED, SPELLVALUE_BASE_POINT0, l_Damage, l_Target, true);
                        l_Shield->SetCharges(1);

                        l_Caster->RemoveAura(SPELL_SHA_FULMINATION_INFO);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_fulmination_SpellScript::HandleAfterHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_fulmination_SpellScript();
        }
};

/// Last Update 6.2.3
/// 77762 - Lava Surge
class spell_sha_lava_surge: public SpellScriptLoader
{
    public:
        spell_sha_lava_surge() : SpellScriptLoader("spell_sha_lava_surge") { }

        class spell_sha_lava_surge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_lava_surge_AuraScript);

            enum eSpells
            {
                LavaBurst               = 51505,
                LavaSurgeVisualRight    = 174928
            };

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (GetCaster() == nullptr)
                    return;

                Player* l_Player = GetCaster()->ToPlayer();
                if (l_Player == nullptr)
                    return;

                if (SpellInfo const* l_LavaBurst = sSpellMgr->GetSpellInfo(eSpells::LavaBurst))
                    l_Player->RestoreCharge(l_LavaBurst->ChargeCategoryEntry);

                l_Player->CastSpell(l_Player, eSpells::LavaSurgeVisualRight, true);
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                
                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::LavaSurgeVisualRight))
                    l_Caster->RemoveAura(eSpells::LavaSurgeVisualRight);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sha_lava_surge_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectApplyFn(spell_sha_lava_surge_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_lava_surge_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Healing Stream - 52042
class spell_sha_healing_stream: public SpellScriptLoader
{
    public:
        spell_sha_healing_stream() : SpellScriptLoader("spell_sha_healing_stream") { }

        class spell_sha_healing_stream_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_stream_SpellScript);

            enum eSpells
            {
                RushingStreams = 147074,
                GlyphOfHealingStreamTotem = 55456,
                GlyphOfHealingStreamTotemAura = 119523
            };

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_HEALING_STREAM))
                    return false;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();

                if (Unit* l_Owner = l_Caster->GetOwner())
                    l_Caster = l_Owner;

                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());

                    if (l_Caster->HasAura(eSpells::RushingStreams)) ///< Your Healing Stream Totem now heals two targets 
                        p_Targets.resize(2);
                    else
                        p_Targets.resize(1);
                }
            }

            void HandleOnHit()
            {
                Unit* l_Owner = GetCaster()->GetOwner();
                Unit* l_Target = GetHitUnit();

                if (l_Owner == nullptr || l_Target == nullptr)
                    return;

                Player* l_Player = l_Owner->ToPlayer();

                if (l_Player == nullptr)
                    return;

                /// Glyph of Healing Stream Totem
                if (l_Player->HasGlyph(eSpells::GlyphOfHealingStreamTotem))
                    l_Player->CastSpell(l_Target, eSpells::GlyphOfHealingStreamTotemAura, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_stream_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                OnHit += SpellHitFn(spell_sha_healing_stream_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_stream_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Elemental Blast - 117014
class spell_sha_elemental_blast: public SpellScriptLoader
{
    public:
        spell_sha_elemental_blast() : SpellScriptLoader("spell_sha_elemental_blast") { }

        class spell_sha_elemental_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_elemental_blast_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_BLAST))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                std::vector<uint32> l_SpellsToCast =
                {
                    SPELL_SHA_ELEMENTAL_BLAST_CRIT_BONUS,
                    SPELL_SHA_ELEMENTAL_BLAST_HASTE_BONUS,
                    SPELL_SHA_ELEMENTAL_BLAST_MASTERY_BONUS,
                    SPELL_SHA_ELEMENTAL_BLAST_MULTISTRIKE_BONUS
                };

                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_SHAMAN_ENHANCEMENT)
                    l_SpellsToCast.push_back(SPELL_SHA_ELEMENTAL_BLAST_AGILITY_BONUS);
                else if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_SHAMAN_RESTORATION)
                    l_Player->CastSpell(l_Player, SPELL_SHA_ELEMENTAL_BLAST_SPIRIT_BONUS, true);

                l_Player->CastSpell(GetCaster(), l_SpellsToCast[urand(0, l_SpellsToCast.size() - 1)], true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_elemental_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_elemental_blast_SpellScript();
        }
};

/// Earthquake - 61882
/// last update : 6.1.2 19802
class spell_sha_earthquake: public SpellScriptLoader
{
    public:
        spell_sha_earthquake() : SpellScriptLoader("spell_sha_earthquake") { }

        class spell_sha_earthquake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_earthquake_AuraScript);

            enum eSpells
            {
                Earthquake             = 61882,
                EarthquakeTick         = 77478,
                EarthquakeSlow         = 182387,
                ImprovedChainLightning = 157766,
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                AreaTrigger* l_AreaTrigger = l_Caster->GetAreaTrigger(eSpells::Earthquake);
                if (!l_AreaTrigger)
                    return;

                /// dealing ${$SPN * 0.11 * 10 * (1 + $170374m3 / 100)} Physical damage over $d
                int32 l_Bp0 = l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL) * 0.11 * 10;

                if (AuraEffect* l_ChainLightning = l_Caster->GetAuraEffect(eSpells::ImprovedChainLightning, EFFECT_0))
                    l_Bp0 += CalculatePct(l_Bp0, l_ChainLightning->GetAmount());

                l_Bp0 /= GetSpellInfo()->GetDuration() / IN_MILLISECONDS;

                l_Caster->CastCustomSpell(l_AreaTrigger->GetPositionX(), l_AreaTrigger->GetPositionY(), l_AreaTrigger->GetPositionZ(), eSpells::EarthquakeTick, &l_Bp0, nullptr, nullptr, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthquake_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_earthquake_AuraScript();
        }

        class spell_sha_earthquake_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthquake_SpellScript);

            enum eSpells
            {
                ImprovedChainLightningEarthquakeMod = 157766
            };

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::ImprovedChainLightningEarthquakeMod))
                        l_Caster->RemoveAura(eSpells::ImprovedChainLightningEarthquakeMod);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_earthquake_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthquake_SpellScript();
        }
};

/// Earthquake : Ticks - 77478
/// last update : 6.1.2 19802
class spell_sha_earthquake_tick: public SpellScriptLoader
{
public:
    spell_sha_earthquake_tick() : SpellScriptLoader("spell_sha_earthquake_tick") { }

    class spell_sha_earthquake_tick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_earthquake_tick_SpellScript);

        enum eSpells
        {
            EarthquakeTick      = 77478,
            EarthquakeSlow      = 182387,
            EarthquakeKnockDown = 77505
        };

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(eSpells::EarthquakeTick))
                return false;
            return true;
        }

        void HandleOnHit()
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();
            if (!l_Caster || !l_Target)
                return;

            l_Caster->CastSpell(l_Target, eSpells::EarthquakeSlow, true);

            /// 10% chance of knocking down affected targets
            if (roll_chance_i(GetSpellInfo()->Effects[EFFECT_1].BasePoints))
                l_Caster->CastSpell(l_Target, eSpells::EarthquakeKnockDown, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_sha_earthquake_tick_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_sha_earthquake_tick_SpellScript();
    }
};

/// Healing Rain - 73920
class spell_sha_healing_rain : public SpellScriptLoader
{
    public:
        spell_sha_healing_rain() : SpellScriptLoader("spell_sha_healing_rain") { }

        enum eSpell : uint32
        {
            HealingRainHeal = 73921,
        };

        enum eNPC : uint32
        {
            HealingRainTrigger = 73400
        };

        class spell_sha_healing_rain_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_rain_SpellScript);

            void Hit()
            {
                Unit* l_Caster = GetCaster();

                if (WorldLocation const* l_Loc = GetExplTargetDest())
                {
                    if (Creature* l_Trigger = Creature::GetCreature(*l_Caster, l_Caster->GetHealingRainTrigger()))
                        l_Trigger->DespawnOrUnsummon();

                    if (TempSummon* l_Summon = l_Caster->SummonCreature(eNPC::HealingRainTrigger, *l_Loc, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Summon->SetOwnerGUID(l_Caster->GetGUID());
                        l_Caster->SetHealingRainTrigger(l_Summon->GetGUID());
                    }

                    if (AuraEffect* l_Conductivity = l_Caster->GetAuraEffect(SPELL_SHA_CONDUCTIVITY_TALENT, EFFECT_0))
                        l_Conductivity->SetAmount(l_Conductivity->GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_sha_healing_rain_SpellScript::Hit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_sha_healing_rain_SpellScript();
        }

        class spell_sha_healing_rain_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_healing_rain_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (Creature* l_Trigger = Creature::GetCreature(*l_Caster, l_Caster->GetHealingRainTrigger()))
                    l_Caster->CastSpell(*l_Trigger, eSpell::HealingRainHeal, true);
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (Creature* l_Trigger = Creature::GetCreature(*l_Caster, l_Caster->GetHealingRainTrigger()))
                    l_Trigger->DespawnOrUnsummon();
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_healing_rain_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_sha_healing_rain_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_sha_healing_rain_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Healing Rain (heal) - 73921
class spell_sha_healing_rain_heal : public SpellScriptLoader
{
    public:
        spell_sha_healing_rain_heal() : SpellScriptLoader("spell_sha_healing_rain_heal") { }

        class spell_sha_healing_rain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_rain_heal_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                /// Healing up to 6 allies 
                if (p_Targets.size() > 6)
                    JadeCore::RandomResizeList(p_Targets, 6);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_rain_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_rain_heal_SpellScript();
        }
};

class EarthenPowerTargetSelector
{
    public:
        EarthenPowerTargetSelector() { }

        bool operator() (WorldObject* target)
        {
            if (!target->ToUnit())
                return true;

            if (!target->ToUnit()->HasAuraWithMechanic(1 << MECHANIC_SNARE))
                return true;

            return false;
        }
};

/// last update : 6.1.2 19802
/// Bloodlust - 2825
class spell_sha_bloodlust : public SpellScriptLoader
{
    public:
        spell_sha_bloodlust() : SpellScriptLoader("spell_sha_bloodlust") { }

        class spell_sha_bloodlust_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_bloodlust_SpellScript);

            bool Validate(SpellInfo const* /*p_SpellEntry*/) override
            {
                if (!sSpellMgr->GetSpellInfo(ShamanSpells::SHAMAN_SPELL_SATED))
                    return false;

                return true;
            }

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (GetSpellInfo() && l_Target->HasAura(GetSpellInfo()->Id))
                        l_Target->CastSpell(l_Target, ShamanSpells::SHAMAN_SPELL_SATED, true);
                }
            }

            void HandleImmunity(SpellEffIndex p_EffIndex)
            {
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(SHAMAN_SPELL_EXHAUSTION) || l_Target->HasAura(HUNTER_SPELL_INSANITY) ||
                    l_Target->HasAura(SHAMAN_SPELL_SATED) || l_Target->HasAura(MAGE_SPELL_TEMPORAL_DISPLACEMENT) ||
                    l_Target->HasAura(HUNTER_SPELL_FATIGUED))
                    PreventHitAura();
            }


            void Register() override
            {
                AfterHit += SpellHitFn(spell_sha_bloodlust_SpellScript::ApplyDebuff);
                OnEffectHitTarget += SpellEffectFn(spell_sha_bloodlust_SpellScript::HandleImmunity, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_sha_bloodlust_SpellScript::HandleImmunity, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_bloodlust_SpellScript();
        }
};


/// Heroism - 32182 - last update: 6.1.2 19802
class spell_sha_heroism: public SpellScriptLoader
{
    public:
        spell_sha_heroism() : SpellScriptLoader("spell_sha_heroism") { }

        class spell_sha_heroism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_heroism_SpellScript);

            bool Validate(SpellInfo const* /*p_SpellEntry*/) override
            {
                if (!sSpellMgr->GetSpellInfo(ShamanSpells::SHAMAN_SPELL_EXHAUSTION))
                    return false;

                return true;
            }

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (GetSpellInfo() && l_Target->HasAura(GetSpellInfo()->Id))
                        l_Target->CastSpell(l_Target, ShamanSpells::SHAMAN_SPELL_EXHAUSTION, true);
                }
            }

            void HandleImmunity(SpellEffIndex p_EffIndex)
            {
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(SHAMAN_SPELL_EXHAUSTION) || l_Target->HasAura(HUNTER_SPELL_INSANITY) ||
                    l_Target->HasAura(SHAMAN_SPELL_SATED) || l_Target->HasAura(MAGE_SPELL_TEMPORAL_DISPLACEMENT) ||
                    l_Target->HasAura(HUNTER_SPELL_FATIGUED))
                    PreventHitAura();
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_sha_heroism_SpellScript::ApplyDebuff);
                OnEffectHitTarget += SpellEffectFn(spell_sha_heroism_SpellScript::HandleImmunity, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_sha_heroism_SpellScript::HandleImmunity, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_heroism_SpellScript();
        }
};

/// 105792 - Lava Shock Spread
class spell_sha_lava_lash_spread: public SpellScriptLoader
{
    public:
        spell_sha_lava_lash_spread() : SpellScriptLoader("spell_sha_lava_lash_spread") { }

        class spell_sha_lava_lash_spread_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_lash_spread_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                if (Unit* target = GetHitUnit())
                    GetCaster()->AddAura(SPELL_SHA_FLAME_SHOCK, target);
            }

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                uint32 l_MaxTargets = sSpellMgr->GetSpellInfo(SPELL_SHA_LAVA_LASH)->Effects[EFFECT_3].BasePoints;
                Unit* l_MainTarget = GetExplTargetUnit();

                if (l_MainTarget == nullptr)
                    return;

                p_Targets.remove_if([this, l_MainTarget](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->ToUnit() == nullptr || p_Object->ToUnit()->GetGUID() == l_MainTarget->GetGUID())
                        return true;

                    return false;
                });

                if (p_Targets.size() > l_MaxTargets)
                    JadeCore::RandomResizeList(p_Targets, l_MaxTargets);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_lava_lash_spread_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_lava_lash_spread_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_lash_spread_SpellScript();
        }
};

/// Flame Shock - 8050
class spell_sha_flame_shock : public SpellScriptLoader
{
    public:
        spell_sha_flame_shock() : SpellScriptLoader("spell_sha_flame_shock") { }

        class spell_sha_flame_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_flame_shock_SpellScript);

            bool m_HasUnleashFlame = false;

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                /// In Elemental and Enhancement specs auras have different IDs
                uint32 l_BonusSpellId = 0;
                if (l_Player->GetSpecializationId() == SPEC_SHAMAN_ELEMENTAL)
                    l_BonusSpellId = SPELL_SHA_UNLEASH_FLAME_AURA_ELEMENTAL;
                else if (l_Player->GetSpecializationId() == SPEC_SHAMAN_ENHANCEMENT)
                    l_BonusSpellId = SPELL_SHA_UNLEASH_FLAME_AURA_ENCHANCEMENT;

                SpellInfo const* l_UnleashFlame = sSpellMgr->GetSpellInfo(l_BonusSpellId);
                SpellInfo const* l_ElementalFusion = sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_FUSION_PROC);

                if (l_Caster->HasAura(l_BonusSpellId) && l_UnleashFlame != nullptr)
                {
                    m_HasUnleashFlame = true;
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_UnleashFlame->Effects[EFFECT_1].BasePoints));
                    l_Caster->RemoveAurasDueToSpell(l_BonusSpellId);
                }

                if (l_ElementalFusion != nullptr)
                {
                    if (Aura* l_ElementalFusionAura = l_Caster->GetAura(SPELL_SHA_ELEMENTAL_FUSION_PROC))
                    {
                        /// 1 stack - 40%, 2 stacks - 80%
                        uint32 l_Stacks = l_ElementalFusionAura->GetStackAmount();
                        SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_Stacks * l_ElementalFusion->Effects[EFFECT_0].BasePoints));
                        l_Caster->RemoveAurasDueToSpell(SPELL_SHA_ELEMENTAL_FUSION_PROC);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_flame_shock_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        class spell_sha_flame_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_flame_shock_AuraScript);

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                /// In Elemental and Enhancement specs auras have different IDs
                uint32 l_BonusSpellId = 0;
                if (l_Player->GetSpecializationId() == SPEC_SHAMAN_ELEMENTAL)
                    l_BonusSpellId = SPELL_SHA_UNLEASH_FLAME_AURA_ELEMENTAL;
                else if (l_Player->GetSpecializationId() == SPEC_SHAMAN_ENHANCEMENT)
                    l_BonusSpellId = SPELL_SHA_UNLEASH_FLAME_AURA_ENCHANCEMENT;

                SpellInfo const* l_ElementalFusion = sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_FUSION_PROC);
                SpellInfo const* l_UnleashFlame = sSpellMgr->GetSpellInfo(l_BonusSpellId);

                if (l_ElementalFusion != nullptr)
                {
                    if (Aura* l_ElementalFusionAura = l_Caster->GetAura(SPELL_SHA_ELEMENTAL_FUSION_PROC))
                    {
                        /// 1 stack - 40%, 2 stacks - 80%
                        uint32 l_Stacks = l_ElementalFusionAura->GetStackAmount();
                        p_Amount += CalculatePct(p_Amount, l_Stacks * l_ElementalFusion->Effects[EFFECT_0].BasePoints);
                    }
                }

                if (l_Caster->HasAura(l_BonusSpellId) && l_UnleashFlame != nullptr)
                    p_Amount += CalculatePct(p_Amount, l_UnleashFlame->Effects[EFFECT_1].BasePoints);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_flame_shock_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_flame_shock_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_flame_shock_SpellScript();
        }
};

/// Call by Chain Heal - 1064 and Chain Heal (T17 Proc) - 177972
class spell_sha_improved_chain_heal : public SpellScriptLoader
{
    public:
        spell_sha_improved_chain_heal() : SpellScriptLoader("spell_sha_improved_chain_heal") { }

        class spell_sha_improved_chain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_improved_chain_heal_SpellScript);

            enum eSpells
            {
                ImprovedChainHealAura = 157813
            };

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                Unit* l_ExplTarget = GetExplTargetUnit();

                if (l_Target == nullptr || l_ExplTarget == nullptr)
                    return;

                if (!l_Caster->HasAura(eSpells::ImprovedChainHealAura))
                    return;

                if (l_ExplTarget->GetGUID() != l_Target->GetGUID()) ///< Only first target
                    return;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::ImprovedChainHealAura);
                int32 l_Heal = GetHitHeal();

                l_Heal += CalculatePct(l_Heal, l_SpellInfo->Effects[EFFECT_0].BasePoints);
                SetHitHeal(l_Heal);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_improved_chain_heal_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_improved_chain_heal_SpellScript();
        }
};

/// Healing Wave - 77472
class spell_sha_healing_wave : public SpellScriptLoader
{
    public:
        spell_sha_healing_wave() : SpellScriptLoader("spell_sha_healing_wave") { }

        class spell_sha_healing_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_wave_SpellScript);

            enum eSpells
            {
                GlyphOfHealingWave = 55440,
                GlyphOfHealingWaveHeal = 55533
            };

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                
                if (l_Target == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::GlyphOfHealingWave)) ///< Glyph of Healing Wave
                {
                    SpellInfo const* l_GlyphOfHealingWave = sSpellMgr->GetSpellInfo(eSpells::GlyphOfHealingWave);

                    if (l_Caster->GetGUID() == l_Target->GetGUID() || l_GlyphOfHealingWave == nullptr) ///< When you heal someone else
                        return;

                    int32 l_HealAmount = (GetHitHeal() / l_GlyphOfHealingWave->Effects[EFFECT_0].BasePoints); ///< Your Healing Wave also heals you for 10 % of the healing effect
                    
                    l_Caster->CastCustomSpell(l_Caster, eSpells::GlyphOfHealingWaveHeal, &l_HealAmount, nullptr, nullptr, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_healing_wave_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_wave_SpellScript();
        }
};

/// last Update 6.2.3
/// Lava Lash - 60103
class spell_sha_lava_lash: public SpellScriptLoader
{
    public:
        spell_sha_lava_lash() : SpellScriptLoader("spell_sha_lava_lash") { }

        class spell_sha_lava_lash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_lash_SpellScript);

            enum eSpells
            {
                FlameAura = 73683
            };

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (l_Target->HasAura(SPELL_SHA_FLAME_SHOCK) && !l_Caster->HasAura(SPELL_SHA_GLYPH_OF_LAVA_LASH))
                    l_Caster->CastSpell(l_Target, SPELL_SHA_LAVA_LASH_SPREAD, true);

                if (l_Caster->HasAura(SPELL_SHA_ELEMENTAL_FUSION))
                    l_Caster->CastSpell(l_Caster, SPELL_SHA_ELEMENTAL_FUSION_PROC, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_lava_lash_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_lash_SpellScript();
        }
};

/// 33757 - Windfury and Feral Spirit Windfury Driver - 170523
class spell_sha_windfury: public SpellScriptLoader
{
    public:
        spell_sha_windfury() : SpellScriptLoader("spell_sha_windfury") { }

        class spell_sha_windfury_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_windfury_AuraScript);

            enum eSpells
            {
                FeralSpiritWindFury = 170512
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                        return;

                    if (Unit* l_Victim = p_EventInfo.GetActionTarget())
                    {
                        if (!l_Victim->IsFriendlyTo(l_Player))
                        {
                            l_Player->AddSpellCooldown(GetSpellInfo()->Id, 0, 5 * IN_MILLISECONDS);

                            uint8 l_Count = 3;

                            if (Aura* l_Bonus = GetCaster()->GetAura(SPELL_SHA_PVP_BONUS_WOD_4))
                                l_Count += l_Bonus->GetEffect(EFFECT_0)->GetAmount();

                            for (uint8 l_I = 0; l_I < l_Count; l_I++)
                                l_Player->CastSpell(l_Victim, SPELL_SHA_WINDFURY_ATTACK, true);
                        }
                    }
                }
                else if (Creature* l_Creature = GetCaster()->ToCreature())
                {
                    if (l_Creature->IsAIEnabled)
                        l_Creature->AI()->DoAction(0);  ///< ActionWindfury
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_windfury_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_windfury_AuraScript();
        }
};

enum GlyphOfEternalEarth
{
    LightningBolt   = 403,
    EarthShield     = 974
};

/// Glyph of Eternal Earth (Restoration) - 147781
class spell_sha_glyph_of_eternal_earth : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_eternal_earth() : SpellScriptLoader("spell_sha_glyph_of_eternal_earth") { }

        class spell_sha_glyph_of_eternal_earth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_glyph_of_eternal_earth_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_ProcInfo)
            {
                PreventDefaultAction();

                if (!p_ProcInfo.GetDamageInfo())
                    return;

                if (!p_ProcInfo.GetDamageInfo()->GetSpellInfo() || p_ProcInfo.GetDamageInfo()->GetSpellInfo()->Id != GlyphOfEternalEarth::LightningBolt)
                    return;

                if (Unit* l_Caster = p_ProcInfo.GetActor())
                {
                    if (Aura* l_EarthShield = l_Caster->GetAura(GlyphOfEternalEarth::EarthShield))
                    {
                        if (l_EarthShield->GetCharges() >= GetSpellInfo()->Effects[EFFECT_0].BasePoints) ///< This cannot cause Earth Shield to exceed 9 charges.
                            return;
                        else
                            l_EarthShield->ModCharges(1);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_eternal_earth_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_glyph_of_eternal_earth_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Improved Flame Shock - 157804
class spell_sha_improoved_flame_shock: public SpellScriptLoader
{
    public:
        spell_sha_improoved_flame_shock() : SpellScriptLoader("spell_sha_improoved_flame_shock") { }

        class spell_sha_improoved_flame_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_improoved_flame_shock_AuraScript);

            enum eSpells
            {
                LavaLash = 60103,
                LavaLashHighlight = 144967
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_SHA_FLAME_SHOCK)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;;

                if (SpellInfo const* l_LavaLash = sSpellMgr->GetSpellInfo(eSpells::LavaLash))
                {
                    l_Player->RestoreCharge(l_LavaLash->ChargeCategoryEntry);
                    l_Player->CastSpell(l_Player, LavaLashHighlight, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_improoved_flame_shock_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_improoved_flame_shock_AuraScript();
        }
};

/// 51533 - Feral Spirit
class spell_sha_feral_spirit: public SpellScriptLoader
{
    public:
        spell_sha_feral_spirit() : SpellScriptLoader("spell_sha_feral_spirit") { }

        class spell_sha_feral_spirit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_feral_spirit_SpellScript);

            enum eSpells
            {
                T17Enhancement4P        = 165610,
                FeralSpiritProc         = 167204,
                WolfTransform           = 172752,
                RaptorTransform         = 172753,
                GlyphOfSpiritRaptors    = 147783
            };

            void OnLaunch(SpellEffIndex p_EffIndex)
            {
                /// Broken spellproc
                if (Unit* l_Caster = GetCaster())
                {
                    if (AuraEffect* l_Aura = l_Caster->GetAuraEffect(SPELL_SHA_PVP_BONUS_WOD_2, EFFECT_0))
                        l_Caster->CastSpell(l_Caster, l_Aura->GetTriggerSpell());

                    /// While Feral Spirits is active, you also turn into a Feral Spirit. While you are transformed, you move 25% faster, and your wolves can proc Windfury.
                    if (l_Caster->HasAura(eSpells::T17Enhancement4P))
                    {
                        l_Caster->CastSpell(l_Caster, eSpells::FeralSpiritProc, true);

                        if (l_Caster->HasAura(eSpells::GlyphOfSpiritRaptors))
                            l_Caster->CastSpell(l_Caster, eSpells::RaptorTransform, true);
                        else
                            l_Caster->CastSpell(l_Caster, eSpells::WolfTransform, true);
                    }
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_sha_feral_spirit_SpellScript::OnLaunch, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_feral_spirit_SpellScript();
        }
};

/// Spirit Hunt - 58877
class spell_sha_pet_spirit_hunt: public SpellScriptLoader
{
    public:
        spell_sha_pet_spirit_hunt() : SpellScriptLoader("spell_sha_pet_spirit_hunt") { }

        enum eSpells
        {
            SpiritHuntHeal = 58879,
            GlyphofFeralSpirit = 63271
        };

        class spell_sha_pet_spirit_hunt_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_pet_spirit_hunt_AuraScript);

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Unit* l_Owner = l_Caster->GetOwner();
                if (!l_Owner)
                    return;

                if (p_EventInfo.GetDamageInfo() == nullptr)
                    return;

                int32 l_TakenDamage = p_EventInfo.GetDamageInfo()->GetDamage();
                if (!l_TakenDamage)
                    return;

                int32 l_HealAmount = CalculatePct(l_TakenDamage, p_AurEff->GetAmount());

                if (Aura* l_GlyphofFeralSpirit = l_Owner->GetAura(eSpells::GlyphofFeralSpirit)) ///< Increases the healing done by your Feral Spirits' Spirit Hunt by 40%
                    l_HealAmount += CalculatePct(l_HealAmount, l_GlyphofFeralSpirit->GetEffect(EFFECT_0)->GetAmount());

                l_Caster->CastCustomSpell(l_Owner, eSpells::SpiritHuntHeal, &l_HealAmount, nullptr, nullptr, true);
                l_Caster->CastCustomSpell(l_Caster, eSpells::SpiritHuntHeal, &l_HealAmount, nullptr, nullptr, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_pet_spirit_hunt_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_pet_spirit_hunt_AuraScript();
        }
};

/// 88766 - Fulmination
class spell_sha_fulmination_proc: public SpellScriptLoader
{
    public:
        spell_sha_fulmination_proc() : SpellScriptLoader("spell_sha_fulmination_proc") { }

        class spell_sha_fulmination_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_fulmination_proc_AuraScript);

            enum eSpells
            {
                LightningBolt               = 403,
                ChainLightning              = 421,
                LavaBust                    = 51505,
                ImprovedLightningShield     = 157774
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() == nullptr)
                    return;

                if (!(l_Caster->HasAura(eSpells::ImprovedLightningShield) && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == eSpells::LavaBust))
                {
                    if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != eSpells::LightningBolt && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != eSpells::ChainLightning && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != 114074)
                        return;
                }

                Unit* l_Target = p_EventInfo.GetProcTarget();

                if (l_Target == nullptr)
                    return;

                uint32 l_MaxCharges = GetEffect(EFFECT_0)->CalculateAmount(l_Caster);
                if (Aura* l_Aura = GetCaster()->GetAura(SPELL_SHA_LIGHTNING_SHIELD))
                {
                    if (l_Aura->GetCharges() < l_MaxCharges)
                        l_Aura->SetCharges(l_Aura->GetCharges() + 1);

                    if (l_Aura->GetCharges() == l_MaxCharges && !l_Caster->HasAura(SPELL_SHA_FULMINATION_INFO))
                        l_Caster->CastSpell(l_Caster, SPELL_SHA_FULMINATION_INFO, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_fulmination_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_fulmination_proc_AuraScript();
        }
};

/// Enhanced Chain Lightning - 157765
/// Called by Chain Lightning - 421 and Lava Beam - 114074
class spell_sha_enhanced_chain_lightning: public SpellScriptLoader
{
    public:
        spell_sha_enhanced_chain_lightning() : SpellScriptLoader("spell_sha_enhanced_chain_lightning") { }

        class spell_sha_enhanced_chain_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_enhanced_chain_lightning_SpellScript);

            enum eSpells
            {
                ImprovedChainLightning              = 157765,
                ImprovedChainLightningEarthquakeMod = 157766,
            };

            uint8 m_TargetReach = 0;

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::ImprovedChainLightning) && ++m_TargetReach == 3)
                    l_Caster->CastSpell(l_Caster, eSpells::ImprovedChainLightningEarthquakeMod, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_enhanced_chain_lightning_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_enhanced_chain_lightning_SpellScript();
        }
};

/// Echo of Elements - 108283
class spell_sha_echo_of_elements: public SpellScriptLoader
{
    public:
        spell_sha_echo_of_elements() : SpellScriptLoader("spell_sha_echo_of_elements") { }

        class spell_sha_echo_of_elements_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_echo_of_elements_AuraScript);

            void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                switch (l_Player->GetSpecializationId(l_Player->GetActiveSpec()))
                {
                    case SPEC_SHAMAN_ELEMENTAL:
                        l_Player->CastSpell(l_Player, SPELL_SHA_ECHO_OF_THE_ELEMENTS_ELEMENTAL, true);
                        break;
                    case SPEC_SHAMAN_ENHANCEMENT:
                        l_Player->CastSpell(l_Player, SPELL_SHA_ECHO_OF_THE_ELEMENTS_ENHANCEMENT, true);
                        break;
                    case SPEC_SHAMAN_RESTORATION:
                        l_Player->CastSpell(l_Player, SPELL_SHA_ECHO_OF_THE_ELEMENTS_RESTORATION, true);
                        break;
                    default:
                        break;
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_echo_of_elements_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_MAX_CHARGES);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_echo_of_elements_AuraScript();
        }
};

/// 152255 Liquid Magma - a bit hacky, will do it properly later
class spell_sha_liquid_magma: public SpellScriptLoader
{
    public:
        spell_sha_liquid_magma() : SpellScriptLoader("spell_sha_liquid_magma") { }

        class spell_sha_liquid_magma_AuraScript: public AuraScript
        {
            PrepareAuraScript(spell_sha_liquid_magma_AuraScript);

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                // hardcoded in the tooltip - no DBC data here
                Unit* l_Target = l_Caster->SelectNearbyTarget(nullptr, 40, 0, false, true, true, true);

                if (l_Target)
                    l_Caster->CastSpell(l_Target, p_AurEff->GetTriggerSpell());
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_liquid_magma_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }

        };

        class spell_sha_liquid_magma_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_liquid_magma_SpellScript);

            SpellCastResult HandleCheckCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (Creature* l_Totem = l_Player->GetMap()->GetCreature(l_Player->m_SummonSlot[SUMMON_SLOT_TOTEM]))
                        if (l_Totem && l_Totem->isTotem())
                            return SPELL_CAST_OK;

                if (GetCaster()->isTotem())
                    return SPELL_CAST_OK;

                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_HAVE_FIRE_TOTEM);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            void HandleApply(SpellEffIndex /*effIndex*/)
            {
                if (Player* l_Player = GetHitUnit()->ToPlayer())
                {
                    if (Creature* l_Totem = l_Player->GetMap()->GetCreature(l_Player->m_SummonSlot[SUMMON_SLOT_TOTEM]))
                    {
                        if (l_Totem && l_Totem->isTotem())
                        {
                            l_Totem->AddAura(GetSpellInfo()->Id, l_Totem);
                            PreventHitAura();
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_liquid_magma_SpellScript::HandleCheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_liquid_magma_SpellScript::HandleApply, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_liquid_magma_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_liquid_magma_AuraScript();
        }
};

/// 157501 Liquid Magma
class spell_sha_liquid_magma_visual: public SpellScriptLoader
{
    public:
        spell_sha_liquid_magma_visual() : SpellScriptLoader("spell_sha_liquid_magma_visual") { }

        class spell_sha_liquid_magma_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_liquid_magma_visual_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHA_LIQUID_MAGMA_DAMAGE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_liquid_magma_visual_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_liquid_magma_visual_SpellScript();
        }
};

/// 2645 Ghost Wolf
class spell_sha_ghost_wolf: public SpellScriptLoader
{
    public:
        spell_sha_ghost_wolf() : SpellScriptLoader("spell_sha_ghost_wolf") { }

        class spell_sha_ghost_wolf_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ghost_wolf_AuraScript);

            void CalculateAmount(AuraEffect const* p_AurEff, int32& p_Amount, bool& p_CanBeRecalculated)
            {
                SpellInfo const* l_GhostlySpeed = sSpellMgr->GetSpellInfo(SPELL_SHA_GLYPH_OF_GHOSTLY_SPEED);
                if (! l_GhostlySpeed)
                    return;

                p_CanBeRecalculated = true;
                if (Player* l_Owner = GetOwner()->ToPlayer())
                {
                    if (l_Owner->HasGlyph(l_GhostlySpeed->Id))
                    {
                        if (!l_Owner->isInCombat())
                        {
                            if (Map* l_Map = l_Owner->GetMap())
                            {
                                if (!l_Map->IsBattlegroundOrArena())
                                {
                                    if (l_Owner->IsOutdoors())
                                    {
                                        p_Amount += l_GhostlySpeed->Effects[EFFECT_0].BasePoints;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_ghost_wolf_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_MOD_INCREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ghost_wolf_AuraScript();
        }
};

/// last update : 6.2.3
/// Lava Burst - 51505
class spell_sha_lava_burst: public SpellScriptLoader
{
    public:
        spell_sha_lava_burst() : SpellScriptLoader("spell_sha_lava_burst") { }

        class spell_sha_lava_burst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_burst_SpellScript);

            bool m_HasLavaSurge = false;
            enum eSpells
            {
                LavaSurge = 77762,
                LavaBurst = 51505,
            };

            void HitTarget(SpellEffIndex)
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Player == nullptr)
                    return;

                if (l_Player->HasAura(SPELL_SHA_ELEMENTAL_FUSION))
                    l_Player->CastSpell(l_Player, SPELL_SHA_ELEMENTAL_FUSION_PROC, true);

                /// Lavaburst deals 50% more damage with Flame Shock on target
                if (l_Target->HasAura(SPELL_SHA_FLAME_SHOCK))
                    SetHitDamage(int32(GetHitDamage() * 1.5f));
            }

            void HandleBeforeCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster->HasAura(eSpells::LavaSurge))
                    m_HasLavaSurge = true;
            }

            void HandleAfterCast()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                if (SpellInfo const* l_LavaSurge = sSpellMgr->GetSpellInfo(eSpells::LavaSurge))
                    l_Player->RestoreCharge(l_LavaSurge->ChargeCategoryEntry);
            }

            void HandleAfterHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                if (l_Player->HasAura(eSpells::LavaSurge))
                {
                    if (SpellInfo const* l_LavaBurst = sSpellMgr->GetSpellInfo(eSpells::LavaBurst))
                        l_Player->RestoreCharge(l_LavaBurst->ChargeCategoryEntry);
                }
            }

            void Register()
            {
                OnPrepare += SpellOnPrepareFn(spell_sha_lava_burst_SpellScript::HandleBeforeCast);
                AfterHit += SpellHitFn(spell_sha_lava_burst_SpellScript::HandleAfterHit);
                AfterCast += SpellCastFn(spell_sha_lava_burst_SpellScript::HandleAfterCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_lava_burst_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_burst_SpellScript();
        }
};

/// Call by Chain Heal - 1064, Riptide - 61295 and Chain Heal (T17 Proc) - 177972
/// Tidal Waves - 51564
class spell_sha_tidal_waves : public SpellScriptLoader
{
    public:
        spell_sha_tidal_waves() : SpellScriptLoader("spell_sha_tidal_waves") { }

        class spell_sha_tidal_waves_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_tidal_waves_SpellScript);

            enum eSpells
            {
                T17Restoration4P        = 167702,
                HarmonyOfTheElements    = 167703
            };

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_SHA_TIDAL_WAVES))
                    {
                        l_Caster->CastSpell(l_Caster, SPELL_SHA_TIDAL_WAVES_PROC, true);

                        /// When you gain Tidal Waves, you have a 8% chance to reduce the mana cost of Chain Heal by 50% for 8 sec.
                        if (l_Caster->HasAura(eSpells::T17Restoration4P) && roll_chance_i(8))
                            l_Caster->CastSpell(l_Caster, eSpells::HarmonyOfTheElements, true);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_tidal_waves_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_tidal_waves_SpellScript();
        }
};

/// Last updated : 6.1.2 19802
/// Chain Heal - 1064
class spell_sha_chain_heal : public SpellScriptLoader
{
    public:
        spell_sha_chain_heal() : SpellScriptLoader("spell_sha_chain_heal") { }

        class spell_sha_chain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_chain_heal_SpellScript);

            enum eSpells
            {
                Riptide             = 61295,
                T17Restoration2P    = 165576,
                ChainHealTriggered  = 177972
            };

            uint64 m_ProcTarget;

            bool Load() override
            {
                m_ProcTarget = 0;
                return true;
            }

            void CheckTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (AuraEffect* l_T17Restoration = l_Caster->GetAuraEffect(eSpells::T17Restoration2P, EFFECT_0))
                    {
                        if (!roll_chance_i(l_T17Restoration->GetAmount()))
                            return;

                        m_ProcTarget = JadeCore::Containers::SelectRandomContainerElement(p_Targets)->GetGUID();
                    }
                }
            }

            void HandleHeal(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_FirstTarget = GetExplTargetUnit();
                if (l_FirstTarget == nullptr)
                    return;

                /// You have a chance when you cast Chain Heal to cast a second Chain Heal at one of the targets healed.
                if (Unit* l_Target = GetHitUnit())
                {
                    if (l_Target->GetGUID() == m_ProcTarget)
                        GetCaster()->CastSpell(l_Target, eSpells::ChainHealTriggered, true);
                }

                if (l_FirstTarget->HasAura(eSpells::Riptide))
                {
                    uint32 l_Heal = GetHitHeal();

                    SpellInfo const* l_Riptide = sSpellMgr->GetSpellInfo(eSpells::Riptide);
                    if (l_Riptide == nullptr)
                        return;

                    AddPct(l_Heal, l_Riptide->Effects[EFFECT_2].BasePoints);
                    SetHitHeal(l_Heal);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_chain_heal_SpellScript::CheckTargets, EFFECT_0, TARGET_UNIT_TARGET_CHAINHEAL_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_sha_chain_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_chain_heal_SpellScript;
        }
};

/// Las Update 6.2.3
/// Riptide - 61295
class spell_sha_riptide : public SpellScriptLoader
{
    public:
        spell_sha_riptide() : SpellScriptLoader("spell_sha_riptide") { }

        enum eSpells
        {
            UnleashLife = 73685,
            UnleashFury = 118473
        };

        class spell_sha_riptide_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_riptide_SpellScript);

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_UnleashLife = sSpellMgr->GetSpellInfo(eSpells::UnleashLife);
                SpellInfo const* l_UnleashFurry = sSpellMgr->GetSpellInfo(eSpells::UnleashFury);

                if (l_UnleashLife == nullptr || l_UnleashFurry == nullptr)
                    return;

                int32 l_BonusHealPct = 0;

                if (l_Caster->HasAura(eSpells::UnleashFury))
                    l_BonusHealPct += l_UnleashFurry->Effects[EFFECT_0].BasePoints;

                if (l_Caster->HasAura(eSpells::UnleashLife))
                    l_BonusHealPct += l_UnleashLife->Effects[EFFECT_2].BasePoints;

                SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), l_BonusHealPct));
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::UnleashLife))
                    l_Caster->RemoveAurasDueToSpell(eSpells::UnleashLife);
                if (l_Caster->HasAura(eSpells::UnleashFury))
                    l_Caster->RemoveAurasDueToSpell(eSpells::UnleashFury);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_riptide_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
                AfterHit += SpellHitFn(spell_sha_riptide_SpellScript::HandleAfterHit);
            }
        };

        class spell_sha_riptide_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_riptide_AuraScript);

            void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::UnleashLife);

                if (l_Caster == nullptr || l_SpellInfo == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::UnleashLife))
                {
                    p_Amount += CalculatePct(p_Amount, l_SpellInfo->Effects[EFFECT_2].BasePoints);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_riptide_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_riptide_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_riptide_SpellScript;
        }
};

/// Last Update 6.2.3
/// Maelstrom Weapon - 53817
uint32 g_MaelstromVisualSpellIds[] { 170588, 170587, 170586, 170585, 60349};
class spell_sha_maelstrom_weapon: public SpellScriptLoader
{
    public:
        spell_sha_maelstrom_weapon() : SpellScriptLoader("spell_sha_maelstrom_weapon") { }

        class spell_sha_maelstrom_weapon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_maelstrom_weapon_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                RemoveAllVisuals(l_Caster);
                l_Caster->AddAura(g_MaelstromVisualSpellIds[aurEff->GetBase()->GetStackAmount() - 1], l_Caster);
            }
            
            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                RemoveAllVisuals(l_Caster);
            }

            void RemoveAllVisuals(Unit* l_Caster)
            {
                for (uint32 l_I = 0; l_I < sizeof(g_MaelstromVisualSpellIds) / sizeof(uint32); l_I++)
                    if (Aura* l_Aura = l_Caster->GetAura(g_MaelstromVisualSpellIds[l_I]))
                        l_Aura->Remove();
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                ///< Maelstrom Weapon now increases direct healing done by 5% while in PvP combat (down from a 10% increase to direct healing).
                if ((l_Player->GetMap() && l_Player->GetMap()->IsBattlegroundOrArena()) || l_Player->IsInPvPCombat())
                    p_Amount /= 2;
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sha_maelstrom_weapon_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_sha_maelstrom_weapon_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_maelstrom_weapon_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_maelstrom_weapon_AuraScript();
        }
};

enum CloudburstTotemSpells
{
    SPELL_CLOUDBURST    = 157503,
};

/// Last updated : 6.1.2 19802
/// Cloudburst Totem - 157504
class spell_sha_cloudburst_totem: public SpellScriptLoader
{
    public:
        spell_sha_cloudburst_totem() : SpellScriptLoader("spell_sha_cloudburst_totem") { }

        class spell_sha_cloudburst_totem_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_cloudburst_totem_AuraScript);

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                HealInfo* l_HealInfo = p_EventInfo.GetHealInfo();

                if (!l_HealInfo)
                    return;

                if (SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_CLOUDBURST))
                    GetEffect(p_AurEff->GetEffIndex())->SetAmount(p_AurEff->GetAmount() + CalculatePct(l_HealInfo->GetHeal(), l_SpellInfo->Effects[EFFECT_0].BasePoints));
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /* p_Mode */)
            {
                if (Unit* l_Owner = GetOwner()->ToUnit())
                {
                    if (int32 l_Amount = p_AurEff->GetAmount())
                    {
                        l_Owner->CastCustomSpell(l_Owner, SPELL_CLOUDBURST, &l_Amount, nullptr, nullptr, true);
                        GetEffect(p_AurEff->GetEffIndex())->SetAmount(0);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_cloudburst_totem_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_sha_cloudburst_totem_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_cloudburst_totem_AuraScript();
        }
};

/// Last updated : 6.1.2 19802
/// Clodburst - 157503
class spell_sha_cloudburst: public SpellScriptLoader
{
    public:
        spell_sha_cloudburst() : SpellScriptLoader("spell_sha_cloudburst") { }

        class spell_sha_cloudburst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_cloudburst_SpellScript);

            bool Load()
            {
                l_TargetCount = 0;
                return true;
            }

            void HandleHeal(SpellEffIndex p_EffIndex)
            {
                if (l_TargetCount)
                    SetHitHeal(GetHitHeal() / l_TargetCount);
            }

            void CountTargets(std::list<WorldObject*>& p_Targets)
            {
                l_TargetCount = p_Targets.size();
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_cloudburst_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_sha_cloudburst_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }

            uint8 l_TargetCount;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_cloudburst_SpellScript;
        }
};

/// last update : 6.1.2 19802
/// Call Lightning - 157348, Wind Gust - 157331
class spell_sha_call_lightning : public SpellScriptLoader
{
    public:
        spell_sha_call_lightning() : SpellScriptLoader("spell_sha_call_lightning") { }

        class spell_sha_call_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_call_lightning_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Owner = l_Caster->GetOwner();
                Unit* l_Target = GetHitUnit();

                if (l_Owner == nullptr || l_Target == nullptr)
                    return;

                if (l_Owner->GetTypeId() != TYPEID_PLAYER)
                    return;

                int32 l_Damage = int32(l_Owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * GetSpellInfo()->Effects[EFFECT_0].BonusMultiplier);
                l_Damage = l_Caster->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                l_Damage = l_Target->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                SetHitDamage(l_Damage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_call_lightning_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_call_lightning_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Soothing Winds - 157333
class spell_sha_soothing_wind : public SpellScriptLoader
{
    public:
        spell_sha_soothing_wind() : SpellScriptLoader("spell_sha_soothing_wind") { }

        class spell_sha_soothing_wind_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_soothing_wind_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Owner = l_Caster->GetOwner();

                if (l_Owner == nullptr)
                    return;

                if (l_Owner->GetTypeId() != TYPEID_PLAYER)
                    return;

                int32 l_Heal = int32(l_Owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * GetSpellInfo()->Effects[EFFECT_0].BonusMultiplier);

                SetHitHeal(l_Heal);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_soothing_wind_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_soothing_wind_SpellScript();
        }
};

/// WoD PvP Enhancement 2P Bonus - 166103
class spell_sha_WoDPvPEnhancement2PBonus : public SpellScriptLoader
{
    public:
        spell_sha_WoDPvPEnhancement2PBonus() : SpellScriptLoader("spell_sha_WoDPvPEnhancement2PBonus") { }

        class spell_sha_WoDPvPEnhancement2PBonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_WoDPvPEnhancement2PBonus_AuraScript);

            void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_WoDPvPEnhancement2PBonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_WoDPvPEnhancement2PBonus_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Nature's Guardian - 30884
class spell_sha_natures_guardian : public SpellScriptLoader
{
    public:
        spell_sha_natures_guardian() : SpellScriptLoader("spell_sha_natures_guardian") { }

        class spell_sha_natures_guardian_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_natures_guardian_AuraScript);

            enum eSpells
            {
                NaturesGuardian = 31616
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                if (!(p_EventInfo.GetDamageInfo()->GetDamage()))
                    return;

                if (l_Player->HasSpellCooldown(eSpells::NaturesGuardian))
                    return;

                if ((int32)l_Player->GetHealthPct() > GetSpellInfo()->Effects[EFFECT_1].BasePoints &&
                    (int32)(100.f * (l_Player->GetHealth() - p_EventInfo.GetDamageInfo()->GetDamage()) / l_Player->GetMaxHealth()) <= GetSpellInfo()->Effects[EFFECT_1].BasePoints)
                {
                    l_Player->CastSpell(l_Player, eSpells::NaturesGuardian, true);
                    l_Player->AddSpellCooldown(eSpells::NaturesGuardian, 0, 30 * IN_MILLISECONDS);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_natures_guardian_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_natures_guardian_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Call by Earth Shield - 974
/// Item - Shaman WoD PvP Restoration 4P Bonus - 166063
class spell_sha_pvp_restoration_4p_bonus : public SpellScriptLoader
{
    public:
        spell_sha_pvp_restoration_4p_bonus() : SpellScriptLoader("spell_sha_pvp_restoration_4p_bonus") { }

        class spell_sha_pvp_restoration_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_pvp_restoration_4p_bonus_AuraScript);

            enum eSpells
            {
                ItemWodPvpRestoration4PBonus = 166063,
                EarthShieldProc = 379
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Caster == nullptr || l_Player == nullptr || !l_Caster->HasAura(eSpells::ItemWodPvpRestoration4PBonus))
                    return;

                float l_HealthPct = 0.0f;
                Aura* l_AuraSetBonus = l_Caster->GetAura(eSpells::ItemWodPvpRestoration4PBonus);
                if (AuraEffect* l_AuraEffectHealthPct = l_AuraSetBonus->GetEffect(EFFECT_0))
                    l_HealthPct = (float)l_AuraEffectHealthPct->GetAmount();

                if (p_EventInfo.GetDamageInfo() == nullptr || l_AuraSetBonus == nullptr)
                    return;

                if (l_Player->HasSpellCooldown(eSpells::EarthShieldProc))
                    return;

                Unit* l_Target = GetTarget();
                ///< Should proc only when the target pass from > 50% health to < 50% health
                if (l_Target->GetHealthPct() > l_HealthPct && (100.f * (float)(l_Target->GetHealth() - (float)p_EventInfo.GetDamageInfo()->GetDamage()) / l_Target->GetMaxHealth()) <= l_HealthPct)
                {
                    if (AuraEffect* l_AuraEffectNbrProc = l_AuraSetBonus->GetEffect(EFFECT_1))
                    {
                        /// If we have less than 4 stacks, to prevent crash
                        if (p_AurEff->GetBase() && (p_AurEff->GetBase()->GetCharges() < l_AuraEffectNbrProc->GetAmount()))
                        {
                            for (int8 i = 0; i < p_AurEff->GetBase()->GetCharges(); ++i)
                            {
                                l_Caster->CastSpell(l_Target, eSpells::EarthShieldProc, true);
                                p_AurEff->GetBase()->DropCharge();
                            }
                        }
                        else
                        {
                            for (int8 i = 0; i < l_AuraEffectNbrProc->GetAmount(); ++i)
                            {
                                l_Caster->CastSpell(l_Target, eSpells::EarthShieldProc, true);
                                p_AurEff->GetBase()->DropCharge();
                            }
                        }
                    }
                    
                    /// To prevent multiprocs, blizzlike cooldown 3 seconds
                    l_Player->AddSpellCooldown(eSpells::EarthShieldProc, 0, 3 * IN_MILLISECONDS);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_pvp_restoration_4p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_pvp_restoration_4p_bonus_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Spiritwalker's Grace - 79206
class spell_sha_spiritwalkers_grace : public SpellScriptLoader
{
    public:
        spell_sha_spiritwalkers_grace() : SpellScriptLoader("spell_sha_spiritwalkers_grace") { }

        class spell_sha_spiritwalkers_grace_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_spiritwalkers_grace_SpellScript);

            enum eSpells
            {
                GlyphOfSpiritwalkersGraceAura = 159651,
                GlyphOfSpiritwalkersGrace = 159652
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::GlyphOfSpiritwalkersGraceAura))
                    l_Caster->CastSpell(l_Caster, eSpells::GlyphOfSpiritwalkersGrace, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_spiritwalkers_grace_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_spiritwalkers_grace_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Eye of the Storm - 157382
class spell_sha_eye_of_the_storm : public SpellScriptLoader
{
    public:
        spell_sha_eye_of_the_storm() : SpellScriptLoader("spell_sha_eye_of_the_storm") { }

        class spell_sha_eye_of_the_storm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_eye_of_the_storm_AuraScript);

            enum eSpells
            {
                EyeOfTheStormSpeedEffect = 157384
            };

            std::list<uint64> m_TargetList;

            void OnUpdate(uint32 /*p_Diff*/, AuraEffect* /*p_AurEff*/)
            {
                Unit* l_Caster = GetCaster();
                float l_Radius = 6.0f;

                std::list<Unit*> l_AlliesList;
                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_AlliesList, l_Check);
                l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                for (auto l_Target : l_AlliesList)
                {
                    if (l_Caster->IsValidAssistTarget(l_Target) && std::find(m_TargetList.begin(), m_TargetList.end(), l_Target->GetGUID()) == m_TargetList.end())
                    {
                        l_Target->CastSpell(l_Target, eSpells::EyeOfTheStormSpeedEffect, true);
                        m_TargetList.push_back(l_Target->GetGUID());
                    }
                }

                for (auto l_It = m_TargetList.begin(); l_It != m_TargetList.end();)
                {
                    Unit* l_Target = ObjectAccessor::FindUnit(*l_It);
                    if (!l_Target || (std::find(l_AlliesList.begin(), l_AlliesList.end(), l_Target) == l_AlliesList.end()))
                    {
                        if (l_Target)
                            l_Target->RemoveAura(eSpells::EyeOfTheStormSpeedEffect);

                        l_It = m_TargetList.erase(l_It);
                    }
                    else
                        ++l_It;
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                for (uint64 l_TargetGUID : m_TargetList)
                {
                    Unit* l_Target = ObjectAccessor::FindUnit(l_TargetGUID);
                    if (l_Target)
                        l_Target->RemoveAura(eSpells::EyeOfTheStormSpeedEffect);
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_sha_eye_of_the_storm_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_AREATRIGGER);
                OnEffectRemove += AuraEffectRemoveFn(spell_sha_eye_of_the_storm_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_AREATRIGGER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_eye_of_the_storm_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Glyph of Flame Shock - 55447
class spell_sha_glyph_of_flame_shock : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_flame_shock() : SpellScriptLoader("spell_sha_glyph_of_flame_shock") { }

        class spell_sha_glyph_of_flame_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_glyph_of_flame_shock_AuraScript);

            enum eSpells
            {
                FlameShock = 8050
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != eSpells::FlameShock)
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetDamage())
                    return;

                if (l_Caster->IsValidAssistTarget(l_Caster))
                    l_Caster->HealBySpell(l_Caster, p_EventInfo.GetDamageInfo()->GetSpellInfo(), CalculatePct(p_EventInfo.GetDamageInfo()->GetDamage(), p_AurEff->GetAmount()), false);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_flame_shock_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_glyph_of_flame_shock_AuraScript();
        }
};

/// Glyph of Rain of Frogs - 147707
class spell_sha_glyph_of_rain_of_frogs : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_rain_of_frogs() : SpellScriptLoader("spell_sha_glyph_of_rain_of_frogs") { }

    class spell_sha_glyph_of_rain_of_frogs_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_glyph_of_rain_of_frogs_AuraScript);

        enum eSpells
        {
            GlyphOfRaingOfFrogsSpell = 147709
        };

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Player* l_Player = GetTarget()->ToPlayer())
                l_Player->learnSpell(eSpells::GlyphOfRaingOfFrogsSpell, false);
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Player* l_Player = GetTarget()->ToPlayer())
                if (l_Player->HasSpell(eSpells::GlyphOfRaingOfFrogsSpell))
                    l_Player->removeSpell(eSpells::GlyphOfRaingOfFrogsSpell, false, false);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_sha_glyph_of_rain_of_frogs_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_sha_glyph_of_rain_of_frogs_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_sha_glyph_of_rain_of_frogs_AuraScript();
    }
};

/// Last Update 6.2.3
/// Stormstrike - 17364, Windstrike - 115356
class spell_sha_stormstrike_windstrike : public SpellScriptLoader
{
    public:
        spell_sha_stormstrike_windstrike() : SpellScriptLoader("spell_sha_stormstrike_windstrike") { }

        class spell_sha_stormstrike_windstrike_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_stormstrike_windstrike_AuraScript);

            enum eSpells
            {
                Stormstrike = 17364,
                Windstrike  = 115356
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (GetSpellInfo()->Id == eSpells::Stormstrike && l_Target->HasAura(eSpells::Windstrike, l_Caster->GetGUID()))
                    l_Target->RemoveAura(eSpells::Windstrike, l_Caster->GetGUID());
                else if (GetSpellInfo()->Id == eSpells::Windstrike && l_Target->HasAura(eSpells::Stormstrike, l_Caster->GetGUID()))
                    l_Target->RemoveAura(eSpells::Stormstrike, l_Caster->GetGUID());
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sha_stormstrike_windstrike_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_CRIT_CHANCE_FOR_CASTER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_stormstrike_windstrike_AuraScript();
        }
};

void AddSC_shaman_spell_scripts()
{
    new spell_sha_stormstrike_windstrike();
    new spell_sha_glyph_of_ascendance();
    new spell_sha_ancestral_guidance_heal();
    new spell_sha_glyph_of_flame_shock();
    new spell_sha_eye_of_the_storm();
    new spell_sha_spiritwalkers_grace();
    new spell_sha_pvp_restoration_4p_bonus();
    new spell_sha_natures_guardian();
    new spell_sha_unleashed_fury();
    new spell_sha_high_tide();
    new spell_sha_tidal_waves();
    new spell_sha_totemic_projection();
    new spell_sha_ascendance_flame();
    new spell_sha_ascendance_air();
    new spell_sha_ascendance_water();
    new spell_sha_ascendance_water_heal();
    new spell_sha_glyph_of_shamanistic_rage();
    new spell_sha_glyph_of_lakestrider();
    new spell_sha_call_of_the_elements();
    new spell_sha_conductivity();
    new spell_sha_ancestral_guidance();
    new spell_sha_earthgrab();
    new spell_sha_stone_bulwark();
    new spell_sha_frozen_power();
    new spell_sha_spirit_link();
    new spell_sha_fire_nova();
    new spell_sha_fulmination();
    new spell_sha_lava_surge();
    new spell_sha_healing_stream();
    new spell_sha_elemental_blast();
    new spell_sha_earthquake_tick();
    new spell_sha_earthquake();
    new spell_sha_healing_rain();
    new spell_sha_healing_rain_heal();
    new spell_sha_bloodlust();
    new spell_sha_heroism();
    new spell_sha_lava_lash_spread();
    new spell_sha_windfury();
    new spell_sha_improoved_flame_shock();
    new spell_sha_feral_spirit();
    new spell_sha_pet_spirit_hunt();
    new spell_sha_fulmination_proc();
    new spell_sha_enhanced_chain_lightning();
    new spell_sha_echo_of_elements();
    new spell_sha_lava_lash();
    new spell_sha_liquid_magma();
    new spell_sha_liquid_magma_visual();
    new spell_sha_ghost_wolf();
    new spell_sha_lava_burst();
    new spell_sha_chain_heal();
    new spell_sha_glyph_of_eternal_earth();
    new spell_sha_flame_shock();
    new spell_sha_healing_wave();
    new spell_sha_riptide();
    new spell_sha_maelstrom_weapon();
    new spell_sha_cloudburst_totem();
    new spell_sha_cloudburst();
    new spell_sha_call_lightning();
    new spell_sha_soothing_wind();
    new spell_sha_WoDPvPEnhancement2PBonus();
    new spell_sha_improved_chain_heal();
    new spell_sha_glyph_of_rain_of_frogs();
}
