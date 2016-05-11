////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

//! TODO - Boss not scripted, just ported required spellscript from core

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

enum Spells
{
    SPELL_POSITIVE_POLARITY         = 39088,
    SPELL_POSITIVE_CHARGE_STACK     = 39089,
    SPELL_POSITIVE_CHARGE           = 39090,
    SPELL_NEGATIVE_POLARITY         = 39091,
    SPELL_NEGATIVE_CHARGE_STACK     = 39092,
    SPELL_NEGATIVE_CHARGE           = 39093
};

class spell_capacitus_polarity_charge: public SpellScriptLoader
{
    public:
        spell_capacitus_polarity_charge() : SpellScriptLoader("spell_capacitus_polarity_charge") { }

        class spell_capacitus_polarity_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_capacitus_polarity_charge_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_POSITIVE_CHARGE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_POSITIVE_CHARGE_STACK))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_NEGATIVE_CHARGE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_NEGATIVE_CHARGE_STACK))
                    return false;
                return true;
            }

            void HandleTargets(std::list<WorldObject*>& targetList)
            {
                uint8 count = 0;
                for (std::list<WorldObject*>::iterator ihit = targetList.begin(); ihit != targetList.end(); ++ihit)
                    if ((*ihit)->GetGUID() != GetCaster()->GetGUID())
                        if (Player* target = (*ihit)->ToPlayer())
                            if (target->HasAura(GetTriggeringSpell()->Id))
                                ++count;

                if (count)
                {
                    uint32 spellId = 0;

                    if (GetSpellInfo()->Id == SPELL_POSITIVE_CHARGE)
                        spellId = SPELL_POSITIVE_CHARGE_STACK;
                    else // if (GetSpellInfo()->Id == SPELL_NEGATIVE_CHARGE)
                        spellId = SPELL_NEGATIVE_CHARGE_STACK;

                    GetCaster()->SetAuraStack(spellId, GetCaster(), count);
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (!GetTriggeringSpell())
                    return;

                Unit* target = GetHitUnit();

                if (target->HasAura(GetTriggeringSpell()->Id))
                    SetHitDamage(0);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_capacitus_polarity_charge_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_capacitus_polarity_charge_SpellScript::HandleTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_capacitus_polarity_charge_SpellScript();
        }
};

class spell_capacitus_polarity_shift: public SpellScriptLoader
{
    public:
        spell_capacitus_polarity_shift() : SpellScriptLoader("spell_capacitus_polarity_shift") { }

        class spell_capacitus_polarity_shift_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_capacitus_polarity_shift_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_POSITIVE_POLARITY) || !sSpellMgr->GetSpellInfo(SPELL_NEGATIVE_POLARITY))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* target = GetHitUnit();
                Unit* caster = GetCaster();

                target->CastSpell(target, roll_chance_i(50) ? SPELL_POSITIVE_POLARITY : SPELL_NEGATIVE_POLARITY, true, NULL, nullptr, caster->GetGUID());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_capacitus_polarity_shift_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_capacitus_polarity_shift_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_mechano_lord_capacitus()
{
    new spell_capacitus_polarity_charge();
    new spell_capacitus_polarity_shift();
}
#endif
