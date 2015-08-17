#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "auchindon.h"

Position KaatharCombatJump = { 1911.47f, 3152.13f, 30.972f, 1.550403f };
Position NyamiEscape5 = { 1801.23f, 3178.36f, 35.139f };
Position NyamiEscape4 = { 1912.99f, 3165.06f, 48.221f };
Position NyamiEscape3 = { 1891.66f, 3184.09f, 49.976f };
Position NyamiEscape2 = { 1911.14f, 3211.44f, 51.915f };
Position NyamiEscape1 = { 1935.88f, 3185.43f, 44.528f };

enum KaatharSpells
{
    SPELL_HALLOWED_GROUND = 154526,
    SPELL_HALLOWED_GROUND_VISUAL = 155646,
    SPELL_HALLOWED_GROUND_HACKED_VISUAL = 178266,
    SPELL_HALLOWED_GROUND_HACKED_VISUAL_2 = 174212,
    SPELL_HALLOWED_GROUND_HACKED_VISUAL_3 = 174184,
    SPELL_CONSECRATED_LIGHT = 153006,
    SPELL_CONSECRATED_LIGHT_DAMAGE = 156746,
    SPELL_HOLY_SHIELD_THROW = 153002,
    SPELL_HOLY_SHIELD_LOS = 153028,
    SPELL_HOLY_SHIELD_LOS_SPELLS = 153452,
    SPELL_HOLY_SHIELD_OFF_HAND_DISARM = 174205,
    SPELL_HOLY_SHIELD_TWO_HAND_DISARM = 174206,
    SPELL_HOLY_SHIELD_KNOCK_BACK = 153481,
    SPELL_HOLY_SHIELD_DAMAGE_FROM_KNOCK_BACK = 153480,
    SPELL_SANCTIFIED_STRIKE_DUMMY = 152954,
    SPELL_SANCTIFIED_GROUND_TICK_DAMAGE = 161457,
    SPELL_SANCTIFIED_GROUND_AURA = 153430,
    SPELL_SANCTIFIED_GROUND_DAMAGE_TICK = 161457,
    SPELL_SANCTIFIED_STRIKE_AREA_TRIGGER = 165064,
    SPELL_SANCTIFIED_STRIKE_AREA_TRIGGER_2 = 163559,
    SPELL_SANCITFIED_STRIKE_AREA_TRIGGER_3 = 165055,

    SPELL_EMOTE_ARCANE_CHANNELING = 32783,

    // Heroic
    SPELL_FATE = 157465,
};
enum KaatharEvents
{
    EVENT_HALLOWED_GROUND = 44,
    EVENT_HOLY_SHIELD = 45,
    EVENT_CONSECRATED_LIGHT = 46,
    EVENT_SANCTIFIED_STRIKE = 47,
    EVENT_HOLY_SHIELD_RETURN = 49,
    //hc
    EVENT_FATE = 48,

    // Triggers
    EVENT_CHECK_PLAYER = 60,
    EVENT_DECREASE_SIZE = 61,
};
enum KaatharActions
{
    ACTION_ACTIVATE_BOSS = 944,
    ACTION_FATE_HALLOWED_GROUND = 945,
};
enum KaatharTalks
{
    VIGILANT_KAATHER_INTRO = 18,  // None live who assault the holy Auchenai. (46436)
    VIGILANT_KAATHER_AGRO = 19,   // I will strike you down. (46434)
    VIGILANT_KAATHER_SPELL1 = 20, // Bathe in the glory of the Light! (46438)
    VIGILANT_KAATHER_SPELL2 = 21, // The light guide my Hand! (46439)
    VIGILANT_KAATHER_KILL = 22,   // Light guide you. (46434)
    VIGILANT_KAATHER_DEATH = 23,  // Auchindoun...is...doomed...(46435)
};
enum KaatharTriggers
{
    TRIGGER_HALLOWED_GROUND = 537324,
    TRIGGER_HOLY_SHIELD = 76071,
    TRIGGER_FISSURE_SUMMONER = 543536,
};

Position NewHomePosition = {1908.63f, 3163.94f, 30.799f, 1.498386f};

