////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadowmoon_burial_grounds.hpp"

enum eBoneMawSpells
{
    // Carrion Worm: 88769
    SpellNecroticPitchTriggerMissile    = 153689,
    SpellNecroticPitchAreatrigger       = 153690,
    SpellNecroticPitchDummy             = 153691,
    SpellNecroticPitchDebuff            = 153692,
    SpellCorpseBreathPeriodic           = 165578,
    SpellCorpseBreathDamage             = 165579,
    SpellFetidSpitDamage                = 153681,

    SpellBodySlam                       = 154175,
    SpellInhaleVisual                   = 153721,
    SpellInhalePeriodicChannel          = 153804,
    SpellInhaleDamage                   = 153908,
    SpellInhaleAreaTrigger              = 153961,

    SpellVisualSubmerge                 = 177694
};

enum eBoneMawEvents
{
    // Carrion Worm: 88769
    EventNecroticPitch = 1,
    EventCorpseBreath,
    EventFetidSpit,
    EventBodySlam,
    EventInhale,
    EventCarrionWorm,
    EventCancelBodySlamFlags,
    EventCancelSubmerge
};

enum eBoneMawCreatures
{
    CreatureCarrionWorm    = 88769,
    CreatureInhaleTrigger  = 76250
};

enum eBoneMawGameObjects
{
    GameObjectBonemawDoors = 233988,
};

Position l_PositionBoneMaw = { 1851.256f, -558.721f, 199.027f, 1.991077f };
Position l_PositionAdds[2] =
{
    { 1801.369f, -521.248f, 196.795f, 0.030723f },
    { 1860.075f, -497.532f, 196.796f, 4.054334f },
};

/// Bonemaw - 75452
class boss_bonemaw : public CreatureScript
{
public:
    boss_bonemaw() : CreatureScript("boss_bonemaw") { }

    struct boss_bonemawAI : public BossAI
    {
        boss_bonemawAI(Creature* p_Creature) : BossAI(p_Creature, eShadowmoonBurialGroundsDatas::DataBossBonemaw)
        {
            m_Instance = me->GetInstanceScript();

            HandleDoorActivation();
        }

        InstanceScript* m_Instance;
        bool m_HasVictimOut;

        void Reset() override
        {
            events.Reset();

            m_HasVictimOut = false;

            me->SetDisableGravity(true);
            me->SetCanFly(true);

            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

            me->RemoveAllAuras();
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            HandleDoorActivation();
            summons.DespawnAll();
        }

        void HandleDoorActivation()
        {
            std::list<GameObject*> l_ListDoors;
            me->GetGameObjectListWithEntryInGrid(l_ListDoors, eBoneMawGameObjects::GameObjectBonemawDoors, 100.0f);

            if (l_ListDoors.empty())
                return;

            for (auto itr : l_ListDoors)
            {
                // Activate
                itr->SetLootState(LootState::GO_READY);
                itr->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossBonemaw, EncounterState::IN_PROGRESS);

                DoZoneInCombat();
            }

            events.ScheduleEvent(eBoneMawEvents::EventNecroticPitch, 50 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventBodySlam, 30 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventCorpseBreath, 20 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventInhale, 60 * TimeConstants::IN_MILLISECONDS);

            if (me->GetMap()->IsHeroic())
            events.ScheduleEvent(eBoneMawEvents::EventCarrionWorm, 70 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            summons.DespawnAll();

            HandleDoorActivation();
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            // Fetid Spit mechanism - hardcoded.
            if (Unit* l_Target = me->getVictim())
            {
                if (me->IsWithinMeleeRange(l_Target))
                {
                    if (m_HasVictimOut)
                    {
                        m_HasVictimOut = false;

                        events.CancelEvent(eBoneMawEvents::EventFetidSpit);
                        events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 12 * TimeConstants::IN_MILLISECONDS);
                    }
                }
                else
                {
                    m_HasVictimOut = true;

                    events.CancelEvent(eBoneMawEvents::EventFetidSpit);
                    events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 3 * TimeConstants::IN_MILLISECONDS);
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case eBoneMawEvents::EventBodySlam:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eBoneMawSpells::SpellBodySlam);

