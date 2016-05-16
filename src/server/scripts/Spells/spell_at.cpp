////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

/// Areatrigger defile - 152280
class spell_at_dk_defile : public AreaTriggerEntityScript
{
    public:
        spell_at_dk_defile() : AreaTriggerEntityScript("spell_areatrigger_dk_defile") { }

        enum eDefilebSpell
        {
            NpcDefileVisual     = 82521,
            SpellDefileDamage   = 156000,
            SpellDefile         = 152280,
            TimerDefile         = 1 * IN_MILLISECONDS
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            if (TimerDefile <= m_CurrentTimerDefile)
            {
                if (Unit* l_Caster = p_AreaTrigger->GetCaster())
                {
                    std::list<Unit*> l_TargetListTemp;
                    float l_Radius = p_AreaTrigger->GetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE) * 8.0f;

                    JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                    JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetListTemp, l_Check);
                    p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                    if (!l_TargetListTemp.empty())
                    {
                        std::list<Unit*> l_TargetList;
                        /// Remove unatackable target
                        for (Unit* l_Unit : l_TargetListTemp)
                        {
                            if (l_Caster->IsValidAttackTarget(l_Unit))
                                l_TargetList.push_back(l_Unit);
                        }

                        if (!l_TargetList.empty())
                        {
                            /// Update size
                            if (SpellInfo const* l_Defile = sSpellMgr->GetSpellInfo(eDefilebSpell::SpellDefile))
                            {
                                float l_MultiplicatorVisual = 1.0f + float(l_Defile->Effects[EFFECT_1].BasePoints) / 100 / 100;

                                /// Cast damage
                                for (Unit* l_Unit : l_TargetList)
                                {
                                    /// Update damage
                                    if (SpellInfo const* l_DefileDamage = sSpellMgr->GetSpellInfo(eDefilebSpell::SpellDefileDamage))
                                    {
                                        int32 l_BasePoints = l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * float((l_DefileDamage->Effects[EFFECT_0].BasePoints + m_StackDefile * float(l_Defile->Effects[EFFECT_1].BasePoints) / 100) / 100);

                                        l_Caster->CastCustomSpell(l_Unit, eDefilebSpell::SpellDefileDamage, &l_BasePoints, nullptr, nullptr, true);
                                    }
                                }

                                /// Update size
                                uint64 l_CreatureVisualGUID = p_AreaTrigger->GetGUIDCreatureVisual();
                                if (l_CreatureVisualGUID != 0)
                                {
                                    if (Creature* l_CreatureVisual = p_AreaTrigger->GetMap()->GetCreature(l_CreatureVisualGUID))
                                    {
                                        l_CreatureVisual->SetObjectScale(l_CreatureVisual->GetFloatValue(OBJECT_FIELD_SCALE) * l_MultiplicatorVisual);
                                        p_AreaTrigger->SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, p_AreaTrigger->GetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE) * l_MultiplicatorVisual);
                                    }
                                }

                                m_StackDefile++;
                            }
                        }
                    }
                }
                m_CurrentTimerDefile = 0;
            }
            else
                m_CurrentTimerDefile += p_Time;
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_dk_defile();
        }

    private:
        uint32 m_CurrentTimerDefile = 1000;
        uint8 m_StackDefile = 0;
};

/// Fungal Growth - 164717
class spell_at_druid_fungal_growth : public AreaTriggerEntityScript
{
    public:
        spell_at_druid_fungal_growth() : AreaTriggerEntityScript("at_fungal_growth") { }

        enum eWildMushroomSpells
        {
            FungalCloud         = 81281,
            WildMushroomBalance = 88747,
        };

        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            auto l_SpellInfo = sSpellMgr->GetSpellInfo(eWildMushroomSpells::WildMushroomBalance);
            auto l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster == nullptr || l_SpellInfo == nullptr)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = (float)l_SpellInfo->Effects[EFFECT_0].BasePoints;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_AreaTriggerCaster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Target : l_TargetList)
            {
                if (l_Target == nullptr)
                    return;

                if (!l_Target->HasAura(eWildMushroomSpells::FungalCloud) && l_AreaTriggerCaster->IsValidAttackTarget(l_Target))
                {
                    l_AreaTriggerCaster->CastSpell(l_Target, eWildMushroomSpells::FungalCloud, true);
                    m_Targets.push_back(l_Target->GetGUID());
                }
            }

            std::list<uint64> l_Targets(m_Targets);
            for (uint64 l_TargetGuid : l_Targets)
            {
                Unit* l_Target = ObjectAccessor::GetUnit(*l_AreaTriggerCaster, l_TargetGuid);

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(eWildMushroomSpells::FungalCloud, l_AreaTriggerCaster->GetGUID()) && l_Target->GetDistance(l_AreaTriggerCaster) <= l_Radius)
                    return;

                if (l_Target->HasAura(eWildMushroomSpells::FungalCloud, l_AreaTriggerCaster->GetGUID()))
                    l_Target->RemoveAurasDueToSpell(eWildMushroomSpells::FungalCloud, l_AreaTriggerCaster->GetGUID());

                m_Targets.remove(l_TargetGuid);
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            auto l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster == nullptr)
                return;

            std::list<uint64> l_Targets = m_Targets;
            for (uint64 l_TargetGuid : l_Targets)
            {
                Unit* l_Target = ObjectAccessor::GetUnit(*l_AreaTriggerCaster, l_TargetGuid);

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(eWildMushroomSpells::FungalCloud, l_AreaTriggerCaster->GetGUID()))
                    l_Target->RemoveAurasDueToSpell(eWildMushroomSpells::FungalCloud, l_AreaTriggerCaster->GetGUID());

                m_Targets.remove(l_TargetGuid);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_druid_fungal_growth();
        }
};

