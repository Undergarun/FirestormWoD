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
        Blackrock       = 159679,
        Shadowmoon      = 159678
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
        Blackrock       = 159685,
        Shadowmoon      = 159684
    };
}

namespace eEnchantmentMarkIds
{
    enum
    {
        Thunderlord     = 5330,
        Warsong         = 5337,
        BleedingHollow  = 5384,
        Frostwolf       = 5334,
        Blackrock       = 5336,
        Shadowmoon      = 5335
    };
}

namespace eGloryEnchantmentMarkIds
{
    enum
    {
        Thunderlord = 5352,
        Warsong = 5355,
        Frostwolf = 5356,
        Blackrock = 5354,
        Shadowmoon = 5353
    };
}

/// Mark of the Thunderlord - 159243
/// Mark of Warsong - 159682
/// Mark of the Frostwolf - 159683
/// Mark of Blackrock - 159685
/// Mark of Bleeding Hollow - 173321
class spell_enchantment_mark : public SpellScriptLoader
{
    public:
        spell_enchantment_mark() : SpellScriptLoader("spell_enchantment_mark") { }

        class spell_enchantment_mark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_enchantment_mark_AuraScript);

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                if (!GetOwner())
                    return;

                Player* l_Player = GetOwner()->ToPlayer();
                if (l_Player == nullptr)
                    return;

                uint32 l_SpellID = GetSpellInfo()->Id;
                if (l_SpellID == eEnchantmentMarkAura::Blackrock && l_Player->GetHealthPct() >= 60.0f)
                {
                    PreventDefaultAction();
                    return;
                }

                uint32 l_ProcAuraId     = 0;
                uint32 l_EnchantId      = 0;
                uint32 l_GloryEnchantId = 0;
                bool l_HasEnchant       = false;

                Item* l_Item = nullptr;
                if (p_EventInfo.GetTypeMask() & ProcFlags::PROC_FLAG_DONE_OFFHAND_ATTACK)
                    l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND);
                else
                    l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND);

                if (l_Item == nullptr)
                    return;

                switch (l_SpellID)
                {
                    case eEnchantmentMarkAura::Thunderlord:
                        l_ProcAuraId = eEnchantmentMarkProc::Thunderlord;
                        l_EnchantId = eEnchantmentMarkIds::Thunderlord;
                        l_GloryEnchantId = eGloryEnchantmentMarkIds::Thunderlord;
                        break;
                    case eEnchantmentMarkAura::Warsong:
                        l_ProcAuraId = eEnchantmentMarkProc::Warsong;
                        l_EnchantId = eEnchantmentMarkIds::Warsong;
                        l_GloryEnchantId = eGloryEnchantmentMarkIds::Warsong;
                        break;
                    case eEnchantmentMarkAura::BleedingHollow:
                        l_ProcAuraId = eEnchantmentMarkProc::BleedingHollow;
                        l_EnchantId = eEnchantmentMarkIds::BleedingHollow;
                        break;
                    case eEnchantmentMarkAura::Frostwolf:
                        l_ProcAuraId = eEnchantmentMarkProc::Frostwolf;
                        l_EnchantId = eEnchantmentMarkIds::Frostwolf;
                        l_GloryEnchantId = eGloryEnchantmentMarkIds::Frostwolf;
                        break;
                    case eEnchantmentMarkAura::Shadowmoon:
                        l_ProcAuraId = eEnchantmentMarkProc::Shadowmoon;
                        l_EnchantId = eEnchantmentMarkIds::Shadowmoon;
                        l_GloryEnchantId = eGloryEnchantmentMarkIds::Shadowmoon;
                        break;
                     case eEnchantmentMarkAura::Blackrock:
                        l_ProcAuraId = eEnchantmentMarkProc::Blackrock;
                        l_EnchantId = eEnchantmentMarkIds::Blackrock;
                        l_GloryEnchantId = eGloryEnchantmentMarkIds::Blackrock;
                        break;
                    default:
                        break;
                }

                if (!l_Item || l_Player->HasSpellCooldown(l_ProcAuraId))
                    return;

                /// Check if we have this enchant on that weapon
                for (uint32 l_Slot = EnchantmentSlot::PERM_ENCHANTMENT_SLOT; l_Slot < EnchantmentSlot::MAX_ENCHANTMENT_SLOT; ++l_Slot)
                {
                    if ((l_EnchantId && l_EnchantId == l_Item->GetEnchantmentId(EnchantmentSlot(l_Slot))) ||
                        (l_GloryEnchantId && l_GloryEnchantId == l_Item->GetEnchantmentId(EnchantmentSlot(l_Slot))))
                        l_HasEnchant = true;
                }

                if (l_HasEnchant && l_Player && l_ProcAuraId && l_Item)
                {
                    l_Player->CastSpell(l_Player, l_ProcAuraId, true, l_Item);
                    l_Player->AddSpellCooldown(l_ProcAuraId, 0, 1 * TimeConstants::IN_MILLISECONDS, true);
                }
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eEnchantmentMarkAura::Shadowmoon:
                    case eEnchantmentMarkAura::Blackrock:
                        OnEffectProc += AuraEffectProcFn(spell_enchantment_mark_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
                        break;
                    default:
                        OnEffectProc += AuraEffectProcFn(spell_enchantment_mark_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
                        break;
                }
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

#ifndef __clang_analyzer__
void AddSC_spell_item_enchantment()
{
    new spell_enchantment_mark();
    new spell_Mark_Of_The_Shattered_Bleed();
}
#endif