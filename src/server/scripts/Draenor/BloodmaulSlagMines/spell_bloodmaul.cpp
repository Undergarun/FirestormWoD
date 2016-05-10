#include "instance_bloodmaul.hpp"

namespace MS
{
    namespace Instances
    {
        namespace Bloodmaul
        {
            // Lava splash - 152809
            class spell_LavaSplash: public SpellScriptLoader
            {
            public:
                spell_LavaSplash()
                    : SpellScriptLoader("spell_LavaSplash")
                {
                }

                enum class Spells : uint32
                {
                    LavaExplosion = 152832,
                };

                class spell_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_SpellScript);

                    void HandleDummy(SpellEffIndex /*effIndex*/)
                    {
                        Unit* l_Caster = GetCaster();
                        Unit* l_Target = GetHitUnit();
                        if (!l_Caster || !l_Target)
                            return;

                        l_Target->CastSpell(l_Target, uint32(Spells::LavaExplosion), true);
                    }

                    void Register()
                    {
                        OnEffectHitTarget += SpellEffectFn(spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                    }
                };

                SpellScript* GetSpellScript() const
                {
                    return new spell_SpellScript();
                }
            };

            // Volcanic Eruption - 151698
            class spell_VolcanicEruption: public SpellScriptLoader
            {
            public:
                spell_VolcanicEruption()
                    : SpellScriptLoader("spell_VolcanicEruption")
                {
                }

                enum class Spells : uint32
                {
                    VolcanicEruption = 152499,
                };

                class spell_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_SpellScript);

                    void HandleDummy(SpellEffIndex /*effIndex*/)
                    {
                        Unit* l_Caster = GetCaster();
                        if (!l_Caster)
                            return;

                        if (Unit* l_Unit = ScriptUtils::SelectRandomEnnemy(l_Caster, 30.0f))
                            l_Caster->CastSpell(l_Unit, uint32(Spells::VolcanicEruption), true);
                    }

                    void Register()
                    {
                        OnEffectHit += SpellEffectFn(spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                    }
                };

                SpellScript* GetSpellScript() const
                {
                    return new spell_SpellScript();
                }
            };

            // Raise the miners - 150801
            class spell_RaiseTheMiners: public SpellScriptLoader
            {
            public:
                spell_RaiseTheMiners()
                    : SpellScriptLoader("spell_RaiseTheMiners")
                {
                }

                enum class Spells : uint32
                {
                    SuppresionField = 151582,
                };

                class spell_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_SpellScript);

                    void HandleDummy(SpellEffIndex /*effIndex*/)
                    {
                        Unit* l_Caster = GetCaster();
                        if (l_Caster && l_Caster->GetInstanceScript())
                            l_Caster->GetInstanceScript()->SetData(uint32(Data::RaiseTheMiners), 0);
                    }

                    void Register()
                    {
                        OnEffectHit += SpellEffectFn(spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                    }
                };

                SpellScript* GetSpellScript() const
                {
                    return new spell_SpellScript();
                }
            };

            // FerociousYell - 150759
            class spell_FerociousYell: public SpellScriptLoader
            {
            public:
                spell_FerociousYell()
                    : SpellScriptLoader("spell_FerociousYell")
                {
                }

                class spell_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_SpellScript);

                    void CheckTarget(std::list<WorldObject*>& unitList)
                    {
                        Unit* l_Caster = GetCaster();
                        unitList.remove_if([l_Caster](WorldObject* p_Obj) {
                            return !(p_Obj->GetEntry() == uint32(MobEntries::CapturedMiner1)
                                || p_Obj->GetEntry() == uint32(MobEntries::CapturedMiner2)
                                || p_Obj->GetEntry() == uint32(MobEntries::CapturedMiner3));
                        });
                    }

                    void Register()
                    {
                        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_SpellScript::CheckTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                    }
                };

                SpellScript* GetSpellScript() const
                {
                    return new spell_SpellScript();
                }
            };

            // SuppressField - 151581
            class spell_SuppressionField: public SpellScriptLoader
            {
            public:
                spell_SuppressionField()
                    : SpellScriptLoader("spell_SuppressionField")
                {
                }

                enum class Spells : uint32
                {
                    SuppresionField = 151582,
                };

                class spell_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_SpellScript);

                    void HandleDummy(SpellEffIndex /*effIndex*/)
                    {
                        Unit* l_Caster = GetCaster();
                        if (l_Caster)
                        {
                            if (Player* l_Plr = ScriptUtils::SelectRandomPlayerIncludedTank(l_Caster, 40.0f))
                                l_Caster->CastSpell(l_Plr, uint32(Spells::SuppresionField), true);
                        }
                    }

                    void Register()
                    {
                        OnEffectHit += SpellEffectFn(spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                    }
                };

                SpellScript* GetSpellScript() const
                {
                    return new spell_SpellScript();
                }
            };

            // Chain Grip - 151990
            class spell_ChainGrip: public SpellScriptLoader
            {
            public:
                spell_ChainGrip()
                    : SpellScriptLoader("spell_ChainGrip")
                {
                }

                enum class Spells : uint32
                {
                    ChainGrip = 152024,
                    ViciousSlash = 152043,
                };

                class spell_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_SpellScript);

                    void HandleDummy(SpellEffIndex /*effIndex*/)
                    {
                        Unit* l_Caster = GetCaster();
                        Unit* l_Target = GetHitUnit();
                        if (l_Caster && l_Target)
                        {
                            l_Caster->CastSpell(l_Target, uint32(Spells::ChainGrip), true);
                            l_Target->GetMotionMaster()->MoveJump(l_Caster->GetPositionX(), l_Caster->GetPositionY(), l_Caster->GetPositionZ(), 25.0f, 2.0f);
                            l_Caster->CastSpell(l_Target, uint32(Spells::ViciousSlash), false);
                        }
                    }

                    void Register()
                    {
                        OnEffectHitTarget += SpellEffectFn(spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                    }
                };

                SpellScript* GetSpellScript() const
                {
                    return new spell_SpellScript();
                }
            };
        }
    }
}

#ifndef __clang_analyzer__
void AddSC_spell_Bloodmaul()
{
    new MS::Instances::Bloodmaul::spell_ChainGrip();
    new MS::Instances::Bloodmaul::spell_SuppressionField();
    new MS::Instances::Bloodmaul::spell_FerociousYell();
    new MS::Instances::Bloodmaul::spell_RaiseTheMiners();
    new MS::Instances::Bloodmaul::spell_VolcanicEruption();
    new MS::Instances::Bloodmaul::spell_LavaSplash();
}
#endif
