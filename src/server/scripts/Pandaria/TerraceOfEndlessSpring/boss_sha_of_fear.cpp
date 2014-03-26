/*
 * Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"

enum eShaOfFearSpells
{
    // Sha of Fear
    SPELL_REACHING_ATTACK           = 119775,
    SPELL_EERIE_SKULL               = 119519,
    SPELL_ENERGY_TO_ZERO            = 119417,
    SPELL_BREATH_OF_FEAR            = 119414,
    SPELL_THRASH_AURA               = 131996,
    SPELL_THRASH_EXTRA_ATTACKS      = 131994,
    SPELL_CONJURE_TERROR_SPAWN_TICK = 119108,
    // 4 spells for spawn, cauz' of different spawn coordinates
    SPELL_CONJURE_TERROR_SPAWN_01   = 119312,
    SPELL_CONJURE_TERROR_SPAWN_02   = 119370,
    SPELL_CONJURE_TERROR_SPAWN_03   = 119371,
    SPELL_CONJURE_TERROR_SPAWN_04   = 119372,

    // Other mobs
    SPELL_LIGHT_WALL                = 117865,
    SPELL_CHAMPION_OF_LIGHT         = 117866,
    SPELL_LIGHT_WALL_READY          = 117770,
    SPELL_LIGHT_WALL_VISUAL         = 107141,
    SPELL_FEARLESS                  = 118977,
    SPELL_WALL_OF_LIGHT_BUFF        = 117999,
    SPELL_PENETRATING_BOLT          = 129075,
    SPELL_PENETRATING_BOLT_MISSILE  = 129077,
    SPELL_DARK_BULWARK              = 119083
};

enum eShaOfFearEvents
{
    EVENT_CHECK_MELEE       = 1,
    EVENT_EERIE_SKULL       = 2,
    EVENT_CHECK_ENERGY      = 3,
    EVENT_FIRST_TERRORS     = 4,
    EVENT_PENETRATING_BOLT  = 5
};

enum eShaOfFearActions
{
    ACTION_ACTIVATE_WALL_OF_LIGHT,
    ACTION_DESACTIVATE_WALL_OF_LIGHT,
    ACTION_SPAWN_TERROR
};

enum eShaOfFearSays
{
    TALK_INTRO,
    TALK_AGGRO,
    TALK_BREATH_OF_FEAR,
    TALK_SLAY,
    TALK_SLAY_HEROIC,
    TALK_SUBMERGE,
    TALK_HUDDLE
};

Player* GetChampionOfLight(Creature* me)
{
    if (!me)
        return NULL;

    Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
        if (Player* player = itr->getSource())
            if (player->HasAura(SPELL_CHAMPION_OF_LIGHT))
                return player;

    return NULL;
}

Position spawnTerrorPos[4] =
{
    { -1052.588f, -2788.164f, 38.268f, 0.0f },
    { -983.4550f, -2787.942f, 38.269f, 0.0f },
    { -989.6860f, -2772.245f, 38.303f, 0.0f },
    { -1046.274f, -2772.215f, 38.303f, 0.0f }
};

class boss_sha_of_fear : public CreatureScript
{
    public:
        boss_sha_of_fear() : CreatureScript("boss_sha_of_fear") { }

        struct boss_sha_of_fearAI : public BossAI
        {
            boss_sha_of_fearAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_FEAR)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            uint8 attacksCounter;
            uint8 terrorCounter;

            void Reset()
            {
                _Reset();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetPower(POWER_ENERGY, 0);
                me->SetInt32Value(UNIT_FIELD_POWER1, 0);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetInt32Value(UNIT_FIELD_MAXPOWER1, 100);
                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->CastSpell(me, SPELL_ENERGY_TO_ZERO, true);
 
                summons.DespawnAll();

                events.Reset();

                events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                events.ScheduleEvent(EVENT_EERIE_SKULL, 5000);
                events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                events.ScheduleEvent(EVENT_FIRST_TERRORS, 30000);

                attacksCounter = 0;
                terrorCounter  = 0;
 
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
 
                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);
                }
            }

            void JustReachedHome()
            {
                _JustReachedHome();
 
                if (pInstance)
                    pInstance->SetBossState(DATA_SHA_OF_FEAR, FAIL);
            }

            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_SHA_OF_FEAR, IN_PROGRESS);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                    Talk(TALK_AGGRO);
                }
            }

            void JustDied(Unit* killer)
            {
                if (pInstance)
                {
                    summons.DespawnAll();
                    pInstance->SetBossState(DATA_SHA_OF_FEAR, DONE);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    _JustDied();
                }
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == NPC_TERROR_SPAWN)
                    ++terrorCounter;

                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                if (summon->GetEntry() == NPC_TERROR_SPAWN)
                    --terrorCounter;

                summons.Despawn(summon);
            }

            void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                {
                    if (IsHeroic())
                        Talk(TALK_SLAY_HEROIC);
                    else
                        Talk(TALK_SLAY);
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SPAWN_TERROR:
                    {
                        if (!terrorCounter)
                        {
                            me->CastSpell(spawnTerrorPos[0].GetPositionX(), spawnTerrorPos[0].GetPositionY(),
                                          spawnTerrorPos[0].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_01, true);
                            me->CastSpell(spawnTerrorPos[1].GetPositionX(), spawnTerrorPos[1].GetPositionY(),
                                          spawnTerrorPos[1].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_02, true);
                        }
                        else
                        {
                            me->CastSpell(spawnTerrorPos[2].GetPositionX(), spawnTerrorPos[2].GetPositionY(),
                                          spawnTerrorPos[2].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_03, true);
                            me->CastSpell(spawnTerrorPos[3].GetPositionX(), spawnTerrorPos[3].GetPositionY(),
                                          spawnTerrorPos[3].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_04, true);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageDealt(Unit* /*victim*/, uint32& /*damage*/, DamageEffectType damageType)
            {
                if (damageType == DIRECT_DAMAGE)
                {
                    if (attacksCounter >= 3 && !me->m_extraAttacks)
                    {
                        me->CastSpell(me, SPELL_THRASH_EXTRA_ATTACKS, true);
                        attacksCounter = 0;
                    }
                    else if (attacksCounter >= 2 && !me->m_extraAttacks)
                    {
                        me->CastSpell(me, SPELL_THRASH_AURA, true);
                        ++attacksCounter;
                    }
                    else if (!me->m_extraAttacks)
                    {
                        me->RemoveAura(SPELL_THRASH_AURA);
                        ++attacksCounter;
                    }
                }
            }

            void OnAddThreat(Unit* /*victim*/, float& fThreat, SpellSchoolMask /*schoolMask*/, SpellInfo const* /*threatSpell*/)
            {
                fThreat = 0;
                return;
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (!me->isInCombat())
                {
                    value = 0;
                    return;
                }

                // Sha of Fear regenerates 6 energy every 2s (15 energy for 5s)
                value = 6;

                int32 val = me->GetPower(POWER_ENERGY);
                if (val + value > 100)
                    val = 100;
                else
                    val += value;

                me->SetInt32Value(UNIT_FIELD_POWER1, val);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                        me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);

                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
 
                events.Update(diff);
 
                switch (events.ExecuteEvent())
                {
                    case EVENT_CHECK_MELEE:
                    {
                        if (!me->IsWithinMeleeRange(me->getVictim(), 2.0f))
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f))
                                me->CastSpell(target, SPELL_REACHING_ATTACK, false);
                        }
                        else
                        {
                            // Always attack champion of light
                            if (Player* target = GetChampionOfLight(me))
                                if (me->getVictim() && me->getVictim()->GetGUID() != target->GetGUID())
                                    AttackStart(target);
                        }
                        events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                        break;
                    }
                    case EVENT_EERIE_SKULL:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 60.0f))
                            me->CastSpell(target, SPELL_EERIE_SKULL, false);
                        events.ScheduleEvent(EVENT_EERIE_SKULL, 5000);
                        break;
                    }
                    case EVENT_CHECK_ENERGY:
                    {
                        if (!pInstance)
                        {
                            events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                            break;
                        }

                        if (me->GetPower(POWER_ENERGY) >= me->GetMaxPower(POWER_ENERGY))
                        {
                            if (Creature* pureLight = Creature::GetCreature(*me, pInstance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                                me->CastSpell(pureLight, SPELL_BREATH_OF_FEAR, false);
                            Talk(TALK_BREATH_OF_FEAR);
                            me->SetPower(POWER_ENERGY, 0);
                            me->SetInt32Value(UNIT_FIELD_POWER1, 0);
                        }

                        events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                        break;
                    }
                    case EVENT_FIRST_TERRORS:
                    {
                        me->CastSpell(me, SPELL_CONJURE_TERROR_SPAWN_TICK, true);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_fearAI(creature);
        }
};

// Pure Light Terrace - 60788
class mob_pure_light_terrace : public CreatureScript
{
    public:
        mob_pure_light_terrace() : CreatureScript("mob_pure_light_terrace") { }

        struct mob_pure_light_terraceAI : public ScriptedAI
        {
            mob_pure_light_terraceAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                wallActivated = false;
            }

            InstanceScript* pInstance;

            bool wallActivated;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
                me->CastSpell(me, SPELL_LIGHT_WALL, true);
                me->CastSpell(me, SPELL_LIGHT_WALL_READY, true);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ACTIVATE_WALL_OF_LIGHT:
                    {
                        if (wallActivated)
                            break;

                        wallActivated = true;
                        me->CastSpell(me, SPELL_LIGHT_WALL_VISUAL, true);
                        me->RemoveAura(SPELL_LIGHT_WALL_READY);
                        break;
                    }
                    case ACTION_DESACTIVATE_WALL_OF_LIGHT:
                    {
                        if (!wallActivated)
                            break;

                        wallActivated = false;
                        me->CastSpell(me, SPELL_LIGHT_WALL_READY, true);
                        me->RemoveAura(SPELL_LIGHT_WALL_VISUAL);
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                damage = 0;
                return;
            }

            void UpdateAI(const uint32 diff)
            {
                Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    if (Player* player = itr->getSource())
                    {
                        if (player->isInFront(me, M_PI / 3) && !player->HasAura(SPELL_WALL_OF_LIGHT_BUFF))
                            player->CastSpell(player, SPELL_WALL_OF_LIGHT_BUFF, true);
                        else
                            player->RemoveAura(SPELL_WALL_OF_LIGHT_BUFF);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_pure_light_terraceAI(creature);
        }
};

// Return to the Terrace - 65736
class mob_return_to_the_terrace : public CreatureScript
{
    public:
        mob_return_to_the_terrace() : CreatureScript("mob_return_to_the_terrace") { }

        struct mob_return_to_the_terraceAI : public ScriptedAI
        {
            mob_return_to_the_terraceAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            void Reset()
            {
                // Sniffed values
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->SetFlag(UNIT_FIELD_INTERACT_SPELL_ID, 118977);
            }

            void UpdateAI(const uint32 diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_return_to_the_terraceAI(creature);
        }
};

// Terror Spawn - 61034
class mob_terror_spawn : public CreatureScript
{
    public:
        mob_terror_spawn() : CreatureScript("mob_terror_spawn") { }

        struct mob_terror_spawnAI : public ScriptedAI
        {
            mob_terror_spawnAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                if (pInstance)
                    if (Creature* pureLight = Creature::GetCreature(*me, pInstance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                        me->SetFacingToObject(pureLight);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                me->CastSpell(me, SPELL_DARK_BULWARK, true);

                events.Reset();
                events.ScheduleEvent(EVENT_PENETRATING_BOLT, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_PENETRATING_BOLT:
                    {
                        me->CastSpell(me, SPELL_PENETRATING_BOLT, false);
                        events.ScheduleEvent(EVENT_PENETRATING_BOLT, 5000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_terror_spawnAI(creature);
        }
};

// Champion of Light - 117866
class spell_champion_of_light : public SpellScriptLoader
{
    public:
        spell_champion_of_light() : SpellScriptLoader("spell_champion_of_light") { }

        class spell_champion_of_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_champion_of_light_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->ToCreature() && caster->ToCreature()->AI())
                        caster->ToCreature()->AI()->DoAction(ACTION_ACTIVATE_WALL_OF_LIGHT);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->ToCreature() && caster->ToCreature()->AI())
                        caster->ToCreature()->AI()->DoAction(ACTION_DESACTIVATE_WALL_OF_LIGHT);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_champion_of_light_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_champion_of_light_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_champion_of_light_AuraScript();
        }
};

// Breath of Fear - 119414 / 125786
class spell_breath_of_fear : public SpellScriptLoader
{
    public:
        spell_breath_of_fear() : SpellScriptLoader("spell_breath_of_fear") { }

        class spell_breath_of_fear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_breath_of_fear_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                // Breath of Fear only affects targets not in the wall of light
                Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
                if (!players.isEmpty())
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        if (Player* player = itr->getSource())
                            if (!GetCaster()->isInFront(player, M_PI / 3))
                                targets.push_back(player);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_breath_of_fear_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_breath_of_fear_SpellScript();
        }
};

// Conjure Terror Spawn - 119108
class spell_conjure_terror_spawn : public SpellScriptLoader
{
    public:
        spell_conjure_terror_spawn() : SpellScriptLoader("spell_conjure_terror_spawn") { }

        class spell_conjure_terror_spawn_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_conjure_terror_spawn_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                if (Creature* caster = GetCaster()->ToCreature())
                    caster->AI()->DoAction(ACTION_SPAWN_TERROR);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_conjure_terror_spawn_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_conjure_terror_spawn_AuraScript();
        }
};

// Penetrating Bolt - 129075
class spell_penetrating_bolt : public SpellScriptLoader
{
    public:
        spell_penetrating_bolt() : SpellScriptLoader("spell_penetrating_bolt") { }

        class spell_penetrating_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_penetrating_bolt_SpellScript);

            uint64 targetGuid;

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                targetGuid = 0;

                if (!targets.empty())
                    JadeCore::Containers::RandomResizeList(targets, 1);

                for (auto itr : targets)
                    if (itr->GetGUID())
                        targetGuid = itr->GetGUID();
            }

            void HandleDummy(SpellEffIndex index)
            {
                if (Unit* caster = GetCaster())
                {
                    if (InstanceScript* instance = caster->GetInstanceScript())
                    {
                        if (Player* target = Player::GetPlayer(*caster, targetGuid))
                            caster->CastSpell(target, SPELL_PENETRATING_BOLT_MISSILE, true, NULL, NULLAURA_EFFECT, instance->GetData64(NPC_SHA_OF_FEAR));
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_penetrating_bolt_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectLaunch += SpellEffectFn(spell_penetrating_bolt_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_penetrating_bolt_SpellScript();
        }
};

void AddSC_boss_sha_of_fear()
{
    new boss_sha_of_fear();
    new mob_pure_light_terrace();
    new mob_return_to_the_terrace();
    new mob_terror_spawn();
    new spell_champion_of_light();
    new spell_breath_of_fear();
    new spell_conjure_terror_spawn();
    new spell_penetrating_bolt();
}
