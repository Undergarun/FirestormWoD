////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "instance_skyreach.hpp"

namespace MS
{
    // Summon Quills - 159381
    class spell_Quills: public SpellScriptLoader
    {
    public:
        spell_Quills()
            : SpellScriptLoader("spell_Quills")
        {
        }

        enum class Spells : uint32
        {
            Quills = 156742,
        };

        class spell_QuillsSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_QuillsSpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster)
                    l_Caster->CastSpell(l_Caster, uint32(Spells::Quills), true);
            }

            /// This method is used to filter the targets that are behind the big pillar, in the Rukhran event, when
            /// Rukhran is casting Quills (player should go behind the big pillar at the center of the platform to hide).
            void CheckTarget(std::list<WorldObject*>& p_UnitList)
            {
                p_UnitList.remove_if([this](WorldObject* p_Object)
                {
                    /// p_Ref and p_Point should be vectors.
                    auto l_IsToTheRightFromRef = [](Position const& p_Ref, Position const& p_Point) -> bool {
                        return p_Point.m_positionX * p_Ref.m_positionY - p_Point.m_positionY * p_Ref.m_positionX > 0;
                    };

                    /// Those both position are the extreme position of the pillar.
                    /// The three position refers to those points. We want to check if the player
                    /// is inside and far from the target for a certain distance.
                    /// O---------------- L
                    ///  \
                    ///   \        Player
                    ///    \      x
                    ///     \
                    ///      \
                    ///       \
                    ///        R
                    static const Position s_OriginPosition = { 918.919f, 1913.459f, 213.0f, 0.0f};
                    static const Position s_RightPosition = { 936.999f, 1872.137f, 213.0f, 0.0f };
                    static const Position s_LeftPosition = { 951.305f, 1882.338f, 213.0f, 0.0f };

                    Position l_RefLeftVect = s_LeftPosition - s_OriginPosition;
                    Position l_RefRightVect = s_RightPosition - s_OriginPosition;

                    Position l_PlayerVect = *p_Object - s_OriginPosition;

                    if (!l_IsToTheRightFromRef(l_RefLeftVect, l_PlayerVect) || l_IsToTheRightFromRef(l_RefRightVect, l_PlayerVect))
                        return false;

                    if (GetCaster()->GetExactDist2d(p_Object) < 30.0f)
                        return false;

                    return true;
                });
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_QuillsSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_QuillsSpellScript::CheckTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_QuillsSpellScript();
        }
    };

    // Summon Cast Down - 165845
    class spell_CastDown: public SpellScriptLoader
    {
    public:
        spell_CastDown()
            : SpellScriptLoader("spell_CastDown")
        {
        }

        enum class Spells : uint32
        {
            CastDown = 153955,
        };

        class spell_CastDownSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_CastDownSpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster() && GetHitUnit())
                {
                    GetHitUnit()->CastSpell(GetHitUnit(), uint32(Spells::CastDown), true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_CastDownSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_CastDownSpellScript();
        }
    };

    // AreaTriggers for spells: 154044
    class AreaTrigger_LensFlare : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            LensFlare_Dmg = 154043,
        };

        std::forward_list<uint64> m_Targets;

    public:
        AreaTrigger_LensFlare()
            : AreaTriggerEntityScript("at_LensFlare"),
            m_Targets()
        {
            }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_LensFlare();
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::LensFlare_Dmg)))
                    l_Target->RemoveAura(uint32(Spells::LensFlare_Dmg));
            }
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 3.5f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
                {
                    if (l_Target->HasAura(uint32(Spells::LensFlare_Dmg)))
                    {
                        l_ToRemove.emplace_front(l_Guid);
                        l_Target->RemoveAura(uint32(Spells::LensFlare_Dmg));
                    }
                }
            }

            for (auto l_Guid : l_ToRemove)
            {
                m_Targets.remove(l_Guid);
            }

            for (Unit* l_Unit : l_TargetList)
            {
                if (!l_Unit || l_Unit->GetExactDist2d(p_AreaTrigger) > l_Radius || l_Unit->HasAura(uint32(Spells::LensFlare_Dmg)))
                    continue;

                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(Spells::LensFlare_Dmg), true);
                m_Targets.emplace_front(l_Unit->GetGUID());

                // Achievement handling.
                if (Player* l_Plr = l_Unit->ToPlayer())
                {
                    // We check if the caster is Ranjit.
                    if (p_AreaTrigger->GetInstanceScript() && p_AreaTrigger->GetCaster()->GetEntry() == InstanceSkyreach::BossEntries::RANJIT)
                        p_AreaTrigger->GetInstanceScript()->SetData64(InstanceSkyreach::Data::PlayerIsHittedByRanjitSpells, l_Plr->GetGUID());
                }
            }
        }
    };

    // AreaTriggers for spells: 152973
    class AreaTrigger_ProtectiveBarrier : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            ProtectiveBarrier = 152975,
            ProtectiveBarrier_at = 152973,
        };

        std::forward_list<uint64> m_Targets;

    public:
        AreaTrigger_ProtectiveBarrier()
            : AreaTriggerEntityScript("at_ProtectiveBarrier"),
            m_Targets()
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_ProtectiveBarrier();
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::ProtectiveBarrier)))
                    l_Target->RemoveAura(uint32(Spells::ProtectiveBarrier));
            }
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 30.0f;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
                {
                    if (l_Target->HasAura(uint32(Spells::ProtectiveBarrier)))
                    {
                        l_ToRemove.emplace_front(l_Guid);
                        l_Target->RemoveAura(uint32(Spells::ProtectiveBarrier));
                    }
                }
            }

            for (auto l_Guid : l_ToRemove)
            {
                m_Targets.remove(l_Guid);
            }

            for (Unit* l_Unit : l_TargetList)
            {
                if (!l_Unit
                    || l_Unit->GetExactDist2d(p_AreaTrigger) > l_Radius
                    || l_Unit->HasAura(uint32(Spells::ProtectiveBarrier))
                    || l_Unit->HasAura(uint32(Spells::ProtectiveBarrier_at)))
                    continue;

                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(Spells::ProtectiveBarrier), true);
                m_Targets.emplace_front(l_Unit->GetGUID());
            }
        }
    };

    // AreaTriggers for spells: 154110
    class AreaTrigger_Smash : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            SMASH = 154110,
            SMASH_2 = 154113,
            SMASH_DMG = 154132,
        };

    public:
        AreaTrigger_Smash()
            : AreaTriggerEntityScript("at_Smash")
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_Smash();
        }

        void OnCreate(AreaTrigger* /*p_AreaTrigger*/)
        {
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            static const float k_Radius = 10.0f;
            static const float k_RadiusFromLine = 3.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), k_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(k_Radius, l_Searcher);

            Position l_Pos;
            p_AreaTrigger->GetPosition(&l_Pos);
            l_Pos.m_positionX += k_Radius * cos(p_AreaTrigger->GetOrientation());
            l_Pos.m_positionY += k_Radius * sin(p_AreaTrigger->GetOrientation());

            for (auto l_Target : l_TargetList)
            {
                if (l_Target && p_AreaTrigger->GetCaster()->isInFront(l_Target) && l_Target->GetExactDist2d(p_AreaTrigger) < k_Radius && DistanceFromLine(*p_AreaTrigger, l_Pos, *l_Target) < k_RadiusFromLine)
                {
                    if (p_AreaTrigger->GetCaster())
                        p_AreaTrigger->GetCaster()->CastSpell(l_Target, uint32(Spells::SMASH_DMG), true);
                }
            }
        }
    };

    // Visual Energize - 154177
    class spell_VisualEnergize2: public SpellScriptLoader
    {
    public:
        spell_VisualEnergize2()
            : SpellScriptLoader("spell_VisualEnergize2")
        {
        }

        class spell_VisualEnergize2SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_VisualEnergize2SpellScript);

            void CheckTarget(std::list<WorldObject*>& unitList)
            {
                Unit* l_Caster = GetCaster();
                unitList.remove_if([l_Caster](WorldObject* p_Obj) {
                    if (!p_Obj->ToCreature())
                        return true;

                    if (l_Caster->GetEntry() == 77543)
                        return p_Obj->ToCreature()->GetEntry() != 76367;
                    else
                        return p_Obj->ToCreature()->GetEntry() != 76142;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_VisualEnergize2SpellScript::CheckTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_VisualEnergize2SpellScript();
        }
    };

    // Visual Energize - 154159
    class spell_VisualEnergize: public SpellScriptLoader
    {
    public:
        spell_VisualEnergize()
            : SpellScriptLoader("spell_VisualEnergize")
        {
        }

        class spell_VisualEnergizeSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_VisualEnergizeSpellScript);

            void CheckTarget(std::list<WorldObject*>& unitList)
            {
                Unit* l_Caster = GetCaster();
                unitList.remove_if([l_Caster](WorldObject* p_Obj) {
                    return !(p_Obj->ToCreature()
                        && p_Obj->ToCreature()->GetEntry() == 76142
                        && p_Obj->ToCreature()->GetCurrentSpell(CURRENT_CHANNELED_SPELL));
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_VisualEnergizeSpellScript::CheckTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_VisualEnergizeSpellScript();
        }
    };

    // Flash Bang - 160066
    class spell_FlashBang: public SpellScriptLoader
    {
    public:
        spell_FlashBang()
            : SpellScriptLoader("spell_FlashBang")
        {
        }

        enum class Spells : uint32
        {
        };

        class spell_FlashBangSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_FlashBangSpellScript);

            void CheckTarget(std::list<WorldObject*>& unitList)
            {
                Unit* l_Caster = GetCaster();
                unitList.remove_if([l_Caster](WorldObject* p_Obj) {
                    return !p_Obj->isInFront(l_Caster);
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_FlashBangSpellScript::CheckTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_FlashBangSpellScript::CheckTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_FlashBangSpellScript::CheckTarget, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_FlashBangSpellScript();
        }
    };

    // Sunstrike - 153828
    class spell_Sunstrike: public SpellScriptLoader
    {
    public:
        spell_Sunstrike()
            : SpellScriptLoader("spell_Sunstrike")
        {
        }

        enum class Spells : uint32
        {
        };

        class spell_SunstrikeSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_SunstrikeSpellScript);

            void CheckTargetIn(std::list<WorldObject*>& unitList)
            {
                Unit* l_Caster = GetCaster();
                unitList.remove_if([l_Caster](WorldObject* p_Obj) {
                    return p_Obj->GetExactDist2d(l_Caster) > 10.0f;
                });
            }

            void CheckTargetOut(std::list<WorldObject*>& unitList)
            {
                Unit* l_Caster = GetCaster();
                unitList.remove_if([l_Caster](WorldObject* p_Obj) {
                    return p_Obj->GetExactDist2d(l_Caster) < 10.0f;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_SunstrikeSpellScript::CheckTargetIn, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_SunstrikeSpellScript::CheckTargetOut, EFFECT_2, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_SunstrikeSpellScript();
        }
    };

    // Summon Solar Flare - 153827
    class spell_SummonSolarFlare: public SpellScriptLoader
    {
    public:
        spell_SummonSolarFlare()
            : SpellScriptLoader("spell_SummonSolarFlare")
        {
        }

        enum class Spells : uint32
        {
        };

        class spell_SummonSolarFlareSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_SummonSolarFlareSpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster())
                {
                    Position l_Position;
                    GetSpell()->GetDestTarget()->GetPosition(&l_Position);
                    GetCaster()->SummonCreature(InstanceSkyreach::MobEntries::SOLAR_FLARE, l_Position);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_SummonSolarFlareSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_SummonSolarFlareSpellScript();
        }
    };

    // Energize 154140
    class spell_Energize: public SpellScriptLoader
    {
    public:
        spell_Energize()
            : SpellScriptLoader("spell_Energize")
        {
        }

        enum class Spells : uint32
        {
            ENERGIZE = 154139, // During 12 seconds, restart after 3 seconds.
            ENERGIZE_HEAL = 154149,
            ENERGIZE_DMG = 154150,
            ENERGIZE_VISUAL_1 = 154179,
        };

        class spell_EnergizeSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_EnergizeSpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, uint32(Spells::ENERGIZE_HEAL));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_EnergizeSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_EnergizeSpellScript();
        }
    };

    // AreaTriggers for spells: 159221
    class AreaTrigger_SolarStorm : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            SOLAR_STORM_DMG = 159226,
        };

        std::forward_list<uint64> m_Targets;

    public:
        AreaTrigger_SolarStorm()
            : AreaTriggerEntityScript("at_SolarStorm"),
            m_Targets()
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_SolarStorm();
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::SOLAR_STORM_DMG)))
                    l_Target->RemoveAura(uint32(Spells::SOLAR_STORM_DMG));
            }
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 4.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
                {
                    if (l_Target->HasAura(uint32(Spells::SOLAR_STORM_DMG)))
                    {
                        l_ToRemove.emplace_front(l_Guid);
                        l_Target->RemoveAura(uint32(Spells::SOLAR_STORM_DMG));
                    }
                }
            }

            for (auto l_Guid : l_ToRemove)
            {
                m_Targets.remove(l_Guid);
            }

            for (Unit* l_Unit : l_TargetList)
            {
                if (!l_Unit || l_Unit->GetExactDist2d(p_AreaTrigger) > l_Radius || l_Unit->HasAura(uint32(Spells::SOLAR_STORM_DMG)))
                    continue;

                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(Spells::SOLAR_STORM_DMG), true);
                m_Targets.emplace_front(l_Unit->GetGUID());
            }
        }
    };

    // Solar storm - 159215
    class spell_SolarStorm: public SpellScriptLoader
    {
    public:
        spell_SolarStorm()
            : SpellScriptLoader("spell_SolarStorm")
        {
        }

        enum class Spells : uint32
        {
            SOLAR_STORM = 159215, // FIXME.
            SOLAR_STORM_1 = 159216,
            SOLAR_STORM_2 = 159218,
            SOLAR_STORM_3 = 159221,
            SOLAR_STORM_4 = 159226,
        };

        class spell_SolarStorm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_SolarStorm_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetHitUnit() && GetCaster())
                    GetCaster()->CastSpell(GetHitUnit(), uint32(Spells::SOLAR_STORM_1), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_SolarStorm_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_SolarStorm_SpellScript();
        }
    };

    // AreaTriggers for spells: 156634, 156636
    class AreaTrigger_FourWinds : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            // Four winds. Arrived after 2 or 3 instances of WindWall.
            FOUR_WINDS = 156793,
            FOUR_WINDS_DMG = 153139,
            FOUR_WINDS_AT_1 = 156634,
            FOUR_WINDS_AT_2 = 156636,
            FOUR_WINDS_VISUAL_1 = 166623,
            FOUR_WINDS_VISUAL_2 = 166664
        };

        float m_angle;
        std::forward_list<uint64> m_targets;
        uint32 m_Last;
        uint32 m_IsSpellAt2;

    public:
        AreaTrigger_FourWinds()
            : AreaTriggerEntityScript("at_FourWinds"),
            m_angle(0),
            m_targets(),
            m_Last(60000),
            m_IsSpellAt2(0)
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_FourWinds();
        }

        bool IsInWind(Unit const* p_Unit, AreaTrigger const* p_Area) const
        {
            static const float k_Radius = 3.0f;

            float l_x1 = p_Area->GetPositionX() + cos(m_angle);
            float l_x2 = p_Area->GetPositionX() + cos(m_angle - M_PI);
            float l_y1 = p_Area->GetPositionY() + sin(m_angle);
            float l_y2 = p_Area->GetPositionY() + sin(m_angle - M_PI);

            float l_dx = l_x2 - l_x1;
            float l_dy = l_y2 - l_y1;

            bool l_b1 = std::abs(l_dy * p_Unit->GetPositionX() - l_dx * p_Unit->GetPositionY() - l_x1 * l_y2 + l_x2 * l_y1) / std::sqrt(l_dx * l_dx + l_dy * l_dy) < k_Radius;

            l_x1 = p_Area->GetPositionX() + cos(m_angle + M_PI / 2);
            l_x2 = p_Area->GetPositionX() + cos(m_angle - M_PI / 2);
            l_y1 = p_Area->GetPositionY() + sin(m_angle + M_PI / 2);
            l_y2 = p_Area->GetPositionY() + sin(m_angle - M_PI / 2);

            l_dx = l_x2 - l_x1;
            l_dy = l_y2 - l_y1;

            bool l_b2 = std::abs(l_dy * p_Unit->GetPositionX() - l_dx * p_Unit->GetPositionY() - l_x1 * l_y2 + l_x2 * l_y1) / std::sqrt(l_dx * l_dx + l_dy * l_dy) < k_Radius;

            return l_b1 || l_b2;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (auto l_Guid : m_targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::FOUR_WINDS_DMG)))
                {
                    l_Target->RemoveAura(uint32(Spells::FOUR_WINDS_DMG));
                }
            }
        }

        void OnCreate(AreaTrigger* p_AreaTrigger)
        {
            m_IsSpellAt2 = p_AreaTrigger->GetSpellId() == uint32(Spells::FOUR_WINDS_AT_2);
            m_angle = (m_IsSpellAt2 ? M_PI / + 72 : M_PI / 7) + (m_IsSpellAt2 ? M_PI / 36 : -M_PI / 72); // Magic values ! Taken from tests IG.
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            static const float k_RotSpeed[2] =
            {
                0.014f,
                0.014f
            };
            static const float k_dist = 45.0f;

            // Update targets.
            std::list<Unit*> l_TargetList;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), k_dist);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(k_dist, l_Searcher);

            std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
            for (auto l_Guid : m_targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && (l_Target->GetExactDist2d(p_AreaTrigger) > k_dist || !IsInWind(l_Target, p_AreaTrigger)))
                {
                    if (l_Target->HasAura(uint32(Spells::FOUR_WINDS_DMG)))
                    {
                        l_ToRemove.emplace_front(l_Guid);
                        l_Target->RemoveAura(uint32(Spells::FOUR_WINDS_DMG));
                    }
                }
            }

            for (auto l_Guid : l_ToRemove)
            {
                m_targets.remove(l_Guid);
            }

            for (Unit* l_Unit : l_TargetList)
            {
                if (!l_Unit
                    || l_Unit->GetExactDist2d(p_AreaTrigger) > k_dist
                    || l_Unit->HasAura(uint32(Spells::FOUR_WINDS_DMG))
                    || !IsInWind(l_Unit, p_AreaTrigger))
                    continue;

                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(Spells::FOUR_WINDS_DMG), true);
                m_targets.emplace_front(l_Unit->GetGUID());

                // Achievement handling.
                if (Player* l_Plr = l_Unit->ToPlayer())
                {
                    // We check if the caster is Ranjit.
                    if (p_AreaTrigger->GetInstanceScript() && p_AreaTrigger->GetCaster()->GetEntry() == InstanceSkyreach::BossEntries::RANJIT)
                        p_AreaTrigger->GetInstanceScript()->SetData64(InstanceSkyreach::Data::PlayerIsHittedByRanjitSpells, l_Plr->GetGUID());
                }
            }

            // Update rotation.
            if ((m_Last - p_AreaTrigger->GetDuration() < 100))
                return;

            if (m_IsSpellAt2)
                m_angle -= k_RotSpeed[m_IsSpellAt2];
            else
                m_angle += k_RotSpeed[m_IsSpellAt2];

            // We are staying in [0, 2pi]
            if (m_angle > 2 * M_PI)
                m_angle -= 2 * M_PI;
            if (m_angle < 0)
                m_angle += 2 * M_PI;

            m_Last = p_AreaTrigger->GetDuration();
        }
    };

    // Four wind - 156793
    class spell_FourWind: public SpellScriptLoader
    {
    public:
        spell_FourWind()
            : SpellScriptLoader("spell_FourWind")
        {
        }

        enum class Spells : uint32
        {
            // Four winds. Arrived after 2 or 3 instances of WindWall.
            FOUR_WINDS = 156793,
            FOUR_WINDS_DMG = 153139,
            FOUR_WINDS_AT_1 = 156634,
            FOUR_WINDS_AT_2 = 156636,
            FOUR_WINDS_VISUAL_1 = 166623,
            FOUR_WINDS_VISUAL_2 = 166664
        };

        class spell_FourWind_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_FourWind_SpellScript);

            void HandleTriggerMissible(SpellEffIndex /*effIndex*/)
            {
                static const uint32 k_spells1[2] = 
                {
                    uint32(Spells::FOUR_WINDS_AT_1),
                    uint32(Spells::FOUR_WINDS_VISUAL_1)
                };
                static const uint32 k_spells2[2] =
                {
                    uint32(Spells::FOUR_WINDS_AT_2),
                    uint32(Spells::FOUR_WINDS_VISUAL_2)
                };
                if (GetCaster())
                {
                    std::list<Unit*> l_Target = ScriptUtils::SelectNearestCreatureListWithEntry(GetCaster(), InstanceSkyreach::MobEntries::ArakkoaPincerBirdsController, 40.0f);
                    if (l_Target.empty())
                        return;

                    uint32 l_Random = urand(0, 1);
                    uint32 i = 0;
                    for (Unit* l_Trigger : l_Target)
                    {
                        l_Trigger->SetOrientation(0);
                        if (l_Random == 0)
                            l_Trigger->CastSpell(l_Trigger, k_spells1[i], true);
                        else
                            l_Trigger->CastSpell(l_Trigger, k_spells2[i], true);
                        ++i;
                    }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_FourWind_SpellScript::HandleTriggerMissible, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_FourWind_SpellScript();
        }
    };

    // AreaTriggers for spells: 153311, 153314
    class AreaTrigger_WindWall : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            WINDWALL_DMG = 153759,
            WINDWALL_AT_1 = 153311,
            WINDWALL_AT_2 = 153314,
        };

        float m_angle;
        std::forward_list<uint64> m_targets;
        uint32 m_Last;
        uint32 m_IsSpellAt2;

    public:
        AreaTrigger_WindWall()
            : AreaTriggerEntityScript("at_WindWall"),
            m_angle(0),
            m_targets(),
            m_Last(60000),
            m_IsSpellAt2(0)
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_WindWall();
        }

        bool IsInWind(Unit const* p_Unit, AreaTrigger const* p_Area) const
        {
            static const float k_Radius = 2.0f;

            float l_x1 = p_Area->GetPositionX() + cos(m_angle);
            float l_x2 = p_Area->GetPositionX() + cos(m_angle - M_PI);
            float l_y1 = p_Area->GetPositionY() + sin(m_angle);
            float l_y2 = p_Area->GetPositionY() + sin(m_angle - M_PI);

            float l_dx = l_x2 - l_x1;
            float l_dy = l_y2 - l_y1;

            return std::abs(l_dy * p_Unit->GetPositionX() - l_dx * p_Unit->GetPositionY() - l_x1 * l_y2 + l_x2 * l_y1) / std::sqrt(l_dx * l_dx + l_dy * l_dy) < k_Radius;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (auto l_Guid : m_targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::WINDWALL_DMG)))
                {
                    l_Target->RemoveAura(uint32(Spells::WINDWALL_DMG));
                }
            }
        }

        void OnCreate(AreaTrigger* p_AreaTrigger)
        {
            m_angle = p_AreaTrigger->GetOrientation();
            m_IsSpellAt2 = p_AreaTrigger->GetSpellId() == uint32(Spells::WINDWALL_AT_2);
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            static const float k_RotSpeed[2] =
            {
                0.015f,
                0.022f
            };
            static const int32 k_Start[2] =
            {
                55000,
                37000
            };
            static const float k_dist = 10.0f;

            // Update targets.
            std::list<Unit*> l_TargetList;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), k_dist);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(k_dist, l_Searcher);

            std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
            for (auto l_Guid : m_targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && (l_Target->GetExactDist2d(p_AreaTrigger) > k_dist || !IsInWind(l_Target, p_AreaTrigger)))
                {
                    if (l_Target->HasAura(uint32(Spells::WINDWALL_DMG)))
                    {
                        l_ToRemove.emplace_front(l_Guid);
                        l_Target->RemoveAura(uint32(Spells::WINDWALL_DMG));
                    }
                }
            }

            for (auto l_Guid : l_ToRemove)
            {
                m_targets.remove(l_Guid);
            }

            for (Unit* l_Unit : l_TargetList)
            {
                if (!l_Unit
                    || l_Unit->GetExactDist2d(p_AreaTrigger) > k_dist
                    || l_Unit->HasAura(uint32(Spells::WINDWALL_DMG))
                    || !IsInWind(l_Unit, p_AreaTrigger))
                    continue;

                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(Spells::WINDWALL_DMG), true);
                m_targets.emplace_front(l_Unit->GetGUID());

                // Achievement handling.
                if (Player* l_Plr = l_Unit->ToPlayer())
                {
                    // We check if the caster is Ranjit.
                    if (p_AreaTrigger->GetInstanceScript() && p_AreaTrigger->GetCaster()->GetEntry() == InstanceSkyreach::BossEntries::RANJIT)
                        p_AreaTrigger->GetInstanceScript()->SetData64(InstanceSkyreach::Data::PlayerIsHittedByRanjitSpells, l_Plr->GetGUID());
                }
            }

            // Update rotation.
            if (p_AreaTrigger->GetDuration() > k_Start[m_IsSpellAt2] || (m_Last - p_AreaTrigger->GetDuration() < 100))
                return;

            if (m_IsSpellAt2)
                m_angle += k_RotSpeed[m_IsSpellAt2];
            else
                m_angle -= k_RotSpeed[m_IsSpellAt2];

            // We are staying in [0, 2pi]
            if (m_angle > 2 * M_PI)
                m_angle -= 2 * M_PI;
            if (m_angle < 0)
                m_angle += 2 * M_PI;

            m_Last = p_AreaTrigger->GetDuration();
        }
    };

    // Windwall - 153315
    class spell_Windwall: public SpellScriptLoader
    {
    public:
        spell_Windwall()
            : SpellScriptLoader("spell_Windwall")
        {
        }

        enum class Spells : uint32
        {
            // Clock and counter clock windwalls.
            WINDWALL_1 = 153593,
            WINDWALL_2= 153594,
        };

        class spell_Windwall_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_Windwall_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster())
                {
                    Unit* l_Target = nullptr;
                    if ((l_Target = ScriptUtils::SelectRandomPlayerIncludedTank(GetCaster(), 30.0f)))
                    {
                        uint32 l_Random = urand(0, 1);
                        if (l_Random == 0)
                            GetCaster()->CastSpell(l_Target, uint32(Spells::WINDWALL_1));
                        else
                            GetCaster()->CastSpell(l_Target, uint32(Spells::WINDWALL_2));
                    }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_Windwall_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_Windwall_SpellScript();
        }
    };

    // AreaTriggers for spells: 153535, 153536, 153537, 153538, 153583, 153584, 153585,153586, 153587, 153588
    class AreaTrigger_spinning_blade : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            SPINNING_BLADE_DMG = 153123,
        };

        std::forward_list<uint64> m_targets;

    public:
        AreaTrigger_spinning_blade()
            : AreaTriggerEntityScript("at_spinning_blade"), m_targets()
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_spinning_blade();
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
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

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
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

                // Achievement handling.
                if (Player* l_Plr = l_Unit->ToPlayer())
                {
                    // We check if the caster is Ranjit.
                    if (p_AreaTrigger->GetInstanceScript() && p_AreaTrigger->GetCaster()->GetEntry() == InstanceSkyreach::BossEntries::RANJIT)
                        p_AreaTrigger->GetInstanceScript()->SetData64(InstanceSkyreach::Data::PlayerIsHittedByRanjitSpells, l_Plr->GetGUID());
                }
            }
        }
    };

    // AreaTriggers for spells: 160935
    class AreaTrigger_solar_zone : public AreaTriggerEntityScript
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
            : AreaTriggerEntityScript("at_solar_zone"),
            m_Targets()
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_solar_zone();
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
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

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
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

    // AreaTriggers for spells: 156840
    class AreaTrigger_storm_zone : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            TWISTER_DNT = 178617,
            STORM = 156515,
            STORM_AT = 156840,
            STORM_DMG = 156841,
        };

        std::forward_list<uint64> m_Targets;

    public:
        AreaTrigger_storm_zone()
            : AreaTriggerEntityScript("at_storm_zone"),
            m_Targets()
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_storm_zone();
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::STORM_DMG)))
                    l_Target->RemoveAura(uint32(Spells::STORM_DMG));
            }
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 4.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
            for (auto l_Unit : l_TargetList)
            {
                uint64 l_Guid = l_Unit->GetGUID();
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
                {
                    if (l_Target->HasAura(uint32(Spells::STORM_DMG)))
                    {
                        l_ToRemove.emplace_front(l_Guid);
                        l_Target->RemoveAura(uint32(Spells::STORM_DMG));
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

                if (!l_Unit->HasAura(uint32(Spells::STORM_DMG)))
                {
                    p_AreaTrigger->GetCaster()->AddAura(uint32(Spells::STORM_DMG), l_Unit);
                    m_Targets.emplace_front(l_Unit->GetGUID());
                }
            }
        }
    };

    // AreaTriggers for spells: 153905
    class AreaTrigger_dervish : public AreaTriggerEntityScript
    {
        enum class Spells : uint32
        {
            DERVISH = 153905,
            DERVISH_DMG = 153907,
        };

        std::forward_list<uint64> m_Targets;

    public:
        AreaTrigger_dervish()
            : AreaTriggerEntityScript("at_dervish"),
            m_Targets()
        {
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new AreaTrigger_dervish();
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(uint32(Spells::DERVISH_DMG)))
                    l_Target->RemoveAura(uint32(Spells::DERVISH_DMG));
            }
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 4.0f;
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
            for (auto l_Unit : l_TargetList)
            {
                uint64 l_Guid = l_Unit->GetGUID();
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
                {
                    if (l_Target->HasAura(uint32(Spells::DERVISH_DMG)))
                    {
                        l_ToRemove.emplace_front(l_Guid);
                        l_Target->RemoveAura(uint32(Spells::DERVISH_DMG));
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

                if (!l_Unit->HasAura(uint32(Spells::DERVISH_DMG)))
                {
                    p_AreaTrigger->GetCaster()->AddAura(uint32(Spells::DERVISH_DMG), l_Unit);
                    m_Targets.emplace_front(l_Unit->GetGUID());
                }
            }
        }
    };

    // Spinning Blade - 153544
    class spell_SpinningBlade: public SpellScriptLoader
    {
    public:
        spell_SpinningBlade()
            : SpellScriptLoader("spell_SpinningBlade")
        {
        }

        enum class Spells : uint32
        {
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

    // Blade Dance - 153581
    class spell_BladeDance: public SpellScriptLoader
    {
    public:
        spell_BladeDance()
            : SpellScriptLoader("spell_BladeDance")
        {
        }

        enum class Spells : uint32
        {
            // Those are for Blade Dance
            SPINNING_BLADE_4 = 153535,
            SPINNING_BLADE_5 = 153536,
            SPINNING_BLADE_6 = 153537,
            SPINNING_BLADE_7 = 153538,
        };

        class spell_BladeDance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_BladeDance_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster() && GetHitUnit())
                {
                    GetCaster()->CastSpell(GetHitUnit(), uint32(Spells::SPINNING_BLADE_4), true);
                    GetCaster()->SetOrientation(GetCaster()->GetOrientation() + M_PI / 2);
                    GetCaster()->CastSpell(GetHitUnit(), uint32(Spells::SPINNING_BLADE_5), true);
                    GetCaster()->SetOrientation(GetCaster()->GetOrientation() + M_PI / 2);
                    GetCaster()->CastSpell(GetHitUnit(), uint32(Spells::SPINNING_BLADE_6), true);
                    GetCaster()->SetOrientation(GetCaster()->GetOrientation() + M_PI / 2);
                    GetCaster()->CastSpell(GetHitUnit(), uint32(Spells::SPINNING_BLADE_7), true);
                    GetCaster()->SetOrientation(GetCaster()->GetOrientation() + M_PI / 2);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_BladeDance_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_BladeDance_SpellScript();
        }
    };

    // Storm - 156515
    class spell_Storm: public SpellScriptLoader
    {
    public:
        spell_Storm()
            : SpellScriptLoader("spell_Storm")
        {
        }

        enum class Spells : uint32
        {
            STORM = 156515,
            STORM_AT = 156840,
            STORM_DMG = 156841,
        };

        class spell_Storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_Storm_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster() && GetHitUnit())
                    GetCaster()->CastSpell(GetHitUnit(), uint32(Spells::STORM_AT), true);
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
}

