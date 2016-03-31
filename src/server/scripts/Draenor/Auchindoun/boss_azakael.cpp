
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptedCreature.h"
#include "auchindon.hpp"

enum eAzzakelSpells
{
    SpellClawsOfArgusBuff            = 153762,
    SpellClawsOfArgusVisual          = 153764,
    SpellClawsOfArgusDmg             = 153772,
    SpellCurtainOfFlameAura          = 153392,
    SpellCurtainOfFlameForceCast     = 153396,
    SpellCurtainOfFlameVisual        = 153400,
    SpellFelLashVisual               = 153234,
    SpellFelLashDummy                = 174872,
    SpellFelLashDebuff               = 177120,
    SpellFelLashDebuffTwo            = 177121,
    SpellMalevilentCrush             = 153499,
    SpellFelPoolAreatriger           = 153500, 
    SpellFelPoolDebuffDmg            = 153616,
    SpellVisualFelBurst              = 169682,
    SpellFelSparkAreaTrigger         = 153725,
    SpellFelSparkDamage              = 153726,
    SpellFelSparkPerioidicCreation   = 153727,
    SpellSummonImp                   = 153775,
    SpellFelGuard                    = 164080,
    SpellSummonPyromaniac            = 164127,
};

enum eAzzakelEvents
{
    EventClawsOfArgus = 1,
    EventCurtainOfFlame,
    EventFelLash,
    EventFelPool,
    EventFelSpark = 78,
    EventMalevolentCrush01,
    EventMalevolentCrush02,
    EventSummonAzzakel01,
    EventSummonAzzakel02
};

enum eAzzakelTalks
{
    AzzakelIntro   = 37,  ///< Who Dares Meddlie In The Works Of The Legion?! (46776) 
    AzzakelAggro   = 38,  ///< This World...All World...Shell Burn!(46774)
    AzzakelSpell03 = 39,  ///< Die, Insect!(46781)
    AzzakelSpell02 = 40,  ///< Burn In The Master'S Fire!(46780)
    AzzakelSpell01 = 41,  ///< Come Forth, Servants!(46779)
    AzzakelKill01  = 42,  ///< The Masters Blase Your Soul! (46777)
    AzzakelKill02  = 43,  ///< Burn! (46778)
    AzzakelDeath   = 44   ///< (46775)
};

enum eAzzakelTriggers
{
    TriggerFelPool = 326526,
    TriggerFelSpark = 76197,
    TriggerDemonSummoning = 432636
};

enum eAzzakelActions
{
    ActionFelSpark = 595,
    ActionSummonDemons = 596,
    ActionRenewEvents = 597,
    ActionBoolActivate = 598,
    ActionBoolDeactivate = 599
};

Position const g_PositionAzzakel_Blackgate = { 1929.65f, 2699.27f, 30.799f, 4.428220f };
Position const g_PositionSpawningFlyCoords[2] =
{
    { 1912.13f, 2720.44f, 49.818f, 1.600908f },
    { 1911.65f, 2757.73f, 30.799f, 4.748000f }
};
Position const g_PositionAzzakel_Blackgate_little[4] =
{
    { 1911.90f, 2680.62f, 31.418f, 1.450705f },
    { 1911.79f, 2764.35f, 31.418f, 4.721891f },
    { 1953.55f, 2722.47f, 31.418f, 3.139304f },
    { 1869.70f, 2722.45f, 31.418f, 0.001632f }
};

static void HandleDoors(Unit* me)
{
    std::list<GameObject*> l_ListGameObjects;
    me->GetGameObjectListWithEntryInGrid(l_ListGameObjects, eAuchindonObjects::GameobjectFelBarrier, 100.0f);

    if (l_ListGameObjects.empty())
        return;

    for (auto itr : l_ListGameObjects)
    {
        itr->Delete();
    }
}

/// Azzakael Controller - 
class auchindon_azzakel_controller : public CreatureScript
{
public:
    auchindon_azzakel_controller() : CreatureScript("auchindon_azzakel_controller") { }

