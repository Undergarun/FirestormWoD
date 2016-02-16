////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Kromog <Legend of the Mountain> - 77692
class boss_kromog : public CreatureScript
{
    public:
        boss_kromog() : CreatureScript("boss_kromog") { }

        enum eTalks
        {
            TalkAggro,
            TalkAbility,
            TalkThunderingBlows,
            TalkSlay,
            TalkFrenzy,
            TalkDeath,
            TalkRipplingSmash,
            TalkGraspingEarth
        };

        enum eSpells
        {
            /// Misc
            StoneBreathChannel          = 156852,
            WarpedArmor                 = 156766,
            Slam                        = 156704,
            Frenzy                      = 156861,
            FistsOfStone                = 162348,
            ThunderingBlows             = 157054,
            /// Rippling Smash
            RipplingSmashSearcher       = 161049,
            RipplingSmashCast           = 157592,
            /// Rune of Grasping Earth
            RuneOfGraspingEarthCast     = 157060,
            RuneOfGraspingEarthVehicle  = 157212
        };

        enum eEvents
        {
            EventCheckMeleePlayers = 1,
            EventBerserker,
            EventStoneBreath,
            EventWarpedArmor,
            EventSlam,
            EventRipplingSmash,
            EventGraspingEarth,
            EventThunderingBlows
        };

        enum eTimers
        {
            TimerCheckMeleePlayers  = 500,
            TimerBerserker          = 540 * TimeConstants::IN_MILLISECONDS,
            TimerStoneBreath        = 9 * TimeConstants::IN_MILLISECONDS,
            TimerStoneBreathAgain   = 24 * TimeConstants::IN_MILLISECONDS,
            TimerStoneBreathSecLFR  = 28 * TimeConstants::IN_MILLISECONDS,
            TimerStoneBreathSecond  = 31 * TimeConstants::IN_MILLISECONDS,
            TimerWarpedArmor        = 14 * TimeConstants::IN_MILLISECONDS,
            TimerSlam               = 18 * TimeConstants::IN_MILLISECONDS,
            TimerSlamAgain          = 23 * TimeConstants::IN_MILLISECONDS,
            TimerRipplingSmash      = 23 * TimeConstants::IN_MILLISECONDS + 500,
            TimerRipplingSmashAgain = 21 * TimeConstants::IN_MILLISECONDS,
            TimerRipplingSmashSec   = 35 * TimeConstants::IN_MILLISECONDS,
            TimerGraspingEarth      = 50 * TimeConstants::IN_MILLISECONDS,
            TimerGraspingEarthAgain = 112 * TimeConstants::IN_MILLISECONDS,
            TimerThunderingBlows    = 12 * TimeConstants::IN_MILLISECONDS,
            TimerThunderingBlowsLFR = 20 * TimeConstants::IN_MILLISECONDS + 500
        };

        enum eCosmeticEvents
        {
        };

        enum eActions
        {
            ActionRipplingSmash
        };

        enum eCreatures
        {
            RipplingSmashTrigger    = 78055,
            GraspingEarthTrigger    = 77893
        };

        enum eVisuals
        {
        };

        struct boss_kromogAI : public BossAI
        {
            boss_kromogAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataKromog)
            {
                m_Instance  = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            std::set<uint64> m_RipplingSmashTriggers;
            std::set<uint64> m_GraspingEarthTriggers;

            float m_FrenzyHealthPct;

            bool CanRespawn() override
            {
                return false;
            }

            void Reset() override
            {
                ClearDelayedOperations();

                summons.DespawnAll();

                _Reset();

                me->RemoveAllAreasTrigger();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

                me->RemoveAura(eFoundrySpells::Berserker);
                me->RemoveAura(eSpells::Frenzy);
                me->RemoveAura(eSpells::FistsOfStone);

                if (m_RipplingSmashTriggers.empty())
                {
                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        std::list<Creature*> l_Triggers;
                        me->GetCreatureListWithEntryInGrid(l_Triggers, eCreatures::RipplingSmashTrigger, 15.0f);

                        for (Creature* l_Trigger : l_Triggers)
                            m_RipplingSmashTriggers.insert(l_Trigger->GetGUID());
                    });
                }

