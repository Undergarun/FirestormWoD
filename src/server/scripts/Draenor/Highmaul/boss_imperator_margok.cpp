////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "highmaul.hpp"

/// Imperator Mar'gok <Sorcerer King> - 77428
class boss_imperator_margok : public CreatureScript
{
    public:
        boss_imperator_margok() : CreatureScript("boss_imperator_margok") { }

        enum eSpells
        {
            /// Cosmetic
            CosmeticSitThrone           = 88648,
            TeleportOffThrone           = 166090,
            /// Mark of Chaos
            MarkOfChaosAura             = 158605,
            /// AcceleratedAssault
            AcceleratedAssault          = 159515,
            /// Arcane Aberration
            SummonArcaneAberrationCast  = 156471,
            SummonArcaneAberrationCosm  = 164318,   ///< Sniffed, but I don't know why
            /// Destructive Resonance
            DestructiveResonanceSearch  = 156467,
            DestructiveResonanceCosm    = 164074,   ///< Sniffed, but I don't know why
            DestructiveResonanceSummon  = 156734
        };

        enum eEvents
        {
            /// Phase 1
            EventMarkOfChaos = 1,
            EventForceNova,
            EventArcaneWrath,
            EventDestructiveResonance,
            EventArcaneAberration,
            EventBerserk
        };

        enum eActions
        {
            ActionIntro,
            ActionFinishIntro
        };

