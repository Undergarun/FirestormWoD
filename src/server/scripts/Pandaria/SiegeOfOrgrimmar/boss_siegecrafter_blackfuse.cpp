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
    SPELL_ELECTROSTATIC_CHARGE        = 143385,
    SPELL_OVERCHARGE                  = 145774,
    SPELL_LAUNCH_SAWBLADE             = 143265,
    SPELL_PROTECTIVE_FRENZY           = 145365
};

enum eEvents
{
};

enum eSays
{
    /*
    Siegecrafter Blackfuse yells: Alright, Shanna. You're up.
    Siegecrafter Blackfuse yells: Another satisfied customer.
    Siegecrafter Blackfuse yells: Goro'dan! You're on deck!
    Siegecrafter Blackfuse yells: Hey! You're gonna pay for that!
    Siegecrafter Blackfuse yells: Hey, I heard you like mines, so... enjoy!
    Siegecrafter Blackfuse yells: Let's settle this.
    Siegecrafter Blackfuse yells: Looks like I'm in the market for a new assistant.
    Siegecrafter Blackfuse yells: Meet the LT-403 Evaporation laser. A personal point of pride.
    Siegecrafter Blackfuse yells: Presenting... the beautiful new ST-03 Shockwave missile turret!
    Siegecrafter Blackfuse yells: That's for the Juggernaut.
    Siegecrafter Blackfuse yells: The SD-5 Automated Shredder defense unit. A labor of love.
    Siegecrafter Blackfuse yells: Time to mix business with pleasure.
    Siegecrafter Blackfuse yells: What's better than a bomb, huh?! I'll tell ya - a bomb with two bombs inside!
    Siegecrafter Blackfuse yells: Ya' ever wonder how magnets work?
    Siegecrafter Blackfuse yells: You son of a...
    Siegecrafter Blackfuse says: Don't... think you've won...
    Siegecrafter Blackfuse says: My legacy... will live on...
    */
};

class boss_siegecrafter_blackfuse : public CreatureScript
{
    public:
        boss_siegecrafter_blackfuse() : CreatureScript("boss_siegecrafter_blackfuse") { }

        struct boss_siegecrafter_blackfuseAI : public BossAI
        {
            boss_siegecrafter_blackfuseAI(Creature* creature) : BossAI(creature, DATA_SIEGECRAFTER_BLACKFUSE)
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
                    pInstance->SetBossState(DATA_SIEGECRAFTER_BLACKFUSE, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_SIEGECRAFTER_BLACKFUSE, IN_PROGRESS);
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
                    pInstance->SetBossState(DATA_SIEGECRAFTER_BLACKFUSE, DONE);
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
            return new boss_siegecrafter_blackfuseAI(pCreature);
        }
};

class mob_automated_shredder : public CreatureScript
{
    public:
        mob_automated_shredder() : CreatureScript("mob_automated_shredder") { }

        struct mob_automated_shredderAI : public ScriptedAI
        {
            mob_automated_shredderAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (!UpdateVictim())
                    return;
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_automated_shredderAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_siegecrafter_blackfuse()
{
    new boss_siegecrafter_blackfuse();
    new mob_automated_shredder();
};
#endif
