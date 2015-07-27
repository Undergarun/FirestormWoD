////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "instance_bloodmaul.hpp"

namespace MS { namespace Instances { namespace Bloodmaul
{
    /// Slagna - 75406
    class mob_Slagna : public CreatureScript
    {
        public:
            mob_Slagna() : CreatureScript("mob_Slagna") { }

            enum class Spells : uint32
            {
                SubmergeVisual  = 139832,
                LavaSpit        = 152183
            };

            enum class Events : uint32
            {
                LavaSpit = 1
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature)
                {
                    me->AddAura(uint32(Spells::SubmergeVisual), me);
                }

                void Reset()
                {
                    events.Reset();

                    me->AddUnitState(UNIT_STATE_ROOT);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_UNK_6 | eUnitFlags::UNIT_FLAG_UNK_15 | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                }

                void SetData(uint32 p_Type, uint32)
                {
                    if (p_Type == uint32(Data::SpawnSlagna))
                    {
                        me->RemoveAura(uint32(Spells::SubmergeVisual));
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_UNK_6 | eUnitFlags::UNIT_FLAG_UNK_15 | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                        if (Unit* l_Target = me->SelectNearestPlayerNotGM(20.0f))
                            AttackStart(l_Target);
                    }
                }

                void EnterCombat(Unit*)
                {
                    me->RemoveAura(uint32(Spells::SubmergeVisual));
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_UNK_6 | eUnitFlags::UNIT_FLAG_UNK_15 | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                    events.ScheduleEvent(uint32(Events::LavaSpit), urand(2000, 2500));
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

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

    /// Captured Miner - 74355
    class mob_CapturedMiner : public CreatureScript
    {
        public:
            mob_CapturedMiner() : CreatureScript("mob_CapturedMiner") { }

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            enum eEvents
            {
                EventTraumaticStrike = 1
            };

            enum eSpells
            {
                SpellTraumaticStrike = 151092
            };

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature)
                {
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_FLAGS, 0);
                }

                EventMap m_Events;

                void EnterCombat(Unit* p_Target)
                {
                    me->getThreatManager().addThreat(p_Target, 1000.0f);

                    m_Events.Reset();

                    m_Events.ScheduleEvent(eEvents::EventTraumaticStrike, 4000);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_Events.Update(p_Diff);

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventTraumaticStrike:
                            DoCastVictim(eSpells::SpellTraumaticStrike);
                            break;
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };
    };

    /// Bloodmaul Ogre Mage - 75272
    class mob_BloodmaulOgreMage : public CreatureScript
    {
        public:
            mob_BloodmaulOgreMage() : CreatureScript("mob_BloodmaulOgreMage") { }

            enum class Spells : uint32
            {
                LavaBurst = 151558,
                BloodRage = 151548
            };

            enum class Events : uint32
            {
                LavaBurst = 1,
                BloodRage
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    events.ScheduleEvent(uint32(Events::LavaBurst), urand(500, 1500));
                    events.ScheduleEvent(uint32(Events::BloodRage), urand(10000, 12000));
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

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

    /// Bloodmaul Warder - 75210
    class mob_BloodmaulWarder : public CreatureScript
    {
        public:
            mob_BloodmaulWarder() : CreatureScript("mob_BloodmaulWarder") { }

            enum class Spells : uint32
            {
                Cleave          = 40505,
                FrighteningRoar = 151545
            };

            enum class Events : uint32
            {
                Cleave = 1,
                FrighteningRoar
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    events.ScheduleEvent(uint32(Events::Cleave), urand(500, 1500));
                    events.ScheduleEvent(uint32(Events::FrighteningRoar), urand(14000, 16000));
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

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

    /// Bloodmaul Geomancer - 75198
    class mob_BloodmaulGeomancer : public CreatureScript
    {
        public:
            mob_BloodmaulGeomancer() : CreatureScript("mob_BloodmaulGeomancer") { }

            enum class Spells : uint32
            {
                StoneBolt       = 164592,
                StoneBulwark    = 164597
            };

            enum class Events : uint32
            {
                StoneBolt = 1,
                StoneBulwark
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    events.ScheduleEvent(uint32(Events::StoneBolt), urand(500, 1500));
                    events.ScheduleEvent(uint32(Events::StoneBulwark), urand(6000, 7000));
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

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
                            break;
                        }
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };
    };

    /// Bloodmaul Overseer - 75426
    class mob_BloodmaulOverseer : public CreatureScript
    {
        public:
            mob_BloodmaulOverseer() : CreatureScript("mob_BloodmaulOverseer") { }

            enum class Spells : uint32
            {
                SuppresionField = 151581,
                Subjugate       = 151697
            };

