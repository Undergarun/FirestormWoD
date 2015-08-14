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
            CosmeticSitThrone                       = 88648,
            TeleportOffThrone                       = 166090,
            TeleportToDisplacement                  = 164336,
            TeleportToFortification                 = 164751,
            EncounterEvent                          = 181089,   ///< Sniffed, don't know why, related to phases switch ?
            TransitionVisualPhase2                  = 176576,
            TransitionVisualPhase3                  = 176578,
            TransitionVisualPeriodic                = 176580,
            PowerOfDisplacement                     = 158013,
            PowerOfFortification                    = 158012,
            DisplacementTransform                   = 174020,
            FortificationTransform                  = 174022,
            ArcaneProtection                        = 174057,
            AwakenRunestone                         = 157278,
            /// Mark of Chaos
            MarkOfChaosAura                         = 158605,
            MarkOfChaosCosmetic                     = 164161,
            MarkOfChaosDisplacementAura             = 164176,
            /// AcceleratedAssault
            AcceleratedAssault                      = 159515,
            /// Arcane Aberration
            SummonArcaneAberrationCast              = 156471,
            SummonArcaneAberrationCosmetic          = 164318,   ///< Sniffed, but I don't know why
            SummonDisplacingArcaneAberration        = 164299,
            /// Destructive Resonance
            DestructiveResonanceDebuff              = 159200,
            DestructiveResonanceSearcher            = 156467,
            DestructiveResonanceCosmetic            = 164074,   ///< Sniffed, but I don't know why
            DestructiveResonanceSummon              = 156734,
            DestructiveResonanceDisplacementSearch  = 164075,
            /// Arcane Wrath
            ArcaneWrathSearcher                     = 156238,
            ArcaneWrathCosmetic                     = 163968,   ///< Sniffed, but I don't know why
            ArcaneWrathBranded                      = 156225,
            ArcaneWrathDisplacementSearcher         = 163988,
            ArcaneWrathBrandedDisplacement          = 164004,
            /// Force Nova
            ForceNovaCasting                        = 157349,   ///< CastTime
            ForceNovaScriptEffect                   = 164227,   ///< Sniffed, but I don't know why
            ForceNovaDummy                          = 157320,   ///< Visual effect of the nova
            ForceNovaDoT                            = 157353,
            ForceNovaKnockBack                      = 157325,
            ForceNovaDisplacement                   = 164232,   ///< CastTime
            ForceNovaDisplacementDummy              = 164252,   ///< Visual effect of the nova
            ForceNovaAreaTrigger                    = 157327    ///< Triggers knock back too
        };

        enum eEvents
        {
            /// Phase 1
            EventMarkOfChaos = 1,
            EventForceNova,
            EventArcaneWrath,
            EventDestructiveResonance,
            EventArcaneAberration,
            /// Phase 2
            EventMarkOfChaosDisplacement,
            EventForceNovaDisplacement,
            EventArcaneWrathDisplacement,
            EventDestructiveResonanceDisplacement,
            EventArcaneAberrationDisplacement,
            EventBerserk
        };

        enum eActions
        {
            ActionIntro,
            ActionFinishIntro
        };

        enum eCreatures
        {
            SorcererKingVisualPoint = 89081,
            NpcRuneOfDisplacement   = 77429,
            KingPrison              = 89185
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
            TalkRuneOfFortification1,
            TalkRuneOfFortification2,
            TalkRuneOfFortification3,
            TalkRuneOfFortification4,
            TalkRuneOfFortification5,
            TalkRuneOfReplication,
            Slay,
            Berserk,
            Death
        };

        enum eAnimKit
        {
            AnimKitFlyingRune = 6420
        };

        enum eMoves
        {
            MoveUp = 1,
            MoveDown
        };

        enum eDatas
        {
            /// Values datas
            BrandedStacks,
            PhaseID,
            /// Misc
            MaxVisualPoint  = 8
        };

        enum ePhases
        {
            MightOfTheCrown = 1,    ///< Phase 1: Might of the Crown
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

                g_SwitchPhasePcts[ePhases::MightOfTheCrown - 1]     = 85;   ///< Phase One lasts from 100 % to 85 % of Mar'gok's health.
                g_SwitchPhasePcts[ePhases::RuneOfDisplacement - 1]  = 55;   ///< Phase Two lasts from 85 % to 55 % of Mar'gok's health.
                g_SwitchPhasePcts[ePhases::DormantRunestones - 1]   = -1;   ///< The First Transition Phase lasts for 1 minute after the end of Phase Two.
                g_SwitchPhasePcts[ePhases::RuneOfFortification - 1] = 25;   ///< Phase Three lasts from 55 % of Mar'gok's health(end of the First Transition Phase) to 25 % of Mar'gok's health.
                g_SwitchPhasePcts[ePhases::LineageOfPower - 1]      = -1;   ///< The Second transition Phase lasts for 1 minute after the end of Phase Three.
                g_SwitchPhasePcts[ePhases::RuneOfReplication - 1]   = 0;    ///< Phase Four lasts from 25 % of Mar'gok's health(end of the Second Transition Phase) to Mar'gok's death.
            }

            EventMap m_Events;

            bool m_InCombat;

            uint8 m_Phase;
            uint8 g_SwitchPhasePcts[ePhases::MaxPhases - 1];

            uint64 m_MeleeTargetGuid;

            uint8 m_BrandedStacks;

            bool m_IsInNova;
            uint32 m_NovaTime;
            Position m_NovaPos;

            Vehicle* m_Vehicle;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                summons.DespawnAll();

                m_InCombat = false;

                m_Phase = ePhases::MightOfTheCrown;
                m_Events.SetPhase(m_Phase);

                m_MeleeTargetGuid = 0;

                m_BrandedStacks = 0;

                m_IsInNova = false;
                m_NovaTime = 0;
                m_NovaPos  = Position();

                me->RemoveAura(eSpells::AcceleratedAssault);
                me->RemoveAura(eHighmaulSpells::Berserker);

                _Reset();

                AddTimedDelayedOperation(200, [this]() -> void
                {
                    me->CastSpell(me, eSpells::CosmeticSitThrone, true);

                    for (uint8 l_I = 0; l_I < eDatas::MaxVisualPoint; ++l_I)
                    {
                        if (Creature* l_Creature = me->SummonCreature(eCreatures::SorcererKingVisualPoint, *me))
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
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveUp:
                    {
                        if (Creature* l_Rune = me->FindNearestCreature(eCreatures::NpcRuneOfDisplacement, 40.0f))
                            l_Rune->CastSpell(l_Rune, eSpells::TransitionVisualPeriodic, true);

                        break;
                    }
                    case eMoves::MoveDown:
                    {
                        AddTimedDelayedOperation(100, [this]() -> void
                        {
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                            me->SetAIAnimKitId(0);
                            me->SetAnimTier(0);
                            me->SetDisableGravity(false);
                            me->SetPlayerHoverAnim(false);
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                            me->RemoveAura(eSpells::TransitionVisualPhase2);
                            me->RemoveAura(eSpells::TransitionVisualPhase3);

                            if (Creature* l_Rune = me->FindNearestCreature(eCreatures::NpcRuneOfDisplacement, 40.0f))
                                l_Rune->RemoveAura(eSpells::TransitionVisualPeriodic);

                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                                AttackStart(l_Target);
                        });

                        if (m_Phase == ePhases::DormantRunestones)
                        {
                            AddTimedDelayedOperation(200, [this]() -> void
                            {
                                me->RemoveAura(eSpells::DisplacementTransform);
                                me->CastSpell(me, eSpells::FortificationTransform, true);

                                me->RemoveAura(eSpells::PowerOfDisplacement);
                                me->CastSpell(me, eSpells::PowerOfFortification, true);
                            });
                        }

                        break;
                    }
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
                if (g_SwitchPhasePcts[m_Phase - 1] == 255 || !g_SwitchPhasePcts[m_Phase - 1])
                    return;

                if (me->HealthBelowPctDamaged(g_SwitchPhasePcts[m_Phase - 1], p_Damage))
                {
                    ++m_Phase;

                    switch (m_Phase)
                    {
                        case ePhases::RuneOfDisplacement:
                        {
                            ScheduleSecondPhase();
                            break;
                        }
                        case ePhases::DormantRunestones:
                        {
                            ScheduleFirstTransitionPhase();
                            break;
                        }
                        default:
                            break;
                    }

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
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosDisplacementAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::DestructiveResonanceDebuff);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBranded);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedDisplacement);

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
                ClearDelayedOperations();

                me->SetAIAnimKitId(0);
                me->SetAnimTier(0);
                me->SetDisableGravity(false);
                me->SetPlayerHoverAnim(false);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

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

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosDisplacementAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::DestructiveResonanceDebuff);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBranded);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedDisplacement);
                }
            }

            uint32 GetData(uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eDatas::BrandedStacks:
                        return m_BrandedStacks;
                    case eDatas::PhaseID:
                        return m_Phase;
                    default:
                        break;
                }

                return 0;
            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
                    case eDatas::BrandedStacks:
                    {
                        if (p_Value)
                            ++m_BrandedStacks;
                        else
                            m_BrandedStacks = 0;

                        break;
                    }
                    default:
                        break;
                }
            }

            void AreaTriggerDespawned(AreaTrigger* p_AreaTrigger) override
            {
                if (p_AreaTrigger->GetSpellId() == eSpells::ForceNovaAreaTrigger)
                {
                    if (m_Instance)
                        m_Instance->DoRemoveForcedMovementsOnPlayers(p_AreaTrigger->GetGUID());
                }
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::ForceNovaCasting:
                    {
                        /// This spell should knock back players in melee range
                        me->CastSpell(me, eSpells::ForceNovaKnockBack, true);
                        me->CastSpell(me, eSpells::ForceNovaDummy, true);

                        m_IsInNova  = true;
                        m_NovaTime  = 0;
                        m_NovaPos   = *me;
                        break;
                    }
                    case eSpells::DestructiveResonanceSearcher:
                    case eSpells::DestructiveResonanceDisplacementSearch:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, -10.0f))
                            me->CastSpell(l_Target, eSpells::DestructiveResonanceSummon, true);

                        break;
                    }
                    case eSpells::ArcaneWrathSearcher:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2))
                            me->CastSpell(l_Target, eSpells::ArcaneWrathBranded, true);

                        break;
                    }
                    case eSpells::ArcaneWrathDisplacementSearcher:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2))
                            me->CastSpell(l_Target, eSpells::ArcaneWrathBrandedDisplacement, true);

                        break;
                    }
                    case eSpells::ForceNovaDisplacement:
                    {
                        /// This spell should knock back players in melee range
                        me->CastSpell(me, eSpells::ForceNovaAreaTrigger, true);
                        me->CastSpell(me, eSpells::ForceNovaDisplacementDummy, true);

                        m_IsInNova = true;
                        m_NovaTime = 0;
                        m_NovaPos = *me;
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
                    case eSpells::ArcaneWrathBranded:
                    {
                        /// m_BrandedStacks only counts the jumps, we must add 1
                        if (AuraPtr l_Branded = p_Target->GetAura(eSpells::ArcaneWrathBranded, me->GetGUID()))
                            l_Branded->SetStackAmount(m_BrandedStacks + 1);

                        break;
                    }
                    case eSpells::ArcaneWrathBrandedDisplacement:
                    {
                        /// m_BrandedStacks only counts the jumps, we must add 1
                        if (AuraPtr l_Branded = p_Target->GetAura(eSpells::ArcaneWrathBrandedDisplacement, me->GetGUID()))
                            l_Branded->SetStackAmount(m_BrandedStacks + 1);

                        break;
                    }
                    case eSpells::MarkOfChaosDisplacementAura:
                    {
                        /// In addition to Mark of Chaos' normal effects, the target is teleported to a random location.
                        if (Creature* l_Prison = me->FindNearestCreature(eCreatures::KingPrison, 150.0f))
                        {
                            float l_Range       = frand(15.0f, 50.0f);
                            float l_Orientation = frand(0.0f, 2 * M_PI);
                            float l_X           = me->GetPositionX() + (l_Range * cos(l_Orientation));
                            float l_Y           = me->GetPositionY() + (l_Range * sin(l_Orientation));

                            p_Target->NearTeleportTo(l_X, l_Y, me->GetPositionZ(), p_Target->GetOrientation());
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

                if (!m_InCombat)
                {
                    if (Player* l_Player = me->FindNearestPlayer(20.0f))
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

                if (m_IsInNova)
                    UpdateNovaTargets(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    /// Phase 1
                    case eEvents::EventMarkOfChaos:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::MarkOfChaosAura, false);
                            me->CastSpell(l_Target, eSpells::MarkOfChaosCosmetic, true);
                        }

                        Talk(eTalks::MarkOfChaos);
                        m_Events.ScheduleEvent(eEvents::EventMarkOfChaos, 51 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventForceNova:
                    {
                        me->CastSpell(me, eSpells::ForceNovaCasting, false);
                        me->CastSpell(me, eSpells::ForceNovaScriptEffect, true);

                        Talk(eTalks::ForceNova);

                        m_Events.ScheduleEvent(eEvents::EventForceNova, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);

                        /// Force Nova has a radius of 100 yards and moves with a speed of 7 yards per second
                        uint32 l_Time = float(100.0f / 7.0f) * float(TimeConstants::IN_MILLISECONDS);
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            m_IsInNova = false;
                            m_NovaTime = 0;
                            m_NovaPos  = Position();

                            if (m_Instance)
                                m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ForceNovaDoT);
                        });

                        break;
                    }
                    case eEvents::EventArcaneWrath:
                    {
                        Talk(eTalks::ArcaneWrath);
                        me->CastSpell(me, eSpells::ArcaneWrathSearcher, false);
                        me->CastSpell(me, eSpells::ArcaneWrathCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventArcaneWrath, 50 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventDestructiveResonance:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceSearcher, false);
                        me->CastSpell(me, eSpells::DestructiveResonanceCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventDestructiveResonance, 15 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    case eEvents::EventArcaneAberration:
                    {
                        me->CastSpell(me, eSpells::SummonArcaneAberrationCast, false);
                        me->CastSpell(me, eSpells::SummonArcaneAberrationCosmetic, true);
                        Talk(eTalks::ArcaneAberration);
                        m_Events.ScheduleEvent(eEvents::EventArcaneAberration, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::MightOfTheCrown);
                        break;
                    }
                    /// Phase 2
                    case eEvents::EventMarkOfChaosDisplacement:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::MarkOfChaosDisplacementAura, false);
                            me->CastSpell(l_Target, eSpells::MarkOfChaosCosmetic, true);
                        }

                        Talk(eTalks::MarkOfChaos);
                        m_Events.ScheduleEvent(eEvents::EventMarkOfChaosDisplacement, 51 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfDisplacement);
                        break;
                    }
                    case eEvents::EventForceNovaDisplacement:
                    {
                        me->CastSpell(me, eSpells::ForceNovaDisplacement, false);
                        me->CastSpell(me, eSpells::ForceNovaScriptEffect, true);

                        Talk(eTalks::ForceNova);

                        m_Events.ScheduleEvent(eEvents::EventForceNovaDisplacement, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfDisplacement);

                        /// Force Nova has a radius of 100 yards and moves with a speed of 7 yards per second
                        uint32 l_Time = float(100.0f / 7.0f) * float(TimeConstants::IN_MILLISECONDS);
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            m_IsInNova = false;
                            m_NovaTime = 0;
                            m_NovaPos = Position();

                            if (m_Instance)
                                m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ForceNovaDoT);
                        });

                        break;
                    }
                    case eEvents::EventArcaneWrathDisplacement:
                    {
                        Talk(eTalks::ArcaneWrath);
                        me->CastSpell(me, eSpells::ArcaneWrathDisplacementSearcher, false);
                        me->CastSpell(me, eSpells::ArcaneWrathCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventArcaneWrathDisplacement, 50 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfDisplacement);
                        break;
                    }
                    case eEvents::EventDestructiveResonanceDisplacement:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceDisplacementSearch, false);
                        me->CastSpell(me, eSpells::DestructiveResonanceCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventDestructiveResonanceDisplacement, 15 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfDisplacement);
                        break;
                    }
                    case eEvents::EventArcaneAberrationDisplacement:
                    {
                        me->CastSpell(me, eSpells::SummonDisplacingArcaneAberration, false);
                        me->CastSpell(me, eSpells::SummonArcaneAberrationCosmetic, true);
                        Talk(eTalks::ArcaneAberration);
                        m_Events.ScheduleEvent(eEvents::EventArcaneAberrationDisplacement, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfDisplacement);
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

            void UpdateNovaTargets(uint32 const p_Diff)
            {
                m_NovaTime += p_Diff;

                float l_YardsPerMs  = 7.0f / (float)TimeConstants::IN_MILLISECONDS;
                /// Base min radius is 10 yards, which is increased depending on time passed
                float l_MinRadius   = 8.0f;
                float l_InnerRange  = 5.0f;

                l_MinRadius += (l_YardsPerMs * m_NovaTime);

                uint64 l_TriggerGuid    = 0;
                AreaTrigger* l_Trigger  = nullptr;
                if (l_Trigger = me->GetAreaTrigger(eSpells::ForceNovaAreaTrigger))
                    l_TriggerGuid = l_Trigger->GetGUID();

                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                for (HostileReference* l_Ref : l_ThreatList)
                {
                    if (Player* l_Player = Player::GetPlayer(*me, l_Ref->getUnitGuid()))
                    {
                        if (l_Player->GetDistance(m_NovaPos) >= (l_MinRadius - l_InnerRange) &&
                            l_Player->GetDistance(m_NovaPos) <= l_MinRadius)
                        {
                            if (!l_Player->HasAura(eSpells::ForceNovaDoT))
                                me->CastSpell(l_Player, eSpells::ForceNovaDoT, true);

                            /// In addition to Force Nova's normal effects, it now also pushes players away as the nova moves outwards.
                            if (m_Phase == ePhases::RuneOfDisplacement && l_TriggerGuid && l_Trigger)
                            {
                                if (!l_Player->HasMovementForce(l_TriggerGuid))
                                    l_Player->SendApplyMovementForce(l_TriggerGuid, true, *l_Trigger, -5.5f, 1);
                            }
                        }
                        else
                        {
                            if (l_Player->HasAura(eSpells::ForceNovaDoT))
                                l_Player->RemoveAura(eSpells::ForceNovaDoT);

                            /// In addition to Force Nova's normal effects, it now also pushes players away as the nova moves outwards.
                            if (m_Phase == ePhases::RuneOfDisplacement && l_TriggerGuid)
                            {
                                if (l_Player->HasMovementForce(l_TriggerGuid))
                                    l_Player->SendApplyMovementForce(l_TriggerGuid, false, Position());
                            }
                        }
                    }
                }
            }

            void ScheduleSecondPhase()
            {
                me->AttackStop();
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                Talk(eTalks::TalkRuneOfDisplacement);

                me->CastSpell(me, eSpells::EncounterEvent, true);
                me->CastSpell(me, eSpells::TeleportToDisplacement, true);

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpells::TransitionVisualPhase2, true);
                    me->SetAIAnimKitId(eAnimKit::AnimKitFlyingRune);

                    me->SetAnimTier(3);
                    me->SetDisableGravity(true);
                    me->SetPlayerHoverAnim(true);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ += 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveUp, l_Pos);
                });

                AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpells::PowerOfDisplacement, true);
                    me->CastSpell(me, eSpells::DisplacementTransform, true);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ -= 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveDown, l_Pos);
                });

                uint32 l_Time = m_Events.GetEventTime(eEvents::EventMarkOfChaos) + 13 * TimeConstants::IN_MILLISECONDS;
                m_Events.ScheduleEvent(eEvents::EventMarkOfChaosDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                l_Time = m_Events.GetEventTime(eEvents::EventForceNova) + 13 * TimeConstants::IN_MILLISECONDS;
                m_Events.ScheduleEvent(eEvents::EventForceNovaDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                l_Time = m_Events.GetEventTime(eEvents::EventArcaneWrath) + 13 * TimeConstants::IN_MILLISECONDS;
                m_Events.ScheduleEvent(eEvents::EventArcaneWrathDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                l_Time = m_Events.GetEventTime(eEvents::EventDestructiveResonance) + 13 * TimeConstants::IN_MILLISECONDS;
                m_Events.ScheduleEvent(eEvents::EventDestructiveResonanceDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                l_Time = m_Events.GetEventTime(eEvents::EventArcaneAberration) + 13 * TimeConstants::IN_MILLISECONDS;
                m_Events.ScheduleEvent(eEvents::EventArcaneAberrationDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);
            }

            void ScheduleFirstTransitionPhase()
            {
                me->AttackStop();
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->CastSpell(me, eSpells::EncounterEvent, true);
                me->CastSpell(me, eSpells::TeleportToFortification, true);

                Talk(eTalks::TalkRuneOfFortification1);

                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->SetAIAnimKitId(eAnimKit::AnimKitFlyingRune);

                    me->SetAnimTier(3);
                    me->SetDisableGravity(true);
                    me->SetPlayerHoverAnim(true);

                    me->CastSpell(me, eSpells::ArcaneProtection, true);
                    me->CastSpell(me, eSpells::TransitionVisualPhase3, true);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ += 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveUp, l_Pos);
                });

                AddTimedDelayedOperation(10 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfFortification2);

                    me->CastSpell(me, eSpells::AwakenRunestone, false);
                });

                AddTimedDelayedOperation(25 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfFortification3);
                });

                AddTimedDelayedOperation(56 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfFortification4);
                });

                AddTimedDelayedOperation(70 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->RemoveAura(eSpells::AwakenRunestone);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ -= 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveDown, l_Pos);
                });

                AddTimedDelayedOperation(75 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfFortification5);
                });
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

            void Reset() override { }

            void UpdateAI(uint32 const p_Diff) override { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_rune_of_displacementAI(p_Creature);
        }
};

