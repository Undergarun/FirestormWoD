#include "ScriptedCreature.h"
#include "auchindon.hpp"

enum TerongorSpells
{
    SPELL_AGONY = 156925,
    SPELL_CHAOS_BOLT = 156975,
    SPELL_CHAOS_WAVE_DUMMY = 157001,
    SPELL_CHAOS_WAVE_DMG = 157002,
    SPELL_CONFLAGRATE = 154083,
    SPELL_CORRUPTION_DUMMY = 170608,
    SPELL_CORRUPTION_DMG = 156842,
    SPELL_CURSE_OF_EXHAUSTION_DEBUFF = 164841,
    SPELL_DEMONIC_LEAP_DUMMY = 157039,
    SPELL_DEMONIC_LEAP_AREATRIGER = 157040,
    SPELL_DOOM_BUFF = 156965,
    SPELL_DRAIN_LIFE = 156854,
    SPELL_IMMOLATE = 156964,
    SPELL_INCINRATE = 156963,
    SPELL_RAIN_OF_FIRE = 165757,
    SPELL_SEED_OF_MALEVOLENCE_APPLY_AURA = 156921,
    SPELL_SEED_OF_MALEVOLENCE_BUFF = 166451,
    SPELL_SEED_OF_MALEVOLENCE_DMG = 156924,
    SPELL_SEED_OF_MALEVOLENCE_VISUAL_TRIGER = 166462,
    SPELL_SHADOW_BOLT = 156829,
    SPELL_TOUCH_OF_CHAOS_DUMMY = 169028,
    SPELL_TOUCH_OF_CHAOS_DMG = 156968,
    SPELL_UNSTABLE_AFFLICTION = 156954,
    // Shaadum
    SPELL_WRATH_CLEAVE = 159035,
    SPELL_WRATH_STORM = 159033,
    // cosmetic
    SPELL_TRANSCEND = 164009,
    // forms
    SPELL_AFFLICTION_TRANSFORM = 156863,
    SPELL_DESTRUCTION_TRANSFORM = 156866,
    SPELL_DEMONOLOGY_TRANSFORM = 156919,

