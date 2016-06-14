////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

/// Stonebound - 129755
class spell_toy_stonebound : public SpellScriptLoader
{
    public:
        spell_toy_stonebound() : SpellScriptLoader("spell_toy_stonebound") { }

        class spell_toy_stonebound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_toy_stonebound_AuraScript);

            uint32 m_Update;

            bool Load() override
            {
                m_Update = 500;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_Update <= p_Diff)
                {
                    m_Update = 500;

                    if (Unit* l_Target = GetUnitOwner())
                    {
                        bool l_Remove = false;
                        if (l_Target->isAlive())
                            l_Remove = true;

                        if (l_Target->GetTypeId() == TypeID::TYPEID_PLAYER)
                        {
                            if (l_Target->ToPlayer()->HasFlag(EPlayerFields::PLAYER_FIELD_PLAYER_FLAGS, PlayerFlags::PLAYER_FLAGS_GHOST))
                                l_Remove = true;
                        }

                        if (l_Remove)
                            Remove();
                    }
                }
                else
                    m_Update -= p_Diff;
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_toy_stonebound_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_toy_stonebound_AuraScript();
        }
};

/// Warsong Orc Costume - 178305
class spell_toy_warsong_orc_costume : public SpellScriptLoader
{
    public:
        spell_toy_warsong_orc_costume() : SpellScriptLoader("spell_toy_warsong_orc_costume") { }

        enum eSpells
        {
            WarsongOrcCostumeMale   = 178306,
            WarsongOrcCostumeFemale = 178307
        };

        class spell_toy_warsong_orc_costume_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_toy_warsong_orc_costume_SpellScript);

            void HandleScriptEffect(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->getGender() == Gender::GENDER_MALE)
                    l_Caster->CastSpell(l_Caster, eSpells::WarsongOrcCostumeMale, true);
                else
                    l_Caster->CastSpell(l_Caster, eSpells::WarsongOrcCostumeFemale, true);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(spell_toy_warsong_orc_costume_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_toy_warsong_orc_costume_SpellScript();
        }
};

/// Flip It - 129339
class spell_toy_flippable_table : public SpellScriptLoader
{
    public:
        spell_toy_flippable_table() : SpellScriptLoader("spell_toy_flippable_table") { }

        enum eSpell
        {
            FlipIt = 129338
        };

        class spell_toy_flippable_table_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_toy_flippable_table_SpellScript);

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->CastSpell(l_Caster, eSpell::FlipIt, false);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(spell_toy_flippable_table_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_toy_flippable_table_SpellScript();
        }
};

/// Leyara's Locket - 101184
class spell_toy_leyaras_locket : public SpellScriptLoader
{
    public:
        spell_toy_leyaras_locket() : SpellScriptLoader("spell_toy_leyaras_locket") { }

        enum eSpells
        {
            LeyarasLocketMorphMale   = 101186,
            LeyarasLocketMorphFemale = 101185
        };

        class spell_toy_leyaras_locket_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_toy_leyaras_locket_SpellScript);

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->getGender() == Gender::GENDER_MALE)
                    l_Caster->CastSpell(l_Caster, eSpells::LeyarasLocketMorphMale, true);
                else
                    l_Caster->CastSpell(l_Caster, eSpells::LeyarasLocketMorphFemale, true);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(spell_toy_leyaras_locket_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_toy_leyaras_locket_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_spell_toy()
{
    new spell_toy_stonebound();
    new spell_toy_warsong_orc_costume();
    new spell_toy_flippable_table();
    new spell_toy_leyaras_locket();
}
#endif