/// Arcane Aberration - 77809
/// Displacing Arcane Aberration - 77879
class npc_highmaul_arcane_aberration : public CreatureScript
{
    public:
        npc_highmaul_arcane_aberration() : CreatureScript("npc_highmaul_arcane_aberration") { }

        enum eSpells
        {
            /// Phase 1
            CollapsingEntityAura    = 158703,
            ReverseDeath            = 157099,
            /// Phase 2
            DisplacerCharge         = 157254,
            ImpactfulPulse          = 160367
        };

        enum eCreatures
        {
            DisplacingArcaneAberration  = 77879
        };

        struct npc_highmaul_arcane_aberrationAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_arcane_aberrationAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->CastSpell(me, eSpells::CollapsingEntityAura, true);
                me->CastSpell(me, eSpells::ReverseDeath, true);

                switch (me->GetEntry())
                {
                    case eCreatures::DisplacingArcaneAberration:
                    {
                        me->CastSpell(me, eSpells::DisplacerCharge, true);
                        break;
                    }
                    default:
                        break;
                }

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                    if (Player* l_Player = me->FindNearestPlayer(50.0f))
                        AttackStart(l_Player);
                });
            }

            void JustDied(Unit* p_Killer) override
            {
                switch (me->GetEntry())
                {
                    case eCreatures::DisplacingArcaneAberration:
                    {
                        me->CastSpell(me, eSpells::ImpactfulPulse, true);
                        break;
                    }
                    default:
                        break;
                }
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
            /// Destructive Resonance
            DestructiveResonanceSpawn               = 156467,
            DestructiveResonanceAura                = 156639,
            BaseMineVisualAura                      = 156961,
            DestructiveResonanceDebuff              = 159200,
            DestructiveResonanceDamage              = 156673,
            /// Destructive Resonance: Displacement
            DestructiveResonanceDisplacementSpawn   = 164075,
            DestructiveResonanceDisplacementAura    = 164086,
            DisplacementMineVisualAura              = 156959,
            DisplacementVisualAura                  = 156983,
            DestructiveResonanceGrowScaleAura       = 156941
        };

        enum eVisuals
        {
            DestructiveResonanceDespawn             = 41168,
            DestructiveResonanceDisplacementDespawn = 45691
        };

        enum eData
        {
            PhaseID = 1
        };

        enum eTalk
        {
            DestructiveResonance
        };

        struct npc_highmaul_destructive_resonanceAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_destructive_resonanceAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_CanExplode = false;

                if (InstanceScript* l_Script = p_Creature->GetInstanceScript())
                {
                    if (Creature* l_Margok = Creature::GetCreature(*p_Creature, l_Script->GetData64(eHighmaulCreatures::ImperatorMargok)))
                    {
                        if (l_Margok->IsAIEnabled)
                            m_Phase = l_Margok->AI()->GetData(eData::PhaseID);
                    }
                }
            }

            bool m_CanExplode;

            uint32 m_SpawnTime;

            uint8 m_Phase;

            void Reset() override
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                if (m_Phase == 1)
                {
                    me->CastSpell(me, eSpells::DestructiveResonanceAura, true);

                    AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->RemoveAura(eSpells::DestructiveResonanceAura);

                        me->CastSpell(me, eSpells::BaseMineVisualAura, true);

                        m_CanExplode = true;
                    });
                }
                else
                {
                    me->CastSpell(me, eSpells::DestructiveResonanceDisplacementAura, true);

                    AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->RemoveAura(eSpells::DestructiveResonanceDisplacementAura);

                        me->CastSpell(me, eSpells::DisplacementMineVisualAura, true);
                        me->CastSpell(me, eSpells::DisplacementVisualAura, true);

                        me->CastSpell(me, eSpells::DestructiveResonanceGrowScaleAura, true);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_SCALE_DURATION, 30 * TimeConstants::IN_MILLISECONDS);

                        m_CanExplode = true;
                    });
                }

                m_SpawnTime = 0;

                /// At the end of the 1 minute, the mine despawns harmlessly.
                AddTimedDelayedOperation(60 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Despawn();
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_SpawnTime += p_Diff;

                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!m_CanExplode)
                    return;

                float l_Radius = 3.0f;

                if (m_Phase == 2)
                    AddPct(l_Radius, float((float)m_SpawnTime / float(30 * TimeConstants::IN_MILLISECONDS) * 100.0f));

                if (l_Radius > 6.0f)
                    l_Radius = 6.0f;

                if (Player* l_Player = me->FindNearestPlayer(l_Radius))
                {
                    me->CastSpell(l_Player, eSpells::DestructiveResonanceDebuff, true);
                    me->CastSpell(l_Player, eSpells::DestructiveResonanceDamage, true);

                    if (m_Phase == 2)
                        me->SendPlaySpellVisualKit(eVisuals::DestructiveResonanceDisplacementDespawn, 0);
                    else
                        me->SendPlaySpellVisualKit(eVisuals::DestructiveResonanceDespawn, 0);

                    Talk(eTalk::DestructiveResonance, l_Player->GetGUID());

                    Despawn();

                    m_CanExplode = false;
                }
            }

            void Despawn()
            {
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

/// Branded - 156225
class spell_highmaul_branded : public SpellScriptLoader
{
    public:
        spell_highmaul_branded() : SpellScriptLoader("spell_highmaul_branded") { }

        enum eSpells
        {
            ArcaneWrathDamage = 156239
        };

        enum eData
        {
            BrandedStacks
        };

        class spell_highmaul_branded_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_branded_AuraScript);

            void OnAuraRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_DEFAULT || GetCaster() == nullptr)
                    return;

                /// Caster is Mar'gok
                if (Creature* l_Margok = GetCaster()->ToCreature())
                {
                    if (!l_Margok->IsAIEnabled)
                        return;

                    if (Unit* l_Target = GetTarget())
                    {
                        l_Margok->CastSpell(l_Target, eSpells::ArcaneWrathDamage, true);

                        uint8 l_Stacks = l_Margok->AI()->GetData(eData::BrandedStacks);

                        /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                        /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 50%.
                        float l_JumpRange = 200.0f;
                        for (uint8 l_I = 0; l_I < l_Stacks; ++l_I)
                            l_JumpRange -= CalculatePct(l_JumpRange, 50.0f);

                        if (Player* l_OtherPlayer = l_Target->FindNearestPlayer(l_JumpRange))
                        {
                            /// Increase jump count
                            l_Margok->AI()->SetData(eData::BrandedStacks, 1);

                            l_Margok->CastSpell(l_OtherPlayer, GetSpellInfo()->Id, true);
                            return;
                        }

                        /// If no player found, the debuff will drop because there will be no one within 25 yards of the afflicted player.
                        l_Margok->AI()->SetData(eData::BrandedStacks, 0);
                    }
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_branded_AuraScript::OnAuraRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_branded_AuraScript();
        }
};

