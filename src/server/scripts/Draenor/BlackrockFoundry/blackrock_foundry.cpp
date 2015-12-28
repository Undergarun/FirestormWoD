////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Iron Flame Binder - 87515
class npc_foundry_iron_flame_binder : public CreatureScript
{
    public:
        npc_foundry_iron_flame_binder() : CreatureScript("npc_foundry_iron_flame_binder") { }

        enum eSpells
        {
            FireChannelCosmetic = 174883,

            Cauterize           = 175620,
            Lavaburst           = 175621,
            UnbindFlame         = 175609
        };

        enum eEvents
        {
            EventCauterize = 1,
            EventLavaburst,
            EventUnbindFlame
        };

        enum eCreature
        {
            FoundryArtifact = 87516
        };

        struct npc_foundry_iron_flame_binderAI : public ScriptedAI
        {
            npc_foundry_iron_flame_binderAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(urand(50, 350), [this]() -> void
                {
                    std::list<Creature*> l_Artifacts;
                    me->GetCreatureListWithEntryInGrid(l_Artifacts, eCreature::FoundryArtifact, 30.0f);

                    for (Creature* l_Artifact : l_Artifacts)
                    {
                        if (l_Artifact->HasAura(eSpells::FireChannelCosmetic))
                            continue;

                        me->CastSpell(l_Artifact, eSpells::FireChannelCosmetic, true);
                    }
                });
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventCauterize, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventLavaburst, 2 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventUnbindFlame, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCauterize:
                    {
                        if (Unit* l_Ally = me->SelectNearbyMostInjuredAlly())
                            me->CastSpell(l_Ally, eSpells::Cauterize, false);

                        m_Events.ScheduleEvent(eEvents::EventCauterize, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventLavaburst:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Lavaburst, false);

                        m_Events.ScheduleEvent(eEvents::EventLavaburst, 4 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventUnbindFlame:
                    {
                        me->CastSpell(me, eSpells::UnbindFlame, false);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_iron_flame_binderAI(p_Creature);
        }
};

/// Living Flames - 87993
class npc_foundry_living_flames : public CreatureScript
{
    public:
        npc_foundry_living_flames() : CreatureScript("npc_foundry_living_flames") { }

        enum eSpell
        {
            UnleashFlame = 175613
        };

        struct npc_foundry_living_flamesAI : public ScriptedAI
        {
            npc_foundry_living_flamesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpell::UnleashFlame, false);
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                ScriptedAI::UpdateAI(p_Diff);

                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_living_flamesAI(p_Creature);
        }
};

/// Workshop Guardian - 87411
class npc_foundry_workshop_guardian : public CreatureScript
{
    public:
        npc_foundry_workshop_guardian() : CreatureScript("npc_foundry_workshop_guardian") { }

        enum eSpells
        {
            GrievousMortalWounds    = 175624,
            SpinningBladeMissile    = 175627
        };

        enum eEvents
        {
            EventGrievousMortalWounds = 1,
            EventSpinningBlade
        };

        struct npc_foundry_workshop_guardianAI : public ScriptedAI
        {
            npc_foundry_workshop_guardianAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventGrievousMortalWounds, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSpinningBlade, 8 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventGrievousMortalWounds:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::GrievousMortalWounds, false);

                        m_Events.ScheduleEvent(eEvents::EventGrievousMortalWounds, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSpinningBlade:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpinningBladeMissile, false);

                        m_Events.ScheduleEvent(eEvents::EventSpinningBlade, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_workshop_guardianAI(p_Creature);
        }
};

/// Spinning Blade - 88008
class npc_foundry_spinning_blade : public CreatureScript
{
    public:
        npc_foundry_spinning_blade() : CreatureScript("npc_foundry_spinning_blade") { }

        enum eSpell
        {
            SpinningBladeAura = 175644
        };

        struct npc_foundry_spinning_bladeAI : public ScriptedAI
        {
            npc_foundry_spinning_bladeAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->CastSpell(me, eSpell::SpinningBladeAura, false);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                me->DespawnOrUnsummon(20 * TimeConstants::IN_MILLISECONDS);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_spinning_bladeAI(p_Creature);
        }
};

/// Ogron Hauler - 87719
class npc_foundry_ogron_hauler : public CreatureScript
{
    public:
        npc_foundry_ogron_hauler() : CreatureScript("npc_foundry_ogron_hauler") { }

        enum eSpells
        {
            ThrowSlagContainmentCrate       = 175290,
            CarryingSlagContainmentCrate    = 175285,

            GroundStomp                     = 175762,
            OverheadSmash                   = 175765,
            SlagBreath                      = 175752
        };

        enum eEvents
        {
            EventGroundStomp = 1,
            EventOverheadSmash,
            EventSlagBreath
        };

        struct npc_foundry_ogron_haulerAI : public ScriptedAI
        {
            npc_foundry_ogron_haulerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me ,eSpells::CarryingSlagContainmentCrate, true);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->RemoveAura(eSpells::CarryingSlagContainmentCrate);

                me->CastSpell(p_Attacker, eSpells::ThrowSlagContainmentCrate, true);

