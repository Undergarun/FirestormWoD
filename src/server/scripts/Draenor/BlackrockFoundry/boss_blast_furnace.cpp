////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Heart of the Mountain - 76806
class boss_heart_of_the_mountain : public CreatureScript
{
    public:
        boss_heart_of_the_mountain() : CreatureScript("boss_heart_of_the_mountain") { }

        enum eTalks
        {
            Phase3Freedom,
            HeatLevelRising,
            WarnBlast,
            Melt,
            Slay,
            Wipe,
            Death,
            BlastWarning,
            BombWarning,
            VolatileFireWarning,
            MeltWarning
        };

        enum eSpells
        {
            /// Misc
            HeartOfTheFurnace   = 155288,
            /// Encounter
            Blast               = 155209
        };

        enum eEvents
        {
            EventBerserker = 1,
            EventWarnBlast,
            EventBlast
        };

        struct boss_heart_of_the_mountainAI : public BossAI
        {
            boss_heart_of_the_mountainAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataBlastFurnace)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eSpells::HeartOfTheFurnace);
                me->RemoveAura(eFoundrySpells::Berserker);

                me->SetPower(Powers::POWER_ALTERNATE_POWER, 0);

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);
                }

                me->CastSpell(me, eSpells::HeartOfTheFurnace, true);

                m_Events.ScheduleEvent(eEvents::EventBerserker, 780 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventWarnBlast, 27 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventBlast, 30 * TimeConstants::IN_MILLISECONDS);
            }

            void EnterEvadeMode() override
            {
                Talk(eTalks::Wipe);

                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();
            }

            void DoAction(int32 const p_Action) override
            {
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
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        break;
                    }
                    case eEvents::EventWarnBlast:
                    {
                        Talk(eTalks::BlastWarning);
                        m_Events.ScheduleEvent(eEvents::EventWarnBlast, GetBlastTimer());
                        break;
                    }
                    case eEvents::EventBlast:
                    {
                        me->CastSpell(me, eSpells::Blast, true);
                        m_Events.ScheduleEvent(eEvents::EventBlast, GetBlastTimer());
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            uint32 GetBlastTimer() const
            {
                /// The time until the next cast of Blast is indicated by The Heart of the Mountain's Energy bar, with Blast being cast whenever it reaches maximum Energy.
                /// In addition to this, The Heart of the Mountain also has a Heat resource, which determines exactly how quickly its Energy regenerates.
                /// The longer The Heart of the Mountain spends inside the Blast Furnace (in other words, the longer your raid spends in Phase One), the higher its Heat will be.
                uint32 l_Time = 0;
                int32 l_Power = me->GetPower(Powers::POWER_ALTERNATE_POWER);

                if (l_Power >= 75)
                    l_Time = 5 * TimeConstants::IN_MILLISECONDS;
                else if (l_Power >= 50)
                    l_Time = 10 * TimeConstants::IN_MILLISECONDS;
                else if (l_Power >= 25)
                    l_Time = 15 * TimeConstants::IN_MILLISECONDS;
                else
                    l_Time = 25 * TimeConstants::IN_MILLISECONDS;

                return l_Time;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_heart_of_the_mountainAI(p_Creature);
        }
};

/// Foreman Feldspar - 76809
class boss_foreman_feldspar : public CreatureScript
{
    public:
        boss_foreman_feldspar() : CreatureScript("boss_foreman_feldspar") { }

        enum eTalks
        {
            Aggro,
            BellowsOperators,
            FirstHeatDestroyed,
            Ability,
            Slay,
            Wipe,
            Death,
            BlastIncreased,
            RuptureWarning
        };

        enum eCreatures
        {
            BellowsOperator = 88821,
            HeatRegulator   = 76808
        };

        enum eAction
        {
            ActionActivateBellows
        };

        enum eEvents
        {
            EventBerserker = 1
        };