    // Heroic
    SPELL_SUMMON_ABYSSAL_METEOR = 164508,
    SPELL_SUMMON_ABYSSAL_GROUND_EFFECT = 159681,
    SPELL_SUMMON_ABYSSAL_DUMMY = 157214,
    SPELL_SUMMON_ABYSSAL_SUMMON_SPELL = 157216,
};
enum TerongorEvents
{
    // Teron'gor 
    EVENT_AGONY = 75,
    EVENT_CHAOS_BOLT = 76,
    EVENT_CHAOS_WAVE = 77,
    EVENT_CONFLAGRATE = 78,
    EVENT_CORRUPTION = 79,
    EVENT_CURSE_OF_EXHAUSTION = 80,
    EVENT_DEMONIC_LEAP = 81,
    EVENT_DOOM = 82,
    EVENT_DRAIN_LIFE = 83,
    EVENT_IMMOLATE = 84,
    EVENT_INCINRATE = 85,
    EVENT_RAIN_OF_FIRE = 86,
    EVENT_SEED_OF_MALEVOLENCE = 87,
    EVENT_TOUCH_OF_CHAOS = 88,
    EVENT_UNSTABLE_AFFLICTION = 89,
    EVENT_SHADOW_BOLT = 90,
    EVENT_WRATHCLEAVE = 91,
    EVENT_WRATHSTORM = 92,
    EVENT_BLOOM_OF_MALEVOLENCE = 93,
    EVENT_DEPASSIVE = 94,
    EVENT_TRANSFORM = 95,
    EVENT_TRANSFORM_REMOVE_PASSIVE = 96,
};
enum TerongorTalks
{
    TERONGOR_INTRO_01 = 42, // Who know the draenei held such...delicious treasure in their temple?  (44423)
    TERONGOR_INTRO_02 = 43, // Do you dare challenge me,defenders of Auchindoun?!(44424)
    TERONGOR_INTRO_03 = 44, // Such decadence...it will all burn. (44425)
    TERONGOR_INTRO_05 = 45, // Gul'dan, such foolshness. This gift...so much more you could ever know...(44427)
    TERONGOR_INTRO_06 = 46, // Long have i waited...(44428)
    TERONGOR_INTRO_07 = 47, // ...hungered... to be more... (44429)
    TERONGOR_INTRO_08 = 48, //  And now, all shall bow before me!(44430)
    TERONGOR_KILL_01 = 49, // All will fall before me!(44431)
    TERONGOR_AGGRO_01 = 50, // This power. you will be the first to know it.(44418)
    TERONGOR_SPELL_01 = 52, // Destruction!(44433)
    TERONGOR_SPELL_02 = 53, // Wither.. away! (44434)
    TERONGOR_SPELL_03 = 53, // I become something greater!(44435)
    TERONGOR_SPELL_04 = 54, // Die! (44436)
    TERONGOR_SPELL_05 = 55, // Your demise awaits! (44437)
    TERONGOR_SPELL_06 = 56, // Quickly now. (44438)
    TERONGOR_SPELL_07 = 57, // More! I...need...more!(44439)
    TERONGOR_EVENT_01 = 60, // Jorrun.. who battle demons in life, in death.. your power will feed them! (44420)
    TERONGOR_EVENT_02 = 61, // Joraa, paragon of order - i shall twist your soul to power Destruction!(44421)
    TERONGOR_EVENT_03 = 61, // Elum, life time of healing... now, you shall fuel Deziz and Decay!(44422)
    TERONGOR_DEATH = 100, // (44419)
};
enum TeronogorActions
{
    ACTION_TRANSPORT = 900,
    ACTION_CHOOSE_POWER = 901,
};
enum TreonogorTriggers
{
    TRIGGER_SUMMON_ABYSSAl = 213593,
};
/*
enum SoulTransportation
{
GAMEOBJECT_SOUL_TRANSPORT_START = ,
GAMEOBJECT_SOUL_TRANSPORT_1 = ,
GAMEOBJECT_SOUL_TRANSPORT_2 = ,
GAMEOBJECT_SOUL_TRANSPORT_3 = ,
GAMEOBJECT_SOUL_TRANSPORT_4 = ,
};
*/
Position soul_move[4] =
{
    { 2010.29f, 2855.26f, 35.213f, 2.397908f },
    { 2011.11f, 3051.70f, 35.212f, 4.003247f },
    { 1812.13f, 3052.10f, 35.212f, 5.536366f },
    { 1812.37f, 2854.15f, 35.212f, 0.796488f },
};
Position dead_souls[3] =
{
    { 1922.36f, 2988.91f, 27.743f, 4.405081f },
    { 1946.59f, 2973.00f, 30.193f, 3.678247f },
    { 1963.11f, 3003.42f, 22.359f, 3.904775f },
};
// 1st
Position firstplatformfirstmove = { 1910.91f, 2810.94F, 119.795f }; // DURAG
Position firstplatformsecondmove = { 1959.16f, 2907.14f, 41.030f };
Position firstplatformthirdmove = { 2003.36f, 2863.18f, 35.212f };
// 2nd
Position secondplatformfirstmove = { 1981.34f, 2877.99f, 53.373f }; // GULKOSH
Position secondplatformsecondmove = { 1959.05f, 2999.68f, 75.242f };
Position secondplatformthirdmove = { 2003.70f, 3043.89f, 35.212f };
// 3rd
Position thirdplatformfirstmove = { 2003.70f, 3043.89f, 56.912f }; // GROMTASH
Position thirdplatformsecondmove = { 1914.66f, 2991.45f, 77.863f };
Position thirdplatformthirdmove = { 1817.49f, 3046.65f, 35.212f };
// 4th
Position fourthmovement = { 1846.60f, 2950.43f, 15.170f };
Position TERONGOR_START_POINT[1] =
{
    { 1923.63f, 2974.06f, 16.844f, 1.104046f }
};
Position platform_1[2] =
{
    { 1985.33f, 2876.17f, 35.282f, 5.431586f }, // CREATURE_ZIPTEQ 
    { 1987.57f, 2880.12f, 35.282f, 5.169260f }, //CREATURE_DURAG_THE_DOMINATOR
};
Position platform_2[2] =
{
    { 1983.61f, 3030.35f, 35.280f, 0.613946f }, // CREATURE_SHAADUM  
    { 1987.36f, 3023.54f, 35.282f, 0.747463f }, //CREATURE_GULKOSH
};
Position platform_3[1] =
{
    { 1838.42f, 3027.05f, 35.283f, 2.149401f }, // CREATURE_GROMTASH_THE_DESTRUCTOR
};

Position trigger_position_drain = {1960.20f, 3000.69f, 16.236f, 0.847815f};

class auchindon_boss_terongor : public CreatureScript
{
public:
    auchindon_boss_terongor() : CreatureScript("auchindon_boss_terongor") { }

