////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "boss_operator_thogar.hpp"

/// Operator Thogar <Grimrail Overseer> - 76906
class boss_operator_thogar : public CreatureScript
{
    public:
        boss_operator_thogar() : CreatureScript("boss_operator_thogar") { }

        enum eEvents
        {
            EventBerserker = 1
        };

        enum eTimers
        {
            TimerBerserker = 8 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS
        };

        enum eGuidTypes
        {
            IntroAdd,
            IntroRemove
        };

        enum eIntroStates
        {
            IronRaiders,
            GunnerySergeants
        };

        struct boss_operator_thogarAI : public BossAI
        {
            boss_operator_thogarAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataOperatorThogar)
            {
                m_Instance      = p_Creature->GetInstanceScript();
                m_IntroDone     = false;
                m_IntroState    = eIntroStates::IronRaiders;
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            uint8 m_IntroState;
            bool m_IntroDone;
            std::set<uint64> m_IntroTrashes;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    m_IntroTrashes.clear();

                    std::list<Creature*> l_AddsList;
                    me->GetCreatureListWithEntryInGrid(l_AddsList, eFoundryCreatures::GromkarManAtArms, 110.0f);

                    uint8 l_Count = 0;
                    for (Creature* l_Add : l_AddsList)
                    {
                        if (!l_Add->isAlive())
                            continue;

                        ++l_Count;
                    }

                    m_IntroDone = l_Count == 0;
                });

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (!m_IntroDone)
                    {
                    }
                });
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
                switch (p_ID)
                {
                    case eGuidTypes::IntroAdd:
                    {
                        m_IntroTrashes.insert(p_Guid);
                        break;
                    }
                    case eGuidTypes::IntroRemove:
                    {
                        m_IntroTrashes.erase(p_Guid);

                        /// Intro finished!
                        if (m_IntroTrashes.empty() && m_Instance != nullptr)
                        {
                            m_IntroDone = true;

                            if (Creature* l_Wheels = me->FindNearestCreature(eThogarCreatures::TrainWheels, 100.0f))
                            {
                                if (l_Wheels->IsAIEnabled)
                                    l_Wheels->AI()->DoAction(eThogarActions::TrainMoveEnd);
                            }

                            AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                SummonIntroSiegeTrain(me, eThogarMiscDatas::FourthTrack);
                            });
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                Talk(eThogarTalks::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventBerserker, eTimers::TimerBerserker);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                Talk(eThogarTalks::TalkDeath);
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                Talk(eThogarTalks::TalkWipe);
            }

            void DoAction(int32 const /*p_Action*/) override
            {
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
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        Talk(eThogarTalks::TalkBerserk);
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
            return new boss_operator_thogarAI(p_Creature);
        }
};

/// Train - 76907
class npc_foundry_train_controller : public CreatureScript
{
    public:
        npc_foundry_train_controller() : CreatureScript("npc_foundry_train_controller") { }

        enum eMoves
        {
            MovementIntro = 5,
            MovementOuttro
        };

        struct npc_foundry_train_controllerAI : public ScriptedAI
        {
            npc_foundry_train_controllerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_Vehicle   = p_Creature->GetVehicleKit();
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            uint64 m_SummonerGUID;

            void Reset() override
            {
                m_SummonerGUID = 0;

                me->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
            }

            void DoAction(int32 const p_Action) override
            {
                if (m_Vehicle == nullptr || m_Instance == nullptr)
                    return;

                switch (p_Action)
                {
                    /// Intro: Part1 - Move intro train
                    case eThogarActions::IntroBegin:
                    /// Intro: Part2 - Move intro train
                    case eThogarActions::IntroBeginPart2:
                    {
                        for (int8 l_I = 0; l_I < 4; ++l_I)
                        {
                            if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                            {
                                l_Passenger->RemoveAura(eThogarSpells::StoppedFrontAura);

                                l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingTrain, true);
                                l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingFrontAura, true);
                            }
                        }

                        me->GetMotionMaster()->MovePoint(eMoves::MovementIntro, p_Action == eThogarActions::IntroBegin ? g_TrainTrackIntroEndPos : g_TrainTrackIntroSiegeEndPos, false);

                        if (GameObject* l_RightDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[eThogarMiscDatas::FourthTrack].RightDoor)))
                            l_RightDoor->SetGoState(GOState::GO_STATE_ACTIVE);

