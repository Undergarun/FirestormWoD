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

void StartBrothers(Creature* p_Source, Unit* p_Target)
{
    if (p_Source == nullptr || p_Target == nullptr)
        return;

    if (Creature* l_Other = p_Source->FindNearestCreature((p_Source->GetEntry() == eFoundryCreatures::BossFranzok) ? eFoundryCreatures::BossHansgar : eFoundryCreatures::BossFranzok, 30.0f))
        l_Other->AI()->AttackStart(p_Target);
}

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
            PumpedUp = 155665
        };

        enum eEvents
        {
        };

        enum eCosmeticEvents
        {
            EventCosmeticStampingPresses = 1
        };

        enum eActions
        {
            ActionIntro,
            ActionIntroFinished
        };

        enum eVisuals
        {
        };

        enum eCreatures
        {
            MobStampingPresses          = 78358,
            BlackrockForgeSpecialist    = 79200,
            BlackrockEnforcer           = 79208
        };

        enum eGameObjects
        {
            StampingPress16     = 229574,
            StampingPress20     = 229575,
            StampingPress19     = 229576,
            StampingPress18     = 228577,
            StampingPress17     = 228578,
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
            MaxStampingPresses = 20
        };

        struct boss_hansgarAI : public BossAI
        {
            boss_hansgarAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataHansgarAndFranzok)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            InstanceScript* m_Instance;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            bool m_IntroDone;

            StampingPressList m_StampingPresses;
            std::list<uint64> m_SmartStampCollisions;

            std::set<uint64> m_IntroTrashs;

            void Reset() override
            {
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
                            m_IntroTrashs.insert(l_Trash->GetGUID());
                    });
                }

                me->CastSpell(me, eSpells::PumpedUp, true);
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

                StartBrothers(me, p_Attacker);

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::Aggro);
                });
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                RespawnBrothers(me, m_Instance);
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

                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::Death);
                });
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo) override
            {
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;
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
                                }
                            }
                        });

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

                    ActivatePress(eGameObjects::StampingPress09, true);
                    ActivatePress(eGameObjects::StampingPress11, true);

                    if (m_Instance != nullptr)
                    {
                        if (Creature* l_Franzok = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossFranzok)))
                        {
                            if (l_Franzok->IsAIEnabled)
                                l_Franzok->AI()->DoAction(eActions::ActionIntroFinished);
                        }
                    }

                    me->GetMotionMaster()->MoveJump(g_HansgarJumpPosIn, 30.0f, 10.0f);

                    AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                        me->SetHomePosition(*me);
                    });
                }
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

                        std::set<uint32> l_UsedEntries;

                        for (uint8 l_I = 0; l_I < 4; ++l_I)
                        {
                            uint32 l_Entry = 0;

                            do
                            {
                                l_Entry = urand(eGameObjects::StampingPress16, (eGameObjects::StampingPress16 + eDatas::MaxStampingPresses - 1));
                            }
                            while (l_Entry && l_UsedEntries.find(l_Entry) != l_UsedEntries.end());

                            ActivatePress(l_Entry);

                            l_UsedEntries.insert(l_Entry);
                        }

                        m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCosmeticStampingPresses, 7 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ActivatePress(uint32 p_Entry, bool p_Maintain = false)
            {
                for (StampingPressData l_StampingPress : m_StampingPresses)
                {
                    if (GameObject* l_Press = GameObject::GetGameObject(*me, l_StampingPress.StampingPress))
                    {
                        if (l_Press->GetEntry() == p_Entry)
                        {
                            if (p_Maintain)
                            {
                                l_Press->SetLootState(LootState::GO_READY);
                                l_Press->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                            }
                            else
                                l_Press->Use(me);
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
            PumpedUp            = 155665,
            /// Disrupting Roar
            SpellDisruptingRoar = 160838,
            /// Skullcracker
            Skullcracker        = 153470
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
            ActionIntroFinished
        };

        enum eVisuals
        {
        };

        enum eCreatures
        {
        };

        enum eDatas
        {
        };

        struct boss_franzokAI : public BossAI
        {
            boss_franzokAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataHansgarAndFranzok)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            InstanceScript* m_Instance;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            bool m_IntroDone;

            void Reset() override
            {
                m_Events.Reset();
                m_CosmeticEvents.Reset();

                _Reset();

                me->RemoveAllAreasTrigger();

                me->CastSpell(me, eSpells::PumpedUp, true);
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

                StartBrothers(me, p_Attacker);

                Talk(eTalks::Aggro);

                m_Events.ScheduleEvent(eEvents::EventDisruptingRoar, 45 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSkullcracker, 20 * TimeConstants::IN_MILLISECONDS);
            }

            void EnterEvadeMode() override
            {
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
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo) override
            {
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;
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
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 5.0f))
                            me->CastSpell(l_Target, eSpells::Skullcracker, false);
                        else if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::Skullcracker, false);

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

void AddSC_boss_hansgar_and_franzok()
{
    /// Bosses
    new boss_hansgar();
    new boss_franzok();
}