////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Blade Twister - 178795
class spell_ashran_blade_twister: public SpellScriptLoader
{
    public:
        spell_ashran_blade_twister() : SpellScriptLoader("spell_ashran_blade_twister") { }

        class spell_ashran_blade_twister_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_blade_twister_AuraScript);

            enum eSpells
            {
                BladeTwisterDamage = 177167
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    std::list<Creature*> l_Blades;
                    l_Target->GetCreatureListWithEntryInGrid(l_Blades, eCreatures::BladeTwisterTrigger, 50.0f);

                    if (l_Blades.empty())
                    {
                        p_AurEff->GetBase()->Remove();
                        return;
                    }

                    l_Blades.remove_if([this, l_Target](Creature* p_Creature) -> bool
                    {
                        if (!p_Creature->GetOwner())
                            return true;

                        if (p_Creature->GetOwner() != l_Target)
                            return true;

                        return false;
                    });

                    for (Creature* l_Creature : l_Blades)
                        l_Target->CastSpell(l_Creature, eSpells::BladeTwisterDamage, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ashran_blade_twister_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ashran_blade_twister_AuraScript();
        }
};

/// Living Bomb - 176670
class spell_ashran_emberfall_living_bomb: public SpellScriptLoader
{
    public:
        spell_ashran_emberfall_living_bomb() : SpellScriptLoader("spell_ashran_emberfall_living_bomb") { }

        class spell_ashran_emberfall_living_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_emberfall_living_bomb_AuraScript);

            enum eSpells
            {
                LivingBombTriggered = 176673
            };

            void AfterRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode != AuraRemoveMode::AURA_REMOVE_BY_DEATH && removeMode != AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, eSpells::LivingBombTriggered, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_ashran_emberfall_living_bomb_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ashran_emberfall_living_bomb_AuraScript();
        }
};

/// Alliance Reward - 178531
/// Horde Reward - 178533
class spell_ashran_faction_rewards : public SpellScriptLoader
{
    public:
        spell_ashran_faction_rewards() : SpellScriptLoader("spell_ashran_faction_rewards") { }

        class spell_ashran_faction_rewards_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_faction_rewards_SpellScript);

            enum Items
            {
                StrongboxHorde      = 120151,
                StrongboxAlliance   = 118065
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return true;

                    Player* l_Player = p_Object->ToPlayer();
                    if (l_Player == nullptr)
                        return true;

                    /// Only one strongbox per day
                    if (!l_Player->CanHaveDailyLootForItem(Items::StrongboxAlliance) ||
                        !l_Player->CanHaveDailyLootForItem(Items::StrongboxHorde))
                        return true;

                    return false;
                });
            }

            void HandleOnHit()
            {
                if (GetHitUnit() == nullptr)
                    return;

                if (Player* l_Player = GetHitUnit()->ToPlayer())
                {
                    if (GetSpellInfo()->Id == eAshranSpells::SpellAllianceReward)
                        l_Player->AddDailyLootCooldown(Items::StrongboxHorde);
                    else
                        l_Player->AddDailyLootCooldown(Items::StrongboxAlliance);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_faction_rewards_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnHit += SpellHitFn(spell_ashran_faction_rewards_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ashran_faction_rewards_SpellScript();
        }
};

/// Booming Shout - 177150
class spell_ashran_booming_shout : public SpellScriptLoader
{
    public:
        spell_ashran_booming_shout() : SpellScriptLoader("spell_ashran_booming_shout") { }

        class spell_ashran_booming_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_booming_shout_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (p_Object->GetEntry() == eCreatures::GaulDunFirok ||
                        p_Object->GetEntry() == eCreatures::MukmarRaz ||
                        p_Object->GetEntry() == eCreatures::KorlokTheOgreKing)
                        return true;

                    return false;
                });
            }

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        float l_MaxRadius = GetSpellInfo()->Effects[p_EffIndex].CalcRadius(l_Caster, GetSpell());
                        float l_Dist = l_Caster->GetDistance(l_Target);
                        float l_Pct = l_Dist / l_MaxRadius;
                        SetHitDamage(int32(GetHitDamage() * l_Pct));
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_booming_shout_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_booming_shout_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_ashran_booming_shout_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ashran_booming_shout_SpellScript();
        }
};

/// Curse of Krong - 165134
class spell_ashran_curse_of_krong : public SpellScriptLoader
{
    public:
        spell_ashran_curse_of_krong() : SpellScriptLoader("spell_ashran_curse_of_krong") { }

        class spell_ashran_curse_of_krong_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_curse_of_krong_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->DealDamage(l_Target, l_Target->CountPctFromMaxHealth(p_AurEff->GetAmount()), nullptr, DOT);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ashran_curse_of_krong_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ashran_curse_of_krong_AuraScript();
        }
};

void AddSC_AshranSpells()
{
    new spell_ashran_blade_twister();
    new spell_ashran_emberfall_living_bomb();
    new spell_ashran_faction_rewards();
    new spell_ashran_booming_shout();
    new spell_ashran_curse_of_krong();
}