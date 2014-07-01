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
    SPELL_FROSTBITE_AURA                 = 137575,
    // Kaz'Ra Jin
    SPELL_RECKLESS_CHARGE_AREATRIGGER    = 138026,
    SPELL_RECKLESS_CHARGE_ROLLING        = 137117, // aura
    SPELL_RECKLESS_CHARGE_MOVEMENT       = 137131,
    SPELL_RECKLESS_CHARGE_DAMAGE         = 137133,
    SPELL_RECKLESS_CHARGE_KNOCK_BACK     = 137122,
    SPELL_OVERLOAD                       = 137149,
    SPELL_GENERIC_STUN                   = 135781,
    // Sun
    SPELL_SAND_BOLT                      = 136189,
    SPELL_QUICKSAND                      = 136521,
    SPELL_SAND_PERIODIC_DMG              = 136860,
    SPELL_SAND_VISUAL                    = 136851,
    SPELL_ENSNARED                       = 136878,
    SPELL_ENTRAPPED                      = 136857,
    SPELL_SANDSTORM                      = 136894,
    SPELL_SANDSTORM_DUMMY                = 136895,
    SPELL_FORTIFIED                      = 136864,
    // High Priestress Mar'Li
    SPELL_WRATH_OF_THE_LOA_BLESSED       = 137344,
    SPELL_WRATH_OF_THE_LOA_SHADOW        = 137347,
    SPELL_BLESSED_LOA_SPIRIT_SUMMON      = 137200,
    SPELL_SHADOWED_LOA_SPIRIT_SUMMONED   = 137351,
    SPELL_SHADOWED_GIFT                  = 137407,
    SPELL_MARKED_SOUL                    = 137359,
    SPELL_DARK_POWER                     = 136507
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
    EVENT_FROSTBITE                              = 10,
    EVENT_SAND_BOLT                              = 11,
    EVENT_QUICKSAND                              = 12,
    EVENT_SANDSTORM                              = 13,
    EVENT_WRATH_OF_THE_LOA_BLESSED               = 14,
    EVENT_WRATH_OF_THE_LOA_SHADOW                = 15,
    EVENT_BLESSED_LOA_SPIRIT_SUMMON              = 16,
    EVENT_HEAL_WEAKER_TROLL                      = 17,
    EVENT_SHADOWED_LOA_SPIRIT_SUMMON             = 18,
    EVENT_OS_PLAYER                              = 19,
    EVENT_DARK_POWER                             = 20
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
    ACTION_SCHEDULE_FROSTBITE                      = 7,
    ACTION_SCHEDULE_SANDSTROM                      = 8,
    ACTION_SANDSTORM                               = 9,
    ACTION_SCHEDULE_WRATH_OF_THE_LOA_SHADOW        = 10,
    ACTION_SCHEDULE_SHADOWED_SPIRIT_SPAWN          = 11
};

enum ePhases
{
};

enum eDatas
{
    DATA_LINGERING_PRESENCE_COEF,
    DATA_SPELL_DMG_MULTIPLIER
};

