////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

uint8 GetEnergyGainFromHealth(float p_HealthPct)
{
    if (p_HealthPct >= 50.0f)
        return 1;
    else if (p_HealthPct >= 20.0f)
        return 2;
    else if (p_HealthPct >= 10.0f)
        return 3;
    else if (p_HealthPct >= 5.0f)
        return 4;
    else
        return 5;
}

void RespawnGuardians(Creature* p_Source, InstanceScript* p_Instance)
{
    if (p_Source == nullptr || p_Instance == nullptr)
        return;

    if (Creature* l_Rokka = Creature::GetCreature(*p_Source, p_Instance->GetData64(eHighmaulCreatures::Rokka)))
    {
        l_Rokka->Respawn();
        l_Rokka->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Lokk = Creature::GetCreature(*p_Source, p_Instance->GetData64(eHighmaulCreatures::Lokk)))
    {
        l_Lokk->Respawn();
        l_Lokk->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Oro = Creature::GetCreature(*p_Source, p_Instance->GetData64(eHighmaulCreatures::Oro)))
    {
        l_Oro->Respawn();
        l_Oro->GetMotionMaster()->MoveTargetedHome();
    }
}

void StartGuardians(Creature* p_Source, Unit* p_Target)
{
    if (p_Source == nullptr || p_Target == nullptr)
        return;

    if (Creature* l_Rokka = p_Source->FindNearestCreature(eHighmaulCreatures::Rokka, 100.0f))
        l_Rokka->AI()->AttackStart(p_Target);

    if (Creature* l_Lokk = p_Source->FindNearestCreature(eHighmaulCreatures::Lokk, 100.0f))
        l_Lokk->AI()->AttackStart(p_Target);

    if (Creature* l_Oro = p_Source->FindNearestCreature(eHighmaulCreatures::Oro, 100.0f))
        l_Oro->AI()->AttackStart(p_Target);
}

/// Tectus <The Living Mountain> - 78948
class boss_tectus : public CreatureScript
{
    public:
        boss_tectus() : CreatureScript("boss_tectus") { }

        enum eSpells
        {
            /// Energy Management
            ZeroPowerZeroRegen          = 118357,
            /// Periodic dummy of 1s
            TheLivingMountain           = 162287,
            /// Arrow visual on player
            CrystallineBarrage          = 162346,
            CrystallineBarrageSummon    = 162371,
            /// +5% damage done
            Accretion                   = 162288
        };

        enum eEvents
        {
            EventEnrage = 1,
            EventSpawnBersererk,
            EventSpawnEarthwarper,
            EventCrystallineBarrage,
            /// Used at 25 energy
            EventEarthenPillar,
            EventFracture,
            EventAccretion
        };

        enum eActions
        {
            GuardianDead,
            ScheduleEarthenPillar
        };

        enum eAnimKit
        {
            AnimRise = 6961
        };

        enum eCreatures
        {
            EarthenPillarStalker = 80476
        };

        enum eTalks
        {
            Aggro,
            Shattered,
            TectonicUpheaval,
            TectonicUpheavalCompleted,
            EarthenPillar,
            Slay,
            Death
        };

        struct boss_tectusAI : public BossAI
        {
            boss_tectusAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTectus)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->CastSpell(me, eSpells::ZeroPowerZeroRegen, true);
                me->CastSpell(me, eSpells::TheLivingMountain, true);

                me->RemoveAura(eHighmaulSpells::Berserker);

                if (!AllGardiansDead())
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                {
                    me->SetAIAnimKitId(0);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }

                me->SetPower(Powers::POWER_ENERGY, 0);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);
            }

            void JustReachedHome() override
            {
                if (!AllGardiansDead())
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                {
                    me->SetAIAnimKitId(0);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::GuardianDead:
                    {
                        if (!AllGardiansDead())
                            return;

                        me->SetAIAnimKitId(0);
                        me->PlayOneShotAnimKit(eAnimKit::AnimRise);

                        AddTimedDelayedOperation(4500, [this]() -> void
                        {
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        });

                        break;
                    }
                    case eActions::ScheduleEarthenPillar:
                    {
                        if (m_Events.HasEvent(eEvents::EventEarthenPillar))
                            break;
                        m_Events.ScheduleEvent(eEvents::EventEarthenPillar, 100);
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

                m_Events.ScheduleEvent(eEvents::EventEnrage, IsMythic() ? 480 * TimeConstants::IN_MILLISECONDS : 600 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSpawnBersererk, 18 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSpawnEarthwarper, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventCrystallineBarrage, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventFracture, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventAccretion, 5 * TimeConstants::IN_MILLISECONDS);

                if (m_Instance)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
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
                    m_Instance->SetBossState(eHighmaulDatas::BossTectus, EncounterState::FAIL);
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                /*switch (p_SpellInfo->Id)
                {
                    default:
                        break;
                }*/
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                /// This buff cause Tectus to be unkillable, although he can still be damaged during this time.
                if (me->HasAura(eSpells::TheLivingMountain) && me->HealthBelowPctDamaged(1, p_Damage))
                {
                    me->SetHealth(1);
                    p_Damage = 0;
                    return;
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value) override
            {
                /// Tectus only regens by script
                p_Value = 0;
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
                    case eEvents::EventEnrage:
                        me->CastSpell(me, eHighmaulSpells::Berserker, true);
                        break;
                    case eEvents::EventSpawnBersererk:
                        break;
                    case eEvents::EventSpawnEarthwarper:
                        break;
                    case eEvents::EventCrystallineBarrage:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(l_Target, eSpells::CrystallineBarrage, true);

                            AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                float l_Range = 0.01f;
                                float l_Orientation = me->GetOrientation();
                                float l_OrigX = me->GetPositionX();
                                float l_OrigY = me->GetPositionY();
                                float l_Z = me->GetPositionZ();

                                float l_X = l_OrigX + (l_Range * cos(l_Orientation - M_PI / 3));
                                float l_Y = l_OrigY + (l_Range * sin(l_Orientation - M_PI / 3));
                                me->CastSpell(l_X, l_Y, l_Z, eSpells::CrystallineBarrageSummon, true);

                                l_X = l_OrigX + (l_Range * cos(l_Orientation + M_PI / 3));
                                l_Y = l_OrigY + (l_Range * sin(l_Orientation + M_PI / 3));
                                me->CastSpell(l_X, l_Y, l_Z, eSpells::CrystallineBarrageSummon, true);
                            });
                        }

                        m_Events.ScheduleEvent(eEvents::EventCrystallineBarrage, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventFracture:
                        m_Events.ScheduleEvent(eEvents::EventFracture, 3 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventEarthenPillar:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -10.0f))
                            me->SummonCreature(eCreatures::EarthenPillarStalker, *l_Target);

                        if (!m_Events.HasEvent(eEvents::EventEarthenPillar))
                            m_Events.ScheduleEvent(eEvents::EventEarthenPillar, 60 * TimeConstants::IN_MILLISECONDS);

                        break;
                    }
                    case eEvents::EventAccretion:
                        me->CastSpell(me, eSpells::Accretion, true);
                        m_Events.ScheduleEvent(eEvents::EventAccretion, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            bool AllGardiansDead() const
            {
                if (m_Instance == nullptr)
                    return false;

                if (Creature* l_Rokka = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Rokka)))
                {
                    if (l_Rokka->isAlive())
                        return false;
                }

                if (Creature* l_Lokk = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Lokk)))
                {
                    if (l_Lokk->isAlive())
                        return false;
                }

                if (Creature* l_Oro = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Oro)))
                {
                    if (l_Oro->isAlive())
                        return false;
                }

                return true;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_tectusAI(p_Creature);
        }
};

