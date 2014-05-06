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
                events.ScheduleEvent(EVENT_ARTERIAL_SPIRIT, 10000);
                events.ScheduleEvent(EVENT_VITAL_STRIKES, 5000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_4);
            }

            void DamageTaken(Unit* killer, uint32& damage)
            {
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
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_5);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_6);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_7);

            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 67177 - 66181 - 63853 - Zar'thik Supplicant
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
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 63030 - Enslaved Bonesmasher
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

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_8);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 64358 - Set'thik Tempest
class mob_set_thik_tempest : public CreatureScript
{
    public:
        mob_set_thik_tempest() : CreatureScript("mob_set_thik_tempest") {}

        struct mob_set_thik_tempestAI : public ScriptedAI
        {
            mob_set_thik_tempestAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_WIND_SLASH, 7000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_4);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 63031 - Set'thik Fanatic
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
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 63593 - Set'thik Zephyrian
class mob_set_thik_zephyrian : public CreatureScript
{
    public:
        mob_set_thik_zephyrian() : CreatureScript("mob_set_thik_zephyrian") {}

        struct mob_set_thik_zephyrianAI : public ScriptedAI
        {
            mob_set_thik_zephyrianAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_SUMMON_ZEPHYR, 4000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_2);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* garalon = pInstance->instance->GetCreature(pInstance->GetData64(NPC_GARALON)))
                        garalon->AI()->DoAction(ACTION_GARALON_VISIBLE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 64353 - Set'thik Gale-Slicer
class mob_set_thik_gale_slicer : public CreatureScript
{
    public:
        mob_set_thik_gale_slicer() : CreatureScript("mob_set_thik_gale_slicer") {}

        struct mob_set_thik_gale_slicerAI : public ScriptedAI
        {
            mob_set_thik_gale_slicerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_SWIFT_STEP, 4000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_7);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 64338 - Instructor Kli'thak <Keeper of Wind Step>
class mob_instructor_kli_thak : public CreatureScript
{
    public:
        mob_instructor_kli_thak() : CreatureScript("mob_instructor_kli_thak") {}

        struct mob_instructor_kli_thakAI : public ScriptedAI
        {
            mob_instructor_kli_thakAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_WIND_STEP, 15000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_7);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_WIND_STEP:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WIND_STEP_TP, true);
                            events.ScheduleEvent(EVENT_WIND_STEP, 22000);
                            break;
                        }
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

// 64339 - Instructor Tak'thok <Keeper of Overwhelming Assault>
class mob_instructor_tak_thok : public CreatureScript
{
    public:
        mob_instructor_tak_thok() : CreatureScript("mob_instructor_tak_thok") {}

        struct mob_instructor_tak_thokAI : public ScriptedAI
        {
            mob_instructor_tak_thokAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_OVERWHELMING_ASSAULT, 4000);

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_6);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    if (id == EVENT_OVERWHELMING_ASSAULT)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_OVERWHELMING_ASS, true);
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

// 64340 - Instructor Maltik <Keeper of Unseen Strike>
class mob_instructor_maltik : public CreatureScript
{
    public:
        mob_instructor_maltik() : CreatureScript("mob_instructor_maltik") { }

        struct mob_instructor_maltikAI : public ScriptedAI
        {
            mob_instructor_maltikAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_TRASH_UNSEEN_STRIKE, 15000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_1);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_TRASH_UNSEEN_STRIKE:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            {
                                me->CastSpell(target, SPELL_UNSEEN_STRIKE_TR, true);
                                me->AddAura(SPELL_UNSEEN_STRIKE_MKR, target);
                                me->GetMotionMaster()->MoveChase(target);
                                me->SetReactState(REACT_PASSIVE);
                            }
                            events.ScheduleEvent(EVENT_TRASH_UNSEEN_STRIKE, 22000);

                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_instructor_maltikAI(creature);
        }
};

// 64341 - Instructor Zarik <Keeper of Tempest Slash>
class mob_instructor_zarik : public CreatureScript
{
    public:
        mob_instructor_zarik() : CreatureScript("mob_instructor_zarik") { }

        struct mob_instructor_zarikAI : public ScriptedAI
        {
            mob_instructor_zarikAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_TRASH_TEMPEST_SLASH, 15000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_4);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_TRASH_TEMPEST_SLASH:
                        {
                            DoCast(me, SPELL_TEMPEST_SLASH);
                            events.ScheduleEvent(EVENT_TRASH_TEMPEST_SLASH, 22000);
                            break;
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_instructor_zarikAI(creature);
        }
};

// 63035 - Zar'thik Zealot
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

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_9);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 64357 - Kor'thik Swarmer
class mob_kor_thik_swarmer : public CreatureScript
{
    public:
        mob_kor_thik_swarmer() : CreatureScript("mob_kor_thik_swarmer") {}

        struct mob_kor_thik_swarmerAI : public ScriptedAI
        {
            mob_kor_thik_swarmerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_UNDERWHELMING_ASSAULT, 4000);

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_6);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 63592 - Set'thik Gustwing
class mob_set_thik_gustwing : public CreatureScript
{
    public:
        mob_set_thik_gustwing() : CreatureScript("mob_set_thik_gustwing") {}

        struct mob_set_thik_gustwingAI : public ScriptedAI
        {
            mob_set_thik_gustwingAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GUST, 3000);
                events.ScheduleEvent(EVENT_ICE_TRAP, 7000);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_3);
            }

            void JustDied()
            {
                if (pInstance)
                    if (Creature* garalon = pInstance->instance->GetCreature(pInstance->GetData64(NPC_GARALON)))
                        garalon->AI()->DoAction(ACTION_GARALON_VISIBLE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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

// 63597 - Coagulated Amber
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
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
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


// 64355 - Kor'thik Silentwing
class mob_kor_thik_silentwing : public CreatureScript
{
    public:
        mob_kor_thik_silentwing() : CreatureScript("mob_kor_thik_silentwing") { }

        struct mob_kor_thik_silentwingAI : public ScriptedAI
        {
            mob_kor_thik_silentwingAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_TRASH_1);
                events.ScheduleEvent(EVENT_ALMOST_UNSEEN_STRIKE, 15000);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (pInstance)
                    if (Creature* tayak = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TAYAK)))
                        tayak->AI()->DoAction(ACTION_TAYAK_TALK_TRASH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_ALMOST_UNSEEN_STRIKE:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ALMOST_UNSEEN_STRIKE, true);
                            events.ScheduleEvent(EVENT_ALMOST_UNSEEN_STRIKE, 22000);
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kor_thik_silentwingAI(creature);
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

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                int32 bp = (eventInfo.GetDamageInfo()->GetDamage() / 2) / 6;

                if (Unit* victim = eventInfo.GetProcTarget())
                    if (!victim->HasAura(SPELL_ARTERIAL_SPIRIT))
                        GetCaster()->CastCustomSpell(victim, SPELL_ARTERIAL_SPIRIT, &bp, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_vital_strikes_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_vital_strikes_AuraScript();
        }
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
    new mob_instructor_maltik();
    new mob_instructor_zarik();
    new mob_zar_thik_zealot();
    new mob_kor_thik_swarmer();
    new mob_set_thik_gustwing();
    new mob_coagulated_amber();
    new mob_kor_thik_silentwing();
    new spell_vital_strikes();
}