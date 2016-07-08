
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "ScriptedCreature.h"
#include "auchindoun.hpp"

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
    SpellFly                         = 161778
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

enum eAzzakelCreatures
{
    TriggerFelPool             = 326526,
    TriggerFelSpark            = 76197,
    TriggerDemonSummoning      = 432636,
    CreatureFelguard           = 76259,
    CreatureCacklingPyromaniac = 76260,
    CreatureBlazingTrickster   = 79511,
    CreatureBlazingTrickster02 = 76220
};

enum eAzzakelActions
{
    ActionFelSpark  = 1,
    ActionSummonDemons,
    ActionRenewEvents,
    ActionBoolActivate,
    ActionBoolDeactivate,
    ActionMalevolentCrash
};

enum eAzzakelMovements
{
    MovementAzzakelMalevolentCrash = 2
};

Position const g_PositionAzzakel_Blackgate = { 1929.65f, 2699.27f, 30.799f, 4.428220f };

Position const g_PositionSpawningFlyCoords[2] =
{
    { 1912.13f, 2720.44f, 49.818f, 1.600908f },
    { 1911.65f, 2757.73f, 30.799f, 4.748000f }
};

Position const g_PositionAzzakelBlackgateLittle[4] =
{
    { 1911.90f, 2680.62f, 31.418f, 1.450705f },
    { 1911.79f, 2764.35f, 31.418f, 4.721891f },
    { 1953.55f, 2722.47f, 31.418f, 3.139304f },
    { 1869.70f, 2722.45f, 31.418f, 0.001632f }
};

static void HandleDoors(Unit* p_Me)
{
    std::list<GameObject*> l_ListGameObjects;
    p_Me->GetGameObjectListWithEntryInGrid(l_ListGameObjects, eAuchindounObjects::GameobjectFelBarrier, 100.0f);
    if (l_ListGameObjects.empty())
        return;

    for (GameObject* l_Itr : l_ListGameObjects)
        l_Itr->Delete();
}

/// Azzakael Controller - 76216
class auchindoun_azzakel_mob_controller : public CreatureScript
{
    public:

    auchindoun_azzakel_mob_controller() : CreatureScript("auchindoun_azzakel_mob_controller") { }

    struct auchindoun_azzakel_mob_controllerAI : public Scripted_NoMovementAI
    {
        auchindoun_azzakel_mob_controllerAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_First = true;
        }

        InstanceScript* m_Instance;
        uint64 m_Azzakel;
        int32 m_Counting;
        bool m_Summoned;
        bool m_First;

        void Reset() override
        {
            if (m_First)
            {
                HandleDoors(me);
                m_Azzakel = 0;
                m_Counting = 0;
                m_First = false;
                m_Summoned = false;                  
                me->setFaction(FriendlyFaction);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            }
        }

        void JustSummoned(Creature* p_Summon) override
        {
            if (p_Summon)
            {
                switch (p_Summon->GetEntry())
                {
                case eAzzakelCreatures::CreatureBlazingTrickster:
                case eAzzakelCreatures::CreatureCacklingPyromaniac:
                case eAzzakelCreatures::CreatureFelguard:
                    p_Summon->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    p_Summon->SetInCombatWithZone();
                    break;
                default:
                    break;
                }
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eAuchindounActions::ActionCountPre3StBossKill:
                {
                    m_Counting = m_Counting + 1;

                    if (m_Counting > 6 && !m_Summoned)
                    {
                        m_Summoned = true;
                        me->CastSpell(me, eAzzakelSpells::SpellVisualFelBurst);
                        me->NearTeleportTo(1911.50f, 2722.52f, 30.799f, g_PositionAzzakel_Blackgate.GetOrientation());
                        events.ScheduleEvent(EventSummonAzzakel01, 2 * TimeConstants::IN_MILLISECONDS);
                    }
                    break;
                }
                default:
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
                    if (Creature* TempAzzakael = me->SummonCreature(eAuchindounBosses::BossAzaakel, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        if (GameObject* l_Portal = me->SummonGameObject(eAuchindounObjects::GameobjectDemonicPortal, *me, 0, 0, 0, 0, 0))
                        {
                            m_Azzakel = TempAzzakael->GetGUID();
                            l_Portal->SetFlag(11, GameObjectFlags::GO_FLAG_NOT_SELECTABLE | GameObjectFlags::GO_FLAG_NODESPAWN | GameObjectFlags::GO_FLAG_INTERACT_COND);
                            TempAzzakael->GetMotionMaster()->MoveJump(g_PositionSpawningFlyCoords[1].GetPositionX(), g_PositionSpawningFlyCoords[1].GetPositionY(), g_PositionSpawningFlyCoords[1].GetPositionZ(), 30.0f, 25.0f);                                    
                        }
                    }
                    events.ScheduleEvent(eAzzakelEvents::EventSummonAzzakel02, 1 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eAzzakelEvents::EventSummonAzzakel02:
                {
                    if (m_Azzakel)
                    {
                        if (Creature* l_Azzakael = Creature::GetCreature(*me, m_Azzakel))
                        {
                            l_Azzakael->setFaction(HostileFaction);
                            l_Azzakael->GetMotionMaster()->MoveCharge(1911.93f, 2754.40f, 30.973f, 42.0f);                     
                        }
                        break;
                    }
                }
                default:
                    break;
            }
        }
    
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_azzakel_mob_controllerAI(p_Creature);
    }
};


/// Fel Pool - 326526
class auchindoun_azzakel_mob_fel_pool : public CreatureScript
{
public:

