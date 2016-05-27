#include "instance_skyreach.hpp"

namespace MS
{
    namespace InstanceSkyreach
    {
        static const Position k_Waypoints[12] =
        {
            { 929.272f, 1937.448f, 224.75f, 0.0f},
            { 910.023f, 1921.640f, 219.96f, 0.0f},
            { 896.675f, 1901.697f, 219.96f, 0.0f},
            { 898.726f, 1872.698f, 224.75f, 0.0f},
            { 876.896f, 1888.450f, 224.75f, 0.0f},
            { 895.448f, 1928.450f, 224.75f, 0.0f},


            { 895.448f, 1928.450f, 224.75f, 0.0f},
            { 876.896f, 1888.450f, 224.75f, 0.0f},
            { 898.726f, 1872.698f, 224.75f, 0.0f},
            { 896.675f, 1901.697f, 219.96f, 0.0f},
            { 910.023f, 1921.640f, 219.96f, 0.0f},
            { 929.272f, 1937.448f, 224.75f, 0.0f},
        };

        static const Position k_RandomSummonSolarFlare[] =
        {
            { 939.31f, 1907.440f, 213.86f, 0.0f },
            { 938.64f, 1899.550f, 213.86f, 0.0f },
            { 942.61f, 1915.518f, 214.40f, 0.0f },
            { 933.88f, 1908.990f, 213.86f, 0.0f },
            { 926.94f, 1898.300f, 213.86f, 0.0f },
            { 918.43f, 1892.141f, 213.86f, 0.0f },
            { 913.15f, 1887.884f, 213.86f, 0.0f },
            { 920.61f, 1882.406f, 213.86f, 0.0f },
            { 917.99f, 1876.340f, 213.86f, 0.0f },
            { 918.28f, 1884.108f, 213.86f, 0.0f },
            { 909.16f, 1883.930f, 214.40f, 0.0f },
        };

        class mob_SolarFlare : public CreatureScript
        {
        public:
            // Entry: 76227
            mob_SolarFlare()
                : CreatureScript("mob_SolarFlare")
            {
            }

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SolarFlareAI(creature);
            }

            enum class Spells : uint32
            {
                SUNSTRIKE = 153828,
                SUNBURST = 169810,
                SOLAR_FLARE = 160964,
                DORMANT = 160641,
                FIXATE = 176544,
            };

            enum class Events : uint32
            {
                UPDATE_POSITION = 1,
            };

            struct mob_SolarFlareAI : public ScriptedAI
            {
                mob_SolarFlareAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events(),
                m_PlayerTargetGuid(0),
                m_IsKilledByPlayers(true)
                {
                    me->setFaction(16);
                    me->SetSpeed(MOVE_WALK, 0.35f);
                    me->SetSpeed(MOVE_RUN, 0.3f);
                    me->SetSpeed(MOVE_FLIGHT, 0.3f);
                    me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 0);
                    me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 0);
                    me->SetReactState(REACT_PASSIVE);
                }

                void SpawnPileOfAshes()
                {
                    // For each pile of ashes in range, we summon a new solar flare and we despawn the pile.
                    m_PlayerTargetGuid = 0;
                    if (m_Instance)
                        m_Instance->SetData64(Data::SolarFlareDying, me->GetGUID());
                }

                void Reset()
                {
                    m_events.Reset();
                    m_IsKilledByPlayers = true;
                    
                    if (Player* l_Plr = ScriptUtils::SelectRandomPlayerIncludedTank(me, 100.0f, false))
                    {
                        m_PlayerTargetGuid = l_Plr->GetGUID();
                        me->AddThreat(l_Plr, 1000000.0f);
                        me->CastSpell(l_Plr, uint32(Spells::FIXATE));
                        events.ScheduleEvent(uint32(Events::UPDATE_POSITION), 500);
                    }
                }

                void JustDied(Unit*)
                {
                    // If creature dies, it spawns a new pile of ashes and it explodes.
                    if (m_IsKilledByPlayers)
                        me->CastSpell(me, uint32(Spells::SUNBURST), true);
                    SpawnPileOfAshes();
                }

                void UpdateAI(const uint32 diff)
                {
                    events.Update(diff);

                    // If creature reaches the target, it spawns a new pile of ashes and explodes violently.
                    if (Player* l_Plr = Player::GetPlayer(*me, m_PlayerTargetGuid))
                    {
                        // It should be less than 2 but I don't know how to make the monsters touch players.
                        if (l_Plr->GetExactDist2d(me) < 0.2f)
                        {
                            m_IsKilledByPlayers = false;
                            me->CastSpell(me, uint32(Spells::SUNSTRIKE), true);
                            SpawnPileOfAshes();
                        }
                    }

                    while (uint32 l_EventId = events.ExecuteEvent())
                    {
                        switch (l_EventId)
                        {
                        case uint32(Events::UPDATE_POSITION):
                            if (Player* l_Plr = Player::GetPlayer(*me, m_PlayerTargetGuid))
                            {
                                Position l_Pos;
                                l_Plr->GetPosition(&l_Pos);
                                me->GetMotionMaster()->MovePoint(0, l_Pos);
                                events.ScheduleEvent(uint32(Events::UPDATE_POSITION), 200);
                            }
                            break;
                        }
                    }
                }