Position ThreeCorpses[2] = 
{
    {1749.40f, 3145.85f, 35.140f, 3.690412f},
    {1751.31f, 3141.03f, 35.140f, 3.278078f},
};
Position MagusAndDefenderHostile[2] =
{
    {1735.47f, 3139.28f, 35.140f, 0.185946f},
    {1738.86f, 3145.71f, 35.140f, 5.495237f},
};
Position SargereiDefenders[2] =
{
    {1713.76f, 3104.75f, 35.140f, 0.902229f},
    {1710.27f, 3108.19f, 35.140f, 0.815836f},
};
Position MagusWhoControlFootmans = {1708.01f, 3107.09f, 35.139f, 6.217793f};
Position ThreePrisoners[3] =
{
    {1700.59f, 3097.08f, 40.908f, 0.701154f},
    {1707.98f, 3103.10f, 42.982f, 1.499199f},
    {1714.57f, 3111.26f, 43.196f, 3.066774f},
};
Position ThreeHostileArbitrerMagusSoulPriest[3] =
{
    { 1671.63f, 3042.13f, 35.138f, 0.704871f },
    { 1678.88f, 3040.05f, 35.138f, 1.837415f },
    { 1668.86f, 3034.10f, 35.139f, 1.298632f },
};
Position CorpsesNearNyomi[12] = 
{
    {1673.98f, 2971.02f, 35.151f, 4.399558f},
    {1671.73f, 2971.23f, 35.200f, 4.517365f},
    {1674.24f, 2967.03f, 35.194f, 4.327302f},
    {1648.88f, 2967.87f, 35.211f, 4.877079f},
    {1651.50f, 2968.30f, 35.219f, 4.888861f},
    {1649.75f, 2937.92f, 35.215f, 1.334930f},
    {1644.28f, 2937.78f, 35.173f, 1.460592f},
    {1672.23f, 2938.41f, 35.208f, 1.825676f},
    {1674.24f, 2933.18f, 35.131f, 1.916782f},
    {1669.27f, 2931.85f, 34.764f, 1.751064f},
};
Position FourMagesThatSpawnAfterKaatharIsKaaput[5] = 
{
    {1846.22f, 3198.23f, 29.424f, 6.066411f},
    {1849.56f, 3203.73f, 29.424f, 6.023211f},
    {1844.11f, 3170.28f, 29.410f, 6.176361f},
    {1848.42f, 3162.75f, 29.423f, 0.003129f},
};
Position FourPositionAfterKaatharDeathForFourMages[4] =
{
    {1879.55f, 3201.58f, 34.418f},
    {1876.59f, 3191.82f, 31.418f},
    {1876.75f, 3171.40f, 31.418f},
    {1877.25f, 3165.11f, 31.418f},
};
Position PositionMageSpawning = {1857.07f, 3167.93f, 29.422f, 0.275066f};
Position PositionMageMoveTo = {1895.09f, 3183.77f, 30.799f}; 
Position TuulaniGobjectPortalSpawn = {1912.11f, 3183.67f, 30.799f, 3.173182f};
Position NyamiSpawnPosition = {1660.17f, 2953.36f, 34.291f, 1.413091f};
Position WardenAndGuards = {1666.69f, 3015.78f, 35.145f, 1.302555f};
Position GuardsAndWardens[2] =
{
    {1672.54f, 3012.73f, 35.139f, 1.318899f},
    {1659.39f, 3016.55f, 35.165f, 1.212869f},
};
Position WallInMiddleFromNyami = {1689.59f, 2953.11f, 35.613f, 3.139696f};
Position DefenderBehindMiddleWallOfNyami[4] = 
{
    {1696.44f, 2957.14f, 35.613f, 3.265362f},
    {2952.62f, 2952.80f, 35.613f, 3.120063f},
    {1695.18f, 2947.33f, 35.613f, 3.040738f},
    {1692.70f, 2953.55f, 35.613f, 3.140484f},
};

class tuulani_2nd_event_nyaomi_escape : public BasicEvent
{
public:
    explicit tuulani_2nd_event_nyaomi_escape(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (obj)
        {
            if (InstanceScript* instance = obj->GetInstanceScript())
            {
                if (Creature* tuulina = instance->instance->GetCreature(instance->GetData64(DATA_TUULANI)))
                {
                    if (Creature* Nyami = instance->instance->GetCreature(instance->GetData64(DATA_NYAMI)))
                    {
                        if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
                        {
                            switch (modifier)
                            {
                                case 100:
                                {
                                    Nyami->AI()->Talk(NYAMI_TALK_6);
                                    Nyami->CastSpell(Nyami, SPELL_NYAMI_EXPLODE_CRYSTAL);
                                    tuulina->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(tuulina, 101), tuulina->m_Events.CalculateTime(7000));
                                    break;
                                }
                                case 101:
                                {
                                    Nyami->AI()->Talk(NYAMI_TALK_7);
                                    tuulina->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(tuulina, 0), tuulina->m_Events.CalculateTime(9000));
                                    break;
                                }
                                case 0:
                                {

                                    Nyami->AI()->Talk(NYAMI_TALK_8);       
                                    Nyami->GetMotionMaster()->MovePoint(5, NyamiEscape1);
                                    break;
                                }
                                case 1:
                                {
                                    Nyami->GetMotionMaster()->MovePoint(6, NyamiEscape2);
                                    break;
                                }
                                case 2:
                                {
                                    Nyami->RemoveAura(SPELL_TUULANI_CAPTURED_VOID_PRISON);
                                    Nyami->AddAura(SPELL_DISPERSION_VISUAL_NYAMI, Nyami);
                                    Nyami->SetSpeed(MOVE_RUN, 10.0f, true);

                                    Nyami->CastSpell(Nyami, SPELL_CRYSTAL_EARTHQUAKE);
                                   // tuulina->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(tuulina, 7), tuulina->m_Events.CalculateTime(14000));

                                    if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
                                    {
                                        Kaathar->setFaction(16);
                                        Kaathar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                                        // Kaathar->SetReactState(REACT_AGGRESSIVE);
                                        Kaathar->GetMotionMaster()->MoveJump(KaatharCombatJump.GetPositionX(), KaatharCombatJump.GetPositionY(), KaatharCombatJump.GetPositionZ(), 10.0f, 10.0f, 10.0f, 0);
                                    }
                                    Nyami->GetMotionMaster()->MovePoint(7, NyamiEscape3);
                                    break;
                                }
                                case 3:
                                {                                  
                                    Nyami->GetMotionMaster()->MovePoint(8, NyamiEscape4);
                                    break;
                                }
                                case 4:
                                {
                                   // tuulina->AI()->Talk(TUULANI_TALK_13);
                                    tuulina->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(tuulina, 6), tuulina->m_Events.CalculateTime(500));
                                    break;
                                }
                                case 6:
                                {
                                    tuulina->AI()->Talk(TUULANI_TALK_10);
                                    tuulina->AddAura(SPELL_TUULANI_CAPTURED_VOID_PRISON, tuulina);
                                    tuulina->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(tuulina, 7), tuulina->m_Events.CalculateTime(8000));
                                    break;
                                }
                                case 7:
                                {                                          
                                    tuulina->AI()->Talk(TUULANI_TALK_11);
                                    tuulina->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(tuulina, 8), tuulina->m_Events.CalculateTime(8000));
                                    break;
                                }
                                case 8:
                                {
                                    tuulina->AI()->Talk(TUULANI_TALK_9);                                
                                    Nyami->GetMotionMaster()->MovePoint(9, NyamiEscape5);
                                    Nyami->DespawnOrUnsummon(10000);
                                    tuulina->AddAura(SPELL_TUULANI_CAPTURED_VOID_PRISON, tuulina);
                                    break;
                                }
                            }
                        }
                    }
                    return true;
                }
            }
        }
    }