    struct boss_terongorAI : public BossAI
    {
        boss_terongorAI(Creature* creature) : BossAI(creature, DATA_TERONOGOR)
        {
            instance = me->GetInstanceScript();
            intro = false;     

            Reset();
            me->Respawn(true);

            soultransport1 = false;
            soultransport2 = false;
            soultransport3 = false;
            soultransport4 = false;
            soultransport5 = false;

            me->GetMap()->SetObjectVisibility(1000.0f);
        }
        InstanceScript* instance = me->GetInstanceScript();

        bool secondphase;
        bool intro;

        bool soultransport1;
        bool soultransport2;
        bool soultransport3;
        bool soultransport4;
        bool soultransport5;
        void Reset()
        {
            _Reset();
            events.Reset();
            summons.DespawnAll();

            secondphase = false;

            me->CastSpell(me, 149133); // demonic circle
            me->AddAura(SPELL_TERONOGOR_SHIELD, me);

            DespawnCreaturesInArea(CREATURE_FELBORNE_ABYSSAL, me);

            if (Creature* nearest = me->FindNearestCreature(60975, 300.0f, true))
            {
                me->CastSpell(nearest, SPELL_DRAIN_SOUL_VISUAL);
            }
        }
        void MoveInLineOfSight(Unit* who)
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 18.0f) && !intro)
            {
                intro = true;
                Talk(TERONGOR_INTRO_01);
            }
        }
        void JustReachedHome() override
        {
            summons.DespawnAll();

            if (instance)
                instance->SetBossState(DATA_TERONOGOR, FAIL);
        }
        void DamageTaken(Unit* attacker, uint32 &damage, SpellInfo const* p_SpellInfo)
        {
            if (me->GetHealthPct() <= 75 && !secondphase)
            {
                secondphase = true;
                events.Reset();

                events.ScheduleEvent(EVENT_TRANSFORM, 6000);
            }
        }
        void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 5000.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_SOUL_MOVE_1:
                soultransport1 = true;
                break;
            case ACTION_SOUL_MOVE_2:
                soultransport2 = true;
                break;
            case ACTION_SOUL_MOVE_3:
                soultransport3 = true;
                break;
            case ACTION_SOUL_MOVE_4:
                soultransport4 = true;
                break;
            case ACTION_CHOOSE_POWER:
                events.Reset();

                switch (urand(0, 2))
                {
                case 0: // Mender Elum - Affliction
                    events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(8000, 10000));
                    events.ScheduleEvent(EVENT_CURSE_OF_EXHAUSTION, 12500);
                    events.ScheduleEvent(EVENT_SEED_OF_MALEVOLENCE, urand(22000, 25000));
                    events.ScheduleEvent(EVENT_AGONY, 15600);
                    events.ScheduleEvent(EVENT_DRAIN_LIFE, urand(13000, 16000));
                    events.ScheduleEvent(EVENT_UNSTABLE_AFFLICTION, 20000);
                    // events.ScheduleEvent(EVENT_BLOOM_OF_MALEVOLENCE, 16000);

                    if (Creature* nearest = me->FindNearestCreature(CREATURE_DUREM, 100.0f, true))
                    {
                        //me->CastSpell(nearest, SPELL_DRAIN_SOUL_VISUAL);
                      //  me->SetReactState(REACT_PASSIVE);
                        events.ScheduleEvent(EVENT_TRANSFORM_REMOVE_PASSIVE, 4000);
                    }

                    me->CastSpell(me, SPELL_AFFLICTION_TRANSFORM);
                    Talk(TERONGOR_EVENT_03);
                    break;
                case 1: // Arcanist Jorra's - destruction
                    events.ScheduleEvent(EVENT_CHAOS_BOLT, 20000);
                    events.ScheduleEvent(EVENT_IMMOLATE, urand(10000, 14000));
                    events.ScheduleEvent(EVENT_CONFLAGRATE, urand(8000, 10000));
                    events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 24000);
                    events.ScheduleEvent(EVENT_INCINRATE, 16000);

                    if (Creature* nearest = me->FindNearestCreature(CREATURE_JORAA, 100.0f, true))
                    {
                       // me->CastSpell(nearest, SPELL_DRAIN_SOUL_VISUAL);
                       // me->SetReactState(REACT_PASSIVE);
                        events.ScheduleEvent(EVENT_TRANSFORM_REMOVE_PASSIVE, 4000);
                    }

                    me->CastSpell(me, SPELL_DESTRUCTION_TRANSFORM);
                    Talk(TERONGOR_EVENT_01);
                    break;
                case 2: // Vindication Iruun's - demonology
                    events.ScheduleEvent(EVENT_DOOM, urand(8000, 12000));
                    events.ScheduleEvent(EVENT_DEMONIC_LEAP, urand(10000, 14000));
                    events.ScheduleEvent(EVENT_CURSE_OF_EXHAUSTION, 18000);
                    events.ScheduleEvent(EVENT_CORRUPTION, urand(10000, 14000));
                    events.ScheduleEvent(EVENT_CHAOS_WAVE, 25000);
                    events.ScheduleEvent(EVENT_TOUCH_OF_CHAOS, 16000);

                    if (Creature* nearest = me->FindNearestCreature(CREATURE_IRUUN, 100.0f, true))
                    {
                        //me->CastSpell(nearest, SPELL_DRAIN_SOUL_VISUAL);
                       // me->SetReactState(REACT_PASSIVE);
                        events.ScheduleEvent(EVENT_TRANSFORM_REMOVE_PASSIVE, 4000);
                    }

                    me->CastSpell(me, SPELL_DEMONOLOGY_TRANSFORM);
                    Talk(TERONGOR_EVENT_02);
                    break;
                }
                break;
            }
        }
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            Talk(TERONGOR_AGGRO_01);

            //me->CastSpell(me, SPELL_SUMMON_ABYSSAL_DUMMY);
            if (me->GetMap()->IsHeroic())
            {
                Position pos;
                me->GetRandomNearPosition(pos, 10.0f);
                me->SummonCreature(TRIGGER_SUMMON_ABYSSAl, pos, TEMPSUMMON_MANUAL_DESPAWN);
            }
            events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(8000, 16000));
            events.ScheduleEvent(EVENT_CORRUPTION, urand(10000, 12000));
            events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 21000);
            events.ScheduleEvent(EVENT_DRAIN_LIFE, 16000);

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }
            me->RemoveAura(SPELL_TERONOGOR_SHIELD);

            instance->SetBossState(DATA_TERONOGOR, IN_PROGRESS);
        }
        void KilledUnit(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                if (roll_chance_i(50))
                    Talk(TERONGOR_KILL_01);
        }
        void JustDied(Unit* /*killer*/) override
        {
            Talk(TERONGOR_DEATH);
            _JustDied();

            if (instance)
                instance->SetBossState(DATA_KATHAAR, DONE);

            DespawnCreaturesInArea(CREATURE_FELBORNE_ABYSSAL, me);

            me->Respawn(true);
            me->setFaction(35);
            me->SetReactState(REACT_PASSIVE);
            me->CastSpell(me, 166453);
            me->SetCanFly(true);
            me->GetMotionMaster()->MoveTakeoff(0, me->GetPositionX(), me->GetPositionY(), 21.261f);
            me->DespawnOrUnsummon(4000);


            me->SummonGameObject(GAMEOBJECT_CHEST_AUCHENI, 1891.84f, 2973.80f, 16.844f, 5.664811f, 0,0,0,0,0);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TRANSFORM:
                    {
                        if (Creature * trigger = me->SummonCreature(60975, trigger_position_drain, TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            DoAction(ACTION_CHOOSE_POWER);
                        }

                        events.ScheduleEvent(EVENT_TRANSFORM_REMOVE_PASSIVE, 5000);
                        break;
                    }
                    case EVENT_SHADOW_BOLT:
                    {
                        me->CastSpell(me->getVictim(), SPELL_SHADOW_BOLT);
                        events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(6000, 8000));
                        break;
                    }
                    case EVENT_CORRUPTION:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_CORRUPTION_DUMMY);
                            events.ScheduleEvent(EVENT_CORRUPTION, urand(10000, 14000));
                        }
                        break;
                    }
                    case EVENT_DRAIN_LIFE:
                    {
                        me->CastSpell(me->getVictim(), SPELL_DRAIN_LIFE);
                        events.ScheduleEvent(EVENT_DRAIN_LIFE, 16000);
                        break;
                    }
                    case EVENT_CURSE_OF_EXHAUSTION:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_CURSE_OF_EXHAUSTION_DEBUFF);
                            events.ScheduleEvent(EVENT_CURSE_OF_EXHAUSTION, 12500);
                        }
                        break;
                    }
                    case EVENT_AGONY:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_AGONY);
                            events.ScheduleEvent(EVENT_AGONY, 15600);
                        }
                        break;
                    }
                    case EVENT_UNSTABLE_AFFLICTION:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_UNSTABLE_AFFLICTION);
                            events.ScheduleEvent(EVENT_UNSTABLE_AFFLICTION, 20000);
                        }
                        break;
                    }
                    case EVENT_SEED_OF_MALEVOLENCE:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_SEED_OF_MALEVOLENCE_APPLY_AURA);
                            events.ScheduleEvent(EVENT_SEED_OF_MALEVOLENCE, urand(22000, 25000));
                        }
                        break;
                    }
                    case EVENT_CHAOS_BOLT:
                    {
                        me->CastSpell(me->getVictim(), SPELL_CHAOS_BOLT);
                        events.ScheduleEvent(EVENT_CHAOS_BOLT, 20000);
                        break;
                    }
                    case EVENT_IMMOLATE:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_IMMOLATE);
                            events.ScheduleEvent(EVENT_IMMOLATE, 20000);
                            break;
                        }
                    }
                    case EVENT_CONFLAGRATE:
                    {
                        me->CastSpell(me->getVictim(), SPELL_CONFLAGRATE);
                        events.ScheduleEvent(EVENT_CONFLAGRATE, urand(15000, 20000));
                        break;
                    }
                    case EVENT_INCINRATE:
                    {
                        me->CastSpell(me->getVictim(), SPELL_INCINRATE);
                        events.ScheduleEvent(EVENT_INCINRATE, 16000);
                        break;
                    }
                    case EVENT_RAIN_OF_FIRE:
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_RAIN_OF_FIRE);
                            events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 25000);
                            break;
                        }
                    case EVENT_DOOM:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_DOOM_BUFF);
                            events.ScheduleEvent(EVENT_DOOM, urand(8000, 12000));
                            break;
                        }
                    }
                    case EVENT_DEMONIC_LEAP:
                    {
                        if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(random, SPELL_DEMONIC_LEAP_DUMMY);
                            break;
                        }
                    }
                    case EVENT_CHAOS_WAVE:
                    {
                        me->CastSpell(me->getVictim(), SPELL_CHAOS_WAVE_DUMMY);
                        events.ScheduleEvent(EVENT_CHAOS_WAVE, urand(8000, 10000));
                        break;
                    }
                    case EVENT_TOUCH_OF_CHAOS:
                    {
                        me->CastSpell(me->getVictim(), SPELL_TOUCH_OF_CHAOS_DUMMY);
                        events.ScheduleEvent(EVENT_TOUCH_OF_CHAOS, 16000);
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_terongorAI(creature);
    }
};
class auchindon_teronogor_durag : public CreatureScript
{
public:
    auchindon_teronogor_durag() : CreatureScript("auchindon_teronogor_durag") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
            me->GetMap()->SetObjectVisibility(1000.0f);
        }
        InstanceScript* instance;
        void Reset()
        {
            events.Reset();

            if (Creature* zipteq = me->FindNearestCreature(CREATURE_ZIPTEQ, 20.0f, true))
                zipteq->Respawn(true);

            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                if (Creature* trigger = me->SummonCreature(60975, teronogor->GetPositionX(), teronogor->GetPositionY(), teronogor->GetPositionZ(), teronogor->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
                    me->CastSpell(trigger, SPELL_DRAIN_SOUL_VISUAL);
            }
            //if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            // {
            //me->CastSpell(me, 150909);
            // }
            // Visual to cast on Terongor
        }

        void EnterCombat(Unit* attacker)
        {
            me->CastSpell(me, 159021);
            events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(8000, 16000));
            events.ScheduleEvent(EVENT_CORRUPTION, urand(10000, 14000));
            events.ScheduleEvent(EVENT_CHAOS_WAVE, urand(8000, 10000));
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_SHADOW_BOLT:
                me->CastSpell(me->getVictim(), SPELL_SHADOW_BOLT);
                events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(8000, 10000));
                break;
            case EVENT_CORRUPTION:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_CORRUPTION_DUMMY);
                    events.ScheduleEvent(EVENT_CORRUPTION, urand(10000, 14000));
                }
                break;
            case EVENT_CHAOS_WAVE:
                me->CastSpell(me->getVictim(), SPELL_CHAOS_WAVE_DUMMY);
                events.ScheduleEvent(EVENT_CHAOS_WAVE, urand(8000, 10000));
                break;
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
class auchindon_teronogor_gulkosh : public CreatureScript
{
public:
    auchindon_teronogor_gulkosh() : CreatureScript("auchindon_teronogor_gulkosh") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
            me->GetMap()->SetObjectVisibility(1000.0f);
        }
        InstanceScript* instance;
        void Reset()
        {
            if (Creature* shaddum = me->FindNearestCreature(CREATURE_SHAADUM, 20.0f, true))
                shaddum->Respawn(true);

            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                if (Creature* trigger = me->SummonCreature(60975, teronogor->GetPositionX(), teronogor->GetPositionY(), teronogor->GetPositionZ(), teronogor->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
                    me->CastSpell(trigger, SPELL_DRAIN_SOUL_VISUAL);
            }

            //me->CastSpell(me, 150909);
            // Visual to cast on Terongor
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_SHADOW_BOLT, 8000);
            events.ScheduleEvent(EVENT_UNSTABLE_AFFLICTION, urand(10000, 12000));
            events.ScheduleEvent(EVENT_DRAIN_LIFE, 16000);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_SHADOW_BOLT:
                me->CastSpell(me->getVictim(), SPELL_SHADOW_BOLT);
                events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(8000, 10000));
                break;
            case EVENT_UNSTABLE_AFFLICTION:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_UNSTABLE_AFFLICTION);
                    events.ScheduleEvent(EVENT_UNSTABLE_AFFLICTION, urand(10000, 14000));
                }
                break;
            case EVENT_DRAIN_LIFE:
                me->CastSpell(me->getVictim(), SPELL_DRAIN_LIFE);
                events.ScheduleEvent(EVENT_DRAIN_LIFE, 16000);
                break;
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
class auchindon_teronogor_shaadum : public CreatureScript
{
public:
    auchindon_teronogor_shaadum() : CreatureScript("auchindon_teronogor_shaadum") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }
        void Reset()
        {
            // Visual to cast on Terongor
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_WRATHCLEAVE, 10000);
            events.ScheduleEvent(EVENT_WRATHSTORM, urand(14000, 16000));
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_WRATHSTORM:
                me->CastSpell(me->getVictim(), SPELL_WRATH_STORM);
                events.ScheduleEvent(EVENT_WRATHSTORM, urand(8000, 10000));
                break;
            case EVENT_WRATHCLEAVE:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_WRATH_CLEAVE);
                    events.ScheduleEvent(EVENT_WRATHCLEAVE, urand(10000, 14000));
                }
                break;
            }
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
class auchindon_teronogor_gromkash : public CreatureScript
{
public:
    auchindon_teronogor_gromkash() : CreatureScript("auchindon_teronogor_gromkash") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
            me->GetMap()->SetObjectVisibility(1000.0f);
        }
        InstanceScript* instance;
        void Reset()
        {
            events.Reset();

            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                if (Creature* trigger = me->SummonCreature(60975, teronogor->GetPositionX(), teronogor->GetPositionY(), teronogor->GetPositionZ(), teronogor->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
                    me->CastSpell(trigger, SPELL_DRAIN_SOUL_VISUAL);
            }
            // Visual to cast on Terongor
            if (Creature* zashoo = me->FindNearestCreature(CREATURE_ZASHOO, 20.0f, true))
                zashoo->Respawn(true);
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_IMMOLATE, 8000);
            events.ScheduleEvent(EVENT_INCINRATE, urand(10000, 12000));
            events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 18000);

            if (Creature* zashoo = me->FindNearestCreature(CREATURE_ZASHOO, 20.0f, true))
                me->CastSpell(zashoo, 159024); // GOS
        }
        void JustDied(Unit* killer)
        {
            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                teronogor->RemoveAura(SPELL_TERONOGOR_SHIELD);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_IMMOLATE:
                me->CastSpell(me->getVictim(), SPELL_IMMOLATE);
                events.ScheduleEvent(EVENT_IMMOLATE, 8000);
                break;
            case EVENT_INCINRATE:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_INCINRATE);
                    events.ScheduleEvent(EVENT_INCINRATE, urand(10000, 12000));
                }
                break;
            case EVENT_RAIN_OF_FIRE:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_RAIN_OF_FIRE);
                    events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 18000);
                    break;
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
class auchindon_terongor_abyssal_dummy : public CreatureScript
{
public:
    auchindon_terongor_abyssal_dummy() : CreatureScript("auchindon_terongor_abyssal_dummy") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }
        InstanceScript* instance;

        uint32 abyssalsummon;
        uint32 explosion;
        void Reset()
        {
            abyssalsummon = 4000;
            explosion = 2000;

            me->CastSpell(me, SPELL_SUMMON_ABYSSAL_METEOR);
        }
        void UpdateAI(const uint32 diff)
        {
            if (abyssalsummon <= diff)
            {
                me->CastSpell(me, SPELL_SUMMON_ABYSSAL_SUMMON_SPELL);
                me->DespawnOrUnsummon(500);
                abyssalsummon = 500000;
            }
            else
                abyssalsummon -= diff;

            if (explosion)
            {
                me->CastSpell(me, SPELL_SUMMON_ABYSSAL_GROUND_EFFECT);
                explosion = 500000;
            }
            else
                explosion -= diff;
            
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
class auchindon_teronogor_spell_chaos_wave : public SpellScriptLoader
{
public:
    auchindon_teronogor_spell_chaos_wave() : SpellScriptLoader("auchindon_teronogor_spell_chaos_wave") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster() && !GetExplTargetUnit())
                return;

            GetCaster()->CastSpell(GetExplTargetUnit(), SPELL_CHAOS_WAVE_DMG);
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindon_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};
class auchindon_teronogor_spell_demonic_leap : public SpellScriptLoader
{
public:
    auchindon_teronogor_spell_demonic_leap() : SpellScriptLoader("auchindon_teronogor_spell_demonic_leap") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        void HandleAfterHit()
        {
            if (!GetCaster() && !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), SPELL_DEMONIC_LEAP_AREATRIGER);
        }

        void Register()
        {
            AfterHit += SpellHitFn(auchindon_spells::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};
class auchindon_soul_transportation_event : public BasicEvent
{
public:
    explicit auchindon_soul_transportation_event(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (obj)
        {
            if (InstanceScript* instance = obj->GetInstanceScript())
            {
                switch (modifier)
                {
                    // first soul transport
                case 0:
                    obj->GetMotionMaster()->MoveCharge(firstplatformfirstmove.GetPositionX(), firstplatformfirstmove.GetPositionY(), firstplatformfirstmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 1), obj->m_Events.CalculateTime(3000));

                    break;
                case 1:
                    obj->GetMotionMaster()->MoveCharge(firstplatformsecondmove.GetPositionX(), firstplatformsecondmove.GetPositionY(), firstplatformsecondmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 2), obj->m_Events.CalculateTime(3000));
                    break;
                case 2:
                    obj->GetMotionMaster()->MoveCharge(firstplatformthirdmove.GetPositionX(), firstplatformthirdmove.GetPositionY(), firstplatformthirdmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 100), obj->m_Events.CalculateTime(3000));
                    break;

                    // second soul transport
                case 4:
                    obj->GetMotionMaster()->MoveCharge(secondplatformthirdmove.GetPositionX(), secondplatformthirdmove.GetPositionY(), secondplatformthirdmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 5), obj->m_Events.CalculateTime(3000));
                    break;
                case 5:
                    obj->GetMotionMaster()->MoveCharge(secondplatformsecondmove.GetPositionX(), firstplatformsecondmove.GetPositionY(), firstplatformsecondmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 6), obj->m_Events.CalculateTime(3000));
                    break;
                case 6:
                    obj->GetMotionMaster()->MoveCharge(secondplatformthirdmove.GetPositionX(), secondplatformthirdmove.GetPositionY(), secondplatformthirdmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 100), obj->m_Events.CalculateTime(3000));
                    break;

                    // third soul transport
                case 7:
                    obj->GetMotionMaster()->MoveCharge(thirdplatformfirstmove.GetPositionX(), thirdplatformfirstmove.GetPositionY(), thirdplatformfirstmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 8), obj->m_Events.CalculateTime(3000));
                    break;
                case 8:
                    obj->GetMotionMaster()->MoveCharge(thirdplatformsecondmove.GetPositionX(), thirdplatformsecondmove.GetPositionY(), thirdplatformsecondmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 9), obj->m_Events.CalculateTime(3000));
                    break;
                case 9:
                    obj->GetMotionMaster()->MoveCharge(thirdplatformthirdmove.GetPositionX(), thirdplatformthirdmove.GetPositionY(), thirdplatformthirdmove.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 100), obj->m_Events.CalculateTime(3000));
                    break;

                    // fourth soul transport and last
                case 10:
                    obj->GetMotionMaster()->MoveCharge(fourthmovement.GetPositionX(), fourthmovement.GetPositionY(), fourthmovement.GetPositionZ(), 60.0f);
                    obj->m_Events.AddEvent(new auchindon_soul_transportation_event(obj, 100), obj->m_Events.CalculateTime(3000));
                    break;
                case 100:
                    obj->RemoveUnitMovementFlag(MOVEMENTFLAG_ROOT);

                    obj->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    obj->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                    obj->RemoveAura(SPELL_TRANSCEND);
                    break;
                }
            }
        }

        return true;
    }
