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

#include "ScriptedCreature.h"
#include "heart_of_fear.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "CreatureAI.h"
#include "MoveSplineInit.h"

// 63048 - Kor'thik Slicer
class mob_kor_thik_slicer : public CreatureScript
{
    public:
        mob_kor_thik_slicer() : CreatureScript("mob_kor_thik_slicer") {}

        struct mob_kor_thik_slicerAI : public ScriptedAI
        {
            mob_kor_thik_slicerAI(Creature* creature) : ScriptedAI(creature)
            {
                eventScheduled = false;
            }

            EventMap events;
            bool eventScheduled;

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                events.ScheduleEvent(EVENT_ARTERIAL_SPIRIT, 5000);
                events.ScheduleEvent(EVENT_VITAL_STRIKES, 12000);
                if (me->GetPositionY() > 348.5f && me->GetPositionY() < 403.3f)
                    me->AddUnitState(UNIT_STATE_NOT_MOVE);
            }

            void DamageTaken(Unit* killer, uint32& damage)
            {
                if (me->HasUnitState(UNIT_STATE_NOT_MOVE))
                    me->ClearUnitState(UNIT_STATE_NOT_MOVE);

                if(!eventScheduled)
                {
                    if (me->HealthBelowPctDamaged(25, damage))
                    {
                        events.ScheduleEvent(EVENT_GOUGE_THROAT, 12000);
                        eventScheduled = true;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_ARTERIAL_SPIRIT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_ARTERIAL_SPIRIT, true);
                            events.ScheduleEvent(EVENT_ARTERIAL_SPIRIT, 20000);
                            break;
                        case EVENT_GOUGE_THROAT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_GOUGE_THROAT, true);
                            events.ScheduleEvent(EVENT_GOUGE_THROAT, 32000);
                            break;
                        case EVENT_VITAL_STRIKES:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_VITAL_STRIKES, true);
                            events.ScheduleEvent(EVENT_VITAL_STRIKES, 20000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kor_thik_slicerAI(creature);
        }
};

// 63036 - Kor'thik extremist
class mob_kor_thik_extremist : public CreatureScript
{
    public:
        mob_kor_thik_extremist() : CreatureScript("mob_kor_thik_extremist") {}

        struct mob_kor_thik_extremistAI : public ScriptedAI
        {
            mob_kor_thik_extremistAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                events.ScheduleEvent(EVENT_UNDYING_DEVOTION, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_UNDYING_DEVOTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_UNDYING_DEVOTION, true);
                            events.ScheduleEvent(EVENT_UNDYING_DEVOTION, 20000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kor_thik_extremistAI(creature);
        }
};

// 63032 - Sra'thik Shield Master
class mob_sra_thik_shield_master : public CreatureScript
{
    public:
        mob_sra_thik_shield_master() : CreatureScript("mob_sra_thik_shield_master") {}

        struct mob_sra_thik_shield_masterAI : public ScriptedAI
        {
            mob_sra_thik_shield_masterAI(Creature* creature) : ScriptedAI(creature)
            {            
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                events.ScheduleEvent(EVENT_BRAIN_FREEZE, 7000);
                events.ScheduleEvent(EVENT_DISMANTLED_ARMOR, 12000);
                events.ScheduleEvent(EVENT_MASS_SPELL_REFLEXION, 19000);
                events.ScheduleEvent(EVENT_SHIELD_SLAM, 26000);
                events.ScheduleEvent(EVENT_STUNNING_STRIKE, 35000);

                if (me->GetPositionY() > 348.5f && me->GetPositionY() < 403.3f)
                    me->AddUnitState(UNIT_STATE_NOT_MOVE);
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (me->HasUnitState(UNIT_STATE_NOT_MOVE))
                    me->ClearUnitState(UNIT_STATE_NOT_MOVE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_BRAIN_FREEZE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BRAIN_FREEZE, true);
                            events.ScheduleEvent(EVENT_BRAIN_FREEZE, 60000);
                            break;
                        case EVENT_DISMANTLED_ARMOR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DISMANTLED_ARMOR, true);
                            events.ScheduleEvent(EVENT_DISMANTLED_ARMOR, 40000);
                            break;
                        case EVENT_MASS_SPELL_REFLEXION:
                                me->CastSpell(me, SPELL_MASS_SPELL_REFLEXION, true);
                            events.ScheduleEvent(EVENT_MASS_SPELL_REFLEXION, 40000);
                            break;
                        case EVENT_SHIELD_SLAM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHIELD_SLAM, true);
                            events.ScheduleEvent(EVENT_SHIELD_SLAM, 40000);
                            break;
                        case EVENT_STUNNING_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_STUNNING_STRIKE, true);
                            events.ScheduleEvent(EVENT_STUNNING_STRIKE, 40000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sra_thik_shield_masterAI(creature);
        }
};

