/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "throne_of_thunder.h"
#include "Vehicle.h"
#include "ThreatManager.h"

enum eSpells
{
    // Kaz'Ra Jin
    SPELL_RECKLESS_CHARGE_AREATRIGGER    = 138026,
    SPELL_RECKLESS_CHARGE_ROLLING        = 137117, // aura
    SPELL_RECKLESS_CHARGE_MOVEMENT       = 137131,
    SPELL_RECKLESS_CHARGE_DAMAGE         = 137133,
    SPELL_RECKLESS_CHARGE_KNOCK_BACK     = 137122,
    // Gara'Jal's Soul
    SPELL_LINGERING_PRESENCE             = 136467,
    SPELL_POSSESSED                      = 136442,
    // Frost King Malakk
    SPELL_FRIGID_ASSAULT                 = 136904,
    SPELL_FRIGID_ASSAULT_STUN            = 136910,
    SPELL_BITING_COLD                    = 136917,
    SPELL_BITING_COLD_AURA               = 136992,
    SPELL_FROSTBITE                      = 136990,
    SPELL_FROSTBITE_PERIODIC             = 136922,
    SPELL_
};

enum eEvents
{
    EVENT_RECKLESS_CHARGE                        = 1,
    EVENT_RECKLESS_CHARGE_AREATRIGGER            = 2,
    EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG        = 3,
    EVENT_FRIGID_ASSAULT                         = 4,
    EVENT_LINGERING_PRESENCE_MALAKK              = 5,
    EVENT_LINGERING_PRESENCE_KAZRA_JIN           = 6,
    EVENT_LINGERING_PRESENCE_HIGH_PRIESTRESS     = 7,
    EVENT_LINGERING_PRESENCE_SUL_THE_SANDCRAWLER = 8,
    EVENT_BITING_COLD                            = 9,
    EVENT_FROSTBITE                              = 10
};

enum eSays
{
};

enum eActions
{
    ACTION_SCHEDULE_AT_DMG                         = 1,
    ACTION_SCHEDULE_FRIGID_ASSAULT                 = 2,
    ACTION_SCHEDULE_POSSESSION_MALAKK              = 3,
    ACTION_SCHEDULE_POSSESSION_KAZRA_JIN           = 4,
    ACTION_SCHEDULE_POSSESSION_HIGH_PRIESTRESS     = 5,
    ACTION_SCHEDULE_POSSESSION_SUL_THE_SANDCRAWLER = 6,
    ACTION_SCHEDULE_FROSTBITE                      = 7
};

enum ePhases
{
};

enum eDatas
{
    DATA_LINGERING_PRESENCE_COEF
};

void StartFight(InstanceScript* instance, Creature* me, Unit* /*target*/)
{
    if (!instance)
        return;

    if (instance->GetBossState(DATA_CONCIL_OF_ELDERS) == IN_PROGRESS)
        return; // Prevent recursive calls

    instance->SetBossState(DATA_CONCIL_OF_ELDERS, IN_PROGRESS);

    Creature* kazraJin = instance->instance->GetCreature(instance->GetData64(NPC_KAZRA_JIN));
    if (kazraJin)
        kazraJin->SetInCombatWithZone();

    Creature* sulTheSandCrawler = instance->instance->GetCreature(instance->GetData64(NPC_SUL_THE_SANDCRAWLER));
    if (sulTheSandCrawler)
        sulTheSandCrawler->SetInCombatWithZone();

    Creature* highPriestress = instance->instance->GetCreature(instance->GetData64(NPC_HIGH_PRIESTRESS_MAR_LI));
    if (highPriestress)
        highPriestress->SetInCombatWithZone();
    
    Creature* malakk = instance->instance->GetCreature(instance->GetData64(NPC_FROST_KING_MALAKK));
    if (malakk)
        malakk->SetInCombatWithZone();

    if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
        if (garaJalSoul->GetAI())
            garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_MALAKK);
}

// Gara'Jal's Soul - 69182
class npc_gara_jal_s_soul : public CreatureScript
{
    public:
        npc_gara_jal_s_soul() : CreatureScript("npc_gara_jal_s_soul") { }

        struct npc_gara_jal_s_soulAI : public ScriptedAI
        {
            npc_gara_jal_s_soulAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();

            }

