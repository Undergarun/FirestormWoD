////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

# include "highmaul.hpp"

Position const g_GorianReaverPos = { 4026.755f, 8584.76f, 572.6546f, 3.138298f };

/// For position Z periodical check
float const g_MinAllowedZ = 560.0f;

Position const g_CenterPos = { 3917.63f, 8590.89f, 565.341f, 0.0f };

Position const g_VolatileAnomalyPos[eHighmaulDatas::MaxIntervalles] =
{
    { 3885.65f, 8557.80f, 565.34f, 0.747137f }, ///< Rune of Fortification
    { 3890.47f, 8628.17f, 565.34f, 5.375480f }  ///< Rune of Replication
};

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
            TeleportToReplication                   = 164810,
            EncounterEvent                          = 181089,   ///< Sniffed, don't know why, related to phases switch ?
            TransitionVisualPhase2                  = 176576,
            TransitionVisualPhase3                  = 176578,
            TransitionVisualPhase4                  = 176579,
            TransitionVisualPeriodic                = 176580,
            PowerOfDisplacement                     = 158013,
            PowerOfFortification                    = 158012,
            PowerOfReplication                      = 157964,
            DisplacementTransform                   = 174020,
            FortificationTransform                  = 174022,
            DisplacementTransform2                  = 174023,
            ArcaneProtection                        = 174057,
            AwakenRunestone                         = 157278,
            FortificationBaseVisual                 = 174043,
            FortificationAchievement                = 143809,
            FortificationRuneActive                 = 166429,
            ReplicationBaseVisual                   = 174044,
            ReplicationAchievement                  = 166391,
            ReplicationRuneActive                   = 174085,
            SummonRuneVisual                        = 160351,
            SummonWarmage                           = 160366,
            VolatileAnomalies                       = 157265,
            ImperatorMargokBonus                    = 177528,
            /// Mark of Chaos
            MarkOfChaosAura                         = 158605,
            MarkOfChaosCosmetic                     = 164161,
            MarkOfChaosDisplacementAura             = 164176,
            MarkOfChaosFortificationAura            = 164178,
            FetterMarkOfChaosRootAura               = 158619,
            MarkOfChaosReplicationAura              = 164191,
            OrbsOfChaosDummyAura                    = 160447,
            /// AcceleratedAssault
            AcceleratedAssault                      = 159515,
            /// Arcane Aberration
            SummonArcaneAberrationCast              = 156471,
            SummonArcaneAberrationCosmetic          = 164318,   ///< Sniffed, but I don't know why
            SummonDisplacingArcaneAberration        = 164299,
            SummonFortifiedArcaneAberration         = 164301,
            SummonReplicatingArcaneAberration       = 164303,
            /// Destructive Resonance
            DestructiveResonanceDebuff              = 159200,
            DestructiveResonanceSearcher            = 156467,
            DestructiveResonanceCosmetic            = 164074,   ///< Sniffed, but I don't know why
            DestructiveResonanceSummon              = 156734,
            DestructiveResonanceDisplacementSearch  = 164075,
            DestructiveResonanceFortificationSearch = 164076,
            DestructiveResonanceReplicationSearch   = 164077,
            /// Arcane Wrath
            ArcaneWrathSearcher                     = 156238,
            ArcaneWrathCosmetic                     = 163968,   ///< Sniffed, but I don't know why
            ArcaneWrathBranded                      = 156225,
            ArcaneWrathDisplacementSearcher         = 163988,
            ArcaneWrathBrandedDisplacement          = 164004,
            ArcaneWrathFortificationSearcher        = 163989,
            ArcaneWrathBrandedFortification         = 164005,
            ArcaneWrathReplicationSearcher          = 163990,
            ArcaneWrathBrandedReplication           = 164006,
            /// Force Nova
            ForceNovaCasting                        = 157349,   ///< CastTime
            ForceNovaScriptEffect                   = 164227,   ///< Sniffed, but I don't know why
            ForceNovaDummy                          = 157320,   ///< Visual effect of the nova
            ForceNovaDoT                            = 157353,
            ForceNovaKnockBack                      = 157325,
            ForceNovaDisplacement                   = 164232,   ///< CastTime
            ForceNovaDisplacementDummy              = 164252,   ///< Visual effect of the nova
            ForceNovaAreaTrigger                    = 157327,   ///< Triggers knock back too
            ForceNovaFortificationCasting           = 164235,   ///< Cast Time
            ForceNovaFortifiedPeriodicAura          = 157323,   ///< 8s periodic for Phase 3
            ForceNovaFortificationDummy             = 164253,
            ForceNovaReplicationCasting             = 164240,   ///< CastTime
            ForceNovaReplicationDummy               = 164254    ///< Visual effect
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
            /// Phase 3
            EventMarkOfChaosFortification,
            EventForceNovaFortification,
            EventArcaneWrathFortification,
            EventDestructiveResonanceFortification,
            EventArcaneAberrationFortification,
            /// Phase 4
            EventMarkOfChaosReplication,
            EventForceNovaReplication,
            EventArcaneWrathReplication,
            EventDestructiveResonanceReplication,
            EventArcaneAberrationReplication,
            EventBerserk
        };

        enum eCosmeticEvents
        {
            EventSummonWarmages = 1,
            EventCheckPlayerZ
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
            SLGGenericMoPLargeAoI   = 68553,
            WarmageSummonStalker    = 77682,
            GorianWarmage           = 78121,
            VolatileAnomaly         = 78077,
            GorianReaver            = 78549,
            ArcaneRemnant           = 79388
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
            TalkRuneOfReplication1,
            TalkRuneOfReplication2,
            TalkRuneOfReplication3,
            TalkRuneOfReplication4,
            Slay,
            Berserk,
            Death,
            Branded
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
            OrbOfChaosAngle,
            /// Misc
            MaxVisualPoint  = 8,
            MaxNovaPhase3   = 3
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
            EventMap m_CosmeticEvents;

            bool m_InCombat;

            uint8 m_Phase;
            uint8 g_SwitchPhasePcts[ePhases::MaxPhases - 1];

            uint64 m_MeleeTargetGuid;

            bool m_IsInNova;
            uint32 m_NovaTime;
            Position m_NovaPos;

            /// Force Nova datas for Phase 3: Rune of Fortification
            uint8 m_NovaCount;
            bool m_IsInNovaPhase3[eDatas::MaxNovaPhase3];
            uint32 m_NovaTimePhase3[eDatas::MaxNovaPhase3];
            Position m_NovaPosPhase3[eDatas::MaxNovaPhase3];

            uint8 m_OrbCount;

            Vehicle* m_Vehicle;
            InstanceScript* m_Instance;

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                summons.DespawnAll();

                m_InCombat = false;

                m_Phase = ePhases::MightOfTheCrown;
                m_Events.SetPhase(m_Phase);

                m_MeleeTargetGuid = 0;

                m_IsInNova  = false;
                m_NovaTime  = 0;
                m_NovaPos   = Position();
                m_NovaCount = 0;

                for (uint8 l_I = 0; l_I < eDatas::MaxNovaPhase3; ++l_I)
                {
                    m_IsInNovaPhase3[l_I] = false;
                    m_NovaTimePhase3[l_I] = 0;
                    m_NovaPosPhase3[l_I]  = Position();
                }

                m_OrbCount = 0;

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

                ResetRunes();
                DespawnAdds();
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
                        AddTimedDelayedOperation(200, [this]() -> void
                        {
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                            me->SetAIAnimKitId(0);
                            me->SetAnimTier(0);
                            me->SetDisableGravity(false);
                            me->SetPlayerHoverAnim(false);
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                            me->RemoveAura(eSpells::TransitionVisualPhase2);
                            me->RemoveAura(eSpells::TransitionVisualPhase3);
                            me->RemoveAura(eSpells::TransitionVisualPhase4);

                            if (Creature* l_Rune = me->FindNearestCreature(eCreatures::NpcRuneOfDisplacement, 40.0f))
                            {
                                l_Rune->RemoveAura(eSpells::TransitionVisualPeriodic);

                                switch (m_Phase)
                                {
                                    case ePhases::DormantRunestones:
                                    {
                                        l_Rune->CastSpell(l_Rune, eSpells::FortificationRuneActive, true);
                                        break;
                                    }
                                    case ePhases::LineageOfPower:
                                    {
                                        l_Rune->CastSpell(l_Rune, eSpells::ReplicationRuneActive, true);
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }

                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                                AttackStart(l_Target);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
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

                m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCheckPlayerZ, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void DamageDealt(Unit* p_Victim, uint32& /*p_Damage*/, DamageEffectType p_DamageType) override
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

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
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
                        case ePhases::LineageOfPower:
                        {
                            ScheduleSecondTransitionPhase();
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

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                Talk(eTalks::Death);

                ResetRunes();
                DespawnAdds();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosDisplacementAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosFortificationAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosReplicationAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::DestructiveResonanceDebuff);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBranded);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedDisplacement);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedFortification);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedReplication);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::FetterMarkOfChaosRootAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::OrbsOfChaosDummyAura);

                    if (sObjectMgr->IsDisabledEncounter(m_Instance->GetEncounterIDForBoss(me), GetDifficulty()))
                        me->SetLootRecipient(nullptr);
                    else
                        CastSpellToPlayers(me->GetMap(), me, eSpells::ImperatorMargokBonus, true);
                }
            }

            void EnterEvadeMode() override
            {
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
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosFortificationAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::MarkOfChaosReplicationAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::DestructiveResonanceDebuff);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBranded);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedDisplacement);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedFortification);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ArcaneWrathBrandedReplication);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::FetterMarkOfChaosRootAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::OrbsOfChaosDummyAura);
                }
            }

            uint32 GetData(uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eDatas::PhaseID:
                        return m_Phase;
                    case eDatas::OrbOfChaosAngle:
                        return m_OrbCount;
                    default:
                        break;
                }

                return 0;
            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
                    case eDatas::OrbOfChaosAngle:
                    {
                        m_OrbCount = p_Value;
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

                        /// Force Nova has a radius of 100 yards and moves with a speed of 7 yards per second
                        uint32 l_Time = uint32(float(100.0f / 7.0f) * float(TimeConstants::IN_MILLISECONDS));
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
                    case eSpells::DestructiveResonanceSearcher:
                    case eSpells::DestructiveResonanceDisplacementSearch:
                    case eSpells::DestructiveResonanceFortificationSearch:
                    case eSpells::DestructiveResonanceReplicationSearch:
                    {
                        Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, -10.0f, true);
                        if (l_Target == nullptr || me->GetDistance(l_Target) >= 100.0f)
                            l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 60.0f, true);

                        if (l_Target != nullptr)
                            me->CastSpell(*l_Target, eSpells::DestructiveResonanceSummon, true);

                        break;
                    }
                    case eSpells::ArcaneWrathSearcher:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 60.0f))
                            me->CastSpell(l_Target, eSpells::ArcaneWrathBranded, true);

                        break;
                    }
                    case eSpells::ArcaneWrathDisplacementSearcher:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 60.0f))
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

                        /// Force Nova has a radius of 100 yards and moves with a speed of 7 yards per second
                        uint32 l_Time = uint32(float(100.0f / 7.0f) * float(TimeConstants::IN_MILLISECONDS));
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
                    case eSpells::AwakenRunestone:
                    {
                        std::list<Creature*> l_TriggerList;
                        me->GetCreatureListWithEntryInGrid(l_TriggerList, eHighmaulGameobjects::SLGGenericMoPLargeAoI, 30.0f);

                        /// Should have only two triggers, the base one and the large one
                        for (Creature* l_Trigger : l_TriggerList)
                        {
                            switch (m_Phase)
                            {
                                case ePhases::DormantRunestones:
                                {
                                    /// Small gaze for base visual
                                    if (l_Trigger->HasAura(eSpells::FortificationBaseVisual))
                                        continue;

                                    /// Big gaze for large visual during activation
                                    l_Trigger->CastSpell(l_Trigger, eSpells::FortificationAchievement, true);
                                    l_Trigger->CastSpell(l_Trigger, eSpells::VolatileAnomalies, true);
                                    break;
                                }
                                case ePhases::LineageOfPower:
                                {
                                    /// Small gaze for base visual
                                    if (l_Trigger->HasAura(eSpells::ReplicationBaseVisual))
                                        continue;

                                    /// Big gaze for large visual during activation
                                    l_Trigger->CastSpell(l_Trigger, eSpells::ReplicationAchievement, true);
                                    l_Trigger->CastSpell(l_Trigger, eSpells::VolatileAnomalies, true);
                                    break;
                                }
                                default:
                                    break;
                            }

                            break;
                        }

                        break;
                    }
                    case eSpells::ArcaneWrathFortificationSearcher:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 60.0f))
                            me->CastSpell(l_Target, eSpells::ArcaneWrathBrandedFortification, true);

                        break;
                    }
                    case eSpells::ForceNovaFortificationCasting:
                    {
                        me->CastSpell(me, eSpells::ForceNovaFortifiedPeriodicAura, true);
                        break;
                    }
                    case eSpells::ForceNovaFortificationDummy:
                    {
                        me->CastSpell(me, eSpells::ForceNovaKnockBack, true);

                        m_IsInNovaPhase3[m_NovaCount] = true;
                        m_NovaTimePhase3[m_NovaCount] = 0;
                        m_NovaPosPhase3[m_NovaCount]  = *me;

                        /// Force Nova has a radius of 100 yards and moves with a speed of 7 yards per second
                        uint32 l_Time = uint32(float(100.0f / 7.0f) * float(TimeConstants::IN_MILLISECONDS));
                        /// Must save the current value
                        uint8 l_Count = m_NovaCount;
                        AddTimedDelayedOperation(l_Time, [this, l_Count]() -> void
                        {
                            m_IsInNovaPhase3[l_Count] = false;
                            m_NovaTimePhase3[l_Count] = 0;
                            m_NovaPosPhase3[l_Count]  = Position();

                            if (m_Instance && l_Count >= (eDatas::MaxNovaPhase3 - 1))
                            {
                                m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ForceNovaDoT);
                                m_NovaCount = 0;
                            }
                        });

                        ++m_NovaCount;
                        break;
                    }
                    case eSpells::ArcaneWrathReplicationSearcher:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 60.0f))
                            me->CastSpell(l_Target, eSpells::ArcaneWrathBrandedReplication, true);

                        break;
                    }
                    case eSpells::ForceNovaReplicationCasting:
                    {
                        /// This spell should knock back players in melee range
                        me->CastSpell(me, eSpells::ForceNovaKnockBack, true);
                        me->CastSpell(me, eSpells::ForceNovaReplicationDummy, true);

                        m_IsInNova = true;
                        m_NovaTime = 0;
                        m_NovaPos = *me;

                        /// Force Nova has a radius of 100 yards and moves with a speed of 7 yards per second
                        uint32 l_Time = uint32(float(100.0f / 7.0f) * float(TimeConstants::IN_MILLISECONDS));
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
                    case eSpells::ArcaneWrathBrandedDisplacement:
                    case eSpells::ArcaneWrathBrandedFortification:
                    case eSpells::ArcaneWrathBrandedReplication:
                    {
                        Talk(eTalks::Branded, p_Target->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                        break;
                    }
                    case eSpells::MarkOfChaosDisplacementAura:
                    {
                        /// In addition to Mark of Chaos' normal effects, the target is teleported to a random location.
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, -10.0f))
                            p_Target->NearTeleportTo(*l_Target);

                        break;
                    }
                    case eSpells::MarkOfChaosFortificationAura:
                    {
                        me->CastSpell(p_Target, eSpells::FetterMarkOfChaosRootAura, true);
                        break;
                    }
                    case eSpells::MarkOfChaosReplicationAura:
                    {
                        m_OrbCount = 0;
                        me->CastSpell(p_Target, eSpells::OrbsOfChaosDummyAura, true);
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
                    if (Player* l_Player = me->SelectNearestPlayerNotGM(20.0f))
                    {
                        me->CastSpell(me, eSpells::TeleportOffThrone, true);
                        m_InCombat = true;

                        uint64 l_Guid = l_Player->GetGUID();
                        AddTimedDelayedOperation(50, [this, l_Guid]() -> void
                        {
                            if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                                AttackStart(l_Player);
                        });
                    }
                }

                if (m_IsInNova || (m_NovaCount > 0 && m_IsInNovaPhase3[m_NovaCount - 1]))
                    UpdateNovaTargets(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    case eCosmeticEvents::EventSummonWarmages:
                    {
                        std::list<Creature*> l_TriggerList;
                        me->GetCreatureListWithEntryInGrid(l_TriggerList, eCreatures::WarmageSummonStalker, 150.0f);

                        for (Creature* l_Stalker : l_TriggerList)
                            l_Stalker->CastSpell(l_Stalker, eSpells::SummonWarmage, true);

                        break;
                    }
                    case eCosmeticEvents::EventCheckPlayerZ:
                    {
                        std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                        for (HostileReference* l_Ref : l_ThreatList)
                        {
                            if (Player* l_Player = Player::GetPlayer(*me, l_Ref->getUnitGuid()))
                            {
                                if (l_Player->GetPositionZ() <= g_MinAllowedZ)
                                    l_Player->NearTeleportTo(g_CenterPos);
                            }
                        }

                        m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCheckPlayerZ, 1 * TimeConstants::IN_MILLISECONDS);
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
                    /// Phase 3
                    case eEvents::EventMarkOfChaosFortification:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::MarkOfChaosFortificationAura, false);
                            me->CastSpell(l_Target, eSpells::MarkOfChaosCosmetic, true);
                        }

                        Talk(eTalks::MarkOfChaos);
                        m_Events.ScheduleEvent(eEvents::EventMarkOfChaosFortification, 51 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfFortification);
                        break;
                    }
                    case eEvents::EventForceNovaFortification:
                    {
                        me->CastSpell(me, eSpells::ForceNovaFortificationCasting, false);
                        me->CastSpell(me, eSpells::ForceNovaScriptEffect, true);

                        Talk(eTalks::ForceNova);

                        m_Events.ScheduleEvent(eEvents::EventForceNovaFortification, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfFortification);
                        break;
                    }
                    case eEvents::EventArcaneWrathFortification:
                    {
                        Talk(eTalks::ArcaneWrath);
                        me->CastSpell(me, eSpells::ArcaneWrathFortificationSearcher, false);
                        me->CastSpell(me, eSpells::ArcaneWrathCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventArcaneWrathFortification, 50 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfFortification);
                        break;
                    }
                    case eEvents::EventDestructiveResonanceFortification:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceFortificationSearch, false);
                        me->CastSpell(me, eSpells::DestructiveResonanceCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventDestructiveResonanceFortification, 15 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfFortification);
                        break;
                    }
                    case eEvents::EventArcaneAberrationFortification:
                    {
                        me->CastSpell(me, eSpells::SummonFortifiedArcaneAberration, false);
                        me->CastSpell(me, eSpells::SummonArcaneAberrationCosmetic, true);
                        Talk(eTalks::ArcaneAberration);
                        m_Events.ScheduleEvent(eEvents::EventArcaneAberrationFortification, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfFortification);
                        break;
                    }
                    /// Phase 4
                    case eEvents::EventMarkOfChaosReplication:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::MarkOfChaosReplicationAura, false);
                            me->CastSpell(l_Target, eSpells::MarkOfChaosCosmetic, true);
                        }

                        Talk(eTalks::MarkOfChaos);
                        m_Events.ScheduleEvent(eEvents::EventMarkOfChaosReplication, 51 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfReplication);
                        break;
                    }
                    case eEvents::EventForceNovaReplication:
                    {
                        me->CastSpell(me, eSpells::ForceNovaReplicationCasting, false);
                        me->CastSpell(me, eSpells::ForceNovaScriptEffect, true);

                        Talk(eTalks::ForceNova);

                        m_Events.ScheduleEvent(eEvents::EventForceNovaReplication, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfReplication);
                        break;
                    }
                    case eEvents::EventArcaneWrathReplication:
                    {
                        Talk(eTalks::ArcaneWrath);
                        me->CastSpell(me, eSpells::ArcaneWrathReplicationSearcher, false);
                        me->CastSpell(me, eSpells::ArcaneWrathCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventArcaneWrathReplication, 50 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfReplication);
                        break;
                    }
                    case eEvents::EventDestructiveResonanceReplication:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceReplicationSearch, false);
                        me->CastSpell(me, eSpells::DestructiveResonanceCosmetic, true);
                        m_Events.ScheduleEvent(eEvents::EventDestructiveResonanceReplication, 15 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfReplication);
                        break;
                    }
                    case eEvents::EventArcaneAberrationReplication:
                    {
                        me->CastSpell(me, eSpells::SummonReplicatingArcaneAberration, false);
                        me->CastSpell(me, eSpells::SummonArcaneAberrationCosmetic, true);
                        Talk(eTalks::ArcaneAberration);
                        m_Events.ScheduleEvent(eEvents::EventArcaneAberrationReplication, 45 * TimeConstants::IN_MILLISECONDS, 0, ePhases::RuneOfReplication);
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
                /// Used for Phase 1 & Phase 2
                if (m_IsInNova)
                {
                    m_NovaTime += p_Diff;

                    float l_YardsPerMs = 7.0f / (float)TimeConstants::IN_MILLISECONDS;
                    /// Base min radius is 10 yards, which is increased depending on time passed
                    float l_MinRadius = 8.0f;
                    float l_InnerRange = 6.0f;

                    l_MinRadius += (l_YardsPerMs * m_NovaTime);

                    uint64 l_TriggerGuid = 0;
                    AreaTrigger* l_Trigger = nullptr;
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
                /// Phase 3
                else
                {
                    std::set<uint64> l_AffectedPlayers;

                    for (uint8 l_I = 0; l_I < eDatas::MaxNovaPhase3; ++l_I)
                    {
                        if (m_IsInNovaPhase3[l_I])
                        {
                            m_NovaTimePhase3[l_I] += p_Diff;

                            float l_YardsPerMs = 7.0f / (float)TimeConstants::IN_MILLISECONDS;
                            /// Base min radius is 10 yards, which is increased depending on time passed
                            float l_MinRadius = 8.0f;
                            float l_InnerRange = 6.0f;

                            l_MinRadius += (l_YardsPerMs * m_NovaTimePhase3[l_I]);

                            std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                            for (HostileReference* l_Ref : l_ThreatList)
                            {
                                if (Player* l_Player = Player::GetPlayer(*me, l_Ref->getUnitGuid()))
                                {
                                    if (l_Player->GetDistance(m_NovaPosPhase3[l_I]) >= (l_MinRadius - l_InnerRange) &&
                                        l_Player->GetDistance(m_NovaPosPhase3[l_I]) <= l_MinRadius)
                                        l_AffectedPlayers.insert(l_Player->GetGUID());
                                }
                            }
                        }
                    }

                    std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                    for (HostileReference* l_Ref : l_ThreatList)
                    {
                        if (Player* l_Player = Player::GetPlayer(*me, l_Ref->getUnitGuid()))
                        {
                            if (l_AffectedPlayers.find(l_Player->GetGUID()) != l_AffectedPlayers.end())
                            {
                                if (!l_Player->HasAura(eSpells::ForceNovaDoT))
                                    me->CastSpell(l_Player, eSpells::ForceNovaDoT, true);
                            }
                            else
                            {
                                if (l_Player->HasAura(eSpells::ForceNovaDoT))
                                    l_Player->RemoveAura(eSpells::ForceNovaDoT);
                            }
                        }
                    }
                }
            }

            void ScheduleSecondPhase()
            {
                me->AttackStop();
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

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

                    uint32 l_Time = m_Events.GetEventTime(eEvents::EventMarkOfChaos);
                    m_Events.ScheduleEvent(eEvents::EventMarkOfChaosDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                    l_Time = m_Events.GetEventTime(eEvents::EventForceNova);
                    m_Events.ScheduleEvent(eEvents::EventForceNovaDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                    l_Time = m_Events.GetEventTime(eEvents::EventArcaneWrath);
                    m_Events.ScheduleEvent(eEvents::EventArcaneWrathDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                    l_Time = m_Events.GetEventTime(eEvents::EventDestructiveResonance);
                    m_Events.ScheduleEvent(eEvents::EventDestructiveResonanceDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);

                    l_Time = m_Events.GetEventTime(eEvents::EventArcaneAberration) + 13 * TimeConstants::IN_MILLISECONDS;
                    m_Events.ScheduleEvent(eEvents::EventArcaneAberrationDisplacement, l_Time, 0, ePhases::RuneOfDisplacement);
                });
            }

            void ScheduleFirstTransitionPhase()
            {
                me->AttackStop();
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->CastSpell(me, eSpells::EncounterEvent, true);
                me->CastSpell(me, eSpells::TeleportToFortification, true);

                Talk(eTalks::TalkRuneOfFortification1);

                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpells::ArcaneProtection, true);
                    me->CastSpell(me, eSpells::TransitionVisualPhase3, true);

                    me->SetAIAnimKitId(eAnimKit::AnimKitFlyingRune);

                    me->SetAnimTier(3);
                    me->SetDisableGravity(true);
                    me->SetPlayerHoverAnim(true);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ += 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveUp, l_Pos);
                });

                AddTimedDelayedOperation(10 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfFortification2);

                    me->CastSpell(me, eSpells::AwakenRunestone, false);

                    std::list<Creature*> l_TriggerList;
                    me->GetCreatureListWithEntryInGrid(l_TriggerList, eCreatures::WarmageSummonStalker, 150.0f);

                    for (Creature* l_Stalker : l_TriggerList)
                        l_Stalker->CastSpell(l_Stalker, eSpells::SummonRuneVisual, true);

                    m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventSummonWarmages, 1 * TimeConstants::IN_MILLISECONDS);
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

                    me->RemoveAura(eSpells::DisplacementTransform);
                    me->CastSpell(me, eSpells::FortificationTransform, true);

                    me->RemoveAura(eSpells::PowerOfDisplacement);
                    me->CastSpell(me, eSpells::PowerOfFortification, true);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ -= 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveDown, l_Pos);
                });

                AddTimedDelayedOperation(75 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfFortification5);

                    ++m_Phase;

                    m_Events.SetPhase(m_Phase);

                    uint32 l_Time = m_Events.GetEventTime(eEvents::EventMarkOfChaosDisplacement);
                    m_Events.ScheduleEvent(eEvents::EventMarkOfChaosFortification, l_Time, 0, ePhases::RuneOfFortification);

                    l_Time = m_Events.GetEventTime(eEvents::EventForceNovaDisplacement);
                    m_Events.ScheduleEvent(eEvents::EventForceNovaFortification, l_Time, 0, ePhases::RuneOfFortification);

                    l_Time = m_Events.GetEventTime(eEvents::EventArcaneWrathDisplacement);
                    m_Events.ScheduleEvent(eEvents::EventArcaneWrathFortification, l_Time, 0, ePhases::RuneOfFortification);

                    l_Time = m_Events.GetEventTime(eEvents::EventDestructiveResonanceDisplacement);
                    m_Events.ScheduleEvent(eEvents::EventDestructiveResonanceFortification, l_Time, 0, ePhases::RuneOfFortification);

                    l_Time = m_Events.GetEventTime(eEvents::EventArcaneAberrationDisplacement);
                    m_Events.ScheduleEvent(eEvents::EventArcaneAberrationFortification, l_Time, 0, ePhases::RuneOfFortification);
                });
            }

            void ScheduleSecondTransitionPhase()
            {
                me->AttackStop();
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->CastSpell(me, eSpells::EncounterEvent, true);
                me->CastSpell(me, eSpells::TeleportToReplication, true);

                Talk(eTalks::TalkRuneOfReplication1);

                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpells::ArcaneProtection, true);
                    me->CastSpell(me, eSpells::TransitionVisualPhase4, true);

                    me->SetAIAnimKitId(eAnimKit::AnimKitFlyingRune);

                    me->SetAnimTier(3);
                    me->SetDisableGravity(true);
                    me->SetPlayerHoverAnim(true);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ += 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveUp, l_Pos);
                });

                AddTimedDelayedOperation(10 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfReplication2);

                    me->CastSpell(me, eSpells::AwakenRunestone, false);

                    std::list<Creature*> l_TriggerList;
                    me->GetCreatureListWithEntryInGrid(l_TriggerList, eCreatures::WarmageSummonStalker, 150.0f);

                    for (Creature* l_Stalker : l_TriggerList)
                        l_Stalker->CastSpell(l_Stalker, eSpells::SummonRuneVisual, true);

                    m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventSummonWarmages, 1 * TimeConstants::IN_MILLISECONDS);

                    me->SummonCreature(eCreatures::GorianReaver, g_GorianReaverPos);
                });

                AddTimedDelayedOperation(41 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfReplication3);
                });

                AddTimedDelayedOperation(70 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->RemoveAura(eSpells::AwakenRunestone);

                    me->RemoveAura(eSpells::FortificationTransform);
                    me->RemoveAura(eSpells::ArcaneProtection);
                    me->CastSpell(me, eSpells::DisplacementTransform2, true);

                    me->RemoveAura(eSpells::PowerOfFortification);
                    me->CastSpell(me, eSpells::PowerOfReplication, true);

                    Position l_Pos = *me;
                    l_Pos.m_positionZ -= 16.0f;
                    me->GetMotionMaster()->MovePoint(eMoves::MoveDown, l_Pos);
                });

                AddTimedDelayedOperation(75 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::TalkRuneOfReplication4);

                    ++m_Phase;

                    m_Events.SetPhase(m_Phase);

                    uint32 l_Time = m_Events.GetEventTime(eEvents::EventMarkOfChaosFortification);
                    m_Events.ScheduleEvent(eEvents::EventMarkOfChaosReplication, l_Time, 0, ePhases::RuneOfReplication);

                    l_Time = m_Events.GetEventTime(eEvents::EventForceNovaFortification);
                    m_Events.ScheduleEvent(eEvents::EventForceNovaReplication, l_Time, 0, ePhases::RuneOfReplication);

                    l_Time = m_Events.GetEventTime(eEvents::EventArcaneWrathFortification);
                    m_Events.ScheduleEvent(eEvents::EventArcaneWrathReplication, l_Time, 0, ePhases::RuneOfReplication);

                    l_Time = m_Events.GetEventTime(eEvents::EventDestructiveResonanceFortification);
                    m_Events.ScheduleEvent(eEvents::EventDestructiveResonanceReplication, l_Time, 0, ePhases::RuneOfReplication);

                    l_Time = m_Events.GetEventTime(eEvents::EventArcaneAberrationFortification);
                    m_Events.ScheduleEvent(eEvents::EventArcaneAberrationReplication, l_Time, 0, ePhases::RuneOfReplication);
                });
            }

            void ResetRunes()
            {
                std::list<Creature*> l_TriggerList;
                me->GetCreatureListWithEntryInGrid(l_TriggerList, eHighmaulGameobjects::SLGGenericMoPLargeAoI, 200.0f);

                for (Creature* l_Trigger : l_TriggerList)
                {
                    l_Trigger->RemoveAura(eSpells::FortificationAchievement);
                    l_Trigger->RemoveAura(eSpells::ReplicationAchievement);
                    l_Trigger->RemoveAura(eSpells::VolatileAnomalies);
                }

                l_TriggerList.clear();

                me->GetCreatureListWithEntryInGrid(l_TriggerList, eCreatures::NpcRuneOfDisplacement, 200.0f);

                for (Creature* l_Trigger : l_TriggerList)
                {
                    l_Trigger->RemoveAura(eSpells::FortificationRuneActive);
                    l_Trigger->RemoveAura(eSpells::ReplicationRuneActive);
                }
            }

            void DespawnAdds()
            {
                std::list<Creature*> l_CreatureList;
                me->GetCreatureListWithEntryInGrid(l_CreatureList, eCreatures::GorianWarmage, 200.0f);

                for (Creature* l_Iter : l_CreatureList)
                    l_Iter->DespawnOrUnsummon();

                l_CreatureList.clear();
                me->GetCreatureListWithEntryInGrid(l_CreatureList, eCreatures::VolatileAnomaly, 200.0f);

                for (Creature* l_Iter : l_CreatureList)
                    l_Iter->DespawnOrUnsummon();

                l_CreatureList.clear();
                me->GetCreatureListWithEntryInGrid(l_CreatureList, eCreatures::ArcaneRemnant, 200.0f);

                for (Creature* l_Iter : l_CreatureList)
                    l_Iter->DespawnOrUnsummon();
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

        struct npc_highmaul_rune_of_displacementAI : public ScriptedAI
        {
            npc_highmaul_rune_of_displacementAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override { }

            void UpdateAI(uint32 const /*p_Diff*/) override { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_rune_of_displacementAI(p_Creature);
        }
};

/// Arcane Aberration - 77809
/// Displacing Arcane Aberration - 77879
/// Fortified Arcane Aberration - 77878
/// Replicating Arcane Aberration - 77877
class npc_highmaul_arcane_aberration : public CreatureScript
{
    public:
        npc_highmaul_arcane_aberration() : CreatureScript("npc_highmaul_arcane_aberration") { }

        enum eSpells
        {
            /// Phase 1
            CollapsingEntityAura    = 158703,
            CollapsingEntityTrigger = 158705,
            ReverseDeath            = 157099,
            /// Phase 2
            DisplacerCharge         = 157254,
            ImpactfulPulse          = 160367,
            /// Phase  3
            Fortified               = 157252,
            /// Phase 4
            Replicator              = 157249,
            Replicate               = 160341
        };

        enum eCreatures
        {
            ClassicArcaneAberration     = 77809,
            DisplacingArcaneAberration  = 77879,
            FortifiedArcaneAberration   = 77878,
            ReplicatingArcaneAberration = 77877
        };

        enum eVisuals
        {
            ClassicAberration       = 42489,
            DisplacingAberration    = 45790,
            FortifiedAberration     = 45791,
            ReplicatingAberration   = 45792
        };

        enum eData
        {
            MaxRemnants = 7
        };

        struct npc_highmaul_arcane_aberrationAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_arcane_aberrationAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

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
                    case eCreatures::FortifiedArcaneAberration:
                    {
                        /// The Arcane Aberrations are now Fortified Icon Fortified, having 75% more health
                        /// And being immune to all crowd-control effects.
                        me->CastSpell(me, eSpells::Fortified, true);
                        break;
                    }
                    case eCreatures::ReplicatingArcaneAberration:
                    {
                        me->CastSpell(me, eSpells::Replicator, true);
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

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                /// I don't know why, but it's sent twice on retail servers
                if (p_SpellInfo->Id == eSpells::CollapsingEntityTrigger)
                {
                    switch (me->GetEntry())
                    {
                        case eCreatures::ClassicArcaneAberration:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::ClassicAberration, 0);
                            me->SendPlaySpellVisualKit(eVisuals::ClassicAberration, 0);
                            break;
                        }
                        case eCreatures::DisplacingArcaneAberration:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::DisplacingAberration, 0);
                            me->SendPlaySpellVisualKit(eVisuals::DisplacingAberration, 0);
                            break;
                        }
                        case eCreatures::FortifiedArcaneAberration:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::FortifiedAberration, 0);
                            me->SendPlaySpellVisualKit(eVisuals::FortifiedAberration, 0);
                            break;
                        }
                        case eCreatures::ReplicatingArcaneAberration:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::ReplicatingAberration, 0);
                            me->SendPlaySpellVisualKit(eVisuals::ReplicatingAberration, 0);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 2);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                switch (me->GetEntry())
                {
                    case eCreatures::DisplacingArcaneAberration:
                    {
                        me->CastSpell(me, eSpells::ImpactfulPulse, true);
                        break;
                    }
                    case eCreatures::ReplicatingArcaneAberration:
                    {
                        /// Upon death, Replicating Arcane Aberrations split into 7 Arcane Remnants.
                        for (uint8 l_I = 0; l_I < eData::MaxRemnants; ++l_I)
                            me->CastSpell(me, eSpells::Replicate, true);

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
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
            DestructiveResonanceAura                = 156639,
            BaseMineVisualAura                      = 156961,
            DestructiveResonanceDebuff              = 159200,
            DestructiveResonanceDamage              = 156673,
            /// Destructive Resonance: Displacement
            DestructiveResonanceDisplacementAura    = 164086,
            DisplacementMineVisualAura              = 156959,
            DisplacementVisualAura                  = 156983,
            DestructiveResonanceGrowScaleAura       = 156941,
            /// Destructive Resonance: Fortification
            DestructiveResonanceFortificationAura   = 164088,
            FortificationMineVisualAura             = 156958,
            FortificationVisualAura                 = 156982,
            /// Destructive Resonance: Replication
            DestructiveResonanceReplicationAura     = 164096,
            ReplicationMineVisualAura               = 156957,
            ReplicationVisualAura                   = 156977,
            DestructiveResonanceReplicating         = 156799
        };

        enum eVisuals
        {
            DestructiveResonanceDespawn             = 41168,
            DestructiveResonanceDisplacementDespawn = 45691,
            DestructiveResonanceFortificationDesp   = 45692,
            DestructiveResonanceReplicationDespawn  = 45693
        };

        enum eDatas
        {
            PhaseID = 1,
            /// Misc
            ReplicationDupliCount = 2
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
                            m_Phase = l_Margok->AI()->GetData(eDatas::PhaseID);
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

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                switch (m_Phase)
                {
                    case ePhases::MightOfTheCrown:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceAura, true);

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveAura(eSpells::DestructiveResonanceAura);

                            me->CastSpell(me, eSpells::BaseMineVisualAura, true);

                            m_CanExplode = true;
                        });

                        break;
                    }
                    case ePhases::RuneOfDisplacement:
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

                        break;
                    }
                    case ePhases::RuneOfFortification:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceFortificationAura, true);

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveAura(eSpells::DestructiveResonanceFortificationAura);

                            me->CastSpell(me, eSpells::FortificationMineVisualAura, true);
                            me->CastSpell(me, eSpells::FortificationVisualAura, true);

                            m_CanExplode = true;
                        });

                        /// The mines caused by Destructive Resonance: Fortification Icon Destructive Resonance: Fortification now last 2 minutes, up from 1 minute.
                        AddTimedDelayedOperation(120 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Despawn();
                        });

                        return;
                    }
                    case ePhases::RuneOfReplication:
                    {
                        me->CastSpell(me, eSpells::DestructiveResonanceReplicationAura, true);

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveAura(eSpells::DestructiveResonanceReplicationAura);

                            me->CastSpell(me, eSpells::ReplicationMineVisualAura, true);
                            me->CastSpell(me, eSpells::ReplicationVisualAura, true);

                            m_CanExplode = true;
                        });

                        break;
                    }
                    default:
                        break;
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

                    switch (m_Phase)
                    {
                        case ePhases::MightOfTheCrown:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::DestructiveResonanceDespawn, 0);
                            break;
                        }
                        case ePhases::RuneOfDisplacement:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::DestructiveResonanceDisplacementDespawn, 0);
                            break;
                        }
                        case ePhases::RuneOfFortification:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::DestructiveResonanceFortificationDesp, 0);
                            break;
                        }
                        case ePhases::RuneOfReplication:
                        {
                            me->SendPlaySpellVisualKit(eVisuals::DestructiveResonanceReplicationDespawn, 0);
                            break;
                        }
                        default:
                            break;
                    }

                    Talk(eTalk::DestructiveResonance, l_Player->GetGUID());

                    Despawn();

                    m_CanExplode = false;
                }
            }

            void Despawn()
            {
                me->RemoveAllAuras();
                me->DespawnOrUnsummon();

                /// In addition to Destructive Resonance's normal effects, two additional Destructive Resonances are created nearby when it detonates or expires.
                if (m_Phase == ePhases::RuneOfReplication)
                {
                    if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                    {
                        if (Creature* l_Margok = Creature::GetCreature(*me, l_InstanceScript->GetData64(eHighmaulCreatures::ImperatorMargok)))
                        {
                            float l_Radius = 8.0f;
                            Position l_Pos = *me;

                            for (uint8 l_I = 0; l_I < eDatas::ReplicationDupliCount; ++l_I)
                            {
                                float l_O = frand(0, 2 * M_PI);
                                float l_X = l_Pos.m_positionX + (l_Radius * cos(l_O));
                                float l_Y = l_Pos.m_positionY + (l_Radius * sin(l_O));

                                l_Margok->CastSpell(l_X, l_Y, l_Pos.m_positionZ, eSpells::DestructiveResonanceReplicating, true);
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_destructive_resonanceAI(p_Creature);
        }
};

/// Destructive Resonance (Replication) - 77681
class npc_highmaul_destructive_resonance_replication : public CreatureScript
{
    public:
        npc_highmaul_destructive_resonance_replication() : CreatureScript("npc_highmaul_destructive_resonance_replication") { }

        enum eSpells
        {
            DestructiveResonanceDebuff              = 159200,
            DestructiveResonanceDamage              = 156673,
            /// Destructive Resonance: Replication
            DestructiveResonanceReplicationAura     = 164096,
            ReplicationMineVisualAura               = 156957,
            ReplicationVisualAura                   = 156977,
            DestructiveResonanceReplicating         = 156799
        };

        enum eVisual
        {
            DestructiveResonanceReplicationDespawn = 45693
        };

        enum eTalk
        {
            DestructiveResonance
        };

        struct npc_highmaul_destructive_resonance_replicationAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_destructive_resonance_replicationAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_CanExplode = false;
            }

            bool m_CanExplode;

            void Reset() override
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->CastSpell(me, eSpells::DestructiveResonanceReplicationAura, true);

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->RemoveAura(eSpells::DestructiveResonanceReplicationAura);

                    me->CastSpell(me, eSpells::ReplicationMineVisualAura, true);
                    me->CastSpell(me, eSpells::ReplicationVisualAura, true);

                    m_CanExplode = true;
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!m_CanExplode)
                    return;

                float l_Radius = 3.0f;

                if (Player* l_Player = me->FindNearestPlayer(l_Radius))
                {
                    me->CastSpell(l_Player, eSpells::DestructiveResonanceDebuff, true);
                    me->CastSpell(l_Player, eSpells::DestructiveResonanceDamage, true);

                    me->SendPlaySpellVisualKit(eVisual::DestructiveResonanceReplicationDespawn, 0);

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
            return new npc_highmaul_destructive_resonance_replicationAI(p_Creature);
        }
};

/// Gorian Warmage - 78121
class npc_highmaul_gorian_warmage : public CreatureScript
{
    public:
        npc_highmaul_gorian_warmage() : CreatureScript("npc_highmaul_gorian_warmage") { }

        enum eSpells
        {
            /// Passive ability that buffs all allies within 25 yards, increasing their damage done by 50% and their attack and casting speed by 50%
            DominanceAura               = 174126,
            Fixate                      = 157763,
            Slow                        = 157801,
            NetherBlast                 = 157769,
            /// Misc
            FortificationAchievement    = 143809,
            PowerOfFortification        = 155040,
            ReplicationAchievement      = 166391,
            PowerOfReplication          = 166389
        };

        enum eEvents
        {
            EventFixate = 1,
            EventSlow,
            EventNetherBlast
        };

        enum eCosmeticEvent
        {
            EventCheckRune = 1
        };

        struct npc_highmaul_gorian_warmageAI : public ScriptedAI
        {
            npc_highmaul_gorian_warmageAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            uint64 m_FixateTarget;

            void Reset() override
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                m_FixateTarget = 0;

                me->CastSpell(me, eSpells::DominanceAura, true);

                if (Player* l_Target = me->SelectNearestPlayerNotGM(50.0f))
                    AttackStart(l_Target);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 2);

                m_Events.Reset();

                m_CosmeticEvents.Reset();

                m_Events.ScheduleEvent(eEvents::EventFixate, 3 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSlow, 5 * TimeConstants::IN_MILLISECONDS);

                m_CosmeticEvents.ScheduleEvent(eCosmeticEvent::EventCheckRune, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    if (me->HasAura(eSpells::PowerOfFortification) && me->HasAura(eSpells::PowerOfReplication))
                        m_Instance->SetData(eHighmaulDatas::ImperatorAchievement, 1);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_CosmeticEvents.Update(p_Diff);

                if (m_CosmeticEvents.ExecuteEvent() == eCosmeticEvent::EventCheckRune)
                {
                    std::list<Creature*> l_TriggerList;
                    me->GetCreatureListWithEntryInGrid(l_TriggerList, eHighmaulGameobjects::SLGGenericMoPLargeAoI, 15.0f);

                    for (Creature* l_Trigger : l_TriggerList)
                    {
                        if (l_Trigger->HasAura(eSpells::FortificationAchievement))
                            me->CastSpell(me, eSpells::PowerOfFortification, true);
                        else if (l_Trigger->HasAura(eSpells::ReplicationAchievement))
                            me->CastSpell(me, eSpells::PowerOfReplication, true);
                    }

                    /// Don't need to do that again if the Warmage already has the two auras
                    if (!me->HasAura(eSpells::PowerOfFortification) || !me->HasAura(eSpells::PowerOfReplication))
                        m_CosmeticEvents.ScheduleEvent(eCosmeticEvent::EventCheckRune, 1 * TimeConstants::IN_MILLISECONDS);
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFixate:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 0.0f, true, -eSpells::Fixate))
                        {
                            m_FixateTarget = l_Target->GetGUID();
                            me->CastSpell(l_Target, eSpells::Fixate, true);
                        }

                        m_Events.ScheduleEvent(eEvents::EventNetherBlast, 100);
                        m_Events.ScheduleEvent(eEvents::EventFixate, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSlow:
                    {
                        me->CastSpell(me, eSpells::Slow, true);
                        m_Events.ScheduleEvent(eEvents::EventSlow, 17 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventNetherBlast:
                    {
                        if (Unit* l_Target = Player::GetPlayer(*me, m_FixateTarget))
                            me->CastSpell(l_Target, eSpells::NetherBlast, false);

                        m_Events.ScheduleEvent(eEvents::EventNetherBlast, 200);
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
            return new npc_highmaul_gorian_warmageAI(p_Creature);
        }
};

/// Volatile Anomaly - 78077
class npc_highmaul_volatile_anomaly : public CreatureScript
{
    public:
        npc_highmaul_volatile_anomaly() : CreatureScript("npc_highmaul_volatile_anomaly") { }

        enum eSpells
        {
            Destabilize     = 157657,
            AlphaFadeOut    = 141608
        };

        enum eVisual
        {
            AnimKit = 5709
        };

        struct npc_highmaul_volatile_anomalyAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_volatile_anomalyAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                if (Player* l_Target = me->SelectNearestPlayerNotGM(100.0f))
                    AttackStart(l_Target);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->SetAIAnimKitId(eVisual::AnimKit);

                me->CastSpell(me, eSpells::Destabilize, true);
                me->CastSpell(me, eSpells::AlphaFadeOut, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_volatile_anomalyAI(p_Creature);
        }
};

/// Gorian Reaver - 78549
class npc_highmaul_gorian_reaver : public CreatureScript
{
    public:
        npc_highmaul_gorian_reaver() : CreatureScript("npc_highmaul_gorian_reaver") { }

        enum eSpells
        {
            DevastatingShockwave    = 158547,
            CrushArmor              = 158553,
            KickToTheFace           = 158563
        };

        enum eEvents
        {
            EventCrushArmor = 1,
            EventKickToTheFace,
            EventDevastatingShockwave
        };

        struct npc_highmaul_gorian_reaverAI : public ScriptedAI
        {
            npc_highmaul_gorian_reaverAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            void Reset() override
            {
                if (Player* l_Target = me->SelectNearestPlayerNotGM(200.0f))
                    AttackStart(l_Target);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 2);

                m_Events.Reset();

                m_Events.ScheduleEvent(eEvents::EventCrushArmor, 22 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventKickToTheFace, 41 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDevastatingShockwave, 12 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
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
                    case eEvents::EventCrushArmor:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::CrushArmor, true);

                        m_Events.ScheduleEvent(eEvents::EventCrushArmor, 11 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventKickToTheFace:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::KickToTheFace, true);
                            me->getThreatManager().modifyThreatPercent(l_Target, -100);
                        }

                        m_Events.ScheduleEvent(eEvents::EventKickToTheFace, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDevastatingShockwave:
                    {
                        me->CastSpell(me, eSpells::DevastatingShockwave, true);
                        m_Events.ScheduleEvent(eEvents::EventDevastatingShockwave, 12 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_highmaul_gorian_reaverAI(p_Creature);
        }
};

/// Arcane Remnant - 79388
class npc_highmaul_arcane_remnant : public CreatureScript
{
    public:
        npc_highmaul_arcane_remnant() : CreatureScript("npc_highmaul_arcane_remnant") { }

        enum eSpell
        {
            ReverseDeath = 157099
        };

        struct npc_highmaul_arcane_remnantAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_arcane_remnantAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->CastSpell(me, eSpell::ReverseDeath, true);

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (Player* l_Target = me->SelectNearestPlayerNotGM(50.0f))
                        AttackStart(l_Target);
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_arcane_remnantAI(p_Creature);
        }
};

/// Mark of Chaos - 158605
/// Mark of Chaos: Displacement - 164176
/// Mark of Chaos: Fortification - 164178
/// Mark of Chaos: Replication - 164191
class spell_highmaul_mark_of_chaos : public SpellScriptLoader
{
    public:
        spell_highmaul_mark_of_chaos() : SpellScriptLoader("spell_highmaul_mark_of_chaos") { }

        enum eSpells
        {
            MarkOfChaosAoE              = 158609,
            FetterMarkOfChaosRootAura   = 158619
        };

        class spell_highmaul_mark_of_chaos_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_mark_of_chaos_AuraScript)

            void AfterAuraRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_DEFAULT)
                    return;

                if (Unit* l_Target = GetTarget())
                {
                    l_Target->CastSpell(l_Target, eSpells::MarkOfChaosAoE, true);
                    l_Target->RemoveAura(eSpells::FetterMarkOfChaosRootAura);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_mark_of_chaos_AuraScript::AfterAuraRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
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
            PrepareAuraScript(spell_highmaul_destructive_resonance_AuraScript)

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
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

        enum eTalk
        {
            Branded = 21
        };

        class spell_highmaul_branded_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_branded_AuraScript)

            void AfterAuraRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
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
                        if (boss_imperator_margok::boss_imperator_margokAI* l_AI = CAST_AI(boss_imperator_margok::boss_imperator_margokAI, l_Margok->GetAI()))
                        {
                            uint64 l_Guid = l_Target->GetGUID();
                            uint64 l_MeGuid = l_Margok->GetGUID();

                            uint32 l_SpellID = GetSpellInfo()->Id;
                            uint8 l_Stacks = p_AurEff->GetBase()->GetStackAmount();
                            l_AI->AddTimedDelayedOperation(100, [l_SpellID, l_Stacks, l_Guid, l_MeGuid]() -> void
                            {
                                uint8 l_StacksCopy = l_Stacks;

                                if (Creature* l_Margok = sObjectAccessor->FindCreature(l_MeGuid))
                                {
                                    if (Unit* l_Target = Unit::GetUnit(*l_Margok, l_Guid))
                                    {
                                        CustomSpellValues l_Values;
                                        l_Values.AddSpellMod(SpellValueMod::SPELLVALUE_AURA_STACK, l_StacksCopy);

                                        l_Margok->CastCustomSpell(eSpells::ArcaneWrathDamage, l_Values, l_Target, true);

                                        /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                                        /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 50%.
                                        float l_JumpRange = 200.0f;
                                        for (uint8 l_I = 0; l_I < l_Stacks; ++l_I)
                                            l_JumpRange -= CalculatePct(l_JumpRange, 50.0f);

                                        if (Player* l_OtherPlayer = l_Target->FindNearestPlayer(l_JumpRange))
                                        {
                                            /// Increase jump count
                                            ++l_StacksCopy;

                                            if (Aura* l_Aura = l_Margok->AddAura(l_SpellID, l_OtherPlayer))
                                            {
                                                l_Aura->SetStackAmount(l_StacksCopy);
                                                l_Margok->AI()->Talk(eTalk::Branded, l_OtherPlayer->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                                            }
                                        }
                                    }
                                }
                            });
                        }
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_branded_AuraScript::AfterAuraRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
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
            ArcaneWrathZoneVisu = 160369,
            ArcaneWrathTeleport = 160370
        };

        enum eData
        {
            BrandedStacks
        };

        enum eTalk
        {
            Branded = 21
        };

        class spell_highmaul_branded_displacement_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_branded_displacement_AuraScript)

            Position m_MarkPos;

            void OnAuraApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    /// Visual area during 4s to see where you can go without being teleported
                    l_Target->CastSpell(l_Target, eSpells::ArcaneWrathZoneVisu, true);
                    m_MarkPos = *l_Target;
                }
            }

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    /// In addition to Arcane Wrath's normal effects, Branded Players are unable to move more than 10 yards from the location they were marked.
                    /// Attempting to leave the marked area will teleport the player back to the location they were originally marked.
                    if (l_Target->GetDistance(m_MarkPos) > 10.0f)
                        l_Target->CastSpell(m_MarkPos, eSpells::ArcaneWrathTeleport, true);
                }
            }

            void AfterAuraRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
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
                        if (boss_imperator_margok::boss_imperator_margokAI* l_AI = CAST_AI(boss_imperator_margok::boss_imperator_margokAI, l_Margok->GetAI()))
                        {
                            uint64 l_Guid = l_Target->GetGUID();
                            uint64 l_MeGuid = l_Margok->GetGUID();

                            uint32 l_SpellID = GetSpellInfo()->Id;
                            uint8 l_Stacks = p_AurEff->GetBase()->GetStackAmount();
                            l_AI->AddTimedDelayedOperation(100, [l_SpellID, l_Stacks, l_Guid, l_MeGuid]() -> void
                            {
                                uint8 l_StackCopy = l_Stacks;

                                if (Creature* l_Margok = sObjectAccessor->FindCreature(l_MeGuid))
                                {
                                    if (Unit* l_Target = Unit::GetUnit(*l_Margok, l_Guid))
                                    {
                                        CustomSpellValues l_Values;
                                        l_Values.AddSpellMod(SpellValueMod::SPELLVALUE_AURA_STACK, l_StackCopy);

                                        l_Margok->CastCustomSpell(eSpells::ArcaneWrathDamage, l_Values, l_Target, true);

                                        /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                                        /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 50%.
                                        float l_JumpRange = 200.0f;
                                        for (uint8 l_I = 0; l_I < l_StackCopy; ++l_I)
                                            l_JumpRange -= CalculatePct(l_JumpRange, 50.0f);

                                        if (Player* l_OtherPlayer = l_Target->FindNearestPlayer(l_JumpRange))
                                        {
                                            /// Increase jump count
                                            ++l_StackCopy;

                                            if (Aura* l_Aura = l_Margok->AddAura(l_SpellID, l_OtherPlayer))
                                            {
                                                l_Aura->SetStackAmount(l_StackCopy);
                                                l_Margok->AI()->Talk(eTalk::Branded, l_OtherPlayer->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                                            }
                                        }
                                    }
                                }
                            });
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_highmaul_branded_displacement_AuraScript::OnAuraApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_branded_displacement_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_branded_displacement_AuraScript::AfterAuraRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_branded_displacement_AuraScript();
        }
};