// 63049 - Set'thik Swiftblade
class mob_set_thik_swiftblade : public CreatureScript
{
    public:
        mob_set_thik_swiftblade() : CreatureScript("mob_set_thik_swiftblade") {}

        struct mob_set_thik_swiftbladeAI : public ScriptedAI
        {
            mob_set_thik_swiftbladeAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                events.ScheduleEvent(EVENT_BLURRING_SLASH, 7000);
                events.ScheduleEvent(EVENT_RIPOSTE, 12000);

                if (me->GetPositionY() > 348.5f && me->GetPositionY() < 403.3f)
                    me->AddUnitState(UNIT_STATE_NOT_MOVE);
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (me->HasUnitState(UNIT_STATE_NOT_MOVE))
                    me->ClearUnitState(UNIT_STATE_NOT_MOVE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_BLURRING_SLASH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLURRING_SLASH, true);
                            events.ScheduleEvent(EVENT_BLURRING_SLASH, 22000);
                            break;
                        case EVENT_RIPOSTE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RIPOSTE, true);
                            events.ScheduleEvent(EVENT_RIPOSTE, 22000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_set_thik_swiftbladeAI(creature);
        }
};

class mob_zar_thik_supplicant : public CreatureScript
{
    public:
        mob_zar_thik_supplicant() : CreatureScript("mob_zar_thik_supplicant") {}

        struct mob_zar_thik_supplicantAI : public ScriptedAI
        {
            mob_zar_thik_supplicantAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                if (IsHeroic() && me->GetEntry() == NPC_ZARTHIK_SUPPLICANT)
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                if (me->GetEntry() == NPC_ZARTHIK_SUPPLICANT_3 || me->GetEntry() == NPC_ZARTHIK_SUPPLICANT)
                    events.ScheduleEvent(EVENT_ICE_TRAP, 7000);
                events.ScheduleEvent(EVENT_MASH_AND_GNASH, 12000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_ICE_TRAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ICE_TRAP, true);
                            events.ScheduleEvent(EVENT_ICE_TRAP, 22000);
                            break;
                        case EVENT_MASH_AND_GNASH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_MASH_AND_GNASH, true);
                            events.ScheduleEvent(EVENT_MASH_AND_GNASH, 22000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zar_thik_supplicantAI(creature);
        }
};

class mob_enslaved_bonesmasher : public CreatureScript
{
    public:
        mob_enslaved_bonesmasher() : CreatureScript("mob_enslaved_bonesmasher") {}

        struct mob_enslaved_bonesmasherAI : public ScriptedAI
        {
            mob_enslaved_bonesmasherAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                events.ScheduleEvent(EVENT_JAWBONE_SLAM, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_JAWBONE_SLAM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_JAWBONE_SLAM, true);
                            events.ScheduleEvent(EVENT_JAWBONE_SLAM, 22000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_enslaved_bonesmasherAI(creature);
        }
};

class mob_set_thik_tempest : public CreatureScript
{
    public:
        mob_set_thik_tempest() : CreatureScript("mob_set_thik_tempest") {}

