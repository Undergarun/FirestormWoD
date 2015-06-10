////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

/// Ko'ragh <Breaker of Magic> - 79015
class boss_koragh : public CreatureScript
{
    public:
        boss_koragh() : CreatureScript("boss_koragh") { }

        enum eSpells
        {
            /// Cosmetic
            RuneChargingPermanent       = 174415,
            NullificationRuneEmpowered  = 166482,
            /// Nullification Barrier
            NullificationBarrierPower   = 163612,
            NullificationBarrierAbsorb  = 156803
        };

        enum eEvents
        {
        };

        enum eActions
        {
        };

        enum eCreatures
        {
            BreakerOfFel        = 86330,
            BreakerOfFire       = 86329,
            BreakerOfFrost      = 86326,
            RuneOfNullification = 79559
        };

        enum eTalks
        {
            Intro,
            Aggro,
            BarrierShattered,
            ExpelMagic,
            SuppressionField,
            DominatingPower,
            Berserk,
            Slay,
            Death,
            CausticEnergyWarn,
            ExpelMagicFelWarn,
            ExpelMagicArcaneWarn
        };

        enum eAnimKit
        {
            AnimWaiting = 7289
        };

        struct boss_koraghAI : public BossAI
        {
            boss_koraghAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossKoragh)
            {
                m_Instance      = p_Creature->GetInstanceScript();
                m_Init          = false;
                m_FlyingRune    = 0;
                m_FloorRune     = 0;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            std::set<uint64> m_RitualistGuids;
            bool m_Init;

            uint64 m_FlyingRune;
            uint64 m_FloorRune;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                if (m_RitualistGuids.empty() && m_Init)
                {
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                    me->SetAIAnimKitId(0);
                }
                else
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                    me->SetAIAnimKitId(eAnimKit::AnimWaiting);
                }

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (m_RitualistGuids.empty() && !m_Init)
                    {
                        std::list<Creature*> l_BreakerList;
                        me->GetCreatureListWithEntryInGrid(l_BreakerList, eCreatures::BreakerOfFel, 30.0f);

                        for (Creature* l_Breaker : l_BreakerList)
                        {
                            if (l_Breaker->isAlive())
                                m_RitualistGuids.insert(l_Breaker->GetGUID());
                        }

                        l_BreakerList.clear();
                        me->GetCreatureListWithEntryInGrid(l_BreakerList, eCreatures::BreakerOfFire, 30.0f);

                        for (Creature* l_Breaker : l_BreakerList)
                        {
                            if (l_Breaker->isAlive())
                                m_RitualistGuids.insert(l_Breaker->GetGUID());
                        }

                        l_BreakerList.clear();
                        me->GetCreatureListWithEntryInGrid(l_BreakerList, eCreatures::BreakerOfFrost, 30.0f);

                        for (Creature* l_Breaker : l_BreakerList)
                        {
                            if (l_Breaker->isAlive())
                                m_RitualistGuids.insert(l_Breaker->GetGUID());
                        }

                        m_Init = true;
                    }

                    std::list<Creature*> l_RuneList;
                    me->GetCreatureListWithEntryInGrid(l_RuneList, eCreatures::RuneOfNullification, 50.0f);

