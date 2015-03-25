////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

Position const g_GhargFirstPos = { 3466.11f, 7577.58f, 15.203f, 0.8954f };
Position const g_GhargSecondPos = { 3483.23f, 7598.67f, 10.65f, 0.8954f };
Position const g_TeleportPos = { 3466.42f, 7578.84f, 55.34f, 4.0125f };
Position const g_MargokTeleport = { 3432.25f, 7536.13f, 73.664f, 0.896154f };
Position const g_KargathPos = { 3444.50f, 7550.76f, 55.39f, 0.90f };

/// Gharg <Arena Master> - 84971
class npc_highmaul_gharg_arena_master : public CreatureScript
{
    public:
        npc_highmaul_gharg_arena_master() : CreatureScript("npc_highmaul_gharg_arena_master") { }

        enum eMove
        {
            MoveFirstPos,
            MoveSecondPos
        };

        struct npc_highmaul_gharg_arena_masterAI : public ScriptedAI
        {
            npc_highmaul_gharg_arena_masterAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->SetWalk(true);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SummonGameObject(eHighmaulGameobjects::ArenaElevator, 3466.438f, 7577.974f, 14.94214f, 0.8901166f, 0.0f, 0.0f, 0.4305113f, 0.9025852f, 1000);
                me->GetMotionMaster()->MovePoint(eMove::MoveFirstPos, g_GhargFirstPos);
            }

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                if (m_Instance == nullptr)
                    return;

                /// Teleport player
                if (m_Instance->GetData(eHighmaulDatas::ElevatorActivated))
                    p_Player->NearTeleportTo(g_TeleportPos);
                else
                {
                    me->GetMotionMaster()->MovePoint(eMove::MoveSecondPos, g_GhargSecondPos);
                    m_Instance->SetData(eHighmaulDatas::ElevatorActivated, true);
                }

                p_Player->PlayerTalkClass->SendCloseGossip();
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (m_Instance == nullptr || p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMove::MoveFirstPos:
                    {
                        me->SetFacingTo(0.8954f);
                        me->SetHomePosition(g_GhargSecondPos);
                        break;
                    }
                    case eMove::MoveSecondPos:
                    {
                        me->SetHomePosition(g_GhargSecondPos);

                        /// Start elevator
                        if (GameObject* l_Elevator = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::ArenaElevator)))
                            l_Elevator->SetTransportState(GOState::GO_STATE_TRANSPORT_STOPPED);

                        /// Active Collision Wall
                        if (GameObject* l_Wall = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::CollisionWall)))
                            l_Wall->SetGoState(GOState::GO_STATE_READY);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gharg_arena_masterAI(p_Creature);
        }
};

/// Area Trigger for Crowd - 79260
class npc_highmaul_areatrigger_for_crowd : public CreatureScript
{
    public:
        npc_highmaul_areatrigger_for_crowd() : CreatureScript("npc_highmaul_areatrigger_for_crowd") { }

        enum eAction
        {
            StartIntro
        };

        enum eSpell
        {
            ElevatorSoundTrigger = 166694
        };

        struct npc_highmaul_areatrigger_for_crowdAI : public ScriptedAI
        {
            npc_highmaul_areatrigger_for_crowdAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_IntroStarted  = false;
                m_Instance      = p_Creature->GetInstanceScript();
                m_CheckTimer    = 1000;
            }

            bool m_IntroStarted;
            InstanceScript* m_Instance;

            uint32 m_CheckTimer;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_CheckTimer || m_IntroStarted)
                    return;

                if (m_CheckTimer <= p_Diff)
                {
                    if (Player* l_Player = me->FindNearestPlayer(10.0f))
                    {
                        m_IntroStarted = true;

                        me->CastSpell(me, eSpell::ElevatorSoundTrigger, true);

                        if (Creature* l_Jhorn = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::JhornTheMad)))
                            l_Jhorn->AI()->DoAction(eAction::StartIntro);

                        if (Creature* l_Thoktar = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::ThoktarIronskull)))
                            l_Thoktar->AI()->DoAction(eAction::StartIntro);
                    }
                    else
                        m_CheckTimer = 1000;
                }
                else
                    m_CheckTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_areatrigger_for_crowdAI(p_Creature);
        }
};