/// Night-Twisted Supplicant - 86185
class npc_highmaul_night_twisted_supplicant : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_supplicant() : CreatureScript("npc_highmaul_night_twisted_supplicant") { }

        enum eSpell
        {
            NightTwistedCovenant = 172138
        };

        enum eDisplay
        {
            InvisDisplay = 11686
        };

        enum eAction
        {
            Rise
        };

        struct npc_highmaul_night_twisted_supplicantAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_night_twisted_supplicantAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                SetCanSeeEvenInPassiveMode(true);
            }

            bool CanRespawn() override
            {
                /// This mob is only cosmetic for Rokka, Lokk and Oro spawn
                return false;
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo)
            {
                if (p_Target == nullptr || p_SpellInfo->Id != eSpell::NightTwistedCovenant)
                    return;

                p_Target->SetDisplayId(eDisplay::InvisDisplay);
            }

            void MoveInLineOfSight(Unit* p_Mover) override
            {
                if (me->GetDisplayId() == eDisplay::InvisDisplay)
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (p_Mover->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                std::list<Creature*> l_OtherMe;
                me->GetCreatureListWithEntryInGrid(l_OtherMe, me->GetEntry(), 100.0f);

                for (Creature* l_Covenant : l_OtherMe)
                    l_Covenant->CastSpell(l_Covenant, eSpell::NightTwistedCovenant, false);

                AddTimedDelayedOperation(5000, [this]() -> void
                {
                    if (Creature* l_Rokka = me->FindNearestCreature(eHighmaulCreatures::Rokka, 100.0f))
                        l_Rokka->AI()->DoAction(eAction::Rise);

                    if (Creature* l_Lokk = me->FindNearestCreature(eHighmaulCreatures::Lokk, 100.0f))
                        l_Lokk->AI()->DoAction(eAction::Rise);

                    if (Creature* l_Oro = me->FindNearestCreature(eHighmaulCreatures::Oro, 100.0f))
                        l_Oro->AI()->DoAction(eAction::Rise);
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_supplicantAI(p_Creature);
        }
};

/// Rokkaa <Heart of Tectus> - 86071
/// Lokk <Hands of Tectus> - 86073
class npc_highmaul_rokka_and_lokk : public CreatureScript
{
    public:
        npc_highmaul_rokka_and_lokk() : CreatureScript("npc_highmaul_rokka_and_lokk") { }

        enum eAnimKit
        {
            AnimRise = 6961
        };

        enum eActions
        {
            Rise         = 0,
            GuardianDead = 0
        };

        enum eSpells
        {
            /// Grow scale, regens 10% of health every second
            Reconstitution      = 172116,
            ReconstitutionScale = 172126,
            /// AoE on caster
            EarthenThrust       = 172115,
            /// Trigger 172073 missile
            /// If no player hit, casts 172078
            MeteoricEarthspire  = 172110
        };

        enum eEvents
        {
            EventEarthenThrust = 1,
            EventMeteoricEarthspire,
            EventReconstitution
        };

        struct npc_highmaul_rokka_and_lokkAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_rokka_and_lokkAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Risen = false;
            }

            bool m_Risen;

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                if (!m_Risen)
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                    me->SetAIAnimKitId(0);

                m_Events.Reset();

                RespawnGuardians(me, m_Instance);
            }

            void JustReachedHome() override
            {
                if (m_Risen)
                    me->SetAIAnimKitId(0);
            }

            bool CanRespawn() override
            {
                return false;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartGuardians(me, p_Attacker);

                m_Events.ScheduleEvent(eEvents::EventEarthenThrust, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMeteoricEarthspire, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_Events.HasEvent(eEvents::EventReconstitution) || me->HasAura(eSpells::ReconstitutionScale))
                    return;

                if (me->HealthBelowPctDamaged(21, p_Damage))
                    m_Events.ScheduleEvent(eEvents::EventReconstitution, 100);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::Rise && !m_Risen)
                {
                    m_Risen = true;
                    me->SetAIAnimKitId(0);
                    me->PlayOneShotAnimKit(eAnimKit::AnimRise);

                    AddTimedDelayedOperation(4500, [this]() -> void
                    {
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    });
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                if (Creature* l_Tectus = me->FindNearestCreature(eHighmaulCreatures::Tectus, 100.0f))
                    l_Tectus->AI()->DoAction(eActions::GuardianDead);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventEarthenThrust:
                        me->CastSpell(me, eSpells::EarthenThrust, false);
                        m_Events.ScheduleEvent(eEvents::EventEarthenThrust, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventMeteoricEarthspire:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::MeteoricEarthspire, false);
                        m_Events.ScheduleEvent(eEvents::EventMeteoricEarthspire, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventReconstitution:
                        me->CastSpell(me, eSpells::ReconstitutionScale, true);
                        me->CastSpell(me, eSpells::Reconstitution, false);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_rokka_and_lokkAI(p_Creature);
        }
};

/// Oro <Wrath of Tectus> - 86072
class npc_highmaul_oro : public CreatureScript
{
    public:
        npc_highmaul_oro() : CreatureScript("npc_highmaul_oro") { }

        enum eAnimKit
        {
            AnimRise = 6961
        };

        enum eActions
        {
            Rise         = 0,
            GuardianDead = 0
        };

        enum eSpells
        {
            /// Grow scale, regens 10% of health every second
            Reconstitution      = 172116,
            ReconstitutionScale = 172126,
            /// Launches Stonebolt at targets further than 5 yards
            StoneboltVolley     = 172058,
            /// Poisons a player, inflicting damage to his allies
            RadiatingPoison     = 172066
        };

        enum eEvents
        {
            EventStoneboltVolley = 1,
            EventRadiatingPoison,
            EventReconstitution
        };

        struct npc_highmaul_oroAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_oroAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Risen = false;
            }

            bool m_Risen;

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                if (!m_Risen)
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                    me->SetAIAnimKitId(0);

                m_Events.Reset();

                RespawnGuardians(me, m_Instance);
            }

            void JustReachedHome() override
            {
                if (m_Risen)
                    me->SetAIAnimKitId(0);
            }

            bool CanRespawn() override
            {
                return false;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartGuardians(me, p_Attacker);

                m_Events.ScheduleEvent(eEvents::EventStoneboltVolley, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRadiatingPoison, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::Rise && !m_Risen)
                {
                    m_Risen = true;
                    me->SetAIAnimKitId(0);
                    me->PlayOneShotAnimKit(eAnimKit::AnimRise);

                    AddTimedDelayedOperation(4500, [this]() -> void
                    {
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    });
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_Events.HasEvent(eEvents::EventReconstitution) || me->HasAura(eSpells::ReconstitutionScale))
                    return;

                if (me->HealthBelowPctDamaged(21, p_Damage))
                    m_Events.ScheduleEvent(eEvents::EventReconstitution, 100);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (Creature* l_Tectus = me->FindNearestCreature(eHighmaulCreatures::Tectus, 100.0f))
                    l_Tectus->AI()->DoAction(eActions::GuardianDead);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventStoneboltVolley:
                        me->CastSpell(me, eSpells::StoneboltVolley, false);
                        m_Events.ScheduleEvent(eEvents::EventStoneboltVolley, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventRadiatingPoison:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::RadiatingPoison, false);
                        m_Events.ScheduleEvent(eEvents::EventRadiatingPoison, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventReconstitution:
                        me->CastSpell(me, eSpells::ReconstitutionScale, true);
                        me->CastSpell(me, eSpells::Reconstitution, false);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_oroAI(p_Creature);
        }
};

