////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

Position const g_EncounterAddSpawns[2][3] =
{
    /// Left side
    {
        127.299f, 3496.977f, 246.085f, 0.7516f,
        127.599f, 3504.188f, 246.085f, 0.0054f,
        130.231f, 3515.183f, 245.085f, 0.0368f
    },
    /// Right side
    {
        265.278f, 3515.901f, 246.551f, 2.2870f,
        265.739f, 3520.793f, 246.551f, 3.0881f,
        265.634f, 3527.234f, 246.551f, 3.2569f
    }
};

Position const g_BellowsOperatorSpawns[2] =
{
    272.4757f, 3591.769f, 246.4359f, 3.071f,
    123.9306f, 3592.554f, 246.4359f, 0.106f
};

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
            BlastFurnaceBonus   = 177531,
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

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    CastSpellToPlayers(me->GetMap(), me, eSpells::BlastFurnaceBonus, true);
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
            /// Before fight
            SecurityGuard           = 88818,
            FurnaceEngineer         = 88820,
            BellowsOperator         = 88821,
            HeatRegulator           = 76808,
            /// During fight
            FurnaceEngineerFight    = 76810,
            BellowsOperatorFight    = 76811,
            SecurityGuardFight      = 76812
        };

        enum eAction
        {
            ActionActivateBellows
        };

        enum eEvents
        {
            EventBerserker = 1,
            EventFurnaceEngineer,
            EventSecurityGuard,
            EventBellowsOperator,
            EventPyroclasm,
            EventRupture
        };

        enum eSpells
        {
            Pyroclasm   = 156937,
            Rupture     = 156934,
            RuptureDoT  = 156932,
            HotBlooded  = 158247
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

                me->RemoveAllAreasTrigger();

                me->RemoveAura(eFoundrySpells::Berserker);

                me->CastSpell(me, eSpells::HotBlooded, true);
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
                m_Events.ScheduleEvent(eEvents::EventFurnaceEngineer, GetSummoningTimer());
                m_Events.ScheduleEvent(eEvents::EventSecurityGuard, GetSummoningTimer());
                m_Events.ScheduleEvent(eEvents::EventBellowsOperator, GetSummoningTimer());
                m_Events.ScheduleEvent(eEvents::EventPyroclasm, 18 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRupture, 26 * TimeConstants::IN_MILLISECONDS);
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

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RuptureDoT);
                }

                summons.DespawnAll();

                ResetFirstAdds();
            }

            void DoAction(int32 const p_Action) override
            {
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::Rupture:
                    {
                        Talk(eTalks::RuptureWarning, p_Target->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                        break;
                    }
                    default:
                        break;
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
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        break;
                    }
                    case eEvents::EventFurnaceEngineer:
                    {
                        for (uint8 l_I = 0; l_I < 2; ++l_I)
                        {
                            if (Creature* l_Engineer = me->SummonCreature(eCreatures::FurnaceEngineerFight, g_EncounterAddSpawns[l_I][urand(0, 2)]))
                            {
                                float l_O = l_Engineer->GetAngle(me);
                                float l_X = l_Engineer->GetPositionX() + 10.0f * cos(l_O);
                                float l_Y = l_Engineer->GetPositionY() + 10.0f * sin(l_O);

                                l_Engineer->GetMotionMaster()->MoveJump(l_X, l_Y, me->GetPositionZ(), 10.0f, 30.0f);
                            }
                        }

                        m_Events.ScheduleEvent(eEvents::EventFurnaceEngineer, GetSummoningTimer());
                        break;
                    }
                    case eEvents::EventSecurityGuard:
                    {
                        for (uint8 l_I = 0; l_I < 2; ++l_I)
                        {
                            if (Creature* l_Guard = me->SummonCreature(eCreatures::SecurityGuardFight, g_EncounterAddSpawns[l_I][urand(0, 2)]))
                            {
                                float l_O = l_Guard->GetAngle(me);
                                float l_X = l_Guard->GetPositionX() + 10.0f * cos(l_O);
                                float l_Y = l_Guard->GetPositionY() + 10.0f * sin(l_O);

                                l_Guard->GetMotionMaster()->MoveJump(l_X, l_Y, me->GetPositionZ(), 10.0f, 30.0f);
                            }
                        }

                        m_Events.ScheduleEvent(eEvents::EventSecurityGuard, GetSummoningTimer());
                        break;
                    }
                    case eEvents::EventBellowsOperator:
                    {
                        for (uint8 l_I = 0; l_I < 2; ++l_I)
                        {
                            if (Creature* l_Operator = me->SummonCreature(eCreatures::BellowsOperatorFight, g_BellowsOperatorSpawns[l_I]))
                            {
                                float l_O = l_Operator->GetAngle(me);
                                float l_X = l_Operator->GetPositionX() + 10.0f * cos(l_O);
                                float l_Y = l_Operator->GetPositionY() + 10.0f * sin(l_O);

                                l_Operator->GetMotionMaster()->MoveJump(l_X, l_Y, me->GetPositionZ(), 10.0f, 30.0f);
                            }
                        }

                        m_Events.ScheduleEvent(eEvents::EventBellowsOperator, GetSummoningTimer());
                        break;
                    }
                    case eEvents::EventPyroclasm:
                    {
                        me->CastSpell(me, eSpells::Pyroclasm, false);
                        m_Events.ScheduleEvent(eEvents::EventPyroclasm, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventRupture:
                    {
                        me->CastSpell(me, eSpells::Rupture, false);
                        m_Events.ScheduleEvent(eEvents::EventRupture, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ResetFirstAdds()
            {
                std::list<Creature*> l_CreatureList;

                uint32 const l_Entries[4] = { eCreatures::SecurityGuard, eCreatures::FurnaceEngineer, eCreatures::BellowsOperator, eCreatures::HeatRegulator };

                for (uint8 l_I = 0; l_I < 4; ++l_I)
                {
                    l_CreatureList.clear();

                    me->GetCreatureListWithEntryInGrid(l_CreatureList, l_Entries[l_I], 150.0f);

                    for (Creature* l_Iter : l_CreatureList)
                    {
                        if (l_Iter->isAlive() && l_Iter->IsAIEnabled)
                            l_Iter->AI()->EnterEvadeMode();
                        else
                        {
                            l_Iter->Respawn();
                            l_Iter->GetMotionMaster()->MoveTargetedHome();
                        }
                    }
                }
            }

            uint32 GetSummoningTimer() const
            {
                uint32 l_Timer = 0;

                if (IsLFR())
                    l_Timer = 65 * TimeConstants::IN_MILLISECONDS;
                else if (IsHeroic())
                    l_Timer = 55 * TimeConstants::IN_MILLISECONDS;
                else if (IsMythic())
                    l_Timer = 40 * TimeConstants::IN_MILLISECONDS;
                else
                    l_Timer = 60 * TimeConstants::IN_MILLISECONDS;

                return l_Timer;
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
            npc_foundry_bellows_operatorAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_SwitchStatePct(50) { }

            EventMap m_CosmeticEvent;

            int32 m_SwitchStatePct;

            void Reset() override
            {
                m_CosmeticEvent.Reset();

                me->RemoveAura(eSpells::Loading);

                me->SetReactState(ReactStates::REACT_PASSIVE);
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

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->GetReactState() == ReactStates::REACT_AGGRESSIVE)
                    return;

                /// In Mythic Difficulty, the Bellows Operators will stop operating the bellows of the furnace and engage players in melee combat after reaching 50% health remaining.
                if (me->HealthBelowPctDamaged(m_SwitchStatePct, p_Damage))
                {
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    AttackStart(p_Attacker);
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

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
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
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
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
                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);

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

/// Security Guard - 88818
class npc_foundry_security_guard : public CreatureScript
{
    public:
        npc_foundry_security_guard() : CreatureScript("npc_foundry_security_guard") { }

        enum eSpell
        {
            DefenseAura = 160379
        };

        struct npc_foundry_security_guardAI : public ScriptedAI
        {
            npc_foundry_security_guardAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_SwitchStatePct(50) { }

            int32 m_SwitchStatePct;

            void Reset() override
            {
                me->RemoveAura(eSpell::DefenseAura);

                me->SetCanDualWield(false);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HasAura(eSpell::DefenseAura))
                    return;

                /// In Mythic Difficulty, the Bellows Operators will stop operating the bellows of the furnace and engage players in melee combat after reaching 50% health remaining.
                if (me->HealthBelowPctDamaged(m_SwitchStatePct, p_Damage))
                    me->CastSpell(me, eSpell::DefenseAura, false);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_security_guardAI(p_Creature);
        }
};

/// Furnace Engineer - 88820
class npc_foundry_furnace_engineer : public CreatureScript
{
    public:
        npc_foundry_furnace_engineer() : CreatureScript("npc_foundry_furnace_engineer") { }

        enum eSpells
        {
            Electrocution   = 155201,
            Bomb            = 155192,
            DropLitBombs    = 174726,
            Repair          = 155179
        };

        enum eEvents
        {
            EventElectrocution = 1,
            EventBomb,
            EventRepair
        };

        enum eTalk
        {
            BombWarning = 8
        };

        enum eCreature
        {
            HeatRegulator = 76808
        };

        enum eMove
        {
            MoveRegulator = 1
        };

        struct npc_foundry_furnace_engineerAI : public ScriptedAI
        {
            npc_foundry_furnace_engineerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            bool m_IsInRepair;

            void Reset() override
            {
                m_Events.Reset();

                m_IsInRepair = false;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventElectrocution, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventBomb, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRepair, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::Bomb)
                {
                    if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                    {
                        if (Creature* l_Creature = Creature::GetCreature(*me, l_InstanceScript->GetData64(eFoundryCreatures::HeartOfTheMountain)))
                        {
                            if (l_Creature->IsAIEnabled)
                                l_Creature->AI()->Talk(eTalk::BombWarning, p_Target->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                        }
                    }
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                me->CastSpell(me, eSpells::DropLitBombs, true);
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                if (p_ID == eMove::MoveRegulator)
                    me->CastSpell(me, eSpells::Repair, false);
            }

            void OnSpellFinished(SpellInfo const* p_SpellInfo) override
            {
                if (p_SpellInfo->Id == eSpells::Repair)
                    m_IsInRepair = false;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim() || m_IsInRepair)
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventElectrocution:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::Electrocution, false);

                        m_Events.ScheduleEvent(eEvents::EventElectrocution, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventBomb:
                    {
                        me->CastSpell(me, eSpells::Bomb, false);
                        m_Events.ScheduleEvent(eEvents::EventBomb, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventRepair:
                    {
                        if (Unit* l_Regulator = me->FindNearestCreature(eCreature::HeatRegulator, 100.0f))
                        {
                            float l_O = l_Regulator->GetAngle(me);
                            float l_X = l_Regulator->GetPositionX() + 10.0f * cos(l_O);
                            float l_Y = l_Regulator->GetPositionY() + 10.0f * sin(l_O);

                            me->GetMotionMaster()->MovePoint(eMove::MoveRegulator, l_X, l_Y, me->GetPositionZ());

                            m_IsInRepair = true;
                        }

                        m_Events.ScheduleEvent(eEvents::EventRepair, 20 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_foundry_furnace_engineerAI(p_Creature);
        }
};

/// Cluster of Lit Bombs - 87339
class npc_foundry_cluster_of_lit_bombs : public CreatureScript
{
    public:
        npc_foundry_cluster_of_lit_bombs() : CreatureScript("npc_foundry_cluster_of_lit_bombs") { }

        enum eSpells
        {
            ClusterOfLitBombs   = 174731,
            BombAoEDespawn      = 155187,
            BombOverrider       = 174716
        };

        struct npc_foundry_cluster_of_lit_bombsAI : public ScriptedAI
        {
            npc_foundry_cluster_of_lit_bombsAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_MustExplode(true) { }

            bool m_MustExplode;

            void Reset() override
            {
                me->CastSpell(me, eSpells::ClusterOfLitBombs, true);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_NPC_FLAGS, NPCFlags::UNIT_NPC_FLAG_SPELLCLICK);

                me->SetUInt32Value(EUnitFields::UNIT_FIELD_INTERACT_SPELL_ID, eSpells::BombOverrider);
            }

            void OnSpellClick(Unit* p_Clicker) override
            {
                p_Clicker->CastSpell(p_Clicker, eSpells::BombOverrider, true);

                if (AuraPtr l_Aura = me->GetAura(eSpells::ClusterOfLitBombs))
                    l_Aura->DropCharge();

                if (!me->HasAura(eSpells::ClusterOfLitBombs))
                {
                    m_MustExplode = false;
                    me->DespawnOrUnsummon();
                }
            }

            void JustDespawned() override
            {
                if (m_MustExplode)
                    me->CastSpell(me, eSpells::BombAoEDespawn, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_cluster_of_lit_bombsAI(p_Creature);
        }
};

/// Defense - 160379
class spell_foundry_defense_aura : public SpellScriptLoader
{
    public:
        spell_foundry_defense_aura() : SpellScriptLoader("spell_foundry_defense_aura") { }

        class spell_foundry_defense_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_defense_aura_AuraScript);

            enum eSpells
            {
                DefenseAura = 160379,
                DefenseProc = 160382
            };

            uint32 m_DamageTimer;

            bool Load()
            {
                m_DamageTimer = 500;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_DamageTimer)
                {
                    if (m_DamageTimer <= p_Diff)
                    {
                        if (Unit* l_Caster = GetCaster())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 7.0f;

                            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                            l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                            for (Unit* l_Iter : l_TargetList)
                            {
                                if (!l_Iter->HasAura(eSpells::DefenseProc))
                                    l_Caster->CastSpell(l_Iter, eSpells::DefenseProc, true);
                            }
                        }

                        m_DamageTimer = 500;
                    }
                    else
                        m_DamageTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_foundry_defense_aura_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_defense_aura_AuraScript();
        }
};

/// Bomb (overrider) - 155192
/// Bomb (overrider - second) - 174716
class spell_foundry_bomb_overrider : public SpellScriptLoader
{
    public:
        spell_foundry_bomb_overrider() : SpellScriptLoader("spell_foundry_bomb_overrider") { }

        class spell_foundry_bomb_overrider_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_bomb_overrider_AuraScript);

            enum eSpell
            {
                BombAoE = 155187
            };

            void AfterRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(*l_Target, eSpell::BombAoE, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_foundry_bomb_overrider_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_bomb_overrider_AuraScript();
        }
};

/// Rupture - 156934
class spell_foundry_rupture_aura : public SpellScriptLoader
{
    public:
        spell_foundry_rupture_aura() : SpellScriptLoader("spell_foundry_rupture_aura") { }

        class spell_foundry_rupture_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_rupture_aura_AuraScript);

            enum eSpell
            {
                RuptureAreatrigger = 156933
            };

            void AfterRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(*l_Target, eSpell::RuptureAreatrigger, true);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_foundry_rupture_aura_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_rupture_aura_AuraScript();
        }
};

/// Hot Blooded - 158247
class spell_foundry_hot_blooded_aura : public SpellScriptLoader
{
    public:
        spell_foundry_hot_blooded_aura() : SpellScriptLoader("spell_foundry_hot_blooded_aura") { }

        class spell_foundry_hot_blooded_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_hot_blooded_aura_AuraScript);

            enum eSpells
            {
                HotBloodedDoT = 158246
            };

            uint32 m_DamageTimer;

            bool Load()
            {
                m_DamageTimer = 500;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_DamageTimer)
                {
                    if (m_DamageTimer <= p_Diff)
                    {
                        if (Unit* l_Caster = GetCaster())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 20.0f;

                            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                            l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                            for (Unit* l_Iter : l_TargetList)
                                l_Caster->CastSpell(l_Iter, eSpells::HotBloodedDoT, true);
                        }

                        m_DamageTimer = 500;
                    }
                    else
                        m_DamageTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_foundry_hot_blooded_aura_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_hot_blooded_aura_AuraScript();
        }
};

/// Rupture - 156933
class areatrigger_foundry_rupture : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_rupture() : AreaTriggerEntityScript("areatrigger_foundry_rupture") { }

        enum eSpell
        {
            RuptureDoT = 156932
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 6.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Unit->GetDistance(p_AreaTrigger) <= 2.5f)
                    {
                        if (!l_Unit->HasAura(eSpell::RuptureDoT))
                            l_Caster->CastSpell(l_Unit, eSpell::RuptureDoT, true);
                    }
                    else if (!l_Unit->FindNearestAreaTrigger(p_AreaTrigger->GetSpellId(), 2.5f))
                    {
                        if (l_Unit->HasAura(eSpell::RuptureDoT))
                            l_Unit->RemoveAura(eSpell::RuptureDoT);
                    }
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 2.5f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (!l_Unit->FindNearestAreaTrigger(p_AreaTrigger->GetSpellId(), l_Radius))
                    {
                        if (l_Unit->HasAura(eSpell::RuptureDoT))
                            l_Unit->RemoveAura(eSpell::RuptureDoT);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_rupture();
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
    new npc_foundry_security_guard();
    new npc_foundry_furnace_engineer();
    new npc_foundry_cluster_of_lit_bombs();

    /// Spells
    new spell_foundry_defense_aura();
    new spell_foundry_bomb_overrider();
    new spell_foundry_rupture_aura();
    new spell_foundry_hot_blooded_aura();

    /// AreaTriggers
    new areatrigger_foundry_rupture();
}