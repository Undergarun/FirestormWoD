////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

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

        enum eSpell
        {
            PunctureWound = 175987
        };

        enum eEvent
        {
            EventPunctureWound = 1
        };

        enum eTalk
        {
            TalkCosmetic
        };

        struct npc_foundry_slagshop_workerAI : public ScriptedAI
        {
            npc_foundry_slagshop_workerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(urand(1 * TimeConstants::IN_MILLISECONDS, 60 * TimeConstants::IN_MILLISECONDS), [this]() -> void
                {
                    Talk(eTalk::TalkCosmetic);
                });
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                ClearDelayedOperations();

                me->SetUInt32Value(EUnitFields::UNIT_FIELD_ANIM_TIER, 0);

                m_Events.ScheduleEvent(eEvent::EventPunctureWound, 5 * TimeConstants::IN_MILLISECONDS);
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
                    case eEvent::EventPunctureWound:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpell::PunctureWound, true);

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
                AddTimedDelayedOperation(urand(1 * TimeConstants::IN_MILLISECONDS, 60 * TimeConstants::IN_MILLISECONDS), [this]() -> void
                {
                    Talk(eTalk::TalkCosmetic);
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
            EarthAttack     = 165318
        };

        enum eEvent
        {
            EventGronnlingSmash = 1
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

                m_Events.ScheduleEvent(eEvent::EventGronnlingSmash, 10 * TimeConstants::IN_MILLISECONDS);
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
                    case eEvent::EventGronnlingSmash:
                    {
                        me->CastSpell(me, eSpells::GronnlingSmash, false);
                        m_Events.ScheduleEvent(eEvent::EventGronnlingSmash, 10 * TimeConstants::IN_MILLISECONDS);
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

                        m_CheckTimer = 500;
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
                    l_Target->CastSpell(l_Target, eSpell::GronnlingSmashAoE, true);
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

    /// Spells
    new spell_foundry_grievous_mortal_wounds();
    new spell_foundry_spinning_blade();
    new spell_foundry_slag_breath();
    new spell_foundry_animate_slag();
    new spell_foundry_gronnling_smash();

    /// GameObjects

    /// AreaTriggers
}