/// last update : 6.1.2 19802
/// Ursol Vortex - 102793
class spell_at_druid_ursol_vortex : public AreaTriggerEntityScript
{
    public:
        spell_at_druid_ursol_vortex(): AreaTriggerEntityScript("at_ursol_vortex") { }

        enum eSpells
        {
            VortexJump = 118283,
            VortexSnare = 127797
        };

        std::list<uint64> m_TargetList;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_NewTargetList;
            float l_Radius = 3.5f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_NewTargetList, u_check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

            for (auto l_It = m_TargetList.begin(); l_It != m_TargetList.end();)
            {
                Unit* l_Target = ObjectAccessor::GetUnit(*l_Caster, *l_It);

                if (l_Target == nullptr)
                {
                    l_It = m_TargetList.erase(l_It);
                    continue;
                }

                if (std::find(l_NewTargetList.begin(), l_NewTargetList.end(), l_Target) == l_NewTargetList.end())
                {
                    const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::VortexJump);

                    if (!l_Target->HasAura(eSpells::VortexJump, l_Target->GetGUID()) && !l_Target->IsImmunedToSpellEffect(l_SpellInfo, EFFECT_0))
                        l_Target->CastSpell(p_AreaTrigger, eSpells::VortexJump, true);
                    else
                    {
                        l_Target->RemoveAura(eSpells::VortexSnare);
                        l_It = m_TargetList.erase(l_It);
                        continue;
                    }
                }
                ++l_It;
            }

            if (!l_NewTargetList.empty())
            {
                for (auto itr : l_NewTargetList)
                {
                    if (!itr->HasAura(eSpells::VortexSnare))
                        l_Caster->CastSpell(itr, eSpells::VortexSnare, true);
                    if (std::find(m_TargetList.begin(), m_TargetList.end(), itr->GetGUID()) == m_TargetList.end())
                        m_TargetList.push_back(itr->GetGUID());
                }
            }
        }

        void OnRemove(AreaTrigger* /*p_AreaTrigger*/, uint32 /*p_Time*/)
        {
            for (auto l_It : m_TargetList)
            {
                Unit* l_Target = ObjectAccessor::FindUnit(l_It);
                if (l_Target)
                {
                    l_Target->RemoveAura(eSpells::VortexSnare);
                    l_Target->RemoveAura(eSpells::VortexJump);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_druid_ursol_vortex();
        }
};

/// last update : 6.1.2 19802
/// Solar Beam - 78675
class spell_at_druid_solar_beam : public AreaTriggerEntityScript
{
    public:
        spell_at_druid_solar_beam() : AreaTriggerEntityScript("spell_at_druid_solar_beam") { }

        std::list<uint64> m_TargetList;

        enum eSpells
        {
            solarBeamSilence = 81261
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            float l_Radius = 5.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster == nullptr)
                return;

            std::list<Unit*> l_NewTargetList;
            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_NewTargetList, u_check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

            for (Unit* l_Target : l_NewTargetList)
            {
                if (l_Caster->IsValidAttackTarget(l_Target) && std::find(m_TargetList.begin(), m_TargetList.end(), l_Target->GetGUID()) == m_TargetList.end())
                {
                    m_TargetList.push_back(l_Target->GetGUID());
                    l_Caster->CastSpell(l_Target, eSpells::solarBeamSilence, true);
                    if (Aura* l_SolarBeamSilence = l_Caster->GetAura(eSpells::solarBeamSilence))
                        l_SolarBeamSilence->SetDuration(p_AreaTrigger->GetDuration());
                }
            }

            for (auto l_It = m_TargetList.begin(); l_It != m_TargetList.end(); )
            {
                Unit* l_Target = ObjectAccessor::FindUnit(*l_It);
                if (!l_Target || (std::find(l_NewTargetList.begin(), l_NewTargetList.end(), l_Target) == l_NewTargetList.end()))
                {
                    if (l_Target)
                        l_Target->RemoveAura(eSpells::solarBeamSilence);

                    l_It = m_TargetList.erase(l_It);
                }
                else
                {
                    if (!l_Target->HasAura(eSpells::solarBeamSilence, l_Caster->GetGUID()))
                    {
                        l_Caster->CastSpell(l_Target, eSpells::solarBeamSilence, true);
                        if (Aura* l_SolarBeamSilence = l_Caster->GetAura(eSpells::solarBeamSilence))
                            l_SolarBeamSilence->SetDuration(p_AreaTrigger->GetDuration());
                    }
                    ++l_It;
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (uint64 l_TargetGUID : m_TargetList)
            {
                Unit* l_Target = ObjectAccessor::FindUnit(l_TargetGUID);
                if (l_Target)
                    l_Target->RemoveAura(eSpells::solarBeamSilence);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_druid_solar_beam();
        }
};

/// Binding Shot - 109248
class spell_at_hun_binding_shot : public AreaTriggerEntityScript
{
    public:
        spell_at_hun_binding_shot() : AreaTriggerEntityScript("spell_hun_binding_shot_areatrigger") { }

        enum eSpells
        {
            BindingShotLink         = 117405,
            BindingShotImmune       = 117553,
            BindingShotVisualLink   = 117614
        };

        uint32 m_LinkVisualTimer = 1000;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = sSpellMgr->GetSpellInfo(p_AreaTrigger->GetSpellId())->Effects[EFFECT_1].CalcRadius(l_Caster);

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                if (l_TargetList.empty())
                    return;

                l_TargetList.remove_if([this, l_Caster](Unit* p_Unit) -> bool
                                       {
                                           if (p_Unit == nullptr || !l_Caster->IsValidAttackTarget(p_Unit))
                                               return true;

                                           if (p_Unit->HasAura(eSpells::BindingShotImmune))
                                               return true;

                                           return false;
                                       });

                for (Unit* l_Target : l_TargetList)
                {
                    if (!l_Target->HasAura(eSpells::BindingShotLink))
                        l_Caster->CastSpell(l_Target, eSpells::BindingShotLink, true);
                }

                if (m_LinkVisualTimer <= p_Time)
                {
                    m_LinkVisualTimer = 1000;

                    for (Unit* l_Target : l_TargetList)
                        l_Target->CastSpell(p_AreaTrigger->m_positionX, p_AreaTrigger->m_positionY, p_AreaTrigger->m_positionZ, eSpells::BindingShotVisualLink, true);
                }
                else
                    m_LinkVisualTimer -= p_Time;
            }
        }
        
        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_hun_binding_shot();
        }
};