/// Branded: Fortification - 164005
class spell_highmaul_branded_fortification : public SpellScriptLoader
{
    public:
        spell_highmaul_branded_fortification() : SpellScriptLoader("spell_highmaul_branded_fortification") { }

        enum eSpells
        {
            ArcaneWrathDamage = 156239
        };

        enum eData
        {
            BrandedStacks
        };

        enum eTalk
        {
            Branded = 21
        };

        class spell_highmaul_branded_fortification_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_branded_fortification_AuraScript)

            void AfterAuraRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
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
                        if (boss_imperator_margok::boss_imperator_margokAI* l_AI = CAST_AI(boss_imperator_margok::boss_imperator_margokAI, l_Margok->GetAI()))
                        {
                            uint64 l_Guid = l_Target->GetGUID();
                            uint64 l_MeGuid = l_Margok->GetGUID();

                            uint32 l_SpellID = GetSpellInfo()->Id;
                            uint8 l_Stacks = p_AurEff->GetBase()->GetStackAmount();
                            l_AI->AddTimedDelayedOperation(100, [l_SpellID, l_Stacks, l_Guid, l_MeGuid]() -> void
                            {
                                uint8 l_StacksCopy = l_Stacks;

                                if (Creature* l_Margok = sObjectAccessor->FindCreature(l_MeGuid))
                                {
                                    if (Unit* l_Target = Unit::GetUnit(*l_Margok, l_Guid))
                                    {
                                        CustomSpellValues l_Values;
                                        l_Values.AddSpellMod(SpellValueMod::SPELLVALUE_AURA_STACK, l_StacksCopy);

                                        l_Margok->CastCustomSpell(eSpells::ArcaneWrathDamage, l_Values, l_Target, true);

                                        /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                                        /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 25%.
                                        float l_JumpRange = 200.0f;
                                        for (uint8 l_I = 0; l_I < l_StacksCopy; ++l_I)
                                            l_JumpRange -= CalculatePct(l_JumpRange, 25.0f);

                                        if (Player* l_OtherPlayer = l_Target->FindNearestPlayer(l_JumpRange))
                                        {
                                            /// Increase jump count
                                            ++l_StacksCopy;

                                            if (Aura* l_Aura = l_Margok->AddAura(l_SpellID, l_OtherPlayer))
                                            {
                                                l_Aura->SetStackAmount(l_StacksCopy);
                                                l_Margok->AI()->Talk(eTalk::Branded, l_OtherPlayer->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                                            }
                                        }
                                    }
                                }
                            });
                        }
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_branded_fortification_AuraScript::AfterAuraRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_branded_fortification_AuraScript();
        }
};