        enum eCreature
        {
            SorcererKingVisualPoint = 89081
        };

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3,
            Aggro,
            ForceNova,
            MarkOfChaos,
            ArcaneWrath,
            ArcaneAberration,
            TalkRuneOfDisplacement,
            TalkRuneOfFortification,
            TalkRuneOfReplication,
            Slay,
            Berserk,
            Death
        };

        enum eAnimKit
        {
        };

        enum eMove
        {
        };

        enum eData
        {
            MaxVisualPoint = 8
        };

        enum ePhases
        {
            MightOfTheCrown,        ///< Phase 1: Might of the Crown
            RuneOfDisplacement,     ///< Phase 2: Rune of Displacement
            DormantRunestones,      ///< Intermission: Dormant Runestones
            RuneOfFortification,    ///< Phase 3: Rune of Fortification
            LineageOfPower,         ///< Intermission: Lineage of Power
            RuneOfReplication,      ///< Phase 4: Rune of Replication
            MaxPhases
        };

        struct boss_imperator_margokAI : public BossAI
        {
            boss_imperator_margokAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossImperatorMargok), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = p_Creature->GetInstanceScript();

                g_SwitchPhasePcts[ePhases::MightOfTheCrown]     = 85;   ///< Phase One lasts from 100 % to 85 % of Mar'gok's health.
                g_SwitchPhasePcts[ePhases::RuneOfDisplacement]  = 55;   ///< Phase Two lasts from 85 % to 55 % of Mar'gok's health.
                g_SwitchPhasePcts[ePhases::DormantRunestones]   = -1;   ///< The First Transition Phase lasts for 1 minute after the end of Phase Two.
                g_SwitchPhasePcts[ePhases::RuneOfFortification] = 25;   ///< Phase Three lasts from 55 % of Mar'gok's health(end of the First Transition Phase) to 25 % of Mar'gok's health.
                g_SwitchPhasePcts[ePhases::LineageOfPower]      = -1;   ///< The Second transition Phase lasts for 1 minute after the end of Phase Three.
                g_SwitchPhasePcts[ePhases::RuneOfReplication]   = 0;    ///< Phase Four lasts from 25 % of Mar'gok's health(end of the Second Transition Phase) to Mar'gok's death.
            }

            EventMap m_Events;

            bool m_InCombat;

            uint8 m_Phase;
            uint8 g_SwitchPhasePcts[ePhases::MaxPhases];

            uint64 m_MeleeTargetGuid;

            Vehicle* m_Vehicle;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                summons.DespawnAll();

                m_InCombat = false;

                m_Phase = ePhases::MightOfTheCrown;
                m_Events.SetPhase(m_Phase);

                me->RemoveAura(eSpells::AcceleratedAssault);
                me->RemoveAura(eHighmaulSpells::Berserker);

                _Reset();

                AddTimedDelayedOperation(200, [this]() -> void
                {
                    me->CastSpell(me, eSpells::CosmeticSitThrone, true);

                    for (uint8 l_I = 0; l_I < eData::MaxVisualPoint; ++l_I)
                    {
                        if (Creature* l_Creature = me->SummonCreature(eCreature::SorcererKingVisualPoint, *me))
                            l_Creature->EnterVehicle(me);
                    }
                });
            }

            void JustReachedHome() override
            {
                m_Events.Reset();
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionIntro:
                    {
                        AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Talk(eTalks::Intro1);
                        });

                        AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Talk(eTalks::Intro2);
                        });

                        break;
                    }
                    case eActions::ActionFinishIntro:
                    {
                        Talk(eTalks::Intro3);
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
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                Talk(eTalks::Aggro);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                m_Events.ScheduleEvent(eEvents::EventMarkOfChaos, 34 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                m_Events.ScheduleEvent(eEvents::EventForceNova, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                m_Events.ScheduleEvent(eEvents::EventArcaneWrath, 6 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                m_Events.ScheduleEvent(eEvents::EventDestructiveResonance, 15 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                m_Events.ScheduleEvent(eEvents::EventArcaneAberration, 25 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);

                if (IsLFR())
                    m_Events.ScheduleEvent(eEvents::EventBerserk, 900 * TimeConstants::IN_MILLISECONDS);
            }

            void DamageDealt(Unit* p_Victim, uint32& p_Damage, DamageEffectType p_DamageType) override
            {
                /// Accelerated Assault Icon Accelerated Assault is a stacking buff that Mar'gok applies to himself each time he performs consecutive attacks against the same target.
                /// Each stack of the buff increases Mar'gok's attack speed by 8%.
                if (p_DamageType != DamageEffectType::DIRECT_DAMAGE)
                    return;

                /// This is a new target, reset stacks
                if (p_Victim->GetGUID() != m_MeleeTargetGuid)
                {
                    me->RemoveAura(eSpells::AcceleratedAssault);
                    m_MeleeTargetGuid = p_Victim->GetGUID();
                }
                /// Still same target, continue stacking
                else
                    me->CastSpell(me, eSpells::AcceleratedAssault, true);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                /// -1 means transition phase, 0 means last phase, until Mar'gok's death
                if (g_SwitchPhasePcts[m_Phase] == -1 || !g_SwitchPhasePcts[m_Phase])
                    return;

                if (me->HealthBelowPctDamaged(g_SwitchPhasePcts[m_Phase], p_Damage))
                {
                    ++m_Phase;

                    if (m_Phase == ePhases::RuneOfDisplacement)
                        Talk(eTalks::TalkRuneOfDisplacement);

                    m_Events.SetPhase(m_Phase);
                }
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                Talk(eTalks::Death);

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosAura);

                    if (IsLFR())
                    {
                        Map::PlayerList const& l_PlayerList = m_Instance->instance->GetPlayers();
                        if (l_PlayerList.isEmpty())
                            return;

                        for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                        {
                            if (Player* l_Player = l_Itr->getSource())
                            {
                                uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                if (!me || l_Player->IsAtGroupRewardDistance(me))
                                    sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);
                            }
                        }

                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }
            }

            void EnterEvadeMode() override
            {
                me->RemoveAllAuras();

                me->InterruptNonMeleeSpells(true);

                /// Just in case, to prevent the fail Return to Home
                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);
                me->ClearUnitState(UnitState::UNIT_STATE_DISTRACTED);
                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);

                CreatureAI::EnterEvadeMode();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eHighmaulDatas::BossImperatorMargok, EncounterState::FAIL);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
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

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::DestructiveResonanceSearch:
                    {
                        me->CastSpell(*p_Target, eSpells::DestructiveResonanceSummon, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!m_InCombat)
                {
                    if (Player* l_Player = me->FindNearestPlayer(18.0f))
                    {
                        me->CastSpell(me, eSpells::TeleportOffThrone, true);
                        m_InCombat = true;

                        uint64 l_Guid = l_Player->GetGUID();
                        AddTimedDelayedOperation(500, [this, l_Guid]() -> void
                        {
                            if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                                AttackStart(l_Player);
                        });
                    }
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventMarkOfChaos:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::MarkOfChaosAura, false);

                        Talk(eTalks::MarkOfChaos);
                        m_Events.ScheduleEvent(eEvents::EventMarkOfChaos, 51 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventForceNova:
                    {
                        Talk(eTalks::ForceNova);
                        m_Events.ScheduleEvent(eEvents::EventForceNova, 50 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventArcaneWrath:
                    {
                        Talk(eTalks::ArcaneWrath);
                        m_Events.ScheduleEvent(eEvents::EventArcaneWrath, 50 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventDestructiveResonance:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceSearch, false);
                        me->CastSpell(me, eSpells::DestructiveResonanceCosm, true);
                        m_Events.ScheduleEvent(eEvents::EventDestructiveResonance, 15 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventArcaneAberration:
                    {
                        me->CastSpell(me, eSpells::SummonArcaneAberrationCast, false);
                        me->CastSpell(me, eSpells::SummonArcaneAberrationCosm, true);
                        Talk(eTalks::ArcaneAberration);
                        m_Events.ScheduleEvent(eEvents::EventArcaneAberration, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventBerserk:
                    {
                        Talk(eTalks::Berserk);
                        me->CastSpell(me, eHighmaulSpells::Berserker, true);
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
            return new boss_imperator_margokAI(p_Creature);
        }
};

/// Rune of Displacement - 77429
class npc_highmaul_rune_of_displacement : public CreatureScript
{
    public:
        npc_highmaul_rune_of_displacement() : CreatureScript("npc_highmaul_rune_of_displacement") { }

        enum eSpell
        {
        };

        struct npc_highmaul_rune_of_displacementAI : public ScriptedAI
        {
            npc_highmaul_rune_of_displacementAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_rune_of_displacementAI(p_Creature);
        }
};

/// Arcane Aberration - 77809
class npc_highmaul_arcane_aberration : public CreatureScript
{
    public:
        npc_highmaul_arcane_aberration() : CreatureScript("npc_highmaul_arcane_aberration") { }

        enum eSpells
        {
            CollapsingEntityAura    = 158703,
            ReverseDeath            = 157099
        };

        struct npc_highmaul_arcane_aberrationAI : public ScriptedAI
        {
            npc_highmaul_arcane_aberrationAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->CastSpell(me, eSpells::CollapsingEntityAura, true);
                me->CastSpell(me, eSpells::ReverseDeath, true);

                if (Player* l_Player = me->FindNearestPlayer(50.0f))
                    AttackStart(l_Player);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_arcane_aberrationAI(p_Creature);
        }
};

/// Destructive Resonance - 77637
class npc_highmaul_destructive_resonance : public CreatureScript
{
    public:
        npc_highmaul_destructive_resonance() : CreatureScript("npc_highmaul_destructive_resonance") { }

        enum eSpells
        {
            DestructiveResonanceAura    = 156639,
            BaseMineVisualAura          = 156961,
            DestructiveResonanceAT      = 156469,
            DestructiveResonanceDebuff  = 159200,
            DestructiveResonanceDamage  = 156673
        };

        struct npc_highmaul_destructive_resonanceAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_destructive_resonanceAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_CanExplode = false;
            }

            bool m_CanExplode;

            void Reset() override
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->CastSpell(me, eSpells::DestructiveResonanceAura, true);

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->RemoveAura(eSpells::DestructiveResonanceAura);

                    me->CastSpell(me, eSpells::BaseMineVisualAura, true);
                    me->CastSpell(me, eSpells::DestructiveResonanceAT, true);

                    m_CanExplode = true;
                });

                /// At the end of the 1 minute, the mine despawns harmlessly.
                AddTimedDelayedOperation(60 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Despawn();
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!m_CanExplode)
                    return;

                if (Player* l_Player = me->FindNearestPlayer(3.0f))
                {
                    me->CastSpell(l_Player, eSpells::DestructiveResonanceDebuff, true);
                    me->CastSpell(l_Player, eSpells::DestructiveResonanceDamage, true);

                    Despawn();

                    m_CanExplode = false;
                }
            }

            void Despawn()
            {
                me->RemoveAllAreasTrigger();
                me->RemoveAllAuras();
                me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_destructive_resonanceAI(p_Creature);
        }
};

/// Mark of Chaos - 158605
class spell_highmaul_mark_of_chaos : public SpellScriptLoader
{
    public:
        spell_highmaul_mark_of_chaos() : SpellScriptLoader("spell_highmaul_mark_of_chaos") { }

        enum eSpells
        {
            MarkOfChaosAoE = 158609
        };

        class spell_highmaul_mark_of_chaos_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_mark_of_chaos_AuraScript);

            void OnAuraRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_DEFAULT)
                    return;

                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpells::MarkOfChaosAoE, true);
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_mark_of_chaos_AuraScript::OnAuraRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_mark_of_chaos_AuraScript();
        }
};

