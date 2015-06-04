////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

Position const g_PolMovePos[3] =
{
    { 4031.884f, 8587.818f, 343.6748f, 4.55423f },
    { 4028.196f, 8568.284f, 341.8094f, 4.51496f },
    { 4012.667f, 8523.617f, 327.2006f, 5.63415f }
};

Position const g_PolJumpPos = { 4043.08f, 8500.94f, 322.226f, 5.63415f };

Position const g_PhemosMovePos[3] =
{
    { 3949.489f, 8484.431f, 343.7032f, 0.31307f },
    { 3974.303f, 8497.139f, 335.4293f, 0.40340f },
    { 4002.597f, 8510.825f, 327.1879f, 5.63415f }
};

Position const g_PhemosJumpPos = { 4028.90f, 8485.51f, 322.226f, 5.63415f };

float g_GorthenonFloor = 330.0f;

void RespawnOgrons(Creature* p_Source, InstanceScript* p_Instance)
{
    if (p_Source == nullptr || p_Instance == nullptr)
        return;

    if (Creature* l_Other = Creature::GetCreature(*p_Source, (p_Source->GetEntry() == eHighmaulCreatures::Phemos) ? eHighmaulCreatures::Pol : eHighmaulCreatures::Phemos))
    {
        l_Other->Respawn();
        l_Other->GetMotionMaster()->MoveTargetedHome();
    }
}

void StartOgrons(Creature* p_Source, Unit* p_Target)
{
    if (p_Source == nullptr || p_Target == nullptr)
        return;

    if (Creature* l_Other = p_Source->FindNearestCreature((p_Source->GetEntry() == eHighmaulCreatures::Phemos) ? eHighmaulCreatures::Pol : eHighmaulCreatures::Phemos, 30.0f))
        l_Other->AI()->AttackStart(p_Target);
}

/// Pol - 78238
class boss_twin_ogron_pol : public CreatureScript
{
    public:
        boss_twin_ogron_pol() : CreatureScript("boss_twin_ogron_pol") { }

        enum eSpells
        {
            WarmingUp               = 173425,
            AggressiveDisposition   = 157951,
            /// Interrupting Shout
            InterruptingShout       = 158093,
            InterruptingShoutDmg    = 158102
        };

        enum eEvents
        {
        };

        enum eActions
        {
            SchedulePulverize,
            ScheduleInterruptingShout,
            ScheduleShieldCharge
        };

        enum eCreatures
        {
        };

        enum eTalks
        {
            Intro,
            Aggro,
            InterruptingShout,
            ShieldCharge,
            Pulverize,
            Slay
        };

        enum eMoves
        {
            MoveFirst = 10,
            MoveSecond,
            MoveThird,
            MoveJump
        };

        struct boss_twin_ogron_polAI : public BossAI
        {
            boss_twin_ogron_polAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTwinOgron)
            {
                m_Instance  = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            bool m_ShieldChargeScheduled;
            bool m_InterruptingShoutScheduled;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->SetPower(Powers::POWER_ENERGY, 0);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                me->CastSpell(me, eSpells::AggressiveDisposition, true);

                m_ShieldChargeScheduled = false;
                m_InterruptingShoutScheduled = false;
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::SchedulePulverize:
                    {
                        m_InterruptingShoutScheduled = false;
                        m_ShieldChargeScheduled = false;
                        break;
                    }
                    case eActions::ScheduleInterruptingShout:
                    {
                        if (m_InterruptingShoutScheduled)
                            break;

                        m_InterruptingShoutScheduled = true;
                        me->CastSpell(me, eSpells::InterruptingShout, false);
                        break;
                    }
                    case eActions::ScheduleShieldCharge:
                    {
                        if (m_ShieldChargeScheduled)
                            break;

                        m_ShieldChargeScheduled = true;
                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalks::Aggro);

                _EnterCombat();

                me->CastSpell(me, eSpells::WarmingUp, true);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                StartOgrons(me, p_Attacker);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    if (IsLFR())
                    {
                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eHighmaulDatas::BossTwinOgron, EncounterState::FAIL);
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    RespawnOgrons(me, m_Instance);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eMoves::MoveFirst:
                        me->GetMotionMaster()->MovePoint(eMoves::MoveSecond, g_PolMovePos[1]);
                        break;
                    case eMoves::MoveSecond:
                        me->GetMotionMaster()->MovePoint(eMoves::MoveThird, g_PolMovePos[2]);
                        break;
                    case eMoves::MoveThird:
                        me->SetHomePosition(*me);
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->SetFacingTo(g_PolMovePos[2].m_orientation); });
                        break;
                    case eMoves::MoveJump:
                        me->SetHomePosition(*me);
                        break;
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
                    case eSpells::InterruptingShout:
                        me->CastSpell(p_Target, eSpells::InterruptingShoutDmg, true);
                        break;
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value) override
            {
                /// Pol only regens by script
                p_Value = 0;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                if (!me->HasAura(eSpells::WarmingUp))
                    m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                /*switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }*/

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_twin_ogron_polAI(p_Creature);
        }
};