/// Branded: Replication - 164006
class spell_highmaul_branded_replication : public SpellScriptLoader
{
    public:
        spell_highmaul_branded_replication() : SpellScriptLoader("spell_highmaul_branded_replication") { }

        enum eSpells
        {
            ArcaneWrathDamage = 156239
        };

        enum eData
        {
            BrandedStacks
        };

        enum eTalk
        {
            Branded = 21
        };

        class spell_highmaul_branded_replication_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_branded_replication_AuraScript)

            void AfterAuraRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
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
                        if (boss_imperator_margok::boss_imperator_margokAI* l_AI = CAST_AI(boss_imperator_margok::boss_imperator_margokAI, l_Margok->GetAI()))
                        {
                            uint64 l_Guid = l_Target->GetGUID();
                            uint64 l_MeGuid = l_Margok->GetGUID();

                            uint32 l_SpellID = GetSpellInfo()->Id;
                            uint8 l_Stacks = p_AurEff->GetBase()->GetStackAmount();
                            l_AI->AddTimedDelayedOperation(100, [l_SpellID, l_Stacks, l_Guid, l_MeGuid]() -> void
                            {
                                uint8 l_StacksCopy = l_Stacks;

                                if (Creature* l_Margok = sObjectAccessor->FindCreature(l_MeGuid))
                                {
                                    if (Unit* l_Target = Unit::GetUnit(*l_Margok, l_Guid))
                                    {
                                        CustomSpellValues l_Values;
                                        l_Values.AddSpellMod(SpellValueMod::SPELLVALUE_AURA_STACK, l_StacksCopy);

                                        l_Margok->CastCustomSpell(eSpells::ArcaneWrathDamage, l_Values, l_Target, true);

                                        /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                                        /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 25%.
                                        float l_JumpRange = 200.0f;
                                        for (uint8 l_I = 0; l_I < l_StacksCopy; ++l_I)
                                            l_JumpRange -= CalculatePct(l_JumpRange, 25.0f);

                                        /// In addition to Arcane Wrath's normal effects, a second player will be Branded the first time Arcane Wrath jumps.
                                        if (l_StacksCopy <= 1)
                                        {
                                            std::list<Player*> l_PlrList;
                                            l_Target->GetPlayerListInGrid(l_PlrList, l_JumpRange);

                                            if (l_PlrList.size() > 2)
                                            {
                                                l_PlrList.sort(JadeCore::ObjectDistanceOrderPred(l_Target));
                                                JadeCore::RandomResizeList(l_PlrList, 2);
                                            }

                                            /// Increase jump count
                                            ++l_StacksCopy;

                                            for (Player* l_Player : l_PlrList)
                                            {
                                                if (Aura* l_Aura = l_Margok->AddAura(l_SpellID, l_Player))
                                                {
                                                    l_Aura->SetStackAmount(l_StacksCopy);
                                                    l_Margok->AI()->Talk(eTalk::Branded, l_Player->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                                                }
                                            }

                                            return;
                                        }

                                        std::list<Player*> l_PlrList;
                                        l_Target->GetPlayerListInGrid(l_PlrList, l_JumpRange);

                                        /// It cannot jumps twice on the same player at the same time
                                        if (!l_PlrList.empty())
                                            l_PlrList.remove_if(JadeCore::UnitAuraCheck(true, l_SpellID));

                                        if (!l_PlrList.empty())
                                        {
                                            l_PlrList.sort(JadeCore::ObjectDistanceOrderPred(l_Target));

                                            if (Player* l_OtherPlayer = l_PlrList.front())
                                            {
                                                /// Increase jump count
                                                ++l_StacksCopy;

                                                if (Aura* l_Aura = l_Margok->AddAura(l_SpellID, l_OtherPlayer))
                                                {
                                                    l_Aura->SetStackAmount(l_StacksCopy);
                                                    l_Margok->AI()->Talk(eTalk::Branded, l_OtherPlayer->GetGUID(), TextRange::TEXT_RANGE_NORMAL);
                                                }
                                            }
                                        }
                                    }
                                }
                            });
                        }
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_branded_replication_AuraScript::AfterAuraRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_branded_replication_AuraScript();
        }
};

