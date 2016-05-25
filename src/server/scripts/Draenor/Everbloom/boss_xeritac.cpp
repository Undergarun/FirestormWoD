
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eXeritacCreatures
{
    CreatureGas         = 213152,
    CreatureDescend     = 53467
};

enum eXeritacActions
{
    ActionCountSpiderling = 1,
    ActionInhale,
    ActionCount,
    ActionConsumeEffect,
    ActionXeritacRandomTopMovements
};

enum eXeritacMovementInformed
{
    MovementInformedSpidersReachGround = 1,
    MovementInformedXeritacReachedGround,
    MovementInformedXeritacReachedTopWaypoint,
    MovementInformedXeritacMovementHappenedTopWaypoint
};

Position const g_PositionHomePoint   = { 923.86f, 1455.56f, 90.867f, 5.414306f };

Position const g_PositionMotionStart = { 934.475f, 1430.094f, 85.754f };

Position const g_PositionRandomMovements[5] =
{
    { 931.20f, 1441.587f, 85.600f, 3.289805f },
    { 908.84f, 1449.853f, 85.600f, 0.615524f },
    { 921.09f, 1456.374f, 85.600f, 5.465359f },
    { 938.89f, 1443.991f, 85.600f, 3.359706f },
    { 930.02f, 1434.803f, 85.600f, 2.946586f }
};

Position const g_PositionitionPaleOne[2] =
{
    { 926.44f, 1413.111f, 66.364f, 2.336323f },
    { 956.79f, 1448.274f, 64.274f, 3.184339f }
};

#define DescendBeamAlttitude 63.7426f
#define CielingAlttitude 89.008f

/// Xeri'tac - 84550
class boss_xeritac : public CreatureScript
{
public:
    boss_xeritac() : CreatureScript("boss_xeritac") { }

