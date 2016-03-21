/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
    SPELL_ACCELERATION             =143411,
    SPELL_ACID_BREATH              =143780,
    SPELL_BERSERK                  =26662,
    SPELL_BLOOD_FRENZY             =143442,
    SPELL_DEAFENING_SCREECH        =143343,
    SPELL_FEARSOME_ROAR            =143426,
    SPELL_FIXATE                   =146540,
    SPELL_FREEZING_BREATH          =143773,
    SPELL_MENDING                  =149569,
    SPELL_PANIC                    =143766,
    SPELL_SCORCHING_BREATH         =143767,
    SPELL_SUMMON_PLAYER            =25104,
    SPELL_TAIL_LASH                =143428
};

enum eEvents
{
};

enum eSays
{
};

class boss_thok_the_bloodthirsty : public CreatureScript
{
    public:
        boss_thok_the_bloodthirsty() : CreatureScript("boss_thok_the_bloodthirsty") { }

        struct boss_thok_the_bloodthirstyAI : public BossAI
        {
            boss_thok_the_bloodthirstyAI(Creature* creature) : BossAI(creature, DATA_THOK_THE_BLOODTHIRSTY)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_THOK_THE_BLOODTHIRSTY, FAIL);
            }
            
            void EnterCombat(Unit* attacker)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_THOK_THE_BLOODTHIRSTY, IN_PROGRESS);
                }
            }
            
            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }
            
            void KilledUnit(Unit* who)
            {
            }
            
            void JustDied(Unit* killer)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_THOK_THE_BLOODTHIRSTY, DONE);
                }
            }
            
            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_thok_the_bloodthirstyAI(pCreature);
        }
};

class mob_korkron_jailer : public CreatureScript
{
    public:
        mob_korkron_jailer() : CreatureScript("mob_korkron_jailer") { }

        struct mob_korkron_jailerAI : public ScriptedAI
        {
            mob_korkron_jailerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                return;
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
        return new mob_korkron_jailerAI(creature);
        }
};

class mob_akolik : public CreatureScript
{
    public:
        mob_akolik() : CreatureScript("mob_akolik") { }

        struct mob_akolikAI : public ScriptedAI
        {
            mob_akolikAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
        return new mob_akolikAI(creature);
        }
};

class mob_waterspeaker_gorai : public CreatureScript
{
    public:
        mob_waterspeaker_gorai() : CreatureScript("mob_waterspeaker_gorai") { }

        struct mob_waterspeaker_goraiAI : public ScriptedAI
        {
            mob_waterspeaker_goraiAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
        return new mob_waterspeaker_goraiAI(creature);
        }
};

class mob_montak : public CreatureScript
{
    public:
        mob_montak() : CreatureScript("mob_montak") { }

        struct mob_montakAI : public ScriptedAI
        {
            mob_montakAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_montakAI(creature);
        }
};

void AddSC_thok_the_bloodthirsty()
{
    new boss_thok_the_bloodthirsty();
    new mob_korkron_jailer();
    new mob_akolik();
    new mob_waterspeaker_gorai();
    new mob_montak();
};