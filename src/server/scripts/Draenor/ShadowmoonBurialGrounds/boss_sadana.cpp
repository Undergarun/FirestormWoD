////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadowmoon_burial_grounds.hpp"

enum eSadanaSpells
{
    SpellDaggerFallDummy                    = 153240,
    SpellDaggerFallSummon                   = 153200,
    SpellDaggerProjectile                   = 153225,
    SpellDaggerAura                         = 153236,
    SpellDaggerAuraFunctioning              = 153236,
    SpellDaggerExplosion                    = 153232,

    SpellDarkCommunion                      = 153153,
    SpellDarkCommunionBuff                  = 153164,

    SpellDeathSpikeProcTriggerSpell         = 153079,
    SpellDeathSpikeDamage                   = 153089,
    SpellDeathSpikeFull                     = 162696,

    SpellWhispersOfTheDarkStarDamage        = 153093,
    SpellWhisperOfTheDarkStarTriggerSpell   = 153094,

    SpellDarkEclipsePeriodicDummy           = 164685,
    SpellDarkEclipseDamage                  = 164686,
    SpellDarkEclipseAreatrirger             = 164704,
    SpellDarkEclipseDummy                   = 164705,
    SpellDarkEclipseDummy02                 = 164706,
    SpellDarkEclipsePeriodicTrigger         = 164710,
    SpellDarkElcipseUnknownPeriodicDummy    = 164974,
    SpellLunaryPurtiyBuff                   = 162652,
    //SpellLunarPurityAreaTrigger             = 162620,

    SpellLunarPurityAreaTrigger             = 164974,
    SpellLunarPurityDamageModification      = 162652,

    SpellShadowRitual                       = 152158,
    SpellTenebreuxViolet                    = 152311,
};

enum eSadanaEvents
{
    EventDaggerFall = 1,
    EventDarkCommunion,
    EventDeathSpike,
    EventWhisperOfTheDarkStar,
    EventDarkEclipse,
    EventLunarPurity,
    EventDefiledSpirtMovement,
};

enum eSadanaTalks
{
    TalkAggro = 1, ///< Anscestors of the Shadowmoon.. shine in the darkness! Lend me your strength! [43539]
    TalkDeath,     ///< The dead shall rise again.. [43540]
    TalkIntro,     ///< Rivers of the damned.. rise! RISE in the name of Darkness... RISE in the name of NERZ'UL!.. the Shadowmoon beckons [43541]
    TalkKill01,    ///< Another spirit for the army! [43542]
    TalkKill02,    ///< Embrace the eternal darkness! [43543]
    TalkSpell01,   ///< For Nerz'ul! [43544]
    TalkSpell02,   ///< Mmm.. mmm.. YES! the perfect Sacrifice! [43545]
    TalkSpell03,   ///< Your souls will belong to me! [43546]
    TalkSpell04,   ///< Restless spirits.. heed my command! [43547]
};

enum eSadanaActions
{
    ActionActivateDefiledSpirit = 1, 
    ActionActivateLunarTriggers = 2,  
};

enum eSadanaCreatures
{
    CreatureDaggerFall     = 75981,
    CreatureEclipseTrigger = 76052,
    CreatureLunarTrigger   = 231315,
};
   
#define DaggerFallAltitude 268.028f
#define DaggerGroundAltitude 261.308f

Position l_PositionCircles[8] = 
{
    {1805.502f, -16.298f, 261.308f, 4.011894f},
    {1809.695f, -26.531f, 261.308f, 3.086692f},
    {1805.346f, -37.156f, 261.308f, 2.155996f},
    {1795.560f, -40.853f, 261.308f, 1.508042f},
    {1785.818f, -37.061f, 261.308f, 0.640176f},
    {1781.712f, -26.524f, 261.308f, 6.263623f},
    {1786.218f, -16.460f, 261.308f, 5.482167f},
    {1795.574f, -12.252f, 261.308f, 4.767466f},
};

void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
{
    std::list<Creature*> creatures;
    GetCreatureListWithEntryInGrid(creatures, object, entry, 5000.0f);
    if (creatures.empty())
        return;

    for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
        (*iter)->DespawnOrUnsummon();
}

/// Sadana Bloodyfury - 75509
class boss_sadana_bloodfury : public CreatureScript
{
public:
    boss_sadana_bloodfury() : CreatureScript("boss_sadana_bloodfury") { }

