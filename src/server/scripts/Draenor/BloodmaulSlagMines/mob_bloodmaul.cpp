#include "instance_bloodmaul.h"

namespace MS
{
    namespace Instances
    {
        namespace Bloodmaul
        {
            // Entry: 75211
            class mob_MagmaLord : public CreatureScript
            {
            public:
                mob_MagmaLord()
                    : CreatureScript("mob_MagmaLord")
                {
                }

                enum class Spells : uint32
                {
                    LavaSplash = 152809,
                    PillarOfFlames = 151623,
                    Fireball = 152427,
                };

                enum class Events : uint32
                {
                    LavaSplash = 1,
                    PillarOfFlames = 2,
                    Fireball = 3,
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature),
                    m_PillarGuids(),
                    m_Events()
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                        m_Events.Reset();
                        m_Events.ScheduleEvent(uint32(Events::LavaSplash), 11000);
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::PillarOfFlames), urand(2500, 3500));
                        events.ScheduleEvent(uint32(Events::Fireball), urand(2000, 4000));
                    }

                    void JustDied(Unit*)
                    {
                        for (uint64 l_Guid : m_PillarGuids)
                        {
                            if (Creature* l_Creature = sObjectAccessor->FindCreature(l_Guid))
                                l_Creature->DespawnOrUnsummon();
                        }
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        m_Events.Update(diff);
                        if (m_Events.ExecuteEvent() == uint32(Events::LavaSplash))
                        {
                            if (Unit* l_Unit = ScriptUtils::SelectRandomCreatureWithEntry(me, uint32(MobEntries::LavaExplosionStalker), 80.0f))
                                me->CastSpell(l_Unit, uint32(Spells::LavaSplash));
                            m_Events.ScheduleEvent(uint32(Events::LavaSplash), 11000);
                        }

                        if (!UpdateVictim())
                            return;

                        events.Update(diff);

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                            return;

                        switch (events.ExecuteEvent())
                        {
                        case uint32(Events::PillarOfFlames):
                            me->CastSpell(me, uint32(Spells::PillarOfFlames));
                            if (TempSummon* l_Pillar = me->SummonCreature(uint32(MobEntries::FirePillar), *me, TEMPSUMMON_TIMED_DESPAWN, 60000))
                            {
                                m_PillarGuids.emplace_back(l_Pillar->GetGUID());
                                if (Player* l_Plr = ScriptUtils::SelectRandomPlayerIncludedTank(me, 20.0f))
                                    l_Pillar->GetMotionMaster()->MoveChase(l_Plr);
                                else
                                    l_Pillar->DespawnOrUnsummon();
                            }
                            events.ScheduleEvent(uint32(Events::PillarOfFlames), urand(2500, 3500));
                            break;
                        case uint32(Events::Fireball):
                            me->CastSpell(me->getVictim(), uint32(Spells::Fireball));
                            events.ScheduleEvent(uint32(Events::Fireball), urand(2000, 4000));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }

                    std::list<uint64> m_PillarGuids;
                    EventMap m_Events;
                };
            };

            // Entry: 75209
            class mob_MoltenEarthElemental : public CreatureScript
            {
            public:
                mob_MoltenEarthElemental()
                    : CreatureScript("mob_MoltenEarthElemental")
                {
                }

                enum class Spells : uint32
                {
                    LavaArc = 151720,
                    VolcanicEruption = 151698,
                };

                enum class Events : uint32
                {
                    LavaArc = 1,
                    VolcanicEruption = 2,
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::LavaArc), urand(2500, 3500));
                        events.ScheduleEvent(uint32(Events::VolcanicEruption), urand(7500, 8500));
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        events.Update(diff);

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                            return;

                        switch (events.ExecuteEvent())
                        {
                        case uint32(Events::LavaArc):
                            me->CastSpell(me->getVictim(), uint32(Spells::LavaArc));
                            events.ScheduleEvent(uint32(Events::LavaArc), urand(2500, 3500));
                            break;
                        case uint32(Events::VolcanicEruption):
                            me->CastSpell(me, uint32(Spells::VolcanicEruption));
                            events.ScheduleEvent(uint32(Events::VolcanicEruption), urand(7500, 8500));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 75406
            class mob_Slagna : public CreatureScript
            {
            public:
                mob_Slagna()
                    : CreatureScript("mob_Slagna")
                {
                }

                enum class Spells : uint32
                {
                    SubmergeVisual = 139832,
                    LavaSpit = 152183,

                };

                enum class Events : uint32
                {
                    LavaSpit = 1,
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                        me->AddAura(uint32(Spells::SubmergeVisual), me);
                    }

                    void Reset()
                    {
                        events.Reset();
                        me->AddUnitState(UNIT_STATE_ROOT);
                    }

                    void SetData(uint32 p_Type, uint32)
                    {
                        if (p_Type == uint32(Data::SpawnSlagna))
                            me->RemoveAura(uint32(Spells::SubmergeVisual));
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::LavaSpit), urand(2000, 2500));
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        events.Update(diff);

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                            return;

                        switch (events.ExecuteEvent())
                        {
                        case uint32(Events::LavaSpit):
                            if (Player* l_Plr = ScriptUtils::SelectRandomPlayerIncludedTank(me, 30.0f))
                                me->CastSpell(l_Plr, uint32(Spells::LavaSpit));
                            events.ScheduleEvent(uint32(Events::LavaSpit), urand(2000, 2500));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 74355
            class mob_CapturedMiner : public CreatureScript
            {
            public:
                mob_CapturedMiner()
                    : CreatureScript("mob_CapturedMiner")
                {
                }

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                        me->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
                    }

                    void EnterCombat(Unit* p_Target)
                    {
                        me->getThreatManager().addThreat(p_Target, 1000.0f);
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 75272
            class mob_BloodmaulOgreMage : public CreatureScript
            {
            public:
                mob_BloodmaulOgreMage()
                    : CreatureScript("mob_BloodmaulOgreMage")
                {
                }

                enum class Spells : uint32
                {
                    LavaBurst = 151558,
                    BloodRage = 151548,

                };

                enum class Events : uint32
                {
                    LavaBurst = 1,
                    BloodRage = 2,
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::LavaBurst), urand(500, 1500));
                        events.ScheduleEvent(uint32(Events::BloodRage), urand(10000, 12000));
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        events.Update(diff);

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                            return;

                        switch (events.ExecuteEvent())
                        {
                        case uint32(Events::LavaBurst):
                            me->CastSpell(me->getVictim(), uint32(Spells::LavaBurst));
                            events.ScheduleEvent(uint32(Events::LavaBurst), urand(2500, 3500));
                            break;
                        case uint32(Events::BloodRage):
                            me->CastSpell(me, uint32(Spells::BloodRage));
                            events.ScheduleEvent(uint32(Events::BloodRage), urand(10000, 12000));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 75210
            class mob_BloodmaulWarder : public CreatureScript
            {
            public:
                mob_BloodmaulWarder()
                    : CreatureScript("mob_BloodmaulWarder")
                {
                }

                enum class Spells : uint32
                {
                    Cleave = 40505,
                    FrighteningRoar = 151545,

                };

                enum class Events : uint32
                {
                    Cleave = 1,
                    FrighteningRoar = 2,
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::Cleave), urand(500, 1500));
                        events.ScheduleEvent(uint32(Events::FrighteningRoar), urand(14000, 16000));
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        events.Update(diff);

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                            return;

                        switch (events.ExecuteEvent())
                        {
                        case uint32(Events::Cleave):
                            me->CastSpell(me->getVictim(), uint32(Spells::Cleave));
                            events.ScheduleEvent(uint32(Events::Cleave), urand(7500, 9500));
                            break;
                        case uint32(Events::FrighteningRoar):
                            me->CastSpell(me, uint32(Spells::FrighteningRoar));
                            events.ScheduleEvent(uint32(Events::FrighteningRoar), urand(26000, 29000));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 75198
            class mob_BloodmaulGeomancer : public CreatureScript
            {
            public:
                mob_BloodmaulGeomancer()
                    : CreatureScript("mob_BloodmaulGeomancer")
                {
                }

                enum class Spells : uint32
                {
                    StoneBolt = 164592,
                    StoneBulwark = 164597,

                };

                enum class Events : uint32
                {
                    StoneBolt = 1,
                    StoneBulwark = 2,
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::StoneBolt), urand(500, 1500));
                        events.ScheduleEvent(uint32(Events::StoneBulwark), urand(6000, 7000));
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        events.Update(diff);

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                            return;

                        switch (events.ExecuteEvent())
                        {
                        case uint32(Events::StoneBolt):
                            me->CastSpell(me->getVictim(), uint32(Spells::StoneBolt));
                            events.ScheduleEvent(uint32(Events::StoneBolt), urand(4500, 5500));
                            break;
                        case uint32(Events::StoneBulwark):
                        {
                            Unit* l_Target = me;
                            if (Unit* l_Unit = ScriptUtils::SelectNearestFriendExcluededMe(me, 40.0f))
                            {
                                if (l_Unit->isInCombat())
                                    l_Target = l_Unit;
                            }
                            
                            me->CastSpell(l_Target, uint32(Spells::StoneBulwark));

                            events.ScheduleEvent(uint32(Events::StoneBulwark), urand(3000, 35000));
                        } break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 75426
            class mob_BloodmaulOverseer : public CreatureScript
            {
            public:
                mob_BloodmaulOverseer()
                    : CreatureScript("mob_BloodmaulOverseer")
                {
                }

                enum class Spells : uint32
                {
                    SuppresionField = 151581,
                    Subjugate = 151697,
                };

                enum class Events : uint32
                {
                    SuppresionField = 1,
                    Subjugate = 2,
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::SuppresionField), urand(500, 1500));
                        events.ScheduleEvent(uint32(Events::Subjugate), urand(15000, 17000));
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        events.Update(diff);

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL))
                            return;

                        switch (events.ExecuteEvent())
                        {
                        case uint32(Events::SuppresionField):
                            if (Player* l_Plr = ScriptUtils::SelectRandomPlayerIncludedTank(me, 40.0f))
                                me->CastSpell(l_Plr, uint32(Spells::SuppresionField));
                            events.ScheduleEvent(uint32(Events::SuppresionField), urand(16000, 18000));
                            break;
                        case uint32(Events::Subjugate):
                            if (ScriptUtils::SelectRandomPlayerIncludedTank(me, 40.0f))
                                me->CastSpell(me->getVictim(), uint32(Spells::Subjugate));
                            events.ScheduleEvent(uint32(Events::Subjugate), urand(21000, 24000));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 75191
            class mob_BloodmaulSlaver : public CreatureScript
            {
            public:
                mob_BloodmaulSlaver()
                    : CreatureScript("mob_BloodmaulSlaver")
                {
                }

                enum class Spells : uint32
                {
                    ChainGrip = 151990,
                    SlaversRage = 151965,
                    ShockBola = 152073,
                };

                enum class Events : uint32
                {
                    ChainGrip = 1,
                    SlaversRage = 2,
                    ShockBola = 3
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::ChainGrip), urand(500, 1500));
                        events.ScheduleEvent(uint32(Events::ShockBola), urand(15000, 17000));
                        events.ScheduleEvent(uint32(Events::SlaversRage), 6000);
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
                        case uint32(Events::ChainGrip):
                            if (Player* l_Plr = ScriptUtils::SelectFarEnoughPlayerIncludedTank(me, 5.0f))
                                me->CastSpell(l_Plr, uint32(Spells::ChainGrip));
                            events.ScheduleEvent(uint32(Events::ChainGrip), urand(16000, 18000));
                            break;
                        case uint32(Events::SlaversRage):
                            me->CastSpell(me->getVictim(), uint32(Spells::SlaversRage));
                            events.ScheduleEvent(uint32(Events::SlaversRage), urand(21000, 24000));
                            break;
                        case uint32(Events::ShockBola):
                            if (Player* l_Plr = ScriptUtils::SelectRandomPlayerIncludedTank(me, 15.0f))
                                me->CastSpell(l_Plr, uint32(Spells::ShockBola));
                            events.ScheduleEvent(uint32(Events::ShockBola), urand(15000, 17000));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };

            // Entry: 84978
            class mob_BloodmaulEnforcer : public CreatureScript
            {
            public:
                mob_BloodmaulEnforcer()
                    : CreatureScript("mob_BloodmaulEnforcer")
                {
                }

                enum class Spells : uint32
                {
                    Beatdown = 151415,
                    Crush = 151447,
                    LumberingLeap = 151542,
                    LumberingLeapDmg = 151541,
                };

                enum class Events : uint32
                {
                    Beatdown = 1,
                    Crush = 2,
                    LumberingLeap = 3
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new mob_AI(creature);
                }

                struct mob_AI : public ScriptedAI
                {
                    mob_AI(Creature* creature)
                    : ScriptedAI(creature),
                    m_TargetGUID(0)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();

                        m_TargetGUID = 0;
                    }

                    void EnterCombat(Unit* p_Who)
                    {
                        events.ScheduleEvent(uint32(Events::Beatdown), urand(500, 1500));
                        events.ScheduleEvent(uint32(Events::Crush), urand(4500, 5500));
                        events.ScheduleEvent(uint32(Events::LumberingLeap), urand(15000, 17000));
                    }

                    void MovementInform(uint32 p_Type, uint32 p_Id)
                    {
                        switch (p_Type)
                        {
                        case EFFECT_MOTION_TYPE:
                            if (Player* l_Plr = sObjectAccessor->FindPlayer(m_TargetGUID))
                            {
                                me->CastSpell(l_Plr, uint32(Spells::LumberingLeapDmg));
                                m_TargetGUID = 0;
                                ScriptUtils::ApplyOnEveryPlayer(me, [](Unit* p_Me, Player* p_Plr) {
                                    if (p_Me->GetExactDist2d(p_Plr) <= 5.0f)
                                        p_Plr->GetMotionMaster()->MoveKnockbackFrom(p_Me->GetPositionX(), p_Me->GetPositionY(), 2.0f, 2.0f);
                                });
                            }
                            break;
                        }
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
                        case uint32(Events::Beatdown):
                            me->CastSpell(me->getVictim(), uint32(Spells::Beatdown));
                            events.ScheduleEvent(uint32(Events::Beatdown), urand(22000, 26000));
                            break;
                        case uint32(Events::Crush):
                            me->CastSpell(me->getVictim(), uint32(Spells::Crush));
                            events.ScheduleEvent(uint32(Events::Crush), urand(10000, 11000));
                            break;
                        case uint32(Events::LumberingLeap):
                            if (Player* l_Plr = ScriptUtils::SelectFarEnoughPlayerIncludedTank(me, 8.0f))
                            {
                                m_TargetGUID = l_Plr->GetGUID();
                                me->CastSpell(l_Plr, uint32(Spells::LumberingLeap));
                            }
                            events.ScheduleEvent(uint32(Events::LumberingLeap), urand(15000, 17000));
                            break;
                        default:
                            break;
                        }

                        DoMeleeAttackIfReady();
                    }

                    uint64 m_TargetGUID;
                };
            };
        }
    }
}

void AddSC_mob_Bloodmaul()
{
    new MS::Instances::Bloodmaul::mob_BloodmaulEnforcer();
    new MS::Instances::Bloodmaul::mob_BloodmaulSlaver();
    new MS::Instances::Bloodmaul::mob_BloodmaulOverseer();
    new MS::Instances::Bloodmaul::mob_BloodmaulGeomancer();
    new MS::Instances::Bloodmaul::mob_BloodmaulWarder();
    new MS::Instances::Bloodmaul::mob_BloodmaulOgreMage();
    new MS::Instances::Bloodmaul::mob_CapturedMiner();
    new MS::Instances::Bloodmaul::mob_Slagna();
    new MS::Instances::Bloodmaul::mob_MoltenEarthElemental();
    new MS::Instances::Bloodmaul::mob_MagmaLord();
}