/// Arcane Wrath (damage) - 156239
class spell_highmaul_arcane_wrath_damage : public SpellScriptLoader
{
    public:
        spell_highmaul_arcane_wrath_damage() : SpellScriptLoader("spell_highmaul_arcane_wrath_damage") { }

        class spell_highmaul_arcane_wrath_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_arcane_wrath_damage_SpellScript)

            void HandleDamage()
            {
                /// When Branded expires it inflicts Arcane damage to the wearer and jumps to their closest ally within 200 yards.
                /// Each time Arcane Wrath jumps, its damage increases by 25% and range decreases by 50%.
                int32 l_Damage = GetHitDamage();
                uint8 l_Stacks = GetSpell()->GetSpellValue(SpellValueMod::SPELLVALUE_AURA_STACK);
                AddPct(l_Damage, int32(25 * l_Stacks));

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

        enum eSpells
        {
            TransitionVisualMissileP1   = 176581,
            TransitionVisualMissileP2   = 176582,
            TransitionVisualMissileP3   = 176583
        };

        enum eDatas
        {
            PhaseID         = 1,
            MissileCountP1  = 6,
            MissileCountP2  = 3,
            MissileCountP3  = 2
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
            PrepareAuraScript(spell_highmaul_transition_visuals_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
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
                                    l_Target->CastSpell(l_Margok, eSpells::TransitionVisualMissileP1, true);

                                break;
                            }
                            case ePhases::DormantRunestones:
                            {
                                for (uint8 l_I = 0; l_I < eDatas::MissileCountP2; ++l_I)
                                    l_Target->CastSpell(l_Margok, eSpells::TransitionVisualMissileP2, true);

                                break;
                            }
                            case ePhases::LineageOfPower:
                            {
                                for (uint8 l_I = 0; l_I < eDatas::MissileCountP3; ++l_I)
                                    l_Target->CastSpell(l_Margok, eSpells::TransitionVisualMissileP3, true);

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

/// Dominance Aura - 174126
class spell_highmaul_dominance_aura : public SpellScriptLoader
{
    public:
        spell_highmaul_dominance_aura() : SpellScriptLoader("spell_highmaul_dominance_aura") { }

        class spell_highmaul_dominance_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_dominance_aura_AuraScript)

            enum eSpell
            {
                DominanceAuraBuff = 174128
            };

            enum eData
            {
                InvisDisplay = 11686
            };

            uint32 m_CheckTimer;

            bool Load() override
            {
                m_CheckTimer = 200;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= p_Diff)
                    {
                        if (Unit* l_Caster = GetCaster())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 40.0f;

                            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                            l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                            l_TargetList.remove(l_Caster);

                            for (Unit* l_Iter : l_TargetList)
                            {
                                if (l_Iter->GetDisplayId() == eData::InvisDisplay)
                                    continue;

                                if (l_Iter->GetDistance(l_Caster) <= 25.0f)
                                {
                                    if (!l_Iter->HasAura(eSpell::DominanceAuraBuff))
                                        l_Caster->CastSpell(l_Iter, eSpell::DominanceAuraBuff, true);
                                }
                                else
                                {
                                    if (l_Iter->HasAura(eSpell::DominanceAuraBuff))
                                        l_Iter->RemoveAura(eSpell::DominanceAuraBuff);
                                }
                            }
                        }

                        m_CheckTimer = 500;
                    }
                    else
                        m_CheckTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_highmaul_dominance_aura_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_dominance_aura_AuraScript();
        }
};