            void SetGUID(uint64 guid, int32 id /*= 0*/)
            {
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_SCHEDULE_POSSESSION_MALAKK:
                        events.ScheduleEvent(EVENT_LINGERING_PRESENCE_MALAKK, 10000);
                        break;
                    case ACTION_SCHEDULE_POSSESSION_KAZRA_JIN:
                        events.ScheduleEvent(EVENT_LINGERING_PRESENCE_KAZRA_JIN, 1000);
                        break;
                    case ACTION_SCHEDULE_POSSESSION_HIGH_PRIESTRESS:
                        events.ScheduleEvent(EVENT_LINGERING_PRESENCE_HIGH_PRIESTRESS, 1000);
                        break;
                    case ACTION_SCHEDULE_POSSESSION_SUL_THE_SANDCRAWLER:
                        events.ScheduleEvent(EVENT_LINGERING_PRESENCE_SUL_THE_SANDCRAWLER, 1000);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_LINGERING_PRESENCE_MALAKK:
                        if (Creature* malakk = pInstance->instance->GetCreature(pInstance->GetData64(NPC_FROST_KING_MALAKK)))
                        {
                            me->AddAura(SPELL_POSSESSED, malakk);

                            if (malakk->GetAI())
                                malakk->AI()->DoAction(ACTION_SCHEDULE_FROSTBITE);
                        }
                        break;
                    case EVENT_LINGERING_PRESENCE_KAZRA_JIN:
                        if (Creature* kazraJin = pInstance->instance->GetCreature(pInstance->GetData64(NPC_KAZRA_JIN)))
                        {
                            me->AddAura(SPELL_POSSESSED, kazraJin);
                        }
                        break;
                    case EVENT_LINGERING_PRESENCE_HIGH_PRIESTRESS:
                        if (Creature* priestress = pInstance->instance->GetCreature(pInstance->GetData64(NPC_HIGH_PRIESTRESS_MAR_LI)))
                        {
                            me->AddAura(SPELL_POSSESSED, priestress);
                        }
                        break;
                    case EVENT_LINGERING_PRESENCE_SUL_THE_SANDCRAWLER:
                        if (Creature* sul = pInstance->instance->GetCreature(pInstance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                        {
                            me->AddAura(SPELL_POSSESSED, sul);
                        }
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_gara_jal_s_soulAI(creature);
        }
};

// Frost King Malakk - 69131
class boss_king_malakk : public CreatureScript
{
    public:
        boss_king_malakk() : CreatureScript("boss_king_malakk") { }

        struct boss_king_malakkAI : public BossAI
        {
            boss_king_malakkAI(Creature* creature) : BossAI(creature, DATA_JIN_ROKH_THE_BREAKER)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            bool firstPossessSwitched;
            bool secondPossessSwitched;
            uint32 coefficient;

            void Reset()
            {
                events.Reset();

                coefficient = 0;
                firstPossessSwitched = false;
                secondPossessSwitched = false;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void JustReachedHome()
            {
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_CONCIL_OF_ELDERS, FAIL);
                }
            }

            void KilledUnit(Unit* who)
            {
            }

            void EnterCombat(Unit* who)
            {
                StartFight(pInstance, me, who);
                events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 30000);
                events.ScheduleEvent(EVENT_BITING_COLD, 62000);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_SCHEDULE_FRIGID_ASSAULT)
                    events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 40000);

                else if (action == ACTION_SCHEDULE_FROSTBITE)
                    events.ScheduleEvent(EVENT_FROSTBITE, 1000);
            }

            void JustDied(Unit* killer)
            {
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(67.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_KAZRA_JIN);

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_KAZRA_JIN);

                            secondPossessSwitched = true;
                        }
                    }
                }
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                value = 0;

                if (me->HasAura(SPELL_POSSESSED))
                    value = 2; // Generates 1 point per minute

                if (me->HasAura(SPELL_LINGERING_PRESENCE))
                    value = 2 + coefficient;
            }

            uint32 GetData(uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    return coefficient;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FRIGID_ASSAULT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            if (!target->HasAura(SPELL_FRIGID_ASSAULT_STUN))
                                me->CastSpell(target, SPELL_FRIGID_ASSAULT, true);
                        break;
                    case EVENT_BITING_COLD:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_BITING_COLD, true);
                        events.ScheduleEvent(EVENT_BITING_COLD, 62000);
                        break;
                    case EVENT_FROSTBITE:
                        if (me->HasAura(SPELL_POSSESSED))
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FROSTBITE, true);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_king_malakkAI(creature);
        }
};

