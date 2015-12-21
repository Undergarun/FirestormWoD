////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

Position const g_StampingPressCosmeticPos[4] =
{
    { 143.168f, 3492.93f, 130.463f, 0.0f },
    { 123.540f, 3507.37f, 130.466f, 0.0f },
    { 123.540f, 3478.47f, 130.466f, 0.0f },
    { 103.832f, 3492.93f, 130.468f, 0.0f }
};

Position const g_SmartStampCollisionPos[4] =
{
    { 146.668f, 3507.37f, 130.116f, 0.0f },
    { 139.668f, 3507.37f, 130.116f, 0.0f },
    { 127.040f, 3463.96f, 130.116f, 0.0f },
    { 120.040f, 3463.96f, 130.116f, 0.0f }
};

Position const g_StampingPressIntroPos[2] =
{
    { 143.168f, 3507.37f, 130.463f, 0.0f },
    { 123.540f, 3478.47f, 130.466f, 0.0f }
};

Position const g_HansgarJumpPosIn = { 103.434f, 3517.066f, 130.1161f, 0.0f };
Position const g_FranzokJumpPosIn = { 102.6545f, 3465.035f, 130.2772f, 0.0f };

Position const g_HansgarJumpPosOut = { 88.03299f, 3467.769f, 138.4438f, 3.141593f };
Position const g_HansgarJumpPosRetreat = { 85.60069f, 3517.861f, 138.235f, 3.141593f };

float const g_HansgarRetreatOrientation = 5.94246f;

float const g_ScorchingBurnsXPos = 182.5f;

/// Cyclic patterns which always happens for normal/heroic mode
std::vector<uint32> const g_SearingPlatesCycles[5] =
{
    { eFoundryGameObjects::ConveyorBelt002, eFoundryGameObjects::ConveyorBelt003, eFoundryGameObjects::ConveyorBelt004, eFoundryGameObjects::ConveyorBelt005, eFoundryGameObjects::ConveyorBelt001 },
    { eFoundryGameObjects::ConveyorBelt001, eFoundryGameObjects::ConveyorBelt002, eFoundryGameObjects::ConveyorBelt003, eFoundryGameObjects::ConveyorBelt004, eFoundryGameObjects::ConveyorBelt005 },
    { eFoundryGameObjects::ConveyorBelt005, eFoundryGameObjects::ConveyorBelt001, eFoundryGameObjects::ConveyorBelt002, eFoundryGameObjects::ConveyorBelt003, eFoundryGameObjects::ConveyorBelt004 },
    { eFoundryGameObjects::ConveyorBelt004, eFoundryGameObjects::ConveyorBelt005, eFoundryGameObjects::ConveyorBelt001, eFoundryGameObjects::ConveyorBelt002, eFoundryGameObjects::ConveyorBelt003 },
    { eFoundryGameObjects::ConveyorBelt003, eFoundryGameObjects::ConveyorBelt004, eFoundryGameObjects::ConveyorBelt005, eFoundryGameObjects::ConveyorBelt001, eFoundryGameObjects::ConveyorBelt002 }
};

/// Forge Overdrive spawn pos for each Conveyor Belt
std::map<uint32, Position const> g_ForgeOverdriveSpawnPos =
{
    { eFoundryGameObjects::ConveyorBelt001, { 191.8142f, 3522.539f, 130.5489f, 3.196286f } },
    { eFoundryGameObjects::ConveyorBelt002, { 191.7986f, 3508.009f, 130.5486f, 3.147189f } },
    { eFoundryGameObjects::ConveyorBelt003, { 191.5920f, 3492.890f, 130.5486f, 3.139562f } },
    { eFoundryGameObjects::ConveyorBelt004, { 190.5399f, 3478.265f, 130.3344f, 3.141593f } },
    { eFoundryGameObjects::ConveyorBelt005, { 190.8819f, 3463.978f, 130.5486f, 3.140998f } }
};

/// Forge Overdrive dest pos for each Conveyor Belt
std::map<uint32, Position const> g_ForgeOverdriveDestPos =
{
    { eFoundryGameObjects::ConveyorBelt001, { 74.61285f, 3522.023f, 130.4776f, 3.196286f } },
    { eFoundryGameObjects::ConveyorBelt002, { 72.65278f, 3507.342f, 130.7546f, 3.147189f } },
    { eFoundryGameObjects::ConveyorBelt003, { 71.13541f, 3493.135f, 130.5421f, 3.139562f } },
    { eFoundryGameObjects::ConveyorBelt004, { 80.53993f, 3478.265f, 130.3344f, 3.141593f } },
    { eFoundryGameObjects::ConveyorBelt005, { 76.74827f, 3464.046f, 130.4773f, 3.140998f } }
};

/// Movement Force directions for each Conveyor Belt
std::map<uint32, Position const> g_ConveyorForceMovesDirection =
{
    { eFoundryGameObjects::ConveyorBelt001, { 136.2731f, 3521.869f, 130.4693f, 0.0f } },
    { eFoundryGameObjects::ConveyorBelt002, { 136.0504f, 3507.557f, 130.4693f, 0.0f } },
    { eFoundryGameObjects::ConveyorBelt003, { 132.4271f, 3492.807f, 130.4693f, 0.0f } },
    { eFoundryGameObjects::ConveyorBelt004, { 132.4292f, 3478.443f, 130.4693f, 0.0f } },
    { eFoundryGameObjects::ConveyorBelt005, { 131.7153f, 3464.024f, 130.4693f, 0.0f } }
};

struct StampingPressData
{
    StampingPressData()
    {
        StampingPresses = 0;
        StampingPress   = 0;
    }

    StampingPressData(uint64 p_Creature, uint64 p_GameObject)
    {
        StampingPresses = p_Creature;
        StampingPress   = p_GameObject;
    }

    uint64 StampingPresses; ///< Creature
    uint64 StampingPress;   ///< GameObject
};

typedef std::list<StampingPressData> StampingPressList;

