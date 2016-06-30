#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum RibbonPoleData
{
    SPELL_HAS_FULL_MIDSUMMER_SET = 58933,
    SPELL_BURNING_HOT_POLE_DANCE = 58934,
    SPELL_RIBBON_DANCE = 29175,
    GO_RIBBON_POLE = 181605,
};

class spell_gen_ribbon_pole_dancer_check : public SpellScriptLoader
{
    public:
        spell_gen_ribbon_pole_dancer_check() : SpellScriptLoader("spell_gen_ribbon_pole_dancer_check") { }

        class spell_gen_ribbon_pole_dancer_check_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_ribbon_pole_dancer_check_AuraScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HAS_FULL_MIDSUMMER_SET))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_RIBBON_DANCE))
                    return false;
                return true;
            }

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                Unit* target = GetTarget();

                if (!target)
                    return;

                // check if aura needs to be removed
                if (!target->FindNearestGameObject(GO_RIBBON_POLE, 20.0f) || !target->HasUnitState(UNIT_STATE_CASTING))
                {
                    target->InterruptNonMeleeSpells(false);
                    target->RemoveAurasDueToSpell(GetId());
                    return;
                }

                // set xp buff duration
                if (Aura* aur = target->GetAura(SPELL_RIBBON_DANCE))
                {
                    aur->SetMaxDuration(aur->GetMaxDuration() >= 3600000 ? 3600000 : aur->GetMaxDuration() + 180000);
                    aur->RefreshDuration();

                    // reward achievement criteria
                    if (aur->GetMaxDuration() == 3600000 && target->HasAura(SPELL_HAS_FULL_MIDSUMMER_SET))
                        //target->CastSpell(target, SPELL_BURNING_HOT_POLE_DANCE, true);
                        if (target->GetTypeId() == TYPEID_PLAYER)
                            target->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_BURNING_HOT_POLE_DANCE);
                }
                else
                    target->AddAura(SPELL_RIBBON_DANCE, target);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_ribbon_pole_dancer_check_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_ribbon_pole_dancer_check_AuraScript();
        }
};

enum TorchCatchingData
{
    SPELL_FLING_TORCH_MISSILE = 45669,
    SPELL_TOSS_TORCH_SHADOW = 46105,
    SPELL_TORCH_TARGET_PICKER = 45907,
    SPELL_TORCHES_COUGHT = 45693,
    SPELL_JUGGLE_TORCH_MISSED = 45676,
    SPELL_TORCH_CATCHING_SUCCESS = 46081,
    SPELL_TORCH_DAILY_SUCCESS = 46654,
    NPC_JUGGLE_TARGET = 25515,
    QUEST_TORCH_CATCHING_A = 11657,
    QUEST_TORCH_CATCHING_H = 11923,
    QUEST_MORE_TORCH_CATCHING_A = 11924,
    QUEST_MORE_TORCH_CATCHING_H = 11925
};

class spell_gen_torch_target_picker : public SpellScriptLoader
{
    public:
        spell_gen_torch_target_picker() : SpellScriptLoader("spell_gen_torch_target_picker") {}

        class spell_gen_torch_target_picker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_torch_target_picker_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FLING_TORCH_MISSILE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_TOSS_TORCH_SHADOW))
                    return false;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();

                if (!caster)
                    return;

                std::list<Creature*> juggleList;
                caster->GetCreatureListWithEntryInGrid(juggleList, NPC_JUGGLE_TARGET, 10.0f);

                if (!juggleList.empty())
                    for (std::list<Creature*>::iterator iter = juggleList.begin(); iter != juggleList.end(); ++iter)
                        targets.remove(*iter);

                if (targets.empty())
                    return;

                std::list<WorldObject*>::iterator itr = targets.begin();
                std::advance(itr, urand(0, targets.size() - 1));

                WorldObject* target = *itr;
                targets.clear();
                targets.push_back(target);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                caster->CastSpell(target, SPELL_FLING_TORCH_MISSILE, true);
                caster->CastSpell(target, SPELL_TOSS_TORCH_SHADOW, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_torch_target_picker_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_gen_torch_target_picker_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_torch_target_picker_SpellScript();
        }
};