/// Last Update 6.2.3
/// Ice Trap - 13809
/// Ice Trap (Frost - Trap Launcher) - 82940
class spell_at_hun_ice_trap : public AreaTriggerEntityScript
{
    public:
        spell_at_hun_ice_trap() : AreaTriggerEntityScript("at_ice_trap") { }

        enum eSpells
        {
            SpellIceTrapEffect  = 13810,
            SpellEntrapment     = 19387,
            SpellEntrapmentRoot = 64803
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            SpellInfo const* l_CreateSpell = sSpellMgr->GetSpellInfo(p_AreaTrigger->GetSpellId());
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster && l_CreateSpell)
            {
                float l_Radius = MELEE_RANGE * 2;

                std::list<Unit*> l_NewTargetList;
                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_NewTargetList, u_check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

                l_NewTargetList.sort(JadeCore::DistanceCompareOrderPred(p_AreaTrigger));
                for (Unit* l_Target : l_NewTargetList)
                {
                    if (l_Target != nullptr && l_Caster->IsValidAttackTarget(l_Target) && !l_Target->isTotem() && !l_Target->isStatue() && p_AreaTrigger->IsInWorld())
                    {
                        l_Target->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
                        l_Caster->CastSpell(p_AreaTrigger->GetPositionX(), p_AreaTrigger->GetPositionY(), p_AreaTrigger->GetPositionZ(), eSpells::SpellIceTrapEffect, true);

                        if (l_Caster->HasAura(eSpells::SpellEntrapment)) ///< Entrapment
                            l_Caster->CastSpell(p_AreaTrigger->GetPositionX(), p_AreaTrigger->GetPositionY(), p_AreaTrigger->GetPositionZ(), eSpells::SpellEntrapmentRoot, true);

                        p_AreaTrigger->Remove(0);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_hun_ice_trap();
        }
};

/// Last Update 6.2.3
/// Snake Trap - 34600
/// Snake Trap (Fire - Trap Launcher) - 82949
class spell_at_hun_snake_trap : public AreaTriggerEntityScript
{
    public:
        spell_at_hun_snake_trap() : AreaTriggerEntityScript("at_snake_trap") { }

        enum eSpells
        {
            SummonSnakes        = 57879,
            SpellEntrapment     = 19387,
            SpellEntrapmentRoot = 64803
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            SpellInfo const* l_CreateSpell = sSpellMgr->GetSpellInfo(p_AreaTrigger->GetSpellId());
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster && l_CreateSpell)
            {
                float l_Radius = MELEE_RANGE * 2;

                std::list<Unit*> l_NewTargetList;
                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_NewTargetList, u_check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

                l_NewTargetList.sort(JadeCore::DistanceCompareOrderPred(p_AreaTrigger));
                for (Unit* l_Target : l_NewTargetList)
                {
                    if (l_Target != nullptr && l_Caster->IsValidAttackTarget(l_Target) && !l_Target->isTotem() && !l_Target->isStatue() && p_AreaTrigger->IsInWorld())
                    {
                        l_Target->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
                        l_Caster->CastSpell(l_Target, eSpells::SummonSnakes, true);

                        if (l_Caster->HasAura(eSpells::SpellEntrapment)) ///< Entrapment
                            l_Caster->CastSpell(p_AreaTrigger->GetPositionX(), p_AreaTrigger->GetPositionY(), p_AreaTrigger->GetPositionZ(), eSpells::SpellEntrapmentRoot, true);

                        p_AreaTrigger->Remove(0);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_hun_snake_trap();
        }
};

/// Last Update 6.2.3
/// Ice trap effect - 13810
class spell_at_hun_ice_trap_effect : public AreaTriggerEntityScript
{
    public:
        spell_at_hun_ice_trap_effect() : AreaTriggerEntityScript("at_ice_trap_effect") { }

        enum eSpells
        {
            GlyphOfBlackIce = 109263,
            BlackIceEffect = 83559,
            IceTrapEffect = 135299
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            std::list<Unit*> targetList;
            float l_Radius = 10.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(p_AreaTrigger, targetList, u_check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

            for (auto itr : targetList)
            {
                if (itr != nullptr && l_Caster->IsValidAttackTarget(itr) && !itr->isTotem() && !itr->HasAura(eSpells::IceTrapEffect) && !itr->isStatue())
                    itr->CastSpell(itr, IceTrapEffect, true);
            }

            // Glyph of Black Ice
            if (l_Caster->GetDistance(p_AreaTrigger) <= l_Radius && l_Caster->HasAura(GlyphOfBlackIce) && !l_Caster->HasAura(BlackIceEffect))
                l_Caster->CastSpell(l_Caster, BlackIceEffect, true);
            if (l_Caster->GetDistance(p_AreaTrigger) > l_Radius || !l_Caster->HasAura(GlyphOfBlackIce))
                l_Caster->RemoveAura(BlackIceEffect);
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster == nullptr)
                return;

            if (l_Caster->HasAura(BlackIceEffect))
                l_Caster->RemoveAura(BlackIceEffect);
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_hun_ice_trap_effect();
        }
};

/// Last Update 6.2.3
/// Freezing Trap - 1499
/// Freezing Trap (Frost - Trap Launcher) - 60202
class spell_at_hun_freezing_trap : public AreaTriggerEntityScript
{
    public:
        spell_at_hun_freezing_trap() : AreaTriggerEntityScript("at_freezing_trap") { }

        enum class HunterFreezingTrap : uint32
        {
            SpellIncapacitate         = 3355,
            SpellGlyphOfSolace        = 119407,
            HunterWodPvp2PBonus       = 166005,
            HunterWodPvp2PBonusEffect = 166009
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            auto l_CreateSpell       = sSpellMgr->GetSpellInfo(p_AreaTrigger->GetSpellId());
            auto l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster && l_CreateSpell)
            {
                float l_Radius = MELEE_RANGE * 2;

                std::list<Unit*> l_NewTargetList;
                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_AreaTriggerCaster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_NewTargetList, u_check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

                l_NewTargetList.sort(JadeCore::DistanceCompareOrderPred(p_AreaTrigger));
                for (Unit* l_Target : l_NewTargetList)
                {
                    if (l_Target != nullptr && l_AreaTriggerCaster->IsValidAttackTarget(l_Target) && !l_Target->isTotem() && !l_Target->isStatue())
                    {
                        uint32 l_MovementFlag = l_Target->GetUnitMovementFlags();
                        if (l_AreaTriggerCaster->HasAura((uint32)HunterFreezingTrap::SpellGlyphOfSolace)) ///< Your Freezing Trap also removes all damage over time effects from its target.
                        {
                            l_Target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                            l_Target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
                            l_Target->RemoveAurasByType(SPELL_AURA_PERIODIC_LEECH);
                        }
                        l_AreaTriggerCaster->CastSpell(l_Target, (uint32)HunterFreezingTrap::SpellIncapacitate, true);
                        p_AreaTrigger->Remove(0);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_hun_freezing_trap();
        }
};

/// Last Update 6.2.3
/// Explosive Trap - 13813
/// Explosive Trap (Fire - Trap Launcher) - 82938
class spell_at_hun_explosive_trap : public AreaTriggerEntityScript
{
    public:
        spell_at_hun_explosive_trap() : AreaTriggerEntityScript("at_explosive_trap") { }

        enum HunterExplosiveTrap
        {
            SpellExplosiveEffect = 13812
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            auto l_CreateSpell = sSpellMgr->GetSpellInfo(p_AreaTrigger->GetSpellId());
            auto l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster && l_CreateSpell)
            {
                float l_Radius = MELEE_RANGE * 2;

                std::list<Unit*> l_NewTargetList;
                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_AreaTriggerCaster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_NewTargetList, u_check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

                l_NewTargetList.sort(JadeCore::DistanceCompareOrderPred(p_AreaTrigger));
                for (Unit* l_Target : l_NewTargetList)
                {
                    if (l_Target != nullptr && l_AreaTriggerCaster->IsValidAttackTarget(l_Target) && !l_Target->isTotem() && !l_Target->isStatue())
                    {
                        l_AreaTriggerCaster->CastSpell(p_AreaTrigger->GetPositionX(), p_AreaTrigger->GetPositionY(), p_AreaTrigger->GetPositionZ(), HunterExplosiveTrap::SpellExplosiveEffect, true);
                        p_AreaTrigger->Remove(0);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_hun_explosive_trap();
        }
};

/// Item - Mage WoD PvP Frost 2P Bonus - 180723
class spell_at_mage_wod_frost_2p_bonus : public AreaTriggerEntityScript
{
    public:
        spell_at_mage_wod_frost_2p_bonus() : AreaTriggerEntityScript("at_mage_wod_frost_2p_bonus") { }

        enum eSpells
        {
            SlickIce = 180724
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
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

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_mage_wod_frost_2p_bonus();
        }
};

/// Last Update 6.2.3
/// Arcane Orb - 153626
class spell_at_mage_arcane_orb : public AreaTriggerEntityScript
{
    public:
        spell_at_mage_arcane_orb() : AreaTriggerEntityScript("spell_areatrigger_arcane_orb") { }

        enum eArcaneOrbSpell
        {
            ArcaneChrage = 36032,
            ArcaneOrbDamage = 153640
        };

        enum eDatas
        {
            DamageDelay = 1 * IN_MILLISECONDS ///< Delay between damage cast (and self-snare check)
        };

        int32 m_Delay = 700;

        void OnCreate(AreaTrigger* p_AreaTrigger)
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
                l_Caster->CastSpell(l_Caster, eArcaneOrbSpell::ArcaneChrage, true);
        }

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination)
        {
            Position l_Position;
            float l_Dist = 40.f;

            l_Position.m_positionX = p_SourcePosition.m_positionX + (l_Dist * cos(p_Caster->GetOrientation()));
            l_Position.m_positionY = p_SourcePosition.m_positionY + (l_Dist * sin(p_Caster->GetOrientation()));
            l_Position.m_positionZ = p_SourcePosition.m_positionZ;

            p_PathToLinearDestination.push_back(l_Position);
            p_DestinationPosition = l_Position;
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            m_Delay += p_Time;
            if (eDatas::DamageDelay < m_Delay)
            {
                if (Unit* l_Caster = p_AreaTrigger->GetCaster())
                {
                    std::list<Unit*> l_TargetList;
                    float l_Radius = 7.0f;

                    JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                    JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                    p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                    for (Unit* l_Unit : l_TargetList)
                    {
                        if (l_Caster->IsValidAttackTarget(l_Unit))
                            l_Caster->CastSpell(l_Unit, eArcaneOrbSpell::ArcaneOrbDamage, true);
                    }
                }
                m_Delay -= eDatas::DamageDelay;
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_mage_arcane_orb();
        }
};

/// Rune of Power - 116011
class spell_at_mage_rune_of_power : public AreaTriggerEntityScript
{
    public:
        spell_at_mage_rune_of_power() : AreaTriggerEntityScript("at_rune_of_power") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            std::list<Unit*> targetList;
            float l_Radius = 5.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster->IsWithinDistInMap(p_AreaTrigger, 5.0f))
            {
                if (!l_Caster->HasAura(116014))
                    l_Caster->CastSpell(l_Caster, 116014, true);
                else if (Aura* runeOfPower = l_Caster->GetAura(116014))
                    runeOfPower->RefreshDuration();

                if (l_Caster->ToPlayer())
                    l_Caster->ToPlayer()->UpdateManaRegen();
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32(p_time))
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (l_Caster && l_Caster->HasAura(116014))
                l_Caster->RemoveAura(116014);
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_mage_rune_of_power();
        }
};

/// Meteor Burn - 175396
class spell_at_mage_meteor_burn : public AreaTriggerEntityScript
{
    public:
        spell_at_mage_meteor_burn() : AreaTriggerEntityScript("spell_areatrigger_meteor_burn") { }

        enum eMeteorSpell
        {
            MeteorDoT = 155158,
            VisualID  = 45326
        };

        void OnCreate(AreaTrigger* p_AreaTrigger)
        {
            /// VisualID of 175396 is not the same of his AreaTrigger
            p_AreaTrigger->SetUInt32Value(EAreaTriggerFields::AREATRIGGER_FIELD_SPELL_VISUAL_ID, eMeteorSpell::VisualID);
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 8.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Caster->IsValidAttackTarget(l_Unit))
                        l_Caster->CastSpell(l_Unit, eMeteorSpell::MeteorDoT, true);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_mage_meteor_burn();
        }
};

/// Last Update
/// Meteor - 177345
class spell_at_mage_meteor_timestamp : public AreaTriggerEntityScript
{
    public:
        spell_at_mage_meteor_timestamp() : AreaTriggerEntityScript("spell_areatrigger_meteor_timestamp") {}

        enum eSpells
        {
            MeteorDamage = 153564,
            MeteorVisualEffect = 174556
        };

        void OnCreate(AreaTrigger* p_AreaTrigger)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster == nullptr)
                return;

            l_Caster->CastSpell(p_AreaTrigger->m_positionX, p_AreaTrigger->m_positionY, p_AreaTrigger->m_positionZ, eSpells::MeteorVisualEffect, true);
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster == nullptr)
                return;

            l_Caster->CastSpell(p_AreaTrigger->m_positionX, p_AreaTrigger->m_positionY, p_AreaTrigger->m_positionZ, eSpells::MeteorDamage, true);
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_mage_meteor_timestamp();
        }
};

/// Afterlife (healing sphere) - 117032
class spell_at_monk_afterlife_healing_sphere : public AreaTriggerEntityScript
{
    public:
        spell_at_monk_afterlife_healing_sphere() : AreaTriggerEntityScript("at_afterlife_healing_sphere") { }