void RespawnBrothers(Creature* p_Source, InstanceScript* p_Instance)
{
    if (p_Source == nullptr || p_Instance == nullptr)
        return;

    if (Creature* l_Other = Creature::GetCreature(*p_Source, (p_Source->GetEntry() == eFoundryCreatures::BossFranzok) ? eFoundryCreatures::BossHansgar : eFoundryCreatures::BossFranzok))
    {
        l_Other->Respawn();
        l_Other->GetMotionMaster()->MoveTargetedHome();
    }
}

void StartBrothers(Creature* p_Source, Unit* p_Target, InstanceScript* p_Instance)
{
    if (p_Source == nullptr || p_Target == nullptr || p_Instance == nullptr)
        return;

    uint32 l_Entry = (p_Source->GetEntry() == eFoundryCreatures::BossFranzok) ? eFoundryCreatures::BossHansgar : eFoundryCreatures::BossFranzok;
    if (Creature* l_Other = Creature::GetCreature(*p_Source, p_Instance->GetData64(l_Entry)))
    {
        if (l_Other->IsAIEnabled)
            l_Other->AI()->AttackStart(p_Target);
    }
}

/// This class is used to activate Stamping Press
class StampingPressActivation : public BasicEvent
{
    public:
        StampingPressActivation(uint64 p_Guid, uint32 p_AnimID, bool p_Maintain) : m_Guid(p_Guid), m_AnimID(p_AnimID), m_Maintain(p_Maintain), BasicEvent() { }
        virtual ~StampingPressActivation() { }

        virtual bool Execute(uint64 p_EndTime, uint32 p_Time)
        {
            if (GameObject* l_GameObject = HashMapHolder<GameObject>::Find(m_Guid))
            {
                l_GameObject->SendGameObjectActivateAnimKit(m_AnimID, m_Maintain);

                /// Register AnimKitID to send it in UpdateObject
                l_GameObject->SetAIAnimKitId(m_AnimID, false);
            }

            return true;
        }

        virtual void Abort(uint64 p_EndTime) { }

    private:
        uint64 m_Guid;
        uint32 m_AnimID;
        bool m_Maintain;
};