                InstanceScript* m_Instance;
                EventMap m_events;
                uint64 m_PlayerTargetGuid;
                bool m_IsKilledByPlayers;
            };
        };

        class mob_SkyreachRavenWhisperer : public CreatureScript
        {
        public:
            // Entry: 76154
            mob_SkyreachRavenWhisperer()
                : CreatureScript("mob_SkyreachRavenWhisperer")
            {
            }

            enum class Spells : uint32
            {
                BLAZE_OF_GLORY = 153926
            };

            enum class Events : uint32
            {
                BLAZE_OF_GLORY = 1,
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SkyreachRavenWhispererAI(creature);
            }

            struct mob_SkyreachRavenWhispererAI : public ScriptedAI
            {
                mob_SkyreachRavenWhispererAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript())
                {
                }

                void Reset()
                {
                    events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    events.ScheduleEvent(uint32(Events::BLAZE_OF_GLORY), 2000);
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

                    events.Update(diff);

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;

                    switch (events.ExecuteEvent())
                    {
                    case uint32(Events::BLAZE_OF_GLORY):
                        me->CastSpell(me, uint32(Spells::BLAZE_OF_GLORY));
                        events.ScheduleEvent(uint32(Events::BLAZE_OF_GLORY), 10000);
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
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

            enum class Spells : uint32
            {
                PIERCE_ARMOR = 153794,
                SCREECH = 153896,
                SCREECH_DMG = 153898,
                WEAK = 160149,
                SUMMON_SOLAR_FLARE = 153810,
                QUILLS = 159382,
                QUILLS_DMG = 159381,
            };

            enum class Events : uint32
            {
                PIERCE_ARMOR = 1,    // Every 11 to 12 seconds.
                SUMMON_SOLAR_FLARE = 2,         // Every 15 to 16 seconds.
                QUILLS = 3
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
                m_CombatStarted(false),
                m_LastTarget(0)
                {
                    me->SetDisableGravity(true);
                    me->SetCanFly(true);
                    me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                    me->setPowerType(Powers::POWER_ENERGY);
                    me->SetMaxPower(Powers::POWER_ENERGY, 100);
                    me->SetPower(Powers::POWER_ENERGY, 100);

                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                }

                void Reset()
                {
                    events.Reset();

                    m_CombatStarted = false;
                    if (instance && instance->GetBossState(Data::Rukhran) != EncounterState::SPECIAL
                        && instance->GetBossState(Data::Rukhran) != EncounterState::FAIL)
                    {
                        _Reset();

                        m_WaypointId = 0;
                        me->SetReactState(REACT_PASSIVE);
                        me->GetMotionMaster()->MovePoint(m_WaypointId, k_Waypoints[0]);
                        me->SetControlled(false, UNIT_STATE_ROOT);
                    }
                    else if (instance)
                    {
                        me->GetMotionMaster()->MovePoint(12, 918.92f, 1913.46f, 215.87f);
                        instance->SetData(Data::SkyreachRavenWhispererIsDead, 0);
                    }

                    // Cleaning the summons.
                    auto l_Piles = ScriptUtils::SelectNearestCreatureListWithEntry(me, MobEntries::PILE_OF_ASHES, 50.0f);
                    for (auto l_Pile : l_Piles)
                    {
                        if (l_Pile->ToCreature())
                            l_Pile->ToCreature()->DespawnOrUnsummon();
                    }
                    auto l_SolarFlares = ScriptUtils::SelectNearestCreatureListWithEntry(me, MobEntries::SOLAR_FLARE, 50.0f);
                    for (auto l_SolarFlare : l_SolarFlares)
                    {
                        if (l_SolarFlare->ToCreature())
                            l_SolarFlare->ToCreature()->DespawnOrUnsummon();
                    }
                    me->CastStop();
                }

                void MovementInform(uint32 p_Type, uint32 p_Id)
                {
                    switch (p_Type)
                    {
                    case POINT_MOTION_TYPE:
                        if (p_Id == 12)
                        {
                            me->SetControlled(true, UNIT_STATE_ROOT);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->Attack(ScriptUtils::SelectRandomPlayerIncludedTank(me, 40.0f, false), true);
                            m_CombatStarted = true;
                            me->SetOrientation(5.4f);
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

                void JustDied(Unit* p_Killer)
                {
                    _JustDied();

                    // Cleaning the summons.
                    auto l_Piles = ScriptUtils::SelectNearestCreatureListWithEntry(me, MobEntries::PILE_OF_ASHES, 50.0f);
                    for (auto l_Pile : l_Piles)
                    {
                        if (l_Pile->ToCreature())
                            l_Pile->ToCreature()->DespawnOrUnsummon();
                    }
                    auto l_SolarFlares = ScriptUtils::SelectNearestCreatureListWithEntry(me, MobEntries::SOLAR_FLARE, 50.0f);
                    for (auto l_SolarFlare : l_SolarFlares)
                    {
                        if (l_SolarFlare->ToCreature())
                            l_SolarFlare->ToCreature()->DespawnOrUnsummon();
                    }

                    if (!me->GetMap()->IsChallengeMode())
                        p_Killer->SummonGameObject(GameObjectEntries::CACHE_OF_ARAKKOAN_TREASURES, 925.22f, 1904.54f, 213.86f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                void KilledUnit(Unit* /*victim*/)
                {
                }

                void EnterCombat(Unit* /*who*/)
                {
                    _EnterCombat();

                    events.ScheduleEvent(uint32(Events::PIERCE_ARMOR), 6000);
                    events.ScheduleEvent(uint32(Events::SUMMON_SOLAR_FLARE), 10000);

                    if (instance && IsHeroic())
                        events.ScheduleEvent(uint32(Events::QUILLS), 60000);

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }

                void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo)
                {
                    if ((p_SpellInfo->Id == uint32(Spells::SCREECH_DMG) || p_SpellInfo->Id == uint32(Spells::QUILLS_DMG)) && p_Target)
                    {
                        me->AddAura(uint32(Spells::WEAK), p_Target);
                    }
                }

                void UpdateAI(const uint32 diff)
                {
                    if (!UpdateVictim() || (me->getVictim() && me->getVictim()->GetDistance(me) > 50.0f))
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
                    {
                        // Are we casting Screech ?
                        if (Spell* l_Spell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                        {
                            if (l_Spell->GetSpellInfo()->Id == uint32(Spells::SCREECH))
                            {
                                // Should we stop ?
                                Player* l_Plr = ScriptUtils::SelectNearestPlayer(me, 15.0f);
                                if (l_Plr && l_Plr->IsWithinMeleeRange(me))
                                    me->CastStop();
                            }
                        }
                        return;
                    }

                    // We check if someone is in melee range.
                    if (me->getVictim() && !me->IsWithinMeleeRange(me->getVictim()))
                    {
                        Player* l_Plr = ScriptUtils::SelectNearestPlayer(me, 15.0f);
                        if (!l_Plr || !l_Plr->IsWithinMeleeRange(me))
                        {
                            // We can't, so we cast Screech and Weak.
                            if (!me->HasAura(uint32(Spells::SCREECH)))
                                me->CastSpell(me, uint32(Spells::SCREECH));
                        }
                    }

                    switch (events.ExecuteEvent())
                    {
                    case uint32(Events::SUMMON_SOLAR_FLARE):
                        events.ScheduleEvent(uint32(Events::SUMMON_SOLAR_FLARE), urand(14000, 16000));

                        if (ScriptUtils::SelectRandomPlayerIncludedTank(me, 30.0f))
                        {
                            m_LastTarget = urand(0, 10) + 1;
                            Position l_Pos = k_RandomSummonSolarFlare[m_LastTarget - 1];
                            me->CastSpell(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, uint32(Spells::SUMMON_SOLAR_FLARE), false);
                        }
                        break;
                    case uint32(Events::PIERCE_ARMOR):
                        events.ScheduleEvent(uint32(Events::PIERCE_ARMOR), urand(10500, 13000));
                        // We want to cast PierceArmor on the closest ennemy.
                        if (me->getVictim() && me->getVictim()->IsWithinMeleeRange(me))
                            me->CastSpell(me->getVictim(), uint32(Spells::PIERCE_ARMOR));
                        else if (ScriptUtils::SelectNearestPlayer(me, 15.0f))
                            me->CastSpell(me->getVictim(), uint32(Spells::PIERCE_ARMOR));
                        break;
                    case uint32(Events::QUILLS):
                        events.ScheduleEvent(uint32(Events::QUILLS), 60000);
                        me->CastSpell(me, uint32(Spells::QUILLS));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                uint32 m_WaypointId;
                bool m_CombatStarted;
                uint32 m_LastTarget;
            };
        };
    }
}

#ifndef __clang_analyzer__
void AddSC_boss_Rukhran()
{
    new MS::InstanceSkyreach::boss_Rukhran();
    new MS::InstanceSkyreach::mob_SkyreachRavenWhisperer();
    new MS::InstanceSkyreach::mob_SolarFlare();
}
#endif