            enum class Events : uint32
            {
                SuppresionField = 1,
                Subjugate
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    events.ScheduleEvent(uint32(Events::SuppresionField), urand(500, 1500));
                    events.ScheduleEvent(uint32(Events::Subjugate), urand(15000, 17000));
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

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

    /// Bloodmaul Slaver - 75191
    class mob_BloodmaulSlaver : public CreatureScript
    {
        public:
            mob_BloodmaulSlaver() : CreatureScript("mob_BloodmaulSlaver") { }

            enum class Spells : uint32
            {
                ChainGrip   = 151990,
                SlaversRage = 151965,
                ShockBola   = 152073
            };

            enum class Events : uint32
            {
                ChainGrip = 1,
                SlaversRage,
                ShockBola
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    events.ScheduleEvent(uint32(Events::ChainGrip), urand(500, 1500));
                    events.ScheduleEvent(uint32(Events::ShockBola), urand(15000, 17000));
                    events.ScheduleEvent(uint32(Events::SlaversRage), 6000);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

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

    /// Bloodmaul Enforcer - 84978
    class mob_BloodmaulEnforcer : public CreatureScript
    {
        public:
            mob_BloodmaulEnforcer() : CreatureScript("mob_BloodmaulEnforcer") { }

            enum class Spells : uint32
            {
                Beatdown            = 151415,
                Crush               = 151447,
                LumberingLeap       = 151542,
                LumberingLeapDmg    = 151541
            };

            enum class Events : uint32
            {
                Beatdown = 1,
                Crush,
                LumberingLeap
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_AI(p_Creature);
            }

            struct mob_AI : public ScriptedAI
            {
                mob_AI(Creature* p_Creature) : ScriptedAI(p_Creature), m_TargetGUID(0) { }

                void Reset()
                {
                    events.Reset();

                    m_TargetGUID = 0;
                }

                void EnterCombat(Unit*)
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

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

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

    /// Bloodmaul Ogron - 81750
    class mob_bloodmaul_ogron : public CreatureScript
    {
        public:
            mob_bloodmaul_ogron() : CreatureScript("mob_bloodmaul_ogron") { }

            struct mob_bloodmaul_ogronAI : public ScriptedAI
            {
                mob_bloodmaul_ogronAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum eSpells
                {
                    SpellColossalRoar = 164582,
                    SpellMassiveStomp = 164587
                };

                enum eEvents
                {
                    EventColossalRoar = 1,
                    EventMassiveStomp
                };

                EventMap m_Events;

                void Reset()
                {
                    m_Events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    m_Events.ScheduleEvent(eEvents::EventColossalRoar, 5000);
                    m_Events.ScheduleEvent(eEvents::EventMassiveStomp, 9000);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_Events.Update(p_Diff);

                    if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                        return;

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventColossalRoar:
                            me->CastSpell(me, eSpells::SpellColossalRoar, true);
                            m_Events.ScheduleEvent(eEvents::EventColossalRoar, 10000);
                            break;
                        case eEvents::EventMassiveStomp:
                            me->CastSpell(me, eSpells::SpellMassiveStomp, true);
                            m_Events.ScheduleEvent(eEvents::EventMassiveStomp, 15000);
                            break;
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_bloodmaul_ogronAI(p_Creature);
            }
    };

    /// Bloodmaul Flamespeaker - 81767
    class mob_bloodmaul_flamespeaker : public CreatureScript
    {
        public:
            mob_bloodmaul_flamespeaker() : CreatureScript("mob_bloodmaul_flamespeaker") { }

            struct mob_bloodmaul_flamespeakerAI : public ScriptedAI
            {
                mob_bloodmaul_flamespeakerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum eSpells
                {
                    SpellChannelFlames      = 164615,
                    SpellExplodingFlames    = 164617
                };

                enum eEvents
                {
                    EventChannelFlames = 1,
                    EventExplodingFlames
                };

                EventMap m_Events;

                void Reset()
                {
                    m_Events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    m_Events.ScheduleEvent(eEvents::EventChannelFlames, 5000);
                    m_Events.ScheduleEvent(eEvents::EventExplodingFlames, 10000);
                }

                void UpdateAI(uint32 const p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_Events.Update(p_Diff);

                    if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                        return;

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventChannelFlames:
                            me->SetFacingTo(me->GetOrientation());
                            me->CastSpell(me, eSpells::SpellChannelFlames, false);
                            m_Events.ScheduleEvent(eEvents::EventChannelFlames, 30000);
                            break;
                        case eEvents::EventExplodingFlames:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                                me->CastSpell(l_Target, eSpells::SpellExplodingFlames, false);
                            m_Events.ScheduleEvent(eEvents::EventExplodingFlames, 20000);
                            break;
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_bloodmaul_flamespeakerAI(p_Creature);
            }
    };