/// Hans'gar - 76973
class boss_hansgar : public CreatureScript
{
    public:
        boss_hansgar() : CreatureScript("boss_hansgar") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3,
            Aggro,
            CripplingSuplex,
            ActivateAssemblyLine,
            ReturningFromControls,
            Slay,
            Death,
            BodySlam
        };

        enum eSpells
        {
            /// Misc
            PumpedUp                = 155665,
            TacticalRetreat         = 156220,
            TacticalRetreatOther    = 156883,
            BoundByBlood            = 161029,
            ShatteredVertebrae      = 157139,
            AftershockDoT           = 157853,
            NotReady                = 158656,
            /// Body Slam
            JumpSlamSearcher        = 157922,
            JumpSlamCast            = 157923,
            BodySlamTriggered       = 154785,
            BodySlamRedArrowAura    = 156892,
            BodySlamOut             = 155747,
            /// Crippling Suplex
            CripplingSuplexScript   = 156546,   ///< This triggers 156547
            CripplingSuplexJump     = 156547,   ///< Force player to enter boss
            CripplingSuplexSwitch   = 156609
        };

        enum eEvents
        {
            EventBodySlam = 1
        };

        enum eCosmeticEvents
        {
            EventCosmeticStampingPresses = 1
        };

        enum eActions
        {
            ActionIntro,
            ActionIntroFinished,
            ActionCripplingSuplex,
            ActionSearingPlate
        };

        enum eVisuals
        {
            AnimStamp1      = 5924,
            AnimStamp2      = 6741,
            AnimStamp3      = 5836,
            BodySlamVisual  = 38379,
            ChainPullVisual = 6006
        };

        enum eCreatures
        {
            MobStampingPresses          = 78358,
            BlackrockForgeSpecialist    = 79200,
            BlackrockEnforcer           = 79208,
            ScorchingBurns              = 78823
        };

        enum eGameObjects
        {
            StampingPress16     = 229574,
            StampingPress20     = 229575,
            StampingPress19     = 229576,
            StampingPress18     = 229577,
            StampingPress17     = 229578,
            StampingPress15     = 229579,
            StampingPress14     = 229580,
            StampingPress13     = 229581,
            StampingPress12     = 229582,
            StampingPress11     = 229583,
            StampingPress06     = 229584,
            StampingPress07     = 229585,
            StampingPress08     = 229586,
            StampingPress09     = 229587,
            StampingPress10     = 229588,
            StampingPress05     = 229589,
            StampingPress04     = 229590,
            StampingPress01     = 229591,
            StampingPress02     = 229592,
            StampingPress03     = 229593,
            SmartStampCollision = 231082
        };

        enum eDatas
        {
            MaxStampingPresses  = 20,
            MaxConveyorBelts    = 5,
            DataBeltEntry       = 0,
            DataSpawnTimer      = 1
        };

        enum eStates
        {
            BothInArena1,
            HansgarOut1,
            BothInArena2,
            FranzokOut,
            BothInArena3,
            HansgarOut2,
            BothInArenaFinal,
            MaxSwitchStates
        };

        struct boss_hansgarAI : public BossAI
        {
            boss_hansgarAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataHansgarAndFranzok), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;

                m_SwitchStatePct.resize(eStates::MaxSwitchStates);
                m_SwitchStatePct = { 85, 70, 55, 40, 25, 15, 0 };
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            bool m_IntroDone;

            StampingPressList m_StampingPresses;
            std::list<uint64> m_SmartStampCollisions;

            std::set<uint64> m_IntroTrashs;

            uint8 m_State;
            std::vector<int32> m_SwitchStatePct;

            uint8 m_BodySlamJumps;
            uint64 m_BodySlamTarget;

            uint32 m_DisabledBelt;

            bool CanRespawn() override
            {
                return false;
            }

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                _Reset();

                me->RemoveAllAreasTrigger();

                if (!m_IntroDone)
                {
                    m_StampingPresses.clear();

                    m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCosmeticStampingPresses, 1 * TimeConstants::IN_MILLISECONDS);

                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        std::list<Creature*> l_TrashList;

                        me->GetCreatureListWithEntryInGrid(l_TrashList, eCreatures::BlackrockEnforcer, 50.0f);
                        me->GetCreatureListWithEntryInGrid(l_TrashList, eCreatures::BlackrockForgeSpecialist, 50.0f);

                        for (Creature* l_Trash : l_TrashList)
                        {
                            l_Trash->Respawn();
                            m_IntroTrashs.insert(l_Trash->GetGUID());
                        }
                    });
                }

                me->CastSpell(me, eSpells::PumpedUp, true);

                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                m_State = 0;

                m_BodySlamJumps = 0;
                m_BodySlamTarget = 0;

                m_DisabledBelt = 0;

                me->setPowerType(Powers::POWER_ENERGY);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetPower(Powers::POWER_ENERGY, 0);

                me->SetAIAnimKitId(0);

                ResetArea();
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(eTalks::Slay);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                StartBrothers(me, p_Attacker, m_Instance);

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::Aggro);
                });

                if (Creature* l_Other = me->FindNearestCreature(eFoundryCreatures::BossFranzok, 150.0f))
                    me->AddAura(eSpells::BoundByBlood, l_Other);

                m_Events.ScheduleEvent(eEvents::EventBodySlam, 20 * TimeConstants::IN_MILLISECONDS + 500);
            }

            void EnterEvadeMode() override
            {
                m_Vehicle->RemoveAllPassengers();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ShatteredVertebrae);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::AftershockDoT);
                }

                CreatureAI::EnterEvadeMode();

                RespawnBrothers(me, m_Instance);
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ShatteredVertebrae);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::AftershockDoT);
                }

                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::Death);
                });
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::JumpSlamCast:
                    case eSpells::BodySlamOut:
                    {
                        if (Unit* l_Target = Unit::GetUnit(*me, m_BodySlamTarget))
                        {
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveJump(*l_Target, 30.0f, 10.0f, p_SpellInfo->Id);
                        }

                        break;
                    }
                    case eSpells::TacticalRetreat:
                    {
                        m_Events.ScheduleEvent(eEvents::EventBodySlam, 3 * TimeConstants::IN_MILLISECONDS);
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
                    case eSpells::JumpSlamSearcher:
                    {
                        /// Red arrow on affected target
                        me->SendPlaySpellVisual(eVisuals::BodySlamVisual, p_Target, 20.0f, 0.0f, Position());
                        me->CastSpell(p_Target, eSpells::JumpSlamCast, false);

                        me->SetFacingTo(me->GetAngle(p_Target));

                        m_BodySlamTarget = p_Target->GetGUID();
                        break;
                    }
                    case eSpells::CripplingSuplexScript:
                    {
                        p_Target->CastSpell(me, eSpells::CripplingSuplexJump, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            /// Handle that only for Hans'gar is enough, brothers have shared health
            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HealthBelowPctDamaged(m_SwitchStatePct[m_State], p_Damage))
                {
                    ++m_State;

                    switch (m_State)
                    {
                        /// Handle "phase" switch
                        case eStates::HansgarOut1:
                        {
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                            me->GetMotionMaster()->Clear();
                            me->SetReactState(ReactStates::REACT_PASSIVE);

                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(l_Target, eSpells::CripplingSuplexScript, true);

                            m_Events.CancelEvent(eEvents::EventBodySlam);

                            uint32 l_Time = 4 * TimeConstants::IN_MILLISECONDS;
                            AddTimedDelayedOperation(l_Time, [this]() -> void
                            {
                                /// Force player to cast 156611 and enter other boss
                                me->CastSpell(me, eSpells::CripplingSuplexSwitch, true);
                            });

                            l_Time += 3 * TimeConstants::IN_MILLISECONDS;
                            AddTimedDelayedOperation(l_Time, [this]() -> void
                            {
                                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                                me->GetMotionMaster()->Clear();
                                me->CastSpell(g_HansgarJumpPosOut, eSpells::TacticalRetreat, true);

                                Talk(eTalks::ActivateAssemblyLine);
                            });

                            StartSearingPlatesEvent();
                            break;
                        }
                        case eStates::BothInArena2:
                        {
                            EndSearingPlatesEvent();
                            Talk(eTalks::ReturningFromControls);
                            break;
                        }
                        case eStates::FranzokOut:
                        case eStates::BothInArena3:
                            break;
                        case eStates::HansgarOut2:
                        {
                            Talk(eTalks::ActivateAssemblyLine);

                            StartSearingPlatesEvent();
                            break;
                        }
                        case eStates::BothInArenaFinal:
                            break;
                        /// End of the fight
                        default:
                            break;
                    }
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionIntro:
                    {
                        if (m_IntroDone)
                            break;

                        m_IntroDone = true;

                        m_CosmeticEvents.CancelEvent(eCosmeticEvents::EventCosmeticStampingPresses);

                        DeactivatePress();

                        uint32 l_Time = 5 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            ActivatePress(eGameObjects::StampingPress09, true);
                            ActivatePress(eGameObjects::StampingPress11, true);

                            m_SmartStampCollisions.clear();

                            for (uint8 l_I = 0; l_I < 4; ++l_I)
                            {
                                if (GameObject* l_Collision = me->SummonGameObject(eGameObjects::SmartStampCollision, g_SmartStampCollisionPos[l_I], 0.0f, 0.0f, 0.0f, 0.0f, 0))
                                    m_SmartStampCollisions.push_back(l_Collision->GetGUID());
                            }
                        });

                        l_Time += 4 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro1);
                        });

                        l_Time += 11 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro2);
                        });

                        l_Time += 10 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro3);

                            for (uint64 l_Guid : m_IntroTrashs)
                            {
                                if (Creature* l_Trash = Creature::GetCreature(*me, l_Guid))
                                {
                                    l_Trash->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                                    float l_O = l_Trash->GetOrientation();
                                    float l_X = l_Trash->GetPositionX() + (10.0f * cos(l_O));
                                    float l_Y = l_Trash->GetPositionY() + (10.0f * sin(l_O));
                                    float l_Z = 130.5f;

                                    l_Trash->GetMotionMaster()->MoveJump(l_X, l_Y, l_Z, 30.0f, 10.0f);
                                    l_Trash->SetReactState(ReactStates::REACT_AGGRESSIVE);

                                    Position const l_Pos = { l_X, l_Y, l_Z, l_O };

                                    l_Trash->SetHomePosition(l_Pos);
                                }
                            }
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eSpells::JumpSlamCast:
                    case eSpells::BodySlamOut:
                    {
                        me->CastSpell(me, eSpells::BodySlamTriggered, true);

                        --m_BodySlamJumps;

                        if (m_BodySlamJumps)
                            me->CastSpell(me, eSpells::JumpSlamSearcher, true);

                        if (p_ID == eSpells::BodySlamOut)
                        {
                            me->RemoveAura(eSpells::NotReady);

                            AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                me->GetMotionMaster()->Clear();
                                me->CastSpell(g_HansgarJumpPosRetreat, eSpells::TacticalRetreatOther, true);
                            });
                        }

                        break;
                    }
                    case eSpells::TacticalRetreatOther:
                    {
                        me->SetAIAnimKitId(eVisuals::ChainPullVisual);
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_ID) override
            {
                switch (p_ID)
                {
                    default:
                        break;
                }

                return 0;
            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
                    default:
                        break;
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
                if (m_IntroTrashs.find(p_Guid) != m_IntroTrashs.end())
                    m_IntroTrashs.erase(p_Guid);

                if (m_IntroTrashs.empty())
                {
                    for (uint64 l_Guid : m_SmartStampCollisions)
                    {
                        if (GameObject* l_Collision = GameObject::GetGameObject(*me, l_Guid))
                            l_Collision->Delete();
                    }

                    m_SmartStampCollisions.clear();

                    DeactivatePress();

                    if (m_Instance != nullptr)
                    {
                        if (Creature* l_Franzok = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossFranzok)))
                        {
                            if (l_Franzok->IsAIEnabled)
                                l_Franzok->AI()->DoAction(eActions::ActionIntroFinished);
                        }
                    }

                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveJump(g_HansgarJumpPosIn, 30.0f, 10.0f);

                    AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                        me->SetHomePosition(*me);
                    });
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                /// Hans'gar only regens by script
                p_Value = 0;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    case eCosmeticEvents::EventCosmeticStampingPresses:
                    {
                        if (m_StampingPresses.empty())
                        {
                            std::list<Creature*> l_StampingPresses;
                            me->GetCreatureListWithEntryInGrid(l_StampingPresses, eCreatures::MobStampingPresses, 150.0f);

                            for (Creature* l_Press : l_StampingPresses)
                            {
                                if (GameObject* l_StampingPress = l_Press->FindNearestGameObject(2.0f))
                                    m_StampingPresses.push_back(StampingPressData(l_Press->GetGUID(), l_StampingPress->GetGUID()));
                            }

                            /// Shouldn't happens
                            if (m_StampingPresses.empty() || (int)m_StampingPresses.size() < eDatas::MaxStampingPresses)
                                break;
                        }

                        std::vector<uint32> l_Entries;

                        if (urand(0, 1))
                        {
                            l_Entries = { eGameObjects::StampingPress01, eGameObjects::StampingPress03, eGameObjects::StampingPress05, eGameObjects::StampingPress07,
                                          eGameObjects::StampingPress09, eGameObjects::StampingPress11, eGameObjects::StampingPress13, eGameObjects::StampingPress15,
                                          eGameObjects::StampingPress17, eGameObjects::StampingPress19 };
                        }
                        else
                        {
                            l_Entries = { eGameObjects::StampingPress02, eGameObjects::StampingPress04, eGameObjects::StampingPress06, eGameObjects::StampingPress08,
                                          eGameObjects::StampingPress10, eGameObjects::StampingPress12, eGameObjects::StampingPress14, eGameObjects::StampingPress16,
                                          eGameObjects::StampingPress18, eGameObjects::StampingPress20 };
                        }

                        for (uint32 l_Entry : l_Entries)
                            ActivatePress(l_Entry);

                        m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCosmeticStampingPresses, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || m_BodySlamJumps)
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBodySlam:
                    {
                        int32 l_Power = me->GetPower(Powers::POWER_RAGE);

                        if (l_Power >= 75)
                            m_BodySlamJumps = 5;
                        else if (l_Power >= 50)
                            m_BodySlamJumps = 4;
                        else if (l_Power >= 25)
                            m_BodySlamJumps = 3;
                        else
                            m_BodySlamJumps = 2;

                        /// When Hans'gar is out, jumps only once
                        if (me->GetReactState() == ReactStates::REACT_PASSIVE)
                        {
                            me->CastSpell(me, eSpells::NotReady, true);

                            me->SetAIAnimKitId(0);

                            m_BodySlamJumps = 1;

                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            {
                                m_BodySlamTarget = l_Target->GetGUID();

                                me->SetFacingTo(me->GetAngle(l_Target));
                                me->CastSpell(l_Target, eSpells::BodySlamRedArrowAura, true);
                            }
                        }
                        else
                            me->CastSpell(me, eSpells::JumpSlamSearcher, true);

                        m_Events.ScheduleEvent(eEvents::EventBodySlam, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                if (me->GetReactState() == ReactStates::REACT_AGGRESSIVE)
                    DoMeleeAttackIfReady();
            }

            void ActivatePress(uint32 p_Entry, bool p_Maintain = false)
            {
                for (StampingPressData l_StampingPress : m_StampingPresses)
                {
                    if (GameObject* l_Press = GameObject::GetGameObject(*me, l_StampingPress.StampingPress))
                    {
                        if (l_Press->GetEntry() != p_Entry)
                            continue;

                        /// Floor anim + down anim
                        l_Press->m_Events.AddEvent(new StampingPressActivation(l_StampingPress.StampingPress, eVisuals::AnimStamp1, true),
                            l_Press->m_Events.CalculateTime(1));

                        if (p_Maintain)
                            return;

                        /// Stamp + up anim
                        l_Press->m_Events.AddEvent(new StampingPressActivation(l_StampingPress.StampingPress, eVisuals::AnimStamp2, true),
                            l_Press->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));

                        /// Stay up
                        l_Press->m_Events.AddEvent(new StampingPressActivation(l_StampingPress.StampingPress, eVisuals::AnimStamp3, true),
                            l_Press->m_Events.CalculateTime(5 * TimeConstants::IN_MILLISECONDS));
                    }
                }
            }

            void DeactivatePress(uint32 p_Entry = 0)
            {
                for (StampingPressData l_StampingPress : m_StampingPresses)
                {
                    if (GameObject* l_Press = GameObject::GetGameObject(*me, l_StampingPress.StampingPress))
                    {
                        if (p_Entry && l_Press->GetEntry() != p_Entry)
                            continue;

                        l_Press->m_Events.KillAllEvents(false);

                        l_Press->m_Events.AddEvent(new StampingPressActivation(l_StampingPress.StampingPress, eVisuals::AnimStamp3, true),
                            l_Press->m_Events.CalculateTime(1));
                    }
                }
            }

            void StartSearingPlatesEvent()
            {
                /// CoveyorBelt Entry - StampingPresses Y pos (or Scorching Burns Y pos)
                using ConveyorDatas = std::map <uint32, float>;

                ConveyorDatas const l_ConveyorBeltPos =
                {
                    { eFoundryGameObjects::ConveyorBelt001, 3521.840088f },
                    { eFoundryGameObjects::ConveyorBelt002, 3507.370117f },
                    { eFoundryGameObjects::ConveyorBelt003, 3492.929932f },
                    { eFoundryGameObjects::ConveyorBelt004, 3478.469971f },
                    { eFoundryGameObjects::ConveyorBelt005, 3463.969961f }
                };

                ConveyorDatas const l_ConveyorBeltBurns =
                {
                    { eFoundryGameObjects::ConveyorBelt001, 3522.769043f },
                    { eFoundryGameObjects::ConveyorBelt002, 3507.606934f },
                    { eFoundryGameObjects::ConveyorBelt003, 3492.773926f },
                    { eFoundryGameObjects::ConveyorBelt004, 3477.840088f },
                    { eFoundryGameObjects::ConveyorBelt005, 3461.691895f }
                };

                for (uint32 l_Entry = eFoundryGameObjects::ConveyorBelt002; l_Entry <= eFoundryGameObjects::ConveyorBelt005; ++l_Entry)
                {
                    if (GameObject* l_ConveyorBelt = me->FindNearestGameObject(l_Entry, 100.0f))
                        l_ConveyorBelt->SendGameObjectActivateAnimKit(eFoundryVisuals::ConveyorsStart);
                }

                /// The searing plates come in waves, and there are two different ways in which the searing plates can be laid out.
                bool l_NonAligned = IsMythic() ? urand(0, 1) : true;
                if (l_NonAligned)
                {
                    /// One way is that 5 searing plates enter the room, each located on one of the 5 belts, but these plates are not aligned with one another.
                    /// This means that players can use one of the gaps between the plates to avoid taking damage.
                    /// This pattern seems to start always with the second belt -> 5th - 1st - 2nd - 3rd - 4th
                    /// Then it moves to the left -> 1st - 2nd - 3rd - 4th - 5th ... 2nd - 3rd - 4th - 5th - 1st ...

                    uint8 l_Index = 1;
                    uint32 l_Time = 4 * TimeConstants::IN_MILLISECONDS;

                    std::list<Creature*> l_BurnList;
                    me->GetCreatureListWithEntryInGrid(l_BurnList, eCreatures::ScorchingBurns, 200.0f);

                    if (l_BurnList.empty())
                        return;

                    /// Start all cycles in advance
                    for (uint8 l_I = 0; l_I < eDatas::MaxConveyorBelts; ++l_I)
                    {
                        for (uint32 l_BeltEntry : g_SearingPlatesCycles[l_I])
                        {
                            if (GameObject* l_Belt = me->FindNearestGameObject(l_BeltEntry, 100.0f))
                            {
                                std::list<Creature*> l_TempList = l_BurnList;

                                auto l_BurnsDatas = l_ConveyorBeltBurns.find(l_BeltEntry);

                                l_TempList.remove_if([this, l_BurnsDatas](Creature* p_Creature) -> bool
                                {
                                    if (p_Creature == nullptr)
                                        return true;

                                    if (p_Creature->GetPositionY() != l_BurnsDatas->second)
                                    {
                                        Position l_Pos      = *p_Creature;
                                        l_Pos.m_positionX   = g_ScorchingBurnsXPos;
                                        l_Pos.m_positionY   = l_BurnsDatas->second;

                                        if (!p_Creature->IsNearPosition(&l_Pos, 2.0f))
                                            return true;
                                    }

                                    return false;
                                });

                                if (l_TempList.empty())
                                    continue;

                                if (Creature* l_Burns = l_TempList.front())
                                {
                                    if (l_Burns->IsAIEnabled)
                                    {
                                        l_Burns->AI()->SetData(eDatas::DataBeltEntry, l_BeltEntry);
                                        l_Burns->AI()->SetData(eDatas::DataSpawnTimer, l_Time * l_Index);
                                    }
                                }
                            }

                            ++l_Index;
                        }
                    }
                }
                else
                {
                    /// This seems to be possible only during Mythic encounter
                    /// Another way is that 4 searing plates enter the room, each located on one of the 5 belts.
                    /// These 4 plates are perfectly aligned, but the 5th conveyor belt has no plate and thus provides a large gap through which players can move.
                    /// With each wave, the gap moves to a different conveyor belt, but the gaps are always located on adjacent belts.
                    /// Sometimes, however, when the gap is located on a belt that is at the edge of the room, the next gap can be on belt that is on the opposite edge.

                    /*if (!m_DisabledBelt)
                        m_DisabledBelt = urand(eFoundryGameObjects::ConveyorBelt002, eFoundryGameObjects::ConveyorBelt005);
                    else
                    {
                        if (m_DisabledBelt > eFoundryGameObjects::ConveyorBelt002 && m_DisabledBelt < eFoundryGameObjects::ConveyorBelt005)
                            m_DisabledBelt += (urand(0, 1) ? -1 : 1);
                        else
                        {
                            if (m_DisabledBelt == eFoundryGameObjects::ConveyorBelt002)
                                m_DisabledBelt = (urand(0, 1) ? eFoundryGameObjects::ConveyorBelt001 : eFoundryGameObjects::ConveyorBelt005);
                            else ///< m_DisabledBelt == eFoundryGameObjects::ConveyorBelt005
                                m_DisabledBelt = (urand(0, 1) ? eFoundryGameObjects::ConveyorBelt002 : eFoundryGameObjects::ConveyorBelt004);
                        }
                    }

                    for (auto l_Datas : l_ConveyorBeltPos)
                    {
                        if (l_Datas.first == m_DisabledBelt)
                            continue;

                        auto l_BurnsDatas = l_ConveyorBeltBurns.find(l_Datas.first);
                        if (l_BurnsDatas == l_ConveyorBeltBurns.end())
                            continue;

                        float l_PressY = l_Datas.second;
                        float l_BurnsY = l_BurnsDatas->second;
                    }*/
                }
            }

            void EndSearingPlatesEvent()
            {
                for (uint32 l_Entry = eFoundryGameObjects::ConveyorBelt002; l_Entry <= eFoundryGameObjects::ConveyorBelt005; ++l_Entry)
                {
                    if (GameObject* l_ConveyorBelt = me->FindNearestGameObject(l_Entry, 100.0f))
                        l_ConveyorBelt->SendGameObjectActivateAnimKit(eFoundryVisuals::ConveyorsStop);
                }

                /// Clear all pending cycles
                for (uint8 l_I = 0; l_I < eDatas::MaxConveyorBelts; ++l_I)
                {
                    for (uint32 l_BeltEntry : g_SearingPlatesCycles[l_I])
                    {
                        if (GameObject* l_Belt = me->FindNearestGameObject(l_BeltEntry, 100.0f))
                        {
                            if (Creature* l_Burns = l_Belt->FindNearestCreature(eCreatures::ScorchingBurns, 50.0f))
                            {
                                if (l_Burns->IsAIEnabled)
                                    l_Burns->AI()->Reset();
                            }
                        }
                    }
                }
            }

            void ResetArea()
            {
                for (uint32 l_Entry = eFoundryGameObjects::ConveyorBelt002; l_Entry <= eFoundryGameObjects::ConveyorBelt005; ++l_Entry)
                {
                    if (GameObject* l_ConveyorBelt = me->FindNearestGameObject(l_Entry, 100.0f))
                        l_ConveyorBelt->SendGameObjectActivateAnimKit(eFoundryVisuals::ConveyorsStop);
                }

                /// Clear all pending cycles
                for (uint8 l_I = 0; l_I < eDatas::MaxConveyorBelts; ++l_I)
                {
                    for (uint32 l_BeltEntry : g_SearingPlatesCycles[l_I])
                    {
                        if (GameObject* l_Belt = me->FindNearestGameObject(l_BeltEntry, 100.0f))
                        {
                            if (Creature* l_Burns = l_Belt->FindNearestCreature(eCreatures::ScorchingBurns, 50.0f))
                            {
                                if (l_Burns->IsAIEnabled)
                                    l_Burns->AI()->Reset();
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_hansgarAI(p_Creature);
        }
};

/// Franzok - 76974
class boss_franzok : public CreatureScript
{
    public:
        boss_franzok() : CreatureScript("boss_franzok") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3,
            Aggro,
            ActivateAssemblyLine,
            ReturningFromControls,
            Slay,
            Wipe,
            DisruptingRoar,
            BodySlam
        };

        enum eSpells
        {
            /// Misc
            PumpedUp                = 155665,
            BoundByBlood            = 161030,
            /// Disrupting Roar
            SpellDisruptingRoar     = 160838,
            /// Skullcracker
            Skullcracker            = 153470,
            /// Crippling Suplex
            CripplingSuplexThrow    = 156938
        };

        enum eEvents
        {
            EventDisruptingRoar = 1,
            EventSkullcracker
        };

        enum eCosmeticEvents
        {
        };

        enum eActions
        {
            ActionIntro,
            ActionIntroFinished,
            ActionCripplingSuplex
        };

        enum eVisuals
        {
        };

        enum eCreatures
        {
        };

        enum eDatas
        {
            DataMainTankHealth,
            DataOffTankHealth,  ///< ExplicitTarget of Crippling Suplex
            DataMaxTankHealths
        };

        struct boss_franzokAI : public BossAI
        {
            boss_franzokAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataHansgarAndFranzok), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;

                m_TankHealths.resize(eDatas::DataMaxTankHealths);
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            bool m_IntroDone;

            uint64 m_ExitTankGuid;
            std::vector<uint32> m_TankHealths;

            bool CanRespawn() override
            {
                return false;
            }

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                _Reset();

                me->RemoveAllAreasTrigger();

                me->CastSpell(me, eSpells::PumpedUp, true);

                me->setPowerType(Powers::POWER_ENERGY);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetPower(Powers::POWER_ENERGY, 0);

                m_ExitTankGuid = 0;
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(eTalks::Slay);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                StartBrothers(me, p_Attacker, m_Instance);

                Talk(eTalks::Aggro);

                m_Events.ScheduleEvent(eEvents::EventDisruptingRoar, 45 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSkullcracker, 20 * TimeConstants::IN_MILLISECONDS);

                if (Creature* l_Other = me->FindNearestCreature(eFoundryCreatures::BossHansgar, 150.0f))
                    me->AddAura(eSpells::BoundByBlood, l_Other);
            }

            void EnterEvadeMode() override
            {
                m_Vehicle->RemoveAllPassengers();

                CreatureAI::EnterEvadeMode();

                RespawnBrothers(me, m_Instance);

                Talk(eTalks::Wipe);
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::CripplingSuplexThrow:
                    {
                        if (Unit* l_Passenger = m_Vehicle->GetPassenger(0))
                        {
                            if (Unit* l_Tank = Unit::GetUnit(*me, m_ExitTankGuid))
                                l_Passenger->ExitVehicle(l_Tank);
                        }

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
            }

            void PassengerBoarded(Unit* p_Passenger, int8 p_SeatID, bool p_Apply) override
            {
                /// Handle Crippling Suplex
                if (p_Apply)
                {
                    m_TankHealths[eDatas::DataMainTankHealth] = p_Passenger->GetMaxHealth();

                    AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            m_ExitTankGuid = l_Target->GetGUID();
                            m_TankHealths[eDatas::DataOffTankHealth] = l_Target->GetMaxHealth();

                            me->CastSpell(l_Target, eSpells::CripplingSuplexThrow, false);
                        }
                    });
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionIntro:
                    {
                        if (m_IntroDone)
                            break;

                        m_IntroDone = true;

                        uint32 l_Time = 14 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro1);
                        });

                        l_Time += 11 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro2);
                        });

                        l_Time += 11 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro3);
                        });

                        break;
                    }
                    case eActions::ActionIntroFinished:
                    {
                        me->GetMotionMaster()->MoveJump(g_FranzokJumpPosIn, 30.0f, 10.0f);

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                            me->SetHomePosition(*me);
                        });

                        break;
                    }
                    case eActions::ActionCripplingSuplex:
                    {
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eDatas::DataMainTankHealth:
                        return m_TankHealths[eDatas::DataMainTankHealth];
                    case eDatas::DataOffTankHealth:
                        return m_TankHealths[eDatas::DataOffTankHealth];
                    default:
                        break;
                }

                return 0;
            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                /// Hans'gar only regens by script
                p_Value = 0;
            }

            void OnCalculateCastingTime(SpellInfo const* p_SpellInfo, int32& p_CastingTime) override
            {
                if (p_SpellInfo->Id == eSpells::SpellDisruptingRoar)
                {
                    int32 l_Rage = me->GetPower(Powers::POWER_RAGE);

                    if (l_Rage >= 75)
                        p_CastingTime = 500 + 1 * TimeConstants::IN_MILLISECONDS;
                    else if (l_Rage >= 50)
                        p_CastingTime = 2 * TimeConstants::IN_MILLISECONDS;
                    else if (l_Rage >= 25)
                        p_CastingTime = 500 + 2 * TimeConstants::IN_MILLISECONDS;
                    else
                        p_CastingTime = 3 * TimeConstants::IN_MILLISECONDS;
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
                    case eEvents::EventDisruptingRoar:
                    {
                        Talk(eTalks::DisruptingRoar);
                        me->CastSpell(me, eSpells::SpellDisruptingRoar, false);
                        m_Events.ScheduleEvent(eEvents::EventDisruptingRoar, 46 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSkullcracker:
                    {
                        CustomSpellValues l_Values;

                        int32 l_Count = 0;
                        int32 l_Rage = me->GetPower(Powers::POWER_RAGE);

                        if (l_Rage >= 75)
                            l_Count = 5;
                        else if (l_Rage >= 50)
                            l_Count = 4;
                        else if (l_Rage >= 25)
                            l_Count = 3;
                        else
                            l_Count = 2;

                        l_Values.AddSpellMod(SpellValueMod::SPELLVALUE_MAX_TARGETS, l_Count);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 5.0f))
                            me->CastCustomSpell(eSpells::Skullcracker, l_Values, l_Target, false);
                        else if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastCustomSpell(eSpells::Skullcracker, l_Values, l_Target, false);

                        m_Events.ScheduleEvent(eEvents::EventSkullcracker, 21 * TimeConstants::IN_MILLISECONDS);
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
            return new boss_franzokAI(p_Creature);
        }
};

/// Forge Overdrive - 77258
class npc_foundry_forge_overdrive : public CreatureScript
{
    public:
        npc_foundry_forge_overdrive() : CreatureScript("npc_foundry_forge_overdrive") { }

        enum eSpells
        {
            SearingPlatesAT     = 155994,
            SearingPlatesDoT    = 161570
        };

        enum eMove
        {
            MoveDespawn = 1
        };

        struct npc_foundry_forge_overdriveAI : public ScriptedAI
        {
            npc_foundry_forge_overdriveAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_BeltEntry = 0;

                p_Creature->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_CAN_SAFE_FALL);

                p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
            }

            uint32 m_BeltEntry;

            void Reset() override
            {
                me->CastSpell(me, eSpells::SearingPlatesAT, true);

                AddTimedDelayedOperation(100, [this]() -> void
                {
                    if (g_ForgeOverdriveDestPos.find(m_BeltEntry) != g_ForgeOverdriveDestPos.end())
                    {
                        Position const l_DestPos = g_ForgeOverdriveDestPos[m_BeltEntry];

                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(eMove::MoveDespawn, l_DestPos);
                    }
                });
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                if (p_ID == eMove::MoveDespawn)
                {
                    me->RemoveAllAuras();
                    me->DespawnOrUnsummon();
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_forge_overdriveAI(p_Creature);
        }
};

/// Scorching Burns - 78823
class npc_foundry_scorching_burns : public CreatureScript
{
    public:
        npc_foundry_scorching_burns() : CreatureScript("npc_foundry_scorching_burns") { }

        enum eSpells
        {
            ScorchingBurnsAT    = 159182
        };

        enum eEvents
        {
        };

        enum eAction
        {
            ActionSearingPlate = 3
        };

        enum eCreature
        {
            ForgeOverdrive = 77258
        };

        enum eDatas
        {
            DataBeltEntry,
            DataSpawnTimer
        };

        struct npc_foundry_scorching_burnsAI : public ScriptedAI
        {
            npc_foundry_scorching_burnsAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            uint32 m_BeltEntry;

            std::set<uint64> m_AffectedPlayers;

            void Reset() override
            {
                m_Events.Reset();

                summons.DespawnAll();

                for (uint64 l_Guid : m_AffectedPlayers)
                {
                    if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                        l_Player->SendApplyMovementForce(me->GetGUID(), false, Position());
                }

                m_AffectedPlayers.clear();

                m_BeltEntry = 0;

                ClearDelayedOperations();
            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
                    case eDatas::DataBeltEntry:
                    {
                        m_BeltEntry = p_Value;
                        break;
                    }
                    case eDatas::DataSpawnTimer:
                    {
                        AddTimedDelayedOperation(p_Value, [this]() -> void
                        {
                            DoAction(eAction::ActionSearingPlate);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionSearingPlate:
                    {
                        me->CastSpell(me, eSpells::ScorchingBurnsAT, false);

                        if (g_ForgeOverdriveSpawnPos.find(m_BeltEntry) != g_ForgeOverdriveSpawnPos.end())
                        {
                            Position const l_SpawnPos = g_ForgeOverdriveSpawnPos[m_BeltEntry];

                            if (Creature* l_ForgeOverdrive = me->SummonCreature(eCreature::ForgeOverdrive, l_SpawnPos))
                            {
                                using ForgeOverdriveAIPtr   = npc_foundry_forge_overdrive::npc_foundry_forge_overdriveAI*;
                                using ForgeOverdriveAI      = npc_foundry_forge_overdrive::npc_foundry_forge_overdriveAI;

                                if (ForgeOverdriveAIPtr l_AI = CAST_AI(ForgeOverdriveAI, l_ForgeOverdrive->GetAI()))
                                    l_AI->m_BeltEntry = m_BeltEntry;
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

                if (g_ConveyorForceMovesDirection.find(m_BeltEntry) != g_ConveyorForceMovesDirection.end())
                {
                    float l_MaxDist = 84.0f;

                    std::set<uint64> l_Targets;

                    Map::PlayerList const& l_PlayerList = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
                    {
                        if (Player* l_Player = l_Iter->getSource())
                        {
                            if (l_Player->GetDistance(me) >= l_MaxDist)
                                continue;

                            if (l_Player->GetPositionY() <= me->GetPositionY() + 7.2f &&
                                l_Player->GetPositionY() >= me->GetPositionY() - 7.2f)
                                l_Targets.insert(l_Player->GetGUID());
                        }
                    }

                    for (std::set<uint64>::iterator l_Iter = m_AffectedPlayers.begin(); l_Iter != m_AffectedPlayers.end();)
                    {
                        if (l_Targets.find((*l_Iter)) == l_Targets.end())
                        {
                            if (Player* l_Player = Player::GetPlayer(*me, (*l_Iter)))
                                l_Player->SendApplyMovementForce(me->GetGUID(), false, Position());

                            l_Iter = m_AffectedPlayers.erase(l_Iter);
                        }
                        else
                            ++l_Iter;
                    }

                    for (uint64 l_Guid : l_Targets)
                    {
                        if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                        {
                            if (!l_Player->HasMovementForce(me->GetGUID()))
                            {
                                l_Player->SendApplyMovementForce(me->GetGUID(), true, g_ConveyorForceMovesDirection[m_BeltEntry], 6.0f, 0, G3D::Vector3(-6.0f, 0.0f, 0.0f));

                                m_AffectedPlayers.insert(l_Guid);
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_scorching_burnsAI(p_Creature);
        }
};

/// Pumped Up - 155665
class spell_foundry_pumped_up : public SpellScriptLoader
{
    public:
        spell_foundry_pumped_up() : SpellScriptLoader("spell_foundry_pumped_up") { }

        class spell_foundry_pumped_up_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_pumped_up_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (AuraEffectPtr l_AurEff = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                    {
                        int32 l_Pct = 100 - (int32)l_Target->GetHealthPct();

                        l_Target->SetPower(Powers::POWER_ENERGY, l_Pct);
                        l_AurEff->ChangeAmount(l_Pct);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundry_pumped_up_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_pumped_up_AuraScript();
        }
};

/// Crippling Suplex - 156938
class spell_foundry_crippling_suplex : public SpellScriptLoader
{
    public:
        spell_foundry_crippling_suplex() : SpellScriptLoader("spell_foundry_crippling_suplex") { }

        enum eDatas
        {
            DataMainTankHealth,
            DataOffTankHealth   ///< ExplicitTarget of Crippling Suplex
        };

        class spell_foundry_crippling_suplex_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_crippling_suplex_SpellScript);

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Creature* l_Boss = GetCaster()->ToCreature())
                {
                    if (Unit* l_MainTarget = GetExplTargetUnit())
                    {
                        if (Unit* l_Target = GetHitUnit())
                        {
                            int32 l_Damage = 0;

                            if (l_MainTarget != l_Target)
                                l_Damage = l_Boss->IsAIEnabled ? l_Boss->AI()->GetData(eDatas::DataMainTankHealth) : 0;
                            else
                                l_Damage = l_Boss->IsAIEnabled ? l_Boss->AI()->GetData(eDatas::DataOffTankHealth) : 0;

                            if (l_Damage)
                                SetHitDamage(l_Damage);
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_foundry_crippling_suplex_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_crippling_suplex_SpellScript();
        }
};

/// Pumped Up - 155665
class spell_foundry_body_slam_red_arrow : public SpellScriptLoader
{
    public:
        spell_foundry_body_slam_red_arrow() : SpellScriptLoader("spell_foundry_body_slam_red_arrow") { }

        class spell_foundry_body_slam_red_arrow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_body_slam_red_arrow_AuraScript);

            enum eSpell
            {
                BodySlam = 155747
            };

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, eSpell::BodySlam, true);
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_foundry_body_slam_red_arrow_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_body_slam_red_arrow_AuraScript();
        }
};

void AddSC_boss_hansgar_and_franzok()
{
    /// Bosses
    new boss_hansgar();
    new boss_franzok();

    /// Creatures
    new npc_foundry_forge_overdrive();
    new npc_foundry_scorching_burns();

    /// Spells
    new spell_foundry_pumped_up();
    new spell_foundry_crippling_suplex();
    new spell_foundry_body_slam_red_arrow();
}