    auchindoun_azzakel_mob_fel_pool() : CreatureScript("auchindoun_azzakel_mob_fel_pool") {}

    struct auchindoun_azzakel_mob_fel_poolAI : public Scripted_NoMovementAI
    {
        auchindoun_azzakel_mob_fel_poolAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_First = true;
        }

        enum eFelPoolSpells
        {
            SpellFelPoolDebuffDmg = 153616
        };

        bool m_First;

        void Reset() override
        {
            if (m_First)
            {
                m_First = false;
                me->setFaction(HostileFaction);
                me->SetDisplayId(InvisibleDisplay);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            std::list<Player*> l_ListPlayers;
            JadeCore::AnyPlayerInObjectRangeCheck check(me, 15.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, l_ListPlayers, check);
            me->VisitNearbyObject(15.0f, searcher);
            if (!l_ListPlayers.empty())
            {
                for (std::list<Player*>::const_iterator l_Itr = l_ListPlayers.begin(); l_Itr != l_ListPlayers.end(); ++l_Itr)
                {
                    if (!(*l_Itr))
                        continue;

                    if ((*l_Itr)->IsWithinDistInMap(me, 5.0f))
                    {
                        if (!(*l_Itr)->HasAura(eFelPoolSpells::SpellFelPoolDebuffDmg))
                            me->AddAura(eFelPoolSpells::SpellFelPoolDebuffDmg, (*l_Itr));
                    }
                    else
                    {
                        if ((*l_Itr)->HasAura(eFelPoolSpells::SpellFelPoolDebuffDmg, me->GetGUID()))
                            (*l_Itr)->RemoveAura(eFelPoolSpells::SpellFelPoolDebuffDmg);
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_azzakel_mob_fel_poolAI(p_Creature);
    }
};

/// Fel Spark Trigger - 326527
class auchindoun_azzakel_mob_fel_spark_trigger : public CreatureScript
{
public:

    auchindoun_azzakel_mob_fel_spark_trigger() : CreatureScript("auchindoun_azzakel_mob_fel_spark_trigger") {}

    struct auchindoun_azzakel_mob_fel_spark_triggerAI : public Scripted_NoMovementAI
    {
        auchindoun_azzakel_mob_fel_spark_triggerAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_First = true;
        }

        enum eFelSparkSpells
        {
            SpellFelSparkAreaTrigger = 153725
        };

        enum eFelSparkCreatures
        {
            CreatureFelSparkNullAI        = 326527,
            CreatureFelSparkNullAITrigger = 326528
        };

        bool m_First;

        void Reset() override
        {
            m_First = false;
            me->setFaction(HostileFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

            for (uint8 l_I = 0; l_I < 20; ++l_I)
            {
                float l_X = me->m_positionX + (l_I + 1) * cos(me->m_orientation);
                float l_Y = me->m_positionY + (l_I + 1) * sin(me->m_orientation);

                /// 326528
                if (Creature* l_FelSparkNullAITrigger = me->SummonCreature(eFelSparkCreatures::CreatureFelSparkNullAITrigger, l_X, l_Y, me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 7 * TimeConstants::IN_MILLISECONDS))
                {
                    l_FelSparkNullAITrigger->setFaction(HostileFaction);
                    l_FelSparkNullAITrigger->SetDisplayId(InvisibleDisplay);
                    l_FelSparkNullAITrigger->SetReactState(ReactStates::REACT_PASSIVE);
                    l_FelSparkNullAITrigger->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

                    l_FelSparkNullAITrigger->CastSpell(l_FelSparkNullAITrigger, eFelSparkSpells::SpellFelSparkAreaTrigger, true);
                }
            }

            me->DespawnOrUnsummon(7 * TimeConstants::IN_MILLISECONDS);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_azzakel_mob_fel_spark_triggerAI(p_Creature);
    }
};

/// Azzakael - 75927
class boss_azzakel : public CreatureScript
{
    public:

    boss_azzakel() : CreatureScript("boss_azzakel") { }

    struct boss_azzakelAI : public BossAI
    {
        boss_azzakelAI(Creature* p_Creature) : BossAI(p_Creature, eAuchindounDatas::DataBossAzzakael)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
            m_First = true;
        }

        InstanceScript* m_Instance;
        uint32 m_Interval;
        bool m_Intro;
        bool m_Argus;
        bool m_First;
        bool m_Achievement;

        void Reset() override
        {
            _Reset();
            events.Reset();
            m_Argus = false;
            m_Interval = 3 * TimeConstants::IN_MILLISECONDS;
            if (m_First)
            {
                m_First = false;
                me->setFaction(FriendlyFaction);
            }
   
            m_Instance->DoRemoveAurasDueToSpellOnPlayers(eAzzakelSpells::SpellFelPoolDebuffDmg);
            m_Instance->DoRemoveAurasDueToSpellOnPlayers(eAzzakelSpells::SpellFelSparkPerioidicCreation);
            me->SetHomePosition(g_PositionSpawningFlyCoords[1].GetPositionX(), g_PositionSpawningFlyCoords[1].GetPositionY(), g_PositionSpawningFlyCoords[1].GetPositionZ(), me->GetOrientation());
            me->GetMotionMaster()->MoveTargetedHome();

            uint32 l_Entries[5] = { eAzzakelCreatures::TriggerDemonSummoning, eAzzakelCreatures::CreatureCacklingPyromaniac, eAzzakelCreatures::CreatureBlazingTrickster, eAzzakelCreatures::CreatureFelguard, eAzzakelCreatures::CreatureBlazingTrickster02 };
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
                case eAuchindounActions::ActionDemonSoulsAchievement:
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
                    me->UpdatePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), false);              
                    events.ScheduleEvent(eAzzakelEvents::EventFelLash,           8  * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eAzzakelEvents::EventClawsOfArgus,      45 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eAzzakelEvents::EventCurtainOfFlame,    14 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eAzzakelEvents::EventMalevolentCrush01, 20 * TimeConstants::IN_MILLISECONDS);
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                        AttackStart(l_Target);
                    break;
                }
                case eAzzakelActions::ActionMalevolentCrash:
                    events.ScheduleEvent(eAzzakelEvents::EventMalevolentCrush01, 1 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            HandleDoors(me);
            summons.DespawnAll();

            uint32 l_Entries[4] = { CreatureCacklingPyromaniac, CreatureBlazingTrickster, CreatureFelguard, CreatureBlazingTrickster02 };
            for (uint32 l_Entry : l_Entries)
                DespawnCreaturesInArea(l_Entry, me);
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            _EnterCombat();
            m_Achievement = true;
            Talk(eAzzakelTalks::AzzakelAggro);

            me->SummonGameObject(eAuchindounObjects::GameobjectFelBarrier, 1911.01f, 2722.89f, 30.799f, g_PositionAzzakel_Blackgate.GetOrientation(), 0, 0, 0, 0, 0);
            events.ScheduleEvent(eAzzakelEvents::EventFelLash, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAzzakelEvents::EventClawsOfArgus, 45 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAzzakelEvents::EventCurtainOfFlame, 14 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAzzakelEvents::EventMalevolentCrush01, 20 * TimeConstants::IN_MILLISECONDS);

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eAzzakelTalks::AzzakelKill01);
                else
                    Talk(eAzzakelTalks::AzzakelKill02);
            }
        }

        void MovementInform(uint32 p_Type, uint32 p_ID) override
        {    
            if (p_ID == eAzzakelMovements::MovementAzzakelMalevolentCrash)
            {
                me->SummonCreature(eAzzakelCreatures::TriggerFelPool, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                me->CastSpell(me, eAzzakelSpells::SpellFelPoolAreatriger);

                me->SetCanFly(false);
                me->SetDisableGravity(false);
                Talk(eAzzakelTalks::AzzakelSpell02);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                if (m_Instance != nullptr)
                {
                    if (m_Instance->instance->IsHeroic())
                    {
                        float l_Range = 5.0f;
                        float l_Angle = 0.0f;
                        float l_Step = (2 * M_PI) / 4;

                        for (uint8 l_I = 0; l_I < 4; ++l_I)
                        {
                            float l_X = me->GetPositionX() + (l_Range * cos(l_Angle));
                            float l_Y = me->GetPositionY() + (l_Range * sin(l_Angle));
                            me->SummonCreature(eAzzakelCreatures::TriggerFelSpark, l_X, l_Y, me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                            l_Angle += l_Step;
                        }
                    }
                }

                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                {
                    me->GetMotionMaster()->MoveChase(l_Target, 0.0f, 0.0f);
                    me->Attack(l_Target, true);
                }
            }  
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            Talk(eAzzakelTalks::AzzakelDeath);
            uint32 l_Entries[4] = { eAzzakelCreatures::CreatureCacklingPyromaniac, eAzzakelCreatures::CreatureBlazingTrickster, eAzzakelCreatures::CreatureFelguard, eAzzakelCreatures::CreatureBlazingTrickster02 };
            for (uint32 l_Entry : l_Entries)
                DespawnCreaturesInArea(l_Entry, me);

            if (m_Instance != nullptr)
            {
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(eAzzakelSpells::SpellFelPoolDebuffDmg);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(eAzzakelSpells::SpellFelSparkPerioidicCreation);
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);;

                if (GameObject* l_Transport = m_Instance->instance->GetGameObject(m_Instance->GetData64(eAuchindounDatas::DataSoulTransport1)))
                {
                    l_Transport->SetLootState(LootState::GO_READY);
                    l_Transport->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }

                if (m_Achievement)
                {
                    if (me->GetMap() && me->GetMap()->IsHeroic())
                        m_Instance->DoCompleteAchievement(eAuchindounAchievements::AchievementDemonSouls);
                }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            if (m_Argus) 
            {
                if (m_Interval <= p_Diff)
                {
                    if (Creature* l_AzzakelController = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataTriggerAzzakelController)))
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
                            default:
                                break;
                        }
                    }

                    m_Interval = 3 * TimeConstants::IN_MILLISECONDS;
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
                        me->SetCanFly(true);
                        me->SetDisableGravity(true);
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 2.5f, true);
                        Talk(eAzzakelTalks::AzzakelSpell01);
						me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_FLYING);
                        me->CastSpell(me, eAzzakelSpells::SpellClawsOfArgusVisual);               
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
                        }

