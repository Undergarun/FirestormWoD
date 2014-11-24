#include "instance_skyreach.h"

namespace MS
{

    class mob_SkyreachArcanologist : public CreatureScript
    {
    public:
        // Entry: 76376
        mob_SkyreachArcanologist()
            : CreatureScript("mob_SkyreachArcanologist")
        {
        }

        enum class Spells : uint32
        {
            ENERGIZE = 154179,
            SUBMERGED = 154163,
            SOLAR_DETONATION = 160288, // 7 seconds.
            SOLAR_STORM = 159215, // 12 to 15 seconds.
            SOLAR_STORM_1 = 159216,
            SOLAR_STORM_2 = 159218,
            SOLAR_STORM_3 = 159221,
            SOLAR_STORM_DMG = 159226,
        };

        enum class Events : uint32
        {
            SOLAR_DETONATION = 1,
            SOLAR_STORM = 2,
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_SkyreachArcanologistAI(creature);
        }

        struct mob_SkyreachArcanologistAI : public ScriptedAI
        {
            mob_SkyreachArcanologistAI(Creature* creature) : ScriptedAI(creature),
            m_instance(creature->GetInstanceScript()),
            m_events(),
            m_ConstructorBoss(0)
            {
            }

            void Reset()
            {
                m_events.Reset();

                // Get the boss.
                Creature* l_Creature = nullptr;
                if (!m_ConstructorBoss)
                {
                    l_Creature = InstanceSkyreach::SelectNearestCreatureWithEntry(me, 76141, 40.0f);
                    if (!l_Creature)
                        return;
                    m_ConstructorBoss = l_Creature->GetGUID();
                }
                else
                    l_Creature = me->GetMap()->GetCreature(m_ConstructorBoss);

                if (l_Creature)
                {
                    l_Creature->AddAura(uint32(Spells::SUBMERGED), l_Creature);
                    l_Creature->SetReactState(REACT_PASSIVE);
                }

                // Initialize all the Solar Constructors.
                std::list<Unit*> l_SolarConstructors = InstanceSkyreach::SelectNearestCreatureListWithEntry(me, 76142, 80.0f);
                for (Unit* l_Unit : l_SolarConstructors)
                {
                    if (l_Unit->HasAura(uint32(Spells::SUBMERGED)))
                        l_Unit->RemoveAura(uint32(Spells::SUBMERGED));

                    if (l_Unit->ToCreature())
                        l_Unit->ToCreature()->SetReactState(REACT_PASSIVE);
                    l_Unit->CastSpell(l_Creature, uint32(Spells::ENERGIZE), true);
                }
            }

            void JustDied(Unit*)
            {
                if (!m_ConstructorBoss)
                    return;

                // When just died, the boss is starting to attack and solar constructors stops casting Energize.
                Creature* l_Boss = me->GetMap()->GetCreature(m_ConstructorBoss);
                if (!l_Boss)
                    return;

                l_Boss->RemoveAura(uint32(Spells::SUBMERGED));
                l_Boss->SetReactState(REACT_AGGRESSIVE);

                std::list<Unit*> l_SolarConstructors = InstanceSkyreach::SelectNearestCreatureListWithEntry(me, 76142, 80.0f);

                for (Unit* l_Unit : l_SolarConstructors)
                {
                    l_Unit->AddAura(uint32(Spells::SUBMERGED), l_Unit);
                    l_Unit->CastStop();
                }
            }

