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

namespace eEnchantmentMarkProc
{
    enum
    {
        Thunderlord     = 159234,
        Warsong         = 159675,
        BleedingHollow  = 173322,
        Frostwolf       = 159676,
        //Blackrock       = 159679,
        //Shadowmoon      = 159678
    };
}
namespace eEnchantmentMarkAura
{
    enum
    {
        Thunderlord     = 159243,
        Warsong         = 159682,
        BleedingHollow  = 173321,
        Frostwolf       = 159683,
        //Blackrock       = 159685,
        //Shadowmoon      = 159684
    };
}

class spell_enchantment_mark : public SpellScriptLoader
{
    public:
        spell_enchantment_mark() : SpellScriptLoader("spell_enchantment_mark") { }

        class spell_enchantment_mark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_enchantment_mark_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                if (!GetOwner() || !GetOwner()->ToPlayer())
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case eEnchantmentMarkAura::Thunderlord:
                        GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), eEnchantmentMarkProc::Thunderlord, TRIGGERED_FULL_MASK);
                        break;
                    case eEnchantmentMarkAura::Warsong:
                        GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), eEnchantmentMarkProc::Warsong, TRIGGERED_FULL_MASK);
                        break;
                    case eEnchantmentMarkAura::BleedingHollow:
                        GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), eEnchantmentMarkProc::BleedingHollow, TRIGGERED_FULL_MASK);
                        break;
                    case eEnchantmentMarkAura::Frostwolf:
                        GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), eEnchantmentMarkProc::Frostwolf, TRIGGERED_FULL_MASK);
                        break;
                    /*
                     case eEnchantmentMarkAura::Blackrock:
                        GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), eEnchantmentMarkProc::Blackrock, TRIGGERED_FULL_MASK);
                        break;
                    case eEnchantmentMarkAura::Shadowmoon:
                        GetOwner()->ToPlayer()->CastSpell(GetOwner()->ToPlayer(), eEnchantmentMarkProc::Shadowmoon, TRIGGERED_FULL_MASK);
                        break;
                     */
                    default:
                        break;
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_enchantment_mark_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_enchantment_mark_AuraScript();
        }
};

/// last update : 6.1.2 19865
/// Shattered Bleed - 159238
class spell_Mark_Of_The_Shattered_Bleed : public SpellScriptLoader
{
    public:
        spell_Mark_Of_The_Shattered_Bleed() : SpellScriptLoader("spell_Mark_Of_The_Shattered_Bleed") { }

        class spell_Mark_Of_The_Shattered_Bleed_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_Mark_Of_The_Shattered_Bleed_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (l_Target == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                if (l_Caster->IsValidAssistTarget(l_Target))
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_Mark_Of_The_Shattered_Bleed_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_Mark_Of_The_Shattered_Bleed_SpellScript();
        }
};

void AddSC_spell_item_enchantment()
{
    new spell_enchantment_mark();
    new spell_Mark_Of_The_Shattered_Bleed();
}