        enum eAfterlife
        {
            SpellHealingSphere = 125355
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            SpellInfo const* l_CreateSpell = sSpellMgr->GetSpellInfo(p_AreaTrigger->GetSpellId());
            Unit* l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster && l_CreateSpell)
            {
                float l_Radius = 1.0f;
                Unit* l_Target = nullptr;

                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Checker(p_AreaTrigger, l_AreaTriggerCaster, l_Radius);
                JadeCore::UnitSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_Target, l_Checker);
                p_AreaTrigger->VisitNearbyGridObject(l_Radius, l_Searcher);
                if (!l_Target)
                    p_AreaTrigger->VisitNearbyWorldObject(l_Radius, l_Searcher);

                if (l_Target != nullptr && l_Target->GetGUID() == l_AreaTriggerCaster->GetGUID())
                {
                    l_AreaTriggerCaster->CastSpell(l_Target, eAfterlife::SpellHealingSphere, true);
                    p_AreaTrigger->Remove(0);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_monk_afterlife_healing_sphere();
        }
};

/// Gift of the Serpent (healing sphere) - 119031
class spell_at_monk_healing_sphere : public AreaTriggerEntityScript
{
    public:
        spell_at_monk_healing_sphere() : AreaTriggerEntityScript("at_healing_sphere") { }