    /// Exploding Flames - 83808
    class mob_bloodmaul_exploding_flames : public CreatureScript
    {
        public:
            mob_bloodmaul_exploding_flames() : CreatureScript("mob_bloodmaul_exploding_flames") { }

            struct mob_bloodmaul_exploding_flamesAI : public ScriptedAI
            {
                mob_bloodmaul_exploding_flamesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum eSpells
                {
                    ExplodingFlamesPeriodic = 167701
                };

                void Reset()
                {
                    me->CastSpell(me, eSpells::ExplodingFlamesPeriodic, true);

                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                }

                void UpdateAI(uint32 const p_Diff) { }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_bloodmaul_exploding_flamesAI(p_Creature);
            }
    };

    /// Molten Earth Elemental - 75209
    class mob_molten_earth_elemental : public CreatureScript
    {
        public:
            mob_molten_earth_elemental() : CreatureScript("mob_molten_earth_elemental") { }

            struct mob_molten_earth_elementalAI : public ScriptedAI
            {
                mob_molten_earth_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum Spells
                {
                    SpellLavaArc = 151720,
                };

                enum Events
                {
                    EventLavaArc = 1,
                };

                EventMap m_Events;

                void Reset()
                {
                    m_Events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    m_Events.ScheduleEvent(Events::EventLavaArc, 3000);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_Events.Update(p_Diff);

                    if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                        return;

                    switch ((Events)m_Events.ExecuteEvent())
                    {
                        case Events::EventLavaArc:
                        {
                            if (Map* l_Map = me->GetMap())
                                if (!l_Map->IsHeroic() && !l_Map->IsChallengeMode())
                                    break;

                            me->CastSpell(me, Spells::SpellLavaArc, false);
                            m_Events.ScheduleEvent(Events::EventLavaArc, 12000);
                            break;
                        }
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_molten_earth_elementalAI(p_Creature);
            }
    };

    /// Vengeful Magma Elemental - 75820
    class mob_vengeful_magma_elemental : public CreatureScript
    {
        public:
            mob_vengeful_magma_elemental() : CreatureScript("mob_vengeful_magma_elemental") { }

            struct mob_vengeful_magma_elementalAI : public ScriptedAI
            {
                mob_vengeful_magma_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum Spells
                {
                    SpellArmorDent = 151685,
                    SpellCinderSplash = 152298,
                };

                enum Events
                {
                    EventArmorDent = 1,
                    EventCinderSplash = 2
                };

                EventMap m_Events;

                void Reset()
                {
                    m_Events.Reset();

                    if (Map* l_Map = me->GetMap())
                        m_IsHC = l_Map->IsHeroic() || l_Map->IsChallengeMode();
                    else
                        m_IsHC = false;
                }

                void EnterCombat(Unit*)
                {
                    m_Events.ScheduleEvent(Events::EventArmorDent, 3000);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_Events.Update(p_Diff);

                    if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                        return;

                    switch ((Events)m_Events.ExecuteEvent())
                    {
                        case Events::EventArmorDent:
                            me->CastSpell(me->getVictim(), Spells::SpellArmorDent, false);
                            m_Events.ScheduleEvent(m_IsHC ? urand(Events::EventArmorDent, Events::EventCinderSplash) : Events::EventArmorDent, 6000);
                            break;
                        case Events::EventCinderSplash:
                            me->CastSpell(me->getVictim(), Spells::SpellCinderSplash, false);
                            m_Events.ScheduleEvent(urand(Events::EventArmorDent, Events::EventCinderSplash), 6000);
                            break;
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }

                bool m_IsHC;
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_vengeful_magma_elementalAI(p_Creature);
            }
    };

    /// Magma Lord - 75211
    class mob_magma_lord : public CreatureScript
    {
        public:
            mob_magma_lord() : CreatureScript("mob_magma_lord") { }

            struct mob_magma_lordAI : public ScriptedAI
            {
                mob_magma_lordAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum Spells
                {
                    SpellFireball       = 152427,
                    SpellPillarOfFlames = 151623
                };

                enum Events
                {
                    EvenFireball = 1,
                };

                EventMap m_Events;

                void Reset()
                {
                    m_Events.Reset();
                }

                void EnterCombat(Unit*)
                {
                    m_Events.ScheduleEvent(Events::EvenFireball, 3000);
                    me->CastSpell(me, Spells::SpellPillarOfFlames, true);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_Events.Update(p_Diff);

                    if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                        return;

                    switch ((Events)m_Events.ExecuteEvent())
                    {
                        case Events::EvenFireball:
                        {
                            if (Map* l_Map = me->GetMap())
                                if (!l_Map->IsHeroic() && !l_Map->IsChallengeMode())
                                    break;

                            me->CastSpell(me->getVictim(), Spells::SpellFireball, false);
                            m_Events.ScheduleEvent(Events::EvenFireball, 8000);
                            break;
                        }
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_magma_lordAI(p_Creature);
            }
    };