                if (m_RipplingSmashTriggers.empty())
                {
                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        std::list<Creature*> l_Triggers;
                        me->GetCreatureListWithEntryInGrid(l_Triggers, eCreatures::GraspingEarthTrigger, 80.0f);

                        for (Creature* l_Trigger : l_Triggers)
                            m_GraspingEarthTriggers.insert(l_Trigger->GetGUID());
                    });
                }

                for (uint64 l_Guid : m_GraspingEarthTriggers)
                {
                    if (Creature* l_Trigger = Creature::GetCreature(*me, l_Guid))
                    {
                        l_Trigger->DespawnOrUnsummon();
                        l_Trigger->Respawn();
                    }
                }

                m_FrenzyHealthPct = 30.0f;
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(eTalks::TalkSlay);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                Talk(eTalks::TalkAggro);

                me->CastSpell(me, eSpells::FistsOfStone, true);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                m_Events.ScheduleEvent(eEvents::EventCheckMeleePlayers, eTimers::TimerCheckMeleePlayers);
                m_Events.ScheduleEvent(eEvents::EventBerserker, eTimers::TimerBerserker);
                m_Events.ScheduleEvent(eEvents::EventStoneBreath, eTimers::TimerStoneBreath);
                m_Events.ScheduleEvent(eEvents::EventWarpedArmor, eTimers::TimerWarpedArmor);
                m_Events.ScheduleEvent(eEvents::EventSlam, eTimers::TimerSlam);
                m_Events.ScheduleEvent(eEvents::EventRipplingSmash, eTimers::TimerRipplingSmash);
                m_Events.ScheduleEvent(eEvents::EventGraspingEarth, eTimers::TimerGraspingEarth);
            }

            void EnterEvadeMode() override
            {
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::WarpedArmor);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RuneOfGraspingEarthVehicle);
                }

                CreatureAI::EnterEvadeMode();

                Reset();
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                Talk(eTalks::TalkDeath);

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::WarpedArmor);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RuneOfGraspingEarthVehicle);
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::RipplingSmashSearcher:
                    {
                        me->SetFacingTo(me->GetAngle(p_Target));

                        uint64 l_Guid = p_Target->GetGUID();
                        AddTimedDelayedOperation(50, [this, l_Guid]() -> void
                        {
                            if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                            {
                                if (Creature* l_Trigger = GetRipplingSmashTrigger(l_Target))
                                {
                                    Talk(eTalks::TalkRipplingSmash, l_Trigger->GetGUID());
                                    me->CastSpell(l_Trigger, eSpells::RipplingSmashCast, false);
                                }
                            }
                        });

                        break;
                    }
                    case eSpells::RipplingSmashCast:
                    {
                        if (Creature* l_Trigger = p_Target->ToCreature())
                        {
                            if (l_Trigger->IsAIEnabled)
                                l_Trigger->AI()->DoAction(eActions::ActionRipplingSmash);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HealthBelowPctDamaged(m_FrenzyHealthPct, p_Damage) && !me->HasAura(eSpells::Frenzy))
                {
                    m_FrenzyHealthPct = 0.0f;

                    me->CastSpell(me, eSpells::Frenzy, true);

                    Talk(eTalks::TalkFrenzy);
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
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
                    case eEvents::EventCheckMeleePlayers:
                    {
                        Map::PlayerList const& l_PlayerList = me->GetMap()->GetPlayers();
                        if (l_PlayerList.isEmpty())
                        {
                            EnterEvadeMode();
                            break;
                        }

                        bool l_InMelee = false;
                        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
                        {
                            if (Player* l_Player = l_Iter->getSource())
                            {
                                if (l_Player->IsWithinMeleeRange(me))
                                {
                                    l_InMelee = true;
                                    break;
                                }
                            }
                        }

                        /// If there are no valid targets in melee range, Kromog will continue to breathe until he finds one.
                        if (!l_InMelee)
                            me->CastSpell(me, eSpells::StoneBreathChannel, false);

                        m_Events.ScheduleEvent(eEvents::EventCheckMeleePlayers, eTimers::TimerCheckMeleePlayers);
                        break;
                    }
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        break;
                    }
                    case eEvents::EventStoneBreath:
                    {
                        me->CastSpell(me, eSpells::StoneBreathChannel, false);
                        m_Events.ScheduleEvent(eEvents::EventStoneBreath, eTimers::TimerStoneBreathAgain);
                        break;
                    }
                    case eEvents::EventWarpedArmor:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::WarpedArmor, true);
                        m_Events.ScheduleEvent(eEvents::EventWarpedArmor, eTimers::TimerWarpedArmor);
                        break;
                    }
                    case eEvents::EventSlam:
                    {
                        me->CastSpell(me, eSpells::Slam, false);
                        m_Events.ScheduleEvent(eEvents::EventSlam, eTimers::TimerSlamAgain);
                        break;
                    }
                    case eEvents::EventRipplingSmash:
                    {
                        me->CastSpell(me, eSpells::RipplingSmashSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventRipplingSmash, eTimers::TimerRipplingSmashAgain);
                        break;
                    }
                    case eEvents::EventGraspingEarth:
                    {
                        /// Thundering Blows Icon Thundering Blows is an ability that Kromog always uses shortly after using Rune of Grasping Earth Icon Rune of Grasping Earth.
                        m_Events.ScheduleEvent(eEvents::EventThunderingBlows, IsLFR() ? eTimers::TimerThunderingBlowsLFR : eTimers::TimerThunderingBlows);

                        /// Those two events have specific timer after Grasping Earth
                        m_Events.RescheduleEvent(eEvents::EventStoneBreath, IsLFR() ? eTimers::TimerStoneBreathSecLFR : eTimers::TimerStoneBreathSecond);
                        m_Events.RescheduleEvent(eEvents::EventRipplingSmash, eTimers::TimerRipplingSmashSec);

                        /// Delay those events to proc after Thundering Blows
                        m_Events.DelayEvent(eEvents::EventCheckMeleePlayers, IsLFR() ? eTimers::TimerThunderingBlowsLFR : eTimers::TimerThunderingBlows);
                        m_Events.DelayEvent(eEvents::EventSlam, IsLFR() ? eTimers::TimerThunderingBlowsLFR : eTimers::TimerThunderingBlows);
                        m_Events.DelayEvent(eEvents::EventWarpedArmor, IsLFR() ? eTimers::TimerThunderingBlowsLFR : eTimers::TimerThunderingBlows);

                        me->SetFacingTo(2.92434f);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                        AddTimedDelayedOperation(50, [this]() -> void
                        {
                            Talk(eTalks::TalkGraspingEarth);

                            me->CastSpell(me, eSpells::RuneOfGraspingEarthCast, false);
                        });

                        m_Events.ScheduleEvent(eEvents::EventGraspingEarth, eTimers::TimerGraspingEarthAgain);
                        break;
                    }
                    case eEvents::EventThunderingBlows:
                    {
                        Talk(eTalks::TalkThunderingBlows);
                        me->CastSpell(me, eSpells::ThunderingBlows, false);

                        AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        });

                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            Creature* GetRipplingSmashTrigger(Unit* p_Target) const
            {
                float l_Distance    = 50000.0f;
                Creature* l_Trigger = nullptr;

                for (uint64 l_Guid : m_RipplingSmashTriggers)
                {
                    if (Creature* l_Iter = Creature::GetCreature(*me, l_Guid))
                    {
                        if (l_Iter->GetDistance(p_Target) < l_Distance)
                        {
                            l_Distance  = l_Iter->GetDistance(p_Target);
                            l_Trigger   = l_Iter;
                        }
                    }
                }

                return l_Trigger;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_kromogAI(p_Creature);
        }
};

