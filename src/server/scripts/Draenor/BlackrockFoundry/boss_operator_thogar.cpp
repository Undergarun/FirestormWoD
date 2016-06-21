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

        enum eSpells
        {
            /// Enkindle
            SpellEnkindle                       = 155921,
            /// Prototype Pulse Grenade
            SpellPrototypePulseGrenade          = 155864,
            SpellPrototypePulseGrenadeMissile   = 165190,
            SpellPrototypePulseGrenadeAT        = 165194,
            /// Berated
            SpellBeratingPeriodic               = 156274,
            /// Misc
            SpellThogarBonus                    = 177537
        };

        enum eEvents
        {
            EventBerserker = 1,
            EventEnkindle,
            EventPrototypePulseGrenade,
            EventSummonTrain
        };

        enum eTimers
        {
            TimerBerserker      = 8 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS,
            TimerEnkindle       = 16 * TimeConstants::IN_MILLISECONDS,
            TimerPulseGrenade   = 7 * TimeConstants::IN_MILLISECONDS,
            TimerPulseGrenadeCD = 12 * TimeConstants::IN_MILLISECONDS
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

        enum eMove
        {
            MoveJump = 5
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

            uint8 m_TrainID;

            std::array<bool, eThogarMiscDatas::MaxTrainTracks> m_AvailableLines;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);

                me->RemoveAura(eSpells::SpellBeratingPeriodic);

                if (!m_IntroDone)
                {
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
                        if (m_IntroDone)
                        {
                            m_IntroState = eIntroStates::IntroEnd;

                            /// Must be done for train spawning
                            me->GetMap()->SetObjectVisibility(500.0f);

                            DoAction(eThogarActions::IntroEnd);

                            if (GameObject* l_IronGate = GameObject::GetGameObject(*me, m_Instance->GetData64(eFoundryGameObjects::IronGate)))
                                l_IronGate->SetGoState(GOState::GO_STATE_ACTIVE);
                        }
                    });
                }

                m_TrainID = eThogarTrains::FightTrainBeginning;

                m_AvailableLines = { true, true, true, true };

                if (m_Instance != nullptr)
                {
                    for (uint32 l_Entry = eFoundryGameObjects::MassiveDoorTrack4Right; l_Entry != eFoundryGameObjects::MassiveDoorTrack1Left; ++l_Entry)
                    {
                        if (GameObject* l_Door = GameObject::GetGameObject(*me, m_Instance->GetData64(l_Entry)))
                            l_Door->SetGoState(GOState::GO_STATE_READY);
                    }
                }
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
                        if (m_IntroTrashes.find(p_Guid) == m_IntroTrashes.end())
                            break;

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
                                    SummonTrain(me, eThogarTrains::IntroSiegeTrain, eThogarActions::IntroBeginPart2, false);
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

                                AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                                {
                                    SummonTrain(me, eThogarTrains::IntroWoodTrain, eThogarActions::IntroBeginPart3, false);
                                });

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

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
                    case eThogarMiscDatas::FirstTrack:
                    case eThogarMiscDatas::SecondTrack:
                    case eThogarMiscDatas::ThirdTrack:
                    case eThogarMiscDatas::FourthTrack:
                        m_AvailableLines[p_ID] = p_Value != 0;
                        break;
                    default:
                        break;
                }
            }

            void JustSummoned(Creature* p_Summon) override
            {
                if (m_IntroState == eIntroStates::GunnerySergeants && !m_IntroDone)
                {
                    if (p_Summon->GetEntry() == eThogarCreatures::IronGunnerySergeant ||
                        p_Summon->GetEntry() == eThogarCreatures::ManAtArmsIntro)
                        m_IntroTrashes.insert(p_Summon->GetGUID());
                }
                else
                    BossAI::JustSummoned(p_Summon);
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

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                me->CastSpell(me, eSpells::SpellBeratingPeriodic, true);

                m_Events.ScheduleEvent(eEvents::EventBerserker, eTimers::TimerBerserker);
                m_Events.ScheduleEvent(eEvents::EventEnkindle, eTimers::TimerEnkindle);
                m_Events.ScheduleEvent(eEvents::EventPrototypePulseGrenade, eTimers::TimerPulseGrenade);
                m_Events.ScheduleEvent(eEvents::EventSummonTrain, g_TrainDatas[m_TrainID].SpawnTimer);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                Talk(eThogarTalks::TalkDeath);

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SpellEnkindle);

                    /// Allow loots and bonus loots to be enabled/disabled with a simple reload
                    if (sObjectMgr->IsDisabledEncounter(m_Instance->GetEncounterIDForBoss(me), GetDifficulty()))
                        me->SetLootRecipient(nullptr);
                    else
                        CastSpellToPlayers(me->GetMap(), me, eSpells::SpellThogarBonus, true);
                }
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                Talk(eThogarTalks::TalkWipe);

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eFoundryDatas::DataOperatorThogar, EncounterState::FAIL);

                    for (uint32 l_Entry = eFoundryGameObjects::MassiveDoorTrack4Right; l_Entry <= eFoundryGameObjects::MassiveDoorTrack1Left; ++l_Entry)
                    {
                        if (GameObject* l_Door = GameObject::GetGameObject(*me, m_Instance->GetData64(l_Entry)))
                            l_Door->SetGoState(GOState::GO_STATE_READY);
                    }

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SpellEnkindle);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type == MovementGeneratorType::EFFECT_MOTION_TYPE && p_ID == eMove::MoveJump)
                {
                    m_IntroDone = true;

                    me->SetHomePosition(g_ThogarJumpPos);

                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eThogarActions::IntroEnd:
                    {
                        me->GetMotionMaster()->MoveJump(g_ThogarJumpPos, 30.0f, 10.0f, eMove::MoveJump);
                        break;
                    }
                    default:
                        break;
                }
            }

            void SpellHitDest(SpellDestination const* p_Dest, SpellInfo const* p_SpellInfo) override
            {
                if (p_Dest == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::SpellPrototypePulseGrenadeMissile)
                    me->CastSpell(p_Dest, eSpells::SpellPrototypePulseGrenadeAT, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                if (me->GetDistance(me->GetHomePosition()) >= 90.0f)
                {
                    EnterEvadeMode();
                    return;
                }

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
                    case eEvents::EventEnkindle:
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellEnkindle, false);

                        m_Events.ScheduleEvent(eEvents::EventEnkindle, eTimers::TimerEnkindle);
                        break;
                    }
                    case eEvents::EventPrototypePulseGrenade:
                    {
                        if (Player* l_Target = SelectRangedTarget(true))
                            me->CastSpell(l_Target, eSpells::SpellPrototypePulseGrenade, true);

                        m_Events.ScheduleEvent(eEvents::EventPrototypePulseGrenade, eTimers::TimerPulseGrenadeCD);
                        break;
                    }
                    case eEvents::EventSummonTrain:
                    {
                        /// Spawn train only if track is available
                        if (m_AvailableLines[g_TrainDatas[m_TrainID].TrackID])
                            SummonTrain(me, m_TrainID, eThogarActions::ActionNone);

                        ++m_TrainID;

                        /// Few timers are set to 0, because of double spawns
                        uint32 l_Timer = g_TrainDatas[m_TrainID].SpawnTimer;
                        if (!l_Timer)
                            m_Events.ScheduleEvent(eEvents::EventSummonTrain, 0);
                        else
                        {
                            uint32 l_TrainID = m_TrainID;
                            uint32 l_PrevTimer = 0;
                            do
                            {
                                l_PrevTimer = g_TrainDatas[--l_TrainID].SpawnTimer;
                            }
                            while (l_TrainID >= eThogarTrains::FightTrainBeginning && l_PrevTimer == 0);

                            m_Events.ScheduleEvent(eEvents::EventSummonTrain, l_Timer - l_PrevTimer);
                        }

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
            SecondMovementOuttro,
            ThirdMovementIntro,
            ThirdMovementOuttro,
            MovementInFight
        };

        enum eVisual
        {
            PreMovingTrain = 39795
        };

        struct npc_foundry_train_controllerAI : public ScriptedAI
        {
            npc_foundry_train_controllerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_Vehicle   = p_Creature->GetVehicleKit();
                m_TrainID   = eThogarTrains::IntroTroopsTrain;
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            uint64 m_SummonerGUID;

            uint8 m_TrainID;

            std::set<uint64> m_Passengers;
            int32 m_CheckPassengersTime;

            std::set<uint64> m_CollisionBoxes;

            void Reset() override
            {
                m_SummonerGUID = 0;

                m_CheckPassengersTime = 0;

                me->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
            }

            void DoAction(int32 const p_Action) override
            {
                if (m_Vehicle == nullptr || m_Instance == nullptr)
                    return;

                switch (p_Action)
                {
                    /// Action none, just move by default
                    case eThogarActions::ActionNone:
                    {
                        TrainDatas l_Datas = g_TrainDatas[m_TrainID];

                        switch (l_Datas.TrainType)
                        {
                            case eThogarMiscDatas::NonAddTrain:
                            {
                                StartTrain(0, false);

                                me->GetMotionMaster()->MovePoint(eMoves::ThirdMovementOuttro, l_Datas.RightToLeft ? g_TrainTrackEndPos[l_Datas.TrackID] : g_TrainTrackSpawnPos[l_Datas.TrackID], false);

                                HandleDoors(true);
                                break;
                            }
                            case eThogarMiscDatas::AddsTrain:
                            case eThogarMiscDatas::BigAddsTrain:
                            case eThogarMiscDatas::SiegeTrain:
                            {
                                StartTrain(0, false);

                                Position l_Pos      = l_Datas.RightToLeft ? g_TrainTrackEndPos[l_Datas.TrackID] : g_TrainTrackSpawnPos[l_Datas.TrackID];
                                l_Pos.m_positionY   = g_InFightStopPosY;

                                me->GetMotionMaster()->MovePoint(eMoves::MovementInFight, l_Pos, false);

                                HandleDoors(true);
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    /// Intro: Part1 - Move intro train
                    case eThogarActions::IntroBegin:
                    /// Intro: Part2 - Move intro train
                    case eThogarActions::IntroBeginPart2:
                    {
                        StartTrain(0, false);

                        me->GetMotionMaster()->MovePoint(p_Action == eThogarActions::IntroBegin ? eMoves::MovementIntro : eMoves::SecondMovementIntro, p_Action == eThogarActions::IntroBegin ? g_TrainTrackIntroEndPos : g_TrainTrackIntroSiegeEndPos, false);

                        HandleDoors(true);
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
                        for (int8 l_I = 0; l_I < MAX_VEHICLE_SEATS; ++l_I)
                        {
                            if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                            {
                                /// Troop Transport visual
                                if (l_I == 1 || l_I == 3)
                                    l_Passenger->CastSpell(l_Passenger, eThogarSpells::TroopTransportClosed, true);

                                StartTrain(l_Passenger->GetGUID());
                            }
                        }

                        AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->GetMotionMaster()->MovePoint(eMoves::MovementOuttro, g_TrainTrackEndPos[g_TrainDatas[m_TrainID].TrackID]);
                        });

                        break;
                    }
                    /// Intro: Part2 - Move to end of the track
                    case eThogarActions::TrainMoveEndPart2:
                    {
                        StartTrain(0);

                        AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->GetMotionMaster()->MovePoint(eMoves::MovementOuttro, g_TrainTrackEndPos[g_TrainDatas[m_TrainID].TrackID]);
                        });

                        AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            HandleDoors(false);

                            if (GameObject* l_IronGate = GameObject::GetGameObject(*me, m_Instance->GetData64(eFoundryGameObjects::IronGate)))
                                l_IronGate->SetGoState(GOState::GO_STATE_ACTIVE);
                        });

                        break;
                    }
                    /// Intro: Part3 - Move wood train to boss
                    case eThogarActions::IntroBeginPart3:
                    {
                        StartTrain(0, false);

                        me->GetMotionMaster()->MovePoint(eMoves::ThirdMovementIntro, g_TrainTrackIntroWoodEndPos, false);

                        HandleDoors(true);
                        break;
                    }
                    case eThogarActions::TrainFightMoveEnd:
                    {
                        StartTrain(0);

                        AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            TrainDatas l_Datas = g_TrainDatas[m_TrainID];

                            /// Set track to available
                            if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                            {
                                if (l_Thogar->IsAIEnabled)
                                    l_Thogar->AI()->SetData(l_Datas.TrackID, true);
                            }

                            me->GetMotionMaster()->MovePoint(eMoves::ThirdMovementOuttro, l_Datas.RightToLeft ? g_TrainTrackEndPos[l_Datas.TrackID] : g_TrainTrackSpawnPos[l_Datas.TrackID], false);

                            for (uint64 l_Guid : m_CollisionBoxes)
                            {
                                if (GameObject* l_Box = GameObject::GetGameObject(*me, l_Guid))
                                    l_Box->Delete();
                            }
                        });

                        AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            HandleDoors(false);
                        });

                        break;
                    }
                    case eThogarActions::SiegeTrainMoveEnd:
                    {
                        AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            StartTrain(0);
                        });

                        AddTimedDelayedOperation(9 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            TrainDatas l_Datas = g_TrainDatas[m_TrainID];

                            /// Set track to available
                            if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                            {
                                if (l_Thogar->IsAIEnabled)
                                    l_Thogar->AI()->SetData(l_Datas.TrackID, true);
                            }

                            me->GetMotionMaster()->MovePoint(eMoves::ThirdMovementOuttro, l_Datas.RightToLeft ? g_TrainTrackEndPos[l_Datas.TrackID] : g_TrainTrackSpawnPos[l_Datas.TrackID], false);

                            for (uint64 l_Guid : m_CollisionBoxes)
                            {
                                if (GameObject* l_Box = GameObject::GetGameObject(*me, l_Guid))
                                    l_Box->Delete();
                            }
                        });

                        AddTimedDelayedOperation(12 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            HandleDoors(false);
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

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                m_TrainID = p_Value;
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID) override
            {
                if (m_Vehicle == nullptr && m_Instance == nullptr)
                    return;

                switch (p_ID)
                {
                    case eMoves::MovementIntro:
                    {
                        StopTrain();

                        if (Creature* l_Summoner = Creature::GetCreature(*me, m_SummonerGUID))
                            RemovePassengers(l_Summoner);

                        break;
                    }
                    case eMoves::MovementOuttro:
                    case eMoves::SecondMovementOuttro:
                    case eMoves::ThirdMovementOuttro:
                    {
                        if (p_ID != eMoves::MovementOuttro)
                            HandleDoors(false);

                        DespawnTrain();
                        break;
                    }
                    case eMoves::SecondMovementIntro:
                    {
                        StopTrain();

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
                                        l_ManAtArms->SetHomePosition(g_ManAtArmsExitPos);
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
                    case eMoves::ThirdMovementIntro:
                    {
                        StopTrain();

                        AddTimedDelayedOperation(10 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance != nullptr)
                            {
                                if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                                {
                                    if (l_Thogar->IsAIEnabled)
                                        l_Thogar->AI()->Talk(eThogarTalks::TalkIntro);
                                }
                            }
                        });

                        StartTrain(0, true, 20 * TimeConstants::IN_MILLISECONDS);

                        AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance != nullptr)
                            {
                                if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                                {
                                    /// Launch jump into room
                                    if (l_Thogar->IsAIEnabled)
                                        l_Thogar->AI()->DoAction(eThogarActions::IntroEnd);
                                }
                            }
                        });

                        AddTimedDelayedOperation(22 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            /// Launch move
                            me->GetMotionMaster()->MovePoint(eMoves::ThirdMovementOuttro, g_TrainTrackEndPos[g_TrainDatas[m_TrainID].TrackID]);

                            HandleDoors(true);
                        });

                        break;
                    }
                    case eMoves::MovementInFight:
                    {
                        TrainDatas l_Datas = g_TrainDatas[m_TrainID];

                        StopTrain();

                        /// Set track to unavailable
                        if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                        {
                            if (l_Thogar->IsAIEnabled)
                                l_Thogar->AI()->SetData(l_Datas.TrackID, false);
                        }

                        switch (l_Datas.TrainType)
                        {
                            case eThogarMiscDatas::AddsTrain:
                            case eThogarMiscDatas::BigAddsTrain:
                            {
                                for (int8 l_I = 1; l_I <= 2; ++l_I)
                                {
                                    if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                                    {
                                        if (Vehicle* l_Vehicle = l_Passenger->GetVehicleKit())
                                        {
                                            for (int8 l_J = 0; l_J < MAX_VEHICLE_SEATS; ++l_J)
                                            {
                                                if (l_Vehicle->GetPassenger(l_J) == nullptr)
                                                    continue;

                                                if (Creature* l_Add = l_Vehicle->GetPassenger(l_J)->ToCreature())
                                                {
                                                    l_Add->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                                    l_Add->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

                                                    l_Add->ExitVehicle();

                                                    uint64 l_Guid = l_Add->GetGUID();

                                                    if (l_Datas.TrainType == eThogarMiscDatas::AddsTrain)
                                                    {
                                                        m_Passengers.insert(l_Guid);

                                                        m_CheckPassengersTime = 1 * TimeConstants::IN_MILLISECONDS;
                                                    }

                                                    l_Add->NearTeleportTo(*me);

                                                    AddTimedDelayedOperation(100, [this, l_Guid]() -> void
                                                    {
                                                        if (Creature* l_Passenger = Creature::GetCreature(*me, l_Guid))
                                                        {
                                                            if (Creature* l_Thogar = Creature::GetCreature(*me, m_SummonerGUID))
                                                            {
                                                                if (l_Thogar->IsAIEnabled)
                                                                {
                                                                    if (Player* l_Tank = urand(0, 1) ? l_Thogar->AI()->SelectMainTank() : l_Thogar->AI()->SelectOffTank())
                                                                        l_Passenger->GetMotionMaster()->MoveJump(*l_Tank, 30.0f, 10.0f);
                                                                }
                                                            }
                                                        }
                                                    });

                                                    AddTimedDelayedOperation(500, [this, l_Guid]() -> void
                                                    {
                                                        if (Creature* l_Passenger = Creature::GetCreature(*me, l_Guid))
                                                        {
                                                            if (Unit* l_Victim = l_Passenger->SelectNearestPlayerNotGM(60.0f))
                                                            {
                                                                if (l_Passenger->IsAIEnabled)
                                                                    l_Passenger->AI()->AttackStart(l_Victim);
                                                            }
                                                        }
                                                    });
                                                }
                                            }

                                            if (l_Datas.TrainType == eThogarMiscDatas::BigAddsTrain)
                                                DoAction(eThogarActions::SiegeTrainMoveEnd);
                                        }
                                    }
                                }

                                break;
                            }
                            case eThogarMiscDatas::SiegeTrain:
                            {
                                /// Gunner wagon
                                if (Unit* l_Passenger = m_Vehicle->GetPassenger(2))
                                {
                                    if (l_Passenger->GetVehicleKit() == nullptr)
                                        break;

                                    l_Passenger = l_Passenger->GetVehicleKit()->GetPassenger(0);

                                    if (Vehicle* l_SiegeEngine = l_Passenger->GetVehicleKit())
                                    {
                                        l_Passenger = l_SiegeEngine->GetPassenger(0);
                                        if (l_Passenger == nullptr)
                                            break;

                                        if (Creature* l_Sergeant = l_Passenger->ToCreature())
                                        {
                                            l_Sergeant->ExitVehicle();

                                            uint64 l_VehicleGuid = l_SiegeEngine->GetBase()->GetGUID();
                                            uint64 l_PassengerGuid = l_Sergeant->GetGUID();
                                            AddTimedDelayedOperation(100, [this, l_VehicleGuid, l_PassengerGuid]() -> void
                                            {
                                                if (Creature* l_Vehicle = Creature::GetCreature(*me, l_VehicleGuid))
                                                {
                                                    if (Creature* l_Sergeant = Creature::GetCreature(*me, l_PassengerGuid))
                                                    {
                                                        l_Sergeant->NearTeleportTo(*l_Vehicle);
                                                        l_Sergeant->EnterVehicle(l_Vehicle);

                                                        l_Sergeant->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                                        l_Sergeant->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

                                                        if (Player* l_Target = l_Sergeant->SelectNearestPlayerNotGM(60.0f))
                                                        {
                                                            if (l_Sergeant->IsAIEnabled)
                                                                l_Sergeant->AI()->AttackStart(l_Target);
                                                        }
                                                    }
                                                }
                                            });
                                        }
                                    }
                                }

                                break;
                            }
                            default:
                                break;
                        }

                        /// Spawn collision boxes
                        for (int8 l_I = 0; l_I < MAX_VEHICLE_SEATS; ++l_I)
                        {
                            Unit* l_Passenger = m_Vehicle->GetPassenger(l_I);
                            if (l_Passenger == nullptr)
                                continue;

                            uint32 l_BoxEntry   = 0;
                            float l_LessVal     = 0.0f;
                            uint8 l_Iterations  = 0;
                            switch (l_I)
                            {
                                case 0: ///< Always the engine
                                {
                                    l_BoxEntry      = eThogarGameObjects::EngineCollisionBox;
                                    l_LessVal       = 20.0f;
                                    l_Iterations    = 5;
                                    break;
                                }
                                default:
                                {
                                    l_BoxEntry      = urand(0, 1) ? eThogarGameObjects::TrainAndCarCollisionBox1 : eThogarGameObjects::TrainAndCarCollisionBox2;
                                    l_LessVal       = 10.0f;
                                    l_Iterations    = 3;
                                    break;
                                }
                            }

                            Position l_Pos      = *l_Passenger;
                            l_Pos.m_positionY   += l_LessVal;

                            for (uint8 l_I = 0; l_I < l_Iterations; ++l_I)
                            {
                                if (GameObject* l_Box = me->SummonGameObject(l_BoxEntry, l_Pos, 0.0f, 0.0f, 0.7071066f, 0.7071069f, 0))
                                {
                                    l_Box->SetRotationQuat(0.0f, 0.0f, 0.7071066f, 0.7071069f);
                                    l_Box->SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 0, 0.0f);
                                    l_Box->SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 1, 0.0f);
                                    l_Box->SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 2, 0.0f);
                                    l_Box->SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 3, 1.0f);
                                    m_CollisionBoxes.insert(l_Box->GetGUID());
                                }

                                l_Pos.m_positionY -= 10.0f;
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (m_CheckPassengersTime)
                {
                    if (m_CheckPassengersTime <= (int32)p_Diff)
                    {
                        for (auto l_Iter = m_Passengers.begin(); l_Iter != m_Passengers.end();)
                        {
                            if (Unit* l_Passenger = Unit::GetUnit(*me, *l_Iter))
                            {
                                if (!l_Passenger->isAlive())
                                {
                                    l_Iter = m_Passengers.erase(l_Iter);
                                    continue;
                                }
                                else
                                    ++l_Iter;
                            }
                        }

                        if (m_Passengers.empty())
                        {
                            DoAction(eThogarActions::TrainFightMoveEnd);
                            m_CheckPassengersTime = 0;
                        }
                        else
                            m_CheckPassengersTime = 1 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_CheckPassengersTime -= p_Diff;
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
                                    {
                                        Position l_Pos = l_IsLeft ? g_IronRaiderLeftExitPos[l_I] : g_IronRaiderRightExitPos[l_I];

                                        l_Passenger->GetMotionMaster()->MoveJump(l_Pos, 30.0f, 10.0f);
                                        l_Passenger->SetHomePosition(l_Pos);
                                    }
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

            void DespawnPassenger(Vehicle* p_Vehicle)
            {
                for (int8 l_I = 0; l_I < MAX_VEHICLE_SEATS; ++l_I)
                {
                    if (Unit* l_Passenger = p_Vehicle->GetPassenger(l_I))
                    {
                        if (Vehicle* l_Vehicle = l_Passenger->GetVehicleKit())
                            DespawnPassenger(l_Vehicle);

                        l_Passenger->ToCreature()->DespawnOrUnsummon();
                    }
                }
            }

            void DespawnTrain()
            {
                if (m_Vehicle == nullptr)
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                DespawnPassenger(m_Vehicle);
            }

            /// Handle visuals for starting
            void StartTrain(uint64 p_Guid, bool p_Delayed = true, uint32 p_Delay = 1 * TimeConstants::IN_MILLISECONDS)
            {
                if (p_Guid)
                {
                    AddTimedDelayedOperation(p_Delayed ? p_Delay : 0, [this, p_Guid]() -> void
                    {
                        if (Unit* l_Passenger = Unit::GetUnit(*me, p_Guid))
                            l_Passenger->SendPlaySpellVisualKit(eVisual::PreMovingTrain, 0);
                    });

                    p_Delay += 2 * TimeConstants::IN_MILLISECONDS;

                    AddTimedDelayedOperation(p_Delayed ? p_Delay : 0, [this, p_Guid]() -> void
                    {
                        if (Unit* l_Passenger = Unit::GetUnit(*me, p_Guid))
                        {
                            l_Passenger->RemoveAura(eThogarSpells::StoppedFrontAura);

                            l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingTrain, true);
                            l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingFrontAura, true);
                        }
                    });
                }
                else
                {
                    for (int8 l_I = 0; l_I < MAX_VEHICLE_SEATS; ++l_I)
                    {
                        if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                            StartTrain(l_Passenger->GetGUID(), p_Delayed, p_Delay);
                    }
                }
            }

            /// Handle visuals for stopping
            void StopTrain()
            {
                /// Update visuals
                for (int8 l_I = 0; l_I < MAX_VEHICLE_SEATS; ++l_I)
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
            }

            void HandleDoors(bool p_Apply)
            {
                if (GameObject* l_RightDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[g_TrainDatas[m_TrainID].TrackID].RightDoor)))
                    l_RightDoor->SetGoState(p_Apply ? GOState::GO_STATE_ACTIVE : GOState::GO_STATE_READY);

                if (GameObject* l_LeftDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[g_TrainDatas[m_TrainID].TrackID].LeftDoor)))
                    l_LeftDoor->SetGoState(p_Apply ? GOState::GO_STATE_ACTIVE : GOState::GO_STATE_READY);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_train_controllerAI(p_Creature);
        }
};