/// Force Nova: Fortified - 157323
class spell_highmaul_force_nova_fortified : public SpellScriptLoader
{
    public:
        spell_highmaul_force_nova_fortified() : SpellScriptLoader("spell_highmaul_force_nova_fortified") { }

        enum eSpell
        {
            ForceNovaFortificationDummy = 164253
        };

        class spell_highmaul_force_nova_fortified_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_force_nova_fortified_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpell::ForceNovaFortificationDummy, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_force_nova_fortified_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_force_nova_fortified_AuraScript();
        }
};

/// Devastating Shockwave - 158547
class spell_highmaul_devastating_shockwave : public SpellScriptLoader
{
    public:
        spell_highmaul_devastating_shockwave() : SpellScriptLoader("spell_highmaul_devastating_shockwave") { }

        class spell_highmaul_devastating_shockwave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_devastating_shockwave_SpellScript)

            enum eSpell
            {
                TargetRestrict = 19856
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_devastating_shockwave_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_devastating_shockwave_SpellScript();
        }
};

/// Force Nova (DoT) - 157353
class spell_highmaul_force_nova_dot : public SpellScriptLoader
{
    public:
        spell_highmaul_force_nova_dot() : SpellScriptLoader("spell_highmaul_force_nova_dot") { }

        enum eSpell
        {
            ForceNovaReplicationAoEDamage = 157357  ///< Damaging spell for allies
        };