    struct auchindon_creatures : public Scripted_NoMovementAI
    {
        auchindon_creatures(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();

            m_Counting = 0;
            m_Summoned = false;

            me->setFaction(eAuchindonInformation::InformationFriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);

            HandleDoors(me);

            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        InstanceScript* m_Instance;
        uint64 m_Azzakel = NULL;
        int32 m_Counting;
        bool m_Summoned;

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eAuchindonGeneralActions::ActionCountPre3StBossKill:
                    m_Counting = m_Counting + 1;

                    if (m_Counting > 6 && !m_Summoned)
                    {
                        m_Summoned = true;
                        me->NearTeleportTo(1911.50f, 2722.52f, 30.799f, g_PositionAzzakel_Blackgate.GetOrientation());

                        me->CastSpell(me, SpellVisualFelBurst);
                        events.ScheduleEvent(EventSummonAzzakel01, 2 * TimeConstants::IN_MILLISECONDS);
                    }
                    break;
            }
        }

        void UpdateAI(const uint32 p_p_Diff) override
        {
            events.Update(p_p_Diff);

            switch (events.ExecuteEvent())
            {
                case eAzzakelEvents::EventSummonAzzakel01:
                    {
                        if (Creature* TempAzzakael = me->SummonCreature(eAuchindonBosses::BossAzaakel, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            if (GameObject* l_Portal = me->SummonGameObject(eAuchindonObjects::GameobjectDemonicPortal, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), 0, 0, 0, 0, 0))
                            {
                                m_Azzakel = TempAzzakael->GetGUID();
                                TempAzzakael->GetMotionMaster()->MoveJump(g_PositionSpawningFlyCoords[1].GetPositionX(), g_PositionSpawningFlyCoords[1].GetPositionY(), g_PositionSpawningFlyCoords[1].GetPositionZ(), 30.0f, 25.0f);

                                l_Portal->SetFlag(11, GameObjectFlags::GO_FLAG_NOT_SELECTABLE | GameObjectFlags::GO_FLAG_NODESPAWN | GameObjectFlags::GO_FLAG_INTERACT_COND);
                                events.ScheduleEvent(eAzzakelEvents::EventSummonAzzakel02, 1 * TimeConstants::IN_MILLISECONDS);
                                break;
                            }
                        }
                    }
                case eAzzakelEvents::EventSummonAzzakel02:
                    {
                        if (m_Azzakel != NULL)
                        {
                            if (Creature* l_Azzakael = sObjectAccessor->GetCreature(*me, m_Azzakel))
                            {
                                l_Azzakael->GetMotionMaster()->MoveCharge(1911.93f, 2754.40f, 30.973f, 42.0f);
                                l_Azzakael->setFaction(eAuchindonInformation::InformationHostileFaction);
                            }
                            break;
                        }
                    }
           }
        }
    
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creatures(p_Creature);
    }
};

/// Azzakael - 75927
class auchindon_boss_azzakel : public CreatureScript
{
public:
    auchindon_boss_azzakel() : CreatureScript("auchindon_boss_azzakel") { }

    struct boss_azzakelAI : public BossAI
    {
        boss_azzakelAI(Creature* p_Creature) : BossAI(p_Creature, eDataAuchindonDatas::DataBossAzzakael)
        {
            m_Instance = me->GetInstanceScript();

            me->setFaction(eAuchindonInformation::InformationFriendlyFaction);
            m_Intro = false;
        }

        InstanceScript* m_Instance;
        uint32 m_Interval;
        bool m_Intro;
        bool m_Argus;
        bool m_Achievement;

        void Reset() override
        {
            _Reset();
            m_Interval = 3 * TimeConstants::IN_MILLISECONDS;
            m_Argus = false;

            uint32 l_Entries[4] = { TriggerDemonSummoning, CreatureCacklingPyromaniac, CreatureBlazingTrickster, CreatureFelguard };
            for (uint32 l_Entry : l_Entries)
                DespawnCreaturesInArea(l_Entry, me);
        }

