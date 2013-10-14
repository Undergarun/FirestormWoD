#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"

enum eNessosSpells
{
    SPELL_THROW        = 38557,
};

enum eZhaoEvents
{
    EVENT_THROW        = 1,
};

class mob_high_road_scout : public CreatureScript
{
    public:
        mob_high_road_scout() : CreatureScript("mob_high_road_scout") { }

        struct mob_high_road_scoutAI : public ScriptedAI
        {
            mob_high_road_scoutAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_THROW,      7000);
            }

            void JustDied(Unit* /*killer*/)
            {
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
                        case EVENT_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_THROW, false);
                            events.ScheduleEvent(EVENT_THROW,      7000);
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
            return new mob_high_road_scoutAI(creature);
        }
};

void AddSC_southern_barrens()
{
    new mob_high_road_scout();
}