#ifndef __clang_analyzer__
void AddSC_spell_instance_skyreach()
{
    // Spells.
    new MS::AreaTrigger_solar_zone();
    new MS::AreaTrigger_spinning_blade();
    new MS::spell_SpinningBlade();
    new MS::spell_Storm();
    new MS::AreaTrigger_storm_zone();
    new MS::AreaTrigger_dervish();
    new MS::spell_BladeDance();
    new MS::spell_SolarStorm();
    new MS::AreaTrigger_SolarStorm();
    new MS::spell_FlashBang();
    new MS::AreaTrigger_ProtectiveBarrier();

    // Boss Ranjit.
    new MS::AreaTrigger_WindWall();
    new MS::spell_Windwall();
    new MS::spell_FourWind();
    new MS::AreaTrigger_FourWinds();

    // Boss Araknath.
    new MS::spell_Energize();
    new MS::spell_VisualEnergize();
    new MS::spell_VisualEnergize2();
    new MS::AreaTrigger_Smash();

    // Boss Rukhran.
    new MS::spell_SummonSolarFlare();
    new MS::spell_Sunstrike();
    new MS::spell_Quills();

    // Boss High Save Viryx.
    new MS::AreaTrigger_LensFlare();
    new MS::spell_CastDown();
}
#endif
