#include "instance_skyreach.h"

namespace MS
{
    class boss_Ranjit : public CreatureScript
    {
    public:
        // Entry: 76205
        boss_Ranjit()
            : CreatureScript("boss_Ranjit")
        {
        }

        enum class Spells : uint32
        {
            // Windwall.
            WINDWALL = 153315,      // 2:43:38 - 2-43-52
            WINDWALL_2 = 153311,
            WINDWALL_3 = 153314,
            WINDWALL_4 = 153593,
            WINDWALL_5 = 153594,
            WINDWALL_6 = 153759,
            // Four winds.
            FOUR_WINDS = 156793,
            FOUR_WINDS_2 = 153139,
            FOUR_WINDS_3 = 156634,
            FOUR_WINDS_4 = 156636,
            FOUR_WINDS_5 = 166623,
            FOUR_WINDS_6 = 166664,
            // Fan of blades.
            FAN_OF_BLADES = 153757, // 2:43:34 - 2:43:50
            // Piercing rush.
            PIERCING_RUSH = 165731, // 2:43:29 - 2:43:44
        };

        enum class Texts : uint32
        {
            COMBAT_START = 0,
        };

        enum class Events : uint32
        {
            WINDWALL = 1,
            FOUR_WINDS = 2,
            PIERCING_RUSH = 3,
            FAN_OF_BLADES = 4,
            LENS_FLARE = 5
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_RanjitAI(creature);
        }

        struct boss_RanjitAI : public ScriptedAI
        {
            boss_RanjitAI(Creature* creature) : ScriptedAI(creature),
            m_instance(creature->GetInstanceScript()),
            m_events()
            {
            }

            void Reset()
            {
                m_events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), urand(5000, 13000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_events.Update(diff);

                while (uint32 eventId = m_events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case uint32(Events::PIERCING_RUSH):
                        m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), urand(5000, 13000));
                        me->CastSpell(me->getVictim(), uint32(Spells::PIERCING_RUSH));
                        break;
                    default:
                        break;
                    }

                    // If we cast something, we don't want to execute the other events.
                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }

                DoMeleeAttackIfReady();
            }

            InstanceScript* m_instance;
            EventMap m_events;
        };
    };
}

void AddSC_boss_ranjit()
{

}