/// Branded: Displacement - 164004
class spell_highmaul_branded_displacement : public SpellScriptLoader
{
    public:
        spell_highmaul_branded_displacement() : SpellScriptLoader("spell_highmaul_branded_displacement") { }

        enum eSpells
        {
            ArcaneWrathDamage   = 156239,
            ArcaneWrathTeleport = 160370
        };

        enum eData
        {
            BrandedStacks
        };

        class spell_highmaul_branded_displacement_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_branded_displacement_AuraScript);

            Position m_MarkPos;

            void OnAuraApply(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Target = GetTarget())
                    m_MarkPos = *l_Target;
            }

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    /// In addition to Arcane Wrath's normal effects, Branded Players are unable to move more than 10 yards from the location they were marked.
                    /// Attempting to leave the marked area will teleport the player back to the location they were originally marked.
                    if (l_Target->GetDistance(m_MarkPos) > 10.0f)
                        l_Target->CastSpell(m_MarkPos, eSpells::ArcaneWrathTeleport, true);
                }
            }

            void OnAuraRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_DEFAULT || GetCaster() == nullptr)
                    return;

                /// Caster is Mar'gok
                if (Creature* l_Margok = GetCaster()->ToCreature())
                {
                    if (!l_Margok->IsAIEnabled)
                        return;

                    if (Unit* l_Target = GetTarget())
                    {
                        l_Margok->CastSpell(l_Target, eSpells::ArcaneWrathDamage, true);

                        uint8 l_Stacks = l_Margok->AI()->GetData(eData::BrandedStacks);

                        /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                        /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 50%.
                        float l_JumpRange = 200.0f;
                        for (uint8 l_I = 0; l_I < l_Stacks; ++l_I)
                            l_JumpRange -= CalculatePct(l_JumpRange, 50.0f);

                        if (Player* l_OtherPlayer = l_Target->FindNearestPlayer(l_JumpRange))
                        {
                            /// Increase jump count
                            l_Margok->AI()->SetData(eData::BrandedStacks, 1);

                            l_Margok->CastSpell(l_OtherPlayer, GetSpellInfo()->Id, true);
                            return;
                        }

                        /// If no player found, the debuff will drop because there will be no one within 25 yards of the afflicted player.
                        l_Margok->AI()->SetData(eData::BrandedStacks, 0);
                    }
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_highmaul_branded_displacement_AuraScript::OnAuraApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_branded_displacement_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_branded_displacement_AuraScript::OnAuraRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_branded_displacement_AuraScript();
        }
};