/// Earthen Pillar Stalker - 80476
class npc_highmaul_earthen_pillar_stalker : public CreatureScript
{
    public:
        npc_highmaul_earthen_pillar_stalker() : CreatureScript("npc_highmaul_earthen_pillar_stalker") { }

        struct npc_highmaul_earthen_pillar_stalkerAI: public ScriptedAI
        {
            npc_highmaul_earthen_pillar_stalkerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                EarthenPillarTimer  = 166024,
                EarthenPillarKill   = 162522
            };

            enum eGameObject
            {
                GoBEarthenPillar = 229649
            };

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->CastSpell(me, eSpells::EarthenPillarTimer, true);

                me->SummonGameObject(eGameObject::GoBEarthenPillar, *me, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, nullptr, 100, 0, false, true);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action)
                    return;

                me->CastSpell(me, eSpells::EarthenPillarKill, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_earthen_pillar_stalkerAI(p_Creature);
        }
};

/// Meteoric Earthspire - 172073
class spell_highmaul_meteoric_earthspire : public SpellScriptLoader
{
    public:
        spell_highmaul_meteoric_earthspire() : SpellScriptLoader("spell_highmaul_meteoric_earthspire") { }

        class spell_highmaul_meteoric_earthspire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_meteoric_earthspire_SpellScript);

            enum eSpell
            {
                RupturedEarth = 172078
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (!p_Targets.empty())
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (WorldLocation const* l_Location = GetExplTargetDest())
                        l_Caster->CastSpell(l_Location, eSpell::RupturedEarth, true);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_meteoric_earthspire_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_meteoric_earthspire_SpellScript();
        }
};

/// Stonebolt Volley - 172058
class spell_highmaul_stonebolt_volley : public SpellScriptLoader
{
    public:
        spell_highmaul_stonebolt_volley() : SpellScriptLoader("spell_highmaul_stonebolt_volley") { }