/// Phemos - 78237
class boss_twin_ogron_phemos : public CreatureScript
{
    public:
        boss_twin_ogron_phemos() : CreatureScript("boss_twin_ogron_phemos") { }

        enum eSpells
        {
            /// Misc
            WarmingUp               = 173425,
            AggressiveDisposition   = 157951,
            /// Quake
            SpellQuake              = 158200,
            BlazeWeaponVisual       = 158206,
            /// Enfeebling Roar
            SpellEnfeeblingRoar     = 158057,
            EnfeeblingRoarDebuff    = 158026,
            /// Whirlwind
            SpellWhirlwind          = 157943,
            WeakenedDefenses        = 159709,

            DoubleSlashMainHand     = 158521,
            DoubleSlashOffHand      = 167198
        };

        enum eEvents
        {
            EventCheckPlayer = 1,
            EventBerserker,
            EventDoubleSlash
        };

        enum eActions
        {
            ActionMoveHome,
            ScheduleQuake,
            ScheduleEnfeeblingRoar,
            ScheduleWhirlwind
        };

        enum eCreature
        {
            InvisibleStalker = 15214
        };

        enum eTalks
        {
            Intro,
            Aggro,
            Whirlwind,
            Quake,
            EnfeeblingRoar,
            Berserk,
            Slay,
            Wipe
        };

        enum eMoves
        {
            MoveFirst = 10,
            MoveSecond,
            MoveThird,
            MoveJump
        };

        enum eVisuals
        {
            QuakeVisualID       = 41369,
            QuakeSpellVisual    = 37816
        };

        enum eMisc
        {
            EnfeeblingCounter
        };