                    for (Creature* l_Rune : l_RuneList)
                    {
                        if (l_Rune->GetPositionZ() > me->GetPositionZ() + 10.0f)
                            m_FlyingRune = l_Rune->GetGUID();
                        else
                            m_FloorRune = l_Rune->GetGUID();
                    }
                });

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (me->GetReactState() == ReactStates::REACT_AGGRESSIVE)
                        return;

                    if (Creature* l_Flying = Creature::GetCreature(*me, m_FlyingRune))
                    {
                        l_Flying->CastSpell(me, eSpells::RuneChargingPermanent, true);

                        l_Flying->SetReactState(ReactStates::REACT_PASSIVE);
                        l_Flying->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                    }
                });
            }

            void JustReachedHome() override
            {
                if (m_RitualistGuids.empty() && m_Init)
                {
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                    me->SetAIAnimKitId(0);
                }
                else
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                    me->SetAIAnimKitId(eAnimKit::AnimWaiting);
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
                m_RitualistGuids.erase(p_Guid);

                if (m_RitualistGuids.empty())
                {
                    if (Creature* l_Flying = Creature::GetCreature(*me, m_FlyingRune))
                        l_Flying->RemoveAura(eSpells::RuneChargingPermanent);

                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                    me->SetAIAnimKitId(0);

                    Talk(eTalks::Intro);
                }
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                /*switch (p_Action)
                {
                    default:
                        break;
                }*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                Talk(eTalks::Aggro);

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);
                }

                if (Creature* l_Flying = Creature::GetCreature(*me, m_FlyingRune))
                    l_Flying->CastSpell(me, eSpells::NullificationRuneEmpowered, true);

                me->CastSpell(me, eSpells::NullificationBarrierPower, true);
                me->CastSpell(me, eSpells::NullificationBarrierAbsorb, true);
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

                Talk(eTalks::Death);
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eHighmaulDatas::BossKoragh, EncounterState::FAIL);

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

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

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
            return new boss_koraghAI(p_Creature);
        }
};

/// Breaker Ritualist <Breaker of Fel> - 86330
class npc_highmaul_breaker_of_fel : public CreatureScript
{
    public:
        npc_highmaul_breaker_of_fel() : CreatureScript("npc_highmaul_breaker_of_fel") { }

        enum eSpells
        {
            /// Cosmetic
            FelBreakerFelChannel    = 172664,

            /// Damaging Spells
            FelBlast                = 174422,
            FelNova                 = 174403
        };

        enum eEvent
        {
            EventFelNova = 1
        };

        struct npc_highmaul_breaker_of_felAI : public ScriptedAI
        {
            npc_highmaul_breaker_of_felAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::FelBreakerFelChannel, false);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvent::EventFelNova, 12 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer)
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Boss = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Koragh)))
                {
                    if (l_Boss->IsAIEnabled)
                        l_Boss->AI()->SetGUID(me->GetGUID());
                }
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
                    case eEvent::EventFelNova:
                        me->CastSpell(me, eSpells::FelNova, false);
                        m_Events.ScheduleEvent(eEvent::EventFelNova, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoSpellAttackIfReady(eSpells::FelBlast);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_breaker_of_felAI(p_Creature);
        }
};

/// Breaker Ritualist <Breaker of Fire> - 86329
class npc_highmaul_breaker_of_fire : public CreatureScript
{
    public:
        npc_highmaul_breaker_of_fire() : CreatureScript("npc_highmaul_breaker_of_fire") { }

        enum eSpells
        {
            /// Cosmetic
            FelBreakerFireChannel   = 172455,

            /// Damaging Spells
            WildFlamesSearcher      = 173763,
            WildFlamesMissile       = 173618
        };

        enum eEvent
        {
            EventWildFlames = 1
        };

        struct npc_highmaul_breaker_of_fireAI : public ScriptedAI
        {
            npc_highmaul_breaker_of_fireAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::FelBreakerFireChannel, false);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvent::EventWildFlames, 4 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::WildFlamesSearcher:
                        me->CastSpell(p_Target, eSpells::WildFlamesMissile, true);
                        break;
                    default:
                        break;
                }
            }

            void JustDied(Unit* p_Killer)
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Boss = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Koragh)))
                {
                    if (l_Boss->IsAIEnabled)
                        l_Boss->AI()->SetGUID(me->GetGUID());
                }
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
                    case eEvent::EventWildFlames:
                        me->CastSpell(me, eSpells::WildFlamesSearcher, false);
                        m_Events.ScheduleEvent(eEvent::EventWildFlames, 11 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_breaker_of_fireAI(p_Creature);
        }
};

/// Wild Flames - 86875
class npc_highmaul_wild_flames : public CreatureScript
{
    public:
        npc_highmaul_wild_flames() : CreatureScript("npc_highmaul_wild_flames") { }

        enum eSpells
        {
            WildFlamesAreaTrigger   = 173616,
            WildFlamesSearcher      = 173799,
            WildFlamesMissile       = 173823
        };

        enum eEvent
        {
        };

        struct npc_highmaul_wild_flamesAI : public ScriptedAI
        {
            npc_highmaul_wild_flamesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_wild_flamesAI(p_Creature);
        }
};

/// Breaker Ritualist <Breaker of Frost> - 86326
class npc_highmaul_breaker_of_frost : public CreatureScript
{
    public:
        npc_highmaul_breaker_of_frost() : CreatureScript("npc_highmaul_breaker_of_frost") { }

        enum eSpells
        {
            /// Cosmetic
            FelBreakerFrostChannel  = 172448,

            /// Damaging Spells
            FrozenCore              = 174404
        };

        enum eEvent
        {
            EventFrozenCore = 1
        };

        struct npc_highmaul_breaker_of_frostAI : public ScriptedAI
        {
            npc_highmaul_breaker_of_frostAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::FelBreakerFrostChannel, false);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvent::EventFrozenCore, 4 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer)
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Boss = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Koragh)))
                {
                    if (l_Boss->IsAIEnabled)
                        l_Boss->AI()->SetGUID(me->GetGUID());
                }
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
                    case eEvent::EventFrozenCore:
                        me->CastSpell(me, eSpells::FrozenCore, false);
                        m_Events.ScheduleEvent(eEvent::EventFrozenCore, 13 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_breaker_of_frostAI(p_Creature);
        }
};

/// Chain - 233127
class go_highmaul_chain : public GameObjectScript
{
    public:
        go_highmaul_chain() : GameObjectScript("go_highmaul_chain") { }

        struct go_highmaul_chainAI : public GameObjectAI
        {
            go_highmaul_chainAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject) { }

            enum eAnimID
            {
                AnimKit = 947
            };

            void Reset() override
            {
                go->SetAIAnimKitId(eAnimID::AnimKit);
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_highmaul_chainAI(p_GameObject);
        }
};

/// Frozen Core - 174404
class spell_highmaul_frozen_core : public SpellScriptLoader
{
    public:
        spell_highmaul_frozen_core() : SpellScriptLoader("spell_highmaul_frozen_core") { }

        class spell_highmaul_frozen_core_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_frozen_core_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                if (Unit* l_Caster = GetCaster())
                    p_Targets.remove_if(JadeCore::UnitAuraCheck(true, GetSpellInfo()->Id, l_Caster->GetGUID()));
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_frozen_core_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_frozen_core_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_frozen_core_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_frozen_core_SpellScript();
        }

        class spell_highmaul_frozen_core_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_frozen_core_AuraScript);

            enum eSpell
            {
                FrozenCore = 174405
            };

            uint32 m_DamageTimer;

            bool Load()
            {
                m_DamageTimer = 200;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_DamageTimer)
                {
                    if (m_DamageTimer <= p_Diff)
                    {
                        if (Unit* l_Target = GetTarget())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 12.0f;

                            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_Target, l_Target, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Target, l_TargetList, l_Check);
                            l_Target->VisitNearbyObject(l_Radius, l_Searcher);

                            for (Unit* l_Iter : l_TargetList)
                            {
                                if (l_Iter->GetDistance(l_Target) <= 8.0f)
                                    l_Target->CastSpell(l_Iter, eSpell::FrozenCore, true);
                                else
                                    l_Iter->RemoveAura(eSpell::FrozenCore);
                            }
                        }

                        m_DamageTimer = 200;
                    }
                    else
                        m_DamageTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_highmaul_frozen_core_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_frozen_core_AuraScript();
        }
};

void AddSC_boss_koragh()
{
    /// Boss
    new boss_koragh();

    /// NPCs
    new npc_highmaul_breaker_of_fel();
    new npc_highmaul_breaker_of_fire();
    new npc_highmaul_wild_flames();
    new npc_highmaul_breaker_of_frost();

    /// GameObjects
    new go_highmaul_chain();

    /// Spells
    new spell_highmaul_frozen_core();

    /// AreaTriggers
}