        void MoveInLineOfSight(Unit* p_Who)
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro)
            {
                m_Intro = true;
                Talk(eAzzakelTalks::AzzakelIntro);
            }
        }

        void DoAction(int32 const p_Action)
        {
            switch (p_Action)
            {
                case eAuchindonGeneralActions::ActionDemonSoulsAchievement:
                    m_Achievement = false;
                    break;
                case eAzzakelActions::ActionBoolDeactivate:
                    m_Argus = false;
                    me->UpdatePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), true);
                    break;
                case eAzzakelActions::ActionBoolActivate:
                    m_Argus = true;
                    break;
                case eAzzakelActions::ActionFelSpark:
                    events.ScheduleEvent(eAzzakelEvents::EventFelSpark, 1 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eAzzakelActions::ActionRenewEvents:
                {
                    events.Reset();
                    me->GetMotionMaster()->Clear();

                    // Fix stuck boss
                    me->UpdatePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), false);
                    
                    // Shampoo <3
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        AttackStart(l_Target);

                    events.ScheduleEvent(eAzzakelEvents::EventFelLash,           8  * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eAzzakelEvents::EventClawsOfArgus,      45 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eAzzakelEvents::EventCurtainOfFlame,    14 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eAzzakelEvents::EventMalevolentCrush02, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();

            HandleDoors(me);

            if (m_Instance != nullptr)
            {
                instance->SetBossState(eDataAuchindonDatas::DataBossAzzakael, EncounterState::FAIL);
            }

            uint32 l_Entries[3] = { CreatureCacklingPyromaniac, CreatureBlazingTrickster, CreatureFelguard };
            for (uint32 l_Entry : l_Entries)
                DespawnCreaturesInArea(l_Entry, me);
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            _EnterCombat();

            Talk(eAzzakelTalks::AzzakelAggro);

            if (m_Instance != nullptr)
            {
                m_Instance->SetBossState(eDataAuchindonDatas::DataBossAzzakael, IN_PROGRESS);
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }

            m_Achievement = true;

            // Manual, I don't remember why I handled it that way - there must be a reason, I'm not touching
            me->SummonGameObject(eAuchindonObjects::GameobjectFelBarrier, 1911.01f, 2722.89f, 30.799f, g_PositionAzzakel_Blackgate.GetOrientation(), 0, 0, 0, 0, 0);

            events.ScheduleEvent(eAzzakelEvents::EventFelLash, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAzzakelEvents::EventClawsOfArgus, 45 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAzzakelEvents::EventCurtainOfFlame, 14 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAzzakelEvents::EventMalevolentCrush02, 20 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eAzzakelTalks::AzzakelKill01);
                else
                    Talk(eAzzakelTalks::AzzakelKill02);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            Talk(eAzzakelTalks::AzzakelDeath);

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);;
                m_Instance->SetBossState(eDataAuchindonDatas::DataBossAzzakael, EncounterState::DONE);

                if (m_Achievement)
                {
                    if (me->GetMap() && me->GetMap()->IsHeroic())
                    {
                        m_Instance->DoCompleteAchievement(eAuchindonAchievements::AchievementDemonSouls);
                    }
                }
            }

            uint32 l_Entries[3] = { CreatureCacklingPyromaniac, CreatureBlazingTrickster, CreatureFelguard };
            for (uint32 l_Entry : l_Entries)
                DespawnCreaturesInArea(l_Entry, me);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            if (m_Argus) // claws of argus
            {
                if (m_Interval < p_Diff)
                {
                    if (Creature* l_AzzakelController = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataTriggerAzzakelController)))
                    {
                        switch (urand(0, 3))
                        {
                            case 0:
                                l_AzzakelController->CastSpell(l_AzzakelController, eAzzakelSpells::SpellSummonImp);
                                break;
                            case 1:
                                l_AzzakelController->CastSpell(l_AzzakelController, eAzzakelSpells::SpellFelGuard);
                                break;
                            case 2:
                                l_AzzakelController->CastSpell(l_AzzakelController, eAzzakelSpells::SpellSummonPyromaniac);
                                break;
                        }
                    }

                    m_Interval = 3000;
                }
                else
                    m_Interval -= p_Diff;
            }

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAzzakelEvents::EventFelLash:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eAzzakelSpells::SpellFelLashVisual);

                        events.ScheduleEvent(eAzzakelEvents::EventFelLash, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eAzzakelEvents::EventClawsOfArgus:
                    {
                        events.Reset();
                        Talk(eAzzakelTalks::AzzakelSpell01);

                        me->CastSpell(me, 153764); 
                        me->MonsterTextEmote("Azzakel casts |cffff0000[Azzakael casts [Claws of Agrus]|cfffaeb00!", me->GetGUID(), true);
                        events.ScheduleEvent(eAzzakelEvents::EventClawsOfArgus, 45 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                case eAzzakelEvents::EventCurtainOfFlame:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            Talk(eAzzakelTalks::AzzakelSpell03);
                            me->AddAura(eAzzakelSpells::SpellCurtainOfFlameAura, l_Target);

                            std::string l_Str;
                            l_Str += "Azzakel casts |cffff0000[Curtain of Flame]|cfffaeb00! on ";
                            l_Str += l_Target->GetName();

                            me->MonsterTextEmote(l_Str.c_str(), me->GetGUID(), true);

                            events.ScheduleEvent(eAzzakelEvents::EventCurtainOfFlame, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    }
                case eAzzakelEvents::EventMalevolentCrush01:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->GetMotionMaster()->MoveJump(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), 15.0f, 20.0f);

                        Talk(eAzzakelTalks::AzzakelSpell02);

                        events.ScheduleEvent(eAzzakelEvents::EventMalevolentCrush02, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                case eAzzakelEvents::EventMalevolentCrush02:
                    {
                        me->SummonCreature(eAzzakelTriggers::TriggerFelPool, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                        me->CastSpell(me, eAzzakelSpells::SpellFelPoolAreatriger);
                        break;
                    }
                case eAzzakelEvents::EventFelSpark:
                    {
                        Position l_Position;
                        me->GetPosition(&l_Position);

                        for (int i = 0; i < 3; i++)
                        {
                            me->SummonCreature(eAzzakelTriggers::TriggerFelSpark, l_Position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        }
                        break;
                    }
            }

            DoMeleeAttackIfReady();
        }
           
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_azzakelAI(p_Creature);
    }
};

/// Curtain of Flames - 153392
class auchindon_azzakel_spell_curtain_flames : public SpellScriptLoader
{
public:
    auchindon_azzakel_spell_curtain_flames() : SpellScriptLoader("auchindon_azzakel_spell_curtain_flames") { }

    class spell_auras : public AuraScript
    {
        PrepareAuraScript(spell_auras)

        void OnPeriodic(AuraEffect const* p_AurEff)
        {
            if (Unit* l_Target = GetTarget())
            {
                if (l_Target->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                std::list<Player*> l_ListPlayers;
                l_Target->GetPlayerListInGrid(l_ListPlayers, 3.0f);

                if (l_ListPlayers.empty())
                    return;

                for (auto itr : l_ListPlayers)
                {
                    if (!itr)
                        return;

                    if (GetTarget()->GetGUID() != itr->GetGUID())
                    {
                        l_Target->AddAura(SpellCurtainOfFlameAura, itr);
                        l_Target->CastSpell(l_Target, SpellCurtainOfFlameVisual);
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_auras::OnPeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_auras();
    }
};

/// Claw of Flames Loader - 153764
class auchindon_azzakel_spell_claws_of_argus : public SpellScriptLoader
{
public:
    auchindon_azzakel_spell_claws_of_argus() : SpellScriptLoader("auchindon_azzakel_spell_claws_of_argus") { }

    class auchindon_auras : public AuraScript
    {
        PrepareAuraScript(auchindon_auras);

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (!GetCaster())
                return;

            if (InstanceScript* l_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Azzakel = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataBossAzzakael)))
                {
                    if (!l_Azzakel->GetAI())
                        return;

                    l_Azzakel->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                    l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionBoolActivate);

                    l_Azzakel->SetCanFly(true);
                    l_Azzakel->SetDisableGravity(true);

                    l_Azzakel->GetMotionMaster()->MoveTakeoff(0, l_Azzakel->GetPositionX(), l_Azzakel->GetPositionY(), 42.118f);         
                }
            }
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (!GetCaster())
                return;

            if (InstanceScript* l_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Azzakel = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataBossAzzakael)))
                {
                    if (!l_Azzakel->GetAI())
                        return;

                    l_Azzakel->RemoveAllAuras();

                    if (l_Azzakel->GetMap() && l_Azzakel->GetMap()->IsHeroic())
                    {
                        if (Player* l_Nearest = l_Azzakel->FindNearestPlayer(20.0f, true))
                        {
                            l_Azzakel->GetMotionMaster()->MoveCharge(l_Nearest->GetPositionX(), l_Nearest->GetPositionY(), l_Nearest->GetPositionZ(), 42.0f);
                            l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionFelSpark);
                        }
                    }
                    else
                    {
                        GetCaster()->GetMotionMaster()->MovePoint(0, GetTarget()->GetPositionX(), GetTarget()->GetPositionY(), 35.590f);
                    }

                    l_Azzakel->SetReactState(ReactStates::REACT_DEFENSIVE);
                    l_Azzakel->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionBoolDeactivate);
                    l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionRenewEvents);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(auchindon_auras::OnApply,           SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(auchindon_auras::OnRemove,        SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindon_auras();
    }
};