            void EnterCombat(Unit* who)
            {
                m_events.ScheduleEvent(uint32(Events::SOLAR_DETONATION), urand(2500, 7500));
                m_events.ScheduleEvent(uint32(Events::SOLAR_STORM), urand(5000, 10000));
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
                    case uint32(Events::SOLAR_DETONATION):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_DETONATION), urand(6500, 7500));
                        if (Player* l_Plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 15.0f))
                            me->CastSpell(l_Plr, uint32(Spells::SOLAR_DETONATION));
                        break;
                    case uint32(Events::SOLAR_STORM):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_STORM), urand(12000, 15000));
                        me->CastSpell(me->getVictim(), uint32(Spells::SOLAR_STORM));
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
            uint64 m_ConstructorBoss;
        };
    };

    class boss_Araknath : public CreatureScript
    {
    public:
        // Entry: 76141
        boss_Araknath()
            : CreatureScript("boss_Araknath")
        {
        }

        enum class Spells : uint32
        {
            MELEE = 154121, // Every 2 seconds.
            SMASH = 154113, // Every 6 to 7 seconds.
            BURST = 154135, // Every 23 seconds.
            BREATH_OF_SINDRAGOSA = 155168,
            ENERGIZE = 154179, // During 15 seconds, restart after 3 seconds.
            SUBMERGED = 154163,
        };

        enum class Events : uint32
        {
            SHUT_DOORS = 1,
            MELEE = 2,
            SMASH = 3,
            BURST = 4,
            ENERGIZE_START = 5,
            ENERGIZE_STOP = 6
        };

        ScriptedAI* GetAI(Creature* creature) const
        {
            return new boss_AraknathAI(creature);
        }

        struct boss_AraknathAI : public ScriptedAI
        {
            boss_AraknathAI(Creature* creature) : ScriptedAI(creature),
            m_instance(creature->GetInstanceScript()),
            m_events(),
            m_HasCombatStarted(false),
            m_SolarConstructorInUse(0)
            {
            }

            void Reset()
            {
                m_events.Reset();
                m_SolarConstructorInUse = 0;
            }

            void JustDied(Unit* /*killer*/)
            {
                // Open the doors.
                if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234312, 40.0f))
                    l_Go->UseDoorOrButton();
                if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234313, 40.0f))
                    l_Go->UseDoorOrButton();

                if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234315, 40.0f))
                    l_Go->UseDoorOrButton();
                if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234314, 40.0f))
                    l_Go->UseDoorOrButton();
            }

            void EnterEvadeMode()
            {
                // If the boss wins.
                events.Reset();

                /// Open the entrance doors.
                if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234315, 40.0f))
                    l_Go->UseDoorOrButton();
                if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234314, 40.0f))
                    l_Go->UseDoorOrButton();
            }

            void KilledUnit(Unit* /*victim*/)
            {
            }

            void EnterCombat(Unit* who)
            {
                m_HasCombatStarted = true;
                m_events.ScheduleEvent(uint32(Events::SHUT_DOORS), 3000);
                m_events.ScheduleEvent(uint32(Events::MELEE), 2000);
                m_events.ScheduleEvent(uint32(Events::SMASH), urand(5500, 7000));
                m_events.ScheduleEvent(uint32(Events::BURST), urand(21500, 23000));
                m_events.ScheduleEvent(uint32(Events::ENERGIZE_START), 15000);
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
                    case uint32(Events::SHUT_DOORS):
                        // Shut the door.
                        if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234315, 40.0f))
                            l_Go->UseDoorOrButton();
                        if (GameObject* l_Go = InstanceSkyreach::SelectNearestGameObjectWithEntry(me, 234314, 40.0f))
                            l_Go->UseDoorOrButton();
                        break;
                    case uint32(Events::MELEE):
                        m_events.ScheduleEvent(uint32(Events::MELEE), 2000);
                        me->CastSpell(me->getVictim(), uint32(Spells::MELEE));
                        break;
                    case uint32(Events::SMASH):
                        m_events.ScheduleEvent(uint32(Events::SMASH), urand(5500, 7000));
                        me->CastSpell(me->getVictim(), uint32(Spells::SMASH));
                        break;
                    case uint32(Events::BURST):
                        m_events.ScheduleEvent(uint32(Events::BURST), urand(21500, 23000));
                        me->CastSpell(me->getVictim(), uint32(Spells::BURST));
                        break;
                    case uint32(Events::ENERGIZE_START):
                        m_events.ScheduleEvent(uint32(Events::ENERGIZE_STOP), 15000);
                        
                        /*if (Unit* l_SolarConstructor = InstanceSkyreach::SelectRandomCreatureWithEntry(me, 76142, 80.0f))
                        {
                            m_SolarConstructorInUse = l_SolarConstructor->GetGUID();
                            if (l_SolarConstructor->HasAura(uint32(Spells::SUBMERGED)))
                                l_SolarConstructor->RemoveAura(uint32(Spells::SUBMERGED));
                            l_SolarConstructor->CastSpell(me, uint32(Spells::ENERGIZE), true);
                        }*/
                        break;
                    case uint32(Events::ENERGIZE_STOP):
                        m_events.ScheduleEvent(uint32(Events::ENERGIZE_START), 3000);
                        /*if (Unit* l_SolarConstructor = me->GetMap()->GetCreature(m_SolarConstructorInUse))
                        {
                            m_SolarConstructorInUse = 0;
                            l_SolarConstructor->AddAura(uint32(Spells::SUBMERGED), l_SolarConstructor);
                            l_SolarConstructor->CastStop();
                        }
                        break;*/
                    default:
                        break;
                    }

                    // If we cast something, we don't want to execute the other events.
                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }
            }

            InstanceScript* m_instance;
            EventMap m_events;
            bool m_HasCombatStarted;
            uint64 m_SolarConstructorInUse;
        };
    };
}

void AddSC_boss_Araknath()
{
    new MS::mob_SkyreachArcanologist();
    new MS::boss_Araknath();
}