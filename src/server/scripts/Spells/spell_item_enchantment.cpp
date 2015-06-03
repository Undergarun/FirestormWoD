////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SkillDiscovery.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "InstanceScript.h"
#include "Group.h"
#include "LFGMgr.h"
#include "Player.h"
#include "GameEventMgr.h"

class spell_Mark_of_Bleeding_Hollow : public SpellScriptLoader
{
    public:
        spell_Mark_of_Bleeding_Hollow()
            : SpellScriptLoader("spell_Mark_of_Bleeding_Hollow")
        {

        }

        class spell_Mark_of_Bleeding_Hollow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_Mark_of_Bleeding_Hollow_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                if (!GetOwner() || !GetOwner()->ToPlayer())
                    return;

                GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), 173322, TRIGGERED_FULL_MASK);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_Mark_of_Bleeding_Hollow_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_Mark_of_Bleeding_Hollow_AuraScript();
        }

};

class spell_Mark_of_Blackrock : public SpellScriptLoader
{
    public:
        spell_Mark_of_Blackrock()
            : SpellScriptLoader("spell_Mark_of_Blackrock")
        {

        }

        class spell_Mark_of_Blackrock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_Mark_of_Blackrock_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                if (!GetOwner() || !GetOwner()->ToPlayer())
                    return;

                GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), 159679, TRIGGERED_FULL_MASK);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_Mark_of_Blackrock_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_Mark_of_Blackrock_AuraScript();
        }

};

class spell_Mark_of_Thunderlord : public SpellScriptLoader
{
    public:
        spell_Mark_of_Thunderlord()
            : SpellScriptLoader("spell_Mark_of_Thunderlord")
        {

        }

        class spell_Mark_of_Thunderlord_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_Mark_of_Thunderlord_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                if (!GetOwner() || !GetOwner()->ToPlayer())
                    return;

                GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), 159234, TRIGGERED_FULL_MASK);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_Mark_of_Thunderlord_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_Mark_of_Thunderlord_AuraScript();
        }

};

class spell_Mark_of_Warsong : public SpellScriptLoader
{
    public:
        spell_Mark_of_Warsong()
            : SpellScriptLoader("spell_Mark_of_Warsong")
        {

        }

        class spell_Mark_of_Warsong_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_Mark_of_Warsong_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                if (!GetOwner() || !GetOwner()->ToPlayer())
                    return;

                GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), 159675, TRIGGERED_FULL_MASK);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_Mark_of_Warsong_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_Mark_of_Warsong_AuraScript();
        }

};

void AddSC_spell_item_enchantment()
{
    new spell_Mark_of_Bleeding_Hollow();
    new spell_Mark_of_Blackrock();
    new spell_Mark_of_Thunderlord();
    new spell_Mark_of_Warsong();
}