/// Rippling Smash - 78055
class npc_foundry_rippling_smash : public CreatureScript
{
    public:
        npc_foundry_rippling_smash() : CreatureScript("npc_foundry_rippling_smash") { }

        enum eSpell
        {
            RipplingSmashAreaTrigger = 161437
        };

        enum eAction
        {
            ActionRipplingSmash
        };

        enum eVisual
        {
            RipplingSmashVisual = 38917
        };

        struct npc_foundry_rippling_smashAI : public ScriptedAI
        {
            npc_foundry_rippling_smashAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionRipplingSmash)
                {
                    me->CastSpell(me, eSpell::RipplingSmashAreaTrigger, true);

                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        float l_X = me->m_positionX + 2.0f * cos(me->m_orientation);
                        float l_Y = me->m_positionY + 2.0f * sin(me->m_orientation);
                        float l_Z = 105.055f;

                        me->SendPlaySpellVisual(eVisual::RipplingSmashVisual, nullptr, 5.0f, 0.0f, { l_X, l_Y, l_Z, 0.0f }, false, true);
                    });
                }
            }

            void UpdateAI(uint32 const p_Diff) override { UpdateOperations(p_Diff); }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_rippling_smashAI(p_Creature);
        }
};

/// Grasping Earth - 77893
class npc_foundry_grasping_earth : public CreatureScript
{
    public:
        npc_foundry_grasping_earth() : CreatureScript("npc_foundry_grasping_earth") { }