/// Arcane Wrath (damage) - 156239
class spell_highmaul_arcane_wrath_damage : public SpellScriptLoader
{
    public:
        spell_highmaul_arcane_wrath_damage() : SpellScriptLoader("spell_highmaul_arcane_wrath_damage") { }

        class spell_highmaul_arcane_wrath_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_arcane_wrath_damage_SpellScript);

            enum eData
            {
                BrandedStacks
            };

            uint8 m_Stacks;

            bool Load() override
            {
                m_Stacks = 0;

                /// We must save the stacks amount before the spell hit the player
                /// It'll be reset before if it doesn't jump anymore
                if (GetCaster() == nullptr)
                    return false;

                if (Creature* l_Margok = GetCaster()->ToCreature())
                {
                    if (!l_Margok->IsAIEnabled)
                        return false;

                    m_Stacks = l_Margok->AI()->GetData(eData::BrandedStacks);
                    return true;
                }

                return false;
            }

            void HandleDamage()
            {
                /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 50%.
                int32 l_Damage = GetHitDamage();
                AddPct(l_Damage, int32(25 * m_Stacks));

                SetHitDamage(l_Damage);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_highmaul_arcane_wrath_damage_SpellScript::HandleDamage);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_arcane_wrath_damage_SpellScript();
        }
};

