////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "boss_iron_maidens.hpp"

/// Admiral Gar'an - 77557
class boss_admiral_garan : public CreatureScript
{
    public:
        boss_admiral_garan() : CreatureScript("boss_admiral_garan") { }

        enum eSpells
        {
            IronMaidenIntroConversation = 172658,
            AfterTrashesConversation    = 172686,
            /// Combat spells
            /// Iron Shot
            SpellIronShotSearcher       = 156666,
            SpellIronShotDamage         = 156669,
            /// Rapid Fire
            SpellRapidFireRedArrow      = 156631,
            SpellRapideFirePeriodic     = 156626,
            SpellRapidFireTargetVisual  = 156649
        };

        enum eEvents
        {
            EventRapidFire = 1,
            EventRegenIronFury,
            EventPenetratingShot,
            EventDeployTurret
        };

        enum eTimers
        {
            TimerRapidFire      = 19 * TimeConstants::IN_MILLISECONDS,
            TimerRapidFireCD    = 30 * TimeConstants::IN_MILLISECONDS,
            TimerEnergyRegen    = 6 * TimeConstants::IN_MILLISECONDS + 333
        };

        enum eTalks
        {
        };

        enum eMoves
        {
            MoveJump = 1,
            MoveDown,
            MoveLast
        };

        enum eVisual
        {
            IntroVisual = 6636
        };

        struct boss_admiral_garanAI : public BossAI
        {
            boss_admiral_garanAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataIronMaidens)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;