private:
    Creature* storm;
    Unit* obj;
    int modifier;
    int Event;
};
class auchindon_mob_nyami : public CreatureScript
{
public:
    auchindon_mob_nyami() : CreatureScript("auchindon_mob_nyami") {}

    struct auchindon_creatures : public CreatureAI
    {
        auchindon_creatures(Creature* creature) : CreatureAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }
        EventMap events;
        InstanceScript* pInstance;
        bool startevent;
        void MovementInform(uint32 /*type*/, uint32 id)
        {
            switch (id)
            {
            case 5:
                me->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(me, 1), me->m_Events.CalculateTime(200));
                break;
            case 6:
                me->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(me, 2), me->m_Events.CalculateTime(200));
                break;
            case 7:
                me->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(me, 3), me->m_Events.CalculateTime(200));
                break;
            case 8:
                me->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(me, 4), me->m_Events.CalculateTime(200));
                break;
            }
        }
        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                default:
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
class kaathar_death_post_nyami : public BasicEvent
{
public:
    explicit kaathar_death_post_nyami(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (obj)
        {
            if (InstanceScript* instance = obj->GetInstanceScript())
            {
                if (Creature* tuulina = instance->instance->GetCreature(instance->GetData64(DATA_TUULANI)))
                {
                        switch (modifier)
                        {
                            case 0:
                            {
                                tuulina->AI()->Talk(TUULANI_TALK_14);
                                tuulina->RemoveAura(154840); // void prison
                                for (int i = 0; i < 4; i++)
                                {
                                    Creature* defender = obj->SummonCreature(CREATURE_AUCHENI_DEFENDER, FourMagesThatSpawnAfterKaatharIsKaaput[i], TEMPSUMMON_MANUAL_DESPAWN);
                                    defender->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 505);
                                    defender->GetMotionMaster()->MovePoint(0, FourPositionAfterKaatharDeathForFourMages[i]);
                                }
                                Creature* magus = obj->SummonCreature(CREATURE_AUCHENI_MAGUS, PositionMageSpawning, TEMPSUMMON_MANUAL_DESPAWN);
                                if (magus)
                                {
                                    magus->GetMotionMaster()->MovePoint(0, PositionMageMoveTo);
                                    magus->m_Events.AddEvent(new kaathar_death_post_nyami(magus, 1), magus->m_Events.CalculateTime(7000));
                                }
                               
                                break;
                            }
                            case 1:
                            {
                                std::list<Creature*> dispeonsor;
                                // Spawn Hostile Monsters in the right hallway
                                     
                                Creature* nyami = tuulina->SummonCreature(BOSS_NYAMI, NyamiSpawnPosition, TEMPSUMMON_MANUAL_DESPAWN);
                                dispeonsor.push_back(nyami);

                                // Corpses
                                for (int i = 0; i < 2; i++)
                                {
                                   Creature* corpses =  tuulina->SummonCreature(CREATURE_AUCHENI_DEFENDER, ThreeCorpses[i], TEMPSUMMON_MANUAL_DESPAWN);

                                   if (corpses)
                                   {
                                       corpses->SetReactState(REACT_PASSIVE);
                                       corpses->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                                       corpses->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                       corpses->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                                       corpses->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                       corpses->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                                       dispeonsor.push_back(corpses);
                                   }
                                }
                                // Hostile near Two corpses
                                Creature* Magus = tuulina->SummonCreature(CREATURE_SARGEREI_MAGUS, MagusAndDefenderHostile[0], TEMPSUMMON_MANUAL_DESPAWN);
                                Creature* Defender = tuulina->SummonCreature(CREATURE_SARGEREI_DEFENDER, MagusAndDefenderHostile[1], TEMPSUMMON_MANUAL_DESPAWN);
                                if (Magus)
                                {
                                    Magus->CastSpell(Magus, 157794);
                                    dispeonsor.push_back(Magus);
                                }  
                                // Two defender
                                for (int i = 0; i < 1; i++)
                                {
                                    Creature* TwoDefenders = tuulina->SummonCreature(CREATURE_SARGEREI_DEFENDER, SargereiDefenders[i], TEMPSUMMON_MANUAL_DESPAWN);
                                    dispeonsor.push_back(TwoDefenders);
                                }
                                // Magus who control footmans
                                Creature* MagusWhoControlFootman = tuulina->SummonCreature(CREATURE_SARGEREI_MAGUS, MagusWhoControlFootmans, TEMPSUMMON_MANUAL_DESPAWN);
                                if (MagusWhoControlFootman)
                                {
                                    MagusWhoControlFootman->CastSpell(MagusWhoControlFootman, SPELL_ARCANE_CHANNELING);
                                    dispeonsor.push_back(MagusWhoControlFootman);
                                }
                                // Three prisonners
                                for (int i = 0; i <= 2; i++)
                                {
                                    Creature* prisoners = tuulina->SummonCreature(CREATURE_AUCHENI_SOUL_PRIEST, ThreePrisoners[i], TEMPSUMMON_MANUAL_DESPAWN);

                                    if (prisoners)
                                    {
                                        prisoners->SetCanFly(true);
                                        prisoners->SetReactState(REACT_PASSIVE);
                                        prisoners->CastSpell(prisoners, SPELL_PRISON_AURA);
                                        prisoners->CastSpell(prisoners, SPELL_STRANGULATE);
                                        prisoners->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                        prisoners->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                                        prisoners->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                        prisoners->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);   
                                        dispeonsor.push_back(prisoners);
                                    }
                                }
                                // Twelve prisoners (cosmetic)
                                for (int i = 0; i <= 11; i++)
                                {
                                    Creature* prisonerstwelve = tuulina->SummonCreature(CREATURE_AUCHENI_DEFENDER, CorpsesNearNyomi[i], TEMPSUMMON_MANUAL_DESPAWN);

                                    if (prisonerstwelve)
                                    {
                                        prisonerstwelve->SetReactState(REACT_PASSIVE);
                                        prisonerstwelve->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                                        prisonerstwelve->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                        prisonerstwelve->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                                        prisonerstwelve->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                        prisonerstwelve->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                                        dispeonsor.push_back(prisonerstwelve);
                                    }
                                }
                                // Pack of arbitrer and magus and defender.
                                //tuulina->SummonCreature(CREATURE_);
                                Creature* spiteful_arbitrer = tuulina->SummonCreature(CREATURE_SPITEFUL_ARBITRER_TRASH, ThreeHostileArbitrerMagusSoulPriest[0], TEMPSUMMON_MANUAL_DESPAWN);
                                Creature* saregeri_magus = tuulina->SummonCreature(CREATURE_SARGEREI_MAGUS, ThreeHostileArbitrerMagusSoulPriest[1], TEMPSUMMON_MANUAL_DESPAWN);
                                Creature* sargerei_soul = tuulina->SummonCreature(CREATURE_SARGERI_SOUL_PRIEST, ThreeHostileArbitrerMagusSoulPriest[2], TEMPSUMMON_MANUAL_DESPAWN);

                                dispeonsor.push_back(spiteful_arbitrer);
                                dispeonsor.push_back(saregeri_magus);
                                dispeonsor.push_back(sargerei_soul);


                                Creature* wardenBeforeGate = tuulina->SummonCreature(CREATURE_SARGERI_WARDEN, WardenAndGuards, TEMPSUMMON_MANUAL_DESPAWN);
                                dispeonsor.push_back(wardenBeforeGate);
                                for (int i = 0; i < 2; i++)
                                {
                                   Creature* sargeri_defender = tuulina->SummonCreature(CREATURE_SARGEREI_DEFENDER, GuardsAndWardens[i], TEMPSUMMON_MANUAL_DESPAWN);
                                   dispeonsor.push_back(sargeri_defender);
                                }

                                // cosmetic
                                tuulina->SummonGameObject(GAMEOBJECT_HOLY_WALL, WallInMiddleFromNyami.GetPositionX(), WallInMiddleFromNyami.GetPositionY(), WallInMiddleFromNyami.GetPositionZ(), WallInMiddleFromNyami.GetOrientation(), 0,0,0,0,0);
                                for (int i = 0; i < 4; i++)
                                {           
                                    Creature* defendersbehindthewall /*got reference :3*/ = tuulina->SummonCreature(CREATURE_AUCHENI_WARDEN, DefenderBehindMiddleWallOfNyami[i], TEMPSUMMON_MANUAL_DESPAWN);
                               
                                    if (Creature* trigger_invisible_stalker = defendersbehindthewall->FindNearestCreature(TRIGGER_LIGHT_WALL_TARGETS, 9.0F))
                                        defendersbehindthewall->CastSpell(trigger_invisible_stalker, SPELL_HOLY_BEAM);         

                                    dispeonsor.push_back(defendersbehindthewall);
                                }


                                tuulina->AI()->Talk(TUULANI_TALK_15);
                                // Magus
                                obj->CastSpell(obj, SPELL_EMOTE_ARCANE_CHANNELING);
                                obj->SummonGameObject(GAMEOBJECT_TALADOR_PORTAL, TuulaniGobjectPortalSpawn.GetPositionX(), TuulaniGobjectPortalSpawn.GetPositionY(), TuulaniGobjectPortalSpawn.GetPositionZ(), TuulaniGobjectPortalSpawn.GetOrientation(), 0, 0, 0, 0, 0);
                                obj->m_Events.AddEvent(new kaathar_death_post_nyami(obj, 2), obj->m_Events.CalculateTime(7000));

                                if (dispeonsor.empty())
                                    return false;

                                for (auto itr : dispeonsor)
                                {
                                    itr->SetRespawnDelay(1000000);
                                    itr->SetCurrentEquipmentId(1);                             
                                }
                                break;
                            }
                            case 2:
                            {
                                /*
                                std::list<Creature*> trigger_corpses;
                                BossNyami->GetCreatureListWithEntryInGrid(trigger_corpses, TRIGGER_CORPSES_NYAMI_FIGHT, 100.0f);

                                if (trigger_corpses.empty())
                                    return false;

                                std::list<Creature*>::const_iterator it = trigger_corpses.begin();
                                std::advance(it, urand(0, trigger_corpses.size() - 1));

                                BossNyami->CastSpell((*it), SPELL_NYAMI_ESCAPE);   
                                */
                                break;
                            }
                        }
                    return true;
                }
            }
        }
    }