                        events.ScheduleEvent(eBoneMawEvents::EventBodySlam, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eBoneMawEvents::EventCarrionWorm:
                    {
                        me->CastSpell(me, eBoneMawSpells::SpellVisualSubmerge);

                        for (int i = 0; i < 2; i++)
                        {
                            me->SummonCreature(eBoneMawCreatures::CreatureCarrionWorm, l_PositionAdds[i], TEMPSUMMON_MANUAL_DESPAWN);
                        }

                        events.ScheduleEvent(eBoneMawEvents::EventCancelSubmerge, 3 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eBoneMawEvents::EventCancelSubmerge:
                    {
                        std::list<Creature*> l_ListCarrionWorms;
                        me->GetCreatureListWithEntryInGrid(l_ListCarrionWorms, eBoneMawCreatures::CreatureCarrionWorm, 100.0f);

                        if (l_ListCarrionWorms.empty())
                            return;

                        for (auto itr : l_ListCarrionWorms)
                        {
                            if (itr->isAlive())
                            {
                                itr->RemoveAura(eBoneMawSpells::SpellVisualSubmerge);
                            }
                        }
                        break;
                    }
                    case eBoneMawEvents::EventCorpseBreath:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eBoneMawSpells::SpellBodySlam);

                        events.ScheduleEvent(eBoneMawEvents::EventCorpseBreath, 35 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eBoneMawEvents::EventFetidSpit:
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eBoneMawSpells::SpellFetidSpitDamage);

                        events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eBoneMawEvents::EventInhale:
                    {
                        const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(153804);

                        if (!l_SpellInfo)
                            return;

                        me->SummonCreature(eBoneMawCreatures::CreatureInhaleTrigger, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, l_SpellInfo->GetDuration());
                        me->CastSpell(me, eBoneMawSpells::SpellInhalePeriodicChannel);
                        break;
                    }
                    case eBoneMawEvents::EventNecroticPitch:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eBoneMawSpells::SpellNecroticPitchTriggerMissile);

                        events.ScheduleEvent(eBoneMawEvents::EventNecroticPitch, TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

        static void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 5000.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_bonemawAI(p_Creature);
    }
};

/// Carrion Worm - 88769
class shadowmoon_burial_grounds_creature_carrion_worm : public CreatureScript
{
public:
    shadowmoon_burial_grounds_creature_carrion_worm() : CreatureScript("shadowmoon_burial_grounds_creature_carrion_worm") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            Reset();
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        bool m_HasVictimOut;

        void Reset() override
        {
            events.Reset();
            DoZoneInCombat();
            me->SetDisableGravity(true);
            me->SetCanFly(true);

            m_HasVictimOut = false;

            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS,   eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

            me->AddAura(eBoneMawSpells::SpellVisualSubmerge, me);
        }

        void EnterCombat(Unit* pAttacker) override
        {
            events.ScheduleEvent(eBoneMawEvents::EventBodySlam, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*pKiller*/) override
        {
            me->DespawnOrUnsummon(1000);
        }

        void UpdateAI(const uint32 pDiff) override
        {
            events.Update(pDiff);

            if (!UpdateVictim())
                return;

            // Fetid Spit mechanism - hardcoded.
            if (Unit* l_Target = me->getVictim())
            {
                if (me->IsWithinMeleeRange(l_Target))
                {
                    if (m_HasVictimOut)
                    {
                        m_HasVictimOut = false;

                        events.CancelEvent(eBoneMawEvents::EventFetidSpit);
                        events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 12 * TimeConstants::IN_MILLISECONDS);
                    }
                }
                else
                {
                    m_HasVictimOut = true;

                    events.CancelEvent(eBoneMawEvents::EventFetidSpit);
                    events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 3 * TimeConstants::IN_MILLISECONDS);
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eBoneMawEvents::EventBodySlam:
                if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    me->CastSpell(l_Target, eBoneMawSpells::SpellBodySlam);

                events.ScheduleEvent(eBoneMawEvents::EventBodySlam, 20 * TimeConstants::IN_MILLISECONDS);
                break;
            case eBoneMawEvents::EventFetidSpit:
                if (Unit* l_Target = me->getVictim())
                    me->CastSpell(l_Target, eBoneMawSpells::SpellFetidSpitDamage);

                events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
                break;
            }

            DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

/// Inhale - 153804 
class spell_inhale : public SpellScriptLoader
{
public:
    spell_inhale() : SpellScriptLoader("spell_inhale") { }

    class spell_inhale_SpellScript : public AuraScript
    {
        PrepareAuraScript(spell_inhale_SpellScript);

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                std::list<Player*> l_ListPlayers;
                JadeCore::AnyPlayerInObjectRangeCheck check(l_Caster, 1.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(l_Caster, l_ListPlayers, check);
                l_Caster->VisitNearbyObject(1.0f, searcher);

                if (l_ListPlayers.empty())
                    return;

                for (std::list<Player*>::const_iterator itr = l_ListPlayers.begin(); itr != l_ListPlayers.end(); ++itr)
                {
                    (*itr)->CastSpell((*itr), eBoneMawSpells::SpellInhaleDamage);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_inhale_SpellScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_inhale_SpellScript();
    }
};

/// Areatrigger - Necrotic Pitch - 153690 
class areatrigger_necrotic_pitch : public AreaTriggerEntityScript
{
public:
    areatrigger_necrotic_pitch() : AreaTriggerEntityScript("areatrigger_necrotic_pitch")
    {
    }

    int32 m_Diff = 500;
    std::list<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (m_Diff <= int32(p_Time))
        {
            std::list<Player*> l_ListPlayers;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 3.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_ListPlayers, check);
            p_AreaTrigger->VisitNearbyObject(3.0f, searcher);

            if (l_ListPlayers.empty())
                return;

            for (std::list<Player*>::const_iterator itr = l_ListPlayers.begin(); itr != l_ListPlayers.end(); ++itr)
            {
                if (!(*itr)->HasAura(eBoneMawSpells::SpellNecroticPitchDebuff))
                {
                    (*itr)->CastSpell((*itr), eBoneMawSpells::SpellNecroticPitchDebuff);
                    m_Targets.push_back((*itr)->GetGUID());

                    if ((*itr)->HasAura(eBoneMawSpells::SpellNecroticPitchDebuff))
                    {
                        AuraPtr aura = (*itr)->GetAura(eBoneMawSpells::SpellNecroticPitchDebuff);

                        if (aura)
                        {
                            aura->SetDuration(1);
                        }
                    }
                }
            }

            m_Diff = 500;
        }
        else
        {
            m_Diff -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        for (auto l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->HasAura(SpellNecroticPitchDebuff))
                l_Target->RemoveAura(SpellNecroticPitchDebuff);
        }
    }

    areatrigger_necrotic_pitch* GetAI() const
    {
        return new areatrigger_necrotic_pitch();
    }
};

/// Inhale - 76250 // Hacked, hardcoded
class shadowmoon_void_inhale_trigger : public CreatureScript
{
public:
    shadowmoon_void_inhale_trigger() : CreatureScript("shadowmoon_void_inhale_trigger") { }

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        uint32 m_Timer;
        InstanceScript* m_Instance;
        std::list<uint64> m_Targets;

        void Reset() override
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(35);
            me->SetDisplayId(11686);

            me->CastSpell(me, eBoneMawSpells::SpellInhaleVisual);

            m_Timer = 1000;
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);         

            if (m_Timer <= p_Diff)
            {
                std::list<Player*> l_ListPlayers;
                JadeCore::AnyPlayerInObjectRangeCheck check(me, 10.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, l_ListPlayers, check);
                me->VisitNearbyObject(10.0f, searcher);

                if (l_ListPlayers.empty())
                    return;

                Position l_Pos;

                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Bonemaw = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossBonemaw)))
                {
                    l_Bonemaw->GetPosition(&l_Pos);

                    for (std::list<Player*>::const_iterator itr = l_ListPlayers.begin(); itr != l_ListPlayers.end(); ++itr)
                    {
                        Player* l_Player = (*itr);

                        if (l_Player->isAlive() && !l_Player->HasMovementForce(l_Bonemaw->GetGUID()))
                            l_Player->SendApplyMovementForce(l_Bonemaw->GetGUID(), true, l_Pos, 3.0f);
                        else if (!l_Player->isAlive() && l_Player->HasMovementForce(l_Bonemaw->GetGUID()))
                            l_Player->RemoveAllMovementForces();

                        m_Targets.push_back(l_Player->GetGUID());
                    }
                }