void StartFight(InstanceScript* instance, Creature* me, Unit* /*target*/)
{
    if (!instance)
        return;

    if (instance->GetBossState(DATA_CONCIL_OF_ELDERS) == IN_PROGRESS)
        return; // Prevent recursive calls

    instance->SetBossState(DATA_CONCIL_OF_ELDERS, IN_PROGRESS);

    uint32 mobEntries[4] = {NPC_KAZRA_JIN, NPC_SUL_THE_SANDCRAWLER, NPC_HIGH_PRIESTRESS_MAR_LI, NPC_FROST_KING_MALAKK};

    for (uint32 entry : mobEntries)
        if (Creature* boss = instance->instance->GetCreature(instance->GetData64(entry)))
            boss->SetInCombatWithZone();

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

                            if (priestress->GetAI())
                                priestress->AI()->DoAction(ACTION_SCHEDULE_WRATH_OF_THE_LOA_SHADOW);
                        }
                        break;
                    case EVENT_LINGERING_PRESENCE_SUL_THE_SANDCRAWLER:
                        if (Creature* sul = pInstance->instance->GetCreature(pInstance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                        {
                            me->AddAura(SPELL_POSSESSED, sul);

                            if (sul->GetAI())
                                sul->AI()->DoAction(ACTION_SCHEDULE_SANDSTROM);
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
            uint32 counter;

            void Reset()
            {
                coefficient = 0;
                counter     = 0;
                firstPossessSwitched = false;
                secondPossessSwitched = false;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);

                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
            }

            void EnterEvadeMode()
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);

                _EnterEvadeMode();
                me->GetMotionMaster()->MoveTargetedHome();

                firstPossessSwitched = false;
                secondPossessSwitched = false;
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

                events.Reset();
                events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 30000);
                events.ScheduleEvent(EVENT_BITING_COLD, 62000);
            }

            void JustDied(Unit* killer)
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_SCHEDULE_FRIGID_ASSAULT)
                    events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 40000);

                else if (action == ACTION_SCHEDULE_FROSTBITE)
                {
                    events.Reset();
                    events.ScheduleEvent(EVENT_FROSTBITE, 2000);
                }
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(67.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 30000);
                            events.ScheduleEvent(EVENT_BITING_COLD, 62000);

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
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 30000);
                            events.ScheduleEvent(EVENT_BITING_COLD, 62000);

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

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

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
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();

                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
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
                            {
                                me->AddAura(SPELL_FROSTBITE_AURA, target);
                                me->CastSpell(target, SPELL_FROSTBITE, true);
                                events.ScheduleEvent(EVENT_FROSTBITE, 52000);
                            }
                        break;
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, true);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
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
            uint32 coefficient;
            uint32 counter;

            void Reset()
            {
                events.Reset();

                if (pInstance)
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, NOT_STARTED);

                touchedTarget         = true;
                firstPossessSwitched  = false;
                secondPossessSwitched = false;

                coefficient = 0;
                counter     = 0;
                playerGuid  = 0;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);

                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
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

            void JustDied(Unit* killer)
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
            }

            void EnterEvadeMode()
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);

                _EnterEvadeMode();
                me->GetMotionMaster()->MoveTargetedHome();

                firstPossessSwitched = false;
                secondPossessSwitched = false;
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

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
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
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_HIGH_PRIESTRESS);

                            secondPossessSwitched = true;
                        }
                    }
                }

                if (me->HasAura(SPELL_OVERLOAD))
                {
                    me->DealDamage(attacker, damage / 100 * 40);
                    me->SendSpellNonMeleeDamageLog(attacker, SPELL_OVERLOAD, damage / 100 * 40, SPELL_SCHOOL_MASK_NATURE, 0, 0, false, 0, false);
                }
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

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
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();

                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
                }

                if (me->HasAura(SPELL_OVERLOAD))
                    me->SendMeleeAttackStop(me->getVictim());

                if (me->HasAura(SPELL_RECKLESS_CHARGE_ROLLING))
                {
                    if (!touchedTarget)
                    {
                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER, 500);
                        if (Player* target = Player::GetPlayer(*me, playerGuid))
                        {
                            if (me->GetDistance(target) <= 3.5f)
                            {
                                if (target->GetGUID() == playerGuid)
                                {
                                    me->CastSpell(target, SPELL_RECKLESS_CHARGE_KNOCK_BACK, false);
                                    me->canStartAttack(target, true);
                                    me->RemoveAura(SPELL_RECKLESS_CHARGE_ROLLING);
                                    playerGuid = 0;
                                    me->SetReactState(REACT_AGGRESSIVE);
                                    touchedTarget = true;

                                    if (me->HasAura(SPELL_POSSESSED))
                                        me->AddAura(SPELL_OVERLOAD, me);
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
                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE, 45000);
                        break;
                    case EVENT_RECKLESS_CHARGE_AREATRIGGER:
                        if (me->isMoving())
                            me->CastSpell(me, SPELL_RECKLESS_CHARGE_AREATRIGGER, false);
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
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, true);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
                        break;
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
            uint32 coefficient;
            uint32 counter;

            void Reset()
            {
                events.Reset();

                firstPossessSwitched = false;
                secondPossessSwitched = false;

                coefficient = 0;
                counter     = 0;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);

                std::list<Creature*> livingSandList;
                GetCreatureListWithEntryInGrid(livingSandList, me, NPC_LIVING_SAND, 200.0f);

                for (auto creature : livingSandList)
                    creature->DespawnOrUnsummon();

                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
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

                events.ScheduleEvent(EVENT_SAND_BOLT, 10000);
                events.ScheduleEvent(EVENT_QUICKSAND, 35000);
            }

            void JustDied(Unit* killer)
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
            }

            void EnterEvadeMode()
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);

                _EnterEvadeMode();
                me->GetMotionMaster()->MoveTargetedHome();

                firstPossessSwitched = false;
                secondPossessSwitched = false;
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_MALAKK);

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
                            me->SetPower(POWER_ENERGY, 0, false);
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

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

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
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_SCHEDULE_SANDSTROM)
                    events.ScheduleEvent(EVENT_SANDSTORM, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();

                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SAND_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_SAND_BOLT, true);
                        events.ScheduleEvent(EVENT_SAND_BOLT, 10000);
                        break;
                    case EVENT_QUICKSAND:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(target, SPELL_QUICKSAND, true);
                            target->AddAura(SPELL_GENERIC_STUN, target);
                            me->SummonCreature(NPC_LIVING_SAND, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        }
                        events.ScheduleEvent(EVENT_QUICKSAND, 34000);
                        break;
                    case EVENT_SANDSTORM:
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SANDSTORM, true);

                            me->AddAura(SPELL_SANDSTORM_DUMMY, me);
                            events.ScheduleEvent(EVENT_SANDSTORM, 40000);
                        }
                        break;
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, true);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
                        break;
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
            uint32 coefficient;
            uint32 counter;

            void Reset()
            {
                events.Reset();

                firstPossessSwitched  = false;
                secondPossessSwitched = false;

                coefficient = 0;
                counter     = 0;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);

                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
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

            void DoAction(int32 const action)
            {
                if (IsHeroic())
                    return;

                if (action == ACTION_SCHEDULE_WRATH_OF_THE_LOA_SHADOW || action == ACTION_SCHEDULE_SHADOWED_SPIRIT_SPAWN)
                {
                    events.Reset();
                    events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_SHADOW, 3000);
                    events.ScheduleEvent(EVENT_SHADOWED_LOA_SPIRIT_SUMMON, 5000);
                }
            }

            void EnterEvadeMode()
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);

                _EnterEvadeMode();
                me->GetMotionMaster()->MoveTargetedHome();

                firstPossessSwitched = false;
                secondPossessSwitched = false;
            }

            void EnterCombat(Unit* who)
            {
                StartFight(pInstance, me, who);

                events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 10000);
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                            events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 10000);

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
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                            events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 10000);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                                if (garaJalSoul->GetAI())
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION_SUL_THE_SANDCRAWLER);

                            secondPossessSwitched = true;
                        }
                    }
                }
            }

            void JustDied(Unit* killer)
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

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
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();

                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WRATH_OF_THE_LOA_BLESSED:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_WRATH_OF_THE_LOA_BLESSED, true);
                        events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                        break;
                    case EVENT_WRATH_OF_THE_LOA_SHADOW:
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_WRATH_OF_THE_LOA_SHADOW, true);
                            events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_SHADOW, 10000);
                        }
                        break;
                    case EVENT_BLESSED_LOA_SPIRIT_SUMMON:
                        me->CastSpell(me, SPELL_BLESSED_LOA_SPIRIT_SUMMON, false);
                        events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 35000);
                        break;
                    case EVENT_SHADOWED_LOA_SPIRIT_SUMMON:
                        if (me->HasAura(SPELL_POSSESSED))
                            me->CastSpell(me, SPELL_SHADOWED_LOA_SPIRIT_SUMMONED, false);
                        events.ScheduleEvent(EVENT_SHADOWED_LOA_SPIRIT_SUMMON, 37000);
                        break;
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, true);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
                        break;
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
                        Unit* target = caster->AI()->SelectTarget(SELECT_TARGET_RANDOM);

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
                if (Unit* target = GetExplTargetUnit())
                {
                    if (Creature* caster = GetCaster()->ToCreature())
                    {
                        if (caster->GetEntry() == NPC_KAZRA_JIN && caster->GetAI())
                        {
                            caster->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                            caster->SetReactState(REACT_PASSIVE);
                            caster->GetMotionMaster()->MoveCharge(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 10.0f);
                            caster->SetReactState(REACT_AGGRESSIVE);
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

// Overload (aura) - 137149
class spell_overload_kazra_jin : public SpellScriptLoader
{
    public:
        spell_overload_kazra_jin() : SpellScriptLoader("spell_overload_kazra_jin") { }

        class spell_overload_kazra_jin_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_overload_kazra_jin_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_GENERIC_STUN, caster);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveAura(SPELL_GENERIC_STUN);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_overload_kazra_jin_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_overload_kazra_jin_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_overload_kazra_jin_AuraScript();
        }
};

// Ensnared - 136878
class spell_ensnared : public SpellScriptLoader
{
    public:
        spell_ensnared() : SpellScriptLoader("spell_ensnared") { }

        class spell_ensnared_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ensnared_AuraScript);

            void OnApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (AuraPtr ensnared = aurEff->GetBase())
                {
                    if (uint8 stack = ensnared->GetStackAmount())
                    {
                        if (Unit* caster = GetCaster())
                        {
                            std::list<Player*> playerList;
                            caster->GetPlayerListInGrid(playerList, 7.0f);

                            for (auto player: playerList)
                                if (stack == 5)
                                    caster->AddAura(SPELL_ENTRAPPED, player);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_ensnared_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ensnared_AuraScript();
        }
};

// SandStorm - 136895
class spell_sandstorm : public SpellScriptLoader
{
    public:
        spell_sandstorm() : SpellScriptLoader("spell_sandstorm") { }

        class spell_sandstorm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sandstorm_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        std::list<Creature*> creatureList;
                        GetCreatureListWithEntryInGrid(creatureList, caster, NPC_LIVING_SAND, 200.0f);

                        for (auto livingSand : creatureList)
                        {
                            if (livingSand->GetAI())
                            {
                                if (livingSand->HasAura(SPELL_SAND_VISUAL))
                                    livingSand->AI()->DoAction(ACTION_SANDSTORM);
                                else
                                    livingSand->CastSpell(livingSand, SPELL_FORTIFIED, false);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sandstorm_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sandstorm_AuraScript();
        }
};

// Dark Power - 136507
class spell_dark_power : public SpellScriptLoader
{
    public:
        spell_dark_power() : SpellScriptLoader("spell_dark_power") { }

        class spell_dark_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_power_SpellScript);

            void HandleOnHit()
            {
                if (Unit* target = GetExplTargetUnit())
                    if (Creature* caster = GetCaster()->ToCreature())
                        if (caster->GetAI())
                        {
                            uint32 counter = caster->AI()->GetData(DATA_SPELL_DMG_MULTIPLIER);
                            SetHitDamage(GetSpellInfo()->Effects[0].BasePoints * 1 + (counter * 0.10));
                        }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dark_power_SpellScript ::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_power_SpellScript();
        }
};

// Living Sand - 69153
class mob_living_sand : public CreatureScript
{
    public:
        mob_living_sand() : CreatureScript("mob_living_sand") { }

        struct mob_living_sandAI : public ScriptedAI
        {
            mob_living_sandAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->AddAura(SPELL_SAND_VISUAL, me);
                me->SetReactState(REACT_PASSIVE);
                me->SetFullHealth();

                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 7.0f);

                for (auto player: playerList)
                {
                    me->AddAura(SPELL_SAND_PERIODIC_DMG, player);
                    me->AddAura(SPELL_ENSNARED, player);
                }
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_SANDSTORM)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->RemoveAura(SPELL_SAND_VISUAL);
                    me->SetReactState(REACT_AGGRESSIVE);
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32 &damage)
            {
                if (damage > me->GetHealth())
                {
                    damage = 0;
                    Reset();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_living_sandAI(creature);
        }
};

// Blessed Loa Spirit - 69480
class mob_blessed_loa_spirit : public CreatureScript
{
    public:
        mob_blessed_loa_spirit() : CreatureScript("mob_blessed_loa_spirit") { }

        struct mob_blessed_loa_spiritAI : public ScriptedAI
        {
            mob_blessed_loa_spiritAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* instance;
            uint32 bossEntry;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HEAL_WEAKER_TROLL, 20000);
                me->SetReactState(REACT_PASSIVE);
                bossEntry = 0;
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (bossEntry)
                {
                    if (Creature* boss = instance->instance->GetCreature(instance->GetData64(bossEntry)))
                    {
                        if (me->GetDistance(boss) <= 2.0f)
                        {
                            boss->ModifyHealth(boss->GetHealth() * 5 / 100);
                            me->DespawnOrUnsummon();
                        }
                    }
                }

                switch (events.ExecuteEvent())
                {
                    case EVENT_HEAL_WEAKER_TROLL:
                        {
                            uint32 mobEntries[4] = {NPC_KAZRA_JIN, NPC_SUL_THE_SANDCRAWLER, NPC_HIGH_PRIESTRESS_MAR_LI, NPC_FROST_KING_MALAKK};
                            uint32 minHealth = 0;
                            Creature* minBoss = NULL;

                            for (uint32 entry : mobEntries)
                            {
                                if (Creature* boss = instance->instance->GetCreature(instance->GetData64(entry)))
                                {
                                    if (entry == NPC_KAZRA_JIN)
                                        minHealth = boss->GetMaxHealth();

                                    if (minHealth > boss->GetHealth())
                                    {
                                        minHealth = boss->GetHealth();
                                        minBoss = boss;
                                    }
                                }
                            }

                            me->GetMotionMaster()->MoveChase(minBoss, 1.0f, 1.0f);
                            bossEntry = minBoss->GetEntry();
                        }
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_blessed_loa_spiritAI(creature);
        }
};

// Shadowed Loa Spirit - 69548
class mob_shadowed_lua_spirit : public CreatureScript
{
public:
    mob_shadowed_lua_spirit() : CreatureScript("mob_shadowed_lua_spirit") { }

    struct mob_shadowed_lua_spiritAI : public ScriptedAI
    {
        mob_shadowed_lua_spiritAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        EventMap events;
        uint32 despawnTimer;
        uint64 targetGuid;
        InstanceScript* instance;

        void Reset()
        {
            events.Reset();
            despawnTimer = 0;
            targetGuid   = 0;

            events.ScheduleEvent(EVENT_OS_PLAYER, 20000);
            me->SetReactState(REACT_PASSIVE);
        }

        void SetGUID(uint64 guid, int32 /*index*/)
        {
            targetGuid = guid;
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            if (Player* player = Player::GetPlayer(*me, targetGuid))
            {
                if (despawnTimer <= diff)
                {

                    if (player->HasAura(SPELL_MARKED_SOUL))
                        player->RemoveAura(SPELL_MARKED_SOUL);

                    me->CastSpell(player, SPELL_SHADOWED_GIFT, false);
                }
                else
                    despawnTimer -= diff;

                if (player->GetDistance(me) <= 6.0f)
                {
                    me->CastSpell(player, SPELL_SHADOWED_GIFT, false);
                    me->DespawnOrUnsummon();
                }
            }

            switch (events.ExecuteEvent())
            {
                case EVENT_OS_PLAYER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    {
                        me->AddAura(SPELL_MARKED_SOUL, target);
                        SetGUID(target->GetGUID(), 0);
                        despawnTimer = 20000;
                    }
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_shadowed_lua_spiritAI(creature);
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
    new spell_overload_kazra_jin();
    new spell_ensnared();
    new spell_sandstorm();
    new spell_dark_power();
    new mob_living_sand();
    new mob_blessed_loa_spirit();
    new mob_shadowed_lua_spirit();
}