private:
    Creature* storm;
    Unit* obj;
    int modifier;
    int Event;
};
class auchindon_boss_kaathar : public CreatureScript
{
public:
    auchindon_boss_kaathar() : CreatureScript("auchindon_boss_kaathar") { }

    struct boss_kaatharAI : public BossAI
    {
        boss_kaatharAI(Creature* creature) : BossAI(creature, DATA_KATHAAR)
        {
           // me->AI()->Reset();
            me->setFaction(35);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            //me->SetReactState(REACT_PASSIVE);

            // Get Barrier Down
            if (GameObject* holybarrier = instance->instance->GetGameObject(instance->GetData64(DATA_HOLY_BARRIER)))
            {
                // Activate
                holybarrier->SetLootState(GO_READY);
                holybarrier->UseDoorOrButton();
            }
            
            me->CastSpell(me, SPELL_GUARD);
            counting = 0;
            introdone = false;
            intro = false;
        }

        uint32 counting;
        bool introdone;
        bool intro;
        void Reset() override
        {
            _Reset();
            me->SetCurrentEquipmentId(77819); // Equipment Id
           
            summons.DespawnAll();
            events.Reset();
        }
        void JustReachedHome() override
        {
            _JustReachedHome();
            // Get Barrier Down
            if (GameObject* holybarrier = instance->instance->GetGameObject(instance->GetData64(DATA_HOLY_BARRIER)))
            {
                // Activate
                holybarrier->SetLootState(GO_READY);
                holybarrier->UseDoorOrButton();
            }

            if (instance)
                instance->SetBossState(DATA_KATHAAR, FAIL);
        }
        void MoveInLineOfSight(Unit* who) override
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && !intro && me->IsWithinDistInMap(who, 10.0f) && introdone)
            {
                intro = true;
                Talk(VIGILANT_KAATHER_INTRO);

                me->SetHomePosition(NewHomePosition);
            }
        }
        void KilledUnit(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                Talk(VIGILANT_KAATHER_KILL);
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
        void EnterCombat(Unit* who) override
        {
            _EnterCombat();
            Talk(VIGILANT_KAATHER_AGRO);
            // Get Barrier Down
            if (GameObject* holybarrier = instance->instance->GetGameObject(instance->GetData64(DATA_HOLY_BARRIER)))
            {
                // Activate
                holybarrier->SetLootState(GO_READY);
                holybarrier->UseDoorOrButton();
            }
            events.ScheduleEvent(EVENT_HALLOWED_GROUND, urand(12000, 17000));
            events.ScheduleEvent(EVENT_SANCTIFIED_STRIKE, 8000);
            events.ScheduleEvent(EVENT_HOLY_SHIELD, 30000);

            if (me->GetMap()->IsHeroic())
                events.ScheduleEvent(EVENT_FATE, 45000);

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }

            instance->SetBossState(DATA_KATHAAR, IN_PROGRESS);
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_COUNT_PRE_1ST_BOSS_KILL:
                counting = counting + 1;

                if (counting == 12)
                {
                    me->RemoveAura(SPELL_GUARD);

                    if (Creature* tuulina = instance->instance->GetCreature(instance->GetData64(DATA_TUULANI)))
                    {
                        if (Creature* Nyami = instance->instance->GetCreature(instance->GetData64(DATA_NYAMI)))
                        {
                            tuulina->m_Events.KillAllEvents(true);
                            Nyami->m_Events.KillAllEvents(true);
                        }
                    }

                    introdone = true;
                    me->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(me, 100), me->m_Events.CalculateTime(500));
                }
                break;
            }
        }
        void ChangeToHostile()
        {
            DespawnCreaturesInArea(CREATURE_AUCHENI_CLERIC, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_MAGUS, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_DEFENDER, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_SOUL_PRIEST, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_CLERIC, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_ARBITER, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_SOUL_PRIEST, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_MAGUS, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_RITUALIST, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_WARDEN, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_ZEALOT, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_HOPLITE, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_VIGILIANT, me);
            DespawnCreaturesInArea(CREATURE_AUCHENI_MAGUS_2, me);
        }
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
            Talk(VIGILANT_KAATHER_DEATH);

            ChangeToHostile();

            if (instance)
                instance->SetBossState(DATA_KATHAAR, DONE);

            me->m_Events.AddEvent(new kaathar_death_post_nyami(me, 0), me->m_Events.CalculateTime(2000));
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
                    case EVENT_FATE:
                    {
                        me->CastSpell(me, SPELL_FATE);
                        me->MonsterTextEmote("|cffff0000[Fate]|cfffaeb00!", me->GetGUID(), true);
                        events.ScheduleEvent(EVENT_FATE, 30000);
                        break;
                    }
                    case EVENT_HALLOWED_GROUND:
                    {
                        Position pos;
                        me->GetRandomNearPosition(pos, 40.0f);

                        Creature* trigger = me->SummonCreature(TRIGGER_HALLOWED_GROUND, pos, TEMPSUMMON_MANUAL_DESPAWN);
                        me->CastSpell(trigger, SPELL_HALLOWED_GROUND_VISUAL);

                        events.ScheduleEvent(EVENT_HALLOWED_GROUND, 8000);
                        break;
                    }
                    case EVENT_HOLY_SHIELD:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            Talk(VIGILANT_KAATHER_SPELL2);
                            me->CastSpell(target, SPELL_HOLY_SHIELD_THROW);

                            events.ScheduleEvent(EVENT_HOLY_SHIELD, 30000);
                            events.ScheduleEvent(EVENT_CONSECRATED_LIGHT, 4000);
                            events.ScheduleEvent(EVENT_HOLY_SHIELD_RETURN, 14000);
                            
                            if (me->HasAura(SPELL_HOLY_SHIELD_OFF_HAND_DISARM))
                                me->AddAura(SPELL_HOLY_SHIELD_TWO_HAND_DISARM, me);
                            else
                                me->AddAura(SPELL_HOLY_SHIELD_OFF_HAND_DISARM, me);

                            std::string str;
                            str += "Vigilant kaathar hurls his |cffff0000[Holy Shield]|cfffaeb00! at ";
                            str += target->GetName();

                            me->MonsterTextEmote(str.c_str(), me->GetGUID(), true);
                        }
                        break;
                    }
                    case EVENT_HOLY_SHIELD_RETURN:
                    {
                        if (Creature* Shield = me->FindNearestCreature(TRIGGER_HOLY_SHIELD, 50.0f, true))
                        {
                            Shield->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 20.0f, 10.0f, 10.0f);
                            Shield->DespawnOrUnsummon(3000);

                            me->SetCurrentEquipmentId(77819); // Equipment Id

                            if (me->HasAura(SPELL_HOLY_SHIELD_OFF_HAND_DISARM))
                                me->RemoveAura(SPELL_HOLY_SHIELD_OFF_HAND_DISARM);
                            else if (me->HasAura(SPELL_HOLY_SHIELD_TWO_HAND_DISARM))
                                me->RemoveAura(SPELL_HOLY_SHIELD_TWO_HAND_DISARM);

                            DespawnCreaturesInArea(TRIGGER_HOLY_SHIELD, me);
                        }
                        break;
                    }
                    case EVENT_CONSECRATED_LIGHT:
                    {
                        Talk(VIGILANT_KAATHER_SPELL1);
                        DoCast(SPELL_CONSECRATED_LIGHT);

                        me->MonsterTextEmote("Vigilant Kaathar readies |cffff0000[Consecrated Light]|cfffaeb00!. Hide!", me->GetGUID(), true);                  
                        break;
                    }
                    case EVENT_SANCTIFIED_STRIKE:
                    {
                        DoCastVictim(SPELL_SANCTIFIED_STRIKE_DUMMY);
                        
                        float pos_x = me->GetPositionX();
                        float pos_y = me->GetPositionY();
                        float orientation = me->GetOrientation() * M_PI;
                        for (int i = 0; i <= 10; i++)
                        {                 
                            if (i == 9 || i == 5 || i == 6)
                                orientation = orientation / 4;

                            me->SummonCreature(TRIGGER_FISSURE_SUMMONER, pos_x, pos_y, me->GetPositionZ(), orientation, TEMPSUMMON_TIMED_DESPAWN, 15000);

                            pos_x +=  frand(0.5f, 1.8f);
                            pos_y +=  frand(0.7f, 1.9f);
                        }
                        for (int i = 0; i <= 4; i++)
                        {
                            pos_x -= 3;
                            pos_y -= 2.7;

                            me->SummonCreature(TRIGGER_FISSURE_SUMMONER, pos_x, pos_y, me->GetPositionZ(), orientation / 2 + i, TEMPSUMMON_TIMED_DESPAWN, 15000);
                        }
                                                                     
                        events.ScheduleEvent(EVENT_SANCTIFIED_STRIKE, 8000);
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_kaatharAI(creature);
    }
};
class auchindon_kaathar_areatrigger_fissure : public AreaTriggerEntityScript
{
public:
    auchindon_kaathar_areatrigger_fissure() : AreaTriggerEntityScript("auchindon_kaathar_areatrigger_fissure")
    {
    }