                m_Timer = 1000;
            }
            else
            {
                m_Timer -= p_Diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Areatrigger - Inhale - 153961  -- Another Areatrigger missing, hack fixing it and hardcoding.
/*
class areatrigger_inhalse : public AreaTriggerEntityScript
{
public:
    areatrigger_inhalse() : AreaTriggerEntityScript("areatrigger_inhalse")
    {
    }

    int m_Diff = 500;
    std::list<uint64> m_Targets;
    InstanceScript* m_Instance;
    Position l_Pos;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (m_Diff <= p_Time)
        {
            std::list<Player*> l_ListPlayers;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 3.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_ListPlayers, check);
            p_AreaTrigger->VisitNearbyObject(3.0f, searcher);

            if (l_ListPlayers.empty())
                return;

            Position l_Pos;
            m_Instance = p_AreaTrigger->GetInstanceScript();

            if (m_Instance == nullptr)
                return;

            if (Creature* l_Bonemaw = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossBonemaw)))
            {
                l_Bonemaw->GetPosition(&l_Pos);
            }

            for (std::list<Player*>::const_iterator itr = l_ListPlayers.begin(); itr != l_ListPlayers.end(); ++itr)
            {
                Player* l_Player = (*itr);

                if (l_Player->isAlive() && !l_Player->hasForcedMovement)
                    l_Player->SendApplyMovementForce(true, l_Pos, 3.0f);
                else if (!l_Player->isAlive() && l_Player->hasForcedMovement)
                    l_Player->SendApplyMovementForce(false, l_Pos);

                m_Targets.push_back(l_Player->GetGUID());
            }
                 
            m_Diff = 500;
        }
        else
        {
            m_Diff -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        Position l_OldPosition;

        if (Creature* l_Bonemaw = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossBonemaw)))
        {
            l_Bonemaw->GetPosition(&l_OldPosition);
        }

        for (auto l_Guid : m_Targets)
        {
            if (Player* l_OldPlayer = sObjectAccessor->GetPlayer(*p_AreaTrigger, l_Guid))
            {
                l_OldPlayer->SendApplyMovementForce(false, l_OldPosition);
            }
        }
    }

    areatrigger_inhalse* GetAI() const
    {
        return new areatrigger_inhalse();
    }
};
*/

void AddSC_bonemaw()
{
    new boss_bonemaw();

    new spell_inhale();

    new shadowmoon_burial_grounds_creature_carrion_worm();
    new shadowmoon_void_inhale_trigger();

    new areatrigger_necrotic_pitch();
}