        struct boss_twin_ogron_phemosAI : public BossAI
        {
            boss_twin_ogron_phemosAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTwinOgron)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_Init = false;
                m_HomeChanged = false;
            }

            EventMap m_CosmeticEvents;
            EventMap m_Events;
            InstanceScript* m_Instance;

            std::set<uint64> m_TrashsMobs;
            bool m_Init;
            bool m_HomeChanged;

            bool m_WhirlwindScheduled;
            bool m_EnfeeblingRoarScheduled;

            uint32 m_EnfeeblingRoarCounter;

            void Reset() override
            {
                if (!m_HomeChanged)
                    m_CosmeticEvents.ScheduleEvent(eEvents::EventCheckPlayer, 1 * TimeConstants::IN_MILLISECONDS);

                m_Events.Reset();

                _Reset();

                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetPower(Powers::POWER_ENERGY, 98);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                me->CastSpell(me, eSpells::AggressiveDisposition, true);

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (m_TrashsMobs.empty() && !m_Init)
                    {
                        std::list<Creature*> l_TrashList;
                        me->GetCreatureListInGrid(l_TrashList, 250.0f);

                        if (!l_TrashList.empty())
                        {
                            l_TrashList.remove_if([this](Creature* p_Creature) -> bool
                            {
                                if (p_Creature == nullptr || p_Creature->GetPositionZ() > g_GorthenonFloor)
                                    return true;

                                if (p_Creature->GetEntry() == eCreature::InvisibleStalker)
                                    return true;

                                if (!p_Creature->isAlive())
                                    return true;

                                return false;
                            });
                        }

                        for (Creature* l_Creature : l_TrashList)
                            m_TrashsMobs.insert(l_Creature->GetGUID());

                        m_Init = true;
                    }
                });

                m_WhirlwindScheduled = false;
                m_EnfeeblingRoarScheduled = false;

                m_EnfeeblingRoarCounter = 0;
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionMoveHome:
                    {
                        DoMoveHome();
                        break;
                    }
                    case eActions::ScheduleQuake:
                    {
                        Talk(eTalks::Quake);

                        me->SetPower(Powers::POWER_ENERGY, 0);
                        me->CastSpell(me, eSpells::SpellQuake, false);

                        m_EnfeeblingRoarScheduled = false;
                        m_WhirlwindScheduled = false;

                        Position l_Pos;
                        me->GetRandomNearPosition(l_Pos, 30.0f);

                        me->CastSpell(l_Pos, eSpells::BlazeWeaponVisual, true);
                        me->SendPlaySpellVisualKit(eVisuals::QuakeVisualID, 0, 0);

                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this, l_Pos]() -> void
                        {
                            me->SendPlaySpellVisual(eVisuals::QuakeSpellVisual, nullptr, 0.75f, 0.0f, l_Pos, false, true);
                        });

                        me->GetRandomNearPosition(l_Pos, 30.0f);

                        AddTimedDelayedOperation(1500, [this, l_Pos]() -> void
                        {
                            me->CastSpell(l_Pos, eSpells::BlazeWeaponVisual, true);
                            me->SendPlaySpellVisualKit(eVisuals::QuakeVisualID, 0, 0);
                        });

                        AddTimedDelayedOperation(2500, [this, l_Pos]() -> void
                        {
                            me->SendPlaySpellVisual(eVisuals::QuakeSpellVisual, nullptr, 0.75f, 0.0f, l_Pos, false, true);
                        });

                        break;
                    }
                    case eActions::ScheduleEnfeeblingRoar:
                    {
                        if (m_EnfeeblingRoarScheduled)
                            break;

                        m_EnfeeblingRoarCounter = 0;
                        Talk(eTalks::EnfeeblingRoar);
                        me->CastSpell(me, eSpells::SpellEnfeeblingRoar, false);
                        m_EnfeeblingRoarScheduled = true;
                        break;
                    }
                    case eActions::ScheduleWhirlwind:
                    {
                        if (m_WhirlwindScheduled)
                            break;

                        Talk(eTalks::Whirlwind);
                        me->CastSpell(me, eSpells::SpellWhirlwind, false);
                        m_WhirlwindScheduled = true;
                        break;
                    }
                    default:
                        break;
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
                m_TrashsMobs.erase(p_Guid);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalks::Aggro);

                _EnterCombat();

                me->CastSpell(me, eSpells::WarmingUp, true);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                if (IsMythic())
                {
                    m_Events.ScheduleEvent(eEvents::EventBerserker, 420 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventDoubleSlash, 28 * TimeConstants::IN_MILLISECONDS);
                }
                else if (IsHeroic())
                {
                    m_Events.ScheduleEvent(eEvents::EventBerserker, 480 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventDoubleSlash, 31 * TimeConstants::IN_MILLISECONDS);
                }
                else
                {
                    m_Events.ScheduleEvent(eEvents::EventBerserker, 480 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventDoubleSlash, 33 * TimeConstants::IN_MILLISECONDS);
                }

                StartOgrons(me, p_Attacker);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    if (IsLFR())
                    {
                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }
            }

            void EnterEvadeMode() override
            {
                Talk(eTalks::Wipe);

                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eHighmaulDatas::BossTwinOgron, EncounterState::FAIL);
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    RespawnOgrons(me, m_Instance);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eMoves::MoveFirst:
                        me->GetMotionMaster()->MovePoint(eMoves::MoveSecond, g_PhemosMovePos[1]);
                        break;
                    case eMoves::MoveSecond:
                        me->GetMotionMaster()->MovePoint(eMoves::MoveThird, g_PhemosMovePos[2]);
                        break;
                    case eMoves::MoveThird:
                    {
                        me->SetHomePosition(*me);
                        m_HomeChanged = true;

                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->SetFacingTo(g_PhemosMovePos[2].m_orientation);
                        });

                        if (m_TrashsMobs.empty())
                        {
                            AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                DoMoveHome();
                            });
                        }

                        break;
                    }
                    case eMoves::MoveJump:
                        me->SetHomePosition(*me);
                        break;
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
                    case eSpells::SpellWhirlwind:
                        me->CastSpell(p_Target, eSpells::WeakenedDefenses, true);
                        break;
                    case eSpells::SpellEnfeeblingRoar:
                        ++m_EnfeeblingRoarCounter;
                        me->CastSpell(p_Target, eSpells::EnfeeblingRoarDebuff, true);
                        break;
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value) override
            {
                /// Phemos only regens by script
                p_Value = 0;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    case eEvents::EventCheckPlayer:
                    {
                        if (me->FindNearestPlayer(150.0f))
                            DoFirstMove();
                        else
                            m_CosmeticEvents.ScheduleEvent(eEvents::EventCheckPlayer, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                if (!me->HasAura(eSpells::WarmingUp))
                    m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eHighmaulSpells::Berserker, true);
                        Talk(eTalks::Berserk);
                        break;
                    }
                    case eEvents::EventDoubleSlash:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::DoubleSlashMainHand, false);

                            uint64 l_Guid = l_Target->GetGUID();
                            AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                            {
                                if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                                    me->CastSpell(l_Target, eSpells::DoubleSlashOffHand, true);
                            });
                        }

                        if (IsMythic())
                            m_Events.ScheduleEvent(eEvents::EventDoubleSlash, 28 * TimeConstants::IN_MILLISECONDS);
                        else if (IsHeroic())
                            m_Events.ScheduleEvent(eEvents::EventDoubleSlash, 31 * TimeConstants::IN_MILLISECONDS);
                        else
                            m_Events.ScheduleEvent(eEvents::EventDoubleSlash, 33 * TimeConstants::IN_MILLISECONDS);

                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            uint32 GetData(uint32 p_ID) override
            {
                if (p_ID == eMisc::EnfeeblingCounter)
                    return m_EnfeeblingRoarCounter;
            }

            void DoFirstMove()
            {
                Talk(eTalks::Intro);
                me->GetMotionMaster()->MovePoint(eMoves::MoveFirst, g_PhemosMovePos[0]);

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Pol = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Pol)))
                    {
                        l_Pol->GetMotionMaster()->MovePoint(eMoves::MoveFirst, g_PolMovePos[0]);

                        uint64 l_Guid = l_Pol->GetGUID();
                        AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                        {
                            if (Creature* l_Boss = Creature::GetCreature(*me, l_Guid))
                            {
                                if (l_Boss->IsAIEnabled)
                                    l_Boss->AI()->Talk(eTalks::Intro);
                            }
                        });
                    }
                }
            }

            void DoMoveHome()
            {
                if (!m_TrashsMobs.empty())
                    return;

                me->GetMotionMaster()->MoveJump(g_PhemosJumpPos, 30.0f, 20.0f, eMoves::MoveJump);

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Pol = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Pol)))
                        l_Pol->GetMotionMaster()->MoveJump(g_PolJumpPos, 30.0f, 20.0f, eMoves::MoveJump);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_twin_ogron_phemosAI(p_Creature);
        }
};

