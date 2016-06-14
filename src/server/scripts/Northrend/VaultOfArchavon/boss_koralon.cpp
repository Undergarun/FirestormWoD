////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "vault_of_archavon.h"

enum Events
{
    // Koralon
    EVENT_BURNING_BREATH    = 1,
    EVENT_BURNING_FURY      = 2,
    EVENT_FLAME_CINDER_A    = 3,
    EVENT_METEOR_FISTS_A    = 4,
    EVENT_METEOR_FISTS_B    = 5,

    // Flame Warder
    EVENT_FW_LAVA_BIRST     = 6,
    EVENT_FW_METEOR_FISTS_A = 7,
    EVENT_FW_METEOR_FISTS_B = 8
};

enum Spells
{
    // Spells Koralon
    SPELL_BURNING_BREATH                        = 66665,
    SPELL_BURNING_FURY                          = 66721,
    SPELL_FLAME_CINDER_A                        = 66684,
    SPELL_FLAME_CINDER_B                        = 66681, // don't know the real relation to SPELL_FLAME_CINDER_A atm.
    SPELL_METEOR_FISTS_A                        = 66725,
    SPELL_METEOR_FISTS_B                        = 67333,

    // Spells Flame Warder
    SPELL_FW_LAVA_BIRST                         = 66813,
    SPELL_FW_METEOR_FISTS_A                     = 66808,
    SPELL_FW_METEOR_FISTS_B                     = 67331
};

class boss_koralon : public CreatureScript
{
    public:
        boss_koralon() : CreatureScript("boss_koralon") { }

        struct boss_koralonAI : public BossAI
        {
            boss_koralonAI(Creature* creature) : BossAI(creature, DATA_KORALON)
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_BURNING_FURY);

                events.ScheduleEvent(EVENT_BURNING_FURY, 20000);    // TODO check timer
                events.ScheduleEvent(EVENT_BURNING_BREATH, 15000);  // 1st after 15sec, then every 45sec
                events.ScheduleEvent(EVENT_METEOR_FISTS_A, 75000);  // 1st after 75sec, then every 45sec
                events.ScheduleEvent(EVENT_FLAME_CINDER_A, 30000);  // TODO check timer

                _EnterCombat();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BURNING_FURY:
                            DoCast(me, SPELL_BURNING_FURY);
                            events.ScheduleEvent(EVENT_BURNING_FURY, 20000);
                            break;
                        case EVENT_BURNING_BREATH:
                            DoCast(me, SPELL_BURNING_BREATH);
                            events.ScheduleEvent(EVENT_BURNING_BREATH, 45000);
                            break;
                        case EVENT_METEOR_FISTS_A:
                            DoCast(me, SPELL_METEOR_FISTS_A);
                            events.ScheduleEvent(EVENT_METEOR_FISTS_B, 1500);
                            break;
                        case EVENT_METEOR_FISTS_B:
                            DoCast(me, SPELL_METEOR_FISTS_B);
                            events.ScheduleEvent(EVENT_METEOR_FISTS_A, 45000);
                            break;
                        case EVENT_FLAME_CINDER_A:
                            DoCast(me, SPELL_FLAME_CINDER_A);
                            events.ScheduleEvent(EVENT_FLAME_CINDER_A, 30000);
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
            return new boss_koralonAI(creature);
        }
};

/*######
##  Mob Flame Warder
######*/
class mob_flame_warder : public CreatureScript
{
    public:
        mob_flame_warder() : CreatureScript("mob_flame_warder") { }

        struct mob_flame_warderAI : public ScriptedAI
        {
            mob_flame_warderAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoZoneInCombat();

                events.ScheduleEvent(EVENT_FW_LAVA_BIRST, 5000);
                events.ScheduleEvent(EVENT_FW_METEOR_FISTS_A, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FW_LAVA_BIRST:
                            DoCastVictim(SPELL_FW_LAVA_BIRST);
                            events.ScheduleEvent(EVENT_FW_LAVA_BIRST, 15000);
                            break;
                        case EVENT_FW_METEOR_FISTS_A:
                            DoCast(me, SPELL_FW_METEOR_FISTS_A);
                            events.ScheduleEvent(EVENT_FW_METEOR_FISTS_B, 1500);
                            break;
                        case EVENT_FW_METEOR_FISTS_B:
                            DoCast(me, SPELL_FW_METEOR_FISTS_B);
                            events.ScheduleEvent(EVENT_FW_METEOR_FISTS_A, 20000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_flame_warderAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_koralon()
{
    new boss_koralon();
    new mob_flame_warder();
}
#endif