        enum eData
        {
            PhaseID = 1
        };

        enum ePhase
        {
            RuneOfReplication = 6
        };

        class spell_highmaul_force_nova_dot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_force_nova_dot_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Creature* l_Margok = l_Target->FindNearestCreature(eHighmaulCreatures::ImperatorMargok, 40.0f))
                    {
                        if (!l_Margok->IsAIEnabled)
                            return;

                        uint8 l_Phase = l_Margok->AI()->GetData(eData::PhaseID);
                        if (l_Phase != ePhase::RuneOfReplication)
                            return;

                        l_Target->CastSpell(l_Target, eSpell::ForceNovaReplicationAoEDamage, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_force_nova_dot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_force_nova_dot_AuraScript();
        }
};

/// Orbs of Chaos (Dummy aura) - 160447
class spell_highmaul_orbs_of_chaos_aura : public SpellScriptLoader
{
    public:
        spell_highmaul_orbs_of_chaos_aura() : SpellScriptLoader("spell_highmaul_orbs_of_chaos_aura") { }

        enum eSpells
        {
            OrbsOfChaosSummoning = 158639
        };

        enum eTalk
        {
            OrbsOfChaos = 22
        };

        enum eData
        {
            OrbOfChaosAngle = 2
        };

        class spell_highmaul_orbs_of_chaos_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_orbs_of_chaos_aura_AuraScript)

            void AfterAuraRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_DEFAULT)
                    return;

                if (Unit* l_Target = GetTarget())
                {
                    l_Target->CastSpell(l_Target, eSpells::OrbsOfChaosSummoning, true);

                    if (Creature* l_Margok = l_Target->FindNearestCreature(eHighmaulCreatures::ImperatorMargok, 300.0f))
                    {
                        if (!l_Margok->IsAIEnabled)
                            return;

                        l_Margok->AI()->SetData(eData::OrbOfChaosAngle, 0);
                        l_Margok->AI()->Talk(eTalk::OrbsOfChaos, l_Target->GetGUID());

                        if (boss_imperator_margok::boss_imperator_margokAI* l_AI = CAST_AI(boss_imperator_margok::boss_imperator_margokAI, l_Margok->GetAI()))
                        {
                            uint64 l_Guid = l_Target->GetGUID();
                            l_AI->AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [l_AI, l_Guid]() -> void
                            {
                                if (Unit* l_Target = Unit::GetUnit(*l_AI->me, l_Guid))
                                    l_Target->CastSpell(l_Target, eSpells::OrbsOfChaosSummoning, true);
                            });
                        }
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_orbs_of_chaos_aura_AuraScript::AfterAuraRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_orbs_of_chaos_aura_AuraScript();
        }
};