    int diff = 1500;
    std::list<uint64> m_Targets;
    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (diff <= p_Time)
        {
            std::list<Player*> PL_list;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, PL_list, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

            if (PL_list.empty())
                return;

            for (std::list<Player*>::const_iterator itr = PL_list.begin(); itr != PL_list.end(); ++itr)
            {
                if (!(*itr)->HasAura(SPELL_SANCTIFIED_GROUND_AURA))
                {
                    (*itr)->CastSpell((*itr), SPELL_SANCTIFIED_GROUND_AURA);
                    m_Targets.push_back((*itr)->GetGUID());

                    if ((*itr)->HasAura(SPELL_SANCTIFIED_GROUND_AURA))
                    {
                        AuraPtr aura = (*itr)->GetAura(SPELL_SANCTIFIED_GROUND_AURA);

                        if (aura)
                        {
                            aura->SetDuration(3);
                        }
                    }
                }
            }
            diff = 1500;
        }
        else
        {
            diff -= p_Time;
        }
    }
    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        for (auto l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->HasAura(SPELL_SANCTIFIED_GROUND_AURA))
                l_Target->RemoveAura(SPELL_SANCTIFIED_GROUND_AURA);
        }
    }

    auchindon_kaathar_areatrigger_fissure* GetAI() const
    {
        return new auchindon_kaathar_areatrigger_fissure();
    }
};
class auchindon_trigger_hallowed_ground : public CreatureScript
{
public:
    auchindon_trigger_hallowed_ground() : CreatureScript("auchindon_trigger_hallowed_ground") { }