        enum eGiftOfTheSerpent
        {
            SpellHealingSphere = 124041
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            SpellInfo const* l_CreateSpell = sSpellMgr->GetSpellInfo(p_AreaTrigger->GetSpellId());
            Unit* l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster && l_CreateSpell)
            {
                float l_Radius = 1.0f;
                Unit* l_Target = nullptr;
                std::list<Unit*> l_TargetList;

                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Checker(p_AreaTrigger, l_AreaTriggerCaster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Checker);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                l_TargetList.remove_if([this, l_AreaTriggerCaster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->ToUnit() == nullptr)
                        return true;

                    if (!l_AreaTriggerCaster->IsValidAssistTarget(p_Object->ToUnit()))
                        return true;

                    return false;
                });

                if (l_TargetList.size() > 1)
                {
                    l_TargetList.sort(JadeCore::ObjectDistanceOrderPred(p_AreaTrigger));
                    l_TargetList.resize(1);
                }

                if (l_TargetList.size() == 1)
                l_Target = l_TargetList.front();

                if (l_Target != nullptr)
                {
                    l_AreaTriggerCaster->CastSpell(l_Target, eGiftOfTheSerpent::SpellHealingSphere, true);
                    p_AreaTrigger->Remove(0);
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            auto l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster == nullptr)
                return;