private:
    Creature* storm;
    Unit* obj;
    int modifier;
    int Event;
};
class auchindon_teronogor_soul_transporter_gulkosh_first : public GameObjectScript
{
public:
    auchindon_teronogor_soul_transporter_gulkosh_first() : GameObjectScript("auchindon_teronogor_soul_transporter_gulkosh_first") { }

    bool OnGossipHello(Player* player, GameObject* gobject)
    {
        if (InstanceScript* instance = gobject->GetInstanceScript())
        {
            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                if (auchindon_boss_terongor::boss_terongorAI* linkAI = CAST_AI(auchindon_boss_terongor::boss_terongorAI, teronogor->GetAI()))
                {
                    if (linkAI->soultransport1)
                    {
                        player->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                        player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        player->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                        player->AddAura(SPELL_TRANSCEND, player);
                        player->m_Events.AddEvent(new auchindon_soul_transportation_event(player, 0), player->m_Events.CalculateTime(1000));
                        return true;
                    }
                }
            }
        }
        return true;
    }
};
class auchindon_teronogor_soul_transporter_gulkosh_second : public GameObjectScript
{
public:
    auchindon_teronogor_soul_transporter_gulkosh_second() : GameObjectScript("auchindon_teronogor_soul_transporter_gulkosh_second") { }

