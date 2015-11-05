////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eAction
{
    ActionLeaveTurret = 1
};

enum eCreature
{
    CreatureBackdraft = 432551
};

enum eTalks
{
    TalkKoramar12       = 50, ///< How dare you marching into my docks, and so bravery set foot onto my ship.. nothing will stop the Iron Horde least of all you.. enjoy your death weaklings. (46911)
    TalkKoramar13       = 51, ///< Zoggosh.. ready the ready the rocks! This.. Ends... Now! (46912)
    TalkZoggosh03       = 11, ///< Yes sir.. (44049)
    TalkKoramarVengence = 60
};

Position const g_BackdraftNpcPos = { 6859.93f, -989.91f, 23.054f, 3.000260f };

class basicevent_pre_boss : public BasicEvent
{
public:

    explicit basicevent_pre_boss(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                {
                    if (l_Koramar->IsAIEnabled)
                    {
                        switch (m_Modifier)
                        {
                        case 0:
                            l_Koramar->AI()->Talk(eTalks::TalkKoramar12);
                            l_Koramar->m_Events.AddEvent(new basicevent_pre_boss(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                            break;
                        case 1:
                            l_Koramar->AI()->Talk(eTalks::TalkKoramar13);
                            l_Koramar->m_Events.AddEvent(new basicevent_pre_boss(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                            break;
                        case 2:
                            l_Koramar->AI()->Talk(eTalks::TalkZoggosh03);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }

        return true;
    }

private:

    Unit* m_Obj;
    int m_Modifier;
};

/// Skulloc <Son of Gruul> - 83612
class boss_skulloc : public CreatureScript
{
    public:

        boss_skulloc() : CreatureScript("boss_skulloc") { }

        struct boss_skullocAI : public BossAI
        {
            boss_skullocAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataSkulloc), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = me->GetInstanceScript();
                m_BombardmentPosition = 0;
                m_First = false;
                m_BoolPreEvent = false;
            }

            enum eSkullocSpells
            {
                SpellCannonBarrageMissileOriginal = 168539,
                SpellGronnSmash                   = 168227,
                SpellBackdraft                    = 169132
            };

            enum eSkullocEvents
            {
                EventBackdraft = 1,
                EventCannonBarrage,
                EventCannonBarragePre,
                EventCannonBarrageCancel
            };

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;
            uint8 m_BombardmentPosition;
            bool m_BoolPreEvent;
            bool m_First;

            void Reset() override
            {
                _Reset();
                events.Reset();
                ASSERT(m_Vehicle);
                if (!m_First)
                {
                    m_First = true;
                    me->SetUnitMovementFlags(MovementFlags::MOVEMENTFLAG_ROOT);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->Kill(me);
                    me->Respawn();
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 20.0f) && !m_BoolPreEvent)
                {
                    m_BoolPreEvent = true;
                    me->m_Events.AddEvent(new basicevent_pre_boss(me, 0), me->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                }
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                _EnterCombat();
                events.ScheduleEvent(eSkullocEvents::EventCannonBarragePre, 40 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eSkullocEvents::EventBackdraft, 15 * TimeConstants::IN_MILLISECONDS);
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Turret = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataTurret)))
                    {
                        if (l_Turret->IsAIEnabled)
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 60.0f, true))
                                l_Turret->AI()->AttackStart(l_Target);
                        }
                    }
                }
            }

            void JustReachedHome() override
            {
                _JustReachedHome();
                summons.DespawnAll();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eSkullocEvents::EventCannonBarragePre:
                        {
                            me->CastSpell(me, eSkullocSpells::SpellGronnSmash);
                            events.ScheduleEvent(eSkullocEvents::EventCannonBarrage, 6 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eSkullocEvents::EventCannonBarrageCancel, 30 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eSkullocEvents::EventCannonBarragePre, 50 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eSkullocEvents::EventCannonBarrage:
                        {
                            me->AddAura(eIronDocksSpell::SpellCannonBarrageAura, me);
                            events.ScheduleEvent(eSkullocEvents::EventCannonBarrage, 12 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eSkullocEvents::EventCannonBarrageCancel:
                        {
                            events.CancelEvent(eSkullocEvents::EventCannonBarrage);
                            break;
                        }
                    case eSkullocEvents::EventBackdraft:
                        {
                            if (Creature* l_BackdraftTrigger = me->FindNearestCreature(eCreature::CreatureBackdraft, 300.0f, true))
                                l_BackdraftTrigger->CastSpell(l_BackdraftTrigger, eSkullocSpells::SpellBackdraft);
                            events.ScheduleEvent(eSkullocEvents::EventBackdraft, 30 * TimeConstants::IN_MILLISECONDS);
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
            return new boss_skullocAI(p_Creature);
        }
};

/// Koramar <Captain of Blackhand's Might> - 83613
class iron_docks_skulloc_mob_koramar : public CreatureScript
{
    public:

        iron_docks_skulloc_mob_koramar() : CreatureScript("iron_docks_skulloc_mob_koramar") { }

        struct iron_docks_skulloc_mob_koramarAI : public ScriptedAI
        {
            iron_docks_skulloc_mob_koramarAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            enum eKoramarSpells
            {
                /// Koramar
                SpellBerserkerLeapLightVisual = 168399,
                SpellBerserkerLeapJump        = 168964,
                SpellBerserkerLeapDummy       = 168965,
                SpellBladestorm               = 168997,
                SpellShatteringBlades         = 168939
            };

            enum eKoramarEvents
            {
                /// Karomar
                EventBerserkerLeap = 1,
                EventBladestorm,
                EventShatteringBlades,
                EventBladestormMoveJitterCancel,
                EventBladestormMoveJitter
            };

            InstanceScript* m_Instance;

            void Reset() override
            {
                events.Reset();
                me->setFaction(HostileFaction);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eKoramarEvents::EventBladestorm, 14 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eKoramarEvents::EventShatteringBlades, 40 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eKoramarEvents::EventBerserkerLeap, 24 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Turret = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataTurret)))
                    {
                        if (l_Turret->IsAIEnabled)
                            l_Turret->GetAI()->DoAction(eAction::ActionLeaveTurret);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eKoramarEvents::EventBladestorm:
                            me->CastSpell(me, eKoramarSpells::SpellBladestorm);
                            events.ScheduleEvent(eKoramarEvents::EventBladestorm, 14 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eKoramarEvents::EventBladestormMoveJitter, 1 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eKoramarEvents::EventBladestormMoveJitterCancel, 6 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eKoramarEvents::EventBladestormMoveJitter:
                            me->GetMotionMaster()->MoveRandom(10.0f);
                            events.ScheduleEvent(eKoramarEvents::EventBladestormMoveJitter, 1 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eKoramarEvents::EventBladestormMoveJitterCancel:
                        events.CancelEvent(eKoramarEvents::EventBladestormMoveJitter);
                            break;
                    case eKoramarEvents::EventShatteringBlades:
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eKoramarSpells::SpellShatteringBlades);
                            events.ScheduleEvent(eKoramarEvents::EventShatteringBlades, 40 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eKoramarEvents::EventBerserkerLeap:
                            me->CastSpell(me, eKoramarSpells::SpellBerserkerLeapDummy);
                            events.ScheduleEvent(eKoramarEvents::EventBerserkerLeap, 24 * TimeConstants::IN_MILLISECONDS);
                            break;
                        default:
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_skulloc_mob_koramarAI(p_Creature);
        }
};

/// Blackhand's Might Turret - 84215
class iron_docks_skulloc_mob_turret : public CreatureScript
{
    public:

        iron_docks_skulloc_mob_turret() : CreatureScript("iron_docks_skulloc_mob_turret") { }

        struct iron_docks_skulloc_mob_turretAI : public ScriptedAI
        {
            iron_docks_skulloc_mob_turretAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = me->GetInstanceScript();
                m_First = false;
            }

            enum eTurretSpells
            {
                SpellRapidFire = 156628             
            };

            enum eTurretEvents
            {
                EventRapidFire = 1,
                EventRapidFireChangeTarget
            };

            Vehicle* m_Vehicle;
            uint64 m_TargetGuid;
            bool m_First;
            InstanceScript* m_Instance;

            void Reset() override
            {
                events.Reset();
                ASSERT(m_Vehicle);
                m_TargetGuid = 0;
                if (!m_First)
                {
                    m_First = true;
                    me->Kill(me);
                    me->Respawn();
                    me->SetUnitMovementFlags(MovementFlags::MOVEMENTFLAG_ROOT);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    me->setFaction(HostileFaction);      
                }
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                events.ScheduleEvent(eTurretEvents::EventRapidFireChangeTarget, 2 * TimeConstants::IN_MILLISECONDS);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionLeaveTurret:
                    {                                   
                        if (m_Instance != nullptr)
                        {
                            if (Creature* l_Zoggosh = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                            {
                                if (l_Zoggosh->IsAIEnabled)
                                    l_Zoggosh->AI()->Talk(eTalks::TalkKoramarVengence);
                            }
                        }
                  
                        me->CombatStop();
                        me->setFaction(FriendlyFaction);
                        me->DespawnOrUnsummon();
                        me->getHostileRefManager().clearReferences();
                        events.CancelEvent(eTurretEvents::EventRapidFire);
                        if (m_Vehicle != nullptr)
                        m_Vehicle->RemoveAllPassengers();
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eTurretEvents::EventRapidFireChangeTarget:
                        {
                            events.CancelEvent(eTurretEvents::EventRapidFire);

                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 60.0f, true))
                                m_TargetGuid = l_Target->GetGUID();
                            events.ScheduleEvent(eTurretEvents::EventRapidFireChangeTarget, 10 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTurretEvents::EventRapidFire, 2 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eTurretEvents::EventRapidFire:
                        {
                            if (Unit* l_Target = Unit::GetUnit(*me, m_TargetGuid))
                            {
                                if (l_Target->IsInWorld() && l_Target->IsWithinDistInMap(me, 200.0f, true))
                                    me->CastSpell(l_Target, eTurretSpells::SpellRapidFire);
                            }
                            events.ScheduleEvent(eTurretEvents::EventRapidFire, 2 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        default:
                            break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_skulloc_mob_turretAI(p_Creature);
        }
};

/// Backdraft - 169132
class iron_docks_skulloc_spell_back_draft : public SpellScriptLoader
{
    public:

        iron_docks_skulloc_spell_back_draft() : SpellScriptLoader("iron_docks_skulloc_spell_back_draft") { }

        class iron_docks_skulloc_spell_back_draft_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_skulloc_spell_back_draft_SpellScript);

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target->IsWithinLOSInMap(l_Caster))
                            SetHitDamage(0);
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(iron_docks_skulloc_spell_back_draft_SpellScript::HandleDamage, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_skulloc_spell_back_draft_SpellScript();
        }
};

/// Gronn Smash - 168227
class iron_docks_skulloc_spell_gronn_smash : public SpellScriptLoader
{
    public:

        iron_docks_skulloc_spell_gronn_smash() : SpellScriptLoader("iron_docks_skulloc_spell_gronn_smash") { }

        class iron_docks_skulloc_spell_gronn_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_skulloc_spell_gronn_smash_SpellScript);

            void HandleScriptEffect(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    l_Caster->GetPlayerListInGrid(l_PlayerList, 80.0f);
                    if (l_PlayerList.empty())
                        return;

                    for (auto l_Iter : l_PlayerList)
                    {
                        l_Iter->GetMotionMaster()->MoveJump(g_BackdraftNpcPos.GetPositionX(), g_BackdraftNpcPos.GetPositionY(), g_BackdraftNpcPos.GetPositionZ(), 40.0f, 20.0f, 10.0f);
                    }
                }
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(iron_docks_skulloc_spell_gronn_smash_SpellScript::HandleScriptEffect, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_skulloc_spell_gronn_smash_SpellScript();
        }
};

/// Cannon Barrage - 168537
class iron_docks_skulloc_spell_cannon_barrage : public SpellScriptLoader
{
    public:
        iron_docks_skulloc_spell_cannon_barrage() : SpellScriptLoader("iron_docks_skulloc_spell_cannon_barrage") { }

        class iron_docks_skulloc_spell_cannon_barrage_SpellScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_skulloc_spell_cannon_barrage_SpellScript);

            enum eCannonBarrageSpells
            {
                SpellCannonBarrageMissileOriginal = 168539
            };

            std::list<uint64> m_TargetGuids;
            bool Load() override
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Creature*> l_CreatureList;
                    m_TargetGuids.clear();
                    if (l_Caster->GetEntry() == eIronDocksCreatures::BossSkulloc)
                        l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::TriggerCannonBarrageInBossFight, 200.0f);
                    else
                    {
                        if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                        {
                            /// Position is changing incrementally, instead of randomally
                            if (Creature * l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                            {
                                if (l_Skulloc->IsAIEnabled)
                                {
                                    if (boss_skulloc::boss_skullocAI* l_LinkAI = CAST_AI(boss_skulloc::boss_skullocAI, l_Skulloc->GetAI()))
                                    {
                                        switch (l_LinkAI->m_BombardmentPosition)
                                        {
                                            case 0:
                                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::TriggerCannonBarrageTarget, 1500.0f);
                                                break;
                                            case 1:
                                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::TriggerCannonBarrageTarget2, 1500.0f);
                                                break;
                                            case 2:
                                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::TriggerCannonBarrageTarget3, 1500.0f);
                                                break;
                                            case 3:
                                                l_LinkAI->m_BombardmentPosition = 0;
                                                break;
                                            default:
                                                break;
                                        }

                                        l_LinkAI->m_BombardmentPosition++;
                                    }
                                }
                            }
                        }                
                    }

                    for (auto l_Iter : l_CreatureList)
                        m_TargetGuids.push_back(l_Iter->GetGUID());
                }
                return true;
            }

            void PeriodicTick(constAuraEffectPtr /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (m_TargetGuids.empty())
                        return;

                    for (auto l_Iter : m_TargetGuids)
                    {
                        if (Creature* l_Target = Creature::GetCreature(*l_Caster, l_Iter))
                            l_Caster->CastSpell(l_Target, eCannonBarrageSpells::SpellCannonBarrageMissileOriginal);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_skulloc_spell_cannon_barrage_SpellScript::PeriodicTick, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_skulloc_spell_cannon_barrage_SpellScript();
        }
};

/// Berserker Leap - 168965
class iron_docks_skulloc_spell_berserker_jump : public SpellScriptLoader
{
    public:

        iron_docks_skulloc_spell_berserker_jump() : SpellScriptLoader("iron_docks_skulloc_spell_berserker_jump") { }

        class iron_docks_skulloc_spell_berserker_jump_AuraScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_skulloc_spell_berserker_jump_AuraScript);

            void HandlePeriodic(constAuraEffectPtr /*p_AurEff*/)
            {
                PreventDefaultAction();

                // Responsible for the jump
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_skulloc_spell_berserker_jump_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_skulloc_spell_berserker_jump_AuraScript();
        }
};

/// Rapid Fire - 156637
class iron_docks_skulloc_spell_rapid_fire_damage : public SpellScriptLoader
{
    public:

        iron_docks_skulloc_spell_rapid_fire_damage() : SpellScriptLoader("iron_docks_skulloc_spell_rapid_fire_damage") { }

        class iron_docks_skulloc_spell_rapid_fire_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_skulloc_spell_rapid_fire_damage_SpellScript);

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                // Responsible for the damage
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(iron_docks_skulloc_spell_rapid_fire_damage_SpellScript::HandleDamage, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_skulloc_spell_rapid_fire_damage_SpellScript();
        }
};

void AddSC_boss_skulloc()
{
    /// Bosses
    new boss_skulloc();
    new iron_docks_skulloc_mob_koramar();
    new iron_docks_skulloc_mob_turret();
    /// Spells
    new iron_docks_skulloc_spell_back_draft();
    new iron_docks_skulloc_spell_gronn_smash();
    new iron_docks_skulloc_spell_cannon_barrage();
    new iron_docks_skulloc_spell_berserker_jump();
    new iron_docks_skulloc_spell_rapid_fire_damage();
}