        enum eSpells
        {
            GraspingEarthSpawnVisual    = 160957,
            RuneOfGraspingEarthTrigger  = 157056,
            RuneOfGraspingEarthSpawn    = 157061,
            RuneOfGraspingEarthSelect   = 157059,
            RuneOfGraspingEarthVehicle  = 157212
        };

        struct npc_foundry_grasping_earthAI : public ScriptedAI
        {
            npc_foundry_grasping_earthAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_Activated;

            void Reset() override
            {
                m_Activated = false;

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpells::GraspingEarthSpawnVisual, true);
                    //me->SetDisplayId(11686);
                });

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->setFaction(35);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE /*| eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE*/);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN | eUnitFlags2::UNIT_FLAG2_UNK6 | eUnitFlags2::UNIT_FLAG2_UNK11);
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::RuneOfGraspingEarthTrigger:
                    {
                        m_Activated = true;

                        AddTimedDelayedOperation(50, [this]() -> void
                        {
                            me->CastSpell(me, eSpells::RuneOfGraspingEarthSpawn, false);
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
                    case eSpells::RuneOfGraspingEarthSelect:
                    {
                        p_Target->CastSpell(me, eSpells::RuneOfGraspingEarthVehicle, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                if (p_SpellInfo->Id == eSpells::RuneOfGraspingEarthSpawn)
                {
                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        //me->RestoreDisplayId();
                        me->setFaction(14);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE /*| eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE*/);

                        me->ForceValuesUpdateAtIndex(EUnitFields::UNIT_FIELD_DISPLAY_ID);

                        me->CastSpell(me, eSpells::RuneOfGraspingEarthSelect, true);
                    });
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_grasping_earthAI(p_Creature);
        }
};

/// Slam - 156704
class spell_foundry_slam : public SpellScriptLoader
{
    public:
        spell_foundry_slam() : SpellScriptLoader("spell_foundry_slam") { }

        class spell_foundry_slam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_slam_SpellScript);

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Boss = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        /// Kromog strikes the ground beneath his primary target, dealing up to 780000 Physical damage to all players, reduced based on their distance from the impact point.
                        float l_Distance = l_Target->GetDistance(l_Boss);
                        if (l_Distance <= 1.0f)
                            return;

                        int32 l_Damage = float(GetSpell()->GetDamage()) / l_Distance;

                        GetSpell()->SetDamage(l_Damage);
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_foundry_slam_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_slam_SpellScript();
        }
};

/// Fists of Stone - 162348
class spell_foundry_fists_of_stone : public SpellScriptLoader
{
    public:
        spell_foundry_fists_of_stone() : SpellScriptLoader("spell_foundry_fists_of_stone") { }

        enum eSpell
        {
            FistsOfStoneProc = 162349
        };

        class spell_foundry_fists_of_stone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_fists_of_stone_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() != nullptr)
                    return;

                if (Creature* l_Kromog = GetTarget()->ToCreature())
                {
                    if (l_Kromog->HasSpellCooldown(eSpell::FistsOfStoneProc))
                        return;

                    if (Unit* l_Target = p_EventInfo.GetActionTarget())
                    {
                        l_Kromog->CastSpell(l_Target, eSpell::FistsOfStoneProc, true);
                        l_Kromog->_AddCreatureSpellCooldown(eSpell::FistsOfStoneProc, 4 * TimeConstants::IN_MILLISECONDS);
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_foundry_fists_of_stone_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_fists_of_stone_AuraScript();
        }
};