    bool OnGossipHello(Player* player, GameObject* gobject)
    {
        if (InstanceScript* instance = gobject->GetInstanceScript())
        {
            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                if (auchindon_boss_terongor::boss_terongorAI* linkAI = CAST_AI(auchindon_boss_terongor::boss_terongorAI, teronogor->GetAI()))
                {
                    if (linkAI->soultransport2)
                    {
                        player->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                        player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        player->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                        player->AddAura(SPELL_TRANSCEND, player);
                        player->m_Events.AddEvent(new auchindon_soul_transportation_event(player, 4), player->m_Events.CalculateTime(1000));
                        return true;
                    }
                }
            }
        }
        return true;
    }
};
class auchindon_teronogor_soul_transporter_gulkosh_third : public GameObjectScript
{
public:
    auchindon_teronogor_soul_transporter_gulkosh_third() : GameObjectScript("auchindon_teronogor_soul_transporter_gulkosh_third") { }

    bool OnGossipHello(Player* player, GameObject* gobject)
    {
        if (InstanceScript* instance = gobject->GetInstanceScript())
        {
            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                if (auchindon_boss_terongor::boss_terongorAI* linkAI = CAST_AI(auchindon_boss_terongor::boss_terongorAI, teronogor->GetAI()))
                {
                    if (linkAI->soultransport3)
                    {
                        player->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                        player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        player->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                        player->AddAura(SPELL_TRANSCEND, player);
                        player->m_Events.AddEvent(new auchindon_soul_transportation_event(player, 7), player->m_Events.CalculateTime(1000));
                        return true;
                    }
                }
            }
        }
        return true;
    }
};
class auchindon_teronogor_soul_transporter_gulkosh_fourth : public GameObjectScript
{
public:
    auchindon_teronogor_soul_transporter_gulkosh_fourth() : GameObjectScript("auchindon_teronogor_soul_transporter_gulkosh_fourth") { }