    struct boss_xeritacAI : public BossAI
    {
        boss_xeritacAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataXeritac)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
            m_First = false;
        }

        enum eXeritacSpells
        {
            /// Descend
            SpellDecsendDummy            = 172643,
            SpellDecsendBeam             = 169322,
            SpellDescendEffectDamage     = 169275,
            SpellDescendDamage           = 169278,
            SpellWebCrawl                = 169293,
            SpellGasVolleyDummy          = 169382,
            SpellGasVolleyMissile        = 169383,
            SpellToxicGasDamage          = 169223,
            SpellToxicGasAreaTrigger     = 169224,
            SpellToxicGas                = 169218,
            SpellVenoumousSting          = 169376,
            SpellSwipeDamage             = 169371,
            SpellConsume                 = 169248,
            SpellConsumeAura             = 169249,
            SpellVenomSpray              = 172727,
            SpellToxicBlood              = 169218,
            SpellToxicBolt               = 169375,
            SpellToxicBloodExplode       = 169267,
            SpellToxicitiy               = 169219,       
            SpellFixate                  = 173080,
            SpellBurst                   = 173081,
            SpellInhaleDummy             = 169233
        };

        enum eXeritacEvents
        {
            EventConsume = 1,
            EventDescend,
            EventDecsendStartMoving,
            EventGasVolley,
            EventToxicGas,
            EventToxicBolt,
            EventToxicAura,
            EventVenomousString,
            EventWebCrawl,
            EventToxicSpiderling,
            EventVenomCrazedPaleOne,
            EventGorgendBusters,
            EventVenomSprayers,
            EventExplode,
            EventSwipe,
            EventCancelSwipe,
            EventInhale,
            EventMove
        };

        InstanceScript* m_Instance;
        uint32 m_TimeBetween;
        uint8  m_Count;
        uint32 m_Phase;
        uint64 m_ConsumedGuid;
        bool m_Intro;
        bool m_Descend;
        bool m_First;
        bool m_Consuming;

        void Reset() override
        {         
            events.Reset();    
            summons.DespawnAll();
            m_Consuming = false;
            m_Descend = false;
            m_Count = 0;
            m_Phase = 1;
            m_ConsumedGuid = 0;
            m_TimeBetween = 6 * TimeConstants::IN_MILLISECONDS;   
            me->setFaction(HostileFaction);       
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            if (!m_First)
            {
                m_First = true;
                OpenEncounterDoor();
            }
        }

        void OpenEncounterDoor()
        {
            if (m_Instance != nullptr)
            {
                // Get Barrier Down
                if (GameObject* l_OpenEncounterDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(eEverbloomData::DataObjectWebDoor)))
                {
                    // Activate
                    l_OpenEncounterDoor->SetLootState(LootState::GO_READY);
                    l_OpenEncounterDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }
            }
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro)
                m_Intro = true;

            if (p_Who && p_Who->IsInWorld() && p_Who->GetEntry() == eEverbloomCreature::CreatureVenomCrazedPaleOne && p_Who->IsWithinDistInMap(me, 3.0f, true) && !m_Consuming && p_Who->isAlive())
            {
                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                m_Consuming = true;
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            me->GetMotionMaster()->MovePoint(0, 935.628f, 1430.930f, 64.988f);    
            std::list<AreaTrigger*> l_GasAreatriggerList;
            me->GetAreaTriggerList(l_GasAreatriggerList, eXeritacSpells::SpellToxicGasAreaTrigger);
            if (l_GasAreatriggerList.empty())
                return;

            for (AreaTrigger* l_Itr : l_GasAreatriggerList)
            {
                l_Itr->Remove(1 * TimeConstants::IN_MILLISECONDS);
            }
            if (m_First)
                OpenEncounterDoor();
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            _EnterCombat();        
            if (m_Instance != nullptr)
            {
                if (me->GetMap() && me->GetMap()->IsHeroic())
                    events.ScheduleEvent(eXeritacEvents::EventGorgendBusters, 10 * TimeConstants::IN_MILLISECONDS);

                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
            }

            m_Phase = 1;      
            OpenEncounterDoor();
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            events.ScheduleEvent(eXeritacEvents::EventDescend, 20 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eXeritacEvents::EventToxicSpiderling, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eXeritacEvents::EventVenomSprayers, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eXeritacEvents::EventVenomCrazedPaleOne, 20 * TimeConstants::IN_MILLISECONDS);
            me->CastSpell(me->GetPositionX(), me->GetPositionY(), CielingAlttitude, eXeritacSpells::SpellDecsendBeam, false);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            me->GetMotionMaster()->MoveCharge(me->GetPositionX(), me->GetPositionY(), CielingAlttitude, 30.0f, eXeritacMovementInformed::MovementInformedXeritacReachedTopWaypoint);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {          
                case eXeritacMovementInformed::MovementInformedXeritacReachedTopWaypoint:
                    DoAction(eXeritacActions::ActionXeritacRandomTopMovements);
                    break;
                case eXeritacMovementInformed::MovementInformedXeritacMovementHappenedTopWaypoint:
                    DoAction(eXeritacActions::ActionXeritacRandomTopMovements);
                    break;
                default:
                    break;
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            OpenEncounterDoor();
            summons.DespawnAll();           
            if (m_Instance != nullptr)
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

            std::list<AreaTrigger*> l_GasAreatriggerList;
            me->GetAreaTriggerList(l_GasAreatriggerList, eXeritacSpells::SpellToxicGasAreaTrigger);
            if (l_GasAreatriggerList.empty())
                return;

            for (AreaTrigger* l_Itr : l_GasAreatriggerList)
            {
                l_Itr->Remove(1 * TimeConstants::IN_MILLISECONDS);
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eEverbloomActions::ActionCounting:
                {
                    m_Count++;
                    if (m_Count >= 4 && m_Phase == 1)
                    {
                        m_Phase = 2;
                        events.Reset();
                        m_Descend = true;
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        if (Unit* l_Victim = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                        {
                            me->GetMotionMaster()->MoveChase(l_Victim);
                            me->Attack(l_Victim, true);
                        }
                        events.ScheduleEvent(eXeritacEvents::EventToxicBolt, urand(7 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        events.ScheduleEvent(eXeritacEvents::EventVenomousString, 16 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eXeritacEvents::EventGasVolley, 30 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eXeritacEvents::EventVenomCrazedPaleOne, 20 * TimeConstants::IN_MILLISECONDS);
                        me->MonsterTextEmote("Xeri'tac descends from her web!", Language::LANG_UNIVERSAL, me->GetGUID());
                    }
                    break;
                }
                case eXeritacActions::ActionConsumeEffect:
                    if (m_ConsumedGuid)
                    {
                        if (Creature* l_ConsumedPaleOne = Creature::GetCreature(*me, m_ConsumedGuid))
                        {
                            m_Consuming = false;
                            m_ConsumedGuid = 0;
                            l_ConsumedPaleOne->Kill(l_ConsumedPaleOne);
                            l_ConsumedPaleOne->DespawnOrUnsummon();                         
                        }
                    }
                    break;
                case eXeritacActions::ActionXeritacRandomTopMovements:
                {    
                        std::list<Position> l_Position;
                        for (uint8 l_I = 0; l_I < 5; l_I++)
                            l_Position.push_back(g_PositionRandomMovements[l_I]);

                        if (l_Position.empty())
                            return;

                        std::list<Position>::const_iterator l_It = l_Position.begin();
                        std::advance(l_It, urand(0, l_Position.size() - 1));
                        me->GetMotionMaster()->MovePoint(eXeritacMovementInformed::MovementInformedXeritacMovementHappenedTopWaypoint, *l_It);
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

            // Consume
            if (m_ConsumedGuid)
            {
                if (Creature* l_ConsumedPaleOne = sObjectAccessor->GetCreature(*me, m_ConsumedGuid))
                {
                    if (l_ConsumedPaleOne->isDead() && l_ConsumedPaleOne->IsInWorld())
                    {
                        me->CastStop();
                        m_Consuming = false;
                    }
                }
            }
        
            switch (events.ExecuteEvent())
            {
                case eXeritacEvents::EventDescend:
                {
                    me->StopMoving();
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MovePoint(eXeritacMovementInformed::MovementInformedXeritacReachedGround, me->GetPositionX(), me->GetPositionY(), 64.0f);
                    if (Creature* l_Creature = me->SummonCreature(eXeritacCreatures::CreatureDescend, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 64.631f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS))
                        me->CastSpell(l_Creature, eXeritacSpells::SpellDecsendDummy);
                    events.ScheduleEvent(eXeritacEvents::EventDescend, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eXeritacEvents::EventToxicBolt:
                {
                    if (Unit* l_Victim = me->getVictim())
                        me->CastSpell(l_Victim, eXeritacSpells::SpellToxicBolt);
                    events.ScheduleEvent(eXeritacEvents::EventToxicBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eXeritacEvents::EventVenomousString:
                {
                    if (Unit* l_Victim = me->getVictim())
                        me->CastSpell(l_Victim, eXeritacSpells::SpellVenoumousSting);
                        events.ScheduleEvent(eXeritacEvents::EventVenomousString, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                }
                case eXeritacEvents::EventGasVolley:
                {
                    me->CastSpell(me, eXeritacSpells::SpellGasVolleyDummy);
                    events.ScheduleEvent(eXeritacEvents::EventGasVolley, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eXeritacEvents::EventGorgendBusters:
                {
                    std::list<Position> l_Position;
                    for (uint8 l_I = 0; l_I < 5; l_I++)
                        l_Position.push_back(g_PositionRandomMovements[l_I]);

                    if (l_Position.empty())
                        return;

                    std::list<Position>::const_iterator l_It = l_Position.begin();
                    std::advance(l_It, urand(0, l_Position.size() - 1));
                    me->SummonCreature(eEverbloomCreature::CreatureGorgendBusters, *l_It, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    events.ScheduleEvent(eXeritacEvents::EventGorgendBusters, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eXeritacEvents::EventToxicSpiderling:
                {
                    std::list<Position> l_Position;
                    for (uint8 l_I = 0; l_I < 5; l_I++)
                        l_Position.push_back(g_PositionRandomMovements[l_I]);

                    if (l_Position.empty())
                        return;

                    std::list<Position>::const_iterator l_It = l_Position.begin();
                    std::advance(l_It, urand(0, l_Position.size() - 1));
                    me->SummonCreature(eEverbloomCreature::CreatureToxicSpiderling, *l_It, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    me->MonsterTextEmote("|TInterface\\Icons\\achievement_halloween_rottenegg_01.blp:20|tXeri'tac begins unleashing Toxic Spiderlings down on you!", me->GetGUID());
                    events.ScheduleEvent(eXeritacEvents::EventToxicSpiderling, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eXeritacEvents::EventVenomSprayers:
                {
                    std::list<Position> l_Position;
                    for (uint8 l_I = 0; l_I < 5; l_I++)
                        l_Position.push_back(g_PositionRandomMovements[l_I]);

                    if (l_Position.empty())
                        return;

                    std::list<Position>::const_iterator l_It = l_Position.begin();
                    std::advance(l_It, urand(0, l_Position.size() - 1));
                    me->SummonCreature(eEverbloomCreature::CreatureVenomSprayer, *l_It, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    events.ScheduleEvent(eXeritacEvents::EventVenomSprayers, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eXeritacEvents::EventVenomCrazedPaleOne:
                {
                    if (roll_chance_i(50)) /// Right or Left spawning
                        me->SummonCreature(eEverbloomCreature::CreatureVenomCrazedPaleOne, g_PositionitionPaleOne[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    else
                        me->SummonCreature(eEverbloomCreature::CreatureVenomCrazedPaleOne, g_PositionitionPaleOne[1], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    events.ScheduleEvent(eXeritacEvents::EventVenomCrazedPaleOne, 35 * TimeConstants::IN_MILLISECONDS);
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
        return new boss_xeritacAI(p_Creature);
    }
};

/// Venom Sprayer - 86547
class the_everbloom_xeritac_mob_venom_sprayer : public CreatureScript
{
public:

    the_everbloom_xeritac_mob_venom_sprayer() : CreatureScript("the_everbloom_xeritac_mob_venom_sprayer") { }

    struct the_everbloom_xeritac_mob_venom_sprayerAI : public ScriptedAI
    {
        the_everbloom_xeritac_mob_venom_sprayerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
             m_Instance = me->GetInstanceScript();
        }

        enum eVenomSprayerEvents
        {
            EventVenomSprayers    = 1
        };

        enum eVenomSprayerSpells
        {
            SpellDecsendBeam            = 169322,
            SpellDescendBeamVisual      = 169326,
            SpellVenomSpray             = 172727
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            DoZoneInCombat();
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->GetMotionMaster()->MoveTakeoff(eXeritacMovementInformed::MovementInformedSpidersReachGround, me->GetPositionX(), me->GetPositionY(), DescendBeamAlttitude);
            me->CastSpell(me, eVenomSprayerSpells::SpellDecsendBeam);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eXeritacMovementInformed::MovementInformedSpidersReachGround:
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveAura(eVenomSprayerSpells::SpellDescendBeamVisual);
                    DoZoneInCombat();
                    break;
                default:
                    break;
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
        }

        void EnterCombat(Unit* p_Attacker) override
        {         
            events.ScheduleEvent(eVenomSprayerEvents::EventVenomSprayers, urand(7 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eVenomSprayerEvents::EventVenomSprayers:
                        me->CastSpell(me, eVenomSprayerSpells::SpellVenomSpray);
                        events.ScheduleEvent(eVenomSprayerEvents::EventVenomSprayers, urand(7 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_xeritac_mob_venom_sprayerAI(p_Creature);
    }
};

/// Crazed Pale One - 84554
class the_everbloom_xeritac_mob_crazed_pale_one : public CreatureScript
{
public:

    the_everbloom_xeritac_mob_crazed_pale_one() : CreatureScript("the_everbloom_xeritac_mob_crazed_pale_one") { }

    struct the_everbloom_xeritac_mob_crazed_pale_oneAI : public ScriptedAI
    {
        the_everbloom_xeritac_mob_crazed_pale_oneAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eCrazedPaleOrcEvents
        {
            EventSwipe = 1,
            EventCancelSwipe
        };

        enum eCrazedPaleOrcSpells
        {
            SpellToxicGas    = 169218,
            SpellInhaleDummy = 169233,
            SpellSwipe       = 169371
        };

        InstanceScript* m_Instance;
        bool m_Transformed;

        void Reset() override
        {
            events.Reset();
            DoZoneInCombat();
            m_Transformed = false;
            me->setFaction(HostileFaction);
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.6f, true);
            if (Player* l_Target = me->FindNearestPlayer(50.0f, true))
                me->Attack(l_Target, true);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eCrazedPaleOrcEvents::EventSwipe, 2 * TimeConstants::IN_MILLISECONDS);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eXeritacActions::ActionInhale:
                    me->CastSpell(me, eCrazedPaleOrcSpells::SpellInhaleDummy);
                    break;
                default:
                    break;
            }
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

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (me->FindNearestCreature(eXeritacCreatures::CreatureGas, 1.5f) && !m_Transformed)
            {
                m_Transformed = true;
                me->CastSpell(me, eCrazedPaleOrcSpells::SpellToxicGas);
            }
    
            switch (events.ExecuteEvent())
            {
                case eCrazedPaleOrcEvents::EventSwipe:
                        me->CastSpell(me, eCrazedPaleOrcSpells::SpellSwipe); // swipe
                        events.ScheduleEvent(eCrazedPaleOrcEvents::EventSwipe, 3 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eCrazedPaleOrcEvents::EventCancelSwipe, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                case eCrazedPaleOrcEvents::EventCancelSwipe:
                    events.CancelEvent(eCrazedPaleOrcEvents::EventSwipe);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_xeritac_mob_crazed_pale_oneAI(p_Creature);
    }
};

/// Toxic Spiderling - 84552
class the_everbloom_xeritac_mob_toxic_spiderling : public CreatureScript
{
public:

    the_everbloom_xeritac_mob_toxic_spiderling() : CreatureScript("the_everbloom_xeritac_mob_toxic_spiderling") { }

    struct the_everbloom_xeritac_mob_toxic_spiderlingAI : public ScriptedAI
    {
        the_everbloom_xeritac_mob_toxic_spiderlingAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eToxicSpiderlingEvents
        {
            EventToxicBolt = 1
        };

        enum eToxicSpiderlingsSpells
        {
            SpellToxicBlood          = 169218,
            SpellToxicBolt           = 169375,
            SpellToxicBloodExplode   = 169267,
            SpellDecsendBeam         = 169322,
            SpellToxicity            = 169219,
            SpellDescendBeamSpell    = 169326,
            SpellToxicGasAreaTrigger = 169224
        };

        InstanceScript* m_Instance;
        bool m_Explode;

        void Reset() override
        {
            events.Reset();
            m_Explode = false;
            me->RemoveAllAuras();
            me->SetDisableGravity(true);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->GetMotionMaster()->MoveTakeoff(eXeritacMovementInformed::MovementInformedSpidersReachGround, me->GetPositionX(), me->GetPositionY(), DescendBeamAlttitude);
            me->CastSpell(me, eToxicSpiderlingsSpells::SpellDecsendBeam);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eXeritacMovementInformed::MovementInformedSpidersReachGround:
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveAura(eToxicSpiderlingsSpells::SpellDescendBeamSpell);
                    DoZoneInCombat();
                    break;
                default:
                    break;
            }
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->CastSpell(me, eToxicSpiderlingsSpells::SpellToxicBlood);
            events.ScheduleEvent(eToxicSpiderlingEvents::EventToxicBolt, 8 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);

            if (m_Instance != nullptr)
            {
                if (Creature* l_Xeritac = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataXeritac)))
                {
                    if (l_Xeritac->IsAIEnabled)
                        l_Xeritac->GetAI()->DoAction(eEverbloomActions::ActionCounting);
                    me->SummonCreature(eXeritacCreatures::CreatureGas, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                }
            }
        }

        void JustSummoned(Creature* p_Summon) override
        {
            if (p_Summon != nullptr)
            {
                switch (p_Summon->GetEntry())
                {
                    case eXeritacCreatures::CreatureGas:
                    {
                        p_Summon->CastSpell(p_Summon, eToxicSpiderlingsSpells::SpellToxicGasAreaTrigger);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (me->HasAura(eToxicSpiderlingsSpells::SpellToxicity))
            {
                if (me->GetAura(eToxicSpiderlingsSpells::SpellToxicity) && me->GetAura(eToxicSpiderlingsSpells::SpellToxicity)->GetStackAmount() >= 10 && !m_Explode)
                {
                    m_Explode = true;
                    me->RemoveAllAuras();
                    me->CastSpell(me, eToxicSpiderlingsSpells::SpellToxicBloodExplode);
                }
            }
     
            switch (events.ExecuteEvent())
            {
                case eToxicSpiderlingEvents::EventToxicBolt:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eToxicSpiderlingsSpells::SpellToxicBolt);
                    events.ScheduleEvent(eToxicSpiderlingEvents::EventToxicBolt, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_xeritac_mob_toxic_spiderlingAI(p_Creature);
    }
};

/// Gorged Bursters - 86552
class the_everbloom_xeritac_mob_gorged_bruster : public CreatureScript
{
public:

    the_everbloom_xeritac_mob_gorged_bruster() : CreatureScript("the_everbloom_xeritac_mob_gorged_bruster") { }

    struct the_everbloom_xeritac_mob_gorged_brusterAI : public ScriptedAI
    {
        the_everbloom_xeritac_mob_gorged_brusterAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eGorgedBustersSpells
        {
            SpellFixate           = 173080,
            SpellBurst            = 173081,
            SpellDecsendBeam      = 169322,
            SpellDescendBeamSpell = 169326
        };

        InstanceScript* m_Instance;
        uint64 m_Target = 0;
        bool m_Transformed;
        bool m_Explosion;

        void Reset() override
        {
            events.Reset();
            m_Transformed = false;
            m_Explosion = false;      
            me->SetDisableGravity(true);          
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);  
            me->CastSpell(me, eGorgedBustersSpells::SpellDecsendBeam);
            me->GetMotionMaster()->MoveTakeoff(eXeritacMovementInformed::MovementInformedSpidersReachGround, me->GetPositionX(), me->GetPositionY(), DescendBeamAlttitude);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eXeritacMovementInformed::MovementInformedSpidersReachGround:
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveAura(eGorgedBustersSpells::SpellDescendBeamSpell);
                    me->RemoveAura(eGorgedBustersSpells::SpellDecsendBeam);
                    DoZoneInCombat();
                    BeginFixation();
                    break;
                default:
                    break;
            }
        }

        void BeginFixation()
        {
            if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
            {
                me->GetMotionMaster()->MoveChase(l_Random);
                me->AddAura(eGorgedBustersSpells::SpellFixate, l_Random);
                m_Target = l_Random->GetGUID();
            }
        }

        void OnAddThreat(Unit* p_Victim, float& p_FThreat, SpellSchoolMask /*p_SchoolMask*/, SpellInfo const* /*p_ThreatSpell*/) override
        {
            if (m_Target)
            {
                if (p_Victim->GetGUID() != m_Target)
                    p_FThreat = 0;
                return;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.3f, true);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                return;

            if (!m_Explosion)
            {
                if (m_Target)
                {
                    // Burst hardcoded
                    if (Player* l_Player = sObjectAccessor->GetPlayer(*me, m_Target))
                    {
                        if (!l_Player->HasAura(eGorgedBustersSpells::SpellFixate)) /// Fixated aura
                            me->AddAura(eGorgedBustersSpells::SpellFixate, l_Player);

                        if (!me->isMoving())
                            me->GetMotionMaster()->MoveFollow(l_Player, 0, 0, MovementSlot::MOTION_SLOT_ACTIVE);

                        if (l_Player->IsWithinDistInMap(me, 1.0f))
                        {
                            m_Explosion = true;
                            me->CastSpell(l_Player, eGorgedBustersSpells::SpellBurst);
                            me->DespawnOrUnsummon(3 * TimeConstants::IN_MILLISECONDS);
                        }
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_xeritac_mob_gorged_brusterAI(p_Creature);
    }
};

/// Gaseous Volley - 169382 
class the_everbloom_xeritac_spell_gaseous_volley : public SpellScriptLoader
{
public:

    the_everbloom_xeritac_spell_gaseous_volley() : SpellScriptLoader("the_everbloom_xeritac_spell_gaseous_volley") { }

    class the_everbloom_xeritac_spell_gaseous_volley_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_xeritac_spell_gaseous_volley_SpellScript);

        enum eGasouesVolleySpells
        {
            SpellGasVolleyDummy   = 169382,
            SpellGasVolleyMissile = 169383
        };

        void HandleDummy(SpellEffIndex /*p_EffIndex*/)
        {
            if (!GetCaster())
                return;

            std::list<Player*> l_GaseousVolleyList;
            GetCaster()->GetPlayerListInGrid(l_GaseousVolleyList, 200.0f);

            if (l_GaseousVolleyList.empty())
                return;

            std::list<Player*>::const_iterator l_It = l_GaseousVolleyList.begin();
            std::advance(l_It, urand(0, (l_GaseousVolleyList.size() - 2) ? l_GaseousVolleyList.size() > 1 : 1));
            GetCaster()->CastSpell((*l_It), eGasouesVolleySpells::SpellGasVolleyMissile, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(the_everbloom_xeritac_spell_gaseous_volley_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_xeritac_spell_gaseous_volley_SpellScript();
    }
};

/// Toxic Gas Areatrigger - 169224 
class the_everbloom_xeritac_areatrigger_toxic_gas : public AreaTriggerEntityScript
{
public:

    the_everbloom_xeritac_areatrigger_toxic_gas() : AreaTriggerEntityScript("the_everbloom_xeritac_areatrigger_toxic_gas")
    {
    }

    uint32 m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
    uint32 m_DiffInhale = 6 * TimeConstants::IN_MILLISECONDS;

    enum eToxicGasSpells
    {
        SpellToxicGasDamage = 169223,
        SpellToxicity       = 169219
    };

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        /// Gas Damage
        if (m_Diff <= p_Time)
        {
            std::list<Player*> l_ListTargets;
            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 1.5f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_ListTargets, check);
            p_AreaTrigger->VisitNearbyObject(1.5f, searcher);

            if (l_ListTargets.empty())
                return;

            for (std::list<Player*>::const_iterator l_It = l_ListTargets.begin(); l_It != l_ListTargets.end(); ++l_It)
            {
                if (!(*l_It)->HasAura(eToxicGasSpells::SpellToxicGasDamage))
                    (*l_It)->AddAura(eToxicGasSpells::SpellToxicGasDamage, (*l_It));
            }

            m_Diff = 2 * TimeConstants::IN_MILLISECONDS;;
        }
        else
            m_Diff -= p_Time;

        /// Inhale
        if (m_DiffInhale <= p_Time)
        {
            if (Creature* l_PaleOne = p_AreaTrigger->FindNearestCreature(eEverbloomCreature::CreatureVenomCrazedPaleOne, 2.0f, true))
            {
                if (Aura* l_Aura = l_PaleOne->GetAura(eToxicGasSpells::SpellToxicity))
                {
                    if (l_Aura->GetStackAmount() > 10)
                        return;

                    if (l_PaleOne->IsAIEnabled)
                        l_PaleOne->GetAI()->DoAction(eXeritacActions::ActionInhale);
                }
            }

            m_DiffInhale = 6 * TimeConstants::IN_MILLISECONDS;
        }
        else
            m_DiffInhale -= p_Time;
    }

    the_everbloom_xeritac_areatrigger_toxic_gas* GetAI() const override
    {
        return new the_everbloom_xeritac_areatrigger_toxic_gas();
    }
};

class the_everbloom_remove_non_players
{
public:
    the_everbloom_remove_non_players() { }

    bool operator()(WorldObject* p_Object)
    {
        if (p_Object->GetTypeId() == TypeID::TYPEID_PLAYER)
            return false;
        else
            return true;
    }
};

/// Descend - 169278  
class the_everbloom_xeritac_spell_descend : public SpellScriptLoader
{
public:
    the_everbloom_xeritac_spell_descend() : SpellScriptLoader("the_everbloom_xeritac_spell_descend") { }

    enum eDescendSpells
    {
        SpellDecsendBeam       = 169322,
        SpellDescendBeamVisual = 169326
    };

    class the_everbloom_xeritac_spell_descend_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_xeritac_spell_descend_SpellScript);

        
        void HandlCastSpell()
        {
            if (Unit* l_Caster = GetCaster())
            {
                GetCaster()->CastSpell(GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), 87.611f, eDescendSpells::SpellDescendBeamVisual, false);
                GetCaster()->GetMotionMaster()->MoveCharge(GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), 87.611f, 42.0f, eXeritacMovementInformed::MovementInformedXeritacReachedTopWaypoint);
            }
        }       

        void CorrectTargets(std::list<WorldObject*>& p_Targets)
        {
            if (!GetCaster())
                return;

            if (p_Targets.empty())
                return;

            p_Targets.remove_if(the_everbloom_remove_non_players());
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_xeritac_spell_descend_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_SRC_CASTER);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_xeritac_spell_descend_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_xeritac_spell_descend_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_1, Targets::TARGET_SRC_CASTER);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_xeritac_spell_descend_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_1, Targets::TARGET_UNIT_SRC_AREA_ENTRY);
            OnCast += SpellCastFn(the_everbloom_xeritac_spell_descend_SpellScript::HandlCastSpell);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_xeritac_spell_descend_SpellScript();
    }
};

/// Xeritac eggs - 234113
class the_everbloom_xertiac_gameobject_eggs : public GameObjectScript
{
public:
    the_everbloom_xertiac_gameobject_eggs() : GameObjectScript("the_everbloom_xertiac_gameobject_eggs") {}

    bool OnGossipHello(Player* p_Player, GameObject* p_Gobject)
    {
        p_Gobject->SummonCreature(eEverbloomCreature::CreatureToxicSpiderling, *p_Gobject, TEMPSUMMON_DEAD_DESPAWN);
        p_Gobject->Delete();
        return true;
    }
};

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "Player.h"
#include "WorldSession.h"
#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "ScriptedGossip.h"
#include "SpellMgr.h"
#include "Spell.h"
#include "SpellScript.h"
#include "Item.h"
#include "Bag.h"
/*
CREATE TABLE `tempcharacter_action` (
`guid` INT(11) NULL DEFAULT NULL,
`spec` INT(11) NULL DEFAULT NULL,
`buttom` INT(11) NULL DEFAULT NULL,
`action` INT(11) NULL DEFAULT NULL,
`type` INT(11) NULL DEFAULT NULL
)
COLLATE = 'latin1_swedish_ci'
ENGINE = InnoDB;

CREATE TABLE `tempmobacharacters` (
`ID` INT(11) NULL DEFAULT NULL,
`Name` INT(11) NULL DEFAULT NULL,
`Display` INT(11) NULL DEFAULT NULL,
`Health` INT(11) NULL DEFAULT NULL,
`Power1` INT(11) NULL DEFAULT NULL COMMENT 'Mana',
`Power2` INT(11) NULL DEFAULT NULL COMMENT 'Rage',
`Power3` INT(11) NULL DEFAULT NULL COMMENT 'Energy',
`Strength` INT(11) NULL DEFAULT NULL,
`Agility` INT(11) NULL DEFAULT NULL,
`Intellect` INT(11) NULL DEFAULT NULL,
`Stamina` INT(11) NULL DEFAULT NULL,
`Spirit` INT(11) NULL DEFAULT NULL,
`Spell01` INT(11) NULL DEFAULT NULL,
`Spell02` INT(11) NULL DEFAULT NULL,
`Spell03` INT(11) NULL DEFAULT NULL,
`Spell04` INT(11) NULL DEFAULT NULL,
`Spell05` INT(11) NULL DEFAULT NULL,
`Spell06` INT(11) NULL DEFAULT NULL COMMENT 'Ultimate',
`Spell07` INT(11) NULL DEFAULT NULL COMMENT 'Talent (trait)',
`Head` INT(11) NULL DEFAULT NULL,
`Neck` INT(11) NULL DEFAULT NULL,
`Shoulder` INT(11) NULL DEFAULT NULL,
`Shirt` INT(11) NULL DEFAULT NULL,
`Chest` INT(11) NULL DEFAULT NULL,
`Waist` INT(11) NULL DEFAULT NULL,
`Legs` INT(11) NULL DEFAULT NULL,
`Feet` INT(11) NULL DEFAULT NULL,
`Wrist` INT(11) NULL DEFAULT NULL,
`Hands` INT(11) NULL DEFAULT NULL,
`Finger1` INT(11) NULL DEFAULT NULL,
`Finger2` INT(11) NULL DEFAULT NULL,
`Trinket1` INT(11) NULL DEFAULT NULL,
`Trinket2` INT(11) NULL DEFAULT NULL,
`Back` INT(11) NULL DEFAULT NULL,
`Main Hand` INT(11) NULL DEFAULT NULL,
`Off Hand` INT(11) NULL DEFAULT NULL,
`Ranged` INT(11) NULL DEFAULT NULL,
`Tabard` INT(11) NULL DEFAULT NULL,
`First Bag` INT(11) NULL DEFAULT NULL,
`Second Bag` INT(11) NULL DEFAULT NULL,
`Third Bag` INT(11) NULL DEFAULT NULL,
`Fourth Bag` INT(11) NULL DEFAULT NULL
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB;

*/

enum eActions
{
    ACTION_CHARACTER_CHOICE = 1,
};


static void ActionCharacterUnChosel(Player* p_Player, uint32 l_MobaCharacterEntry)
{
    if (p_Player)
    {
        // Next, load any items that were saved
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TREFFLE_TEMP_INVENTORY);
        stmt->setUInt32(0, p_Player->GetGUID());
        PreparedQueryResult l_CheckCastResult = CharacterDatabase.Query(stmt);

        if (l_CheckCastResult)
        {
            do
            {
                Field* field = l_CheckCastResult->Fetch();
                uint32 l_Id = field[0].GetInt32();
                uint32 l_Bag = field[1].GetInt32();
                uint32 l_Slot = field[2].GetInt32();
                uint32 l_Item = field[3].GetInt32();

                PreparedStatement* stmt_2 = CharacterDatabase.GetPreparedStatement(CHAR_INS_TREFFLE_INVENTORY);

                int index = 0;

                p_Player->AddItem(l_Item, 1);
                p_Player->DeMorph();



                CharacterDatabase.Execute(stmt);
            } while (l_CheckCastResult->NextRow());
        }
    }
}

static void ActionCharacterChosel(Player* p_Player, uint32 l_MobaCharacterEntry)
{
    if (p_Player)
    {
        // Next, load any items that were saved
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TREFFLE_MOBA_CHARACTER_SELECTION);
        stmt->setUInt32(0, l_MobaCharacterEntry);
        PreparedQueryResult l_CheckCastResult = CharacterDatabase.Query(stmt);

        if (l_CheckCastResult)
        {
            do
            {
                Field* field = l_CheckCastResult->Fetch();
                uint32 l_Id = field[0].GetInt32();
                const std::string l_Name = field[1].GetString();
                uint32 l_Display = field[2].GetInt32();
                uint32 l_Health = field[3].GetInt32();
                uint32 l_Power1 = field[4].GetInt32();
                uint32 l_Power2 = field[5].GetInt32();
                uint32 l_Power3 = field[6].GetInt32();
                uint32 l_Strength = field[7].GetInt32();
                uint32 l_Agillity = field[8].GetInt32();
                uint32 l_Stamina = field[9].GetInt32();
                uint32 l_Intellect = field[10].GetInt32();
                uint32 l_Spirit = field[11].GetInt32();
                uint32 l_Spell01 = field[12].GetInt32();
                uint32 l_Spell02 = field[13].GetInt32();
                uint32 l_Spell03 = field[14].GetInt32();
                uint32 l_Spell04 = field[15].GetInt32();
                uint32 l_Spell05 = field[16].GetInt32();
                uint32 l_Spell06 = field[17].GetInt32();
                uint32 l_Spell07 = field[18].GetInt32();
                uint32 l_Head = field[19].GetInt32();
                uint32 l_Neck = field[20].GetInt32();
                uint32 l_Shoulder = field[21].GetInt32();
                uint32 l_Shirt = field[22].GetInt32();
                uint32 l_Chest = field[23].GetInt32();
                uint32 l_Waist = field[24].GetInt32();
                uint32 l_Legs = field[25].GetInt32();
                uint32 l_Feet = field[26].GetInt32();
                uint32 l_Wrist = field[27].GetInt32();
                uint32 l_Hands = field[28].GetInt32();
                uint32 l_Finger1 = field[29].GetInt32();
                uint32 l_Finger2 = field[30].GetInt32();
                uint32 l_Trinket1 = field[31].GetInt32();
                uint32 l_Trinket2 = field[32].GetInt32();
                uint32 l_Back = field[33].GetInt32();
                uint32 l_MainHand = field[34].GetInt32();
                uint32 l_OffHand = field[35].GetInt32();
                uint32 l_Ranged = field[36].GetInt32();
                uint32 l_Tabard = field[37].GetInt32();
                uint32 l_FirstBag = field[38].GetInt32();
                uint32 l_SecondBag = field[39].GetInt32();
                uint32 l_ThirdBag = field[40].GetInt32();
                uint32 l_FourthBag = field[41].GetInt32();

                PreparedStatement* stmt_2 = CharacterDatabase.GetPreparedStatement(CHAR_INS_TREFFLE_TEMP_INVENTORY);

                int index = 1;

                stmt->setString(index++, l_Name);
                stmt->setUInt32(index++, l_Display);
                stmt->setUInt32(index++, l_Health);
                stmt->setUInt32(index, l_Power1);
                stmt->setUInt32(index++, l_Power2);
                stmt->setUInt32(index++, l_Power3);
                stmt->setUInt32(index++, l_Strength);
                stmt->setUInt32(index++, l_Agillity);
                stmt->setUInt32(index++, l_Stamina);
                stmt->setUInt32(index++, l_Intellect);
                stmt->setUInt32(index++, l_Spirit);
                stmt->setUInt32(index++, l_Spell01);
                stmt->setUInt32(index++, l_Spell02);
                stmt->setUInt32(index++, l_Spell03);
                stmt->setUInt32(index++, l_Spell04);
                stmt->setUInt32(index++, l_Spell05);
                stmt->setUInt32(index++, l_Spell06);
                stmt->setUInt32(index++, l_Spell07);
                stmt->setUInt32(index++, l_Head);
                stmt->setUInt32(index++, l_Neck);
                stmt->setUInt32(index++, l_Shoulder);
                stmt->setUInt32(index++, l_Shirt);
                stmt->setUInt32(index++, l_Chest);
                stmt->setUInt32(index++, l_Waist);
                stmt->setUInt32(index++, l_Legs);
                stmt->setUInt32(index++, l_Feet);
                stmt->setUInt32(index++, l_Hands);
                stmt->setUInt32(index++, l_Wrist);
                stmt->setUInt32(index++, l_Finger1);
                stmt->setUInt32(index++, l_Finger2);
                stmt->setUInt32(index++, l_Trinket1);
                stmt->setUInt32(index++, l_Trinket2);
                stmt->setUInt32(index++, l_Back);
                stmt->setUInt32(index++, l_MainHand);
                stmt->setUInt32(index++, l_OffHand);
                stmt->setUInt32(index++, l_Ranged);
                stmt->setUInt32(index++, l_Tabard);
                stmt->setUInt32(index++, l_FirstBag);
                stmt->setUInt32(index++, l_SecondBag);
                stmt->setUInt32(index++, l_ThirdBag);
                stmt->setUInt32(index++, l_FourthBag);

                uint32 l_SpellEntries[7] = { l_Spell01, l_Spell02, l_Spell03, l_Spell04, l_Spell05, l_Spell06, l_Spell07 };
                p_Player->SendInitialActionButtons();

                for (uint8 l_I = 0; l_I < 7; l_I++)
                {
                    p_Player->learnSpell(l_SpellEntries[l_I], true);
                    p_Player->addActionButton(l_I++, l_SpellEntries[l_I], ACTION_BUTTON_SPELL);
                }

                /*
                PreparedStatement* stmt_3 = CharacterDatabase.GetPreparedStatement(CHAR_INS_TREFFLE_TEMP_STATS);
                do
                {
                stmt_3->setUInt32(0, p_Player->GetGUID());
                stmt_3->setUInt32(1, );
                stmt_3->setUInt32(2, l_Spec);
                stmt_3->setUInt32(3, l_Disabled);
                } while ()
                CharacterDatabase.Execute(stmt_3);
                */

                int32 l_StatEntries[5] = { l_Strength, l_Agillity, l_Stamina, l_Intellect, l_Spirit };

                p_Player->SetName(l_Name);
                p_Player->SetDisplayId(l_Display);
                p_Player->SetMaxHealth(l_Health);
                p_Player->SetHealth(l_Health);

                for (uint8 l_I = 0; l_I < 5; l_I)
                    p_Player->SetStatInt32Value(Stats(l_I), l_StatEntries[l_I]);
                /*
                for (uint8 l_I = 0; l_I < 5; l_I++)
                p_Player->SetStat(Stats(Stats::STAT_STRENGTH), l_StatEntries[l_I]);
                */

                uint32 l_ItemEntries[22] = { l_Head, l_Neck, l_Shoulder, l_Shirt, l_Chest, l_Waist, l_Legs, l_Feet, l_Wrist, l_Finger1, l_Finger2, l_Trinket1, l_Trinket2, l_Back, l_MainHand, l_OffHand, l_Ranged, l_Tabard, l_FirstBag, l_SecondBag, l_ThirdBag, l_FourthBag };

                for (uint8 l_I = 1; l_I < 22; l_I++)
                {
                    p_Player->EquipNewItem(l_I, l_ItemEntries[l_I], true);
                }

                p_Player->RemoveAllAuras();
                p_Player->SendInitialPacketsAfterAddToMap();
                p_Player->SendInitialActionButtons();

                CharacterDatabase.Execute(stmt);
            } while (l_CheckCastResult->NextRow());
        }
    }
}

void ActionCharacterDataSaveInventory(Player* p_Player)
{
    if (p_Player)
    {
        // Next, load any items that were saved
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TREFFLE_INVENTORY);
        stmt->setUInt32(0, p_Player->GetGUID());
        PreparedQueryResult l_CheckCastResult = CharacterDatabase.Query(stmt);

        if (l_CheckCastResult)
        {
            do
            {
                Field* field = l_CheckCastResult->Fetch();
                uint32 l_Id = field[0].GetInt32();
                uint32 l_Bag = field[1].GetInt32();
                uint32 l_Slot = field[2].GetInt32();
                uint32 l_Item = field[3].GetInt32();

                PreparedStatement* stmt_2 = CharacterDatabase.GetPreparedStatement(CHAR_INS_TREFFLE_TEMP_INVENTORY);

                int index = 0;

                stmt->setUInt32(index++, l_Id);
                stmt->setUInt32(index++, l_Bag);
                stmt->setUInt32(index++, l_Slot);
                stmt->setUInt32(index++, l_Item);

                p_Player->DestroyItem(l_Bag, l_Slot, true);

                CharacterDatabase.Execute(stmt);
            } while (l_CheckCastResult->NextRow());
        }
    }
}

void ActionCharacterDataSaveStats(Player* p_Player)
{
    if (p_Player)
    {
        // Next, load any items that were saved
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TREFFLE_STATS);
        stmt->setUInt32(0, p_Player->GetGUID());
        PreparedQueryResult l_CheckCastResult = CharacterDatabase.Query(stmt);

        if (l_CheckCastResult)
        {
            Field* field = l_CheckCastResult->Fetch();
            uint32 l_Guid = field[0].GetInt32();
            uint32 l_MaxHealth = field[1].GetInt32();
            uint32 l_MaxPower1 = field[2].GetInt32();
            uint32 l_MaxPower2 = field[3].GetInt32();
            uint32 l_MaxPower3 = field[4].GetInt32();
            uint32 l_MaxPower4 = field[5].GetInt32();
            uint32 l_MaxPower5 = field[6].GetInt32();
            uint32 l_MaxPower6 = field[7].GetInt32();
            uint32 l_MaxPower7 = field[8].GetInt32();
            uint32 l_Strength = field[9].GetInt32();
            uint32 l_Agillity = field[10].GetInt32();
            uint32 l_Stamina = field[11].GetInt32();
            uint32 l_Intellect = field[12].GetInt32();
            uint32 l_Spirit = field[13].GetInt32();
            uint32 l_Armor = field[14].GetInt32();
            uint32 l_ResHoly = field[15].GetInt32();
            uint32 l_ResFire = field[16].GetInt32();
            uint32 l_ResNature = field[17].GetInt32();
            uint32 l_ResFrost = field[18].GetInt32();
            uint32 l_ResShadow = field[19].GetInt32();
            uint32 l_ResArcane = field[20].GetInt32();
            float l_BlockPct = field[21].GetFloat();
            float l_DodgePct = field[22].GetFloat();
            float l_ParrtPct = field[23].GetFloat();
            float l_CritPct = field[24].GetFloat();
            float l_RangedCritPct = field[25].GetFloat();
            float l_SpellCritPct = field[26].GetFloat();
            uint32 l_AttackPower = field[27].GetInt32();
            uint32 l_RangedAttackPower = field[28].GetInt32();
            uint32 l_SpellPower = field[29].GetInt32();
            uint32 l_Resillience = field[30].GetInt32();

            int index = 0;

            PreparedStatement* stmt_2 = CharacterDatabase.GetPreparedStatement(CHAR_INS_TREFFLE_STATS);

            stmt_2 = CharacterDatabase.GetPreparedStatement(CHAR_INS_TREFFLE_STATS);
            stmt_2->setUInt32(index++, l_Guid);
            stmt_2->setUInt32(index++, l_MaxHealth);
            stmt_2->setUInt32(index++, l_MaxPower1);
            stmt_2->setUInt32(index++, l_MaxPower2);
            stmt_2->setUInt32(index++, l_MaxPower3);
            stmt_2->setUInt32(index++, l_MaxPower4);
            stmt_2->setUInt32(index++, l_MaxPower5);
            stmt_2->setUInt32(index++, l_MaxPower6);
            stmt_2->setUInt32(index++, l_MaxPower7);
            stmt_2->setUInt32(index++, l_Strength);
            stmt_2->setUInt32(index++, l_Agillity);
            stmt_2->setUInt32(index++, l_Stamina);
            stmt_2->setUInt32(index++, l_Intellect);
            stmt_2->setUInt32(index++, l_Spirit);
            stmt_2->setUInt32(index++, l_Armor);
            stmt_2->setUInt32(index++, l_ResHoly);
            stmt_2->setUInt32(index++, l_ResFire);
            stmt_2->setUInt32(index++, l_ResNature);
            stmt_2->setUInt32(index++, l_ResFrost);
            stmt_2->setUInt32(index++, l_ResShadow);
            stmt_2->setUInt32(index++, l_ResArcane);
            stmt_2->setFloat(index++, l_BlockPct);
            stmt_2->setFloat(index++, l_DodgePct);
            stmt_2->setFloat(index++, l_ParrtPct);
            stmt_2->setFloat(index++, l_CritPct);
            stmt_2->setFloat(index++, l_RangedCritPct);
            stmt_2->setFloat(index++, l_SpellCritPct);
            stmt_2->setUInt32(index++, l_AttackPower);
            stmt_2->setUInt32(index++, l_RangedAttackPower);
            stmt_2->setUInt32(index++, l_SpellPower);
            stmt_2->setUInt32(index++, l_Resillience);

            CharacterDatabase.Execute(stmt);

            /*
            /// Client load - What? I was probably super high as the function name says SAVESTATS *breakdownastrix
            uint32 l_Stats[5] = {l_Strength, l_Agillity, l_Stamina, l_Intellect, l_Spirit};
            float l_Resistanes[6] = { l_ResHoly, l_ResFire, l_ResNature, l_ResFrost, l_ResShadow, l_ResArcane };

            // save new stats
            for (uint8 i = POWER_MANA; i < MAX_POWERS; ++i)
            p_Player->SetMaxPower(Powers(i), uint32(p_Player->GetCreatePowers(Powers(i))));

            p_Player->SetMaxHealth(l_MaxHealth);
            p_Player->SetHealth(l_MaxHealth);

            for (uint8 l_I = 1; l_I < MAX_SPELL_SCHOOL; ++l_I)
            {
            p_Player->SetResistance(SpellSchools(l_I), l_Resistanes[l_I]);
            p_Player->SetResistanceBuffMods(SpellSchools(l_I), true, l_Resistanes[l_I]);
            p_Player->SetResistanceBuffMods(SpellSchools(l_I), false, l_Resistanes[l_I]);
            }

            p_Player->SetFloatValue(PLAYER_PARRY_PERCENTAGE, l_ParrtPct);
            p_Player->SetFloatValue(PLAYER_BLOCK_PERCENTAGE, l_BlockPct);
            p_Player->SetArmor(int32(l_Armor));

            // Dodge percentage
            p_Player->SetFloatValue(PLAYER_DODGE_PERCENTAGE, l_DodgePct);

            for (uint8 l_I = STAT_STRENGTH; l_I < MAX_STATS; ++l_I)
            p_Player->SetStat(Stats(l_I), l_Stats[l_I]);

            for (uint32 l_I = 0; l_I < 24; l_I)
            p_Player->UpdateRating(CombatRating(l_I));

            p_Player->UpdateAttackPowerAndDamage();
            p_Player->UpdateAttackPowerAndDamage(true);
            */
        }
    }
}

void ActionCharacterDataSaveSpells(Player* p_Player)
{
    if (p_Player)
    {
        // Next, load any items that were saved
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TREFFLE_STATS);
        stmt->setUInt32(0, p_Player->GetGUID());
        PreparedQueryResult l_CheckCastResult = CharacterDatabase.Query(stmt);

        if (l_CheckCastResult)
        {
            do
            {
                Field* field = l_CheckCastResult->Fetch();
                uint32 l_Spell = field[1].GetInt32();
                uint32 l_Spec = field[2].GetInt32();
                uint32 l_Disabled = field[3].GetInt32();

                PreparedStatement* stmt_2 = CharacterDatabase.GetPreparedStatement(CHAR_INS_TREFFLE_TEMP_STATS);

                stmt->setUInt32(0, p_Player->GetGUID());
                stmt->setUInt32(1, l_Spell);
                stmt->setUInt32(2, l_Spec);
                stmt->setUInt32(3, l_Disabled);

                CharacterDatabase.Execute(stmt);
            } while (l_CheckCastResult->NextRow());
        }
    }
}

class treffle : public ItemScript
{
public:

    treffle() : ItemScript("treffle") {}

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Choose a character", GOSSIP_SENDER_MAIN, eActions::ACTION_CHARACTER_CHOICE);
        player->PlayerTalkClass->SendGossipMenu(99999, item->GetGUID());
        return false;
    }

    void OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
    {
        if (sender != GOSSIP_SENDER_MAIN)
        {
            return;
        }

        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case eActions::ACTION_CHARACTER_CHOICE:
            ActionCharacterDataSaveSpells(player);
            ActionCharacterDataSaveStats(player);
            ActionCharacterDataSaveInventory(player);
            ActionCharacterChosel(player, 1);
            break;
        default:
            break;
        }
    }
};
/*
void AddSC_treffle()
{
    new treffle();
}
*/


void AddSC_boss_xeritac()
{
    new treffle();
    new boss_xeritac();                                 ///< 84550
    new the_everbloom_xeritac_mob_venom_sprayer();      ///< 86547
    new the_everbloom_xeritac_mob_crazed_pale_one();    ///< 84554
    new the_everbloom_xeritac_mob_toxic_spiderling();   ///< 84552
    new the_everbloom_xeritac_mob_gorged_bruster();     ///< 86552
    new the_everbloom_xeritac_spell_gaseous_volley();   ///< 169382
    new the_everbloom_xeritac_spell_descend();          ///< 169278
    new the_everbloom_xeritac_areatrigger_toxic_gas();  ///< 169224
    new the_everbloom_xertiac_gameobject_eggs();        ///< 234113
}