class spell_gen_juggle_torch_catch : public SpellScriptLoader
{
    public:
        spell_gen_juggle_torch_catch() : SpellScriptLoader("spell_gen_juggle_torch_catch") {}

        class spell_gen_juggle_torch_catch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_juggle_torch_catch_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TORCH_TARGET_PICKER))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_TORCHES_COUGHT))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_JUGGLE_TORCH_MISSED))
                    return false;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();
                WorldObject* juggleTarget = NULL;
                bool missed = true;

                if (targets.empty() || !caster || !caster->ToPlayer())
                     return;

                for (std::list<WorldObject*>::iterator iter = targets.begin(); iter != targets.end(); ++iter)
                {
                    if (*iter == caster)
                        missed = false;

                    if ((*iter)->ToCreature())
                        juggleTarget = *iter;
                }

                if (missed)
                {
                    if (juggleTarget)
                        if (Unit* castTarget = juggleTarget->ToUnit())
                            castTarget->CastSpell(castTarget, SPELL_JUGGLE_TORCH_MISSED, true);
                    caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                }
                else
                {
                    uint8 neededCatches;

                    if (caster->ToPlayer()->GetQuestStatus(QUEST_TORCH_CATCHING_A) == QUEST_STATUS_INCOMPLETE
                        || caster->ToPlayer()->GetQuestStatus(QUEST_TORCH_CATCHING_H) == QUEST_STATUS_INCOMPLETE)
                    {
                        neededCatches = 4;
                    }
                    else if (caster->ToPlayer()->GetQuestStatus(QUEST_MORE_TORCH_CATCHING_A) == QUEST_STATUS_INCOMPLETE
                        || caster->ToPlayer()->GetQuestStatus(QUEST_MORE_TORCH_CATCHING_H) == QUEST_STATUS_INCOMPLETE)
                    {
                        neededCatches = 10;
                    }
                    else
                    {
                        caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                        return;
                    }

                    caster->CastSpell(caster, SPELL_TORCH_TARGET_PICKER, true);
                    caster->CastSpell(caster, SPELL_TORCHES_COUGHT, true);

                    // reward quest
                    if (caster->GetAuraCount(SPELL_TORCHES_COUGHT) >= neededCatches)
                    {
                        caster->CastSpell(caster, SPELL_TORCH_CATCHING_SUCCESS, true);
                        caster->CastSpell(caster, SPELL_TORCH_DAILY_SUCCESS, true);
                        caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_juggle_torch_catch_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_juggle_torch_catch_SpellScript();
        }
};

class spell_gen_throw_torch : public SpellScriptLoader
{
    public:
        spell_gen_throw_torch() : SpellScriptLoader("spell_gen_throw_torch") {}

        class spell_gen_throw_torch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_throw_torch_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitPlayer();

                if (!caster || !target)
                    return;
                
                if (caster != target)
                {
                    uint32 spellId;
                    switch (urand(0, 2))
                    {
                        case 1:
                            spellId = 45816;
                            break;
                        case 2:
                            spellId = 45806;
                            break;
                        default:
                            spellId = 45792;
                            break;
                    }
                    caster->CastSpell(target, spellId, true);
                }
                else
                {
                    caster->CastSpell(caster, 45638, true);
                    caster->CastSpell(caster, 45280, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_throw_torch_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_throw_torch_SpellScript();
        }
};

class go_midsummer_fire_festival_ribbon_pole : public GameObjectScript
{
    public:
        go_midsummer_fire_festival_ribbon_pole() : GameObjectScript("go_midsummer_fire_festival_ribbon_pole") { }

        bool OnGossipHello(Player* player, GameObject* go) override
        {
            player->CastSpell(player, 29531);

            return true;
        }
};

void AddSC_midsummer_fire_festival()
{
    new spell_gen_ribbon_pole_dancer_check();
    new spell_gen_torch_target_picker();
    new spell_gen_juggle_torch_catch();
    new spell_gen_throw_torch();

    new go_midsummer_fire_festival_ribbon_pole(); // 181605
}