/// Jhorn the Mad - 83377
class npc_highmaul_jhorn_the_mad : public CreatureScript
{
    public:
        npc_highmaul_jhorn_the_mad() : CreatureScript("npc_highmaul_jhorn_the_mad") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3,
            Intro4,
            Intro5,
            Trash1,
            Trash2,
            Kargath1,
            Kargath2
        };

        enum eActions
        {
            StartIntro,
            ContinueIntro,
            VulgorDied
        };

        struct npc_highmaul_jhorn_the_madAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_jhorn_the_madAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::StartIntro:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro1); });
                        AddTimedDelayedOperation(16 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro2); });
                        AddTimedDelayedOperation(38 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro3); });

                        AddTimedDelayedOperation(55 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Talk(eTalks::Intro4);

                            if (m_Instance != nullptr)
                            {
                                if (Creature* l_Vulgor = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Vulgor)))
                                    l_Vulgor->AI()->DoAction(eActions::StartIntro);
                            }
                        });

                        AddTimedDelayedOperation(56 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance != nullptr)
                            {
                                if (GameObject* l_InnerGate = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::GateArenaInner)))
                                    l_InnerGate->Use(me);
                            }
                        });

                        AddTimedDelayedOperation(70 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro5); });
                        break;
                    }
                    case eActions::ContinueIntro:
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash1); });
                        AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash2); });
                        break;
                    case eActions::VulgorDied:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Kargath1); });

                        AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                                l_Kargath->AI()->DoAction(eActions::VulgorDied);
                        });

                        AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Kargath2); });

                        AddTimedDelayedOperation(21 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                                l_Kargath->SetFacingTo(4.02f);
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
            return new npc_highmaul_jhorn_the_madAI(p_Creature);
        }
};

/// Thoktar Ironskull - 83378
class npc_highmaul_thoktar_ironskull : public CreatureScript
{
    public:
        npc_highmaul_thoktar_ironskull() : CreatureScript("npc_highmaul_thoktar_ironskull") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3,
            Trash1,
            Trash2,
            Kargath1
        };

        enum eActions
        {
            StartIntro,
            ContinueIntro,
            VulgorDied
        };

        struct npc_highmaul_thoktar_ironskullAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_thoktar_ironskullAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::StartIntro:
                        AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro1); });
                        AddTimedDelayedOperation(31 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro2); });
                        AddTimedDelayedOperation(50 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro3); });
                        break;
                    case eActions::ContinueIntro:
                        AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash1); });
                        AddTimedDelayedOperation(17 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash2); });
                        break;
                    case eActions::VulgorDied:
                        AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Kargath1); });
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_thoktar_ironskullAI(p_Creature);
        }
};

/// Imperator Mar'gok <Sorcerer King> (Only cosmetic) - 83268
class npc_highmaul_imperator_margok : public CreatureScript
{
    public:
        npc_highmaul_imperator_margok() : CreatureScript("npc_highmaul_imperator_margok") { }

        enum eTalks
        {
            SorckingEvent12,
            SorckingEvent13
        };

        enum eActions
        {
            VulgorDied = 2,
            KargathLastTalk
        };

        enum eMove
        {
            MoveFrontGate = 1
        };

        enum eSpells
        {
            TeleportIntoArena   = 167048,
            TeleportVisual      = 167050,
            SitThrone           = 88648
        };

        struct npc_highmaul_imperator_margokAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_imperator_margokAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::VulgorDied:
                    {
                        /// Teleport in Coliseum
                        AddTimedDelayedOperation(19 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->CastSpell(me, eSpells::TeleportIntoArena, true);
                            me->NearTeleportTo(g_MargokTeleport);
                            me->CastSpell(me, eSpells::TeleportVisual, true);
                        });

                        AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->SetFacingTo(g_MargokTeleport.m_orientation);
                            me->RemoveAura(eSpells::TeleportIntoArena);
                        });

                        AddTimedDelayedOperation(28 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::SorckingEvent12); });

                        AddTimedDelayedOperation(49 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance == nullptr)
                                return;

                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                            {
                                l_Kargath->SetWalk(true);
                                l_Kargath->GetMotionMaster()->MovePoint(eMove::MoveFrontGate, g_KargathPos);
                            }
                        });

                        AddTimedDelayedOperation(39 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance == nullptr)
                                return;

                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                                l_Kargath->AI()->DoAction(eActions::KargathLastTalk);
                        });

                        AddTimedDelayedOperation(51 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::SorckingEvent13); });
                        AddTimedDelayedOperation(52 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->CastSpell(me, eSpells::SitThrone, true); });
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_imperator_margokAI(p_Creature);
        }
};

/// Arena Elevator - 233098
class go_highmaul_arena_elevator : public GameObjectScript
{
    public:
        go_highmaul_arena_elevator() : GameObjectScript("go_highmaul_arena_elevator") { }

        bool OnGameObjectElevatorCheck(GameObject const* p_GameObject) const override
        {
            return false;
        }
};

void AddSC_highmaul()
{
    /// NPCs
    new npc_highmaul_gharg_arena_master();
    new npc_highmaul_areatrigger_for_crowd();
    new npc_highmaul_jhorn_the_mad();
    new npc_highmaul_thoktar_ironskull();
    new npc_highmaul_imperator_margok();

    /// GameObjects
    new go_highmaul_arena_elevator();
}