    struct trigger_hallowed_groundAI : public Scripted_NoMovementAI
    {
        trigger_hallowed_groundAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);

            me->SetObjectScale(1.0f);
        }
        void Reset() override
        {
            me->CastSpell(me, SPELL_HALLOWED_GROUND_HACKED_VISUAL);
            me->CastSpell(me, SPELL_HALLOWED_GROUND_HACKED_VISUAL_2);
            me->CastSpell(me, SPELL_HALLOWED_GROUND_HACKED_VISUAL_3);

            me->setFaction(16);
            events.ScheduleEvent(EVENT_CHECK_PLAYER, 4000);
            //events.ScheduleEvent(EVENT_DECREASE_SIZE, 50);
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_FATE_HALLOWED_GROUND:
                DoCast(me, SPELL_HALLOWED_GROUND);
                me->DespawnOrUnsummon(800);
                break;
            }
        }
        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_PLAYER:
                    if (Player* player = me->FindNearestPlayer(6.0f, true))
                    {
                        if (player->IsWithinDistInMap(me, 6.0f))
                        {
                            DoCast(me, SPELL_HALLOWED_GROUND);
                            me->DespawnOrUnsummon(800);
                        }
                        else
                            events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                    }
                    else
                        events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                    break;
                    /*
                case EVENT_DECREASE_SIZE:
                {
                    if (me->GetObjectSize() > 1.0f)
                    {
                        float c_scale = me->GetObjectSize();
                        float r_scale = 0.1;
                        float scale = c_scale - r_scale;

                        me->SetObjectScale(scale);
                    }
                    events.ScheduleEvent(EVENT_DECREASE_SIZE, 1000); // increase *
                    break;
                }
                */
                }
            }
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new trigger_hallowed_groundAI(creature);
    }
};
class auchindon_trigger_holy_shield : public CreatureScript
{
public:
    auchindon_trigger_holy_shield() : CreatureScript("auchindon_trigger_holy_shield") { }