        class spell_highmaul_stonebolt_volley_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_stonebolt_volley_SpellScript);

            enum eSpell
            {
                RupturedEarth = 172078
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (p_Object->GetDistance(l_Caster) <= 5.0f)
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_stonebolt_volley_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_stonebolt_volley_SpellScript();
        }
};

/// The Living Mountain - 162287
/// Shard of the Mountain - 162658
class spell_highmaul_tectus_energy_gain : public SpellScriptLoader
{
    public:
        spell_highmaul_tectus_energy_gain() : SpellScriptLoader("spell_highmaul_tectus_energy_gain") { }

        class spell_highmaul_tectus_energy_gain_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_tectus_energy_gain_AuraScript);

            enum eAction
            {
                ScheduleEarthenPillar = 1
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    if (!l_Target->isInCombat())
                        return;

                    l_Target->EnergizeBySpell(l_Target, GetSpellInfo()->Id, GetEnergyGainFromHealth(l_Target->GetHealthPct()), Powers::POWER_ENERGY);

                    if (l_Target->IsAIEnabled && l_Target->GetPower(Powers::POWER_ENERGY) >= 25)
                        l_Target->AI()->DoAction(eAction::ScheduleEarthenPillar);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_tectus_energy_gain_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_tectus_energy_gain_AuraScript();
        }
};

/// Earthen Pillar (timer) - 166024
class spell_highmaul_earthen_pillar_timer : public SpellScriptLoader
{
    public:
        spell_highmaul_earthen_pillar_timer() : SpellScriptLoader("spell_highmaul_earthen_pillar_timer") { }

        class spell_highmaul_earthen_pillar_timer_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_earthen_pillar_timer_AuraScript);

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    if (l_Target->IsAIEnabled)
                        l_Target->AI()->DoAction(0);
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_earthen_pillar_timer_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_earthen_pillar_timer_AuraScript();
        }
};

/// Accretion - 162288
class spell_highmaul_accretion : public SpellScriptLoader
{
    public:
        spell_highmaul_accretion() : SpellScriptLoader("spell_highmaul_accretion") { }

        class spell_highmaul_accretion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_accretion_AuraScript);

            uint32 m_DamageTaken;

            bool Load()
            {
                m_DamageTaken = 0;
                return true;
            }

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Target = GetTarget())
                {
                    m_DamageTaken += p_EventInfo.GetDamageInfo()->GetDamage();

                    if (m_DamageTaken >= l_Target->CountPctFromMaxHealth(2))
                    {
                        p_AurEff->GetBase()->DropStack();
                        m_DamageTaken = 0;
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_highmaul_accretion_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_accretion_AuraScript();
        }
};

/// Crystalline Barrage - 162370
class areatrigger_highmaul_crystalline_barrage : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_crystalline_barrage() : AreaTriggerEntityScript("areatrigger_highmaul_crystalline_barrage") { }

        enum eSpell
        {
            CrystallineBarrage = 162370
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 4.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                if (l_TargetList.empty())
                    return;

                l_TargetList.remove_if([this, p_AreaTrigger](Unit* p_Unit) -> bool
                {
                    if (p_Unit == nullptr)
                        return true;

                    if (p_Unit->GetDistance(p_AreaTrigger) > 1.0f && !p_Unit->HasAura(eSpell::CrystallineBarrage))
                        return true;

                    if (p_Unit->GetDistance(p_AreaTrigger) <= 1.0f && p_Unit->HasAura(eSpell::CrystallineBarrage))
                        return true;

                    return false;
                });

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Unit->GetDistance(p_AreaTrigger) <= 1.0f)
                        l_Caster->CastSpell(l_Unit, eSpell::CrystallineBarrage, true);
                    else
                        l_Unit->RemoveAura(eSpell::CrystallineBarrage);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_crystalline_barrage();
        }
};

void AddSC_boss_tectus()
{
    /// Boss
    new boss_tectus();

    /// NPCs
    new npc_highmaul_night_twisted_supplicant();
    new npc_highmaul_rokka_and_lokk();
    new npc_highmaul_oro();
    new npc_highmaul_earthen_pillar_stalker();

    /// Spells
    new spell_highmaul_meteoric_earthspire();
    new spell_highmaul_stonebolt_volley();
    new spell_highmaul_tectus_energy_gain();
    new spell_highmaul_earthen_pillar_timer();
    new spell_highmaul_accretion();

    /// AreaTriggers
    new areatrigger_highmaul_crystalline_barrage();
}