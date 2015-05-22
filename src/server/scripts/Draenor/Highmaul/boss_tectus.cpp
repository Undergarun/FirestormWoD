////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

void RespawnGuardians(Creature* p_Source)
{
    if (p_Source == nullptr)
        return;

    if (Creature* l_Rokka = p_Source->FindNearestCreature(eHighmaulCreatures::Rokka, 100.0f))
    {
        l_Rokka->Respawn();
        l_Rokka->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Lokk = p_Source->FindNearestCreature(eHighmaulCreatures::Lokk, 100.0f))
    {
        l_Lokk->Respawn();
        l_Lokk->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Oro = p_Source->FindNearestCreature(eHighmaulCreatures::Oro, 100.0f))
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
            /// Periodic dummy of 1s
            TheLivingMountain   = 162287,
            /// Arrow visual on player
            CrystallingBarrage  = 162346
        };

        enum eEvents
        {
            EventEnrage = 1,
            EventSpawnBersererk,
            EventSpawnEarthwarper,
            EventCrystallineBarrage,
            /// Used at 25 energy
            EventEarthenPillar,
            EventFracture
        };

        enum eActions
        {
            GuardianDead
        };

        enum eAnimKit
        {
            AnimRise = 6961
        };

        enum eCreatures
        {
            EarthenPillarStalker = 80476
        };

        enum eGameObjects
        {
            GoBEarthenPillar = 229649
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
                m_Instance  = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eHighmaulSpells::Berserker);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
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
                    me->SetAIAnimKit(0);
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

                        me->SetAIAnimKit(0);
                        me->PlayOneShotAnimKit(eAnimKit::AnimRise);

                        AddTimedDelayedOperation(4500, [this]() -> void
                        {
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        });

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

                me->CastSpell(me, eSpells::TheLivingMountain, true);
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
                    m_Instance->SetBossState(eHighmaulDatas::BossTectus, EncounterState::FAIL);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
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
                    case eEvents::EventEnrage:
                        me->CastSpell(me, eHighmaulSpells::Berserker, true);
                        break;
                    case eEvents::EventSpawnBersererk:
                        break;
                    case eEvents::EventSpawnEarthwarper:
                        break;
                    case eEvents::EventCrystallineBarrage:
                        break;
                    case eEvents::EventFracture:
                        m_Events.ScheduleEvent(eEvents::EventFracture, 3 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            bool AllGardiansDead() const
            {
                if (Creature* l_Rokka = me->FindNearestCreature(eHighmaulCreatures::Rokka, 100.0f))
                    return false;

                if (Creature* l_Lokk = me->FindNearestCreature(eHighmaulCreatures::Lokk, 100.0f))
                    return false;

                if (Creature* l_Oro = me->FindNearestCreature(eHighmaulCreatures::Oro, 100.0f))
                    return false;

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
                m_Risen = false;
            }

            bool m_Risen;

            EventMap m_Events;

            void Reset() override
            {
                if (!m_Risen)
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                    me->SetAIAnimKit(0);

                m_Events.Reset();

                RespawnGuardians(me);
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
                    me->SetAIAnimKit(0);
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
                m_Risen = false;
            }

            bool m_Risen;

            EventMap m_Events;

            void Reset() override
            {
                if (!m_Risen)
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                    me->SetAIAnimKit(0);

                m_Events.Reset();

                RespawnGuardians(me);
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
                    me->SetAIAnimKit(0);
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

void AddSC_boss_tectus()
{
    /// Boss
    new boss_tectus();

    /// NPCs
    new npc_highmaul_night_twisted_supplicant();
    new npc_highmaul_rokka_and_lokk();
    new npc_highmaul_oro();

    /// Spells
    new spell_highmaul_meteoric_earthspire();
    new spell_highmaul_stonebolt_volley();
}