    bool OnGossipHello(Player* player, GameObject* gobject)
    {
        if (InstanceScript* instance = gobject->GetInstanceScript())
        {
            if (Creature* teronogor = instance->instance->GetCreature(instance->GetData64(DATA_TERONOGOR)))
            {
                if (auchindon_boss_terongor::boss_terongorAI* linkAI = CAST_AI(auchindon_boss_terongor::boss_terongorAI, teronogor->GetAI()))
                {
                    if (linkAI->soultransport4)
                    {
                        player->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                        player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        player->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                        player->AddAura(SPELL_TRANSCEND, player);
                        player->m_Events.AddEvent(new auchindon_soul_transportation_event(player, 10), player->m_Events.CalculateTime(1000));
                        return true;
                    }
                }
            }
        }
        return true;
    }
};
void AddSC_teronogoer()
{
    new auchindon_boss_terongor();
    new auchindon_teronogor_gromkash();
    new auchindon_terongor_abyssal_dummy();
    new auchindon_teronogor_durag();
    new auchindon_teronogor_gulkosh();
    new auchindon_teronogor_shaadum();
    new auchindon_teronogor_soul_transporter_gulkosh_first();
    new auchindon_teronogor_soul_transporter_gulkosh_second();
    new auchindon_teronogor_soul_transporter_gulkosh_third();
    new auchindon_teronogor_soul_transporter_gulkosh_fourth();
    new auchindon_teronogor_spell_chaos_wave();
    new auchindon_teronogor_spell_demonic_leap();
}