/// Warming Up - 173425
class spell_highmaul_warming_up : public SpellScriptLoader
{
    public:
        spell_highmaul_warming_up() : SpellScriptLoader("spell_highmaul_warming_up") { }

        class spell_highmaul_warming_up_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_warming_up_AuraScript);

            enum eSpell
            {
                Disposition = 157953
            };

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpell::Disposition, true);
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_warming_up_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_warming_up_AuraScript();
        }
};

/// Disposition - 157953
class spell_highmaul_disposition : public SpellScriptLoader
{
    public:
        spell_highmaul_disposition() : SpellScriptLoader("spell_highmaul_disposition") { }

        class spell_highmaul_disposition_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_disposition_AuraScript);

            enum eActions
            {
                /// Phemos
                ScheduleQuake               = 1,
                ScheduleEnfleeblingRoar     = 2,
                ScheduleWhirlwind           = 3,
                /// Pol
                SchedulePulverize           = 0,
                ScheduleInterruptingShout   = 1,
                ScheduleShieldCharge        = 2
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (GetTarget() == nullptr)
                    return;

                if (Creature* l_Boss = GetTarget()->ToCreature())
                {
                    l_Boss->EnergizeBySpell(l_Boss, GetSpellInfo()->Id, 1, Powers::POWER_ENERGY);

                    if (!l_Boss->IsAIEnabled)
                        return;

                    switch (l_Boss->GetEntry())
                    {
                        case eHighmaulCreatures::Phemos:
                        {
                            if (l_Boss->GetPower(Powers::POWER_ENERGY) >= 100)
                                l_Boss->AI()->DoAction(eActions::ScheduleQuake);
                            else if (l_Boss->GetPower(Powers::POWER_ENERGY) >= 66 && l_Boss->GetPower(Powers::POWER_ENERGY) <= 80)
                                l_Boss->AI()->DoAction(eActions::ScheduleEnfleeblingRoar);
                            else if (l_Boss->GetPower(Powers::POWER_ENERGY) >= 33 && l_Boss->GetPower(Powers::POWER_ENERGY) <= 50)
                                l_Boss->AI()->DoAction(eActions::ScheduleWhirlwind);

                            break;
                        }
                        case eHighmaulCreatures::Pol:
                        {
                            if (l_Boss->GetPower(Powers::POWER_ENERGY) >= 100)
                                l_Boss->AI()->DoAction(eActions::SchedulePulverize);
                            else if (l_Boss->GetPower(Powers::POWER_ENERGY) >= 66 && l_Boss->GetPower(Powers::POWER_ENERGY) <= 80)
                                l_Boss->AI()->DoAction(eActions::ScheduleInterruptingShout);
                            else if (l_Boss->GetPower(Powers::POWER_ENERGY) >= 33 && l_Boss->GetPower(Powers::POWER_ENERGY) <= 50)
                                l_Boss->AI()->DoAction(eActions::ScheduleShieldCharge);

                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_disposition_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_disposition_AuraScript();
        }
};

/// Enfeebling Roar - 158026
class spell_highmaul_enfeebling_roar : public SpellScriptLoader
{
    public:
        spell_highmaul_enfeebling_roar() : SpellScriptLoader("spell_highmaul_enfeebling_roar") { }

        class spell_highmaul_enfeebling_roar_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_enfeebling_roar_AuraScript);

            enum eMisc
            {
                EnfeeblingCounter
            };

            void AfterApply(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Creature* l_Phemos = GetCaster()->ToCreature())
                {
                    if (!l_Phemos->IsAIEnabled)
                        return;

                    AuraPtr l_Aura = p_AurEff->GetBase();
                    if (!l_Aura)
                        return;

                    uint32 l_Count = l_Phemos->AI()->GetData(eMisc::EnfeeblingCounter);
                    if (!l_Count)
                        return;

                    uint32 l_MaxDuration = l_Aura->GetDuration() * 10;
                    int32 l_Amount = p_AurEff->GetAmount() * 10;

                    l_Aura->SetDuration(l_MaxDuration / l_Count);
                    l_Aura->SetMaxDuration(l_MaxDuration / l_Count);
                    l_Aura->GetEffect(EFFECT_1)->ChangeAmount(l_Amount / l_Count);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_highmaul_enfeebling_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_enfeebling_roar_AuraScript();
        }
};

void AddSC_boss_twin_ogron()
{
    /// Bosses
    new boss_twin_ogron_pol();
    new boss_twin_ogron_phemos();

    /// Spells
    new spell_highmaul_warming_up();
    new spell_highmaul_disposition();
    new spell_highmaul_enfeebling_roar();
}