            if ((uint32)p_AreaTrigger->GetDuration() < p_Time)
                l_AreaTriggerCaster->CastSpell(p_AreaTrigger->GetPositionX(), p_AreaTrigger->GetPositionY(), p_AreaTrigger->GetPositionZ(), 135920, true);
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_monk_healing_sphere();
        }
};

/// Chi Sphere - 121286
class spell_at_monk_chi_sphere_afterlife : public AreaTriggerEntityScript
{
    public:
        spell_at_monk_chi_sphere_afterlife() : AreaTriggerEntityScript("at_chi_sphere_afterlife") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            std::list<Unit*> targetList;
            float l_Radius = 1.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, targetList, u_check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    if (itr->GetGUID() == l_Caster->GetGUID())
                    {
                        l_Caster->CastSpell(itr, 121283, true); // Restore 1 Chi
                        p_AreaTrigger->SetDuration(0);
                        return;
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_monk_chi_sphere_afterlife();
        }
};

// Gift of the Ox 124503 124506
class spell_at_monk_gift_of_the_ox : public AreaTriggerEntityScript
{
    public:
        spell_at_monk_gift_of_the_ox()  : AreaTriggerEntityScript("at_gift_of_the_ox") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            std::list<Unit*> targetList;
            float l_Radius = 1.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, targetList, u_check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

            for (auto itr : targetList)
            {
                if (itr->GetGUID() != l_Caster->GetGUID())
                    continue;

                l_Caster->CastSpell(itr, 124507, true); // Gift of the Ox - Heal
                p_AreaTrigger->SetDuration(0);
                return;
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_monk_gift_of_the_ox();
        }
};

class spell_at_pri_divine_star : public AreaTriggerEntityScript
{
    public:
        spell_at_pri_divine_star() : AreaTriggerEntityScript("at_pri_divine_star") { }

        enum AreaTriggerSpells
        {
            SPELL_DIVINE_STAR_HOLY = 110744,
            SPELL_DIVINE_STAR_HEAL = 110745,
            SPELL_DIVINE_STAR_DAMAGE = 122128,
        };

        std::map<uint64, uint32> m_Cooldows;

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination)
        {
            Position l_Position;
            float l_Dist = 24.f; // Hardcoded in the tooltip;

            l_Position.m_positionX = p_SourcePosition.m_positionX + (l_Dist * cos(p_Caster->GetOrientation()));
            l_Position.m_positionY = p_SourcePosition.m_positionY + (l_Dist * sin(p_Caster->GetOrientation()));
            l_Position.m_positionZ = p_SourcePosition.m_positionZ;
            p_Caster->UpdateGroundPositionZ(l_Position.m_positionX, l_Position.m_positionY, l_Position.m_positionZ);

            p_PathToLinearDestination.push_back(l_Position);
            p_Caster->GetPosition(&p_DestinationPosition); // Return back
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

            Position l_CasterPosition;
            l_CasterPosition.m_positionX = l_Caster->GetPositionX();
            l_CasterPosition.m_positionY = l_Caster->GetPositionY();
            l_CasterPosition.m_positionZ = l_Caster->GetPositionZ();

            if (l_CasterPosition != p_AreaTrigger->GetDestination())
            {
                p_AreaTrigger->SetDestination(l_CasterPosition);
                p_AreaTrigger->SetTimeToTarget(p_AreaTrigger->GetDuration());
                p_AreaTrigger->SendAreaTriggerRePath(p_AreaTrigger->GetDuration(), p_AreaTrigger->GetDuration());
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_pri_divine_star();
        }
};