        struct mob_set_thik_tempestAI : public ScriptedAI
        {
            mob_set_thik_tempestAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_WIND_SLASH, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    if (id = EVENT_WIND_SLASH)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_WIND_SLASH, true);
                        events.ScheduleEvent(EVENT_WIND_SLASH, 40000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_set_thik_tempestAI(creature);
        }
};

class mob_set_thik_fanatic : public CreatureScript
{
    public:
        mob_set_thik_fanatic() : CreatureScript("mob_set_thik_fanatic") {}

        struct mob_set_thik_fanaticAI : public ScriptedAI
        {
            mob_set_thik_fanaticAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                if (!IsHeroic())
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                events.ScheduleEvent(EVENT_GALE_FORCE_WINDS, 7000);
                events.ScheduleEvent(EVENT_WINDBURST, 15000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_GALE_FORCE_WINDS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GALE_FORCE_WINDS, true);
                            events.ScheduleEvent(EVENT_GALE_FORCE_WINDS, 22000);
                            break;
                        case EVENT_WINDBURST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WINDBURST, true);
                            events.ScheduleEvent(EVENT_WINDBURST, 22000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_set_thik_fanaticAI(creature);
        }
};

class mob_set_thik_zephyrian : public CreatureScript
{
    public:
        mob_set_thik_zephyrian() : CreatureScript("mob_set_thik_zephyrian") {}

        struct mob_set_thik_zephyrianAI : public ScriptedAI
        {
            mob_set_thik_zephyrianAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SUMMON_ZEPHYR, 4000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    if (id == EVENT_SUMMON_ZEPHYR)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_SUMMON_ZEPHYR, true);
                        events.ScheduleEvent(EVENT_SUMMON_ZEPHYR, 22000);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_set_thik_zephyrianAI(creature);
        }
};

class mob_set_thik_gale_slicer : public CreatureScript
{
    public:
        mob_set_thik_gale_slicer() : CreatureScript("mob_set_thik_gale_slicer") {}

        struct mob_set_thik_gale_slicerAI : public ScriptedAI
        {
            mob_set_thik_gale_slicerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SWIFT_STEP, 4000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    if (id == EVENT_SWIFT_STEP)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_SWIFT_STEP, true);
                        events.ScheduleEvent(EVENT_SWIFT_STEP, 22000);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_set_thik_gale_slicerAI(creature);
        }
};

class mob_instructor_kli_thak : public CreatureScript
{
    public:
        mob_instructor_kli_thak() : CreatureScript("mob_instructor_kli_thak") {}

        struct mob_instructor_kli_thakAI : public ScriptedAI
        {
            mob_instructor_kli_thakAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_WIND_STEP, 7000);
                events.ScheduleEvent(EVENT_WIND_STEP_2, 15000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_WIND_STEP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WIND_STEP, true);
                            events.ScheduleEvent(EVENT_WIND_STEP, 22000);
                            break;
                        case EVENT_WIND_STEP_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WIND_STEP_2, true);
                            events.ScheduleEvent(EVENT_WIND_STEP_2, 22000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_instructor_kli_thakAI(creature);
        }
};

class mob_instructor_tak_thok : public CreatureScript
{
    public:
        mob_instructor_tak_thok() : CreatureScript("mob_instructor_tak_thok") {}

        struct mob_instructor_tak_thokAI : public ScriptedAI
        {
            mob_instructor_tak_thokAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_OVERWHELMING_ASSAULT, 4000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    if (id == EVENT_OVERWHELMING_ASSAULT)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_OVERWHELMING_ASSAULT, true);
                        events.ScheduleEvent(EVENT_OVERWHELMING_ASSAULT, 22000);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_instructor_tak_thokAI(creature);
        }
};

class mob_zar_thik_zealot : public CreatureScript
{
    public:
        mob_zar_thik_zealot() : CreatureScript("mob_zar_thik_zealot") {}

