#include "instance_skyreach.h"

namespace MS
{
    namespace InstanceSkyreach
    {
        static const Position k_Waypoints[12] =
        {
            { 929.272f, 1937.448f, 224.75f },
            { 910.023f, 1921.640f, 219.96f },
            { 896.675f, 1901.697f, 219.96f },
            { 898.726f, 1872.698f, 224.75f },
            { 876.896f, 1888.450f, 224.75f },
            { 895.448f, 1928.450f, 224.75f },


            { 895.448f, 1928.450f, 224.75f },
            { 876.896f, 1888.450f, 224.75f },
            { 898.726f, 1872.698f, 224.75f },
            { 896.675f, 1901.697f, 219.96f },
            { 910.023f, 1921.640f, 219.96f },
            { 929.272f, 1937.448f, 224.75f },
        };

        class mob_SkyreachRavenWhisperer : public CreatureScript
        {
        public:
            // Entry: 76154
            mob_SkyreachRavenWhisperer()
                : CreatureScript("mob_SkyreachRavenWhisperer")
            {
            }

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SkyreachRavenWhispererAI(creature);
            }

            struct mob_SkyreachRavenWhispererAI : public ScriptedAI
            {
                mob_SkyreachRavenWhispererAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void JustDied(Unit*)
                {
                    if (m_Instance)
                        m_Instance->SetData(Data::SkyreachRavenWhispererIsDead, 0);
                }

                void UpdateAI(const uint32 diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_events.Update(diff);

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class boss_Rukhran : public CreatureScript
        {
        public:
            // Entry: 76143
            boss_Rukhran()
                : CreatureScript("boss_Rukhran")
            {
            }

            enum class Events : uint32
            {
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new boss_RukranAI(creature);
            }

            struct boss_RukranAI : public BossAI
            {
                boss_RukranAI(Creature* creature)
                : BossAI(creature, Data::Rukhran),
                m_WaypointId(0),
                m_CombatStarted(false)
                {
                    if (instance)
                        instance->SetBossState(Data::Rukhran, TO_BE_DECIDED);

                    me->SetDisableGravity(true);
                    me->SetCanFly(true);
                    me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                }

                void Reset()
                {
                    _Reset();

                    m_WaypointId = 0;
                    m_CombatStarted = false;
                    me->SetReactState(REACT_PASSIVE);
                    me->GetMotionMaster()->MovePoint(m_WaypointId, k_Waypoints[0]);
                    me->SetControlled(false, UNIT_STATE_ROOT);
                }

                void MovementInform(uint32 p_Type, uint32 p_Id)
                {
                    switch (p_Type)
                    {
                    case POINT_MOTION_TYPE:
                        if (p_Id == 12)
                        {
                            me->SetInCombatWithZone();
                            me->SetControlled(true, UNIT_STATE_ROOT);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->Attack(InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 40.0f, false), true);
                            m_CombatStarted = true;
                        }
                        else
                        {
                            m_WaypointId = (m_WaypointId + 1) % 12;
                            me->GetMotionMaster()->MovePoint(m_WaypointId, k_Waypoints[m_WaypointId]);
                        }
                        break;
                    }
                }

                void EnterEvadeMode()
                {
                    me->ClearInCombat();
                    me->CombatStop();

                    if (instance)
                    {
                        instance->SetBossState(Data::Rukhran, FAIL);
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    }
                }

                void JustReachedHome()
                {
                    _JustReachedHome();
                }

                void JustDied(Unit* /*killer*/)
                {
                    _JustDied();

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                void KilledUnit(Unit* /*victim*/)
                {
                }

                void EnterCombat(Unit* who)
                {
                    _EnterCombat();

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }

                void UpdateAI(const uint32 diff)
                {
                    if (!UpdateVictim())
                    {
                        if (m_CombatStarted)
                        {
                            Reset();
                            EnterEvadeMode();
                        }
                        return;
                    }

                    events.Update(diff);

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;

                    switch (events.ExecuteEvent())
                    {
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                uint32 m_WaypointId;
                bool m_CombatStarted;
            };
        };
    }
}

void AddSC_boss_Rukhran()
{
    new MS::InstanceSkyreach::boss_Rukhran();
    new MS::InstanceSkyreach::mob_SkyreachRavenWhisperer();
}