/// Power Word: Barrier - 62618
class spell_at_pri_power_word_barrier : public AreaTriggerEntityScript
{
    public:
        spell_at_pri_power_word_barrier() : AreaTriggerEntityScript("spell_areatrigger_power_word_barrier") { }

        enum ePowerWordBarrierSpell
        {
            PowerWordBarrierAura = 81782
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
            return new spell_at_pri_power_word_barrier();
        }
};

/// Angelic Feather - 121536
class spell_at_pri_angelic_feather : public AreaTriggerEntityScript
{
    public:
        spell_at_pri_angelic_feather() : AreaTriggerEntityScript("at_angelic_feather") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            std::list<Unit*> targetList;
            float l_Radius = 1.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(p_AreaTrigger, targetList, u_check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    l_Caster->CastSpell(itr, 121557, true); // Angelic Feather increase speed
                    p_AreaTrigger->SetDuration(0);
                    return;
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_pri_angelic_feather();
        }
};

/// Last Update 6.2.3
/// Smoke Bomb - 76577
class spell_at_rogue_smoke_bomb : public AreaTriggerEntityScript
{
    public:
        spell_at_rogue_smoke_bomb() : AreaTriggerEntityScript("spell_areatrigger_smoke_bomb") { }

        std::list<uint64> m_TargetList;

        enum eSmokeSpells
        {
            SmokeBombAura = 88611
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            float l_Radius = 4.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster == nullptr)
                return;

            std::list<Unit*> l_NewTargetList;
            JadeCore::AnyUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_NewTargetList, u_check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

            for (Unit* l_Target : l_NewTargetList)
            {
                if (std::find(m_TargetList.begin(), m_TargetList.end(), l_Target->GetGUID()) == m_TargetList.end())
                {
                    m_TargetList.push_back(l_Target->GetGUID());
                    l_Caster->CastSpell(l_Target, eSmokeSpells::SmokeBombAura, true);
                }
            }

            for (auto l_It = m_TargetList.begin(); l_It != m_TargetList.end();)
            {
                Unit* l_Target = ObjectAccessor::FindUnit(*l_It);
                if (!l_Target || (std::find(l_NewTargetList.begin(), l_NewTargetList.end(), l_Target) == l_NewTargetList.end()))
                {
                    if (l_Target)
                        l_Target->RemoveAura(eSmokeSpells::SmokeBombAura);

                    l_It = m_TargetList.erase(l_It);
                }
                else
                    ++l_It;
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (uint64 l_TargetGUID : m_TargetList)
            {
                Unit* l_Target = ObjectAccessor::FindUnit(l_TargetGUID);
                if (l_Target)
                    l_Target->RemoveAura(eSmokeSpells::SmokeBombAura);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_rogue_smoke_bomb();
        }
};

/// Last Update 6.2.3
/// Chi burst - 123986
class spell_at_monk_chi_burst : public AreaTriggerEntityScript
{
    public:
        spell_at_monk_chi_burst() : AreaTriggerEntityScript("spell_at_monk_chi_burst") { }

        enum eSpells
        {
            SerpentsWise = 115070,
            FierceTiger = 103985,
            ChiBurstDamage = 148135,
            ChiBurstHeal = 130654
        };

        std::list<uint64> m_UnitGUIDList;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster == nullptr)
                return;

            Player* l_Player = l_Caster->ToPlayer();

            if (l_Player == nullptr)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 3.0f;

            JadeCore::AnyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            float l_DmgMult = l_Player->HasSpell(eSpells::FierceTiger) ? 1.2f : 1.0f;
            float l_HealMult = l_Player->HasSpell(eSpells::SerpentsWise) ? 1.2f : 1.0f;

            int32 l_Damage = sSpellMgr->GetSpellInfo(eSpells::ChiBurstDamage)->Effects[EFFECT_0].BasePoints + l_DmgMult * l_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 2.75f;
            int32 l_Healing = sSpellMgr->GetSpellInfo(eSpells::ChiBurstHeal)->Effects[EFFECT_0].BasePoints + l_HealMult * l_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 2.75f;

            std::list<uint64> l_UnitGUIDList = m_UnitGUIDList;
            l_TargetList.remove_if([this, l_Caster, l_UnitGUIDList](Unit* p_Unit) -> bool
            {
                if (p_Unit == nullptr)
                    return true;

                if (!(std::find(l_UnitGUIDList.begin(), l_UnitGUIDList.end(), p_Unit->GetGUID()) == l_UnitGUIDList.end()))
                    return true;

                return false;
            });

            for (Unit* l_Target : l_TargetList)
            {
                if (l_Target->IsFriendlyTo(l_Caster) && l_Caster->IsValidAssistTarget(l_Target))
                    l_Player->CastCustomSpell(l_Target, eSpells::ChiBurstHeal, &l_Healing, NULL, NULL, true);
                else if (l_Caster->IsValidAttackTarget(l_Target))
                    l_Player->CastCustomSpell(l_Target, eSpells::ChiBurstDamage, &l_Damage, NULL, NULL, true);

                m_UnitGUIDList.push_back(l_Target->GetGUID());
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_monk_chi_burst();
        }
};

