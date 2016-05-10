////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
};

enum eEvents
{
};

enum eSays
{
};

class boss_kilruk_the_wind_reaver : public CreatureScript
{
    public:
        boss_kilruk_the_wind_reaver() : CreatureScript("boss_kilruk_the_wind_reaver") { }

        struct boss_kilruk_the_wind_reaverAI : public BossAI
        {
            boss_kilruk_the_wind_reaverAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_kilruk_the_wind_reaverAI(creature);
        }
};

class boss_xaril_the_poisoned_mind : public CreatureScript
{
    public:
        boss_xaril_the_poisoned_mind() : CreatureScript("boss_xaril_the_poisoned_mind") { }

        struct boss_xaril_the_poisoned_mindAI : public BossAI
        {
            boss_xaril_the_poisoned_mindAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
        return new boss_xaril_the_poisoned_mindAI(creature);
        }
};

class boss_kaztik_the_manipulator : public CreatureScript
{
    public:
        boss_kaztik_the_manipulator() : CreatureScript("boss_kaztik_the_manipulator") { }

        struct boss_kaztik_the_manipulatorAI : public BossAI
        {
            boss_kaztik_the_manipulatorAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
            _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
        return new boss_kaztik_the_manipulatorAI(creature);
        }
};

class boss_korven_the_prime : public CreatureScript
{
    public:
        boss_korven_the_prime() : CreatureScript("boss_korven_the_prime") { }

        struct boss_korven_the_primeAI : public BossAI
        {
            boss_korven_the_primeAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_korven_the_primeAI(creature);
        }
};

class boss_iyyokuk_the_lucid : public CreatureScript
{
    public:
        boss_iyyokuk_the_lucid() : CreatureScript("boss_iyyokuk_the_lucid") { }

        struct boss_iyyokuk_the_lucidAI : public BossAI
        {
            boss_iyyokuk_the_lucidAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_iyyokuk_the_lucidAI(creature);
        }
};

class boss_karoz_the_locust : public CreatureScript
{
    public:
        boss_karoz_the_locust() : CreatureScript("boss_karoz_the_locust") { }

        struct boss_karoz_the_locustAI : public BossAI
        {
            boss_karoz_the_locustAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_karoz_the_locustAI(creature);
        }
};

class boss_skeer_the_bloodseeker : public CreatureScript
{
    public:
        boss_skeer_the_bloodseeker() : CreatureScript("boss_skeer_the_bloodseeker") { }

        struct boss_skeer_the_bloodseekerAI : public BossAI
        {
            boss_skeer_the_bloodseekerAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_skeer_the_bloodseekerAI(creature);
        }
};

class boss_rikkal_the_dissector : public CreatureScript
{
    public:
        boss_rikkal_the_dissector() : CreatureScript("boss_rikkal_the_dissector") { }

        struct boss_rikkal_the_dissectorAI : public BossAI
        {
            boss_rikkal_the_dissectorAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
            _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_rikkal_the_dissectorAI(creature);
        }
};

class boss_hisek_the_swarmkeeper : public CreatureScript
{
    public:
        boss_hisek_the_swarmkeeper() : CreatureScript("boss_hisek_the_swarmkeeper") { }

        struct boss_hisek_the_swarmkeeperAI : public BossAI
        {
            boss_hisek_the_swarmkeeperAI(Creature* creature) : BossAI(creature, DATA_PARAGONS_OF_THE_KLAXXI)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, IN_PROGRESS);
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
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_PARAGONS_OF_THE_KLAXXI, DONE);
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

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_hisek_the_swarmkeeperAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_paragons_of_the_klaxxi()
{
    new boss_kilruk_the_wind_reaver();
    new boss_xaril_the_poisoned_mind();
    new boss_kaztik_the_manipulator();
    new boss_korven_the_prime();
    new boss_iyyokuk_the_lucid();
    new boss_karoz_the_locust();
    new boss_skeer_the_bloodseeker();
    new boss_rikkal_the_dissector();
    new boss_hisek_the_swarmkeeper();
};
#endif