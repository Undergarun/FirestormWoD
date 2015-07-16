////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eSpells
{
    /// Skulloc
    /// Cannon Barrage
    SpellCannonBarrageMissileOriginal   = 168539,

    SpellGronnSmash                     = 168227,

    SpellBackdraft                      = 169132,

    /// Koramar
    SpellBerserkerLeapLightVisual       = 168399,
    SpellBerserkerLeapJump              = 168964,
    SpellBerserkerLeapDummy             = 168965,
    SpellBladestorm                     = 168997,
    SpellShatteringBlades               = 168939,

    /// ZUGGOSH
    SpellRapidFire                      = 156628
};

enum eEvents
{
    /// Skulloc
    EventBackdraft                  = 955,
    EventCannonBarrage              = 51,
    EventCannonBarragePre           = 53,
    EventCannonBarrageCancel        = 912,

    /// Karomar
    EventBerserkerLeap              = 53,
    EventBladestorm                 = 54,
    EventShatteringBlades           = 55,
    EventBladestormMoveJitterCancel = 57,
    EventBladestormMoveJitter       = 58,

    /// Zuggosh
    EventRapidFire                  = 56,
    EventRapidFireChangeTarget      = 85
};

enum eAction
{
    ActionLeaveTurret = 70
};

enum eCreature
{
    TriggerBackdraft = 432551,
};

enum eTalks
{
    TalkKoramar12       = 50, ///< How dare you marching into my docks, and so bravery set foot onto my ship.. nothing will stop the Iron Horde least of all you.. enjoy your death weaklings. (46911)
    TalkKoramar13       = 51, ///< Zoggosh.. ready the ready the rocks! This.. Ends... Now! (46912)
    TalkZoggosh03       = 11, ///< Yes sir.. (44049)
    TalkKoramarVengence = 60
};

Position const g_BackdraftNpcPos = { 6859.93f, -989.91f, 23.054f, 3.000260f };