/// Transition Visuals - 176580
class spell_highmaul_transition_visuals : public SpellScriptLoader
{
    public:
        spell_highmaul_transition_visuals() : SpellScriptLoader("spell_highmaul_transition_visuals") { }

        enum eSpell
        {
            TransitionVisualMissileP1   = 176581,
            TransitionVisualMissileP2   = 176582
        };

        enum eDatas
        {
            PhaseID         = 1,
            MissileCountP1  = 6,
            MissileCountP2  = 2
        };

        enum ePhases
        {
            MightOfTheCrown = 1,    ///< Phase 1: Might of the Crown
            RuneOfDisplacement,     ///< Phase 2: Rune of Displacement
            DormantRunestones,      ///< Intermission: Dormant Runestones
            RuneOfFortification,    ///< Phase 3: Rune of Fortification
            LineageOfPower,         ///< Intermission: Lineage of Power
            RuneOfReplication,      ///< Phase 4: Rune of Replication
            MaxPhases
        };

        class spell_highmaul_transition_visuals_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_transition_visuals_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Creature* l_Margok = l_Target->FindNearestCreature(eHighmaulCreatures::ImperatorMargok, 40.0f))
                    {
                        if (!l_Margok->IsAIEnabled)
                            return;

                        uint8 l_Phase = l_Margok->AI()->GetData(eDatas::PhaseID);

                        switch (l_Phase)
                        {
                            case ePhases::RuneOfDisplacement:
                            {
                                for (uint8 l_I = 0; l_I < eDatas::MissileCountP1; ++l_I)
                                    l_Target->CastSpell(l_Margok, eSpell::TransitionVisualMissileP1, true);

                                break;
                            }
                            case ePhases::DormantRunestones:
                            {
                                for (uint8 l_I = 0; l_I < eDatas::MissileCountP2; ++l_I)
                                    l_Target->CastSpell(l_Margok, eSpell::TransitionVisualMissileP2, true);

                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_transition_visuals_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_transition_visuals_AuraScript();
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
    new spell_highmaul_branded();
    new spell_highmaul_branded_displacement();
    new spell_highmaul_arcane_wrath_damage();
    new spell_highmaul_transition_visuals();

    /// AreaTriggers
}