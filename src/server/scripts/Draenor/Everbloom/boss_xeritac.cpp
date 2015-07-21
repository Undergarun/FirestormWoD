
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eXeritacSpells
{
    // Descend
    SpellDecsendDummy        = 172643,
    SpellDecsendBeam         = 169322,
    SpellDescendEffectDamage = 169275, 
    SpellDescendDamage       = 169278, 
    SpellWebCrawl            = 169293,

    SpellGasVolleyDummy      = 169382,
    SpellGasVolleyMissile    = 169383,
    SpellToxicGasDamage      = 169223, 
    SpellToxicGasAreaTrigger = 169224,
    SpellToxicBolt           = 169375,
    SpellToxicGas            = 169218,
    SpellVenoumousSting      = 169376, 
    SpellSwipeDamage         = 169371,
    SpellConsume             = 169248,
    SpellConsumeAura         = 169249,
    SpellVenomSpray          = 172727,
    SpellToxicBlood          = 169218,
    SpellToxicitiy           = 169219,
    SpellToxicBloodExplode   = 169267,
    SpellFixate              = 173080,
    SpellBurst               = 173081,
    SpellInhaleDummy         = 169233,
};

enum eXeritacEvents
{
    EventConsume            = 211,
    EventDescend            = 212,
    EventDecsendDummy       = 250,
    EventGasVolley          = 213,
    EventToxicGas           = 214,
    EventToxicBolt          = 215,
    EventToxicAura          = 216,
    EventVenomousString     = 217,
    EventWebCrawl           = 218,

    EventToxicSpiderling    = 220,
    EventVenomCrazedPaleOne = 221,
    EventGorgendBusters     = 222,
    EventVenomSprayers      = 223,

    EventExplode            = 225,
    EventSwipe              = 224,
    EventCancelSwipe        = 226,
    EventInhale             = 227,
    EventRemoveBeam         = 228,
    EventMoveHack           = 229,
    EventInitialBeam        = 230,
};

enum eXeritacCreatures
{
    TriggerGas         = 213152,
    TriggerWebCrawl    = 534536,
    TriggerBeamTrigger = 432521,
};

enum eXeritacActions
{
    ActionCountSpiderLing = 1,
    ActionInhale          = 2,
    ActionCount           = 3,
    ActionConsumeEffect   = 4,
};

enum eXeritacTalks
{
    // |TInterface\Icons\AbilityRacialCannibalize.blp:20|tXeri'tac starts to |cFFFF0000|Hspell:169248|h[Consume]|h|r a Pale Orc!

    /*
    SAYSPELL1 = Inhale
    SAYSPELL2 = Consume
    SAYSPELL3 = [Special Warining]Gaseous Volley !
    SAYSPELL4 = [Special Warining] Consume
    SAYSPELL5 = Xeri'tac begins unleashing Toxic Spideling down on you!
    SAYSPELL6 = Xeri'tac descends from her web!
    SAYSPELL7 = Xeri'tac start to [Consume] a pale orc!
    */
};

/*
Position l_PosHomePoint = { 923.86f, 1455.56f, 90.867f, 5.414306f };
Position l_PosRandomMovements[5] =
{
    { 931.20f, 1441.587f, 88.727f, 3.289805f },
    { 908.84f, 1449.853f, 87.077f, 0.615524f },
    { 921.09f, 1456.374f, 87.475f, 5.465359f },
    { 938.89f, 1443.991f, 88.411f, 3.359706f },
    { 930.02f, 1434.803f, 87.784f, 2.946586f },
};
Position l_PositionPaleOne[2] =
{
    { 926.44f, 1413.111f, 66.364f, 2.336323f },
    { 956.79f, 1448.274f, 64.274f, 3.184339f },
};
*/

Position l_PosHomePoint = { 923.86f, 1455.56f, 90.867f, 5.414306f };
Position l_PosMotionStart = {934.475f, 1430.094f, 85.754f};
Position l_PosRandomMovements[5] =
{
    { 931.20f, 1441.587f, 85.600f, 3.289805f },
    { 908.84f, 1449.853f, 85.600f, 0.615524f },
    { 921.09f, 1456.374f, 85.600f, 5.465359f },
    { 938.89f, 1443.991f, 85.600f, 3.359706f },
    { 930.02f, 1434.803f, 85.600f, 2.946586f },
};
Position l_PositionPaleOne[2] =
{
    { 926.44f, 1413.111f, 66.364f, 2.336323f },
    { 956.79f, 1448.274f, 64.274f, 3.184339f },
};