/// Volatile Anomalies - 157265
class spell_highmaul_volatile_anomalies : public SpellScriptLoader
{
    public:
        spell_highmaul_volatile_anomalies() : SpellScriptLoader("spell_highmaul_volatile_anomalies") { }

        enum eData
        {
            PhaseID = 1
        };

        enum eSpells
        {
            VolatileAnomalies1  = 158512,
            VolatileAnomalies2  = 159158,
            VolatileAnomalies3  = 159159
        };

        enum ePhases
        {
            DormantRunestones   = 3,    ///< Intermission: Dormant Runestones
            LineageOfPower      = 5     ///< Intermission: Lineage of Power
        };

        class spell_highmaul_volatile_anomalies_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_volatile_anomalies_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Creature* l_Margok = l_Target->FindNearestCreature(eHighmaulCreatures::ImperatorMargok, 150.0f))
                    {
                        if (!l_Margok->IsAIEnabled)
                            return;

                        switch (l_Margok->AI()->GetData(eData::PhaseID))
                        {
                            case ePhases::DormantRunestones:
                            {
                                l_Target->CastSpell(g_VolatileAnomalyPos[0], eSpells::VolatileAnomalies1, true);
                                l_Target->CastSpell(g_VolatileAnomalyPos[0], eSpells::VolatileAnomalies2, true);
                                l_Target->CastSpell(g_VolatileAnomalyPos[0], eSpells::VolatileAnomalies3, true);
                                break;
                            }
                            case ePhases::LineageOfPower:
                            {
                                l_Target->CastSpell(g_VolatileAnomalyPos[1], eSpells::VolatileAnomalies1, true);
                                l_Target->CastSpell(g_VolatileAnomalyPos[1], eSpells::VolatileAnomalies2, true);
                                l_Target->CastSpell(g_VolatileAnomalyPos[1], eSpells::VolatileAnomalies3, true);
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
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_volatile_anomalies_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_volatile_anomalies_AuraScript();
        }
};

/// Orb of Chaos - 158639
class areatrigger_highmaul_orb_of_chaos : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_orb_of_chaos() : AreaTriggerEntityScript("areatrigger_highmaul_orb_of_chaos")
        {
            m_DamageTimer = 0;
        }

        enum eSpell
        {
            OrbOfChaosDamage = 158648
        };

        enum eData
        {
            OrbOfChaosAngle = 2
        };

        uint32 m_DamageTimer;

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& /*p_SourcePosition*/, Position& p_DestinationPosition, std::list<Position>& /*p_PathToLinearDestination*/) override
        {
            p_AreaTrigger->SetTimeToTarget(30 * TimeConstants::IN_MILLISECONDS);

            m_DamageTimer = 1 * TimeConstants::IN_MILLISECONDS;

            if (InstanceScript* l_InstanceScript = p_Caster->GetInstanceScript())
            {
                if (Creature* l_Margok = Creature::GetCreature(*p_Caster, l_InstanceScript->GetData64(eHighmaulCreatures::ImperatorMargok)))
                {
                    if (!l_Margok->IsAIEnabled)
                        return;

                    float l_Angle = 0.0f;

                    if (Creature* l_Prison = p_Caster->FindNearestCreature(eHighmaulCreatures::KingPrison, 150.0f))
                        l_Angle = p_Caster->GetAngle(l_Prison);

                    uint8 l_Count       = l_Margok->AI()->GetData(eData::OrbOfChaosAngle);
                    float l_AddedVal    = 2 * M_PI / 8.0f;

                    l_Angle = l_Angle + (l_Count * l_AddedVal);

                    if (l_Count >= 7)
                        l_Margok->AI()->SetData(eData::OrbOfChaosAngle, 0);
                    else
                        l_Margok->AI()->SetData(eData::OrbOfChaosAngle, ++l_Count);

                    float l_Radius = 250.0f;
                    p_DestinationPosition.m_positionX = p_DestinationPosition.m_positionX + (l_Radius * cos(l_Angle));
                    p_DestinationPosition.m_positionY = p_DestinationPosition.m_positionY + (l_Radius * sin(l_Angle));
                }
            }
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                if (m_DamageTimer)
                {
                    if (m_DamageTimer <= p_Time)
                    {
                        std::list<Player*> l_TargetList;
                        float l_Radius = 1.5f;

                        JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, l_Radius);
                        JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                        p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                        for (Player* l_Player : l_TargetList)
                            l_Player->CastSpell(l_Player, eSpell::OrbOfChaosDamage, true);

                        m_DamageTimer = 200;
                    }
                    else
                        m_DamageTimer -= p_Time;
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_orb_of_chaos();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_imperator_margok()
{
    /// Boss
    new boss_imperator_margok();

    /// NPCs
    new npc_highmaul_rune_of_displacement();
    new npc_highmaul_arcane_aberration();
    new npc_highmaul_destructive_resonance();
    new npc_highmaul_destructive_resonance_replication();
    new npc_highmaul_gorian_warmage();
    new npc_highmaul_volatile_anomaly();
    new npc_highmaul_gorian_reaver();
    new npc_highmaul_arcane_remnant();

    /// Spells
    new spell_highmaul_mark_of_chaos();
    new spell_highmaul_destructive_resonance();
    new spell_highmaul_branded();
    new spell_highmaul_branded_displacement();
    new spell_highmaul_branded_fortification();
    new spell_highmaul_branded_replication();
    new spell_highmaul_arcane_wrath_damage();
    new spell_highmaul_transition_visuals();
    new spell_highmaul_dominance_aura();
    new spell_highmaul_force_nova_fortified();
    new spell_highmaul_devastating_shockwave();
    new spell_highmaul_force_nova_dot();
    new spell_highmaul_orbs_of_chaos_aura();
    new spell_highmaul_volatile_anomalies();

    /// AreaTrigger
    new areatrigger_highmaul_orb_of_chaos();
}
#endif