                p_Creature->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_DISABLE_COLLISION);
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            bool m_IntroDone;
            std::set<uint64> m_TrashesGuids;

            void Reset() override
            {
                me->setPowerType(Powers::POWER_ENERGY);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetPower(Powers::POWER_ENERGY, 0);

                me->CastSpell(me, eIronMaidensSpells::ZeroPowerZeroRegen, true);

                m_Events.Reset();

                RespawnMaidens(m_Instance, me);

                _Reset();

                if (!m_IntroDone)
                {
                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        std::list<Creature*> l_TrashesList;

                        me->GetCreatureListWithEntryInGrid(l_TrashesList, eIronMaidensCreatures::AquaticTechnician, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_TrashesList, eIronMaidensCreatures::IronDockworker, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_TrashesList, eIronMaidensCreatures::IronEarthbinder, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_TrashesList, eIronMaidensCreatures::IronMauler, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_TrashesList, eIronMaidensCreatures::IronCleaver, 150.0f);

                        for (Creature* l_Trash : l_TrashesList)
                        {
                            if (l_Trash->isAlive())
                                m_TrashesGuids.insert(l_Trash->GetGUID());
                        }

                        if (m_TrashesGuids.empty())
                        {
                            m_IntroDone = true;

                            AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                if (Creature* l_Sorka = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossEnforcerSorka)))
                                {
                                    if (l_Sorka->IsAIEnabled)
                                        l_Sorka->AI()->DoAction(eIronMaidensActions::ActionAfterTrashesIntro);
                                }

                                if (Creature* l_Marak = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossMarakTheBlooded)))
                                {
                                    if (l_Marak->IsAIEnabled)
                                        l_Marak->AI()->DoAction(eIronMaidensActions::ActionAfterTrashesIntro);
                                }

                                DoAction(eIronMaidensActions::ActionAfterTrashesIntro);
                            });
                        }
                    });
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_ID /*= 0*/) override
            {
                m_TrashesGuids.erase(p_Guid);

                if (!m_IntroDone && m_TrashesGuids.empty() && m_Instance != nullptr)
                {
                    m_IntroDone = true;

                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        if (Creature* l_Sorka = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossEnforcerSorka)))
                        {
                            if (l_Sorka->IsAIEnabled)
                                l_Sorka->AI()->DoAction(eIronMaidensActions::ActionAfterTrashesIntro);
                        }

                        if (Creature* l_Marak = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossMarakTheBlooded)))
                        {
                            if (l_Marak->IsAIEnabled)
                                l_Marak->AI()->DoAction(eIronMaidensActions::ActionAfterTrashesIntro);
                        }

                        DoAction(eIronMaidensActions::ActionAfterTrashesIntro);
                    });
                }
            }

            void KilledUnit(Unit* p_Killed) override
            {
                /*if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartMaidens(m_Instance, me, p_Attacker);

                _EnterCombat();

                m_Events.ScheduleEvent(eEvents::EventRapidFire, eTimers::TimerRapidFire);
                m_Events.ScheduleEvent(eEvents::EventRegenIronFury, eTimers::TimerEnergyRegen);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void EnterEvadeMode() override
            {
                WipeMaidens(m_Instance);

                CreatureAI::EnterEvadeMode();

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eIronMaidensActions::ActionIntro:
                    {
                        if (m_IntroDone)
                            break;

                        me->CastSpell(me, eSpells::IronMaidenIntroConversation, false);
                        break;
                    }
                    case eIronMaidensActions::ActionAfterTrashesIntro:
                    {
                        me->CastSpell(me, eSpells::AfterTrashesConversation, false);

                        std::list<Creature*> l_CosmeticMobs;
                        me->GetCreatureListWithEntryInGrid(l_CosmeticMobs, eIronMaidensCreatures::Ukurogg, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_CosmeticMobs, eIronMaidensCreatures::Uktar, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_CosmeticMobs, eIronMaidensCreatures::BattleMedicRogg, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_CosmeticMobs, eIronMaidensCreatures::Gorak, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_CosmeticMobs, eIronMaidensCreatures::IronEviscerator, 150.0f);

                        for (Creature* l_Mob : l_CosmeticMobs)
                            l_Mob->DespawnOrUnsummon();

                        AddTimedDelayedOperation(9 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->GetMotionMaster()->MoveJump(g_BoatBossFirstJumpPos, 30.0f, 20.0f, eMoves::MoveJump);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::EFFECT_MOTION_TYPE &&
                    p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveJump:
                    {
                        me->SetAIAnimKitId(eVisual::IntroVisual);

                        AddTimedDelayedOperation(10, [this]() -> void
                        {
                            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_FLYING);
                            me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 4.0f);
                            me->GetMotionMaster()->MoveSmoothFlyPath(eMoves::MoveDown, g_BoatBossFlyingMoves);
                        });

                        break;
                    }
                    case eMoves::MoveDown:
                    {
                        me->SetAIAnimKitId(0);

                        AddTimedDelayedOperation(10, [this]() -> void
                        {
                            me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_FLYING);
                            me->SetSpeed(UnitMoveType::MOVE_FLIGHT, me->GetCreatureTemplate()->speed_fly);
                            me->GetMotionMaster()->MoveJump(g_GaranHomePos, 30.0f, 20.0f, eMoves::MoveLast);
                        });

                        break;
                    }
                    case eMoves::MoveLast:
                    {
                        for (uint8 l_I = 0; l_I < MAX_EQUIPMENT_ITEMS * 2; ++l_I)
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEMS + l_I, 0);

                        me->SetHomePosition(g_GaranHomePos);

                        AddTimedDelayedOperation(10, [this]() -> void
                        {
                            me->SetFacingTo(g_GaranFinalFacing);
                        });

                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->LoadEquipment();

                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                            if (Creature* l_Sorka = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossEnforcerSorka)))
                                l_Sorka->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                            if (Creature* l_Marak = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossMarakTheBlooded)))
                                l_Marak->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers /*p_Power*/, int32& p_Value) override
            {
                /// Iron Maidens only regens by script
                p_Value = 0;
            }

            void SetPower(Powers p_Power, int32 p_Value) override
            {
                switch (p_Value)
                {
                    case eIronMaidensDatas::FirstIronFuryAbility:
                    {
                        m_Events.ScheduleEvent(eEvents::EventPenetratingShot, 1);
                        break;
                    }
                    case eIronMaidensDatas::SecondIronFuryAbility:
                    {
                        m_Events.ScheduleEvent(eEvents::EventDeployTurret, 1);
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
                    case eSpells::SpellIronShotSearcher:
                    {
                        me->CastSpell(p_Target, eSpells::SpellIronShotDamage, false);
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

                if (me->GetDistance(me->GetHomePosition()) >= 80.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventRapidFire:
                    {
                        if (Creature* l_Stalker = me->SummonCreature(eIronMaidensCreatures::RapidFireStalker, *me))
                        {
                            l_Stalker->DespawnOrUnsummon(11 * TimeConstants::IN_MILLISECONDS);

                            me->CastSpell(l_Stalker, eSpells::SpellRapideFirePeriodic, false);

                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            {
                                me->CastSpell(l_Target, eSpells::SpellRapidFireRedArrow, true);

                                uint64 l_TargetGUID = l_Target->GetGUID();
                                uint64 l_Guid       = l_Stalker->GetGUID();
                                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this, l_Guid, l_TargetGUID]() -> void
                                {
                                    if (Creature* l_Stalker = Creature::GetCreature(*me, l_Guid))
                                    {
                                        if (l_Stalker->IsAIEnabled)
                                            l_Stalker->AI()->SetGUID(l_TargetGUID);

                                        l_Stalker->CastSpell(l_Stalker, eSpells::SpellRapidFireTargetVisual, true);
                                    }
                                });
                            }
                        }

                        m_Events.ScheduleEvent(eEvents::EventRapidFire, eTimers::TimerRapidFireCD);
                        break;
                    }
                    case eEvents::EventRegenIronFury:
                    {
                        me->ModifyPower(Powers::POWER_ENERGY, 1);
                        m_Events.ScheduleEvent(eEvents::EventRegenIronFury, eTimers::TimerEnergyRegen);
                        break;
                    }
                    default:
                        break;
                }

                /// Must be checked again, before using Iron Shot
                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                me->CastSpell(me, eSpells::SpellIronShotSearcher, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_admiral_garanAI(p_Creature);
        }
};

/// Enforcer Sorka - 77231
class boss_enforcer_sorka : public CreatureScript
{
    public:
        boss_enforcer_sorka() : CreatureScript("boss_enforcer_sorka") { }

        enum eSpells
        {
            /// Combat Spells
            /// Blade Dash
            SpellBladeDashCast      = 155794,
            SpellBladeDashDamage    = 155841
        };

        enum eEvents
        {
            EventBladeDash = 1,
            EventRegenIronFury
        };

        enum eTimers
        {
            TimerBladeDash      = 11 * TimeConstants::IN_MILLISECONDS,
            TimerBladeDashCD    = 20 * TimeConstants::IN_MILLISECONDS,
            TimerEnergyRegen    = 7 * TimeConstants::IN_MILLISECONDS
        };

        enum eTalks
        {
        };

        enum eMove
        {
            MoveJump = 1
        };

        struct boss_enforcer_sorkaAI : public BossAI
        {
            boss_enforcer_sorkaAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataIronMaidens)
            {
                m_Instance = p_Creature->GetInstanceScript();

                p_Creature->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_DISABLE_COLLISION);
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            bool m_IsInBladeDash;
            std::set<uint64> m_BladeDashTargets;

            void Reset() override
            {
                me->setPowerType(Powers::POWER_ENERGY);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetPower(Powers::POWER_ENERGY, 0);

                me->CastSpell(me, eIronMaidensSpells::ZeroPowerZeroRegen, true);

                m_Events.Reset();

                RespawnMaidens(m_Instance, me);

                _Reset();

                m_IsInBladeDash = false;
                m_BladeDashTargets.clear();
            }

            void KilledUnit(Unit* p_Killed) override
            {
                /*if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartMaidens(m_Instance, me, p_Attacker);

                _EnterCombat();

                m_Events.ScheduleEvent(eEvents::EventBladeDash, eTimers::TimerBladeDash);
                m_Events.ScheduleEvent(eEvents::EventRegenIronFury, eTimers::TimerEnergyRegen);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void EnterEvadeMode() override
            {
                WipeMaidens(m_Instance);

                CreatureAI::EnterEvadeMode();

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eIronMaidensActions::ActionAfterTrashesIntro:
                    {
                        AddTimedDelayedOperation(13 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->GetMotionMaster()->MoveJump(g_SorkaHomePos, 30.0f, 20.0f, eMove::MoveJump);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::EFFECT_MOTION_TYPE)
                    return;

                if (p_ID == eMove::MoveJump)
                    me->SetHomePosition(g_SorkaHomePos);
            }

            void RegeneratePower(Powers /*p_Power*/, int32& p_Value) override
            {
                /// Iron Maidens only regens by script
                p_Value = 0;
            }

            void SetPower(Powers p_Power, int32 p_Value) override
            {
                switch (p_Value)
                {
                    case eIronMaidensDatas::FirstIronFuryAbility:
                    {
                        break;
                    }
                    case eIronMaidensDatas::SecondIronFuryAbility:
                    {
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
                    case eSpells::SpellBladeDashCast:
                    {
                        if (!p_Target->IsPlayer())
                            break;

                        DashToTarget(p_Target);
                        break;
                    }
                    case eSpells::SpellBladeDashDamage:
                    {
                        if (!m_IsInBladeDash)
                        {
                            if (Player* l_Target = SelectMainTank())
                                me->GetMotionMaster()->MoveChase(l_Target);

                            me->RemoveAura(eSpells::SpellBladeDashCast);
                            break;
                        }

                        /// Cannot select a player twice
                        m_BladeDashTargets.insert(p_Target->GetGUID());

                        std::list<Player*> l_PossibleTargets;
                        me->GetPlayerListInGrid(l_PossibleTargets, 8.0f);

                        if (!l_PossibleTargets.empty())
                        {
                            l_PossibleTargets.remove_if([this](Player* p_Player) -> bool
                            {
                                if (p_Player == nullptr || m_BladeDashTargets.find(p_Player->GetGUID()) != m_BladeDashTargets.end())
                                    return true;

                                return false;
                            });
                        }

                        if (l_PossibleTargets.empty())
                        {
                            m_IsInBladeDash = false;

                            if (Player* l_Target = SelectMainTank())
                                DashToTarget(l_Target);

                            break;
                        }

                        auto l_Iter = l_PossibleTargets.begin();
                        std::advance(l_Iter, urand(0, l_PossibleTargets.size() - 1));

                        DashToTarget(*l_Iter);
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

                if (me->GetDistance(me->GetHomePosition()) >= 80.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || m_IsInBladeDash)
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBladeDash:
                    {
                        m_IsInBladeDash = true;
                        m_BladeDashTargets.clear();

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 45.0f, true))
                        {
                            me->SetFacingTo(me->GetAngle(l_Target));

                            uint64 l_Guid = l_Target->GetGUID();
                            AddTimedDelayedOperation(10, [this, l_Guid]() -> void
                            {
                                if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                                    me->CastSpell(l_Target, eSpells::SpellBladeDashCast, false);
                            });
                        }

                        m_Events.ScheduleEvent(eEvents::EventBladeDash, eTimers::TimerBladeDashCD);
                        break;
                    }
                    case eEvents::EventRegenIronFury:
                    {
                        me->ModifyPower(Powers::POWER_ENERGY, 1);
                        m_Events.ScheduleEvent(eEvents::EventRegenIronFury, eTimers::TimerEnergyRegen);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void DashToTarget(Unit* p_Target)
            {
                me->NearTeleportTo(*p_Target);

                uint64 l_Guid = p_Target->GetGUID();
                AddTimedDelayedOperation(10, [this, l_Guid]() -> void
                {
                    if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                        me->CastSpell(l_Target, eSpells::SpellBladeDashDamage, true);
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_enforcer_sorkaAI(p_Creature);
        }
};

/// Marak the Blooded - 77477
class boss_marak_the_blooded : public CreatureScript
{
    public:
        boss_marak_the_blooded() : CreatureScript("boss_marak_the_blooded") { }

        enum eSpells
        {
            /// Combat Spells
            /// Blood Ritual
            SpellBloodRitualAura    = 159724,
            SpellBloodRitualCast    = 158078
        };

        enum eEvents
        {
            EventBloodRitual = 1,
            EventRegenIronFury
        };

        enum eTimers
        {
            TimerBloodRitual    = 5 * TimeConstants::IN_MILLISECONDS,
            TimerBloodRitualCD  = 20 * TimeConstants::IN_MILLISECONDS,
            TimerEnergyRegen    = 7 * TimeConstants::IN_MILLISECONDS
        };

        enum eTalks
        {
        };

        enum eVisual
        {
            IntroVisual = 6636
        };

        enum eMoves
        {
            MoveJump = 1,
            MoveDown,
            MoveLast
        };

        struct boss_marak_the_bloodedAI : public BossAI
        {
            boss_marak_the_bloodedAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataIronMaidens)
            {
                m_Instance = p_Creature->GetInstanceScript();

                p_Creature->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_DISABLE_COLLISION);
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            void Reset() override
            {
                me->setPowerType(Powers::POWER_ENERGY);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetPower(Powers::POWER_ENERGY, 0);

                me->CastSpell(me, eIronMaidensSpells::ZeroPowerZeroRegen, true);

                m_Events.Reset();

                RespawnMaidens(m_Instance, me);

                _Reset();
            }

            void KilledUnit(Unit* p_Killed) override
            {
                /*if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartMaidens(m_Instance, me, p_Attacker);

                _EnterCombat();

                m_Events.ScheduleEvent(eEvents::EventBloodRitual, eTimers::TimerBloodRitual);
                m_Events.ScheduleEvent(eEvents::EventRegenIronFury, eTimers::TimerEnergyRegen);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void EnterEvadeMode() override
            {
                WipeMaidens(m_Instance);

                CreatureAI::EnterEvadeMode();

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eIronMaidensActions::ActionAfterTrashesIntro:
                    {
                        me->GetMotionMaster()->MoveJump(g_BoatBossFirstJumpPos, 30.0f, 20.0f, eMoves::MoveJump);
                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::EFFECT_MOTION_TYPE &&
                    p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveJump:
                    {
                        me->SetAIAnimKitId(eVisual::IntroVisual);

                        AddTimedDelayedOperation(10, [this]() -> void
                        {
                            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_FLYING);
                            me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 4.0f);
                            me->GetMotionMaster()->MoveSmoothFlyPath(eMoves::MoveDown, g_BoatBossFlyingMoves);
                        });

                        break;
                    }
                    case eMoves::MoveDown:
                    {
                        me->SetAIAnimKitId(0);

                        AddTimedDelayedOperation(10, [this]() -> void
                        {
                            me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_FLYING);
                            me->SetSpeed(UnitMoveType::MOVE_FLIGHT, me->GetCreatureTemplate()->speed_fly);
                            me->GetMotionMaster()->MoveJump(g_MarakHomePos, 30.0f, 20.0f, eMoves::MoveLast);
                        });

                        break;
                    }
                    case eMoves::MoveLast:
                    {
                        me->SetHomePosition(g_MarakHomePos);

                        AddTimedDelayedOperation(10, [this]() -> void
                        {
                            me->SetFacingTo(g_MarakFinalFacing);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers /*p_Power*/, int32& p_Value) override
            {
                /// Iron Maidens only regens by script
                p_Value = 0;
            }

            void SetPower(Powers p_Power, int32 p_Value) override
            {
                switch (p_Value)
                {
                    case eIronMaidensDatas::FirstIronFuryAbility:
                    {
                        break;
                    }
                    case eIronMaidensDatas::SecondIronFuryAbility:
                    {
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::SpellBloodRitualCast:
                    {
                        std::list<Player*> l_PlayerList;
                        me->GetPlayerListInGrid(l_PlayerList, 45.0f);

                        if (!l_PlayerList.empty())
                        {
                            l_PlayerList.remove_if([this](Player* p_Player) -> bool
                            {
                                if (p_Player == nullptr || !p_Player->isInFront(me))
                                    return true;

                                return false;
                            });
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

                if (!UpdateVictim())
                    return;

                if (me->GetDistance(me->GetHomePosition()) >= 80.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBloodRitual:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 45.0f, true))
                        {
                            me->CastSpell(l_Target, eSpells::SpellBloodRitualAura, true);
                            me->CastSpell(l_Target, eSpells::SpellBloodRitualCast, false);
                        }

                        m_Events.ScheduleEvent(eEvents::EventBloodRitual, eTimers::TimerBloodRitualCD);
                        break;
                    }
                    case eEvents::EventRegenIronFury:
                    {
                        me->ModifyPower(Powers::POWER_ENERGY, 1);
                        m_Events.ScheduleEvent(eEvents::EventRegenIronFury, eTimers::TimerEnergyRegen);
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
            return new boss_marak_the_bloodedAI(p_Creature);
        }
};

/// Loading Chain - 78767
class npc_foundry_loading_chain : public CreatureScript
{
    public:
        npc_foundry_loading_chain() : CreatureScript("npc_foundry_loading_chain") { }

        enum eSpells
        {
            RideLoadingChain    = 158646,
            LoadingChainVisual  = 159086,
            LoadCrate           = 171209
        };

        enum eMoves
        {
            MoveBoss = 1,
            MoveBoat
        };

        struct npc_foundry_loading_chainAI : public ScriptedAI
        {
            npc_foundry_loading_chainAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_ChainID(0) { }

            uint8 m_ChainID;
            bool m_IsAvailable;

            void Reset() override
            {
                m_IsAvailable = true;

                me->CastSpell(me, eSpells::LoadingChainVisual, true);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetUInt32Value(EUnitFields::UNIT_FIELD_INTERACT_SPELL_ID, eSpells::RideLoadingChain);

                /// Init chain ID
                for (Position const l_Pos : g_LoadingChainsSpawnPos)
                {
                    if (me->IsNearPosition(&l_Pos, 0.5f))
                        break;

                    ++m_ChainID;
                }
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo) override
            {
                if (p_SpellInfo->Id == eSpells::LoadCrate && m_ChainID < eIronMaidensDatas::MaxLoadingChains)
                    me->GetMotionMaster()->MovePoint(eMoves::MoveBoat, g_LoadingChainsMoveBoatPos[m_ChainID]);
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveBoss:
                    {
                        m_IsAvailable = true;
                        break;
                    }
                    case eMoves::MoveBoat:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveAura(eSpells::LoadCrate);

                            me->GetMotionMaster()->MovePoint(eMoves::MoveBoss, g_LoadingChainsSpawnPos[m_ChainID]);
                        });

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
                    case eIronMaidensDatas::LoadingChainAvailable:
                        m_IsAvailable = p_Value != 0;
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_ID = 0) override
            {
                switch (p_ID)
                {
                    case eIronMaidensDatas::LoadingChainID:
                        return uint32(m_ChainID);
                    case eIronMaidensDatas::LoadingChainAvailable:
                        return uint32(m_IsAvailable);
                    default:
                        return 0;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_loading_chainAI(p_Creature);
        }
};

/// Uk'urogg <Deckhand of Marak the Blooded> - 78341
class npc_foundry_ukurogg : public CreatureScript
{
    public:
        npc_foundry_ukurogg() : CreatureScript("npc_foundry_ukurogg") { }

        enum eSpells
        {
            CarryingCrate = 171198
        };

        enum eEvents
        {
        };

        enum eMoves
        {
            MoveCarryCrate = 1,
            MoveThrowCrate
        };

        struct npc_foundry_ukuroggAI : public ScriptedAI
        {
            npc_foundry_ukuroggAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            Position const m_UkuroggThrowCratePos = { 495.576f, 3273.3f, 141.388f, 0.0f };
            Position const m_UkuroggCarryCratePos = { 478.2083f, 3280.669f, 141.388f, 0.0f };

            float const m_UkuroggFinalFacing = 2.859696f;

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetWalk(false);

                ClearDelayedOperations();
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::CarryingCrate)
                {
                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->SetWalk(true);

                        me->GetMotionMaster()->MovePoint(eMoves::MoveThrowCrate, m_UkuroggThrowCratePos);
                    });
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveCarryCrate:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->SetFacingTo(m_UkuroggFinalFacing);
                        });

                        break;
                    }
                    case eMoves::MoveThrowCrate:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveAura(eSpells::CarryingCrate);

                            me->GetMotionMaster()->MovePoint(eMoves::MoveCarryCrate, m_UkuroggCarryCratePos);
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_ukuroggAI(p_Creature);
        }
};

/// Zipline Stalker - 82538
class npc_foundry_zipline_stalker : public CreatureScript
{
    public:
        npc_foundry_zipline_stalker() : CreatureScript("npc_foundry_zipline_stalker") { }

        enum eSpell
        {
            ZiplineStalkerVisual = 166239
        };

        enum eMoves
        {
            MoveNone,
            MoveSecond
        };

        struct npc_foundry_zipline_stalkerAI : public ScriptedAI
        {
            npc_foundry_zipline_stalkerAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit()) { }

            Vehicle* m_Vehicle;

            void Reset() override
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->CastSpell(me, eSpell::ZiplineStalkerVisual, true);
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                /*if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE || m_Vehicle == nullptr)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveSecond:
                    {
                        for (int8 l_SeatID = 0; l_SeatID < MAX_VEHICLE_SEATS; ++l_SeatID)
                        {
                            if (m_Vehicle->GetPassenger(l_SeatID) == nullptr)
                                continue;

                            if (Creature* l_Passenger = m_Vehicle->GetPassenger(l_SeatID)->ToCreature())
                            {
                                if (l_Passenger->IsAIEnabled)
                                    l_Passenger->AI()->DoAction(eIronMaidensActions::ActionZiplineArrived);
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }*/
            }

            void PassengerBoarded(Unit* p_Passenger, int8 p_SeatID, bool p_Apply) override
            {
                /*if (p_Apply)
                {
                    if (p_Passenger->GetEntry() == eFoundryCreatures::BossAdmiralGaran)
                    {
                        if (Creature* l_Garan = p_Passenger->ToCreature())
                        {
                            if (l_Garan->IsAIEnabled)
                                l_Garan->AI()->DoAction(eIronMaidensActions::ActionEnteredZipline);
                        }
                    }
                }*/
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_zipline_stalkerAI(p_Creature);
        }
};

/// Rapid Fire Stalker - 77636
class npc_foundry_rapid_fire_stalker : public CreatureScript
{
    public:
        npc_foundry_rapid_fire_stalker() : CreatureScript("npc_foundry_rapid_fire_stalker") { }

        struct npc_foundry_rapid_fire_stalkerAI : public ScriptedAI
        {
            npc_foundry_rapid_fire_stalkerAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_TargetGUID(0) { }

            uint64 m_TargetGUID;

            void SetGUID(uint64 p_Guid, int32 p_ID /*= 0*/) override
            {
                m_TargetGUID = p_Guid;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (Unit* l_Target = Unit::GetUnit(*me, m_TargetGUID))
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(0, *l_Target, false);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_rapid_fire_stalkerAI(p_Creature);
        }
};

/// Blood Ritual - 158078
class spell_foundry_blood_ritual : public SpellScriptLoader
{
    public:
        spell_foundry_blood_ritual() : SpellScriptLoader("spell_foundry_blood_ritual") { }

        enum eSpell
        {
            CrystallizedBlood = 158080
        };

        class spell_foundry_blood_ritual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_blood_ritual_SpellScript)

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                p_Targets.sort(JadeCore::ObjectDistanceOrderPred(l_Caster));

                if (Unit* l_Target = (*p_Targets.begin())->ToUnit())
                    l_Caster->CastSpell(l_Target, eSpell::CrystallizedBlood, true);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_blood_ritual_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_blood_ritual_SpellScript();
        }
};

/// Iron Maidens Boat - 9945
class areatrigger_at_foundry_iron_maidens_boat : public AreaTriggerScript
{
    public:
        areatrigger_at_foundry_iron_maidens_boat() : AreaTriggerScript("areatrigger_at_foundry_iron_maidens_boat") { }

        enum eSpell
        {
            OnABoatPeriodic = 158726
        };

        void OnEnter(Player* p_Player, AreaTriggerEntry const* /*p_AreaTrigger*/) override
        {
            p_Player->CastSpell(p_Player, eSpell::OnABoatPeriodic, true);
        }

        void OnExit(Player* p_Player, AreaTriggerEntry const* /*p_AreaTrigger*/) override
        {
            p_Player->RemoveAura(eSpell::OnABoatPeriodic);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_iron_maidens()
{
    /// Bosses
    new boss_admiral_garan();
    new boss_enforcer_sorka();
    new boss_marak_the_blooded();

    /// Creatures
    new npc_foundry_loading_chain();
    new npc_foundry_ukurogg();
    new npc_foundry_zipline_stalker();
    new npc_foundry_rapid_fire_stalker();

    /// Spell
    new spell_foundry_blood_ritual();

    /// AreaTrigger (world)
    new areatrigger_at_foundry_iron_maidens_boat();
}
#endif
