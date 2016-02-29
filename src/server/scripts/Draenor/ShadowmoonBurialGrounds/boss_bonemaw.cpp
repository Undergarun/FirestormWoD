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
    SpellInhaleSleep                    = 154010,
    SpellnhaleScreenEffect              = 154008,
    SpellVisualSubmerge                 = 177694
};

enum eBoneMawEvents
{
    EventNecroticPitch = 1,
    EventCorpseBreath,
    EventFetidSpit,
    EventBodySlam,
    EventInhale,
    EventCarrionWorm,
    EventCancelBodySlamFlags,
    EventCancelSubmerge,
    EventSubmerge
};

enum eBoneMawCreatures
{
    CreatureCarrionWorm          = 88769,
    CreatureInhaleTrigger        = 76250,
    CreatureNecroticPitchTrigger = 76191
};

enum eBoneMawGameObjects
{
    GameObjectBonemawDoors   = 233988,
    GameObjectBonemawDoors01 = 233989
};

enum eBoneMawActions
{
    ActionInhaleDeactivate = 1
};

Position const l_PositionBoneMaw = { 1851.256f, -558.721f, 199.027f, 1.991077f };

Position const l_PositionAdds[2] =
{
    { 1801.369f, -521.248f, 196.795f, 0.030723f },
    { 1860.075f, -497.532f, 196.796f, 4.054334f }
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
        }

        enum eBoneMawSpells
        {
            SpellNecroticPitchTriggerMissile  = 153689,
            SpellNecroticPitchAreatrigger     = 153690,
            SpellNecroticPitchDummy           = 153691,
            SpellNecroticPitchDebuff          = 153692,
            SpellCorpseBreathPeriodic         = 165578,
            SpellCorpseBreathDamage           = 165579,
            SpellFetidSpitDamage              = 153681,
            SpellBodySlam                     = 154175,
            SpellInhaleVisual                 = 153721,
            SpellInhalePeriodicChannel        = 153804,
            SpellInhaleDamage                 = 153908,
            SpellInhaleAreaTrigger            = 153961,
            SpellInhaleSleep                  = 154010,
            SpellnhaleScreenEffect            = 154008,
            SpellVisualSubmerge               = 177694
        };

        enum eBoneMawEvents
        {
            EventNecroticPitch = 1,
            EventCorpseBreath,
            EventFetidSpit,
            EventBodySlam,
            EventInhale,
            EventCarrionWorm,
            EventCancelBodySlamFlags,
            EventCancelSubmerge,
            EventSubmerge
        };

        InstanceScript* m_Instance;
        uint32 m_PoolDiff;
        uint32 m_InhaleDiff;
        bool m_HasVictimOut;
        bool m_InhaleActivated;

        void Reset() override
        {
            _Reset();
            events.Reset();
            me->SetCanFly(true);
            me->RemoveAllAuras();
            m_HasVictimOut = false;
            m_InhaleActivated = false;
            me->SetDisableGravity(true);
            m_PoolDiff   = 3 * TimeConstants::IN_MILLISECONDS;;
            m_InhaleDiff = 4 * TimeConstants::IN_MILLISECONDS;       
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();
            HandleEntranceDoorActivation();
            if (m_Instance != nullptr)
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossBonemaw, EncounterState::FAIL);
        }

        void HandleDoorActivation()
        {
            std::list<GameObject*> l_ListDoors;
            me->GetGameObjectListWithEntryInGrid(l_ListDoors, eBoneMawGameObjects::GameObjectBonemawDoors, 100.0f);
            me->GetGameObjectListWithEntryInGrid(l_ListDoors, eBoneMawGameObjects::GameObjectBonemawDoors01, 100.0f);
            if (l_ListDoors.empty())
                return;

            for (GameObject* l_Itr : l_ListDoors)
            {
                /// Activate
                l_Itr->SetLootState(LootState::GO_READY);
                l_Itr->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
            }
        }

        void HandleEntranceDoorActivation()
        {
            if (m_Instance != nullptr)
            {
                if (GameObject* l_BonemawEntranceDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBonemawDoorEntrance)))
                {
                    /// Activate
                    l_BonemawEntranceDoor->SetLootState(LootState::GO_READY);
                    l_BonemawEntranceDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eBoneMawActions::ActionInhaleDeactivate:
                    m_InhaleActivated = false;
                    break;
                default:
                    break;
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();
            HandleEntranceDoorActivation();
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossBonemaw, EncounterState::IN_PROGRESS);
                if (m_Instance->instance->IsHeroic())
                    events.ScheduleEvent(eBoneMawEvents::EventCarrionWorm, 70 * TimeConstants::IN_MILLISECONDS);
            }     
            events.ScheduleEvent(eBoneMawEvents::EventNecroticPitch, 50 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventBodySlam, 30 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventCorpseBreath, 20 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventInhale, 60 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBoneMawEvents::EventSubmerge, urand(70 * TimeConstants::IN_MILLISECONDS, 100 * TimeConstants::IN_MILLISECONDS));     
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            summons.DespawnAll();
            HandleDoorActivation();
            HandleEntranceDoorActivation();
            if (m_Instance != nullptr)
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossBonemaw, EncounterState::DONE);    
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            /// Inhale mechanismActionInhaleDeactivate
            if (m_InhaleActivated)
            {
                if (m_InhaleDiff <= p_Diff)
                {
                    std::list<Player*> l_ListPlayers;
                    me->GetPlayerListInGrid(l_ListPlayers, 100.0f);
                    if (l_ListPlayers.empty())
                        return;

                    Position l_Position;
                    me->GetPosition(&l_Position);
                    for (Player* l_Itr : l_ListPlayers)
                    {            
                        if (m_Instance != nullptr)
                        {
                            if (Creature* l_BonemawMouth = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBonemawMouth)))
                            {
                                if (l_Itr->IsWithinDistInMap(l_BonemawMouth, 4.0f))
                                    l_Itr->CastSpell(l_Itr, eBoneMawSpells::SpellInhaleDamage);

                                if (l_Itr->IsWithinDist(l_BonemawMouth, 100.0f, true))
                                {
                                    if (l_Itr->isAlive() && !l_Itr->HasMovementForce(l_BonemawMouth->GetGUID()))
                                        l_Itr->SendApplyMovementForce(l_BonemawMouth->GetGUID(), true, l_Position, 3.0f, 1);
                                    else if (!l_Itr->isAlive() && l_Itr->HasMovementForce(l_BonemawMouth->GetGUID()))
                                        l_Itr->SendApplyMovementForce(l_BonemawMouth->GetGUID(), false, l_Position);
                                }
                                else if (l_Itr->HasMovementForce(l_BonemawMouth->GetGUID()))
                                    l_Itr->SendApplyMovementForce(l_BonemawMouth->GetGUID(), false, l_Position);
                            }
                        }
                    }
                    m_InhaleDiff = 4 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_InhaleDiff -= p_Diff;
            }

            /// Fetid Spit mechanism - hardcoded.
            if (Unit* l_Target = me->getVictim())
            {
                if (me->IsWithinMeleeRange(l_Target))
                {
                    if (m_HasVictimOut)
                    {
                        m_HasVictimOut = false;
                        events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
                    }
                }
                else
                {
                    if (m_PoolDiff <= p_Diff)
                    {
                        m_HasVictimOut = true;
                        events.CancelEvent(eBoneMawEvents::EventFetidSpit);
                        DoCastAOE(eBoneMawSpells::SpellFetidSpitDamage, true);

                        m_PoolDiff = 5 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_PoolDiff -= p_Diff;
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eBoneMawEvents::EventFetidSpit:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eBoneMawSpells::SpellFetidSpitDamage);
                    events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eBoneMawEvents::EventSubmerge:
                    events.Reset();
                    me->CastSpell(me, eBoneMawSpells::SpellVisualSubmerge);             
                    events.ScheduleEvent(eBoneMawEvents::EventCancelSubmerge, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eBoneMawEvents::EventBodySlam:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eBoneMawSpells::SpellBodySlam);
                    events.ScheduleEvent(eBoneMawEvents::EventBodySlam, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eBoneMawEvents::EventCarrionWorm:
                {
                    me->CastSpell(me, eBoneMawSpells::SpellVisualSubmerge);
                    for (uint8 l_I = 0; l_I < 2; l_I++)
                        me->SummonCreature(eBoneMawCreatures::CreatureCarrionWorm, l_PositionAdds[l_I], TEMPSUMMON_MANUAL_DESPAWN);
                    events.ScheduleEvent(eBoneMawEvents::EventCancelSubmerge, 3 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eBoneMawEvents::EventCancelSubmerge:
                {
                    std::list<Creature*> l_ListCarrionWorms;
                    me->GetCreatureListWithEntryInGrid(l_ListCarrionWorms, eBoneMawCreatures::CreatureCarrionWorm, 70.0f);
                    if (!l_ListCarrionWorms.empty())
                    {
                        for (Creature* l_Itr : l_ListCarrionWorms)
                        {
                            if (l_Itr->isAlive())
                                l_Itr->RemoveAura(eBoneMawSpells::SpellVisualSubmerge);
                        }
                    }
                    me->RemoveAura(eBoneMawSpells::SpellVisualSubmerge);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                    /// Resets all events and then rescheduel them to prevent the events from running while submerged.
                    events.Reset();
                    events.ScheduleEvent(eBoneMawEvents::EventNecroticPitch, 50 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eBoneMawEvents::EventBodySlam, 30 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eBoneMawEvents::EventCorpseBreath, 20 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eBoneMawEvents::EventInhale, 60 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eBoneMawEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eBoneMawEvents::EventSubmerge, urand(30 * TimeConstants::IN_MILLISECONDS, 70 * TimeConstants::IN_MILLISECONDS));
                    if (m_Instance != nullptr)
                    {
                        if (m_Instance->instance->IsHeroic())
                            events.ScheduleEvent(eBoneMawEvents::EventCarrionWorm, 70 * TimeConstants::IN_MILLISECONDS);
                    }
                    events.ScheduleEvent(eBoneMawEvents::EventSubmerge, urand(70 * TimeConstants::IN_MILLISECONDS, 100 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eBoneMawEvents::EventCorpseBreath:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eBoneMawSpells::SpellCorpseBreathPeriodic);
                    events.ScheduleEvent(eBoneMawEvents::EventCorpseBreath, 15 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eBoneMawEvents::EventInhale:
                {
                    m_InhaleActivated = true;
                    m_InhaleDiff = 2 * TimeConstants::IN_MILLISECONDS;
                    me->CastSpell(me, eBoneMawSpells::SpellInhalePeriodicChannel);
                    events.ScheduleEvent(eBoneMawEvents::EventInhale, 55 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eBoneMawEvents::EventNecroticPitch:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eBoneMawSpells::SpellNecroticPitchTriggerMissile);
                    events.ScheduleEvent(eBoneMawEvents::EventNecroticPitch, 25 * TimeConstants::IN_MILLISECONDS);
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
        return new boss_bonemawAI(p_Creature);
    }
}; /// Morgo, if you find this - you're a fat whore. <3.

/// Carrion Worm - 76057
class shadowmoon_burial_grounds_bonemaw_creature_carrion_worm : public CreatureScript
{
public:
    shadowmoon_burial_grounds_bonemaw_creature_carrion_worm() : CreatureScript("shadowmoon_burial_grounds_bonemaw_creature_carrion_worm") { }

    struct shadowmoon_burial_grounds_bonemaw_creature_carrion_wormAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_bonemaw_creature_carrion_wormAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eCarrionWormSpells
        {
            SpellFetidSpitDamage = 153681,
            SpellVisualSubmerge  = 177694
        };

        enum eCarrionWormEvents
        {
            EventFetidSpit = 1
        };

        InstanceScript* m_Instance;
        uint32 m_PoolDiff;
        bool m_HasVictimOut;

        void Reset() override
        {
            events.Reset();
            DoZoneInCombat();
            me->SetCanFly(true);
            m_HasVictimOut = false;
            me->SetDisableGravity(true);
            m_PoolDiff = 2 * TimeConstants::IN_MILLISECONDS;
            me->AddAura(eCarrionWormSpells::SpellVisualSubmerge, me);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS,  eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eCarrionWormEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            /// Fetid Spit mechanism - hardcoded.
            if (Unit* l_Target = me->getVictim())
            {
                if (me->IsWithinMeleeRange(l_Target))
                {
                    if (m_HasVictimOut)
                    {
                        m_HasVictimOut = false;
                        events.ScheduleEvent(eCarrionWormEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
                    }
                }
                else
                {
                    if (m_PoolDiff <= p_Diff)
                    {                
                        m_HasVictimOut = true;
                        DoCastAOE(eCarrionWormSpells::SpellFetidSpitDamage, true);               
                        events.CancelEvent(eCarrionWormEvents::EventFetidSpit);
                        m_PoolDiff = 5 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_PoolDiff -= p_Diff;
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eCarrionWormEvents::EventFetidSpit:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eCarrionWormSpells::SpellFetidSpitDamage);
                        events.ScheduleEvent(eCarrionWormEvents::EventFetidSpit, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_bonemaw_creature_carrion_wormAI(p_Creature);
    }
};

/// Necrotic Pitch - 76191
class shadowmoon_burial_grounds_bonemaw_creature_necrotic_pitch : public CreatureScript
{
public:

    shadowmoon_burial_grounds_bonemaw_creature_necrotic_pitch() : CreatureScript("shadowmoon_burial_grounds_bonemaw_creature_necrotic_pitch") { }

    struct shadowmoon_burial_grounds_bonemaw_creature_necrotic_pitchAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_bonemaw_creature_necrotic_pitchAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eNecroticPitchSpells
        {
            SpellNecroticPitchDebuff = 153692
        };

        uint32 m_Timer;
        InstanceScript* m_Instance;
       
        void Reset() override
        {
            events.Reset();
            me->setFaction(FriendlyFaction);
            me->SetDisplayId(InvisibleDisplay);
            m_Timer = 2 * TimeConstants::IN_MILLISECONDS;
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);                
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (m_Timer <= p_Diff)
            {
                std::list<Player*> l_ListPlayers;
                me->GetPlayerListInGrid(l_ListPlayers, 1.12f);
                if (l_ListPlayers.empty())
                    return;

                for (Player* l_Itr : l_ListPlayers)
                {
                    if (!l_Itr->HasAura(eNecroticPitchSpells::SpellNecroticPitchDebuff))
                        l_Itr->AddAura(eNecroticPitchSpells::SpellNecroticPitchDebuff, l_Itr);
                }

                m_Timer = 2 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Timer -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_bonemaw_creature_necrotic_pitchAI(p_Creature);
    }
};

/// Inhale - 153804 
class shadowmoon_burial_grounds_bonemaw_spell_inhale : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_bonemaw_spell_inhale() : SpellScriptLoader("shadowmoon_burial_grounds_bonemaw_spell_inhale") { }

    class shadowmoon_burial_grounds_bonemaw_spell_inhale_AuraScript : public AuraScript
    {
        PrepareAuraScript(shadowmoon_burial_grounds_bonemaw_spell_inhale_AuraScript);

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                {
                    if (Creature* l_Bonemaw = l_Instance->instance->GetCreature(l_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossBonemaw)))
                    {
                        if (l_Bonemaw->IsAIEnabled)
                            l_Bonemaw->GetAI()->DoAction(eBoneMawActions::ActionInhaleDeactivate);
                    }
                }
            }
        }

        void Register()
        {     
            AfterEffectRemove += AuraEffectRemoveFn(shadowmoon_burial_grounds_bonemaw_spell_inhale_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new shadowmoon_burial_grounds_bonemaw_spell_inhale_AuraScript();
    }
};

/// Body Slam - 153686
class shadowmoon_burial_grounds_bonemaw_spell_body_slam : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_bonemaw_spell_body_slam() : SpellScriptLoader("shadowmoon_burial_grounds_bonemaw_spell_body_slam") { }

    class shadowmoon_burial_grounds_bonemaw_spell_body_slam_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_bonemaw_spell_body_slam_SpellScript);

        enum eBodySlamSpells
        {
            TargetRestrict = 18748
        };

        void CorrectTargets(std::list<WorldObject*>& p_Targets)
        {
            if (p_Targets.empty())
                return;

            SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eBodySlamSpells::TargetRestrict);
            if (l_Restriction == nullptr)
                return;

            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            float l_Radius = GetSpellInfo()->Effects[SpellEffIndex::EFFECT_0].CalcRadius(l_Caster);
            p_Targets.remove_if([l_Radius, l_Caster, l_Restriction](WorldObject* p_Object) -> bool
            {
                if (p_Object == nullptr)
                    return true;

                if (!p_Object->IsInAxe(l_Caster, l_Restriction->Width, l_Radius))
                    return true;

                if (!p_Object->isInFront(l_Caster))
                    return true;

                return false;
            });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(shadowmoon_burial_grounds_bonemaw_spell_body_slam_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_CONE_ENEMY_129);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(shadowmoon_burial_grounds_bonemaw_spell_body_slam_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_1, Targets::TARGET_UNIT_CONE_ENEMY_129);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_bonemaw_spell_body_slam_SpellScript();
    }
};

/// Corpse Breath - 165578  
class shadowmoon_burial_grounds_bonemaw_spell_corpse_breath : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_bonemaw_spell_corpse_breath() : SpellScriptLoader("shadowmoon_burial_grounds_bonemaw_spell_corpse_breath") { } /// Outside smoking ma Jointe

    class shadowmoon_burial_grounds_bonemaw_spell_corpse_breath_AuraScript : public AuraScript
    {
        PrepareAuraScript(shadowmoon_burial_grounds_bonemaw_spell_corpse_breath_AuraScript);

        enum eCorpseBreathSpells
        {
            SpellCorpseBreathDamage = 165579
        };

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->IsAIEnabled)
                {
                    if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        l_Caster->CastSpell(l_Target, eCorpseBreathSpells::SpellCorpseBreathDamage);
                }
            }
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                        l_Instance->DoRemoveAurasDueToSpellOnPlayers(eCorpseBreathSpells::SpellCorpseBreathDamage);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(shadowmoon_burial_grounds_bonemaw_spell_corpse_breath_AuraScript::OnApply, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(shadowmoon_burial_grounds_bonemaw_spell_corpse_breath_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new shadowmoon_burial_grounds_bonemaw_spell_corpse_breath_AuraScript();
    }
};

/// Necrotic Pitch - 153689
class shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch() : SpellScriptLoader("shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch") { }

    class shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch_SpellScript);

        void HandleTriggerMissile(SpellEffIndex p_EffIndex)
        {
            PreventHitDefaultEffect(p_EffIndex);

            if (!GetCaster())
                return;

            if (const WorldLocation* l_WorldLocation = GetExplTargetDest())
            GetCaster()->SummonCreature(eBoneMawCreatures::CreatureNecroticPitchTrigger, l_WorldLocation->GetPositionX(), l_WorldLocation->GetPositionY(), l_WorldLocation->GetPositionZ(), l_WorldLocation->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 60 * TimeConstants::IN_MILLISECONDS);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch_SpellScript::HandleTriggerMissile, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_TRIGGER_MISSILE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch_SpellScript();
    }
};

/// Drowned - 154010  
class shadowmoon_burial_grounds_bonemaw_spell_drowned : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_bonemaw_spell_drowned() : SpellScriptLoader("shadowmoon_burial_grounds_bonemaw_spell_drowned")
    {
    }

    class shadowmoon_burial_grounds_bonemaw_spell_drowned_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_bonemaw_spell_drowned_SpellScript)

        void HandleScript(SpellEffIndex /*p_EffIndex*/)
        {
            if (!GetCaster())
                return;

            GetCaster()->NearTeleportTo(1830.360f, -505.889f, 201.652f, 5.194756f, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(shadowmoon_burial_grounds_bonemaw_spell_drowned_SpellScript::HandleScript, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_bonemaw_spell_drowned_SpellScript();
    }
};

void AddSC_boss_bonemaw()
{
    new boss_bonemaw();                                                 ///< 75452
    new shadowmoon_burial_grounds_bonemaw_creature_carrion_worm();      ///< 76057
    new shadowmoon_burial_grounds_bonemaw_creature_necrotic_pitch();    ///< 76191
    new shadowmoon_burial_grounds_bonemaw_spell_inhale();               ///< 153804
    new shadowmoon_burial_grounds_bonemaw_spell_drowned();              ///< 154010
    new shadowmoon_burial_grounds_bonemaw_spell_body_slam();            ///< 153686
    new shadowmoon_burial_grounds_bonemaw_spell_necrotic_pitch();       ///< 153689
    new shadowmoon_burial_grounds_bonemaw_spell_corpse_breath();        ///< 165578
}