// Kazra'jin - 69134
class boss_kazra_jin : public CreatureScript
{
    public:
        boss_kazra_jin() : CreatureScript("boss_kazra_jin") { }

        struct boss_kazra_jinAI : public BossAI
        {
            boss_kazra_jinAI(Creature* creature) : BossAI(creature, DATA_JIN_ROKH_THE_BREAKER)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            uint64 playerGuid;
            bool touchedTarget;
            bool firstPossessSwitched;
            bool secondPossessSwitched;

            void Reset()
            {
                events.Reset();

                if (pInstance)
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, NOT_STARTED);

                touchedTarget         = false;
                firstPossessSwitched  = false;
                secondPossessSwitched = false;

                playerGuid = 0;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_SCHEDULE_AT_DMG)
                    events.ScheduleEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG, 1000);
            }

            void JustReachedHome()
            {
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_CONCIL_OF_ELDERS, FAIL);
                }
            }

            void EnterCombat(Unit* who)
            {
                StartFight(pInstance, me, who);
                events.ScheduleEvent(EVENT_RECKLESS_CHARGE, 45000);
            }

            void SetGUID(uint64 guid, int32 id)
            {
                if (id == 1)
                    playerGuid = guid;
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_HIGH_PRIESTRESS);

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_HIGH_PRIESTRESS);

                            secondPossessSwitched = true;
                        }
                    }
                }
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

                value = 6;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->HasAura(SPELL_RECKLESS_CHARGE_ROLLING))
                {
                    if (!touchedTarget)
                    {
                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER, 500);
                        if (Player* target = Player::GetPlayer(*me, playerGuid))
                        {
                            if (me->GetDistance(target) <= 1.0f)
                            {
                                if (target->GetGUID() == playerGuid)
                                {
                                    me->CastSpell(target, SPELL_RECKLESS_CHARGE_KNOCK_BACK, true);
                                    me->canStartAttack(target, true);
                                    me->RemoveAura(SPELL_RECKLESS_CHARGE_ROLLING);
                                    playerGuid = 0;
                                    me->SetReactState(REACT_AGGRESSIVE);
                                    touchedTarget = true;
                                }
                            }
                        }
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_RECKLESS_CHARGE:
                            me->CastSpell(me, SPELL_RECKLESS_CHARGE_ROLLING, false);

                        touchedTarget = false;
                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE, 25000);
                        break;
                    case EVENT_RECKLESS_CHARGE_AREATRIGGER:
                        if (me->isMoving() == true)
                            me->CastSpell(me, SPELL_RECKLESS_CHARGE_AREATRIGGER, true);
                        break;
                    case EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG:
                    {
                        std::list<AreaTrigger*> areatriggerList;
                        me->GetAreaTriggerList(areatriggerList, SPELL_RECKLESS_CHARGE_AREATRIGGER);

                        if (areatriggerList.empty())
                            return;

                        for (auto areaTrigger : areatriggerList)
                            me->CastSpell(areaTrigger->GetPositionX(), areaTrigger->GetPositionY(), areaTrigger->GetPositionZ(), SPELL_RECKLESS_CHARGE_DAMAGE, true);

                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG, 1000);
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
            return new boss_kazra_jinAI(creature);
        }
};

// Sul the Sandcrawler - 69078
class boss_sul_the_sandcrawler : public CreatureScript
{
    public:
        boss_sul_the_sandcrawler() : CreatureScript("boss_sul_the_sandcrawler") { }

        struct boss_sul_the_sandcrawlerAI : public BossAI
        {
            boss_sul_the_sandcrawlerAI(Creature* creature) : BossAI(creature, DATA_JIN_ROKH_THE_BREAKER)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            bool firstPossessSwitched;
            bool secondPossessSwitched;

            void Reset()
            {
                events.Reset();

                firstPossessSwitched = false;
                secondPossessSwitched = false;

                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void JustReachedHome()
            {
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, FAIL);
                }
            }