/// last update : 6.2.3
/// Charging Ox Wave - 119392
class spell_at_monk_charging_ox_wave : public AreaTriggerEntityScript
{
    public:
        spell_at_monk_charging_ox_wave() : AreaTriggerEntityScript("spell_at_monk_charging_ox_wave") { }

        enum eSpells
        {
            Stun = 123687
        };

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination)
        {
            Position l_Position;
            float l_Dist = 30.f;

            l_Position.m_positionX = p_SourcePosition.m_positionX + (l_Dist * cos(p_Caster->GetOrientation()));
            l_Position.m_positionY = p_SourcePosition.m_positionY + (l_Dist * sin(p_Caster->GetOrientation()));
            l_Position.m_positionZ = p_SourcePosition.m_positionZ;

            p_PathToLinearDestination.push_back(l_Position);
            p_DestinationPosition = l_Position;
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            Unit* l_AreaTriggerCaster = p_AreaTrigger->GetCaster();

            if (l_AreaTriggerCaster == nullptr)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 2.0f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_AreaTriggerCaster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Target : l_TargetList)
            {
                if (l_Target == nullptr)
                    return;

                if (!l_Target->HasAura(eSpells::Stun) && l_AreaTriggerCaster->IsValidAttackTarget(l_Target))
                    l_AreaTriggerCaster->CastSpell(l_Target, eSpells::Stun, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_monk_charging_ox_wave();
        }
};

/// last update : 6.2.3
/// Anti-Magic Zone - 51052
class spell_at_dk_anti_magic_zone : public AreaTriggerEntityScript
{
    public:
        spell_at_dk_anti_magic_zone() : AreaTriggerEntityScript("spell_at_dk_anti_magic_zone") { }

        std::list<uint64> m_TargetList;

        enum eSpells
        {
            antiMagicAura = 145629
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            float l_Radius = 6.50f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (l_Caster == nullptr)
                return;

            std::list<Unit*> l_NewTargetList;
            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Checker(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_NewTargetList, l_Checker);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Target : l_NewTargetList)
            {
                if (l_Caster->IsValidAssistTarget(l_Target) && l_Caster->IsInRaidWith(l_Target) && std::find(m_TargetList.begin(), m_TargetList.end(), l_Target->GetGUID()) == m_TargetList.end())
                {
                    m_TargetList.push_back(l_Target->GetGUID());
                    l_Target->CastSpell(l_Target, eSpells::antiMagicAura, true);
                    if (Aura* l_AntiMagicAura = l_Caster->GetAura(eSpells::antiMagicAura))
                        l_AntiMagicAura->SetDuration(p_AreaTrigger->GetDuration());
                }
            }

            for (auto l_It = m_TargetList.begin(); l_It != m_TargetList.end();)
            {
                Unit* l_Target = ObjectAccessor::FindUnit(*l_It);
                if (!l_Target || (std::find(l_NewTargetList.begin(), l_NewTargetList.end(), l_Target) == l_NewTargetList.end()))
                {
                    if (l_Target)
                        l_Target->RemoveAura(eSpells::antiMagicAura);

                    l_It = m_TargetList.erase(l_It);
                }
                else
                    ++l_It;
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (uint64 l_TargetGUID : m_TargetList)
            {
                Unit* l_Target = ObjectAccessor::FindUnit(l_TargetGUID);
                if (l_Target)
                    l_Target->RemoveAura(eSpells::antiMagicAura);
            }
        }

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination)
        {
            ;
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new spell_at_dk_anti_magic_zone();
        }
};

void AddSC_areatrigger_spell_scripts()
{
    /// Deathknight Area Trigger
    new spell_at_dk_defile();
    new spell_at_dk_anti_magic_zone();

    /// Druid Area Trigger
    new spell_at_druid_fungal_growth();
    new spell_at_druid_ursol_vortex();
    new spell_at_druid_solar_beam();

    /// Hunter Area Trigger
    new spell_at_hun_binding_shot();
    new spell_at_hun_ice_trap();
    new spell_at_hun_ice_trap_effect();
    new spell_at_hun_freezing_trap();
    new spell_at_hun_explosive_trap();
    new spell_at_hun_snake_trap();

    /// Mage Area Trigger
    new spell_at_mage_wod_frost_2p_bonus();
    new spell_at_mage_arcane_orb();
    new spell_at_mage_meteor_burn();
    new spell_at_mage_meteor_timestamp();
    new spell_at_mage_rune_of_power();

    /// Monk Area Trigger
    new spell_at_monk_healing_sphere();
    new spell_at_monk_afterlife_healing_sphere();
    new spell_at_monk_chi_sphere_afterlife();
    new spell_at_monk_gift_of_the_ox();
    new spell_at_monk_chi_burst();
    new spell_at_monk_charging_ox_wave();

    /// Priest Area Trigger
    new spell_at_pri_divine_star();
    new spell_at_pri_power_word_barrier();
    new spell_at_pri_angelic_feather();

    /// Rogue Area Trigger
    new spell_at_rogue_smoke_bomb();
}