#define HostileFaction  16
#define FriendlyFaction 35

static void ShootBeam(Unit* caster)
{
    Creature* beam = caster->SummonCreature(TriggerBeamTrigger, caster->GetPositionX(), caster->GetPositionY(), 87.611f, caster->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 8 * TimeConstants::IN_MILLISECONDS);

    if (beam)
    {
        beam->NearTeleportTo(caster->GetPositionX(), caster->GetPositionY(), 87.611f, caster->GetOrientation());
        caster->CastSpell(beam, eXeritacSpells::SpellDecsendBeam);
        caster->GetMotionMaster()->MovePoint(10, caster->GetPositionX(), caster->GetPositionY(), 87.611f);
    }
}

static void RandomMove(Unit* p_Me)
{
    std::list<Position> Epos;

    for (int i = 0; i < 5; i++)
        Epos.push_back(l_PosRandomMovements[i]);

    std::list<Position>::const_iterator it = Epos.begin();
    std::advance(it, urand(0, Epos.size() - 1));

    p_Me->GetMotionMaster()->MovePoint(0, it->GetPositionX(), it->GetPositionY(), it->GetPositionZ());
}

static void DescendBeam(Creature* p_Creature)
{
    if (p_Creature)
    {
        Creature* beam = p_Creature->SummonCreature(TriggerBeamTrigger, p_Creature->GetPositionX(), p_Creature->GetPositionY(), p_Creature->GetPositionZ(), p_Creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 8 * TimeConstants::IN_MILLISECONDS);

        if (beam)
        {
            p_Creature->CastSpell(beam, eXeritacSpells::SpellDecsendBeam);
        }

        p_Creature->GetMotionMaster()->MoveTakeoff(0, p_Creature->GetPositionX(), p_Creature->GetPositionY(), 64.589f);
        //p_Creature->GetMotionMaster()->MovePoint(0, p_Creature->GetPositionX(), p_Creature->GetPositionY(), 68.081f);
    }
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

class the_everbloom_remove_non_players
{
public:
    the_everbloom_remove_non_players() {}
    bool operator()(WorldObject* object)
    {
        if (object->GetTypeId() == TYPEID_PLAYER)
            return false;
        else
            return true;
    }
};

// Xeri'tac - 84550
class boss_xeritac : public CreatureScript
{
public:
    boss_xeritac() : CreatureScript("boss_xeritac") { }

    struct boss_xeritacAI : public BossAI
    {
        boss_xeritacAI(Creature* pCreature) : BossAI(pCreature, eEverbloomData::DataXeritac)
        {
            m_Instance = me->GetInstanceScript();
            Reset();

            WebDoor();

            m_Intro = false;
        }

        InstanceScript* m_Instance;
        uint32 m_TimeBetween;
        uint8  m_Count;
        uint32 m_Phase;
        uint64 m_ConsumedGUID;
        bool m_Intro;
        bool m_Descend;
        bool m_Consuming;

        void Reset() override
        {
            events.Reset();
            summons.DespawnAll();

            m_Consuming = false;
            m_Descend = false;
            m_Count = 0;
            m_TimeBetween = 6 * TimeConstants::IN_MILLISECONDS;
            m_Phase = 1;
            m_ConsumedGUID = 0;
         
            me->setFaction(HostileFaction);       
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
        }

        void WebDoor()
        {
            // Get Barrier Down
            if (GameObject* l_webDoor = instance->instance->GetGameObject(instance->GetData64(eEverbloomData::DataObjectWebDoor)))
            {
                // Activate
                l_webDoor->SetLootState(GO_READY);
                l_webDoor->UseDoorOrButton();
            }
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro)
            {
                m_Intro = true;
            }

            if (p_Who && p_Who->IsInWorld() && p_Who->GetEntry() == eEverbloomCreature::CreatureVenomCrazedPaleOne && p_Who->IsWithinDistInMap(me, 3.0f, true) && !m_Consuming && p_Who->isAlive())
            {
                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                m_Consuming = true;

                //m_ConsumedGUID = p_Who->GetGUID();

                //me->CastSpell(p_Who, eXeritacSpells::SpellConsume);
                //me->AddAura(eXeritacSpells::SpellConsumeAura, p_Who);

               // p_Who->AddUnitState(UNIT_STATE_STUNNED);      
               // me->MonsterTextEmote("|TInterface\Icons\AbilityRacialCannibalize.blp:20|tXeri'tac starts to |cFFFF0000|Hspell:169248|h[Consume]|h|r a Pale Orc!", Language::LANG_UNIVERSAL, me->GetGUID());
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();

            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 1);
            me->GetMotionMaster()->MovePoint(0, 935.628f, 1430.930f, 64.988f);

            WebDoor();

            std::list<AreaTrigger*> l_GasAreatriggerList;
            me->GetAreaTriggerList(l_GasAreatriggerList, eXeritacSpells::SpellToxicGasAreaTrigger);

            if (l_GasAreatriggerList.empty())
                return;

            for (auto itr : l_GasAreatriggerList)
            {
                itr->Remove(500);
            }
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            _EnterCombat();

            events.ScheduleEvent(eXeritacEvents::EventDescend,  urand(13 * TimeConstants::IN_MILLISECONDS, 21 * TimeConstants::IN_MILLISECONDS));          
            events.ScheduleEvent(eXeritacEvents::EventToxicSpiderling, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eXeritacEvents::EventVenomSprayers, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eXeritacEvents::EventVenomCrazedPaleOne, 20 * TimeConstants::IN_MILLISECONDS);
            //events.ScheduleEvent(eXeritacEvents::EventInitialBeam, 3 * TimeConstants::IN_MILLISECONDS);

            if (me->GetMap()->IsHeroic())
            {
                events.ScheduleEvent(eXeritacEvents::EventGorgendBusters, 10 * TimeConstants::IN_MILLISECONDS);
            }
            if (m_Instance != nullptr)
            {
                m_Instance->SetBossState(eEverbloomData::DataXeritac, EncounterState::IN_PROGRESS);
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }

            m_Phase = 1;
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                     
            me->GetMotionMaster()->MovePoint(10, me->GetPositionX(), me->GetPositionY(), 87.611f);
        }

        void MovementInform(uint32 /*type*/, uint32 id)
        {
            if (me->isInCombat())
                return;

            switch (id)
            {          
                case 10:   // Beam
                case 11:   // Random Movement
                    RandomMove(me);
                    break;
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                m_Instance->SetBossState(eEverbloomData::DataXeritac, EncounterState::DONE);
            }

            std::list<AreaTrigger*> l_GasAreatriggerList;
            me->GetAreaTriggerList(l_GasAreatriggerList, eXeritacSpells::SpellToxicGasAreaTrigger);

            if (l_GasAreatriggerList.empty())
                return;

            for (auto itr : l_GasAreatriggerList)
            {
                itr->Remove(500);
            }

            WebDoor();

            summons.DespawnAll();
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case eEverbloomActions::ActionCounting:
                    m_Count++;

                    printf("Counter is at %u", m_Count);

                    if (m_Count >= 5 && m_Phase == 1)
                    {
                        m_Phase = 2;
                        events.Reset();

                        m_Descend = true;

                        Position l_CurrentPos;
                        me->GetPosition(&l_CurrentPos);
                        l_CurrentPos.m_positionZ = 65.619f;

                        me->GetMotionMaster()->Clear(true);
                        me->GetMotionMaster()->MovePoint(0, l_CurrentPos);
                    
                        me->SetReactState(REACT_AGGRESSIVE);

                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                        events.ScheduleEvent(eXeritacEvents::EventToxicBolt, urand(7 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        events.ScheduleEvent(eXeritacEvents::EventVenomousString, urand(16 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                        events.ScheduleEvent(eXeritacEvents::EventGasVolley, 30 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eXeritacEvents::EventVenomCrazedPaleOne, 20 * TimeConstants::IN_MILLISECONDS);

                        me->MonsterTextEmote("Xeri'tac descends from her web!", me->GetGUID(), true);

                       // me->GetMotionMaster()->MoveTakeoff(0, me->GetPositionX(), me->GetPositionY(), 65.619f);
                    }
                    break;
                case eXeritacActions::ActionConsumeEffect:
                    if (Creature* l_ConsumedPaleOne = sObjectAccessor->GetCreature(*me, m_ConsumedGUID))
                    {
                        l_ConsumedPaleOne->Kill(l_ConsumedPaleOne);
                        l_ConsumedPaleOne->DespawnOrUnsummon();

                        m_Consuming = false;
                    }
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            // Random Movements
            if (m_TimeBetween <= p_Diff)
            {
                if (!m_Descend)
                {
                    RandomMove(me);
                }
            }

            // Consume
            if (Creature* l_ConsumedPaleOne = sObjectAccessor->GetCreature(*me, m_ConsumedGUID))
            {
                if (l_ConsumedPaleOne->isDead() && l_ConsumedPaleOne->IsInWorld())
                {
                    me->CastStop();
                    m_Consuming = false;
                }
            }

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case eXeritacEvents::EventInitialBeam:
                        me->CastSpell(933.281f, 1424.252f, 90.216f, eXeritacSpells::SpellDecsendBeam, true);
                        break;
                    // Descend Phase
                    case eXeritacEvents::EventDescend:
                    {
                        if (Unit* random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            DescendBeam(me);

                            events.ScheduleEvent(eXeritacEvents::EventDescend, 20 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eXeritacEvents::EventDecsendDummy, 3 * TimeConstants::IN_MILLISECONDS);
                        }
                        break;
                    }
                    case eXeritacEvents::EventDecsendDummy:
                    {
                        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 1);
                        me->CastSpell(me, eXeritacSpells::SpellDecsendDummy);
                        m_Descend = false;
                        m_TimeBetween = 7000;
                        break;
                    }
                    // 2nd Phase
                    case eXeritacEvents::EventToxicBolt:
                    {
                        if (Unit* l_Victim = me->getVictim())
                        {
                            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                                return;

                            me->CastSpell(l_Victim, eXeritacSpells::SpellToxicBolt);
                            events.ScheduleEvent(eXeritacEvents::EventToxicBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                        }
                        break;
                    }
                    case eXeritacEvents::EventVenomousString:
                    {
                        if (Unit* random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                        {
                            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                                return;

                            me->CastSpell(random, eXeritacSpells::SpellVenoumousSting);
                            events.ScheduleEvent(eXeritacEvents::EventVenomousString, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        }
                        break;
                    }
                    case eXeritacEvents::EventGasVolley:
                    {
                        me->CastSpell(me, SpellGasVolleyDummy);
                        events.ScheduleEvent(eXeritacEvents::EventGasVolley, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    // Adds
                    case eXeritacEvents::EventGorgendBusters:
                    {
                        std::list<Position> pos;

                        for (int i = 0; i < 6; i++)
                            pos.push_back(l_PosRandomMovements[i]);

                        std::list<Position>::const_iterator it = pos.begin();
                        std::advance(it, urand(0, pos.size() - 1));

                        Creature* spiderling = me->SummonCreature(eEverbloomCreature::CreatureGorgendBusters, it->GetPositionX(), it->GetPositionY(), it->GetPositionZ(), it->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        if (spiderling)
                        {
                            DescendBeam(spiderling);
                        }

                        events.ScheduleEvent(eXeritacEvents::EventGorgendBusters, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eXeritacEvents::EventToxicSpiderling:
                    {
                        std::list<Position> pos;

                        for (int i = 0; i < 6; i++)
                            pos.push_back(l_PosRandomMovements[i]);

                        std::list<Position>::const_iterator it = pos.begin();
                        std::advance(it, urand(0, pos.size() - 1));

                        Creature* spiderling = me->SummonCreature(eEverbloomCreature::CreatureToxicSpiderling, it->GetPositionX(), it->GetPositionY(), it->GetPositionZ(), it->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        if (spiderling)
                        DescendBeam(spiderling);

                        me->MonsterTextEmote("|TInterface\\Icons\\achievement_halloween_rottenegg_01.blp:20|tXeri'tac begins unleashing Toxic Spiderlings down on you!", me->GetGUID());

                        events.ScheduleEvent(eXeritacEvents::EventToxicSpiderling, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eXeritacEvents::EventVenomSprayers:
                    {
                        std::list<Position> pos;

                        for (int i = 0; i < 6; i++)
                            pos.push_back(l_PosRandomMovements[i]);

                        std::list<Position>::const_iterator it = pos.begin();
                        std::advance(it, urand(0, pos.size() - 1));

                        Creature* spiderling = me->SummonCreature(eEverbloomCreature::CreatureVenomSprayer, it->GetPositionX(), it->GetPositionY(), it->GetPositionZ(), it->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        if (spiderling)
                        DescendBeam(spiderling);

                        events.ScheduleEvent(eXeritacEvents::EventVenomSprayers, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eXeritacEvents::EventVenomCrazedPaleOne:
                    {
                        if (roll_chance_i(50))
                        {
                             me->SummonCreature(eEverbloomCreature::CreatureVenomCrazedPaleOne, l_PositionPaleOne[1], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        }
                        else
                        {
                             me->SummonCreature(eEverbloomCreature::CreatureVenomCrazedPaleOne, l_PositionPaleOne[2], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        }

                        events.ScheduleEvent(eXeritacEvents::EventVenomCrazedPaleOne, 35 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }

                DoMeleeAttackIfReady();

            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new boss_xeritacAI(pCreature);
    }
};

// Vendom Sprayer - 86547
class the_everbloom_venom_sprayer : public CreatureScript
{
public:
    the_everbloom_venom_sprayer() : CreatureScript("the_everbloom_venom_sprayer") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
             m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            DoZoneInCombat();
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);

            events.ScheduleEvent(eXeritacEvents::EventRemoveBeam, 3 * TimeConstants::IN_MILLISECONDS);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eXeritacEvents::EventMoveHack, 3 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eXeritacEvents::EventVenomSprayers, urand(7 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eXeritacEvents::EventMoveHack:
                    if (Unit* l_Target = me->getVictim())
                        me->GetMotionMaster()->MoveFollow(l_Target, 0, 0, MOTION_SLOT_ACTIVE);

                    events.ScheduleEvent(eXeritacEvents::EventMoveHack, 3000);
                    break;
                case eXeritacEvents::EventVenomSprayers:
                    me->CastSpell(me, eXeritacSpells::SpellVenomSpray);
                    events.ScheduleEvent(eXeritacEvents::EventToxicGas, urand(7 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eXeritacEvents::EventRemoveBeam:
                    me->RemoveAura(eXeritacSpells::SpellDecsendBeam);
                    break;
            }

            DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Crazed Pale One - 84554
class the_everbloom_venom_crazed_pale_one : public CreatureScript
{
public:
    the_everbloom_venom_crazed_pale_one() : CreatureScript("the_everbloom_venom_crazed_pale_one") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        bool m_Transformed;

        void Reset() override
        {
            m_Transformed = false;
            DoZoneInCombat();     
            me->setFaction(HostileFaction);

            me->SetSpeed(MOVE_RUN, 0.6f, true);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eXeritacEvents::EventSwipe, 2 * TimeConstants::IN_MILLISECONDS);
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
            case eXeritacActions::ActionInhale:
                me->CastSpell(me, eXeritacSpells::SpellInhaleDummy);
                break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->FindNearestCreature(eXeritacCreatures::TriggerGas, 1.5f) && !m_Transformed)
            {
                m_Transformed = true;
                me->CastSpell(me, eXeritacSpells::SpellToxicGas);
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eXeritacEvents::EventSwipe:
                    me->CastSpell(me, eXeritacSpells::SpellSwipeDamage); // swipe

                    events.ScheduleEvent(eXeritacEvents::EventSwipe, 3 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eXeritacEvents::EventCancelSwipe, 15 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eXeritacEvents::EventCancelSwipe:
                    events.CancelEvent(eXeritacEvents::EventSwipe);
                    break;
            }

            DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Toxic Spiderling - 84552
class the_everbloom_toxic_spiderling : public CreatureScript
{
public:
    the_everbloom_toxic_spiderling() : CreatureScript("the_everbloom_toxic_spiderling") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        bool m_Explode;

        void Reset() override
        {
            DoZoneInCombat();
            me->RemoveAllAuras();
            me->SetDisableGravity(true);
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);

            m_Explode = false;

           events.ScheduleEvent(eXeritacEvents::EventRemoveBeam, 3 * TimeConstants::IN_MILLISECONDS);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->CastSpell(me, eXeritacSpells::SpellToxicBlood);
            
            events.ScheduleEvent(eXeritacEvents::EventMoveHack, 3 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eXeritacEvents::EventToxicBolt, 8 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* Xeritac = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataXeritac)))
                {
                    if (Xeritac->GetAI())
                        Xeritac->GetAI()->DoAction(eEverbloomActions::ActionCounting);

                    //me->CastSpell(me, eXeritacSpells::SpellToxicGasAreaTrigger);
                    me->SummonCreature(eXeritacCreatures::TriggerGas, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                }
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasAura(eEverbloomSpells::SpellToxicity) && me->GetAura(eEverbloomSpells::SpellToxicity)->GetStackAmount() >= 10 && !m_Explode)
            {
                m_Explode = true;
                me->RemoveAllAuras();

                me->CastSpell(me, eXeritacSpells::SpellToxicBloodExplode);
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eXeritacEvents::EventMoveHack:
                    if (Unit* l_Target = me->getVictim())
                        me->GetMotionMaster()->MoveFollow(l_Target, 0, 0, MOTION_SLOT_ACTIVE);

                    events.ScheduleEvent(eXeritacEvents::EventMoveHack, 3000);
                    break;
                case eXeritacEvents::EventToxicBolt:

                    if (Unit* l_Victim = me->getVictim())
                        me->CastSpell(l_Victim, eXeritacSpells::SpellToxicBolt);

                    events.ScheduleEvent(eXeritacEvents::EventToxicBolt, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eXeritacEvents::EventRemoveBeam:
                    me->RemoveAura(eXeritacSpells::SpellDecsendBeam);
                    break;
            }

            DoMeleeAttackIfReady();

        }

    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Gorged Bursters - 86552
class the_everbloom_gorged_bruster : public CreatureScript
{
public:
    the_everbloom_gorged_bruster() : CreatureScript("the_everbloom_gorged_bruster") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint64 m_Target = NULL;
        bool m_Transformed;
        bool m_Explosion;

        void Reset() override
        {
            m_Transformed = false;
            m_Explosion = false;
            DoZoneInCombat();
            me->SetDisableGravity(true);
            me->SetSpeed(MOVE_RUN, 0.3f, true);

            if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
            {
                me->GetMotionMaster()->MoveFollow(l_Random, 0, 0, MOTION_SLOT_ACTIVE);
                me->CastSpell(l_Random, eXeritacSpells::SpellFixate);

                m_Target = l_Random->GetGUID();
            }

            events.ScheduleEvent(eXeritacEvents::EventMoveHack, 3000);
            events.ScheduleEvent(eXeritacEvents::EventRemoveBeam, 3 * TimeConstants::IN_MILLISECONDS);
        }

        void OnAddThreat(Unit* victim, float& fThreat, SpellSchoolMask /*schoolMask*/, SpellInfo const* /*threatSpell*/) override
        {
            if (victim->GetGUID() != m_Target)
                fThreat = 0;
            return;
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (!m_Explosion)
            {
                // Burst hardcoded
                if (Player* l_Player = sObjectAccessor->GetPlayer(*me, m_Target))
                {
                    me->SetSpeed(MOVE_RUN, 0.3f, true);

                    if (!me->isMoving())
                        me->GetMotionMaster()->MoveFollow(l_Player, 0, 0, MOTION_SLOT_ACTIVE);

                    if (l_Player->IsWithinDistInMap(me, 1.0f))
                    {
                        m_Explosion = true;
                        me->CastSpell(l_Player, eXeritacSpells::SpellBurst);
                    }
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eXeritacEvents::EventRemoveBeam:
                    me->RemoveAura(eXeritacSpells::SpellDecsendBeam);
                    break;
                case eXeritacEvents::EventMoveHack:
                    if (Unit * l_Target = me->getVictim())
                        me->Attack(l_Target, true);

                    events.ScheduleEvent(eXeritacEvents::EventMoveHack, 3000);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Xeritac Beam - 432521
class the_everbloom_xeritac_beam : public CreatureScript
{
public:
    the_everbloom_xeritac_beam() : CreatureScript("the_everbloom_xeritac_beam") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->setFaction(35);
            me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 87.808f, me->GetOrientation());
        }

        void OnAddThreat(Unit* victim, float& fThreat, SpellSchoolMask /*schoolMask*/, SpellInfo const* /*threatSpell*/) override
        {
                fThreat = 0;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Trigger Gas - 213152
class the_everbloom_trigger_gas : public CreatureScript
{
public:
    the_everbloom_trigger_gas() : CreatureScript("the_everbloom_trigger_gas") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->setFaction(35);
            me->CastSpell(me, eXeritacSpells::SpellToxicGasAreaTrigger);
        }

        void OnAddThreat(Unit* victim, float& fThreat, SpellSchoolMask /*schoolMask*/, SpellInfo const* /*threatSpell*/) override
        {
            fThreat = 0;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_creaturesAI(pCreature);
    }
};

// Toxic Gas Areatrigger - 169224 
class the_everbloom_xeritac_toxic_gas : public AreaTriggerEntityScript
{
public:
    the_everbloom_xeritac_toxic_gas() : AreaTriggerEntityScript("the_everbloom_xeritac_toxic_gas")
    {
    }

    uint32 m_Diff = 1500;
    uint32 m_DiffInhale = 6000;

    void OnUpdate(AreaTrigger* pAreaTrigger, uint32 pTime)
    {
        // Gas Damage
        if (m_Diff <= pTime)
        {
            std::list<Player*> l_ListTargets;
            JadeCore::AnyPlayerInObjectRangeCheck check(pAreaTrigger, 1.5f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(pAreaTrigger, l_ListTargets, check);
            pAreaTrigger->VisitNearbyObject(1.5f, searcher);

            if (l_ListTargets.empty())
                return;

            for (std::list<Player*>::const_iterator it = l_ListTargets.begin(); it != l_ListTargets.end(); ++it)
            {
                if (!(*it))
                    return;

                if (!(*it)->HasAura(eXeritacSpells::SpellToxicGasDamage))
                {
                    (*it)->AddAura(eXeritacSpells::SpellToxicGasDamage, (*it));
                }
                else
                {
                    AuraPtr aura = (*it)->GetAura(eXeritacSpells::SpellToxicGasDamage);

                    if (aura)
                    {
                        aura->SetDuration(2);
                    }
                }
            }

            m_Diff = 1500;
        }
        else
        {
            m_Diff -= pTime;
        }

        // Inhale
        if (m_DiffInhale <= pTime)
        {
            // Pale Orc 
            if (Creature* l_PaleOne = pAreaTrigger->FindNearestCreature(eEverbloomCreature::CreatureVenomCrazedPaleOne, 2.0f, true))
                if (l_PaleOne->GetAI())
                    l_PaleOne->GetAI()->DoAction(eXeritacActions::ActionInhale);

            m_DiffInhale = 6000;
        }
        else
            m_DiffInhale -= pTime;
    }

    void OnRemove(AreaTrigger* pAreaTrigger, uint32 pTime)
    {
    }

    the_everbloom_xeritac_toxic_gas* GetAI() const
    {
        return new the_everbloom_xeritac_toxic_gas();
    }
};

// Descend - 172643
class the_everbloom_descend : public SpellScriptLoader
{
public:
    the_everbloom_descend() : SpellScriptLoader("the_everbloom_descend") { }

    class the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spells);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[2].TriggerSpell = 0;
            return true;
        }

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            targets.remove_if(the_everbloom_remove_non_players());
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_spells::CorrectTargets, EFFECT_1, TARGET_DEST_DEST);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_spells::CorrectTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };
        SpellScript* GetSpellScript() const
        {
            return new the_everbloom_spells();
        }   
};

// Descend - 169278 
class the_everbloom_descend_trigger : public SpellScriptLoader
{
public:
    the_everbloom_descend_trigger() : SpellScriptLoader("the_everbloom_descend_trigger") { }

    class the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spells);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            targets.remove_if(the_everbloom_remove_non_players());
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_spells::CorrectTargets, EFFECT_0, TARGET_SRC_CASTER);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_spells::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_spells::CorrectTargets, EFFECT_1, TARGET_SRC_CASTER);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(the_everbloom_spells::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };
    SpellScript* GetSpellScript() const
    {
        return new the_everbloom_spells();
    }
};

// Gaseous Volley - 169382 
class the_everbloom_gaseous_volley : public SpellScriptLoader
{
public:
    the_everbloom_gaseous_volley() : SpellScriptLoader("the_everbloom_gaseous_volley") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            std::list<Player*> l_GaseousVolleyList;
            GetCaster()->GetPlayerListInGrid(l_GaseousVolleyList, 200.0f);

            std::list<Player*>::const_iterator it = l_GaseousVolleyList.begin();
            std::advance(it, urand(0, l_GaseousVolleyList.size() - 2 ? l_GaseousVolleyList.size() > 1 : 1));

            GetCaster()->CastSpell((*it), eXeritacSpells::SpellGasVolleyMissile, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new everbloom_spells();
    }
};

// Consume - 169246  
class the_everbloom_consume : public SpellScriptLoader
{
public:
    the_everbloom_consume() : SpellScriptLoader("the_everbloom_consume") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            if (GetCaster()->GetAI())
                GetCaster()->GetAI()->DoAction(eXeritacActions::ActionConsumeEffect);
        }

        void Register()
        {
            AfterCast += SpellCastFn(everbloom_spells::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new everbloom_spells();
    }
};

// Burst- 173081  
class the_everbloom_burst : public SpellScriptLoader
{
public:
    the_everbloom_burst() : SpellScriptLoader("the_everbloom_burst") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            if (GetCaster()->GetTypeId() == TYPEID_PLAYER)
                return;

            GetCaster()->Kill(GetCaster());
            GetCaster()->ToCreature()->DespawnOrUnsummon(2000);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_1, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new everbloom_spells();
    }
};


// Toxic Blood Explosion - 169267  
class the_everbloom_toxic_blood_explosion : public SpellScriptLoader
{
public:
    the_everbloom_toxic_blood_explosion() : SpellScriptLoader("the_everbloom_toxic_blood_explosion") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            GetCaster()->CastSpell(GetCaster(), eXeritacSpells::SpellToxicGasAreaTrigger);
            GetCaster()->Kill(GetCaster());
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new everbloom_spells();
    }
};

// Descend - 169278  
class the_everbloom_descend_damage_trigger : public SpellScriptLoader
{
public:
    the_everbloom_descend_damage_trigger() : SpellScriptLoader("the_everbloom_descend_damage_trigger") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandlCastSpell()
        {
            if (!GetCaster())
                return;

            Creature* beam = GetCaster()->SummonCreature(TriggerBeamTrigger, GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), 87.611f, GetCaster()->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 8 * TimeConstants::IN_MILLISECONDS);

            if (beam)
            {
                //beam->NearTeleportTo(GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), 87.611f, GetCaster()->GetOrientation());
                //GetCaster()->CastSpell(beam, eXeritacSpells::SpellDecsendBeam);
                GetCaster()->GetMotionMaster()->MoveCharge(GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), 87.611f);
                RandomMove(GetCaster());
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(everbloom_spells::HandlCastSpell);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new everbloom_spells();
    }
};

void AddSC_boss_xeritac()
{
    // boss
    new boss_xeritac();

    // creatures
    new the_everbloom_venom_sprayer();
    new the_everbloom_venom_crazed_pale_one();
    new the_everbloom_toxic_spiderling();
    new the_everbloom_gorged_bruster();
    new the_everbloom_xeritac_beam();
    new the_everbloom_trigger_gas();

    // spells
    new the_everbloom_descend_trigger();
    new the_everbloom_descend();
    new the_everbloom_gaseous_volley();
    new the_everbloom_toxic_blood_explosion();
    new the_everbloom_descend_damage_trigger();
    new the_everbloom_burst();
    new the_everbloom_consume();

    // areatriggers
    new the_everbloom_xeritac_toxic_gas();
}