                       events.ScheduleEvent(eAzzakelEvents::EventCurtainOfFlame, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                       break;                       
                    }
                case eAzzakelEvents::EventMalevolentCrush01:
                    {
                        me->StopMoving();
                        me->AttackStop();
                        me->SetCanFly(true);
                        me->SetDisableGravity(true);
                        Talk(eAzzakelTalks::AzzakelSpell02);
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 2.5f, true);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->GetMotionMaster()->MoveJump(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), 15.0f, 20.0f, 10.0f, eAzzakelMovements::MovementAzzakelMalevolentCrash);

                        events.ScheduleEvent(eAzzakelEvents::EventMalevolentCrush01, 20 * TimeConstants::IN_MILLISECONDS);                      
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
        return new boss_azzakelAI(p_Creature);
    }
};

/// Curtain of Flames - 153392
class auchindoun_azzakel_spell_curtain_flames : public SpellScriptLoader
{
    public:

    auchindoun_azzakel_spell_curtain_flames() : SpellScriptLoader("auchindoun_azzakel_spell_curtain_flames") { }

    class auchindoun_azzakel_spell_curtain_flames_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_azzakel_spell_curtain_flames_AuraScript)

        enum eCurtainFlamesSpells
        {
            SpellCurtainOfFlameAura   = 153392,
            SpellCurtainOfFlameVisual = 153400
        };

        void OnPeriodic(AuraEffect const* p_AurEff)
        {
            if (Unit* l_Target = GetTarget())
            {
                if (l_Target->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                std::list<Player*> l_ListPlayers;
                l_Target->GetPlayerListInGrid(l_ListPlayers, 3.0f);
                if (!l_ListPlayers.empty())
                {
                    for (Player* l_Itr : l_ListPlayers)
                    {
                        if (!l_Itr)
                            continue;

                        if (GetTarget()->GetGUID() != l_Itr->GetGUID())
                        {
                            l_Target->AddAura(eCurtainFlamesSpells::SpellCurtainOfFlameAura, l_Itr);
                            l_Target->CastSpell(l_Target, eCurtainFlamesSpells::SpellCurtainOfFlameVisual);
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_azzakel_spell_curtain_flames_AuraScript::OnPeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_azzakel_spell_curtain_flames_AuraScript();
    }
};

/// Claw of Flames Loader - 153764
class auchindoun_azzakel_spell_claws_of_argus : public SpellScriptLoader
{
    public:

      auchindoun_azzakel_spell_claws_of_argus() : SpellScriptLoader("auchindoun_azzakel_spell_claws_of_argus") { }

    class auchindoun_azzakel_spell_claws_of_argus_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_azzakel_spell_claws_of_argus_AuraScript);

        enum eAzzakelEvents
        {
            EventMalevolentCrush01 = 78
        };

        void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
        {
            if (!GetCaster())
                return;

            if (InstanceScript* l_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Azzakel = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossAzzakael)))
                {
                    if (l_Azzakel->IsAIEnabled)
                    {
                      l_Azzakel->SetCanFly(true);
                      l_Azzakel->SetDisableGravity(true);
                      l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionBoolActivate);
                      l_Azzakel->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);                
                      l_Azzakel->GetMotionMaster()->MoveTakeoff(1000, l_Azzakel->GetPositionX(), l_Azzakel->GetPositionY(), 45.0f);
                    }
                }
            }
        }

        void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
        {
            if (!GetCaster())
                return;

            if (InstanceScript* l_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Azzakel = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossAzzakael)))
                {
                    if (l_Azzakel->IsAIEnabled)
                    {                  
                        l_Azzakel->RemoveAllAuras();
                        l_Azzakel->SetReactState(ReactStates::REACT_DEFENSIVE);
                        l_Azzakel->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionBoolDeactivate);
                        l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionRenewEvents);

                        l_Azzakel->SetCanFly(false);
                        l_Azzakel->SetDisableGravity(false);
                        l_Azzakel->RemoveAura(eAzzakelSpells::SpellFly);
                        l_Azzakel->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        l_Azzakel->SetSpeed(UnitMoveType::MOVE_FLIGHT, 0.3f, true);

                        if (l_Azzakel->GetMap() && l_Azzakel->GetMap()->IsHeroic())
                            l_Azzakel->GetAI()->DoAction(eAzzakelActions::ActionMalevolentCrash);
                        else
                        {
                            if (Unit* l_Target = GetCaster()->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                            {
                                GetCaster()->GetMotionMaster()->MoveChase(l_Target, 0.0f, 0.0f);
                                GetCaster()->Attack(l_Target, true);
                            }
                        }
                    }
                }
            }
        }

        void Register()
        {
            AfterEffectApply  += AuraEffectApplyFn(auchindoun_azzakel_spell_claws_of_argus_AuraScript::OnApply, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(auchindoun_azzakel_spell_claws_of_argus_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_azzakel_spell_claws_of_argus_AuraScript();
    }
};

/// Fel Spark - 153725
class auchindoun_azzakel_at_fel_spark : public AreaTriggerEntityScript
{
    public:

    auchindoun_azzakel_at_fel_spark() : AreaTriggerEntityScript("auchindoun_azzakel_at_fel_spark") {}

    enum eFelSparkSpells
    {
        SpellFelSparkDebuffDmg = 153726
    };

    uint32 m_Diff = 1 * TimeConstants::IN_MILLISECONDS;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Diff <= p_Time)
        {
            std::list<Player*> l_ListPlayers;
            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_ListPlayers, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);
            if (!l_ListPlayers.empty())
            {
                for (std::list<Player*>::const_iterator l_Itr = l_ListPlayers.begin(); l_Itr != l_ListPlayers.end(); ++l_Itr)
                {
                    if (!(*l_Itr))
                        continue;

                    if (!(*l_Itr)->HasAura(eFelSparkSpells::SpellFelSparkDebuffDmg))
                        (*l_Itr)->AddAura(eFelSparkSpells::SpellFelSparkDebuffDmg, (*l_Itr));
                }
            }

            m_Diff = 1 * TimeConstants::IN_MILLISECONDS;
        }
        else
            m_Diff -= p_Time;
    }

    auchindoun_azzakel_at_fel_spark* GetAI() const override
    {
        return new auchindoun_azzakel_at_fel_spark();
    }
};

void AddSC_boss_azzakel()
{
   new boss_azzakel();                             ///< 75927
   new auchindoun_azzakel_mob_controller();        ///< 76216
   new auchindoun_azzakel_mob_fel_spark_trigger(); ///< 326527
   new auchindoun_azzakel_mob_fel_pool();          ///< 326526
   new auchindoun_azzakel_spell_curtain_flames();  ///< 153392
   new auchindoun_azzakel_spell_claws_of_argus();  ///< 153764
   new auchindoun_azzakel_at_fel_spark();          ///< 153725
}