        struct mob_zar_thik_zealotAI : public ScriptedAI
        {
            mob_zar_thik_zealotAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SLOW, 3000);
                events.ScheduleEvent(EVENT_ZEALOUS_RUSH, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_ZEALOUS_RUSH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_ZEALOUS_RUSH, true);
                            events.ScheduleEvent(EVENT_ZEALOUS_RUSH, 22000);
                            break;
                        case EVENT_SLOW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_SLOW, true);
                            events.ScheduleEvent(EVENT_SLOW, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zar_thik_zealotAI(creature);
        }
};

class mob_kor_thik_swarmer : public CreatureScript
{
    public:
        mob_kor_thik_swarmer() : CreatureScript("mob_kor_thik_swarmer") {}

        struct mob_kor_thik_swarmerAI : public ScriptedAI
        {
            mob_kor_thik_swarmerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_UNDERWHELMING_ASSAULT, 4000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    if (id == EVENT_UNDERWHELMING_ASSAULT)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_UNDERWHELMING_ASSAULT, true);
                        events.ScheduleEvent(EVENT_UNDERWHELMING_ASSAULT, 22000);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kor_thik_swarmerAI(creature);
        }
};

class mob_set_thik_gustwing : public CreatureScript
{
    public:
        mob_set_thik_gustwing() : CreatureScript("mob_set_thik_gustwing") {}

        struct mob_set_thik_gustwingAI : public ScriptedAI
        {
            mob_set_thik_gustwingAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GUST, 3000);
                events.ScheduleEvent(EVENT_ICE_TRAP, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_GUST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_GUST, true);
                            events.ScheduleEvent(EVENT_GUST, 22000);
                            break;
                        case EVENT_ICE_TRAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                                me->CastSpell(target, SPELL_ICE_TRAP, true);
                            events.ScheduleEvent(EVENT_ICE_TRAP, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_set_thik_gustwingAI(creature);
        }
};

class mob_coagulated_amber : public CreatureScript
{
    public:
        mob_coagulated_amber() : CreatureScript("mob_coagulated_amber") {}

        struct mob_coagulated_amberAI : public ScriptedAI
        {
            mob_coagulated_amberAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BURST, 4000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    if (id == EVENT_BURST)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_BURST, true);
                        events.ScheduleEvent(EVENT_BURST, 22000);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_coagulated_amberAI(creature);
        }
};

// 123422 - Arterial Bleeding
class spell_arterial_bleed : public SpellScriptLoader
{
    public:
        spell_arterial_bleed() : SpellScriptLoader("spell_arterial_bleed") { }

        class spell_arterial_bleed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arterial_bleed_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        uint32 dmgIni = target->GetDamageTakenInPastSecs(1);
                        (int32)GetSpellInfo()->Effects[0].MiscValue = dmgIni * 0.5;
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_arterial_bleed_AuraScript::Apply, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_arterial_bleed_AuraScript();
        }
};

// 123421 - Vital Strikes
class spell_vital_strikes : public SpellScriptLoader
{
    public:
        spell_vital_strikes() : SpellScriptLoader("spell_vital_strikes") { }

        class spell_vital_strikes_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_vital_strikes_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        caster->AddAura(SPELL_VITAL_STRIKES, target);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_vital_strikes_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_vital_strikes_AuraScript();
        };
};

void AddSC_heart_of_fear()
{
    // Trashes
    new mob_kor_thik_slicer();
    new mob_kor_thik_extremist();
    new mob_sra_thik_shield_master();
    new mob_set_thik_swiftblade();
    new mob_zar_thik_supplicant();
    new mob_enslaved_bonesmasher();
    new mob_set_thik_tempest();
    new mob_set_thik_fanatic();
    new mob_set_thik_zephyrian();
    new mob_set_thik_gale_slicer();
    new mob_instructor_kli_thak();
    new mob_instructor_tak_thok();
    new mob_zar_thik_zealot();
    new mob_kor_thik_swarmer();
    new mob_set_thik_gustwing();
    new mob_coagulated_amber();
    new spell_arterial_bleed();
    new spell_vital_strikes();
}