/// Fel Spark - 153725
class auchindon_azzakel_fel_spark_area_trigger : public AreaTriggerEntityScript
{
public:
    auchindon_azzakel_fel_spark_area_trigger() : AreaTriggerEntityScript("auchindon_azzakel_fel_spark_area_trigger") {}

    uint32 m_Diff = 1 * TimeConstants::IN_MILLISECONDS;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Diff <= p_Time)
        {
            std::list<Player*> l_ListPlayers;
            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_ListPlayers, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

            if (l_ListPlayers.empty())
                return;

            for (std::list<Player*>::const_iterator it = l_ListPlayers.begin(); it != l_ListPlayers.end(); ++it)
            {
                if (!(*it))
                    return;

                if (!(*it)->HasAura(eAzzakelSpells::SpellFelPoolDebuffDmg))
                {
                    (*it)->AddAura(eAzzakelSpells::SpellFelPoolDebuffDmg, (*it));
                }
                else
                {
                    Aura* l_Aura = (*it)->GetAura(eAzzakelSpells::SpellFelPoolDebuffDmg);

                    if (l_Aura)
                    {
                        l_Aura->SetDuration(1);
                    }
                }
            }

            m_Diff = 1 * TimeConstants::IN_MILLISECONDS;
        }
        else
        {
            m_Diff -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        // No usage, duration = 1
    }

    auchindon_azzakel_fel_spark_area_trigger* GetAI() const override
    {
        return new auchindon_azzakel_fel_spark_area_trigger();
    }
};