            void KilledUnit(Unit* who)
            {
            }

            void EnterCombat(Unit* who)
            {
                StartFight(pInstance, me, who);
            }

            void JustDied(Unit* killer)
            {
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_SUL_THE_SANDCRAWLER);

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);
                            secondPossessSwitched = true;
                        }
                    }
                }
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

                value = 6;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sul_the_sandcrawlerAI(creature);
        }
};

// High Priestess Mar'li - 69132
class boss_high_priestress_mar_li : public CreatureScript
{
    public:
        boss_high_priestress_mar_li() : CreatureScript("boss_high_priestress_mar_li") { }

        struct boss_high_priestress_mar_liAI : public BossAI
        {
            boss_high_priestress_mar_liAI(Creature* creature) : BossAI(creature, DATA_JIN_ROKH_THE_BREAKER)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            bool firstPossessSwitched;
            bool secondPossessSwitched;

            void Reset()
            {
                events.Reset();

                firstPossessSwitched  = false;
                secondPossessSwitched = false;

                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void JustReachedHome()
            {
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, FAIL);
                }
            }

            void KilledUnit(Unit* who)
            {
            }

            void EnterCombat(Unit* who)
            {
                StartFight(pInstance, me, who);
            }

            void JustDied(Unit* killer)
            {
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_SUL_THE_SANDCRAWLER);

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_SUL_THE_SANDCRAWLER);

                            secondPossessSwitched = true;
                        }
                    }
                }
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

                value = 6;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_high_priestress_mar_liAI(creature);
        }
};

// Reckless Charge (rolling) - 137117
class spell_reckless_charge_rolling : public SpellScriptLoader
{
    public:
        spell_reckless_charge_rolling() : SpellScriptLoader("spell_reckless_charge_rolling") { }

        class spell_reckless_charge_rolling_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_reckless_charge_rolling_SpellScript);

            void HandleAfterCast()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    if (caster->GetEntry() == NPC_KAZRA_JIN && caster->GetAI())
                    {
                        Unit* target = NULL;
                        std::list<Player*> plrList;
                        for (auto itr : caster->getThreatManager().getThreatList())
                        {
                            if (Unit* unit = Unit::GetUnit(*caster, itr->getUnitGuid()))
                            {
                                if (unit->ToPlayer() && unit->GetDistance(caster) >= 10.0f)
                                    plrList.push_back(unit->ToPlayer());
                            }
                        }

                        if (plrList.empty())
                            target = caster->AI()->SelectTarget(SELECT_TARGET_RANDOM);
                        else
                        {
                            JadeCore::RandomResizeList(plrList, 1);
                            target = plrList.front();
                        }

                        caster->CastSpell(target, SPELL_RECKLESS_CHARGE_MOVEMENT, true);

                        if (Player* player = target->ToPlayer())
                        {
                            caster->AI()->SetGUID(player->GetGUID(), 1);
                            caster->AI()->DoAction(ACTION_SCHEDULE_AT_DMG);
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_reckless_charge_rolling_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_reckless_charge_rolling_SpellScript();
        }
};

// Reckless Charge (movement) - 137131
class spell_reckless_charge_movement : public SpellScriptLoader
{
    public:
        spell_reckless_charge_movement() : SpellScriptLoader("spell_reckless_charge_movement") { }

        class spell_reckless_charge_movement_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_reckless_charge_movement_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Creature* caster = GetCaster()->ToCreature())
                    {
                        if (caster->GetEntry() == NPC_KAZRA_JIN && caster->GetAI())
                        {
                            caster->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                            caster->SetReactState(REACT_PASSIVE);
                            caster->GetMotionMaster()->MoveCharge(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 10.0f);
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_reckless_charge_movement_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_reckless_charge_movement_SpellScript();
        }
};

// Frigid Assault (Stun) - 136904
class spell_frigid_assault : public SpellScriptLoader
{
    public:
        spell_frigid_assault() : SpellScriptLoader("spell_frigid_assault") { }

