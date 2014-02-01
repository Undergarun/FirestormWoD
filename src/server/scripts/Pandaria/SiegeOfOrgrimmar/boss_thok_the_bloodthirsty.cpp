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
    /*
    SPELL_ACCELERATION	           =143411,	 // Rate of energy regeneration increased.
    SPELL_ACID_BREATH	           =143780,  // Inflicts 487500 to 512500 Nature damage in a frontal cone. The acid coats the armor of enemies, reducing its effectiveness by 20% per stack.
    SPELL_BERSERK	               =26662,   // Increases the caster's attack and movement speeds by 150% and all damage it deals by 500%.  Also grants immunity to Taunt effects
    SPELL_BLOOD_FRENZY             =143442,	 // This unit is in a frenzy for blood! Movement speed periodically increasing.
    SPELL_BLOOD_FRENZY             =143440,  // The caster grows more powerful over time, increasing damage done and movement speed. Stacks.
    SPELL_DEAFENING_SCREECH	       =143343,  // Inflicts 292500 to 307500 Physical damage to all enemies and interrupts spellcasting for 2 sec.
    SPELL_FEARSOME_ROAR            =143426,  // Inflicts 195000 to 205000 Physical damage in a frontal cone. The roar incites terror in the heart of enemies, causing them to take 25% more damage per stack.
    SPELL_FIXATE                   =146540,  // Fixated
    SPELL_FIXATE                   =146581,  // Gul'rok smells your blood and pursues you!
    SPELL_FIXATE	               =143445,  // Fixating.
    SPELL_FREEZING_BREATH          =143773,  // Inflicts 195000 to 205000 Frost damage in a frontal cone. After 5 stacks, the target will be frozen in a tomb of ice.
    SPELL_MENDING                  =149569,  // Heals the caster for 8% of their maximum life
    SPELL_PANIC                    =143766,  // Inflicts 195000 to 205000 Sonic damage in a frontal cone. The roar incites terror in the heart of enemies, this panic causes them to take 25% more damage per stack.
    SPELL_SCORCHING_BREATH         =143767,  // Inflicts 195000 to 205000 Fire damage in a frontal cone. The flames sear enemy flesh, causing 75000 Fire damage every 2 seconds.
    SPELL_SUMMON_PLAYER            =25104,   // Summons an enemy player.
    SPELL_TAIL_LASH                =143428,  // A sweeping tail strike hits all enemies behind the caster, inflicting 195000 to 205000 damage and stunning them for 2 sec.
    */
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