    struct boss_sadana_bloodfuryAI : public BossAI
    {
        boss_sadana_bloodfuryAI(Creature* p_Creature) : BossAI(p_Creature, eShadowmoonBurialGroundsDatas::DataBossSadana)
        {
            m_Instance = me->GetInstanceScript();

            HandleDoorActivation();

            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventTalk01, 60 * TimeConstants::IN_MILLISECONDS);
        }

        InstanceScript* m_Instance;
        uint64 m_CommunionGuid;

        void Reset() override
        {
            events.Reset();

            me->CastSpell(me, eSadanaSpells::SpellShadowRitual);
            m_CommunionGuid = 0;
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();

            DespawnCreaturesInArea(eSadanaCreatures::CreatureDaggerFall, me);
            DespawnCreaturesInArea(eSadanaCreatures::CreatureLunarTrigger, me);
            DespawnCreaturesInArea(eSadanaCreatures::CreatureEclipseTrigger, me);

            HandleDoorActivation();
        }

        void HandleDoorActivation()
        {
            if (m_Instance != nullptr)
            {
                if (GameObject* l_SadanaFightDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataSadanaFightDoor)))
                {
                    // Activate
                    l_SadanaFightDoor->SetLootState(LootState::GO_READY);
                    l_SadanaFightDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }
            }
        }

        void SummonPathsDefiledSpirits()
        {
            for (int i = 0; i <= 15; i++)
            {
                Position l_position;
                me->GetRandomNearPosition(l_position, 20.0f);
                l_position.m_positionZ = frand(261.391f, 284.058f);

                Creature* l_DefiledSpirit = me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureDefliedSpirit01, l_position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }
        }

        void SummonEclipseTriggers()
        {
            for (int i = 0; i <= 7; i++)
            {
                me->SummonCreature(eSadanaCreatures::CreatureEclipseTrigger, l_PositionCircles[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();
            me->RemoveAura(eSadanaSpells::SpellShadowRitual);

            SummonPathsDefiledSpirits();
            SummonEclipseTriggers();

            Talk(eSadanaTalks::TalkAggro);

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossSadana, EncounterState::IN_PROGRESS);

                DoZoneInCombat();
            }

            HandleDoorActivation();

            events.ScheduleEvent(eSadanaEvents::EventDaggerFall, 12 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSadanaEvents::EventWhisperOfTheDarkStar, 25 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSadanaEvents::EventDarkCommunion, 37 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSadanaEvents::EventDarkEclipse, 60 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eSadanaTalks::TalkKill01);
                else
                    Talk(eSadanaTalks::TalkKill02);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();

            Talk(eSadanaTalks::TalkDeath);
            HandleDoorActivation();

            if (m_Instance != nullptr)
            {
                if (GameObject* l_SadanaDoor = m_Instance->instance->GetGameObject(instance->GetData64(eShadowmoonBurialGroundsDatas::DataStoneDoorSadana)))
                {
                    // Activate
                    l_SadanaDoor->SetLootState(LootState::GO_READY);
                    l_SadanaDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            // RP introduction event
            if (events.ExecuteEvent() == eShadowmoonBurialGroundsEvents::EventTalk01)
            {
                Talk(eSadanaTalks::TalkIntro);
            }

            // Dark Communion
            if (m_CommunionGuid)
            {
                if (Creature* l_Creature = sObjectAccessor->GetCreature(*me, m_CommunionGuid))
                {
                    if (l_Creature->isDead())
                    {
                        l_Creature->DespawnOrUnsummon();
                    }              
                    else if (me->IsWithinDistInMap(l_Creature, 1.5f))
                    {
                        l_Creature->DespawnOrUnsummon();
                        me->CastSpell(me, eSadanaSpells::SpellDarkCommunionBuff);
                    }
                }
            }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case eSadanaEvents::EventDaggerFall:
                    {
                        for (int i = 0; i <= 2; i++)
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                Creature* l_DaggerFall = me->SummonCreature(eSadanaCreatures::CreatureDaggerFall, l_Target->GetPositionX(), l_Target->GetPositionX(), DaggerFallAltitude, l_Target->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                            }
                        }

                        events.ScheduleEvent(eSadanaEvents::EventDaggerFall, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eSadanaEvents::EventWhisperOfTheDarkStar:
                        me->CastSpell(me, eSadanaSpells::SpellWhisperOfTheDarkStarTriggerSpell);

                        events.ScheduleEvent(eSadanaEvents::EventWhisperOfTheDarkStar, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eSadanaEvents::EventDarkCommunion:
                        if (Creature* l_nearestDefiledSpirit = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureDefliedSpirit01, 100.0f, true))
                        {
                            me->CastSpell(l_nearestDefiledSpirit, eSadanaSpells::SpellDarkCommunion);
                            m_CommunionGuid = l_nearestDefiledSpirit->GetGUID();
                        }

                        events.ScheduleEvent(eSadanaEvents::EventDarkCommunion, 45 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eSadanaEvents::EventDeathSpike:
                        me->CastSpell(me, eSadanaSpells::SpellDeathSpikeFull);

                        events.ScheduleEvent(eSadanaEvents::EventDeathSpike, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eSadanaEvents::EventDarkEclipse:
                        me->NearTeleportTo(1795.788f, -26.7637f, 261.3088f, 4.629905f);

                        // Activate lunar triggers;
                        std::list<Creature*> l_ListTriggersLunars;
                        me->GetCreatureListWithEntryInGrid(l_ListTriggersLunars, eSadanaCreatures::CreatureLunarTrigger, 200.0f);

                        if (l_ListTriggersLunars.empty())
                            return;

                        for (auto itr : l_ListTriggersLunars)
                        {
                            if (itr->GetAI())
                                itr->GetAI()->DoAction(eSadanaActions::ActionActivateLunarTriggers);
                        }

                        me->CastSpell(me, eSadanaSpells::SpellDarkEclipsePeriodicDummy);
                        break;
                    }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_sadana_bloodfuryAI(p_Creature);
    }
};

/// Defiled Ground - 75966
class shadowmoon_burial_grounds_creature_defiled_ground : public CreatureScript
{
public:
    shadowmoon_burial_grounds_creature_defiled_ground() : CreatureScript("shadowmoon_burial_grounds_creature_defiled_ground") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* creature) : Scripted_NoMovementAI(creature), victimGUID(0)
        {
            m_Instance = creature->GetInstanceScript();
        }

        EventMap m_Events;
        InstanceScript* m_Instance;
        bool m_StartEvent;
        uint64 victimGUID;

        void Reset() override
        {
            me->setFaction(16);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->SetSpeed(MOVE_RUN, 0.3f, true);
            me->CastSpell(me, eSadanaSpells::SpellTenebreuxViolet);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);  

            events.ScheduleEvent(eSadanaEvents::EventDefiledSpirtMovement, 6 * TimeConstants::IN_MILLISECONDS);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eSadanaActions::ActionActivateDefiledSpirit:
                    events.Reset();

                    me->SetSpeed(MOVE_RUN, 0.3f, true);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case eSadanaEvents::EventDefiledSpirtMovement:
                    Position l_Position;
                    me->GetRandomNearPosition(l_Position, 10.0f);
                    l_Position.m_positionZ = frand(261.391f, 284.058f);

                    me->GetMotionMaster()->MoveTakeoff(0, l_Position);
                   
                    events.ScheduleEvent(eSadanaEvents::EventDefiledSpirtMovement + 1, 10 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eSadanaEvents::EventDefiledSpirtMovement, 16 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eSadanaEvents::EventDefiledSpirtMovement + 1:
                    me->GetMotionMaster()->MoveTargetedHome();
                    break;
                }
            }
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(creature);
    }
};

/// Falling Dagger - 75981
class shadowmoon_burial_grounds_creature_falling_dagger : public CreatureScript
{
public:
    shadowmoon_burial_grounds_creature_falling_dagger() : CreatureScript("shadowmoon_burial_grounds_creature_falling_dagger") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            m_Instance = creature->GetInstanceScript();
        }

        EventMap m_Events;
        InstanceScript* m_Instance;
        bool m_StartEvent;

        void Reset() override
        {
            me->setFaction(16);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->SetSpeed(MOVE_RUN, 0.5f, true);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

            me->CastSpell(me, eSadanaSpells::SpellDaggerAuraFunctioning);

            me->GetMotionMaster()->MoveTakeoff(0, me->GetPositionX(), me->GetPositionY(), DaggerGroundAltitude);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(creature);
    }
};