/// Rune of Grasping Earth (periodic) - 157060
class spell_foundry_rune_of_grasping_earth_periodic : public SpellScriptLoader
{
    public:
        spell_foundry_rune_of_grasping_earth_periodic() : SpellScriptLoader("spell_foundry_rune_of_grasping_earth_periodic") { }

        class spell_foundry_rune_of_grasping_earth_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_rune_of_grasping_earth_periodic_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Creature* l_Trigger = l_Caster->FindNearestCreature(86873, 100.0f))
                    {
                        l_Trigger->SetReactState(ReactStates::REACT_PASSIVE);
                        l_Trigger->CombatStop();
                        l_Trigger->CastSpell(l_Trigger, 157056, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundry_rune_of_grasping_earth_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_rune_of_grasping_earth_periodic_AuraScript();
        }
};

/// Rune of Grasping Earth (Searcher) - 157056
class spell_foundry_rune_of_grasping_earth : public SpellScriptLoader
{
    public:
        spell_foundry_rune_of_grasping_earth() : SpellScriptLoader("spell_foundry_rune_of_grasping_earth") { }

        class spell_foundry_rune_of_grasping_earth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_rune_of_grasping_earth_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->GetTypeId() != TypeID::TYPEID_UNIT)
                        return true;

                    if (Creature* l_Trigger = p_Object->ToCreature())
                    {
                        if (npc_foundry_grasping_earth::npc_foundry_grasping_earthAI* l_AI = CAST_AI(npc_foundry_grasping_earth::npc_foundry_grasping_earthAI, l_Trigger->GetAI()))
                        {
                            if (l_AI->m_Activated)
                                return true;
                        }
                        else
                            return true;
                    }
                    else
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_rune_of_grasping_earth_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_rune_of_grasping_earth_SpellScript();
        }
};

/// Rippling Smash - 161437
class areatrigger_foundry_rippling_smash : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_rippling_smash() : AreaTriggerEntityScript("areatrigger_foundry_rippling_smash") { }

        enum eSpell
        {
            RipplingSmashDamage = 157659
        };

        std::set<uint64> m_AffectedTargets;

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination) override
        {
            float l_Orientation = p_Caster->GetOrientation();

            p_DestinationPosition.m_positionX = p_Caster->GetPositionX() + 60.0f * cos(l_Orientation);
            p_DestinationPosition.m_positionY = p_Caster->GetPositionY() + 60.0f * sin(l_Orientation);
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                float l_Radius = 30.0f;
                std::list<Unit*> l_TargetList;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (m_AffectedTargets.find(l_Unit->GetGUID()) != m_AffectedTargets.end())
                        continue;

                    if (!IsInBox(l_Unit, *p_AreaTrigger))
                        continue;

                    m_AffectedTargets.insert(l_Unit->GetGUID());
                    l_Unit->CastSpell(l_Unit, eSpell::RipplingSmashDamage, true, nullptr, NULLAURA_EFFECT, l_Caster->GetGUID());
                }
            }
        }

        bool IsInBox(Unit* p_Target, Position const p_Pos) const
        {
            float l_SizeX = 4.0f;
            float l_SizeY = 21.5f;

            float l_MinX = p_Pos.m_positionX - (l_SizeX / 2.0f);
            float l_MaxX = p_Pos.m_positionX + (l_SizeX / 2.0f);
            float l_MinY = p_Pos.m_positionY - (l_SizeY / 2.0f);
            float l_MaxY = p_Pos.m_positionY + (l_SizeY / 2.0f);

            if (p_Target->m_positionX >= l_MinX && p_Target->m_positionX <= l_MaxX &&
                p_Target->m_positionY >= l_MinY && p_Target->m_positionY <= l_MaxY)
                return true;

            return false;
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_rippling_smash();
        }
};

void AddSC_boss_kromog()
{
    /// Boss
    new boss_kromog();

    /// Creatures
    new npc_foundry_rippling_smash();
    new npc_foundry_grasping_earth();

    /// Spells
    new spell_foundry_slam();
    new spell_foundry_fists_of_stone();
    new spell_foundry_rune_of_grasping_earth_periodic();
    new spell_foundry_rune_of_grasping_earth();

    /// AreaTriggers
    new areatrigger_foundry_rippling_smash();
}