    struct trigger_holy_shieldAI : public Scripted_NoMovementAI
    {
        trigger_holy_shieldAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(16);

            me->SetDisplayId(11686);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

            instance = creature->GetInstanceScript();          
        }
        InstanceScript* instance;
        void Reset() override
        {
            me->CastSpell(me, SPELL_HOLY_SHIELD_LOS);

            // Knockback
            std::list<Player*> player_list;
            me->GetPlayerListInGrid(player_list, 6.5f);

            if (player_list.empty())
                return;

            for (auto itr : player_list)
            {
                itr->CastSpell(itr, SPELL_HOLY_SHIELD_KNOCK_BACK);
                me->CastSpell(itr, SPELL_HOLY_SHIELD_DAMAGE_FROM_KNOCK_BACK);
            }
        }
        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                }
            }
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new trigger_holy_shieldAI(creature);
    }
};
class auchindon_trigger_spawn_fissures : public CreatureScript
{
public:
    auchindon_trigger_spawn_fissures() : CreatureScript("auchindon_trigger_spawn_fissures") { }

    struct trigger_spawn_fissuresAI : public Scripted_NoMovementAI
    {
        trigger_spawn_fissuresAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(16);

            me->SetDisplayId(11686);
            instance = creature->GetInstanceScript();
        }
        InstanceScript* instance;
        void Reset() override
        {
            me->CastSpell(me, 165065);
        }
        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                }
            }
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new trigger_spawn_fissuresAI(creature);
    }
};
// called by 153006 
class auchindon_spell_consecrated_light : public SpellScriptLoader
{
public:
    auchindon_spell_consecrated_light() : SpellScriptLoader("auchindon_spell_consecrated_light") { }

