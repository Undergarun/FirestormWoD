////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
/*
TODO:
       Find the real timer.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "shadopan_monastery.h"

enum eSpells
{
    SPELL_SMOKE_BLADES          = 106826,
    SPELL_SHA_SPIKE             = 106871,
    SPELL_DISORIENTING_SMASH    = 106872,
    SPELL_PARTING_SMOKE         = 127576,
    SPELL_ENRAGE                = 130196,

    SPELL_ICE_TRAP              = 110610,
    SPELL_EXPLOSION             = 106966
};

enum eEvents
{
    // Gu
    EVENT_SMOKE_BLADES          = 1,
    EVENT_SHA_SPIKE             = 2,
    EVENT_DISORIENTING_SMASH    = 3
};

class boss_sha_of_violence : public CreatureScript
{
    public:
        boss_sha_of_violence() : CreatureScript("boss_sha_of_violence") {}

        struct boss_sha_of_violenceAI : public BossAI
        {
            boss_sha_of_violenceAI(Creature* creature) : BossAI(creature, DATA_SHA_VIOLENCE)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            bool enrageDone;

            void Reset()
            {
                _Reset();
                enrageDone = false;

                events.ScheduleEvent(EVENT_SMOKE_BLADES,        urand(25000, 35000));
                events.ScheduleEvent(EVENT_SHA_SPIKE,           urand(10000, 20000));
                events.ScheduleEvent(EVENT_DISORIENTING_SMASH,  urand(20000, 30000));
            }

            void JustReachedHome()
            {
                pInstance->SetBossState(DATA_SHA_VIOLENCE, FAIL);
                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->CastSpell(summon, SPELL_ICE_TRAP, true);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!enrageDone && me->HealthBelowPctDamaged(20, damage))
                    me->CastSpell(me, SPELL_ENRAGE, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch(events.ExecuteEvent())
                {
                    case EVENT_SMOKE_BLADES:
                        me->CastSpell(me, SPELL_SMOKE_BLADES, false);
                        events.ScheduleEvent(EVENT_SMOKE_BLADES,        urand(25000, 35000));
                        break;
                    case EVENT_SHA_SPIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            me->CastSpell(target, SPELL_SHA_SPIKE, false);

                        events.ScheduleEvent(EVENT_SHA_SPIKE,           urand(10000, 20000));
                        break;
                    case EVENT_DISORIENTING_SMASH:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            me->CastSpell(target, SPELL_DISORIENTING_SMASH, false);

                        events.ScheduleEvent(EVENT_DISORIENTING_SMASH,  urand(20000, 30000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_violenceAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_sha_of_violence()
{
    new boss_sha_of_violence();
}
#endif