/// Destructive Resonance - 174116
class spell_highmaul_destructive_resonance : public SpellScriptLoader
{
    public:
        spell_highmaul_destructive_resonance() : SpellScriptLoader("spell_highmaul_destructive_resonance") { }

        class spell_highmaul_destructive_resonance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_destructive_resonance_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (l_ProcSpell == nullptr)
                    return;

                if (!(l_ProcSpell->SchoolMask & SPELL_SCHOOL_MASK_ARCANE))
                    return;

                /// Moreover, the player who triggers a Destructive Resonance mine is stunned for 1.5 seconds each time he take Arcane damage.
                if (Unit* l_Target = GetTarget())
                {
                    uint32 l_SpellID = GetSpellInfo()->Effects[EFFECT_0].TriggerSpell;
                    l_Target->CastSpell(l_Target, l_SpellID, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_highmaul_destructive_resonance_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_destructive_resonance_AuraScript();
        }
};

void AddSC_boss_imperator_margok()
{
    /// Boss
    new boss_imperator_margok();

    /// NPCs
    new npc_highmaul_rune_of_displacement();
    new npc_highmaul_arcane_aberration();
    new npc_highmaul_destructive_resonance();

    /// Spells
    new spell_highmaul_mark_of_chaos();
    new spell_highmaul_destructive_resonance();

    /// AreaTriggers
}