        struct boss_foreman_feldsparAI : public BossAI
        {
            boss_foreman_feldsparAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataForemanFeldspar)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(eTalks::Slay);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalks::Aggro);

                std::list<Creature*> l_BellowsOperators;
                me->GetCreatureListWithEntryInGrid(l_BellowsOperators, eCreatures::BellowsOperator, 150.0f);

                for (Creature* l_Operator : l_BellowsOperators)
                {
                    if (l_Operator->IsAIEnabled)
                        l_Operator->AI()->DoAction(eAction::ActionActivateBellows);
                }

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 2);

                    if (Creature* l_HeartOfTheMountain = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::HeartOfTheMountain)))
                    {
                        if (l_HeartOfTheMountain->IsAIEnabled)
                            l_HeartOfTheMountain->AI()->AttackStart(p_Attacker);
                    }

                    std::list<Creature*> l_Regulators;
                    me->GetCreatureListWithEntryInGrid(l_Regulators, eCreatures::HeatRegulator, 150.0f);

                    for (Creature* l_Regulator : l_Regulators)
                    {
                        if (l_Regulator->IsAIEnabled)
                            l_Regulator->AI()->AttackStart(p_Attacker);
                    }
                }

                m_Events.ScheduleEvent(eEvents::EventBerserker, 780 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalks::Death);
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }

                std::list<Creature*> l_Regulators;
                me->GetCreatureListWithEntryInGrid(l_Regulators, eCreatures::HeatRegulator, 150.0f);

                for (Creature* l_Regulator : l_Regulators)
                {
                    if (l_Regulator->IsAIEnabled)
                        l_Regulator->AI()->EnterEvadeMode();
                }

                std::list<Creature*> l_BellowsOperators;
                me->GetCreatureListWithEntryInGrid(l_BellowsOperators, eCreatures::BellowsOperator, 150.0f);

                for (Creature* l_Operator : l_BellowsOperators)
                {
                    if (l_Operator->IsAIEnabled)
                        l_Operator->AI()->EnterEvadeMode();
                }
            }

            void DoAction(int32 const p_Action) override
            {
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
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
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
            return new boss_foreman_feldsparAI(p_Creature);
        }
};

/// Blackhand (Cosmetic) - 76831
class npc_foundry_blackhand_cosmetic : public CreatureScript
{
    public:
        npc_foundry_blackhand_cosmetic() : CreatureScript("npc_foundry_blackhand_cosmetic") { }

        enum eTalks
        {
            Intro,
            Begin,
            BellowsOperators,
            FirstHeatDestroyedBefore,
            FirstHeatDestroyedAfter,
            HeartExposedBefore,
            HeartExposedAfter,
            Phase2,
            ThreeElementalists,
            TwoElementalists,
            OneElementalist,
            Phase3Freedom1,
            Phase3Freedom2,
            HeartDeath
        };

        enum eAction
        {
            ActionIntro
        };

        struct npc_foundry_blackhand_cosmeticAI : public ScriptedAI
        {
            npc_foundry_blackhand_cosmeticAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionIntro:
                    {
                        Talk(eTalks::Intro);
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
            return new npc_foundry_blackhand_cosmeticAI(p_Creature);
        }
};

/// Primal Elementalist - 76815
class npc_foundry_primal_elementalist : public CreatureScript
{
    public:
        npc_foundry_primal_elementalist() : CreatureScript("npc_foundry_primal_elementalist") { }

        enum eTalks
        {
            HeartExposed1,
            HeartExposed2,
            HeartExposed3,
            Phase3Freedom
        };

        struct npc_foundry_primal_elementalistAI : public ScriptedAI
        {
            npc_foundry_primal_elementalistAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
            }

            void JustDied(Unit* p_Killer) override
            {
                if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                    l_InstanceScript->SetData(eFoundryDatas::PrimalElementalistTime, (uint32)time(nullptr));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_primal_elementalistAI(p_Creature);
        }
};

/// Bellows Operator - 88821
class npc_foundry_bellows_operator : public CreatureScript
{
    public:
        npc_foundry_bellows_operator() : CreatureScript("npc_foundry_bellows_operator") { }

        enum eSpells
        {
            Loading = 155181
        };

        enum eAction
        {
            ActionActivateBellows
        };

        enum eCosmeticEvent
        {
            EventActivateBellows = 1
        };

        struct npc_foundry_bellows_operatorAI : public ScriptedAI
        {
            npc_foundry_bellows_operatorAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_CosmeticEvent;

            void Reset() override
            {
                m_CosmeticEvent.Reset();

                me->RemoveAura(eSpells::Loading);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionActivateBellows:
                    {
                        m_CosmeticEvent.ScheduleEvent(eCosmeticEvent::EventActivateBellows, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvent.Update(p_Diff);

                switch (m_CosmeticEvent.ExecuteEvent())
                {
                    case eCosmeticEvent::EventActivateBellows:
                    {
                        AddTimedDelayedOperation(100 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->CastSpell(me, eSpells::Loading, false);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_bellows_operatorAI(p_Creature);
        }
};

/// Heat Regulator - 76808
class npc_foundry_heat_regulator : public CreatureScript
{
    public:
        npc_foundry_heat_regulator() : CreatureScript("npc_foundry_heat_regulator") { }

        struct npc_foundry_heat_regulatorAI : public ScriptedAI
        {
            npc_foundry_heat_regulatorAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 2);
                }
            }

            void EnterEvadeMode() override
            {
                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);

                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_heat_regulatorAI(p_Creature);
        }
};

void AddSC_boss_blast_furnace()
{
    /// Bosses
    new boss_heart_of_the_mountain();
    new boss_foreman_feldspar();

    /// Creatures
    new npc_foundry_blackhand_cosmetic();
    new npc_foundry_primal_elementalist();
    new npc_foundry_bellows_operator();
    new npc_foundry_heat_regulator();

    /// Spells

    /// AreaTriggers
}