                        if (GameObject* l_LeftDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[eThogarMiscDatas::FourthTrack].LeftDoor)))
                            l_LeftDoor->SetGoState(GOState::GO_STATE_ACTIVE);

                        break;
                    }
                    /// Intro: Part1 - Exit remaining passengers
                    case eThogarActions::IntroEnd:
                    {
                        if (Creature* l_Summoner = Creature::GetCreature(*me, m_SummonerGUID))
                            RemovePassengers(l_Summoner);

                        break;
                    }
                    /// Intro: Part1 - Move to end of the track
                    case eThogarActions::TrainMoveEnd:
                    {
                        for (int8 l_I = 0; l_I < 4; ++l_I)
                        {
                            if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                            {
                                /// Troop Transport visual
                                if (l_I == 1 || l_I == 3)
                                    l_Passenger->RemoveAura(eThogarSpells::TroopTransportClosed);

                                uint64 l_Guid = l_Passenger->GetGUID();
                                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                                {
                                    if (Unit* l_Passenger = Unit::GetUnit(*me, l_Guid))
                                    {
                                        l_Passenger->RemoveAura(eThogarSpells::StoppedFrontAura);

                                        l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingTrain, true);
                                        l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingFrontAura, true);
                                    }
                                });
                            }
                        }

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->GetMotionMaster()->MovePoint(eMoves::MovementOuttro, g_TrainTrackEndPos[eThogarMiscDatas::FourthTrack]);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
                m_SummonerGUID = p_Guid;
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (m_Vehicle == nullptr && m_Instance == nullptr)
                    return;

                switch (p_ID)
                {
                    case eMoves::MovementIntro:
                    {
                        for (int8 l_I = 0; l_I < 4; ++l_I)
                        {
                            if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                            {
                                l_Passenger->RemoveAura(eThogarSpells::MovingFrontAura);
                                l_Passenger->RemoveAura(eThogarSpells::MovingTrain);

                                if (AreaTrigger* l_AreaTrigger = l_Passenger->GetAreaTrigger(eThogarSpells::MovingTrain))
                                    l_AreaTrigger->SetDuration(1);

                                l_Passenger->CastSpell(l_Passenger, eThogarSpells::StoppedFrontAura, true);
                            }
                        }

                        if (Creature* l_Summoner = Creature::GetCreature(*me, m_SummonerGUID))
                            RemovePassengers(l_Summoner);

                        break;
                    }
                    case eMoves::MovementOuttro:
                    {
                        /// Remove Engine
                        if (Unit* l_Engine = m_Vehicle->GetPassenger(0))
                        {
                            l_Engine->RemoveAllAreasTrigger();
                            l_Engine->ToCreature()->DespawnOrUnsummon();
                        }

                        /// Remove troop transports
                        if (Unit* l_Transport = m_Vehicle->GetPassenger(1))
                        {
                            l_Transport->RemoveAllAreasTrigger();
                            l_Transport->ToCreature()->DespawnOrUnsummon();
                        }

                        if (Unit* l_Transport = m_Vehicle->GetPassenger(3))
                        {
                            l_Transport->RemoveAllAreasTrigger();
                            l_Transport->ToCreature()->DespawnOrUnsummon();
                        }

                        if (Unit* l_SlagTank = m_Vehicle->GetPassenger(2))
                        {
                            if (Vehicle* l_Vehicle = l_SlagTank->GetVehicleKit())
                            {
                                for (int8 l_I = 0; l_I < 8; ++l_I)
                                {
                                    if (!l_Vehicle->GetPassenger(l_I) || l_Vehicle->GetPassenger(l_I)->GetTypeId() != TypeID::TYPEID_UNIT)
                                        continue;

                                    l_Vehicle->GetPassenger(l_I)->ToCreature()->DespawnOrUnsummon();
                                }
                            }

                            l_SlagTank->RemoveAllAreasTrigger();
                            l_SlagTank->ToCreature()->DespawnOrUnsummon();
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void RemovePassengers(Creature* p_Source)
            {
                bool l_IsLeft = p_Source->IsNearPosition(&g_GromkarManAtArmsIntroLeftPos, 5.0f);

                if (Unit* l_TroopTransport = m_Vehicle->GetPassenger(l_IsLeft ? 1 : 3))
                {
                    l_TroopTransport->CastSpell(l_TroopTransport, eThogarSpells::TroopTransportOpen, true);

                    if (Vehicle* l_Transport = l_TroopTransport->GetVehicleKit())
                    {
                        for (int8 l_I = 0; l_I < (int8)eThogarMiscDatas::MaxIronRaiders; ++l_I)
                        {
                            if (!l_Transport->GetPassenger(l_I) || l_Transport->GetPassenger(l_I)->GetTypeId() != TypeID::TYPEID_UNIT)
                                continue;

                            if (Creature* l_Passenger = l_Transport->GetPassenger(l_I)->ToCreature())
                            {
                                l_Passenger->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                l_Passenger->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

                                l_Passenger->_ExitVehicle(l_IsLeft ? &g_IronRaiderLeftExitPos[l_I] : &g_IronRaiderRightExitPos[l_I]);

                                if (Unit* l_Victim = l_Passenger->SelectNearestPlayerNotGM(30.0f))
                                {
                                    if (l_Passenger->IsAIEnabled)
                                        l_Passenger->AI()->AttackStart(l_Victim);
                                }
                            }
                        }
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_train_controllerAI(p_Creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_operator_thogar()
{
    /// Boss
    new boss_operator_thogar();

    /// Creatures
    new npc_foundry_train_controller();
}
#endif