/// Lunar Trigger - 534513
class shadowmoon_burial_grounds_creature_lunar_trigger : public CreatureScript
{
public:
    shadowmoon_burial_grounds_creature_lunar_trigger() : CreatureScript("shadowmoon_burial_grounds_creature_lunar_trigger") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            m_Instance = creature->GetInstanceScript();
        }

        EventMap m_Events;
        InstanceScript* m_Instance;
        bool m_ReadyForAction;

        void Reset() override
        {
            m_ReadyForAction = false;

            me->setFaction(16);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->SetSpeed(MOVE_RUN, 0.5f, true);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

            me->CastSpell(me, eSadanaSpells::SpellDaggerAuraFunctioning);

            me->GetMotionMaster()->MoveTakeoff(0, me->GetPositionX(), me->GetPositionY(), DaggerGroundAltitude);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
            case eSadanaActions::ActionActivateLunarTriggers:
                me->CastSpell(me, eSadanaSpells::SpellLunarPurityAreaTrigger);

                if (!m_ReadyForAction)
                {
                    m_ReadyForAction = true;
                }
                break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            // Lunar Purity
            if (m_ReadyForAction)
            {
                std::list<Player*> l_PlayerList;
                me->GetPlayerListInGrid(l_PlayerList, 0.5f);

                if (l_PlayerList.empty())
                    return;

                for (auto itr : l_PlayerList)
                {
                    itr->CastSpell(itr, eSadanaSpells::SpellLunarPurityDamageModification);

                    if (AuraPtr l_Aura = itr->GetAura(eSadanaSpells::SpellLunarPurityDamageModification))
                        l_Aura->SetDuration(1);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(creature);
    }
};

/// Shadow Burn - 153224
class spell_shadow_burn : public SpellScriptLoader
{
public:
    spell_shadow_burn() : SpellScriptLoader("spell_shadow_burn") { }

    class shadow_burn_spell_script : public SpellScript
    {
        PrepareSpellScript(shadow_burn_spell_script);

        SpellCastResult CheckTarget()
        {
            if (!GetCaster())
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
        
            if (GetCaster()->GetPositionZ() == DaggerGroundAltitude)
                return SpellCastResult::SPELL_CAST_OK;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(shadow_burn_spell_script::CheckTarget);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new shadow_burn_spell_script;
    }
};

/// Dark Communion - 153153  
class spell_dark_communion : public SpellScriptLoader
{
public:
    spell_dark_communion() : SpellScriptLoader("spell_dark_communion") { }

    class spell_dark_communion_aura_script : public AuraScript
    {
        PrepareAuraScript(spell_dark_communion_aura_script);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetA = SelectTargetType::SELECT_TARGET_SINGLE_ENEMY;
            spell->Effects[0].TargetB = NULL;
            return true;
        }

        void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
        {
            if (!GetTarget())
                return;

            if (!GetCaster())
                return;

            GetTarget()->GetMotionMaster()->MovePoint(0, GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), GetCaster()->GetPositionZ());
            GetTarget()->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_dark_communion_aura_script::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dark_communion_aura_script();
    }
};

/// Dark Eclipse - 164685 
class spell_dark_eclipse : public SpellScriptLoader
{
public:
    spell_dark_eclipse() : SpellScriptLoader("spell_dark_eclipse") { }

    class spell_dark_eclipse_aura_script : public AuraScript
    {
        PrepareAuraScript(spell_dark_eclipse_aura_script);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].Amplitude = 4 * TimeConstants::IN_MILLISECONDS;
            return true;
        }

        void HandlePeriodic(constAuraEffectPtr aurEff)
        {
            PreventDefaultAction();
            if (GetCaster())
            {
                SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
                spell->Effects[0].Amplitude = 600;

                GetCaster()->CastSpell(GetCaster(), eSadanaSpells::SpellDarkEclipseDamage);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dark_eclipse_aura_script::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dark_eclipse_aura_script();
    }
};

/// Dark Eclipse damage - 164686 
class spell_dark_eclipse_damage : public SpellScriptLoader
{
public:
    spell_dark_eclipse_damage() : SpellScriptLoader("spell_dark_eclipse_damage") { }

    class spell_dark_eclipse_spell_script : public SpellScript
    {
        PrepareSpellScript(spell_dark_eclipse_spell_script);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            if (GetCaster()->HasAura(eSadanaSpells::SpellLunaryPurtiyBuff))
                SetHitDamage(0);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dark_eclipse_spell_script::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dark_eclipse_spell_script();
    }
};

void AddSC_sadana()
{
    new boss_sadana_bloodfury();

    new shadowmoon_burial_grounds_creature_defiled_ground();
    new shadowmoon_burial_grounds_creature_falling_dagger();
    new shadowmoon_burial_grounds_creature_lunar_trigger();

    new spell_shadow_burn();
    new spell_dark_communion();
    new spell_dark_eclipse();
    new spell_dark_eclipse_damage();
}