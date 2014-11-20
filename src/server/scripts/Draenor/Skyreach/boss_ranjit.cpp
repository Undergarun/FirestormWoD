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
            WINDWALL = 153315,      // 2:43:38 - 2-43-52. Random target. Every 14s.
            WINDWALL_AT_1 = 153311,
            WINDWALL_AT_2 = 153314,
            WINDWALL_MISSLE_1 = 153593,
            WINDWALL_MISSILE_2 = 153594,
            WINDWALL_DMG = 153759,
            // Four winds. Arrived after 2 or 3 instances of WindWall.
            FOUR_WINDS = 156793,
            FOUR_WINDS_2 = 153139,
            FOUR_WINDS_3 = 156634,
            FOUR_WINDS_4 = 156636,
            FOUR_WINDS_5 = 166623,
            FOUR_WINDS_6 = 166664,
            // Fan of blades.
            FAN_OF_BLADES = 153757, // 2:43:34 - 2:43:50, every 16s.
            // Piercing rush.
            PIERCING_RUSH = 165731, // 2:43:29 - 2:43:44, every 15s.
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
            m_events(),
            m_countWindwalls(0)
            {
            }

            void Reset()
            {
                m_events.Reset();

                m_countWindwalls = 0;
            }

            void JustDied(Unit* /*killer*/)
            {
            }

            void KilledUnit(Unit* /*victim*/)
            {
            }

            void EnterCombat(Unit* who)
            {
                m_events.ScheduleEvent(uint32(Events::WINDWALL), 8000);
                m_events.ScheduleEvent(uint32(Events::FAN_OF_BLADES), 5000);
                m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), 1000);
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
                    case uint32(Events::FOUR_WINDS):
                        break;
                    case uint32(Events::WINDWALL):
                        m_events.ScheduleEvent(uint32(Events::WINDWALL), urand(13000, 14000));
                        
                        if (m_countWindwalls++ == 2)
                            m_events.ScheduleEvent(uint32(Events::FOUR_WINDS), urand(2000, 3000));
                        break;
                    case uint32(Events::FAN_OF_BLADES):
                        m_events.ScheduleEvent(uint32(Events::FAN_OF_BLADES), urand(15000, 17000));
                        break;
                    case uint32(Events::PIERCING_RUSH):
                        m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), urand(13000, 16000));
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
            uint32 m_countWindwalls;
        };
    };
}

void AddSC_boss_ranjit()
{

}