    class auchindon_spells : public AuraScript
    {
        PrepareAuraScript(auchindon_spells);

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            if (GetCaster())
            {
                InstanceScript* instance = GetCaster()->GetInstanceScript();

                if (!instance)
                    return;

                std::list<Player*> pl_list;
                GetCaster()->GetPlayerListInGrid(pl_list, 500.0f);

                if (pl_list.empty())
                    return;                

                for (auto itr : pl_list)
                {
                    /*
                    if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
                        if (Creature* shield = GetCaster()->FindNearestCreature(TRIGGER_HOLY_SHIELD, 200.0f))
                            if (Kaathar->isInBack(shield, M_PI * 2))
                                if (shield->isInFront(itr, M_PI * 2))
                                    return;
                                    */

                     itr->CastSpell(itr, SPELL_CONSECRATED_LIGHT_DAMAGE, true);
                }
            }
        }

    private:
  
        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_spells::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };
    AuraScript* GetAuraScript() const
    {
        return new auchindon_spells();
    }
};
class auchindon_spell_conscreated_damage : public SpellScriptLoader
{
public:
    auchindon_spell_conscreated_damage() : SpellScriptLoader("auchindon_spell_conscreated_damage") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        void RecalculateDamage(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() && !GetHitUnit())
                return;

            InstanceScript* instance = GetCaster()->GetInstanceScript();

            if (!instance)
                return;

            if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
                if (Creature* shield = GetCaster()->FindNearestCreature(TRIGGER_HOLY_SHIELD, 200.0f))
                    if (shield->IsInBetween(GetHitUnit(), Kaathar))
                          SetHitDamage(0);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindon_spells::RecalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};
class auchindon_spell_shield_spawn_trigger : public SpellScriptLoader
{
public:
    auchindon_spell_shield_spawn_trigger() : SpellScriptLoader("auchindon_spell_shield_spawn_trigger") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);

        void HandleOnHit()
        {
            if (GetCaster() || GetHitUnit())
            {
                Position pos;
                GetHitUnit()->GetPosition(&pos);

               //Creature* trigger =  GetCaster()->SummonCreature(TRIGGER_HOLY_SHIELD, pos, TEMPSUMMON_MANUAL_DESPAWN);                          
            }
        }
        void Register()
        {
            OnHit += SpellHitFn(spells_auchindon::HandleOnHit);
        }
    };
    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};
class auchindon_spell_sanctified_ground_periodic_dummy : public SpellScriptLoader
{
public:
    auchindon_spell_sanctified_ground_periodic_dummy() : SpellScriptLoader("auchindon_spell_sanctified_ground_periodic_dummy") { }

    class auchindon_auras : public AuraScript
    {
        PrepareAuraScript(auchindon_auras);

        void HandlePeriodic(constAuraEffectPtr aurEff)
        {
            PreventDefaultAction();
            if (GetCaster() && GetTarget())
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_SANCTIFIED_GROUND_DAMAGE_TICK);
            }
        }
        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_auras::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new auchindon_auras();
    }
};
class auchindon_spell_fate : public SpellScriptLoader
{
public:
    auchindon_spell_fate() : SpellScriptLoader("auchindon_spell_fate") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            std::list<Creature*> hallowed_ground_triggers;

            GetCaster()->GetCreatureListWithEntryInGrid(hallowed_ground_triggers, TRIGGER_HALLOWED_GROUND, 150.0f);

            if (hallowed_ground_triggers.empty())
                return;

            for (auto itr : hallowed_ground_triggers)
            {
                itr->GetAI()->DoAction(ACTION_FATE_HALLOWED_GROUND);
            }
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
class auchindon_kaathar_hallowed_ground_visual : public SpellScriptLoader
{
public:
    auchindon_kaathar_hallowed_ground_visual() : SpellScriptLoader("auchindon_kaathar_hallowed_ground_visual") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            //spell->Effects[0].TargetA = 1;
            //spell->Effects[0].TargetB = 0;
            return true;
        }
        void HandleTriggerMissile(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindon_spells::HandleTriggerMissile, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};
void AddSC_kaathar()
{
    new auchindon_boss_kaathar();
    new auchindon_trigger_hallowed_ground();
    new auchindon_trigger_holy_shield();
    new auchindon_trigger_spawn_fissures();
    new auchindon_kaathar_areatrigger_fissure();
    new auchindon_spell_consecrated_light();
    new auchindon_spell_shield_spawn_trigger();
    new auchindon_spell_fate();
    new auchindon_spell_sanctified_ground_periodic_dummy();
    new auchindon_spell_conscreated_damage();
    new auchindon_mob_nyami();
}