                m_Events.ScheduleEvent(eEvents::EventGroundStomp, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventOverheadSmash, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSlagBreath, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventGroundStomp:
                    {
                        me->CastSpell(me, eSpells::GroundStomp, false);
                        m_Events.ScheduleEvent(eEvents::EventGroundStomp, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventOverheadSmash:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::OverheadSmash, false);

                        m_Events.ScheduleEvent(eEvents::EventOverheadSmash, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSlagBreath:
                    {
                        me->CastSpell(me, eSpells::SlagBreath, false);
                        m_Events.ScheduleEvent(eEvents::EventSlagBreath, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_ogron_haulerAI(p_Creature);
        }
};

/// Ironworker - 87448
class npc_foundry_ironworker : public CreatureScript
{
    public:
        npc_foundry_ironworker() : CreatureScript("npc_foundry_ironworker") { }

        enum eSpells
        {
            EmptySack           = 175671,
            PackingExplosiveOre = 175670,
            ThrowExplosiveOre   = 175672,
            HammerThrow         = 175668
        };

        enum eEvents
        {
            EventThrowExplosiveOre = 1,
            EventHammerThrow
        };

        struct npc_foundry_ironworkerAI : public ScriptedAI
        {
            npc_foundry_ironworkerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                if (!me->HasAura(eSpells::PackingExplosiveOre))
                    me->CastSpell(me, eSpells::EmptySack, true);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_ANIM_TIER, 0);

                if (me->HasAura(eSpells::PackingExplosiveOre))
                    m_Events.ScheduleEvent(eEvents::EventThrowExplosiveOre, 4 * TimeConstants::IN_MILLISECONDS);
                else
                    m_Events.ScheduleEvent(eEvents::EventHammerThrow, 4 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventThrowExplosiveOre:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::ThrowExplosiveOre, true);

                        m_Events.ScheduleEvent(eEvents::EventThrowExplosiveOre, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventHammerThrow:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::HammerThrow, false);

                        m_Events.ScheduleEvent(eEvents::EventHammerThrow, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_ironworkerAI(p_Creature);
        }
};

/// Flame Jets - 87843
class npc_foundry_flame_jets : public CreatureScript
{
    public:
        npc_foundry_flame_jets() : CreatureScript("npc_foundry_flame_jets") { }

        enum eSpell
        {
            FlameOn = 175459
        };

        struct npc_foundry_flame_jetsAI : public ScriptedAI
        {
            npc_foundry_flame_jetsAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CastTimer = urand(2 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS);
            }

            uint32 m_CastTimer;

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_CastTimer)
                {
                    if (m_CastTimer <= p_Diff)
                    {
                        me->CastSpell(me, eSpell::FlameOn, true);
                        m_CastTimer = urand(2 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS);
                    }
                    else
                        m_CastTimer -= p_Diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_flame_jetsAI(p_Creature);
        }
};

/// Iron Slag-Shaper - 87521
class npc_foundry_iron_slag_shaper : public CreatureScript
{
    public:
        npc_foundry_iron_slag_shaper() : CreatureScript("npc_foundry_iron_slag_shaper") { }

        enum eSpells
        {
            AnimateSlag         = 175088,
            GrippingSlag        = 175603,
            RainOfSlag          = 175981,
            RainOfSlagVisual    = 175983
        };

        enum eEvents
        {
            EventGrippingSlag = 1,
            EventRainOfSlag
        };

        enum eAction
        {
            ActionRescheduleSlag
        };

        struct npc_foundry_iron_slag_shaperAI : public ScriptedAI
        {
            npc_foundry_iron_slag_shaperAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(100, [this]() -> void
                {
                    me->CastSpell(me, eSpells::AnimateSlag, false);
                });
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventGrippingSlag, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRainOfSlag, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionRescheduleSlag)
                {
                    AddTimedDelayedOperation(100, [this]() -> void
                    {
                        me->CastSpell(me, eSpells::AnimateSlag, false);
                    });
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventGrippingSlag:
                    {
                        me->CastSpell(me, eSpells::GrippingSlag, false);
                        m_Events.ScheduleEvent(eEvents::EventGrippingSlag, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventRainOfSlag:
                    {
                        me->CastSpell(me, eSpells::RainOfSlag, false);
                        me->CastSpell(me, eSpells::RainOfSlagVisual, true);
                        m_Events.ScheduleEvent(eEvents::EventRainOfSlag, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_iron_slag_shaperAI(p_Creature);
        }
};

/// Slagshop Worker - 87771
class npc_foundry_slagshop_worker : public CreatureScript
{
    public:
        npc_foundry_slagshop_worker() : CreatureScript("npc_foundry_slagshop_worker") { }

        enum eSpells
        {
            PunctureWound   = 175987,
            ThrowCoin       = 177475
        };

        enum eEvent
        {
            EventPunctureWound = 1
        };

        enum eTalk
        {
            TalkCosmetic
        };

        enum eCreature
        {
            SlagshopBrute = 87780
        };

        struct npc_foundry_slagshop_workerAI : public ScriptedAI
        {
            npc_foundry_slagshop_workerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                if (Creature* l_Brute = me->FindNearestCreature(eCreature::SlagshopBrute, 10.0f))
                {
                    AddTimedDelayedOperation(urand(1 * TimeConstants::IN_MILLISECONDS, 120 * TimeConstants::IN_MILLISECONDS), [this]() -> void
                    {
                        Talk(eTalk::TalkCosmetic);

                        me->CastSpell(me, eSpells::ThrowCoin, true);
                    });
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                ClearDelayedOperations();

                me->SetUInt32Value(EUnitFields::UNIT_FIELD_ANIM_TIER, 0);

                m_Events.ScheduleEvent(eEvent::EventPunctureWound, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventPunctureWound:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::PunctureWound, true);

                        m_Events.ScheduleEvent(eEvent::EventPunctureWound, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void LastOperationCalled() override
            {
                AddTimedDelayedOperation(urand(1 * TimeConstants::IN_MILLISECONDS, 120 * TimeConstants::IN_MILLISECONDS), [this]() -> void
                {
                    Talk(eTalk::TalkCosmetic);

                    me->CastSpell(me, eSpells::ThrowCoin, true);
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_slagshop_workerAI(p_Creature);
        }
};

/// Slagshop Brute - 87780
class npc_foundry_slagshop_brute : public CreatureScript
{
    public:
        npc_foundry_slagshop_brute() : CreatureScript("npc_foundry_slagshop_brute") { }

        enum eSpells
        {
            LumberingStrength   = 175993,
            DeathHowl           = 175992
        };

        enum eEvents
        {
            EventDeathHowl = 1,
            EventLumberingStrength
        };

        struct npc_foundry_slagshop_bruteAI : public ScriptedAI
        {
            npc_foundry_slagshop_bruteAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->SetAIAnimKitId(0);

                m_Events.ScheduleEvent(eEvents::EventDeathHowl, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventLumberingStrength, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventDeathHowl:
                    {
                        me->CastSpell(me, eSpells::DeathHowl, false);
                        m_Events.ScheduleEvent(eEvents::EventDeathHowl, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventLumberingStrength:
                    {
                        me->CastSpell(me, eSpells::LumberingStrength, false);
                        m_Events.ScheduleEvent(eEvents::EventLumberingStrength, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_slagshop_bruteAI(p_Creature);
        }
};

/// Iron Journeyman - 80677
class npc_foundry_iron_journeyman : public CreatureScript
{
    public:
        npc_foundry_iron_journeyman() : CreatureScript("npc_foundry_iron_journeyman") { }

        enum eSpell
        {
            Heymaker = 164102
        };

        enum eEvent
        {
            EventHeymaker = 1
        };

        struct npc_foundry_iron_journeymanAI : public ScriptedAI
        {
            npc_foundry_iron_journeymanAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_ANIM_TIER, 0);

                m_Events.ScheduleEvent(eEvent::EventHeymaker, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventHeymaker:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpell::Heymaker, false);

                        m_Events.ScheduleEvent(eEvent::EventHeymaker, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_iron_journeymanAI(p_Creature);
        }
};

/// Gronnling Laborer - 81114
class npc_foundry_gronnling_laborer : public CreatureScript
{
    public:
        npc_foundry_gronnling_laborer() : CreatureScript("npc_foundry_gronnling_laborer") { }

        enum eSpells
        {
            Enrage          = 18501,
            GronnlingSmash  = 169402,
            EarthAttack     = 165318,
            HeadSmashDmg    = 170604,
            HeadSmashDust   = 165427
        };

        enum eEvents
        {
            EventGronnlingSmash = 1,
            EventHeadSmash
        };

        struct npc_foundry_gronnling_laborerAI : public ScriptedAI
        {
            npc_foundry_gronnling_laborerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_ANIM_TIER, 0);

                me->RemoveAura(eSpells::EarthAttack);

                m_Events.ScheduleEvent(eEvents::EventGronnlingSmash, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventHeadSmash, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HasAura(eSpells::Enrage))
                    return;

                if (me->HealthBelowPctDamaged(50, p_Damage))
                    me->CastSpell(me, eSpells::Enrage, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventGronnlingSmash:
                    {
                        me->CastSpell(me, eSpells::GronnlingSmash, false);
                        m_Events.ScheduleEvent(eEvents::EventGronnlingSmash, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventHeadSmash:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::HeadSmashDmg, true);
                            me->CastSpell(me, eSpells::HeadSmashDust, true);
                        }

                        m_Events.ScheduleEvent(eEvents::EventHeadSmash, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_gronnling_laborerAI(p_Creature);
        }
};

/// Karnor the Cruel - 81117
class npc_foundry_karnor_the_cruel : public CreatureScript
{
    public:
        npc_foundry_karnor_the_cruel() : CreatureScript("npc_foundry_karnor_the_cruel") { }

        enum eSpells
        {
            ChainVolley             = 170679,
            ChainVolleyDummy        = 170680,
            KillingSpree            = 170687,
            EnvenomedBladeTrigger   = 177943,
            EnvenomedBlade          = 170689
        };

        enum eEvents
        {
            EventChainVolley = 1,
            EventKillingSpree
        };

        enum eData
        {
            ChainVolleyCount = 7
        };

        struct npc_foundry_karnor_the_cruelAI : public ScriptedAI
        {
            npc_foundry_karnor_the_cruelAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventChainVolley, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventKillingSpree, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::ChainVolley:
                    {
                        for (uint8 l_I = 0; l_I < eData::ChainVolleyCount; ++l_I)
                            p_Target->CastSpell(p_Target, eSpells::ChainVolleyDummy, true);

                        break;
                    }
                    case eSpells::EnvenomedBladeTrigger:
                    {
                        me->NearTeleportTo(*p_Target);

                        uint64 l_Guid = p_Target->GetGUID();
                        AddTimedDelayedOperation(100, [this, l_Guid]() -> void
                        {
                            if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                                me->CastSpell(l_Target, eSpells::EnvenomedBlade, false);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventChainVolley:
                    {
                        me->CastSpell(me, eSpells::ChainVolley, true);
                        m_Events.ScheduleEvent(eEvents::EventChainVolley, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventKillingSpree:
                    {
                        me->CastSpell(me, eSpells::KillingSpree, true);
                        m_Events.ScheduleEvent(eEvents::EventKillingSpree, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_karnor_the_cruelAI(p_Creature);
        }
};

/// Iron Laborer (Cosmetic) - 78977
class npc_foundry_iron_laborer_cosmetic : public CreatureScript
{
    public:
        npc_foundry_iron_laborer_cosmetic() : CreatureScript("npc_foundry_iron_laborer_cosmetic") { }

        enum eActions
        {
            ActionDisable   = 0,
            ActionEvent     = 0
        };

        enum eCreature
        {
            IronLaborer = 78917
        };

        struct npc_foundry_iron_laborer_cosmeticAI : public ScriptedAI
        {
            npc_foundry_iron_laborer_cosmeticAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Started(false) { }

            bool m_Started;

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::ActionDisable)
                    m_Started = true;
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (m_Started)
                    return;

                if (p_Who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (p_Who->GetDistance(me) > 30.0f)
                    return;

                m_Started = true;

                std::list<Creature*> l_Creatures;
                me->GetCreatureListWithEntryInGrid(l_Creatures, me->GetEntry(), 200.0f);

                l_Creatures.remove(me);

                for (Creature* l_Me : l_Creatures)
                {
                    if (l_Me->IsAIEnabled)
                        l_Me->AI()->DoAction(eActions::ActionDisable);
                }

                LaunchTunnelEvent();
            }

            void LaunchTunnelEvent()
            {
                std::list<Creature*> l_Creatures;
                me->GetCreatureListWithEntryInGrid(l_Creatures, eCreature::IronLaborer, 200.0f);

                for (Creature* l_Laborer : l_Creatures)
                {
                    if (l_Laborer->IsAIEnabled)
                        l_Laborer->AI()->DoAction(eActions::ActionEvent);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_iron_laborer_cosmeticAI(p_Creature);
        }
};

/// Iron Laborer - 78917
class npc_foundry_iron_laborer : public CreatureScript
{
    public:
        npc_foundry_iron_laborer() : CreatureScript("npc_foundry_iron_laborer") { }

        enum eAction
        {
            ActionEvent
        };

        enum eCreatures
        {
            DarkshardAcidback   = 78801,
            DarkshardGnasher    = 78978
        };

        enum eMoves
        {
            MoveFirst,
            MoveSecond
        };

        struct npc_foundry_iron_laborerAI : public ScriptedAI
        {
            npc_foundry_iron_laborerAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit()) { }

            Vehicle* m_Vehicle;

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionEvent)
                {
                    std::list<Creature*> l_Darkshards;
                    me->GetCreatureListWithEntryInGrid(l_Darkshards, eCreatures::DarkshardAcidback, 5.0f);

                    uint8 l_DarkshardCount = 2;
                    for (Creature* l_Darkshard : l_Darkshards)
                    {
                        if (l_Darkshard->GetVehicle())
                            continue;

                        l_Darkshard->EnterVehicle(me);

                        if (!--l_DarkshardCount)
                            break;
                    }

                    l_Darkshards.clear();

                    me->GetCreatureListWithEntryInGrid(l_Darkshards, eCreatures::DarkshardGnasher, 20.0f);

                    for (Creature* l_Darkshard : l_Darkshards)
                    {
                        if (l_Darkshard->IsAIEnabled)
                            l_Darkshard->AI()->DoAction(eAction::ActionEvent);
                    }

                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        float l_X, l_Y, l_Z, l_O;

                        l_O = me->GetOrientation();
                        l_X = me->GetPositionX() + (30.0f * cos(l_O));
                        l_Y = me->GetPositionY() + (30.0f * sin(l_O));

                        l_Z = me->GetPositionZ();
                        l_Z = me->GetMap()->GetHeight(me->GetPhaseMask(), l_X, l_Y, l_Z) + 1.0f;

                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(eMoves::MoveFirst, l_X, l_Y, l_Z);
                    });
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveFirst:
                    {
                        AddTimedDelayedOperation(200, [this]() -> void
                        {
                            float l_X, l_Y, l_Z;

                            l_X = me->GetPositionX() + (30.0f * cos(0.0f));
                            l_Y = me->GetPositionY() + (30.0f * sin(0.0f));

                            l_Z = me->GetPositionZ();
                            l_Z = me->GetMap()->GetHeight(me->GetPhaseMask(), l_X, l_Y, l_Z);

                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(eMoves::MoveSecond, l_X, l_Y, l_Z);
                        });

                        break;
                    }
                    case eMoves::MoveSecond:
                    {
                        m_Vehicle->RemoveAllPassengers();
                        me->Kill(me);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_iron_laborerAI(p_Creature);
        }
};

/// Darkshard Acidback - 78801
class npc_foundry_darkshard_acidback : public CreatureScript
{
    public:
        npc_foundry_darkshard_acidback() : CreatureScript("npc_foundry_darkshard_acidback") { }

        struct npc_foundry_darkshard_acidbackAI : public ScriptedAI
        {
            npc_foundry_darkshard_acidbackAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void OnVehicleExited(Unit* p_Vehicle) override
            {
                me->SetHomePosition(*me);

                if (Player* l_Target = me->SelectNearestPlayerNotGM(15.0f))
                    AttackStart(l_Target);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_darkshard_acidbackAI(p_Creature);
        }
};

/// Darkshard Gnasher - 78978
class npc_foundry_darkshard_gnasher : public CreatureScript
{
    public:
        npc_foundry_darkshard_gnasher() : CreatureScript("npc_foundry_darkshard_gnasher") { }

        enum eAction
        {
            ActionEvent
        };

        enum eMoves
        {
            MoveFirst,
            MoveSecond
        };

        enum eSpells
        {
            RollingMovement         = 159511,

            ShatteringChargeSeacher = 159551,
            ShatteringChargeTrigger = 159518,
            ShatteringChargeEnding  = 159519,

            InsatiableHungerSearch  = 159631,
            InsatiableHungerTrigger = 159632,
            InsatiableHungerAura    = 159700
        };

        enum eEvents
        {
            EventShatteringCharge = 1,
            EventInsatiableHunger
        };

        struct npc_foundry_darkshard_gnasherAI : public ScriptedAI
        {
            npc_foundry_darkshard_gnasherAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Moved(false) { }

            EventMap m_Events;

            bool m_Moved;

            uint64 m_FixateTarget;

            void Reset() override
            {
                m_Events.Reset();

                m_FixateTarget = 0;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventShatteringCharge, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventInsatiableHunger, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionEvent && !m_Moved)
                {
                    m_Moved = true;

                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->CastSpell(me, eSpells::RollingMovement, true);

                        float l_X, l_Y, l_Z, l_O;

                        l_O = me->GetOrientation();
                        l_X = me->GetPositionX() + (35.0f * cos(l_O));
                        l_Y = me->GetPositionY() + (35.0f * sin(l_O));

                        l_Z = me->GetPositionZ();
                        l_Z = me->GetMap()->GetHeight(me->GetPhaseMask(), l_X, l_Y, l_Z);

                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(eMoves::MoveFirst, l_X, l_Y, l_Z);
                    });
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveFirst:
                    {
                        AddTimedDelayedOperation(100, [this]() -> void
                        {
                            float l_X, l_Y, l_Z;

                            l_X = me->GetPositionX() + (30.0f * cos(0.0f));
                            l_Y = me->GetPositionY() + (30.0f * sin(0.0f));

                            l_Z = me->GetPositionZ();
                            l_Z = me->GetMap()->GetHeight(me->GetPhaseMask(), l_X, l_Y, l_Z);

                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(eMoves::MoveSecond, l_X, l_Y, l_Z);
                        });

                        break;
                    }
                    case eMoves::MoveSecond:
                    {
                        me->RemoveAura(eSpells::RollingMovement);

                        if (Player* l_Target = me->SelectNearestPlayerNotGM(15.0f))
                            AttackStart(l_Target);

                        break;
                    }
                    case EventId::EVENT_CHARGE:
                    {
                        me->CastSpell(me, eSpells::ShatteringChargeEnding, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::ShatteringChargeSeacher:
                    {
                        me->CastSpell(p_Target, eSpells::ShatteringChargeTrigger, false);
                        break;
                    }
                    case eSpells::InsatiableHungerSearch:
                    {
                        m_FixateTarget = p_Target->GetGUID();
                        me->CastSpell(p_Target, eSpells::InsatiableHungerTrigger, false);

                        AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void { m_FixateTarget = 0; });

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (m_FixateTarget)
                {
                    if (Unit* l_Target = Unit::GetUnit(*me, m_FixateTarget))
                    {
                        if (l_Target->isAlive())
                        {
                            AttackStart(l_Target);
                            DoMeleeAttackIfReady();
                            return;
                        }
                        else
                        {
                            me->RemoveAura(eSpells::InsatiableHungerAura);
                            m_FixateTarget = 0;
                        }
                    }
                }

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventShatteringCharge:
                    {
                        me->CastSpell(me, eSpells::ShatteringChargeSeacher, true);
                        m_Events.ScheduleEvent(eEvents::EventShatteringCharge, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventInsatiableHunger:
                    {
                        me->CastSpell(me, eSpells::InsatiableHungerSearch, true);
                        m_Events.ScheduleEvent(eEvents::EventInsatiableHunger, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_darkshard_gnasherAI(p_Creature);
        }
};

/// Darkshard Crystalback - 78233
class npc_foundry_darkshard_crystalback : public CreatureScript
{
    public:
        npc_foundry_darkshard_crystalback() : CreatureScript("npc_foundry_darkshard_crystalback") { }

        enum eSpells
        {
            Acidmaw             = 159939,
            ShardVolleySearcher = 159769,
            ShardVolleyTrigger  = 159775
        };

        enum eEvents
        {
            EventAcidMaw = 1,
            EventShardVolley
        };

        struct npc_foundry_darkshard_crystalbackAI : public ScriptedAI
        {
            npc_foundry_darkshard_crystalbackAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventAcidMaw, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventShardVolley, 9 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::ShardVolleySearcher)
                {
                    me->CastSpell(p_Target, eSpells::ShardVolleyTrigger, true);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventAcidMaw:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Acidmaw, true);

                        m_Events.ScheduleEvent(eEvents::EventAcidMaw, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShardVolley:
                    {
                        me->CastSpell(me, eSpells::ShardVolleySearcher, false);
                        m_Events.ScheduleEvent(eEvents::EventShardVolley, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_darkshard_crystalbackAI(p_Creature);
        }
};

/// Slag Behemoth - 77504
class npc_foundry_slag_behemoth : public CreatureScript
{
    public:
        npc_foundry_slag_behemoth() : CreatureScript("npc_foundry_slag_behemoth") { }

        enum eSpells
        {
            IgniteSearcher      = 156345,

            VolcanicBombSearch  = 156348,
            VolcanicBombMissile = 156413,

            BlastWave           = 156446
        };

        enum eEvents
        {
            EventIgnite = 1,
            EventVolcanicBomb,
            EventBlastWave
        };

        enum eTalk
        {
            TalkBlastWave
        };

        enum eGameObjects
        {
            VolcanicBomb    = 227616,
            IceBlock        = 201722
        };

        enum eAction
        {
            ActionIntro
        };

        struct npc_foundry_slag_behemothAI : public ScriptedAI
        {
            npc_foundry_slag_behemothAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            uint64 m_EncounterDoor;

            void Reset() override
            {
                m_EncounterDoor = 0;

                m_Events.Reset();

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (GameObject* l_FurnaceDoor = me->FindNearestGameObject(eFoundryGameObjects::BlastFurnaceEncounterDoor, 30.0f))
                    {
                        m_EncounterDoor = l_FurnaceDoor->GetGUID();
                        l_FurnaceDoor->SetGoState(GOState::GO_STATE_READY);
                    }
                });

                std::list<GameObject*> l_Gobs;
                me->GetGameObjectListWithEntryInGrid(l_Gobs, eGameObjects::IceBlock, 100.0f);

                for (GameObject* l_GameObject : l_Gobs)
                    l_GameObject->Delete();

                l_Gobs.clear();

                me->GetGameObjectListWithEntryInGrid(l_Gobs, eGameObjects::VolcanicBomb, 100.0f);

                for (GameObject* l_GameObject : l_Gobs)
                    l_GameObject->Delete();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventIgnite, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventVolcanicBomb, 11 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventBlastWave, 30 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (GameObject* l_FurnaceDoor = GameObject::GetGameObject(*me, m_EncounterDoor))
                    l_FurnaceDoor->SetGoState(GOState::GO_STATE_ACTIVE);

                std::list<GameObject*> l_Gobs;
                me->GetGameObjectListWithEntryInGrid(l_Gobs, eGameObjects::IceBlock, 100.0f);

                for (GameObject* l_GameObject : l_Gobs)
                    l_GameObject->Delete();

                l_Gobs.clear();

                me->GetGameObjectListWithEntryInGrid(l_Gobs, eGameObjects::VolcanicBomb, 100.0f);

                for (GameObject* l_GameObject : l_Gobs)
                    l_GameObject->Delete();

                if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                {
                    if (Creature* l_Creature = Creature::GetCreature(*me, l_InstanceScript->GetData64(eFoundryCreatures::BlackhandCosmetic)))
                    {
                        if (l_Creature->IsAIEnabled)
                            l_Creature->AI()->DoAction(eAction::ActionIntro);
                    }
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::VolcanicBombSearch:
                    {
                        me->CastSpell(*p_Target, eSpells::VolcanicBombMissile, false);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventIgnite:
                    {
                        me->CastSpell(me, eSpells::IgniteSearcher, false);
                        m_Events.ScheduleEvent(eEvents::EventIgnite, 13 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventVolcanicBomb:
                    {
                        me->CastSpell(me, eSpells::VolcanicBombSearch, true);
                        m_Events.ScheduleEvent(eEvents::EventVolcanicBomb, 11 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventBlastWave:
                    {
                        Talk(eTalk::TalkBlastWave);

                        me->CastSpell(me, eSpells::BlastWave, false);
                        m_Events.ScheduleEvent(eEvents::EventBlastWave, 35 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_slag_behemothAI(p_Creature);
        }
};

/// Blackrock Enforcer - 79208
class npc_foundry_blackrock_enforcer : public CreatureScript
{
    public:
        npc_foundry_blackrock_enforcer() : CreatureScript("npc_foundry_blackrock_enforcer") { }

        enum eSpells
        {
            ClobberingStrike    = 160102,
            FireBombMissile     = 160250,
            Intimidation        = 160109
        };

        enum eEvents
        {
            EventClobberingStrike = 1,
            EventFireBomb,
            EventIntimidation
        };

        struct npc_foundry_blackrock_enforcerAI : public ScriptedAI
        {
            npc_foundry_blackrock_enforcerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventClobberingStrike, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventFireBomb, 7 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventIntimidation, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (InstanceScript* l_Instance = me->GetInstanceScript())
                {
                    if (Creature* l_Hansgar = Creature::GetCreature(*me, l_Instance->GetData64(eFoundryCreatures::BossHansgar)))
                    {
                        if (l_Hansgar->IsAIEnabled)
                            l_Hansgar->AI()->SetGUID(me->GetGUID(), 0);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventClobberingStrike:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::ClobberingStrike, false);
                        m_Events.ScheduleEvent(eEvents::EventClobberingStrike, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventFireBomb:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::FireBombMissile, false);
                        m_Events.ScheduleEvent(eEvents::EventFireBomb, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventIntimidation:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(*l_Target, eSpells::Intimidation, false);
                        m_Events.ScheduleEvent(eEvents::EventIntimidation, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_blackrock_enforcerAI(p_Creature);
        }
};

/// Blackrock Forge Specialist - 79200
class npc_foundry_blackrock_forge_specialist : public CreatureScript
{
    public:
        npc_foundry_blackrock_forge_specialist() : CreatureScript("npc_foundry_blackrock_forge_specialist") { }

        enum eSpells
        {
            RendingSlash    = 160092,
            ShreddingSpear  = 160079
        };

        enum eEvents
        {
            EventRendingSlash = 1,
            EventShreddingSpear
        };

        struct npc_foundry_blackrock_forge_specialistAI : public ScriptedAI
        {
            npc_foundry_blackrock_forge_specialistAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventRendingSlash, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventShreddingSpear, 8 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (InstanceScript* l_Instance = me->GetInstanceScript())
                {
                    if (Creature* l_Hansgar = Creature::GetCreature(*me, l_Instance->GetData64(eFoundryCreatures::BossHansgar)))
                    {
                        if (l_Hansgar->IsAIEnabled)
                            l_Hansgar->AI()->SetGUID(me->GetGUID(), 0);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventRendingSlash:
                    {
                        me->CastSpell(me, eSpells::RendingSlash, false);
                        m_Events.ScheduleEvent(eEvents::EventRendingSlash, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShreddingSpear:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 5.0f))
                            me->CastSpell(l_Target, eSpells::ShreddingSpear, false);
                        m_Events.ScheduleEvent(eEvents::EventShreddingSpear, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_blackrock_forge_specialistAI(p_Creature);
        }
};

/// Grievous Mortal Wounds - 175624
class spell_foundry_grievous_mortal_wounds : public SpellScriptLoader
{
    public:
        spell_foundry_grievous_mortal_wounds() : SpellScriptLoader("spell_foundry_grievous_mortal_wounds") { }

        class spell_foundry_grievous_mortal_wounds_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_grievous_mortal_wounds_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    /// Inflicts Physical damage and reduces healing received by -5% until the target is healed to full health.
                    if (l_Target->IsFullHealth())
                        p_AurEff->GetBase()->SetDuration(1);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundry_grievous_mortal_wounds_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_grievous_mortal_wounds_AuraScript();
        }
};

/// Spinning Blade - 175644
class spell_foundry_spinning_blade : public SpellScriptLoader
{
    public:
        spell_foundry_spinning_blade() : SpellScriptLoader("spell_foundry_spinning_blade") { }

        class spell_foundry_spinning_blade_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_spinning_blade_AuraScript);

            enum eSpell
            {
                SpinningBladeDoT = 175643
            };

            uint32 m_CheckTimer;

            bool Load()
            {
                m_CheckTimer = 200;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= p_Diff)
                    {
                        if (Unit* l_Caster = GetCaster())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 8.0f;

                            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                            l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                            l_TargetList.remove(l_Caster);

                            for (Unit* l_Iter : l_TargetList)
                            {
                                if (l_Iter->GetDistance(l_Caster) <= 1.0f)
                                {
                                    if (!l_Iter->HasAura(eSpell::SpinningBladeDoT))
                                        l_Iter->CastSpell(l_Iter, eSpell::SpinningBladeDoT, true, nullptr, NULLAURA_EFFECT, l_Caster->GetGUID());
                                }
                                else
                                {
                                    if (l_Iter->HasAura(eSpell::SpinningBladeDoT))
                                        l_Iter->RemoveAura(eSpell::SpinningBladeDoT);
                                }
                            }
                        }

                        m_CheckTimer = 200;
                    }
                    else
                        m_CheckTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_foundry_spinning_blade_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_spinning_blade_AuraScript();
        }
};

/// Slag Breath - 175752
class spell_foundry_slag_breath : public SpellScriptLoader
{
    public:
        spell_foundry_slag_breath() : SpellScriptLoader("spell_foundry_slag_breath") { }

        class spell_foundry_slag_breath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_slag_breath_SpellScript);

            enum eSpell
            {
                TargetRestrict = 24223
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_slag_breath_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_slag_breath_SpellScript();
        }
};

/// Animate Slag - 175088
class spell_foundry_animate_slag : public SpellScriptLoader
{
    public:
        spell_foundry_animate_slag() : SpellScriptLoader("spell_foundry_animate_slag") { }

        class spell_foundry_animate_slag_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_animate_slag_AuraScript);

            enum eSpell
            {
                AnimateSlagMissile = 175089
            };

            enum eAction
            {
                ActionRescheduleSlag
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Unit* l_Caster = GetCaster())
                        l_Target->CastSpell(l_Caster, eSpell::AnimateSlagMissile, true);
                }
            }

            void AfterRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode != AuraRemoveMode::AURA_REMOVE_BY_EXPIRE || GetCaster() == nullptr)
                    return;

                if (Creature* l_Creature = GetCaster()->ToCreature())
                {
                    if (!l_Creature->IsAIEnabled)
                        return;

                    l_Creature->AI()->DoAction(eAction::ActionRescheduleSlag);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundry_animate_slag_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_foundry_animate_slag_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_animate_slag_AuraScript();
        }
};

/// Gronnling Smash - 169402
class spell_foundry_gronnling_smash : public SpellScriptLoader
{
    public:
        spell_foundry_gronnling_smash() : SpellScriptLoader("spell_foundry_gronnling_smash") { }

        class spell_foundry_gronnling_smash_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_gronnling_smash_AuraScript);

            enum eSpell
            {
                GronnlingSmashAoE = 169401
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpell::GronnlingSmashAoE, TriggerCastFlags::TRIGGERED_IGNORE_CAST_IN_PROGRESS);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundry_gronnling_smash_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_gronnling_smash_AuraScript();
        }
};

/// Shattering Charge - 159518
class spell_foundry_shattering_charge : public SpellScriptLoader
{
    public:
        spell_foundry_shattering_charge() : SpellScriptLoader("spell_foundry_shattering_charge") { }

        class spell_foundry_shattering_charge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_shattering_charge_AuraScript);

            enum eSpell
            {
                ShatteringCharge = 159520
            };

            uint32 m_CheckTimer;

            bool Load()
            {
                m_CheckTimer = 200;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= p_Diff)
                    {
                        if (Unit* l_Caster = GetCaster())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 8.0f;

                            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                            l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                            l_TargetList.remove(l_Caster);

                            for (Unit* l_Iter : l_TargetList)
                            {
                                if (l_Iter->GetDistance(l_Caster) <= 2.5f)
                                    l_Caster->CastSpell(l_Iter, eSpell::ShatteringCharge, true);
                            }
                        }

                        m_CheckTimer = 200;
                    }
                    else
                        m_CheckTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_foundry_shattering_charge_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_shattering_charge_AuraScript();
        }
};

/// Ignite - 156345
class spell_foundry_ignite_aura : public SpellScriptLoader
{
    public:
        spell_foundry_ignite_aura() : SpellScriptLoader("spell_foundry_ignite_aura") { }

        class spell_foundry_ignite_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_ignite_aura_AuraScript);

            enum eSpell
            {
                IgniteAoE = 156346
            };

            void AfterRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(*l_Target, eSpell::IgniteAoE, true);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_foundry_ignite_aura_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_ignite_aura_AuraScript();
        }
};

/// Blast Wave - 158424
class spell_foundry_blast_wave : public SpellScriptLoader
{
    public:
        spell_foundry_blast_wave() : SpellScriptLoader("spell_foundry_blast_wave") { }

        class spell_foundry_blast_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_blast_wave_SpellScript);

            enum eGameObjects
            {
                VolcanicBomb    = 227616,
                IceBlock        = 201722
            };

            void SelectIceBlocks(std::list<WorldObject*>& p_Targets)
            {
                p_Targets.clear();

                if (Unit* l_Caster = GetCaster())
                {
                    for (uint8 l_I = 0; l_I < (uint8)EFFECT_2; ++l_I)
                    {
                        float l_Radius = GetSpellInfo()->Effects[l_I].CalcRadius(l_Caster);

                        std::list<GameObject*> l_GoBList;
                        l_Caster->GetGameObjectListWithEntryInGrid(l_GoBList, l_I == 0 ? eGameObjects::IceBlock : eGameObjects::VolcanicBomb, l_Radius);

                        for (GameObject* l_Gob : l_GoBList)
                            p_Targets.push_back(l_Gob);
                    }
                }
            }

            void HandleActivateObject(SpellEffIndex p_EffIndex)
            {
                if (GameObject* l_GameObject = GetHitGObj())
                    l_GameObject->Delete();
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_blast_wave_SpellScript::SelectIceBlocks, EFFECT_0, TARGET_GAMEOBJECT_SRC_AREA);
                OnEffectHitTarget += SpellEffectFn(spell_foundry_blast_wave_SpellScript::HandleActivateObject, EFFECT_0, SPELL_EFFECT_ACTIVATE_OBJECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_blast_wave_SpellScript();
        }
};

/// Rending Slash - 160092
class spell_foundry_rending_slash : public SpellScriptLoader
{
    public:
        spell_foundry_rending_slash() : SpellScriptLoader("spell_foundry_rending_slash") { }

        class spell_foundry_rending_slash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_rending_slash_SpellScript);

            enum eSpell
            {
                TargetRestrict = 22509
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_rending_slash_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_rending_slash_SpellScript();
        }
};

/// Acidback Puddle - 159121
class areatrigger_foundry_acidback_puddle : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_acidback_puddle() : AreaTriggerEntityScript("areatrigger_foundry_acidback_puddle") { }

        enum eSpell
        {
            AcidbackPuddleDoT = 159686
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 15.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Unit->GetDistance(p_AreaTrigger) <= 2.8f)
                    {
                        if (!l_Unit->HasAura(eSpell::AcidbackPuddleDoT))
                            l_Unit->CastSpell(l_Unit, eSpell::AcidbackPuddleDoT, true);
                    }
                    else if (!l_Unit->FindNearestAreaTrigger(p_AreaTrigger->GetSpellId(), 2.8f))
                    {
                        if (l_Unit->HasAura(eSpell::AcidbackPuddleDoT))
                            l_Unit->RemoveAura(eSpell::AcidbackPuddleDoT);
                    }
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 6.5f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    std::list<AreaTrigger*> l_ATList;
                    l_Unit->GetAreatriggerListInRange(l_ATList, 2.8f);

                    if (l_ATList.empty())
                    {
                        if (l_Unit->HasAura(eSpell::AcidbackPuddleDoT))
                            l_Unit->RemoveAura(eSpell::AcidbackPuddleDoT);
                    }

                    uint32 l_SpellID = p_AreaTrigger->GetSpellId();
                    l_ATList.remove_if([this, l_SpellID](AreaTrigger* p_AreaTrigger) -> bool
                    {
                        if (p_AreaTrigger == nullptr || p_AreaTrigger->GetSpellId() != l_SpellID)
                            return true;

                        return false;
                    });

                    if (l_ATList.empty())
                    {
                        if (l_Unit->HasAura(eSpell::AcidbackPuddleDoT))
                            l_Unit->RemoveAura(eSpell::AcidbackPuddleDoT);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_acidback_puddle();
        }
};

/// Fire Bomb - 160259
class areatrigger_foundry_fire_bomb : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_fire_bomb() : AreaTriggerEntityScript("areatrigger_foundry_fire_bomb") { }

        enum eSpell
        {
            FireBombDoT = 160260
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 30.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Unit->GetDistance(p_AreaTrigger) <= 12.0f)
                    {
                        if (!l_Unit->HasAura(eSpell::FireBombDoT, l_Caster->GetGUID()))
                            l_Caster->CastSpell(l_Unit, eSpell::FireBombDoT, true);
                    }
                    else
                    {
                        if (l_Unit->HasAura(eSpell::FireBombDoT, l_Caster->GetGUID()))
                            l_Unit->RemoveAura(eSpell::FireBombDoT, l_Caster->GetGUID());
                    }
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 30.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Unit->HasAura(eSpell::FireBombDoT, l_Caster->GetGUID()))
                        l_Unit->RemoveAura(eSpell::FireBombDoT, l_Caster->GetGUID());
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_fire_bomb();
        }
};

/// First Floor Trap - 10276
class areatrigger_at_foundry_first_floor_trap : public AreaTriggerScript
{
    public:
        areatrigger_at_foundry_first_floor_trap() : AreaTriggerScript("areatrigger_at_foundry_first_floor_trap") { }

        enum eSpell
        {
            KromogsFury = 175331
        };

        enum eCreature
        {
            KromogsWrath = 87727
        };

        void OnEnter(Player* p_Player, AreaTriggerEntry const* p_AreaTrigger) override
        {
            float l_MaxZ = 308.45f;
            float l_MinZ = 274.45f;

            std::list<Creature*> l_WrathList;
            p_Player->GetCreatureListWithEntryInGrid(l_WrathList, eCreature::KromogsWrath, 150.0f);

            if (l_WrathList.empty())
                return;

            l_WrathList.remove_if([this, l_MaxZ, l_MinZ](Creature* p_Creature) -> bool
            {
                if (p_Creature == nullptr || p_Creature->GetPositionZ() >= l_MaxZ || p_Creature->GetPositionZ() <= l_MinZ)
                    return true;

                return false;
            });

            for (Creature* l_Wrath : l_WrathList)
            {
                if (l_Wrath->HasAura(eSpell::KromogsFury))
                    continue;

                l_Wrath->CastSpell(l_Wrath, eSpell::KromogsFury, true);
            }
        }
};

/// Second Floor Trap - 10277
class areatrigger_at_foundry_second_floor_trap : public AreaTriggerScript
{
    public:
        areatrigger_at_foundry_second_floor_trap() : AreaTriggerScript("areatrigger_at_foundry_second_floor_trap") { }

        enum eSpell
        {
            KromogsFury = 175331
        };

        enum eCreature
        {
            KromogsWrath = 87727
        };

        void OnEnter(Player* p_Player, AreaTriggerEntry const* p_AreaTrigger) override
        {
            float l_MaxZ = 266.06f;
            float l_MinZ = 227.40f;

            std::list<Creature*> l_WrathList;
            p_Player->GetCreatureListWithEntryInGrid(l_WrathList, eCreature::KromogsWrath, 150.0f);

            if (l_WrathList.empty())
                return;

            l_WrathList.remove_if([this, l_MaxZ, l_MinZ](Creature* p_Creature) -> bool
            {
                if (p_Creature == nullptr || p_Creature->GetPositionZ() >= l_MaxZ || p_Creature->GetPositionZ() <= l_MinZ)
                    return true;

                return false;
            });

            for (Creature* l_Wrath : l_WrathList)
            {
                if (l_Wrath->HasAura(eSpell::KromogsFury))
                    continue;

                l_Wrath->CastSpell(l_Wrath, eSpell::KromogsFury, true);
            }
        }
};

/// Hans'gar & Franzok Entrance - 9998
class areatrigger_at_foundry_hansgar_and_franzok_entrance : public AreaTriggerScript
{
    public:
        areatrigger_at_foundry_hansgar_and_franzok_entrance() : AreaTriggerScript("areatrigger_at_foundry_hansgar_and_franzok_entrance") { }

        enum eAction
        {
            ActionIntro
        };

        void OnEnter(Player* p_Player, AreaTriggerEntry const* p_AreaTrigger) override
        {
            if (InstanceScript* l_Instance = p_Player->GetInstanceScript())
            {
                if (Creature* l_Hansgar = Creature::GetCreature(*p_Player, l_Instance->GetData64(eFoundryCreatures::BossHansgar)))
                {
                    if (l_Hansgar->IsAIEnabled)
                        l_Hansgar->AI()->DoAction(eAction::ActionIntro);
                }

                if (Creature* l_Franzok = Creature::GetCreature(*p_Player, l_Instance->GetData64(eFoundryCreatures::BossFranzok)))
                {
                    if (l_Franzok->IsAIEnabled)
                        l_Franzok->AI()->DoAction(eAction::ActionIntro);
                }
            }
        }
};

void AddSC_blackrock_foundry()
{
    /// NPCs
    new npc_foundry_iron_flame_binder();
    new npc_foundry_living_flames();
    new npc_foundry_workshop_guardian();
    new npc_foundry_spinning_blade();
    new npc_foundry_ogron_hauler();
    new npc_foundry_ironworker();
    new npc_foundry_flame_jets();
    new npc_foundry_iron_slag_shaper();
    new npc_foundry_slagshop_worker();
    new npc_foundry_slagshop_brute();
    new npc_foundry_iron_journeyman();
    new npc_foundry_gronnling_laborer();
    new npc_foundry_karnor_the_cruel();
    new npc_foundry_iron_laborer_cosmetic();
    new npc_foundry_iron_laborer();
    new npc_foundry_darkshard_acidback();
    new npc_foundry_darkshard_gnasher();
    new npc_foundry_darkshard_crystalback();
    new npc_foundry_slag_behemoth();
    new npc_foundry_blackrock_enforcer();
    new npc_foundry_blackrock_forge_specialist();

    /// Spells
    new spell_foundry_grievous_mortal_wounds();
    new spell_foundry_spinning_blade();
    new spell_foundry_slag_breath();
    new spell_foundry_animate_slag();
    new spell_foundry_gronnling_smash();
    new spell_foundry_shattering_charge();
    new spell_foundry_ignite_aura();
    new spell_foundry_blast_wave();
    new spell_foundry_rending_slash();

    /// GameObjects

    /// AreaTriggers (spell)
    new areatrigger_foundry_acidback_puddle();
    new areatrigger_foundry_fire_bomb();

    /// AreaTriggers (world)
    new areatrigger_at_foundry_first_floor_trap();
    new areatrigger_at_foundry_second_floor_trap();
    new areatrigger_at_foundry_hansgar_and_franzok_entrance();
}