    /// Pillar Of Flame - 75327
    class mob_pillar_of_flame : public CreatureScript
    {
        public:
            mob_pillar_of_flame() : CreatureScript("mob_pillar_of_flame") { }

            struct mob_pillar_of_flameAI : public ScriptedAI
            {
                mob_pillar_of_flameAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum Spells
                {
                    SpellATSummon   = 145844,
                    SpellDebuff     = 145843
                };

                enum Events
                {
                    EventDamage = 1,
                };

                EventMap m_Events;

                void Reset()
                {
                    m_Events.Reset();
                    m_CanDamage = false;
                    me->SetControlled(true, UNIT_STATE_ROOT);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NON_ATTACKABLE);
                    m_Events.ScheduleEvent(Events::EventDamage, 3000);
                    me->CastSpell(me, Spells::SpellATSummon, false);
                    me->DespawnOrUnsummon(6000);
                }

                void MoveInLineOfSight(Unit* p_Who)
                {
                    if (p_Who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDist2d(p_Who, 5.f) && !p_Who->HasAura(Spells::SpellDebuff))
                        me->CastSpell(p_Who, Spells::SpellDebuff, true);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_Events.Update(p_Diff);

                    if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                        return;

                    switch ((Events)m_Events.ExecuteEvent())
                    {
                        case Events::EventDamage:
                        {
                            if (Map* l_Map = me->GetMap())
                                if (!l_Map->IsHeroic() && !l_Map->IsChallengeMode())
                                    break;

                            m_CanDamage = true;
                            break;
                        }
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }

                bool m_CanDamage;
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new mob_pillar_of_flameAI(p_Creature);
            }
    };

    /// Channel Flames - 164615
    class spell_bloodmaul_channel_flames : public SpellScriptLoader
    {
        public:
            spell_bloodmaul_channel_flames() : SpellScriptLoader("spell_bloodmaul_channel_flames") { }

            class spell_bloodmaul_channel_flames_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_bloodmaul_channel_flames_AuraScript);

                enum eSpells
                {
                    ChannelFlamesDoT = 164616
                };

                void OnUpdate(uint32)
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        std::list<Unit*> l_TargetList;
                        float l_Radius = 15.0f;

                        JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(l_Caster, l_Caster, l_Radius);
                        JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(l_Caster, l_TargetList, u_check);
                        l_Caster->VisitNearbyObject(l_Radius, searcher);

                        if (l_TargetList.empty())
                            return;

                        l_TargetList.remove_if([this, l_Caster](Unit* p_Unit) -> bool
                        {
                            if (p_Unit == nullptr || !l_Caster->isInFront(p_Unit, M_PI / 6))
                                return true;

                            return false;
                        });

                        for (Unit* l_Unit : l_TargetList)
                            l_Caster->CastSpell(l_Unit, eSpells::ChannelFlamesDoT, true);
                    }
                }

                void Register()
                {
                    OnAuraUpdate += AuraUpdateFn(spell_bloodmaul_channel_flames_AuraScript::OnUpdate);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_bloodmaul_channel_flames_AuraScript();
            }
    };
}
} } ///< namespace MS::Instances

void AddSC_mob_Bloodmaul()
{
    /// Creatures
    new MS::Instances::Bloodmaul::mob_BloodmaulEnforcer();
    new MS::Instances::Bloodmaul::mob_BloodmaulSlaver();
    new MS::Instances::Bloodmaul::mob_BloodmaulOverseer();
    new MS::Instances::Bloodmaul::mob_BloodmaulGeomancer();
    new MS::Instances::Bloodmaul::mob_BloodmaulWarder();
    new MS::Instances::Bloodmaul::mob_BloodmaulOgreMage();
    new MS::Instances::Bloodmaul::mob_CapturedMiner();
    new MS::Instances::Bloodmaul::mob_Slagna();
    new MS::Instances::Bloodmaul::mob_bloodmaul_ogron();
    new MS::Instances::Bloodmaul::mob_bloodmaul_flamespeaker();
    new MS::Instances::Bloodmaul::mob_bloodmaul_exploding_flames();
    new MS::Instances::Bloodmaul::mob_molten_earth_elemental();
    new MS::Instances::Bloodmaul::mob_vengeful_magma_elemental();
    new MS::Instances::Bloodmaul::mob_magma_lord();
    new MS::Instances::Bloodmaul::mob_pillar_of_flame();

    /// Spells
    new MS::Instances::Bloodmaul::spell_bloodmaul_channel_flames();
}