class pre_last_boss_event : public BasicEvent
{
    public:
        explicit pre_last_boss_event(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (!l_Koramar->IsAIEnabled)
                            return true;

                        switch (m_Modifier)
                        {
                            case 0:
                                l_Koramar->AI()->Talk(eTalks::TalkKoramar12);
                                l_Koramar->m_Events.AddEvent(new pre_last_boss_event(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                break;
                            case 1:
                                l_Koramar->AI()->Talk(eTalks::TalkKoramar13);
                                l_Koramar->m_Events.AddEvent(new pre_last_boss_event(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
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

                me->SetUnitMovementFlags(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->Kill(me);
                me->Respawn();

                m_BoolPreEvent = false;
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;
            bool m_BoolPreEvent;

            void Reset() override
            {
                _Reset();

                DespawnCreaturesInArea(eCreature::TriggerBackdraft, me);

                ASSERT(m_Vehicle);

                me->SummonCreature(eCreature::TriggerBackdraft, g_BackdraftNpcPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }

            void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
            {
                std::list<Creature*> l_Creatures;
                GetCreatureListWithEntryInGrid(l_Creatures, p_Object, p_Entry, 300.0f);

                for (Creature* l_Iter : l_Creatures)
                    l_Iter->DespawnOrUnsummon();
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 20.0f) && !m_BoolPreEvent)
                {
                    m_BoolPreEvent = true;
                    me->m_Events.AddEvent(new pre_last_boss_event(me, 0), me->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                }
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Turret = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataTurret)))
                    {
                        if (l_Turret->IsAIEnabled)
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 60.0f, true))
                                l_Turret->AI()->EnterCombat(l_Target);
                        }
                    }
                }

                events.ScheduleEvent(eEvents::EventCannonBarragePre, 40 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventBackdraft, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void JustReachedHome() override
            {
                me->Kill(me);
                me->Respawn();

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Zoggosh = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                        l_Zoggosh->DespawnOrUnsummon(500);

                    if (Creature* l_Turret = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataTurret)))
                    {
                        l_Turret->Kill(l_Turret);
                        l_Turret->Respawn();
                    }

                    if (Creature* l_Koramar = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        l_Koramar->AI()->Reset();
                        l_Koramar->Respawn();
                    }
                }
            }

            void JustDied(Unit* /*killer*/) override
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
                    case eEvents::EventCannonBarragePre:
                    {
                        me->CastSpell(me, eSpells::SpellGronnSmash);
                        events.ScheduleEvent(eEvents::EventCannonBarrage, 6 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEvents::EventCannonBarrageCancel, 30 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEvents::EventCannonBarragePre, 50 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventCannonBarrage:
                    {
                        me->AddAura(eIronDocksSpell::SpellCannonBarrageAura, me);
                        events.ScheduleEvent(eEvents::EventCannonBarrage, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventCannonBarrageCancel:
                    {
                        events.CancelEvent(eEvents::EventCannonBarrage);
                        break;
                    }
                    case eEvents::EventBackdraft:
                    {
                        if (Creature* l_BackdraftTrigger = me->FindNearestCreature(eCreature::TriggerBackdraft, 300.0f, true))
                        {
                            l_BackdraftTrigger->SetUnitMovementFlags(MovementFlags::MOVEMENTFLAG_ROOT);
                            l_BackdraftTrigger->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                            l_BackdraftTrigger->SetLevel(101);
                            l_BackdraftTrigger->setFaction(14);

                            l_BackdraftTrigger->CastSpell(l_BackdraftTrigger, eSpells::SpellBackdraft);
                            events.ScheduleEvent(eEvents::EventBackdraft, 30 * TimeConstants::IN_MILLISECONDS);
                        }
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
class boss_koramar : public CreatureScript
{
    public:
        boss_koramar() : CreatureScript("boss_koramar") { }

        struct boss_koramarAI : public BossAI
        {
            boss_koramarAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataKoramar)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                _Reset();

                me->setFaction(16);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                events.ScheduleEvent(eEvents::EventBladestorm, 14 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventShatteringBlades, 40 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventBerserkerLeap, 24 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Turret = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataTurret)))
                {
                    if (l_Turret->IsAIEnabled)
                        l_Turret->GetAI()->DoAction(eAction::ActionLeaveTurret);
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
                    case eEvents::EventBladestorm:
                        me->CastSpell(me, eSpells::SpellBladestorm);
                        events.ScheduleEvent(eEvents::EventBladestorm, 14 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEvents::EventBladestormMoveJitter, 1 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEvents::EventBladestormMoveJitterCancel, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventBladestormMoveJitter:
                        me->GetMotionMaster()->MoveRandom(10.0f);
                        events.ScheduleEvent(eEvents::EventBladestormMoveJitter, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventBladestormMoveJitterCancel:
                        events.CancelEvent(eEvents::EventBladestormMoveJitter);
                        break;
                    case eEvents::EventShatteringBlades:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellShatteringBlades);
                        events.ScheduleEvent(eEvents::EventShatteringBlades, 40 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventBerserkerLeap:
                        me->CastSpell(me, eSpells::SpellBerserkerLeapDummy);
                        events.ScheduleEvent(eEvents::EventBerserkerLeap, 24 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_koramarAI(p_Creature);
        }
};

/// Blackhand's Might Turret - 84215
class boss_turret : public CreatureScript
{
    public:
        boss_turret() : CreatureScript("boss_turret") { }

        struct boss_zoggoshAI : public BossAI
        {
            boss_zoggoshAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataTurret), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = me->GetInstanceScript();
                me->SetUnitMovementFlags(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                me->setFaction(16);

                me->Kill(me);
                me->Respawn();
            }

            Vehicle* m_Vehicle;
            uint64 m_TargetGuid;
            InstanceScript* m_Instance;

            void Reset() override
            {
                _Reset();

                m_TargetGuid = 0;

                ASSERT(m_Vehicle);
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                _EnterCombat();

                events.ScheduleEvent(eEvents::EventRapidFireChangeTarget, 2 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionLeaveTurret:
                    {
                        m_Vehicle->RemoveAllPassengers();

                        if (m_Instance != nullptr)
                        {
                            if (Creature* l_Zoggosh = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                            {
                                if (l_Zoggosh->IsAIEnabled)
                                    l_Zoggosh->AI()->Talk(eTalks::TalkKoramarVengence);
                            }
                        }

                        me->DespawnOrUnsummon(500);
                        me->getHostileRefManager().clearReferences();
                        me->CombatStop();
                        me->setFaction(35);
                        events.CancelEvent(eEvents::EventRapidFire);
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
                    case eEvents::EventRapidFireChangeTarget:
                    {
                        events.CancelEvent(eEvents::EventRapidFire);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            m_TargetGuid = l_Target->GetGUID();

                        events.ScheduleEvent(eEvents::EventRapidFireChangeTarget, 10 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eEvents::EventRapidFire, 1500);
                        break;
                    }
                    case eEvents::EventRapidFire:
                    {
                        if (Unit* l_Target = Unit::GetUnit(*me, m_TargetGuid))
                        {
                            if (l_Target->IsInWorld() && l_Target->IsWithinDistInMap(me, 200.0f, true))
                                me->CastSpell(l_Target, eSpells::SpellRapidFire);
                        }

                        events.ScheduleEvent(eEvents::EventRapidFire, 1500);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_zoggoshAI(p_Creature);
        }
};

/// Backdraft - 169132
class spell_back_draft : public SpellScriptLoader
{
    public:
        spell_back_draft() : SpellScriptLoader("spell_back_draft") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            bool Load() override
            {
                SpellInfo* l_SpellInfo = const_cast<SpellInfo*>(GetSpellInfo());
                l_SpellInfo->Effects[0].TargetB = Targets::TARGET_DEST_DEST;
                return true;
            }

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
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Gronn Smash - 168227
class spell_gronn_smash : public SpellScriptLoader
{
    public:
        spell_gronn_smash() : SpellScriptLoader("spell_gronn_smash") { }

        class spell_gronn_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gronn_smash_SpellScript);

            void HandleScriptEffect(SpellEffIndex p_EffIndex)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                std::list<Player*> l_PlayerList;
                l_Caster->GetPlayerListInGrid(l_PlayerList, 80.0f);

                for (Player* l_Iter : l_PlayerList)
                    l_Iter->GetMotionMaster()->MoveJump(g_BackdraftNpcPos.GetPositionX(), g_BackdraftNpcPos.GetPositionY(), g_BackdraftNpcPos.GetPositionZ(), 40.0f, 20.0f, 10.0f);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(spell_gronn_smash_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gronn_smash_SpellScript();
        }
};

/// Cannon Barrage - 168537
class spell_barrage_targets : public SpellScriptLoader
{
    public:
        spell_barrage_targets() : SpellScriptLoader("spell_barrage_targets") { }

        class iron_docks_spells : public AuraScript
        {
            PrepareAuraScript(iron_docks_spells);

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
                        switch (urand(0, 2))
                        {
                            case 0:
                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::TriggerCannonBarrageTarget, 1500.0F);
                                break;
                            case 1:
                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::TriggerCannonBarrageTarget2, 1500.0F);
                                break;
                            case 2:
                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::TriggerCannonBarrageTarget3, 1500.0F);
                                break;
                            default:
                                break;
                        }
                    }

                    for (Creature* l_Iter : l_CreatureList)
                        m_TargetGuids.push_back(l_Iter->GetGUID());
                }

                return true;
            }

            void PeriodicTick(constAuraEffectPtr /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    for (uint64 l_Iter : m_TargetGuids)
                    {
                        if (Creature* l_Target = Creature::GetCreature(*l_Caster, l_Iter))
                            l_Caster->CastSpell(l_Target, eSpells::SpellCannonBarrageMissileOriginal);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_spells::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Berserker Leap - 168965
class koramar_berserker_jump : public SpellScriptLoader
{
    public:
        koramar_berserker_jump() : SpellScriptLoader("koramar_berserker_jump") { }

        class iron_docks_auras : public AuraScript
        {
            PrepareAuraScript(iron_docks_auras);

            bool Load() override
            {
                SpellInfo* l_SpellInfo = const_cast<SpellInfo*>(GetSpellInfo());
                l_SpellInfo->Effects[0].Amplitude = 500;
                return true;
            }

            void HandlePeriodic(constAuraEffectPtr /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (Player* l_Player = GetCaster()->FindNearestPlayer(50.0f, true))
                    {
                        l_Caster->CastSpell(l_Player, eSpells::SpellBerserkerLeapJump);
                        l_Caster->CastSpell(l_Player, eSpells::SpellBerserkerLeapLightVisual);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_auras::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_auras();
        }
};

/// Rapid Fire - 156637
class spell_rapid_fire_damage : public SpellScriptLoader
{
    public:
        spell_rapid_fire_damage() : SpellScriptLoader("spell_rapid_fire_damage") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetEntry() != eIronDocksCreatures::BossTurret)
                        return;

                    SetHitDamage(urand(2500, 3 * TimeConstants::IN_MILLISECONDS));
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

void AddSC_boss_skulloc()
{
    /// Bosses
    new boss_skulloc();
    new boss_koramar();
    new boss_turret();

    /// Spells
    new spell_back_draft();
    new spell_gronn_smash();
    new spell_barrage_targets();
    new koramar_berserker_jump();
    new spell_rapid_fire_damage();
}