/// Fel Pool - 326526
class auchindon_azzakel_fel_pool_trigger : public CreatureScript
{
public:
    auchindon_azzakel_fel_pool_trigger() : CreatureScript("auchindon_azzakel_fel_pool_trigger") {}

    struct auchindon_creaturesAI : public Scripted_NoMovementAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            me->SetDisplayId(eAuchindonInformation::InformationDisplayIdInvis);
            me->setFaction(eAuchindonInformation::InformationHostileFaction);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            std::list<Player*> l_ListPlayers;
            JadeCore::AnyPlayerInObjectRangeCheck check(me, 15.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, l_ListPlayers, check);
            me->VisitNearbyObject(15.0f, searcher);

            if (l_ListPlayers.empty())
                return;

            for (std::list<Player*>::const_iterator it = l_ListPlayers.begin(); it != l_ListPlayers.end(); ++it)
            {
                if (!(*it))
                    return;

                if ((*it)->IsWithinDistInMap(me, 5.0f))
                {
                    if (!(*it)->HasAura(eAzzakelSpells::SpellFelPoolDebuffDmg))
                    {
                        me->AddAura(eAzzakelSpells::SpellFelPoolDebuffDmg, (*it));
                    }
                }
                else
                {
                    if ((*it)->HasAura(eAzzakelSpells::SpellFelPoolDebuffDmg, me->GetGUID()))
                        (*it)->RemoveAura(eAzzakelSpells::SpellFelPoolDebuffDmg);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

void AddSC_azzakel()
{
    new auchindon_boss_azzakel();
    new auchindon_azzakel_controller();
    new auchindon_azzakel_spell_curtain_flames();
    new auchindon_azzakel_spell_claws_of_argus();
    new auchindon_azzakel_fel_spark_area_trigger();
    new auchindon_azzakel_fel_pool_trigger();
}