/// Iron Gunnery Sergeant (Intro) - 81318
/// Iron Gunnery Sergeant (Fight) - 78981
class npc_foundry_iron_gunnery_sergeant : public CreatureScript
{
    public:
        npc_foundry_iron_gunnery_sergeant() : CreatureScript("npc_foundry_iron_gunnery_sergeant") { }

        enum eSpells
        {
            DelayedSiegeBombSearcher    = 159480,
            DelayedSiegeBombChannel     = 159481,
            DelayedSiegeBombVisual      = 156489
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
                    if (!m_Instance->IsEncounterInProgress())
                    {
                        if (Creature* l_Thogar = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossOperatorThogar)))
                        {
                            if (l_Thogar->IsAIEnabled)
                                l_Thogar->AI()->SetGUID(me->GetGUID(), 1);
                        }
                    }
                }

                me->DespawnOrUnsummon(20 * TimeConstants::IN_MILLISECONDS);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
            {
                if (p_Damage >= me->GetHealth())
                {
                    if (m_Instance != nullptr && m_Instance->IsEncounterInProgress())
                    {
                        if (Unit* l_Base = me->GetVehicleBase())
                        {
                            while (l_Base->GetVehicleBase() != nullptr)
                                l_Base = l_Base->GetVehicleBase();

                            if (Creature* l_Wheels = l_Base->ToCreature())
                            {
                                if (l_Wheels->IsAIEnabled)
                                    l_Wheels->AI()->DoAction(eThogarActions::TrainFightMoveEnd);
                            }
                        }
                    }

                    if (Vehicle* l_SiegeEngine = me->GetVehicle())
                    {
                        if (l_SiegeEngine->GetBase()->GetTypeId() == TypeID::TYPEID_UNIT)
                            l_SiegeEngine->GetBase()->ToCreature()->DespawnOrUnsummon();
                    }

                    me->GetMotionMaster()->MoveFall();
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
                        l_SiegeEngine->SendPlaySpellVisualKit(eVisual::SiegeEngineVisual, 0);
                        l_SiegeEngine->CastSpell(p_Target, eSpells::DelayedSiegeBombChannel, false);
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
                    case eEvent::EventDelayedSiegeBomb:
                    {
                        me->CastSpell(me, eSpells::DelayedSiegeBombSearcher, true);
                        m_Events.ScheduleEvent(eEvent::EventDelayedSiegeBomb, 11 * TimeConstants::IN_MILLISECONDS);
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

/// Siege Engine - 78982
/// Siege Engine - 81316
class npc_foundry_siege_engine : public CreatureScript
{
    public:
        npc_foundry_siege_engine() : CreatureScript("npc_foundry_siege_engine") { }

        enum eSpells
        {
            DelayedSiegeBombAoE     = 158084,
            DelayedSiegeBombMissile = 162286
        };

        struct npc_foundry_siege_engineAI : public ScriptedAI
        {
            npc_foundry_siege_engineAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                uint64 l_Guid = p_Summoner->GetGUID();
                AddTimedDelayedOperation(50, [this, l_Guid]() -> void
                {
                    if (Unit* l_Summoner = Creature::GetCreature(*me, l_Guid))
                    {
                        bool l_IsIntro = me->GetEntry() == eThogarCreatures::SiegeEngine1;
                        if (Creature* l_Sergeant = l_Summoner->SummonCreature(l_IsIntro ? eThogarCreatures::IronGunnerySergeant : eThogarCreatures::ThogarSergeant, *me))
                        {
                            ApplyPassengerFlags(l_Sergeant, false);

                            uint64 l_SergeantGuid = l_Sergeant->GetGUID();
                            AddTimedDelayedOperation(300, [this, l_SergeantGuid]() -> void
                            {
                                if (Unit* l_Sergeant = Creature::GetCreature(*me, l_SergeantGuid))
                                {
                                    l_Sergeant->NearTeleportTo(*me);
                                    l_Sergeant->EnterVehicle(me, 0);
                                }
                            });
                        }
                    }
                });
            }

            void SpellHitDest(SpellDestination const* p_Dest, SpellInfo const* p_SpellInfo) override
            {
                if (p_Dest == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::DelayedSiegeBombAoE)
                {
                    if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                    {
                        if (Creature* l_Thogar = Creature::GetCreature(*me, l_InstanceScript->GetData64(eFoundryCreatures::BossOperatorThogar)))
                            l_Thogar->CastSpell(p_Dest, eSpells::DelayedSiegeBombMissile, true);
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
            return new npc_foundry_siege_engineAI(p_Creature);
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

/// Berated - 156281
class spell_foundry_berated : public SpellScriptLoader
{
    public:
        spell_foundry_berated() : SpellScriptLoader("spell_foundry_berated") { }

        class spell_foundry_berated_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_berated_SpellScript)

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (p_Object->IsPlayer() || p_Object->ToUnit()->isCharmedOwnedByPlayerOrPlayer())
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_berated_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_berated_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_berated_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_berated_SpellScript::CorrectTargets, EFFECT_3, TARGET_UNIT_SRC_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_berated_SpellScript::CorrectTargets, EFFECT_4, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_berated_SpellScript();
        }
};

/// Moving Train - 156553
class areatrigger_foundry_moving_train : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_moving_train() : AreaTriggerEntityScript("areatrigger_foundry_moving_train") { }

        enum eSpells
        {
            MovingTrainDamage   = 156554,
            DodgedAMovingTrain  = 160350
        };

        std::set<uint64> m_AffectedPlayers;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            InstanceScript* l_InstanceScript = p_AreaTrigger->GetInstanceScript();
            if (l_InstanceScript == nullptr || !l_InstanceScript->IsEncounterInProgress())
                return;

            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                Creature* l_Thogar = Creature::GetCreature(*l_Caster, l_InstanceScript->GetData64(eFoundryCreatures::BossOperatorThogar));
                if (l_Thogar == nullptr)
                    return;

                std::list<Unit*> l_TargetList;
                float l_Radius = 15.0f;

                JadeCore::AnyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                float l_CheckX = 14.0f;
                float l_CheckY = 10.0f;

                for (Unit* l_Iter : l_TargetList)
                {
                    if (!l_Iter->IsPlayer())
                        continue;

                    if (m_AffectedPlayers.find(l_Iter->GetGUID()) != m_AffectedPlayers.end())
                        continue;

                    if (l_Iter->GetPositionX() <= (p_AreaTrigger->GetPositionX() + l_CheckX) && l_Iter->GetPositionX() >= (p_AreaTrigger->GetPositionX() - l_CheckX) &&
                        l_Iter->GetPositionY() <= (p_AreaTrigger->GetPositionY() + l_CheckY) && l_Iter->GetPositionY() >= (p_AreaTrigger->GetPositionY() - l_CheckY))
                    {
                        l_Iter->CastSpell(l_Iter, eSpells::MovingTrainDamage, true, nullptr, nullptr, l_Thogar->GetGUID());

                        Position l_Pos;

                        float l_Orientation = p_AreaTrigger->GetAngle(l_Iter);
                        l_Pos.m_positionX   = l_Iter->m_positionX + 10.0f * cos(l_Orientation);
                        l_Pos.m_positionY   = l_Iter->m_positionY + 10.0f * sin(l_Orientation);
                        l_Pos.m_positionZ   = g_CenterPos.m_positionZ;
                        l_Pos.m_orientation = l_Orientation;

                        l_Iter->CastSpell(l_Pos, eSpells::DodgedAMovingTrain, true);

                        m_AffectedPlayers.insert(l_Iter->GetGUID());
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_moving_train();
        }
};

/// Prototype Pulse Grenade - 165194
class areatrigger_foundry_prototype_pulse_grenade : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_prototype_pulse_grenade() : AreaTriggerEntityScript("areatrigger_foundry_prototype_pulse_grenade") { }

        enum eSpell
        {
            PrototypePulseGrenadeDoT = 165195
        };

        std::set<uint64> m_AffectedPlayers;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 2.5f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                std::set<uint64> l_Targets;

                if (!l_TargetList.empty())
                {
                    l_TargetList.remove_if([this](Unit* p_Unit) -> bool
                    {
                        if (!p_Unit->IsPlayer() && !p_Unit->ToUnit()->isCharmedOwnedByPlayerOrPlayer())
                            return true;

                        return false;
                    });
                }

                for (Unit* l_Iter : l_TargetList)
                {
                    l_Targets.insert(l_Iter->GetGUID());

                    if (!l_Iter->HasAura(eSpell::PrototypePulseGrenadeDoT, l_Caster->GetGUID()))
                    {
                        m_AffectedPlayers.insert(l_Iter->GetGUID());
                        l_Caster->CastSpell(l_Iter, eSpell::PrototypePulseGrenadeDoT, true);

                        if (l_Iter->IsPlayer())
                            l_Iter->ToPlayer()->SendApplyMovementForce(p_AreaTrigger->GetGUID(), true, *p_AreaTrigger, 0.7f, 1);
                    }
                }

                for (std::set<uint64>::iterator l_Iter = m_AffectedPlayers.begin(); l_Iter != m_AffectedPlayers.end();)
                {
                    if (l_Targets.find((*l_Iter)) != l_Targets.end())
                    {
                        ++l_Iter;
                        continue;
                    }

                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, (*l_Iter)))
                    {
                        l_Iter = m_AffectedPlayers.erase(l_Iter);
                        l_Unit->RemoveAura(eSpell::PrototypePulseGrenadeDoT, l_Caster->GetGUID());

                        if (l_Unit->IsPlayer())
                            l_Unit->ToPlayer()->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, Position());

                        continue;
                    }

                    ++l_Iter;
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                for (uint64 l_Guid : m_AffectedPlayers)
                {
                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, l_Guid))
                    {
                        l_Unit->RemoveAura(eSpell::PrototypePulseGrenadeDoT, l_Caster->GetGUID());

                        if (l_Unit->IsPlayer())
                            l_Unit->ToPlayer()->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, Position());
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_prototype_pulse_grenade();
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
    new npc_foundry_siege_engine();

    /// Spells
    new spell_foundry_delayed_siege_bomb_periodic();
    new spell_foundry_berated();

    /// AreaTriggers
    new areatrigger_foundry_moving_train();
    new areatrigger_foundry_prototype_pulse_grenade();
}
#endif
