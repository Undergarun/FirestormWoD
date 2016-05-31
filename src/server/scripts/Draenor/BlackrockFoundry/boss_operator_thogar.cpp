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

        enum eIntroStates : uint8
        {
            IronRaiders,
            GunnerySergeants,
            IntroEnd
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
                        if (!m_IntroTrashes.empty() || m_Instance == nullptr)
                            break;

                        switch (m_IntroState)
                        {
                            case eIntroStates::IronRaiders:
                            {
                                m_IntroState = eIntroStates::GunnerySergeants;

                                if (Creature* l_Wheels = me->FindNearestCreature(eThogarCreatures::TrainWheels, 100.0f))
                                {
                                    if (l_Wheels->IsAIEnabled)
                                        l_Wheels->AI()->DoAction(eThogarActions::TrainMoveEnd);
                                }

                                AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                                {
                                    SummonIntroSiegeTrain(me, eThogarMiscDatas::FourthTrack);
                                });

                                break;
                            }
                            case eIntroStates::GunnerySergeants:
                            {
                                m_IntroState = eIntroStates::IntroEnd;

                                if (Creature* l_Wheels = me->FindNearestCreature(eThogarCreatures::TrainWheels, 100.0f))
                                {
                                    if (l_Wheels->IsAIEnabled)
                                        l_Wheels->AI()->DoAction(eThogarActions::TrainMoveEndPart2);
                                }

                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void JustSummoned(Creature* p_Summon) override
            {
                if (m_IntroState == eIntroStates::GunnerySergeants && !m_IntroDone)
                {
                    if (p_Summon->GetEntry() == eThogarCreatures::IronGunnerySergeant)
                        m_IntroTrashes.insert(p_Summon->GetGUID());
                }
                else
                    CreatureAI::JustSummoned(p_Summon);
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
            MovementOuttro,
            SecondMovementIntro,
            SecondMovementOuttro
        };

        struct npc_foundry_train_controllerAI : public ScriptedAI
        {
            npc_foundry_train_controllerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_Vehicle   = p_Creature->GetVehicleKit();
                m_TrackID   = eThogarMiscDatas::FirstTrack;
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            uint64 m_SummonerGUID;

            uint8 m_TrackID;

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

                        me->GetMotionMaster()->MovePoint(p_Action == eThogarActions::IntroBegin ? eMoves::MovementIntro : eMoves::SecondMovementIntro, p_Action == eThogarActions::IntroBegin ? g_TrainTrackIntroEndPos : g_TrainTrackIntroSiegeEndPos, false);

                        if (GameObject* l_RightDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[m_TrackID].RightDoor)))
                            l_RightDoor->SetGoState(GOState::GO_STATE_ACTIVE);

                        if (GameObject* l_LeftDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[m_TrackID].LeftDoor)))
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
                                    l_Passenger->CastSpell(l_Passenger, eThogarSpells::TroopTransportClosed, true);

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
                    /// Intro: Part2 - Move to end of the track
                    case eThogarActions::TrainMoveEndPart2:
                    {
                        for (int8 l_I = 0; l_I < 4; ++l_I)
                        {
                            if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                            {
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

                        AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (GameObject* l_RightDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[m_TrackID].RightDoor)))
                                l_RightDoor->SetGoState(GOState::GO_STATE_READY);

                            if (GameObject* l_LeftDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[m_TrackID].LeftDoor)))
                                l_LeftDoor->SetGoState(GOState::GO_STATE_READY);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void SetGUID(uint64 p_Guid, int32 /*p_ID*/) override
            {
                m_SummonerGUID = p_Guid;
            }

            void SetData(uint32 /*p_ID*/, uint32 p_Value) override
            {
                m_TrackID = p_Value;
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID) override
            {
                if (m_Vehicle == nullptr && m_Instance == nullptr)
                    return;

                switch (p_ID)
                {
                    case eMoves::MovementIntro:
                    {
                        if (GameObject* l_RightDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[m_TrackID].RightDoor)))
                            l_RightDoor->SetGoState(GOState::GO_STATE_READY);

                        if (GameObject* l_LeftDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[m_TrackID].LeftDoor)))
                            l_LeftDoor->SetGoState(GOState::GO_STATE_READY);

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
                    case eMoves::SecondMovementIntro:
                    {
                        if (GameObject* l_RightDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[m_TrackID].RightDoor)))
                            l_RightDoor->SetGoState(GOState::GO_STATE_READY);

                        /// Update visuals
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

                        for (int8 l_I = 1; l_I < 3; ++l_I)
                        {
                            Unit* l_Passenger = m_Vehicle->GetPassenger(l_I);
                            if (l_Passenger == nullptr)
                                continue;

                            if (Vehicle* l_Train = l_Passenger->GetVehicleKit())
                            {
                                l_Passenger = l_Train->GetPassenger(0);
                                if (l_Passenger == nullptr)
                                    continue;

                                if (Vehicle* l_SiegeEngine = l_Passenger->GetVehicleKit())
                                {
                                    l_Passenger = l_SiegeEngine->GetPassenger(0);
                                    if (l_Passenger == nullptr)
                                        continue;

                                    if (Creature* l_Sergeant = l_Passenger->ToCreature())
                                    {
                                        l_Sergeant->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                        l_Sergeant->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

                                        if (Player* l_Target = l_Sergeant->SelectNearestPlayerNotGM(60.0f))
                                        {
                                            if (l_Sergeant->IsAIEnabled)
                                                l_Sergeant->AI()->AttackStart(l_Target);
                                        }
                                    }
                                }

                                l_Passenger = l_Train->GetPassenger(1);
                                if (l_Passenger == nullptr)
                                    continue;

                                l_Passenger->ExitVehicle();

                                uint64 l_Guid = l_Passenger->GetGUID();
                                AddTimedDelayedOperation(100, [this, l_Guid]() -> void
                                {
                                    if (Creature* l_ManAtArms = Creature::GetCreature(*me, l_Guid))
                                    {
                                        l_ManAtArms->GetMotionMaster()->MoveJump(g_ManAtArmsExitPos, 30.0f, 20.0f);

                                        l_ManAtArms->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                        l_ManAtArms->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                                    }
                                });

                                AddTimedDelayedOperation(500, [this, l_Guid]() -> void
                                {
                                    if (Creature* l_ManAtArms = Creature::GetCreature(*me, l_Guid))
                                    {
                                        if (Player* l_Target = l_ManAtArms->SelectNearestPlayerNotGM(30.0f))
                                        {
                                            if (l_ManAtArms->IsAIEnabled)
                                                l_ManAtArms->AI()->AttackStart(l_Target);
                                        }
                                    }
                                });
                            }
                        }

                        break;
                    }
                    case eMoves::SecondMovementOuttro:
                    {
                        /// Remove Engine
                        if (Unit* l_Engine = m_Vehicle->GetPassenger(0))
                        {
                            l_Engine->RemoveAllAreasTrigger();
                            l_Engine->ToCreature()->DespawnOrUnsummon();
                        }

                        /// Remove siege engine trains
                        for (int8 l_I = 1; l_I < 3; ++l_I)
                        {
                            if (Unit* l_Train = m_Vehicle->GetPassenger(l_I))
                            {
                                if (Vehicle* l_Vehicle = l_Train->GetVehicleKit())
                                {
                                    if (Unit* l_SiegeEngine = l_Vehicle->GetPassenger(0))
                                    {
                                        if (Vehicle* l_VehicleSec = l_SiegeEngine->GetVehicleKit())
                                        {
                                            if (Unit* l_Sergeant = l_VehicleSec->GetPassenger(0))
                                                l_Sergeant->ToCreature()->DespawnOrUnsummon();
                                        }

                                        l_SiegeEngine->ToCreature()->DespawnOrUnsummon();
                                    }
                                }

                                l_Train->RemoveAllAreasTrigger();
                                l_Train->ToCreature()->DespawnOrUnsummon();
                            }
                        }

                        /// Remove Supplies transport
                        if (Unit* l_Supplies = m_Vehicle->GetPassenger(3))
                        {
                            l_Supplies->RemoveAllAreasTrigger();
                            l_Supplies->ToCreature()->DespawnOrUnsummon();
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void RemovePassengers(Creature* p_Source)
            {
                Position const l_SourcePos = p_Source->GetHomePosition();
                bool l_IsLeft = l_SourcePos.IsNearPosition(&g_GromkarManAtArmsIntroLeftPos, 5.0f);

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

                                l_Passenger->ExitVehicle();

                                uint64 l_Guid = l_Passenger->GetGUID();
                                AddTimedDelayedOperation(100, [this, l_Guid, l_IsLeft, l_I]() -> void
                                {
                                    if (Creature* l_Passenger = Creature::GetCreature(*me, l_Guid))
                                        l_Passenger->GetMotionMaster()->MoveJump(l_IsLeft ? g_IronRaiderLeftExitPos[l_I] : g_IronRaiderRightExitPos[l_I], 30.0f, 10.0f);
                                });

                                AddTimedDelayedOperation(500, [this, l_Guid]() -> void
                                {
                                    if (Creature* l_Passenger = Creature::GetCreature(*me, l_Guid))
                                    {
                                        if (Unit* l_Victim = l_Passenger->SelectNearestPlayerNotGM(30.0f))
                                        {
                                            if (l_Passenger->IsAIEnabled)
                                                l_Passenger->AI()->AttackStart(l_Victim);
                                        }
                                    }
                                });
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

/// Iron Gunnery Sergeant - 81318
class npc_foundry_iron_gunnery_sergeant : public CreatureScript
{
    public:
        npc_foundry_iron_gunnery_sergeant() : CreatureScript("npc_foundry_iron_gunnery_sergeant") { }

        enum eSpells
        {
            DelayedSiegeBombSearcher    = 159480,
            DelayedSiegeBombChannel     = 159481,
            DelayedSiegeBombAoE         = 158084,
            DelayedSiegeBombVisual      = 156489,
            DelayedSiegeBombMissile     = 162286
        };

        enum eEvent
        {
            EventDelayedSiegeBomb = 1
        };

        enum eVisual
        {
            SiegeEngineVisual = 42258
        };

        struct npc_foundry_iron_gunnery_sergeantAI : public ScriptedAI
        {
            npc_foundry_iron_gunnery_sergeantAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            bool CanTargetOutOfLOS() override
            {
                return true;
            }

            bool CanBeTargetedOutOfLOS() override
            {
                return true;
            }

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventDelayedSiegeBomb, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                    {
                        if (l_Thogar->IsAIEnabled)
                            l_Thogar->AI()->SetGUID(me->GetGUID(), 1);
                    }
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
            {
                if (p_Damage >= me->GetHealth())
                {
                    if (Vehicle* l_SiegeEngine = me->GetVehicle())
                    {
                        if (l_SiegeEngine->GetBase()->GetTypeId() == TypeID::TYPEID_UNIT)
                            l_SiegeEngine->GetBase()->ToCreature()->DespawnOrUnsummon();
                    }
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::DelayedSiegeBombSearcher)
                {
                    if (Unit* l_SiegeEngine = me->GetVehicleBase())
                    {
                        l_SiegeEngine->SetFacingTo(me->GetAngle(p_Target));
                        l_SiegeEngine->CastSpell(p_Target, eSpells::DelayedSiegeBombVisual, false);
                        l_SiegeEngine->SendPlaySpellVisualKit(eVisual::SiegeEngineVisual, 0);
                    }

                    me->CastSpell(p_Target, eSpells::DelayedSiegeBombChannel, false);
                }
            }

            void SpellHitDest(SpellDestination const* p_Dest, SpellInfo const* p_SpellInfo) override
            {
                if (p_Dest == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::DelayedSiegeBombAoE)
                {
                    if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                        l_Thogar->CastSpell(p_Dest, eSpells::DelayedSiegeBombMissile, true);
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
                    case eEvent::EventDelayedSiegeBomb:
                    {
                        me->CastSpell(me, eSpells::DelayedSiegeBombSearcher, true);
                        m_Events.ScheduleEvent(eEvent::EventDelayedSiegeBomb, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_iron_gunnery_sergeantAI(p_Creature);
        }
};

/// Delayed Siege Bomb (periodic) - 159481
class spell_foundry_delayed_siege_bomb_periodic : public SpellScriptLoader
{
    public:
        spell_foundry_delayed_siege_bomb_periodic() : SpellScriptLoader("spell_foundry_delayed_siege_bomb_periodic") { }

        class spell_foundry_delayed_siege_bomb_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_delayed_siege_bomb_periodic_AuraScript)

            enum eSpell
            {
                DelayedSiegeBombMissile = 159482
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Unit* l_Caster = GetCaster())
                        l_Caster->CastSpell(l_Target, eSpell::DelayedSiegeBombMissile, true);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundry_delayed_siege_bomb_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_delayed_siege_bomb_periodic_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_operator_thogar()
{
    /// Boss
    new boss_operator_thogar();

    /// Creatures
    new npc_foundry_train_controller();
    new npc_foundry_iron_gunnery_sergeant();

    /// Spells
    new spell_foundry_delayed_siege_bomb_periodic();
}
#endif
