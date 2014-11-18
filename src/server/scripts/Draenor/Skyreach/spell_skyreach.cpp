#include "AreaTriggerScript.h"
#include <forward_list>

class AreaTrigger_spinning_blade : public MS::AreaTriggerEntityScript
{
    enum class Spells : uint32
    {
        SPINNING_BLADE_DMG = 153123,
    };

    std::forward_list<uint64> m_targets;

public:
    AreaTrigger_spinning_blade()
        : MS::AreaTriggerEntityScript("at_spinning_blade"), m_targets()
    {
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        // If We are on the last tick.
        if (p_AreaTrigger->GetDuration() < 100)
        {
            for (auto l_Guid : m_targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::SPINNING_BLADE_DMG)))
                {
                    l_Target->RemoveAura(uint32(Spells::SPINNING_BLADE_DMG));
                }
            }
        }
    }

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        std::list<Unit*> l_TargetList;
        float l_Radius = 4.0f;

        JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
        JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
        p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

        std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
        for (auto l_Guid : m_targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
            {
                if (l_Target->HasAura(uint32(Spells::SPINNING_BLADE_DMG)))
                {
                    l_ToRemove.emplace_front(l_Guid);
                    l_Target->RemoveAura(uint32(Spells::SPINNING_BLADE_DMG));
                }
            }
        }

        for (auto l_Guid : l_ToRemove)
        {
            m_targets.remove(l_Guid);
        }

        for (Unit* l_Unit : l_TargetList)
        {
            if (!l_Unit || l_Unit->GetExactDist2d(p_AreaTrigger) > l_Radius || l_Unit->HasAura(uint32(Spells::SPINNING_BLADE_DMG)))
                continue;

            p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(Spells::SPINNING_BLADE_DMG), true);
            m_targets.emplace_front(l_Unit->GetGUID());
        }
    }
};

class AreaTrigger_solar_zone : public MS::AreaTriggerEntityScript
{
    enum class SolarHealSpells : uint32
    {
        SOLAR_ZONE_1 = 160935,
        SOLAR_ZONE_HEAL = 160281,
        SOLAR_ZONE_DMG = 158441,
    };

    std::forward_list<uint64> m_Targets;

public:
    AreaTrigger_solar_zone()
        : MS::AreaTriggerEntityScript("at_solar_zone"),
        m_Targets()
    {
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        // If We are on the last tick.
        if (p_AreaTrigger->GetDuration() < 100)
        {
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(SolarHealSpells::SOLAR_ZONE_HEAL)))
                    l_Target->RemoveAura(uint32(SolarHealSpells::SOLAR_ZONE_HEAL));

                if (l_Target && l_Target->HasAura(uint32(SolarHealSpells::SOLAR_ZONE_DMG)))
                    l_Target->RemoveAura(uint32(SolarHealSpells::SOLAR_ZONE_DMG));
            }
        }
    }

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        std::list<Unit*> l_TargetList;
        float l_Radius = 6.0f;

        JadeCore::AnyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Radius);
        JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
        p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

        std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
        for (auto l_Unit : l_TargetList)
        {
            uint64 l_Guid = l_Unit->GetGUID();
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
            {
                if (l_Target->HasAura(uint32(SolarHealSpells::SOLAR_ZONE_HEAL)))
                {
                    l_ToRemove.emplace_front(l_Guid);
                    l_Target->RemoveAura(uint32(SolarHealSpells::SOLAR_ZONE_HEAL));
                }
                if (l_Target->HasAura(uint32(SolarHealSpells::SOLAR_ZONE_DMG)))
                {
                    l_ToRemove.emplace_front(l_Guid);
                    l_Target->RemoveAura(uint32(SolarHealSpells::SOLAR_ZONE_DMG));
                }
            }
        }

        for (auto l_Guid : l_ToRemove)
        {
            m_Targets.remove(l_Guid);
        }

        for (Unit* l_Unit : l_TargetList)
        {
            if (!l_Unit || l_Unit->GetExactDist2d(p_AreaTrigger) > l_Radius)
                continue;

            if (l_Unit->IsFriendlyTo(p_AreaTrigger->GetCaster()) && !l_Unit->HasAura(uint32(SolarHealSpells::SOLAR_ZONE_HEAL)))
            {
                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(SolarHealSpells::SOLAR_ZONE_HEAL), true);
                m_Targets.emplace_front(l_Unit->GetGUID());
            }
            else if (!l_Unit->IsFriendlyTo(p_AreaTrigger->GetCaster()) && !l_Unit->HasAura(uint32(SolarHealSpells::SOLAR_ZONE_DMG)))
            {
                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(SolarHealSpells::SOLAR_ZONE_DMG), true);
                m_Targets.emplace_front(l_Unit->GetGUID());
            }
        }
    }
};

// Spinning Blade - 153544
class spell_SpinningBlade : public SpellScriptLoader
{
public:
    spell_SpinningBlade()
        : SpellScriptLoader("spell_SpinningBlade")
    {
    }

    enum class Spells : uint32
    {
        // Those are for Blade Dance
        SPINNING_BLADE_4 = 153535,
        SPINNING_BLADE_5 = 153536,
        SPINNING_BLADE_6 = 153537,
        SPINNING_BLADE_7 = 153538,

        // Those are for Spinning Blade
        SPINNING_BLADE_9 = 153583,
        SPINNING_BLADE_11 = 153584,
        SPINNING_BLADE_10 = 153585,
        SPINNING_BLADE_3 = 153586,
        SPINNING_BLADE_12 = 153587,
        SPINNING_BLADE_8 = 153588,
    };

    class spell_SpinningBlade_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_SpinningBlade_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            static const Spells k_Spells[] = {
                Spells::SPINNING_BLADE_9,
                Spells::SPINNING_BLADE_11,
                Spells::SPINNING_BLADE_10,
                Spells::SPINNING_BLADE_3,
                Spells::SPINNING_BLADE_12,
                Spells::SPINNING_BLADE_8,
            };

            if (GetCaster() && GetHitUnit())
            {
                // Spinning Blade AreaTrigger
                uint32 l_Random = urand(0, 5);
                uint32 l_SpellToCast = uint32(k_Spells[l_Random]);
                GetCaster()->CastSpell(GetHitUnit(), l_SpellToCast, true);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_SpinningBlade_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_SpinningBlade_SpellScript();
    }
};

// Storm - 156515
class spell_Storm : public SpellScriptLoader
{
public:
    spell_Storm()
        : SpellScriptLoader("spell_Storm")
    {
    }

    enum class Spells : uint32
    {
        TWISTER_DNT = 178617,
        STORM = 156515,
        STORM_AT = 156840,
        STORM_DMG = 156841,
    };

    class spell_Storm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_Storm_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster())
            {
                GetCaster()->CastSpell(GetCaster(), uint32(Spells::STORM_AT), true);
                GetCaster()->CastSpell(GetCaster(), uint32(Spells::TWISTER_DNT), true);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_Storm_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_Storm_SpellScript();
    }
};

void AddSC_spell_instance_skyreach()
{
    // Spells.
    new AreaTrigger_solar_zone();
    new AreaTrigger_spinning_blade();
    new spell_SpinningBlade();
    new spell_Storm();
}