        class spell_frigid_assault_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_frigid_assault_AuraScript);

            void OnApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (AuraPtr frigidAssault = aurEff->GetBase())
                    if (frigidAssault->GetStackAmount() == 15)
                    {
                        if (Unit* caster = GetCaster())
                        {
                            if (Unit* target = GetTarget())
                            {
                                target->RemoveAurasDueToSpell(SPELL_FRIGID_ASSAULT);
                                caster->RemoveAurasDueToSpell(SPELL_FRIGID_ASSAULT);
                                target->CastSpell(target, SPELL_FRIGID_ASSAULT_STUN, true);

                                if (Creature* malakk = caster->ToCreature())
                                    if (malakk->GetAI())
                                        malakk->AI()->DoAction(ACTION_SCHEDULE_FRIGID_ASSAULT);
                            }
                        }
                    }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Creature* malakk = caster->ToCreature())
                        if (malakk->GetAI())
                            malakk->AI()->DoAction(ACTION_SCHEDULE_FRIGID_ASSAULT);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_frigid_assault_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_frigid_assault_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_frigid_assault_AuraScript();
        }
};

// Lingering Presence - 136467
class spell_lingering_presence : public SpellScriptLoader
{
    public:
        spell_lingering_presence() : SpellScriptLoader("spell_lingering_presence") { }

        class spell_lingering_presence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lingering_presence_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                uint32 coefficient = 2 * 10 / 100;

                if (Unit* target = GetTarget())
                {
                    if (Creature* creature = target->ToCreature())
                    {
                        if (creature->GetAI())
                        {
                            creature->SetPower(POWER_ENERGY, 0, true);
                            creature->AI()->SetData(coefficient, DATA_LINGERING_PRESENCE_COEF);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_lingering_presence_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lingering_presence_AuraScript();
        }
};

// Biting Cold - 136917
class spell_biting_cold_malakk : public SpellScriptLoader
{
    public:
        spell_biting_cold_malakk() : SpellScriptLoader("spell_biting_cold_malakk") { }

        class spell_biting_cold_malakk_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_biting_cold_malakk_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* target = GetExplTargetUnit())
                    if (Creature* caster = GetCaster()->ToCreature())
                        if (caster->GetEntry() == NPC_FROST_KING_MALAKK && caster->GetAI())
                            caster->AddAura(SPELL_BITING_COLD_AURA, target); 
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_biting_cold_malakk_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_biting_cold_malakk_SpellScript();
        }
};

// Frostbite - 136990
class spell_frosbite_malakk : public SpellScriptLoader
{
    public:
        spell_frosbite_malakk() : SpellScriptLoader("spell_frosbite_malakk") { }

        class spell_frosbite_malakk_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_frosbite_malakk_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* target = GetExplTargetUnit())
                    if (Creature* caster = GetCaster()->ToCreature())
                        if (caster->GetEntry() == NPC_FROST_KING_MALAKK && caster->GetAI())
                            caster->CastSpell(target, SPELL_FROSTBITE_PERIODIC, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_frosbite_malakk_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_frosbite_malakk_SpellScript();
        }
};

// Frostbite (aura) - 136922
class spell_frosbite_malakk_aura : public SpellScriptLoader
{
    public:
        spell_frosbite_malakk_aura() : SpellScriptLoader("spell_frosbite_malakk_aura") { }

        class spell_frosbite_malakk_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_frosbite_malakk_aura_AuraScript);

            void OnApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (AuraPtr frostBite = aurEff->GetBase())
                {
                    if (uint8 stack = frostBite->GetStackAmount())
                    {
                        if (Unit* target = GetTarget())
                        {
                            std::list<Player*> playerList;
                            target->GetPlayerListInGrid(playerList, 4.0f);
                            uint8 size = playerList.size();
                            uint8 amount = std::max(1, (stack - (2 * size)));
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_frosbite_malakk_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_frosbite_malakk_aura_AuraScript();
        }
};

void AddSC_boss_council_of_elders()
{
    new npc_gara_jal_s_soul();
    new boss_king_malakk();
    new boss_kazra_jin();
    new boss_sul_the_sandcrawler();
    new boss_high_priestress_mar_li();
    new spell_reckless_charge_rolling();
    new spell_reckless_charge_movement();
    new spell_frigid_assault();
    new spell_lingering_presence();
    new spell_biting_cold_malakk();
    new spell_frosbite_malakk();
    new spell_frosbite_malakk_aura();
}
